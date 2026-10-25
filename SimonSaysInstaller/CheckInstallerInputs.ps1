# Pre-build check for SimonSaysInstaller (dwell.spec.md REQ-N11).
#
# The installer packages Release\SimonSays.exe (32-bit) and
# Release\SimonSaysGaze64.exe (the 64-bit Tobii Stream Engine helper,
# REQ-F96-F100). The helper is a separate x64 project, so a build that skipped
# it would silently ship a SimonSays that cannot read the Eye Tracker 5. This
# script fails the installer build, with a clear message, when either file is
# missing or has the wrong architecture.
#
# Usage (PreBuildEvent):
#   powershell -ExecutionPolicy Bypass -File "$(ProjectDir)CheckInstallerInputs.ps1"
#              -ReleaseDir "$(ProjectDir)..\Release"

[CmdletBinding()]
param (
    [Parameter(Mandatory = $true)]
    [string]$ReleaseDir
)

$ErrorActionPreference = 'Stop'

# PE machine type of an executable: 'x86', 'x64', 'arm64' or the raw value.
function Get-PeArchitecture([string]$Path) {
    $bytes = [System.IO.File]::ReadAllBytes($Path)
    $peOffset = [BitConverter]::ToInt32($bytes, 0x3C)
    $machine = [BitConverter]::ToUInt16($bytes, $peOffset + 4)
    switch ($machine) {
        0x014C  { 'x86' }
        0x8664  { 'x64' }
        0xAA64  { 'arm64' }
        default { '0x{0:X4}' -f $machine }
    }
}

$expected = [ordered]@{
    'SimonSays.exe'       = 'x86'
    'SimonSaysGaze64.exe' = 'x64'
}

$failed = $false
foreach ($name in $expected.Keys) {
    $path = Join-Path $ReleaseDir $name
    if (-not (Test-Path -LiteralPath $path)) {
        Write-Host "CheckInstallerInputs: ERROR: $path is missing. Build the solution's Release|Win32 configuration (it builds both SimonSays and the x64 SimonSaysGaze64 helper) before building the installer."
        $failed = $true
        continue
    }
    $arch = Get-PeArchitecture $path
    if ($arch -ne $expected[$name]) {
        Write-Host "CheckInstallerInputs: ERROR: $path is $arch, expected $($expected[$name])."
        $failed = $true
        continue
    }
    Write-Host "CheckInstallerInputs: OK $name ($arch, $((Get-Item -LiteralPath $path).LastWriteTime))"
}

if ($failed) { exit 1 }
exit 0
