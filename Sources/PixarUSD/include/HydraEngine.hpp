// HydraEngine.hpp - Hydra rendering facade for Swift/C++ interop
// Clean header - NO USD includes, uses pimpl pattern

#pragma once

// Include swift/bridging only when Swift is importing (not pure C++)
// __swift__ is defined by Swift's ClangImporter
#ifdef __swift__
#include <swift/bridging>
#else
// Fallback for pure C++ compilation - empty macros
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_RETURNS_RETAINED
#define SWIFT_SELF_CONTAINED
#endif

#include <cstdint>
#include <atomic>
#include "Types.hpp"

namespace pixarusd {
namespace detail { struct HydraEngineImpl; }
class Stage;
class HydraEngine;
} // namespace pixarusd

// Retain/release functions at global scope for Swift interop
void retainHydraEngine(pixarusd::HydraEngine* e);
void releaseHydraEngine(pixarusd::HydraEngine* e);

namespace pixarusd {

// ============================================================================
// MARK: - Render Settings
// ============================================================================

struct RenderSettings {
    uint32_t width{1920};
    uint32_t height{1080};
    bool enableLighting{true};
    bool enableShadows{true};
    float complexityLevel{1.0f};  // LOD: 0.0 = lowest, 1.0 = highest

    RenderSettings() = default;
    RenderSettings(uint32_t w, uint32_t h)
        : width(w), height(h) {}
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Camera Settings
// ============================================================================

struct CameraSettings {
    Matrix4d viewMatrix;
    Matrix4d projectionMatrix;
    float nearClip{0.1f};
    float farClip{10000.0f};
    float fov{45.0f};  // Field of view in degrees

    CameraSettings() = default;
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Hydra Engine
// ============================================================================

/// High-level Hydra rendering facade.
/// Provides a simple API for rendering USD stages to pixel buffers.
class HydraEngine {
    detail::HydraEngineImpl* impl_;
    mutable std::atomic<int> refCount_{1};

public:
    explicit HydraEngine(const RenderSettings& settings);
    ~HydraEngine();

    // Non-copyable
    HydraEngine(const HydraEngine&) = delete;
    HydraEngine& operator=(const HydraEngine&) = delete;

    // ========================================================================
    // Factory
    // ========================================================================

    /// Create a new HydraEngine with the given settings.
    static HydraEngine* Create(const RenderSettings& settings) SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Setup
    // ========================================================================

    /// Set the stage to render.
    void SetStage(Stage* stage);

    /// Set camera matrices directly.
    void SetCamera(const Matrix4d& viewMatrix, const Matrix4d& projMatrix);

    /// Set camera settings.
    void SetCameraSettings(const CameraSettings& settings);

    /// Set the time code for animation.
    void SetTime(TimeCode time);

    // ========================================================================
    // Rendering
    // ========================================================================

    /// Render the scene to the internal pixel buffer.
    void Render();

    /// Check if rendering is complete (for async rendering).
    bool IsRenderComplete() const;

    // ========================================================================
    // Pixel Access
    // ========================================================================

    /// Get pointer to rendered pixel data (RGBA8 format).
    /// Valid until next Render() call or destruction.
    const uint8_t* GetPixelData() const;

    /// Get current render width.
    uint32_t GetWidth() const;

    /// Get current render height.
    uint32_t GetHeight() const;

    /// Get total pixel buffer size in bytes (width * height * 4).
    size_t GetPixelDataSize() const;

    // ========================================================================
    // Resize
    // ========================================================================

    /// Resize the render target.
    void Resize(uint32_t width, uint32_t height);

    // ========================================================================
    // Settings
    // ========================================================================

    /// Update render settings.
    void SetRenderSettings(const RenderSettings& settings);

    /// Get current render settings.
    RenderSettings GetRenderSettings() const;

    // ========================================================================
    // Debug
    // ========================================================================

    /// Get render statistics as a string.
    std::string GetRenderStats() const;

    friend void ::retainHydraEngine(HydraEngine* e);
    friend void ::releaseHydraEngine(HydraEngine* e);
} SWIFT_SHARED_REFERENCE(retainHydraEngine, releaseHydraEngine);

} // namespace pixarusd
