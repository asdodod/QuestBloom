param(
    [string]$NdkPath = $env:ANDROID_NDK_HOME,
    [string]$NodePath = 'node'
)
$ErrorActionPreference = 'Stop'
if (-not $NdkPath) { throw 'Set ANDROID_NDK_HOME or pass -NdkPath.' }
$projectRoot = [IO.Path]::GetFullPath((Split-Path $PSScriptRoot -Parent))
$outputDirectory = Join-Path $projectRoot 'build/size-validation'
New-Item -ItemType Directory -Path $outputDirectory -Force | Out-Null
$wasm = Join-Path $outputDirectory 'size.wasm'
$compiler = Join-Path $NdkPath 'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
& $compiler --target=wasm32 -std=c++20 -O2 -nostdlib '-Wl,--no-entry' '-Wl,--export-all' `
    "-I$projectRoot/include" (Join-Path $projectRoot 'tests/bloom_size_regression.cpp') -o $wasm
if ($LASTEXITCODE -ne 0) { throw 'Bloom size helper build failed.' }
& $NodePath (Join-Path $projectRoot 'tests/bloom_size_regression.mjs') $wasm
if ($LASTEXITCODE -ne 0) { throw 'Bloom size regression failed.' }
