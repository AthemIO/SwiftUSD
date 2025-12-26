// HydraEngine.cpp - Hydra rendering implementation
// This file includes real USD headers - NOT visible to Swift

#include "../include/HydraEngine.hpp"
#include "../include/Stage.hpp"

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usdImaging/usdImagingGL/engine.h>
#include <pxr/imaging/hd/engine.h>
#include <pxr/imaging/hd/renderIndex.h>
#include <pxr/imaging/hd/rprimCollection.h>
#include <pxr/imaging/glf/drawTarget.h>
#include <pxr/base/gf/camera.h>
#include <pxr/base/gf/frustum.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/vec4d.h>

#include <sstream>
#include <vector>

PXR_NAMESPACE_USING_DIRECTIVE

namespace pixarusd {

// ============================================================================
// MARK: - HydraEngine Implementation
// ============================================================================

namespace detail {

struct HydraEngineImpl {
    RenderSettings settings;
    Stage* stage{nullptr};
    GfMatrix4d viewMatrix;
    GfMatrix4d projMatrix;
    double timeCode{0.0};

    std::unique_ptr<UsdImagingGLEngine> engine;
    std::vector<uint8_t> pixelBuffer;

    HydraEngineImpl(const RenderSettings& s) : settings(s) {
        pixelBuffer.resize(s.width * s.height * 4, 0);
    }

    ~HydraEngineImpl() {
        if (stage) {
            releaseStage(stage);
        }
    }

    void EnsureEngine() {
        if (!engine && stage) {
            void* rawStage = stage->GetRawStagePtr();
            if (rawStage) {
                UsdStagePtr usdStage = TfDynamic_cast<UsdStageRefPtr>(
                    *static_cast<UsdStageRefPtr*>(rawStage)
                );
                if (usdStage) {
                    engine = std::make_unique<UsdImagingGLEngine>();
                }
            }
        }
    }
};

} // namespace detail

HydraEngine::HydraEngine(const RenderSettings& settings)
    : impl_(new detail::HydraEngineImpl(settings)) {}

HydraEngine::~HydraEngine() {
    delete impl_;
}

HydraEngine* HydraEngine::Create(const RenderSettings& settings) {
    return new HydraEngine(settings);
}

void HydraEngine::SetStage(Stage* stage) {
    if (impl_->stage) {
        releaseStage(impl_->stage);
    }
    impl_->stage = stage;
    if (impl_->stage) {
        retainStage(impl_->stage);
    }
    // Invalidate engine so it gets recreated with new stage
    impl_->engine.reset();
}

void HydraEngine::SetCamera(const Matrix4d& viewMatrix, const Matrix4d& projMatrix) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            impl_->viewMatrix[row][col] = viewMatrix.m[row][col];
            impl_->projMatrix[row][col] = projMatrix.m[row][col];
        }
    }
}

void HydraEngine::SetCameraSettings(const CameraSettings& settings) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            impl_->viewMatrix[row][col] = settings.viewMatrix.m[row][col];
            impl_->projMatrix[row][col] = settings.projectionMatrix.m[row][col];
        }
    }
}

void HydraEngine::SetTime(TimeCode time) {
    impl_->timeCode = time.value;
}

void HydraEngine::Render() {
    impl_->EnsureEngine();

    if (!impl_->engine || !impl_->stage) {
        // Fill with a test pattern if no engine
        for (uint32_t y = 0; y < impl_->settings.height; ++y) {
            for (uint32_t x = 0; x < impl_->settings.width; ++x) {
                size_t offset = (y * impl_->settings.width + x) * 4;
                // Gradient pattern to show something is rendering
                impl_->pixelBuffer[offset + 0] = static_cast<uint8_t>(x * 255 / impl_->settings.width);
                impl_->pixelBuffer[offset + 1] = static_cast<uint8_t>(y * 255 / impl_->settings.height);
                impl_->pixelBuffer[offset + 2] = 128;
                impl_->pixelBuffer[offset + 3] = 255;
            }
        }
        return;
    }

    // Get the USD stage
    void* rawStage = impl_->stage->GetRawStagePtr();
    if (!rawStage) return;

    UsdStagePtr usdStage = *static_cast<UsdStageRefPtr*>(rawStage);

    // Set up render parameters
    UsdImagingGLRenderParams params;
    params.frame = impl_->timeCode;
    params.complexity = impl_->settings.complexityLevel;
    params.enableLighting = impl_->settings.enableLighting;
    params.enableSceneMaterials = true;
    params.clearColor = GfVec4f(0.1f, 0.1f, 0.1f, 1.0f);

    // Set up viewport
    GfVec4d viewport(0, 0, impl_->settings.width, impl_->settings.height);

    // Render the scene
    impl_->engine->SetRenderViewport(viewport);
    impl_->engine->SetCameraState(impl_->viewMatrix, impl_->projMatrix);

    SdfPath rootPath = SdfPath::AbsoluteRootPath();
    impl_->engine->Render(usdStage->GetPseudoRoot(), params);

    // Read back pixels
    // Note: In a real implementation, you'd use FBO to render to texture
    // and then read back. This is a simplified version.
    std::fill(impl_->pixelBuffer.begin(), impl_->pixelBuffer.end(), 0);

    // For now, render a placeholder - real implementation would
    // use OpenGL/Metal to capture the rendered frame
    for (uint32_t y = 0; y < impl_->settings.height; ++y) {
        for (uint32_t x = 0; x < impl_->settings.width; ++x) {
            size_t offset = (y * impl_->settings.width + x) * 4;
            impl_->pixelBuffer[offset + 0] = 50;  // R
            impl_->pixelBuffer[offset + 1] = 80;  // G
            impl_->pixelBuffer[offset + 2] = 120; // B
            impl_->pixelBuffer[offset + 3] = 255; // A
        }
    }
}

bool HydraEngine::IsRenderComplete() const {
    // For synchronous rendering, always complete after Render()
    return true;
}

const uint8_t* HydraEngine::GetPixelData() const {
    return impl_->pixelBuffer.data();
}

uint32_t HydraEngine::GetWidth() const {
    return impl_->settings.width;
}

uint32_t HydraEngine::GetHeight() const {
    return impl_->settings.height;
}

size_t HydraEngine::GetPixelDataSize() const {
    return impl_->pixelBuffer.size();
}

void HydraEngine::Resize(uint32_t width, uint32_t height) {
    impl_->settings.width = width;
    impl_->settings.height = height;
    impl_->pixelBuffer.resize(width * height * 4, 0);
}

void HydraEngine::SetRenderSettings(const RenderSettings& settings) {
    impl_->settings = settings;
    impl_->pixelBuffer.resize(settings.width * settings.height * 4, 0);
}

RenderSettings HydraEngine::GetRenderSettings() const {
    return impl_->settings;
}

std::string HydraEngine::GetRenderStats() const {
    std::ostringstream ss;
    ss << "Width: " << impl_->settings.width << "\n";
    ss << "Height: " << impl_->settings.height << "\n";
    ss << "Lighting: " << (impl_->settings.enableLighting ? "On" : "Off") << "\n";
    ss << "Shadows: " << (impl_->settings.enableShadows ? "On" : "Off") << "\n";
    ss << "Complexity: " << impl_->settings.complexityLevel << "\n";
    ss << "Engine: " << (impl_->engine ? "Initialized" : "Not initialized") << "\n";
    ss << "Stage: " << (impl_->stage ? "Set" : "Not set") << "\n";
    return ss.str();
}

} // namespace pixarusd

// Global retain/release functions for Swift interop
void retainHydraEngine(pixarusd::HydraEngine* e) {
    if (e) {
        e->refCount_.fetch_add(1, std::memory_order_relaxed);
    }
}

void releaseHydraEngine(pixarusd::HydraEngine* e) {
    if (e && e->refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete e;
    }
}
