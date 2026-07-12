<#
.SYNOPSIS
  Apply a TSV of UI-string translations into include/localized_strings.h.

.DESCRIPTION
  Reads a TSV whose rows are  LANG<TAB>ID<TAB>VALUE  (lines starting with '#'
  and blank lines are ignored). For each row it edits the target language table
  (<LANG>_LOCALIZED_UI_STRINGS):
    * if ID already exists in that table  -> replaces its value in place;
    * otherwise                           -> inserts a new entry just before the
                                             table's closing "};".
  The generated HELP_CONTENT_ID raw string is detected and never touched. The
  file's UTF-8 BOM and CRLF line endings are preserved.

  VALUE is the raw C++ string-literal body (what goes between L" and "); keep
  escape sequences such as \n and \0 literal in the TSV.

.PARAMETER Tsv
  Path to the TSV file (required).

.PARAMETER MirrorValencianFromCatalan
  When $true (default), every CATALAN row is also applied to VALENCIAN unless
  the TSV already specifies that ID for VALENCIAN. Valencian mirrors Catalan in
  this project. Pass -MirrorValencianFromCatalan:$false to disable.

.PARAMETER Header
  Path to localized_strings.h (default: ..\include\localized_strings.h relative
  to this script).

.EXAMPLE
  powershell -File scripts/apply_translations.ps1 -Tsv pending.tsv
#>
param(
  [Parameter(Mandatory = $true)][string]$Tsv,
  [bool]$MirrorValencianFromCatalan = $true,
  [string]$Header
)
$ErrorActionPreference = 'Stop'
if (-not $Header) { $Header = Join-Path $PSScriptRoot '..\include\localized_strings.h' }

# --- load translations: section -> ordered dict(id -> value) ----------------
$data = @{}
foreach ($line in [System.IO.File]::ReadAllLines($Tsv, [System.Text.Encoding]::UTF8)) {
  if ($line -match '^\s*#' -or $line.Trim() -eq '') { continue }
  $parts = $line -split "`t", 3
  if ($parts.Count -lt 3) { Write-Warning "Skipping malformed row: $line"; continue }
  $lang = $parts[0].Trim(); $id = $parts[1].Trim(); $val = $parts[2]
  if (-not $data.ContainsKey($lang)) { $data[$lang] = [ordered]@{} }
  $data[$lang][$id] = $val
}
if ($MirrorValencianFromCatalan -and $data.ContainsKey('CATALAN')) {
  if (-not $data.ContainsKey('VALENCIAN')) { $data['VALENCIAN'] = [ordered]@{} }
  foreach ($k in $data['CATALAN'].Keys) {
    if (-not $data['VALENCIAN'].Contains($k)) { $data['VALENCIAN'][$k] = $data['CATALAN'][$k] }
  }
}
if ($data.Count -eq 0) { throw "No translation rows found in $Tsv" }

# --- rewrite header, single pass, preserving CRLF + BOM ---------------------
$raw   = [System.IO.File]::ReadAllText($Header, [System.Text.Encoding]::UTF8)
$lines = [regex]::Split($raw, "`r`n")
$secRe = [regex]'\b([A-Z_]+)_LOCALIZED_UI_STRINGS\s*='

$out    = New-Object System.Collections.Generic.List[string]
$cur    = $null    # current target section name, or $null
$done   = $null    # ids already replaced in place for $cur
$inRaw  = $false   # inside LR"HELP( ... )HELP"
$report = @{}

foreach ($ln in $lines) {
  if ($inRaw) { $out.Add($ln); if ($ln -match '\)HELP"') { $inRaw = $false }; continue }
  if ($ln -match 'LR"HELP\(') { $out.Add($ln); if ($ln -notmatch '\)HELP"') { $inRaw = $true }; continue }

  $ms = $secRe.Match($ln)
  if ($ms.Success) {
    $name = $ms.Groups[1].Value
    if ($data.ContainsKey($name)) { $cur = $name; $done = New-Object System.Collections.Generic.HashSet[string] }
    else { $cur = $null }
    $out.Add($ln); continue
  }

  if ($null -ne $cur) {
    if ($ln.Trim() -eq '};') {
      $ins = 0
      foreach ($id in $data[$cur].Keys) {
        if (-not $done.Contains($id)) {
          $out.Add('  { ' + $id + ', L"' + $data[$cur][$id] + '" },')
          $ins++
        }
      }
      $report[$cur] = "replaced=$($done.Count) inserted=$ins"
      $out.Add($ln); $cur = $null; $done = $null; continue
    }
    $mid = [regex]::Match($ln, '^\s*\{\s*([A-Z0-9_]+_ID)\s*,')
    if ($mid.Success -and $data[$cur].Contains($mid.Groups[1].Value)) {
      $id = $mid.Groups[1].Value
      $out.Add('  { ' + $id + ', L"' + $data[$cur][$id] + '" },')
      [void]$done.Add($id)
      continue
    }
  }
  $out.Add($ln)
}

$result = [string]::Join("`r`n", $out)
[System.IO.File]::WriteAllText($Header, $result, (New-Object System.Text.UTF8Encoding($true)))  # UTF-8 WITH BOM

"Applied to $Header :"
foreach ($k in ($data.Keys | Sort-Object)) {
  $r = if ($report.ContainsKey($k)) { $report[$k] } else { 'SECTION NOT FOUND' }
  "  {0,-14} {1}" -f $k, $r
}
