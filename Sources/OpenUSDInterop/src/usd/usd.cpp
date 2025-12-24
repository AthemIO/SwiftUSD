// usd.cpp - Core USD module C wrapper implementation
// Mirrors: pxr/usd/usd/stage.h, pxr/usd/usd/prim.h, pxr/usd/usd/attribute.h, etc.

#include "../../include/usd/usd.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usd/stage.h>
    #include <pxr/usd/usd/prim.h>
    #include <pxr/usd/usd/primRange.h>
    #include <pxr/usd/usd/attribute.h>
    #include <pxr/usd/usd/relationship.h>
    #include <pxr/usd/usd/property.h>
    #include <pxr/usd/usd/references.h>
    #include <pxr/usd/usd/payloads.h>
    #include <pxr/usd/usd/variantSets.h>
    #include <pxr/usd/sdf/layer.h>
    #include <pxr/usd/sdf/path.h>
    #include <pxr/base/tf/token.h>
    #include <pxr/base/vt/value.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/gf/vec3d.h>
    #include <pxr/base/gf/matrix4d.h>
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

// ============================================================================
// MARK: - Wrapper Structures
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

struct UsdPrimOpaque {
#if USD_USE_FULL
    UsdPrim prim;
#else
    std::string path;
    std::string name;
    std::string typeName;
    bool isActive;
    bool isDefined;
#endif
    std::atomic<int> refCount;

    UsdPrimOpaque() : refCount(1) {
#if !USD_USE_FULL
        isActive = true;
        isDefined = false;
#endif
    }

#if USD_USE_FULL
    explicit UsdPrimOpaque(const UsdPrim& p)
        : prim(p), refCount(1) {}
#endif
};

struct UsdAttributeOpaque {
#if USD_USE_FULL
    UsdAttribute attr;
#else
    std::string name;
    std::string typeName;
    std::string path;
#endif
    std::atomic<int> refCount;

    UsdAttributeOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdAttributeOpaque(const UsdAttribute& a)
        : attr(a), refCount(1) {}
#endif
};

struct UsdRelationshipOpaque {
#if USD_USE_FULL
    UsdRelationship rel;
#else
    std::string name;
    std::string path;
#endif
    std::atomic<int> refCount;

    UsdRelationshipOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdRelationshipOpaque(const UsdRelationship& r)
        : rel(r), refCount(1) {}
#endif
};

struct UsdPropertyOpaque {
#if USD_USE_FULL
    UsdProperty prop;
#else
    std::string name;
    std::string path;
    bool isAttribute;
#endif
    std::atomic<int> refCount;

    UsdPropertyOpaque() : refCount(1) {
#if !USD_USE_FULL
        isAttribute = true;
#endif
    }

#if USD_USE_FULL
    explicit UsdPropertyOpaque(const UsdProperty& p)
        : prop(p), refCount(1) {}
#endif
};

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - UsdTimeCode Implementation
// ============================================================================

extern "C" {

UsdTimeCode UsdTimeCode_Default(void) {
    UsdTimeCode tc;
    tc.time = 0.0;
    tc.isDefault = true;
    return tc;
}

UsdTimeCode UsdTimeCode_Time(double time) {
    UsdTimeCode tc;
    tc.time = time;
    tc.isDefault = false;
    return tc;
}

UsdTimeCode UsdTimeCode_EarliestTime(void) {
    UsdTimeCode tc;
    tc.time = -std::numeric_limits<double>::max();
    tc.isDefault = false;
    return tc;
}

// ============================================================================
// MARK: - UsdStage Implementation
// ============================================================================

UsdStageRef UsdStage_CreateNew(const char* identifier, UsdInitialLoadSet load) {
    if (!identifier) return nullptr;
    try {
#if USD_USE_FULL
        auto loadSet = (load == USD_LOAD_ALL) ? UsdStage::LoadAll : UsdStage::LoadNone;
        UsdStageRefPtr stage = UsdStage::CreateNew(identifier, loadSet);
        if (!stage) return nullptr;
        return new UsdStageOpaque(stage);
#else
        auto* wrapper = new UsdStageOpaque();
        wrapper->identifier = identifier;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdStageRef UsdStage_CreateInMemory(const char* identifier, UsdInitialLoadSet load) {
    try {
#if USD_USE_FULL
        auto loadSet = (load == USD_LOAD_ALL) ? UsdStage::LoadAll : UsdStage::LoadNone;
        UsdStageRefPtr stage = identifier ?
            UsdStage::CreateInMemory(identifier, loadSet) :
            UsdStage::CreateInMemory(loadSet);
        if (!stage) return nullptr;
        return new UsdStageOpaque(stage);
#else
        auto* wrapper = new UsdStageOpaque();
        wrapper->identifier = identifier ? identifier : "anon";
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdStageRef UsdStage_Open(const char* filePath, UsdInitialLoadSet load) {
    if (!filePath) return nullptr;
    try {
#if USD_USE_FULL
        auto loadSet = (load == USD_LOAD_ALL) ? UsdStage::LoadAll : UsdStage::LoadNone;
        UsdStageRefPtr stage = UsdStage::Open(filePath, loadSet);
        if (!stage) return nullptr;
        return new UsdStageOpaque(stage);
#else
        auto* wrapper = new UsdStageOpaque();
        wrapper->identifier = filePath;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdStageRef UsdStage_OpenLayer(SdfLayerRef rootLayer, UsdInitialLoadSet load) {
    if (!rootLayer) return nullptr;
    try {
#if USD_USE_FULL
        auto loadSet = (load == USD_LOAD_ALL) ? UsdStage::LoadAll : UsdStage::LoadNone;
        UsdStageRefPtr stage = UsdStage::Open(rootLayer->layer, loadSet);
        if (!stage) return nullptr;
        return new UsdStageOpaque(stage);
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdStageRef UsdStage_Retain(UsdStageRef stage) {
    if (!stage) return nullptr;
    stage->refCount.fetch_add(1, std::memory_order_relaxed);
    return stage;
}

void UsdStage_Release(UsdStageRef stage) {
    if (!stage) return;
    if (stage->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete stage;
    }
}

SdfLayerRef UsdStage_GetRootLayer(UsdStageRef stage) {
    if (!stage) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            SdfLayerHandle layer = stage->stage->GetRootLayer();
            if (layer) {
                // Use FindOrOpen to get a refptr we can wrap
                return SdfLayer_FindOrOpen(layer->GetIdentifier().c_str());
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfLayerRef UsdStage_GetSessionLayer(UsdStageRef stage) {
    if (!stage) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            SdfLayerHandle layer = stage->stage->GetSessionLayer();
            if (layer) {
                return SdfLayer_FindOrOpen(layer->GetIdentifier().c_str());
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

double UsdStage_GetStartTimeCode(UsdStageRef stage) {
    if (!stage) return 0.0;
    try {
#if USD_USE_FULL
        return stage->stage ? stage->stage->GetStartTimeCode() : 0.0;
#else
        return stage->startTimeCode;
#endif
    } CATCH_AND_RETURN(0.0)
}

UsdResult UsdStage_SetStartTimeCode(UsdStageRef stage, double timeCode) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->SetStartTimeCode(timeCode);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        stage->startTimeCode = timeCode;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

double UsdStage_GetEndTimeCode(UsdStageRef stage) {
    if (!stage) return 0.0;
    try {
#if USD_USE_FULL
        return stage->stage ? stage->stage->GetEndTimeCode() : 0.0;
#else
        return stage->endTimeCode;
#endif
    } CATCH_AND_RETURN(0.0)
}

UsdResult UsdStage_SetEndTimeCode(UsdStageRef stage, double timeCode) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->SetEndTimeCode(timeCode);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        stage->endTimeCode = timeCode;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

double UsdStage_GetTimeCodesPerSecond(UsdStageRef stage) {
    if (!stage) return 24.0;
    try {
#if USD_USE_FULL
        return stage->stage ? stage->stage->GetTimeCodesPerSecond() : 24.0;
#else
        return stage->timeCodesPerSecond;
#endif
    } CATCH_AND_RETURN(24.0)
}

UsdResult UsdStage_SetTimeCodesPerSecond(UsdStageRef stage, double tcps) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->SetTimeCodesPerSecond(tcps);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        stage->timeCodesPerSecond = tcps;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

double UsdStage_GetFramesPerSecond(UsdStageRef stage) {
    if (!stage) return 24.0;
    try {
#if USD_USE_FULL
        return stage->stage ? stage->stage->GetFramesPerSecond() : 24.0;
#else
        return stage->framesPerSecond;
#endif
    } CATCH_AND_RETURN(24.0)
}

UsdResult UsdStage_SetFramesPerSecond(UsdStageRef stage, double fps) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->SetFramesPerSecond(fps);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        stage->framesPerSecond = fps;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdPrimRef UsdStage_GetPseudoRoot(UsdStageRef stage) {
    if (!stage) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim prim = stage->stage->GetPseudoRoot();
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_GetDefaultPrim(UsdStageRef stage) {
    if (!stage) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim prim = stage->stage->GetDefaultPrim();
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdStage_SetDefaultPrim(UsdStageRef stage, UsdPrimRef prim) {
    if (!stage || !prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage && prim->prim) {
            stage->stage->SetDefaultPrim(prim->prim);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdStage_ClearDefaultPrim(UsdStageRef stage) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->ClearDefaultPrim();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdStage_HasDefaultPrim(UsdStageRef stage) {
    if (!stage) return false;
    try {
#if USD_USE_FULL
        return stage->stage && stage->stage->HasDefaultPrim();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

UsdPrimRef UsdStage_GetPrimAtPath(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim prim = stage->stage->GetPrimAtPath(path->path);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_GetPrimAtPathString(UsdStageRef stage, const char* pathString) {
    if (!stage || !pathString) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            SdfPath path(pathString);
            UsdPrim prim = stage->stage->GetPrimAtPath(path);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_DefinePrim(UsdStageRef stage, SdfPathRef path, TfTokenRef typeName) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            TfToken type = typeName ? TfToken(TfToken_GetText(typeName)) : TfToken();
            UsdPrim prim = stage->stage->DefinePrim(path->path, type);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_DefinePrimString(UsdStageRef stage, const char* pathString, const char* typeName) {
    if (!stage || !pathString) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            SdfPath path(pathString);
            TfToken type = typeName ? TfToken(typeName) : TfToken();
            UsdPrim prim = stage->stage->DefinePrim(path, type);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdPrimOpaque();
        wrapper->path = pathString;
        wrapper->typeName = typeName ? typeName : "";
        wrapper->isDefined = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_OverridePrim(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim prim = stage->stage->OverridePrim(path->path);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_OverridePrimString(UsdStageRef stage, const char* pathString) {
    if (!stage || !pathString) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            SdfPath path(pathString);
            UsdPrim prim = stage->stage->OverridePrim(path);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdPrimOpaque();
        wrapper->path = pathString;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdStage_CreateClassPrim(UsdStageRef stage, SdfPathRef rootPrimPath) {
    if (!stage || !rootPrimPath) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim prim = stage->stage->CreateClassPrim(rootPrimPath->path);
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdStage_RemovePrim(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            bool success = stage->stage->RemovePrim(path->path);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdStage_GetRootPrimCount(UsdStageRef stage) {
    if (!stage) return 0;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim root = stage->stage->GetPseudoRoot();
            if (root) {
                size_t count = 0;
                for (const auto& child : root.GetChildren()) {
                    (void)child;
                    count++;
                }
                return count;
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdStage_GetRootPrims(UsdStageRef stage, UsdPrimRef* outPrims, size_t maxCount) {
    if (!stage || !outPrims || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdPrim root = stage->stage->GetPseudoRoot();
            if (root) {
                size_t count = 0;
                for (const auto& child : root.GetChildren()) {
                    if (count >= maxCount) break;
                    outPrims[count] = new UsdPrimOpaque(child);
                    count++;
                }
                return count;
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdStage_Traverse(UsdStageRef stage, UsdPrimRef* outPrims, size_t maxCount) {
    if (!stage || !outPrims || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            size_t count = 0;
            for (const auto& prim : stage->stage->Traverse()) {
                if (count >= maxCount) break;
                outPrims[count] = new UsdPrimOpaque(prim);
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdStage_GetTraversalCount(UsdStageRef stage) {
    if (!stage) return 0;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            size_t count = 0;
            for (const auto& prim : stage->stage->Traverse()) {
                (void)prim;
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdStage_Save(UsdStageRef stage) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->Save();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdStage_Export(UsdStageRef stage, const char* filename, bool addSourceFileComment) {
    if (!stage || !filename) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            bool success = stage->stage->Export(filename, addSourceFileComment);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_IO_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

char* UsdStage_ExportToString(UsdStageRef stage) {
    if (!stage) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            std::string result;
            if (stage->stage->ExportToString(&result)) {
                char* str = static_cast<char*>(malloc(result.length() + 1));
                if (str) {
                    memcpy(str, result.c_str(), result.length() + 1);
                }
                return str;
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

void UsdStage_FreeString(char* str) {
    free(str);
}

UsdResult UsdStage_Reload(UsdStageRef stage) {
    if (!stage) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            stage->stage->Reload();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdStage_GetLayerStackCount(UsdStageRef stage, bool includeSessionLayers) {
    if (!stage) return 0;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            return stage->stage->GetLayerStack(includeSessionLayers).size();
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdStage_GetLayerStack(UsdStageRef stage, SdfLayerRef* outLayers, size_t maxCount, bool includeSessionLayers) {
    if (!stage || !outLayers || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            auto layers = stage->stage->GetLayerStack(includeSessionLayers);
            size_t count = std::min(layers.size(), maxCount);
            for (size_t i = 0; i < count; i++) {
                outLayers[i] = SdfLayer_FindOrOpen(layers[i]->GetIdentifier().c_str());
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

// ============================================================================
// MARK: - UsdPrim Implementation
// ============================================================================

UsdPrimRef UsdPrim_Retain(UsdPrimRef prim) {
    if (!prim) return nullptr;
    prim->refCount.fetch_add(1, std::memory_order_relaxed);
    return prim;
}

void UsdPrim_Release(UsdPrimRef prim) {
    if (!prim) return;
    if (prim->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete prim;
    }
}

bool UsdPrim_IsValid(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim.IsValid();
#else
    return true;
#endif
}

bool UsdPrim_IsActive(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsActive();
#else
    return prim->isActive;
#endif
}

UsdResult UsdPrim_SetActive(UsdPrimRef prim, bool active) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            bool success = prim->prim.SetActive(active);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        prim->isActive = active;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdPrim_IsLoaded(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsLoaded();
#else
    return true;
#endif
}

bool UsdPrim_IsModel(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsModel();
#else
    return false;
#endif
}

bool UsdPrim_IsGroup(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsGroup();
#else
    return false;
#endif
}

bool UsdPrim_IsComponent(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsComponent();
#else
    return false;
#endif
}

bool UsdPrim_IsAbstract(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsAbstract();
#else
    return false;
#endif
}

bool UsdPrim_IsDefined(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.IsDefined();
#else
    return prim->isDefined;
#endif
}

bool UsdPrim_HasDefiningSpecifier(UsdPrimRef prim) {
    if (!prim) return false;
#if USD_USE_FULL
    return prim->prim && prim->prim.HasDefiningSpecifier();
#else
    return prim->isDefined;
#endif
}

SdfPathRef UsdPrim_GetPath(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return SdfPath_Create(prim->prim.GetPath().GetText());
        }
        return nullptr;
#else
        return SdfPath_Create(prim->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdPrim_GetName(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return TfToken_Create(prim->prim.GetName().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(prim->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdPrim_GetTypeName(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return TfToken_Create(prim->prim.GetTypeName().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(prim->typeName.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdPrim_SetTypeName(UsdPrimRef prim, TfTokenRef typeName) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken type = typeName ? TfToken(TfToken_GetText(typeName)) : TfToken();
            bool success = prim->prim.SetTypeName(type);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        if (typeName) prim->typeName = TfToken_GetText(typeName);
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_ClearTypeName(UsdPrimRef prim) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            bool success = prim->prim.ClearTypeName();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        prim->typeName.clear();
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdSpecifier UsdPrim_GetSpecifier(UsdPrimRef prim) {
    if (!prim) return USD_SPECIFIER_OVER;
#if USD_USE_FULL
    if (prim->prim) {
        SdfSpecifier spec = prim->prim.GetSpecifier();
        switch (spec) {
            case SdfSpecifierDef: return USD_SPECIFIER_DEF;
            case SdfSpecifierOver: return USD_SPECIFIER_OVER;
            case SdfSpecifierClass: return USD_SPECIFIER_CLASS;
            default: return USD_SPECIFIER_OVER;
        }
    }
    return USD_SPECIFIER_OVER;
#else
    return prim->isDefined ? USD_SPECIFIER_DEF : USD_SPECIFIER_OVER;
#endif
}

UsdResult UsdPrim_SetSpecifier(UsdPrimRef prim, UsdSpecifier specifier) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            SdfSpecifier spec;
            switch (specifier) {
                case USD_SPECIFIER_DEF: spec = SdfSpecifierDef; break;
                case USD_SPECIFIER_OVER: spec = SdfSpecifierOver; break;
                case USD_SPECIFIER_CLASS: spec = SdfSpecifierClass; break;
                default: return USD_RESULT_INVALID_ARGUMENT;
            }
            bool success = prim->prim.SetSpecifier(spec);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        prim->isDefined = (specifier == USD_SPECIFIER_DEF);
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdPrim_GetKind(UsdPrimRef prim, TfTokenRef* kind) {
    if (!prim || !kind) return false;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken k;
            if (prim->prim.GetKind(&k)) {
                *kind = TfToken_Create(k.GetText());
                return true;
            }
        }
        return false;
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

UsdResult UsdPrim_SetKind(UsdPrimRef prim, TfTokenRef kind) {
    if (!prim || !kind) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken k(TfToken_GetText(kind));
            bool success = prim->prim.SetKind(k);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdPrimRef UsdPrim_GetParent(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdPrim parent = prim->prim.GetParent();
            if (parent) {
                return new UsdPrimOpaque(parent);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdStageRef UsdPrim_GetStage(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdStagePtr stage = prim->prim.GetStage();
            if (stage) {
                // Note: This creates a new wrapper but the underlying stage is shared
                return new UsdStageOpaque(stage);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdPrim_GetChildCount(UsdPrimRef prim) {
    if (!prim) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            size_t count = 0;
            for (const auto& child : prim->prim.GetChildren()) {
                (void)child;
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetChildren(UsdPrimRef prim, UsdPrimRef* outChildren, size_t maxCount) {
    if (!prim || !outChildren || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            size_t count = 0;
            for (const auto& child : prim->prim.GetChildren()) {
                if (count >= maxCount) break;
                outChildren[count] = new UsdPrimOpaque(child);
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetFilteredChildren(UsdPrimRef prim, UsdPrimRef* outChildren, size_t maxCount) {
    if (!prim || !outChildren || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            size_t count = 0;
            for (const auto& child : prim->prim.GetFilteredChildren(UsdPrimDefaultPredicate)) {
                if (count >= maxCount) break;
                outChildren[count] = new UsdPrimOpaque(child);
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetAllChildren(UsdPrimRef prim, UsdPrimRef* outChildren, size_t maxCount) {
    if (!prim || !outChildren || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            size_t count = 0;
            for (const auto& child : prim->prim.GetAllChildren()) {
                if (count >= maxCount) break;
                outChildren[count] = new UsdPrimOpaque(child);
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdPrimRef UsdPrim_GetChild(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            UsdPrim child = prim->prim.GetChild(n);
            if (child) {
                return new UsdPrimOpaque(child);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdPrim_GetChildString(UsdPrimRef prim, const char* name) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(name);
            UsdPrim child = prim->prim.GetChild(n);
            if (child) {
                return new UsdPrimOpaque(child);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdPrim_GetDescendantCount(UsdPrimRef prim) {
    if (!prim) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            size_t count = 0;
            UsdPrimRange range(prim->prim);
            for (auto it = range.begin(); it != range.end(); ++it) {
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetDescendants(UsdPrimRef prim, UsdPrimRef* outPrims, size_t maxCount) {
    if (!prim || !outPrims || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            size_t count = 0;
            UsdPrimRange range(prim->prim);
            for (auto it = range.begin(); it != range.end(); ++it) {
                if (count >= maxCount) break;
                outPrims[count] = new UsdPrimOpaque(*it);
                count++;
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetPropertyCount(UsdPrimRef prim) {
    if (!prim) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return prim->prim.GetPropertyNames().size();
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetPropertyNames(UsdPrimRef prim, TfTokenRef* outNames, size_t maxCount) {
    if (!prim || !outNames || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            auto names = prim->prim.GetPropertyNames();
            size_t count = std::min(names.size(), maxCount);
            for (size_t i = 0; i < count; i++) {
                outNames[i] = TfToken_Create(names[i].GetText());
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdPropertyRef UsdPrim_GetProperty(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            UsdProperty prop = prim->prim.GetProperty(n);
            if (prop) {
                return new UsdPropertyOpaque(prop);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

bool UsdPrim_HasProperty(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return false;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            return prim->prim.HasProperty(n);
        }
        return false;
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

UsdResult UsdPrim_RemoveProperty(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            bool success = prim->prim.RemoveProperty(n);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_NOT_FOUND;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdPrim_GetAttributeCount(UsdPrimRef prim) {
    if (!prim) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return prim->prim.GetAttributes().size();
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetAttributeNames(UsdPrimRef prim, TfTokenRef* outNames, size_t maxCount) {
    if (!prim || !outNames || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            auto attrs = prim->prim.GetAttributes();
            size_t count = std::min(attrs.size(), maxCount);
            for (size_t i = 0; i < count; i++) {
                outNames[i] = TfToken_Create(attrs[i].GetName().GetText());
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdAttributeRef UsdPrim_GetAttribute(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            UsdAttribute attr = prim->prim.GetAttribute(n);
            if (attr) {
                return new UsdAttributeOpaque(attr);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdAttributeRef UsdPrim_GetAttributeString(UsdPrimRef prim, const char* name) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(name);
            UsdAttribute attr = prim->prim.GetAttribute(n);
            if (attr) {
                return new UsdAttributeOpaque(attr);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

bool UsdPrim_HasAttribute(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return false;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            return prim->prim.HasAttribute(n);
        }
        return false;
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

UsdAttributeRef UsdPrim_CreateAttribute(
    UsdPrimRef prim,
    TfTokenRef name,
    TfTokenRef typeName,
    bool custom,
    UsdVariability variability
) {
    if (!prim || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            SdfValueTypeName type = SdfSchema::GetInstance().FindType(TfToken_GetText(typeName));
            SdfVariability var = (variability == USD_VARIABILITY_UNIFORM) ?
                SdfVariabilityUniform : SdfVariabilityVarying;
            UsdAttribute attr = prim->prim.CreateAttribute(n, type, custom, var);
            if (attr) {
                return new UsdAttributeOpaque(attr);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdAttributeOpaque();
        wrapper->name = TfToken_GetText(name);
        wrapper->typeName = TfToken_GetText(typeName);
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdAttributeRef UsdPrim_CreateAttributeString(
    UsdPrimRef prim,
    const char* name,
    const char* typeName,
    bool custom,
    UsdVariability variability
) {
    if (!prim || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(name);
            SdfValueTypeName type = SdfSchema::GetInstance().FindType(typeName);
            SdfVariability var = (variability == USD_VARIABILITY_UNIFORM) ?
                SdfVariabilityUniform : SdfVariabilityVarying;
            UsdAttribute attr = prim->prim.CreateAttribute(n, type, custom, var);
            if (attr) {
                return new UsdAttributeOpaque(attr);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdAttributeOpaque();
        wrapper->name = name;
        wrapper->typeName = typeName;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdPrim_GetRelationshipCount(UsdPrimRef prim) {
    if (!prim) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return prim->prim.GetRelationships().size();
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetRelationshipNames(UsdPrimRef prim, TfTokenRef* outNames, size_t maxCount) {
    if (!prim || !outNames || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            auto rels = prim->prim.GetRelationships();
            size_t count = std::min(rels.size(), maxCount);
            for (size_t i = 0; i < count; i++) {
                outNames[i] = TfToken_Create(rels[i].GetName().GetText());
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdRelationshipRef UsdPrim_GetRelationship(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            UsdRelationship rel = prim->prim.GetRelationship(n);
            if (rel) {
                return new UsdRelationshipOpaque(rel);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

bool UsdPrim_HasRelationship(UsdPrimRef prim, TfTokenRef name) {
    if (!prim || !name) return false;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            return prim->prim.HasRelationship(n);
        }
        return false;
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

UsdRelationshipRef UsdPrim_CreateRelationship(UsdPrimRef prim, TfTokenRef name, bool custom) {
    if (!prim || !name) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            TfToken n(TfToken_GetText(name));
            UsdRelationship rel = prim->prim.CreateRelationship(n, custom);
            if (rel) {
                return new UsdRelationshipOpaque(rel);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdRelationshipOpaque();
        wrapper->name = TfToken_GetText(name);
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdPrim_AddReference(
    UsdPrimRef prim,
    const char* identifier,
    SdfPathRef primPath,
    UsdListPosition position
) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdReferences refs = prim->prim.GetReferences();
            UsdListPosition pos;
            switch (position) {
                case USD_LIST_POSITION_FRONT_OF_PREPEND_LIST:
                    pos = UsdListPositionFrontOfPrependList; break;
                case USD_LIST_POSITION_BACK_OF_PREPEND_LIST:
                    pos = UsdListPositionBackOfPrependList; break;
                case USD_LIST_POSITION_FRONT_OF_APPEND_LIST:
                    pos = UsdListPositionFrontOfAppendList; break;
                case USD_LIST_POSITION_BACK_OF_APPEND_LIST:
                    pos = UsdListPositionBackOfAppendList; break;
                default:
                    pos = UsdListPositionBackOfPrependList;
            }
            bool success;
            if (identifier && primPath) {
                success = refs.AddReference(identifier, primPath->path, SdfLayerOffset(), pos);
            } else if (identifier) {
                success = refs.AddReference(identifier, SdfLayerOffset(), pos);
            } else {
                return USD_RESULT_INVALID_ARGUMENT;
            }
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_AddInternalReference(
    UsdPrimRef prim,
    SdfPathRef primPath,
    UsdListPosition position
) {
    if (!prim || !primPath) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdReferences refs = prim->prim.GetReferences();
            UsdListPosition pos;
            switch (position) {
                case USD_LIST_POSITION_FRONT_OF_PREPEND_LIST:
                    pos = UsdListPositionFrontOfPrependList; break;
                case USD_LIST_POSITION_BACK_OF_PREPEND_LIST:
                    pos = UsdListPositionBackOfPrependList; break;
                case USD_LIST_POSITION_FRONT_OF_APPEND_LIST:
                    pos = UsdListPositionFrontOfAppendList; break;
                case USD_LIST_POSITION_BACK_OF_APPEND_LIST:
                    pos = UsdListPositionBackOfAppendList; break;
                default:
                    pos = UsdListPositionBackOfPrependList;
            }
            bool success = refs.AddInternalReference(primPath->path, SdfLayerOffset(), pos);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_ClearReferences(UsdPrimRef prim) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdReferences refs = prim->prim.GetReferences();
            bool success = refs.ClearReferences();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_AddPayload(
    UsdPrimRef prim,
    const char* identifier,
    SdfPathRef primPath,
    UsdListPosition position
) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdPayloads payloads = prim->prim.GetPayloads();
            UsdListPosition pos;
            switch (position) {
                case USD_LIST_POSITION_FRONT_OF_PREPEND_LIST:
                    pos = UsdListPositionFrontOfPrependList; break;
                case USD_LIST_POSITION_BACK_OF_PREPEND_LIST:
                    pos = UsdListPositionBackOfPrependList; break;
                case USD_LIST_POSITION_FRONT_OF_APPEND_LIST:
                    pos = UsdListPositionFrontOfAppendList; break;
                case USD_LIST_POSITION_BACK_OF_APPEND_LIST:
                    pos = UsdListPositionBackOfAppendList; break;
                default:
                    pos = UsdListPositionBackOfPrependList;
            }
            bool success;
            if (identifier && primPath) {
                SdfPayload payload(identifier, primPath->path);
                success = payloads.AddPayload(payload, pos);
            } else if (identifier) {
                SdfPayload payload(identifier);
                success = payloads.AddPayload(payload, pos);
            } else {
                return USD_RESULT_INVALID_ARGUMENT;
            }
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_ClearPayloads(UsdPrimRef prim) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdPayloads payloads = prim->prim.GetPayloads();
            bool success = payloads.ClearPayloads();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_Load(UsdPrimRef prim) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            prim->prim.Load();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdPrim_Unload(UsdPrimRef prim) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            prim->prim.Unload();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdPrim_GetVariantSetCount(UsdPrimRef prim) {
    if (!prim) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return prim->prim.GetVariantSets().GetNames().size();
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdPrim_GetVariantSetNames(UsdPrimRef prim, TfTokenRef* outNames, size_t maxCount) {
    if (!prim || !outNames || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            auto names = prim->prim.GetVariantSets().GetNames();
            size_t count = std::min(names.size(), maxCount);
            for (size_t i = 0; i < count; i++) {
                outNames[i] = TfToken_Create(names[i].c_str());
            }
            return count;
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

bool UsdPrim_HasVariantSet(UsdPrimRef prim, const char* variantSetName) {
    if (!prim || !variantSetName) return false;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            return prim->prim.GetVariantSets().HasVariantSet(variantSetName);
        }
        return false;
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

char* UsdPrim_GetVariantSelection(UsdPrimRef prim, const char* variantSetName) {
    if (!prim || !variantSetName) return nullptr;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdVariantSet vs = prim->prim.GetVariantSet(variantSetName);
            if (vs) {
                std::string selection = vs.GetVariantSelection();
                if (!selection.empty()) {
                    char* result = static_cast<char*>(malloc(selection.length() + 1));
                    if (result) {
                        memcpy(result, selection.c_str(), selection.length() + 1);
                    }
                    return result;
                }
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdPrim_SetVariantSelection(
    UsdPrimRef prim,
    const char* variantSetName,
    const char* variantName
) {
    if (!prim || !variantSetName || !variantName) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (prim->prim) {
            UsdVariantSet vs = prim->prim.GetVariantSet(variantSetName);
            if (vs) {
                bool success = vs.SetVariantSelection(variantName);
                return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

void UsdPrim_FreeString(char* str) {
    free(str);
}

void UsdPrim_ReleaseBatch(UsdPrimRef* prims, size_t count) {
    if (!prims) return;
    for (size_t i = 0; i < count; i++) {
        UsdPrim_Release(prims[i]);
        prims[i] = nullptr;
    }
}

// ============================================================================
// MARK: - UsdProperty Implementation
// ============================================================================

UsdPropertyRef UsdProperty_Retain(UsdPropertyRef prop) {
    if (!prop) return nullptr;
    prop->refCount.fetch_add(1, std::memory_order_relaxed);
    return prop;
}

void UsdProperty_Release(UsdPropertyRef prop) {
    if (!prop) return;
    if (prop->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete prop;
    }
}

bool UsdProperty_IsValid(UsdPropertyRef prop) {
    if (!prop) return false;
#if USD_USE_FULL
    return prop->prop.IsValid();
#else
    return true;
#endif
}

bool UsdProperty_IsAttribute(UsdPropertyRef prop) {
    if (!prop) return false;
#if USD_USE_FULL
    return prop->prop.Is<UsdAttribute>();
#else
    return prop->isAttribute;
#endif
}

bool UsdProperty_IsRelationship(UsdPropertyRef prop) {
    if (!prop) return false;
#if USD_USE_FULL
    return prop->prop.Is<UsdRelationship>();
#else
    return !prop->isAttribute;
#endif
}

TfTokenRef UsdProperty_GetName(UsdPropertyRef prop) {
    if (!prop) return nullptr;
    try {
#if USD_USE_FULL
        if (prop->prop) {
            return TfToken_Create(prop->prop.GetName().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(prop->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdProperty_GetBaseName(UsdPropertyRef prop) {
    if (!prop) return nullptr;
    try {
#if USD_USE_FULL
        if (prop->prop) {
            return TfToken_Create(prop->prop.GetBaseName().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(prop->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdProperty_GetNamespace(UsdPropertyRef prop) {
    if (!prop) return nullptr;
    try {
#if USD_USE_FULL
        if (prop->prop) {
            return TfToken_Create(prop->prop.GetNamespace().GetText());
        }
        return nullptr;
#else
        return TfToken_Create("");
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdProperty_GetPath(UsdPropertyRef prop) {
    if (!prop) return nullptr;
    try {
#if USD_USE_FULL
        if (prop->prop) {
            return SdfPath_Create(prop->prop.GetPath().GetText());
        }
        return nullptr;
#else
        return SdfPath_Create(prop->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdProperty_GetPrim(UsdPropertyRef prop) {
    if (!prop) return nullptr;
    try {
#if USD_USE_FULL
        if (prop->prop) {
            UsdPrim prim = prop->prop.GetPrim();
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

bool UsdProperty_IsCustom(UsdPropertyRef prop) {
    if (!prop) return false;
#if USD_USE_FULL
    return prop->prop && prop->prop.IsCustom();
#else
    return false;
#endif
}

bool UsdProperty_IsDefined(UsdPropertyRef prop) {
    if (!prop) return false;
#if USD_USE_FULL
    return prop->prop && prop->prop.IsDefined();
#else
    return true;
#endif
}

bool UsdProperty_IsAuthored(UsdPropertyRef prop) {
    if (!prop) return false;
#if USD_USE_FULL
    return prop->prop && prop->prop.IsAuthored();
#else
    return true;
#endif
}

// ============================================================================
// MARK: - UsdAttribute Implementation
// ============================================================================

UsdAttributeRef UsdAttribute_Retain(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    attr->refCount.fetch_add(1, std::memory_order_relaxed);
    return attr;
}

void UsdAttribute_Release(UsdAttributeRef attr) {
    if (!attr) return;
    if (attr->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete attr;
    }
}

bool UsdAttribute_IsValid(UsdAttributeRef attr) {
    if (!attr) return false;
#if USD_USE_FULL
    return attr->attr.IsValid();
#else
    return true;
#endif
}

TfTokenRef UsdAttribute_GetName(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            return TfToken_Create(attr->attr.GetName().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(attr->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdAttribute_GetPath(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            return SdfPath_Create(attr->attr.GetPath().GetText());
        }
        return nullptr;
#else
        return SdfPath_Create(attr->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdAttribute_GetPrim(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            UsdPrim prim = attr->attr.GetPrim();
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdAttribute_GetTypeName(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            return TfToken_Create(attr->attr.GetTypeName().GetAsToken().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(attr->typeName.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVariability UsdAttribute_GetVariability(UsdAttributeRef attr) {
    if (!attr) return USD_VARIABILITY_VARYING;
#if USD_USE_FULL
    if (attr->attr) {
        SdfVariability var = attr->attr.GetVariability();
        return (var == SdfVariabilityUniform) ? USD_VARIABILITY_UNIFORM : USD_VARIABILITY_VARYING;
    }
    return USD_VARIABILITY_VARYING;
#else
    return USD_VARIABILITY_VARYING;
#endif
}

bool UsdAttribute_HasValue(UsdAttributeRef attr) {
    if (!attr) return false;
#if USD_USE_FULL
    return attr->attr && attr->attr.HasValue();
#else
    return false;
#endif
}

bool UsdAttribute_HasAuthoredValue(UsdAttributeRef attr) {
    if (!attr) return false;
#if USD_USE_FULL
    return attr->attr && attr->attr.HasAuthoredValue();
#else
    return false;
#endif
}

bool UsdAttribute_HasFallbackValue(UsdAttributeRef attr) {
    if (!attr) return false;
#if USD_USE_FULL
    return attr->attr && attr->attr.HasFallbackValue();
#else
    return false;
#endif
}

bool UsdAttribute_ValueMightBeTimeVarying(UsdAttributeRef attr) {
    if (!attr) return false;
#if USD_USE_FULL
    return attr->attr && attr->attr.ValueMightBeTimeVarying();
#else
    return false;
#endif
}

// Helper to convert UsdTimeCode
#if USD_USE_FULL
static UsdTimeCode ToUsdTimeCode(UsdTimeCode tc) {
    if (tc.isDefault) {
        return UsdTimeCode::Default();
    }
    return UsdTimeCode(tc.time);
}
#endif

UsdResult UsdAttribute_GetBool(UsdAttributeRef attr, UsdTimeCode time, bool* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Get(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetBool(UsdAttributeRef attr, UsdTimeCode time, bool value) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Set(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetInt(UsdAttributeRef attr, UsdTimeCode time, int* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Get(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetInt(UsdAttributeRef attr, UsdTimeCode time, int value) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Set(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetFloat(UsdAttributeRef attr, UsdTimeCode time, float* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Get(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetFloat(UsdAttributeRef attr, UsdTimeCode time, float value) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Set(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetDouble(UsdAttributeRef attr, UsdTimeCode time, double* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Get(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetDouble(UsdAttributeRef attr, UsdTimeCode time, double value) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.Set(value, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetString(UsdAttributeRef attr, UsdTimeCode time, char** value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            std::string str;
            if (attr->attr.Get(&str, ToUsdTimeCode(time))) {
                *value = static_cast<char*>(malloc(str.length() + 1));
                if (*value) {
                    memcpy(*value, str.c_str(), str.length() + 1);
                }
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetString(UsdAttributeRef attr, UsdTimeCode time, const char* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            std::string str(value);
            if (attr->attr.Set(str, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetToken(UsdAttributeRef attr, UsdTimeCode time, TfTokenRef* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            TfToken token;
            if (attr->attr.Get(&token, ToUsdTimeCode(time))) {
                *value = TfToken_Create(token.GetText());
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetToken(UsdAttributeRef attr, UsdTimeCode time, TfTokenRef value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            TfToken token(TfToken_GetText(value));
            if (attr->attr.Set(token, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetVec3f(UsdAttributeRef attr, UsdTimeCode time, float* x, float* y, float* z) {
    if (!attr || !x || !y || !z) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            GfVec3f vec;
            if (attr->attr.Get(&vec, ToUsdTimeCode(time))) {
                *x = vec[0];
                *y = vec[1];
                *z = vec[2];
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetVec3f(UsdAttributeRef attr, UsdTimeCode time, float x, float y, float z) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            GfVec3f vec(x, y, z);
            if (attr->attr.Set(vec, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetVec3d(UsdAttributeRef attr, UsdTimeCode time, double* x, double* y, double* z) {
    if (!attr || !x || !y || !z) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            GfVec3d vec;
            if (attr->attr.Get(&vec, ToUsdTimeCode(time))) {
                *x = vec[0];
                *y = vec[1];
                *z = vec[2];
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetVec3d(UsdAttributeRef attr, UsdTimeCode time, double x, double y, double z) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            GfVec3d vec(x, y, z);
            if (attr->attr.Set(vec, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetMatrix4d(UsdAttributeRef attr, UsdTimeCode time, double* values) {
    if (!attr || !values) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            GfMatrix4d mat;
            if (attr->attr.Get(&mat, ToUsdTimeCode(time))) {
                const double* data = mat.data();
                memcpy(values, data, 16 * sizeof(double));
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetMatrix4d(UsdAttributeRef attr, UsdTimeCode time, const double* values) {
    if (!attr || !values) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            GfMatrix4d mat;
            double* data = mat.data();
            memcpy(data, values, 16 * sizeof(double));
            if (attr->attr.Set(mat, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetValue(UsdAttributeRef attr, UsdTimeCode time, VtValueRef* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            VtValue val;
            if (attr->attr.Get(&val, ToUsdTimeCode(time))) {
                *value = VtValue_CreateFromDouble(0.0); // Placeholder
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetValue(UsdAttributeRef attr, UsdTimeCode time, VtValueRef value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        // VtValue setting requires type conversion - simplified for now
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_GetAssetPath(UsdAttributeRef attr, UsdTimeCode time, SdfAssetPathRef* value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            SdfAssetPath ap;
            if (attr->attr.Get(&ap, ToUsdTimeCode(time))) {
                *value = SdfAssetPath_Create(ap.GetAssetPath().c_str());
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetAssetPath(UsdAttributeRef attr, UsdTimeCode time, SdfAssetPathRef value) {
    if (!attr || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            SdfAssetPath ap(SdfAssetPath_GetAssetPath(value));
            if (attr->attr.Set(ap, ToUsdTimeCode(time))) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdAttribute_GetNumTimeSamples(UsdAttributeRef attr) {
    if (!attr) return 0;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            return attr->attr.GetNumTimeSamples();
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdAttribute_GetTimeSamples(UsdAttributeRef attr, double* outTimes, size_t maxCount) {
    if (!attr || !outTimes || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            std::vector<double> times;
            if (attr->attr.GetTimeSamples(&times)) {
                size_t count = std::min(times.size(), maxCount);
                memcpy(outTimes, times.data(), count * sizeof(double));
                return count;
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdAttribute_GetBracketingTimeSamples(
    UsdAttributeRef attr,
    double desiredTime,
    double* lowerTime,
    double* upperTime,
    bool* hasTimeSamples
) {
    if (!attr || !lowerTime || !upperTime || !hasTimeSamples) {
        return USD_RESULT_INVALID_ARGUMENT;
    }
    try {
#if USD_USE_FULL
        if (attr->attr) {
            if (attr->attr.GetBracketingTimeSamples(desiredTime, lowerTime, upperTime, hasTimeSamples)) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_NOT_FOUND;
#else
        *hasTimeSamples = false;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_Clear(UsdAttributeRef attr) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            bool success = attr->attr.Clear();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_ClearAtTime(UsdAttributeRef attr, UsdTimeCode time) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            bool success = attr->attr.ClearAtTime(ToUsdTimeCode(time));
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_ClearDefault(UsdAttributeRef attr) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            bool success = attr->attr.ClearDefault();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_Block(UsdAttributeRef attr) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            attr->attr.Block();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

void UsdAttribute_FreeString(char* str) {
    free(str);
}

size_t UsdAttribute_GetConnectionCount(UsdAttributeRef attr) {
    if (!attr) return 0;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            SdfPathVector paths;
            if (attr->attr.GetConnections(&paths)) {
                return paths.size();
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdAttribute_GetConnections(UsdAttributeRef attr, SdfPathRef* outPaths, size_t maxCount) {
    if (!attr || !outPaths || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            SdfPathVector paths;
            if (attr->attr.GetConnections(&paths)) {
                size_t count = std::min(paths.size(), maxCount);
                for (size_t i = 0; i < count; i++) {
                    outPaths[i] = SdfPath_Create(paths[i].GetText());
                }
                return count;
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdAttribute_AddConnection(
    UsdAttributeRef attr,
    SdfPathRef source,
    UsdListPosition position
) {
    if (!attr || !source) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            UsdListPosition pos;
            switch (position) {
                case USD_LIST_POSITION_FRONT_OF_PREPEND_LIST:
                    pos = UsdListPositionFrontOfPrependList; break;
                case USD_LIST_POSITION_BACK_OF_PREPEND_LIST:
                    pos = UsdListPositionBackOfPrependList; break;
                case USD_LIST_POSITION_FRONT_OF_APPEND_LIST:
                    pos = UsdListPositionFrontOfAppendList; break;
                case USD_LIST_POSITION_BACK_OF_APPEND_LIST:
                    pos = UsdListPositionBackOfAppendList; break;
                default:
                    pos = UsdListPositionBackOfPrependList;
            }
            bool success = attr->attr.AddConnection(source->path, pos);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_RemoveConnection(UsdAttributeRef attr, SdfPathRef source) {
    if (!attr || !source) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            bool success = attr->attr.RemoveConnection(source->path);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_ClearConnections(UsdAttributeRef attr) {
    if (!attr) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            bool success = attr->attr.ClearConnections();
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdAttribute_SetConnections(UsdAttributeRef attr, SdfPathRef* sources, size_t count) {
    if (!attr || (!sources && count > 0)) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (attr->attr) {
            SdfPathVector paths;
            paths.reserve(count);
            for (size_t i = 0; i < count; i++) {
                if (sources[i]) {
                    paths.push_back(sources[i]->path);
                }
            }
            bool success = attr->attr.SetConnections(paths);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdAttribute_HasAuthoredConnections(UsdAttributeRef attr) {
    if (!attr) return false;
#if USD_USE_FULL
    return attr->attr && attr->attr.HasAuthoredConnections();
#else
    return false;
#endif
}

// ============================================================================
// MARK: - UsdRelationship Implementation
// ============================================================================

UsdRelationshipRef UsdRelationship_Retain(UsdRelationshipRef rel) {
    if (!rel) return nullptr;
    rel->refCount.fetch_add(1, std::memory_order_relaxed);
    return rel;
}

void UsdRelationship_Release(UsdRelationshipRef rel) {
    if (!rel) return;
    if (rel->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete rel;
    }
}

bool UsdRelationship_IsValid(UsdRelationshipRef rel) {
    if (!rel) return false;
#if USD_USE_FULL
    return rel->rel.IsValid();
#else
    return true;
#endif
}

TfTokenRef UsdRelationship_GetName(UsdRelationshipRef rel) {
    if (!rel) return nullptr;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            return TfToken_Create(rel->rel.GetName().GetText());
        }
        return nullptr;
#else
        return TfToken_Create(rel->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdRelationship_GetPath(UsdRelationshipRef rel) {
    if (!rel) return nullptr;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            return SdfPath_Create(rel->rel.GetPath().GetText());
        }
        return nullptr;
#else
        return SdfPath_Create(rel->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdRelationship_GetPrim(UsdRelationshipRef rel) {
    if (!rel) return nullptr;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            UsdPrim prim = rel->rel.GetPrim();
            if (prim) {
                return new UsdPrimOpaque(prim);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdRelationship_GetTargetCount(UsdRelationshipRef rel) {
    if (!rel) return 0;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            SdfPathVector targets;
            if (rel->rel.GetTargets(&targets)) {
                return targets.size();
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdRelationship_GetTargets(UsdRelationshipRef rel, SdfPathRef* outPaths, size_t maxCount) {
    if (!rel || !outPaths || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            SdfPathVector targets;
            if (rel->rel.GetTargets(&targets)) {
                size_t count = std::min(targets.size(), maxCount);
                for (size_t i = 0; i < count; i++) {
                    outPaths[i] = SdfPath_Create(targets[i].GetText());
                }
                return count;
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdRelationship_GetForwardedTargets(UsdRelationshipRef rel, SdfPathRef* outPaths, size_t maxCount) {
    if (!rel || !outPaths || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            SdfPathVector targets;
            if (rel->rel.GetForwardedTargets(&targets)) {
                size_t count = std::min(targets.size(), maxCount);
                for (size_t i = 0; i < count; i++) {
                    outPaths[i] = SdfPath_Create(targets[i].GetText());
                }
                return count;
            }
        }
        return 0;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdRelationship_AddTarget(
    UsdRelationshipRef rel,
    SdfPathRef target,
    UsdListPosition position
) {
    if (!rel || !target) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            UsdListPosition pos;
            switch (position) {
                case USD_LIST_POSITION_FRONT_OF_PREPEND_LIST:
                    pos = UsdListPositionFrontOfPrependList; break;
                case USD_LIST_POSITION_BACK_OF_PREPEND_LIST:
                    pos = UsdListPositionBackOfPrependList; break;
                case USD_LIST_POSITION_FRONT_OF_APPEND_LIST:
                    pos = UsdListPositionFrontOfAppendList; break;
                case USD_LIST_POSITION_BACK_OF_APPEND_LIST:
                    pos = UsdListPositionBackOfAppendList; break;
                default:
                    pos = UsdListPositionBackOfPrependList;
            }
            bool success = rel->rel.AddTarget(target->path, pos);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdRelationship_RemoveTarget(UsdRelationshipRef rel, SdfPathRef target) {
    if (!rel || !target) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            bool success = rel->rel.RemoveTarget(target->path);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdRelationship_SetTargets(UsdRelationshipRef rel, SdfPathRef* targets, size_t count) {
    if (!rel || (!targets && count > 0)) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            SdfPathVector paths;
            paths.reserve(count);
            for (size_t i = 0; i < count; i++) {
                if (targets[i]) {
                    paths.push_back(targets[i]->path);
                }
            }
            bool success = rel->rel.SetTargets(paths);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdRelationship_ClearTargets(UsdRelationshipRef rel, bool removeSpec) {
    if (!rel) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (rel->rel) {
            bool success = rel->rel.ClearTargets(removeSpec);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdRelationship_HasAuthoredTargets(UsdRelationshipRef rel) {
    if (!rel) return false;
#if USD_USE_FULL
    return rel->rel && rel->rel.HasAuthoredTargets();
#else
    return false;
#endif
}

} // extern "C"
