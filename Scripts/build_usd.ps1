#
# Build OpenUSD monolithic library for Windows
#
# Usage:
#   .\Scripts\build_usd.ps1              # Build with imaging
#   .\Scripts\build_usd.ps1 -NoImaging   # Build without imaging (faster)
#
param(
    [switch]$NoImaging,
    [int]$Jobs = 0,
    [switch]$Help
)

$ErrorActionPreference = "Stop"

if ($Help) {
    Write-Host "Usage: .\build_usd.ps1 [OPTIONS]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -NoImaging    Skip imaging libraries (faster build)"
    Write-Host "  -Jobs N       Number of parallel jobs (default: auto)"
    Write-Host "  -Help         Show this help"
    exit 0
}

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectDir = Split-Path -Parent $ScriptDir
$BuildDir = Join-Path $ProjectDir "Vendor\USD\windows"

# Auto-detect job count
if ($Jobs -eq 0) {
    $Jobs = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors
}

Write-Host "=============================================="
Write-Host "Building OpenUSD for Windows"
Write-Host "=============================================="
Write-Host "Output directory: $BuildDir"
Write-Host "Parallel jobs: $Jobs"
Write-Host "With imaging: $(-not $NoImaging)"
Write-Host ""

# Check prerequisites
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: cmake not found. Please install CMake." -ForegroundColor Red
    exit 1
}

if (-not (Get-Command python -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: python not found. Please install Python 3." -ForegroundColor Red
    exit 1
}

# Build arguments
$BuildArgs = @(
    $BuildDir,
    "--build-monolithic",
    "--no-python",
    "--no-examples",
    "--no-tutorials",
    "--no-docs",
    "--no-tests",
    "-j", $Jobs
)

if ($NoImaging) {
    $BuildArgs += "--no-imaging"
} else {
    Write-Host "Including imaging libraries (Hydra, Storm)..."
    $BuildArgs += "--openimageio"
    $BuildArgs += "--opencolorio"
}

Write-Host ""
Write-Host "Starting build (this may take 2-3 hours with imaging)..."
Write-Host ""

Push-Location (Join-Path $ProjectDir "OpenUSD")
try {
    & python build_scripts\build_usd.py @BuildArgs
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed with exit code $LASTEXITCODE"
    }
} finally {
    Pop-Location
}

Write-Host ""
Write-Host "=============================================="
Write-Host "Build complete!"
Write-Host "=============================================="
Write-Host ""
Write-Host "Library: $BuildDir\lib\usd_ms.dll"
Write-Host "Headers: $BuildDir\include\"
Write-Host "Plugins: $BuildDir\lib\usd\"
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Run 'swift build' to compile SwiftUSD with real USD"
Write-Host "2. Run 'swift test' to verify integration"
