// Hio.h - Hydra Image I/O for SwiftUSD
// Mirrors: pxr/imaging/hio/image.h, pxr/imaging/hio/types.h
// C++ header with Swift interop annotations
//
// Hio provides image loading and texture support for Hydra.
// Supports common image formats: OpenEXR, PNG, JPEG, TIFF.

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hio/image.h"
#include "pxr/imaging/hio/types.h"
#include "pxr/imaging/hio/imageRegistry.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - HioFormat (Image buffer format enumeration)
// ============================================================================

/// Describes the memory format of image buffers.
///
/// These formats are aligned with HioFormat from Pixar's USD.
/// For reference, see: https://www.khronos.org/registry/vulkan/specs/1.1/html/vkspec.html#VkFormat
enum class HioFormat : int32_t {
    Invalid = -1,

    // UNorm8 - a 1-byte value representing a float between 0 and 1.
    // float value = (unorm / 255.0f);
    UNorm8 = 0,
    UNorm8Vec2,
    UNorm8Vec3,
    UNorm8Vec4,

    // SNorm8 - a 1-byte value representing a float between -1 and 1.
    // float value = max(snorm / 127.0f, -1.0f);
    SNorm8,
    SNorm8Vec2,
    SNorm8Vec3,
    SNorm8Vec4,

    // Float16 - a 2-byte IEEE half-precision float.
    Float16,
    Float16Vec2,
    Float16Vec3,
    Float16Vec4,

    // Float32 - a 4-byte IEEE float.
    Float32,
    Float32Vec2,
    Float32Vec3,
    Float32Vec4,

    // Double64 - an 8-byte IEEE double.
    Double64,
    Double64Vec2,
    Double64Vec3,
    Double64Vec4,

    // UInt16 - a 2-byte unsigned short integer.
    UInt16,
    UInt16Vec2,
    UInt16Vec3,
    UInt16Vec4,

    // Int16 - a 2-byte signed short integer.
    Int16,
    Int16Vec2,
    Int16Vec3,
    Int16Vec4,

    // UInt32 - a 4-byte unsigned integer.
    UInt32,
    UInt32Vec2,
    UInt32Vec3,
    UInt32Vec4,

    // Int32 - a 4-byte signed integer.
    Int32,
    Int32Vec2,
    Int32Vec3,
    Int32Vec4,

    // UNorm8 SRGB - sRGB encoded formats
    UNorm8srgb,
    UNorm8Vec2srgb,
    UNorm8Vec3srgb,
    UNorm8Vec4srgb,

    // BPTC compressed formats
    BC6FloatVec3,      // 3-component, 4x4 blocks, signed floating-point
    BC6UFloatVec3,     // 3-component, 4x4 blocks, unsigned floating-point
    BC7UNorm8Vec4,     // 4-component, 4x4 blocks, unsigned byte
    BC7UNorm8Vec4srgb, // 4-component, 4x4 blocks, unsigned byte, sRGB

    // S3TC/DXT compressed formats
    BC1UNorm8Vec4,     // 4-component, 4x4 blocks, unsigned byte
    BC3UNorm8Vec4,     // 4-component, 4x4 blocks, unsigned byte

    Count
};

// ============================================================================
// MARK: - HioType (Component type enumeration)
// ============================================================================

/// Component data type for image pixels.
enum class HioType : int32_t {
    UnsignedByte = 0,
    UnsignedByteSRGB,
    SignedByte,
    UnsignedShort,
    SignedShort,
    UnsignedInt,
    Int,
    HalfFloat,
    Float,
    Double,

    Count
};

// ============================================================================
// MARK: - HioAddressDimension
// ============================================================================

/// Available texture sampling dimensions.
enum class HioAddressDimension : int32_t {
    U = 0,
    V,
    W
};

// ============================================================================
// MARK: - HioAddressMode
// ============================================================================

/// Various modes used during sampling of a texture.
enum class HioAddressMode : int32_t {
    ClampToEdge = 0,
    MirrorClampToEdge,
    Repeat,
    MirrorRepeat,
    ClampToBorderColor
};

// ============================================================================
// MARK: - HioImageOriginLocation
// ============================================================================

/// Specifies whether to treat the image origin as the upper-left corner
/// or the lower left.
enum class HioImageOriginLocation : int32_t {
    UpperLeft = 0,
    LowerLeft
};

// ============================================================================
// MARK: - HioSourceColorSpace
// ============================================================================

/// Specifies the source color space in which the texture is encoded.
/// "Auto" indicates the texture reader should determine color space based
/// on hints from the image (e.g. file type, number of channels, image metadata).
enum class HioSourceColorSpace : int32_t {
    Raw = 0,
    SRGB,
    Auto
};

// ============================================================================
// MARK: - Utility Functions (defined before classes that use them)
// ============================================================================

/// Get the HioType corresponding to the given HioFormat.
inline HioType HioGetType(HioFormat format) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return static_cast<HioType>(PXR_NS::HioGetHioType(static_cast<PXR_NS::HioFormat>(format)));
#else
    switch (format) {
        case HioFormat::UNorm8:
        case HioFormat::UNorm8Vec2:
        case HioFormat::UNorm8Vec3:
        case HioFormat::UNorm8Vec4:
            return HioType::UnsignedByte;

        case HioFormat::UNorm8srgb:
        case HioFormat::UNorm8Vec2srgb:
        case HioFormat::UNorm8Vec3srgb:
        case HioFormat::UNorm8Vec4srgb:
            return HioType::UnsignedByteSRGB;

        case HioFormat::SNorm8:
        case HioFormat::SNorm8Vec2:
        case HioFormat::SNorm8Vec3:
        case HioFormat::SNorm8Vec4:
            return HioType::SignedByte;

        case HioFormat::Float16:
        case HioFormat::Float16Vec2:
        case HioFormat::Float16Vec3:
        case HioFormat::Float16Vec4:
            return HioType::HalfFloat;

        case HioFormat::Float32:
        case HioFormat::Float32Vec2:
        case HioFormat::Float32Vec3:
        case HioFormat::Float32Vec4:
            return HioType::Float;

        case HioFormat::Double64:
        case HioFormat::Double64Vec2:
        case HioFormat::Double64Vec3:
        case HioFormat::Double64Vec4:
            return HioType::Double;

        case HioFormat::UInt16:
        case HioFormat::UInt16Vec2:
        case HioFormat::UInt16Vec3:
        case HioFormat::UInt16Vec4:
            return HioType::UnsignedShort;

        case HioFormat::Int16:
        case HioFormat::Int16Vec2:
        case HioFormat::Int16Vec3:
        case HioFormat::Int16Vec4:
            return HioType::SignedShort;

        case HioFormat::UInt32:
        case HioFormat::UInt32Vec2:
        case HioFormat::UInt32Vec3:
        case HioFormat::UInt32Vec4:
            return HioType::UnsignedInt;

        case HioFormat::Int32:
        case HioFormat::Int32Vec2:
        case HioFormat::Int32Vec3:
        case HioFormat::Int32Vec4:
            return HioType::Int;

        default:
            return HioType::UnsignedByte;
    }
#endif
}

/// Get the number of components (channels) in the given HioFormat.
inline int HioGetComponentCount(HioFormat format) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::HioGetComponentCount(static_cast<PXR_NS::HioFormat>(format));
#else
    switch (format) {
        case HioFormat::UNorm8:
        case HioFormat::SNorm8:
        case HioFormat::Float16:
        case HioFormat::Float32:
        case HioFormat::Double64:
        case HioFormat::UInt16:
        case HioFormat::Int16:
        case HioFormat::UInt32:
        case HioFormat::Int32:
        case HioFormat::UNorm8srgb:
            return 1;

        case HioFormat::UNorm8Vec2:
        case HioFormat::SNorm8Vec2:
        case HioFormat::Float16Vec2:
        case HioFormat::Float32Vec2:
        case HioFormat::Double64Vec2:
        case HioFormat::UInt16Vec2:
        case HioFormat::Int16Vec2:
        case HioFormat::UInt32Vec2:
        case HioFormat::Int32Vec2:
        case HioFormat::UNorm8Vec2srgb:
            return 2;

        case HioFormat::UNorm8Vec3:
        case HioFormat::SNorm8Vec3:
        case HioFormat::Float16Vec3:
        case HioFormat::Float32Vec3:
        case HioFormat::Double64Vec3:
        case HioFormat::UInt16Vec3:
        case HioFormat::Int16Vec3:
        case HioFormat::UInt32Vec3:
        case HioFormat::Int32Vec3:
        case HioFormat::UNorm8Vec3srgb:
        case HioFormat::BC6FloatVec3:
        case HioFormat::BC6UFloatVec3:
            return 3;

        case HioFormat::UNorm8Vec4:
        case HioFormat::SNorm8Vec4:
        case HioFormat::Float16Vec4:
        case HioFormat::Float32Vec4:
        case HioFormat::Double64Vec4:
        case HioFormat::UInt16Vec4:
        case HioFormat::Int16Vec4:
        case HioFormat::UInt32Vec4:
        case HioFormat::Int32Vec4:
        case HioFormat::UNorm8Vec4srgb:
        case HioFormat::BC7UNorm8Vec4:
        case HioFormat::BC7UNorm8Vec4srgb:
        case HioFormat::BC1UNorm8Vec4:
        case HioFormat::BC3UNorm8Vec4:
            return 4;

        default:
            return 0;
    }
#endif
}

/// Get the size in bytes of a single component (channel) for the given HioType.
inline size_t HioGetDataSizeOfType(HioType type) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::HioGetDataSizeOfType(static_cast<PXR_NS::HioType>(type));
#else
    switch (type) {
        case HioType::UnsignedByte:
        case HioType::UnsignedByteSRGB:
        case HioType::SignedByte:
            return 1;
        case HioType::UnsignedShort:
        case HioType::SignedShort:
        case HioType::HalfFloat:
            return 2;
        case HioType::UnsignedInt:
        case HioType::Int:
        case HioType::Float:
            return 4;
        case HioType::Double:
            return 8;
        default:
            return 0;
    }
#endif
}

/// Check if the given format is compressed.
inline bool HioIsCompressed(HioFormat format) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::HioIsCompressed(static_cast<PXR_NS::HioFormat>(format));
#else
    switch (format) {
        case HioFormat::BC6FloatVec3:
        case HioFormat::BC6UFloatVec3:
        case HioFormat::BC7UNorm8Vec4:
        case HioFormat::BC7UNorm8Vec4srgb:
        case HioFormat::BC1UNorm8Vec4:
        case HioFormat::BC3UNorm8Vec4:
            return true;
        default:
            return false;
    }
#endif
}

/// Get the size in bytes per pixel for the given HioFormat.
inline size_t HioGetDataSizeOfFormat(HioFormat format) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::HioGetDataSizeOfFormat(static_cast<PXR_NS::HioFormat>(format));
#else
    // Handle compressed formats
    switch (format) {
        case HioFormat::BC6FloatVec3:
        case HioFormat::BC6UFloatVec3:
        case HioFormat::BC7UNorm8Vec4:
        case HioFormat::BC7UNorm8Vec4srgb:
        case HioFormat::BC3UNorm8Vec4:
            return 16;  // 16 bytes per 4x4 block
        case HioFormat::BC1UNorm8Vec4:
            return 8;   // 8 bytes per 4x4 block
        default:
            break;
    }

    int components = HioGetComponentCount(format);
    HioType type = HioGetType(format);

    return components * HioGetDataSizeOfType(type);
#endif
}

/// Calculate the byte size of texture data. If compressed, takes block size into account.
inline size_t HioGetDataSize(HioFormat format, int width, int height, int depth = 1) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return PXR_NS::HioGetDataSize(static_cast<PXR_NS::HioFormat>(format),
                                   PXR_NS::GfVec3i(width, height, depth));
#else
    if (HioIsCompressed(format)) {
        // Compressed formats use 4x4 blocks
        int blocksX = (width + 3) / 4;
        int blocksY = (height + 3) / 4;
        size_t blockSize = HioGetDataSizeOfFormat(format);
        return blocksX * blocksY * depth * blockSize;
    }

    return width * height * depth * HioGetDataSizeOfFormat(format);
#endif
}

/// Get the HioFormat for a given number of channels and type.
inline HioFormat HioGetFormat(uint32_t nchannels, HioType type, bool isSRGB) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    return static_cast<HioFormat>(PXR_NS::HioGetFormat(
        nchannels, static_cast<PXR_NS::HioType>(type), isSRGB));
#else
    // Standalone implementation
    if (nchannels == 0 || nchannels > 4) return HioFormat::Invalid;

    if (isSRGB && type == HioType::UnsignedByte) {
        switch (nchannels) {
            case 1: return HioFormat::UNorm8srgb;
            case 2: return HioFormat::UNorm8Vec2srgb;
            case 3: return HioFormat::UNorm8Vec3srgb;
            case 4: return HioFormat::UNorm8Vec4srgb;
            default: return HioFormat::Invalid;
        }
    }

    switch (type) {
        case HioType::UnsignedByte:
        case HioType::UnsignedByteSRGB:
            switch (nchannels) {
                case 1: return HioFormat::UNorm8;
                case 2: return HioFormat::UNorm8Vec2;
                case 3: return HioFormat::UNorm8Vec3;
                case 4: return HioFormat::UNorm8Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::SignedByte:
            switch (nchannels) {
                case 1: return HioFormat::SNorm8;
                case 2: return HioFormat::SNorm8Vec2;
                case 3: return HioFormat::SNorm8Vec3;
                case 4: return HioFormat::SNorm8Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::HalfFloat:
            switch (nchannels) {
                case 1: return HioFormat::Float16;
                case 2: return HioFormat::Float16Vec2;
                case 3: return HioFormat::Float16Vec3;
                case 4: return HioFormat::Float16Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::Float:
            switch (nchannels) {
                case 1: return HioFormat::Float32;
                case 2: return HioFormat::Float32Vec2;
                case 3: return HioFormat::Float32Vec3;
                case 4: return HioFormat::Float32Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::Double:
            switch (nchannels) {
                case 1: return HioFormat::Double64;
                case 2: return HioFormat::Double64Vec2;
                case 3: return HioFormat::Double64Vec3;
                case 4: return HioFormat::Double64Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::UnsignedShort:
            switch (nchannels) {
                case 1: return HioFormat::UInt16;
                case 2: return HioFormat::UInt16Vec2;
                case 3: return HioFormat::UInt16Vec3;
                case 4: return HioFormat::UInt16Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::SignedShort:
            switch (nchannels) {
                case 1: return HioFormat::Int16;
                case 2: return HioFormat::Int16Vec2;
                case 3: return HioFormat::Int16Vec3;
                case 4: return HioFormat::Int16Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::UnsignedInt:
            switch (nchannels) {
                case 1: return HioFormat::UInt32;
                case 2: return HioFormat::UInt32Vec2;
                case 3: return HioFormat::UInt32Vec3;
                case 4: return HioFormat::UInt32Vec4;
                default: return HioFormat::Invalid;
            }
        case HioType::Int:
            switch (nchannels) {
                case 1: return HioFormat::Int32;
                case 2: return HioFormat::Int32Vec2;
                case 3: return HioFormat::Int32Vec3;
                case 4: return HioFormat::Int32Vec4;
                default: return HioFormat::Invalid;
            }
        default:
            return HioFormat::Invalid;
    }
#endif
}

// ============================================================================
// MARK: - Forward declarations
// ============================================================================

class HioImageRef;

// ============================================================================
// MARK: - HioStorageSpec (Image storage specification)
// ============================================================================

/// Describes the memory layout and storage of a texture image.
struct HioStorageSpec {
    /// Width of the image in pixels.
    int width;

    /// Height of the image in pixels.
    int height;

    /// Depth of the image (for 3D textures).
    int depth;

    /// Pixel format of the image.
    HioFormat format;

    /// Whether the image is vertically flipped.
    bool flipped;

    /// Pointer to the image data buffer.
    void* data;

    HioStorageSpec() SWIFTUSD_NOEXCEPT
        : width(0),
          height(0),
          depth(0),
          format(HioFormat::Invalid),
          flipped(false),
          data(nullptr) {}

    HioStorageSpec(int w, int h, HioFormat fmt) SWIFTUSD_NOEXCEPT
        : width(w),
          height(h),
          depth(1),
          format(fmt),
          flipped(false),
          data(nullptr) {}

    HioStorageSpec(int w, int h, int d, HioFormat fmt) SWIFTUSD_NOEXCEPT
        : width(w),
          height(h),
          depth(d),
          format(fmt),
          flipped(false),
          data(nullptr) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HioImageRef (Image handle)
// ============================================================================

/// A reference to an image file for reading or writing.
///
/// HioImageRef provides the main interface for loading and saving
/// texture images in various formats (PNG, JPEG, OpenEXR, TIFF).
///
/// Usage:
/// ```swift
/// // Open an image for reading
/// if let image = HioImageRef.OpenForReading("/path/to/texture.png") {
///     print("Image size: \(image.GetWidth()) x \(image.GetHeight())")
///     print("Format: \(image.GetFormat())")
/// }
///
/// // Check if a file is a supported image format
/// let supported = HioImageRef.IsSupportedImageFile("texture.exr")
/// ```
class HioImageRef {
public:
    // ========================================================================
    // MARK: - Factory Methods
    // ========================================================================

    /// Check whether a filename is a supported image file format.
    static bool IsSupportedImageFile(const char* filename) SWIFTUSD_NOEXCEPT {
        if (!filename) return false;
#if defined(USE_PIXAR_USD)
        return PXR_NS::HioImage::IsSupportedImageFile(std::string(filename));
#else
        // Standalone: check common extensions
        std::string path(filename);
        size_t dot = path.rfind('.');
        if (dot == std::string::npos) return false;

        std::string ext = path.substr(dot);
        // Convert to lowercase for comparison
        for (auto& c : ext) {
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        }

        return ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
               ext == ".exr" || ext == ".tif" || ext == ".tiff" ||
               ext == ".hdr" || ext == ".bmp" || ext == ".tga";
#endif
    }

    /// Open an image file for reading.
    ///
    /// Parameters:
    ///   - filename: Path to the image file
    ///   - subimage: Subimage index (for multi-part images like EXR)
    ///   - mip: Mip level to read
    ///   - colorSpace: Color space hint for interpreting the texture
    ///   - suppressErrors: If true, don't log errors for missing files
    static HioImageRef* OpenForReading(
        const char* filename,
        int subimage = 0,
        int mip = 0,
        HioSourceColorSpace colorSpace = HioSourceColorSpace::Auto,
        bool suppressErrors = false) SWIFTUSD_NOEXCEPT
    {
        if (!filename) return nullptr;
#if defined(USE_PIXAR_USD)
        auto pxrColorSpace = static_cast<PXR_NS::HioImage::SourceColorSpace>(colorSpace);
        auto image = PXR_NS::HioImage::OpenForReading(
            std::string(filename), subimage, mip, pxrColorSpace, suppressErrors);
        if (!image) return nullptr;
        return new HioImageRef(image);
#else
        // Standalone implementation
        auto* ref = new HioImageRef(std::string(filename));
        ref->_subimage = subimage;
        ref->_mip = mip;
        ref->_colorSpace = colorSpace;
        ref->_isForReading = true;
        return ref;
#endif
    }

    /// Open an image file for writing.
    static HioImageRef* OpenForWriting(const char* filename) SWIFTUSD_NOEXCEPT {
        if (!filename) return nullptr;
#if defined(USE_PIXAR_USD)
        auto image = PXR_NS::HioImage::OpenForWriting(std::string(filename));
        if (!image) return nullptr;
        return new HioImageRef(image);
#else
        auto* ref = new HioImageRef(std::string(filename));
        ref->_isForReading = false;
        return ref;
#endif
    }

    // ========================================================================
    // MARK: - Image Properties
    // ========================================================================

    /// Get the filename of this image.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetFilename() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_image) {
            _filenameCache = _image->GetFilename();
            return _filenameCache.c_str();
        }
        return "";
#else
        return _filename.c_str();
#endif
    }

    /// Get the width of the image in pixels.
    int GetWidth() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _image ? _image->GetWidth() : 0;
#else
        return _width;
#endif
    }

    /// Get the height of the image in pixels.
    int GetHeight() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _image ? _image->GetHeight() : 0;
#else
        return _height;
#endif
    }

    /// Get the pixel format of the image.
    HioFormat GetFormat() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _image ? static_cast<HioFormat>(_image->GetFormat()) : HioFormat::Invalid;
#else
        return _format;
#endif
    }

    /// Get the number of bytes per pixel.
    int GetBytesPerPixel() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _image ? _image->GetBytesPerPixel() : 0;
#else
        return static_cast<int>(HioGetDataSizeOfFormat(_format));
#endif
    }

    /// Get the number of mip levels available.
    int GetNumMipLevels() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _image ? _image->GetNumMipLevels() : 1;
#else
        return _numMipLevels;
#endif
    }

    /// Check whether the image is in sRGB color space.
    bool IsColorSpaceSRGB() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _image ? _image->IsColorSpaceSRGB() : false;
#else
        return _isSRGB;
#endif
    }

    // ========================================================================
    // MARK: - Reading
    // ========================================================================

    /// Read the image into the provided storage specification.
    /// The storage.data pointer must point to a buffer large enough
    /// to hold the image data.
    bool Read(HioStorageSpec& storage) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_image) return false;
        PXR_NS::HioImage::StorageSpec pxrStorage;
        pxrStorage.width = storage.width;
        pxrStorage.height = storage.height;
        pxrStorage.depth = storage.depth;
        pxrStorage.format = static_cast<PXR_NS::HioFormat>(storage.format);
        pxrStorage.flipped = storage.flipped;
        pxrStorage.data = storage.data;
        return _image->Read(pxrStorage);
#else
        // Standalone: simulate successful read
        return storage.data != nullptr;
#endif
    }

    /// Read a cropped portion of the image.
    bool ReadCropped(int cropTop, int cropBottom, int cropLeft, int cropRight,
                     HioStorageSpec& storage) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_image) return false;
        PXR_NS::HioImage::StorageSpec pxrStorage;
        pxrStorage.width = storage.width;
        pxrStorage.height = storage.height;
        pxrStorage.depth = storage.depth;
        pxrStorage.format = static_cast<PXR_NS::HioFormat>(storage.format);
        pxrStorage.flipped = storage.flipped;
        pxrStorage.data = storage.data;
        return _image->ReadCropped(cropTop, cropBottom, cropLeft, cropRight, pxrStorage);
#else
        // Standalone: simulate successful read
        (void)cropTop; (void)cropBottom; (void)cropLeft; (void)cropRight;
        return storage.data != nullptr;
#endif
    }

    // ========================================================================
    // MARK: - Writing
    // ========================================================================

    /// Write image data to the file.
    bool Write(const HioStorageSpec& storage) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_image) return false;
        PXR_NS::HioImage::StorageSpec pxrStorage;
        pxrStorage.width = storage.width;
        pxrStorage.height = storage.height;
        pxrStorage.depth = storage.depth;
        pxrStorage.format = static_cast<PXR_NS::HioFormat>(storage.format);
        pxrStorage.flipped = storage.flipped;
        pxrStorage.data = const_cast<void*>(storage.data);
        return _image->Write(pxrStorage);
#else
        // Standalone: simulate successful write
        return storage.data != nullptr;
#endif
    }

    // ========================================================================
    // MARK: - Sampler Metadata
    // ========================================================================

    /// Get the address mode for a specific dimension from image metadata.
    bool GetSamplerAddressMode(HioAddressDimension dim, HioAddressMode& mode) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_image) return false;
        PXR_NS::HioAddressMode pxrMode;
        if (_image->GetSamplerMetadata(static_cast<PXR_NS::HioAddressDimension>(dim), &pxrMode)) {
            mode = static_cast<HioAddressMode>(pxrMode);
            return true;
        }
        return false;
#else
        mode = HioAddressMode::Repeat;
        (void)dim;
        return true;
#endif
    }

    // ========================================================================
    // MARK: - Reference Counting
    // ========================================================================

    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    explicit HioImageRef(PXR_NS::HioImageSharedPtr image) SWIFTUSD_NOEXCEPT
        : _image(image), _refCount(1) {}

    PXR_NS::HioImageSharedPtr _image;
    mutable std::string _filenameCache;
#else
    explicit HioImageRef(const std::string& filename) SWIFTUSD_NOEXCEPT
        : _filename(filename),
          _width(256),
          _height(256),
          _format(HioFormat::UNorm8Vec4),
          _numMipLevels(1),
          _isSRGB(false),
          _subimage(0),
          _mip(0),
          _colorSpace(HioSourceColorSpace::Auto),
          _isForReading(true),
          _refCount(1) {}

    std::string _filename;
    int _width;
    int _height;
    HioFormat _format;
    int _numMipLevels;
    bool _isSRGB;
    int _subimage;
    int _mip;
    HioSourceColorSpace _colorSpace;
    bool _isForReading;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
