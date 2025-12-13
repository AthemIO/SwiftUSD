//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "Usd/swiftBridge.h"
#include "Usd/attribute.h"
#include "Usd/prim.h"
#include "Usd/primRange.h"
#include "Usd/stage.h"
#include "Sdf/assetPath.h"
#include "Tf/token.h"
#include "Vt/value.h"

PXR_NAMESPACE_OPEN_SCOPE

// ---------------------------------------------------------------------------
// UsdAttribute Swift Bridge Functions
// ---------------------------------------------------------------------------

bool
Usd_Swift_AttributeGet(
    const UsdAttribute& attr,
    VtValue* value,
    UsdTimeCode time)
{
    if (!attr.IsValid() || !value) {
        return false;
    }
    return attr.Get(value, time);
}

bool
Usd_Swift_AttributeSet(
    const UsdAttribute& attr,
    const VtValue& value,
    UsdTimeCode time)
{
    if (!attr.IsValid()) {
        return false;
    }
    return attr.Set(value, time);
}

std::string
Usd_Swift_AttributeGetName(const UsdAttribute& attr)
{
    if (!attr.IsValid()) {
        return std::string();
    }
    return attr.GetName().GetString();
}

std::string
Usd_Swift_AttributeGetPath(const UsdAttribute& attr)
{
    if (!attr.IsValid()) {
        return std::string();
    }
    return attr.GetPath().GetString();
}

bool
Usd_Swift_AttributeHoldsAssetPath(
    const UsdAttribute& attr,
    UsdTimeCode time)
{
    if (!attr.IsValid()) {
        return false;
    }
    VtValue value;
    if (!attr.Get(&value, time)) {
        return false;
    }
    return value.IsHolding<SdfAssetPath>();
}

SdfAssetPath
Usd_Swift_AttributeGetAssetPath(
    const UsdAttribute& attr,
    UsdTimeCode time)
{
    if (!attr.IsValid()) {
        return SdfAssetPath();
    }
    VtValue value;
    if (!attr.Get(&value, time)) {
        return SdfAssetPath();
    }
    if (value.IsHolding<SdfAssetPath>()) {
        return value.UncheckedGet<SdfAssetPath>();
    }
    return SdfAssetPath();
}

bool
Usd_Swift_AttributeSetAssetPath(
    const UsdAttribute& attr,
    const SdfAssetPath& assetPath,
    UsdTimeCode time)
{
    if (!attr.IsValid()) {
        return false;
    }
    return attr.Set(assetPath, time);
}

// ---------------------------------------------------------------------------
// UsdPrim Attribute Access Swift Bridge Functions
// ---------------------------------------------------------------------------

size_t
Usd_Swift_PrimGetAttributeCount(const UsdPrim& prim)
{
    if (!prim.IsValid()) {
        return 0;
    }
    std::vector<UsdAttribute> attrs = prim.GetAttributes();
    return attrs.size();
}

UsdAttribute
Usd_Swift_PrimGetAttributeAtIndex(const UsdPrim& prim, size_t index)
{
    if (!prim.IsValid()) {
        return UsdAttribute();
    }
    std::vector<UsdAttribute> attrs = prim.GetAttributes();
    if (index >= attrs.size()) {
        return UsdAttribute();
    }
    return attrs[index];
}

UsdAttribute
Usd_Swift_PrimGetAttributeByName(
    const UsdPrim& prim,
    const std::string& attrName)
{
    if (!prim.IsValid()) {
        return UsdAttribute();
    }
    return prim.GetAttribute(TfToken(attrName));
}

bool
Usd_Swift_PrimHasAttribute(
    const UsdPrim& prim,
    const std::string& attrName)
{
    if (!prim.IsValid()) {
        return false;
    }
    return prim.HasAttribute(TfToken(attrName));
}

std::string
Usd_Swift_PrimGetPath(const UsdPrim& prim)
{
    if (!prim.IsValid()) {
        return std::string();
    }
    return prim.GetPath().GetString();
}

std::string
Usd_Swift_PrimGetTypeName(const UsdPrim& prim)
{
    if (!prim.IsValid()) {
        return std::string();
    }
    return prim.GetTypeName().GetString();
}

// ---------------------------------------------------------------------------
// TfToken Swift Bridge Functions
// ---------------------------------------------------------------------------

TfToken
Usd_Swift_CreateToken(const std::string& tokenString)
{
    return TfToken(tokenString);
}

std::string
Usd_Swift_GetTokenString(const TfToken& token)
{
    return token.GetString();
}

// ---------------------------------------------------------------------------
// UsdStage Texture Path Utilities
// ---------------------------------------------------------------------------

size_t
Usd_Swift_StageCountAssetPathAttributes(const UsdStageRefPtr& stage)
{
    if (!stage) {
        return 0;
    }

    size_t count = 0;
    for (const UsdPrim& prim : stage->Traverse()) {
        for (const UsdAttribute& attr : prim.GetAttributes()) {
            VtValue value;
            if (attr.Get(&value) && value.IsHolding<SdfAssetPath>()) {
                count++;
            }
        }
    }
    return count;
}

std::vector<Usd_Swift_AssetPathAttributeInfo>
Usd_Swift_StageGetAssetPathAttributes(const UsdStageRefPtr& stage)
{
    std::vector<Usd_Swift_AssetPathAttributeInfo> results;

    if (!stage) {
        return results;
    }

    for (const UsdPrim& prim : stage->Traverse()) {
        for (const UsdAttribute& attr : prim.GetAttributes()) {
            VtValue value;
            if (attr.Get(&value) && value.IsHolding<SdfAssetPath>()) {
                SdfAssetPath assetPath = value.UncheckedGet<SdfAssetPath>();

                Usd_Swift_AssetPathAttributeInfo info;
                info.primPath = prim.GetPath().GetString();
                info.attrName = attr.GetName().GetString();
                info.assetPath = assetPath.GetAssetPath();
                info.resolvedPath = assetPath.GetResolvedPath();

                results.push_back(std::move(info));
            }
        }
    }

    return results;
}

PXR_NAMESPACE_CLOSE_SCOPE
