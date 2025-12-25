// Hdx.h - Hydra Extension Framework for SwiftUSD
// Mirrors: pxr/imaging/hdx/
// C++ header with Swift interop annotations
//
// The Hdx (Hydra Extension) module provides high-level rendering tasks and
// utilities built on top of Hydra. Key components include:
// - HdxTaskController: High-level task management for rendering
// - HdxRenderTask: Main rendering task
// - HdxSelectionTask: Selection highlighting task
// - HdxPickTask: GPU-based picking/selection task
// - HdxSelectionTracker: Selection state management

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../base/Vt.h"
#include "../usd/Sdf.h"
#include "Hgi.h"
#include "Hd.h"
#include "HdSt.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <functional>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/hdx/taskController.h"
#include "pxr/imaging/hdx/renderTask.h"
#include "pxr/imaging/hdx/selectionTask.h"
#include "pxr/imaging/hdx/pickTask.h"
#include "pxr/imaging/hdx/selectionTracker.h"
#include "pxr/imaging/hdx/tokens.h"
#include "pxr/imaging/hdx/types.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class HdxTaskControllerRef;
class HdxSelectionTrackerRef;

// ============================================================================
// MARK: - HdxPickTarget (Pick operation target)
// ============================================================================

/// Specifies what elements the pick operation should target.
enum class HdxPickTarget : int32_t {
    PrimsAndInstances = 0,  ///< Pick prims and instances
    Faces,                  ///< Pick individual faces
    Edges,                  ///< Pick individual edges
    Points,                 ///< Pick individual points
    PointsAndInstances      ///< Pick points and instances
};

// ============================================================================
// MARK: - HdxPickResolveMode (Pick resolution strategy)
// ============================================================================

/// Specifies how pick results should be resolved.
enum class HdxPickResolveMode : int32_t {
    NearestToCamera = 0,    ///< Return hit nearest to camera
    NearestToCenter,        ///< Return hit nearest to pick center
    Unique,                 ///< Return unique hits only
    All,                    ///< Return all hits
    Deep                    ///< Return all hits including occluded
};

// ============================================================================
// MARK: - HdxTokens (Hdx-specific tokens)
// ============================================================================

/// Tokens specific to the Hdx module.
namespace HdxTokens {
    // Task context tokens
    inline Token PickParams() { return Token("pickParams"); }
    inline Token PickHits() { return Token("pickHits"); }
    inline Token RenderParams() { return Token("renderParams"); }
    inline Token SelectionState() { return Token("selectionState"); }

    // Pick target tokens
    inline Token PickPrimsAndInstances() { return Token("pickPrimsAndInstances"); }
    inline Token PickFaces() { return Token("pickFaces"); }
    inline Token PickEdges() { return Token("pickEdges"); }
    inline Token PickPoints() { return Token("pickPoints"); }
    inline Token PickPointsAndInstances() { return Token("pickPointsAndInstances"); }

    // Resolve mode tokens
    inline Token ResolveNearestToCamera() { return Token("resolveNearestToCamera"); }
    inline Token ResolveNearestToCenter() { return Token("resolveNearestToCenter"); }
    inline Token ResolveUnique() { return Token("resolveUnique"); }
    inline Token ResolveAll() { return Token("resolveAll"); }
    inline Token ResolveDeep() { return Token("resolveDeep"); }

    // AOV tokens
    inline Token ColorIntermediate() { return Token("colorIntermediate"); }
    inline Token DepthIntermediate() { return Token("depthIntermediate"); }
}

// ============================================================================
// MARK: - HdxPickHit (Pick result structure)
// ============================================================================

/// Represents a single hit from a picking operation.
struct HdxPickHit {
    Path delegateId;          ///< Scene delegate that owns the prim
    Path objectId;            ///< Path to the picked object
    Path instancerId;         ///< Path to the instancer (if instanced)
    int32_t instanceIndex;    ///< Index of the picked instance
    int32_t elementIndex;     ///< Index of the picked face/element
    int32_t edgeIndex;        ///< Index of the picked edge
    int32_t pointIndex;       ///< Index of the picked point
    Vec3d worldSpaceHitPoint; ///< Hit position in world space
    Vec3f worldSpaceHitNormal;///< Hit normal in world space
    float normalizedDepth;    ///< Depth in range [0,1]

    HdxPickHit() SWIFTUSD_NOEXCEPT
        : instanceIndex(-1)
        , elementIndex(-1)
        , edgeIndex(-1)
        , pointIndex(-1)
        , normalizedDepth(0.0f) {}

    HdxPickHit(const Path& objId, const Vec3d& hitPoint, float depth) SWIFTUSD_NOEXCEPT
        : objectId(objId)
        , instanceIndex(-1)
        , elementIndex(-1)
        , edgeIndex(-1)
        , pointIndex(-1)
        , worldSpaceHitPoint(hitPoint)
        , normalizedDepth(depth) {}

    /// Check if this is a valid hit.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return !objectId.IsEmpty();
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxRenderTaskParams (Render task configuration)
// ============================================================================

/// Parameters for configuring a render task.
struct HdxRenderTaskParams {
    Vec4d viewport;            ///< Viewport (x, y, width, height)
    HdCullStyle cullStyle;     ///< Face culling mode
    float alphaThreshold;      ///< Alpha cutoff for masked materials
    bool enableLighting;       ///< Enable scene lighting
    bool enableSceneMaterials; ///< Use scene materials vs override
    bool enableIdRender;       ///< Enable ID buffer rendering (for picking)
    Vec4f overrideColor;       ///< Override color (when not using scene materials)

    HdxRenderTaskParams() SWIFTUSD_NOEXCEPT
        : viewport(0, 0, 1, 1)
        , cullStyle(HdCullStyle::BackUnlessDoubleSided)
        , alphaThreshold(0.5f)
        , enableLighting(true)
        , enableSceneMaterials(true)
        , enableIdRender(false)
        , overrideColor(0.5f, 0.5f, 0.5f, 1.0f) {}

    HdxRenderTaskParams(const Vec4d& vp, bool lighting = true) SWIFTUSD_NOEXCEPT
        : viewport(vp)
        , cullStyle(HdCullStyle::BackUnlessDoubleSided)
        , alphaThreshold(0.5f)
        , enableLighting(lighting)
        , enableSceneMaterials(true)
        , enableIdRender(false)
        , overrideColor(0.5f, 0.5f, 0.5f, 1.0f) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxSelectionTaskParams (Selection highlighting configuration)
// ============================================================================

/// Parameters for configuring selection highlighting.
struct HdxSelectionTaskParams {
    bool enableSelectionHighlight;      ///< Show selection highlight
    bool enableLocateHighlight;         ///< Show hover/locate highlight
    float occludedSelectionOpacity;     ///< Opacity of selection when occluded
    Vec4f selectionColor;               ///< Active selection color
    Vec4f locateColor;                  ///< Hover/locate color

    HdxSelectionTaskParams() SWIFTUSD_NOEXCEPT
        : enableSelectionHighlight(true)
        , enableLocateHighlight(true)
        , occludedSelectionOpacity(0.5f)
        , selectionColor(1.0f, 1.0f, 0.0f, 0.5f)  // Yellow
        , locateColor(0.0f, 1.0f, 1.0f, 0.5f) {}  // Cyan

    HdxSelectionTaskParams(const Vec4f& selColor, const Vec4f& locColor) SWIFTUSD_NOEXCEPT
        : enableSelectionHighlight(true)
        , enableLocateHighlight(true)
        , occludedSelectionOpacity(0.5f)
        , selectionColor(selColor)
        , locateColor(locColor) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxPickTaskParams (Pick task configuration)
// ============================================================================

/// Parameters for configuring a pick task.
struct HdxPickTaskParams {
    HdCullStyle cullStyle;  ///< Face culling during picking

    HdxPickTaskParams() SWIFTUSD_NOEXCEPT
        : cullStyle(HdCullStyle::Nothing) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxPickTaskContextParams (Pick operation parameters)
// ============================================================================

/// Context parameters for pick operations.
/// These parameters are set per-pick rather than as task params.
struct HdxPickTaskContextParams {
    Vec2i resolution;          ///< Pick buffer resolution
    int32_t maxNumDeepEntries; ///< Maximum entries for deep picking
    HdxPickTarget pickTarget;  ///< What to pick (prims, faces, etc.)
    HdxPickResolveMode resolveMode; ///< How to resolve hits
    bool doUnpickablesOcclude; ///< Do unpickable objects block picking
    Matrix4d viewMatrix;       ///< View matrix for picking
    Matrix4d projectionMatrix; ///< Projection matrix for picking
    float alphaThreshold;      ///< Alpha threshold for picking

    HdxPickTaskContextParams() SWIFTUSD_NOEXCEPT
        : resolution(128, 128)
        , maxNumDeepEntries(32000)
        , pickTarget(HdxPickTarget::PrimsAndInstances)
        , resolveMode(HdxPickResolveMode::NearestToCamera)
        , doUnpickablesOcclude(false)
        , alphaThreshold(0.0001f) {}

    HdxPickTaskContextParams(const Vec2i& res, const Matrix4d& view, const Matrix4d& proj) SWIFTUSD_NOEXCEPT
        : resolution(res)
        , maxNumDeepEntries(32000)
        , pickTarget(HdxPickTarget::PrimsAndInstances)
        , resolveMode(HdxPickResolveMode::NearestToCamera)
        , doUnpickablesOcclude(false)
        , viewMatrix(view)
        , projectionMatrix(proj)
        , alphaThreshold(0.0001f) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxColorCorrectionTaskParams (Color correction configuration)
// ============================================================================

/// Parameters for color correction.
struct HdxColorCorrectionTaskParams {
    Token colorCorrectionMode;  ///< Color correction mode (disabled, sRGB, openColorIO)
    std::string displayColorSpace; ///< Target display color space
    std::string viewTransform;  ///< OCIO view transform
    std::string ocioConfigPath; ///< Path to OCIO config file

    HdxColorCorrectionTaskParams() SWIFTUSD_NOEXCEPT
        : colorCorrectionMode(Token("disabled")) {}

    HdxColorCorrectionTaskParams(const Token& mode) SWIFTUSD_NOEXCEPT
        : colorCorrectionMode(mode) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxBoundingBoxTaskParams (Bounding box display configuration)
// ============================================================================

/// Parameters for bounding box visualization.
struct HdxBoundingBoxTaskParams {
    Vec4f bboxColor;           ///< Bounding box line color
    float dashSize;            ///< Dash pattern size (0 = solid)

    HdxBoundingBoxTaskParams() SWIFTUSD_NOEXCEPT
        : bboxColor(1.0f, 1.0f, 0.0f, 1.0f)  // Yellow
        , dashSize(3.0f) {}

    HdxBoundingBoxTaskParams(const Vec4f& color, float dash = 3.0f) SWIFTUSD_NOEXCEPT
        : bboxColor(color)
        , dashSize(dash) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxShadowTaskParams (Shadow rendering configuration)
// ============================================================================

/// Parameters for shadow rendering.
struct HdxShadowTaskParams {
    bool enable;               ///< Enable shadow rendering
    int32_t resolution;        ///< Shadow map resolution

    HdxShadowTaskParams() SWIFTUSD_NOEXCEPT
        : enable(true)
        , resolution(2048) {}

    HdxShadowTaskParams(bool enabled, int32_t res = 2048) SWIFTUSD_NOEXCEPT
        : enable(enabled)
        , resolution(res) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - HdxSelectionTrackerRef (Selection state management)
// ============================================================================

/// Manages selection state for Hdx rendering.
///
/// The selection tracker maintains the set of selected objects and provides
/// this information to the selection task for highlighting.
class HdxSelectionTrackerRef {
public:
    /// Create a new selection tracker.
    static HdxSelectionTrackerRef* Create() SWIFTUSD_NOEXCEPT {
        return new HdxSelectionTrackerRef();
    }

    /// Get the current selection version.
    int32_t GetVersion() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_tracker) return _tracker->GetVersion();
        return _version;
#else
        return _version;
#endif
    }

    /// Update selection version to indicate changes.
    void IncrementVersion() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // In real USD, selection changes are tracked automatically
        _version++;
#else
        _version++;
#endif
    }

    /// Clear all selections.
    void Clear() SWIFTUSD_NOEXCEPT {
        _selectedPaths.clear();
        _version++;
    }

    /// Add a path to the selection.
    void AddSelection(const Path& path) SWIFTUSD_NOEXCEPT {
        _selectedPaths.push_back(path);
        _version++;
    }

    /// Remove a path from the selection.
    void RemoveSelection(const Path& path) SWIFTUSD_NOEXCEPT {
        auto it = std::find_if(_selectedPaths.begin(), _selectedPaths.end(),
            [&path](const Path& p) { return p == path; });
        if (it != _selectedPaths.end()) {
            _selectedPaths.erase(it);
            _version++;
        }
    }

    /// Check if a path is selected.
    bool IsSelected(const Path& path) const SWIFTUSD_NOEXCEPT {
        for (const auto& p : _selectedPaths) {
            if (p == path) return true;
        }
        return false;
    }

    /// Get the number of selected items.
    size_t GetNumSelections() const SWIFTUSD_NOEXCEPT {
        return _selectedPaths.size();
    }

    /// Get selected path at index.
    Path GetSelection(size_t index) const SWIFTUSD_NOEXCEPT {
        if (index < _selectedPaths.size()) {
            return _selectedPaths[index];
        }
        return Path();
    }

#if defined(USE_PIXAR_USD)
    /// Get the underlying Pixar selection tracker.
    std::shared_ptr<PXR_NS::HdxSelectionTracker> GetPxrSelectionTracker() SWIFTUSD_NOEXCEPT {
        return _tracker;
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
    HdxSelectionTrackerRef() SWIFTUSD_NOEXCEPT
        : _version(0)
        , _refCount(1) {
#if defined(USE_PIXAR_USD)
        _tracker = std::make_shared<PXR_NS::HdxSelectionTracker>();
#endif
    }

    ~HdxSelectionTrackerRef() = default;

#if defined(USE_PIXAR_USD)
    std::shared_ptr<PXR_NS::HdxSelectionTracker> _tracker;
#endif
    std::vector<Path> _selectedPaths;
    int32_t _version;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HdxTaskControllerRef (High-level task management)
// ============================================================================

/// High-level controller for managing Hdx rendering tasks.
///
/// HdxTaskController provides a simplified interface for common rendering
/// scenarios. It manages the creation and configuration of:
/// - Render tasks (HdxRenderTask)
/// - Selection tasks (HdxSelectionTask)
/// - Pick tasks (HdxPickTask)
/// - Shadow tasks
/// - Color correction tasks
/// - And other Hdx rendering infrastructure
///
/// Usage:
/// 1. Create a HdxTaskController with a render index
/// 2. Configure rendering parameters (viewport, camera, lighting, etc.)
/// 3. Get rendering tasks via GetRenderingTasks()
/// 4. Execute tasks with HdEngine
class HdxTaskControllerRef {
public:
    /// Create a task controller with a render index.
    static HdxTaskControllerRef* Create(HdRenderIndexRef* renderIndex,
                                        const Path& controllerId,
                                        bool gpuEnabled = true) SWIFTUSD_NOEXCEPT {
        if (!renderIndex) return nullptr;
        return new HdxTaskControllerRef(renderIndex, controllerId, gpuEnabled);
    }

    /// Get the render index.
    HdRenderIndexRef* GetRenderIndex() const SWIFTUSD_NOEXCEPT {
        return _renderIndex;
    }

    /// Get the controller's scene-graph ID.
    Path GetControllerId() const SWIFTUSD_NOEXCEPT {
        return _controllerId;
    }

    /// Set the render buffer size (for AOVs).
    void SetRenderBufferSize(const Vec2i& size) SWIFTUSD_NOEXCEPT {
        _renderBufferSize = size;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetRenderBufferSize(PXR_NS::GfVec2i(size.x, size.y));
        }
#endif
    }

    /// Get the current render buffer size.
    Vec2i GetRenderBufferSize() const SWIFTUSD_NOEXCEPT {
        return _renderBufferSize;
    }

    /// Set the viewport for rendering.
    void SetRenderViewport(const Vec4d& viewport) SWIFTUSD_NOEXCEPT {
        _viewport = viewport;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetRenderViewport(
                PXR_NS::GfVec4d(viewport.x, viewport.y, viewport.z, viewport.w));
        }
#endif
    }

    /// Get the current viewport.
    Vec4d GetRenderViewport() const SWIFTUSD_NOEXCEPT {
        return _viewport;
    }

    /// Set the camera path.
    void SetCameraPath(const Path& cameraPath) SWIFTUSD_NOEXCEPT {
        _cameraPath = cameraPath;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetCameraPath(cameraPath.GetSdfPath());
        }
#endif
    }

    /// Get the camera path.
    Path GetCameraPath() const SWIFTUSD_NOEXCEPT {
        return _cameraPath;
    }

    /// Set free camera matrices (for cameras not in the scene).
    void SetFreeCameraMatrices(const Matrix4d& viewMatrix,
                               const Matrix4d& projectionMatrix) SWIFTUSD_NOEXCEPT {
        _viewMatrix = viewMatrix;
        _projectionMatrix = projectionMatrix;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            PXR_NS::GfMatrix4d view, proj;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    view[i][j] = viewMatrix.data[i * 4 + j];
                    proj[i][j] = projectionMatrix.data[i * 4 + j];
                }
            }
            _controller->SetFreeCameraMatrices(view, proj);
        }
#endif
    }

    /// Enable or disable selection highlighting.
    void SetEnableSelection(bool enable) SWIFTUSD_NOEXCEPT {
        _selectionEnabled = enable;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetEnableSelection(enable);
        }
#endif
    }

    /// Check if selection is enabled.
    bool IsSelectionEnabled() const SWIFTUSD_NOEXCEPT {
        return _selectionEnabled;
    }

    /// Set the selection color.
    void SetSelectionColor(const Vec4f& color) SWIFTUSD_NOEXCEPT {
        _selectionColor = color;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetSelectionColor(
                PXR_NS::GfVec4f(color.x, color.y, color.z, color.w));
        }
#endif
    }

    /// Get the selection color.
    Vec4f GetSelectionColor() const SWIFTUSD_NOEXCEPT {
        return _selectionColor;
    }

    /// Set the hover/locate color.
    void SetSelectionLocateColor(const Vec4f& color) SWIFTUSD_NOEXCEPT {
        _locateColor = color;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetSelectionLocateColor(
                PXR_NS::GfVec4f(color.x, color.y, color.z, color.w));
        }
#endif
    }

    /// Get the hover/locate color.
    Vec4f GetSelectionLocateColor() const SWIFTUSD_NOEXCEPT {
        return _locateColor;
    }

    /// Enable or disable selection outline rendering.
    void SetSelectionEnableOutline(bool enable) SWIFTUSD_NOEXCEPT {
        _selectionOutline = enable;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetSelectionEnableOutline(enable);
        }
#endif
    }

    /// Set the selection outline radius in pixels.
    void SetSelectionOutlineRadius(uint32_t radius) SWIFTUSD_NOEXCEPT {
        _outlineRadius = radius;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetSelectionOutlineRadius(radius);
        }
#endif
    }

    /// Enable or disable shadow rendering.
    void SetEnableShadows(bool enable) SWIFTUSD_NOEXCEPT {
        _shadowsEnabled = enable;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetEnableShadows(enable);
        }
#endif
    }

    /// Check if shadows are enabled.
    bool AreShadowsEnabled() const SWIFTUSD_NOEXCEPT {
        return _shadowsEnabled;
    }

    /// Enable or disable presentation (final output to framebuffer).
    void SetEnablePresentation(bool enable) SWIFTUSD_NOEXCEPT {
        _presentationEnabled = enable;
#if defined(USE_PIXAR_USD)
        if (_controller) {
            _controller->SetEnablePresentation(enable);
        }
#endif
    }

    /// Check if presentation is enabled.
    bool IsPresentationEnabled() const SWIFTUSD_NOEXCEPT {
        return _presentationEnabled;
    }

    /// Check if the render has converged (for progressive rendering).
    bool IsConverged() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_controller) {
            return _controller->IsConverged();
        }
        return true;
#else
        return true;
#endif
    }

    /// Get the number of rendering task paths.
    size_t GetRenderingTaskPathCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_controller) {
            return _controller->GetRenderingTaskPaths().size();
        }
#endif
        return 0;
    }

    /// Get the number of picking task paths.
    size_t GetPickingTaskPathCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_controller) {
            return _controller->GetPickingTaskPaths().size();
        }
#endif
        return 0;
    }

#if defined(USE_PIXAR_USD)
    /// Get the underlying Pixar task controller.
    PXR_NS::HdxTaskController* GetPxrTaskController() SWIFTUSD_NOEXCEPT {
        return _controller.get();
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
    HdxTaskControllerRef(HdRenderIndexRef* renderIndex,
                         const Path& controllerId,
                         bool gpuEnabled) SWIFTUSD_NOEXCEPT
        : _renderIndex(renderIndex)
        , _controllerId(controllerId)
        , _gpuEnabled(gpuEnabled)
        , _renderBufferSize(512, 512)
        , _viewport(0, 0, 512, 512)
        , _selectionEnabled(true)
        , _selectionColor(1.0f, 1.0f, 0.0f, 0.5f)
        , _locateColor(0.0f, 1.0f, 1.0f, 0.5f)
        , _selectionOutline(false)
        , _outlineRadius(5)
        , _shadowsEnabled(false)
        , _presentationEnabled(true)
        , _refCount(1) {
        if (_renderIndex) _renderIndex->retain();
#if defined(USE_PIXAR_USD)
        if (_renderIndex && _renderIndex->GetPxrRenderIndex()) {
            _controller = std::make_unique<PXR_NS::HdxTaskController>(
                _renderIndex->GetPxrRenderIndex(),
                controllerId.GetSdfPath(),
                gpuEnabled);
        }
#endif
    }

    ~HdxTaskControllerRef() {
        if (_renderIndex) _renderIndex->release();
    }

    HdRenderIndexRef* _renderIndex;
    Path _controllerId;
    bool _gpuEnabled;
#if defined(USE_PIXAR_USD)
    std::unique_ptr<PXR_NS::HdxTaskController> _controller;
#endif

    // Cached state
    Vec2i _renderBufferSize;
    Vec4d _viewport;
    Path _cameraPath;
    Matrix4d _viewMatrix;
    Matrix4d _projectionMatrix;
    bool _selectionEnabled;
    Vec4f _selectionColor;
    Vec4f _locateColor;
    bool _selectionOutline;
    uint32_t _outlineRadius;
    bool _shadowsEnabled;
    bool _presentationEnabled;

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Convert pick target enum to token.
inline Token HdxGetPickTargetToken(HdxPickTarget target) SWIFTUSD_NOEXCEPT {
    switch (target) {
        case HdxPickTarget::PrimsAndInstances: return HdxTokens::PickPrimsAndInstances();
        case HdxPickTarget::Faces: return HdxTokens::PickFaces();
        case HdxPickTarget::Edges: return HdxTokens::PickEdges();
        case HdxPickTarget::Points: return HdxTokens::PickPoints();
        case HdxPickTarget::PointsAndInstances: return HdxTokens::PickPointsAndInstances();
        default: return HdxTokens::PickPrimsAndInstances();
    }
}

/// Convert resolve mode enum to token.
inline Token HdxGetResolveModeToken(HdxPickResolveMode mode) SWIFTUSD_NOEXCEPT {
    switch (mode) {
        case HdxPickResolveMode::NearestToCamera: return HdxTokens::ResolveNearestToCamera();
        case HdxPickResolveMode::NearestToCenter: return HdxTokens::ResolveNearestToCenter();
        case HdxPickResolveMode::Unique: return HdxTokens::ResolveUnique();
        case HdxPickResolveMode::All: return HdxTokens::ResolveAll();
        case HdxPickResolveMode::Deep: return HdxTokens::ResolveDeep();
        default: return HdxTokens::ResolveNearestToCamera();
    }
}

/// Get the name of a pick target.
inline const char* HdxGetPickTargetName(HdxPickTarget target) SWIFTUSD_NOEXCEPT {
    switch (target) {
        case HdxPickTarget::PrimsAndInstances: return "primsAndInstances";
        case HdxPickTarget::Faces: return "faces";
        case HdxPickTarget::Edges: return "edges";
        case HdxPickTarget::Points: return "points";
        case HdxPickTarget::PointsAndInstances: return "pointsAndInstances";
        default: return "unknown";
    }
}

/// Get the name of a resolve mode.
inline const char* HdxGetResolveModeName(HdxPickResolveMode mode) SWIFTUSD_NOEXCEPT {
    switch (mode) {
        case HdxPickResolveMode::NearestToCamera: return "nearestToCamera";
        case HdxPickResolveMode::NearestToCenter: return "nearestToCenter";
        case HdxPickResolveMode::Unique: return "unique";
        case HdxPickResolveMode::All: return "all";
        case HdxPickResolveMode::Deep: return "deep";
        default: return "unknown";
    }
}

} // namespace swiftusd
