//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_USD_SWIFT_BRIDGE_H
#define PXR_USD_USD_SWIFT_BRIDGE_H

/// \file Usd/swiftBridge.h
/// Swift-compatible bridge functions for USD types.
///
/// This header provides functions to work around Swift C++ interop limitations:
///
/// 1. UsdAttribute value access - Swift cannot use template methods like Get<T>()
///    so we provide type-erased VtValue accessors.
///
/// 2. UsdPrim attribute enumeration - Swift has issues with C++ vector iteration,
///    so we provide helper functions to access attributes by index.
///
/// 3. TfToken creation - Simplified token creation for attribute name lookup.
///
/// ## Usage from Swift
///
/// ### Getting Attribute Values
/// \code
/// let attr = prim.GetAttribute(Pixar.TfToken("inputs:file"))
/// var value = Pixar.VtValue()
/// if Pixar.Usd_Swift_AttributeGet(attr, &value, Pixar.UsdTimeCode.Default()) {
///     if Pixar.Sdf_Swift_VtValueHoldsAssetPath(value) {
///         let assetPath = Pixar.Sdf_Swift_VtValueGetAssetPath(value)
///         let pathString = Pixar.Sdf_Swift_GetAssetPathString(assetPath)
///     }
/// }
/// \endcode
///
/// ### Setting Attribute Values
/// \code
/// let newAssetPath = Pixar.Sdf_Swift_CreateAssetPath("/path/to/texture.exr")
/// let newValue = Pixar.Sdf_Swift_VtValueFromAssetPath(newAssetPath)
/// Pixar.Usd_Swift_AttributeSet(attr, newValue, Pixar.UsdTimeCode.Default())
/// \endcode
///
/// ### Iterating Prim Attributes
/// \code
/// let attrCount = Pixar.Usd_Swift_PrimGetAttributeCount(prim)
/// for i in 0..<attrCount {
///     let attr = Pixar.Usd_Swift_PrimGetAttributeAtIndex(prim, i)
///     let attrName = Pixar.Usd_Swift_AttributeGetName(attr)
///     // Process attribute...
/// }
/// \endcode

#include "pxr/pxrns.h"
#include "Usd/api.h"
#include "Usd/attribute.h"
#include "Usd/prim.h"
#include "Usd/stage.h"
#include "Usd/timeCode.h"
#include "Sdf/assetPath.h"
#include "Tf/token.h"
#include "Vt/value.h"

#include <string>
#include <vector>

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name UsdAttribute Swift Bridge Functions
///
/// Bridge functions for UsdAttribute value access.
/// These work around Swift's inability to use C++ template methods.

/// Gets the value of an attribute as a VtValue.
/// This is the type-erased equivalent of UsdAttribute::Get<T>().
/// \param attr The attribute to get the value from
/// \param value Output parameter for the value
/// \param time The time at which to get the value (defaults to default time)
/// \return true if the value was successfully retrieved
USD_API
bool Usd_Swift_AttributeGet(
    const UsdAttribute& attr,
    VtValue* value,
    UsdTimeCode time = UsdTimeCode::Default());

/// Sets the value of an attribute from a VtValue.
/// This is the type-erased equivalent of UsdAttribute::Set<T>().
/// \param attr The attribute to set the value on
/// \param value The value to set
/// \param time The time at which to set the value (defaults to default time)
/// \return true if the value was successfully set
USD_API
bool Usd_Swift_AttributeSet(
    const UsdAttribute& attr,
    const VtValue& value,
    UsdTimeCode time = UsdTimeCode::Default());

/// Gets the name of an attribute as a string.
/// \param attr The attribute to get the name from
/// \return The attribute name as a string
USD_API
std::string Usd_Swift_AttributeGetName(const UsdAttribute& attr);

/// Gets the full path of an attribute as a string.
/// \param attr The attribute to get the path from
/// \return The full attribute path as a string
USD_API
std::string Usd_Swift_AttributeGetPath(const UsdAttribute& attr);

/// Checks if an attribute holds an SdfAssetPath value.
/// \param attr The attribute to check
/// \param time The time at which to check (defaults to default time)
/// \return true if the attribute value is an SdfAssetPath
USD_API
bool Usd_Swift_AttributeHoldsAssetPath(
    const UsdAttribute& attr,
    UsdTimeCode time = UsdTimeCode::Default());

/// Gets an SdfAssetPath value from an attribute.
/// \param attr The attribute to get the value from
/// \param time The time at which to get the value (defaults to default time)
/// \return The SdfAssetPath value, or empty if not an asset path attribute
USD_API
SdfAssetPath Usd_Swift_AttributeGetAssetPath(
    const UsdAttribute& attr,
    UsdTimeCode time = UsdTimeCode::Default());

/// Sets an SdfAssetPath value on an attribute.
/// \param attr The attribute to set the value on
/// \param assetPath The asset path value to set
/// \param time The time at which to set the value (defaults to default time)
/// \return true if the value was successfully set
USD_API
bool Usd_Swift_AttributeSetAssetPath(
    const UsdAttribute& attr,
    const SdfAssetPath& assetPath,
    UsdTimeCode time = UsdTimeCode::Default());

/// @}

/// @{
/// \name UsdPrim Attribute Access Swift Bridge Functions
///
/// Bridge functions for accessing UsdPrim attributes from Swift.
/// These work around Swift's issues with C++ vector iteration.

/// Gets the number of attributes on a prim.
/// \param prim The prim to query
/// \return The number of attributes
USD_API
size_t Usd_Swift_PrimGetAttributeCount(const UsdPrim& prim);

/// Gets an attribute from a prim by index.
/// \param prim The prim to query
/// \param index The index of the attribute (0-based)
/// \return The attribute at the given index, or an invalid attribute if out of bounds
USD_API
UsdAttribute Usd_Swift_PrimGetAttributeAtIndex(const UsdPrim& prim, size_t index);

/// Gets an attribute from a prim by name.
/// \param prim The prim to query
/// \param attrName The name of the attribute as a string
/// \return The attribute with the given name, or an invalid attribute if not found
USD_API
UsdAttribute Usd_Swift_PrimGetAttributeByName(
    const UsdPrim& prim,
    const std::string& attrName);

/// Checks if a prim has an attribute with the given name.
/// \param prim The prim to query
/// \param attrName The name of the attribute as a string
/// \return true if the attribute exists
USD_API
bool Usd_Swift_PrimHasAttribute(
    const UsdPrim& prim,
    const std::string& attrName);

/// Gets the path of a prim as a string.
/// \param prim The prim to query
/// \return The prim path as a string
USD_API
std::string Usd_Swift_PrimGetPath(const UsdPrim& prim);

/// Gets the type name of a prim as a string.
/// \param prim The prim to query
/// \return The prim type name as a string
USD_API
std::string Usd_Swift_PrimGetTypeName(const UsdPrim& prim);

/// @}

/// @{
/// \name TfToken Swift Bridge Functions
///
/// Convenience functions for creating TfTokens from strings.

/// Creates a TfToken from a string.
/// \param tokenString The string value for the token
/// \return A new TfToken with the given string value
USD_API
TfToken Usd_Swift_CreateToken(const std::string& tokenString);

/// Gets the string representation of a TfToken.
/// \param token The token to convert
/// \return The string representation of the token
USD_API
std::string Usd_Swift_GetTokenString(const TfToken& token);

/// @}

/// @{
/// \name UsdStage Texture Path Utilities
///
/// Helper functions specifically for updating texture paths in USD stages.

/// Finds all attributes in a stage that hold SdfAssetPath values.
/// Returns the count of such attributes found.
/// \param stage The stage to search
/// \return The number of asset path attributes found
USD_API
size_t Usd_Swift_StageCountAssetPathAttributes(const UsdStageRefPtr& stage);

/// Structure to hold asset path attribute information for Swift consumption.
struct Usd_Swift_AssetPathAttributeInfo {
    std::string primPath;
    std::string attrName;
    std::string assetPath;
    std::string resolvedPath;
};

/// Gets information about all asset path attributes in a stage.
/// This is useful for iterating texture references.
/// \param stage The stage to search
/// \return Vector of attribute info structures
USD_API
std::vector<Usd_Swift_AssetPathAttributeInfo>
Usd_Swift_StageGetAssetPathAttributes(const UsdStageRefPtr& stage);

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_USD_SWIFT_BRIDGE_H
