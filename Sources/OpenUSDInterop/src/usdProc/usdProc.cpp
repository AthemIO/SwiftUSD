// usdProc.cpp - UsdProc module C wrapper implementation
// Mirrors: pxr/usd/usdProc/*.h

#include "../../include/usdProc/usdProc.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdProc/generativeProcedural.h>
    #include <pxr/usd/usdProc/tokens.h>
    #include <pxr/usd/usdGeom/boundable.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/vt/array.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>

// ============================================================================
// MARK: - Wrapper Structures
// ============================================================================

struct UsdProcGenerativeProceduralOpaque {
#if USD_USE_FULL
    UsdProcGenerativeProcedural proc;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::string proceduralSystem;
    float extent[6]; // minX, minY, minZ, maxX, maxY, maxZ

    UsdProcGenerativeProceduralOpaque() : refCount(1), valid(false) {
        proceduralSystem = "";
        // Default extent (unit cube centered at origin)
        extent[0] = extent[1] = extent[2] = -0.5f;
        extent[3] = extent[4] = extent[5] = 0.5f;
    }

#if USD_USE_FULL
    explicit UsdProcGenerativeProceduralOpaque(const UsdProcGenerativeProcedural& p)
        : proc(p), refCount(1), valid(true) {
        proceduralSystem = "";
        extent[0] = extent[1] = extent[2] = -0.5f;
        extent[3] = extent[4] = extent[5] = 0.5f;
    }
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
// MARK: - UsdProcGenerativeProcedural Implementation
// ============================================================================

extern "C" {

UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdProcGenerativeProcedural proc = UsdProcGenerativeProcedural::Define(stage->stage, path->path);
        if (proc) {
            return new UsdProcGenerativeProceduralOpaque(proc);
        }
        return nullptr;
#else
        auto* wrapper = new UsdProcGenerativeProceduralOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdProcGenerativeProcedural proc = UsdProcGenerativeProcedural::Get(stage->stage, path->path);
        if (proc) {
            return new UsdProcGenerativeProceduralOpaque(proc);
        }
        return nullptr;
#else
        auto* wrapper = new UsdProcGenerativeProceduralOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdProcGenerativeProcedural proc(prim->prim);
        if (proc) {
            return new UsdProcGenerativeProceduralOpaque(proc);
        }
        return nullptr;
#else
        auto* wrapper = new UsdProcGenerativeProceduralOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_Retain(UsdProcGenerativeProceduralRef proc) {
    if (!proc) return nullptr;
    proc->refCount.fetch_add(1, std::memory_order_relaxed);
    return proc;
}

void UsdProcGenerativeProcedural_Release(UsdProcGenerativeProceduralRef proc) {
    if (!proc) return;
    if (proc->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete proc;
    }
}

bool UsdProcGenerativeProcedural_IsValid(UsdProcGenerativeProceduralRef proc) {
    if (!proc) return false;
#if USD_USE_FULL
    return proc->proc;
#else
    return proc->valid;
#endif
}

UsdPrimRef UsdProcGenerativeProcedural_GetPrim(UsdProcGenerativeProceduralRef proc) {
    if (!proc) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = proc->proc.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

// ============================================================================
// MARK: - ProceduralSystem Attribute Implementation
// ============================================================================

char* UsdProcGenerativeProcedural_GetProceduralSystem(UsdProcGenerativeProceduralRef proc, UsdTimeCode time) {
    if (!proc) return nullptr;
#if USD_USE_FULL
    try {
        TfToken system;
        proc->proc.GetProceduralSystemAttr().Get(&system,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        std::string systemStr = system.GetString();
        char* result = static_cast<char*>(malloc(systemStr.size() + 1));
        if (result) {
            strcpy(result, systemStr.c_str());
        }
        return result;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(proc->proceduralSystem.size() + 1));
    if (result) {
        strcpy(result, proc->proceduralSystem.c_str());
    }
    return result;
#endif
}

UsdResult UsdProcGenerativeProcedural_SetProceduralSystem(UsdProcGenerativeProceduralRef proc, const char* system, UsdTimeCode time) {
    if (!proc || !system) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        proc->proc.GetProceduralSystemAttr().Set(TfToken(system),
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    proc->proceduralSystem = system;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdProcGenerativeProcedural_CreateProceduralSystemAttr(UsdProcGenerativeProceduralRef proc, const char* defaultValue) {
    if (!proc) return false;
#if USD_USE_FULL
    try {
        VtValue defVal;
        if (defaultValue) {
            defVal = VtValue(TfToken(defaultValue));
        }
        UsdAttribute attr = proc->proc.CreateProceduralSystemAttr(defVal);
        return attr.IsValid();
    } CATCH_AND_RETURN(false)
#else
    if (defaultValue) {
        proc->proceduralSystem = defaultValue;
    }
    return true;
#endif
}

bool UsdProcGenerativeProcedural_HasProceduralSystem(UsdProcGenerativeProceduralRef proc) {
    if (!proc) return false;
#if USD_USE_FULL
    try {
        return proc->proc.GetProceduralSystemAttr().HasValue();
    } CATCH_AND_RETURN(false)
#else
    return !proc->proceduralSystem.empty();
#endif
}

// ============================================================================
// MARK: - Boundable API Implementation
// ============================================================================

UsdResult UsdProcGenerativeProcedural_GetExtent(UsdProcGenerativeProceduralRef proc, UsdTimeCode time, float* outExtent) {
    if (!proc || !outExtent) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtArray<GfVec3f> extent;
        UsdGeomBoundable boundable(proc->proc);
        if (boundable.GetExtentAttr().Get(&extent,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            if (extent.size() >= 2) {
                outExtent[0] = extent[0][0];
                outExtent[1] = extent[0][1];
                outExtent[2] = extent[0][2];
                outExtent[3] = extent[1][0];
                outExtent[4] = extent[1][1];
                outExtent[5] = extent[1][2];
                return USD_RESULT_SUCCESS;
            }
        }
        // Return default extent if not found
        outExtent[0] = outExtent[1] = outExtent[2] = -0.5f;
        outExtent[3] = outExtent[4] = outExtent[5] = 0.5f;
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outExtent[0] = proc->extent[0];
    outExtent[1] = proc->extent[1];
    outExtent[2] = proc->extent[2];
    outExtent[3] = proc->extent[3];
    outExtent[4] = proc->extent[4];
    outExtent[5] = proc->extent[5];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdProcGenerativeProcedural_SetExtent(UsdProcGenerativeProceduralRef proc, const float* extent, UsdTimeCode time) {
    if (!proc || !extent) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtArray<GfVec3f> extentArray(2);
        extentArray[0] = GfVec3f(extent[0], extent[1], extent[2]);
        extentArray[1] = GfVec3f(extent[3], extent[4], extent[5]);
        UsdGeomBoundable boundable(proc->proc);
        boundable.GetExtentAttr().Set(extentArray,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    proc->extent[0] = extent[0];
    proc->extent[1] = extent[1];
    proc->extent[2] = extent[2];
    proc->extent[3] = extent[3];
    proc->extent[4] = extent[4];
    proc->extent[5] = extent[5];
    return USD_RESULT_SUCCESS;
#endif
}

} // extern "C"
