// usdUtils.cpp - UsdUtils module C wrapper implementation
// Mirrors: pxr/usd/usdUtils/stageCache.h, flattenLayerStack.h, stitch.h

#include "../../include/usdUtils/usdUtils.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdUtils/stageCache.h>
    #include <pxr/usd/usdUtils/flattenLayerStack.h>
    #include <pxr/usd/usdUtils/stitch.h>
    #include <pxr/usd/usd/stage.h>
    #include <pxr/usd/usd/stageCache.h>
    #include <pxr/usd/sdf/layer.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <mutex>
#include <map>

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - Wrapper Structures (must match those in usd.cpp and sdf.cpp)
// ============================================================================

struct UsdStageOpaque {
#if USD_USE_FULL
    UsdStageRefPtr stage;
#else
    std::string identifier;
    double startTimeCode;
    double endTimeCode;
    double timeCodesPerSecond;
    double framesPerSecond;
#endif
    std::atomic<int> refCount;

    UsdStageOpaque() : refCount(1) {
#if !USD_USE_FULL
        startTimeCode = 0.0;
        endTimeCode = 0.0;
        timeCodesPerSecond = 24.0;
        framesPerSecond = 24.0;
#endif
    }

#if USD_USE_FULL
    explicit UsdStageOpaque(const UsdStageRefPtr& s)
        : stage(s), refCount(1) {}
#endif
};

struct SdfLayerOpaque {
#if USD_USE_FULL
    SdfLayerRefPtr layer;
#else
    std::string identifier;
    std::string displayName;
    std::string documentation;
    std::string comment;
    std::string content;
    std::vector<std::string> subLayerPaths;
    std::vector<std::string> rootPrimNames;
    double startTimeCode;
    double endTimeCode;
    double timeCodesPerSecond;
    double framesPerSecond;
    bool hasStartTimeCode;
    bool hasEndTimeCode;
    bool hasTimeCodesPerSecond;
    bool hasFramesPerSecond;
    bool isAnonymous;
    bool isDirty;
#endif
    std::atomic<int> refCount;

    SdfLayerOpaque()
        : refCount(1)
    {
#if !USD_USE_FULL
        startTimeCode = 0.0;
        endTimeCode = 0.0;
        timeCodesPerSecond = 24.0;
        framesPerSecond = 24.0;
        hasStartTimeCode = false;
        hasEndTimeCode = false;
        hasTimeCodesPerSecond = false;
        hasFramesPerSecond = false;
        isAnonymous = false;
        isDirty = false;
#endif
    }

#if USD_USE_FULL
    explicit SdfLayerOpaque(const SdfLayerRefPtr& l)
        : layer(l)
        , refCount(1)
    {
    }
#endif
};

// ============================================================================
// MARK: - Stub Cache for non-USD builds
// ============================================================================

#if !USD_USE_FULL
// Simple stub cache for testing without Pixar USD
namespace {
    struct StubStageEntry {
        void* stageRef;  // UsdStageRef
        std::string layerPath;
    };

    std::mutex g_stubCacheMutex;
    std::map<int64_t, StubStageEntry> g_stubCache;
    int64_t g_nextStubId = 1;
}
#endif

// ============================================================================
// MARK: - UsdUtilsStageCache Implementation
// ============================================================================

extern "C" {

size_t UsdUtilsStageCache_GetCount(void) {
    try {
#if USD_USE_FULL
        return UsdUtilsStageCache::Get().Size();
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        return g_stubCache.size();
#endif
    }
    CATCH_AND_RETURN(0)
}

UsdStageRef UsdUtilsStageCache_Find(int64_t id) {
    try {
#if USD_USE_FULL
        UsdStageCache::Id cacheId = UsdStageCache::Id::FromLongInt(id);
        UsdStageRefPtr stage = UsdUtilsStageCache::Get().Find(cacheId);
        if (!stage) return nullptr;
        // Create a new wrapper around the stage
        return new UsdStageOpaque(stage);
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        auto it = g_stubCache.find(id);
        if (it == g_stubCache.end()) return nullptr;
        // In stub mode, we return the stored reference (with retain)
        UsdStageRef stageRef = static_cast<UsdStageRef>(it->second.stageRef);
        if (stageRef) {
            UsdStage_Retain(stageRef);
        }
        return stageRef;
#endif
    }
    CATCH_AND_RETURN(nullptr)
}

UsdStageRef UsdUtilsStageCache_FindByLayerPath(const char* layerPath) {
    if (!layerPath) return nullptr;
    try {
#if USD_USE_FULL
        // Get all stages and find the one with matching root layer path
        auto allStages = UsdUtilsStageCache::Get().GetAllStages();
        for (const auto& stage : allStages) {
            if (stage) {
                SdfLayerHandle rootLayer = stage->GetRootLayer();
                if (rootLayer && rootLayer->GetIdentifier() == layerPath) {
                    return new UsdStageOpaque(stage);
                }
            }
        }
        return nullptr;
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        for (const auto& pair : g_stubCache) {
            if (pair.second.layerPath == layerPath) {
                UsdStageRef stageRef = static_cast<UsdStageRef>(pair.second.stageRef);
                if (stageRef) {
                    UsdStage_Retain(stageRef);
                }
                return stageRef;
            }
        }
        return nullptr;
#endif
    }
    CATCH_AND_RETURN(nullptr)
}

int64_t UsdUtilsStageCache_Insert(UsdStageRef stage) {
    if (!stage) return -1;
    try {
#if USD_USE_FULL
        // Get the underlying UsdStageRefPtr from our wrapper
        UsdStageOpaque* wrapper = static_cast<UsdStageOpaque*>(stage);
        if (!wrapper->stage) return -1;
        UsdStageCache::Id cacheId = UsdUtilsStageCache::Get().Insert(wrapper->stage);
        return cacheId.ToLongInt();
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        int64_t id = g_nextStubId++;
        StubStageEntry entry;
        entry.stageRef = UsdStage_Retain(stage);  // Take a reference
        // Try to get layer path for lookup support
        SdfLayerRef rootLayer = UsdStage_GetRootLayer(stage);
        if (rootLayer) {
            const char* layerPath = SdfLayer_GetIdentifier(rootLayer);
            if (layerPath) {
                entry.layerPath = layerPath;
            }
            SdfLayer_Release(rootLayer);
        }
        g_stubCache[id] = entry;
        return id;
#endif
    }
    CATCH_AND_RETURN(-1)
}

bool UsdUtilsStageCache_Erase(int64_t id) {
    try {
#if USD_USE_FULL
        UsdStageCache::Id cacheId = UsdStageCache::Id::FromLongInt(id);
        return UsdUtilsStageCache::Get().Erase(cacheId);
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        auto it = g_stubCache.find(id);
        if (it == g_stubCache.end()) return false;
        // Release the stage reference we took
        if (it->second.stageRef) {
            UsdStage_Release(static_cast<UsdStageRef>(it->second.stageRef));
        }
        g_stubCache.erase(it);
        return true;
#endif
    }
    CATCH_AND_RETURN(false)
}

bool UsdUtilsStageCache_EraseStage(UsdStageRef stage) {
    if (!stage) return false;
    try {
#if USD_USE_FULL
        UsdStageOpaque* wrapper = static_cast<UsdStageOpaque*>(stage);
        if (!wrapper->stage) return false;
        return UsdUtilsStageCache::Get().Erase(wrapper->stage);
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        for (auto it = g_stubCache.begin(); it != g_stubCache.end(); ++it) {
            if (it->second.stageRef == stage) {
                UsdStage_Release(static_cast<UsdStageRef>(it->second.stageRef));
                g_stubCache.erase(it);
                return true;
            }
        }
        return false;
#endif
    }
    CATCH_AND_RETURN(false)
}

void UsdUtilsStageCache_Clear(void) {
    try {
#if USD_USE_FULL
        UsdUtilsStageCache::Get().Clear();
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        for (auto& pair : g_stubCache) {
            if (pair.second.stageRef) {
                UsdStage_Release(static_cast<UsdStageRef>(pair.second.stageRef));
            }
        }
        g_stubCache.clear();
#endif
    }
    catch (...) {}
}

size_t UsdUtilsStageCache_GetAllIds(int64_t* outIds, size_t maxCount) {
    if (!outIds || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        auto allIds = UsdUtilsStageCache::Get().GetAllStages();
        size_t count = std::min(allIds.size(), maxCount);
        // We need to get IDs, not stages - iterate through cache
        auto& cache = UsdUtilsStageCache::Get();
        std::vector<UsdStageCache::Id> ids;
        // Unfortunately UsdStageCache doesn't have a GetAllIds method,
        // so we'll return sequential IDs based on the stages we find
        // This is a limitation of the current API
        size_t written = 0;
        for (size_t i = 0; i < count && written < maxCount; ++i) {
            // This approach won't work perfectly, but for the full USD case
            // we should use the actual cache IDs
        }
        // For now, just return 0 as a placeholder - real implementation
        // would need to track IDs separately
        return 0;
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        size_t written = 0;
        for (const auto& pair : g_stubCache) {
            if (written >= maxCount) break;
            outIds[written++] = pair.first;
        }
        return written;
#endif
    }
    CATCH_AND_RETURN(0)
}

size_t UsdUtilsStageCache_GetAllStages(UsdStageRef* outStages, size_t maxCount) {
    if (!outStages || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        auto allStages = UsdUtilsStageCache::Get().GetAllStages();
        size_t count = std::min(allStages.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outStages[i] = new UsdStageOpaque(allStages[i]);
        }
        return count;
#else
        std::lock_guard<std::mutex> lock(g_stubCacheMutex);
        size_t written = 0;
        for (const auto& pair : g_stubCache) {
            if (written >= maxCount) break;
            UsdStageRef stageRef = static_cast<UsdStageRef>(pair.second.stageRef);
            if (stageRef) {
                outStages[written++] = UsdStage_Retain(stageRef);
            }
        }
        return written;
#endif
    }
    CATCH_AND_RETURN(0)
}

// ============================================================================
// MARK: - UsdUtilsFlattenLayerStack Implementation
// ============================================================================

SdfLayerRef UsdUtilsFlattenLayerStack(UsdStageRef stage, const char* tag) {
    if (!stage) return nullptr;
    try {
#if USD_USE_FULL
        UsdStageOpaque* wrapper = static_cast<UsdStageOpaque*>(stage);
        if (!wrapper->stage) return nullptr;

        std::string tagStr = tag ? tag : "";
        SdfLayerRefPtr result = UsdUtilsFlattenLayerStack(wrapper->stage, tagStr);
        if (!result) return nullptr;

        // Create a wrapper for the layer
        // We need to create an SdfLayerOpaque wrapper
        return new SdfLayerOpaque(result);
#else
        // In stub mode, create an anonymous layer as placeholder
        return SdfLayer_CreateAnonymous(tag);
#endif
    }
    CATCH_AND_RETURN(nullptr)
}

char* UsdUtilsFlattenLayerStackResolveAssetPath(SdfLayerRef sourceLayer, const char* assetPath) {
    if (!assetPath) return nullptr;
    try {
#if USD_USE_FULL
        SdfLayerOpaque* wrapper = sourceLayer ? static_cast<SdfLayerOpaque*>(sourceLayer) : nullptr;
        SdfLayerHandle layerHandle = wrapper ? wrapper->layer : SdfLayerHandle();

        std::string result = UsdUtilsFlattenLayerStackResolveAssetPath(
            layerHandle, std::string(assetPath));

        char* resultStr = static_cast<char*>(malloc(result.size() + 1));
        if (!resultStr) return nullptr;
        strcpy(resultStr, result.c_str());
        return resultStr;
#else
        // In stub mode, just return a copy of the input path
        char* result = static_cast<char*>(malloc(strlen(assetPath) + 1));
        if (!result) return nullptr;
        strcpy(result, assetPath);
        return result;
#endif
    }
    CATCH_AND_RETURN(nullptr)
}

// ============================================================================
// MARK: - UsdUtilsStitchLayers Implementation
// ============================================================================

UsdResult UsdUtilsStitchLayers(SdfLayerRef strongLayer, SdfLayerRef weakLayer) {
    if (!strongLayer || !weakLayer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        SdfLayerOpaque* strongWrapper = static_cast<SdfLayerOpaque*>(strongLayer);
        SdfLayerOpaque* weakWrapper = static_cast<SdfLayerOpaque*>(weakLayer);

        if (!strongWrapper->layer || !weakWrapper->layer) {
            return USD_RESULT_INVALID_ARGUMENT;
        }

        UsdUtilsStitchLayers(strongWrapper->layer, weakWrapper->layer);
        return USD_RESULT_SUCCESS;
#else
        // In stub mode, just return success
        return USD_RESULT_SUCCESS;
#endif
    }
    CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - Utility Functions Implementation
// ============================================================================

void UsdUtils_FreeString(char* str) {
    if (str) {
        free(str);
    }
}

} // extern "C"
