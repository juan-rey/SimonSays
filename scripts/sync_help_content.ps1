$ErrorActionPreference = 'Stop'

$localizedPath = 'include/localized_strings.h'
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$text = [System.IO.File]::ReadAllText($localizedPath, $utf8NoBom)

function Set-HelpContent {
    param(
        [string]$VectorName,
        [string]$HelpPath
    )

    if (-not (Test-Path $HelpPath)) {
        throw "Help file not found: $HelpPath"
    }

    $content = [System.IO.File]::ReadAllText($HelpPath, $utf8NoBom)
    $content = $content -replace "`r`n", "`n"

    $entry = "  { HELP_CONTENT_ID, LR`"HELP($($content.Substring(0, $content.Length / 2)))HELP`" LR`"HELP($($content.Substring($content.Length / 2)))HELP`" }"

    $pattern = "(?s)(static const std::vector<std::pair<int, const wchar_t \*>>\s+$([regex]::Escape($VectorName))\s*=\s*\{)(.*?)(\r?\n\};)"
    $m = [regex]::Match($text, $pattern)
    if (-not $m.Success) {
        throw "Vector block not found: $VectorName"
    }

    $prefix = $m.Groups[1].Value
    $body = $m.Groups[2].Value
    $suffix = $m.Groups[3].Value

    $body = [regex]::Replace($body, "(?s)\r?\n\s*\{\s*HELP_CONTENT_ID\s*,.*?\}\s*,?", "")

    $shortcut = [regex]::Match($body, "\{\s*CATEGORY_SHORTCUTS_TEXT_ID\s*,[^\r\n]*\}")
    if (-not $shortcut.Success) {
        throw "CATEGORY_SHORTCUTS_TEXT_ID not found in vector: $VectorName"
    }

    $insertAt = $shortcut.Index + $shortcut.Length
    $body = $body.Insert($insertAt, ",`n$entry")

    $newBlock = "$prefix$body$suffix"
    $script:text = $script:text.Substring(0, $m.Index) + $newBlock + $script:text.Substring($m.Index + $m.Length)
}

$map = @(
    @{ Vector = 'DEFAULT_LOCALIZED_UI_STRINGS'; File = 'HELP.md' },
    @{ Vector = 'SPANISH_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_es.md' },
    @{ Vector = 'ARABIC_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_ar.md' },
    @{ Vector = 'BASQUE_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_eu.md' },
    @{ Vector = 'CATALAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_ca.md' },
    @{ Vector = 'CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_zh_CN.md' },
    @{ Vector = 'FRENCH_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_fr.md' },
    @{ Vector = 'GALICIAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_gl.md' },
    @{ Vector = 'GERMAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_de.md' },
    @{ Vector = 'HEBREW_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_he.md' },
    @{ Vector = 'HINDI_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_hi.md' },
    @{ Vector = 'ITALIAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_it.md' },
    @{ Vector = 'JAPANESE_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_ja.md' },
    @{ Vector = 'KOREAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_ko.md' },
    @{ Vector = 'PORTUGUESE_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_pt.md' },
    @{ Vector = 'RUSSIAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_ru.md' },
    @{ Vector = 'VALENCIAN_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_val.md' }
)

foreach ($item in $map) {
    Set-HelpContent -VectorName $item.Vector -HelpPath $item.File
}

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
[System.IO.File]::WriteAllText($localizedPath, $text, $utf8NoBom)
Write-Output 'HELP_CONTENT_ID synchronized for all languages.'
