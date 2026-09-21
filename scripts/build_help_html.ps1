<#
.SYNOPSIS
    Builds one HTML help page per language from the SimonSays help Markdown.

.DESCRIPTION
    Converts HELP.md (English) and docs/help/HELP_<lang>.md into
    SimonSays_<Help>_(<native language name>).html pages - both parts in the
    page's own language, e.g. SimonSays_Ayuda_(Espanol).html - that link a
    shared stylesheet and carry a language switcher.

    These pages ARE the app's help: F1 shell-opens the one for the current
    language from <exe dir>\help\, so they are shipped, not throwaway. To keep
    the name the app opens identical to the file written here, this script also
    writes HELP_CONTENT_FILE_NAME_ID into every language table in
    include/localized_strings.h (skip with -NoStringTable). That replaces the
    retired sync_help_content.ps1, which used to compile the help text itself
    into HELP_CONTENT_ID - do not run that script any more.

    Nothing language-specific is hardcoded here, so the script stays pure
    ASCII and cannot drift from the app:
      native names, RTL flags   SUPPORTED_LANGUAGES in include/stdafx.h
      the word "Help"           CATEGORY_SHORTCUTS_TEXT_ID ("F1: Help - F2: ...")
                                in include/localized_strings.h

    The Markdown converter is deliberately small and dependency-free (this
    machine has no pandoc, no PowerShell 7 ConvertFrom-Markdown, and no
    Python/Node Markdown library). It supports exactly what the help files
    use: ATX headings, paragraphs, bold, inline code, fenced code blocks,
    blockquotes, nested bullet/numbered lists (nesting by relative
    indentation), links, backslash escapes and trailing-double-space line
    breaks. Anything else - tables, images, HTML tags or comments, setext
    headings, unclosed fences - stops the script with file and line, rather
    than being silently misrendered. Placeholders such as <icon> are text and
    are escaped, not treated as HTML.

    Heading ids follow GitHub's slug rules, so in-document links like
    #customizing-the-look-board--category-styles keep working.

.PARAMETER CssPath
    Stylesheet to link (or embed). Defaults to help.css in the output folder.

.PARAMETER OutDir
    Output folder. Defaults to docs/help.

.PARAMETER NoStringTable
    Do not touch include/localized_strings.h. Use when building a throwaway
    preview (for example with -OutDir C:\temp\help) rather than the pages that
    ship with the app.

.PARAMETER InlineCss
    Embed the stylesheet in each page instead of linking it, so every page is
    a single self-contained file.

.EXAMPLE
    .\scripts\build_help_html.ps1

.EXAMPLE
    .\scripts\build_help_html.ps1 -InlineCss -OutDir C:\temp\help
#>
[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [string]$CssPath,
    [string]$OutDir,
    [switch]$InlineCss,
    [switch]$NoStringTable
)

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not $OutDir) { $OutDir = Join-Path $repoRoot 'docs/help' }
if (-not $CssPath) { $CssPath = Join-Path $OutDir 'help.css' }
$stdafxPath = Join-Path $repoRoot 'include/stdafx.h'
$stringsPath = Join-Path $repoRoot 'include/localized_strings.h'

# Help sources, their BCP 47 tags, and their UI string tables (the same table
# names scripts/sync_help_content.ps1 uses). English names must match
# SUPPORTED_LANGUAGES.
$sources = @(
    @{ English = 'English';              Lang = 'en';             Table = 'DEFAULT_LOCALIZED_UI_STRINGS';            File = 'HELP.md' }
    @{ English = 'Arabic';               Lang = 'ar';             Table = 'ARABIC_LOCALIZED_UI_STRINGS';             File = 'docs/help/HELP_ar.md' }
    @{ English = 'Basque';               Lang = 'eu';             Table = 'BASQUE_LOCALIZED_UI_STRINGS';             File = 'docs/help/HELP_eu.md' }
    @{ English = 'Catalan';              Lang = 'ca';             Table = 'CATALAN_LOCALIZED_UI_STRINGS';            File = 'docs/help/HELP_ca.md' }
    @{ English = 'Chinese (Simplified)'; Lang = 'zh-CN';          Table = 'CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS'; File = 'docs/help/HELP_zh_CN.md' }
    @{ English = 'French';               Lang = 'fr';             Table = 'FRENCH_LOCALIZED_UI_STRINGS';             File = 'docs/help/HELP_fr.md' }
    @{ English = 'Galician';             Lang = 'gl';             Table = 'GALICIAN_LOCALIZED_UI_STRINGS';           File = 'docs/help/HELP_gl.md' }
    @{ English = 'German';               Lang = 'de';             Table = 'GERMAN_LOCALIZED_UI_STRINGS';             File = 'docs/help/HELP_de.md' }
    @{ English = 'Hebrew';               Lang = 'he';             Table = 'HEBREW_LOCALIZED_UI_STRINGS';             File = 'docs/help/HELP_he.md' }
    @{ English = 'Hindi';                Lang = 'hi';             Table = 'HINDI_LOCALIZED_UI_STRINGS';              File = 'docs/help/HELP_hi.md' }
    @{ English = 'Italian';              Lang = 'it';             Table = 'ITALIAN_LOCALIZED_UI_STRINGS';            File = 'docs/help/HELP_it.md' }
    @{ English = 'Japanese';             Lang = 'ja';             Table = 'JAPANESE_LOCALIZED_UI_STRINGS';           File = 'docs/help/HELP_ja.md' }
    @{ English = 'Korean';               Lang = 'ko';             Table = 'KOREAN_LOCALIZED_UI_STRINGS';             File = 'docs/help/HELP_ko.md' }
    @{ English = 'Portuguese';           Lang = 'pt-PT';          Table = 'PORTUGUESE_LOCALIZED_UI_STRINGS';         File = 'docs/help/HELP_pt.md' }
    @{ English = 'Portuguese (Brazil)';  Lang = 'pt-BR';          Table = 'PORTUGUESE_BRAZIL_LOCALIZED_UI_STRINGS';  File = 'docs/help/HELP_pt_BR.md' }
    @{ English = 'Russian';              Lang = 'ru';             Table = 'RUSSIAN_LOCALIZED_UI_STRINGS';            File = 'docs/help/HELP_ru.md' }
    @{ English = 'Spanish';              Lang = 'es';             Table = 'SPANISH_LOCALIZED_UI_STRINGS';            File = 'docs/help/HELP_es.md' }
    @{ English = 'Valencian';            Lang = 'ca-ES-valencia'; Table = 'VALENCIAN_LOCALIZED_UI_STRINGS';          File = 'docs/help/HELP_val.md' }
)

# Sentinels for protected inline fragments; private-use code points built at
# runtime so this script stays pure ASCII.
$script:TokenMark = [string][char]0xE000
$script:BreakMark = [string][char]0xE001

$reFence        = '^\s{0,3}(`{3,}|~{3,})\s*([^`\s]*)\s*$'
$reHeading      = '^\s{0,3}(#{1,6})(?:[ \t]+(.*?))?(?:[ \t]+#+)?[ \t]*$'
$reHeadingStart = '^\s{0,3}#{1,6}([ \t]|$)'
$reHr           = '^\s{0,3}((\*[ \t]*){3,}|(-[ \t]*){3,}|(_[ \t]*){3,})$'
$reQuote        = '^\s{0,3}>[ ]?(.*)$'
$reTable        = '^\s*\|.*\|\s*$'
$reItem         = '^( *)([-*+]|\d{1,9}[.)])(?:[ \t]+(.*))?$'
$reCodeSpan     = '(?<!`)(`+)(?!`)(.+?)(?<!`)\1(?!`)'
$reHtmlTag      = '</?(a|b|i|u|s|em|strong|br|hr|img|div|span|p|table|thead|tbody|tr|td|th|ul|ol|li|kbd|sup|sub|small|details|summary|code|pre|h[1-6])\b[^>]*>'

function Read-TextLines {
    param([string]$Path)
    # ReadAllText honours a UTF-8 BOM (the translated help files carry one).
    $text = [System.IO.File]::ReadAllText($Path).Replace("`t", '    ')
    return , ($text -split '\r?\n')
}

function ConvertTo-HtmlText {
    param([string]$Text)
    # Only the characters that matter; everything else stays literal UTF-8.
    return $Text.Replace('&', '&amp;').Replace('<', '&lt;').Replace('>', '&gt;').Replace('"', '&quot;')
}

function Get-SupportedLanguages {
    param([string]$Path)
    $list = New-Object System.Collections.ArrayList
    $inTable = $false
    foreach ($line in (Read-TextLines $Path)) {
        if ($line -match 'SUPPORTED_LANGUAGES\s*=\s*\{') { $inTable = $true; continue }
        if (-not $inTable) { continue }
        if ($line -match '^\s*\};') { break }
        $m = [regex]::Match($line, '^\s*\{\s*L"([^"]+)"\s*,\s*L"([^"]+)"\s*,\s*(true|false)\s*,')
        if ($m.Success) {
            $null = $list.Add([pscustomobject]@{
                English = $m.Groups[1].Value
                Native  = $m.Groups[2].Value
                Rtl     = ($m.Groups[3].Value -eq 'true')
            })
        }
    }
    if ($list.Count -eq 0) { throw "SUPPORTED_LANGUAGES table not parsed from $Path" }
    return , $list
}

function Get-LocalizedHelpWords {
    param([string]$Path, [string[]]$Tables)
    # The UI has no standalone "Help" string; every table carries it inside
    # CATEGORY_SHORTCUTS_TEXT_ID as "F1: <Help> - F2: <Settings>". The colon
    # varies ("F1 : Aide" in French, a full-width U+FF1A colon in Chinese).
    $text = [System.IO.File]::ReadAllText($Path)
    $words = @{}
    foreach ($table in $Tables) {
        $block = [regex]::Match($text, '(?s)std::vector<std::pair<int, const wchar_t \*>>\s+' + [regex]::Escape($table) + '\s*=\s*\{(.*?)\r?\n\};')
        if (-not $block.Success) { throw "String table $table not found in $Path" }
        $entry = [regex]::Match($block.Groups[1].Value, '\{\s*CATEGORY_SHORTCUTS_TEXT_ID\s*,\s*L"([^"]*)"')
        if (-not $entry.Success) { throw "CATEGORY_SHORTCUTS_TEXT_ID not found in $table" }
        # Full-width colon built at runtime: a literal one would make this file
        # non-ASCII, which Windows PowerShell 5.1 misreads without a BOM.
        $fullWidthColon = [string][char]0xFF1A
        $word = [regex]::Match($entry.Groups[1].Value, 'F1\s*[:' + $fullWidthColon + ']\s*(.+?)\s+-\s+F2')
        if (-not $word.Success) { throw "Cannot extract the word for Help from $table ('$($entry.Groups[1].Value)')" }
        $words[$table] = $word.Groups[1].Value.Trim()
    }
    return $words
}

function ConvertTo-HelpFileName {
    param([string]$HelpWord, [string]$NativeName)
    $name = ('SimonSays_' + $HelpWord + '_(' + $NativeName + ')') -replace ' ', '_'
    foreach ($c in [System.IO.Path]::GetInvalidFileNameChars()) { $name = $name.Replace([string]$c, '') }
    return "$name.html"
}

function Get-PlainText {
    param([string]$Markdown)
    $t = [regex]::Replace($Markdown, $reCodeSpan, '$2')
    $t = [regex]::Replace($t, '\[([^\]]+)\]\([^)]*\)', '$1')
    $t = $t.Replace('**', '').Replace('__', '')
    return [regex]::Replace($t, '\\(.)', '$1').Trim()
}

function Get-HeadingSlug {
    param([string]$Markdown, [hashtable]$Used)
    # GitHub rules: lower-case, drop punctuation and symbols (keep letters,
    # combining marks, digits, '_' and '-'), each space becomes '-', then
    # de-duplicate with -1, -2...  Combining marks are kept so Devanagari and
    # Arabic headings keep their vowel signs.
    $slug = (Get-PlainText $Markdown).ToLowerInvariant()
    $slug = [regex]::Replace($slug, '[^\p{L}\p{M}\p{Nd}\p{Pc}\- ]', '')
    $slug = $slug.Replace(' ', '-')
    if (-not $slug) { $slug = 'section' }
    $base = $slug
    $n = 0
    while ($Used.ContainsKey($slug)) { $n++; $slug = "$base-$n" }
    $Used[$slug] = $true
    return $slug
}

function ConvertFrom-Inline {
    param([string]$Text)
    $store = New-Object System.Collections.Generic.List[string]
    $mark = $script:TokenMark

    # 1. Code spans first: their contents are literal (backslashes included).
    $Text = [regex]::Replace($Text, $reCodeSpan, [System.Text.RegularExpressions.MatchEvaluator] {
            param($m)
            $code = $m.Groups[2].Value
            if ($code.Length -ge 3 -and $code.StartsWith(' ') -and $code.EndsWith(' ')) {
                $code = $code.Substring(1, $code.Length - 2)
            }
            $store.Add('<code>' + (ConvertTo-HtmlText $code) + '</code>')
            return $mark + ($store.Count - 1) + $mark
        })

    # 2. Backslash escapes outside code.
    $Text = [regex]::Replace($Text, '\\([\\`*_{}\[\]()#+\-.!|<>~])', [System.Text.RegularExpressions.MatchEvaluator] {
            param($m)
            $store.Add((ConvertTo-HtmlText $m.Groups[1].Value))
            return $mark + ($store.Count - 1) + $mark
        })

    # 3. Escape the rest, then add the supported markup.
    $Text = ConvertTo-HtmlText $Text
    $Text = [regex]::Replace($Text, '\[([^\]]+)\]\(([^)\s]+)\)', '<a href="$2">$1</a>')
    $Text = [regex]::Replace($Text, '\*\*(.+?)\*\*', '<strong>$1</strong>')
    $Text = [regex]::Replace($Text, '__(.+?)__', '<strong>$1</strong>')

    # 4. Restore protected fragments.
    return [regex]::Replace($Text, [regex]::Escape($mark) + '(\d+)' + [regex]::Escape($mark), [System.Text.RegularExpressions.MatchEvaluator] {
            param($m)
            return $store[[int]$m.Groups[1].Value]
        })
}

function Join-InlineLines {
    param($Lines)
    $parts = New-Object System.Collections.Generic.List[string]
    for ($k = 0; $k -lt $Lines.Count; $k++) {
        $line = $Lines[$k]
        $isLast = ($k -eq $Lines.Count - 1)
        $parts.Add($line.Trim())
        if (-not $isLast) {
            if ($line -match '\S {2,}$') { $parts.Add($script:BreakMark) } else { $parts.Add("`n") }
        }
    }
    $html = ConvertFrom-Inline (-join $parts)
    return $html.Replace($script:BreakMark, "<br>`n")
}

function Test-ListInterrupt {
    param([string]$Line)
    # A bullet item, or a numbered item starting at 1, may interrupt a
    # paragraph without a blank line (CommonMark).
    if ($Line -match $reHr) { return $false }
    $m = [regex]::Match($Line, $reItem)
    if (-not $m.Success -or $m.Groups[3].Value.Trim().Length -eq 0) { return $false }
    $marker = $m.Groups[2].Value
    if ($marker -match '^\d') { return ($marker -match '^1[.)]$') }
    return $true
}

function Test-BlockStart {
    param([string]$Line)
    return ($Line -match $reFence) -or ($Line -match $reHeadingStart) -or
    ($Line -match $reQuote) -or ($Line -match $reHr) -or (Test-ListInterrupt $Line)
}

function Get-Indent {
    param([string]$Line)
    return ($Line.Length - $Line.TrimStart(' ').Length)
}

function Assert-SupportedMarkdown {
    param([string[]]$Lines, [string]$File)
    $inFence = $false
    $fenceStart = 0
    $fencePattern = ''
    $prevParagraph = $false
    for ($k = 0; $k -lt $Lines.Count; $k++) {
        $line = $Lines[$k]
        $ln = $k + 1
        if ($inFence) {
            if ($line -match $fencePattern) { $inFence = $false }
            continue
        }
        $fm = [regex]::Match($line, $reFence)
        if ($fm.Success) {
            $inFence = $true
            $fenceStart = $ln
            $run = $fm.Groups[1].Value
            $fencePattern = '^\s{0,3}' + [regex]::Escape($run.Substring(0, 1)) + '{' + $run.Length + ',}\s*$'
            $prevParagraph = $false
            continue
        }
        $noCode = [regex]::Replace($line, $reCodeSpan, '')
        if ($line -match $reTable) { throw "${File}:${ln}: tables are not supported by this converter." }
        if ($noCode -match '!\[') { throw "${File}:${ln}: images are not supported by this converter." }
        if ($noCode -match '<!--') { throw "${File}:${ln}: HTML comments are not supported by this converter." }
        if ($noCode -match $reHtmlTag) { throw "${File}:${ln}: raw HTML tags are not supported by this converter ('$($Matches[0])')." }
        if ($prevParagraph -and ($line -match '^\s{0,3}(=+|-+)\s*$')) {
            throw "${File}:${ln}: setext (underlined) headings are not supported; use # headings."
        }
        $isBlank = ($line.Trim().Length -eq 0)
        $prevParagraph = (-not $isBlank) -and -not ($line -match $reHeadingStart) -and
        -not ($line -match $reQuote) -and -not ($line -match $reHr) -and -not ([regex]::IsMatch($line, $reItem))
    }
    if ($inFence) { throw "${File}:${fenceStart}: code fence is never closed." }
}

function Convert-List {
    param([string[]]$Lines, [ref]$Index, [hashtable]$Ctx)
    $n = $Lines.Count
    $first = [regex]::Match($Lines[$Index.Value], $reItem)
    $listIndent = $first.Groups[1].Value.Length
    $ordered = ($first.Groups[2].Value -match '^\d')
    $start = 1
    if ($ordered) { $start = [int]($first.Groups[2].Value -replace '[.)]$', '') }

    $items = New-Object System.Collections.Generic.List[object]
    $current = $null
    $lastWasBlank = $false

    while ($Index.Value -lt $n) {
        $line = $Lines[$Index.Value]

        if ($line.Trim().Length -eq 0) {
            $j = $Index.Value + 1
            while ($j -lt $n -and $Lines[$j].Trim().Length -eq 0) { $j++ }
            if ($j -ge $n) { $Index.Value = $j; break }
            $next = $Lines[$j]
            $nextIndent = Get-Indent $next
            $nm = [regex]::Match($next, $reItem)
            $continues = $false
            if ($nm.Success -and -not ($next -match $reHr)) {
                if ($nextIndent -gt $listIndent) { $continues = $true }
                elseif ($nextIndent -eq $listIndent -and (($nm.Groups[2].Value -match '^\d') -eq $ordered)) { $continues = $true }
            }
            elseif ($nextIndent -gt $listIndent -and $null -ne $current) { $continues = $true }
            $Index.Value = $j
            if (-not $continues) { break }
            $lastWasBlank = $true
            continue
        }

        $indent = Get-Indent $line
        $m = [regex]::Match($line, $reItem)
        if ($m.Success -and -not ($line -match $reHr)) {
            if ($indent -eq $listIndent) {
                if (($m.Groups[2].Value -match '^\d') -ne $ordered) { break }
                $current = [pscustomobject]@{ Chunks = (New-Object System.Collections.Generic.List[object]) }
                $chunk = [pscustomobject]@{ Kind = 'text'; Lines = (New-Object System.Collections.Generic.List[string]) }
                $chunk.Lines.Add($m.Groups[3].Value)
                $current.Chunks.Add($chunk)
                $items.Add($current)
                $Index.Value++
                $lastWasBlank = $false
                continue
            }
            if ($indent -gt $listIndent -and $null -ne $current) {
                $nested = Convert-List -Lines $Lines -Index $Index -Ctx $Ctx
                $current.Chunks.Add([pscustomobject]@{ Kind = 'html'; Html = $nested })
                $lastWasBlank = $false
                continue
            }
            break
        }

        # Not an item: a paragraph continuation, or the end of the list.
        if ($null -eq $current) { break }
        if ((Test-BlockStart $line) -and $indent -le $listIndent) { break }
        if ($lastWasBlank) {
            if ($indent -le $listIndent) { break }
            $chunk = [pscustomobject]@{ Kind = 'text'; Lines = (New-Object System.Collections.Generic.List[string]) }
            $chunk.Lines.Add($line)
            $current.Chunks.Add($chunk)
        }
        else {
            $last = $current.Chunks[$current.Chunks.Count - 1]
            if ($last.Kind -ne 'text') {
                $last = [pscustomobject]@{ Kind = 'text'; Lines = (New-Object System.Collections.Generic.List[string]) }
                $current.Chunks.Add($last)
            }
            $last.Lines.Add($line)
        }
        $Index.Value++
        $lastWasBlank = $false
    }

    $sb = New-Object System.Text.StringBuilder
    if ($ordered) {
        if ($start -ne 1) { $null = $sb.Append('<ol start="' + $start + "`">`n") } else { $null = $sb.Append("<ol>`n") }
    }
    else { $null = $sb.Append("<ul>`n") }
    foreach ($item in $items) {
        $null = $sb.Append('<li>')
        for ($c = 0; $c -lt $item.Chunks.Count; $c++) {
            $chunk = $item.Chunks[$c]
            if ($chunk.Kind -eq 'html') { $null = $sb.Append("`n" + $chunk.Html) }
            elseif ($c -eq 0) { $null = $sb.Append((Join-InlineLines $chunk.Lines)) }
            else { $null = $sb.Append('<p>' + (Join-InlineLines $chunk.Lines) + '</p>') }
        }
        $null = $sb.Append("</li>`n")
    }
    if ($ordered) { $null = $sb.Append("</ol>`n") } else { $null = $sb.Append("</ul>`n") }
    return $sb.ToString()
}

function Convert-Blocks {
    param([string[]]$Lines, [hashtable]$Ctx)
    $sb = New-Object System.Text.StringBuilder
    $i = 0
    $n = $Lines.Count
    while ($i -lt $n) {
        $line = $Lines[$i]
        if ($line.Trim().Length -eq 0) { $i++; continue }

        $m = [regex]::Match($line, $reFence)
        if ($m.Success) {
            $run = $m.Groups[1].Value
            $closePattern = '^\s{0,3}' + [regex]::Escape($run.Substring(0, 1)) + '{' + $run.Length + ',}\s*$'
            $info = $m.Groups[2].Value
            $body = New-Object System.Collections.Generic.List[string]
            $i++
            while ($i -lt $n -and -not ($Lines[$i] -match $closePattern)) { $body.Add($Lines[$i]); $i++ }
            $i++
            $class = ''
            if ($info) { $class = ' class="language-' + (ConvertTo-HtmlText $info) + '"' }
            $null = $sb.Append('<pre><code' + $class + '>' + (ConvertTo-HtmlText ($body -join "`n")) + "</code></pre>`n")
            continue
        }

        $m = [regex]::Match($line, $reHeading)
        if ($m.Success) {
            $level = $m.Groups[1].Value.Length
            $raw = $m.Groups[2].Value
            $id = Get-HeadingSlug -Markdown $raw -Used $Ctx.UsedIds
            if ($level -eq 1 -and -not $Ctx.Title) { $Ctx.Title = Get-PlainText $raw }
            $null = $sb.Append("<h$level id=`"$id`">" + (ConvertFrom-Inline $raw) + "</h$level>`n")
            $i++
            continue
        }

        if ($line -match $reHr) { $null = $sb.Append("<hr>`n"); $i++; continue }

        if ($line -match $reQuote) {
            $inner = New-Object System.Collections.Generic.List[string]
            while ($i -lt $n) {
                $q = [regex]::Match($Lines[$i], $reQuote)
                if ($q.Success) { $inner.Add($q.Groups[1].Value); $i++; continue }
                $lazy = ($Lines[$i].Trim().Length -gt 0) -and ($inner.Count -gt 0) -and
                ($inner[$inner.Count - 1].Trim().Length -gt 0) -and -not (Test-BlockStart $Lines[$i])
                if ($lazy) { $inner.Add($Lines[$i]); $i++; continue }
                break
            }
            $null = $sb.Append("<blockquote>`n" + (Convert-Blocks -Lines $inner.ToArray() -Ctx $Ctx) + "</blockquote>`n")
            continue
        }

        if ([regex]::IsMatch($line, $reItem)) {
            $ref = [ref]$i
            $null = $sb.Append((Convert-List -Lines $Lines -Index $ref -Ctx $Ctx))
            $i = $ref.Value
            continue
        }

        $para = New-Object System.Collections.Generic.List[string]
        $para.Add($line)
        $i++
        while ($i -lt $n -and $Lines[$i].Trim().Length -gt 0 -and -not (Test-BlockStart $Lines[$i])) {
            $para.Add($Lines[$i])
            $i++
        }
        $null = $sb.Append('<p>' + (Join-InlineLines $para) + "</p>`n")
    }
    return $sb.ToString()
}

function Get-RelativeHref {
    param([string]$FromDir, [string]$ToFile)
    $from = [System.IO.Path]::GetFullPath($FromDir).TrimEnd('\', '/') + '\'
    $to = [System.IO.Path]::GetFullPath($ToFile)
    return ([Uri]$from).MakeRelativeUri([Uri]$to).ToString()
}

function Get-LanguageNav {
    param($Pages, $Current)
    $sb = New-Object System.Text.StringBuilder
    $null = $sb.Append("<nav class=`"lang-nav`" aria-label=`"Languages`">`n<ul>`n")
    foreach ($p in $Pages) {
        $dir = 'ltr'
        if ($p.Rtl) { $dir = 'rtl' }
        $label = ConvertTo-HtmlText $p.Native
        if ($p.English -eq $Current.English) {
            $null = $sb.Append("<li><span aria-current=`"page`" lang=`"$($p.Lang)`" dir=`"$dir`">$label</span></li>`n")
        }
        else {
            $href = [Uri]::EscapeDataString($p.FileName)
            $null = $sb.Append("<li><a href=`"$href`" hreflang=`"$($p.Lang)`" lang=`"$($p.Lang)`" dir=`"$dir`">$label</a></li>`n")
        }
    }
    $null = $sb.Append("</ul>`n</nav>`n")
    return $sb.ToString()
}

function Set-HelpFileNameStrings {
    param([string]$Path, $Pages)
    # Write { HELP_CONTENT_FILE_NAME_ID, L"<file>" } into each language table,
    # replacing the entry if it is already there, otherwise inserting it just
    # before the table's closing "};". The app resolves the help page it opens
    # through this id, so writing it here is what keeps the name the app asks
    # for and the file this script produced from drifting apart.
    #
    # The file is read and written as bytes so its UTF-8 BOM survives, and its
    # existing line ending is reused (it is LF in some checkouts, CRLF in
    # others - splitting on CRLF alone silently matches nothing).
    $bytes = [System.IO.File]::ReadAllBytes($Path)
    $bom = ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF)
    $offset = 0
    if ($bom) { $offset = 3 }
    $text = [System.Text.Encoding]::UTF8.GetString($bytes, $offset, $bytes.Length - $offset)
    $eol = "`n"
    if ($text.Contains("`r`n")) { $eol = "`r`n" }

    $byTable = @{}
    foreach ($p in $Pages) { $byTable[$p.Table] = $p.FileName }

    $lines = $text -split '\r?\n', -1
    $out = New-Object System.Collections.Generic.List[string]
    $cur = $null
    $done = @{}
    $written = 0
    foreach ($line in $lines) {
        $ms = [regex]::Match($line, '\b([A-Z_]+)_LOCALIZED_UI_STRINGS\s*=')
        if ($ms.Success) {
            $name = $ms.Groups[1].Value + '_LOCALIZED_UI_STRINGS'
            $cur = $null
            if ($byTable.ContainsKey($name)) { $cur = $name }
            $out.Add($line)
            continue
        }
        if ($null -ne $cur) {
            if ([regex]::IsMatch($line, '^\s*\{\s*HELP_CONTENT_FILE_NAME_ID\s*,')) {
                $out.Add('  { HELP_CONTENT_FILE_NAME_ID, L"' + $byTable[$cur] + '" },')
                $done[$cur] = $true
                $written++
                continue
            }
            if ($line.Trim() -eq '};') {
                if (-not $done.ContainsKey($cur)) {
                    $out.Add('  { HELP_CONTENT_FILE_NAME_ID, L"' + $byTable[$cur] + '" },')
                    $written++
                }
                $cur = $null
            }
        }
        $out.Add($line)
    }

    $missing = @($byTable.Keys | Where-Object { -not $done.ContainsKey($_) -and ($out -join '') -notmatch [regex]::Escape($byTable[$_]) })
    $result = [string]::Join($eol, $out)
    $encoding = New-Object System.Text.UTF8Encoding($bom)
    [System.IO.File]::WriteAllText($Path, $result, $encoding)
    return $written
}

# ---------------------------------------------------------------------------

if (-not (Test-Path $CssPath)) { throw "Stylesheet not found: $CssPath" }
$languages = Get-SupportedLanguages $stdafxPath
$helpWords = Get-LocalizedHelpWords -Path $stringsPath -Tables @($sources | ForEach-Object { $_.Table })

$pages = New-Object System.Collections.Generic.List[object]
foreach ($src in $sources) {
    $info = $languages | Where-Object { $_.English -eq $src.English } | Select-Object -First 1
    if (-not $info) { throw "'$($src.English)' is not in SUPPORTED_LANGUAGES ($stdafxPath)." }
    $path = Join-Path $repoRoot $src.File
    if (-not (Test-Path $path)) { throw "Help source not found: $path" }
    $pages.Add([pscustomobject]@{
            English   = $src.English
            Table     = $src.Table
            Native    = $info.Native
            Rtl       = $info.Rtl
            Lang      = $src.Lang
            Source    = $path
            RelSource = $src.File
            FileName  = (ConvertTo-HelpFileName -HelpWord $helpWords[$src.Table] -NativeName $info.Native)
        })
}

$dupes = @($pages | Group-Object FileName | Where-Object { $_.Count -gt 1 })
if ($dupes) { throw "Output file name collision: $($dupes[0].Name)" }

# Switcher follows the app's own language order.
$navPages = @(foreach ($l in $languages) { $pages | Where-Object { $_.English -eq $l.English } })

if (-not (Test-Path $OutDir)) { $null = New-Item -ItemType Directory -Path $OutDir -Force }

if ($InlineCss) {
    $cssText = [System.IO.File]::ReadAllText($CssPath)
    $cssTag = "<style>`n" + $cssText.TrimEnd() + "`n</style>"
}
else {
    $cssTag = '<link rel="stylesheet" href="' + (ConvertTo-HtmlText (Get-RelativeHref -FromDir $OutDir -ToFile $CssPath)) + '">'
}

$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$written = 0

foreach ($page in $pages) {
    $lines = Read-TextLines $page.Source
    Assert-SupportedMarkdown -Lines $lines -File $page.RelSource

    $ctx = @{ UsedIds = @{}; Title = $null }
    $body = Convert-Blocks -Lines $lines -Ctx $ctx
    $title = 'SimonSays Help'
    if ($ctx.Title) { $title = $ctx.Title }
    $dir = 'ltr'
    if ($page.Rtl) { $dir = 'rtl' }

    $html = "<!DOCTYPE html>`n" +
    "<!-- Generated by scripts/build_help_html.ps1 from $($page.RelSource). Do not edit: change the Markdown and re-run the script. -->`n" +
    "<html lang=`"$($page.Lang)`" dir=`"$dir`">`n" +
    "<head>`n" +
    "<meta charset=`"utf-8`">`n" +
    "<meta name=`"viewport`" content=`"width=device-width, initial-scale=1`">`n" +
    "<title>$(ConvertTo-HtmlText $title)</title>`n" +
    "$cssTag`n" +
    "</head>`n" +
    "<body>`n" +
    (Get-LanguageNav -Pages $navPages -Current $page) +
    "<main>`n" +
    $body +
    "</main>`n" +
    "</body>`n" +
    "</html>`n"

    $outPath = Join-Path $OutDir $page.FileName
    if ($PSCmdlet.ShouldProcess($outPath, 'Write help page')) {
        [System.IO.File]::WriteAllText($outPath, $html, $utf8NoBom)
        $written++
    }
    Write-Host ("{0,-22} {1,-26} -> {2}" -f $page.English, $page.RelSource, $page.FileName)
}

Write-Host ""
Write-Host "$written help page(s) written to $OutDir"

if ($NoStringTable) {
    Write-Host "include/localized_strings.h left untouched (-NoStringTable)."
}
elseif ($PSCmdlet.ShouldProcess($stringsPath, 'Write HELP_CONTENT_FILE_NAME_ID entries')) {
    $count = Set-HelpFileNameStrings -Path $stringsPath -Pages $pages
    Write-Host "$count HELP_CONTENT_FILE_NAME_ID entr(ies) written to include/localized_strings.h"
    Write-Host "Rebuild: this header is included widely."
}
