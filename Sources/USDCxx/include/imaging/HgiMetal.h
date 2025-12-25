// HgiMetal.h - Metal implementation of Hgi for SwiftUSD
// Mirrors: pxr/imaging/hgiMetal/hgi.h
// C++ header with Swift interop annotations
//
// HgiMetal provides Metal-based GPU rendering for Hydra on Apple platforms.
// This module is only available on macOS, iOS, and visionOS.

#pragma once

#include "../Swift.h"
#include "Hgi.h"

#if SWIFTUSD_PLATFORM_APPLE

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hgiMetal/hgi.h"
#include "pxr/imaging/hgiMetal/capabilities.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - HgiMetal API Version Enum
// ============================================================================

/// Metal API version levels.
enum class HgiMetalAPIVersion : int32_t {
    Metal1_0 = 0,
    Metal2_0 = 1,
    Metal3_0 = 2
};

// ============================================================================
// MARK: - HgiMetalCommitWaitType
// ============================================================================

/// Wait types for Metal command buffer commits.
enum class HgiMetalCommitWaitType : int32_t {
    NoWait = 0,
    WaitUntilScheduled = 1,
    WaitUntilCompleted = 2
};

// ============================================================================
// MARK: - Forward declarations
// ============================================================================

class HgiMetalRef;
class HgiMetalCapabilitiesRef;

// ============================================================================
// MARK: - HgiMetalCapabilitiesRef
// ============================================================================

/// Reports the capabilities of the Metal device.
///
/// HgiMetalCapabilitiesRef extends HgiCapabilities with Metal-specific
/// device information and feature detection.
class HgiMetalCapabilitiesRef {
public:
    /// Get the Metal API version.
    HgiMetalAPIVersion GetMetalAPIVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_caps) {
            int version = _caps->GetAPIVersion();
            if (version >= 2) return HgiMetalAPIVersion::Metal3_0;
            if (version >= 1) return HgiMetalAPIVersion::Metal2_0;
            return HgiMetalAPIVersion::Metal1_0;
        }
        return HgiMetalAPIVersion::Metal1_0;
#else
        return _apiVersion;
#endif
    }

    /// Get the API version as an integer.
    int GetAPIVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetAPIVersion() : 0;
#else
        return static_cast<int>(_apiVersion);
#endif
    }

    /// Get the shader language version (MSL version).
    int GetShaderVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetShaderVersion() : 0;
#else
        return _shaderVersion;
#endif
    }

    /// Get maximum uniform block size.
    size_t GetMaxUniformBlockSize() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetMaxUniformBlockSize() : 65536;
#else
        return _maxUniformBlockSize;
#endif
    }

    /// Get maximum shader storage block size.
    size_t GetMaxShaderStorageBlockSize() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetMaxShaderStorageBlockSize() : 268435456;
#else
        return _maxShaderStorageBlockSize;
#endif
    }

    /// Get uniform buffer offset alignment requirement.
    size_t GetUniformBufferOffsetAlignment() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetUniformBufferOffsetAlignment() : 256;
#else
        return _uniformBufferOffsetAlignment;
#endif
    }

    /// Check if the device supports vertex memory barriers.
    bool HasVertexMemoryBarrier() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->hasVertexMemoryBarrier : true;
#else
        return _hasVertexMemoryBarrier;
#endif
    }

    /// Check if parallel encoder is supported.
    bool UseParallelEncoder() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->useParallelEncoder : true;
#else
        return _useParallelEncoder;
#endif
    }

    /// Check if a capability flag is set.
    bool IsSet(HgiDeviceCapabilities mask) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->IsSet(static_cast<PXR_NS::HgiDeviceCapabilities>(mask)) : false;
#else
        return (_flags & mask) != 0;
#endif
    }

    // Reference counting for SWIFT_SHARED_REFERENCE
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class HgiMetalRef;

#if defined(USE_PIXAR_USD)
    explicit HgiMetalCapabilitiesRef(const PXR_NS::HgiMetalCapabilities* caps) SWIFTUSD_NOEXCEPT
        : _caps(caps), _refCount(1) {}

    const PXR_NS::HgiMetalCapabilities* _caps;
#else
    HgiMetalCapabilitiesRef() SWIFTUSD_NOEXCEPT
        : _apiVersion(HgiMetalAPIVersion::Metal2_0),
          _shaderVersion(230),
          _maxUniformBlockSize(65536),
          _maxShaderStorageBlockSize(268435456),
          _uniformBufferOffsetAlignment(256),
          _hasVertexMemoryBarrier(true),
          _useParallelEncoder(true),
          _flags(HgiDeviceCapabilitiesPresentation |
                 HgiDeviceCapabilitiesUnifiedMemory |
                 HgiDeviceCapabilitiesMetalTessellation),
          _refCount(1) {}

    HgiMetalAPIVersion _apiVersion;
    int _shaderVersion;
    size_t _maxUniformBlockSize;
    size_t _maxShaderStorageBlockSize;
    size_t _uniformBufferOffsetAlignment;
    bool _hasVertexMemoryBarrier;
    bool _useParallelEncoder;
    HgiDeviceCapabilities _flags;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HgiMetalRef (Metal GPU interface)
// ============================================================================

/// Metal implementation of the Hydra Graphics Interface.
///
/// HgiMetalRef provides a Metal-based GPU interface for Hydra rendering
/// on Apple platforms. It extends the base Hgi interface with Metal-specific
/// functionality such as access to the MTLDevice and command buffers.
///
/// Usage:
/// ```swift
/// #if os(macOS) || os(iOS) || os(visionOS)
/// // Create Metal Hgi (uses system default device)
/// let hgi = HgiMetalRef.Create()
///
/// // Get Metal-specific capabilities
/// let caps = hgi?.GetMetalCapabilities()
/// print("Metal API version: \(caps?.GetMetalAPIVersion())")
///
/// // Frame rendering
/// hgi?.StartFrame()
/// // ... record commands ...
/// hgi?.EndFrame()
/// #endif
/// ```
class HgiMetalRef {
public:
    // ========================================================================
    // MARK: - Factory Methods
    // ========================================================================

    /// Create a Metal-backed Hgi instance.
    /// Uses the system default Metal device.
    static HgiMetalRef* Create() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto hgi = std::make_unique<PXR_NS::HgiMetal>();
        if (!hgi) return nullptr;
        return new HgiMetalRef(std::move(hgi));
#else
        return new HgiMetalRef();
#endif
    }

    /// Check if Metal is supported on this hardware.
    static bool IsSupported() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Metal requires a valid MTLDevice
        return true; // On Apple platforms with USE_PIXAR_USD, Metal should work
#else
        return true; // Standalone always "supports" its mock backend
#endif
    }

    /// Check if the backend is supported on current hardware.
    bool IsBackendSupported() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _hgi ? _hgi->IsBackendSupported() : false;
#else
        return true;
#endif
    }

    // ========================================================================
    // MARK: - API Information
    // ========================================================================

    /// Get the name of the graphics API (always "Metal").
    Token GetAPIName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _hgi ? Token(_hgi->GetAPIName()) : Token("Metal");
#else
        return Token("Metal");
#endif
    }

    /// Get Metal-specific device capabilities.
    HgiMetalCapabilitiesRef* GetMetalCapabilities() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_hgi) return nullptr;
        return new HgiMetalCapabilitiesRef(_hgi->GetCapabilities());
#else
        return new HgiMetalCapabilitiesRef();
#endif
    }

    /// Get the Metal API version.
    int GetAPIVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _hgi ? _hgi->GetAPIVersion() : 0;
#else
        return 2; // Simulate Metal 2.0
#endif
    }

    // ========================================================================
    // MARK: - Frame Management
    // ========================================================================

    /// Mark the start of a new rendering frame.
    void StartFrame() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) _hgi->StartFrame();
#else
        _frameDepth++;
#endif
    }

    /// Mark the end of the current rendering frame.
    void EndFrame() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) _hgi->EndFrame();
#else
        if (_frameDepth > 0) _frameDepth--;
#endif
    }

    /// Perform garbage collection of pending resource deletes.
    void GarbageCollect() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) _hgi->GarbageCollect();
#endif
    }

    // ========================================================================
    // MARK: - Buffer Management
    // ========================================================================

    /// Create a GPU buffer.
    HgiBufferRef* CreateBuffer(const HgiBufferDesc& desc) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_hgi) return nullptr;

        PXR_NS::HgiBufferDesc pxrDesc;
        pxrDesc.debugName = desc.debugName;
        pxrDesc.byteSize = desc.byteSize;
        pxrDesc.usage = static_cast<PXR_NS::HgiBufferUsage>(desc.usage);
        pxrDesc.initialData = desc.initialData;

        auto handle = _hgi->CreateBuffer(pxrDesc);
        if (!handle) return nullptr;
        return new HgiBufferRef(handle);
#else
        return new HgiBufferRef(desc);
#endif
    }

    /// Destroy a GPU buffer.
    void DestroyBuffer(HgiBufferRef* buffer) SWIFTUSD_NOEXCEPT {
        if (!buffer) return;
#if defined(USE_PIXAR_USD)
        if (_hgi && buffer->_handle) {
            _hgi->DestroyBuffer(&buffer->_handle);
        }
#else
        buffer->_valid = false;
#endif
    }

    // ========================================================================
    // MARK: - Texture Management
    // ========================================================================

    /// Create a GPU texture.
    HgiTextureRef* CreateTexture(const HgiTextureDesc& desc) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_hgi) return nullptr;

        PXR_NS::HgiTextureDesc pxrDesc;
        pxrDesc.debugName = desc.debugName;
        pxrDesc.format = static_cast<PXR_NS::HgiFormat>(desc.format);
        pxrDesc.type = static_cast<PXR_NS::HgiTextureType>(desc.type);
        pxrDesc.dimensions = PXR_NS::GfVec3i(desc.width, desc.height, desc.depth);
        pxrDesc.mipLevels = desc.mipLevels;
        pxrDesc.layerCount = desc.layerCount;
        pxrDesc.sampleCount = static_cast<PXR_NS::HgiSampleCount>(desc.sampleCount);
        pxrDesc.usage = static_cast<PXR_NS::HgiTextureUsage>(desc.usage);
        pxrDesc.initialData = desc.initialData;

        auto handle = _hgi->CreateTexture(pxrDesc);
        if (!handle) return nullptr;
        return new HgiTextureRef(handle);
#else
        return new HgiTextureRef(desc);
#endif
    }

    /// Destroy a GPU texture.
    void DestroyTexture(HgiTextureRef* texture) SWIFTUSD_NOEXCEPT {
        if (!texture) return;
#if defined(USE_PIXAR_USD)
        if (_hgi && texture->_handle) {
            _hgi->DestroyTexture(&texture->_handle);
        }
#else
        texture->_valid = false;
#endif
    }

    // ========================================================================
    // MARK: - Command Buffer Management (Metal-specific)
    // ========================================================================

    /// Signal that there is work to be submitted.
    void SetHasWork() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) _hgi->SetHasWork();
#else
        _hasWork = true;
#endif
    }

    /// Commit the primary command buffer.
    ///
    /// - Parameters:
    ///   - waitType: How to wait for the command buffer.
    ///   - forceNewBuffer: Whether to force creation of a new buffer.
    void CommitPrimaryCommandBuffer(
        HgiMetalCommitWaitType waitType = HgiMetalCommitWaitType::NoWait,
        bool forceNewBuffer = false) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) {
            _hgi->CommitPrimaryCommandBuffer(
                static_cast<PXR_NS::HgiMetal::CommitCommandBufferWaitType>(waitType),
                forceNewBuffer);
        }
#else
        (void)waitType;
        (void)forceNewBuffer;
        _hasWork = false;
#endif
    }

    // ========================================================================
    // MARK: - Reference Counting
    // ========================================================================

    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    explicit HgiMetalRef(std::unique_ptr<PXR_NS::HgiMetal> hgi) SWIFTUSD_NOEXCEPT
        : _hgi(std::move(hgi)), _refCount(1) {}

    std::unique_ptr<PXR_NS::HgiMetal> _hgi;
#else
    HgiMetalRef() SWIFTUSD_NOEXCEPT
        : _frameDepth(0), _hasWork(false), _refCount(1) {}

    int _frameDepth;
    bool _hasWork;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd

#endif // SWIFTUSD_PLATFORM_APPLE
