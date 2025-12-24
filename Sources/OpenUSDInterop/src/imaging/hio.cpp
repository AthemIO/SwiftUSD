// hio.cpp - Hio C wrapper implementation
// Mirrors: pxr/imaging/hio/*.h (Image loading, OpenEXR, texture formats)

#include "../../include/imaging/hio.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define HIO_USE_USD 1
    #include <pxr/imaging/hio/image.h>
    #include <pxr/imaging/hio/imageRegistry.h>
    #include <pxr/imaging/hio/glslfx.h>
    #include <pxr/imaging/hio/fieldTextureData.h>
    #include <pxr/imaging/hio/types.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define HIO_USE_USD 0
#endif

#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <atomic>

// Helper to duplicate a std::string as a C string
static char* DuplicateString(const std::string& str) {
    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

// ============================================================================
// MARK: - HioImage Wrapper
// ============================================================================

struct HioImageOpaque {
    std::atomic<int> refCount;
    std::string filename;
    int width;
    int height;
    int depth;
    HioFormat format;
    int bytesPerPixel;
    int numMipLevels;
    bool isSRGB;
    bool isForWriting;
#if HIO_USE_USD
    HioImageSharedPtr image;
#endif

    HioImageOpaque() : refCount(1), width(0), height(0), depth(1),
                       format(HIO_FORMAT_UNORM8_VEC4), bytesPerPixel(4),
                       numMipLevels(1), isSRGB(false), isForWriting(false) {}
};

// ============================================================================
// MARK: - HioGlslfx Wrapper
// ============================================================================

struct HioGlslfxOpaque {
    std::atomic<int> refCount;
    bool isValid;
    std::string surfaceSource;
    std::string displacementSource;
    std::string volumeSource;
    std::vector<std::string> parameterNames;
    std::vector<std::string> textureNames;
    std::vector<std::string> attributeNames;
    size_t hash;
#if HIO_USE_USD
    std::unique_ptr<HioGlslfx> glslfx;
#endif

    HioGlslfxOpaque() : refCount(1), isValid(false), hash(0) {}
};

// ============================================================================
// MARK: - HioFieldTextureData Wrapper
// ============================================================================

struct HioFieldTextureDataOpaque {
    std::atomic<int> refCount;
    std::string filePath;
    std::string fieldName;
    HioFormat format;
    int width;
    int height;
    int depth;
    double bboxMin[3];
    double bboxMax[3];
    std::vector<uint8_t> rawBuffer;
#if HIO_USE_USD
    HioFieldTextureDataSharedPtr fieldData;
#endif

    HioFieldTextureDataOpaque() : refCount(1), format(HIO_FORMAT_FLOAT32),
                                   width(0), height(0), depth(0) {
        bboxMin[0] = bboxMin[1] = bboxMin[2] = 0.0;
        bboxMax[0] = bboxMax[1] = bboxMax[2] = 1.0;
    }
};

// ============================================================================
// MARK: - Format Conversion Helpers
// ============================================================================

#if HIO_USE_USD
static HioFormat ConvertFromPxrFormat(pxr::HioFormat pxrFormat) {
    // Map pxr::HioFormat to our HioFormat enum
    switch (pxrFormat) {
        case pxr::HioFormatUNorm8: return HIO_FORMAT_UNORM8;
        case pxr::HioFormatUNorm8Vec2: return HIO_FORMAT_UNORM8_VEC2;
        case pxr::HioFormatUNorm8Vec3: return HIO_FORMAT_UNORM8_VEC3;
        case pxr::HioFormatUNorm8Vec4: return HIO_FORMAT_UNORM8_VEC4;
        case pxr::HioFormatSNorm8: return HIO_FORMAT_SNORM8;
        case pxr::HioFormatSNorm8Vec2: return HIO_FORMAT_SNORM8_VEC2;
        case pxr::HioFormatSNorm8Vec3: return HIO_FORMAT_SNORM8_VEC3;
        case pxr::HioFormatSNorm8Vec4: return HIO_FORMAT_SNORM8_VEC4;
        case pxr::HioFormatFloat16: return HIO_FORMAT_FLOAT16;
        case pxr::HioFormatFloat16Vec2: return HIO_FORMAT_FLOAT16_VEC2;
        case pxr::HioFormatFloat16Vec3: return HIO_FORMAT_FLOAT16_VEC3;
        case pxr::HioFormatFloat16Vec4: return HIO_FORMAT_FLOAT16_VEC4;
        case pxr::HioFormatFloat32: return HIO_FORMAT_FLOAT32;
        case pxr::HioFormatFloat32Vec2: return HIO_FORMAT_FLOAT32_VEC2;
        case pxr::HioFormatFloat32Vec3: return HIO_FORMAT_FLOAT32_VEC3;
        case pxr::HioFormatFloat32Vec4: return HIO_FORMAT_FLOAT32_VEC4;
        case pxr::HioFormatDouble64: return HIO_FORMAT_DOUBLE64;
        case pxr::HioFormatDouble64Vec2: return HIO_FORMAT_DOUBLE64_VEC2;
        case pxr::HioFormatDouble64Vec3: return HIO_FORMAT_DOUBLE64_VEC3;
        case pxr::HioFormatDouble64Vec4: return HIO_FORMAT_DOUBLE64_VEC4;
        case pxr::HioFormatUInt16: return HIO_FORMAT_UINT16;
        case pxr::HioFormatUInt16Vec2: return HIO_FORMAT_UINT16_VEC2;
        case pxr::HioFormatUInt16Vec3: return HIO_FORMAT_UINT16_VEC3;
        case pxr::HioFormatUInt16Vec4: return HIO_FORMAT_UINT16_VEC4;
        case pxr::HioFormatInt16: return HIO_FORMAT_INT16;
        case pxr::HioFormatInt16Vec2: return HIO_FORMAT_INT16_VEC2;
        case pxr::HioFormatInt16Vec3: return HIO_FORMAT_INT16_VEC3;
        case pxr::HioFormatInt16Vec4: return HIO_FORMAT_INT16_VEC4;
        case pxr::HioFormatUInt32: return HIO_FORMAT_UINT32;
        case pxr::HioFormatUInt32Vec2: return HIO_FORMAT_UINT32_VEC2;
        case pxr::HioFormatUInt32Vec3: return HIO_FORMAT_UINT32_VEC3;
        case pxr::HioFormatUInt32Vec4: return HIO_FORMAT_UINT32_VEC4;
        case pxr::HioFormatInt32: return HIO_FORMAT_INT32;
        case pxr::HioFormatInt32Vec2: return HIO_FORMAT_INT32_VEC2;
        case pxr::HioFormatInt32Vec3: return HIO_FORMAT_INT32_VEC3;
        case pxr::HioFormatInt32Vec4: return HIO_FORMAT_INT32_VEC4;
        case pxr::HioFormatUNorm8srgb: return HIO_FORMAT_UNORM8_SRGB;
        case pxr::HioFormatUNorm8Vec2srgb: return HIO_FORMAT_UNORM8_VEC2_SRGB;
        case pxr::HioFormatUNorm8Vec3srgb: return HIO_FORMAT_UNORM8_VEC3_SRGB;
        case pxr::HioFormatUNorm8Vec4srgb: return HIO_FORMAT_UNORM8_VEC4_SRGB;
        case pxr::HioFormatBC6FloatVec3: return HIO_FORMAT_BC6_FLOAT_VEC3;
        case pxr::HioFormatBC6UFloatVec3: return HIO_FORMAT_BC6_UFLOAT_VEC3;
        case pxr::HioFormatBC7UNorm8Vec4: return HIO_FORMAT_BC7_UNORM8_VEC4;
        case pxr::HioFormatBC7UNorm8Vec4srgb: return HIO_FORMAT_BC7_UNORM8_VEC4_SRGB;
        case pxr::HioFormatBC1UNorm8Vec4: return HIO_FORMAT_BC1_UNORM8_VEC4;
        case pxr::HioFormatBC3UNorm8Vec4: return HIO_FORMAT_BC3_UNORM8_VEC4;
        default: return HIO_FORMAT_INVALID;
    }
}

static pxr::HioFormat ConvertToPxrFormat(HioFormat format) {
    switch (format) {
        case HIO_FORMAT_UNORM8: return pxr::HioFormatUNorm8;
        case HIO_FORMAT_UNORM8_VEC2: return pxr::HioFormatUNorm8Vec2;
        case HIO_FORMAT_UNORM8_VEC3: return pxr::HioFormatUNorm8Vec3;
        case HIO_FORMAT_UNORM8_VEC4: return pxr::HioFormatUNorm8Vec4;
        case HIO_FORMAT_SNORM8: return pxr::HioFormatSNorm8;
        case HIO_FORMAT_SNORM8_VEC2: return pxr::HioFormatSNorm8Vec2;
        case HIO_FORMAT_SNORM8_VEC3: return pxr::HioFormatSNorm8Vec3;
        case HIO_FORMAT_SNORM8_VEC4: return pxr::HioFormatSNorm8Vec4;
        case HIO_FORMAT_FLOAT16: return pxr::HioFormatFloat16;
        case HIO_FORMAT_FLOAT16_VEC2: return pxr::HioFormatFloat16Vec2;
        case HIO_FORMAT_FLOAT16_VEC3: return pxr::HioFormatFloat16Vec3;
        case HIO_FORMAT_FLOAT16_VEC4: return pxr::HioFormatFloat16Vec4;
        case HIO_FORMAT_FLOAT32: return pxr::HioFormatFloat32;
        case HIO_FORMAT_FLOAT32_VEC2: return pxr::HioFormatFloat32Vec2;
        case HIO_FORMAT_FLOAT32_VEC3: return pxr::HioFormatFloat32Vec3;
        case HIO_FORMAT_FLOAT32_VEC4: return pxr::HioFormatFloat32Vec4;
        case HIO_FORMAT_DOUBLE64: return pxr::HioFormatDouble64;
        case HIO_FORMAT_DOUBLE64_VEC2: return pxr::HioFormatDouble64Vec2;
        case HIO_FORMAT_DOUBLE64_VEC3: return pxr::HioFormatDouble64Vec3;
        case HIO_FORMAT_DOUBLE64_VEC4: return pxr::HioFormatDouble64Vec4;
        case HIO_FORMAT_UINT16: return pxr::HioFormatUInt16;
        case HIO_FORMAT_UINT16_VEC2: return pxr::HioFormatUInt16Vec2;
        case HIO_FORMAT_UINT16_VEC3: return pxr::HioFormatUInt16Vec3;
        case HIO_FORMAT_UINT16_VEC4: return pxr::HioFormatUInt16Vec4;
        case HIO_FORMAT_INT16: return pxr::HioFormatInt16;
        case HIO_FORMAT_INT16_VEC2: return pxr::HioFormatInt16Vec2;
        case HIO_FORMAT_INT16_VEC3: return pxr::HioFormatInt16Vec3;
        case HIO_FORMAT_INT16_VEC4: return pxr::HioFormatInt16Vec4;
        case HIO_FORMAT_UINT32: return pxr::HioFormatUInt32;
        case HIO_FORMAT_UINT32_VEC2: return pxr::HioFormatUInt32Vec2;
        case HIO_FORMAT_UINT32_VEC3: return pxr::HioFormatUInt32Vec3;
        case HIO_FORMAT_UINT32_VEC4: return pxr::HioFormatUInt32Vec4;
        case HIO_FORMAT_INT32: return pxr::HioFormatInt32;
        case HIO_FORMAT_INT32_VEC2: return pxr::HioFormatInt32Vec2;
        case HIO_FORMAT_INT32_VEC3: return pxr::HioFormatInt32Vec3;
        case HIO_FORMAT_INT32_VEC4: return pxr::HioFormatInt32Vec4;
        case HIO_FORMAT_UNORM8_SRGB: return pxr::HioFormatUNorm8srgb;
        case HIO_FORMAT_UNORM8_VEC2_SRGB: return pxr::HioFormatUNorm8Vec2srgb;
        case HIO_FORMAT_UNORM8_VEC3_SRGB: return pxr::HioFormatUNorm8Vec3srgb;
        case HIO_FORMAT_UNORM8_VEC4_SRGB: return pxr::HioFormatUNorm8Vec4srgb;
        case HIO_FORMAT_BC6_FLOAT_VEC3: return pxr::HioFormatBC6FloatVec3;
        case HIO_FORMAT_BC6_UFLOAT_VEC3: return pxr::HioFormatBC6UFloatVec3;
        case HIO_FORMAT_BC7_UNORM8_VEC4: return pxr::HioFormatBC7UNorm8Vec4;
        case HIO_FORMAT_BC7_UNORM8_VEC4_SRGB: return pxr::HioFormatBC7UNorm8Vec4srgb;
        case HIO_FORMAT_BC1_UNORM8_VEC4: return pxr::HioFormatBC1UNorm8Vec4;
        case HIO_FORMAT_BC3_UNORM8_VEC4: return pxr::HioFormatBC3UNorm8Vec4;
        default: return pxr::HioFormatInvalid;
    }
}

static pxr::HioImage::SourceColorSpace ConvertSourceColorSpace(HioSourceColorSpace space) {
    switch (space) {
        case HIO_SOURCE_COLOR_SPACE_RAW: return pxr::HioImage::SourceColorSpace::Raw;
        case HIO_SOURCE_COLOR_SPACE_SRGB: return pxr::HioImage::SourceColorSpace::SRGB;
        case HIO_SOURCE_COLOR_SPACE_AUTO:
        default: return pxr::HioImage::SourceColorSpace::Auto;
    }
}
#endif

extern "C" {

// ============================================================================
// MARK: - HioImage Functions
// ============================================================================

HioImageRef HioImage_OpenForReading(
    const char* filename,
    int subimage,
    int mip,
    HioSourceColorSpace sourceColorSpace,
    bool premultiplyAlpha
) {
    if (!filename) return nullptr;

#if HIO_USE_USD
    try {
        auto pxrColorSpace = ConvertSourceColorSpace(sourceColorSpace);
        auto image = HioImage::OpenForReading(
            std::string(filename),
            subimage,
            mip,
            pxrColorSpace,
            premultiplyAlpha
        );

        if (!image) {
            return nullptr;
        }

        auto* wrapper = new HioImageOpaque();
        wrapper->image = image;
        wrapper->filename = filename;
        wrapper->width = image->GetWidth();
        wrapper->height = image->GetHeight();
        wrapper->depth = 1; // 2D images have depth 1
        wrapper->format = ConvertFromPxrFormat(image->GetFormat());
        wrapper->bytesPerPixel = image->GetBytesPerPixel();
        wrapper->numMipLevels = image->GetNumMipLevels();
        wrapper->isSRGB = image->IsColorSpaceSRGB();
        wrapper->isForWriting = false;

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub image
    auto* wrapper = new HioImageOpaque();
    wrapper->filename = filename;
    wrapper->width = 256;
    wrapper->height = 256;
    wrapper->depth = 1;
    wrapper->format = HIO_FORMAT_UNORM8_VEC4;
    wrapper->bytesPerPixel = 4;
    wrapper->numMipLevels = 1;
    wrapper->isSRGB = false;
    wrapper->isForWriting = false;
    return wrapper;
#endif
}

HioImageRef HioImage_OpenForWriting(const char* filename) {
    if (!filename) return nullptr;

#if HIO_USE_USD
    try {
        auto image = HioImage::OpenForWriting(std::string(filename));
        if (!image) {
            return nullptr;
        }

        auto* wrapper = new HioImageOpaque();
        wrapper->image = image;
        wrapper->filename = filename;
        wrapper->isForWriting = true;

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub image for writing
    auto* wrapper = new HioImageOpaque();
    wrapper->filename = filename;
    wrapper->isForWriting = true;
    return wrapper;
#endif
}

HioImageRef HioImage_Retain(HioImageRef image) {
    if (image) {
        image->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return image;
}

void HioImage_Release(HioImageRef image) {
    if (image) {
        if (image->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete image;
        }
    }
}

bool HioImage_IsSupportedImageFile(const char* filename) {
    if (!filename) return false;

#if HIO_USE_USD
    try {
        return HioImage::IsSupportedImageFile(std::string(filename));
    } catch (...) {
        return false;
    }
#else
    // Standalone mode: check common extensions
    std::string name(filename);
    size_t dot = name.rfind('.');
    if (dot == std::string::npos) return false;

    std::string ext = name.substr(dot + 1);
    // Convert to lowercase
    for (auto& c : ext) c = static_cast<char>(tolower(c));

    return ext == "png" || ext == "jpg" || ext == "jpeg" ||
           ext == "tif" || ext == "tiff" || ext == "exr" ||
           ext == "hdr" || ext == "bmp" || ext == "tga";
#endif
}

UsdResult HioImage_GetFilename(HioImageRef image, char** outFilename) {
    if (!image || !outFilename) return USD_RESULT_INVALID_ARGUMENT;

#if HIO_USE_USD
    try {
        if (image->image) {
            *outFilename = DuplicateString(image->image->GetFilename());
        } else {
            *outFilename = DuplicateString(image->filename);
        }
        return *outFilename ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outFilename = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outFilename = DuplicateString(image->filename);
    return *outFilename ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

int HioImage_GetWidth(HioImageRef image) {
    if (!image) return 0;
#if HIO_USE_USD
    try {
        return image->image ? image->image->GetWidth() : image->width;
    } catch (...) {
        return 0;
    }
#else
    return image->width;
#endif
}

int HioImage_GetHeight(HioImageRef image) {
    if (!image) return 0;
#if HIO_USE_USD
    try {
        return image->image ? image->image->GetHeight() : image->height;
    } catch (...) {
        return 0;
    }
#else
    return image->height;
#endif
}

int HioImage_GetDepth(HioImageRef image) {
    if (!image) return 0;
    return image->depth;
}

HioFormat HioImage_GetFormat(HioImageRef image) {
    if (!image) return HIO_FORMAT_INVALID;
#if HIO_USE_USD
    try {
        if (image->image) {
            return ConvertFromPxrFormat(image->image->GetFormat());
        }
        return image->format;
    } catch (...) {
        return HIO_FORMAT_INVALID;
    }
#else
    return image->format;
#endif
}

int HioImage_GetBytesPerPixel(HioImageRef image) {
    if (!image) return 0;
#if HIO_USE_USD
    try {
        return image->image ? image->image->GetBytesPerPixel() : image->bytesPerPixel;
    } catch (...) {
        return 0;
    }
#else
    return image->bytesPerPixel;
#endif
}

int HioImage_GetNumMipLevels(HioImageRef image) {
    if (!image) return 0;
#if HIO_USE_USD
    try {
        return image->image ? image->image->GetNumMipLevels() : image->numMipLevels;
    } catch (...) {
        return 1;
    }
#else
    return image->numMipLevels;
#endif
}

bool HioImage_IsColorSpaceSRGB(HioImageRef image) {
    if (!image) return false;
#if HIO_USE_USD
    try {
        return image->image ? image->image->IsColorSpaceSRGB() : image->isSRGB;
    } catch (...) {
        return false;
    }
#else
    return image->isSRGB;
#endif
}

UsdResult HioImage_Read(HioImageRef image, HioImageStorageSpec* storageSpec) {
    if (!image || !storageSpec || !storageSpec->data) {
        return USD_RESULT_INVALID_ARGUMENT;
    }

#if HIO_USE_USD
    try {
        if (!image->image) {
            return USD_RESULT_ERROR;
        }

        HioImage::StorageSpec pxrSpec;
        pxrSpec.width = storageSpec->width;
        pxrSpec.height = storageSpec->height;
        pxrSpec.depth = storageSpec->depth;
        pxrSpec.format = ConvertToPxrFormat(storageSpec->format);
        pxrSpec.flipped = storageSpec->flipped;
        pxrSpec.data = storageSpec->data;

        bool success = image->image->Read(pxrSpec);
        return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: fill with test pattern
    size_t size = static_cast<size_t>(storageSpec->width) *
                  static_cast<size_t>(storageSpec->height) *
                  static_cast<size_t>(storageSpec->depth) * 4;
    memset(storageSpec->data, 128, size);
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult HioImage_ReadCropped(
    HioImageRef image,
    int cropTop,
    int cropBottom,
    int cropLeft,
    int cropRight,
    HioImageStorageSpec* storageSpec
) {
    if (!image || !storageSpec || !storageSpec->data) {
        return USD_RESULT_INVALID_ARGUMENT;
    }

#if HIO_USE_USD
    try {
        if (!image->image) {
            return USD_RESULT_ERROR;
        }

        HioImage::StorageSpec pxrSpec;
        pxrSpec.width = storageSpec->width;
        pxrSpec.height = storageSpec->height;
        pxrSpec.depth = storageSpec->depth;
        pxrSpec.format = ConvertToPxrFormat(storageSpec->format);
        pxrSpec.flipped = storageSpec->flipped;
        pxrSpec.data = storageSpec->data;

        bool success = image->image->ReadCropped(
            cropTop, cropBottom, cropLeft, cropRight, pxrSpec
        );
        return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: fill with test pattern
    size_t size = static_cast<size_t>(storageSpec->width) *
                  static_cast<size_t>(storageSpec->height) *
                  static_cast<size_t>(storageSpec->depth) * 4;
    memset(storageSpec->data, 128, size);
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult HioImage_Write(
    HioImageRef image,
    const HioImageStorageSpec* storageSpec
) {
    if (!image || !storageSpec || !storageSpec->data) {
        return USD_RESULT_INVALID_ARGUMENT;
    }

#if HIO_USE_USD
    try {
        if (!image->image) {
            return USD_RESULT_ERROR;
        }

        HioImage::StorageSpec pxrSpec;
        pxrSpec.width = storageSpec->width;
        pxrSpec.height = storageSpec->height;
        pxrSpec.depth = storageSpec->depth;
        pxrSpec.format = ConvertToPxrFormat(storageSpec->format);
        pxrSpec.flipped = storageSpec->flipped;
        pxrSpec.data = const_cast<void*>(storageSpec->data);

        bool success = image->image->Write(pxrSpec);
        return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: no-op success
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - HioGlslfx Functions
// ============================================================================

HioGlslfxRef HioGlslfx_CreateFromFile(const char* filePath) {
    if (!filePath) return nullptr;

#if HIO_USE_USD
    try {
        auto* wrapper = new HioGlslfxOpaque();
        wrapper->glslfx = std::make_unique<HioGlslfx>(std::string(filePath));

        if (!wrapper->glslfx->IsValid()) {
            delete wrapper;
            return nullptr;
        }

        wrapper->isValid = true;
        wrapper->surfaceSource = wrapper->glslfx->GetSurfaceSource();
        wrapper->displacementSource = wrapper->glslfx->GetDisplacementSource();
        wrapper->volumeSource = wrapper->glslfx->GetVolumeSource();
        wrapper->hash = wrapper->glslfx->GetHash();

        // Extract parameter names
        for (const auto& param : wrapper->glslfx->GetParameters()) {
            wrapper->parameterNames.push_back(param.name);
        }

        // Extract texture names
        for (const auto& tex : wrapper->glslfx->GetTextures()) {
            wrapper->textureNames.push_back(tex.name);
        }

        // Extract attribute names
        for (const auto& attr : wrapper->glslfx->GetAttributes()) {
            wrapper->attributeNames.push_back(attr.name);
        }

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub glslfx
    auto* wrapper = new HioGlslfxOpaque();
    wrapper->isValid = true;
    wrapper->surfaceSource = "// Standalone surface shader stub";
    wrapper->displacementSource = "// Standalone displacement shader stub";
    wrapper->volumeSource = "// Standalone volume shader stub";
    wrapper->hash = std::hash<std::string>{}(std::string(filePath));
    return wrapper;
#endif
}

HioGlslfxRef HioGlslfx_CreateFromString(const char* source) {
    if (!source) return nullptr;

#if HIO_USE_USD
    try {
        std::istringstream stream(source);
        auto* wrapper = new HioGlslfxOpaque();
        wrapper->glslfx = std::make_unique<HioGlslfx>(stream);

        if (!wrapper->glslfx->IsValid()) {
            delete wrapper;
            return nullptr;
        }

        wrapper->isValid = true;
        wrapper->surfaceSource = wrapper->glslfx->GetSurfaceSource();
        wrapper->displacementSource = wrapper->glslfx->GetDisplacementSource();
        wrapper->volumeSource = wrapper->glslfx->GetVolumeSource();
        wrapper->hash = wrapper->glslfx->GetHash();

        for (const auto& param : wrapper->glslfx->GetParameters()) {
            wrapper->parameterNames.push_back(param.name);
        }
        for (const auto& tex : wrapper->glslfx->GetTextures()) {
            wrapper->textureNames.push_back(tex.name);
        }
        for (const auto& attr : wrapper->glslfx->GetAttributes()) {
            wrapper->attributeNames.push_back(attr.name);
        }

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub glslfx
    auto* wrapper = new HioGlslfxOpaque();
    wrapper->isValid = true;
    wrapper->surfaceSource = source;
    wrapper->hash = std::hash<std::string>{}(std::string(source));
    return wrapper;
#endif
}

HioGlslfxRef HioGlslfx_Retain(HioGlslfxRef glslfx) {
    if (glslfx) {
        glslfx->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return glslfx;
}

void HioGlslfx_Release(HioGlslfxRef glslfx) {
    if (glslfx) {
        if (glslfx->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete glslfx;
        }
    }
}

bool HioGlslfx_IsValid(HioGlslfxRef glslfx) {
    if (!glslfx) return false;
#if HIO_USE_USD
    try {
        return glslfx->glslfx ? glslfx->glslfx->IsValid() : glslfx->isValid;
    } catch (...) {
        return false;
    }
#else
    return glslfx->isValid;
#endif
}

UsdResult HioGlslfx_GetSurfaceSource(HioGlslfxRef glslfx, char** outSource) {
    if (!glslfx || !outSource) return USD_RESULT_INVALID_ARGUMENT;

#if HIO_USE_USD
    try {
        std::string source = glslfx->glslfx ?
            glslfx->glslfx->GetSurfaceSource() : glslfx->surfaceSource;
        *outSource = DuplicateString(source);
        return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outSource = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outSource = DuplicateString(glslfx->surfaceSource);
    return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult HioGlslfx_GetDisplacementSource(HioGlslfxRef glslfx, char** outSource) {
    if (!glslfx || !outSource) return USD_RESULT_INVALID_ARGUMENT;

#if HIO_USE_USD
    try {
        std::string source = glslfx->glslfx ?
            glslfx->glslfx->GetDisplacementSource() : glslfx->displacementSource;
        *outSource = DuplicateString(source);
        return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outSource = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outSource = DuplicateString(glslfx->displacementSource);
    return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult HioGlslfx_GetVolumeSource(HioGlslfxRef glslfx, char** outSource) {
    if (!glslfx || !outSource) return USD_RESULT_INVALID_ARGUMENT;

#if HIO_USE_USD
    try {
        std::string source = glslfx->glslfx ?
            glslfx->glslfx->GetVolumeSource() : glslfx->volumeSource;
        *outSource = DuplicateString(source);
        return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
    } catch (...) {
        *outSource = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    *outSource = DuplicateString(glslfx->volumeSource);
    return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
#endif
}

UsdResult HioGlslfx_GetSource(HioGlslfxRef glslfx, const char* shaderStageKey, char** outSource) {
    if (!glslfx || !shaderStageKey || !outSource) return USD_RESULT_INVALID_ARGUMENT;

#if HIO_USE_USD
    try {
        if (glslfx->glslfx) {
            std::string source = glslfx->glslfx->GetSource(TfToken(shaderStageKey));
            *outSource = DuplicateString(source);
            return *outSource ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
        }
        *outSource = nullptr;
        return USD_RESULT_NOT_FOUND;
    } catch (...) {
        *outSource = nullptr;
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: return empty source for unknown stages
    *outSource = DuplicateString("");
    return USD_RESULT_SUCCESS;
#endif
}

size_t HioGlslfx_GetParameterCount(HioGlslfxRef glslfx) {
    if (!glslfx) return 0;
    return glslfx->parameterNames.size();
}

size_t HioGlslfx_GetParameterNames(HioGlslfxRef glslfx, char** outNames, size_t maxCount) {
    if (!glslfx || !outNames || maxCount == 0) return 0;

    size_t count = std::min(maxCount, glslfx->parameterNames.size());
    for (size_t i = 0; i < count; ++i) {
        outNames[i] = DuplicateString(glslfx->parameterNames[i]);
    }
    return count;
}

size_t HioGlslfx_GetTextureCount(HioGlslfxRef glslfx) {
    if (!glslfx) return 0;
    return glslfx->textureNames.size();
}

size_t HioGlslfx_GetTextureNames(HioGlslfxRef glslfx, char** outNames, size_t maxCount) {
    if (!glslfx || !outNames || maxCount == 0) return 0;

    size_t count = std::min(maxCount, glslfx->textureNames.size());
    for (size_t i = 0; i < count; ++i) {
        outNames[i] = DuplicateString(glslfx->textureNames[i]);
    }
    return count;
}

size_t HioGlslfx_GetAttributeCount(HioGlslfxRef glslfx) {
    if (!glslfx) return 0;
    return glslfx->attributeNames.size();
}

size_t HioGlslfx_GetAttributeNames(HioGlslfxRef glslfx, char** outNames, size_t maxCount) {
    if (!glslfx || !outNames || maxCount == 0) return 0;

    size_t count = std::min(maxCount, glslfx->attributeNames.size());
    for (size_t i = 0; i < count; ++i) {
        outNames[i] = DuplicateString(glslfx->attributeNames[i]);
    }
    return count;
}

size_t HioGlslfx_GetHash(HioGlslfxRef glslfx) {
    if (!glslfx) return 0;
#if HIO_USE_USD
    try {
        return glslfx->glslfx ? glslfx->glslfx->GetHash() : glslfx->hash;
    } catch (...) {
        return 0;
    }
#else
    return glslfx->hash;
#endif
}

// ============================================================================
// MARK: - HioFieldTextureData Functions
// ============================================================================

HioFieldTextureDataRef HioFieldTextureData_Create(
    const char* filePath,
    const char* fieldName,
    int fieldIndex,
    const char* fieldPurpose,
    size_t targetMemory
) {
    if (!filePath) return nullptr;

#if HIO_USE_USD
    try {
        auto fieldData = HioFieldTextureData::New(
            std::string(filePath),
            TfToken(fieldName ? fieldName : ""),
            fieldIndex,
            TfToken(fieldPurpose ? fieldPurpose : ""),
            targetMemory
        );

        if (!fieldData) {
            return nullptr;
        }

        auto* wrapper = new HioFieldTextureDataOpaque();
        wrapper->fieldData = fieldData;
        wrapper->filePath = filePath;
        wrapper->fieldName = fieldName ? fieldName : "";
        wrapper->format = ConvertFromPxrFormat(fieldData->GetFormat());
        wrapper->width = fieldData->ResizedWidth();
        wrapper->height = fieldData->ResizedHeight();
        wrapper->depth = fieldData->ResizedDepth();

        auto bbox = fieldData->GetBoundingBox();
        wrapper->bboxMin[0] = bbox.GetMin()[0];
        wrapper->bboxMin[1] = bbox.GetMin()[1];
        wrapper->bboxMin[2] = bbox.GetMin()[2];
        wrapper->bboxMax[0] = bbox.GetMax()[0];
        wrapper->bboxMax[1] = bbox.GetMax()[1];
        wrapper->bboxMax[2] = bbox.GetMax()[2];

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create stub field texture data
    auto* wrapper = new HioFieldTextureDataOpaque();
    wrapper->filePath = filePath;
    wrapper->fieldName = fieldName ? fieldName : "";
    wrapper->format = HIO_FORMAT_FLOAT32;
    wrapper->width = 64;
    wrapper->height = 64;
    wrapper->depth = 64;
    wrapper->bboxMin[0] = 0.0; wrapper->bboxMin[1] = 0.0; wrapper->bboxMin[2] = 0.0;
    wrapper->bboxMax[0] = 1.0; wrapper->bboxMax[1] = 1.0; wrapper->bboxMax[2] = 1.0;
    return wrapper;
#endif
}

HioFieldTextureDataRef HioFieldTextureData_Retain(HioFieldTextureDataRef fieldData) {
    if (fieldData) {
        fieldData->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return fieldData;
}

void HioFieldTextureData_Release(HioFieldTextureDataRef fieldData) {
    if (fieldData) {
        if (fieldData->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete fieldData;
        }
    }
}

HioFormat HioFieldTextureData_GetFormat(HioFieldTextureDataRef fieldData) {
    if (!fieldData) return HIO_FORMAT_INVALID;
#if HIO_USE_USD
    try {
        if (fieldData->fieldData) {
            return ConvertFromPxrFormat(fieldData->fieldData->GetFormat());
        }
        return fieldData->format;
    } catch (...) {
        return HIO_FORMAT_INVALID;
    }
#else
    return fieldData->format;
#endif
}

int HioFieldTextureData_GetResizedWidth(HioFieldTextureDataRef fieldData) {
    if (!fieldData) return 0;
#if HIO_USE_USD
    try {
        return fieldData->fieldData ?
            fieldData->fieldData->ResizedWidth() : fieldData->width;
    } catch (...) {
        return 0;
    }
#else
    return fieldData->width;
#endif
}

int HioFieldTextureData_GetResizedHeight(HioFieldTextureDataRef fieldData) {
    if (!fieldData) return 0;
#if HIO_USE_USD
    try {
        return fieldData->fieldData ?
            fieldData->fieldData->ResizedHeight() : fieldData->height;
    } catch (...) {
        return 0;
    }
#else
    return fieldData->height;
#endif
}

int HioFieldTextureData_GetResizedDepth(HioFieldTextureDataRef fieldData) {
    if (!fieldData) return 0;
#if HIO_USE_USD
    try {
        return fieldData->fieldData ?
            fieldData->fieldData->ResizedDepth() : fieldData->depth;
    } catch (...) {
        return 0;
    }
#else
    return fieldData->depth;
#endif
}

void HioFieldTextureData_GetBoundingBoxMin(
    HioFieldTextureDataRef fieldData,
    double* outX, double* outY, double* outZ
) {
    if (!fieldData) {
        if (outX) *outX = 0.0;
        if (outY) *outY = 0.0;
        if (outZ) *outZ = 0.0;
        return;
    }

#if HIO_USE_USD
    try {
        if (fieldData->fieldData) {
            auto bbox = fieldData->fieldData->GetBoundingBox();
            if (outX) *outX = bbox.GetMin()[0];
            if (outY) *outY = bbox.GetMin()[1];
            if (outZ) *outZ = bbox.GetMin()[2];
            return;
        }
    } catch (...) {
        // Fall through to cached values
    }
#endif

    if (outX) *outX = fieldData->bboxMin[0];
    if (outY) *outY = fieldData->bboxMin[1];
    if (outZ) *outZ = fieldData->bboxMin[2];
}

void HioFieldTextureData_GetBoundingBoxMax(
    HioFieldTextureDataRef fieldData,
    double* outX, double* outY, double* outZ
) {
    if (!fieldData) {
        if (outX) *outX = 0.0;
        if (outY) *outY = 0.0;
        if (outZ) *outZ = 0.0;
        return;
    }

#if HIO_USE_USD
    try {
        if (fieldData->fieldData) {
            auto bbox = fieldData->fieldData->GetBoundingBox();
            if (outX) *outX = bbox.GetMax()[0];
            if (outY) *outY = bbox.GetMax()[1];
            if (outZ) *outZ = bbox.GetMax()[2];
            return;
        }
    } catch (...) {
        // Fall through to cached values
    }
#endif

    if (outX) *outX = fieldData->bboxMax[0];
    if (outY) *outY = fieldData->bboxMax[1];
    if (outZ) *outZ = fieldData->bboxMax[2];
}

bool HioFieldTextureData_HasRawBuffer(HioFieldTextureDataRef fieldData) {
    if (!fieldData) return false;
#if HIO_USE_USD
    try {
        return fieldData->fieldData ?
            fieldData->fieldData->HasRawBuffer() : !fieldData->rawBuffer.empty();
    } catch (...) {
        return false;
    }
#else
    return !fieldData->rawBuffer.empty();
#endif
}

bool HioFieldTextureData_GetRawBuffer(
    HioFieldTextureDataRef fieldData,
    const void** outBuffer,
    size_t* outSize
) {
    if (!fieldData || !outBuffer || !outSize) return false;

#if HIO_USE_USD
    try {
        if (fieldData->fieldData && fieldData->fieldData->HasRawBuffer()) {
            *outBuffer = fieldData->fieldData->GetRawBuffer();
            // Calculate size from dimensions and format
            size_t formatSize = HioGetDataSizeOfFormat(fieldData->format);
            *outSize = static_cast<size_t>(fieldData->fieldData->ResizedWidth()) *
                       static_cast<size_t>(fieldData->fieldData->ResizedHeight()) *
                       static_cast<size_t>(fieldData->fieldData->ResizedDepth()) *
                       formatSize;
            return true;
        }
    } catch (...) {
        // Fall through
    }
#endif

    if (!fieldData->rawBuffer.empty()) {
        *outBuffer = fieldData->rawBuffer.data();
        *outSize = fieldData->rawBuffer.size();
        return true;
    }

    *outBuffer = nullptr;
    *outSize = 0;
    return false;
}

UsdResult HioFieldTextureData_Read(HioFieldTextureDataRef fieldData) {
    if (!fieldData) return USD_RESULT_INVALID_ARGUMENT;

#if HIO_USE_USD
    try {
        if (fieldData->fieldData) {
            bool success = fieldData->fieldData->Read();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone mode: generate stub data
    size_t size = static_cast<size_t>(fieldData->width) *
                  static_cast<size_t>(fieldData->height) *
                  static_cast<size_t>(fieldData->depth) * sizeof(float);
    fieldData->rawBuffer.resize(size);
    memset(fieldData->rawBuffer.data(), 0, size);
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - Format Utility Functions
// ============================================================================

HioFormat HioGetFormat(uint32_t nchannels, HioType type, bool isSRGB) {
#if HIO_USE_USD
    try {
        pxr::HioType pxrType;
        switch (type) {
            case HIO_TYPE_UNSIGNED_BYTE: pxrType = pxr::HioTypeUnsignedByte; break;
            case HIO_TYPE_UNSIGNED_BYTE_SRGB: pxrType = pxr::HioTypeUnsignedByteSRGB; break;
            case HIO_TYPE_SIGNED_BYTE: pxrType = pxr::HioTypeSignedByte; break;
            case HIO_TYPE_UNSIGNED_SHORT: pxrType = pxr::HioTypeUnsignedShort; break;
            case HIO_TYPE_SIGNED_SHORT: pxrType = pxr::HioTypeSignedShort; break;
            case HIO_TYPE_UNSIGNED_INT: pxrType = pxr::HioTypeUnsignedInt; break;
            case HIO_TYPE_INT: pxrType = pxr::HioTypeInt; break;
            case HIO_TYPE_HALF_FLOAT: pxrType = pxr::HioTypeHalfFloat; break;
            case HIO_TYPE_FLOAT: pxrType = pxr::HioTypeFloat; break;
            case HIO_TYPE_DOUBLE: pxrType = pxr::HioTypeDouble; break;
            default: return HIO_FORMAT_INVALID;
        }
        return ConvertFromPxrFormat(pxr::HioGetFormat(nchannels, pxrType, isSRGB));
    } catch (...) {
        return HIO_FORMAT_INVALID;
    }
#else
    // Standalone mode: simple format calculation
    if (nchannels < 1 || nchannels > 4) return HIO_FORMAT_INVALID;

    switch (type) {
        case HIO_TYPE_UNSIGNED_BYTE:
        case HIO_TYPE_UNSIGNED_BYTE_SRGB:
            if (isSRGB) {
                return static_cast<HioFormat>(HIO_FORMAT_UNORM8_SRGB + (nchannels - 1));
            }
            return static_cast<HioFormat>(HIO_FORMAT_UNORM8 + (nchannels - 1));
        case HIO_TYPE_SIGNED_BYTE:
            return static_cast<HioFormat>(HIO_FORMAT_SNORM8 + (nchannels - 1));
        case HIO_TYPE_HALF_FLOAT:
            return static_cast<HioFormat>(HIO_FORMAT_FLOAT16 + (nchannels - 1));
        case HIO_TYPE_FLOAT:
            return static_cast<HioFormat>(HIO_FORMAT_FLOAT32 + (nchannels - 1));
        case HIO_TYPE_DOUBLE:
            return static_cast<HioFormat>(HIO_FORMAT_DOUBLE64 + (nchannels - 1));
        case HIO_TYPE_UNSIGNED_SHORT:
            return static_cast<HioFormat>(HIO_FORMAT_UINT16 + (nchannels - 1));
        case HIO_TYPE_SIGNED_SHORT:
            return static_cast<HioFormat>(HIO_FORMAT_INT16 + (nchannels - 1));
        case HIO_TYPE_UNSIGNED_INT:
            return static_cast<HioFormat>(HIO_FORMAT_UINT32 + (nchannels - 1));
        case HIO_TYPE_INT:
            return static_cast<HioFormat>(HIO_FORMAT_INT32 + (nchannels - 1));
        default:
            return HIO_FORMAT_INVALID;
    }
#endif
}

HioType HioGetType(HioFormat format) {
    switch (format) {
        case HIO_FORMAT_UNORM8:
        case HIO_FORMAT_UNORM8_VEC2:
        case HIO_FORMAT_UNORM8_VEC3:
        case HIO_FORMAT_UNORM8_VEC4:
            return HIO_TYPE_UNSIGNED_BYTE;

        case HIO_FORMAT_UNORM8_SRGB:
        case HIO_FORMAT_UNORM8_VEC2_SRGB:
        case HIO_FORMAT_UNORM8_VEC3_SRGB:
        case HIO_FORMAT_UNORM8_VEC4_SRGB:
            return HIO_TYPE_UNSIGNED_BYTE_SRGB;

        case HIO_FORMAT_SNORM8:
        case HIO_FORMAT_SNORM8_VEC2:
        case HIO_FORMAT_SNORM8_VEC3:
        case HIO_FORMAT_SNORM8_VEC4:
            return HIO_TYPE_SIGNED_BYTE;

        case HIO_FORMAT_FLOAT16:
        case HIO_FORMAT_FLOAT16_VEC2:
        case HIO_FORMAT_FLOAT16_VEC3:
        case HIO_FORMAT_FLOAT16_VEC4:
            return HIO_TYPE_HALF_FLOAT;

        case HIO_FORMAT_FLOAT32:
        case HIO_FORMAT_FLOAT32_VEC2:
        case HIO_FORMAT_FLOAT32_VEC3:
        case HIO_FORMAT_FLOAT32_VEC4:
            return HIO_TYPE_FLOAT;

        case HIO_FORMAT_DOUBLE64:
        case HIO_FORMAT_DOUBLE64_VEC2:
        case HIO_FORMAT_DOUBLE64_VEC3:
        case HIO_FORMAT_DOUBLE64_VEC4:
            return HIO_TYPE_DOUBLE;

        case HIO_FORMAT_UINT16:
        case HIO_FORMAT_UINT16_VEC2:
        case HIO_FORMAT_UINT16_VEC3:
        case HIO_FORMAT_UINT16_VEC4:
            return HIO_TYPE_UNSIGNED_SHORT;

        case HIO_FORMAT_INT16:
        case HIO_FORMAT_INT16_VEC2:
        case HIO_FORMAT_INT16_VEC3:
        case HIO_FORMAT_INT16_VEC4:
            return HIO_TYPE_SIGNED_SHORT;

        case HIO_FORMAT_UINT32:
        case HIO_FORMAT_UINT32_VEC2:
        case HIO_FORMAT_UINT32_VEC3:
        case HIO_FORMAT_UINT32_VEC4:
            return HIO_TYPE_UNSIGNED_INT;

        case HIO_FORMAT_INT32:
        case HIO_FORMAT_INT32_VEC2:
        case HIO_FORMAT_INT32_VEC3:
        case HIO_FORMAT_INT32_VEC4:
            return HIO_TYPE_INT;

        default:
            return HIO_TYPE_UNSIGNED_BYTE;
    }
}

int HioGetComponentCount(HioFormat format) {
    switch (format) {
        case HIO_FORMAT_UNORM8:
        case HIO_FORMAT_SNORM8:
        case HIO_FORMAT_FLOAT16:
        case HIO_FORMAT_FLOAT32:
        case HIO_FORMAT_DOUBLE64:
        case HIO_FORMAT_UINT16:
        case HIO_FORMAT_INT16:
        case HIO_FORMAT_UINT32:
        case HIO_FORMAT_INT32:
        case HIO_FORMAT_UNORM8_SRGB:
            return 1;

        case HIO_FORMAT_UNORM8_VEC2:
        case HIO_FORMAT_SNORM8_VEC2:
        case HIO_FORMAT_FLOAT16_VEC2:
        case HIO_FORMAT_FLOAT32_VEC2:
        case HIO_FORMAT_DOUBLE64_VEC2:
        case HIO_FORMAT_UINT16_VEC2:
        case HIO_FORMAT_INT16_VEC2:
        case HIO_FORMAT_UINT32_VEC2:
        case HIO_FORMAT_INT32_VEC2:
        case HIO_FORMAT_UNORM8_VEC2_SRGB:
            return 2;

        case HIO_FORMAT_UNORM8_VEC3:
        case HIO_FORMAT_SNORM8_VEC3:
        case HIO_FORMAT_FLOAT16_VEC3:
        case HIO_FORMAT_FLOAT32_VEC3:
        case HIO_FORMAT_DOUBLE64_VEC3:
        case HIO_FORMAT_UINT16_VEC3:
        case HIO_FORMAT_INT16_VEC3:
        case HIO_FORMAT_UINT32_VEC3:
        case HIO_FORMAT_INT32_VEC3:
        case HIO_FORMAT_UNORM8_VEC3_SRGB:
        case HIO_FORMAT_BC6_FLOAT_VEC3:
        case HIO_FORMAT_BC6_UFLOAT_VEC3:
            return 3;

        case HIO_FORMAT_UNORM8_VEC4:
        case HIO_FORMAT_SNORM8_VEC4:
        case HIO_FORMAT_FLOAT16_VEC4:
        case HIO_FORMAT_FLOAT32_VEC4:
        case HIO_FORMAT_DOUBLE64_VEC4:
        case HIO_FORMAT_UINT16_VEC4:
        case HIO_FORMAT_INT16_VEC4:
        case HIO_FORMAT_UINT32_VEC4:
        case HIO_FORMAT_INT32_VEC4:
        case HIO_FORMAT_UNORM8_VEC4_SRGB:
        case HIO_FORMAT_BC7_UNORM8_VEC4:
        case HIO_FORMAT_BC7_UNORM8_VEC4_SRGB:
        case HIO_FORMAT_BC1_UNORM8_VEC4:
        case HIO_FORMAT_BC3_UNORM8_VEC4:
            return 4;

        default:
            return 0;
    }
}

size_t HioGetDataSizeOfFormat(HioFormat format) {
    int componentCount = HioGetComponentCount(format);
    HioType type = HioGetType(format);

    size_t componentSize = 0;
    switch (type) {
        case HIO_TYPE_UNSIGNED_BYTE:
        case HIO_TYPE_UNSIGNED_BYTE_SRGB:
        case HIO_TYPE_SIGNED_BYTE:
            componentSize = 1;
            break;
        case HIO_TYPE_UNSIGNED_SHORT:
        case HIO_TYPE_SIGNED_SHORT:
        case HIO_TYPE_HALF_FLOAT:
            componentSize = 2;
            break;
        case HIO_TYPE_UNSIGNED_INT:
        case HIO_TYPE_INT:
        case HIO_TYPE_FLOAT:
            componentSize = 4;
            break;
        case HIO_TYPE_DOUBLE:
            componentSize = 8;
            break;
    }

    // Handle compressed formats specially
    if (HioIsCompressed(format)) {
        switch (format) {
            case HIO_FORMAT_BC1_UNORM8_VEC4:
                return 8; // 8 bytes per 4x4 block
            case HIO_FORMAT_BC3_UNORM8_VEC4:
            case HIO_FORMAT_BC6_FLOAT_VEC3:
            case HIO_FORMAT_BC6_UFLOAT_VEC3:
            case HIO_FORMAT_BC7_UNORM8_VEC4:
            case HIO_FORMAT_BC7_UNORM8_VEC4_SRGB:
                return 16; // 16 bytes per 4x4 block
            default:
                return 0;
        }
    }

    return componentCount * componentSize;
}

bool HioIsCompressed(HioFormat format) {
    switch (format) {
        case HIO_FORMAT_BC6_FLOAT_VEC3:
        case HIO_FORMAT_BC6_UFLOAT_VEC3:
        case HIO_FORMAT_BC7_UNORM8_VEC4:
        case HIO_FORMAT_BC7_UNORM8_VEC4_SRGB:
        case HIO_FORMAT_BC1_UNORM8_VEC4:
        case HIO_FORMAT_BC3_UNORM8_VEC4:
            return true;
        default:
            return false;
    }
}

size_t HioGetDataSize(HioFormat format, int width, int height, int depth) {
    if (HioIsCompressed(format)) {
        // Compressed formats use 4x4 blocks
        int blocksX = (width + 3) / 4;
        int blocksY = (height + 3) / 4;
        size_t blockSize = HioGetDataSizeOfFormat(format);
        return static_cast<size_t>(blocksX) *
               static_cast<size_t>(blocksY) *
               static_cast<size_t>(depth) * blockSize;
    }

    size_t elementSize = HioGetDataSizeOfFormat(format);
    return static_cast<size_t>(width) *
           static_cast<size_t>(height) *
           static_cast<size_t>(depth) * elementSize;
}

// ============================================================================
// MARK: - Image Registry Functions
// ============================================================================

bool Hio_IsSupportedImageFile(const char* filename) {
    return HioImage_IsSupportedImageFile(filename);
}

// Supported extensions for standalone mode
static const char* g_standaloneExtensions[] = {
    "png", "jpg", "jpeg", "tif", "tiff", "exr", "hdr", "bmp", "tga"
};
static const size_t g_standaloneExtensionCount = sizeof(g_standaloneExtensions) / sizeof(g_standaloneExtensions[0]);

size_t Hio_GetSupportedImageFormatCount(void) {
#if HIO_USE_USD
    // In real USD mode, we'd query the registry
    // For now, return common formats
    return g_standaloneExtensionCount;
#else
    return g_standaloneExtensionCount;
#endif
}

size_t Hio_GetSupportedImageFormats(char** outExtensions, size_t maxCount) {
    if (!outExtensions || maxCount == 0) return 0;

    size_t count = std::min(maxCount, g_standaloneExtensionCount);
    for (size_t i = 0; i < count; ++i) {
        outExtensions[i] = DuplicateString(g_standaloneExtensions[i]);
    }
    return count;
}

// ============================================================================
// MARK: - Memory Management
// ============================================================================

void Hio_FreeString(char* str) {
    free(str);
}

} // extern "C"
