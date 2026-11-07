<#
.SYNOPSIS
    Captures the help screenshots of SimonSays, once per UI language.

.DESCRIPTION
    Starts the built SimonSays.exe once per language on a fresh profile, opens
    each window the help illustrates by posting the app's own commands (no
    mouse or keyboard input is synthesized), captures it, and writes
    docs/help/img/<lang>/<name>.png - the files build_help_html.ps1 embeds into
    the help pages (localization.spec.md LOC-F43). <lang> is the page's BCP 47
    tag, the same one build_help_html.ps1 uses.

    Shots (name: what it shows):
      bar         the SimonSays bar on the taskbar
      categories  the Categories window with the default board
      menu        the right-click menu on a phrase button
      settings    the Settings window (F2)
      dwell       the Gaze / Dwell-click window (F3)

    Your own SimonSays data is kept: HKCU\SOFTWARE\SimonSays is exported
    before the first run and imported back at the end (also after a failure),
    and files the runs add under %LocalAppData%\SimonSays (each language's
    shipped boards) are removed again unless -KeepNewFiles is given. Files
    that already existed are never touched. SimonSays must not be running.

    For images that match the help, capture at 100 % display scaling, light
    theme, with the taskbar at the bottom and its icons centered; avoid moving
    windows over the bar while it runs (the bar and the menu are copied from
    the screen; the other windows are rendered with PrintWindow and do not
    mind what covers them).

    The script is pure ASCII on purpose: Windows PowerShell 5.1 misreads a
    BOM-less non-ASCII script.

.PARAMETER Languages
    English language names to capture (as in SUPPORTED_LANGUAGES). Default:
    all 18.

.PARAMETER ExePath
    SimonSays.exe to run. Default: Release\SimonSays.exe in the repository.

.PARAMETER OutDir
    Image root. Default: docs/help/img.

.PARAMETER KeepNewFiles
    Keep the files the runs add under %LocalAppData%\SimonSays.

.EXAMPLE
    .\scripts\capture_help_screenshots.ps1

.EXAMPLE
    .\scripts\capture_help_screenshots.ps1 -Languages English,Spanish
#>
[CmdletBinding()]
param(
    [string[]]$Languages,
    [string]$ExePath,
    [string]$OutDir,
    [switch]$KeepNewFiles
)

$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not $ExePath) { $ExePath = Join-Path $repoRoot 'Release\SimonSays.exe' }
if (-not $OutDir) { $OutDir = Join-Path $repoRoot 'docs\help\img' }

# English name (registry value) -> BCP 47 tag (image folder). Must match the
# Lang column of scripts/build_help_html.ps1.
$tags = [ordered]@{
    'English' = 'en'; 'Arabic' = 'ar'; 'Basque' = 'eu'; 'Catalan' = 'ca'
    'Chinese (Simplified)' = 'zh-CN'; 'French' = 'fr'; 'Galician' = 'gl'; 'German' = 'de'
    'Hebrew' = 'he'; 'Hindi' = 'hi'; 'Italian' = 'it'; 'Japanese' = 'ja'; 'Korean' = 'ko'
    'Portuguese' = 'pt-PT'; 'Portuguese (Brazil)' = 'pt-BR'; 'Russian' = 'ru'
    'Spanish' = 'es'; 'Valencian' = 'ca-ES-valencia'
}
if (-not $Languages) { $Languages = @($tags.Keys) }
foreach ($l in $Languages) { if (-not $tags.Contains($l)) { throw "Unknown language '$l'. Use one of: $($tags.Keys -join ', ')" } }
if (-not (Test-Path $ExePath)) { throw "SimonSays.exe not found: $ExePath (build Release Win32 first)." }
if (Get-Process -Name 'SimonSays' -ErrorAction SilentlyContinue) { throw 'SimonSays is running. Exit it first (tray menu > Exit).' }

Add-Type -AssemblyName System.Drawing
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Text;

public static class SsCapture
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RECT { public int Left, Top, Right, Bottom; }

    delegate bool EnumProc(IntPtr hwnd, IntPtr lParam);
    [DllImport("user32.dll")] static extern bool EnumWindows(EnumProc cb, IntPtr lParam);
    [DllImport("user32.dll")] static extern bool EnumChildWindows(IntPtr parent, EnumProc cb, IntPtr lParam);
    [DllImport("user32.dll")] static extern uint GetWindowThreadProcessId(IntPtr hwnd, out uint pid);
    [DllImport("user32.dll", CharSet = CharSet.Unicode)] static extern int GetClassName(IntPtr hwnd, StringBuilder sb, int max);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hwnd);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr hwnd, out RECT r);
    [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr hwnd, uint msg, IntPtr w, IntPtr l);
    [DllImport("user32.dll")] static extern bool PrintWindow(IntPtr hwnd, IntPtr hdc, uint flags);
    [DllImport("dwmapi.dll")] static extern int DwmGetWindowAttribute(IntPtr hwnd, int attr, out RECT r, int size);
    [DllImport("user32.dll")] static extern int GetWindowLong(IntPtr hwnd, int index);

    public static string ClassOf(IntPtr hwnd)
    {
        var sb = new StringBuilder(256);
        GetClassName(hwnd, sb, 256);
        return sb.ToString();
    }

    // Visible top-level windows of a process with the given class.
    public static IntPtr[] TopWindows(int pid, string cls)
    {
        var found = new List<IntPtr>();
        EnumWindows(delegate (IntPtr h, IntPtr l) {
            uint p; GetWindowThreadProcessId(h, out p);
            if (p == (uint)pid && IsWindowVisible(h) && ClassOf(h) == cls) found.Add(h);
            return true;
        }, IntPtr.Zero);
        return found.ToArray();
    }

    // Visible top-level windows of any process with the given class (menus).
    public static IntPtr[] AnyTopWindows(string cls)
    {
        var found = new List<IntPtr>();
        EnumWindows(delegate (IntPtr h, IntPtr l) {
            if (IsWindowVisible(h) && ClassOf(h) == cls) found.Add(h);
            return true;
        }, IntPtr.Zero);
        return found.ToArray();
    }

    public static IntPtr[] Children(IntPtr parent, string cls)
    {
        var found = new List<IntPtr>();
        EnumChildWindows(parent, delegate (IntPtr h, IntPtr l) {
            if (IsWindowVisible(h) && ClassOf(h) == cls) found.Add(h);
            return true;
        }, IntPtr.Zero);
        return found.ToArray();
    }

    // The window's visible bounds (DWM extended frame, without the shadow).
    public static RECT Bounds(IntPtr hwnd)
    {
        RECT r;
        if (DwmGetWindowAttribute(hwnd, 9 /* DWMWA_EXTENDED_FRAME_BOUNDS */, out r, Marshal.SizeOf(typeof(RECT))) != 0)
            GetWindowRect(hwnd, out r);
        return r;
    }

    // Renders the window itself, whatever covers it.
    public static void SaveWindow(IntPtr hwnd, string path)
    {
        RECT w; GetWindowRect(hwnd, out w);
        RECT b = Bounds(hwnd);
        int ww = w.Right - w.Left, wh = w.Bottom - w.Top;
        using (var full = new Bitmap(ww, wh, PixelFormat.Format32bppArgb))
        {
            using (var g = Graphics.FromImage(full))
            {
                IntPtr hdc = g.GetHdc();
                PrintWindow(hwnd, hdc, 2 /* PW_RENDERFULLCONTENT */);
                g.ReleaseHdc(hdc);
            }
            int cropX = b.Left - w.Left;
            // A mirrored (right-to-left layout) window comes back flipped:
            // flip it back, and mirror the crop with it.
            if ((GetWindowLong(hwnd, -20 /* GWL_EXSTYLE */) & 0x00400000 /* WS_EX_LAYOUTRTL */) != 0)
            {
                full.RotateFlip(RotateFlipType.RotateNoneFlipX);
                cropX = w.Right - b.Right;
            }
            var crop = new Rectangle(cropX, b.Top - w.Top, b.Right - b.Left, b.Bottom - b.Top);
            crop.Intersect(new Rectangle(0, 0, ww, wh));
            using (var outBmp = full.Clone(crop, PixelFormat.Format24bppRgb))
                outBmp.Save(path, ImageFormat.Png);
        }
    }

    // Copies a screen rectangle (for the bar over the taskbar, and menus).
    public static void SaveScreen(int left, int top, int right, int bottom, string path)
    {
        using (var bmp = new Bitmap(right - left, bottom - top, PixelFormat.Format24bppRgb))
        {
            using (var g = Graphics.FromImage(bmp))
                g.CopyFromScreen(left, top, 0, 0, new Size(right - left, bottom - top));
            bmp.Save(path, ImageFormat.Png);
        }
    }
}
'@

$WM_COMMAND = 0x0111; $WM_CLOSE = 0x0010; $WM_CONTEXTMENU = 0x007B; $WM_KEYDOWN = 0x0100; $WM_KEYUP = 0x0101
$IDCANCEL = 2; $ID_TRAY_SETTINGS = 2002; $ID_TRAY_DWELL = 2007; $IDC_BUTTON_CATEGORIES = 3003

function Wait-Until {
    param([scriptblock]$Condition, [int]$TimeoutMs = 8000, [string]$What)
    $sw = [Diagnostics.Stopwatch]::StartNew()
    while ($sw.ElapsedMilliseconds -lt $TimeoutMs) {
        $r = & $Condition
        if ($r) { return $r }
        Start-Sleep -Milliseconds 100
    }
    throw "Timed out waiting for $What."
}

function Get-FileSet {
    param([string]$Root)
    $set = @{}
    if (Test-Path $Root) { foreach ($f in Get-ChildItem $Root -Recurse -Force) { $set[$f.FullName] = $true } }
    return $set
}

function Invoke-Language {
    param([string]$Language, [string]$Dir)
    $regRoot = 'HKCU:\SOFTWARE\SimonSays'
    if (Test-Path $regRoot) { Remove-Item $regRoot -Recurse -Force }
    $null = New-Item "$regRoot\Settings" -Force
    $null = New-Item "$regRoot\LastRun" -Force
    New-ItemProperty "$regRoot\Settings" -Name 'Language' -Value $Language -PropertyType String -Force | Out-Null
    New-ItemProperty "$regRoot\LastRun" -Name 'Widgets Hint Shown' -Value '1' -PropertyType String -Force | Out-Null

    $proc = Start-Process -FilePath $ExePath -PassThru
    try {
        $main = Wait-Until -What 'the main window' -Condition { $w = [SsCapture]::TopWindows($proc.Id, 'SimonSaysMainWindow'); if ($w.Count) { $w[0] } }
        Start-Sleep -Milliseconds 1500   # let the bar settle over the taskbar

        # bar: copied from the screen, a little margin around the bar
        $b = [SsCapture]::Bounds($main)
        [SsCapture]::SaveScreen($b.Left - 6, $b.Top - 6, $b.Right + 6, $b.Bottom + 6, (Join-Path $Dir 'bar.png'))

        # categories
        [void][SsCapture]::PostMessage($main, $WM_COMMAND, [IntPtr]$IDC_BUTTON_CATEGORIES, [IntPtr]::Zero)
        $cat = Wait-Until -What 'the Categories window' -Condition { $w = [SsCapture]::TopWindows($proc.Id, 'SimonSaysCategoryWindow'); if ($w.Count) { $w[0] } }
        Start-Sleep -Milliseconds 1000
        [SsCapture]::SaveWindow($cat, (Join-Path $Dir 'categories.png'))

        # menu: right-click menu on the first phrase button (the first button
        # in the lower half of the window), copied from the screen together
        # with the part of the window it covers
        $cb = [SsCapture]::Bounds($cat)
        $mid = ($cb.Top + $cb.Bottom) / 2
        $phrase = [SsCapture]::Children($cat, 'SSButton') | ForEach-Object { [pscustomobject]@{ H = $_; R = [SsCapture]::Bounds($_) } } |
            Where-Object { $_.R.Top -gt $mid } | Sort-Object { $_.R.Top }, { $_.R.Left } | Select-Object -First 1
        if ($phrase) {
            $x = [int](($phrase.R.Left + $phrase.R.Right) / 2); $y = [int](($phrase.R.Top + $phrase.R.Bottom) / 2)
            $lp = [IntPtr](($y -shl 16) -bor ($x -band 0xFFFF))
            [void][SsCapture]::PostMessage($cat, $WM_CONTEXTMENU, $phrase.H, $lp)
            $menu = Wait-Until -What 'the right-click menu' -Condition { $w = [SsCapture]::AnyTopWindows('#32768'); if ($w.Count) { $w[0] } }
            Start-Sleep -Milliseconds 600
            $m = [SsCapture]::Bounds($menu)
            # The menu may open above, below or beside the button: frame both.
            $l = [Math]::Min($m.Left, $phrase.R.Left) - 12; $t = [Math]::Min($m.Top, $phrase.R.Top) - 12
            $r = [Math]::Max($m.Right, $phrase.R.Right) + 12; $bt = [Math]::Max($m.Bottom, $phrase.R.Bottom) + 12
            [SsCapture]::SaveScreen($l, $t, $r, $bt, (Join-Path $Dir 'menu.png'))
            [void][SsCapture]::PostMessage($menu, $WM_KEYDOWN, [IntPtr]0x1B, [IntPtr]::Zero)
            [void][SsCapture]::PostMessage($menu, $WM_KEYUP, [IntPtr]0x1B, [IntPtr]::Zero)
            $null = Wait-Until -What 'the menu to close' -Condition { ([SsCapture]::AnyTopWindows('#32768')).Count -eq 0 }
        }
        else { Write-Warning "${Language}: no phrase button found; menu.png skipped." }

        # settings and dwell: modal dialogs of the app
        foreach ($shot in @(@{ Name = 'settings'; Cmd = $ID_TRAY_SETTINGS }, @{ Name = 'dwell'; Cmd = $ID_TRAY_DWELL })) {
            [void][SsCapture]::PostMessage($main, $WM_COMMAND, [IntPtr]$shot.Cmd, [IntPtr]::Zero)
            $dlg = Wait-Until -What "the $($shot.Name) window" -Condition { $w = [SsCapture]::TopWindows($proc.Id, '#32770'); if ($w.Count) { $w[0] } }
            Start-Sleep -Milliseconds 1200   # dwell: let device detection fill in
            [SsCapture]::SaveWindow($dlg, (Join-Path $Dir "$($shot.Name).png"))
            [void][SsCapture]::PostMessage($dlg, $WM_COMMAND, [IntPtr]$IDCANCEL, [IntPtr]::Zero)
            $null = Wait-Until -What "the $($shot.Name) window to close" -Condition { ([SsCapture]::TopWindows($proc.Id, '#32770')).Count -eq 0 }
        }
    }
    finally {
        # WM_CLOSE takes the normal shutdown path (restores taskbar auto-hide).
        foreach ($w in [SsCapture]::TopWindows($proc.Id, 'SimonSaysMainWindow')) { [void][SsCapture]::PostMessage($w, $WM_CLOSE, [IntPtr]::Zero, [IntPtr]::Zero) }
        if (-not $proc.WaitForExit(10000)) { $proc.Kill(); $proc.WaitForExit() }
    }
}

# ---------------------------------------------------------------------------

$appData = Join-Path $env:LOCALAPPDATA 'SimonSays'
$backup = Join-Path $env:TEMP ('SimonSays_registry_backup_' + (Get-Date -Format 'yyyyMMdd_HHmmss') + '.reg')
$hadKey = Test-Path 'HKCU:\SOFTWARE\SimonSays'
if ($hadKey) {
    # Through cmd: reg.exe reports success on stderr, which Windows PowerShell
    # 5.1 would turn into a terminating error.
    cmd.exe /c "reg.exe export HKCU\SOFTWARE\SimonSays `"$backup`" /y >nul 2>&1"
    if ($LASTEXITCODE -ne 0) { throw 'Could not back up HKCU\SOFTWARE\SimonSays; nothing was changed.' }
    Write-Host "Registry backed up to $backup"
}
$filesBefore = Get-FileSet $appData

try {
    foreach ($lang in $Languages) {
        $dir = Join-Path $OutDir $tags[$lang]
        $null = New-Item -ItemType Directory -Path $dir -Force
        Write-Host ("{0,-22} -> {1}" -f $lang, $dir)
        Invoke-Language -Language $lang -Dir $dir
    }
}
finally {
    if (Test-Path 'HKCU:\SOFTWARE\SimonSays') { Remove-Item 'HKCU:\SOFTWARE\SimonSays' -Recurse -Force }
    if ($hadKey) {
        cmd.exe /c "reg.exe import `"$backup`" >nul 2>&1"
        if ($LASTEXITCODE -ne 0) { Write-Warning "Registry restore failed; import $backup by hand." }
        else { Write-Host 'Registry restored.' }
    }
    if (-not $KeepNewFiles -and (Test-Path $appData)) {
        $new = Get-ChildItem $appData -Recurse -Force | Where-Object { -not $filesBefore.ContainsKey($_.FullName) } |
            Sort-Object { $_.FullName.Length } -Descending
        foreach ($f in $new) { Remove-Item $f.FullName -Recurse -Force -ErrorAction SilentlyContinue }
        Write-Host "$(@($new).Count) file(s) and folder(s) added during the runs removed from $appData."
    }
}
