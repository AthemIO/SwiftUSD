// UsdImaging.h - USD Imaging Delegate for SwiftUSD
// Mirrors: pxr/usdImaging/usdImaging/
// C++ header with Swift interop annotations
//
// UsdImaging provides the scene delegate that translates USD scene
// data into Hydra render primitives. Key components include:
// - UsdImagingDelegate: Main adapter connecting USD to Hydra
// - Schema adapters: Type-specific handlers for geometry, lights, materials
// - Time management: Animation and time-dependent attribute evaluation

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../usd/Sdf.h"
#include "../usd/Usd.h"
#include "Hd.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usdImaging/usdImaging/delegate.h"
#include "pxr/usdImaging/usdImaging/tokens.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class UsdImagingDelegateRef;

// ============================================================================
// MARK: - UsdImagingDelegateParams (Configuration parameters)
// ============================================================================

/// Configuration parameters for UsdImagingDelegate.
/// These control various aspects of scene population and rendering.
struct UsdImagingDelegateParams {
    /// Whether to resolve instance proxies.
    bool resolveInstanceProxies;

    /// Whether to exclude invisible prims from population.
    bool excludeInvisiblePrims;

    /// Default refine level for subdivision surfaces (0-8).
    int refineLevel;

    /// Whether to enable scene material processing.
    bool sceneMaterialsEnabled;

    /// Whether to enable scene lights.
    bool sceneLightsEnabled;

    UsdImagingDelegateParams() SWIFTUSD_NOEXCEPT
        : resolveInstanceProxies(true)
        , excludeInvisiblePrims(false)
        , refineLevel(0)
        , sceneMaterialsEnabled(true)
        , sceneLightsEnabled(true) {}

    bool operator==(const UsdImagingDelegateParams& rhs) const SWIFTUSD_NOEXCEPT {
        return resolveInstanceProxies == rhs.resolveInstanceProxies &&
               excludeInvisiblePrims == rhs.excludeInvisiblePrims &&
               refineLevel == rhs.refineLevel &&
               sceneMaterialsEnabled == rhs.sceneMaterialsEnabled &&
               sceneLightsEnabled == rhs.sceneLightsEnabled;
    }

    bool operator!=(const UsdImagingDelegateParams& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - UsdImagingDelegateRef (Scene delegate wrapper)
// ============================================================================

/// The UsdImagingDelegate is the main adapter connecting USD scene data to
/// the Hydra rendering framework.
///
/// It translates USD prims, attributes, and relationships into the Hydra
/// render index format. The delegate:
/// - Populates the render index from USD stages
/// - Syncs changes from USD to Hydra
/// - Handles time-dependent attribute evaluation
/// - Manages refinement levels for subdivision
/// - Controls material binding and visibility
///
/// UsdImagingDelegate inherits from HdSceneDelegate and implements all the
/// methods needed for Hydra to query scene data.
class UsdImagingDelegateRef {
public:
    /// Create a new UsdImagingDelegate with the given render index and delegate ID.
    ///
    /// @param renderIndex The HdRenderIndex this delegate will populate
    /// @param delegateId A unique path identifying this delegate
    /// @return A new delegate reference, or nullptr if creation fails
    static UsdImagingDelegateRef* Create(HdRenderIndexRef* renderIndex,
                                          const Path& delegateId) SWIFTUSD_NOEXCEPT {
        if (!renderIndex) return nullptr;
#if defined(USE_PIXAR_USD)
        auto* pxrIndex = renderIndex->GetPxrRenderIndex();
        if (!pxrIndex) return nullptr;
        auto* delegate = new PXR_NS::UsdImagingDelegate(pxrIndex, delegateId.GetSdfPath());
        if (!delegate) return nullptr;
        auto* ref = new UsdImagingDelegateRef(delegate, renderIndex, delegateId);
        return ref;
#else
        return new UsdImagingDelegateRef(renderIndex, delegateId);
#endif
    }

    /// Get the delegate ID path.
    Path GetDelegateId() const SWIFTUSD_NOEXCEPT {
        return _delegateId;
    }

    /// Get the render index this delegate populates.
    HdRenderIndexRef* GetRenderIndex() const SWIFTUSD_NOEXCEPT {
        return _renderIndex;
    }

    // ========================================================================
    // MARK: Population Methods
    // ========================================================================

    /// Populate the render index from the given USD stage root prim.
    ///
    /// This traverses the USD scene graph starting at rootPrim and creates
    /// corresponding Hydra prims (Rprim, Sprim, Bprim) in the render index.
    ///
    /// @param stage The USD stage to populate from
    /// @param rootPrimPath Path to the root prim to start population from
    void Populate(StageRef* stage, const Path& rootPrimPath) SWIFTUSD_NOEXCEPT {
        if (!stage) return;
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            auto* pxrStage = stage->GetPxrStage();
            if (pxrStage) {
                PXR_NS::UsdPrim rootPrim = pxrStage->GetPrimAtPath(rootPrimPath.GetSdfPath());
                if (rootPrim) {
                    _delegate->Populate(rootPrim);
                }
            }
        }
#else
        _stage = stage;
        stage->retain();
        _rootPath = rootPrimPath;
        _populatedPaths.push_back(rootPrimPath);
#endif
    }

    /// Populate with excluded paths.
    ///
    /// @param stage The USD stage to populate from
    /// @param rootPrimPath Path to the root prim
    /// @param excludedPaths Array of paths to exclude from population
    /// @param excludedCount Number of paths in excludedPaths
    void PopulateWithExclusions(StageRef* stage,
                                 const Path& rootPrimPath,
                                 const Path* excludedPaths,
                                 size_t excludedCount) SWIFTUSD_NOEXCEPT {
        if (!stage) return;
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            auto* pxrStage = stage->GetPxrStage();
            if (pxrStage) {
                PXR_NS::UsdPrim rootPrim = pxrStage->GetPrimAtPath(rootPrimPath.GetSdfPath());
                if (rootPrim) {
                    PXR_NS::SdfPathVector excludedVec;
                    PXR_NS::SdfPathVector invisedVec;
                    if (excludedPaths && excludedCount > 0) {
                        excludedVec.reserve(excludedCount);
                        for (size_t i = 0; i < excludedCount; ++i) {
                            excludedVec.push_back(excludedPaths[i].GetSdfPath());
                        }
                    }
                    _delegate->Populate(rootPrim, excludedVec, invisedVec);
                }
            }
        }
#else
        _stage = stage;
        stage->retain();
        _rootPath = rootPrimPath;
        _populatedPaths.push_back(rootPrimPath);
        if (excludedPaths && excludedCount > 0) {
            for (size_t i = 0; i < excludedCount; ++i) {
                _excludedPaths.push_back(excludedPaths[i]);
            }
        }
#endif
    }

    /// Clear all prims populated by this delegate.
    void ClearPopulatedPrims() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Note: The real delegate doesn't have a direct clear method.
        // Clearing is typically done by destroying the delegate.
#else
        _populatedPaths.clear();
        _excludedPaths.clear();
#endif
    }

    // ========================================================================
    // MARK: Time Management
    // ========================================================================

    /// Set the current time for scene evaluation.
    ///
    /// This affects time-sampled attributes. Call ApplyPendingUpdates()
    /// after changing time to sync changes to Hydra.
    ///
    /// @param timeCode The USD time code to evaluate at
    void SetTime(double timeCode) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetTime(PXR_NS::UsdTimeCode(timeCode));
        }
#else
        _currentTime = timeCode;
#endif
    }

    /// Get the current time for scene evaluation.
    double GetTime() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetTime().GetValue();
        }
        return 0.0;
#else
        return _currentTime;
#endif
    }

    /// Get the time with an offset applied.
    ///
    /// @param offset Time offset to add to current time
    /// @return The current time plus the offset
    double GetTimeWithOffset(float offset) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetTimeWithOffset(offset).GetValue();
        }
        return offset;
#else
        return _currentTime + static_cast<double>(offset);
#endif
    }

    // ========================================================================
    // MARK: Refinement Control
    // ========================================================================

    /// Get the fallback refine level used when no per-prim level is set.
    int GetRefineLevelFallback() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetRefineLevelFallback();
        }
        return 0;
#else
        return _refineLevelFallback;
#endif
    }

    /// Set the fallback refine level (0-8).
    ///
    /// Higher levels produce smoother subdivision surfaces but require
    /// more computation and memory.
    ///
    /// @param level Refinement level, clamped to [0, 8]
    void SetRefineLevelFallback(int level) SWIFTUSD_NOEXCEPT {
        int clampedLevel = level < 0 ? 0 : (level > 8 ? 8 : level);
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetRefineLevelFallback(clampedLevel);
        }
#else
        _refineLevelFallback = clampedLevel;
#endif
    }

    /// Set the refine level for a specific prim.
    ///
    /// @param primPath Path to the USD prim
    /// @param level Refinement level for that prim
    void SetRefineLevel(const Path& primPath, int level) SWIFTUSD_NOEXCEPT {
        int clampedLevel = level < 0 ? 0 : (level > 8 ? 8 : level);
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetRefineLevel(primPath.GetSdfPath(), clampedLevel);
        }
#else
        _perPrimRefineLevels[primPath.GetText()] = clampedLevel;
#endif
    }

    /// Clear the per-prim refine level, reverting to fallback.
    ///
    /// @param primPath Path to the USD prim
    void ClearRefineLevel(const Path& primPath) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->ClearRefineLevel(primPath.GetSdfPath());
        }
#else
        _perPrimRefineLevels.erase(primPath.GetText());
#endif
    }

    // ========================================================================
    // MARK: Sync and Update
    // ========================================================================

    /// Apply any pending updates to the render index.
    ///
    /// Call this after changing time or prim properties to ensure
    /// Hydra receives the updated data.
    void ApplyPendingUpdates() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->ApplyPendingUpdates();
        }
#else
        // In fallback mode, nothing to do
#endif
    }

    /// Sync all prims to the render index.
    ///
    /// @param includeUnvarying Whether to sync unvarying attributes
    void SyncAll(bool includeUnvarying) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SyncAll(includeUnvarying);
        }
#else
        (void)includeUnvarying;
#endif
    }

    // ========================================================================
    // MARK: Display Options
    // ========================================================================

    /// Get the display guides flag.
    bool GetDisplayGuides() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetDisplayGuides();
        }
        return false;
#else
        return _displayGuides;
#endif
    }

    /// Set whether to display guide geometry.
    void SetDisplayGuides(bool displayGuides) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetDisplayGuides(displayGuides);
        }
#else
        _displayGuides = displayGuides;
#endif
    }

    /// Get the display render flag.
    bool GetDisplayRender() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetDisplayRender();
        }
        return true;
#else
        return _displayRender;
#endif
    }

    /// Set whether to display render geometry.
    void SetDisplayRender(bool displayRender) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetDisplayRender(displayRender);
        }
#else
        _displayRender = displayRender;
#endif
    }

    /// Get the display proxy flag.
    bool GetDisplayProxy() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetDisplayProxy();
        }
        return false;
#else
        return _displayProxy;
#endif
    }

    /// Set whether to display proxy geometry.
    void SetDisplayProxy(bool displayProxy) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetDisplayProxy(displayProxy);
        }
#else
        _displayProxy = displayProxy;
#endif
    }

    // ========================================================================
    // MARK: Scene Material/Light Control
    // ========================================================================

    /// Get whether scene materials are enabled.
    bool GetSceneMaterialsEnabled() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetSceneMaterialsEnabled();
        }
        return true;
#else
        return _sceneMaterialsEnabled;
#endif
    }

    /// Enable or disable scene materials.
    void SetSceneMaterialsEnabled(bool enabled) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetSceneMaterialsEnabled(enabled);
        }
#else
        _sceneMaterialsEnabled = enabled;
#endif
    }

    /// Get whether scene lights are enabled.
    bool GetSceneLightsEnabled() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            return _delegate->GetSceneLightsEnabled();
        }
        return true;
#else
        return _sceneLightsEnabled;
#endif
    }

    /// Enable or disable scene lights.
    void SetSceneLightsEnabled(bool enabled) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetSceneLightsEnabled(enabled);
        }
#else
        _sceneLightsEnabled = enabled;
#endif
    }

    // ========================================================================
    // MARK: Window Policy
    // ========================================================================

    /// Set the camera path for window policy.
    void SetCameraForSampling(const Path& cameraPath) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            _delegate->SetCameraForSampling(cameraPath.GetSdfPath());
        }
#else
        _cameraPath = cameraPath;
#endif
    }

    /// Get the camera path for window policy.
    Path GetCameraForSampling() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Note: GetCameraForSampling doesn't exist in the public API
        return Path();
#else
        return _cameraPath;
#endif
    }

    // ========================================================================
    // MARK: Transform Operations
    // ========================================================================

    /// Get the root transform applied to all prims.
    Matrix4d GetRootTransform() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            const auto& xform = _delegate->GetRootTransform();
            return Matrix4d(
                xform[0][0], xform[0][1], xform[0][2], xform[0][3],
                xform[1][0], xform[1][1], xform[1][2], xform[1][3],
                xform[2][0], xform[2][1], xform[2][2], xform[2][3],
                xform[3][0], xform[3][1], xform[3][2], xform[3][3]
            );
        }
        return Matrix4d::GetIdentity();
#else
        return _rootTransform;
#endif
    }

    /// Set the root transform applied to all prims.
    void SetRootTransform(const Matrix4d& xform) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate) {
            PXR_NS::GfMatrix4d pxrXform;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    pxrXform[i][j] = xform.GetRow(i)[j];
                }
            }
            _delegate->SetRootTransform(pxrXform);
        }
#else
        _rootTransform = xform;
#endif
    }

    /// Get the invisible prim paths.
    std::vector<Path> GetInvisiblePaths() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Note: No direct getter in USD API
        return {};
#else
        return _invisiblePaths;
#endif
    }

    /// Set paths to invisible prims.
    void SetInvisiblePaths(const Path* paths, size_t count) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_delegate && paths && count > 0) {
            PXR_NS::SdfPathVector pathVec;
            pathVec.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                pathVec.push_back(paths[i].GetSdfPath());
            }
            _delegate->SetInvisedPrimPaths(pathVec);
        }
#else
        _invisiblePaths.clear();
        if (paths && count > 0) {
            _invisiblePaths.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                _invisiblePaths.push_back(paths[i]);
            }
        }
#endif
    }

    // ========================================================================
    // MARK: Reference Counting
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
    UsdImagingDelegateRef(PXR_NS::UsdImagingDelegate* delegate,
                           HdRenderIndexRef* renderIndex,
                           const Path& delegateId) SWIFTUSD_NOEXCEPT
        : _delegate(delegate)
        , _renderIndex(renderIndex)
        , _delegateId(delegateId)
        , _refCount(1) {
        if (_renderIndex) _renderIndex->retain();
    }

    ~UsdImagingDelegateRef() {
        delete _delegate;
        if (_renderIndex) _renderIndex->release();
    }

    PXR_NS::UsdImagingDelegate* _delegate;
#else
    UsdImagingDelegateRef(HdRenderIndexRef* renderIndex,
                           const Path& delegateId) SWIFTUSD_NOEXCEPT
        : _renderIndex(renderIndex)
        , _delegateId(delegateId)
        , _stage(nullptr)
        , _currentTime(0.0)
        , _refineLevelFallback(0)
        , _displayGuides(false)
        , _displayRender(true)
        , _displayProxy(false)
        , _sceneMaterialsEnabled(true)
        , _sceneLightsEnabled(true)
        , _rootTransform(Matrix4d::Identity())
        , _refCount(1) {
        if (_renderIndex) _renderIndex->retain();
    }

    ~UsdImagingDelegateRef() {
        if (_renderIndex) _renderIndex->release();
        if (_stage) _stage->release();
    }

    StageRef* _stage;
    Path _rootPath;
    std::vector<Path> _populatedPaths;
    std::vector<Path> _excludedPaths;
    std::vector<Path> _invisiblePaths;
    Path _cameraPath;
    double _currentTime;
    int _refineLevelFallback;
    std::unordered_map<std::string, int> _perPrimRefineLevels;
    bool _displayGuides;
    bool _displayRender;
    bool _displayProxy;
    bool _sceneMaterialsEnabled;
    bool _sceneLightsEnabled;
    Matrix4d _rootTransform;
#endif

    HdRenderIndexRef* _renderIndex;
    Path _delegateId;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - UsdImaging Tokens
// ============================================================================

/// Common tokens used in UsdImaging.
namespace UsdImagingTokens {

/// Material binding purpose tokens
inline Token AllPurpose() { return Token("allPurpose"); }
inline Token Preview() { return Token("preview"); }
inline Token Full() { return Token("full"); }

/// Render visibility tokens
inline Token Invisible() { return Token("invisible"); }
inline Token Visible() { return Token("visible"); }
inline Token Inherited() { return Token("inherited"); }

/// Render tag tokens
inline Token Geometry() { return Token("geometry"); }
inline Token Guide() { return Token("guide"); }
inline Token Proxy() { return Token("proxy"); }
inline Token Render() { return Token("render"); }

/// Prim type tokens (from UsdGeom)
inline Token Mesh() { return Token("Mesh"); }
inline Token BasisCurves() { return Token("BasisCurves"); }
inline Token Points() { return Token("Points"); }
inline Token Sphere() { return Token("Sphere"); }
inline Token Cube() { return Token("Cube"); }
inline Token Cylinder() { return Token("Cylinder"); }
inline Token Capsule() { return Token("Capsule"); }
inline Token Cone() { return Token("Cone"); }

/// Camera tokens
inline Token Camera() { return Token("Camera"); }
inline Token Perspective() { return Token("perspective"); }
inline Token Orthographic() { return Token("orthographic"); }

/// Light type tokens
inline Token DomeLight() { return Token("DomeLight"); }
inline Token DistantLight() { return Token("DistantLight"); }
inline Token SphereLight() { return Token("SphereLight"); }
inline Token RectLight() { return Token("RectLight"); }
inline Token DiskLight() { return Token("DiskLight"); }
inline Token CylinderLight() { return Token("CylinderLight"); }

} // namespace UsdImagingTokens

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Get the default material binding purpose token.
inline Token UsdImagingGetDefaultMaterialPurpose() SWIFTUSD_NOEXCEPT {
    return UsdImagingTokens::AllPurpose();
}

/// Get the preview material binding purpose token.
inline Token UsdImagingGetPreviewMaterialPurpose() SWIFTUSD_NOEXCEPT {
    return UsdImagingTokens::Preview();
}

/// Get the full material binding purpose token.
inline Token UsdImagingGetFullMaterialPurpose() SWIFTUSD_NOEXCEPT {
    return UsdImagingTokens::Full();
}

} // namespace swiftusd
