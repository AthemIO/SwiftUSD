// UsdAppUtils.swift - Swift wrappers for UsdAppUtils module
// Provides convenience extensions and typealiases for USD application utilities

import USDCxx

// MARK: - Type Aliases

/// Utility class for recording images of USD stages
public typealias UsdAppUtilsFrameRecorder = swiftusd.UsdAppUtilsFrameRecorderRef

/// Configuration parameters for the frame recorder
public typealias UsdAppUtilsFrameRecorderParams = swiftusd.UsdAppUtilsFrameRecorderParams

/// Color correction mode configuration
public typealias UsdAppUtilsColorCorrectionMode = swiftusd.UsdAppUtilsColorCorrectionMode

// MARK: - UsdAppUtilsFrameRecorderRef Extensions

extension swiftusd.UsdAppUtilsFrameRecorderRef {
    /// Create a new frame recorder with default settings.
    ///
    /// - Parameters:
    ///   - rendererPluginId: The renderer plugin that Hydra should use (empty for default)
    ///   - gpuEnabled: Whether to allow Hydra to use the GPU
    ///   - enableUsdDrawModes: Whether to respect USD draw modes
    /// - Returns: A new frame recorder reference, or nil if creation fails
    public static func create(
        rendererPluginId: swiftusd.Token = swiftusd.Token(),
        gpuEnabled: Bool = true,
        enableUsdDrawModes: Bool = true
    ) -> swiftusd.UsdAppUtilsFrameRecorderRef? {
        return swiftusd.UsdAppUtilsFrameRecorderRef.Create(
            rendererPluginId,
            gpuEnabled,
            enableUsdDrawModes)
    }

    /// Create a new frame recorder using a string renderer ID.
    ///
    /// - Parameters:
    ///   - rendererPluginId: The renderer plugin name (e.g., "HdStormRendererPlugin")
    ///   - gpuEnabled: Whether to allow Hydra to use the GPU
    ///   - enableUsdDrawModes: Whether to respect USD draw modes
    /// - Returns: A new frame recorder reference, or nil if creation fails
    public static func create(
        rendererPluginId: String,
        gpuEnabled: Bool = true,
        enableUsdDrawModes: Bool = true
    ) -> swiftusd.UsdAppUtilsFrameRecorderRef? {
        return swiftusd.UsdAppUtilsFrameRecorderRef.Create(
            swiftusd.Token(rendererPluginId),
            gpuEnabled,
            enableUsdDrawModes)
    }

    // MARK: - Renderer Configuration

    /// The ID of the Hydra renderer plugin that will be used for recording.
    public var currentRendererId: swiftusd.Token {
        return GetCurrentRendererId()
    }

    /// Sets the Hydra renderer plugin to be used for recording.
    ///
    /// - Parameter id: The renderer plugin ID
    /// - Returns: true if the renderer was set successfully
    @discardableResult
    public func setRendererPlugin(_ id: swiftusd.Token) -> Bool {
        return SetRendererPlugin(id)
    }

    /// Sets the Hydra renderer plugin using a string ID.
    ///
    /// - Parameter id: The renderer plugin ID string
    /// - Returns: true if the renderer was set successfully
    @discardableResult
    public func setRendererPlugin(_ id: String) -> Bool {
        return SetRendererPlugin(swiftusd.Token(id))
    }

    // MARK: - Render Pass/Settings Configuration

    /// Sets the path to the render pass prim to use.
    ///
    /// - Parameter path: Path to the render pass prim
    public func setActiveRenderPassPrimPath(_ path: swiftusd.Path) {
        SetActiveRenderPassPrimPath(path)
    }

    /// Sets the path to the render pass prim using a string path.
    ///
    /// - Parameter path: String path to the render pass prim
    public func setActiveRenderPassPrimPath(_ path: String) {
        SetActiveRenderPassPrimPath(swiftusd.Path(path))
    }

    /// Sets the path to the render settings prim to use.
    ///
    /// - Parameter path: Path to the render settings prim
    public func setActiveRenderSettingsPrimPath(_ path: swiftusd.Path) {
        SetActiveRenderSettingsPrimPath(path)
    }

    /// Sets the path to the render settings prim using a string path.
    ///
    /// - Parameter path: String path to the render settings prim
    public func setActiveRenderSettingsPrimPath(_ path: String) {
        SetActiveRenderSettingsPrimPath(swiftusd.Path(path))
    }

    // MARK: - Image Configuration

    /// The width of the recorded image in pixels.
    /// Height is computed from the camera aspect ratio.
    public var imageWidth: Int {
        get { Int(GetImageWidth()) }
        set { SetImageWidth(newValue) }
    }

    /// The level of refinement complexity (typically 1.0 to 2.0).
    public var complexity: Float {
        get { GetComplexity() }
        set { SetComplexity(newValue) }
    }

    /// The color correction mode for recording.
    public var colorCorrectionMode: swiftusd.Token {
        get { GetColorCorrectionMode() }
        set { SetColorCorrectionMode(newValue) }
    }

    // MARK: - Lighting Configuration

    /// Whether the camera headlight is enabled.
    public var cameraLightEnabled: Bool {
        get { GetCameraLightEnabled() }
        set { SetCameraLightEnabled(newValue) }
    }

    /// Whether dome lights are visible in the background.
    public var domeLightVisibility: Bool {
        get { GetDomeLightVisibility() }
        set { SetDomeLightVisibility(newValue) }
    }

    // MARK: - Purpose Configuration

    /// Sets the purposes to include in rendering.
    ///
    /// - Parameter purposes: Array of purpose tokens
    public func setIncludedPurposes(_ purposes: [swiftusd.Token]) {
        purposes.withUnsafeBufferPointer { buffer in
            SetIncludedPurposes(buffer.baseAddress, buffer.count)
        }
    }

    /// The currently included purposes.
    public var includedPurposes: [swiftusd.Token] {
        var result: [swiftusd.Token] = []
        let cppVector = GetIncludedPurposes()
        for i in 0..<cppVector.size() {
            result.append(cppVector[i])
        }
        return result
    }

    // MARK: - Camera Configuration

    /// The primary camera prim path.
    public var primaryCameraPrimPath: swiftusd.Path {
        get { GetPrimaryCameraPrimPath() }
        set { SetPrimaryCameraPrimPath(newValue) }
    }

    /// Sets the primary camera prim path using a string.
    ///
    /// - Parameter path: String path to the camera prim
    public func setPrimaryCameraPrimPath(_ path: String) {
        SetPrimaryCameraPrimPath(swiftusd.Path(path))
    }

    // MARK: - Recording

    /// Records an image and writes the result to the specified output path.
    ///
    /// - Parameters:
    ///   - stage: The USD stage to record
    ///   - cameraPath: Path to the camera prim
    ///   - timeCode: The time code to evaluate
    ///   - outputImagePath: Path where the output image will be written
    /// - Returns: true if the image was generated and written successfully
    public func record(
        stage: swiftusd.StageRef,
        cameraPath: swiftusd.Path,
        timeCode: Double,
        outputImagePath: String
    ) -> Bool {
        return Record(stage, cameraPath, timeCode, outputImagePath)
    }

    /// Records an image using string paths.
    ///
    /// - Parameters:
    ///   - stage: The USD stage to record
    ///   - cameraPath: String path to the camera prim
    ///   - timeCode: The time code to evaluate
    ///   - outputImagePath: Path where the output image will be written
    /// - Returns: true if the image was generated and written successfully
    public func record(
        stage: swiftusd.StageRef,
        cameraPath: String,
        timeCode: Double,
        outputImagePath: String
    ) -> Bool {
        return Record(stage, swiftusd.Path(cameraPath), timeCode, outputImagePath)
    }
}

// MARK: - UsdAppUtilsFrameRecorderParams Extensions

extension swiftusd.UsdAppUtilsFrameRecorderParams {
    /// Create default frame recorder parameters.
    public static func makeDefault() -> swiftusd.UsdAppUtilsFrameRecorderParams {
        return swiftusd.UsdAppUtilsFrameRecorderParams()
    }

    /// Create frame recorder parameters with specific settings.
    ///
    /// - Parameters:
    ///   - imageWidth: Width of the recorded image in pixels
    ///   - complexity: Refinement complexity level
    ///   - cameraLightEnabled: Whether to enable the camera headlight
    ///   - domeLightsVisible: Whether dome lights are visible
    ///   - gpuEnabled: Whether to enable GPU rendering
    ///   - enableUsdDrawModes: Whether to respect USD draw modes
    public static func make(
        imageWidth: Int = 960,
        complexity: Float = 1.0,
        cameraLightEnabled: Bool = true,
        domeLightsVisible: Bool = true,
        gpuEnabled: Bool = true,
        enableUsdDrawModes: Bool = true
    ) -> swiftusd.UsdAppUtilsFrameRecorderParams {
        var params = swiftusd.UsdAppUtilsFrameRecorderParams()
        params.imageWidth = imageWidth
        params.complexity = complexity
        params.cameraLightEnabled = cameraLightEnabled
        params.domeLightsVisible = domeLightsVisible
        params.gpuEnabled = gpuEnabled
        params.enableUsdDrawModes = enableUsdDrawModes
        return params
    }
}

// MARK: - Token Extensions for UsdAppUtils

extension swiftusd.Token {
    // Color correction mode tokens
    public static var colorCorrectionDisabled: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.ColorCorrectionDisabled()
    }
    public static var colorCorrectionSRGB: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.ColorCorrectionSRGB()
    }
    public static var colorCorrectionOpenColorIO: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.ColorCorrectionOpenColorIO()
    }

    // Purpose tokens (also available from UsdImaging)
    public static var purposeDefault: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.PurposeDefault()
    }
    public static var purposeProxy: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.PurposeProxy()
    }
    public static var purposeRender: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.PurposeRender()
    }
    public static var purposeGuide: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.PurposeGuide()
    }

    // Common renderer IDs
    public static var rendererStorm: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.RendererStorm()
    }
    public static var rendererEmbree: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.RendererEmbree()
    }
    public static var rendererPrman: swiftusd.Token {
        swiftusd.UsdAppUtilsTokens.RendererPrman()
    }
}

// MARK: - Camera Utility Functions

/// Gets the camera at the specified path from the USD stage.
///
/// If the path is absolute, this is equivalent to UsdGeomCamera.Get().
/// If the path is a single-element path (just the camera name), the
/// stage will be searched for a matching camera.
///
/// - Parameters:
///   - stage: The USD stage to search
///   - cameraPath: Path to the camera (absolute or just the camera name)
/// - Returns: The camera at the specified path, or nil if not found
public func usdAppUtilsGetCameraAtPath(
    stage: swiftusd.StageRef,
    cameraPath: swiftusd.Path
) -> swiftusd.GeomCameraRef? {
    return swiftusd.UsdAppUtilsGetCameraAtPath(stage, cameraPath)
}

/// Gets the camera at the specified path from the USD stage using a string path.
///
/// - Parameters:
///   - stage: The USD stage to search
///   - cameraPath: String path to the camera
/// - Returns: The camera at the specified path, or nil if not found
public func usdAppUtilsGetCameraAtPath(
    stage: swiftusd.StageRef,
    cameraPath: String
) -> swiftusd.GeomCameraRef? {
    return swiftusd.UsdAppUtilsGetCameraAtPath(stage, swiftusd.Path(cameraPath))
}

/// Check if a prim type name represents a camera type.
///
/// - Parameter primTypeName: The type name token of the prim
/// - Returns: true if the prim type is a camera type
public func usdAppUtilsIsCameraPrimType(_ primTypeName: swiftusd.Token) -> Bool {
    return swiftusd.UsdAppUtilsIsCameraPrimType(primTypeName)
}
