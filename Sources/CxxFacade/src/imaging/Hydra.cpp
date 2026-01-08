// Hydra.cpp - High-level Hydra rendering implementation
#include "../../include/imaging/Hydra.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/pxr.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/vec4d.h>
#include <pxr/base/gf/vec4f.h>
#include <pxr/base/gf/frustum.h>
#include <pxr/base/tf/token.h>

// UsdImagingGL includes for the render engine
#include <pxr/usdImaging/usdImagingGL/engine.h>
#include <pxr/usdImaging/usdImagingGL/renderParams.h>

// Hdx includes for shadow control
#include <pxr/imaging/hdx/taskController.h>

#include <cmath>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace hydra {

// ============================================================================
// Helper Functions
// ============================================================================

/// Convert our DrawMode to pxr::UsdImagingGLDrawMode
static pxr::UsdImagingGLDrawMode toPxrDrawMode(DrawMode mode) {
    switch (mode) {
        case DrawMode::Points:
            return pxr::UsdImagingGLDrawMode::DRAW_POINTS;
        case DrawMode::Wireframe:
            return pxr::UsdImagingGLDrawMode::DRAW_WIREFRAME;
        case DrawMode::WireframeOnSurface:
            return pxr::UsdImagingGLDrawMode::DRAW_WIREFRAME_ON_SURFACE;
        case DrawMode::ShadedFlat:
            return pxr::UsdImagingGLDrawMode::DRAW_SHADED_FLAT;
        case DrawMode::ShadedSmooth:
            return pxr::UsdImagingGLDrawMode::DRAW_SHADED_SMOOTH;
        case DrawMode::GeomOnly:
            return pxr::UsdImagingGLDrawMode::DRAW_GEOM_ONLY;
        case DrawMode::GeomFlat:
            return pxr::UsdImagingGLDrawMode::DRAW_GEOM_FLAT;
        case DrawMode::GeomSmooth:
            return pxr::UsdImagingGLDrawMode::DRAW_GEOM_SMOOTH;
        default:
            return pxr::UsdImagingGLDrawMode::DRAW_SHADED_SMOOTH;
    }
}

/// Convert our CullStyle to pxr::UsdImagingGLCullStyle
static pxr::UsdImagingGLCullStyle toPxrCullStyle(CullStyle style) {
    switch (style) {
        case CullStyle::NoOpinion:
            return pxr::UsdImagingGLCullStyle::CULL_STYLE_NO_OPINION;
        case CullStyle::Nothing:
            return pxr::UsdImagingGLCullStyle::CULL_STYLE_NOTHING;
        case CullStyle::Back:
            return pxr::UsdImagingGLCullStyle::CULL_STYLE_BACK;
        case CullStyle::Front:
            return pxr::UsdImagingGLCullStyle::CULL_STYLE_FRONT;
        case CullStyle::BackUnlessDoubleSided:
            return pxr::UsdImagingGLCullStyle::CULL_STYLE_BACK_UNLESS_DOUBLE_SIDED;
        default:
            return pxr::UsdImagingGLCullStyle::CULL_STYLE_NOTHING;
    }
}

/// Convert our RenderSettings to pxr::UsdImagingGLRenderParams
static pxr::UsdImagingGLRenderParams toPxrRenderParams(const RenderSettings& settings) {
    pxr::UsdImagingGLRenderParams params;

    params.frame = pxr::UsdTimeCode(settings.frame);
    params.complexity = settings.complexity;
    params.drawMode = toPxrDrawMode(settings.drawMode);
    params.showGuides = settings.showGuides;
    params.showProxy = settings.showProxy;
    params.showRender = settings.showRender;
    params.forceRefresh = settings.forceRefresh;
    params.flipFrontFacing = settings.flipFrontFacing;
    params.cullStyle = toPxrCullStyle(settings.cullStyle);
    params.enableLighting = settings.enableLighting;
    params.enableSampleAlphaToCoverage = settings.enableSampleAlphaToCoverage;
    params.applyRenderState = settings.applyRenderState;
    params.gammaCorrectColors = settings.gammaCorrectColors;
    params.highlight = settings.highlight;
    params.overrideColor = pxr::GfVec4f(
        settings.overrideColor.x,
        settings.overrideColor.y,
        settings.overrideColor.z,
        settings.overrideColor.w);
    params.wireframeColor = pxr::GfVec4f(
        settings.wireframeColor.x,
        settings.wireframeColor.y,
        settings.wireframeColor.z,
        settings.wireframeColor.w);
    params.alphaThreshold = settings.alphaThreshold;
    params.enableSceneMaterials = settings.enableSceneMaterials;
    params.enableSceneLights = settings.enableSceneLights;
    params.enableUsdDrawModes = settings.enableUsdDrawModes;
    params.clearColor = pxr::GfVec4f(
        settings.clearColor.x,
        settings.clearColor.y,
        settings.clearColor.z,
        settings.clearColor.w);

    return params;
}

/// Convert gf::Matrix4d to pxr::GfMatrix4d
static pxr::GfMatrix4d toPxrMatrix4d(const gf::Matrix4d& m) {
    pxr::GfMatrix4d result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[row][col] = m.GetRow(row)[col];
        }
    }
    return result;
}

/// Convert gf::Vec3d to pxr::GfVec3d
static pxr::GfVec3d toPxrVec3d(const gf::Vec3d& v) {
    return pxr::GfVec3d(v.x, v.y, v.z);
}

/// Convert gf::Vec4f to pxr::GfVec4f
static pxr::GfVec4f toPxrVec4f(const gf::Vec4f& v) {
    return pxr::GfVec4f(v.x, v.y, v.z, v.w);
}

/// Convert pxr::GfMatrix4d to gf::Matrix4d
static gf::Matrix4d fromPxrMatrix4d(const pxr::GfMatrix4d& m) {
    return gf::Matrix4d(
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3]
    );
}

// ============================================================================
// ShadowEnabledEngine - Subclass to expose shadow control
// ============================================================================

/// Subclass of UsdImagingGLEngine that exposes shadow rendering control.
/// This is needed because _GetTaskController() is protected in the base class.
class ShadowEnabledEngine : public pxr::UsdImagingGLEngine {
public:
    // Inherit all constructors
    using UsdImagingGLEngine::UsdImagingGLEngine;

    /// Enable or disable shadow rendering via the internal task controller
    void SetEnableShadows(bool enable) {
        if (auto* tc = _GetTaskController()) {
            tc->SetEnableShadows(enable);
        }
    }
};

// ============================================================================
// CameraSettings Implementation
// ============================================================================

CameraSettings CameraSettings::Orthographic(double left, double right,
                                            double bottom, double top,
                                            double nearPlane, double farPlane) {
    CameraSettings settings;

    // Build orthographic projection matrix
    double width = right - left;
    double height = top - bottom;
    double depth = farPlane - nearPlane;

    pxr::GfMatrix4d proj;
    proj.SetIdentity();
    proj[0][0] = 2.0 / width;
    proj[1][1] = 2.0 / height;
    proj[2][2] = -2.0 / depth;
    proj[0][3] = -(right + left) / width;
    proj[1][3] = -(top + bottom) / height;
    proj[2][3] = -(farPlane + nearPlane) / depth;

    settings.projectionMatrix = fromPxrMatrix4d(proj);
    settings.viewMatrix = gf::Matrix4d::Identity();

    return settings;
}

CameraSettings CameraSettings::Perspective(double fovY, double aspectRatio,
                                           double nearPlane, double farPlane) {
    CameraSettings settings;

    // Build perspective projection matrix
    double tanHalfFov = std::tan(fovY * 0.5 * 3.14159265358979323846 / 180.0);
    double depth = farPlane - nearPlane;

    pxr::GfMatrix4d proj;
    proj.SetZero();
    proj[0][0] = 1.0 / (aspectRatio * tanHalfFov);
    proj[1][1] = 1.0 / tanHalfFov;
    proj[2][2] = -(farPlane + nearPlane) / depth;
    proj[2][3] = -2.0 * farPlane * nearPlane / depth;
    proj[3][2] = -1.0;

    settings.projectionMatrix = fromPxrMatrix4d(proj);
    settings.viewMatrix = gf::Matrix4d::Identity();

    return settings;
}

CameraSettings CameraSettings::LookAt(const gf::Vec3d& eye,
                                       const gf::Vec3d& target,
                                       const gf::Vec3d& up,
                                       double fovY, double aspectRatio,
                                       double nearPlane, double farPlane) {
    // Create perspective projection
    CameraSettings settings = Perspective(fovY, aspectRatio, nearPlane, farPlane);

    // Build look-at view matrix
    pxr::GfVec3d pxrEye = toPxrVec3d(eye);
    pxr::GfVec3d pxrTarget = toPxrVec3d(target);
    pxr::GfVec3d pxrUp = toPxrVec3d(up);

    pxr::GfVec3d forward = (pxrTarget - pxrEye).GetNormalized();
    pxr::GfVec3d right = pxr::GfCross(forward, pxrUp).GetNormalized();
    pxr::GfVec3d newUp = pxr::GfCross(right, forward);

    pxr::GfMatrix4d view;
    view.SetIdentity();
    view[0][0] = right[0];  view[0][1] = right[1];  view[0][2] = right[2];
    view[1][0] = newUp[0];  view[1][1] = newUp[1];  view[1][2] = newUp[2];
    view[2][0] = -forward[0]; view[2][1] = -forward[1]; view[2][2] = -forward[2];
    view[0][3] = -pxr::GfDot(right, pxrEye);
    view[1][3] = -pxr::GfDot(newUp, pxrEye);
    view[2][3] = pxr::GfDot(forward, pxrEye);

    settings.viewMatrix = fromPxrMatrix4d(view);

    return settings;
}

// ============================================================================
// HydraEngine Implementation
// ============================================================================

/// Internal implementation structure
struct HydraEngineHandle {
    std::unique_ptr<ShadowEnabledEngine> engine;
    bool gpuEnabled = true;
    std::string currentRenderer;

    HydraEngineHandle() = default;
    explicit HydraEngineHandle(std::unique_ptr<ShadowEnabledEngine> eng, bool gpu = true)
        : engine(std::move(eng)), gpuEnabled(gpu) {}
};

static inline HydraEngineHandle* asHandle(void* impl) {
    return static_cast<HydraEngineHandle*>(impl);
}

HydraEngine::HydraEngine() : impl_(new HydraEngineHandle()) {}

HydraEngine::HydraEngine(const HydraEngine& other) : impl_(nullptr) {
    // HydraEngine is not copyable in the traditional sense since
    // UsdImagingGLEngine is not copyable. Create a new empty handle.
    impl_ = new HydraEngineHandle();
}

HydraEngine::HydraEngine(HydraEngine&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

HydraEngine::~HydraEngine() {
    delete asHandle(impl_);
}

HydraEngine& HydraEngine::operator=(const HydraEngine& other) {
    if (this != &other) {
        delete asHandle(impl_);
        impl_ = new HydraEngineHandle();
    }
    return *this;
}

HydraEngine& HydraEngine::operator=(HydraEngine&& other) noexcept {
    if (this != &other) {
        delete asHandle(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool HydraEngine::IsValid() const {
    auto* handle = asHandle(impl_);
    return handle && handle->engine != nullptr;
}

HydraEngine HydraEngine::Create() {
    HydraEngine engine;
    auto* handle = asHandle(engine.impl_);

    try {
        handle->engine = std::make_unique<ShadowEnabledEngine>();
        handle->gpuEnabled = handle->engine->GetGPUEnabled();
    } catch (...) {
        // Engine creation failed, leave as invalid
    }

    return engine;
}

HydraEngine HydraEngine::Create(const std::string& rendererPluginId, bool gpuEnabled) {
    HydraEngine engine;
    auto* handle = asHandle(engine.impl_);

    try {
        pxr::HdDriver driver;
        pxr::TfToken pluginId = rendererPluginId.empty() ?
            pxr::TfToken() : pxr::TfToken(rendererPluginId);

        handle->engine = std::make_unique<ShadowEnabledEngine>(
            driver, pluginId, gpuEnabled);
        handle->gpuEnabled = gpuEnabled;
        handle->currentRenderer = rendererPluginId;
    } catch (...) {
        // Engine creation failed, leave as invalid
    }

    return engine;
}

std::vector<RendererInfo> HydraEngine::GetAvailableRenderers() {
    std::vector<RendererInfo> result;

    pxr::TfTokenVector plugins = pxr::UsdImagingGLEngine::GetRendererPlugins();
    for (const auto& plugin : plugins) {
        std::string displayName = pxr::UsdImagingGLEngine::GetRendererDisplayName(plugin);
        result.emplace_back(plugin.GetString(), displayName, true);
    }

    return result;
}

std::string HydraEngine::GetCurrentRenderer() const {
    if (!IsValid()) return "";

    auto* handle = asHandle(impl_);
    pxr::TfToken current = handle->engine->GetCurrentRendererId();
    return current.GetString();
}

bool HydraEngine::SetRendererPlugin(const std::string& pluginId) {
    if (!IsValid()) return false;

    auto* handle = asHandle(impl_);
    bool success = handle->engine->SetRendererPlugin(pxr::TfToken(pluginId));
    if (success) {
        handle->currentRenderer = pluginId;
    }
    return success;
}

bool HydraEngine::IsGPUEnabled() const {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->GetGPUEnabled();
}

void HydraEngine::Render(const usd::UsdPrim& root,
                          const RenderSettings& settings) {
    if (!IsValid() || !root.IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::UsdPrim* pxrPrim = static_cast<pxr::UsdPrim*>(root.GetImpl());
    pxr::UsdImagingGLRenderParams params = toPxrRenderParams(settings);

    handle->engine->Render(*pxrPrim, params);
}

void HydraEngine::Render(const usd::UsdPrim& root,
                          const RenderSettings& settings,
                          const CameraSettings& camera) {
    if (!IsValid() || !root.IsValid()) return;

    auto* handle = asHandle(impl_);

    // Set camera state
    pxr::GfMatrix4d viewMatrix = toPxrMatrix4d(camera.viewMatrix);
    pxr::GfMatrix4d projMatrix = toPxrMatrix4d(camera.projectionMatrix);
    handle->engine->SetCameraState(viewMatrix, projMatrix);

    // Set viewport
    handle->engine->SetRenderViewport(pxr::GfVec4d(
        camera.viewport.x,
        camera.viewport.y,
        camera.viewport.z,
        camera.viewport.w));

    // Set render buffer size
    handle->engine->SetRenderBufferSize(pxr::GfVec2i(camera.bufferWidth, camera.bufferHeight));

    // Render
    Render(root, settings);
}

void HydraEngine::PrepareBatch(const usd::UsdPrim& root,
                                const RenderSettings& settings) {
    if (!IsValid() || !root.IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::UsdPrim* pxrPrim = static_cast<pxr::UsdPrim*>(root.GetImpl());
    pxr::UsdImagingGLRenderParams params = toPxrRenderParams(settings);

    handle->engine->PrepareBatch(*pxrPrim, params);
}

void HydraEngine::RenderBatch(const std::vector<sdf::SdfPath>& paths,
                               const RenderSettings& settings) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::UsdImagingGLRenderParams params = toPxrRenderParams(settings);

    pxr::SdfPathVector pxrPaths;
    pxrPaths.reserve(paths.size());
    for (const auto& path : paths) {
        pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(path.GetImpl());
        if (pxrPath) {
            pxrPaths.push_back(*pxrPath);
        }
    }

    handle->engine->RenderBatch(pxrPaths, params);
}

bool HydraEngine::IsConverged() const {
    if (!IsValid()) return true;
    return asHandle(impl_)->engine->IsConverged();
}

void HydraEngine::SetCameraState(const gf::Matrix4d& viewMatrix,
                                  const gf::Matrix4d& projectionMatrix) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::GfMatrix4d view = toPxrMatrix4d(viewMatrix);
    pxr::GfMatrix4d proj = toPxrMatrix4d(projectionMatrix);
    handle->engine->SetCameraState(view, proj);
}

void HydraEngine::SetCameraPath(const sdf::SdfPath& cameraPath) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(cameraPath.GetImpl());
    if (pxrPath) {
        handle->engine->SetCameraPath(*pxrPath);
    }
}

void HydraEngine::SetViewport(int x, int y, int width, int height) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    handle->engine->SetRenderViewport(pxr::GfVec4d(
        static_cast<double>(x),
        static_cast<double>(y),
        static_cast<double>(width),
        static_cast<double>(height)));
}

void HydraEngine::SetRenderBufferSize(int width, int height) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    handle->engine->SetRenderBufferSize(pxr::GfVec2i(width, height));
}

void HydraEngine::SetRootTransform(const gf::Matrix4d& transform) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::GfMatrix4d xf = toPxrMatrix4d(transform);
    handle->engine->SetRootTransform(xf);
}

void HydraEngine::SetRootVisibility(bool visible) {
    if (!IsValid()) return;
    asHandle(impl_)->engine->SetRootVisibility(visible);
}

void HydraEngine::SetSelected(const std::vector<sdf::SdfPath>& paths) {
    if (!IsValid()) return;

    auto* handle = asHandle(impl_);
    pxr::SdfPathVector pxrPaths;
    pxrPaths.reserve(paths.size());
    for (const auto& path : paths) {
        pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(path.GetImpl());
        if (pxrPath) {
            pxrPaths.push_back(*pxrPath);
        }
    }
    handle->engine->SetSelected(pxrPaths);
}

void HydraEngine::ClearSelected() {
    if (!IsValid()) return;
    asHandle(impl_)->engine->ClearSelected();
}

void HydraEngine::SetSelectionColor(const gf::Vec4f& color) {
    if (!IsValid()) return;
    asHandle(impl_)->engine->SetSelectionColor(toPxrVec4f(color));
}

bool HydraEngine::IsPauseSupported() const {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->IsPauseRendererSupported();
}

bool HydraEngine::Pause() {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->PauseRenderer();
}

bool HydraEngine::Resume() {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->ResumeRenderer();
}

bool HydraEngine::IsStopSupported() const {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->IsStopRendererSupported();
}

bool HydraEngine::Stop() {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->StopRenderer();
}

bool HydraEngine::Restart() {
    if (!IsValid()) return false;
    return asHandle(impl_)->engine->RestartRenderer();
}

void HydraEngine::SetEnablePresentation(bool enabled) {
    if (!IsValid()) return;
    asHandle(impl_)->engine->SetEnablePresentation(enabled);
}

void HydraEngine::SetEnableShadows(bool enable) {
    if (!IsValid()) return;
    asHandle(impl_)->engine->SetEnableShadows(enable);
}

HydraEngine HydraEngine::FromImpl(void* impl) {
    HydraEngine engine;
    delete asHandle(engine.impl_);
    engine.impl_ = impl;
    return engine;
}

// ============================================================================
// Utility Functions
// ============================================================================

std::string GetDefaultRendererPluginId() {
    // Try to get available renderers and return the first one
    pxr::TfTokenVector plugins = pxr::UsdImagingGLEngine::GetRendererPlugins();
    if (!plugins.empty()) {
        return plugins[0].GetString();
    }
    return "";
}

bool IsColorCorrectionCapable() {
    return pxr::UsdImagingGLEngine::IsColorCorrectionCapable();
}

// ============================================================================
// Shadow-Enabled Engine Access Implementation
// ============================================================================

/// Internal implementation for shadow engine handle
struct ShadowEngineImpl {
    std::unique_ptr<ShadowEnabledEngine> engine;

    explicit ShadowEngineImpl(std::unique_ptr<ShadowEnabledEngine> eng)
        : engine(std::move(eng)) {}
};

ShadowEngineHandle* CreateShadowEngine(bool gpuEnabled) {
    try {
        auto engine = std::make_unique<ShadowEnabledEngine>(
            pxr::HdDriver(), pxr::TfToken(), gpuEnabled);
        auto handle = new ShadowEngineHandle();
        handle->impl = new ShadowEngineImpl(std::move(engine));
        return handle;
    } catch (...) {
        return nullptr;
    }
}

ShadowEngineHandle* CreateShadowEngineWithRenderer(const std::string& rendererPluginId, bool gpuEnabled) {
    try {
        pxr::TfToken pluginId = rendererPluginId.empty() ?
            pxr::TfToken() : pxr::TfToken(rendererPluginId);
        auto engine = std::make_unique<ShadowEnabledEngine>(
            pxr::HdDriver(), pluginId, gpuEnabled);
        auto handle = new ShadowEngineHandle();
        handle->impl = new ShadowEngineImpl(std::move(engine));
        return handle;
    } catch (...) {
        return nullptr;
    }
}

ShadowEngineHandle* CreateShadowEngineWithDriver(
    void* driverPtr,
    const std::string& rootPath,
    const std::string& rendererPluginId,
    bool gpuEnabled)
{
    try {
        // Cast the driver pointer back to HdDriver
        pxr::HdDriver* driver = static_cast<pxr::HdDriver*>(driverPtr);
        if (!driver) {
            return nullptr;
        }

        pxr::TfToken pluginId = rendererPluginId.empty() ?
            pxr::TfToken() : pxr::TfToken(rendererPluginId);

        pxr::SdfPath root = rootPath.empty() ?
            pxr::SdfPath::AbsoluteRootPath() : pxr::SdfPath(rootPath);

        // Create engine with the provided driver and settings
        auto engine = std::make_unique<ShadowEnabledEngine>(
            root,
            pxr::SdfPathVector(),  // excludedPaths
            pxr::SdfPathVector(),  // invisedPaths
            pxr::SdfPath::AbsoluteRootPath(),  // sceneDelegateID
            *driver,
            pluginId,
            gpuEnabled);

        auto handle = new ShadowEngineHandle();
        handle->impl = new ShadowEngineImpl(std::move(engine));
        return handle;
    } catch (...) {
        return nullptr;
    }
}

void DestroyShadowEngine(ShadowEngineHandle* handle) {
    if (handle) {
        delete static_cast<ShadowEngineImpl*>(handle->impl);
        delete handle;
    }
}

void SetShadowEngineEnableShadows(ShadowEngineHandle* handle, bool enable) {
    if (handle && handle->impl) {
        auto* impl = static_cast<ShadowEngineImpl*>(handle->impl);
        if (impl->engine) {
            impl->engine->SetEnableShadows(enable);
        }
    }
}

void* GetShadowEngineRawPointer(ShadowEngineHandle* handle) {
    if (handle && handle->impl) {
        auto* impl = static_cast<ShadowEngineImpl*>(handle->impl);
        if (impl->engine) {
            return static_cast<pxr::UsdImagingGLEngine*>(impl->engine.get());
        }
    }
    return nullptr;
}

// ============================================================================
// Raw Engine Shadow Control Implementation
// ============================================================================

/// Helper class to access protected _GetTaskController() on any UsdImagingGLEngine.
/// This uses the "accessor pattern" - a derived class that exposes protected members.
class TaskControllerAccessor : public pxr::UsdImagingGLEngine {
public:
    // Get the task controller from any UsdImagingGLEngine pointer.
    // This works because _GetTaskController() is protected (accessible to derived classes)
    // and we're using static_cast which doesn't change the object's memory layout.
    static pxr::HdxTaskController* GetTaskController(pxr::UsdImagingGLEngine* engine) {
        // Cast to accessor to access protected member
        return static_cast<TaskControllerAccessor*>(engine)->_GetTaskController();
    }
};

void EnableShadowsOnRawEngine(void* enginePtr, bool enable) {
    if (!enginePtr) return;

    auto* engine = static_cast<pxr::UsdImagingGLEngine*>(enginePtr);
    if (auto* tc = TaskControllerAccessor::GetTaskController(engine)) {
        tc->SetEnableShadows(enable);
    }
}

} // namespace hydra
} // namespace cxxfacade
