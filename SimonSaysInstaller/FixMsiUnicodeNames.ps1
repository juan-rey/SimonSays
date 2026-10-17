# Post-build fix for SimonSaysInstaller.msi.
#
# Visual Studio Installer Projects writes the MSI tables in the .vdproj code
# page (1252) and silently drops every character that code page cannot hold,
# so "boards\日本語\SimonSays_ゲーム_(日本語).ssc" ends up as
# "boards\~1|\SimonSays__().ssc". This script switches the MSI database to
# UTF-8 (code page 65001) and rewrites the Directory.DefaultDir and
# File.FileName values from the .vdproj, matching rows by key: the MSI File
# key is the .vdproj file entry key and the MSI Directory key is the folder's
# "Property" value, so no guessing is needed.
#
# Usage (PostBuildEvent):
#   powershell -ExecutionPolicy Bypass -File "$(ProjectDir)FixMsiUnicodeNames.ps1"
#              -MsiPath "$(BuiltOuputPath)" -VdprojPath "$(ProjectDir)SimonSaysInstaller.vdproj"

[CmdletBinding()]
param (
    [Parameter(Mandatory = $true)]
    [string]$MsiPath,

    [Parameter(Mandatory = $true)]
    [string]$VdprojPath
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $MsiPath)) { throw "MSI file not found: $MsiPath" }
if (-not (Test-Path -LiteralPath $VdprojPath)) { throw ".vdproj file not found: $VdprojPath" }
$MsiPath = (Resolve-Path -LiteralPath $MsiPath).Path

# --- Windows Installer COM helpers (the automation objects have no type
# library PowerShell can bind to, so every call goes through InvokeMember;
# arguments are unwrapped from PSObject or COM rejects them as a type mismatch).
function ConvertTo-ComArguments([object[]]$Arguments) {
    $Plain = New-Object 'object[]' $Arguments.Count
    for ($i = 0; $i -lt $Arguments.Count; $i++) {
        $Plain[$i] = if ($Arguments[$i] -is [psobject]) { $Arguments[$i].psobject.BaseObject } else { $Arguments[$i] }
    }
    , $Plain
}
function Invoke-Com($Object, [string]$Name, [object[]]$Arguments = @()) {
    $Object.GetType().InvokeMember($Name, [Reflection.BindingFlags]::InvokeMethod, $null, $Object, (ConvertTo-ComArguments $Arguments))
}
function Get-ComProperty($Object, [string]$Name, [object[]]$Arguments = @()) {
    $Object.GetType().InvokeMember($Name, [Reflection.BindingFlags]::GetProperty, $null, $Object, (ConvertTo-ComArguments $Arguments))
}
function Set-ComProperty($Object, [string]$Name, [object[]]$Arguments) {
    [void]$Object.GetType().InvokeMember($Name, [Reflection.BindingFlags]::SetProperty, $null, $Object, (ConvertTo-ComArguments $Arguments))
}

function Get-MsiRows($Database, [string]$Query, [int]$Columns) {
    $View = Invoke-Com $Database 'OpenView' @($Query)
    [void](Invoke-Com $View 'Execute')
    $Rows = @()
    while ($null -ne ($Record = Invoke-Com $View 'Fetch')) {
        $Rows += , @(1..$Columns | ForEach-Object { [string](Get-ComProperty $Record 'StringData' @([int]$_)) })
    }
    [void](Invoke-Com $View 'Close')
    , $Rows
}

# --- Read the intended names from the .vdproj.
function ConvertFrom-VdprojString([string]$Value) { $Value.Replace('\\', '\').Replace('\"', '"') }

$FileNames = @{}       # MSI File key      -> long file name
$DirectoryNames = @{}  # MSI Directory key -> long folder name
$CurrentFileKey = $null
$LastFolderName = $null
foreach ($Line in [IO.File]::ReadAllLines($VdprojPath, [Text.Encoding]::UTF8)) {
    if ($Line -match '^\s*"\{1FB2D0AE-D3B9-43D4-B9DD-F88EC61E35DE\}:(_[0-9A-F]{32})"\s*$') {
        $CurrentFileKey = $Matches[1]
    } elseif ($CurrentFileKey -and $Line -match '^\s*"TargetName" = "8:(.*)"\s*$') {
        $FileNames[$CurrentFileKey] = ConvertFrom-VdprojString $Matches[1]
        $CurrentFileKey = $null
    } elseif ($Line -match '^\s*"Name" = "8:(.*)"\s*$') {
        $LastFolderName = ConvertFrom-VdprojString $Matches[1]
    } elseif ($LastFolderName -and $Line -match '^\s*"Property" = "8:(_[0-9A-F]{32})"\s*$') {
        $DirectoryNames[$Matches[1]] = $LastFolderName
        $LastFolderName = $null
    }
}
Write-Host "FixMsiUnicodeNames: $($FileNames.Count) files and $($DirectoryNames.Count) folders in $(Split-Path -Leaf $VdprojPath)"

# --- Short (8.3) names. Windows Installer wants "short|long" when the long
# name is not a valid short name; the short part is derived from the unique
# row key so two entries in one folder can never collide.
$ShortNamePattern = '^[A-Z0-9_\-!#$%&''()@^{}~]{1,8}(\.[A-Z0-9_\-!#$%&''()@^{}~]{1,3})?$'

function Get-NameField([string]$Key, [string]$LongName, [string]$CurrentValue, [string]$Prefix, [bool]$KeepExtension) {
    if ($LongName -cmatch $ShortNamePattern) { return $LongName }
    $CurrentShort = if ($CurrentValue -like '*|*') { $CurrentValue.Split('|')[0] } else { '' }
    if ($CurrentShort -match $ShortNamePattern -and -not $CurrentShort.StartsWith('~')) {
        $Short = $CurrentShort
    } else {
        $Short = $Prefix + $Key.TrimStart('_').Substring(0, 7)
        $Extension = [IO.Path]::GetExtension($LongName).TrimStart('.').ToUpperInvariant()
        if ($KeepExtension -and $Extension -match '^[A-Z0-9]{1,3}') { $Short += '.' + $Matches[0] }
    }
    "$Short|$LongName"
}

function Get-LongName([string]$Value) {
    $Value = $Value.Split(':')[0]  # DefaultDir may carry "target:source"
    if ($Value -like '*|*') { $Value.Split('|')[1] } else { $Value }
}

# --- Patch the MSI.
$Installer = New-Object -ComObject WindowsInstaller.Installer
$Database = Invoke-Com $Installer 'OpenDatabase' @($MsiPath, 1)  # msiOpenDatabaseModeTransact

# Importing _ForceCodepage re-encodes the existing string pool, so the Latin
# names VS already wrote correctly (Français, Català...) are kept.
$IdtDirectory = Join-Path ([IO.Path]::GetTempPath()) ("FixMsiUnicodeNames_" + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $IdtDirectory | Out-Null
try {
    [IO.File]::WriteAllText((Join-Path $IdtDirectory 'codepage.idt'), "`r`n`r`n65001`t_ForceCodepage`r`n", [Text.Encoding]::ASCII)
    [void](Invoke-Com $Database 'Import' @($IdtDirectory, 'codepage.idt'))
} finally {
    Remove-Item -LiteralPath $IdtDirectory -Recurse -Force
}

function Update-NameColumn([string]$Table, [string]$KeyColumn, [string]$NameColumn, [hashtable]$Names, [string]$Prefix, [bool]$KeepExtension) {
    $Rows = Get-MsiRows $Database "SELECT ``$KeyColumn``, ``$NameColumn`` FROM ``$Table``" 2
    $Update = Invoke-Com $Database 'OpenView' @("UPDATE ``$Table`` SET ``$NameColumn`` = ? WHERE ``$KeyColumn`` = ?")
    $Changed = 0
    $Seen = 0
    foreach ($Row in $Rows) {
        $Key, $Current = $Row
        if (-not $Names.ContainsKey($Key)) { continue }
        $Seen++
        if ((Get-LongName $Current) -ceq $Names[$Key]) { continue }
        $NewValue = Get-NameField $Key $Names[$Key] $Current $Prefix $KeepExtension
        $Record = Invoke-Com $Installer 'CreateRecord' @(2)
        Set-ComProperty $Record 'StringData' @(1, $NewValue)
        Set-ComProperty $Record 'StringData' @(2, $Key)
        [void](Invoke-Com $Update 'Execute' @($Record))
        $Changed++
    }
    [void](Invoke-Com $Update 'Close')
    if ($Seen -ne $Names.Count) {
        Write-Warning "$Table`: $($Names.Count - $Seen) .vdproj entries have no row in the MSI"
    }
    Write-Host "FixMsiUnicodeNames: $Table - $Changed of $Seen names rewritten"
}

Update-NameColumn 'Directory' 'Directory' 'DefaultDir' $DirectoryNames 'D' $false
Update-NameColumn 'File' 'File' 'FileName' $FileNames 'F' $true
[void](Invoke-Com $Database 'Commit')

# --- Verify: every name must now read back exactly as in the .vdproj.
$Errors = 0
foreach ($Check in @(@('Directory', 'Directory', 'DefaultDir', $DirectoryNames), @('File', 'File', 'FileName', $FileNames))) {
    $Table, $KeyColumn, $NameColumn, $Names = $Check
    foreach ($Row in (Get-MsiRows $Database "SELECT ``$KeyColumn``, ``$NameColumn`` FROM ``$Table``" 2)) {
        if ($Names.ContainsKey($Row[0]) -and (Get-LongName $Row[1]) -cne $Names[$Row[0]]) {
            Write-Warning "$Table $($Row[0]) is '$($Row[1])', expected '$($Names[$Row[0]])'"
            $Errors++
        }
    }
}
[void][Runtime.InteropServices.Marshal]::ReleaseComObject($Database)
if ($Errors -gt 0) { throw "FixMsiUnicodeNames: $Errors names are still wrong in $MsiPath" }
Write-Host "FixMsiUnicodeNames: $MsiPath is now UTF-8 with all names restored"
