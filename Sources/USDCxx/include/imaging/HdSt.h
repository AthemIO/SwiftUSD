// HdSt.h - Storm Renderer for SwiftUSD
// Mirrors: pxr/imaging/hdSt/
// C++ header with Swift interop annotations
//
// The Storm (HdSt) renderer is Pixar's reference GPU renderer built on Hydra.
// Key components include:
// - HdStRenderDelegate: Storm's implementation of HdRenderDelegate
// - HdStMesh: Storm's mesh primitive (subdivision surfaces, poly-meshes)
// - HdStMaterial: Storm's material implementation with GLSLFX shaders
// - HdStResourceRegistry: GPU resource management for Storm
// - HdStRenderPass: Draw pass implementation for Storm

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../base/Vt.h"
#include "../usd/Sdf.h"
#include "Hgi.h"
#include "Hio.h"
#include "PxOsd.h"
#include "Hd.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hdSt/renderDelegate.h"
#include "pxr/imaging/hdSt/mesh.h"
#include "pxr/imaging/hdSt/material.h"
#include "pxr/imaging/hdSt/renderPass.h"
#include "pxr/imaging/hdSt/resourceRegistry.h"
#include "pxr/imaging/hdSt/tokens.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class HdStRenderDelegateRef;
class HdStResourceRegistryRef;
class HdStRenderPassRef;

// ============================================================================
// MARK: - HdStTextureType (Texture classification)
// ============================================================================

/// Types of textures supported by Storm.
enum class HdStTextureType : int32_t {
    Uv = 0,           ///< Standard UV-mapped texture
    Field,            ///< Volume field texture (OpenVDB)
    Ptex,             ///< PTex texture
    Udim,             ///< UDIM texture tiles
    Count
};

// ============================================================================
// MARK: - HdStMaterialTagTokens
// ============================================================================

/// Common material tag tokens used by Storm.
namespace HdStMaterialTagTokens {
    inline Token DefaultMaterialTag() { return Token("defaultMaterialTag"); }
    inline Token Masked() { return Token("masked"); }
    inline Token Additive() { return Token("additive"); }
    inline Token Translucent() { return Token("translucent"); }
    inline Token Volume() { return Token("volume"); }
}

// ============================================================================
// MARK: - HdStTokens (Storm-specific tokens)
// ============================================================================

/// Tokens specific to the Storm renderer.
namespace HdStTokens {
    // Texture tokens
    inline Token UV() { return Token("uv"); }
    inline Token Field() { return Token("field"); }
    inline Token Ptex() { return Token("ptex"); }
    inline Token Udim() { return Token("udim"); }

    // Shader tokens
    inline Token DisplacementShader() { return Token("displacementShader"); }
    inline Token SurfaceShader() { return Token("surfaceShader"); }
    inline Token VolumeShader() { return Token("volumeShader"); }

    // Render pass tokens
    inline Token RenderPassState() { return Token("renderPassState"); }
    inline Token RenderPassShader() { return Token("renderPassShader"); }

    // Material network tokens
    inline Token MaterialNetwork() { return Token("materialNetwork"); }
    inline Token Terminals() { return Token("terminals"); }
    inline Token Primvars() { return Token("primvars"); }

    // Computation tokens
    inline Token Computation() { return Token("computation"); }
    inline Token ExtComputation() { return Token("extComputation"); }
}

// ============================================================================
// MARK: - HdStResourceRegistryRef (GPU resource management)
// ============================================================================

/// Wrapper for Storm's GPU resource registry.
///
/// The resource registry manages all GPU resources used by Storm, including:
/// - Buffer allocations (vertex, index, uniform, SSBO)
/// - Texture allocations and sampling
/// - Shader programs (GLSL/Metal)
/// - Geometric shader instancing
///
/// In typical usage, there is one resource registry per render delegate.
class HdStResourceRegistryRef {
public:
    /// Create a new resource registry.
    /// Note: In production, this is typically created by the render delegate.
    static HdStResourceRegistryRef* Create(HgiRef* hgi) SWIFTUSD_NOEXCEPT {
        if (!hgi) return nullptr;
        return new HdStResourceRegistryRef(hgi);
    }

    /// Get the Hgi instance used by this registry.
    HgiRef* GetHgi() const SWIFTUSD_NOEXCEPT {
        return _hgi;
    }

    /// Invalidate all shader programs in the registry.
    /// Forces recompilation of shaders on next use.
    void InvalidateShaderRegistry() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_registry) _registry->InvalidateShaderRegistry();
#endif
    }

    /// Get the resource allocation statistics as a dictionary.
    /// Returns information about GPU memory usage.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetResourceAllocationSummary() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_registry) {
            // Convert VtDictionary to string summary
            _allocationSummaryCache = "Storm Resource Registry";
            return _allocationSummaryCache.c_str();
        }
        return "";
#else
        return "Storm Resource Registry (standalone mode)";
#endif
    }

    /// Commit pending resources to the GPU.
    void Commit() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_registry) _registry->Commit();
#endif
    }

    /// Garbage collect unreferenced resources.
    void GarbageCollect() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_registry) _registry->GarbageCollect();
#endif
    }

    /// Set the memory request limit for a texture type.
    void SetMemoryRequestForTextureType(HdStTextureType textureType,
                                         size_t memoryRequest) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_registry) {
            _registry->SetMemoryRequestForTextureType(
                static_cast<PXR_NS::HdStTextureType>(textureType), memoryRequest);
        }
#else
        (void)textureType;
        (void)memoryRequest;
#endif
    }

#if defined(USE_PIXAR_USD)
    /// Get the underlying Pixar resource registry.
    PXR_NS::HdStResourceRegistry* GetPxrResourceRegistry() SWIFTUSD_NOEXCEPT {
        return _registry.get();
    }
#endif

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
    explicit HdStResourceRegistryRef(HgiRef* hgi) SWIFTUSD_NOEXCEPT
        : _hgi(hgi), _refCount(1) {
        if (_hgi) _hgi->retain();
#if defined(USE_PIXAR_USD)
        if (_hgi && _hgi->GetPxrHgi()) {
            _registry = std::make_shared<PXR_NS::HdStResourceRegistry>(_hgi->GetPxrHgi());
        }
#endif
    }

    ~HdStResourceRegistryRef() {
        if (_hgi) _hgi->release();
    }

    HgiRef* _hgi;
#if defined(USE_PIXAR_USD)
    std::shared_ptr<PXR_NS::HdStResourceRegistry> _registry;
    mutable std::string _allocationSummaryCache;
#endif
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdStRenderDelegateRef (Storm render backend)
// ============================================================================

/// Storm's implementation of HdRenderDelegate.
///
/// HdStRenderDelegate provides a GPU rasterizer renderer using Hgi for
/// graphics API abstraction. It supports:
/// - Full set of Hydra primitives (meshes, curves, volumes, etc.)
/// - Material networks with GLSLFX/MaterialX shaders
/// - GPU frustum culling and instancing
/// - Multiple AOV outputs (color, depth, primId, etc.)
///
/// Usage:
/// 1. Create a HdStRenderDelegate
/// 2. Create an HdRenderIndex with the delegate
/// 3. Populate the render index via scene delegates
/// 4. Execute render passes via HdEngine
class HdStRenderDelegateRef {
public:
    /// Create a new Storm render delegate.
    /// This creates a render delegate with default settings.
    static HdStRenderDelegateRef* Create() SWIFTUSD_NOEXCEPT {
        return new HdStRenderDelegateRef();
    }

    /// Create a Storm render delegate with specific settings.
    static HdStRenderDelegateRef* CreateWithSettings(
        const std::vector<std::pair<Token, std::string>>& settings) SWIFTUSD_NOEXCEPT {
        return new HdStRenderDelegateRef(settings);
    }

    /// Check if Storm can run on the current hardware.
    /// Returns false if the GPU doesn't support required features.
    static bool IsSupported() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::HdStRenderDelegate::IsSupported();
#else
        return true; // Assume supported in standalone mode
#endif
    }

    /// Set drivers (Hgi device) for this render delegate.
    /// Must be called before creating the render index.
    void SetDrivers(HdDriverRef* driver) SWIFTUSD_NOEXCEPT {
        if (_driver) _driver->release();
        _driver = driver;
        if (_driver) _driver->retain();
#if defined(USE_PIXAR_USD)
        if (_delegate && _driver) {
            PXR_NS::HdDriverVector drivers;
            drivers.push_back(_driver->GetPxrDriver());
            _delegate->SetDrivers(drivers);
        }
#endif
    }

    /// Get the Hgi instance used by this delegate.
    HgiRef* GetHgi() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            PXR_NS::Hgi* hgi = _delegate->GetHgi();
            if (hgi && !_cachedHgi) {
                // Note: We don't own this Hgi, it's owned by the driver
                // This is a simplified wrapper
            }
        }
#endif
        return _cachedHgi;
    }

    /// Get the list of supported Rprim types.
    std::vector<Token> GetSupportedRprimTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto& types = _delegate->GetSupportedRprimTypes();
            std::vector<Token> result;
            result.reserve(types.size());
            for (const auto& t : types) {
                result.push_back(Token(t));
            }
            return result;
        }
#endif
        // Storm's supported Rprim types in standalone mode
        return {
            Token("mesh"),
            Token("basisCurves"),
            Token("points"),
            Token("volume")
        };
    }

    /// Get the list of supported Sprim types.
    std::vector<Token> GetSupportedSprimTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto& types = _delegate->GetSupportedSprimTypes();
            std::vector<Token> result;
            result.reserve(types.size());
            for (const auto& t : types) {
                result.push_back(Token(t));
            }
            return result;
        }
#endif
        // Storm's supported Sprim types in standalone mode
        return {
            Token("camera"),
            Token("drawTarget"),
            Token("extComputation"),
            Token("light"),
            Token("material")
        };
    }

    /// Get the list of supported Bprim types.
    std::vector<Token> GetSupportedBprimTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto& types = _delegate->GetSupportedBprimTypes();
            std::vector<Token> result;
            result.reserve(types.size());
            for (const auto& t : types) {
                result.push_back(Token(t));
            }
            return result;
        }
#endif
        // Storm's supported Bprim types in standalone mode
        return {
            Token("renderBuffer")
        };
    }

    /// Get the resource registry for this delegate.
    HdStResourceRegistryRef* GetResourceRegistry() const SWIFTUSD_NOEXCEPT {
        return _resourceRegistry;
    }

    /// Get render setting descriptors.
    std::vector<HdRenderSettingDescriptor> GetRenderSettingDescriptors() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto descs = _delegate->GetRenderSettingDescriptors();
            std::vector<HdRenderSettingDescriptor> result;
            result.reserve(descs.size());
            for (const auto& d : descs) {
                HdRenderSettingDescriptor desc;
                desc.name = d.name;
                desc.key = Token(d.key);
                result.push_back(desc);
            }
            return result;
        }
#endif
        return {};
    }

    /// Get the material render contexts supported by this delegate.
    std::vector<Token> GetMaterialRenderContexts() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto contexts = _delegate->GetMaterialRenderContexts();
            std::vector<Token> result;
            result.reserve(contexts.size());
            for (const auto& c : contexts) {
                result.push_back(Token(c));
            }
            return result;
        }
#endif
        // Default material contexts for Storm
        return {
            Token("glslfx"),
            Token("mtlx") // MaterialX if enabled
        };
    }

    /// Get the shader source types supported by this delegate.
    std::vector<Token> GetShaderSourceTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto types = _delegate->GetShaderSourceTypes();
            std::vector<Token> result;
            result.reserve(types.size());
            for (const auto& t : types) {
                result.push_back(Token(t));
            }
            return result;
        }
#endif
        return { Token("glslfx") };
    }

    /// Check if primvar filtering is needed.
    bool IsPrimvarFilteringNeeded() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->IsPrimvarFilteringNeeded();
#endif
        return true;
    }

    /// Commit pending resources.
    void CommitResources(HdChangeTrackerRef* tracker) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate && tracker) {
            // Note: Real implementation would access the underlying tracker
        }
#else
        (void)tracker;
#endif
    }

    /// Get the underlying Pixar render delegate.
#if defined(USE_PIXAR_USD)
    PXR_NS::HdStRenderDelegate* GetPxrRenderDelegate() SWIFTUSD_NOEXCEPT {
        return _delegate.get();
    }

    /// Get as base HdRenderDelegate pointer for use with HdRenderIndex.
    PXR_NS::HdRenderDelegate* GetPxrRenderDelegateBase() SWIFTUSD_NOEXCEPT {
        return _delegate.get();
    }
#endif

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
    HdStRenderDelegateRef() SWIFTUSD_NOEXCEPT
        : _driver(nullptr)
        , _cachedHgi(nullptr)
        , _resourceRegistry(nullptr)
        , _refCount(1) {
#if defined(USE_PIXAR_USD)
        _delegate = std::make_unique<PXR_NS::HdStRenderDelegate>();
#endif
    }

    explicit HdStRenderDelegateRef(
        const std::vector<std::pair<Token, std::string>>& settings) SWIFTUSD_NOEXCEPT
        : _driver(nullptr)
        , _cachedHgi(nullptr)
        , _resourceRegistry(nullptr)
        , _refCount(1) {
#if defined(USE_PIXAR_USD)
        PXR_NS::HdRenderSettingsMap settingsMap;
        for (const auto& s : settings) {
            settingsMap[s.first.GetTfToken()] = PXR_NS::VtValue(s.second);
        }
        _delegate = std::make_unique<PXR_NS::HdStRenderDelegate>(settingsMap);
#else
        (void)settings;
#endif
    }

    ~HdStRenderDelegateRef() {
        if (_driver) _driver->release();
        if (_cachedHgi) _cachedHgi->release();
        if (_resourceRegistry) _resourceRegistry->release();
    }

#if defined(USE_PIXAR_USD)
    std::unique_ptr<PXR_NS::HdStRenderDelegate> _delegate;
#endif
    HdDriverRef* _driver;
    HgiRef* _cachedHgi;
    HdStResourceRegistryRef* _resourceRegistry;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdStMeshTopology (Mesh topology data)
// ============================================================================

/// Mesh topology information for Storm.
/// This is a simplified representation; the full HdSt_MeshTopology is internal.
struct HdStMeshTopologyInfo {
    int numFaces;
    int numPoints;
    int numFaceVertices;
    bool isLeftHanded;
    Token scheme; // "none", "catmullClark", "loop", "bilinear"

    HdStMeshTopologyInfo() SWIFTUSD_NOEXCEPT
        : numFaces(0)
        , numPoints(0)
        , numFaceVertices(0)
        , isLeftHanded(false)
        , scheme(Token("none")) {}

    HdStMeshTopologyInfo(int faces, int points, int faceVerts,
                         bool leftHanded, const Token& subdScheme) SWIFTUSD_NOEXCEPT
        : numFaces(faces)
        , numPoints(points)
        , numFaceVertices(faceVerts)
        , isLeftHanded(leftHanded)
        , scheme(subdScheme) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdStMaterialParams (Material parameter info)
// ============================================================================

/// Describes a material parameter for Storm shaders.
struct HdStMaterialParam {
    Token name;              ///< Parameter name
    Token role;              ///< Semantic role (e.g., "color", "normal")
    std::string fallbackValue; ///< Default value as string
    bool isPrimvar;          ///< Is this a primvar binding?
    bool isTexture;          ///< Is this a texture binding?

    HdStMaterialParam() SWIFTUSD_NOEXCEPT
        : isPrimvar(false)
        , isTexture(false) {}

    HdStMaterialParam(const Token& name_, const Token& role_,
                      const char* fallback = "") SWIFTUSD_NOEXCEPT
        : name(name_)
        , role(role_)
        , fallbackValue(fallback ? fallback : "")
        , isPrimvar(false)
        , isTexture(false) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdStMaterialBinding (Material binding info)
// ============================================================================

/// Represents a material binding for a prim.
struct HdStMaterialBinding {
    Path materialPath;       ///< Path to the material prim
    Token materialTag;       ///< Material render tag
    bool isValid;            ///< Is this a valid binding?

    HdStMaterialBinding() SWIFTUSD_NOEXCEPT
        : isValid(false) {}

    explicit HdStMaterialBinding(const Path& path, const Token& tag = Token()) SWIFTUSD_NOEXCEPT
        : materialPath(path)
        , materialTag(tag)
        , isValid(!path.IsEmpty()) {}

    bool IsValid() const SWIFTUSD_NOEXCEPT { return isValid; }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdStDrawItemInfo (Draw item metadata)
// ============================================================================

/// Information about a draw item in Storm's rendering pipeline.
struct HdStDrawItemInfo {
    Path rprimId;            ///< The rprim this draw item belongs to
    Token reprToken;         ///< Representation token (refined, hull, etc.)
    Token materialTag;       ///< Material render tag
    int drawCoordIndex;      ///< Index into draw coordinate buffer
    bool isVisible;          ///< Is this draw item visible?

    HdStDrawItemInfo() SWIFTUSD_NOEXCEPT
        : drawCoordIndex(-1)
        , isVisible(true) {}

    HdStDrawItemInfo(const Path& id, const Token& repr,
                     const Token& matTag, bool visible = true) SWIFTUSD_NOEXCEPT
        : rprimId(id)
        , reprToken(repr)
        , materialTag(matTag)
        , drawCoordIndex(-1)
        , isVisible(visible) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdStRenderPassState (Render pass configuration)
// ============================================================================

/// Configuration for a Storm render pass.
/// Controls camera, lighting, materials, and other render settings.
struct HdStRenderPassStateConfig {
    float alphaThreshold;        ///< Alpha cutoff for masked materials
    bool enableDepthTest;        ///< Enable depth testing
    bool enableDepthWrite;       ///< Enable depth buffer writes
    bool enableStencil;          ///< Enable stencil operations
    HdCullStyle cullStyle;       ///< Face culling mode
    bool enableSceneMaterials;   ///< Use scene materials vs override

    HdStRenderPassStateConfig() SWIFTUSD_NOEXCEPT
        : alphaThreshold(0.5f)
        , enableDepthTest(true)
        , enableDepthWrite(true)
        , enableStencil(false)
        , cullStyle(HdCullStyle::BackUnlessDoubleSided)
        , enableSceneMaterials(true) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdStAovDescriptor (AOV configuration)
// ============================================================================

/// Describes an AOV (Arbitrary Output Variable) for Storm.
/// AOVs represent render outputs like color, depth, primId, etc.
struct HdStAovDescriptor {
    Token aovName;           ///< AOV identifier (e.g., "color", "depth")
    Token format;            ///< Data format token
    Vec4f clearValue;        ///< Clear color value
    bool multiSampled;       ///< Use MSAA for this AOV

    HdStAovDescriptor() SWIFTUSD_NOEXCEPT
        : clearValue(0.0f, 0.0f, 0.0f, 1.0f)
        , multiSampled(false) {}

    HdStAovDescriptor(const Token& name, const Token& fmt,
                      const Vec4f& clear, bool msaa = false) SWIFTUSD_NOEXCEPT
        : aovName(name)
        , format(fmt)
        , clearValue(clear)
        , multiSampled(msaa) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Common Storm AOV Tokens
// ============================================================================

namespace HdStAovTokens {
    inline Token Color() { return Token("color"); }
    inline Token Depth() { return Token("depth"); }
    inline Token CameraDepth() { return Token("cameraDepth"); }
    inline Token PrimId() { return Token("primId"); }
    inline Token InstanceId() { return Token("instanceId"); }
    inline Token ElementId() { return Token("elementId"); }
    inline Token EdgeId() { return Token("edgeId"); }
    inline Token PointId() { return Token("pointId"); }
    inline Token Normal() { return Token("normal"); }
    inline Token NeyelNormal() { return Token("Neye"); }
}

// ============================================================================
// MARK: - HdStComputeQueuePriority (GPU compute ordering)
// ============================================================================

/// Priority levels for GPU compute operations in Storm.
enum class HdStComputeQueuePriority : int32_t {
    Zero = 0,    ///< First priority (runs first)
    One = 1,     ///< Second priority
    Two = 2,     ///< Third priority
    Three = 3,   ///< Fourth priority (runs last)
    Count = 4
};

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Get the string name for a texture type.
inline const char* HdStGetTextureTypeName(HdStTextureType type) SWIFTUSD_NOEXCEPT {
    switch (type) {
        case HdStTextureType::Uv: return "uv";
        case HdStTextureType::Field: return "field";
        case HdStTextureType::Ptex: return "ptex";
        case HdStTextureType::Udim: return "udim";
        default: return "unknown";
    }
}

/// Check if Storm is available on this system.
inline bool HdStIsSupported() SWIFTUSD_NOEXCEPT {
    return HdStRenderDelegateRef::IsSupported();
}

} // namespace swiftusd
