// hgi.h - Hgi C wrapper
// Mirrors: pxr/imaging/hgi/*.h (Hydra Graphics Interface - GPU abstraction)

#ifndef OPENUSDINTEROP_IMAGING_HGI_H
#define OPENUSDINTEROP_IMAGING_HGI_H

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

/// Opaque handle for Hgi (main graphics interface)
typedef struct HgiOpaque* HgiRef;

/// Opaque handle for HgiBuffer
typedef struct HgiBufferOpaque* HgiBufferRef;

/// Opaque handle for HgiTexture
typedef struct HgiTextureOpaque* HgiTextureRef;

/// Opaque handle for HgiSampler
typedef struct HgiSamplerOpaque* HgiSamplerRef;

/// Opaque handle for HgiShaderFunction
typedef struct HgiShaderFunctionOpaque* HgiShaderFunctionRef;

/// Opaque handle for HgiShaderProgram
typedef struct HgiShaderProgramOpaque* HgiShaderProgramRef;

/// Opaque handle for HgiGraphicsPipeline
typedef struct HgiGraphicsPipelineOpaque* HgiGraphicsPipelineRef;

/// Opaque handle for HgiComputePipeline
typedef struct HgiComputePipelineOpaque* HgiComputePipelineRef;

/// Opaque handle for HgiResourceBindings
typedef struct HgiResourceBindingsOpaque* HgiResourceBindingsRef;

/// Opaque handle for HgiBlitCmds
typedef struct HgiBlitCmdsOpaque* HgiBlitCmdsRef;

/// Opaque handle for HgiGraphicsCmds
typedef struct HgiGraphicsCmdsOpaque* HgiGraphicsCmdsRef;

/// Opaque handle for HgiComputeCmds
typedef struct HgiComputeCmdsOpaque* HgiComputeCmdsRef;

/// Opaque handle for HgiCapabilities
typedef struct HgiCapabilitiesOpaque* HgiCapabilitiesRef;

// ============================================================================
// MARK: - Format Enums (mirrors pxr::HgiFormat)
// ============================================================================

/// Pixel/vertex format enumeration
typedef enum {
    HGI_FORMAT_INVALID = -1,

    // UNorm8 formats
    HGI_FORMAT_UNORM8 = 0,
    HGI_FORMAT_UNORM8_VEC2,
    HGI_FORMAT_UNORM8_VEC4,

    // SNorm8 formats
    HGI_FORMAT_SNORM8,
    HGI_FORMAT_SNORM8_VEC2,
    HGI_FORMAT_SNORM8_VEC4,

    // Float16 formats
    HGI_FORMAT_FLOAT16,
    HGI_FORMAT_FLOAT16_VEC2,
    HGI_FORMAT_FLOAT16_VEC3,
    HGI_FORMAT_FLOAT16_VEC4,

    // Float32 formats
    HGI_FORMAT_FLOAT32,
    HGI_FORMAT_FLOAT32_VEC2,
    HGI_FORMAT_FLOAT32_VEC3,
    HGI_FORMAT_FLOAT32_VEC4,

    // Int16 formats
    HGI_FORMAT_INT16,
    HGI_FORMAT_INT16_VEC2,
    HGI_FORMAT_INT16_VEC3,
    HGI_FORMAT_INT16_VEC4,

    // UInt16 formats
    HGI_FORMAT_UINT16,
    HGI_FORMAT_UINT16_VEC2,
    HGI_FORMAT_UINT16_VEC3,
    HGI_FORMAT_UINT16_VEC4,

    // Int32 formats
    HGI_FORMAT_INT32,
    HGI_FORMAT_INT32_VEC2,
    HGI_FORMAT_INT32_VEC3,
    HGI_FORMAT_INT32_VEC4,

    // sRGB format
    HGI_FORMAT_UNORM8_VEC4_SRGB,

    // Compressed formats
    HGI_FORMAT_BC6_FLOAT_VEC3,
    HGI_FORMAT_BC6_UFLOAT_VEC3,
    HGI_FORMAT_BC7_UNORM8_VEC4,
    HGI_FORMAT_BC7_UNORM8_VEC4_SRGB,
    HGI_FORMAT_BC1_UNORM8_VEC4,
    HGI_FORMAT_BC3_UNORM8_VEC4,

    // Depth/stencil
    HGI_FORMAT_FLOAT32_UINT8,

    // Packed format
    HGI_FORMAT_PACKED_INT1010102,

    HGI_FORMAT_COUNT
} HgiFormat;

// ============================================================================
// MARK: - Texture Enums
// ============================================================================

/// Texture type enumeration
typedef enum {
    HGI_TEXTURE_TYPE_1D = 0,
    HGI_TEXTURE_TYPE_2D,
    HGI_TEXTURE_TYPE_3D,
    HGI_TEXTURE_TYPE_CUBEMAP,
    HGI_TEXTURE_TYPE_1D_ARRAY,
    HGI_TEXTURE_TYPE_2D_ARRAY,
    HGI_TEXTURE_TYPE_COUNT
} HgiTextureType;

/// Texture usage bits
typedef enum {
    HGI_TEXTURE_USAGE_COLOR_TARGET   = 1 << 0,
    HGI_TEXTURE_USAGE_DEPTH_TARGET   = 1 << 1,
    HGI_TEXTURE_USAGE_STENCIL_TARGET = 1 << 2,
    HGI_TEXTURE_USAGE_SHADER_READ    = 1 << 3,
    HGI_TEXTURE_USAGE_SHADER_WRITE   = 1 << 4
} HgiTextureUsageBits;
typedef uint32_t HgiTextureUsage;

/// Component swizzle
typedef enum {
    HGI_COMPONENT_SWIZZLE_ZERO = 0,
    HGI_COMPONENT_SWIZZLE_ONE,
    HGI_COMPONENT_SWIZZLE_R,
    HGI_COMPONENT_SWIZZLE_G,
    HGI_COMPONENT_SWIZZLE_B,
    HGI_COMPONENT_SWIZZLE_A,
    HGI_COMPONENT_SWIZZLE_COUNT
} HgiComponentSwizzle;

/// Sample count for multi-sampling
typedef enum {
    HGI_SAMPLE_COUNT_1  = 1,
    HGI_SAMPLE_COUNT_2  = 2,
    HGI_SAMPLE_COUNT_4  = 4,
    HGI_SAMPLE_COUNT_8  = 8,
    HGI_SAMPLE_COUNT_16 = 16
} HgiSampleCount;

// ============================================================================
// MARK: - Buffer Enums
// ============================================================================

/// Buffer usage bits
typedef enum {
    HGI_BUFFER_USAGE_UNIFORM  = 1 << 0,
    HGI_BUFFER_USAGE_INDEX32  = 1 << 1,
    HGI_BUFFER_USAGE_VERTEX   = 1 << 2,
    HGI_BUFFER_USAGE_STORAGE  = 1 << 3,
    HGI_BUFFER_USAGE_INDIRECT = 1 << 4
} HgiBufferUsageBits;
typedef uint32_t HgiBufferUsage;

// ============================================================================
// MARK: - Sampler Enums
// ============================================================================

/// Sampler address mode
typedef enum {
    HGI_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE = 0,
    HGI_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
    HGI_SAMPLER_ADDRESS_MODE_REPEAT,
    HGI_SAMPLER_ADDRESS_MODE_MIRROR_REPEAT,
    HGI_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    HGI_SAMPLER_ADDRESS_MODE_COUNT
} HgiSamplerAddressMode;

/// Sampler filter
typedef enum {
    HGI_SAMPLER_FILTER_NEAREST = 0,
    HGI_SAMPLER_FILTER_LINEAR  = 1,
    HGI_SAMPLER_FILTER_COUNT
} HgiSamplerFilter;

/// Mip filter
typedef enum {
    HGI_MIP_FILTER_NOT_MIPMAPPED = 0,
    HGI_MIP_FILTER_NEAREST       = 1,
    HGI_MIP_FILTER_LINEAR        = 2,
    HGI_MIP_FILTER_COUNT
} HgiMipFilter;

/// Border color
typedef enum {
    HGI_BORDER_COLOR_TRANSPARENT_BLACK = 0,
    HGI_BORDER_COLOR_OPAQUE_BLACK      = 1,
    HGI_BORDER_COLOR_OPAQUE_WHITE      = 2,
    HGI_BORDER_COLOR_COUNT
} HgiBorderColor;

/// Compare function
typedef enum {
    HGI_COMPARE_FUNCTION_NEVER = 0,
    HGI_COMPARE_FUNCTION_LESS,
    HGI_COMPARE_FUNCTION_EQUAL,
    HGI_COMPARE_FUNCTION_LEQUAL,
    HGI_COMPARE_FUNCTION_GREATER,
    HGI_COMPARE_FUNCTION_NOT_EQUAL,
    HGI_COMPARE_FUNCTION_GEQUAL,
    HGI_COMPARE_FUNCTION_ALWAYS,
    HGI_COMPARE_FUNCTION_COUNT
} HgiCompareFunction;

// ============================================================================
// MARK: - Shader Enums
// ============================================================================

/// Shader stage bits
typedef enum {
    HGI_SHADER_STAGE_VERTEX                   = 1 << 0,
    HGI_SHADER_STAGE_FRAGMENT                 = 1 << 1,
    HGI_SHADER_STAGE_COMPUTE                  = 1 << 2,
    HGI_SHADER_STAGE_TESSELLATION_CONTROL     = 1 << 3,
    HGI_SHADER_STAGE_TESSELLATION_EVAL        = 1 << 4,
    HGI_SHADER_STAGE_GEOMETRY                 = 1 << 5,
    HGI_SHADER_STAGE_POST_TESSELLATION_CONTROL = 1 << 6,
    HGI_SHADER_STAGE_POST_TESSELLATION_VERTEX = 1 << 7
} HgiShaderStageBits;
typedef uint32_t HgiShaderStage;

// ============================================================================
// MARK: - Pipeline Enums
// ============================================================================

/// Primitive type
typedef enum {
    HGI_PRIMITIVE_TYPE_POINT_LIST = 0,
    HGI_PRIMITIVE_TYPE_LINE_LIST,
    HGI_PRIMITIVE_TYPE_LINE_STRIP,
    HGI_PRIMITIVE_TYPE_TRIANGLE_LIST,
    HGI_PRIMITIVE_TYPE_PATCH_LIST,
    HGI_PRIMITIVE_TYPE_LINE_LIST_WITH_ADJACENCY,
    HGI_PRIMITIVE_TYPE_COUNT
} HgiPrimitiveType;

/// Polygon mode
typedef enum {
    HGI_POLYGON_MODE_FILL = 0,
    HGI_POLYGON_MODE_LINE,
    HGI_POLYGON_MODE_POINT,
    HGI_POLYGON_MODE_COUNT
} HgiPolygonMode;

/// Cull mode
typedef enum {
    HGI_CULL_MODE_NONE = 0,
    HGI_CULL_MODE_FRONT,
    HGI_CULL_MODE_BACK,
    HGI_CULL_MODE_FRONT_AND_BACK,
    HGI_CULL_MODE_COUNT
} HgiCullMode;

/// Winding order
typedef enum {
    HGI_WINDING_CLOCKWISE = 0,
    HGI_WINDING_COUNTER_CLOCKWISE,
    HGI_WINDING_COUNT
} HgiWinding;

/// Blend operation
typedef enum {
    HGI_BLEND_OP_ADD = 0,
    HGI_BLEND_OP_SUBTRACT,
    HGI_BLEND_OP_REVERSE_SUBTRACT,
    HGI_BLEND_OP_MIN,
    HGI_BLEND_OP_MAX,
    HGI_BLEND_OP_COUNT
} HgiBlendOp;

/// Blend factor
typedef enum {
    HGI_BLEND_FACTOR_ZERO = 0,
    HGI_BLEND_FACTOR_ONE,
    HGI_BLEND_FACTOR_SRC_COLOR,
    HGI_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    HGI_BLEND_FACTOR_DST_COLOR,
    HGI_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    HGI_BLEND_FACTOR_SRC_ALPHA,
    HGI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    HGI_BLEND_FACTOR_DST_ALPHA,
    HGI_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    HGI_BLEND_FACTOR_CONSTANT_COLOR,
    HGI_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    HGI_BLEND_FACTOR_CONSTANT_ALPHA,
    HGI_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    HGI_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    HGI_BLEND_FACTOR_SRC1_COLOR,
    HGI_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    HGI_BLEND_FACTOR_SRC1_ALPHA,
    HGI_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,
    HGI_BLEND_FACTOR_COUNT
} HgiBlendFactor;

/// Color mask bits
typedef enum {
    HGI_COLOR_MASK_RED   = 1 << 0,
    HGI_COLOR_MASK_GREEN = 1 << 1,
    HGI_COLOR_MASK_BLUE  = 1 << 2,
    HGI_COLOR_MASK_ALPHA = 1 << 3
} HgiColorMaskBits;
typedef uint32_t HgiColorMask;

/// Stencil operation
typedef enum {
    HGI_STENCIL_OP_KEEP = 0,
    HGI_STENCIL_OP_ZERO,
    HGI_STENCIL_OP_REPLACE,
    HGI_STENCIL_OP_INCREMENT_CLAMP,
    HGI_STENCIL_OP_DECREMENT_CLAMP,
    HGI_STENCIL_OP_INVERT,
    HGI_STENCIL_OP_INCREMENT_WRAP,
    HGI_STENCIL_OP_DECREMENT_WRAP,
    HGI_STENCIL_OP_COUNT
} HgiStencilOp;

/// Attachment load operation
typedef enum {
    HGI_ATTACHMENT_LOAD_OP_DONT_CARE = 0,
    HGI_ATTACHMENT_LOAD_OP_CLEAR,
    HGI_ATTACHMENT_LOAD_OP_LOAD,
    HGI_ATTACHMENT_LOAD_OP_COUNT
} HgiAttachmentLoadOp;

/// Attachment store operation
typedef enum {
    HGI_ATTACHMENT_STORE_OP_DONT_CARE = 0,
    HGI_ATTACHMENT_STORE_OP_STORE,
    HGI_ATTACHMENT_STORE_OP_COUNT
} HgiAttachmentStoreOp;

/// Submit wait type
typedef enum {
    HGI_SUBMIT_WAIT_TYPE_NO_WAIT = 0,
    HGI_SUBMIT_WAIT_TYPE_WAIT_UNTIL_COMPLETED
} HgiSubmitWaitType;

/// Device capabilities bits
typedef enum {
    HGI_DEVICE_CAPABILITIES_PRESENTATION             = 1 << 0,
    HGI_DEVICE_CAPABILITIES_BINDLESS_BUFFERS         = 1 << 1,
    HGI_DEVICE_CAPABILITIES_CONCURRENT_DISPATCH      = 1 << 2,
    HGI_DEVICE_CAPABILITIES_UNIFIED_MEMORY           = 1 << 3,
    HGI_DEVICE_CAPABILITIES_BUILTIN_BARYCENTRICS     = 1 << 4,
    HGI_DEVICE_CAPABILITIES_SHADER_DRAW_PARAMETERS   = 1 << 5,
    HGI_DEVICE_CAPABILITIES_MULTI_DRAW_INDIRECT      = 1 << 6,
    HGI_DEVICE_CAPABILITIES_BINDLESS_TEXTURES        = 1 << 7,
    HGI_DEVICE_CAPABILITIES_SHADER_DOUBLE_PRECISION  = 1 << 8,
    HGI_DEVICE_CAPABILITIES_DEPTH_RANGE_MINUS_ONE    = 1 << 9,
    HGI_DEVICE_CAPABILITIES_CPP_SHADER_PADDING       = 1 << 10,
    HGI_DEVICE_CAPABILITIES_CONSERVATIVE_RASTER      = 1 << 11,
    HGI_DEVICE_CAPABILITIES_STENCIL_READBACK         = 1 << 12,
    HGI_DEVICE_CAPABILITIES_CUSTOM_DEPTH_RANGE       = 1 << 13,
    HGI_DEVICE_CAPABILITIES_METAL_TESSELLATION       = 1 << 14,
    HGI_DEVICE_CAPABILITIES_INDIRECT_COMMAND_BUFFERS = 1 << 17
} HgiDeviceCapabilitiesBits;
typedef uint32_t HgiDeviceCapabilities;

// ============================================================================
// MARK: - Descriptor Structs
// ============================================================================

/// Component mapping for textures
typedef struct {
    HgiComponentSwizzle r;
    HgiComponentSwizzle g;
    HgiComponentSwizzle b;
    HgiComponentSwizzle a;
} HgiComponentMapping;

/// Buffer descriptor
typedef struct {
    const char* debugName;
    HgiBufferUsage usage;
    size_t byteSize;
    uint32_t vertexStride;
    const void* initialData;
} HgiBufferDesc;

/// Texture descriptor
typedef struct {
    const char* debugName;
    HgiTextureUsage usage;
    HgiFormat format;
    HgiComponentMapping componentMapping;
    HgiTextureType type;
    int32_t width;
    int32_t height;
    int32_t depth;
    uint16_t layerCount;
    uint16_t mipLevels;
    HgiSampleCount sampleCount;
    size_t pixelsByteSize;
    const void* initialData;
} HgiTextureDesc;

/// Sampler descriptor
typedef struct {
    const char* debugName;
    HgiSamplerFilter magFilter;
    HgiSamplerFilter minFilter;
    HgiMipFilter mipFilter;
    HgiSamplerAddressMode addressModeU;
    HgiSamplerAddressMode addressModeV;
    HgiSamplerAddressMode addressModeW;
    HgiBorderColor borderColor;
    bool enableCompare;
    HgiCompareFunction compareFunction;
    uint32_t maxAnisotropy;
} HgiSamplerDesc;

/// Shader function descriptor
typedef struct {
    const char* debugName;
    HgiShaderStage shaderStage;
    const char* shaderCode;
    size_t shaderCodeSize;
} HgiShaderFunctionDesc;

/// Shader program descriptor
typedef struct {
    const char* debugName;
    HgiShaderFunctionRef* shaderFunctions;
    size_t shaderFunctionCount;
} HgiShaderProgramDesc;

/// Attachment descriptor
typedef struct {
    HgiFormat format;
    HgiAttachmentLoadOp loadOp;
    HgiAttachmentStoreOp storeOp;
    float clearColorR;
    float clearColorG;
    float clearColorB;
    float clearColorA;
    float clearDepth;
    uint32_t clearStencil;
    bool blendEnabled;
    HgiBlendFactor srcColorBlendFactor;
    HgiBlendFactor dstColorBlendFactor;
    HgiBlendOp colorBlendOp;
    HgiBlendFactor srcAlphaBlendFactor;
    HgiBlendFactor dstAlphaBlendFactor;
    HgiBlendOp alphaBlendOp;
    HgiColorMask colorMask;
} HgiAttachmentDesc;

/// Stencil state
typedef struct {
    HgiCompareFunction compareFn;
    uint32_t referenceValue;
    HgiStencilOp stencilFailOp;
    HgiStencilOp depthFailOp;
    HgiStencilOp depthStencilPassOp;
    uint32_t readMask;
    uint32_t writeMask;
} HgiStencilState;

/// Depth/stencil state
typedef struct {
    bool depthTestEnabled;
    bool depthWriteEnabled;
    HgiCompareFunction depthCompareFn;
    bool depthBiasEnabled;
    float depthBiasConstantFactor;
    float depthBiasSlopeFactor;
    bool stencilTestEnabled;
    HgiStencilState stencilFront;
    HgiStencilState stencilBack;
} HgiDepthStencilState;

/// Multi-sample state
typedef struct {
    bool multiSampleEnable;
    bool alphaToCoverageEnable;
    bool alphaToOneEnable;
    HgiSampleCount sampleCount;
} HgiMultiSampleState;

/// Rasterization state
typedef struct {
    HgiPolygonMode polygonMode;
    float lineWidth;
    HgiCullMode cullMode;
    HgiWinding winding;
    bool rasterizerEnabled;
    bool depthClampEnabled;
    float depthRangeMin;
    float depthRangeMax;
    bool conservativeRaster;
    size_t numClipDistances;
} HgiRasterizationState;

/// Vertex attribute descriptor
typedef struct {
    HgiFormat format;
    uint32_t offset;
    uint32_t shaderBindLocation;
} HgiVertexAttributeDesc;

/// Vertex buffer descriptor
typedef struct {
    uint32_t bindingIndex;
    HgiVertexAttributeDesc* vertexAttributes;
    size_t vertexAttributeCount;
    uint32_t vertexStride;
} HgiVertexBufferDesc;

/// Graphics pipeline descriptor
typedef struct {
    const char* debugName;
    HgiPrimitiveType primitiveType;
    HgiShaderProgramRef shaderProgram;
    HgiDepthStencilState depthState;
    HgiMultiSampleState multiSampleState;
    HgiRasterizationState rasterizationState;
    HgiVertexBufferDesc* vertexBuffers;
    size_t vertexBufferCount;
    HgiAttachmentDesc* colorAttachmentDescs;
    size_t colorAttachmentCount;
    HgiAttachmentDesc depthAttachmentDesc;
    bool resolveAttachments;
} HgiGraphicsPipelineDesc;

/// Compute pipeline descriptor
typedef struct {
    const char* debugName;
    HgiShaderProgramRef shaderProgram;
    uint32_t shaderConstantsByteSize;
} HgiComputePipelineDesc;

// ============================================================================
// MARK: - Hgi Main Interface
// ============================================================================

/// Creates the platform default Hgi instance.
/// On macOS this returns HgiMetal, on Linux HgiVulkan or HgiGL.
/// Returns NULL on failure. Caller must release with Hgi_Release.
USD_INTEROP_API HgiRef Hgi_CreatePlatformDefaultHgi(void);

/// Creates a named Hgi instance.
/// - Parameters:
///   - hgiType: "OpenGL", "Metal", "Vulkan", or NULL for platform default
/// Returns NULL on failure or if the type is not supported.
USD_INTEROP_API HgiRef Hgi_CreateNamedHgi(const char* hgiType);

/// Retains an Hgi reference.
USD_INTEROP_API HgiRef Hgi_Retain(HgiRef hgi);

/// Releases an Hgi reference.
USD_INTEROP_API void Hgi_Release(HgiRef hgi);

/// Returns true if the backend is supported on current hardware.
USD_INTEROP_API bool Hgi_IsBackendSupported(HgiRef hgi);

/// Checks if a specific backend type is supported.
/// - Parameters:
///   - hgiType: "OpenGL", "Metal", "Vulkan", or NULL for platform default
USD_INTEROP_API bool Hgi_IsSupported(const char* hgiType);

/// Gets the API name (e.g., "OpenGL", "Metal", "Vulkan").
/// Caller must free with Hgi_FreeString.
USD_INTEROP_API UsdResult Hgi_GetAPIName(HgiRef hgi, char** outName);

/// Gets the device capabilities.
/// The returned reference is owned by Hgi and should not be released.
USD_INTEROP_API HgiCapabilitiesRef Hgi_GetCapabilities(HgiRef hgi);

/// Marks the start of a new rendering frame.
USD_INTEROP_API void Hgi_StartFrame(HgiRef hgi);

/// Marks the end of a rendering frame.
USD_INTEROP_API void Hgi_EndFrame(HgiRef hgi);

/// Performs garbage collection of destroyed resources.
USD_INTEROP_API void Hgi_GarbageCollect(HgiRef hgi);

// ============================================================================
// MARK: - HgiCapabilities
// ============================================================================

/// Checks if a capability flag is set.
USD_INTEROP_API bool HgiCapabilities_IsSet(HgiCapabilitiesRef caps, HgiDeviceCapabilities mask);

/// Gets the API version.
USD_INTEROP_API int HgiCapabilities_GetAPIVersion(HgiCapabilitiesRef caps);

/// Gets the shader version.
USD_INTEROP_API int HgiCapabilities_GetShaderVersion(HgiCapabilitiesRef caps);

/// Gets the maximum uniform block size.
USD_INTEROP_API size_t HgiCapabilities_GetMaxUniformBlockSize(HgiCapabilitiesRef caps);

/// Gets the maximum shader storage block size.
USD_INTEROP_API size_t HgiCapabilities_GetMaxShaderStorageBlockSize(HgiCapabilitiesRef caps);

/// Gets the uniform buffer offset alignment.
USD_INTEROP_API size_t HgiCapabilities_GetUniformBufferOffsetAlignment(HgiCapabilitiesRef caps);

/// Gets the maximum number of clip distances.
USD_INTEROP_API size_t HgiCapabilities_GetMaxClipDistances(HgiCapabilitiesRef caps);

// ============================================================================
// MARK: - HgiBuffer
// ============================================================================

/// Creates a GPU buffer.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroyBuffer.
USD_INTEROP_API HgiBufferRef Hgi_CreateBuffer(HgiRef hgi, const HgiBufferDesc* desc);

/// Destroys a GPU buffer.
USD_INTEROP_API void Hgi_DestroyBuffer(HgiRef hgi, HgiBufferRef* bufferHandle);

/// Gets the buffer byte size.
USD_INTEROP_API size_t HgiBuffer_GetByteSizeOfResource(HgiBufferRef buffer);

/// Gets the raw GPU resource handle as uint64_t.
USD_INTEROP_API uint64_t HgiBuffer_GetRawResource(HgiBufferRef buffer);

/// Gets the CPU staging address for buffer uploads.
/// Returns NULL if staging is not available.
USD_INTEROP_API void* HgiBuffer_GetCPUStagingAddress(HgiBufferRef buffer);

/// Gets the buffer usage flags.
USD_INTEROP_API HgiBufferUsage HgiBuffer_GetUsage(HgiBufferRef buffer);

/// Gets the buffer byte size from descriptor.
USD_INTEROP_API size_t HgiBuffer_GetByteSize(HgiBufferRef buffer);

// ============================================================================
// MARK: - HgiTexture
// ============================================================================

/// Creates a GPU texture.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroyTexture.
USD_INTEROP_API HgiTextureRef Hgi_CreateTexture(HgiRef hgi, const HgiTextureDesc* desc);

/// Destroys a GPU texture.
USD_INTEROP_API void Hgi_DestroyTexture(HgiRef hgi, HgiTextureRef* textureHandle);

/// Gets the texture byte size.
USD_INTEROP_API size_t HgiTexture_GetByteSizeOfResource(HgiTextureRef texture);

/// Gets the raw GPU resource handle as uint64_t.
USD_INTEROP_API uint64_t HgiTexture_GetRawResource(HgiTextureRef texture);

/// Gets the texture format.
USD_INTEROP_API HgiFormat HgiTexture_GetFormat(HgiTextureRef texture);

/// Gets the texture dimensions.
USD_INTEROP_API void HgiTexture_GetDimensions(HgiTextureRef texture, int32_t* outWidth, int32_t* outHeight, int32_t* outDepth);

/// Gets the texture type.
USD_INTEROP_API HgiTextureType HgiTexture_GetType(HgiTextureRef texture);

/// Gets the layer count.
USD_INTEROP_API uint16_t HgiTexture_GetLayerCount(HgiTextureRef texture);

/// Gets the mip level count.
USD_INTEROP_API uint16_t HgiTexture_GetMipLevels(HgiTextureRef texture);

/// Gets the sample count.
USD_INTEROP_API HgiSampleCount HgiTexture_GetSampleCount(HgiTextureRef texture);

// ============================================================================
// MARK: - HgiSampler
// ============================================================================

/// Creates a GPU sampler.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroySampler.
USD_INTEROP_API HgiSamplerRef Hgi_CreateSampler(HgiRef hgi, const HgiSamplerDesc* desc);

/// Destroys a GPU sampler.
USD_INTEROP_API void Hgi_DestroySampler(HgiRef hgi, HgiSamplerRef* samplerHandle);

/// Gets the raw GPU resource handle as uint64_t.
USD_INTEROP_API uint64_t HgiSampler_GetRawResource(HgiSamplerRef sampler);

// ============================================================================
// MARK: - HgiShaderFunction
// ============================================================================

/// Creates a shader function.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroyShaderFunction.
USD_INTEROP_API HgiShaderFunctionRef Hgi_CreateShaderFunction(HgiRef hgi, const HgiShaderFunctionDesc* desc);

/// Destroys a shader function.
USD_INTEROP_API void Hgi_DestroyShaderFunction(HgiRef hgi, HgiShaderFunctionRef* shaderFunctionHandle);

/// Returns true if the shader function compiled successfully.
USD_INTEROP_API bool HgiShaderFunction_IsValid(HgiShaderFunctionRef shaderFunction);

/// Gets shader compile errors.
/// Caller must free with Hgi_FreeString.
USD_INTEROP_API UsdResult HgiShaderFunction_GetCompileErrors(HgiShaderFunctionRef shaderFunction, char** outErrors);

/// Gets the shader function byte size.
USD_INTEROP_API size_t HgiShaderFunction_GetByteSizeOfResource(HgiShaderFunctionRef shaderFunction);

/// Gets the raw GPU resource handle as uint64_t.
USD_INTEROP_API uint64_t HgiShaderFunction_GetRawResource(HgiShaderFunctionRef shaderFunction);

// ============================================================================
// MARK: - HgiShaderProgram
// ============================================================================

/// Creates a shader program from shader functions.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroyShaderProgram.
USD_INTEROP_API HgiShaderProgramRef Hgi_CreateShaderProgram(HgiRef hgi, const HgiShaderProgramDesc* desc);

/// Destroys a shader program.
/// Note: This does NOT destroy the shader functions in the program.
USD_INTEROP_API void Hgi_DestroyShaderProgram(HgiRef hgi, HgiShaderProgramRef* shaderProgramHandle);

/// Returns true if the shader program linked successfully.
USD_INTEROP_API bool HgiShaderProgram_IsValid(HgiShaderProgramRef shaderProgram);

/// Gets shader program link errors.
/// Caller must free with Hgi_FreeString.
USD_INTEROP_API UsdResult HgiShaderProgram_GetCompileErrors(HgiShaderProgramRef shaderProgram, char** outErrors);

/// Gets the shader program byte size.
USD_INTEROP_API size_t HgiShaderProgram_GetByteSizeOfResource(HgiShaderProgramRef shaderProgram);

/// Gets the raw GPU resource handle as uint64_t.
USD_INTEROP_API uint64_t HgiShaderProgram_GetRawResource(HgiShaderProgramRef shaderProgram);

// ============================================================================
// MARK: - HgiGraphicsPipeline
// ============================================================================

/// Creates a graphics pipeline.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroyGraphicsPipeline.
USD_INTEROP_API HgiGraphicsPipelineRef Hgi_CreateGraphicsPipeline(HgiRef hgi, const HgiGraphicsPipelineDesc* desc);

/// Destroys a graphics pipeline.
USD_INTEROP_API void Hgi_DestroyGraphicsPipeline(HgiRef hgi, HgiGraphicsPipelineRef* pipelineHandle);

// ============================================================================
// MARK: - HgiComputePipeline
// ============================================================================

/// Creates a compute pipeline.
/// Returns NULL on failure. Caller must destroy with Hgi_DestroyComputePipeline.
USD_INTEROP_API HgiComputePipelineRef Hgi_CreateComputePipeline(HgiRef hgi, const HgiComputePipelineDesc* desc);

/// Destroys a compute pipeline.
USD_INTEROP_API void Hgi_DestroyComputePipeline(HgiRef hgi, HgiComputePipelineRef* pipelineHandle);

// ============================================================================
// MARK: - HgiBlitCmds
// ============================================================================

/// Creates a blit command encoder.
/// Returns NULL on failure. Automatically submitted when released.
USD_INTEROP_API HgiBlitCmdsRef Hgi_CreateBlitCmds(HgiRef hgi);

/// Releases a blit command encoder (and submits if not already submitted).
USD_INTEROP_API void HgiBlitCmds_Release(HgiBlitCmdsRef blitCmds);

/// Returns true if the commands have been submitted.
USD_INTEROP_API bool HgiBlitCmds_IsSubmitted(HgiBlitCmdsRef blitCmds);

/// Copies data from CPU to a GPU buffer.
USD_INTEROP_API void HgiBlitCmds_CopyBufferCpuToGpu(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef dstBuffer,
    size_t dstByteOffset,
    const void* srcData,
    size_t srcByteSize
);

/// Copies data from GPU buffer to CPU.
/// Caller must provide a pre-allocated destination buffer.
USD_INTEROP_API void HgiBlitCmds_CopyBufferGpuToCpu(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef srcBuffer,
    size_t srcByteOffset,
    void* dstData,
    size_t dstByteSize
);

/// Copies data from one GPU buffer to another.
USD_INTEROP_API void HgiBlitCmds_CopyBufferGpuToGpu(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef srcBuffer,
    size_t srcByteOffset,
    HgiBufferRef dstBuffer,
    size_t dstByteOffset,
    size_t byteSize
);

/// Copies data from CPU to a GPU texture.
USD_INTEROP_API void HgiBlitCmds_CopyTextureCpuToGpu(
    HgiBlitCmdsRef blitCmds,
    HgiTextureRef dstTexture,
    uint16_t dstMipLevel,
    uint16_t dstLayer,
    const void* srcData,
    size_t srcByteSize
);

/// Generates mipmaps for a texture.
USD_INTEROP_API void HgiBlitCmds_GenerateMipMaps(HgiBlitCmdsRef blitCmds, HgiTextureRef texture);

/// Fills a buffer with a constant value.
USD_INTEROP_API void HgiBlitCmds_FillBuffer(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef buffer,
    uint8_t value
);

// ============================================================================
// MARK: - Command Submission
// ============================================================================

/// Submits commands to the GPU.
/// - Parameters:
///   - cmds: The command buffer (BlitCmds, GraphicsCmds, or ComputeCmds)
///   - wait: Whether to wait for completion
USD_INTEROP_API UsdResult Hgi_SubmitCmds(HgiRef hgi, void* cmds, HgiSubmitWaitType wait);

// ============================================================================
// MARK: - Format Utilities
// ============================================================================

/// Gets the number of components in a format.
USD_INTEROP_API size_t HgiFormat_GetComponentCount(HgiFormat format);

/// Gets the byte size of a single element of the format.
USD_INTEROP_API size_t HgiFormat_GetDataSize(HgiFormat format);

/// Returns true if the format is compressed.
USD_INTEROP_API bool HgiFormat_IsCompressed(HgiFormat format);

/// Returns true if the format is a floating-point type.
USD_INTEROP_API bool HgiFormat_IsFloat(HgiFormat format);

// ============================================================================
// MARK: - Memory Management
// ============================================================================

/// Frees a string allocated by Hgi functions.
USD_INTEROP_API void Hgi_FreeString(char* str);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_HGI_H
