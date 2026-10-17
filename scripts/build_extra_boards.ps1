<#
.SYNOPSIS
    Builds the ready-made extra categories shipped in the boards folder
    (Yes / No, Pain & body, Spelling, NATO alphabet, Alert tones, Morse code).

.DESCRIPTION
    Reads one UTF-8 JSON data file per board from scripts/extra_boards/ and
    writes .ssc (text only) or .ssz (with synthesized audio) files into the
    boards folder. Every file holds ONE category plus a metadata-only $$board
    line (language, title, credits), so importing it ADDS the category to the
    user's board instead of replacing it, while the language check still
    applies (import-export.spec.md PORT-F11/F14, board-style.spec.md STY-F60).

    Data file kinds:
      perLanguage  one file per language in "text"; items reference keys of
                   the language's text ("pain" + "n" fills {n}; "expand" splits
                   a space-separated list such as the alphabet into one key per
                   entry, each shown with the entry itself as its icon;
                   "optional" keys may be missing for a language).
                   File: <native language name>\SimonSays_<localized title>_(<native language name>).ssc
      translatedBundle  a hand-made .ssz ("source") rewritten per language in
                   "text" (keys = the source's texts); style, colors and pictures
                   are kept, title/caption/credits/language replaced, and
                   "dropStyleProperties" removed. The source is never modified.
                   File: <native language name>\<title>_(<native language name>).ssz
      hearingTest  quiet sine tones (WAV, mono or one ear) for each listed
                   frequency and side, the same audio in every language; per
                   language a spoken warning first, then the labelled keys.
                   File: <native language name>\<title>_(<native language name>).ssz
      neutral      one file for every language, no `language` property; items
                   carry their own text, or audio: "tone" segments or a
                   "morse" code, synthesized here and encoded to MP3 (a
                   Morse key shows its letter as the icon and its code as the
                   label).
                   File: SimonSays_<file>.ssc, or .ssz when it has audio, directly
                   in the boards folder.

    The per-language subfolders and file names follow the default boards
    (export_default_boards.ps1); the app copies the current language's folder
    plus the neutral files into the user's flat boards folder (PORT-F41).
    All content is original and released under CC0 (boards/README.md); the
    audio is generated, so nothing needs crediting. The one exception is a
    "translatedBundle" (translations of a hand-made board): it keeps the
    source's pictures and license, stated in its data file - the Fitzgerald Key
    example uses ARASAAC pictograms, CC BY-NC-SA. MP3 encoding uses Windows'
    own Media Foundation encoder (Windows 8 or later), so no tool is needed.

.PARAMETER OutDir
    Target folder. Defaults to the repository's boards folder.

.EXAMPLE
    .\scripts\build_extra_boards.ps1

.NOTES
    Keep this file pure ASCII (no BOM needed): all translated text lives in
    the JSON files, and the two Morse glyphs are built from code points.
#>
[CmdletBinding()]
param(
    [string]$OutDir
)

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not $OutDir) { $OutDir = Join-Path $repoRoot 'boards' }
$dataDir = Join-Path $PSScriptRoot 'extra_boards'
if (-not (Test-Path $OutDir)) { $null = New-Item -ItemType Directory -Path $OutDir -Force }

Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem

# Native language names, read from the app (English name -> native name).
$nativeNames = @{}
$inTable = $false
foreach ($line in ([System.IO.File]::ReadAllText((Join-Path $repoRoot 'include/stdafx.h'), [System.Text.Encoding]::UTF8) -split "`r?`n")) {
    if ($line -match 'SUPPORTED_LANGUAGES\s*=\s*\{') { $inTable = $true; continue }
    if (-not $inTable) { continue }
    if ($line -match '^\s*\};') { break }
    if ($line -match '^\s*\{\s*L"([^"]+)"\s*,\s*L"([^"]+)"\s*,') { $nativeNames[$Matches[1]] = $Matches[2] }
}
if ($nativeNames.Count -eq 0) { throw 'SUPPORTED_LANGUAGES not parsed from include/stdafx.h' }

function Remove-InvalidFileNameChars([string]$name) {
    foreach ($c in [System.IO.Path]::GetInvalidFileNameChars()) { $name = $name.Replace([string]$c, '') }
    return $name
}

# Same rule as GetShippedLanguageFolderName (src/utils.cpp) and
# Get-LanguageFolderName (export_default_boards.ps1).
function Get-LanguageFolderName([string]$nativeName) { return Remove-InvalidFileNameChars $nativeName }

# "SimonSays <title> (<native name>)", spaces as '_', illegal characters
# removed (so "Si / No" loses its slash), runs of '_' collapsed.
function Get-LocalizedFileName([string]$title, [string]$nativeName) {
    $name = Remove-InvalidFileNameChars ("SimonSays $title ($nativeName)" -replace ' ', '_')
    return ($name -replace '_{2,}', '_') + '.ssc'
}

Add-Type -Language CSharp -TypeDefinition @'
using System;
using System.IO;
using System.Runtime.InteropServices;

public static class ToneSynth
{
    // Renders segments to 16-bit mono PCM. A frequency of 0 is silence.
    // "decay" segments fade out exponentially (bells, chimes); every segment
    // gets 5 ms fades so the joins never click.
    public static short[] Render(double[][] freqs, int[] durMs, bool[] decay, int sampleRate, int padMs)
    {
        int pad = sampleRate * padMs / 1000;
        int total = 2 * pad;
        for (int s = 0; s < durMs.Length; s++) total += sampleRate * durMs[s] / 1000;

        short[] pcm = new short[total];
        int pos = pad;
        int fade = Math.Max(1, sampleRate * 5 / 1000);
        for (int s = 0; s < durMs.Length; s++)
        {
            int n = sampleRate * durMs[s] / 1000;
            int voices = 0;
            foreach (double f in freqs[s]) if (f > 0) voices++;
            for (int i = 0; i < n; i++)
            {
                double env = Math.Min(1.0, Math.Min((double)i / fade, (double)(n - 1 - i) / fade));
                if (decay[s]) env *= Math.Exp(-4.0 * i / n);
                double v = 0;
                if (voices > 0)
                {
                    double t = (double)i / sampleRate;
                    foreach (double f in freqs[s]) if (f > 0) v += Math.Sin(2 * Math.PI * f * t);
                    v = v / voices * 0.6 * env;
                }
                pcm[pos + i] = (short)Math.Round(v * 32767);
            }
            pos += n;
        }
        return pcm;
    }

    // One steady sine at `amplitude` (0..1) with raised-cosine fades, for the
    // hearing check: quiet and click-free.
    public static short[] Tone(double freq, int ms, int fadeMs, double amplitude, int sampleRate)
    {
        int n = sampleRate * ms / 1000;
        int fade = Math.Max(1, sampleRate * fadeMs / 1000);
        short[] pcm = new short[n];
        for (int i = 0; i < n; i++)
        {
            double env = 1.0;
            if (i < fade) env = 0.5 - 0.5 * Math.Cos(Math.PI * i / fade);
            else if (i >= n - fade) env = 0.5 - 0.5 * Math.Cos(Math.PI * (n - 1 - i) / fade);
            pcm[i] = (short)Math.Round(amplitude * env * Math.Sin(2 * Math.PI * freq * i / sampleRate) * 32767);
        }
        return pcm;
    }

    // 16-bit PCM WAV: side 0 = mono, 1 = stereo left only, 2 = stereo right only.
    public static byte[] Wav(short[] mono, int side, int sampleRate)
    {
        int channels = side == 0 ? 1 : 2;
        using (MemoryStream ms = new MemoryStream())
        using (BinaryWriter w = new BinaryWriter(ms))
        {
            int dataBytes = mono.Length * 2 * channels;
            w.Write(new char[] { 'R', 'I', 'F', 'F' }); w.Write(36 + dataBytes);
            w.Write(new char[] { 'W', 'A', 'V', 'E' });
            w.Write(new char[] { 'f', 'm', 't', ' ' }); w.Write(16);
            w.Write((short)1); w.Write((short)channels);
            w.Write(sampleRate); w.Write(sampleRate * 2 * channels);
            w.Write((short)(2 * channels)); w.Write((short)16);
            w.Write(new char[] { 'd', 'a', 't', 'a' }); w.Write(dataBytes);
            foreach (short smp in mono)
            {
                if (channels == 1) { w.Write(smp); continue; }
                w.Write(side == 1 ? smp : (short)0);
                w.Write(side == 2 ? smp : (short)0);
            }
            w.Flush();
            return ms.ToArray();
        }
    }
}

// Encodes 16-bit mono PCM to an MP3 file with Windows' own Media Foundation
// MP3 encoder (Windows 8 and later) through a Sink Writer - no external tool.
// Only the COM methods used are typed; the others are placeholders that keep
// the vtable order.
public static class Mp3Encoder
{
    [ComImport, Guid("44ae0fa8-ea31-4109-8d2e-4cae4997c555"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IMFMediaType
    {
        void m00(); void m01(); void m02(); void m03(); void m04(); void m05(); void m06(); void m07();
        void m08(); void m09(); void m10(); void m11(); void m12(); void m13(); void m14(); void m15();
        void m16(); void m17();
        [PreserveSig] int SetUINT32([In, MarshalAs(UnmanagedType.LPStruct)] Guid key, int value);
        void m19(); void m20();
        [PreserveSig] int SetGUID([In, MarshalAs(UnmanagedType.LPStruct)] Guid key, [In, MarshalAs(UnmanagedType.LPStruct)] Guid value);
    }

    [ComImport, Guid("045fa593-8799-42b8-bc8d-8968c6453507"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IMFMediaBuffer
    {
        [PreserveSig] int Lock(out IntPtr buffer, out int maxLength, out int currentLength);
        [PreserveSig] int Unlock();
        void GetCurrentLength();
        [PreserveSig] int SetCurrentLength(int length);
    }

    [ComImport, Guid("c40a00f2-b93a-4d80-ae8c-5a1c634f58e4"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IMFSample
    {
        void m00(); void m01(); void m02(); void m03(); void m04(); void m05(); void m06(); void m07();
        void m08(); void m09(); void m10(); void m11(); void m12(); void m13(); void m14(); void m15();
        void m16(); void m17(); void m18(); void m19(); void m20(); void m21(); void m22(); void m23();
        void m24(); void m25(); void m26(); void m27(); void m28(); void m29();
        void GetSampleFlags(); void SetSampleFlags(); void GetSampleTime();
        [PreserveSig] int SetSampleTime(long time);
        void GetSampleDuration();
        [PreserveSig] int SetSampleDuration(long duration);
        void GetBufferCount(); void GetBufferByIndex(); void ConvertToContiguousBuffer();
        [PreserveSig] int AddBuffer(IMFMediaBuffer buffer);
    }

    [ComImport, Guid("3137f1cd-fe5e-4805-a5d8-fb477448cb3d"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IMFSinkWriter
    {
        [PreserveSig] int AddStream(IMFMediaType targetType, out int streamIndex);
        [PreserveSig] int SetInputMediaType(int streamIndex, IMFMediaType inputType, IntPtr encodingParameters);
        [PreserveSig] int BeginWriting();
        [PreserveSig] int WriteSample(int streamIndex, IMFSample sample);
        void SendStreamTick(); void PlaceMarker(); void NotifyEndOfSegment(); void Flush();
        [PreserveSig] int Finalize_();
    }

    [DllImport("mfplat.dll")] static extern int MFStartup(int version, int flags);
    [DllImport("mfplat.dll")] static extern int MFShutdown();
    [DllImport("mfplat.dll")] static extern int MFCreateMediaType(out IMFMediaType type);
    [DllImport("mfplat.dll")] static extern int MFCreateMemoryBuffer(int maxLength, out IMFMediaBuffer buffer);
    [DllImport("mfplat.dll")] static extern int MFCreateSample(out IMFSample sample);
    [DllImport("mfreadwrite.dll", CharSet = CharSet.Unicode)]
    static extern int MFCreateSinkWriterFromURL(string url, IntPtr byteStream, IntPtr attributes, out IMFSinkWriter writer);

    static readonly Guid MT_MAJOR_TYPE = new Guid("48eba18e-f8c9-4687-bf11-0a74c9f96a8f");
    static readonly Guid MT_SUBTYPE = new Guid("f7e34c9a-42e8-4714-b74b-cb29d72c35e5");
    static readonly Guid MT_CHANNELS = new Guid("37e48bf5-645e-4c5b-89de-ada9e29b696a");
    static readonly Guid MT_SAMPLES_PER_SECOND = new Guid("5faeeae7-0290-4c31-9e8a-c534f68d9dba");
    static readonly Guid MT_AVG_BYTES_PER_SECOND = new Guid("1aab75c8-cfef-451c-ab95-ac034b8e1731");
    static readonly Guid MT_BLOCK_ALIGNMENT = new Guid("322de230-9eeb-43bd-ab7a-ff412251541d");
    static readonly Guid MT_BITS_PER_SAMPLE = new Guid("f2deb57f-40fa-4764-aa33-ed4f2d1ff669");
    static readonly Guid MEDIATYPE_AUDIO = new Guid("73647561-0000-0010-8000-00aa00389b71");
    static readonly Guid AUDIOFORMAT_PCM = new Guid("00000001-0000-0010-8000-00aa00389b71");
    static readonly Guid AUDIOFORMAT_MP3 = new Guid("00000055-0000-0010-8000-00aa00389b71");

    static void Check(int hr, string what)
    {
        if (hr < 0) throw new InvalidOperationException(what + " failed: 0x" + hr.ToString("X8"));
    }

    static IMFMediaType AudioType(Guid subtype, int sampleRate, int avgBytesPerSecond, bool pcm)
    {
        IMFMediaType t;
        Check(MFCreateMediaType(out t), "MFCreateMediaType");
        Check(t.SetGUID(MT_MAJOR_TYPE, MEDIATYPE_AUDIO), "SetGUID major");
        Check(t.SetGUID(MT_SUBTYPE, subtype), "SetGUID subtype");
        Check(t.SetUINT32(MT_CHANNELS, 1), "SetUINT32 channels");
        Check(t.SetUINT32(MT_SAMPLES_PER_SECOND, sampleRate), "SetUINT32 rate");
        Check(t.SetUINT32(MT_AVG_BYTES_PER_SECOND, avgBytesPerSecond), "SetUINT32 bytes/s");
        if (pcm)
        {
            Check(t.SetUINT32(MT_BLOCK_ALIGNMENT, 2), "SetUINT32 block align");
            Check(t.SetUINT32(MT_BITS_PER_SAMPLE, 16), "SetUINT32 bits");
        }
        return t;
    }

    // Tries the lowest supported mono bitrate first; returns the kbps used.
    public static int Encode(short[] pcm, int sampleRate, string mp3Path)
    {
        Check(MFStartup(0x00020070, 0), "MFStartup");
        try
        {
            int[] kbpsCandidates = { 64, 80, 96, 112, 128 };
            string lastError = "";
            foreach (int kbps in kbpsCandidates)
            {
                if (File.Exists(mp3Path)) File.Delete(mp3Path);
                IMFSinkWriter writer = null;
                try
                {
                    Check(MFCreateSinkWriterFromURL(mp3Path, IntPtr.Zero, IntPtr.Zero, out writer), "MFCreateSinkWriterFromURL");
                    int stream;
                    Check(writer.AddStream(AudioType(AUDIOFORMAT_MP3, sampleRate, kbps * 1000 / 8, false), out stream), "AddStream");
                    Check(writer.SetInputMediaType(stream, AudioType(AUDIOFORMAT_PCM, sampleRate, sampleRate * 2, true), IntPtr.Zero), "SetInputMediaType");
                    Check(writer.BeginWriting(), "BeginWriting");

                    int bytes = pcm.Length * 2;
                    IMFMediaBuffer buffer;
                    Check(MFCreateMemoryBuffer(bytes, out buffer), "MFCreateMemoryBuffer");
                    IntPtr data; int max, cur;
                    Check(buffer.Lock(out data, out max, out cur), "Lock");
                    Marshal.Copy(pcm, 0, data, pcm.Length);
                    Check(buffer.Unlock(), "Unlock");
                    Check(buffer.SetCurrentLength(bytes), "SetCurrentLength");

                    IMFSample sample;
                    Check(MFCreateSample(out sample), "MFCreateSample");
                    Check(sample.AddBuffer(buffer), "AddBuffer");
                    Check(sample.SetSampleTime(0), "SetSampleTime");
                    Check(sample.SetSampleDuration((long)pcm.Length * 10000000L / sampleRate), "SetSampleDuration");
                    Check(writer.WriteSample(stream, sample), "WriteSample");
                    Check(writer.Finalize_(), "Finalize");
                    return kbps;
                }
                catch (InvalidOperationException e) { lastError = e.Message; }
                finally { if (writer != null) Marshal.ReleaseComObject(writer); }
            }
            throw new InvalidOperationException("No supported MP3 bitrate: " + lastError);
        }
        finally { MFShutdown(); }
    }
}
'@

$SampleRate = 44100   # the MP3 encoder takes 32, 44.1 or 48 kHz
$Credits = 'SimonSays (CC0)'
$MiddleDot = [string][char]0x00B7   # Morse dot glyph
$MinusSign = [string][char]0x2212   # Morse dash glyph
$utf8Bom = New-Object System.Text.UTF8Encoding($true)

function Get-Prop($obj, [string]$name) {
    $p = $obj.PSObject.Properties[$name]
    if ($null -eq $p) { return $null }
    return $p.Value
}

function Assert-Value([string]$value, [string]$what, [switch]$StyleText) {
    # Markers that would break the .ssc record (format guide section 11).
    $bad = @('|', '##', '::', "`n", "`r")
    if ($StyleText) { $bad += ';' }
    foreach ($b in $bad) {
        if ($value.Contains($b)) { throw "$what contains a reserved marker ('$b'): $value" }
    }
}

function New-Phrase([string]$icon, [string]$text, [string]$audio) {
    Assert-Value $text "Phrase text"
    $s = if ($icon) { "$icon##$text" } else { $text }
    if ($audio) { $s += "::$audio" }
    return $s
}

function New-SscText([string]$boardStyle, [string]$icon, [string]$name, [string]$style, [string[]]$phrases) {
    Assert-Value $name "Category name"
    if ($name.Contains('=')) { throw "Category name contains '=': $name" }
    $tokens = @()
    if ($style) { $tokens += '$$' + $style }
    $tokens += $phrases
    $sb = New-Object System.Text.StringBuilder
    $null = $sb.Append("SIMONSAYS_CATEGORIES_V1`n")
    $null = $sb.Append('$$board=$$' + $boardStyle + "`n")
    $null = $sb.Append("$icon##$name=" + ($tokens -join '|') + "`n")
    return $sb.ToString()
}

function New-BoardStyle([string]$language, [string]$title) {
    Assert-Value $title "Title" -StyleText
    $s = ''
    if ($language) { $s += "language:$language;" }
    return $s + "title:$title;credits:$Credits;"
}

function ConvertTo-Segments($item, $morseCfg) {
    $freqs = New-Object System.Collections.Generic.List[double[]]
    $durs = New-Object System.Collections.Generic.List[int]
    $decays = New-Object System.Collections.Generic.List[bool]
    $code = Get-Prop $item 'morse'
    if ($code) {
        $f = [double]$morseCfg.frequency; $u = [int]$morseCfg.unitMs
        for ($i = 0; $i -lt $code.Length; $i++) {
            if ($i -gt 0) { $freqs.Add([double[]]@(0)); $durs.Add($u); $decays.Add($false) }
            $len = if ($code[$i] -eq '-') { 3 * $u } else { $u }
            $freqs.Add([double[]]@($f)); $durs.Add($len); $decays.Add($false)
        }
    } else {
        foreach ($seg in $item.tone) {
            $freqs.Add([double[]]@($seg[0] | ForEach-Object { [double]$_ }))
            $durs.Add([int]$seg[1]); $decays.Add($seg[2] -eq 'decay')
        }
    }
    $pcm = [ToneSynth]::Render($freqs.ToArray(), $durs.ToArray(), $decays.ToArray(), $SampleRate, 30)
    $tmp = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString() + '.mp3')
    try {
        $script:Mp3Kbps = [Mp3Encoder]::Encode($pcm, $SampleRate, $tmp)
        return [System.IO.File]::ReadAllBytes($tmp)
    } finally { Remove-Item $tmp -ErrorAction SilentlyContinue }
}

function Write-Ssz([string]$path, [string]$ssc, [hashtable]$audio) {
    if (Test-Path $path) { Remove-Item $path -Force }
    $zip = [System.IO.Compression.ZipFile]::Open($path, [System.IO.Compression.ZipArchiveMode]::Create)
    try {
        $entry = $zip.CreateEntry('categories.ssc', [System.IO.Compression.CompressionLevel]::Optimal)
        $bytes = $utf8Bom.GetPreamble() + $utf8Bom.GetBytes($ssc)
        $s = $entry.Open(); $s.Write($bytes, 0, $bytes.Length); $s.Close()
        foreach ($name in ($audio.Keys | Sort-Object)) {
            $entry = $zip.CreateEntry("resources/$name", [System.IO.Compression.CompressionLevel]::Optimal)
            $s = $entry.Open(); $s.Write($audio[$name], 0, $audio[$name].Length); $s.Close()
        }
    } finally { $zip.Dispose() }
}

$written = 0
foreach ($jsonFile in (Get-ChildItem $dataDir -Filter '*.json' | Sort-Object Name)) {
    $data = [System.IO.File]::ReadAllText($jsonFile.FullName, [System.Text.Encoding]::UTF8) | ConvertFrom-Json

    if ($data.kind -eq 'perLanguage') {
        foreach ($lang in $data.text.PSObject.Properties) {
            $t = $lang.Value
            $phrases = @()
            foreach ($item in $data.items) {
                $expand = Get-Prop $item 'expand'
                if ($expand) {
                    $list = Get-Prop $t $expand
                    if (-not $list -and $expand -eq '_digits') { $list = $data.digits }
                    if (-not $list) { throw "$($jsonFile.Name) [$($lang.Name)]: missing '$expand'" }
                    foreach ($entry in ($list -split ' ' | Where-Object { $_ })) { $phrases += New-Phrase $entry $entry '' }
                    continue
                }
                $text = Get-Prop $t $item.key
                if (-not $text) {
                    if (Get-Prop $item 'optional') { continue }
                    throw "$($jsonFile.Name) [$($lang.Name)]: missing key '$($item.key)'"
                }
                $n = Get-Prop $item 'n'
                if ($null -ne $n) { $text = $text.Replace('{n}', [string]$n) }
                $phrases += New-Phrase $item.icon $text ''
            }
            $ssc = New-SscText (New-BoardStyle $lang.Name $t._title) $data.categoryIcon $t._category $data.style $phrases
            $native = $nativeNames[$lang.Name]
            if (-not $native) { throw "$($jsonFile.Name): '$($lang.Name)' is not in SUPPORTED_LANGUAGES" }
            $folder = Join-Path $OutDir (Get-LanguageFolderName $native)
            $null = New-Item -ItemType Directory -Path $folder -Force
            $fileName = Get-LocalizedFileName $t._title $native
            [System.IO.File]::WriteAllText((Join-Path $folder $fileName), $ssc, $utf8Bom)
            $written++
            Write-Host ("{0,-22} {1,3} phrases -> {2}\{3}" -f $lang.Name, $phrases.Count, (Get-LanguageFolderName $native), $fileName)
        }
    } elseif ($data.kind -eq 'hearingTest') {
        # Same WAV tones in every language; only the labels, the spoken warning
        # and the title change. WAV on purpose: low-bitrate MP3 drops the high
        # tones this check is about.
        $amp = [Math]::Pow(10, [double]$data.amplitudeDb / 20)
        $audio = @{}
        foreach ($f in $data.frequencies) {
            $pcm = [ToneSynth]::Tone([double]$f, [int]$data.toneMs, [int]$data.fadeMs, $amp, $SampleRate)
            foreach ($sd in $data.sides) { $audio["hearing_$($f)_$($sd.suffix).wav"] = [ToneSynth]::Wav($pcm, [int]$sd.side, $SampleRate) }
        }
        foreach ($lang in $data.text.PSObject.Properties) {
            $t = $lang.Value
            $phrases = @(New-Phrase $data.warningIcon $t._warning '')
            foreach ($sd in $data.sides) {
                $sideText = Get-Prop $t $sd.key
                if (-not $sideText) { throw "$($jsonFile.Name) [$($lang.Name)]: missing '$($sd.key)'" }
                foreach ($f in $data.frequencies) { $phrases += New-Phrase $sd.icon "$f Hz $([char]0x00B7) $sideText" "hearing_$($f)_$($sd.suffix).wav" }
            }
            $ssc = New-SscText (New-BoardStyle $lang.Name $t._title) $data.categoryIcon $t._category $data.style $phrases
            $native = $nativeNames[$lang.Name]
            if (-not $native) { throw "$($jsonFile.Name): '$($lang.Name)' is not in SUPPORTED_LANGUAGES" }
            $folder = Join-Path $OutDir (Get-LanguageFolderName $native)
            $null = New-Item -ItemType Directory -Path $folder -Force
            $fileName = (Get-LocalizedFileName $t._title $native) -replace '\.ssc$', '.ssz'
            Write-Ssz (Join-Path $folder $fileName) $ssc $audio
            $written++
            Write-Host ("{0,-22} {1,3} phrases -> {2}\{3}" -f $lang.Name, $phrases.Count, (Get-LanguageFolderName $native), $fileName)
        }
    } elseif ($data.kind -eq 'translatedBundle') {
        # A hand-made board (.ssz) translated into other languages: same style,
        # colors and pictures; only the texts, title/caption, credits and
        # `language` change. The source itself is left untouched.
        $srcPath = Join-Path $repoRoot $data.source
        $zip = [System.IO.Compression.ZipFile]::OpenRead($srcPath)
        $resources = @{}
        try {
            $reader = New-Object System.IO.StreamReader($zip.GetEntry('categories.ssc').Open(), [System.Text.Encoding]::UTF8)
            $srcText = $reader.ReadToEnd(); $reader.Close()
            foreach ($entry in $zip.Entries) {
                if (-not $entry.FullName.StartsWith('resources/') -or $entry.FullName.Length -le 10) { continue }
                $ms = New-Object System.IO.MemoryStream
                $es = $entry.Open(); $es.CopyTo($ms); $es.Close()
                $resources[$entry.FullName.Substring(10)] = $ms.ToArray()
            }
        } finally { $zip.Dispose() }
        $srcLines = @($srcText -split "`r?`n" | Where-Object { $_ })
        $drop = @($data.dropStyleProperties)

        foreach ($lang in $data.text.PSObject.Properties) {
            $t = $lang.Value
            $tr = {
                param([string]$es)
                $v = Get-Prop $t $es
                if (-not $v) { throw "$($jsonFile.Name) [$($lang.Name)]: no translation for '$es'" }
                Assert-Value $v "Translation"
                return $v
            }
            # Style list: drop leftovers; title/caption/credits/language replaced.
            $restyle = {
                param([string]$list, [bool]$board)
                $pairs = @()
                foreach ($pair in ($list -split ';' | Where-Object { $_.Trim() })) {
                    $name = $pair.Split(':')[0].Trim().ToLowerInvariant()
                    if ($drop -contains $name) { continue }
                    if ($board -and ($name -eq 'title' -or $name -eq 'caption')) { $pair = "${name}:$($t._title)" }
                    elseif ($board -and $name -eq 'credits') { $pair = "credits:$($t._credits)" }
                    elseif ($board -and $name -eq 'language') { $pair = "language:$($lang.Name)" }
                    $pairs += $pair
                }
                return ($pairs -join ';') + ';'
            }
            Assert-Value $t._title "Title" -StyleText
            Assert-Value $t._credits "Credits" -StyleText

            $out = New-Object System.Text.StringBuilder
            foreach ($line in $srcLines) {
                if ($line -eq 'SIMONSAYS_CATEGORIES_V1') { $null = $out.Append("$line`n"); continue }
                if ($line.StartsWith('$$board=$$')) {
                    $null = $out.Append('$$board=$$' + (& $restyle $line.Substring(10) $true) + "`n"); continue
                }
                $eq = $line.IndexOf('=')
                $left = $line.Substring(0, $eq); $right = $line.Substring($eq + 1)
                $hash = $left.IndexOf('##')
                $catIcon = if ($hash -ge 0) { $left.Substring(0, $hash + 2) } else { '' }
                $catName = if ($hash -ge 0) { $left.Substring($hash + 2) } else { $left }
                $tokens = @()
                foreach ($tok in ($right -split '\|')) {
                    if (-not $tok) { continue }
                    if ($tok.StartsWith('$$')) { $tokens += '$$' + (& $restyle $tok.Substring(2) $false); continue }
                    $audioPart = ''
                    $ai = $tok.IndexOf('::'); if ($ai -ge 0) { $audioPart = $tok.Substring($ai); $tok = $tok.Substring(0, $ai) }
                    $pi = $tok.IndexOf('##')
                    $pIcon = if ($pi -ge 0) { $tok.Substring(0, $pi + 2) } else { '' }
                    $pText = if ($pi -ge 0) { $tok.Substring($pi + 2) } else { $tok }
                    $tokens += $pIcon + (& $tr $pText) + $audioPart
                }
                $null = $out.Append($catIcon + (& $tr $catName) + '=' + ($tokens -join '|') + "`n")
            }

            $native = $nativeNames[$lang.Name]
            if (-not $native) { throw "$($jsonFile.Name): '$($lang.Name)' is not in SUPPORTED_LANGUAGES" }
            $folder = Join-Path $OutDir (Get-LanguageFolderName $native)
            $null = New-Item -ItemType Directory -Path $folder -Force
            $fileName = ((Remove-InvalidFileNameChars ("$($t._title) ($native)" -replace ' ', '_')) -replace '_{2,}', '_') + '.ssz'
            Write-Ssz (Join-Path $folder $fileName) $out.ToString() $resources
            $written++
            Write-Host ("{0,-22} {1,3} pictures -> {2}\{3}  [{4}]" -f $lang.Name, $resources.Count, (Get-LanguageFolderName $native), $fileName, $data.license)
        }
    } else {
        $phrases = @()
        $audio = @{}
        foreach ($item in $data.items) {
            $file = Get-Prop $item 'file'
            $code = Get-Prop $item 'morse'
            $text = Get-Prop $item 'text'
            $icon = Get-Prop $item 'icon'
            if ($code) {
                $text = $code.Replace('.', $MiddleDot).Replace('-', $MinusSign)
                # The letter itself is the key's icon; a prosign with its own
                # emoji keeps its name in the label instead ("AR .-.-.").
                if ($icon) { $text = (Get-Prop $item 'label') + ' ' + $text }
                else { $icon = Get-Prop $item 'label' }
            }
            if ($file) { $audio[$file] = ConvertTo-Segments $item (Get-Prop $data 'morse') }
            $phrases += New-Phrase $icon ([string]$text) $file
        }
        $ssc = New-SscText (New-BoardStyle '' $data.title) $data.categoryIcon $data.category $data.style $phrases
        if ($audio.Count -gt 0) {
            $fileName = "SimonSays_$($data.file).ssz"
            Write-Ssz (Join-Path $OutDir $fileName) $ssc $audio
        } else {
            $fileName = "SimonSays_$($data.file).ssc"
            [System.IO.File]::WriteAllText((Join-Path $OutDir $fileName), $ssc, $utf8Bom)
        }
        $written++
        Write-Host ("{0,-22} {1,3} phrases -> {2}" -f '(any language)', $phrases.Count, $fileName)
    }
}

Write-Host ""
if ($script:Mp3Kbps) { Write-Host "Audio: MP3, $SampleRate Hz mono, $($script:Mp3Kbps) kbps" }
Write-Host "$written board file(s) written to $OutDir"
