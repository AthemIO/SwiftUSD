// UsdAppUtils.h - USD Application Utilities for SwiftUSD
// Mirrors: pxr/usdImaging/usdAppUtils/
// C++ header with Swift interop annotations
//
// UsdAppUtils provides utility classes for USD applications:
// - UsdAppUtilsFrameRecorder: Records images of USD stages using Hydra
// - UsdAppUtilsGetCameraAtPath: Camera lookup utility function

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../usd/Sdf.h"
#include "../usd/Usd.h"
#include "../usd/UsdGeom.h"
#include "Hd.h"
#include "UsdImaging.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usdImaging/usdAppUtils/frameRecorder.h"
#include "pxr/usdImaging/usdAppUtils/camera.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class UsdAppUtilsFrameRecorderRef;

// ============================================================================
// MARK: - UsdAppUtilsColorCorrectionMode (Configuration enum)
// ============================================================================

/// Color correction modes for frame recording.
/// These determine how the rendered image is color corrected before output.
struct UsdAppUtilsColorCorrectionMode {
    /// No color correction applied.
    static Token Disabled() SWIFTUSD_NOEXCEPT { return Token("disabled"); }

    /// sRGB color space conversion.
    static Token sRGB() SWIFTUSD_NOEXCEPT { return Token("sRGB"); }

    /// OpenColorIO configuration.
    static Token OpenColorIO() SWIFTUSD_NOEXCEPT { return Token("openColorIO"); }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - UsdAppUtilsFrameRecorderParams (Configuration parameters)
// ============================================================================

/// Configuration parameters for UsdAppUtilsFrameRecorder.
/// These control various aspects of image recording.
struct UsdAppUtilsFrameRecorderParams {
    /// Width of the recorded image in pixels.
    /// Height is computed from camera aspect ratio.
    /// Default is 960 pixels.
    size_t imageWidth;

    /// Level of refinement complexity (0.0 to 1.0+).
    /// Default is 1.0 ("low" complexity).
    float complexity;

    /// Whether to enable the camera headlight.
    bool cameraLightEnabled;

    /// Whether dome lights should be visible in the background.
    bool domeLightsVisible;

    /// Whether to enable GPU rendering.
    bool gpuEnabled;

    /// Whether to respect USD draw modes.
    bool enableUsdDrawModes;

    UsdAppUtilsFrameRecorderParams() SWIFTUSD_NOEXCEPT
        : imageWidth(960)
        , complexity(1.0f)
        , cameraLightEnabled(true)
        , domeLightsVisible(true)
        , gpuEnabled(true)
        , enableUsdDrawModes(true) {}

    bool operator==(const UsdAppUtilsFrameRecorderParams& rhs) const SWIFTUSD_NOEXCEPT {
        return imageWidth == rhs.imageWidth &&
               complexity == rhs.complexity &&
               cameraLightEnabled == rhs.cameraLightEnabled &&
               domeLightsVisible == rhs.domeLightsVisible &&
               gpuEnabled == rhs.gpuEnabled &&
               enableUsdDrawModes == rhs.enableUsdDrawModes;
    }

    bool operator!=(const UsdAppUtilsFrameRecorderParams& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - UsdAppUtilsFrameRecorderRef (Frame recorder wrapper)
// ============================================================================

/// A utility class for recording images of USD stages.
///
/// UsdAppUtilsFrameRecorder uses Hydra to produce recorded images of a USD
/// stage looking through a particular UsdGeomCamera on that stage at a
/// particular time code. The images generated will be effectively the same
/// as what you would see in the viewer in usdview.
///
/// Note that it is assumed that an OpenGL context has already been setup for
/// the UsdAppUtilsFrameRecorder if OpenGL is being used as the underlying HGI
/// device. This is not required for Metal or Vulkan.
class UsdAppUtilsFrameRecorderRef {
public:
    /// Create a new UsdAppUtilsFrameRecorder.
    ///
    /// @param rendererPluginId The renderer plugin that Hydra should use.
    ///        If empty, a default renderer will be chosen based on gpuEnabled.
    /// @param gpuEnabled Whether to allow Hydra to use the GPU.
    /// @param enableUsdDrawModes Whether to respect USD draw modes.
    /// @return A new frame recorder reference, or nullptr if creation fails.
    static UsdAppUtilsFrameRecorderRef* Create(
            const Token& rendererPluginId = Token(),
            bool gpuEnabled = true,
            bool enableUsdDrawModes = true) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto* recorder = new PXR_NS::UsdAppUtilsFrameRecorder(
            rendererPluginId.GetTfToken(),
            gpuEnabled,
            enableUsdDrawModes);
        if (!recorder) return nullptr;
        return new UsdAppUtilsFrameRecorderRef(recorder, gpuEnabled, enableUsdDrawModes);
#else
        return new UsdAppUtilsFrameRecorderRef(rendererPluginId, gpuEnabled, enableUsdDrawModes);
#endif
    }

    // ========================================================================
    // MARK: Renderer Configuration
    // ========================================================================

    /// Gets the ID of the Hydra renderer plugin that will be used for recording.
    Token GetCurrentRendererId() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            return Token(_recorder->GetCurrentRendererId().GetString());
        }
        return Token();
#else
        return _rendererPluginId;
#endif
    }

    /// Sets the Hydra renderer plugin to be used for recording.
    ///
    /// Note that the renderer plugins that may be set will be restricted if
    /// this UsdAppUtilsFrameRecorder instance has disabled the GPU.
    ///
    /// @param id The renderer plugin ID to use.
    /// @return true if the renderer was set successfully, false otherwise.
    bool SetRendererPlugin(const Token& id) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            return _recorder->SetRendererPlugin(id.GetTfToken());
        }
        return false;
#else
        _rendererPluginId = id;
        return true;
#endif
    }

    // ========================================================================
    // MARK: Render Pass/Settings Configuration
    // ========================================================================

    /// Sets the path to the render pass prim to use.
    ///
    /// If there is a render settings prim designated by the render pass prim
    /// via renderSource, it must also be set with SetActiveRenderSettingsPrimPath().
    void SetActiveRenderPassPrimPath(const Path& path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetActiveRenderPassPrimPath(path.GetSdfPath());
        }
#else
        _renderPassPrimPath = path;
#endif
    }

    /// Sets the path to the render settings prim to use.
    void SetActiveRenderSettingsPrimPath(const Path& path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetActiveRenderSettingsPrimPath(path.GetSdfPath());
        }
#else
        _renderSettingsPrimPath = path;
#endif
    }

    // ========================================================================
    // MARK: Image Configuration
    // ========================================================================

    /// Sets the width of the recorded image.
    ///
    /// The height of the recorded image will be computed using this value and
    /// the aspect ratio of the camera used for recording.
    ///
    /// The default image width is 960 pixels.
    ///
    /// @param imageWidth The desired width in pixels. Must be greater than 0.
    void SetImageWidth(size_t imageWidth) SWIFTUSD_NOEXCEPT {
        if (imageWidth == 0) return;
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetImageWidth(imageWidth);
        }
#else
        _imageWidth = imageWidth;
#endif
    }

    /// Gets the current image width setting.
    size_t GetImageWidth() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _imageWidth;
#else
        return _imageWidth;
#endif
    }

    /// Sets the level of refinement complexity.
    ///
    /// The default complexity is "low" (1.0).
    ///
    /// @param complexity The complexity level (typically 1.0 to 2.0).
    void SetComplexity(float complexity) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetComplexity(complexity);
        }
#else
        _complexity = complexity;
#endif
    }

    /// Gets the current complexity setting.
    float GetComplexity() const SWIFTUSD_NOEXCEPT {
        return _complexity;
    }

    /// Sets the color correction mode to be used for recording.
    ///
    /// By default, color correction is disabled.
    ///
    /// @param colorCorrectionMode The color correction mode token.
    void SetColorCorrectionMode(const Token& colorCorrectionMode) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetColorCorrectionMode(colorCorrectionMode.GetTfToken());
        }
#else
        _colorCorrectionMode = colorCorrectionMode;
#endif
    }

    /// Gets the current color correction mode.
    Token GetColorCorrectionMode() const SWIFTUSD_NOEXCEPT {
        return _colorCorrectionMode;
    }

    // ========================================================================
    // MARK: Lighting Configuration
    // ========================================================================

    /// Turns the built-in camera light on or off.
    ///
    /// When on, this will add a light at the camera's origin.
    /// This is sometimes called a "headlight".
    ///
    /// @param enabled Whether to enable the camera light.
    void SetCameraLightEnabled(bool enabled) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetCameraLightEnabled(enabled);
        }
#else
        _cameraLightEnabled = enabled;
#endif
    }

    /// Gets whether the camera light is enabled.
    bool GetCameraLightEnabled() const SWIFTUSD_NOEXCEPT {
        return _cameraLightEnabled;
    }

    /// Sets the camera visibility of dome lights.
    ///
    /// When on, dome light textures will be drawn to the background as if
    /// mapped onto a sphere infinitely far away.
    ///
    /// @param visible Whether dome lights should be visible in the background.
    void SetDomeLightVisibility(bool visible) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetDomeLightVisibility(visible);
        }
#else
        _domeLightsVisible = visible;
#endif
    }

    /// Gets whether dome lights are visible.
    bool GetDomeLightVisibility() const SWIFTUSD_NOEXCEPT {
        return _domeLightsVisible;
    }

    // ========================================================================
    // MARK: Purpose Configuration
    // ========================================================================

    /// Sets the UsdGeomImageable purposes to be used for rendering.
    ///
    /// We will always include "default" purpose. By default,
    /// we will also include "proxy" purpose. Use this method
    /// to explicitly enumerate an alternate set of purposes to be
    /// included along with "default".
    ///
    /// @param purposes Array of purpose tokens.
    /// @param count Number of tokens in the array.
    void SetIncludedPurposes(const Token* purposes, size_t count) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder && purposes && count > 0) {
            PXR_NS::TfTokenVector purposeVec;
            purposeVec.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                purposeVec.push_back(purposes[i].GetTfToken());
            }
            _recorder->SetIncludedPurposes(purposeVec);
        }
#else
        _purposes.clear();
        if (purposes && count > 0) {
            _purposes.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                _purposes.push_back(purposes[i]);
            }
        }
#endif
    }

    /// Gets the current included purposes.
    std::vector<Token> GetIncludedPurposes() const SWIFTUSD_NOEXCEPT {
        return _purposes;
    }

    // ========================================================================
    // MARK: Camera Configuration
    // ========================================================================

    /// Sets the primary camera prim path.
    ///
    /// @param cameraPath Path to the camera prim.
    void SetPrimaryCameraPrimPath(const Path& cameraPath) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            _recorder->SetPrimaryCameraPrimPath(cameraPath.GetSdfPath());
        }
#else
        _cameraPrimPath = cameraPath;
#endif
    }

    /// Gets the primary camera prim path.
    Path GetPrimaryCameraPrimPath() const SWIFTUSD_NOEXCEPT {
        return _cameraPrimPath;
    }

    // ========================================================================
    // MARK: Recording
    // ========================================================================

    /// Records an image and writes the result to the specified output path.
    ///
    /// The recorded image will represent the view from the specified camera
    /// looking at the imageable prims on the USD stage at the specified time code.
    ///
    /// If the camera is not valid, a camera will be computed to automatically
    /// frame the stage geometry.
    ///
    /// When using a RenderSettings prim, the generated image will be written
    /// to the file indicated on the connected RenderProducts, instead of the
    /// given outputImagePath.
    ///
    /// @param stage The USD stage to record.
    /// @param cameraPath Path to the camera prim to use.
    /// @param timeCode The time code to evaluate.
    /// @param outputImagePath Path where the output image will be written.
    /// @return true if the image was generated and written successfully.
    bool Record(StageRef* stage,
                const Path& cameraPath,
                double timeCode,
                const char* outputImagePath) SWIFTUSD_NOEXCEPT {
        if (!stage || !outputImagePath) return false;
#if defined(USE_PIXAR_USD)
        if (_recorder) {
            auto* pxrStage = stage->GetPxrStage();
            if (!pxrStage) return false;

            PXR_NS::UsdGeomCamera camera = PXR_NS::UsdGeomCamera::Get(
                pxrStage, cameraPath.GetSdfPath());

            return _recorder->Record(
                pxrStage,
                camera,
                PXR_NS::UsdTimeCode(timeCode),
                std::string(outputImagePath));
        }
        return false;
#else
        // Fallback: Recording not supported without Pixar USD
        (void)cameraPath;
        (void)timeCode;
        return false;
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
    UsdAppUtilsFrameRecorderRef(PXR_NS::UsdAppUtilsFrameRecorder* recorder,
                                 bool gpuEnabled,
                                 bool enableUsdDrawModes) SWIFTUSD_NOEXCEPT
        : _recorder(recorder)
        , _imageWidth(960)
        , _complexity(1.0f)
        , _cameraLightEnabled(true)
        , _domeLightsVisible(true)
        , _gpuEnabled(gpuEnabled)
        , _enableUsdDrawModes(enableUsdDrawModes)
        , _refCount(1) {}

    ~UsdAppUtilsFrameRecorderRef() {
        delete _recorder;
    }

    PXR_NS::UsdAppUtilsFrameRecorder* _recorder;
#else
    UsdAppUtilsFrameRecorderRef(const Token& rendererPluginId,
                                 bool gpuEnabled,
                                 bool enableUsdDrawModes) SWIFTUSD_NOEXCEPT
        : _rendererPluginId(rendererPluginId)
        , _imageWidth(960)
        , _complexity(1.0f)
        , _colorCorrectionMode(UsdAppUtilsColorCorrectionMode::Disabled())
        , _cameraLightEnabled(true)
        , _domeLightsVisible(true)
        , _gpuEnabled(gpuEnabled)
        , _enableUsdDrawModes(enableUsdDrawModes)
        , _refCount(1) {
        // Default purposes: default and proxy
        _purposes.push_back(Token("default"));
        _purposes.push_back(Token("proxy"));
    }

    ~UsdAppUtilsFrameRecorderRef() {}

    Token _rendererPluginId;
    Path _renderPassPrimPath;
    Path _renderSettingsPrimPath;
    Path _cameraPrimPath;
    std::vector<Token> _purposes;
#endif

    size_t _imageWidth;
    float _complexity;
    Token _colorCorrectionMode;
    bool _cameraLightEnabled;
    bool _domeLightsVisible;
    bool _gpuEnabled;
    bool _enableUsdDrawModes;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - UsdAppUtils Tokens
// ============================================================================

/// Common tokens used in UsdAppUtils.
namespace UsdAppUtilsTokens {

/// Color correction mode tokens
inline Token ColorCorrectionDisabled() { return Token("disabled"); }
inline Token ColorCorrectionSRGB() { return Token("sRGB"); }
inline Token ColorCorrectionOpenColorIO() { return Token("openColorIO"); }

/// Purpose tokens
inline Token PurposeDefault() { return Token("default"); }
inline Token PurposeProxy() { return Token("proxy"); }
inline Token PurposeRender() { return Token("render"); }
inline Token PurposeGuide() { return Token("guide"); }

/// Common renderer IDs
inline Token RendererStorm() { return Token("HdStormRendererPlugin"); }
inline Token RendererEmbree() { return Token("HdEmbreeRendererPlugin"); }
inline Token RendererPrman() { return Token("HdPrmanRendererPlugin"); }

} // namespace UsdAppUtilsTokens

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Gets the UsdGeomCamera matching the specified path from the USD stage.
///
/// If cameraPath is an absolute path, this is equivalent to UsdGeomCamera::Get().
/// Otherwise, if cameraPath is a single-element path representing just the name
/// of a camera prim, then the stage will be searched looking for a UsdGeomCamera
/// matching that name.
///
/// @param stage The USD stage to search.
/// @param cameraPath Path to the camera (absolute or just the camera name).
/// @return The camera at the specified path, or nullptr if not found.
inline GeomCameraRef* UsdAppUtilsGetCameraAtPath(StageRef* stage,
                                                  const Path& cameraPath) SWIFTUSD_NOEXCEPT {
    if (!stage) return nullptr;
    if (cameraPath.IsEmpty()) return nullptr;

#if defined(USE_PIXAR_USD)
    auto* pxrStage = stage->GetPxrStage();
    if (!pxrStage) return nullptr;

    PXR_NS::UsdGeomCamera camera = PXR_NS::UsdAppUtilsGetCameraAtPath(
        pxrStage, cameraPath.GetSdfPath());

    if (!camera) return nullptr;

    // Get the prim from the found camera and create a reference
    PXR_NS::UsdPrim prim = camera.GetPrim();
    if (!prim) return nullptr;

    PrimRef* primRef = new PrimRef(prim, stage);
    GeomCameraRef* camRef = GeomCameraRef::FromPrim(primRef);
    primRef->release();
    return camRef;
#else
    // Fallback: Try to get camera directly at the path
    // In standalone mode, we don't have search functionality

    // Get the prim at the path and try to create a camera from it
    PrimRef* prim = stage->GetPrimAtPath(cameraPath);
    if (!prim) return nullptr;

    // Check if it's a camera type
    Token primType = prim->GetTypeName();
    if (primType != Token("Camera")) {
        prim->release();
        return nullptr;
    }

    GeomCameraRef* camRef = GeomCameraRef::FromPrim(prim);
    prim->release();
    return camRef;
#endif
}

/// Check if a path represents a valid camera prim type.
///
/// @param primTypeName The type name token of the prim.
/// @return true if the prim type is a camera type.
inline bool UsdAppUtilsIsCameraPrimType(const Token& primTypeName) SWIFTUSD_NOEXCEPT {
    return primTypeName == Token("Camera");
}

} // namespace swiftusd
