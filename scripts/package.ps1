$ErrorActionPreference = 'Stop'
$projectRoot = [IO.Path]::GetFullPath((Split-Path $PSScriptRoot -Parent))
$manifestPath = Join-Path $projectRoot 'mod.json'
$manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
if ($manifest.id -ne 'questbloom' -or $manifest.packageVersion -ne '1.40.8_7379') {
    throw 'Unexpected mod identity or game version.'
}
if ($manifest.modFiles.Count -ne 1 -or $manifest.modFiles[0] -ne 'libquestbloom.so') {
    throw 'Unexpected mod library list.'
}
$libraryPath = Join-Path $projectRoot 'build/libquestbloom.so'
if (-not (Test-Path -LiteralPath $libraryPath)) { throw 'Build the library first.' }
# QMOD is a ZIP with the manifest and library at its root. It contains no game assets.
Add-Type -AssemblyName System.IO.Compression
$outputPath = Join-Path $projectRoot 'QuestBloom.qmod'
$stream = [IO.File]::Open($outputPath, [IO.FileMode]::Create)
try {
    $archive = [IO.Compression.ZipArchive]::new($stream, [IO.Compression.ZipArchiveMode]::Create)
    try {
        foreach ($path in @($manifestPath, $libraryPath, (Join-Path $projectRoot 'THIRD_PARTY_NOTICES.md'))) {
            $entry = $archive.CreateEntry([IO.Path]::GetFileName($path), [IO.Compression.CompressionLevel]::Optimal)
            $entryStream = $entry.Open()
            $inputStream = [IO.File]::OpenRead($path)
            try { $inputStream.CopyTo($entryStream) }
            finally { $inputStream.Dispose(); $entryStream.Dispose() }
        }
    } finally { $archive.Dispose() }
} finally { $stream.Dispose() }
Write-Output "Created $outputPath"
Get-FileHash -LiteralPath $outputPath -Algorithm SHA256
