// UsdImagingCxx.swift - Swift wrappers for UsdImaging module
// Provides convenience extensions and typealiases for connecting USD to Hydra

import USDCxx

// MARK: - Type Aliases

/// Scene delegate connecting USD to Hydra rendering
public typealias UsdImagingDelegate = swiftusd.UsdImagingDelegateRef

/// Configuration parameters for the imaging delegate
public typealias UsdImagingDelegateParams = swiftusd.UsdImagingDelegateParams

// MARK: - UsdImagingDelegateRef Extensions

extension swiftusd.UsdImagingDelegateRef {
    /// Create a new UsdImagingDelegate with the given render index and delegate ID.
    ///
    /// - Parameters:
    ///   - renderIndex: The HdRenderIndex this delegate will populate
    ///   - delegateId: A unique path identifying this delegate
    /// - Returns: A new delegate reference, or nil if creation fails
    public static func create(
        renderIndex: swiftusd.HdRenderIndexRef,
        delegateId: swiftusd.Path
    ) -> swiftusd.UsdImagingDelegateRef? {
        return swiftusd.UsdImagingDelegateRef.Create(renderIndex, delegateId)
    }

    /// Create a new UsdImagingDelegate using a string delegate ID.
    ///
    /// - Parameters:
    ///   - renderIndex: The HdRenderIndex this delegate will populate
    ///   - delegateId: A unique path string identifying this delegate
    /// - Returns: A new delegate reference, or nil if creation fails
    public static func create(
        renderIndex: swiftusd.HdRenderIndexRef,
        delegateId: String
    ) -> swiftusd.UsdImagingDelegateRef? {
        return swiftusd.UsdImagingDelegateRef.Create(renderIndex, swiftusd.Path(delegateId))
    }

    /// The delegate ID path.
    public var delegateId: swiftusd.Path {
        return GetDelegateId()
    }

    /// The render index this delegate populates.
    public var renderIndex: swiftusd.HdRenderIndexRef? {
        return GetRenderIndex()
    }

    // MARK: - Population

    /// Populate the render index from the given USD stage root prim.
    ///
    /// - Parameters:
    ///   - stage: The USD stage to populate from
    ///   - rootPrimPath: Path to the root prim to start population from
    public func populate(stage: swiftusd.StageRef, rootPrimPath: swiftusd.Path) {
        Populate(stage, rootPrimPath)
    }

    /// Populate the render index from the given USD stage using a string path.
    ///
    /// - Parameters:
    ///   - stage: The USD stage to populate from
    ///   - rootPrimPath: String path to the root prim
    public func populate(stage: swiftusd.StageRef, rootPrimPath: String) {
        Populate(stage, swiftusd.Path(rootPrimPath))
    }

    /// Populate with excluded paths.
    ///
    /// - Parameters:
    ///   - stage: The USD stage to populate from
    ///   - rootPrimPath: Path to the root prim
    ///   - excludedPaths: Array of paths to exclude from population
    public func populate(
        stage: swiftusd.StageRef,
        rootPrimPath: swiftusd.Path,
        excludedPaths: [swiftusd.Path]
    ) {
        excludedPaths.withUnsafeBufferPointer { buffer in
            PopulateWithExclusions(stage, rootPrimPath, buffer.baseAddress, buffer.count)
        }
    }

    /// Clear all prims populated by this delegate.
    public func clearPopulatedPrims() {
        ClearPopulatedPrims()
    }

    // MARK: - Time Management

    /// The current time for scene evaluation.
    public var time: Double {
        get { GetTime() }
        set { SetTime(newValue) }
    }

    /// Get the time with an offset applied.
    ///
    /// - Parameter offset: Time offset to add
    /// - Returns: Current time plus the offset
    public func timeWithOffset(_ offset: Float) -> Double {
        return GetTimeWithOffset(offset)
    }

    // MARK: - Refinement Control

    /// The fallback refine level used when no per-prim level is set (0-8).
    public var refineLevelFallback: Int {
        get { Int(GetRefineLevelFallback()) }
        set { SetRefineLevelFallback(Int32(newValue)) }
    }

    /// Set the refine level for a specific prim.
    ///
    /// - Parameters:
    ///   - level: Refinement level for that prim (0-8)
    ///   - primPath: Path to the USD prim
    public func setRefineLevel(_ level: Int, for primPath: swiftusd.Path) {
        SetRefineLevel(primPath, Int32(level))
    }

    /// Clear the per-prim refine level, reverting to fallback.
    ///
    /// - Parameter primPath: Path to the USD prim
    public func clearRefineLevel(for primPath: swiftusd.Path) {
        ClearRefineLevel(primPath)
    }

    // MARK: - Sync and Update

    /// Apply any pending updates to the render index.
    public func applyPendingUpdates() {
        ApplyPendingUpdates()
    }

    /// Sync all prims to the render index.
    ///
    /// - Parameter includeUnvarying: Whether to sync unvarying attributes
    public func syncAll(includeUnvarying: Bool = false) {
        SyncAll(includeUnvarying)
    }

    // MARK: - Display Options

    /// Whether to display guide geometry.
    public var displayGuides: Bool {
        get { GetDisplayGuides() }
        set { SetDisplayGuides(newValue) }
    }

    /// Whether to display render geometry.
    public var displayRender: Bool {
        get { GetDisplayRender() }
        set { SetDisplayRender(newValue) }
    }

    /// Whether to display proxy geometry.
    public var displayProxy: Bool {
        get { GetDisplayProxy() }
        set { SetDisplayProxy(newValue) }
    }

    // MARK: - Scene Material/Light Control

    /// Whether scene materials are enabled.
    public var sceneMaterialsEnabled: Bool {
        get { GetSceneMaterialsEnabled() }
        set { SetSceneMaterialsEnabled(newValue) }
    }

    /// Whether scene lights are enabled.
    public var sceneLightsEnabled: Bool {
        get { GetSceneLightsEnabled() }
        set { SetSceneLightsEnabled(newValue) }
    }

    // MARK: - Transform Operations

    /// The root transform applied to all prims.
    public var rootTransform: swiftusd.Matrix4d {
        get { GetRootTransform() }
        set { SetRootTransform(newValue) }
    }

    /// Set paths to invisible prims.
    ///
    /// - Parameter paths: Array of paths to make invisible
    public func setInvisiblePaths(_ paths: [swiftusd.Path]) {
        paths.withUnsafeBufferPointer { buffer in
            SetInvisiblePaths(buffer.baseAddress, buffer.count)
        }
    }

    /// Set the camera path for window policy.
    ///
    /// - Parameter cameraPath: Path to the camera prim
    public func setCameraForSampling(_ cameraPath: swiftusd.Path) {
        SetCameraForSampling(cameraPath)
    }
}

// MARK: - UsdImagingDelegateParams Extensions

extension swiftusd.UsdImagingDelegateParams {
    /// Create default delegate parameters.
    public static func makeDefault() -> swiftusd.UsdImagingDelegateParams {
        return swiftusd.UsdImagingDelegateParams()
    }

    /// Create delegate parameters with specific settings.
    ///
    /// - Parameters:
    ///   - refineLevel: Default refinement level (0-8)
    ///   - sceneMaterialsEnabled: Whether to enable scene materials
    ///   - sceneLightsEnabled: Whether to enable scene lights
    public static func make(
        refineLevel: Int32 = 0,
        sceneMaterialsEnabled: Bool = true,
        sceneLightsEnabled: Bool = true
    ) -> swiftusd.UsdImagingDelegateParams {
        var params = swiftusd.UsdImagingDelegateParams()
        params.refineLevel = refineLevel
        params.sceneMaterialsEnabled = sceneMaterialsEnabled
        params.sceneLightsEnabled = sceneLightsEnabled
        return params
    }
}

// MARK: - Token Extensions for UsdImaging

extension swiftusd.Token {
    // Material binding purpose tokens
    public static var allPurpose: swiftusd.Token { swiftusd.UsdImagingTokens.AllPurpose() }
    public static var preview: swiftusd.Token { swiftusd.UsdImagingTokens.Preview() }
    public static var full: swiftusd.Token { swiftusd.UsdImagingTokens.Full() }

    // Visibility tokens
    public static var invisible: swiftusd.Token { swiftusd.UsdImagingTokens.Invisible() }
    public static var visible: swiftusd.Token { swiftusd.UsdImagingTokens.Visible() }
    public static var inherited: swiftusd.Token { swiftusd.UsdImagingTokens.Inherited() }

    // Render tag tokens (specific to UsdImaging)
    public static var geometry: swiftusd.Token { swiftusd.UsdImagingTokens.Geometry() }
    public static var guide: swiftusd.Token { swiftusd.UsdImagingTokens.Guide() }
    public static var proxy: swiftusd.Token { swiftusd.UsdImagingTokens.Proxy() }
    public static var render: swiftusd.Token { swiftusd.UsdImagingTokens.Render() }

    // Geometry prim type tokens
    public static var meshType: swiftusd.Token { swiftusd.UsdImagingTokens.Mesh() }
    public static var basisCurvesType: swiftusd.Token { swiftusd.UsdImagingTokens.BasisCurves() }
    public static var pointsType: swiftusd.Token { swiftusd.UsdImagingTokens.Points() }
    public static var sphereType: swiftusd.Token { swiftusd.UsdImagingTokens.Sphere() }
    public static var cubeType: swiftusd.Token { swiftusd.UsdImagingTokens.Cube() }
    public static var cylinderType: swiftusd.Token { swiftusd.UsdImagingTokens.Cylinder() }
    public static var capsuleType: swiftusd.Token { swiftusd.UsdImagingTokens.Capsule() }
    public static var coneType: swiftusd.Token { swiftusd.UsdImagingTokens.Cone() }

    // Camera tokens
    public static var cameraType: swiftusd.Token { swiftusd.UsdImagingTokens.Camera() }
    public static var perspective: swiftusd.Token { swiftusd.UsdImagingTokens.Perspective() }
    public static var orthographic: swiftusd.Token { swiftusd.UsdImagingTokens.Orthographic() }

    // Light type tokens
    public static var domeLight: swiftusd.Token { swiftusd.UsdImagingTokens.DomeLight() }
    public static var distantLight: swiftusd.Token { swiftusd.UsdImagingTokens.DistantLight() }
    public static var sphereLight: swiftusd.Token { swiftusd.UsdImagingTokens.SphereLight() }
    public static var rectLight: swiftusd.Token { swiftusd.UsdImagingTokens.RectLight() }
    public static var diskLight: swiftusd.Token { swiftusd.UsdImagingTokens.DiskLight() }
    public static var cylinderLight: swiftusd.Token { swiftusd.UsdImagingTokens.CylinderLight() }
}

// MARK: - Material Purpose Helpers

/// Get the default material binding purpose.
public func usdImagingDefaultMaterialPurpose() -> swiftusd.Token {
    return swiftusd.UsdImagingGetDefaultMaterialPurpose()
}

/// Get the preview material binding purpose.
public func usdImagingPreviewMaterialPurpose() -> swiftusd.Token {
    return swiftusd.UsdImagingGetPreviewMaterialPurpose()
}

/// Get the full material binding purpose.
public func usdImagingFullMaterialPurpose() -> swiftusd.Token {
    return swiftusd.UsdImagingGetFullMaterialPurpose()
}
