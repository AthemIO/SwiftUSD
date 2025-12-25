// RenderingCxx.swift - Swift wrappers for SwiftRendering module
// Provides convenience extensions and typealiases for the Rendering module

import USDCxx

// MARK: - Type Aliases

/// High-level Hydra renderer facade
public typealias HydraRenderer = swiftusd.HydraRendererRef

/// A rendered frame with color and depth data
public typealias RenderFrame = swiftusd.RenderFrameRef

#if os(macOS) || os(iOS) || os(visionOS)
/// Native platform view for rendering (macOS/iOS/visionOS)
public typealias PlatformView = swiftusd.PlatformViewRef
#endif

/// Rendering configuration parameters
public typealias SwiftRenderingParams = swiftusd.SwiftRenderingParams

/// Camera configuration parameters
public typealias SwiftRenderingCameraParams = swiftusd.SwiftRenderingCameraParams

/// Rendering statistics
public typealias SwiftRenderingStats = swiftusd.SwiftRenderingStats

// MARK: - Enumerations

/// Color space for render output
public typealias SwiftRenderingColorSpace = swiftusd.SwiftRenderingColorSpace

/// Draw mode for geometry rendering
public typealias SwiftRenderingDrawMode = swiftusd.SwiftRenderingDrawMode

// MARK: - HydraRendererRef Extensions

extension swiftusd.HydraRendererRef {
    /// Create a new Hydra renderer with the default (Storm) renderer plugin.
    public static func create() -> swiftusd.HydraRendererRef? {
        return swiftusd.HydraRendererRef.Create()
    }

    /// Create a new Hydra renderer with a specific renderer plugin.
    public static func create(plugin: swiftusd.Token) -> swiftusd.HydraRendererRef? {
        return swiftusd.HydraRendererRef.Create(plugin)
    }

#if os(macOS) || os(iOS) || os(visionOS)
    /// Create a Metal-based Hydra renderer.
    public static func createMetal() -> swiftusd.HydraRendererRef? {
        return swiftusd.HydraRendererRef.CreateMetal(nil)
    }
#endif

    /// Get the renderer plugin ID.
    public var rendererPluginId: swiftusd.Token {
        return GetRendererPluginId()
    }

    /// Check if the renderer is valid.
    public var isValid: Bool {
        return IsValid()
    }

    /// Get the graphics API name.
    public var graphicsAPIName: swiftusd.Token {
        return GetGraphicsAPIName()
    }

    /// Get the last frame's rendering statistics.
    public var stats: swiftusd.SwiftRenderingStats {
        return GetStats()
    }

    /// Get the current stage.
    public var stage: swiftusd.StageRef? {
        return GetStage()
    }

    /// Get the root path for rendering.
    public var rootPath: swiftusd.Path {
        return GetRootPath()
    }

    /// Get/set the current time for animation.
    public var time: Double {
        get { return GetTime() }
        set { SetTime(newValue) }
    }

    /// Set the stage to render.
    public func setStage(_ stage: swiftusd.StageRef, rootPath: swiftusd.Path = swiftusd.Path("/")) {
        SetStage(stage, rootPath)
    }

    /// Get/set rendering parameters.
    public var params: swiftusd.SwiftRenderingParams {
        get { return GetParams() }
        set { SetParams(newValue) }
    }

    /// Get/set camera parameters.
    public var camera: swiftusd.SwiftRenderingCameraParams {
        get { return GetCamera() }
        set { SetCamera(newValue) }
    }

    /// Set the viewport size.
    public func setViewport(width: Int, height: Int) {
        SetViewport(Int32(width), Int32(height))
    }

    /// Get the viewport width.
    public var viewportWidth: Int {
        return Int(GetViewportWidth())
    }

    /// Get the viewport height.
    public var viewportHeight: Int {
        return Int(GetViewportHeight())
    }

    /// Set the selected prim paths.
    public func setSelection(_ paths: [swiftusd.Path]) {
        paths.withUnsafeBufferPointer { buffer in
            SetSelection(buffer.baseAddress, buffer.count)
        }
    }

    /// Clear the selection.
    public func clearSelection() {
        ClearSelection()
    }

    /// Get the number of selected paths.
    public var selectionCount: Int {
        return Int(GetSelectionCount())
    }

    /// Get all selected paths.
    public var selectedPaths: [swiftusd.Path] {
        var result: [swiftusd.Path] = []
        for i in 0..<selectionCount {
            result.append(GetSelection(i))
        }
        return result
    }

    /// Prepare the renderer for a new frame.
    public func prepare() {
        Prepare()
    }

    /// Render a single frame.
    public func render() -> swiftusd.RenderFrameRef? {
        return Render()
    }

    /// Render to an external buffer.
    public func render(to colorBuffer: UnsafeMutablePointer<Float>,
                       depth depthBuffer: UnsafeMutablePointer<Float>? = nil) -> Bool {
        return RenderToBuffer(colorBuffer, depthBuffer)
    }

    /// Check if the render has converged.
    public var isConverged: Bool {
        return IsConverged()
    }

    /// Restart progressive rendering.
    public func restartRender() {
        RestartRender()
    }

    /// Perform a pick operation at screen coordinates.
    public func pick(x: Int, y: Int) -> swiftusd.HdxPickHit? {
        var hit = swiftusd.HdxPickHit()
        if Pick(Int32(x), Int32(y), &hit) {
            return hit
        }
        return nil
    }

    /// Perform a pick operation in a rectangular region.
    public func pickRect(x: Int, y: Int, width: Int, height: Int, maxHits: Int = 100) -> [swiftusd.HdxPickHit] {
        var hits = [swiftusd.HdxPickHit](repeating: swiftusd.HdxPickHit(), count: maxHits)
        let count = hits.withUnsafeMutableBufferPointer { buffer in
            PickRect(Int32(x), Int32(y), Int32(width), Int32(height), buffer.baseAddress, buffer.count)
        }
        return Array(hits.prefix(Int(count)))
    }

    /// Force garbage collection of GPU resources.
    public func garbageCollect() {
        GarbageCollect()
    }

    /// Invalidate all cached data.
    public func invalidate() {
        Invalidate()
    }
}

// MARK: - RenderFrameRef Extensions

extension swiftusd.RenderFrameRef {
    /// Get the frame width.
    public var width: Int {
        return Int(GetWidth())
    }

    /// Get the frame height.
    public var height: Int {
        return Int(GetHeight())
    }

    /// Get bytes per row.
    public var bytesPerRow: Int {
        return Int(GetBytesPerRow())
    }

    /// Get the color data buffer (RGBA float32).
    /// Returns nil if no color buffer is available.
    public func getColorData() -> UnsafePointer<Float>? {
        // Swift C++ interop returns Optional for nullable pointers
        guard let ptr = self.GetColorData() else { return nil }
        return ptr
    }

    /// Get the depth data buffer (float32).
    /// Returns nil if no depth buffer is available.
    public func getDepthData() -> UnsafePointer<Float>? {
        // Swift C++ interop returns Optional for nullable pointers
        guard let ptr = self.GetDepthData() else { return nil }
        return ptr
    }

    /// Get the color data size in bytes.
    public var colorDataSize: Int {
        return Int(GetColorDataSize())
    }

    /// Get the depth data size in bytes.
    public var depthDataSize: Int {
        return Int(GetDepthDataSize())
    }

    /// Get rendering statistics.
    public var stats: swiftusd.SwiftRenderingStats {
        return GetStats()
    }

    /// Check if the frame is valid.
    public var isValid: Bool {
        return IsValid()
    }

    /// Check if the frame has converged.
    public var isConverged: Bool {
        return IsConverged()
    }
}

#if os(macOS) || os(iOS) || os(visionOS)
// MARK: - PlatformViewRef Extensions

extension swiftusd.PlatformViewRef {
    /// Create a platform view with the given renderer.
    public static func create(renderer: swiftusd.HydraRendererRef) -> swiftusd.PlatformViewRef? {
        return swiftusd.PlatformViewRef.Create(renderer)
    }

    /// Get the native layer handle (CAMetalLayer* on Apple).
    public func getNativeLayer() -> UnsafeMutableRawPointer? {
        guard let ptr = self.GetNativeLayer() else { return nil }
        return ptr
    }

    /// Set the drawable size.
    public func setDrawableSize(width: Int, height: Int) {
        SetDrawableSize(Int32(width), Int32(height))
    }

    /// Get the drawable width.
    public var drawableWidth: Int {
        return Int(GetDrawableWidth())
    }

    /// Get the drawable height.
    public var drawableHeight: Int {
        return Int(GetDrawableHeight())
    }

    /// Render and present to the view.
    public func present() -> Bool {
        return Present()
    }

    /// Get the associated renderer.
    public var renderer: swiftusd.HydraRendererRef? {
        return GetRenderer()
    }
}
#endif

// MARK: - SwiftRenderingParams Extensions

extension swiftusd.SwiftRenderingParams {
    /// Create rendering params with viewport size.
    public static func make(width: Int, height: Int) -> swiftusd.SwiftRenderingParams {
        return swiftusd.SwiftRenderingParams(Int32(width), Int32(height))
    }

    /// Create default rendering params.
    public static func makeDefault() -> swiftusd.SwiftRenderingParams {
        return swiftusd.SwiftRenderingParams()
    }
}

// MARK: - SwiftRenderingCameraParams Extensions

extension swiftusd.SwiftRenderingCameraParams {
    /// Create camera params with view and projection matrices.
    public static func make(
        view: swiftusd.Matrix4d,
        projection: swiftusd.Matrix4d
    ) -> swiftusd.SwiftRenderingCameraParams {
        return swiftusd.SwiftRenderingCameraParams(view, projection)
    }

    /// Create camera params with a scene camera path.
    public static func make(cameraPath: swiftusd.Path) -> swiftusd.SwiftRenderingCameraParams {
        return swiftusd.SwiftRenderingCameraParams(cameraPath)
    }

    /// Create default camera params.
    public static func makeDefault() -> swiftusd.SwiftRenderingCameraParams {
        return swiftusd.SwiftRenderingCameraParams()
    }
}

// MARK: - SwiftRenderingColorSpace Helpers

extension swiftusd.SwiftRenderingColorSpace {
    /// Get the token for this color space.
    public var token: swiftusd.Token {
        return swiftusd.SwiftRenderingGetColorSpaceToken(self)
    }

    /// Get the name of this color space.
    public var name: String {
        return String(cString: swiftusd.SwiftRenderingGetColorSpaceName(self))
    }
}

// MARK: - SwiftRenderingDrawMode Helpers

extension swiftusd.SwiftRenderingDrawMode {
    /// Get the token for this draw mode.
    public var token: swiftusd.Token {
        return swiftusd.SwiftRenderingGetDrawModeToken(self)
    }

    /// Get the name of this draw mode.
    public var name: String {
        return String(cString: swiftusd.SwiftRenderingGetDrawModeName(self))
    }
}

// MARK: - Token Extensions for SwiftRendering

extension swiftusd.Token {
    // Renderer plugin tokens
    public static var storm: swiftusd.Token { swiftusd.SwiftRenderingTokens.Storm() }
    public static var embree: swiftusd.Token { swiftusd.SwiftRenderingTokens.Embree() }
    public static var prman: swiftusd.Token { swiftusd.SwiftRenderingTokens.Prman() }

    // Color space tokens
    public static var linearColorSpace: swiftusd.Token { swiftusd.SwiftRenderingTokens.LinearColorSpace() }
    public static var sRGBColorSpace: swiftusd.Token { swiftusd.SwiftRenderingTokens.SRGBColorSpace() }
    public static var displayP3ColorSpace: swiftusd.Token { swiftusd.SwiftRenderingTokens.DisplayP3ColorSpace() }
    public static var rawColorSpace: swiftusd.Token { swiftusd.SwiftRenderingTokens.RawColorSpace() }

    // Draw mode tokens
    public static var shadedDrawMode: swiftusd.Token { swiftusd.SwiftRenderingTokens.ShadedDrawMode() }
    public static var wireframeDrawMode: swiftusd.Token { swiftusd.SwiftRenderingTokens.WireframeDrawMode() }
    public static var shadedWireframeDrawMode: swiftusd.Token { swiftusd.SwiftRenderingTokens.ShadedWireframeDrawMode() }
    public static var pointsDrawMode: swiftusd.Token { swiftusd.SwiftRenderingTokens.PointsDrawMode() }
    public static var geomDrawMode: swiftusd.Token { swiftusd.SwiftRenderingTokens.GeomDrawMode() }
    public static var flatShadedDrawMode: swiftusd.Token { swiftusd.SwiftRenderingTokens.FlatShadedDrawMode() }

    // Note: AOV tokens (colorAov, depthAov, normalAov, primIdAov, instanceIdAov, elementIdAov)
    // are already defined in HdStCxx.swift - use those extensions
}

// MARK: - Global Functions

/// Get the token for a color space.
public func swiftRenderingGetColorSpaceToken(_ colorSpace: swiftusd.SwiftRenderingColorSpace) -> swiftusd.Token {
    return swiftusd.SwiftRenderingGetColorSpaceToken(colorSpace)
}

/// Get the name of a color space.
public func swiftRenderingGetColorSpaceName(_ colorSpace: swiftusd.SwiftRenderingColorSpace) -> String {
    return String(cString: swiftusd.SwiftRenderingGetColorSpaceName(colorSpace))
}

/// Get the token for a draw mode.
public func swiftRenderingGetDrawModeToken(_ drawMode: swiftusd.SwiftRenderingDrawMode) -> swiftusd.Token {
    return swiftusd.SwiftRenderingGetDrawModeToken(drawMode)
}

/// Get the name of a draw mode.
public func swiftRenderingGetDrawModeName(_ drawMode: swiftusd.SwiftRenderingDrawMode) -> String {
    return String(cString: swiftusd.SwiftRenderingGetDrawModeName(drawMode))
}
