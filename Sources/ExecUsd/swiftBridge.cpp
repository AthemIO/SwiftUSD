//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "ExecUsd/swiftBridge.h"

// Include the full headers for the implementation.
// These are NOT exposed to Swift (the header carefully avoids them)
// but we need them here to actually work with the types.
// These headers are in the private/ExecUsd/ directory to prevent SwiftPM from discovering them.
// The headerSearchPath("private") setting allows includes like "ExecUsd/system.h" to work.
#include "ExecUsd/system.h"
#include "ExecUsd/request.h"
#include "ExecUsd/cacheView.h"
#include "ExecUsd/valueKey.h"

#include <vector>

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

PXR_NAMESPACE_CLOSE_SCOPE
