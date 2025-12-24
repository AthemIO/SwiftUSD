// hgiMetal.cpp - HgiMetal C wrapper implementation
// Mirrors: pxr/imaging/hgiMetal/*.h (Metal implementation of Hgi interfaces)

#include "../../include/imaging/hgiMetal.h"

// Check if we should use full USD library and if we're on Apple platforms
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD && (defined(__APPLE__) || defined(__MACH__))
    #define HGIMETAL_USE_USD 1
    #include <pxr/imaging/hgiMetal/hgi.h>
    #include <pxr/imaging/hgiMetal/capabilities.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define HGIMETAL_USE_USD 0
#endif

#include <cstring>
#include <cstdlib>
#include <string>
#include <atomic>
#include <memory>

// Helper to duplicate a std::string as a C string
static char* DuplicateString(const std::string& str) {
    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

// ============================================================================
// MARK: - Wrapper Structs
// ============================================================================

struct HgiMetalCapabilitiesOpaque {
#if HGIMETAL_USE_USD
    HgiMetalCapabilities* caps;
#endif
    uint32_t defaultStorageMode;
    bool hasVertexMemoryBarrier;
    bool useParallelEncoder;
    bool requiresIndirectDrawFix;
    bool requiresReturnAfterDiscard;
    int apiVersion;
    int shaderVersion;

    HgiMetalCapabilitiesOpaque()
        : defaultStorageMode(0)
        , hasVertexMemoryBarrier(true)
        , useParallelEncoder(false)
        , requiresIndirectDrawFix(false)
        , requiresReturnAfterDiscard(false)
        , apiVersion(2)  // Default to Metal 2.0
        , shaderVersion(200)
    {
#if HGIMETAL_USE_USD
        caps = nullptr;
#endif
    }
};

struct HgiMetalOpaque {
    std::atomic<int> refCount;
#if HGIMETAL_USE_USD
    std::unique_ptr<HgiMetal> hgiMetal;
#endif
    HgiMetalCapabilitiesOpaque capabilities;
    int apiVersion;

    HgiMetalOpaque()
        : refCount(1)
        , apiVersion(HGI_METAL_API_VERSION_2_0)
    {}
};

extern "C" {

// ============================================================================
// MARK: - HgiMetal Lifecycle
// ============================================================================

HgiMetalRef HgiMetal_Create(void) {
#if HGIMETAL_USE_USD
    try {
        auto* wrapper = new HgiMetalOpaque();
        wrapper->hgiMetal = std::make_unique<HgiMetal>();
        if (!wrapper->hgiMetal) {
            delete wrapper;
            return nullptr;
        }

        // Cache API version
        wrapper->apiVersion = wrapper->hgiMetal->GetAPIVersion();

        // Cache capabilities
        const HgiMetalCapabilities* caps = wrapper->hgiMetal->GetCapabilities();
        if (caps) {
            wrapper->capabilities.caps = const_cast<HgiMetalCapabilities*>(caps);
            wrapper->capabilities.defaultStorageMode = caps->defaultStorageMode;
            wrapper->capabilities.hasVertexMemoryBarrier = caps->hasVertexMemoryBarrier;
            wrapper->capabilities.useParallelEncoder = caps->useParallelEncoder;
            wrapper->capabilities.requiresIndirectDrawFix = caps->requiresIndirectDrawFix;
            wrapper->capabilities.requiresReturnAfterDiscard = caps->requiresReturnAfterDiscard;
            wrapper->capabilities.apiVersion = caps->GetAPIVersion();
            wrapper->capabilities.shaderVersion = caps->GetShaderVersion();
        }

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub HgiMetal
    // Metal is only available on Apple platforms
#if defined(__APPLE__) || defined(__MACH__)
    auto* wrapper = new HgiMetalOpaque();
    wrapper->apiVersion = HGI_METAL_API_VERSION_2_0;
    return wrapper;
#else
    // Not on Apple platform, Metal is not available
    return nullptr;
#endif
#endif
}

HgiMetalRef HgiMetal_CreateWithDevice(void* device) {
#if HGIMETAL_USE_USD
    try {
        auto* wrapper = new HgiMetalOpaque();
        // In real USD, we'd pass the device to the constructor
        // HgiMetal constructor takes an optional id<MTLDevice>
        id<MTLDevice> mtlDevice = device ? (__bridge id<MTLDevice>)device : nil;
        wrapper->hgiMetal = std::make_unique<HgiMetal>(mtlDevice);
        if (!wrapper->hgiMetal) {
            delete wrapper;
            return nullptr;
        }

        wrapper->apiVersion = wrapper->hgiMetal->GetAPIVersion();

        const HgiMetalCapabilities* caps = wrapper->hgiMetal->GetCapabilities();
        if (caps) {
            wrapper->capabilities.caps = const_cast<HgiMetalCapabilities*>(caps);
            wrapper->capabilities.defaultStorageMode = caps->defaultStorageMode;
            wrapper->capabilities.hasVertexMemoryBarrier = caps->hasVertexMemoryBarrier;
            wrapper->capabilities.useParallelEncoder = caps->useParallelEncoder;
            wrapper->capabilities.requiresIndirectDrawFix = caps->requiresIndirectDrawFix;
            wrapper->capabilities.requiresReturnAfterDiscard = caps->requiresReturnAfterDiscard;
            wrapper->capabilities.apiVersion = caps->GetAPIVersion();
            wrapper->capabilities.shaderVersion = caps->GetShaderVersion();
        }

        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode
#if defined(__APPLE__) || defined(__MACH__)
    (void)device; // Unused in standalone mode
    auto* wrapper = new HgiMetalOpaque();
    wrapper->apiVersion = HGI_METAL_API_VERSION_2_0;
    return wrapper;
#else
    (void)device;
    return nullptr;
#endif
#endif
}

HgiMetalRef HgiMetal_Retain(HgiMetalRef hgiMetal) {
    if (hgiMetal) {
        hgiMetal->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return hgiMetal;
}

void HgiMetal_Release(HgiMetalRef hgiMetal) {
    if (hgiMetal) {
        if (hgiMetal->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete hgiMetal;
        }
    }
}

// ============================================================================
// MARK: - HgiMetal Properties
// ============================================================================

bool HgiMetal_IsBackendSupported(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return false;
#if HGIMETAL_USE_USD
    try {
        return hgiMetal->hgiMetal && hgiMetal->hgiMetal->IsBackendSupported();
    } catch (...) {
        return false;
    }
#else
#if defined(__APPLE__) || defined(__MACH__)
    return true; // Assume Metal is supported on Apple platforms
#else
    return false;
#endif
#endif
}

UsdResult HgiMetal_GetAPIName(HgiMetalRef hgiMetal, char** outName) {
    if (!hgiMetal || !outName) return USD_RESULT_INVALID_ARGUMENT;

    *outName = DuplicateString("Metal");
    return *outName ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
}

HgiMetalAPIVersion HgiMetal_GetAPIVersion(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return HGI_METAL_API_VERSION_2_0;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            int version = hgiMetal->hgiMetal->GetAPIVersion();
            if (version >= 3) return HGI_METAL_API_VERSION_3_0;
            if (version >= 2) return HGI_METAL_API_VERSION_2_0;
            return HGI_METAL_API_VERSION_1_0;
        }
    } catch (...) {}
#endif
    return static_cast<HgiMetalAPIVersion>(hgiMetal->apiVersion);
}

HgiMetalCapabilitiesRef HgiMetal_GetCapabilities(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
    return &hgiMetal->capabilities;
}

// ============================================================================
// MARK: - HgiMetal Frame Management
// ============================================================================

void HgiMetal_StartFrame(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            hgiMetal->hgiMetal->StartFrame();
        }
    } catch (...) {}
#endif
}

void HgiMetal_EndFrame(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            hgiMetal->hgiMetal->EndFrame();
        }
    } catch (...) {}
#endif
}

void HgiMetal_GarbageCollect(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            hgiMetal->hgiMetal->GarbageCollect();
        }
    } catch (...) {}
#endif
}

// ============================================================================
// MARK: - HgiMetal Device Access
// ============================================================================

void* HgiMetal_GetPrimaryDevice(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetPrimaryDevice();
        }
    } catch (...) {}
#endif
    return nullptr;
}

void* HgiMetal_GetQueue(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetQueue();
        }
    } catch (...) {}
#endif
    return nullptr;
}

void* HgiMetal_GetPrimaryCommandBuffer(HgiMetalRef hgiMetal, bool flush) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetPrimaryCommandBuffer(nullptr, flush);
        }
    } catch (...) {}
#endif
    return nullptr;
}

void* HgiMetal_GetSecondaryCommandBuffer(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetSecondaryCommandBuffer();
        }
    } catch (...) {}
#endif
    return nullptr;
}

void HgiMetal_ReleaseSecondaryCommandBuffer(HgiMetalRef hgiMetal, void* commandBuffer) {
    if (!hgiMetal || !commandBuffer) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            hgiMetal->hgiMetal->ReleaseSecondaryCommandBuffer((__bridge id<MTLCommandBuffer>)commandBuffer);
        }
    } catch (...) {}
#endif
}

void HgiMetal_SetHasWork(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            hgiMetal->hgiMetal->SetHasWork();
        }
    } catch (...) {}
#endif
}

// ============================================================================
// MARK: - HgiMetal Command Buffer Commits
// ============================================================================

void HgiMetal_CommitPrimaryCommandBuffer(
    HgiMetalRef hgiMetal,
    HgiMetalCommitWaitType waitType,
    bool forceNewBuffer
) {
    if (!hgiMetal) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            HgiMetal::CommitCommandBufferWaitType pxrWaitType;
            switch (waitType) {
                case HGI_METAL_COMMIT_WAIT_UNTIL_SCHEDULED:
                    pxrWaitType = HgiMetal::CommitCommandBuffer_WaitUntilScheduled;
                    break;
                case HGI_METAL_COMMIT_WAIT_UNTIL_COMPLETED:
                    pxrWaitType = HgiMetal::CommitCommandBuffer_WaitUntilCompleted;
                    break;
                default:
                    pxrWaitType = HgiMetal::CommitCommandBuffer_NoWait;
                    break;
            }
            hgiMetal->hgiMetal->CommitPrimaryCommandBuffer(pxrWaitType, forceNewBuffer);
        }
    } catch (...) {}
#endif
}

void HgiMetal_CommitSecondaryCommandBuffer(
    HgiMetalRef hgiMetal,
    void* commandBuffer,
    HgiMetalCommitWaitType waitType
) {
    if (!hgiMetal || !commandBuffer) return;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            HgiMetal::CommitCommandBufferWaitType pxrWaitType;
            switch (waitType) {
                case HGI_METAL_COMMIT_WAIT_UNTIL_SCHEDULED:
                    pxrWaitType = HgiMetal::CommitCommandBuffer_WaitUntilScheduled;
                    break;
                case HGI_METAL_COMMIT_WAIT_UNTIL_COMPLETED:
                    pxrWaitType = HgiMetal::CommitCommandBuffer_WaitUntilCompleted;
                    break;
                default:
                    pxrWaitType = HgiMetal::CommitCommandBuffer_NoWait;
                    break;
            }
            hgiMetal->hgiMetal->CommitSecondaryCommandBuffer(
                (__bridge id<MTLCommandBuffer>)commandBuffer,
                pxrWaitType
            );
        }
    } catch (...) {}
#endif
}

// ============================================================================
// MARK: - HgiMetal Argument Encoders
// ============================================================================

void* HgiMetal_GetBufferArgumentEncoder(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetBufferArgumentEncoder();
        }
    } catch (...) {}
#endif
    return nullptr;
}

void* HgiMetal_GetSamplerArgumentEncoder(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetSamplerArgumentEncoder();
        }
    } catch (...) {}
#endif
    return nullptr;
}

void* HgiMetal_GetTextureArgumentEncoder(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetTextureArgumentEncoder();
        }
    } catch (...) {}
#endif
    return nullptr;
}

void* HgiMetal_GetArgBuffer(HgiMetalRef hgiMetal) {
    if (!hgiMetal) return nullptr;
#if HGIMETAL_USE_USD
    try {
        if (hgiMetal->hgiMetal) {
            return (__bridge void*)hgiMetal->hgiMetal->GetArgBuffer();
        }
    } catch (...) {}
#endif
    return nullptr;
}

// ============================================================================
// MARK: - HgiMetalCapabilities
// ============================================================================

uint32_t HgiMetalCapabilities_GetDefaultStorageMode(HgiMetalCapabilitiesRef caps) {
    if (!caps) return 0;
    return caps->defaultStorageMode;
}

bool HgiMetalCapabilities_HasVertexMemoryBarrier(HgiMetalCapabilitiesRef caps) {
    if (!caps) return false;
    return caps->hasVertexMemoryBarrier;
}

bool HgiMetalCapabilities_UseParallelEncoder(HgiMetalCapabilitiesRef caps) {
    if (!caps) return false;
    return caps->useParallelEncoder;
}

bool HgiMetalCapabilities_RequiresIndirectDrawFix(HgiMetalCapabilitiesRef caps) {
    if (!caps) return false;
    return caps->requiresIndirectDrawFix;
}

bool HgiMetalCapabilities_RequiresReturnAfterDiscard(HgiMetalCapabilitiesRef caps) {
    if (!caps) return false;
    return caps->requiresReturnAfterDiscard;
}

int HgiMetalCapabilities_GetAPIVersion(HgiMetalCapabilitiesRef caps) {
    if (!caps) return 0;
    return caps->apiVersion;
}

int HgiMetalCapabilities_GetShaderVersion(HgiMetalCapabilitiesRef caps) {
    if (!caps) return 0;
    return caps->shaderVersion;
}

// ============================================================================
// MARK: - Memory Management
// ============================================================================

void HgiMetal_FreeString(char* str) {
    free(str);
}

} // extern "C"
