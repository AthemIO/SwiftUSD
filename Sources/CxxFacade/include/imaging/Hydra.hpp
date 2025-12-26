// Hydra.hpp - High-level Hydra rendering facade for USD scenes
// CLEAN HEADER - NO pxr includes!
//
// This module provides a simplified interface to Pixar's Hydra rendering
// framework through the UsdImagingGL API. It allows rendering USD stages
// with customizable render settings and camera configurations.
#pragma once

#include "../CxxFacade.h"
#include "../usd/Usd.hpp"
#include "../usdGeom/UsdGeom.hpp"
#include "../base/Gf.hpp"

namespace cxxfacade {
namespace hydra {

// Forward declarations
class HydraEngine;

// ============================================================================
// DrawMode - How geometry should be rendered
// ============================================================================

/// Draw mode enumeration matching UsdImagingGLDrawMode
enum class DrawMode {
    Points,                   ///< Draw as points
    Wireframe,               ///< Draw as wireframe
    WireframeOnSurface,      ///< Draw wireframe on top of shaded surface
    ShadedFlat,              ///< Flat shaded (faceted)
    ShadedSmooth,            ///< Smooth shaded (default)
    GeomOnly,                ///< Geometry only, no materials
    GeomFlat,                ///< Geometry only, flat shaded
    GeomSmooth               ///< Geometry only, smooth shaded
};

// ============================================================================
// CullStyle - Face culling options
// ============================================================================

/// Cull style enumeration matching UsdImagingGLCullStyle
enum class CullStyle {
    NoOpinion,               ///< No opinion, use scene default
    Nothing,                 ///< No culling
    Back,                    ///< Cull back faces
    Front,                   ///< Cull front faces
    BackUnlessDoubleSided    ///< Cull back faces unless double-sided
};

// ============================================================================
// RenderSettings - Parameters for rendering
// ============================================================================

/// RenderSettings - Configuration for render operations
///
/// This structure holds all the parameters needed to configure a render pass,
/// including draw mode, culling, lighting, and visual options.
struct RenderSettings {
    /// Time code for animation (default: earliest time)
    double frame = 0.0;

    /// Geometric complexity/subdivision level (1.0 = default)
    float complexity = 1.0f;

    /// Draw mode (shaded, wireframe, etc.)
    DrawMode drawMode = DrawMode::ShadedSmooth;

    /// Show guide geometry
    bool showGuides = false;

    /// Show proxy geometry
    bool showProxy = true;

    /// Show render geometry
    bool showRender = false;

    /// Force a complete refresh
    bool forceRefresh = false;

    /// Flip front-facing direction
    bool flipFrontFacing = false;

    /// Face culling style
    CullStyle cullStyle = CullStyle::Nothing;

    /// Enable lighting
    bool enableLighting = true;

    /// Enable sample alpha to coverage
    bool enableSampleAlphaToCoverage = false;

    /// Apply render state
    bool applyRenderState = true;

    /// Apply gamma correction
    bool gammaCorrectColors = true;

    /// Highlight selected objects
    bool highlight = false;

    /// Override color (RGBA, zero = no override)
    gf::Vec4f overrideColor = gf::Vec4f(0.0f, 0.0f, 0.0f, 0.0f);

    /// Wireframe color (RGBA)
    gf::Vec4f wireframeColor = gf::Vec4f(0.0f, 0.0f, 0.0f, 0.0f);

    /// Alpha threshold for transparency (-1 = automatic)
    float alphaThreshold = -1.0f;

    /// Enable scene materials
    bool enableSceneMaterials = true;

    /// Enable scene lights
    bool enableSceneLights = true;

    /// Enable USD draw modes
    bool enableUsdDrawModes = true;

    /// Clear color (RGBA)
    gf::Vec4f clearColor = gf::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

    /// Default constructor
    RenderSettings() = default;

    /// Create default settings for preview rendering
    static RenderSettings Preview() {
        RenderSettings settings;
        settings.complexity = 1.0f;
        settings.drawMode = DrawMode::ShadedSmooth;
        settings.enableLighting = true;
        return settings;
    }

    /// Create settings for wireframe rendering
    static RenderSettings Wireframe() {
        RenderSettings settings;
        settings.drawMode = DrawMode::Wireframe;
        settings.enableLighting = false;
        settings.wireframeColor = gf::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
        return settings;
    }

    /// Create settings for high-quality rendering
    static RenderSettings HighQuality() {
        RenderSettings settings;
        settings.complexity = 1.5f;
        settings.drawMode = DrawMode::ShadedSmooth;
        settings.enableLighting = true;
        settings.enableSceneMaterials = true;
        settings.enableSceneLights = true;
        settings.gammaCorrectColors = true;
        return settings;
    }
};

// ============================================================================
// CameraSettings - Camera state for rendering
// ============================================================================

/// CameraSettings - View and projection matrices for rendering
///
/// This structure encapsulates the camera state needed for rendering,
/// including view/projection matrices and viewport configuration.
struct CameraSettings {
    /// View matrix (world to camera transform)
    gf::Matrix4d viewMatrix;

    /// Projection matrix
    gf::Matrix4d projectionMatrix;

    /// Viewport (x, y, width, height)
    gf::Vec4d viewport = gf::Vec4d(0.0, 0.0, 1920.0, 1080.0);

    /// Render buffer size (width, height)
    int bufferWidth = 1920;
    int bufferHeight = 1080;

    /// Default constructor
    CameraSettings()
        : viewMatrix(gf::Matrix4d::Identity())
        , projectionMatrix(gf::Matrix4d::Identity()) {}

    /// Construct with view and projection matrices
    CameraSettings(const gf::Matrix4d& view, const gf::Matrix4d& proj)
        : viewMatrix(view)
        , projectionMatrix(proj) {}

    /// Construct with matrices and viewport
    CameraSettings(const gf::Matrix4d& view,
                   const gf::Matrix4d& proj,
                   int width, int height)
        : viewMatrix(view)
        , projectionMatrix(proj)
        , viewport(0.0, 0.0, static_cast<double>(width), static_cast<double>(height))
        , bufferWidth(width)
        , bufferHeight(height) {}

    /// Set viewport dimensions
    void SetViewport(int x, int y, int width, int height) {
        viewport = gf::Vec4d(static_cast<double>(x),
                             static_cast<double>(y),
                             static_cast<double>(width),
                             static_cast<double>(height));
        bufferWidth = width;
        bufferHeight = height;
    }

    /// Get aspect ratio
    double GetAspectRatio() const {
        if (bufferHeight == 0) return 1.0;
        return static_cast<double>(bufferWidth) / static_cast<double>(bufferHeight);
    }

    /// Create an orthographic camera
    static CameraSettings Orthographic(double left, double right,
                                        double bottom, double top,
                                        double nearPlane, double farPlane);

    /// Create a perspective camera
    static CameraSettings Perspective(double fovY, double aspectRatio,
                                       double nearPlane, double farPlane);

    /// Create a look-at camera
    static CameraSettings LookAt(const gf::Vec3d& eye,
                                  const gf::Vec3d& target,
                                  const gf::Vec3d& up,
                                  double fovY, double aspectRatio,
                                  double nearPlane, double farPlane);
};

// ============================================================================
// RendererInfo - Information about available renderers
// ============================================================================

/// Information about a render plugin
struct RendererInfo {
    /// Plugin identifier
    std::string id;

    /// Display name for UI
    std::string displayName;

    /// Whether GPU is enabled
    bool gpuEnabled = true;

    RendererInfo() = default;
    RendererInfo(const std::string& pluginId, const std::string& name, bool gpu = true)
        : id(pluginId), displayName(name), gpuEnabled(gpu) {}
};

// ============================================================================
// HydraEngine - Main rendering engine
// ============================================================================

/// HydraEngine - High-level Hydra rendering engine
///
/// This class provides a simplified interface for rendering USD stages
/// using Pixar's Hydra rendering framework. It wraps UsdImagingGLEngine
/// and provides common operations for scene rendering.
///
/// Example usage:
/// ```cpp
/// auto engine = HydraEngine::Create();
/// engine->SetRendererPlugin("HdStormRendererPlugin");
///
/// RenderSettings settings = RenderSettings::Preview();
/// CameraSettings camera = CameraSettings::LookAt(eye, target, up, ...);
///
/// engine->Render(stage.GetPseudoRoot(), settings, camera);
/// ```
class HydraEngine {
public:
    HydraEngine();
    HydraEngine(const HydraEngine& other);
    HydraEngine(HydraEngine&& other) noexcept;
    ~HydraEngine();

    HydraEngine& operator=(const HydraEngine& other);
    HydraEngine& operator=(HydraEngine&& other) noexcept;

    /// Check if the engine is valid and initialized
    bool IsValid() const;

    /// Create a new Hydra engine with default settings
    static HydraEngine Create();

    /// Create a Hydra engine with a specific renderer plugin
    static HydraEngine Create(const std::string& rendererPluginId, bool gpuEnabled = true);

    // -------------------------------------------------------------------------
    // Renderer Management
    // -------------------------------------------------------------------------

    /// Get list of available renderer plugins
    static std::vector<RendererInfo> GetAvailableRenderers();

    /// Get the current renderer plugin ID
    std::string GetCurrentRenderer() const;

    /// Set the renderer plugin to use
    /// Returns true if successful
    bool SetRendererPlugin(const std::string& pluginId);

    /// Check if GPU rendering is enabled
    bool IsGPUEnabled() const;

    // -------------------------------------------------------------------------
    // Rendering
    // -------------------------------------------------------------------------

    /// Render a USD prim (typically the stage root or pseudo-root)
    void Render(const usd::UsdPrim& root,
                const RenderSettings& settings);

    /// Render with explicit camera settings
    void Render(const usd::UsdPrim& root,
                const RenderSettings& settings,
                const CameraSettings& camera);

    /// Prepare a batch of geometry for rendering
    void PrepareBatch(const usd::UsdPrim& root,
                      const RenderSettings& settings);

    /// Render a batch of specific paths
    void RenderBatch(const std::vector<sdf::SdfPath>& paths,
                     const RenderSettings& settings);

    /// Check if the render has converged (for progressive renderers)
    bool IsConverged() const;

    // -------------------------------------------------------------------------
    // Camera Control
    // -------------------------------------------------------------------------

    /// Set the camera state using view and projection matrices
    void SetCameraState(const gf::Matrix4d& viewMatrix,
                        const gf::Matrix4d& projectionMatrix);

    /// Set the camera using a UsdGeomCamera path
    void SetCameraPath(const sdf::SdfPath& cameraPath);

    /// Set the render viewport dimensions
    void SetViewport(int x, int y, int width, int height);

    /// Set the render buffer size
    void SetRenderBufferSize(int width, int height);

    // -------------------------------------------------------------------------
    // Transform and Visibility
    // -------------------------------------------------------------------------

    /// Set the root transform for all rendered geometry
    void SetRootTransform(const gf::Matrix4d& transform);

    /// Set the root visibility
    void SetRootVisibility(bool visible);

    // -------------------------------------------------------------------------
    // Selection
    // -------------------------------------------------------------------------

    /// Set the selected prim paths for highlighting
    void SetSelected(const std::vector<sdf::SdfPath>& paths);

    /// Clear selection highlighting
    void ClearSelected();

    /// Set the selection highlight color
    void SetSelectionColor(const gf::Vec4f& color);

    // -------------------------------------------------------------------------
    // Render Control
    // -------------------------------------------------------------------------

    /// Check if the renderer supports pausing
    bool IsPauseSupported() const;

    /// Pause the renderer
    bool Pause();

    /// Resume the renderer
    bool Resume();

    /// Check if the renderer supports stopping
    bool IsStopSupported() const;

    /// Stop the renderer
    bool Stop();

    /// Restart the renderer
    bool Restart();

    // -------------------------------------------------------------------------
    // Presentation
    // -------------------------------------------------------------------------

    /// Enable or disable presentation to the bound framebuffer
    void SetEnablePresentation(bool enabled);

    // -------------------------------------------------------------------------
    // Implementation Access
    // -------------------------------------------------------------------------

    void* GetImpl() const { return impl_; }
    static HydraEngine FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// Utility Functions
// ============================================================================

/// Get the default renderer plugin ID
std::string GetDefaultRendererPluginId();

/// Check if color correction is supported on the current platform
bool IsColorCorrectionCapable();

} // namespace hydra
} // namespace cxxfacade
