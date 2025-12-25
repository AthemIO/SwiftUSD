// Hd.h - Hydra Core Framework for SwiftUSD
// Mirrors: pxr/imaging/hd/
// C++ header with Swift interop annotations
//
// The Hydra (Hd) framework provides a scene description transport layer
// for connecting scene graphs to renderers. Key components include:
// - HdRenderIndex: Central flattened scene graph representation
// - HdSceneDelegate: Adapter interface for scene data access
// - HdRenderDelegate: Abstract renderer backend interface
// - HdRprim/HdSprim/HdBprim: Renderable, State, and Buffer primitives
// - HdEngine: Application-facing entry point for rendering
// - HdDriver: Device/context handle passed to render delegates

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../base/Vt.h"
#include "../usd/Sdf.h"
#include "Hgi.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hd/engine.h"
#include "pxr/imaging/hd/renderIndex.h"
#include "pxr/imaging/hd/sceneDelegate.h"
#include "pxr/imaging/hd/renderDelegate.h"
#include "pxr/imaging/hd/rprim.h"
#include "pxr/imaging/hd/sprim.h"
#include "pxr/imaging/hd/bprim.h"
#include "pxr/imaging/hd/driver.h"
#include "pxr/imaging/hd/changeTracker.h"
#include "pxr/imaging/hd/tokens.h"
#include "pxr/imaging/hd/types.h"
#include "pxr/imaging/hd/repr.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class HdRenderIndexRef;
class HdSceneDelegateRef;
class HdRenderDelegateRef;
class HdEngineRef;
class HdDriverRef;
class HdChangeTrackerRef;

// ============================================================================
// MARK: - HdDirtyBits (Change tracking flags)
// ============================================================================

/// Dirty bits for tracking scene changes.
/// These flags indicate which aspects of a prim have been modified.
using HdDirtyBits = uint32_t;

constexpr HdDirtyBits HdDirtyBitsClean = 0;
constexpr HdDirtyBits HdDirtyBitsAllDirty = 0xffffffff;

// Common dirty bit flags for Rprims
constexpr HdDirtyBits HdDirtyBitsDirtyPrimId = 1 << 0;
constexpr HdDirtyBits HdDirtyBitsDirtyExtent = 1 << 1;
constexpr HdDirtyBits HdDirtyBitsDirtyDisplayStyle = 1 << 2;
constexpr HdDirtyBits HdDirtyBitsDirtyPoints = 1 << 3;
constexpr HdDirtyBits HdDirtyBitsDirtyPrimvar = 1 << 4;
constexpr HdDirtyBits HdDirtyBitsDirtyMaterialId = 1 << 5;
constexpr HdDirtyBits HdDirtyBitsDirtyTopology = 1 << 6;
constexpr HdDirtyBits HdDirtyBitsDirtyTransform = 1 << 7;
constexpr HdDirtyBits HdDirtyBitsDirtyVisibility = 1 << 8;
constexpr HdDirtyBits HdDirtyBitsDirtyNormals = 1 << 9;
constexpr HdDirtyBits HdDirtyBitsDirtyDoubleSided = 1 << 10;
constexpr HdDirtyBits HdDirtyBitsDirtyCullStyle = 1 << 11;
constexpr HdDirtyBits HdDirtyBitsDirtySubdivTags = 1 << 12;
constexpr HdDirtyBits HdDirtyBitsDirtyWidths = 1 << 13;
constexpr HdDirtyBits HdDirtyBitsDirtyInstancer = 1 << 14;
constexpr HdDirtyBits HdDirtyBitsDirtyInstanceIndex = 1 << 15;
constexpr HdDirtyBits HdDirtyBitsDirtyRepr = 1 << 16;
constexpr HdDirtyBits HdDirtyBitsDirtyRenderTag = 1 << 17;
constexpr HdDirtyBits HdDirtyBitsDirtyComputationPrimvarDesc = 1 << 18;
constexpr HdDirtyBits HdDirtyBitsDirtyCategories = 1 << 19;
constexpr HdDirtyBits HdDirtyBitsDirtyVolumeField = 1 << 20;

// ============================================================================
// MARK: - HdInterpolation (Primvar interpolation modes)
// ============================================================================

/// Interpolation modes for primvar data.
enum class HdInterpolation : int32_t {
    Constant = 0,     ///< One value for the entire prim
    Uniform,          ///< One value per face/element
    Varying,          ///< One value per vertex (linear interpolation)
    Vertex,           ///< One value per vertex (subdivision interpolation)
    FaceVarying,      ///< One value per face-vertex
    Instance,         ///< One value per instance
    Count
};

// ============================================================================
// MARK: - HdCullStyle (Face culling modes)
// ============================================================================

/// Specifies how face culling should be performed.
enum class HdCullStyle : int32_t {
    DontCare = 0,      ///< No preference
    Nothing,           ///< Don't cull any faces
    Back,              ///< Cull back faces
    Front,             ///< Cull front faces
    BackUnlessDoubleSided,  ///< Cull back unless prim is double-sided
    FrontUnlessDoubleSided  ///< Cull front unless prim is double-sided
};

// ============================================================================
// MARK: - HdReprSelector (Representation selection)
// ============================================================================

/// Selects which representation to use for rendering a prim.
/// A prim can have multiple representations (e.g., refined, hull, points).
struct HdReprSelector {
    Token refinedToken;
    Token unrefinedToken;
    Token pointsToken;

    HdReprSelector() SWIFTUSD_NOEXCEPT {}

    explicit HdReprSelector(const Token& refined) SWIFTUSD_NOEXCEPT
        : refinedToken(refined) {}

    HdReprSelector(const Token& refined, const Token& unrefined) SWIFTUSD_NOEXCEPT
        : refinedToken(refined), unrefinedToken(unrefined) {}

    HdReprSelector(const Token& refined, const Token& unrefined, const Token& points) SWIFTUSD_NOEXCEPT
        : refinedToken(refined), unrefinedToken(unrefined), pointsToken(points) {}

    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
        return refinedToken.IsEmpty() && unrefinedToken.IsEmpty() && pointsToken.IsEmpty();
    }

    bool Contains(const Token& reprToken) const SWIFTUSD_NOEXCEPT {
        return refinedToken == reprToken ||
               unrefinedToken == reprToken ||
               pointsToken == reprToken;
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdDisplayStyle (Prim display settings)
// ============================================================================

/// Describes how the geometry of a prim should be displayed.
struct HdDisplayStyle {
    /// The prim refine level, in the range [0, 8].
    int refineLevel;

    /// Is the prim flat shaded.
    bool flatShadingEnabled;

    /// Is the prim displacement shaded.
    bool displacementEnabled;

    /// Is the prim overlayed on top of other prims.
    bool displayInOverlay;

    /// Does the prim act "transparent" for occluded selection.
    bool occludedSelectionShowsThrough;

    /// Should the prim's points get shaded like surfaces.
    bool pointsShadingEnabled;

    /// Is this prim exempt from material override.
    bool materialIsFinal;

    /// Creates a default DisplayStyle.
    HdDisplayStyle() SWIFTUSD_NOEXCEPT
        : refineLevel(0)
        , flatShadingEnabled(false)
        , displacementEnabled(true)
        , displayInOverlay(false)
        , occludedSelectionShowsThrough(false)
        , pointsShadingEnabled(false)
        , materialIsFinal(false) {}

    /// Creates a DisplayStyle with specified refine level.
    explicit HdDisplayStyle(int refineLevel_,
                           bool flatShading = false,
                           bool displacement = true) SWIFTUSD_NOEXCEPT
        : refineLevel(refineLevel_ > 8 ? 8 : (refineLevel_ < 0 ? 0 : refineLevel_))
        , flatShadingEnabled(flatShading)
        , displacementEnabled(displacement)
        , displayInOverlay(false)
        , occludedSelectionShowsThrough(false)
        , pointsShadingEnabled(false)
        , materialIsFinal(false) {}

    bool operator==(const HdDisplayStyle& rhs) const SWIFTUSD_NOEXCEPT {
        return refineLevel == rhs.refineLevel &&
               flatShadingEnabled == rhs.flatShadingEnabled &&
               displacementEnabled == rhs.displacementEnabled &&
               displayInOverlay == rhs.displayInOverlay &&
               occludedSelectionShowsThrough == rhs.occludedSelectionShowsThrough &&
               pointsShadingEnabled == rhs.pointsShadingEnabled &&
               materialIsFinal == rhs.materialIsFinal;
    }

    bool operator!=(const HdDisplayStyle& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdPrimvarDescriptor (Primvar metadata)
// ============================================================================

/// Describes a primvar (primitive variable).
struct HdPrimvarDescriptor {
    Token name;
    HdInterpolation interpolation;
    Token role;
    bool indexed;

    HdPrimvarDescriptor() SWIFTUSD_NOEXCEPT
        : interpolation(HdInterpolation::Constant)
        , indexed(false) {}

    HdPrimvarDescriptor(const Token& name_,
                        HdInterpolation interp_,
                        const Token& role_ = Token()) SWIFTUSD_NOEXCEPT
        : name(name_)
        , interpolation(interp_)
        , role(role_)
        , indexed(false) {}

    HdPrimvarDescriptor(const Token& name_,
                        HdInterpolation interp_,
                        const Token& role_,
                        bool indexed_) SWIFTUSD_NOEXCEPT
        : name(name_)
        , interpolation(interp_)
        , role(role_)
        , indexed(indexed_) {}

    bool operator==(const HdPrimvarDescriptor& rhs) const SWIFTUSD_NOEXCEPT {
        return name == rhs.name &&
               interpolation == rhs.interpolation &&
               role == rhs.role;
    }

    bool operator!=(const HdPrimvarDescriptor& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdDriverRef (Render device handle)
// ============================================================================

/// A driver represents a device object (commonly a render device) that is
/// owned by the application and passed to HdRenderIndex.
///
/// The application manages the lifetime of HdDriver and must ensure it
/// remains valid while Hydra is running.
class HdDriverRef {
public:
    /// Create a driver with the given name.
    static HdDriverRef* Create(const Token& name) SWIFTUSD_NOEXCEPT {
        return new HdDriverRef(name);
    }

    /// Create a driver with name and an Hgi reference.
    static HdDriverRef* CreateWithHgi(const Token& name, HgiRef* hgi) SWIFTUSD_NOEXCEPT {
        return new HdDriverRef(name, hgi);
    }

    /// Get the driver name.
    Token GetName() const SWIFTUSD_NOEXCEPT { return _name; }

    /// Check if the driver has a valid device.
    bool HasDevice() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return !_driver.driver.IsEmpty();
#else
        return _hgi != nullptr;
#endif
    }

    /// Get the associated Hgi instance (if any).
    HgiRef* GetHgi() const SWIFTUSD_NOEXCEPT {
        return _hgi;
    }

#if defined(USE_PIXAR_USD)
    /// Get the underlying Pixar HdDriver (for internal use).
    PXR_NS::HdDriver* GetPxrDriver() SWIFTUSD_NOEXCEPT { return &_driver; }
    const PXR_NS::HdDriver* GetPxrDriver() const SWIFTUSD_NOEXCEPT { return &_driver; }
#endif

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    explicit HdDriverRef(const Token& name) SWIFTUSD_NOEXCEPT
        : _name(name), _hgi(nullptr), _refCount(1) {
#if defined(USE_PIXAR_USD)
        _driver.name = name.GetTfToken();
#endif
    }

    HdDriverRef(const Token& name, HgiRef* hgi) SWIFTUSD_NOEXCEPT
        : _name(name), _hgi(hgi), _refCount(1) {
        if (_hgi) _hgi->retain();
#if defined(USE_PIXAR_USD)
        _driver.name = name.GetTfToken();
        // Note: In real USD, you'd set _driver.driver to the Hgi pointer
#endif
    }

    ~HdDriverRef() {
        if (_hgi) _hgi->release();
    }

    Token _name;
    HgiRef* _hgi;
#if defined(USE_PIXAR_USD)
    PXR_NS::HdDriver _driver;
#endif
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdChangeTrackerRef (Change tracking)
// ============================================================================

/// Tracks changes to scene primitives for dirty bit management.
///
/// The change tracker is owned by an HdRenderIndex and provides methods
/// to query and mark dirty states for prims.
class HdChangeTrackerRef {
public:
#if !defined(USE_PIXAR_USD)
    /// Create a standalone change tracker (for testing).
    HdChangeTrackerRef() SWIFTUSD_NOEXCEPT
        : _visibilityChangeCount(0)
        , _renderTagVersion(0)
        , _refCount(1) {}
#endif

    /// Mark an rprim dirty with specified dirty bits.
    void MarkRprimDirty(const Path& id, HdDirtyBits bits = HdDirtyBitsAllDirty) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) _tracker->MarkRprimDirty(id.GetSdfPath(), bits);
#else
        (void)id;
        (void)bits;
#endif
    }

    /// Mark an sprim dirty with specified dirty bits.
    void MarkSprimDirty(const Path& id, HdDirtyBits bits = HdDirtyBitsAllDirty) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) _tracker->MarkSprimDirty(id.GetSdfPath(), bits);
#else
        (void)id;
        (void)bits;
#endif
    }

    /// Mark a bprim dirty with specified dirty bits.
    void MarkBprimDirty(const Path& id, HdDirtyBits bits = HdDirtyBitsAllDirty) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) _tracker->MarkBprimDirty(id.GetSdfPath(), bits);
#else
        (void)id;
        (void)bits;
#endif
    }

    /// Mark all rprims dirty.
    void MarkAllRprimsDirty(HdDirtyBits bits) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) _tracker->MarkAllRprimsDirty(bits);
#else
        (void)bits;
#endif
    }

    /// Get the dirty bits for an rprim.
    HdDirtyBits GetRprimDirtyBits(const Path& id) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) return _tracker->GetRprimDirtyBits(id.GetSdfPath());
        return HdDirtyBitsClean;
#else
        (void)id;
        return HdDirtyBitsClean;
#endif
    }

    /// Check if an rprim is dirty.
    bool IsRprimDirty(const Path& id) const SWIFTUSD_NOEXCEPT {
        return GetRprimDirtyBits(id) != HdDirtyBitsClean;
    }

    /// Get the current visibility change count.
    unsigned int GetVisibilityChangeCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) return _tracker->GetVisibilityChangeCount();
        return 0;
#else
        return _visibilityChangeCount;
#endif
    }

    /// Get the current render tag version.
    unsigned int GetRenderTagVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) return _tracker->GetRenderTagVersion();
        return 0;
#else
        return _renderTagVersion;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class HdRenderIndexRef;

#if defined(USE_PIXAR_USD)
    explicit HdChangeTrackerRef(PXR_NS::HdChangeTracker* tracker) SWIFTUSD_NOEXCEPT
        : _tracker(tracker), _refCount(1) {}

    PXR_NS::HdChangeTracker* _tracker;
#else
    unsigned int _visibilityChangeCount;
    unsigned int _renderTagVersion;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdRenderSettingDescriptor (Render setting metadata)
// ============================================================================

/// Describes a render setting that a render delegate exports.
struct HdRenderSettingDescriptor {
    std::string name;      ///< Human readable name
    Token key;             ///< Setting key for Get/SetRenderSetting
    std::string defaultValueStr; ///< Default value as string

    HdRenderSettingDescriptor() SWIFTUSD_NOEXCEPT {}

    HdRenderSettingDescriptor(const char* name_,
                              const Token& key_,
                              const char* defaultValue_ = "") SWIFTUSD_NOEXCEPT
        : name(name_ ? name_ : "")
        , key(key_)
        , defaultValueStr(defaultValue_ ? defaultValue_ : "") {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdRenderDelegateRef (Render backend interface)
// ============================================================================

/// Abstract interface for a rendering backend.
///
/// The render delegate handles the actual creation and management of
/// Hydra scene primitives. It's responsible for:
/// - Creating Rprim/Sprim/Bprim instances
/// - Creating render passes
/// - Managing resources via HdResourceRegistry
/// - Committing resources after sync
///
/// This is a simplified wrapper - full render delegate implementation
/// requires subclassing the real HdRenderDelegate.
class HdRenderDelegateRef {
public:
    /// Get the list of supported Rprim types.
    std::vector<Token> GetSupportedRprimTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_delegate) return {};
        const auto& types = _delegate->GetSupportedRprimTypes();
        std::vector<Token> result;
        result.reserve(types.size());
        for (const auto& t : types) {
            result.push_back(Token(t));
        }
        return result;
#else
        return _supportedRprimTypes;
#endif
    }

    /// Get the list of supported Sprim types.
    std::vector<Token> GetSupportedSprimTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_delegate) return {};
        const auto& types = _delegate->GetSupportedSprimTypes();
        std::vector<Token> result;
        result.reserve(types.size());
        for (const auto& t : types) {
            result.push_back(Token(t));
        }
        return result;
#else
        return _supportedSprimTypes;
#endif
    }

    /// Get the list of supported Bprim types.
    std::vector<Token> GetSupportedBprimTypes() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_delegate) return {};
        const auto& types = _delegate->GetSupportedBprimTypes();
        std::vector<Token> result;
        result.reserve(types.size());
        for (const auto& t : types) {
            result.push_back(Token(t));
        }
        return result;
#else
        return _supportedBprimTypes;
#endif
    }

    /// Check if pausing background rendering is supported.
    bool IsPauseSupported() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->IsPauseSupported();
        return false;
#else
        return false;
#endif
    }

    /// Check if the delegate is currently paused.
    bool IsPaused() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->IsPaused();
        return false;
#else
        return _paused;
#endif
    }

    /// Pause background rendering threads.
    bool Pause() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->Pause();
        return false;
#else
        _paused = true;
        return true;
#endif
    }

    /// Resume background rendering threads.
    bool Resume() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->Resume();
        return false;
#else
        _paused = false;
        return true;
#endif
    }

    /// Check if stopping is supported.
    bool IsStopSupported() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->IsStopSupported();
        return false;
#else
        return false;
#endif
    }

    /// Check if the delegate is stopped.
    bool IsStopped() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->IsStopped();
        return true;
#else
        return _stopped;
#endif
    }

    /// Stop background rendering threads.
    bool Stop(bool blocking = true) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->Stop(blocking);
        return false;
#else
        (void)blocking;
        _stopped = true;
        return true;
#endif
    }

    /// Restart background rendering threads.
    bool Restart() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->Restart();
        return false;
#else
        _stopped = false;
        return true;
#endif
    }

    /// Get the material binding purpose.
    Token GetMaterialBindingPurpose() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return Token(_delegate->GetMaterialBindingPurpose());
        return Token("preview");
#else
        return Token("preview");
#endif
    }

    /// Get render setting descriptors.
    std::vector<HdRenderSettingDescriptor> GetRenderSettingDescriptors() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_delegate) return {};
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
#else
        return {};
#endif
    }

    /// Get the current render settings version.
    unsigned int GetRenderSettingsVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) return _delegate->GetRenderSettingsVersion();
        return 0;
#else
        return _renderSettingsVersion;
#endif
    }

#if defined(USE_PIXAR_USD)
    /// Get the underlying Pixar render delegate.
    PXR_NS::HdRenderDelegate* GetPxrRenderDelegate() SWIFTUSD_NOEXCEPT {
        return _delegate;
    }
#endif

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class HdRenderIndexRef;

#if defined(USE_PIXAR_USD)
    explicit HdRenderDelegateRef(PXR_NS::HdRenderDelegate* delegate, bool owns = false) SWIFTUSD_NOEXCEPT
        : _delegate(delegate), _ownsDelegate(owns), _refCount(1) {}

    ~HdRenderDelegateRef() {
        if (_ownsDelegate && _delegate) {
            delete _delegate;
        }
    }

    PXR_NS::HdRenderDelegate* _delegate;
    bool _ownsDelegate;
#else
    HdRenderDelegateRef() SWIFTUSD_NOEXCEPT
        : _paused(false)
        , _stopped(true)
        , _renderSettingsVersion(0)
        , _refCount(1) {}

    std::vector<Token> _supportedRprimTypes;
    std::vector<Token> _supportedSprimTypes;
    std::vector<Token> _supportedBprimTypes;
    bool _paused;
    bool _stopped;
    unsigned int _renderSettingsVersion;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdRenderIndexRef (Scene graph representation)
// ============================================================================

/// The Hydra render index is a flattened representation of the client scene
/// graph, which may be composed of several self-contained scene graphs.
///
/// Multiple HdSceneDelegate's may be tied to the same HdRenderIndex.
/// The render index is tied to a single HdRenderDelegate, which handles
/// the actual creation and deletion of Hydra scene primitives.
class HdRenderIndexRef {
public:
    /// Create a new render index with the given render delegate and drivers.
    /// Returns nullptr if renderDelegate is null.
    static HdRenderIndexRef* Create(HdRenderDelegateRef* renderDelegate) SWIFTUSD_NOEXCEPT {
        if (!renderDelegate) return nullptr;
#if defined(USE_PIXAR_USD)
        PXR_NS::HdDriverVector drivers;
        auto* index = PXR_NS::HdRenderIndex::New(
            renderDelegate->GetPxrRenderDelegate(), drivers);
        if (!index) return nullptr;
        return new HdRenderIndexRef(index, renderDelegate);
#else
        return new HdRenderIndexRef(renderDelegate);
#endif
    }

    /// Create a render index with driver(s).
    static HdRenderIndexRef* CreateWithDrivers(HdRenderDelegateRef* renderDelegate,
                                                HdDriverRef* driver) SWIFTUSD_NOEXCEPT {
        if (!renderDelegate) return nullptr;
#if defined(USE_PIXAR_USD)
        PXR_NS::HdDriverVector drivers;
        if (driver) {
            drivers.push_back(driver->GetPxrDriver());
        }
        auto* index = PXR_NS::HdRenderIndex::New(
            renderDelegate->GetPxrRenderDelegate(), drivers);
        if (!index) return nullptr;
        auto* ref = new HdRenderIndexRef(index, renderDelegate);
        if (driver) {
            ref->_driver = driver;
            driver->retain();
        }
        return ref;
#else
        auto* ref = new HdRenderIndexRef(renderDelegate);
        if (driver) {
            ref->_driver = driver;
            driver->retain();
        }
        return ref;
#endif
    }

    /// Clear all prims from the render index.
    void Clear() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_index) _index->Clear();
#else
        _rprimIds.clear();
        _sprimIds.clear();
        _bprimIds.clear();
#endif
    }

    /// Get the change tracker for this render index.
    HdChangeTrackerRef* GetChangeTracker() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_index) return nullptr;
        return new HdChangeTrackerRef(&_index->GetChangeTracker());
#else
        return new HdChangeTrackerRef();
#endif
    }

    /// Check if an rprim type is supported.
    bool IsRprimTypeSupported(const Token& typeId) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_index) return _index->IsRprimTypeSupported(typeId.GetTfToken());
        return false;
#else
        (void)typeId;
        return true;
#endif
    }

    /// Check if an sprim type is supported.
    bool IsSprimTypeSupported(const Token& typeId) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_index) return _index->IsSprimTypeSupported(typeId.GetTfToken());
        return false;
#else
        (void)typeId;
        return true;
#endif
    }

    /// Check if a bprim type is supported.
    bool IsBprimTypeSupported(const Token& typeId) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_index) return _index->IsBprimTypeSupported(typeId.GetTfToken());
        return false;
#else
        (void)typeId;
        return true;
#endif
    }

    /// Check if an rprim exists in the index.
    bool HasRprim(const Path& id) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Note: HasRprim is non-const in HdRenderIndex, so we use a const_cast
        if (_index) return const_cast<PXR_NS::HdRenderIndex*>(_index)->HasRprim(id.GetSdfPath());
        return false;
#else
        for (const auto& p : _rprimIds) {
            if (p == id) return true;
        }
        return false;
#endif
    }

    /// Get the number of rprims in the index.
    size_t GetRprimCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_index) return const_cast<PXR_NS::HdRenderIndex*>(_index)->GetRprimIds().size();
        return 0;
#else
        return _rprimIds.size();
#endif
    }

    /// Get the render delegate for this index.
    HdRenderDelegateRef* GetRenderDelegate() const SWIFTUSD_NOEXCEPT {
        return _renderDelegate;
    }

    /// Get the instance name of this render index.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetInstanceName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_index) {
            _instanceNameCache = _index->GetInstanceName();
            return _instanceNameCache.c_str();
        }
        return "";
#else
        return _instanceName.c_str();
#endif
    }

#if defined(USE_PIXAR_USD)
    /// Get the underlying Pixar render index.
    PXR_NS::HdRenderIndex* GetPxrRenderIndex() SWIFTUSD_NOEXCEPT {
        return _index;
    }
#endif

    // Reference counting
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
    HdRenderIndexRef(PXR_NS::HdRenderIndex* index, HdRenderDelegateRef* delegate) SWIFTUSD_NOEXCEPT
        : _index(index)
        , _renderDelegate(delegate)
        , _driver(nullptr)
        , _refCount(1) {
        if (_renderDelegate) _renderDelegate->retain();
    }
#else
    explicit HdRenderIndexRef(HdRenderDelegateRef* delegate) SWIFTUSD_NOEXCEPT
        : _renderDelegate(delegate)
        , _driver(nullptr)
        , _refCount(1) {
        if (_renderDelegate) _renderDelegate->retain();
    }
#endif

    ~HdRenderIndexRef() {
#if defined(USE_PIXAR_USD)
        delete _index;
#endif
        if (_renderDelegate) _renderDelegate->release();
        if (_driver) _driver->release();
    }

#if defined(USE_PIXAR_USD)
    PXR_NS::HdRenderIndex* _index;
    mutable std::string _instanceNameCache;
#else
    std::vector<Path> _rprimIds;
    std::vector<Path> _sprimIds;
    std::vector<Path> _bprimIds;
    std::string _instanceName;
#endif
    HdRenderDelegateRef* _renderDelegate;
    HdDriverRef* _driver;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdEngineRef (Application entry point)
// ============================================================================

/// The application-facing entry point for accessing Hydra.
/// Typically an application would only create one of these.
///
/// The engine orchestrates the execution of render tasks and manages
/// the task context for inter-task communication.
class HdEngineRef {
public:
    /// Create a new Hydra engine.
    static HdEngineRef* Create() SWIFTUSD_NOEXCEPT {
        return new HdEngineRef();
    }

    /// Set data in the task context.
    void SetTaskContextData(const Token& id, const char* data) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _engine.SetTaskContextData(id.GetTfToken(), PXR_NS::VtValue(std::string(data ? data : "")));
#else
        _taskContext[id.GetText()] = std::string(data ? data : "");
#endif
    }

    /// Remove data from the task context.
    void RemoveTaskContextData(const Token& id) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _engine.RemoveTaskContextData(id.GetTfToken());
#else
        _taskContext.erase(id.GetText());
#endif
    }

    /// Clear all task context data.
    void ClearTaskContextData() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _engine.ClearTaskContextData();
#else
        _taskContext.clear();
#endif
    }

    /// Execute rendering with the given render index.
    /// Note: In real usage, you'd pass HdTaskSharedPtrVector or task paths.
    void Execute(HdRenderIndexRef* index) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (index && index->GetPxrRenderIndex()) {
            PXR_NS::HdTaskSharedPtrVector emptyTasks;
            _engine.Execute(index->GetPxrRenderIndex(), &emptyTasks);
        }
#else
        (void)index;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    HdEngineRef() SWIFTUSD_NOEXCEPT : _refCount(1) {}

#if defined(USE_PIXAR_USD)
    PXR_NS::HdEngine _engine;
#else
    std::unordered_map<std::string, std::string> _taskContext;
#endif
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Common Hydra Tokens
// ============================================================================

/// Common tokens used in Hydra.
namespace HdTokens {

/// Standard representation tokens
inline Token Refined() { return Token("refined"); }
inline Token Unrefined() { return Token("hull"); }
inline Token Points() { return Token("points"); }
inline Token Wire() { return Token("wire"); }
inline Token WireOnSurf() { return Token("wireOnSurf"); }
inline Token Smoothed() { return Token("smoothed"); }

/// Standard primvar tokens
inline Token Points_() { return Token("points"); }
inline Token Normals() { return Token("normals"); }
inline Token Widths() { return Token("widths"); }
inline Token DisplayColor() { return Token("displayColor"); }
inline Token DisplayOpacity() { return Token("displayOpacity"); }

/// Standard prim type tokens
inline Token Mesh() { return Token("mesh"); }
inline Token BasisCurves() { return Token("basisCurves"); }
inline Token Points__() { return Token("points"); }
inline Token Volume() { return Token("volume"); }

/// Sprim type tokens
inline Token Camera() { return Token("camera"); }
inline Token Light() { return Token("light"); }
inline Token Material() { return Token("material"); }
inline Token DrawTarget() { return Token("drawTarget"); }
inline Token ExtComputation() { return Token("extComputation"); }

/// Bprim type tokens
inline Token Texture() { return Token("texture"); }
inline Token RenderBuffer() { return Token("renderBuffer"); }

/// Render tags
inline Token RenderTagGeometry() { return Token("geometry"); }
inline Token RenderTagGuide() { return Token("guide"); }
inline Token RenderTagProxy() { return Token("proxy"); }
inline Token RenderTagRender() { return Token("render"); }

/// Driver tokens
inline Token RenderDriver() { return Token("renderDriver"); }
inline Token HgiDriver() { return Token("hgi"); }

} // namespace HdTokens

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Get the string name for an interpolation mode.
inline const char* HdGetInterpolationName(HdInterpolation interp) SWIFTUSD_NOEXCEPT {
    switch (interp) {
        case HdInterpolation::Constant: return "constant";
        case HdInterpolation::Uniform: return "uniform";
        case HdInterpolation::Varying: return "varying";
        case HdInterpolation::Vertex: return "vertex";
        case HdInterpolation::FaceVarying: return "faceVarying";
        case HdInterpolation::Instance: return "instance";
        default: return "unknown";
    }
}

/// Get the string name for a cull style.
inline const char* HdGetCullStyleName(HdCullStyle cullStyle) SWIFTUSD_NOEXCEPT {
    switch (cullStyle) {
        case HdCullStyle::DontCare: return "dontCare";
        case HdCullStyle::Nothing: return "nothing";
        case HdCullStyle::Back: return "back";
        case HdCullStyle::Front: return "front";
        case HdCullStyle::BackUnlessDoubleSided: return "backUnlessDoubleSided";
        case HdCullStyle::FrontUnlessDoubleSided: return "frontUnlessDoubleSided";
        default: return "unknown";
    }
}

} // namespace swiftusd
