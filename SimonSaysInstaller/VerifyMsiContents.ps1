# Post-build check for SimonSaysInstaller.msi (dwell.spec.md REQ-N11).
#
# Runs after FixMsiUnicodeNames.ps1. Opens the finished MSI read-only and fails
# the build unless SimonSays.exe and SimonSaysGaze64.exe are both in its File
# table and install to the same directory (SimonSays looks for the helper next
# to itself).
#
# Usage (PostBuildEvent, after FixMsiUnicodeNames.ps1):
#   powershell -ExecutionPolicy Bypass -File "$(ProjectDir)VerifyMsiContents.ps1"
#              -MsiPath "$(BuiltOuputPath)"

[CmdletBinding()]
param (
    [Parameter(Mandatory = $true)]
    [string]$MsiPath
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $MsiPath)) {
    Write-Host "VerifyMsiContents: ERROR: $MsiPath not found."
    exit 1
}

$installer = New-Object -ComObject WindowsInstaller.Installer
$db = $installer.GetType().InvokeMember('OpenDatabase', 'InvokeMethod', $null, $installer, @($MsiPath, 0)) # 0 = read-only

# File.FileName is "SHORT~1.EXE|LongName.exe" (or just the name when it is
# already a valid short name); keep the long part.
$query = 'SELECT `File`.`FileName`, `Component`.`Directory_` FROM `File`, `Component` WHERE `File`.`Component_` = `Component`.`Component`'
$view = $db.GetType().InvokeMember('OpenView', 'InvokeMethod', $null, $db, @($query))
$view.GetType().InvokeMember('Execute', 'InvokeMethod', $null, $view, $null) | Out-Null

$dirOf = @{}
while ($true) {
    $record = $view.GetType().InvokeMember('Fetch', 'InvokeMethod', $null, $view, $null)
    if ($null -eq $record) { break }
    $fileName = $record.GetType().InvokeMember('StringData', 'GetProperty', $null, $record, @(1))
    $directory = $record.GetType().InvokeMember('StringData', 'GetProperty', $null, $record, @(2))
    $long = ($fileName -split '\|')[-1]
    $dirOf[$long.ToLowerInvariant()] = $directory
}
$view.GetType().InvokeMember('Close', 'InvokeMethod', $null, $view, $null) | Out-Null
[System.Runtime.InteropServices.Marshal]::ReleaseComObject($view) | Out-Null
[System.Runtime.InteropServices.Marshal]::ReleaseComObject($db) | Out-Null

$failed = $false
foreach ($name in 'simonsays.exe', 'simonsaysgaze64.exe') {
    if (-not $dirOf.ContainsKey($name)) {
        Write-Host "VerifyMsiContents: ERROR: $name is not in $MsiPath."
        $failed = $true
    } else {
        Write-Host "VerifyMsiContents: OK $name -> directory $($dirOf[$name])"
    }
}
if (-not $failed -and $dirOf['simonsays.exe'] -ne $dirOf['simonsaysgaze64.exe']) {
    Write-Host "VerifyMsiContents: ERROR: SimonSaysGaze64.exe installs to $($dirOf['simonsaysgaze64.exe']), not next to SimonSays.exe ($($dirOf['simonsays.exe']))."
    $failed = $true
}

if ($failed) { exit 1 }
Write-Host "VerifyMsiContents: OK ($($dirOf.Count) files in the MSI)."
exit 0
