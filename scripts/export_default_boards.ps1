<#
.SYNOPSIS
    Generates one .ssc board file per supported language from the built-in
    default phrase sets.

.DESCRIPTION
    Writes a SIMONSAYS_CATEGORIES_V1 file per language into the boards folder,
    carrying the language's default categories/phrases plus a $$board style
    layer with title, credits and a "language" metadata property.

    Everything except the translated words for "default phrases" is read from
    the source tree, so the output cannot drift from the app:

      include/default_phrases.h            18 languages x 11 categories
      include/stdafx.h SUPPORTED_LANGUAGES English <-> native language names
      resources/SimonSays.rc FILEVERSION   the version used in the title

    Output format (see docs/specs/import-export.spec.md PORT-F20 and
    SerializeCategoriesToUtf8 in src/utils.cpp): UTF-8 with BOM, LF line
    endings, the V1 header, an optional $$board line, then one line per
    category as "<icon>##<name>=<phrase|phrase|...>".

    The "language" style property is not a property the app recognises; it is
    preserved verbatim through registry/.ssc/.ssz round-trips per
    docs/specs/board-style.spec.md STY-F03, which is exactly what makes it
    usable as durable metadata.

.PARAMETER OutDir
    Target folder. Defaults to the repository's boards folder.

.PARAMETER Language
    One or more English language names to generate. Defaults to all.

.EXAMPLE
    .\scripts\export_default_boards.ps1

.EXAMPLE
    .\scripts\export_default_boards.ps1 -Language Spanish, English -WhatIf

.NOTES
    This file must keep its UTF-8 BOM. It embeds non-ASCII text (the title
    wording below), and Windows PowerShell 5.1 reads a BOM-less .ps1 as the
    system ANSI codepage, which mangles that text and breaks parsing. The
    other scripts here are pure ASCII, which is why they need no BOM.
#>
[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [string]$OutDir,
    [string[]]$Language
)

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not $OutDir) { $OutDir = Join-Path $repoRoot 'boards' }

$phrasesPath = Join-Path $repoRoot 'include/default_phrases.h'
$stdafxPath = Join-Path $repoRoot 'include/stdafx.h'
$rcPath = Join-Path $repoRoot 'resources/SimonSays.rc'

foreach ($p in @($phrasesPath, $stdafxPath, $rcPath)) {
    if (-not (Test-Path $p)) { throw "Required source file not found: $p" }
}

# The only hand-maintained text in this script: the title wording per language.
# Everything else is derived from the sources above. Keyed by English name, so
# a new language added to SUPPORTED_LANGUAGES fails loudly below until its
# wording is added here.
$defaultPhrasesWording = [ordered]@{
    'Arabic'               = 'العبارات الافتراضية'
    'Basque'               = 'Lehenetsitako esaldiak'
    'Catalan'              = 'Frases predeterminades'
    'Chinese (Simplified)' = '默认短语'
    'English'              = 'default phrases'
    'French'               = 'phrases par défaut'
    'Galician'             = 'Frases predeterminadas'
    'German'               = 'Standardsätze'
    'Hebrew'               = 'ביטויי ברירת מחדל'
    'Hindi'                = 'डिफ़ॉल्ट वाक्य'
    'Italian'              = 'frasi predefinite'
    'Japanese'             = '既定のフレーズ'
    'Korean'               = '기본 문구'
    'Portuguese'           = 'frases predefinidas'
    'Portuguese (Brazil)'  = 'frases padrão'
    'Russian'              = 'фразы по умолчанию'
    'Spanish'              = 'frases predeterminadas'
    'Valencian'            = 'Frases predeterminades'
}

function Read-SourceLines {
    param([string]$Path)
    # ReadAllText without an explicit encoding honours the UTF-8 BOM these
    # headers carry (see categories-phrases.spec.md 8.4, authoring note).
    $text = [System.IO.File]::ReadAllText($Path)
    return ($text -split "`r?`n")
}

function Get-AppVersion {
    param([string]$Path)
    foreach ($line in (Read-SourceLines $Path)) {
        if ($line -match '^\s*FILEVERSION\s+(\d+)\s*,\s*(\d+)') {
            return "$($Matches[1]).$($Matches[2])"
        }
    }
    throw "FILEVERSION not found in $Path"
}

function Get-NativeLanguageNames {
    param([string]$Path)
    $names = @{}
    $inTable = $false
    foreach ($line in (Read-SourceLines $Path)) {
        if ($line -match 'SUPPORTED_LANGUAGES\s*=\s*\{') { $inTable = $true; continue }
        if (-not $inTable) { continue }
        if ($line -match '^\s*\};') { break }
        if ($line -match '^\s*\{\s*L"([^"]+)"\s*,\s*L"([^"]+)"\s*,') {
            $names[$Matches[1]] = $Matches[2]
        }
    }
    if ($names.Count -eq 0) { throw "SUPPORTED_LANGUAGES table not parsed from $Path" }
    return $names
}

function Get-DefaultPhraseSets {
    param([string]$Path)
    # Ordered: language order, and category order within a language, is the
    # on-screen order (categories-phrases.spec.md 8.4 / CAT-N04).
    $sets = [ordered]@{}
    $current = $null
    foreach ($rawLine in (Read-SourceLines $Path)) {
        $line = $rawLine -replace '\s*\\\s*$', ''
        if ($line -match '^\s*\{\s*L"([^"#]+)"\s*,\s*\{\s*$') {
            $current = $Matches[1]
            $sets[$current] = New-Object System.Collections.ArrayList
            continue
        }
        if ($null -eq $current) { continue }
        if ($line -match '^\s*\{\s*L"([^"]*)"\s*,\s*L"(.*)"\s*\},?\s*$') {
            $null = $sets[$current].Add([pscustomobject]@{
                Label = $Matches[1]
                Data  = $Matches[2]
            })
        }
    }
    if ($sets.Count -eq 0) { throw "No language blocks parsed from $Path" }
    return $sets
}

function ConvertTo-BoardFileName {
    param([string]$Title)
    # The file name is the title with spaces as underscores, in the board's own
    # language: a reader of that language can recognise the file on sight.
    # Non-ASCII names are fine here because these boards are generated on
    # demand and handed to users, not committed - so the cross-platform Git
    # normalisation issue (macOS NFD vs Windows NFC) never arises.
    $name = $Title -replace ' ', '_'

    # Defensive only: no current title contains these, but a future translation
    # must never be able to produce an unusable path.
    $illegal = [System.IO.Path]::GetInvalidFileNameChars()
    foreach ($c in $illegal) { $name = $name.Replace([string]$c, '') }

    return "$name.ssc"
}

$version = Get-AppVersion $rcPath
$nativeNames = Get-NativeLanguageNames $stdafxPath
$phraseSets = Get-DefaultPhraseSets $phrasesPath

if (-not (Test-Path $OutDir)) {
    $null = New-Item -ItemType Directory -Path $OutDir -Force
}

$targets = @($phraseSets.Keys)
if ($Language) {
    $unknown = @($Language | Where-Object { $targets -notcontains $_ })
    if ($unknown) { throw "Unknown language(s): $($unknown -join ', ')" }
    $targets = @($Language)
}

# UTF-8 *with* BOM: the .ssc reader strips an optional BOM and external tools
# use it to auto-detect the encoding (PORT-F20).
$utf8Bom = New-Object System.Text.UTF8Encoding($true)
$written = 0

foreach ($englishName in $targets) {
    if (-not $nativeNames.ContainsKey($englishName)) {
        throw "'$englishName' is in default_phrases.h but not in SUPPORTED_LANGUAGES."
    }
    if (-not $defaultPhrasesWording.Contains($englishName)) {
        throw "No title wording for '$englishName'. Add it to `$defaultPhrasesWording in this script."
    }

    $nativeName = $nativeNames[$englishName]
    $wording = $defaultPhrasesWording[$englishName]
    $title = "SimonSays $version $wording ($nativeName)"

    foreach ($field in @($title, $englishName)) {
        if ($field -match '[;|=]') {
            throw "'$field' contains a reserved character (; | =) and would corrupt the board style line."
        }
    }

    $sb = New-Object System.Text.StringBuilder
    $null = $sb.Append("SIMONSAYS_CATEGORIES_V1`n")
    $style = 'language:' + $englishName + ';title:' + $title + ';credits:SimonSays;'
    $null = $sb.Append('$$board=$$' + $style + "`n")
    foreach ($category in $phraseSets[$englishName]) {
        $null = $sb.Append($category.Label + '=' + $category.Data + "`n")
    }

    $fileName = ConvertTo-BoardFileName -Title $title
    $outPath = Join-Path $OutDir $fileName

    if ($PSCmdlet.ShouldProcess($outPath, 'Write board file')) {
        [System.IO.File]::WriteAllText($outPath, $sb.ToString(), $utf8Bom)
        $written++
    }
    Write-Host ("{0,-22} {1,2} categories -> {2}" -f $englishName, $phraseSets[$englishName].Count, $fileName)
}

Write-Host ""
Write-Host "$written board file(s) written to $OutDir"
