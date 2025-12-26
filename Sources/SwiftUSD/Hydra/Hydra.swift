// Hydra.swift - High-level Hydra Rendering - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Hydra types,
// enabling rendering of USD scenes using Pixar's Hydra rendering framework.

import CxxFacade

// MARK: - Type Aliases

/// DrawMode - How geometry should be rendered
public typealias DrawMode = cxxfacade.hydra.DrawMode

/// CullStyle - Face culling options
public typealias CullStyle = cxxfacade.hydra.CullStyle

/// RenderSettings - Configuration for render operations
public typealias RenderSettings = cxxfacade.hydra.RenderSettings

/// CameraSettings - View and projection matrices for rendering
public typealias CameraSettings = cxxfacade.hydra.CameraSettings

/// RendererInfo - Information about a render plugin
public typealias RendererInfo = cxxfacade.hydra.RendererInfo

/// HydraEngine - High-level Hydra rendering engine
public typealias HydraEngine = cxxfacade.hydra.HydraEngine

// MARK: - DrawMode Extensions

extension DrawMode: CustomStringConvertible {
    public var description: String {
        switch self {
        case .Points:
            return "DrawMode.points"
        case .Wireframe:
            return "DrawMode.wireframe"
        case .WireframeOnSurface:
            return "DrawMode.wireframeOnSurface"
        case .ShadedFlat:
            return "DrawMode.shadedFlat"
        case .ShadedSmooth:
            return "DrawMode.shadedSmooth"
        case .GeomOnly:
            return "DrawMode.geomOnly"
        case .GeomFlat:
            return "DrawMode.geomFlat"
        case .GeomSmooth:
            return "DrawMode.geomSmooth"
        @unknown default:
            return "DrawMode.unknown"
        }
    }

    /// Draw as points
    public static var points: DrawMode { .Points }

    /// Draw as wireframe
    public static var wireframe: DrawMode { .Wireframe }

    /// Draw wireframe on top of shaded surface
    public static var wireframeOnSurface: DrawMode { .WireframeOnSurface }

    /// Flat shaded (faceted)
    public static var shadedFlat: DrawMode { .ShadedFlat }

    /// Smooth shaded (default)
    public static var shadedSmooth: DrawMode { .ShadedSmooth }

    /// Geometry only, no materials
    public static var geomOnly: DrawMode { .GeomOnly }

    /// Geometry only, flat shaded
    public static var geomFlat: DrawMode { .GeomFlat }

    /// Geometry only, smooth shaded
    public static var geomSmooth: DrawMode { .GeomSmooth }
}

// MARK: - CullStyle Extensions

extension CullStyle: CustomStringConvertible {
    public var description: String {
        switch self {
        case .NoOpinion:
            return "CullStyle.noOpinion"
        case .Nothing:
            return "CullStyle.nothing"
        case .Back:
            return "CullStyle.back"
        case .Front:
            return "CullStyle.front"
        case .BackUnlessDoubleSided:
            return "CullStyle.backUnlessDoubleSided"
        @unknown default:
            return "CullStyle.unknown"
        }
    }

    /// No opinion, use scene default
    public static var noOpinion: CullStyle { .NoOpinion }

    /// No culling
    public static var nothing: CullStyle { .Nothing }

    /// Cull back faces
    public static var back: CullStyle { .Back }

    /// Cull front faces
    public static var front: CullStyle { .Front }

    /// Cull back faces unless double-sided
    public static var backUnlessDoubleSided: CullStyle { .BackUnlessDoubleSided }
}

// MARK: - RenderSettings Extensions

extension RenderSettings: CustomStringConvertible {
    public var description: String {
        return "RenderSettings(drawMode: \(drawMode), complexity: \(complexity), lighting: \(enableLighting))"
    }

    /// Create preview render settings
    public static var preview: RenderSettings { RenderSettings.Preview() }

    /// Create wireframe render settings
    public static var wireframe: RenderSettings { RenderSettings.Wireframe() }

    /// Create high-quality render settings
    public static var highQuality: RenderSettings { RenderSettings.HighQuality() }
}

// MARK: - CameraSettings Extensions

extension CameraSettings: CustomStringConvertible {
    public var description: String {
        return "CameraSettings(viewport: (\(Int(viewport.x)), \(Int(viewport.y)), \(Int(viewport.z)), \(Int(viewport.w))))"
    }

    /// Aspect ratio of the camera
    public var aspectRatio: Double { GetAspectRatio() }

    /// Create an orthographic camera
    /// - Parameters:
    ///   - left: Left clipping plane
    ///   - right: Right clipping plane
    ///   - bottom: Bottom clipping plane
    ///   - top: Top clipping plane
    ///   - near: Near clipping distance
    ///   - far: Far clipping distance
    /// - Returns: Camera settings with orthographic projection
    public static func orthographic(
        left: Double,
        right: Double,
        bottom: Double,
        top: Double,
        near: Double,
        far: Double
    ) -> CameraSettings {
        return CameraSettings.Orthographic(left, right, bottom, top, near, far)
    }

    /// Create a perspective camera
    /// - Parameters:
    ///   - fovY: Field of view in the Y direction (degrees)
    ///   - aspectRatio: Aspect ratio (width / height)
    ///   - near: Near clipping distance
    ///   - far: Far clipping distance
    /// - Returns: Camera settings with perspective projection
    public static func perspective(
        fovY: Double,
        aspectRatio: Double,
        near: Double,
        far: Double
    ) -> CameraSettings {
        return CameraSettings.Perspective(fovY, aspectRatio, near, far)
    }

    /// Create a look-at camera with perspective projection
    /// - Parameters:
    ///   - eye: Camera position
    ///   - target: Target position to look at
    ///   - up: Up vector
    ///   - fovY: Field of view in the Y direction (degrees)
    ///   - aspectRatio: Aspect ratio (width / height)
    ///   - near: Near clipping distance
    ///   - far: Far clipping distance
    /// - Returns: Camera settings with look-at view and perspective projection
    public static func lookAt(
        eye: Vec3d,
        target: Vec3d,
        up: Vec3d,
        fovY: Double,
        aspectRatio: Double,
        near: Double,
        far: Double
    ) -> CameraSettings {
        return CameraSettings.LookAt(eye, target, up, fovY, aspectRatio, near, far)
    }

    /// Set the viewport dimensions
    /// - Parameters:
    ///   - x: X coordinate of the viewport origin
    ///   - y: Y coordinate of the viewport origin
    ///   - width: Width of the viewport
    ///   - height: Height of the viewport
    public mutating func setViewport(x: Int, y: Int, width: Int, height: Int) {
        SetViewport(Int32(x), Int32(y), Int32(width), Int32(height))
    }
}

// MARK: - RendererInfo Extensions

extension RendererInfo: CustomStringConvertible {
    public var description: String {
        let gpuStr = gpuEnabled ? "GPU" : "CPU"
        return "RendererInfo(\(displayName), \(gpuStr))"
    }

    /// The plugin identifier
    public var pluginId: String {
        return String(id)
    }

    /// The display name for UI
    public var name: String {
        return String(displayName)
    }
}

// MARK: - HydraEngine Extensions

extension HydraEngine: CustomStringConvertible {
    public var description: String {
        if IsValid() {
            let renderer = GetCurrentRenderer()
            return "HydraEngine(renderer: \(renderer), gpu: \(IsGPUEnabled()))"
        }
        return "HydraEngine(invalid)"
    }

    /// Whether this engine is valid and initialized
    public var isValid: Bool { IsValid() }

    /// Whether GPU rendering is enabled
    public var isGPUEnabled: Bool { IsGPUEnabled() }

    /// Whether the render has converged (for progressive renderers)
    public var isConverged: Bool { IsConverged() }

    /// Whether the renderer supports pausing
    public var isPauseSupported: Bool { IsPauseSupported() }

    /// Whether the renderer supports stopping
    public var isStopSupported: Bool { IsStopSupported() }

    /// The current renderer plugin ID
    public var currentRenderer: String {
        return String(GetCurrentRenderer())
    }

    /// Create a new Hydra engine with default settings
    /// - Returns: A new HydraEngine instance
    public static func create() -> HydraEngine {
        return HydraEngine.Create()
    }

    /// Create a Hydra engine with a specific renderer plugin
    /// - Parameters:
    ///   - rendererPluginId: The renderer plugin ID (e.g., "HdStormRendererPlugin")
    ///   - gpuEnabled: Whether to enable GPU rendering
    /// - Returns: A new HydraEngine instance configured for the specified renderer
    public static func create(renderer rendererPluginId: String, gpuEnabled: Bool = true) -> HydraEngine {
        return HydraEngine.Create(std.string(rendererPluginId), gpuEnabled)
    }

    /// Get the list of available renderer plugins
    /// - Returns: Array of RendererInfo describing available renderers
    public static var availableRenderers: [RendererInfo] {
        let stdRenderers = HydraEngine.GetAvailableRenderers()
        return Array(stdRenderers)
    }

    /// Set the renderer plugin to use
    /// - Parameter pluginId: The renderer plugin ID
    /// - Returns: True if the renderer was successfully set
    @discardableResult
    public mutating func setRenderer(_ pluginId: String) -> Bool {
        return SetRendererPlugin(std.string(pluginId))
    }

    /// Render a USD prim with the given settings
    /// - Parameters:
    ///   - prim: The root prim to render (typically stage.pseudoRoot)
    ///   - settings: Render settings
    public mutating func render(_ prim: UsdPrim, settings: RenderSettings) {
        Render(prim, settings)
    }

    /// Render a USD prim with explicit camera settings
    /// - Parameters:
    ///   - prim: The root prim to render
    ///   - settings: Render settings
    ///   - camera: Camera configuration
    public mutating func render(_ prim: UsdPrim, settings: RenderSettings, camera: CameraSettings) {
        Render(prim, settings, camera)
    }

    /// Prepare a batch of geometry for rendering
    /// - Parameters:
    ///   - prim: The root prim
    ///   - settings: Render settings
    public mutating func prepareBatch(_ prim: UsdPrim, settings: RenderSettings) {
        PrepareBatch(prim, settings)
    }

    /// Set the camera state using view and projection matrices
    /// - Parameters:
    ///   - viewMatrix: The view matrix (world to camera transform)
    ///   - projectionMatrix: The projection matrix
    public mutating func setCameraState(viewMatrix: Matrix4d, projectionMatrix: Matrix4d) {
        SetCameraState(viewMatrix, projectionMatrix)
    }

    /// Set the camera using a UsdGeomCamera path
    /// - Parameter cameraPath: Path to the camera prim
    public mutating func setCameraPath(_ cameraPath: SdfPath) {
        SetCameraPath(cameraPath)
    }

    /// Set the render viewport dimensions
    /// - Parameters:
    ///   - x: X coordinate of the viewport origin
    ///   - y: Y coordinate of the viewport origin
    ///   - width: Width of the viewport
    ///   - height: Height of the viewport
    public mutating func setViewport(x: Int, y: Int, width: Int, height: Int) {
        SetViewport(Int32(x), Int32(y), Int32(width), Int32(height))
    }

    /// Set the render buffer size
    /// - Parameters:
    ///   - width: Buffer width
    ///   - height: Buffer height
    public mutating func setRenderBufferSize(width: Int, height: Int) {
        SetRenderBufferSize(Int32(width), Int32(height))
    }

    /// Set the root transform for all rendered geometry
    /// - Parameter transform: The root transformation matrix
    public mutating func setRootTransform(_ transform: Matrix4d) {
        SetRootTransform(transform)
    }

    /// Set the root visibility
    /// - Parameter visible: Whether the root should be visible
    public mutating func setRootVisibility(_ visible: Bool) {
        SetRootVisibility(visible)
    }

    /// Clear selection highlighting
    public mutating func clearSelected() {
        ClearSelected()
    }

    /// Set the selection highlight color
    /// - Parameter color: The highlight color (RGBA)
    public mutating func setSelectionColor(_ color: Vec4f) {
        SetSelectionColor(color)
    }

    /// Pause the renderer
    /// - Returns: True if pausing succeeded
    @discardableResult
    public mutating func pause() -> Bool {
        return Pause()
    }

    /// Resume the renderer
    /// - Returns: True if resuming succeeded
    @discardableResult
    public mutating func resume() -> Bool {
        return Resume()
    }

    /// Stop the renderer
    /// - Returns: True if stopping succeeded
    @discardableResult
    public mutating func stop() -> Bool {
        return Stop()
    }

    /// Restart the renderer
    /// - Returns: True if restarting succeeded
    @discardableResult
    public mutating func restart() -> Bool {
        return Restart()
    }

    /// Enable or disable presentation to the bound framebuffer
    /// - Parameter enabled: Whether presentation is enabled
    public mutating func setEnablePresentation(_ enabled: Bool) {
        SetEnablePresentation(enabled)
    }
}

// MARK: - Utility Functions

/// Get the default renderer plugin ID
/// - Returns: The default renderer plugin identifier
public func getDefaultRendererPluginId() -> String {
    return String(cxxfacade.hydra.GetDefaultRendererPluginId())
}

/// Check if color correction is supported on the current platform
/// - Returns: True if color correction is capable
public func isColorCorrectionCapable() -> Bool {
    return cxxfacade.hydra.IsColorCorrectionCapable()
}

// MARK: - Convenience Extensions for Stage Rendering

extension UsdStage {
    /// Render this stage using the given engine and settings
    /// - Parameters:
    ///   - engine: The Hydra rendering engine
    ///   - settings: Render settings
    public func render(with engine: inout HydraEngine, settings: RenderSettings = .preview) {
        engine.render(GetPseudoRoot(), settings: settings)
    }

    /// Render this stage with explicit camera settings
    /// - Parameters:
    ///   - engine: The Hydra rendering engine
    ///   - settings: Render settings
    ///   - camera: Camera configuration
    public func render(with engine: inout HydraEngine, settings: RenderSettings, camera: CameraSettings) {
        engine.render(GetPseudoRoot(), settings: settings, camera: camera)
    }
}
