param(
    [string]$NdkPath = $env:ANDROID_NDK_HOME,
    [string]$DependenciesPath = '',
    [string]$CMakePath = 'cmake',
    [string]$NinjaPath = 'ninja'
)
$ErrorActionPreference = 'Stop'
$projectRoot = [IO.Path]::GetFullPath((Split-Path $PSScriptRoot -Parent))
if (-not $NdkPath) { throw 'Pass -NdkPath or set ANDROID_NDK_HOME to your Android NDK r27 folder.' }
if (-not $DependenciesPath) { $DependenciesPath = Join-Path $projectRoot 'extern' }
if (-not (Test-Path -LiteralPath (Join-Path $DependenciesPath 'includes/bs-cordl/include'))) {
    throw 'Dependencies are missing. Run qpm restore in this project first.'
}
& $CMakePath -S $projectRoot -B (Join-Path $projectRoot 'build') -G Ninja `
    "-DCMAKE_MAKE_PROGRAM=$NinjaPath" "-DCMAKE_ANDROID_NDK=$NdkPath" `
    "-DEXTERN_DIR=$DependenciesPath" -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed.' }
& $CMakePath --build (Join-Path $projectRoot 'build') -j 4
if ($LASTEXITCODE -ne 0) { throw 'Native build failed.' }
