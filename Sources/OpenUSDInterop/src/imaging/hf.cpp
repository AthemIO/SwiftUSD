// hf.cpp - Hf C wrapper implementation
// Mirrors: pxr/imaging/hf/*.h (Hydra plugin foundation - plugin registry)

#include "../../include/imaging/hf.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define HF_USE_USD 1
    #include <pxr/imaging/hf/pluginRegistry.h>
    #include <pxr/imaging/hf/pluginDesc.h>
    #include <pxr/imaging/hf/pluginBase.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define HF_USE_USD 0
#endif

#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <atomic>
#include <algorithm>

// ============================================================================
// MARK: - Helper Functions
// ============================================================================

static char* DuplicateString(const std::string& str) {
    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

// ============================================================================
// MARK: - Plugin Description Wrapper
// ============================================================================

struct HfPluginDescOpaque {
    std::atomic<int> refCount;
    std::string id;
    std::string displayName;
    int priority;

    HfPluginDescOpaque() : refCount(1), priority(0) {}

    HfPluginDescOpaque(const std::string& _id, const std::string& _displayName, int _priority)
        : refCount(1), id(_id), displayName(_displayName), priority(_priority) {}

#if HF_USE_USD
    HfPluginDescOpaque(const HfPluginDesc& desc)
        : refCount(1)
        , id(desc.id.GetString())
        , displayName(desc.displayName)
        , priority(desc.priority) {}
#endif
};

// ============================================================================
// MARK: - Plugin Description Vector Wrapper
// ============================================================================

struct HfPluginDescVectorOpaque {
    std::atomic<int> refCount;
    std::vector<HfPluginDescOpaque> descs;

    HfPluginDescVectorOpaque() : refCount(1) {}
};

extern "C" {

// ============================================================================
// MARK: - Plugin Description Lifecycle
// ============================================================================

HfPluginDescRef HfPluginDesc_Create(
    const char* id,
    const char* displayName,
    int priority
) {
    try {
        return new HfPluginDescOpaque(
            id ? id : "",
            displayName ? displayName : "",
            priority
        );
    } catch (...) {
        return nullptr;
    }
}

HfPluginDescRef HfPluginDesc_Retain(HfPluginDescRef desc) {
    if (desc) {
        desc->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return desc;
}

void HfPluginDesc_Release(HfPluginDescRef desc) {
    if (desc) {
        if (desc->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete desc;
        }
    }
}

// ============================================================================
// MARK: - Plugin Description Accessors
// ============================================================================

const char* HfPluginDesc_GetId(HfPluginDescRef desc) {
    if (!desc) return "";
    return desc->id.c_str();
}

const char* HfPluginDesc_GetDisplayName(HfPluginDescRef desc) {
    if (!desc) return "";
    return desc->displayName.c_str();
}

int HfPluginDesc_GetPriority(HfPluginDescRef desc) {
    if (!desc) return 0;
    return desc->priority;
}

UsdResult HfPluginDesc_GetData(HfPluginDescRef desc, HfPluginDescData* outData) {
    if (!desc || !outData) {
        return USD_RESULT_INVALID_ARGUMENT;
    }

    outData->id = desc->id.c_str();
    outData->displayName = desc->displayName.c_str();
    outData->priority = desc->priority;

    return USD_RESULT_SUCCESS;
}

// ============================================================================
// MARK: - Plugin Description Vector Lifecycle
// ============================================================================

HfPluginDescVectorRef HfPluginDescVector_Create(void) {
    try {
        return new HfPluginDescVectorOpaque();
    } catch (...) {
        return nullptr;
    }
}

HfPluginDescVectorRef HfPluginDescVector_Retain(HfPluginDescVectorRef vec) {
    if (vec) {
        vec->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return vec;
}

void HfPluginDescVector_Release(HfPluginDescVectorRef vec) {
    if (vec) {
        if (vec->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete vec;
        }
    }
}

// ============================================================================
// MARK: - Plugin Description Vector Operations
// ============================================================================

size_t HfPluginDescVector_GetCount(HfPluginDescVectorRef vec) {
    if (!vec) return 0;
    return vec->descs.size();
}

UsdResult HfPluginDescVector_GetAt(
    HfPluginDescVectorRef vec,
    size_t index,
    HfPluginDescData* outData
) {
    if (!vec || !outData) {
        return USD_RESULT_INVALID_ARGUMENT;
    }
    if (index >= vec->descs.size()) {
        return USD_RESULT_NOT_FOUND;
    }

    const auto& desc = vec->descs[index];
    outData->id = desc.id.c_str();
    outData->displayName = desc.displayName.c_str();
    outData->priority = desc.priority;

    return USD_RESULT_SUCCESS;
}

size_t HfPluginDescVector_GetAll(
    HfPluginDescVectorRef vec,
    HfPluginDescData* outData,
    size_t maxCount
) {
    if (!vec || !outData || maxCount == 0) return 0;

    size_t count = std::min(maxCount, vec->descs.size());
    for (size_t i = 0; i < count; ++i) {
        const auto& desc = vec->descs[i];
        outData[i].id = desc.id.c_str();
        outData[i].displayName = desc.displayName.c_str();
        outData[i].priority = desc.priority;
    }
    return count;
}

UsdResult HfPluginDescVector_Append(
    HfPluginDescVectorRef vec,
    const char* id,
    const char* displayName,
    int priority
) {
    if (!vec) return USD_RESULT_INVALID_ARGUMENT;

    try {
        vec->descs.emplace_back(
            id ? id : "",
            displayName ? displayName : "",
            priority
        );
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_OUT_OF_MEMORY;
    }
}

void HfPluginDescVector_Clear(HfPluginDescVectorRef vec) {
    if (vec) {
        vec->descs.clear();
    }
}

void HfPluginDescVector_Reserve(HfPluginDescVectorRef vec, size_t capacity) {
    if (vec) {
        try {
            vec->descs.reserve(capacity);
        } catch (...) {
            // Ignore allocation failures for reserve
        }
    }
}

// ============================================================================
// MARK: - Plugin Registry Query Functions
// ============================================================================

bool Hf_IsPluginRegistered(const char* registryType, const char* pluginId) {
    if (!registryType || !pluginId) return false;

#if HF_USE_USD
    // Note: HfPluginRegistry is a base class and cannot be instantiated directly.
    // Specific registries like HdRendererPluginRegistry inherit from it.
    // For general queries, we can't easily check without knowing the concrete type.
    // This would typically be done through specific registry singletons.
    //
    // For now, we return false as we don't have access to concrete registries here.
    // Users should use specific registry APIs (e.g., HdRendererPluginRegistry).
    return false;
#else
    // Standalone mode: no plugins registered
    return false;
#endif
}

size_t Hf_GetRegisteredPluginCount(const char* registryType) {
    if (!registryType) return 0;

#if HF_USE_USD
    // Same note as above - we can't query without a concrete registry.
    return 0;
#else
    // Standalone mode: no plugins registered
    return 0;
#endif
}

UsdResult Hf_GetRegisteredPlugins(
    const char* registryType,
    HfPluginDescVectorRef outPlugins
) {
    if (!registryType || !outPlugins) {
        return USD_RESULT_INVALID_ARGUMENT;
    }

#if HF_USE_USD
    // Same note as above - we can't query without a concrete registry.
    // Clear the output vector and return success (empty list).
    outPlugins->descs.clear();
    return USD_RESULT_SUCCESS;
#else
    // Standalone mode: no plugins, return empty vector
    outPlugins->descs.clear();
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - String Management
// ============================================================================

void Hf_FreeString(char* str) {
    free(str);
}

char* Hf_DuplicateString(const char* str) {
    if (!str) return nullptr;
    return DuplicateString(str);
}

} // extern "C"
