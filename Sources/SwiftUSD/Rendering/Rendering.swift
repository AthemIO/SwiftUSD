// Rendering.swift - SwiftRendering module namespace
// Provides: HydraRenderer facade, Metal/Vulkan platform views
// Dependencies: UsdImaging, Hdx

import USDCxx

// MARK: - Rendering Namespace

/// SwiftRendering module namespace - high-level Hydra rendering facade.
///
/// The SwiftRendering module provides a simplified, platform-native interface
/// for rendering USD stages using the Hydra framework:
/// - `HydraRenderer`: Main renderer facade that manages the full pipeline
/// - `RenderFrame`: A rendered frame with color and depth buffers
/// - `PlatformView`: Native view wrapper for display (Metal on Apple)
/// - `SwiftRenderingParams`: Rendering configuration (viewport, lighting, etc.)
/// - `SwiftRenderingCameraParams`: Camera configuration (matrices, scene camera)
/// - `SwiftRenderingStats`: Rendering statistics
/// - `SwiftRenderingColorSpace`: Output color space options
/// - `SwiftRenderingDrawMode`: Geometry rendering modes
///
/// Provides a high-level facade over the UsdImaging and Hdx modules.
///
/// ## Usage
/// ```swift
/// // Create a renderer
/// guard let renderer = HydraRenderer.create() else { return }
///
/// // Set the stage to render
/// renderer.setStage(stage, rootPath: swiftusd.Path("/"))
///
/// // Configure rendering
/// var params = SwiftRenderingParams.make(width: 1920, height: 1080)
/// params.enableShadows = true
/// renderer.params = params
///
/// // Configure camera
/// let camera = SwiftRenderingCameraParams.make(view: viewMatrix, projection: projMatrix)
/// renderer.camera = camera
///
/// // Set time for animation
/// renderer.time = 1.0
///
/// // Render a frame
/// if let frame = renderer.render() {
///     // Access frame.colorData for display
///     print("Frame size: \(frame.width)x\(frame.height)")
///     print("Converged: \(frame.isConverged)")
/// }
///
/// // Picking
/// if let hit = renderer.pick(x: 100, y: 100) {
///     print("Hit: \(hit.objectId)")
/// }
/// ```
///
/// ## Platform Views
/// On Apple platforms, you can use `PlatformView` for native rendering:
/// ```swift
/// #if os(macOS) || os(iOS) || os(visionOS)
/// guard let view = PlatformView.create(renderer: renderer) else { return }
/// view.setDrawableSize(width: 1920, height: 1080)
///
/// // In your render loop
/// view.present()
/// #endif
/// ```
public enum Rendering {
    // This enum serves as a namespace for Rendering-related types and utilities.
    // All types are defined in RenderingCxx.swift using the C++ interop layer.
}

// MARK: - RenderingError

/// Errors that can occur when working with SwiftRendering types.
public enum RenderingError: Error, CustomStringConvertible {

    /// The renderer could not be created.
    case creationFailed(String)

    /// A rendering operation failed.
    case renderFailed(String)

    /// Stage setup failed.
    case stageFailed(String)

    /// Camera configuration failed.
    case cameraFailed(String)

    /// A picking operation failed.
    case pickFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    /// The platform is not supported.
    case unsupportedPlatform(String)

    /// GPU resource allocation failed.
    case resourceAllocationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Rendering creation failed: \(message)"
        case .renderFailed(let message):
            return "Rendering failed: \(message)"
        case .stageFailed(let message):
            return "Stage setup failed: \(message)"
        case .cameraFailed(let message):
            return "Camera configuration failed: \(message)"
        case .pickFailed(let message):
            return "Picking failed: \(message)"
        case .invalidArgument(let message):
            return "Rendering invalid argument: \(message)"
        case .unsupportedPlatform(let message):
            return "Unsupported platform: \(message)"
        case .resourceAllocationFailed(let message):
            return "Resource allocation failed: \(message)"
        }
    }
}
