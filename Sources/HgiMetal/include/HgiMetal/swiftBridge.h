//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_IMAGING_HGI_METAL_SWIFT_BRIDGE_H
#define PXR_IMAGING_HGI_METAL_SWIFT_BRIDGE_H

/// \file HgiMetal/swiftBridge.h
/// Swift-compatible bridge functions for HgiMetal types.
///
/// This header provides bridge functions to work around Swift C++ interop
/// limitations when dealing with HgiMetal types. Specifically, these functions
/// handle the casting from base HgiTexture handles to HgiMetalTexture to
/// access Metal-specific functionality.

#include <Metal/Metal.h>

#include "pxr/pxr.h"
#include "HgiMetal/api.h"
#include "Hgi/texture.h"

PXR_NAMESPACE_OPEN_SCOPE

/// Gets the MTLTexture from an HgiTextureHandle.
/// This function casts the generic HgiTexture* from the handle to HgiMetalTexture*
/// and retrieves the underlying MTLTexture.
///
/// \param handle The HgiTextureHandle containing the Metal texture
/// \return The underlying MTLTexture, or nil if the handle is empty
///         or doesn't contain an HgiMetalTexture
///
/// Usage from Swift:
/// \code
/// guard let metalTexture = Pixar.HgiMetal_GetMTLTexture(hgiTextureHandle)
/// else { return nil }
/// \endcode
HGIMETAL_API
id<MTLTexture> HgiMetal_GetMTLTexture(const HgiTextureHandle& handle);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_IMAGING_HGI_METAL_SWIFT_BRIDGE_H
