// hio.h - Hio C wrapper
// Mirrors: pxr/imaging/hio/*.h (Image loading, OpenEXR, texture formats)

#ifndef OPENUSDINTEROP_IMAGING_HIO_H
#define OPENUSDINTEROP_IMAGING_HIO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

/// Opaque handle for HioImage
typedef struct HioImageOpaque* HioImageRef;

/// Opaque handle for HioGlslfx shader effects
typedef struct HioGlslfxOpaque* HioGlslfxRef;

/// Opaque handle for HioFieldTextureData (volumetric textures)
typedef struct HioFieldTextureDataOpaque* HioFieldTextureDataRef;

// ============================================================================
// MARK: - HioFormat Enum (mirrors pxr::HioFormat)
// ============================================================================

/// Pixel format enumeration matching pxr::HioFormat
typedef enum {
    HIO_FORMAT_INVALID = 0,

    // UNorm8 formats (8-bit normalized unsigned)
    HIO_FORMAT_UNORM8 = 1,
    HIO_FORMAT_UNORM8_VEC2 = 2,
    HIO_FORMAT_UNORM8_VEC3 = 3,
    HIO_FORMAT_UNORM8_VEC4 = 4,

    // SNorm8 formats (8-bit normalized signed)
    HIO_FORMAT_SNORM8 = 5,
    HIO_FORMAT_SNORM8_VEC2 = 6,
    HIO_FORMAT_SNORM8_VEC3 = 7,
    HIO_FORMAT_SNORM8_VEC4 = 8,

    // Float16 formats (half-precision)
    HIO_FORMAT_FLOAT16 = 9,
    HIO_FORMAT_FLOAT16_VEC2 = 10,
    HIO_FORMAT_FLOAT16_VEC3 = 11,
    HIO_FORMAT_FLOAT16_VEC4 = 12,

    // Float32 formats (single-precision)
    HIO_FORMAT_FLOAT32 = 13,
    HIO_FORMAT_FLOAT32_VEC2 = 14,
    HIO_FORMAT_FLOAT32_VEC3 = 15,
    HIO_FORMAT_FLOAT32_VEC4 = 16,

    // Double64 formats (double-precision)
    HIO_FORMAT_DOUBLE64 = 17,
    HIO_FORMAT_DOUBLE64_VEC2 = 18,
    HIO_FORMAT_DOUBLE64_VEC3 = 19,
    HIO_FORMAT_DOUBLE64_VEC4 = 20,

    // UInt16 formats
    HIO_FORMAT_UINT16 = 21,
    HIO_FORMAT_UINT16_VEC2 = 22,
    HIO_FORMAT_UINT16_VEC3 = 23,
    HIO_FORMAT_UINT16_VEC4 = 24,

    // Int16 formats
    HIO_FORMAT_INT16 = 25,
    HIO_FORMAT_INT16_VEC2 = 26,
    HIO_FORMAT_INT16_VEC3 = 27,
    HIO_FORMAT_INT16_VEC4 = 28,

    // UInt32 formats
    HIO_FORMAT_UINT32 = 29,
    HIO_FORMAT_UINT32_VEC2 = 30,
    HIO_FORMAT_UINT32_VEC3 = 31,
    HIO_FORMAT_UINT32_VEC4 = 32,

    // Int32 formats
    HIO_FORMAT_INT32 = 33,
    HIO_FORMAT_INT32_VEC2 = 34,
    HIO_FORMAT_INT32_VEC3 = 35,
    HIO_FORMAT_INT32_VEC4 = 36,

    // sRGB formats
    HIO_FORMAT_UNORM8_SRGB = 37,
    HIO_FORMAT_UNORM8_VEC2_SRGB = 38,
    HIO_FORMAT_UNORM8_VEC3_SRGB = 39,
    HIO_FORMAT_UNORM8_VEC4_SRGB = 40,

    // Compressed formats (BC/DXT)
    HIO_FORMAT_BC6_FLOAT_VEC3 = 41,
    HIO_FORMAT_BC6_UFLOAT_VEC3 = 42,
    HIO_FORMAT_BC7_UNORM8_VEC4 = 43,
    HIO_FORMAT_BC7_UNORM8_VEC4_SRGB = 44,
    HIO_FORMAT_BC1_UNORM8_VEC4 = 45,
    HIO_FORMAT_BC3_UNORM8_VEC4 = 46,

    HIO_FORMAT_COUNT
} HioFormat;

/// Image origin location
typedef enum {
    HIO_IMAGE_ORIGIN_UPPER_LEFT = 0,
    HIO_IMAGE_ORIGIN_LOWER_LEFT = 1
} HioImageOriginLocation;

/// Source color space
typedef enum {
    HIO_SOURCE_COLOR_SPACE_RAW = 0,
    HIO_SOURCE_COLOR_SPACE_SRGB = 1,
    HIO_SOURCE_COLOR_SPACE_AUTO = 2
} HioSourceColorSpace;

/// Texture address mode
typedef enum {
    HIO_ADDRESS_MODE_CLAMP_TO_EDGE = 0,
    HIO_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE = 1,
    HIO_ADDRESS_MODE_REPEAT = 2,
    HIO_ADDRESS_MODE_MIRROR_REPEAT = 3,
    HIO_ADDRESS_MODE_CLAMP_TO_BORDER_COLOR = 4
} HioAddressMode;

/// Component data type
typedef enum {
    HIO_TYPE_UNSIGNED_BYTE = 0,
    HIO_TYPE_UNSIGNED_BYTE_SRGB = 1,
    HIO_TYPE_SIGNED_BYTE = 2,
    HIO_TYPE_UNSIGNED_SHORT = 3,
    HIO_TYPE_SIGNED_SHORT = 4,
    HIO_TYPE_UNSIGNED_INT = 5,
    HIO_TYPE_INT = 6,
    HIO_TYPE_HALF_FLOAT = 7,
    HIO_TYPE_FLOAT = 8,
    HIO_TYPE_DOUBLE = 9
} HioType;

// ============================================================================
// MARK: - HioImage Storage Spec
// ============================================================================

/// Storage specification for image data
typedef struct {
    int width;
    int height;
    int depth;
    HioFormat format;
    bool flipped;
    void* data;
} HioImageStorageSpec;

// ============================================================================
// MARK: - HioImage Functions
// ============================================================================

/// Opens an image file for reading.
/// - Parameters:
///   - filename: Path to the image file
///   - subimage: Subimage index (for multi-image files like EXR)
///   - mip: Mip level to read
///   - sourceColorSpace: Color space interpretation
///   - premultiplyAlpha: Whether to premultiply alpha
/// Returns NULL on failure. Caller must release with HioImage_Release.
USD_INTEROP_API HioImageRef HioImage_OpenForReading(
    const char* filename,
    int subimage,
    int mip,
    HioSourceColorSpace sourceColorSpace,
    bool premultiplyAlpha
);

/// Opens an image file for writing.
/// - Parameters:
///   - filename: Path to write the image
/// Returns NULL on failure. Caller must release with HioImage_Release.
USD_INTEROP_API HioImageRef HioImage_OpenForWriting(const char* filename);

/// Retains an image reference.
USD_INTEROP_API HioImageRef HioImage_Retain(HioImageRef image);

/// Releases an image reference.
USD_INTEROP_API void HioImage_Release(HioImageRef image);

/// Checks if the file is a supported image format.
USD_INTEROP_API bool HioImage_IsSupportedImageFile(const char* filename);

/// Gets the filename of the image.
/// Caller must free with Hio_FreeString.
USD_INTEROP_API UsdResult HioImage_GetFilename(HioImageRef image, char** outFilename);

/// Gets the image width.
USD_INTEROP_API int HioImage_GetWidth(HioImageRef image);

/// Gets the image height.
USD_INTEROP_API int HioImage_GetHeight(HioImageRef image);

/// Gets the image depth (for 3D textures).
USD_INTEROP_API int HioImage_GetDepth(HioImageRef image);

/// Gets the pixel format.
USD_INTEROP_API HioFormat HioImage_GetFormat(HioImageRef image);

/// Gets bytes per pixel.
USD_INTEROP_API int HioImage_GetBytesPerPixel(HioImageRef image);

/// Gets the number of mip levels.
USD_INTEROP_API int HioImage_GetNumMipLevels(HioImageRef image);

/// Checks if the image is in sRGB color space.
USD_INTEROP_API bool HioImage_IsColorSpaceSRGB(HioImageRef image);

/// Reads the image data into the provided storage spec.
/// The data buffer in storageSpec must be pre-allocated.
USD_INTEROP_API UsdResult HioImage_Read(HioImageRef image, HioImageStorageSpec* storageSpec);

/// Reads a cropped region of the image.
USD_INTEROP_API UsdResult HioImage_ReadCropped(
    HioImageRef image,
    int cropTop,
    int cropBottom,
    int cropLeft,
    int cropRight,
    HioImageStorageSpec* storageSpec
);

/// Writes image data from the storage spec.
USD_INTEROP_API UsdResult HioImage_Write(
    HioImageRef image,
    const HioImageStorageSpec* storageSpec
);

// ============================================================================
// MARK: - HioGlslfx Functions (GLSL Shader Effects)
// ============================================================================

/// Creates a Glslfx object from a file path.
/// Returns NULL on failure. Caller must release with HioGlslfx_Release.
USD_INTEROP_API HioGlslfxRef HioGlslfx_CreateFromFile(const char* filePath);

/// Creates a Glslfx object from a string.
/// Returns NULL on failure. Caller must release with HioGlslfx_Release.
USD_INTEROP_API HioGlslfxRef HioGlslfx_CreateFromString(const char* source);

/// Retains a Glslfx reference.
USD_INTEROP_API HioGlslfxRef HioGlslfx_Retain(HioGlslfxRef glslfx);

/// Releases a Glslfx reference.
USD_INTEROP_API void HioGlslfx_Release(HioGlslfxRef glslfx);

/// Returns whether the Glslfx is valid.
USD_INTEROP_API bool HioGlslfx_IsValid(HioGlslfxRef glslfx);

/// Gets the surface shader source code.
/// Caller must free with Hio_FreeString.
USD_INTEROP_API UsdResult HioGlslfx_GetSurfaceSource(HioGlslfxRef glslfx, char** outSource);

/// Gets the displacement shader source code.
/// Caller must free with Hio_FreeString.
USD_INTEROP_API UsdResult HioGlslfx_GetDisplacementSource(HioGlslfxRef glslfx, char** outSource);

/// Gets the volume shader source code.
/// Caller must free with Hio_FreeString.
USD_INTEROP_API UsdResult HioGlslfx_GetVolumeSource(HioGlslfxRef glslfx, char** outSource);

/// Gets shader source for a specific shader stage key.
/// Caller must free with Hio_FreeString.
USD_INTEROP_API UsdResult HioGlslfx_GetSource(HioGlslfxRef glslfx, const char* shaderStageKey, char** outSource);

/// Gets the number of parameters.
USD_INTEROP_API size_t HioGlslfx_GetParameterCount(HioGlslfxRef glslfx);

/// Gets parameter names.
/// outNames must have space for maxCount pointers.
/// Caller must free each string with Hio_FreeString.
USD_INTEROP_API size_t HioGlslfx_GetParameterNames(HioGlslfxRef glslfx, char** outNames, size_t maxCount);

/// Gets the number of textures.
USD_INTEROP_API size_t HioGlslfx_GetTextureCount(HioGlslfxRef glslfx);

/// Gets texture names.
/// outNames must have space for maxCount pointers.
/// Caller must free each string with Hio_FreeString.
USD_INTEROP_API size_t HioGlslfx_GetTextureNames(HioGlslfxRef glslfx, char** outNames, size_t maxCount);

/// Gets the number of attributes.
USD_INTEROP_API size_t HioGlslfx_GetAttributeCount(HioGlslfxRef glslfx);

/// Gets attribute names.
/// outNames must have space for maxCount pointers.
/// Caller must free each string with Hio_FreeString.
USD_INTEROP_API size_t HioGlslfx_GetAttributeNames(HioGlslfxRef glslfx, char** outNames, size_t maxCount);

/// Gets a hash of the Glslfx contents.
USD_INTEROP_API size_t HioGlslfx_GetHash(HioGlslfxRef glslfx);

// ============================================================================
// MARK: - HioFieldTextureData Functions (Volumetric Textures)
// ============================================================================

/// Creates field texture data from a file.
/// Used for OpenVDB and Field3D volume data.
/// - Parameters:
///   - filePath: Path to the volume file
///   - fieldName: Name of the grid/layer
///   - fieldIndex: Partition index
///   - fieldPurpose: Partition name/grouping
///   - targetMemory: Target memory size for resizing (0 for no limit)
/// Returns NULL on failure. Caller must release with HioFieldTextureData_Release.
USD_INTEROP_API HioFieldTextureDataRef HioFieldTextureData_Create(
    const char* filePath,
    const char* fieldName,
    int fieldIndex,
    const char* fieldPurpose,
    size_t targetMemory
);

/// Retains a field texture data reference.
USD_INTEROP_API HioFieldTextureDataRef HioFieldTextureData_Retain(HioFieldTextureDataRef fieldData);

/// Releases a field texture data reference.
USD_INTEROP_API void HioFieldTextureData_Release(HioFieldTextureDataRef fieldData);

/// Gets the format of the field texture data.
USD_INTEROP_API HioFormat HioFieldTextureData_GetFormat(HioFieldTextureDataRef fieldData);

/// Gets the resized width.
USD_INTEROP_API int HioFieldTextureData_GetResizedWidth(HioFieldTextureDataRef fieldData);

/// Gets the resized height.
USD_INTEROP_API int HioFieldTextureData_GetResizedHeight(HioFieldTextureDataRef fieldData);

/// Gets the resized depth.
USD_INTEROP_API int HioFieldTextureData_GetResizedDepth(HioFieldTextureDataRef fieldData);

/// Gets the bounding box min point.
USD_INTEROP_API void HioFieldTextureData_GetBoundingBoxMin(
    HioFieldTextureDataRef fieldData,
    double* outX, double* outY, double* outZ
);

/// Gets the bounding box max point.
USD_INTEROP_API void HioFieldTextureData_GetBoundingBoxMax(
    HioFieldTextureDataRef fieldData,
    double* outX, double* outY, double* outZ
);

/// Checks if raw buffer is available.
USD_INTEROP_API bool HioFieldTextureData_HasRawBuffer(HioFieldTextureDataRef fieldData);

/// Gets the raw buffer pointer and size.
/// Returns false if no raw buffer is available.
USD_INTEROP_API bool HioFieldTextureData_GetRawBuffer(
    HioFieldTextureDataRef fieldData,
    const void** outBuffer,
    size_t* outSize
);

/// Reads the field texture data.
USD_INTEROP_API UsdResult HioFieldTextureData_Read(HioFieldTextureDataRef fieldData);

// ============================================================================
// MARK: - Format Utility Functions
// ============================================================================

/// Gets a format from component count and type.
USD_INTEROP_API HioFormat HioGetFormat(uint32_t nchannels, HioType type, bool isSRGB);

/// Gets the type from a format.
USD_INTEROP_API HioType HioGetType(HioFormat format);

/// Gets the component count from a format.
USD_INTEROP_API int HioGetComponentCount(HioFormat format);

/// Gets the data size of a single element for the format.
USD_INTEROP_API size_t HioGetDataSizeOfFormat(HioFormat format);

/// Checks if the format is compressed.
USD_INTEROP_API bool HioIsCompressed(HioFormat format);

/// Gets the total data size for an image with given dimensions and format.
USD_INTEROP_API size_t HioGetDataSize(HioFormat format, int width, int height, int depth);

// ============================================================================
// MARK: - Image Registry Functions
// ============================================================================

/// Returns true if the filename has a supported image extension.
USD_INTEROP_API bool Hio_IsSupportedImageFile(const char* filename);

/// Gets the number of supported image format extensions.
USD_INTEROP_API size_t Hio_GetSupportedImageFormatCount(void);

/// Gets supported image format extensions.
/// outExtensions must have space for maxCount strings.
/// Caller must free each string with Hio_FreeString.
USD_INTEROP_API size_t Hio_GetSupportedImageFormats(char** outExtensions, size_t maxCount);

// ============================================================================
// MARK: - Memory Management
// ============================================================================

/// Frees a string allocated by Hio functions.
USD_INTEROP_API void Hio_FreeString(char* str);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_HIO_H
