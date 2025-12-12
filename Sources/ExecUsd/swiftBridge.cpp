//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "ExecUsd/swiftBridge.h"

// Include Gf types for typed value extraction
#include "Gf/matrix4d.h"
#include "Gf/matrix4f.h"
#include "Gf/vec2f.h"
#include "Gf/vec2d.h"
#include "Gf/vec3f.h"
#include "Gf/vec3d.h"
#include "Gf/vec4f.h"
#include "Gf/vec4d.h"
#include "Gf/quatf.h"
#include "Gf/quatd.h"

// Include the full headers for the implementation.
// These are NOT exposed to Swift (the header carefully avoids them)
// but we need them here to actually work with the types.
// These headers are in the private/ExecUsd/ directory to prevent SwiftPM from discovering them.
// The headerSearchPath("private") setting allows includes like "ExecUsd/system.h" to work.
#include "ExecUsd/system.h"
#include "ExecUsd/request.h"
#include "ExecUsd/cacheView.h"
#include "ExecUsd/valueKey.h"
#include "Exec/systemDiagnostics.h"
#include "Exec/request.h"  // For ExecRequestIndexSet and callback types
#include "Ef/time.h"
#include "Ef/timeInterval.h"
#include "Gf/interval.h"
#include "Gf/multiInterval.h"

#include <vector>
#include <limits>

PXR_NAMESPACE_OPEN_SCOPE

// ---------------------------------------------------------------------------
// ExecUsdSystem Bridge Functions
// ---------------------------------------------------------------------------

void*
ExecUsd_Swift_CreateSystem(const UsdStageConstRefPtr &stage)
{
    return static_cast<void*>(new ExecUsdSystem(stage));
}

void*
ExecUsd_Swift_CreateSystemFromStage(const UsdStageRefPtr &stage)
{
    // UsdStageRefPtr can be implicitly converted to UsdStageConstRefPtr
    return static_cast<void*>(new ExecUsdSystem(stage));
}

void
ExecUsd_Swift_DestroySystem(void *system)
{
    delete static_cast<ExecUsdSystem*>(system);
}

void
ExecUsd_Swift_ChangeTime(void *system, double time)
{
    if (system) {
        static_cast<ExecUsdSystem*>(system)->ChangeTime(UsdTimeCode(time));
    }
}

void
ExecUsd_Swift_ChangeTimeCode(void *system, UsdTimeCode time)
{
    if (system) {
        static_cast<ExecUsdSystem*>(system)->ChangeTime(time);
    }
}

void
ExecUsd_Swift_ChangeTimeWithFlags(
    void *system,
    double timeCodeValue,
    bool isDefault,
    uint8_t splineFlags)
{
    if (system) {
        // Construct the appropriate UsdTimeCode
        UsdTimeCode timeCode = isDefault ? UsdTimeCode::Default() : UsdTimeCode(timeCodeValue);

        // Note: The public ExecUsdSystem::ChangeTime only takes UsdTimeCode,
        // which internally creates EfTime(timeCode) with splineFlags=0.
        // The splineFlags parameter is preserved here for future API expansion
        // when the C++ API exposes EfTime directly.
        // For now, we can only pass the time code portion.
        (void)splineFlags; // Unused until C++ API is extended

        static_cast<ExecUsdSystem*>(system)->ChangeTime(timeCode);
    }
}

// ---------------------------------------------------------------------------
// ExecUsdValueKey Bridge Functions
// ---------------------------------------------------------------------------

void*
ExecUsd_Swift_CreateValueKeyFromAttribute(const UsdAttribute &provider)
{
    return static_cast<void*>(new ExecUsdValueKey(provider));
}

void*
ExecUsd_Swift_CreateValueKeyFromAttributeAndToken(
    const UsdAttribute &provider,
    const TfToken &computation)
{
    return static_cast<void*>(new ExecUsdValueKey(provider, computation));
}

void*
ExecUsd_Swift_CreateValueKeyFromPrimAndToken(
    const UsdPrim &provider,
    const TfToken &computation)
{
    return static_cast<void*>(new ExecUsdValueKey(provider, computation));
}

void
ExecUsd_Swift_DestroyValueKey(void *valueKey)
{
    delete static_cast<ExecUsdValueKey*>(valueKey);
}

// ---------------------------------------------------------------------------
// ExecUsdRequest Bridge Functions
// ---------------------------------------------------------------------------

ExecUsd_Swift_RequestHandle
ExecUsd_Swift_BuildRequest(
    void *system,
    void *valueKeyVector)
{
    ExecUsd_Swift_RequestHandle handle;
    handle.request = nullptr;
    handle.isValid = false;

    if (!system || !valueKeyVector) {
        return handle;
    }

    ExecUsdSystem *typedSystem = static_cast<ExecUsdSystem*>(system);
    std::vector<ExecUsdValueKey> *typedVec =
        static_cast<std::vector<ExecUsdValueKey>*>(valueKeyVector);

    // Make a copy of the value keys since BuildRequest takes by rvalue
    std::vector<ExecUsdValueKey> keysCopy = *typedVec;

    // Build the request - note we allocate on heap since ExecUsdRequest
    // contains unique_ptr and Swift can't handle that directly
    ExecUsdRequest request = typedSystem->BuildRequest(std::move(keysCopy));

    if (request.IsValid()) {
        handle.request = static_cast<void*>(new ExecUsdRequest(std::move(request)));
        handle.isValid = true;
    }

    return handle;
}

void
ExecUsd_Swift_DestroyRequest(ExecUsd_Swift_RequestHandle *handle)
{
    if (handle && handle->request) {
        // IMPORTANT: The Swift wrapper (ExecUsd.System) tracks all requests
        // and ensures they are destroyed before the system. This prevents the
        // use-after-free crash that would occur if the system was destroyed
        // before its requests (Request destructor accesses system's request tracker).
        delete static_cast<ExecUsdRequest*>(handle->request);
        handle->request = nullptr;
        handle->isValid = false;
    }
}

bool
ExecUsd_Swift_RequestIsValid(const ExecUsd_Swift_RequestHandle *handle)
{
    if (!handle || !handle->isValid || !handle->request) {
        return false;
    }
    return static_cast<ExecUsdRequest*>(handle->request)->IsValid();
}

void
ExecUsd_Swift_PrepareRequest(
    void *system,
    ExecUsd_Swift_RequestHandle *handle)
{
    if (system && handle && handle->request) {
        ExecUsdSystem *typedSystem = static_cast<ExecUsdSystem*>(system);
        ExecUsdRequest *typedRequest = static_cast<ExecUsdRequest*>(handle->request);
        typedSystem->PrepareRequest(*typedRequest);
    }
}

void*
ExecUsd_Swift_Compute(
    void *system,
    ExecUsd_Swift_RequestHandle *handle)
{
    if (system && handle && handle->request) {
        ExecUsdSystem *typedSystem = static_cast<ExecUsdSystem*>(system);
        ExecUsdRequest *typedRequest = static_cast<ExecUsdRequest*>(handle->request);
        // Allocate on heap and return as opaque pointer
        ExecUsdCacheView cacheView = typedSystem->Compute(*typedRequest);
        return static_cast<void*>(new ExecUsdCacheView(std::move(cacheView)));
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// ExecUsdCacheView Bridge Functions
// ---------------------------------------------------------------------------

void
ExecUsd_Swift_DestroyCacheView(void *cacheView)
{
    if (cacheView) {
        delete static_cast<ExecUsdCacheView*>(cacheView);
    }
}

VtValue
ExecUsd_Swift_CacheViewGet(void *cacheView, int index)
{
    if (cacheView) {
        return static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
    }
    return VtValue();
}

// ---------------------------------------------------------------------------
// ExecUsdCacheView Typed Value Extraction
// ---------------------------------------------------------------------------

GfMatrix4d
ExecUsd_Swift_CacheViewGetMatrix4d(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfMatrix4d>()) {
            return v.UncheckedGet<GfMatrix4d>();
        }
    }
    return GfMatrix4d(1.0); // Identity
}

GfMatrix4f
ExecUsd_Swift_CacheViewGetMatrix4f(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfMatrix4f>()) {
            return v.UncheckedGet<GfMatrix4f>();
        }
    }
    return GfMatrix4f(1.0f); // Identity
}

double
ExecUsd_Swift_CacheViewGetDouble(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<double>()) {
            return v.UncheckedGet<double>();
        }
    }
    return 0.0;
}

float
ExecUsd_Swift_CacheViewGetFloat(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<float>()) {
            return v.UncheckedGet<float>();
        }
    }
    return 0.0f;
}

int
ExecUsd_Swift_CacheViewGetInt(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<int>()) {
            return v.UncheckedGet<int>();
        }
    }
    return 0;
}

bool
ExecUsd_Swift_CacheViewGetBool(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<bool>()) {
            return v.UncheckedGet<bool>();
        }
    }
    return false;
}

GfVec2f
ExecUsd_Swift_CacheViewGetVec2f(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfVec2f>()) {
            return v.UncheckedGet<GfVec2f>();
        }
    }
    return GfVec2f(0.0f);
}

GfVec2d
ExecUsd_Swift_CacheViewGetVec2d(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfVec2d>()) {
            return v.UncheckedGet<GfVec2d>();
        }
    }
    return GfVec2d(0.0);
}

GfVec3f
ExecUsd_Swift_CacheViewGetVec3f(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfVec3f>()) {
            return v.UncheckedGet<GfVec3f>();
        }
    }
    return GfVec3f(0.0f);
}

GfVec3d
ExecUsd_Swift_CacheViewGetVec3d(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfVec3d>()) {
            return v.UncheckedGet<GfVec3d>();
        }
    }
    return GfVec3d(0.0);
}

GfVec4f
ExecUsd_Swift_CacheViewGetVec4f(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfVec4f>()) {
            return v.UncheckedGet<GfVec4f>();
        }
    }
    return GfVec4f(0.0f);
}

GfVec4d
ExecUsd_Swift_CacheViewGetVec4d(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfVec4d>()) {
            return v.UncheckedGet<GfVec4d>();
        }
    }
    return GfVec4d(0.0);
}

GfQuatf
ExecUsd_Swift_CacheViewGetQuatf(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfQuatf>()) {
            return v.UncheckedGet<GfQuatf>();
        }
    }
    return GfQuatf::GetIdentity();
}

GfQuatd
ExecUsd_Swift_CacheViewGetQuatd(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (v.IsHolding<GfQuatd>()) {
            return v.UncheckedGet<GfQuatd>();
        }
    }
    return GfQuatd::GetIdentity();
}

// ---------------------------------------------------------------------------
// ExecUsdCacheView Type Checking
// ---------------------------------------------------------------------------

bool
ExecUsd_Swift_CacheViewIsHoldingMatrix4d(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<GfMatrix4d>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingMatrix4f(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<GfMatrix4f>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingDouble(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<double>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingFloat(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<float>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingInt(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<int>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingBool(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<bool>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingVec3f(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<GfVec3f>();
    }
    return false;
}

bool
ExecUsd_Swift_CacheViewIsHoldingVec3d(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        return v.IsHolding<GfVec3d>();
    }
    return false;
}

std::string
ExecUsd_Swift_CacheViewGetTypeName(void *cacheView, int index)
{
    if (cacheView) {
        VtValue v = static_cast<ExecUsdCacheView*>(cacheView)->Get(index);
        if (!v.IsEmpty()) {
            return v.GetTypeName();
        }
    }
    return std::string();
}

// ---------------------------------------------------------------------------
// Value Key Vector Helpers
// ---------------------------------------------------------------------------

void*
ExecUsd_Swift_CreateValueKeyVector()
{
    return static_cast<void*>(new std::vector<ExecUsdValueKey>());
}

void
ExecUsd_Swift_DestroyValueKeyVector(void *vec)
{
    delete static_cast<std::vector<ExecUsdValueKey>*>(vec);
}

void
ExecUsd_Swift_ValueKeyVectorPush(void *vec, void *key)
{
    if (vec && key) {
        std::vector<ExecUsdValueKey> *typedVec =
            static_cast<std::vector<ExecUsdValueKey>*>(vec);
        ExecUsdValueKey *typedKey = static_cast<ExecUsdValueKey*>(key);
        typedVec->push_back(*typedKey);
    }
}

size_t
ExecUsd_Swift_ValueKeyVectorSize(void *vec)
{
    if (vec) {
        return static_cast<std::vector<ExecUsdValueKey>*>(vec)->size();
    }
    return 0;
}

void
ExecUsd_Swift_ValueKeyVectorClear(void *vec)
{
    if (vec) {
        static_cast<std::vector<ExecUsdValueKey>*>(vec)->clear();
    }
}

// ---------------------------------------------------------------------------
// ExecSystem Diagnostics Bridge Functions
// ---------------------------------------------------------------------------

void*
ExecUsd_Swift_CreateDiagnostics(void *system)
{
    if (!system) {
        return nullptr;
    }
    // ExecUsdSystem inherits from ExecSystem, so we can pass it directly
    ExecUsdSystem *typedSystem = static_cast<ExecUsdSystem*>(system);
    return static_cast<void*>(new ExecSystem::Diagnostics(typedSystem));
}

void
ExecUsd_Swift_DestroyDiagnostics(void *diagnostics)
{
    if (diagnostics) {
        delete static_cast<ExecSystem::Diagnostics*>(diagnostics);
    }
}

void
ExecUsd_Swift_Diagnostics_InvalidateAll(void *diagnostics)
{
    if (diagnostics) {
        static_cast<ExecSystem::Diagnostics*>(diagnostics)->InvalidateAll();
    }
}

void
ExecUsd_Swift_Diagnostics_GraphNetwork(void *diagnostics, const char *filename)
{
    if (diagnostics && filename) {
        static_cast<ExecSystem::Diagnostics*>(diagnostics)->GraphNetwork(filename);
    }
}

// ---------------------------------------------------------------------------
// Invalidation Callback Bridge Functions
// ---------------------------------------------------------------------------

ExecUsd_Swift_RequestHandle
ExecUsd_Swift_BuildRequestWithCallbacks(
    void *system,
    void *valueKeyVector,
    ExecUsd_Swift_ValueInvalidationCallback valueCallback,
    void *valueContext,
    ExecUsd_Swift_TimeChangeCallback timeCallback,
    void *timeContext)
{
    ExecUsd_Swift_RequestHandle handle;
    handle.request = nullptr;
    handle.isValid = false;

    if (!system || !valueKeyVector) {
        return handle;
    }

    ExecUsdSystem *typedSystem = static_cast<ExecUsdSystem*>(system);
    std::vector<ExecUsdValueKey> *typedVec =
        static_cast<std::vector<ExecUsdValueKey>*>(valueKeyVector);

    // Make a copy of the value keys since BuildRequest takes by rvalue
    std::vector<ExecUsdValueKey> keysCopy = *typedVec;

    // Create std::function wrappers for the C-style callbacks
    ExecRequestComputedValueInvalidationCallback wrappedValueCallback;
    if (valueCallback) {
        // Capture the C callback and context by value
        wrappedValueCallback = [valueCallback, valueContext](
            const ExecRequestIndexSet &indexSet,
            const EfTimeInterval &timeInterval)
        {
            // Convert the robin_set to a contiguous array
            std::vector<int> indices(indexSet.begin(), indexSet.end());

            // Extract time interval bounds from the multi-interval
            double timeMin = -std::numeric_limits<double>::infinity();
            double timeMax = std::numeric_limits<double>::infinity();

            const GfMultiInterval &multiInterval = timeInterval.GetTimeMultiInterval();
            if (!multiInterval.IsEmpty()) {
                // Get the bounds of the multi-interval
                timeMin = multiInterval.GetBounds().GetMin();
                timeMax = multiInterval.GetBounds().GetMax();
            }

            // Call the Swift callback
            valueCallback(
                valueContext,
                indices.data(),
                indices.size(),
                timeMin,
                timeMax,
                timeInterval.IsDefaultTimeSet());
        };
    }

    ExecRequestTimeChangeInvalidationCallback wrappedTimeCallback;
    if (timeCallback) {
        // Capture the C callback and context by value
        wrappedTimeCallback = [timeCallback, timeContext](
            const ExecRequestIndexSet &indexSet)
        {
            // Convert the robin_set to a contiguous array
            std::vector<int> indices(indexSet.begin(), indexSet.end());

            // Call the Swift callback
            timeCallback(
                timeContext,
                indices.data(),
                indices.size());
        };
    }

    // Build the request with callbacks
    ExecUsdRequest request = typedSystem->BuildRequest(
        std::move(keysCopy),
        std::move(wrappedValueCallback),
        std::move(wrappedTimeCallback));

    if (request.IsValid()) {
        handle.request = static_cast<void*>(new ExecUsdRequest(std::move(request)));
        handle.isValid = true;
    }

    return handle;
}

PXR_NAMESPACE_CLOSE_SCOPE
