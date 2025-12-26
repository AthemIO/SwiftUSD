// SwiftRendering.h - High-level Hydra rendering facade for SwiftUSD
// Provides: HydraRenderer facade, Metal/Vulkan platform views
// Dependencies: UsdImaging, Hdx
// C++ header with Swift interop annotations
//
// SwiftRendering provides a high-level, platform-native rendering interface
// that wraps the full Hydra rendering pipeline. It simplifies the process of:
// - Creating and managing render delegates
// - Setting up the imaging pipeline
// - Rendering USD stages to platform views (Metal/Vulkan)
// - Handling camera, selection, and frame management

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../usd/Sdf.h"
#include "../usd/Usd.h"
#include "Hgi.h"
#include "Hd.h"
#include "Hdx.h"
#include "UsdImaging.h"

#if SWIFTUSD_PLATFORM_APPLE
#include "HgiMetal.h"
#endif

// Debug logging helper
inline void _SwiftRenderingDebugLog(const char* msg) {
    static std::ofstream logFile("/tmp/hydra_debug.log", std::ios::app);
    logFile << msg << std::endl;
    logFile.flush();
}

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/base/gf/frustum.h"
#include "pxr/base/gf/camera.h"
#include "pxr/imaging/hd/engine.h"
#include "pxr/imaging/hd/renderIndex.h"
#include "pxr/imaging/hd/rendererPlugin.h"
#include "pxr/imaging/hd/rendererPluginRegistry.h"
#include "pxr/imaging/hd/pluginRenderDelegateUniqueHandle.h"
#include "pxr/imaging/hdx/taskController.h"
#include "pxr/imaging/hdx/tokens.h"
#include "pxr/imaging/hd/aov.h"
#include "pxr/imaging/hd/renderBuffer.h"
#include "pxr/imaging/hgi/hgi.h"
#include "pxr/usdImaging/usdImaging/delegate.h"
#if SWIFTUSD_PLATFORM_APPLE
#include "pxr/imaging/hgiMetal/hgi.h"
#endif
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class HydraRendererRef;
class RenderFrameRef;
class PlatformViewRef;

// ============================================================================
// MARK: - SwiftRenderingColorSpace
// ============================================================================

/// Color space for render output.
enum class SwiftRenderingColorSpace : int32_t {
    Linear = 0,    ///< Linear color space (scene-referred)
    SRGB,          ///< sRGB color space (display-referred)
    DisplayP3,     ///< Display P3 color space (wide gamut)
    Raw            ///< No color correction applied
};

// ============================================================================
// MARK: - SwiftRenderingDrawMode
// ============================================================================

/// Draw mode for geometry rendering.
enum class SwiftRenderingDrawMode : int32_t {
    Shaded = 0,    ///< Full shaded rendering with materials
    Wireframe,     ///< Wireframe only
    ShadedWireframe, ///< Shaded with wireframe overlay
    Points,        ///< Point cloud rendering
    Geom,          ///< Geometry only (no materials)
    FlatShaded     ///< Flat shaded (no smooth normals)
};

// ============================================================================
// MARK: - SwiftRenderingParams (Rendering configuration)
// ============================================================================

/// Parameters for configuring the renderer.
struct SwiftRenderingParams {
    /// Viewport dimensions (x, y, width, height).
    Vec4d viewport;

    /// Clear color for the background.
    Vec4f clearColor;

    /// Draw mode for geometry.
    SwiftRenderingDrawMode drawMode;

    /// Color space for output.
    SwiftRenderingColorSpace colorSpace;

    /// Enable depth rendering.
    bool enableDepth;

    /// Enable shadows.
    bool enableShadows;

    /// Enable selection highlighting.
    bool enableSelection;

    /// Enable scene lights.
    bool enableSceneLights;

    /// Enable scene materials.
    bool enableSceneMaterials;

    /// Enable camera light (when no scene lights).
    bool enableCameraLight;

    /// Enable dome light visibility.
    bool enableDomeLightVisibility;

    /// Refinement level for subdivision (0-8).
    int refineLevel;

    /// Complexity level for procedural geometry (0.0-1.0).
    float complexity;

    SwiftRenderingParams() SWIFTUSD_NOEXCEPT
        : viewport(0, 0, 1920, 1080)
        , clearColor(0.18f, 0.18f, 0.18f, 1.0f)  // Gray 18%
        , drawMode(SwiftRenderingDrawMode::Shaded)
        , colorSpace(SwiftRenderingColorSpace::SRGB)
        , enableDepth(true)
        , enableShadows(false)
        , enableSelection(true)
        , enableSceneLights(true)
        , enableSceneMaterials(true)
        , enableCameraLight(true)
        , enableDomeLightVisibility(true)
        , refineLevel(0)
        , complexity(1.0f) {}

    SwiftRenderingParams(int width, int height) SWIFTUSD_NOEXCEPT
        : viewport(0, 0, static_cast<double>(width), static_cast<double>(height))
        , clearColor(0.18f, 0.18f, 0.18f, 1.0f)
        , drawMode(SwiftRenderingDrawMode::Shaded)
        , colorSpace(SwiftRenderingColorSpace::SRGB)
        , enableDepth(true)
        , enableShadows(false)
        , enableSelection(true)
        , enableSceneLights(true)
        , enableSceneMaterials(true)
        , enableCameraLight(true)
        , enableDomeLightVisibility(true)
        , refineLevel(0)
        , complexity(1.0f) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - SwiftRenderingCameraParams (Camera configuration)
// ============================================================================

/// Camera parameters for rendering.
struct SwiftRenderingCameraParams {
    /// View matrix (camera transform).
    Matrix4d viewMatrix;

    /// Projection matrix.
    Matrix4d projectionMatrix;

    /// Camera path in the scene (if using scene camera).
    Path cameraPath;

    /// Whether to use the scene camera or free camera.
    bool useSceneCamera;

    SwiftRenderingCameraParams() SWIFTUSD_NOEXCEPT
        : viewMatrix(Matrix4d::Identity())
        , projectionMatrix(Matrix4d::Identity())
        , useSceneCamera(false) {}

    /// Create with view and projection matrices (free camera).
    SwiftRenderingCameraParams(const Matrix4d& view, const Matrix4d& proj) SWIFTUSD_NOEXCEPT
        : viewMatrix(view)
        , projectionMatrix(proj)
        , useSceneCamera(false) {}

    /// Create with a scene camera path.
    SwiftRenderingCameraParams(const Path& camPath) SWIFTUSD_NOEXCEPT
        : viewMatrix(Matrix4d::Identity())
        , projectionMatrix(Matrix4d::Identity())
        , cameraPath(camPath)
        , useSceneCamera(true) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - SwiftRenderingTokens (SwiftRendering-specific tokens)
// ============================================================================

/// Tokens specific to the SwiftRendering module.
namespace SwiftRenderingTokens {
    // Renderer plugin tokens
    inline Token Storm() { return Token("HdStormRendererPlugin"); }
    inline Token Embree() { return Token("HdEmbreeRendererPlugin"); }
    inline Token Prman() { return Token("HdPrmanRendererPlugin"); }

    // Color space tokens
    inline Token LinearColorSpace() { return Token("linear"); }
    inline Token SRGBColorSpace() { return Token("sRGB"); }
    inline Token DisplayP3ColorSpace() { return Token("displayP3"); }
    inline Token RawColorSpace() { return Token("raw"); }

    // Draw mode tokens
    inline Token ShadedDrawMode() { return Token("shaded"); }
    inline Token WireframeDrawMode() { return Token("wireframe"); }
    inline Token ShadedWireframeDrawMode() { return Token("shadedWireframe"); }
    inline Token PointsDrawMode() { return Token("points"); }
    inline Token GeomDrawMode() { return Token("geom"); }
    inline Token FlatShadedDrawMode() { return Token("flatShaded"); }

    // AOV tokens (render outputs)
    inline Token ColorAov() { return Token("color"); }
    inline Token DepthAov() { return Token("depth"); }
    inline Token NormalAov() { return Token("normal"); }
    inline Token PrimIdAov() { return Token("primId"); }
    inline Token InstanceIdAov() { return Token("instanceId"); }
    inline Token ElementIdAov() { return Token("elementId"); }
}

// ============================================================================
// MARK: - SwiftRenderingStats (Rendering statistics)
// ============================================================================

/// Statistics about the rendering process.
struct SwiftRenderingStats {
    /// Total number of prims in the render index.
    size_t primCount;

    /// Number of visible prims.
    size_t visiblePrimCount;

    /// Number of draw calls.
    size_t drawCallCount;

    /// Vertices drawn.
    size_t vertexCount;

    /// Triangles drawn.
    size_t triangleCount;

    /// Time spent in render (in seconds).
    double renderTime;

    /// Time spent in sync (in seconds).
    double syncTime;

    /// Whether the render has converged (for progressive renderers).
    bool isConverged;

    /// Current iteration (for progressive renderers).
    int32_t iteration;

    SwiftRenderingStats() SWIFTUSD_NOEXCEPT
        : primCount(0)
        , visiblePrimCount(0)
        , drawCallCount(0)
        , vertexCount(0)
        , triangleCount(0)
        , renderTime(0.0)
        , syncTime(0.0)
        , isConverged(false)
        , iteration(0) {}

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - RenderFrameRef (Single rendered frame)
// ============================================================================

/// A reference to a rendered frame.
///
/// RenderFrameRef provides access to the result of a render operation,
/// including color buffer, depth buffer, and statistics.
class RenderFrameRef {
public:
    /// Get the frame width in pixels.
    int GetWidth() const SWIFTUSD_NOEXCEPT {
        return _width;
    }

    /// Get the frame height in pixels.
    int GetHeight() const SWIFTUSD_NOEXCEPT {
        return _height;
    }

    /// Get the number of bytes per row.
    size_t GetBytesPerRow() const SWIFTUSD_NOEXCEPT {
        return _bytesPerRow;
    }

    /// Get the color buffer data (RGBA float32).
    /// Returns nullptr if no color buffer is available.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const float* GetColorData() const SWIFTUSD_NOEXCEPT {
        return _colorData.empty() ? nullptr : _colorData.data();
    }

    /// Get the depth buffer data (float32).
    /// Returns nullptr if no depth buffer is available.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const float* GetDepthData() const SWIFTUSD_NOEXCEPT {
        return _depthData.empty() ? nullptr : _depthData.data();
    }

    /// Get the size of the color buffer in bytes.
    size_t GetColorDataSize() const SWIFTUSD_NOEXCEPT {
        return _colorData.size() * sizeof(float);
    }

    /// Get the size of the depth buffer in bytes.
    size_t GetDepthDataSize() const SWIFTUSD_NOEXCEPT {
        return _depthData.size() * sizeof(float);
    }

    /// Get rendering statistics for this frame.
    SwiftRenderingStats GetStats() const SWIFTUSD_NOEXCEPT {
        return _stats;
    }

    /// Check if the frame is valid.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return _width > 0 && _height > 0;
    }

    /// Check if the frame has converged (for progressive renderers).
    bool IsConverged() const SWIFTUSD_NOEXCEPT {
        return _stats.isConverged;
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
    friend class HydraRendererRef;

    RenderFrameRef(int width, int height) SWIFTUSD_NOEXCEPT
        : _width(width)
        , _height(height)
        , _bytesPerRow(width * 4 * sizeof(float))  // RGBA float32
        , _refCount(1) {
        // Pre-allocate buffers
        _colorData.resize(width * height * 4);  // RGBA
        _depthData.resize(width * height);
    }

    ~RenderFrameRef() = default;

    int _width;
    int _height;
    size_t _bytesPerRow;
    std::vector<float> _colorData;
    std::vector<float> _depthData;
    SwiftRenderingStats _stats;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - HydraRendererRef (Main renderer facade)
// ============================================================================

/// High-level Hydra rendering facade.
///
/// HydraRendererRef provides a simplified interface for rendering USD stages
/// using the Hydra rendering framework. It handles:
/// - Render delegate creation and management
/// - Render index and scene delegate setup
/// - Task controller configuration
/// - Frame rendering and presentation
///
/// This is the primary entry point for rendering in SwiftUSD.
///
/// Usage:
/// ```swift
/// // Create a renderer
/// guard let renderer = HydraRenderer.create() else { return }
///
/// // Set the stage to render
/// renderer.setStage(stage, rootPath: "/")
///
/// // Configure camera
/// var cameraParams = SwiftRenderingCameraParams(view: viewMatrix, proj: projMatrix)
/// renderer.setCamera(cameraParams)
///
/// // Render a frame
/// if let frame = renderer.render() {
///     // Use frame.colorData for display
/// }
/// ```
class HydraRendererRef {
public:
    // ========================================================================
    // MARK: - Factory Methods
    // ========================================================================

    /// Create a new Hydra renderer with the default (Storm) renderer plugin.
    static HydraRendererRef* Create() SWIFTUSD_NOEXCEPT {
        return Create(SwiftRenderingTokens::Storm());
    }

    /// Create a new Hydra renderer with a specific renderer plugin.
    ///
    /// @param rendererPluginId Token identifying the renderer plugin
    ///        (e.g., HdStormRendererPlugin, HdEmbreeRendererPlugin)
    static HydraRendererRef* Create(const Token& rendererPluginId) SWIFTUSD_NOEXCEPT {
        return new HydraRendererRef(rendererPluginId);
    }

#if SWIFTUSD_PLATFORM_APPLE
    /// Create a Metal-based Hydra renderer.
    ///
    /// @param mtlDevice Pointer to MTLDevice (pass nullptr for default device)
    static HydraRendererRef* CreateMetal(void* mtlDevice = nullptr) SWIFTUSD_NOEXCEPT {
        return new HydraRendererRef(SwiftRenderingTokens::Storm(), mtlDevice);
    }
#endif

    /// Get the list of available renderer plugins.
    ///
    /// @param count Output parameter for number of plugins
    /// @return Array of plugin tokens (caller should not free)
    static const Token* GetAvailableRendererPlugins(size_t& count) SWIFTUSD_NOEXCEPT {
        static std::vector<Token> plugins;
        if (plugins.empty()) {
#if defined(USE_PIXAR_USD)
            // Query actual available plugins from HD
            // For now, return Storm as the default
            plugins.push_back(SwiftRenderingTokens::Storm());
#else
            plugins.push_back(SwiftRenderingTokens::Storm());
#endif
        }
        count = plugins.size();
        return plugins.data();
    }

    // ========================================================================
    // MARK: - Renderer Information
    // ========================================================================

    /// Get the current renderer plugin ID.
    Token GetRendererPluginId() const SWIFTUSD_NOEXCEPT {
        return _rendererPluginId;
    }

    /// Check if the renderer is valid and ready.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return _valid;
    }

    /// Get the graphics API name (e.g., "Metal", "OpenGL", "Vulkan").
    Token GetGraphicsAPIName() const SWIFTUSD_NOEXCEPT {
        return _graphicsAPI;
    }

    /// Get rendering statistics for the last frame.
    SwiftRenderingStats GetStats() const SWIFTUSD_NOEXCEPT {
        return _lastStats;
    }

    // ========================================================================
    // MARK: - Stage Management
    // ========================================================================

    /// Set the USD stage to render.
    ///
    /// @param stage The USD stage to render
    /// @param rootPath The root prim path for rendering (default "/")
    void SetStage(StageRef* stage, const Path& rootPath = Path("/")) SWIFTUSD_NOEXCEPT {
        if (_stage != stage) {
            if (_stage) _stage->release();
            _stage = stage;
            if (_stage) _stage->retain();
        }
        _rootPath = rootPath;
        _stageNeedsPopulate = true;
    }

    /// Get the current stage being rendered.
    StageRef* GetStage() const SWIFTUSD_NOEXCEPT {
        return _stage;
    }

    /// Get the root path for rendering.
    Path GetRootPath() const SWIFTUSD_NOEXCEPT {
        return _rootPath;
    }

    /// Set the current time for animation.
    ///
    /// @param timeCode The USD time code to render at
    void SetTime(double timeCode) SWIFTUSD_NOEXCEPT {
        _currentTime = timeCode;
    }

    /// Get the current time.
    double GetTime() const SWIFTUSD_NOEXCEPT {
        return _currentTime;
    }

    // ========================================================================
    // MARK: - Rendering Configuration
    // ========================================================================

    /// Set rendering parameters.
    void SetParams(const SwiftRenderingParams& params) SWIFTUSD_NOEXCEPT {
        _params = params;
    }

    /// Get current rendering parameters.
    SwiftRenderingParams GetParams() const SWIFTUSD_NOEXCEPT {
        return _params;
    }

    /// Set camera parameters.
    void SetCamera(const SwiftRenderingCameraParams& camera) SWIFTUSD_NOEXCEPT {
        _cameraParams = camera;
    }

    /// Get current camera parameters.
    SwiftRenderingCameraParams GetCamera() const SWIFTUSD_NOEXCEPT {
        return _cameraParams;
    }

    /// Set the viewport size.
    void SetViewport(int width, int height) SWIFTUSD_NOEXCEPT {
        _params.viewport = Vec4d(0, 0, static_cast<double>(width), static_cast<double>(height));
    }

    /// Get the viewport width.
    int GetViewportWidth() const SWIFTUSD_NOEXCEPT {
        return static_cast<int>(_params.viewport.z);
    }

    /// Get the viewport height.
    int GetViewportHeight() const SWIFTUSD_NOEXCEPT {
        return static_cast<int>(_params.viewport.w);
    }

    // ========================================================================
    // MARK: - Selection
    // ========================================================================

    /// Set the selected prim paths.
    ///
    /// @param paths Array of selected prim paths
    /// @param count Number of paths
    void SetSelection(const Path* paths, size_t count) SWIFTUSD_NOEXCEPT {
        _selectedPaths.clear();
        if (paths && count > 0) {
            _selectedPaths.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                _selectedPaths.push_back(paths[i]);
            }
        }
    }

    /// Clear the selection.
    void ClearSelection() SWIFTUSD_NOEXCEPT {
        _selectedPaths.clear();
    }

    /// Get the number of selected paths.
    size_t GetSelectionCount() const SWIFTUSD_NOEXCEPT {
        return _selectedPaths.size();
    }

    /// Get a selected path by index.
    Path GetSelection(size_t index) const SWIFTUSD_NOEXCEPT {
        if (index < _selectedPaths.size()) {
            return _selectedPaths[index];
        }
        return Path();
    }

    // ========================================================================
    // MARK: - Rendering
    // ========================================================================

    /// Prepare the renderer for a new frame.
    ///
    /// Call this before render() if you need to sync stage changes.
    void Prepare() SWIFTUSD_NOEXCEPT {
        _PrepareInternal();
    }

    /// Render a single frame and return the result.
    ///
    /// @return A RenderFrameRef containing the rendered output, or nullptr on failure
    RenderFrameRef* Render() SWIFTUSD_NOEXCEPT {
        return _RenderInternal();
    }

    /// Render a frame to an external buffer.
    ///
    /// @param colorBuffer Output color buffer (must be width * height * 4 floats)
    /// @param depthBuffer Output depth buffer (must be width * height floats, or nullptr)
    /// @return true if rendering succeeded
    bool RenderToBuffer(float* colorBuffer, float* depthBuffer = nullptr) SWIFTUSD_NOEXCEPT {
        RenderFrameRef* frame = _RenderInternal();
        if (!frame) return false;

        if (colorBuffer && frame->GetColorData()) {
            memcpy(colorBuffer, frame->GetColorData(), frame->GetColorDataSize());
        }
        if (depthBuffer && frame->GetDepthData()) {
            memcpy(depthBuffer, frame->GetDepthData(), frame->GetDepthDataSize());
        }

        frame->release();
        return true;
    }

    /// Check if the last render has converged (for progressive renderers).
    bool IsConverged() const SWIFTUSD_NOEXCEPT {
        return _lastStats.isConverged;
    }

    /// Restart progressive rendering.
    void RestartRender() SWIFTUSD_NOEXCEPT {
        _needsRestart = true;
    }

    // ========================================================================
    // MARK: - Picking
    // ========================================================================

    /// Perform a pick operation at the given screen coordinates.
    ///
    /// @param x Screen X coordinate
    /// @param y Screen Y coordinate
    /// @param outHit Output pick hit result
    /// @return true if something was hit
    bool Pick(int x, int y, HdxPickHit& outHit) SWIFTUSD_NOEXCEPT {
        return _PickInternal(x, y, 1, 1, &outHit, 1) > 0;
    }

    /// Perform a pick operation in a rectangular region.
    ///
    /// @param x Screen X coordinate
    /// @param y Screen Y coordinate
    /// @param width Width of pick region
    /// @param height Height of pick region
    /// @param outHits Array to receive hit results
    /// @param maxHits Maximum number of hits to return
    /// @return Number of hits found
    size_t PickRect(int x, int y, int width, int height,
                    HdxPickHit* outHits, size_t maxHits) SWIFTUSD_NOEXCEPT {
        return _PickInternal(x, y, width, height, outHits, maxHits);
    }

    // ========================================================================
    // MARK: - Resource Management
    // ========================================================================

    /// Force garbage collection of GPU resources.
    void GarbageCollect() SWIFTUSD_NOEXCEPT {
        // In standalone mode, nothing to do
#if defined(USE_PIXAR_USD)
        // In real USD mode, clean up resources
#endif
    }

    /// Invalidate all cached data and force a full rebuild.
    void Invalidate() SWIFTUSD_NOEXCEPT {
        _stageNeedsPopulate = true;
        _needsRestart = true;
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
    HydraRendererRef(const Token& rendererPluginId,
                     [[maybe_unused]] void* mtlDevice = nullptr) SWIFTUSD_NOEXCEPT
        : _rendererPluginId(rendererPluginId)
        , _stage(nullptr)
        , _rootPath("/")
        , _currentTime(0.0)
        , _stageNeedsPopulate(true)
        , _needsRestart(false)
        , _valid(false)
        , _refCount(1) {
#if SWIFTUSD_PLATFORM_APPLE
        _graphicsAPI = Token("Metal");
#elif SWIFTUSD_HGI_VULKAN
        _graphicsAPI = Token("Vulkan");
#else
        _graphicsAPI = Token("OpenGL");
#endif

#if defined(USE_PIXAR_USD)
        _SwiftRenderingDebugLog("[HYDRA] USE_PIXAR_USD is defined - using real Hydra");

        // 1. Create HGI (Hardware Graphics Interface)
#if SWIFTUSD_PLATFORM_APPLE
        _SwiftRenderingDebugLog("[HYDRA] Creating HgiMetal...");
        _hgi = PXR_NS::HgiMetal::CreateHgi();
#else
        _SwiftRenderingDebugLog("[HYDRA] Creating platform default HGI...");
        _hgi = PXR_NS::Hgi::CreatePlatformDefaultHgi();
#endif
        if (!_hgi) {
            _SwiftRenderingDebugLog("[HYDRA] ERROR: HGI creation failed!");
            return;
        }
        _SwiftRenderingDebugLog("[HYDRA] HGI created successfully");

        // 2. Get the render delegate from the plugin registry
        _SwiftRenderingDebugLog("[HYDRA] Getting renderer plugin registry...");
        PXR_NS::HdRendererPluginRegistry& registry =
            PXR_NS::HdRendererPluginRegistry::GetInstance();

        // Use Storm (HdSt) as the default renderer
        PXR_NS::TfToken pluginId = PXR_NS::TfToken("HdStormRendererPlugin");
        _SwiftRenderingDebugLog("[HYDRA] Creating render delegate for HdStormRendererPlugin...");
        _renderDelegate = registry.CreateRenderDelegate(pluginId);

        if (!_renderDelegate) {
            _SwiftRenderingDebugLog("[HYDRA] ERROR: Render delegate creation failed!");
            return;
        }
        _SwiftRenderingDebugLog("[HYDRA] Render delegate created successfully");

        // 3. Create the render index
        _SwiftRenderingDebugLog("[HYDRA] Creating render index...");
        _renderIndex.reset(PXR_NS::HdRenderIndex::New(
            _renderDelegate.Get(),
            PXR_NS::HdDriverVector()
        ));

        if (!_renderIndex) {
            _SwiftRenderingDebugLog("[HYDRA] ERROR: Render index creation failed!");
            return;
        }
        _SwiftRenderingDebugLog("[HYDRA] Render index created successfully");

        // 4. Create the task controller for managing render tasks
        _SwiftRenderingDebugLog("[HYDRA] Creating task controller...");
        _taskControllerId = PXR_NS::SdfPath("/SwiftUSD/TaskController");
        _taskController.reset(new PXR_NS::HdxTaskController(
            _renderIndex.get(),
            _taskControllerId
        ));

        // Set default render parameters
        _taskController->SetEnablePresentation(false);

        _SwiftRenderingDebugLog("[HYDRA] Task controller created successfully");
        _SwiftRenderingDebugLog("[HYDRA] Hydra renderer fully initialized!");
        _valid = true;
#else
        _SwiftRenderingDebugLog("[HYDRA] USE_PIXAR_USD NOT defined - using standalone mode");
        _valid = true;
#endif
    }

    ~HydraRendererRef() {
        if (_stage) _stage->release();
#if defined(USE_PIXAR_USD)
        // Clean up in reverse order of creation
        _imagingDelegate.reset();
        _taskController.reset();
        _renderIndex.reset();
        _renderDelegate = PXR_NS::HdPluginRenderDelegateUniqueHandle();
        _hgi.reset();
#endif
    }

    void _PrepareInternal() SWIFTUSD_NOEXCEPT {
        if (!_stage) return;

#if defined(USE_PIXAR_USD)
        if (!_renderIndex || !_taskController) return;

        PXR_NS::UsdStageRefPtr usdStage = _stage->GetUsdStage();
        if (!usdStage) return;

        if (_stageNeedsPopulate) {
            // Create or recreate the imaging delegate
            PXR_NS::SdfPath delegateId = PXR_NS::SdfPath("/SwiftUSD/ImagingDelegate");

            _imagingDelegate.reset(new PXR_NS::UsdImagingDelegate(
                _renderIndex.get(),
                delegateId
            ));

            // Populate the scene from the USD stage
            _imagingDelegate->Populate(
                usdStage->GetPseudoRoot()
            );

            _stageNeedsPopulate = false;
        }

        if (_imagingDelegate) {
            // Update time
            PXR_NS::UsdTimeCode timeCode(_currentTime);
            _imagingDelegate->SetTime(timeCode);

            // Apply pending changes
            _imagingDelegate->ApplyPendingUpdates();
        }

        // Update viewport
        int width = GetViewportWidth();
        int height = GetViewportHeight();
        if (width > 0 && height > 0) {
            _taskController->SetRenderViewport(PXR_NS::GfVec4d(0, 0, width, height));
        }

        // Update camera
        _UpdateCamera();

        // Update render params
        _UpdateRenderParams();
#else
        _stageNeedsPopulate = false;
#endif
    }

#if defined(USE_PIXAR_USD)
    void _UpdateCamera() SWIFTUSD_NOEXCEPT {
        if (!_taskController) return;

        int width = GetViewportWidth();
        int height = GetViewportHeight();
        if (width <= 0 || height <= 0) return;

        // Create a GfCamera from the camera params
        PXR_NS::GfCamera gfCamera;

        // Set the frustum from view and projection matrices
        PXR_NS::GfMatrix4d viewMatrix(
            _cameraParams.viewMatrix.m00, _cameraParams.viewMatrix.m01,
            _cameraParams.viewMatrix.m02, _cameraParams.viewMatrix.m03,
            _cameraParams.viewMatrix.m10, _cameraParams.viewMatrix.m11,
            _cameraParams.viewMatrix.m12, _cameraParams.viewMatrix.m13,
            _cameraParams.viewMatrix.m20, _cameraParams.viewMatrix.m21,
            _cameraParams.viewMatrix.m22, _cameraParams.viewMatrix.m23,
            _cameraParams.viewMatrix.m30, _cameraParams.viewMatrix.m31,
            _cameraParams.viewMatrix.m32, _cameraParams.viewMatrix.m33
        );

        PXR_NS::GfMatrix4d projMatrix(
            _cameraParams.projectionMatrix.m00, _cameraParams.projectionMatrix.m01,
            _cameraParams.projectionMatrix.m02, _cameraParams.projectionMatrix.m03,
            _cameraParams.projectionMatrix.m10, _cameraParams.projectionMatrix.m11,
            _cameraParams.projectionMatrix.m12, _cameraParams.projectionMatrix.m13,
            _cameraParams.projectionMatrix.m20, _cameraParams.projectionMatrix.m21,
            _cameraParams.projectionMatrix.m22, _cameraParams.projectionMatrix.m23,
            _cameraParams.projectionMatrix.m30, _cameraParams.projectionMatrix.m31,
            _cameraParams.projectionMatrix.m32, _cameraParams.projectionMatrix.m33
        );

        // Set camera matrices on task controller
        _taskController->SetFreeCameraMatrices(viewMatrix, projMatrix);
    }

    void _UpdateRenderParams() SWIFTUSD_NOEXCEPT {
        if (!_taskController) return;

        // Set lighting
        _taskController->SetEnableShadows(_params.enableShadows);

        // Set render tags based on draw mode
        // Note: More complex setups would configure HdRenderPassState here
    }
#endif

    RenderFrameRef* _RenderInternal() SWIFTUSD_NOEXCEPT {
        static int frameNum = 0;
        frameNum++;

        _PrepareInternal();

        int width = GetViewportWidth();
        int height = GetViewportHeight();

        if (width <= 0 || height <= 0) {
            return nullptr;
        }

        RenderFrameRef* frame = new RenderFrameRef(width, height);

#if defined(USE_PIXAR_USD)
        if (frameNum == 1) {
            _SwiftRenderingDebugLog("[RENDER] Using USE_PIXAR_USD rendering path");
        }

        if (!_taskController || !_renderIndex) {
            if (frameNum == 1) {
                char buf[256];
                snprintf(buf, sizeof(buf), "[RENDER] ERROR: taskController=%p, renderIndex=%p",
                    (void*)_taskController.get(), (void*)_renderIndex.get());
                _SwiftRenderingDebugLog(buf);
            }
            // Fallback to clear color
            for (int i = 0; i < width * height; ++i) {
                frame->_colorData[i * 4 + 0] = _params.clearColor.x;
                frame->_colorData[i * 4 + 1] = _params.clearColor.y;
                frame->_colorData[i * 4 + 2] = _params.clearColor.z;
                frame->_colorData[i * 4 + 3] = _params.clearColor.w;
            }
            return frame;
        }

        if (frameNum == 1) {
            _SwiftRenderingDebugLog("[RENDER] Getting render tasks...");
        }

        // Get render tasks from the task controller
        PXR_NS::HdTaskSharedPtrVector tasks = _taskController->GetRenderingTasks();

        if (frameNum == 1) {
            char buf[256];
            snprintf(buf, sizeof(buf), "[RENDER] Got %zu tasks, executing...", tasks.size());
            _SwiftRenderingDebugLog(buf);
        }

        // Execute the render tasks
        _engine.Execute(_renderIndex.get(), &tasks);

        if (frameNum == 1) {
            _SwiftRenderingDebugLog("[RENDER] Tasks executed, reading AOV color buffer...");
        }

        // Read back the rendered image from the AOV buffers
        PXR_NS::HdRenderBuffer* colorBuffer = _taskController->GetRenderOutput(
            PXR_NS::HdAovTokens->color
        );

        if (colorBuffer) {
            if (frameNum == 1) {
                char buf[256];
                snprintf(buf, sizeof(buf), "[RENDER] Color buffer: %dx%d, format=%d",
                    colorBuffer->GetWidth(), colorBuffer->GetHeight(), (int)colorBuffer->GetFormat());
                _SwiftRenderingDebugLog(buf);
            }

            // Map the buffer to read data
            void* data = colorBuffer->Map();
            if (data) {
                // Get buffer info
                int bufWidth = colorBuffer->GetWidth();
                int bufHeight = colorBuffer->GetHeight();
                PXR_NS::HdFormat format = colorBuffer->GetFormat();

                if (frameNum == 1) {
                    _SwiftRenderingDebugLog("[RENDER] Color buffer mapped successfully");
                }

                // Copy data based on format
                if (format == PXR_NS::HdFormatFloat32Vec4 ||
                    format == PXR_NS::HdFormatFloat16Vec4) {
                    // Direct float copy
                    size_t dataSize = std::min(
                        frame->GetColorDataSize(),
                        static_cast<size_t>(bufWidth * bufHeight * 4 * sizeof(float))
                    );
                    memcpy(frame->_colorData.data(), data, dataSize);
                } else if (format == PXR_NS::HdFormatUNorm8Vec4) {
                    // Convert uint8 to float
                    const uint8_t* src = static_cast<const uint8_t*>(data);
                    for (int i = 0; i < width * height * 4; ++i) {
                        frame->_colorData[i] = static_cast<float>(src[i]) / 255.0f;
                    }
                }

                colorBuffer->Unmap();
            } else {
                if (frameNum == 1) {
                    _SwiftRenderingDebugLog("[RENDER] ERROR: Color buffer Map() returned null!");
                }
            }
        } else {
            if (frameNum == 1) {
                _SwiftRenderingDebugLog("[RENDER] ERROR: No color buffer from task controller!");
            }
        }

        // Try to get depth buffer
        PXR_NS::HdRenderBuffer* depthBuffer = _taskController->GetRenderOutput(
            PXR_NS::HdAovTokens->depth
        );

        if (depthBuffer) {
            void* data = depthBuffer->Map();
            if (data) {
                size_t dataSize = std::min(
                    frame->GetDepthDataSize(),
                    static_cast<size_t>(depthBuffer->GetWidth() * depthBuffer->GetHeight() * sizeof(float))
                );
                memcpy(frame->_depthData.data(), data, dataSize);
                depthBuffer->Unmap();
            }
        }

        // Update stats from render index
        frame->_stats.primCount = _renderIndex->GetRprimIds().size();
        frame->_stats.visiblePrimCount = frame->_stats.primCount;
        frame->_stats.isConverged = _taskController->IsConverged();
        frame->_stats.iteration = 1;

        if (frameNum == 1) {
            char buf[256];
            snprintf(buf, sizeof(buf), "[RENDER] Frame complete: primCount=%zu", frame->_stats.primCount);
            _SwiftRenderingDebugLog(buf);
        }
#else
        // Standalone mode: fill with a test pattern
        if (frameNum == 1) {
            _SwiftRenderingDebugLog("[RENDER] WARNING: Using STANDALONE mode (gradient test pattern)!");
            _SwiftRenderingDebugLog("[RENDER] USE_PIXAR_USD was NOT defined at compile time");
        }
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 4;
                // Simple gradient pattern
                frame->_colorData[idx + 0] = static_cast<float>(x) / width;   // R
                frame->_colorData[idx + 1] = static_cast<float>(y) / height;  // G
                frame->_colorData[idx + 2] = _params.clearColor.z;            // B
                frame->_colorData[idx + 3] = 1.0f;                            // A
            }
        }

        // Fill depth with 1.0 (far plane)
        std::fill(frame->_depthData.begin(), frame->_depthData.end(), 1.0f);

        // Update stats
        frame->_stats.primCount = _stage ? 100 : 0;  // Simulated
        frame->_stats.visiblePrimCount = _stage ? 100 : 0;
        frame->_stats.isConverged = true;
        frame->_stats.iteration = 1;
#endif

        _lastStats = frame->_stats;
        return frame;
    }

    size_t _PickInternal(int x, int y, int width, int height,
                         HdxPickHit* outHits, size_t maxHits) SWIFTUSD_NOEXCEPT {
        if (!outHits || maxHits == 0) return 0;
        (void)x; (void)y; (void)width; (void)height;

#if defined(USE_PIXAR_USD)
        // Perform actual Hydra picking
        return 0;
#else
        // Standalone mode: return no hits
        return 0;
#endif
    }

    Token _rendererPluginId;
    Token _graphicsAPI;
    StageRef* _stage;
    Path _rootPath;
    double _currentTime;
    bool _stageNeedsPopulate;
    bool _needsRestart;
    bool _valid;

    SwiftRenderingParams _params;
    SwiftRenderingCameraParams _cameraParams;
    std::vector<Path> _selectedPaths;
    SwiftRenderingStats _lastStats;

    std::atomic<int> _refCount;

#if defined(USE_PIXAR_USD)
    // Hydra rendering pipeline objects
    PXR_NS::HgiUniquePtr _hgi;
    PXR_NS::HdPluginRenderDelegateUniqueHandle _renderDelegate;
    std::unique_ptr<PXR_NS::HdRenderIndex> _renderIndex;
    std::unique_ptr<PXR_NS::HdxTaskController> _taskController;
    std::unique_ptr<PXR_NS::UsdImagingDelegate> _imagingDelegate;
    PXR_NS::HdEngine _engine;
    PXR_NS::SdfPath _taskControllerId;
#endif

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - PlatformViewRef (Native platform view wrapper)
// ============================================================================

#if SWIFTUSD_PLATFORM_APPLE

/// A wrapper for native platform views that can display rendered content.
///
/// On Apple platforms, this wraps CAMetalLayer for Metal-based rendering.
/// The view can be embedded in UIKit/AppKit view hierarchies.
class PlatformViewRef {
public:
    /// Create a platform view with the given renderer.
    static PlatformViewRef* Create(HydraRendererRef* renderer) SWIFTUSD_NOEXCEPT {
        if (!renderer) return nullptr;
        return new PlatformViewRef(renderer);
    }

    /// Get the native layer handle (CAMetalLayer* on Apple).
    SWIFT_RETURNS_INDEPENDENT_VALUE
    void* GetNativeLayer() const SWIFTUSD_NOEXCEPT {
        return _nativeLayer;
    }

    /// Set the drawable size.
    void SetDrawableSize(int width, int height) SWIFTUSD_NOEXCEPT {
        _drawableWidth = width;
        _drawableHeight = height;
        if (_renderer) {
            _renderer->SetViewport(width, height);
        }
    }

    /// Get the drawable width.
    int GetDrawableWidth() const SWIFTUSD_NOEXCEPT {
        return _drawableWidth;
    }

    /// Get the drawable height.
    int GetDrawableHeight() const SWIFTUSD_NOEXCEPT {
        return _drawableHeight;
    }

    /// Render and present to the view.
    ///
    /// @return true if presentation succeeded
    bool Present() SWIFTUSD_NOEXCEPT {
        if (!_renderer) return false;

        RenderFrameRef* frame = _renderer->Render();
        if (!frame) return false;

        // In real implementation, blit to CAMetalLayer's drawable

        frame->release();
        return true;
    }

    /// Get the associated renderer.
    HydraRendererRef* GetRenderer() const SWIFTUSD_NOEXCEPT {
        return _renderer;
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
    PlatformViewRef(HydraRendererRef* renderer) SWIFTUSD_NOEXCEPT
        : _renderer(renderer)
        , _nativeLayer(nullptr)
        , _drawableWidth(0)
        , _drawableHeight(0)
        , _refCount(1) {
        if (_renderer) _renderer->retain();

#if defined(USE_PIXAR_USD)
        // Create actual CAMetalLayer
#endif
    }

    ~PlatformViewRef() {
        if (_renderer) _renderer->release();
    }

    HydraRendererRef* _renderer;
    void* _nativeLayer;
    int _drawableWidth;
    int _drawableHeight;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

#endif // SWIFTUSD_PLATFORM_APPLE

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Get the token for a color space.
inline Token SwiftRenderingGetColorSpaceToken(SwiftRenderingColorSpace colorSpace) SWIFTUSD_NOEXCEPT {
    switch (colorSpace) {
        case SwiftRenderingColorSpace::Linear: return SwiftRenderingTokens::LinearColorSpace();
        case SwiftRenderingColorSpace::SRGB: return SwiftRenderingTokens::SRGBColorSpace();
        case SwiftRenderingColorSpace::DisplayP3: return SwiftRenderingTokens::DisplayP3ColorSpace();
        case SwiftRenderingColorSpace::Raw: return SwiftRenderingTokens::RawColorSpace();
        default: return SwiftRenderingTokens::SRGBColorSpace();
    }
}

/// Get the name of a color space.
inline const char* SwiftRenderingGetColorSpaceName(SwiftRenderingColorSpace colorSpace) SWIFTUSD_NOEXCEPT {
    switch (colorSpace) {
        case SwiftRenderingColorSpace::Linear: return "Linear";
        case SwiftRenderingColorSpace::SRGB: return "sRGB";
        case SwiftRenderingColorSpace::DisplayP3: return "Display P3";
        case SwiftRenderingColorSpace::Raw: return "Raw";
        default: return "Unknown";
    }
}

/// Get the token for a draw mode.
inline Token SwiftRenderingGetDrawModeToken(SwiftRenderingDrawMode drawMode) SWIFTUSD_NOEXCEPT {
    switch (drawMode) {
        case SwiftRenderingDrawMode::Shaded: return SwiftRenderingTokens::ShadedDrawMode();
        case SwiftRenderingDrawMode::Wireframe: return SwiftRenderingTokens::WireframeDrawMode();
        case SwiftRenderingDrawMode::ShadedWireframe: return SwiftRenderingTokens::ShadedWireframeDrawMode();
        case SwiftRenderingDrawMode::Points: return SwiftRenderingTokens::PointsDrawMode();
        case SwiftRenderingDrawMode::Geom: return SwiftRenderingTokens::GeomDrawMode();
        case SwiftRenderingDrawMode::FlatShaded: return SwiftRenderingTokens::FlatShadedDrawMode();
        default: return SwiftRenderingTokens::ShadedDrawMode();
    }
}

/// Get the name of a draw mode.
inline const char* SwiftRenderingGetDrawModeName(SwiftRenderingDrawMode drawMode) SWIFTUSD_NOEXCEPT {
    switch (drawMode) {
        case SwiftRenderingDrawMode::Shaded: return "Shaded";
        case SwiftRenderingDrawMode::Wireframe: return "Wireframe";
        case SwiftRenderingDrawMode::ShadedWireframe: return "Shaded+Wireframe";
        case SwiftRenderingDrawMode::Points: return "Points";
        case SwiftRenderingDrawMode::Geom: return "Geometry";
        case SwiftRenderingDrawMode::FlatShaded: return "Flat Shaded";
        default: return "Unknown";
    }
}

} // namespace swiftusd
