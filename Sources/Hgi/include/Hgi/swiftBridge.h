//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_IMAGING_HGI_SWIFT_BRIDGE_H
#define PXR_IMAGING_HGI_SWIFT_BRIDGE_H

/// \file Hgi/swiftBridge.h
/// Swift-compatible bridge functions for Hgi types.
///
/// This header provides C-style factory functions to work around
/// Swift C++ interop limitations with std::unique_ptr.
///
/// IMPORTANT: Swift cannot import types that return std::unique_ptr.
/// These bridge functions return void* which Swift can work with,
/// and corresponding destroy functions handle cleanup.
///
/// Usage from Swift:
/// \code
/// // Create Hgi instance
/// guard let hgiPtr = Pixar.Hgi_Swift_CreatePlatformDefaultHgi() else { return }
/// defer { Pixar.Hgi_Swift_DestroyHgi(hgiPtr) }
///
/// // Create command buffer
/// guard let blitPtr = Pixar.Hgi_Swift_CreateBlitCmds(hgiPtr) else { return }
/// defer { Pixar.Hgi_Swift_DestroyBlitCmds(blitPtr) }
///
/// // Submit commands
/// Pixar.Hgi_Swift_SubmitCmds(hgiPtr, blitPtr, .HgiSubmitWaitTypeNoWait)
/// \endcode

#include "pxr/pxr.h"
#include "Hgi/api.h"
#include "Hgi/enums.h"
#include "Tf/token.h"

// Forward declare descriptor types that Swift CAN import
// (they don't contain unique_ptr)
#include "Hgi/graphicsCmdsDesc.h"
#include "Hgi/computeCmdsDesc.h"

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name Hgi Instance Swift Bridge Functions
///
/// Factory functions for creating Hgi instances.
/// These return opaque void* pointers because HgiUniquePtr (std::unique_ptr<Hgi>)
/// cannot be imported by Swift.

/// Creates the platform default Hgi instance.
/// On macOS returns HgiMetal, on Linux returns HgiGL, etc.
/// Returns an opaque pointer that must be destroyed with Hgi_Swift_DestroyHgi.
/// Returns nullptr if creation fails.
/// Thread safety: Not thread safe.
HGI_API
void* Hgi_Swift_CreatePlatformDefaultHgi();

/// Creates an Hgi instance of the specified type.
/// Valid tokens: HgiTokens->OpenGL, HgiTokens->Metal, HgiTokens->Vulkan
/// Returns nullptr if the specified backend is not available.
/// Returns an opaque pointer that must be destroyed with Hgi_Swift_DestroyHgi.
/// Thread safety: Not thread safe.
HGI_API
void* Hgi_Swift_CreateNamedHgi(const TfToken& hgiToken);

/// Destroys an Hgi instance created by Hgi_Swift_Create* functions.
/// After this call, the hgi pointer is invalid and must not be used.
/// Thread safety: Not thread safe.
HGI_API
void Hgi_Swift_DestroyHgi(void* hgi);

/// Returns the API name token from an Hgi instance (e.g., "Metal", "OpenGL").
/// The hgi parameter is void* (actually Hgi*).
/// Thread safety: This call is thread safe.
HGI_API
TfToken Hgi_Swift_GetAPIName(void* hgi);

/// Returns whether the backend is supported on the current hardware.
/// The hgi parameter is void* (actually Hgi*).
/// Thread safety: This call is thread safe.
HGI_API
bool Hgi_Swift_IsBackendSupported(void* hgi);

/// @}

/// @{
/// \name HgiGraphicsCmds Swift Bridge Functions
///
/// Bridge functions for creating and managing graphics command buffers.
/// These return opaque void* pointers because HgiGraphicsCmdsUniquePtr
/// cannot be imported by Swift.

/// Creates a graphics command buffer for rendering operations.
/// The hgi parameter is void* (actually Hgi*).
/// Returns an opaque pointer that must be destroyed with Hgi_Swift_DestroyGraphicsCmds.
/// Returns nullptr if creation fails.
/// Thread safety: Creation on main thread, recording on one secondary thread.
HGI_API
void* Hgi_Swift_CreateGraphicsCmds(void* hgi, const HgiGraphicsCmdsDesc& desc);

/// Destroys a graphics command buffer.
/// After this call, the cmds pointer is invalid and must not be used.
/// The cmds parameter is void* (actually HgiGraphicsCmds*).
HGI_API
void Hgi_Swift_DestroyGraphicsCmds(void* cmds);

/// @}

/// @{
/// \name HgiBlitCmds Swift Bridge Functions
///
/// Bridge functions for creating and managing blit (copy) command buffers.

/// Creates a blit command buffer for resource copy operations.
/// The hgi parameter is void* (actually Hgi*).
/// Returns an opaque pointer that must be destroyed with Hgi_Swift_DestroyBlitCmds.
/// Returns nullptr if creation fails.
/// Thread safety: Creation on main thread, recording on one secondary thread.
HGI_API
void* Hgi_Swift_CreateBlitCmds(void* hgi);

/// Destroys a blit command buffer.
/// After this call, the cmds pointer is invalid and must not be used.
/// The cmds parameter is void* (actually HgiBlitCmds*).
HGI_API
void Hgi_Swift_DestroyBlitCmds(void* cmds);

/// @}

/// @{
/// \name HgiComputeCmds Swift Bridge Functions
///
/// Bridge functions for creating and managing compute command buffers.

/// Creates a compute command buffer for compute shader dispatch.
/// The hgi parameter is void* (actually Hgi*).
/// Returns an opaque pointer that must be destroyed with Hgi_Swift_DestroyComputeCmds.
/// Returns nullptr if creation fails.
/// Thread safety: Creation on main thread, recording on one secondary thread.
HGI_API
void* Hgi_Swift_CreateComputeCmds(void* hgi, const HgiComputeCmdsDesc& desc);

/// Destroys a compute command buffer.
/// After this call, the cmds pointer is invalid and must not be used.
/// The cmds parameter is void* (actually HgiComputeCmds*).
HGI_API
void Hgi_Swift_DestroyComputeCmds(void* cmds);

/// @}

/// @{
/// \name Command Submission Swift Bridge Functions
///
/// Bridge functions for submitting command buffers to the GPU.

/// Submits a command buffer to the GPU for execution.
/// The hgi parameter is void* (actually Hgi*).
/// The cmds parameter is void* (actually HgiCmds* - base class of all cmd types).
/// Thread safety: Must be called on main thread.
HGI_API
void Hgi_Swift_SubmitCmds(void* hgi, void* cmds, HgiSubmitWaitType wait);

/// @}

/// @{
/// \name Frame Management Swift Bridge Functions
///
/// Optional frame lifecycle management functions.

/// Called at the start of a new rendering frame.
/// This is optional and used for GPU frame debug markers.
/// The hgi parameter is void* (actually Hgi*).
/// Thread safety: Not thread safe. Should be called on the main thread.
HGI_API
void Hgi_Swift_StartFrame(void* hgi);

/// Called at the end of a rendering frame.
/// This is optional and used for GPU frame debug markers.
/// The hgi parameter is void* (actually Hgi*).
/// Thread safety: Not thread safe. Should be called on the main thread.
HGI_API
void Hgi_Swift_EndFrame(void* hgi);

/// Performs garbage collection of GPU resources.
/// This can be used to flush pending deletes immediately after unloading assets.
/// The hgi parameter is void* (actually Hgi*).
HGI_API
void Hgi_Swift_GarbageCollect(void* hgi);

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_IMAGING_HGI_SWIFT_BRIDGE_H
