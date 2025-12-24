// hgiMetal.h - HgiMetal C wrapper
// Mirrors: pxr/imaging/hgiMetal/*.h (Metal implementation of Hgi interfaces)

#ifndef OPENUSDINTEROP_IMAGING_HGIMETAL_H
#define OPENUSDINTEROP_IMAGING_HGIMETAL_H

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

/// Opaque handle for HgiMetal (Metal graphics interface)
typedef struct HgiMetalOpaque* HgiMetalRef;

/// Opaque handle for HgiMetalCapabilities
typedef struct HgiMetalCapabilitiesOpaque* HgiMetalCapabilitiesRef;

// ============================================================================
// MARK: - Metal API Version Enum
// ============================================================================

/// Metal API version enumeration
typedef enum {
    HGI_METAL_API_VERSION_1_0 = 0,
    HGI_METAL_API_VERSION_2_0,
    HGI_METAL_API_VERSION_3_0
} HgiMetalAPIVersion;

/// Command buffer wait type for commits
typedef enum {
    HGI_METAL_COMMIT_NO_WAIT = 0,
    HGI_METAL_COMMIT_WAIT_UNTIL_SCHEDULED,
    HGI_METAL_COMMIT_WAIT_UNTIL_COMPLETED
} HgiMetalCommitWaitType;

// ============================================================================
// MARK: - HgiMetal Lifecycle
// ============================================================================

/// Creates an HgiMetal instance.
/// This is the Metal implementation of the Hydra Graphics Interface.
/// Returns NULL on failure or if Metal is not available.
/// Caller must release with HgiMetal_Release.
USD_INTEROP_API HgiMetalRef HgiMetal_Create(void);

/// Creates an HgiMetal instance with a specific Metal device.
/// - Parameters:
///   - device: A pointer to an id<MTLDevice> (pass NULL for default device)
/// Returns NULL on failure.
USD_INTEROP_API HgiMetalRef HgiMetal_CreateWithDevice(void* device);

/// Retains an HgiMetal reference.
USD_INTEROP_API HgiMetalRef HgiMetal_Retain(HgiMetalRef hgiMetal);

/// Releases an HgiMetal reference.
USD_INTEROP_API void HgiMetal_Release(HgiMetalRef hgiMetal);

// ============================================================================
// MARK: - HgiMetal Properties
// ============================================================================

/// Returns true if the Metal backend is supported on current hardware.
USD_INTEROP_API bool HgiMetal_IsBackendSupported(HgiMetalRef hgiMetal);

/// Gets the API name (always "Metal" for HgiMetal).
/// Caller must free with HgiMetal_FreeString.
USD_INTEROP_API UsdResult HgiMetal_GetAPIName(HgiMetalRef hgiMetal, char** outName);

/// Gets the Metal API version.
USD_INTEROP_API HgiMetalAPIVersion HgiMetal_GetAPIVersion(HgiMetalRef hgiMetal);

/// Gets the Metal-specific capabilities.
/// The returned reference is owned by HgiMetal and should not be released.
USD_INTEROP_API HgiMetalCapabilitiesRef HgiMetal_GetCapabilities(HgiMetalRef hgiMetal);

// ============================================================================
// MARK: - HgiMetal Frame Management
// ============================================================================

/// Marks the start of a new rendering frame.
USD_INTEROP_API void HgiMetal_StartFrame(HgiMetalRef hgiMetal);

/// Marks the end of a rendering frame.
USD_INTEROP_API void HgiMetal_EndFrame(HgiMetalRef hgiMetal);

/// Performs garbage collection of destroyed resources.
USD_INTEROP_API void HgiMetal_GarbageCollect(HgiMetalRef hgiMetal);

// ============================================================================
// MARK: - HgiMetal Device Access
// ============================================================================

/// Gets the primary Metal device.
/// Returns a pointer to the id<MTLDevice>.
/// The returned pointer is owned by HgiMetal and should not be released.
USD_INTEROP_API void* HgiMetal_GetPrimaryDevice(HgiMetalRef hgiMetal);

/// Gets the Metal command queue.
/// Returns a pointer to the id<MTLCommandQueue>.
/// The returned pointer is owned by HgiMetal and should not be released.
USD_INTEROP_API void* HgiMetal_GetQueue(HgiMetalRef hgiMetal);

/// Gets the primary command buffer.
/// - Parameters:
///   - flush: If true and there's pending work, flushes the current command buffer
/// Returns a pointer to the id<MTLCommandBuffer>.
USD_INTEROP_API void* HgiMetal_GetPrimaryCommandBuffer(HgiMetalRef hgiMetal, bool flush);

/// Gets a secondary command buffer for concurrent encoding.
/// Returns a pointer to the id<MTLCommandBuffer>.
/// Caller must release with HgiMetal_ReleaseSecondaryCommandBuffer.
USD_INTEROP_API void* HgiMetal_GetSecondaryCommandBuffer(HgiMetalRef hgiMetal);

/// Releases a secondary command buffer.
USD_INTEROP_API void HgiMetal_ReleaseSecondaryCommandBuffer(HgiMetalRef hgiMetal, void* commandBuffer);

/// Indicates that there is work to be flushed from the primary command buffer.
USD_INTEROP_API void HgiMetal_SetHasWork(HgiMetalRef hgiMetal);

// ============================================================================
// MARK: - HgiMetal Command Buffer Commits
// ============================================================================

/// Commits the primary command buffer.
/// - Parameters:
///   - waitType: How to wait for completion
///   - forceNewBuffer: If true, forces creation of a new command buffer
USD_INTEROP_API void HgiMetal_CommitPrimaryCommandBuffer(
    HgiMetalRef hgiMetal,
    HgiMetalCommitWaitType waitType,
    bool forceNewBuffer
);

/// Commits a secondary command buffer.
/// - Parameters:
///   - commandBuffer: The secondary command buffer to commit
///   - waitType: How to wait for completion
USD_INTEROP_API void HgiMetal_CommitSecondaryCommandBuffer(
    HgiMetalRef hgiMetal,
    void* commandBuffer,
    HgiMetalCommitWaitType waitType
);

// ============================================================================
// MARK: - HgiMetal Argument Encoders
// ============================================================================

/// Gets the buffer argument encoder.
/// Returns a pointer to the id<MTLArgumentEncoder>.
USD_INTEROP_API void* HgiMetal_GetBufferArgumentEncoder(HgiMetalRef hgiMetal);

/// Gets the sampler argument encoder.
/// Returns a pointer to the id<MTLArgumentEncoder>.
USD_INTEROP_API void* HgiMetal_GetSamplerArgumentEncoder(HgiMetalRef hgiMetal);

/// Gets the texture argument encoder.
/// Returns a pointer to the id<MTLArgumentEncoder>.
USD_INTEROP_API void* HgiMetal_GetTextureArgumentEncoder(HgiMetalRef hgiMetal);

/// Gets an argument buffer for binding.
/// Returns a pointer to the id<MTLBuffer>.
USD_INTEROP_API void* HgiMetal_GetArgBuffer(HgiMetalRef hgiMetal);

// ============================================================================
// MARK: - HgiMetalCapabilities
// ============================================================================

/// Gets the default storage mode for Metal resources.
/// Returns the MTLResourceOptions value.
USD_INTEROP_API uint32_t HgiMetalCapabilities_GetDefaultStorageMode(HgiMetalCapabilitiesRef caps);

/// Returns true if the device supports vertex memory barriers.
USD_INTEROP_API bool HgiMetalCapabilities_HasVertexMemoryBarrier(HgiMetalCapabilitiesRef caps);

/// Returns true if parallel command encoding should be used.
USD_INTEROP_API bool HgiMetalCapabilities_UseParallelEncoder(HgiMetalCapabilitiesRef caps);

/// Returns true if the indirect draw fix is required.
USD_INTEROP_API bool HgiMetalCapabilities_RequiresIndirectDrawFix(HgiMetalCapabilitiesRef caps);

/// Returns true if return after discard is required.
USD_INTEROP_API bool HgiMetalCapabilities_RequiresReturnAfterDiscard(HgiMetalCapabilitiesRef caps);

/// Gets the API version from capabilities.
USD_INTEROP_API int HgiMetalCapabilities_GetAPIVersion(HgiMetalCapabilitiesRef caps);

/// Gets the shader version from capabilities.
USD_INTEROP_API int HgiMetalCapabilities_GetShaderVersion(HgiMetalCapabilitiesRef caps);

// ============================================================================
// MARK: - Memory Management
// ============================================================================

/// Frees a string allocated by HgiMetal functions.
USD_INTEROP_API void HgiMetal_FreeString(char* str);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_HGIMETAL_H
