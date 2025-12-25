// Hgi.h - Hydra Graphics Interface for SwiftUSD
// Mirrors: pxr/imaging/hgi/hgi.h, pxr/imaging/hgi/types.h
// C++ header with Swift interop annotations
//
// Hgi provides a platform-agnostic GPU abstraction layer for USD rendering.
// On macOS/iOS it uses Metal, on other platforms Vulkan or OpenGL.

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hgi/hgi.h"
#include "pxr/imaging/hgi/types.h"
#include "pxr/imaging/hgi/enums.h"
#include "pxr/imaging/hgi/buffer.h"
#include "pxr/imaging/hgi/texture.h"
#include "pxr/imaging/hgi/capabilities.h"

#if SWIFTUSD_PLATFORM_APPLE
#include "pxr/imaging/hgiMetal/hgi.h"
#endif

#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - HgiFormat (Pixel/Buffer format enumeration)
// ============================================================================

/// Describes the memory format of image buffers.
///
/// These formats are aligned with HgiFormat from Pixar's USD.
enum class HgiFormat : int32_t {
    Invalid = -1,

    // UNorm8 - a 1-byte value representing a float between 0 and 1
    UNorm8 = 0,
    UNorm8Vec2,
    UNorm8Vec4,

    // SNorm8 - a 1-byte value representing a float between -1 and 1
    SNorm8,
    SNorm8Vec2,
    SNorm8Vec4,

    // Float16 - a 2-byte IEEE half-precision float
    Float16,
    Float16Vec2,
    Float16Vec3,
    Float16Vec4,

    // Float32 - a 4-byte IEEE float
    Float32,
    Float32Vec2,
    Float32Vec3,
    Float32Vec4,

    // Int16 - a 2-byte signed integer
    Int16,
    Int16Vec2,
    Int16Vec3,
    Int16Vec4,

    // UInt16 - a 2-byte unsigned integer
    UInt16,
    UInt16Vec2,
    UInt16Vec3,
    UInt16Vec4,

    // Int32 - a 4-byte signed integer
    Int32,
    Int32Vec2,
    Int32Vec3,
    Int32Vec4,

    // sRGB formats
    UNorm8Vec4srgb,

    // Compressed formats
    BC6FloatVec3,
    BC6UFloatVec3,
    BC7UNorm8Vec4,
    BC7UNorm8Vec4srgb,
    BC1UNorm8Vec4,
    BC3UNorm8Vec4,

    // Depth stencil
    Float32UInt8,

    // Packed format
    PackedInt1010102,

    Count
};

// ============================================================================
// MARK: - HgiTextureType
// ============================================================================

/// Types of textures supported by Hgi.
enum class HgiTextureType : int32_t {
    Texture1D = 0,
    Texture2D,
    Texture3D,
    Texturecube,
    Texture1DArray,
    Texture2DArray,
    Count
};

// ============================================================================
// MARK: - HgiTextureUsage (bit flags)
// ============================================================================

/// Usage flags for textures.
using HgiTextureUsage = uint32_t;

constexpr HgiTextureUsage HgiTextureUsageNone = 0;
constexpr HgiTextureUsage HgiTextureUsageColorTarget = 1 << 0;
constexpr HgiTextureUsage HgiTextureUsageDepthTarget = 1 << 1;
constexpr HgiTextureUsage HgiTextureUsageStencilTarget = 1 << 2;
constexpr HgiTextureUsage HgiTextureUsageShaderRead = 1 << 3;
constexpr HgiTextureUsage HgiTextureUsageShaderWrite = 1 << 4;

// ============================================================================
// MARK: - HgiBufferUsage (bit flags)
// ============================================================================

/// Usage flags for buffers.
using HgiBufferUsage = uint32_t;

constexpr HgiBufferUsage HgiBufferUsageNone = 0;
constexpr HgiBufferUsage HgiBufferUsageUniform = 1 << 0;
constexpr HgiBufferUsage HgiBufferUsageIndex32 = 1 << 1;
constexpr HgiBufferUsage HgiBufferUsageVertex = 1 << 2;
constexpr HgiBufferUsage HgiBufferUsageStorage = 1 << 3;
constexpr HgiBufferUsage HgiBufferUsageIndirect = 1 << 4;

// ============================================================================
// MARK: - HgiDeviceCapabilities (bit flags)
// ============================================================================

/// Device capability flags.
using HgiDeviceCapabilities = uint32_t;

constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesNone = 0;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesPresentation = 1 << 0;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesBindlessBuffers = 1 << 1;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesConcurrentDispatch = 1 << 2;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesUnifiedMemory = 1 << 3;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesBuiltinBarycentrics = 1 << 4;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesShaderDoublePrecision = 1 << 5;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesDepthRangeMinusOneToOne = 1 << 6;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesMetalTessellation = 1 << 7;
constexpr HgiDeviceCapabilities HgiDeviceCapabilitiesMultiDrawIndirect = 1 << 8;

// ============================================================================
// MARK: - HgiSubmitWaitType
// ============================================================================

/// Wait types for command submission.
enum class HgiSubmitWaitType : int32_t {
    NoWait = 0,
    WaitUntilCompleted
};

// ============================================================================
// MARK: - Forward declarations
// ============================================================================

class HgiRef;
class HgiCapabilitiesRef;
class HgiBufferRef;
class HgiTextureRef;

// Forward declaration for HgiMetal (Apple platforms only)
#if SWIFTUSD_PLATFORM_APPLE
class HgiMetalRef;
#endif

// ============================================================================
// MARK: - HgiCapabilitiesRef (device capabilities)
// ============================================================================

/// Reports the capabilities of the Hgi device.
///
/// This is a reference-counted wrapper that provides access to device
/// capabilities like API version, shader version, and supported features.
class HgiCapabilitiesRef {
public:
    /// Check if a capability flag is set.
    bool IsSet(HgiDeviceCapabilities mask) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->IsSet(static_cast<PXR_NS::HgiDeviceCapabilities>(mask)) : false;
#else
        return (_flags & mask) != 0;
#endif
    }

    /// Get the API version.
    int GetAPIVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetAPIVersion() : 0;
#else
        return _apiVersion;
#endif
    }

    /// Get the shader language version.
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
        return _caps ? _caps->GetMaxShaderStorageBlockSize() : 134217728;
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

    /// Get maximum clip distances.
    size_t GetMaxClipDistances() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _caps ? _caps->GetMaxClipDistances() : 8;
#else
        return _maxClipDistances;
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
    friend class HgiRef;

#if defined(USE_PIXAR_USD)
    explicit HgiCapabilitiesRef(const PXR_NS::HgiCapabilities* caps) SWIFTUSD_NOEXCEPT
        : _caps(caps), _refCount(1) {}

    const PXR_NS::HgiCapabilities* _caps;
#else
    HgiCapabilitiesRef() SWIFTUSD_NOEXCEPT
        : _flags(HgiDeviceCapabilitiesPresentation),
          _apiVersion(1),
          _shaderVersion(150),
          _maxUniformBlockSize(65536),
          _maxShaderStorageBlockSize(134217728),
          _uniformBufferOffsetAlignment(256),
          _maxClipDistances(8),
          _refCount(1) {}

    HgiDeviceCapabilities _flags;
    int _apiVersion;
    int _shaderVersion;
    size_t _maxUniformBlockSize;
    size_t _maxShaderStorageBlockSize;
    size_t _uniformBufferOffsetAlignment;
    size_t _maxClipDistances;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HgiBufferDesc (buffer description)
// ============================================================================

/// Describes a GPU buffer to be created.
struct HgiBufferDesc {
    /// Debug label for the buffer.
    std::string debugName;

    /// Size of the buffer in bytes.
    size_t byteSize;

    /// Buffer usage flags.
    HgiBufferUsage usage;

    /// Initial data to upload (optional, can be nullptr).
    const void* initialData;

    HgiBufferDesc() SWIFTUSD_NOEXCEPT
        : byteSize(0),
          usage(HgiBufferUsageNone),
          initialData(nullptr) {}

    HgiBufferDesc(const char* name, size_t size, HgiBufferUsage usageFlags) SWIFTUSD_NOEXCEPT
        : debugName(name ? name : ""),
          byteSize(size),
          usage(usageFlags),
          initialData(nullptr) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HgiTextureDesc (texture description)
// ============================================================================

/// Describes a GPU texture to be created.
struct HgiTextureDesc {
    /// Debug label for the texture.
    std::string debugName;

    /// Pixel format.
    HgiFormat format;

    /// Texture type (1D, 2D, 3D, cube, array).
    HgiTextureType type;

    /// Dimensions (width, height, depth).
    int width;
    int height;
    int depth;

    /// Number of mip levels.
    uint16_t mipLevels;

    /// Number of array layers.
    uint16_t layerCount;

    /// Sample count for multisampling.
    uint8_t sampleCount;

    /// Usage flags.
    HgiTextureUsage usage;

    /// Initial data (optional).
    const void* initialData;

    HgiTextureDesc() SWIFTUSD_NOEXCEPT
        : format(HgiFormat::UNorm8Vec4),
          type(HgiTextureType::Texture2D),
          width(1),
          height(1),
          depth(1),
          mipLevels(1),
          layerCount(1),
          sampleCount(1),
          usage(HgiTextureUsageShaderRead),
          initialData(nullptr) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HgiBufferRef (GPU buffer handle)
// ============================================================================

/// A reference to a GPU buffer resource.
///
/// HgiBufferRef wraps a GPU buffer that can be used for vertex data,
/// index data, uniform data, or storage buffers.
class HgiBufferRef {
public:
    /// Get the size of the buffer in bytes.
    size_t GetByteSize() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? _handle->GetDescriptor().byteSize : _desc.byteSize;
#else
        return _desc.byteSize;
#endif
    }

    /// Get the usage flags.
    HgiBufferUsage GetUsage() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? static_cast<HgiBufferUsage>(_handle->GetDescriptor().usage) : _desc.usage;
#else
        return _desc.usage;
#endif
    }

    /// Get the debug name.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetDebugName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_handle) {
            _debugNameCache = _handle->GetDescriptor().debugName;
            return _debugNameCache.c_str();
        }
        return _desc.debugName.c_str();
#else
        return _desc.debugName.c_str();
#endif
    }

    /// Check if the buffer is valid.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle.Get() != nullptr;
#else
        return _valid;
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
    friend class HgiRef;
#if SWIFTUSD_PLATFORM_APPLE
    friend class HgiMetalRef;
#endif

#if defined(USE_PIXAR_USD)
    explicit HgiBufferRef(PXR_NS::HgiBufferHandle handle) SWIFTUSD_NOEXCEPT
        : _handle(handle), _refCount(1) {}

    PXR_NS::HgiBufferHandle _handle;
    mutable std::string _debugNameCache;
#else
    explicit HgiBufferRef(const HgiBufferDesc& desc) SWIFTUSD_NOEXCEPT
        : _desc(desc), _valid(true), _refCount(1) {}
#endif

    HgiBufferDesc _desc;
    bool _valid{false};
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HgiTextureRef (GPU texture handle)
// ============================================================================

/// A reference to a GPU texture resource.
///
/// HgiTextureRef wraps a GPU texture that can be used for sampling,
/// render targets, or storage images.
class HgiTextureRef {
public:
    /// Get the format of the texture.
    HgiFormat GetFormat() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? static_cast<HgiFormat>(_handle->GetDescriptor().format) : _desc.format;
#else
        return _desc.format;
#endif
    }

    /// Get the texture type.
    HgiTextureType GetType() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? static_cast<HgiTextureType>(_handle->GetDescriptor().type) : _desc.type;
#else
        return _desc.type;
#endif
    }

    /// Get the width of the texture.
    int GetWidth() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? _handle->GetDescriptor().dimensions[0] : _desc.width;
#else
        return _desc.width;
#endif
    }

    /// Get the height of the texture.
    int GetHeight() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? _handle->GetDescriptor().dimensions[1] : _desc.height;
#else
        return _desc.height;
#endif
    }

    /// Get the depth of the texture.
    int GetDepth() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? _handle->GetDescriptor().dimensions[2] : _desc.depth;
#else
        return _desc.depth;
#endif
    }

    /// Get the number of mip levels.
    uint16_t GetMipLevels() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? _handle->GetDescriptor().mipLevels : _desc.mipLevels;
#else
        return _desc.mipLevels;
#endif
    }

    /// Get the usage flags.
    HgiTextureUsage GetUsage() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle ? static_cast<HgiTextureUsage>(_handle->GetDescriptor().usage) : _desc.usage;
#else
        return _desc.usage;
#endif
    }

    /// Get the debug name.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetDebugName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_handle) {
            _debugNameCache = _handle->GetDescriptor().debugName;
            return _debugNameCache.c_str();
        }
        return _desc.debugName.c_str();
#else
        return _desc.debugName.c_str();
#endif
    }

    /// Check if the texture is valid.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _handle.Get() != nullptr;
#else
        return _valid;
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
    friend class HgiRef;
#if SWIFTUSD_PLATFORM_APPLE
    friend class HgiMetalRef;
#endif

#if defined(USE_PIXAR_USD)
    explicit HgiTextureRef(PXR_NS::HgiTextureHandle handle) SWIFTUSD_NOEXCEPT
        : _handle(handle), _refCount(1) {}

    PXR_NS::HgiTextureHandle _handle;
    mutable std::string _debugNameCache;
#else
    explicit HgiTextureRef(const HgiTextureDesc& desc) SWIFTUSD_NOEXCEPT
        : _desc(desc), _valid(true), _refCount(1) {}
#endif

    HgiTextureDesc _desc;
    bool _valid{false};
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HgiRef (Main GPU interface)
// ============================================================================

/// Platform-agnostic GPU interface for Hydra rendering.
///
/// HgiRef provides the main interface for creating GPU resources and
/// submitting rendering commands. On Apple platforms it uses Metal,
/// on other platforms Vulkan or OpenGL.
///
/// Usage:
/// ```swift
/// // Create platform-default Hgi
/// let hgi = HgiRef.CreatePlatformDefault()
///
/// // Create a buffer
/// var desc = HgiBufferDesc("myBuffer", 1024, HgiBufferUsageVertex)
/// let buffer = hgi?.CreateBuffer(desc)
///
/// // Frame rendering
/// hgi?.StartFrame()
/// // ... record commands ...
/// hgi?.EndFrame()
/// ```
class HgiRef {
public:
    // ========================================================================
    // MARK: - Factory Methods
    // ========================================================================

    /// Create a platform-appropriate Hgi instance.
    /// On macOS/iOS returns Metal-backed Hgi, on other platforms Vulkan/OpenGL.
    static HgiRef* CreatePlatformDefault() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto hgi = PXR_NS::Hgi::CreatePlatformDefaultHgi();
        if (!hgi) return nullptr;
        return new HgiRef(std::move(hgi));
#else
        return new HgiRef();
#endif
    }

#if SWIFTUSD_PLATFORM_APPLE
    /// Create Metal-backed Hgi with a specific MTLDevice.
    /// Pass nullptr to use the default system device.
    static HgiRef* CreateMetal(void* mtlDevice = nullptr) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto hgi = std::make_unique<PXR_NS::HgiMetal>(
            static_cast<id<MTLDevice>>(mtlDevice));
        if (!hgi) return nullptr;
        return new HgiRef(std::move(hgi));
#else
        return new HgiRef();
#endif
    }
#endif

    /// Check if the backend is supported on current hardware.
    bool IsBackendSupported() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _hgi ? _hgi->IsBackendSupported() : false;
#else
        return true;  // Standalone always "supports" its mock backend
#endif
    }

    /// Static check if a specific backend is supported.
    static bool IsSupported(const Token& backendName = Token()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::Hgi::IsSupported(backendName.GetTfToken());
#else
        (void)backendName;
        return true;
#endif
    }

    // ========================================================================
    // MARK: - API Information
    // ========================================================================

    /// Get the name of the graphics API (e.g., "Metal", "OpenGL", "Vulkan").
    Token GetAPIName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _hgi ? Token(_hgi->GetAPIName()) : Token("Unknown");
#else
#if SWIFTUSD_PLATFORM_APPLE
        return Token("Metal");
#elif SWIFTUSD_HGI_VULKAN
        return Token("Vulkan");
#else
        return Token("OpenGL");
#endif
#endif
    }

    /// Get device capabilities.
    HgiCapabilitiesRef* GetCapabilities() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_hgi) return nullptr;
        return new HgiCapabilitiesRef(_hgi->GetCapabilities());
#else
        return new HgiCapabilitiesRef();
#endif
    }

    // ========================================================================
    // MARK: - Frame Management
    // ========================================================================

    /// Mark the start of a new rendering frame.
    /// This is optional but can be helpful for GPU debugging tools.
    void StartFrame() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) _hgi->StartFrame();
#else
        _frameActive = true;
#endif
    }

    /// Mark the end of the current rendering frame.
    void EndFrame() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_hgi) _hgi->EndFrame();
#else
        _frameActive = false;
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
    explicit HgiRef(PXR_NS::HgiUniquePtr hgi) SWIFTUSD_NOEXCEPT
        : _hgi(std::move(hgi)), _refCount(1) {}

    PXR_NS::HgiUniquePtr _hgi;
#else
    HgiRef() SWIFTUSD_NOEXCEPT
        : _frameActive(false), _refCount(1) {}

    bool _frameActive;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Get the number of components in a format.
inline size_t HgiGetComponentCount(HgiFormat format) SWIFTUSD_NOEXCEPT {
    switch (format) {
        case HgiFormat::UNorm8:
        case HgiFormat::SNorm8:
        case HgiFormat::Float16:
        case HgiFormat::Float32:
        case HgiFormat::Int16:
        case HgiFormat::UInt16:
        case HgiFormat::Int32:
            return 1;
        case HgiFormat::UNorm8Vec2:
        case HgiFormat::SNorm8Vec2:
        case HgiFormat::Float16Vec2:
        case HgiFormat::Float32Vec2:
        case HgiFormat::Int16Vec2:
        case HgiFormat::UInt16Vec2:
        case HgiFormat::Int32Vec2:
            return 2;
        case HgiFormat::Float16Vec3:
        case HgiFormat::Float32Vec3:
        case HgiFormat::Int16Vec3:
        case HgiFormat::UInt16Vec3:
        case HgiFormat::Int32Vec3:
        case HgiFormat::BC6FloatVec3:
        case HgiFormat::BC6UFloatVec3:
            return 3;
        case HgiFormat::UNorm8Vec4:
        case HgiFormat::SNorm8Vec4:
        case HgiFormat::Float16Vec4:
        case HgiFormat::Float32Vec4:
        case HgiFormat::Int16Vec4:
        case HgiFormat::UInt16Vec4:
        case HgiFormat::Int32Vec4:
        case HgiFormat::UNorm8Vec4srgb:
        case HgiFormat::BC7UNorm8Vec4:
        case HgiFormat::BC7UNorm8Vec4srgb:
        case HgiFormat::BC1UNorm8Vec4:
        case HgiFormat::BC3UNorm8Vec4:
        case HgiFormat::PackedInt1010102:
            return 4;
        default:
            return 0;
    }
}

/// Get the byte size of a single element for a format.
inline size_t HgiGetDataSizeOfFormat(HgiFormat format) SWIFTUSD_NOEXCEPT {
    switch (format) {
        case HgiFormat::UNorm8:
        case HgiFormat::SNorm8:
            return 1;
        case HgiFormat::UNorm8Vec2:
        case HgiFormat::SNorm8Vec2:
        case HgiFormat::Float16:
        case HgiFormat::Int16:
        case HgiFormat::UInt16:
            return 2;
        case HgiFormat::Float16Vec2:
        case HgiFormat::Int16Vec2:
        case HgiFormat::UInt16Vec2:
        case HgiFormat::Float32:
        case HgiFormat::Int32:
        case HgiFormat::UNorm8Vec4:
        case HgiFormat::SNorm8Vec4:
        case HgiFormat::UNorm8Vec4srgb:
        case HgiFormat::PackedInt1010102:
            return 4;
        case HgiFormat::Float16Vec3:
        case HgiFormat::Int16Vec3:
        case HgiFormat::UInt16Vec3:
            return 6;
        case HgiFormat::Float16Vec4:
        case HgiFormat::Int16Vec4:
        case HgiFormat::UInt16Vec4:
        case HgiFormat::Float32Vec2:
        case HgiFormat::Int32Vec2:
        case HgiFormat::Float32UInt8:
            return 8;
        case HgiFormat::Float32Vec3:
        case HgiFormat::Int32Vec3:
            return 12;
        case HgiFormat::Float32Vec4:
        case HgiFormat::Int32Vec4:
            return 16;
        // Compressed formats return block size
        case HgiFormat::BC6FloatVec3:
        case HgiFormat::BC6UFloatVec3:
        case HgiFormat::BC7UNorm8Vec4:
        case HgiFormat::BC7UNorm8Vec4srgb:
            return 16;  // 16 bytes per 4x4 block
        case HgiFormat::BC1UNorm8Vec4:
            return 8;   // 8 bytes per 4x4 block
        case HgiFormat::BC3UNorm8Vec4:
            return 16;  // 16 bytes per 4x4 block
        default:
            return 0;
    }
}

/// Check if a format uses compression.
inline bool HgiIsCompressed(HgiFormat format) SWIFTUSD_NOEXCEPT {
    switch (format) {
        case HgiFormat::BC6FloatVec3:
        case HgiFormat::BC6UFloatVec3:
        case HgiFormat::BC7UNorm8Vec4:
        case HgiFormat::BC7UNorm8Vec4srgb:
        case HgiFormat::BC1UNorm8Vec4:
        case HgiFormat::BC3UNorm8Vec4:
            return true;
        default:
            return false;
    }
}

} // namespace swiftusd
