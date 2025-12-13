//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_SDF_SWIFT_BRIDGE_H
#define PXR_USD_SDF_SWIFT_BRIDGE_H

/// \file Sdf/swiftBridge.h
/// Swift-compatible bridge functions for Sdf types.
///
/// This header provides functions to work around Swift C++ interop limitations:
///
/// 1. SdfLayer reference counting - SdfLayer uses SWIFT_SHARED_REFERENCE which
///    requires retain/release functions for Swift's ARC to manage the lifecycle.
///
/// 2. SdfChangeBlock RAII scope - Swift cannot directly use C++ RAII types,
///    so we provide explicit begin/end functions.
///
/// 3. Static data access - Swift cannot import TfStaticData<>::operator->(),
///    so we provide helper functions for common lookups.
///
/// ## Usage from Swift
///
/// ### SdfLayer (automatic via SWIFT_SHARED_REFERENCE)
/// SdfLayer is annotated with SWIFT_SHARED_REFERENCE(SdfLayerRetain, SdfLayerRelease)
/// which allows Swift to automatically manage the reference count via ARC.
/// No explicit calls to retain/release are needed from Swift code.
///
/// ### SdfChangeBlock
/// \code
/// let changeBlock = Pixar.Sdf_Swift_BeginChangeBlock()
/// defer { Pixar.Sdf_Swift_EndChangeBlock(changeBlock) }
/// // ... make multiple Sdf changes ...
/// \endcode

#include "pxr/pxr.h"
#include "Sdf/api.h"
#include "Sdf/assetPath.h"
#include "Sdf/layer.h"
#include "Sdf/path.h"
#include "Tf/token.h"
#include "Vt/value.h"

#include <string>

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name SdfChangeBlock Swift Bridge Functions
///
/// Bridge functions for SdfChangeBlock RAII scope.
/// Swift cannot directly use C++ RAII types, so we provide
/// explicit begin/end functions that manage the scope.

/// Begins an SdfChangeBlock scope.
/// Returns an opaque handle that must be passed to Sdf_Swift_EndChangeBlock.
///
/// While a change block is open, Sdf delays sending change notifications.
/// This improves performance when making many related changes.
///
/// WARNING: Do not use Usd or other downstream APIs while a change block
/// is open! See SdfChangeBlock documentation for details.
///
/// Thread safety: Should be called on the main thread.
SDF_API
void* Sdf_Swift_BeginChangeBlock();

/// Ends an SdfChangeBlock scope.
/// The handle parameter must be the value returned by Sdf_Swift_BeginChangeBlock.
/// After this call, the handle is invalid and must not be used.
///
/// Thread safety: Should be called on the main thread.
SDF_API
void Sdf_Swift_EndChangeBlock(void* changeBlockHandle);

/// @}

/// @{
/// \name SdfLayer Factory Swift Bridge Functions
///
/// Bridge functions for creating SdfLayer instances.
/// These return SdfLayerRefPtr which Swift can manage via ARC
/// thanks to the SWIFT_SHARED_REFERENCE annotation on SdfLayer.

/// Creates a new anonymous layer.
/// Returns an SdfLayerRefPtr that Swift can manage via ARC.
SDF_API
SdfLayerRefPtr Sdf_Swift_CreateAnonymousLayer(const std::string& tag = std::string());

/// Opens an existing layer from a file path.
/// Returns an SdfLayerRefPtr, or a null RefPtr if the layer couldn't be opened.
SDF_API
SdfLayerRefPtr Sdf_Swift_FindOrOpen(const std::string& identifier);

/// Creates a new layer at the given file path.
/// Returns an SdfLayerRefPtr, or a null RefPtr if the layer couldn't be created.
SDF_API
SdfLayerRefPtr Sdf_Swift_CreateNew(const std::string& identifier);

/// @}

/// @{
/// \name SdfPath Swift Bridge Functions
///
/// Helper functions for SdfPath that work around Swift interop limitations.

/// Creates an SdfPath from a string.
/// This is a convenience function for Swift since the SdfPath constructor
/// taking std::string works but this makes the intent clearer.
SDF_API
SdfPath Sdf_Swift_CreatePath(const std::string& pathString);

/// Returns the absolute root path ("/").
SDF_API
SdfPath Sdf_Swift_AbsoluteRootPath();

/// Returns the empty path.
SDF_API
SdfPath Sdf_Swift_EmptyPath();

/// @}

/// @{
/// \name SdfLayerOffset Swift Bridge Functions
///
/// Helper functions for SdfLayerOffset operations.

/// Creates an identity layer offset (offset=0, scale=1).
SDF_API
SdfLayerOffset Sdf_Swift_IdentityLayerOffset();

/// Creates a layer offset with the given offset and scale.
SDF_API
SdfLayerOffset Sdf_Swift_CreateLayerOffset(double offset, double scale);

/// @}

/// @{
/// \name SdfAssetPath Swift Bridge Functions
///
/// Helper functions for SdfAssetPath that work around Swift interop limitations.
/// These are essential for texture path manipulation in USD scenes.

/// Creates an SdfAssetPath from an authored path string.
/// This is used when updating texture references in a USD stage.
SDF_API
SdfAssetPath Sdf_Swift_CreateAssetPath(const std::string& assetPath);

/// Creates an SdfAssetPath from authored and resolved paths.
/// Use this when you have both the original path and the resolved filesystem path.
SDF_API
SdfAssetPath Sdf_Swift_CreateAssetPathWithResolved(
    const std::string& authoredPath,
    const std::string& resolvedPath);

/// Gets the asset path string from an SdfAssetPath.
/// Returns the evaluated path if available, otherwise the authored path.
SDF_API
std::string Sdf_Swift_GetAssetPathString(const SdfAssetPath& assetPath);

/// Gets the authored path string from an SdfAssetPath.
/// This is the path as written in the USD file.
SDF_API
std::string Sdf_Swift_GetAssetPathAuthoredString(const SdfAssetPath& assetPath);

/// Gets the resolved path string from an SdfAssetPath.
/// This is the fully resolved filesystem path, if available.
SDF_API
std::string Sdf_Swift_GetAssetPathResolvedString(const SdfAssetPath& assetPath);

/// Checks if a VtValue holds an SdfAssetPath.
SDF_API
bool Sdf_Swift_VtValueHoldsAssetPath(const VtValue& value);

/// Extracts an SdfAssetPath from a VtValue.
/// Returns an empty SdfAssetPath if the value doesn't hold an SdfAssetPath.
SDF_API
SdfAssetPath Sdf_Swift_VtValueGetAssetPath(const VtValue& value);

/// Creates a VtValue holding an SdfAssetPath.
SDF_API
VtValue Sdf_Swift_VtValueFromAssetPath(const SdfAssetPath& assetPath);

/// @}

/// @{
/// \name SdfPath String Swift Bridge Functions
///
/// Helper functions for getting string representations of SdfPath.

/// Gets the string representation of an SdfPath.
/// This is equivalent to SdfPath::GetText() but works around Swift interop issues.
SDF_API
std::string Sdf_Swift_GetPathString(const SdfPath& path);

/// Gets the name portion of an SdfPath (the last component).
SDF_API
std::string Sdf_Swift_GetPathName(const SdfPath& path);

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_SDF_SWIFT_BRIDGE_H
