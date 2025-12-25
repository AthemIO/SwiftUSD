// HdxCxx.swift - Swift wrappers for Hydra Extension Framework
// Provides convenience extensions and typealiases for the Hdx module

import USDCxx

// MARK: - Type Aliases

/// High-level task controller for managing rendering tasks
public typealias HdxTaskController = swiftusd.HdxTaskControllerRef

/// Selection state manager
public typealias HdxSelectionTracker = swiftusd.HdxSelectionTrackerRef

/// Pick hit result
public typealias HdxPickHit = swiftusd.HdxPickHit

/// Render task parameters
public typealias HdxRenderTaskParams = swiftusd.HdxRenderTaskParams

/// Selection task parameters
public typealias HdxSelectionTaskParams = swiftusd.HdxSelectionTaskParams

/// Pick task parameters
public typealias HdxPickTaskParams = swiftusd.HdxPickTaskParams

/// Pick task context parameters
public typealias HdxPickTaskContextParams = swiftusd.HdxPickTaskContextParams

/// Color correction task parameters
public typealias HdxColorCorrectionTaskParams = swiftusd.HdxColorCorrectionTaskParams

/// Bounding box task parameters
public typealias HdxBoundingBoxTaskParams = swiftusd.HdxBoundingBoxTaskParams

/// Shadow task parameters
public typealias HdxShadowTaskParams = swiftusd.HdxShadowTaskParams

// MARK: - Enumerations

/// Pick operation target
public typealias HdxPickTarget = swiftusd.HdxPickTarget

/// Pick resolution strategy
public typealias HdxPickResolveMode = swiftusd.HdxPickResolveMode

// MARK: - HdxTaskControllerRef Extensions

extension swiftusd.HdxTaskControllerRef {
    /// Create a task controller with a render index.
    public static func create(
        renderIndex: swiftusd.HdRenderIndexRef,
        controllerId: swiftusd.Path,
        gpuEnabled: Bool = true
    ) -> swiftusd.HdxTaskControllerRef? {
        return swiftusd.HdxTaskControllerRef.Create(renderIndex, controllerId, gpuEnabled)
    }

    /// Get the render index.
    public var renderIndex: swiftusd.HdRenderIndexRef? {
        return GetRenderIndex()
    }

    /// Get the controller's ID.
    public var controllerId: swiftusd.Path {
        return GetControllerId()
    }

    /// Set the render buffer size.
    public func setRenderBufferSize(_ size: swiftusd.Vec2i) {
        SetRenderBufferSize(size)
    }

    /// Get the render buffer size.
    public var renderBufferSize: swiftusd.Vec2i {
        return GetRenderBufferSize()
    }

    /// Set the viewport.
    public func setViewport(_ viewport: swiftusd.Vec4d) {
        SetRenderViewport(viewport)
    }

    /// Get the viewport.
    public var viewport: swiftusd.Vec4d {
        return GetRenderViewport()
    }

    /// Set the camera path.
    public func setCameraPath(_ path: swiftusd.Path) {
        SetCameraPath(path)
    }

    /// Get the camera path.
    public var cameraPath: swiftusd.Path {
        return GetCameraPath()
    }

    /// Set free camera matrices.
    public func setFreeCameraMatrices(
        view: swiftusd.Matrix4d,
        projection: swiftusd.Matrix4d
    ) {
        SetFreeCameraMatrices(view, projection)
    }

    /// Enable or disable selection.
    public var isSelectionEnabled: Bool {
        get { return IsSelectionEnabled() }
        set { SetEnableSelection(newValue) }
    }

    /// Get/set the selection color.
    public var selectionColor: swiftusd.Vec4f {
        get { return GetSelectionColor() }
        set { SetSelectionColor(newValue) }
    }

    /// Get/set the locate (hover) color.
    public var locateColor: swiftusd.Vec4f {
        get { return GetSelectionLocateColor() }
        set { SetSelectionLocateColor(newValue) }
    }

    /// Enable or disable selection outline.
    public func setSelectionOutline(enabled: Bool, radius: UInt32 = 5) {
        SetSelectionEnableOutline(enabled)
        SetSelectionOutlineRadius(radius)
    }

    /// Enable or disable shadows.
    public var areShadowsEnabled: Bool {
        get { return AreShadowsEnabled() }
        set { SetEnableShadows(newValue) }
    }

    /// Enable or disable presentation.
    public var isPresentationEnabled: Bool {
        get { return IsPresentationEnabled() }
        set { SetEnablePresentation(newValue) }
    }

    /// Check if render has converged.
    public var isConverged: Bool {
        return IsConverged()
    }

    /// Number of rendering task paths.
    public var renderingTaskPathCount: Int {
        return Int(GetRenderingTaskPathCount())
    }

    /// Number of picking task paths.
    public var pickingTaskPathCount: Int {
        return Int(GetPickingTaskPathCount())
    }
}

// MARK: - HdxSelectionTrackerRef Extensions

extension swiftusd.HdxSelectionTrackerRef {
    /// Create a new selection tracker.
    public static func create() -> swiftusd.HdxSelectionTrackerRef? {
        return swiftusd.HdxSelectionTrackerRef.Create()
    }

    /// Get the selection version.
    public var version: Int32 {
        return GetVersion()
    }

    /// Increment the version.
    public func incrementVersion() {
        IncrementVersion()
    }

    /// Clear all selections.
    public func clear() {
        Clear()
    }

    /// Add a path to selection.
    public func addSelection(_ path: swiftusd.Path) {
        AddSelection(path)
    }

    /// Remove a path from selection.
    public func removeSelection(_ path: swiftusd.Path) {
        RemoveSelection(path)
    }

    /// Check if a path is selected.
    public func isSelected(_ path: swiftusd.Path) -> Bool {
        return IsSelected(path)
    }

    /// Number of selected items.
    public var selectionCount: Int {
        return Int(GetNumSelections())
    }

    /// Get a selection at index.
    public func selection(at index: Int) -> swiftusd.Path {
        return GetSelection(index)
    }

    /// Get all selections as an array.
    public var allSelections: [swiftusd.Path] {
        var result: [swiftusd.Path] = []
        for i in 0..<selectionCount {
            result.append(GetSelection(i))
        }
        return result
    }
}

// MARK: - HdxPickHit Extensions

extension swiftusd.HdxPickHit {
    /// Create a pick hit with object ID and hit point.
    public static func make(
        objectId: swiftusd.Path,
        hitPoint: swiftusd.Vec3d,
        depth: Float = 0.0
    ) -> swiftusd.HdxPickHit {
        return swiftusd.HdxPickHit(objectId, hitPoint, depth)
    }

    /// Check if this is a valid hit.
    public var isValid: Bool {
        return IsValid()
    }
}

// MARK: - HdxRenderTaskParams Extensions

extension swiftusd.HdxRenderTaskParams {
    /// Create render task params with viewport.
    public static func make(
        viewport: swiftusd.Vec4d,
        enableLighting: Bool = true
    ) -> swiftusd.HdxRenderTaskParams {
        return swiftusd.HdxRenderTaskParams(viewport, enableLighting)
    }

    /// Create default render task params.
    public static func makeDefault() -> swiftusd.HdxRenderTaskParams {
        return swiftusd.HdxRenderTaskParams()
    }
}

// MARK: - HdxSelectionTaskParams Extensions

extension swiftusd.HdxSelectionTaskParams {
    /// Create selection task params with colors.
    public static func make(
        selectionColor: swiftusd.Vec4f,
        locateColor: swiftusd.Vec4f
    ) -> swiftusd.HdxSelectionTaskParams {
        return swiftusd.HdxSelectionTaskParams(selectionColor, locateColor)
    }

    /// Create default selection task params.
    public static func makeDefault() -> swiftusd.HdxSelectionTaskParams {
        return swiftusd.HdxSelectionTaskParams()
    }

    /// Yellow selection, cyan hover.
    public static var standard: swiftusd.HdxSelectionTaskParams {
        return swiftusd.HdxSelectionTaskParams()
    }
}

// MARK: - HdxPickTaskContextParams Extensions

extension swiftusd.HdxPickTaskContextParams {
    /// Create pick context params with matrices.
    public static func make(
        resolution: swiftusd.Vec2i,
        viewMatrix: swiftusd.Matrix4d,
        projectionMatrix: swiftusd.Matrix4d
    ) -> swiftusd.HdxPickTaskContextParams {
        return swiftusd.HdxPickTaskContextParams(resolution, viewMatrix, projectionMatrix)
    }

    /// Create default pick context params.
    public static func makeDefault() -> swiftusd.HdxPickTaskContextParams {
        return swiftusd.HdxPickTaskContextParams()
    }
}

// MARK: - HdxColorCorrectionTaskParams Extensions

extension swiftusd.HdxColorCorrectionTaskParams {
    /// Create color correction params with mode.
    public static func make(mode: swiftusd.Token) -> swiftusd.HdxColorCorrectionTaskParams {
        return swiftusd.HdxColorCorrectionTaskParams(mode)
    }

    /// Create disabled color correction params.
    public static func disabled() -> swiftusd.HdxColorCorrectionTaskParams {
        return swiftusd.HdxColorCorrectionTaskParams(swiftusd.Token("disabled"))
    }

    /// Create sRGB color correction params.
    public static func sRGB() -> swiftusd.HdxColorCorrectionTaskParams {
        return swiftusd.HdxColorCorrectionTaskParams(swiftusd.Token("sRGB"))
    }
}

// MARK: - HdxBoundingBoxTaskParams Extensions

extension swiftusd.HdxBoundingBoxTaskParams {
    /// Create bounding box params with color.
    public static func make(
        color: swiftusd.Vec4f,
        dashSize: Float = 3.0
    ) -> swiftusd.HdxBoundingBoxTaskParams {
        return swiftusd.HdxBoundingBoxTaskParams(color, dashSize)
    }

    /// Create default bounding box params (yellow).
    public static func makeDefault() -> swiftusd.HdxBoundingBoxTaskParams {
        return swiftusd.HdxBoundingBoxTaskParams()
    }
}

// MARK: - HdxShadowTaskParams Extensions

extension swiftusd.HdxShadowTaskParams {
    /// Create shadow params.
    public static func make(
        enabled: Bool,
        resolution: Int32 = 2048
    ) -> swiftusd.HdxShadowTaskParams {
        return swiftusd.HdxShadowTaskParams(enabled, resolution)
    }

    /// Create default shadow params (enabled, 2048 resolution).
    public static func makeDefault() -> swiftusd.HdxShadowTaskParams {
        return swiftusd.HdxShadowTaskParams()
    }
}

// MARK: - HdxPickTarget Helpers

extension swiftusd.HdxPickTarget {
    /// Get the token for this pick target.
    public var token: swiftusd.Token {
        return swiftusd.HdxGetPickTargetToken(self)
    }

    /// Get the name for this pick target.
    public var name: String {
        return String(cString: swiftusd.HdxGetPickTargetName(self))
    }
}

// MARK: - HdxPickResolveMode Helpers

extension swiftusd.HdxPickResolveMode {
    /// Get the token for this resolve mode.
    public var token: swiftusd.Token {
        return swiftusd.HdxGetResolveModeToken(self)
    }

    /// Get the name for this resolve mode.
    public var name: String {
        return String(cString: swiftusd.HdxGetResolveModeName(self))
    }
}

// MARK: - Token Extensions for Hdx

extension swiftusd.Token {
    // Hdx task context tokens
    public static var pickParams: swiftusd.Token { swiftusd.HdxTokens.PickParams() }
    public static var pickHits: swiftusd.Token { swiftusd.HdxTokens.PickHits() }
    public static var renderParams: swiftusd.Token { swiftusd.HdxTokens.RenderParams() }
    public static var selectionState: swiftusd.Token { swiftusd.HdxTokens.SelectionState() }

    // Pick target tokens
    public static var pickPrimsAndInstances: swiftusd.Token { swiftusd.HdxTokens.PickPrimsAndInstances() }
    public static var pickFaces: swiftusd.Token { swiftusd.HdxTokens.PickFaces() }
    public static var pickEdges: swiftusd.Token { swiftusd.HdxTokens.PickEdges() }
    public static var pickPoints: swiftusd.Token { swiftusd.HdxTokens.PickPoints() }
    public static var pickPointsAndInstances: swiftusd.Token { swiftusd.HdxTokens.PickPointsAndInstances() }

    // Resolve mode tokens
    public static var resolveNearestToCamera: swiftusd.Token { swiftusd.HdxTokens.ResolveNearestToCamera() }
    public static var resolveNearestToCenter: swiftusd.Token { swiftusd.HdxTokens.ResolveNearestToCenter() }
    public static var resolveUnique: swiftusd.Token { swiftusd.HdxTokens.ResolveUnique() }
    public static var resolveAll: swiftusd.Token { swiftusd.HdxTokens.ResolveAll() }
    public static var resolveDeep: swiftusd.Token { swiftusd.HdxTokens.ResolveDeep() }
}

// MARK: - Global Functions

/// Get the token for a pick target.
public func hdxGetPickTargetToken(_ target: swiftusd.HdxPickTarget) -> swiftusd.Token {
    return swiftusd.HdxGetPickTargetToken(target)
}

/// Get the token for a resolve mode.
public func hdxGetResolveModeToken(_ mode: swiftusd.HdxPickResolveMode) -> swiftusd.Token {
    return swiftusd.HdxGetResolveModeToken(mode)
}

/// Get the name of a pick target.
public func hdxGetPickTargetName(_ target: swiftusd.HdxPickTarget) -> String {
    return String(cString: swiftusd.HdxGetPickTargetName(target))
}

/// Get the name of a resolve mode.
public func hdxGetResolveModeName(_ mode: swiftusd.HdxPickResolveMode) -> String {
    return String(cString: swiftusd.HdxGetResolveModeName(mode))
}
