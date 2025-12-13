//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "Sdf/swiftBridge.h"
#include "Sdf/assetPath.h"
#include "Sdf/changeBlock.h"
#include "Sdf/layer.h"
#include "Sdf/layerOffset.h"
#include "Sdf/path.h"
#include "Vt/value.h"

PXR_NAMESPACE_OPEN_SCOPE

// ---------------------------------------------------------------------------
// SdfChangeBlock Bridge Functions
// ---------------------------------------------------------------------------

// Internal structure to hold the change block.
// We allocate this on the heap and return as void* because SdfChangeBlock
// is an RAII type that Swift cannot directly use.
struct Sdf_Swift_ChangeBlockHandle {
    SdfChangeBlock changeBlock;
};

void*
Sdf_Swift_BeginChangeBlock()
{
    // Allocate a new change block on the heap.
    // The SdfChangeBlock constructor will begin the scope.
    return static_cast<void*>(new Sdf_Swift_ChangeBlockHandle());
}

void
Sdf_Swift_EndChangeBlock(void* changeBlockHandle)
{
    if (changeBlockHandle) {
        // Delete the handle, which will invoke SdfChangeBlock's destructor
        // and end the change block scope.
        delete static_cast<Sdf_Swift_ChangeBlockHandle*>(changeBlockHandle);
    }
}

// ---------------------------------------------------------------------------
// SdfLayer Factory Bridge Functions
// ---------------------------------------------------------------------------

SdfLayerRefPtr
Sdf_Swift_CreateAnonymousLayer(const std::string& tag)
{
    return SdfLayer::CreateAnonymous(tag);
}

SdfLayerRefPtr
Sdf_Swift_FindOrOpen(const std::string& identifier)
{
    return SdfLayer::FindOrOpen(identifier);
}

SdfLayerRefPtr
Sdf_Swift_CreateNew(const std::string& identifier)
{
    return SdfLayer::CreateNew(identifier);
}

// ---------------------------------------------------------------------------
// SdfPath Bridge Functions
// ---------------------------------------------------------------------------

SdfPath
Sdf_Swift_CreatePath(const std::string& pathString)
{
    return SdfPath(pathString);
}

SdfPath
Sdf_Swift_AbsoluteRootPath()
{
    return SdfPath::AbsoluteRootPath();
}

SdfPath
Sdf_Swift_EmptyPath()
{
    return SdfPath::EmptyPath();
}

// ---------------------------------------------------------------------------
// SdfLayerOffset Bridge Functions
// ---------------------------------------------------------------------------

SdfLayerOffset
Sdf_Swift_IdentityLayerOffset()
{
    return SdfLayerOffset();
}

SdfLayerOffset
Sdf_Swift_CreateLayerOffset(double offset, double scale)
{
    return SdfLayerOffset(offset, scale);
}

// ---------------------------------------------------------------------------
// SdfAssetPath Bridge Functions
// ---------------------------------------------------------------------------

SdfAssetPath
Sdf_Swift_CreateAssetPath(const std::string& assetPath)
{
    return SdfAssetPath(assetPath);
}

SdfAssetPath
Sdf_Swift_CreateAssetPathWithResolved(
    const std::string& authoredPath,
    const std::string& resolvedPath)
{
    return SdfAssetPath(authoredPath, resolvedPath);
}

std::string
Sdf_Swift_GetAssetPathString(const SdfAssetPath& assetPath)
{
    return assetPath.GetAssetPath();
}

std::string
Sdf_Swift_GetAssetPathAuthoredString(const SdfAssetPath& assetPath)
{
    return assetPath.GetAuthoredPath();
}

std::string
Sdf_Swift_GetAssetPathResolvedString(const SdfAssetPath& assetPath)
{
    return assetPath.GetResolvedPath();
}

bool
Sdf_Swift_VtValueHoldsAssetPath(const VtValue& value)
{
    return value.IsHolding<SdfAssetPath>();
}

SdfAssetPath
Sdf_Swift_VtValueGetAssetPath(const VtValue& value)
{
    if (value.IsHolding<SdfAssetPath>()) {
        return value.UncheckedGet<SdfAssetPath>();
    }
    return SdfAssetPath();
}

VtValue
Sdf_Swift_VtValueFromAssetPath(const SdfAssetPath& assetPath)
{
    return VtValue(assetPath);
}

// ---------------------------------------------------------------------------
// SdfPath String Bridge Functions
// ---------------------------------------------------------------------------

std::string
Sdf_Swift_GetPathString(const SdfPath& path)
{
    return path.GetString();
}

std::string
Sdf_Swift_GetPathName(const SdfPath& path)
{
    return path.GetName();
}

PXR_NAMESPACE_CLOSE_SCOPE
