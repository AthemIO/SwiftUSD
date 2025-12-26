#!/bin/bash
# SwiftUSD Setup Script
# Downloads pre-built USD libraries from GitHub Releases

set -e

# Configuration
REPO="user/SwiftUSD"  # TODO: Update with actual repo
USD_VERSION="v25.11"
SWIFTUSD_VERSION="v0.1.0"

# Detect platform and architecture
OS="$(uname -s)"
ARCH="$(uname -m)"

case "$OS" in
    Darwin)
        PLATFORM="darwin"
        ;;
    Linux)
        PLATFORM="linux"
        ;;
    *)
        echo "Error: Unsupported platform: $OS"
        exit 1
        ;;
esac

case "$ARCH" in
    arm64|aarch64)
        ARCH="arm64"
        ;;
    x86_64)
        ARCH="x86_64"
        ;;
    *)
        echo "Error: Unsupported architecture: $ARCH"
        exit 1
        ;;
esac

ASSET_NAME="usd-${PLATFORM}-${ARCH}-${USD_VERSION}.zip"
DOWNLOAD_URL="https://github.com/${REPO}/releases/download/${SWIFTUSD_VERSION}/${ASSET_NAME}"

# Checksums for verification (SHA256)
# Format: CHECKSUM_<platform>_<arch>
CHECKSUM_darwin_arm64="af0f5e3706173cf924c83441ecb48c7b76e7aed9efb69224dff79d185a3020a3"
CHECKSUM_darwin_x86_64=""
CHECKSUM_linux_arm64=""
CHECKSUM_linux_x86_64=""

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
VENDOR_DIR="$PROJECT_ROOT/Vendor/USD/${PLATFORM}"

# Check if libraries already exist
if [ -d "$VENDOR_DIR/lib" ] && [ "$(ls -A "$VENDOR_DIR/lib" 2>/dev/null)" ]; then
    echo "USD libraries already installed at: $VENDOR_DIR"
    echo "To reinstall, remove the directory first: rm -rf $VENDOR_DIR"
    exit 0
fi

echo "=============================================="
echo "SwiftUSD Setup"
echo "=============================================="
echo "Platform:     $PLATFORM-$ARCH"
echo "USD Version:  $USD_VERSION"
echo "Download URL: $DOWNLOAD_URL"
echo ""

# Create vendor directory
mkdir -p "$PROJECT_ROOT/Vendor/USD"

# Download
TEMP_ZIP="$(mktemp).zip"
echo "Downloading pre-built USD libraries..."

if command -v curl &> /dev/null; then
    curl -L --progress-bar -o "$TEMP_ZIP" "$DOWNLOAD_URL"
elif command -v wget &> /dev/null; then
    wget -q --show-progress -O "$TEMP_ZIP" "$DOWNLOAD_URL"
else
    echo "Error: Neither curl nor wget found. Please install one."
    exit 1
fi

# Verify checksum
CHECKSUM_VAR="CHECKSUM_${PLATFORM}_${ARCH}"
EXPECTED_CHECKSUM="${!CHECKSUM_VAR}"
if [ -n "$EXPECTED_CHECKSUM" ]; then
    echo "Verifying checksum..."
    if command -v shasum &> /dev/null; then
        ACTUAL_CHECKSUM=$(shasum -a 256 "$TEMP_ZIP" | cut -d' ' -f1)
    elif command -v sha256sum &> /dev/null; then
        ACTUAL_CHECKSUM=$(sha256sum "$TEMP_ZIP" | cut -d' ' -f1)
    else
        echo "Warning: Neither shasum nor sha256sum found. Skipping verification."
        ACTUAL_CHECKSUM="$EXPECTED_CHECKSUM"
    fi

    if [ "$ACTUAL_CHECKSUM" != "$EXPECTED_CHECKSUM" ]; then
        echo "Error: Checksum mismatch!"
        echo "  Expected: $EXPECTED_CHECKSUM"
        echo "  Actual:   $ACTUAL_CHECKSUM"
        rm -f "$TEMP_ZIP"
        exit 1
    fi
    echo "Checksum verified."
fi

# Extract
echo "Extracting to $PROJECT_ROOT/Vendor/USD/..."
unzip -q -o "$TEMP_ZIP" -d "$PROJECT_ROOT"

# Cleanup
rm -f "$TEMP_ZIP"

# Verify
if [ -d "$VENDOR_DIR/lib" ]; then
    LIB_COUNT=$(ls "$VENDOR_DIR/lib"/*.dylib 2>/dev/null | wc -l || echo "0")
    echo ""
    echo "Success! Installed $LIB_COUNT libraries to:"
    echo "  $VENDOR_DIR"
    echo ""
    echo "You can now build SwiftUSD with:"
    echo "  swift build"
else
    echo "Error: Installation failed. Library directory not found."
    exit 1
fi
