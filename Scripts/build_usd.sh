#!/bin/bash
#
# Build OpenUSD monolithic library with imaging support
#
# Usage:
#   ./Scripts/build_usd.sh           # Build for current platform
#   ./Scripts/build_usd.sh --help    # Show options
#
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Detect platform
case "$(uname -s)" in
    Darwin) PLATFORM="darwin" ;;
    Linux)  PLATFORM="linux" ;;
    *)      echo "ERROR: Unsupported platform: $(uname -s)"; exit 1 ;;
esac

# Default options
WITH_IMAGING=true
JOBS=$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --no-imaging)
            WITH_IMAGING=false
            shift
            ;;
        --jobs|-j)
            JOBS="$2"
            shift 2
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --no-imaging    Skip imaging libraries (faster build)"
            echo "  --jobs, -j N    Number of parallel jobs (default: $JOBS)"
            echo "  --help, -h      Show this help"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

BUILD_DIR="$PROJECT_DIR/Vendor/USD/$PLATFORM"

echo "=============================================="
echo "Building OpenUSD for $PLATFORM"
echo "=============================================="
echo "Output directory: $BUILD_DIR"
echo "Parallel jobs: $JOBS"
echo "With imaging: $WITH_IMAGING"
echo ""

# Check prerequisites
if ! command -v cmake &> /dev/null; then
    echo "ERROR: cmake not found. Please install CMake."
    exit 1
fi

if ! command -v python3 &> /dev/null && ! command -v python &> /dev/null; then
    echo "ERROR: python not found. Please install Python 3."
    exit 1
fi

# Use python3 if available, otherwise python
PYTHON=$(command -v python3 || command -v python)

# Build arguments
BUILD_ARGS=(
    "$BUILD_DIR"
    "--build-monolithic"
    "--no-python"
    "--no-examples"
    "--no-tutorials"
    "--no-docs"
    "--no-tests"
    "-j" "$JOBS"
)

# Add imaging libraries if requested
if [ "$WITH_IMAGING" = true ]; then
    echo "Including imaging libraries (Hydra, Storm, HgiMetal)..."
    BUILD_ARGS+=(
        "--openimageio"
        "--opencolorio"
    )
else
    BUILD_ARGS+=(
        "--no-imaging"
    )
fi

# Platform-specific options
if [ "$PLATFORM" = "darwin" ]; then
    echo "Enabling Metal support..."
    # Metal is enabled by default on macOS
fi

echo ""
echo "Starting build (this may take 1-2 hours with imaging)..."
echo ""

cd "$PROJECT_DIR/OpenUSD"
"$PYTHON" build_scripts/build_usd.py "${BUILD_ARGS[@]}"

echo ""
echo "=============================================="
echo "Build complete!"
echo "=============================================="
echo ""
echo "Library: $BUILD_DIR/lib/libusd_ms.dylib"
echo "Headers: $BUILD_DIR/include/"
echo "Plugins: $BUILD_DIR/lib/usd/"
echo ""
echo "Next steps:"
echo "1. Run 'swift build' to compile SwiftUSD with real USD"
echo "2. Run 'swift test' to verify integration"
