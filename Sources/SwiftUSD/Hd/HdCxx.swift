// HdCxx.swift - Swift wrappers for Hydra Core Framework
// Provides convenience extensions and typealiases for the Hd module

import USDCxx

// MARK: - Type Aliases

/// Render device/context handle passed to render delegates
public typealias HdDriver = swiftusd.HdDriverRef

/// Change tracker for scene primitives
public typealias HdChangeTracker = swiftusd.HdChangeTrackerRef

/// Abstract renderer backend interface
public typealias HdRenderDelegate = swiftusd.HdRenderDelegateRef

/// Central flattened scene graph representation
public typealias HdRenderIndex = swiftusd.HdRenderIndexRef

/// Application-facing entry point for Hydra
public typealias HdEngine = swiftusd.HdEngineRef

/// Representation selector for prim rendering
public typealias HdReprSelector = swiftusd.HdReprSelector

/// Display style settings for prim geometry
public typealias HdDisplayStyle = swiftusd.HdDisplayStyle

/// Primvar descriptor metadata
public typealias HdPrimvarDescriptor = swiftusd.HdPrimvarDescriptor

/// Render setting descriptor
public typealias HdRenderSettingDescriptor = swiftusd.HdRenderSettingDescriptor

// MARK: - Enumerations

/// Primvar interpolation modes
public typealias HdInterpolation = swiftusd.HdInterpolation

/// Face culling modes
public typealias HdCullStyle = swiftusd.HdCullStyle

// MARK: - HdDriverRef Extensions

extension swiftusd.HdDriverRef {
    /// Create a driver with the given name.
    public static func create(name: swiftusd.Token) -> swiftusd.HdDriverRef? {
        return swiftusd.HdDriverRef.Create(name)
    }

    /// Create a driver with name and associated Hgi instance.
    public static func create(name: swiftusd.Token, hgi: swiftusd.HgiRef) -> swiftusd.HdDriverRef? {
        return swiftusd.HdDriverRef.CreateWithHgi(name, hgi)
    }

    /// Convenience initializer using string name.
    public static func create(name: String) -> swiftusd.HdDriverRef? {
        return swiftusd.HdDriverRef.Create(swiftusd.Token(name))
    }
}

// MARK: - HdRenderIndexRef Extensions

extension swiftusd.HdRenderIndexRef {
    /// Create a new render index with the given render delegate.
    public static func create(renderDelegate: swiftusd.HdRenderDelegateRef) -> swiftusd.HdRenderIndexRef? {
        return swiftusd.HdRenderIndexRef.Create(renderDelegate)
    }

    /// Create a new render index with render delegate and driver.
    public static func create(renderDelegate: swiftusd.HdRenderDelegateRef, driver: swiftusd.HdDriverRef) -> swiftusd.HdRenderIndexRef? {
        return swiftusd.HdRenderIndexRef.CreateWithDrivers(renderDelegate, driver)
    }

    /// Get the instance name as a String.
    public var instanceName: String {
        return String(cString: GetInstanceName())
    }

    /// Check if the given rprim type is supported.
    public func isRprimTypeSupported(_ typeId: String) -> Bool {
        return IsRprimTypeSupported(swiftusd.Token(typeId))
    }

    /// Check if the given sprim type is supported.
    public func isSprimTypeSupported(_ typeId: String) -> Bool {
        return IsSprimTypeSupported(swiftusd.Token(typeId))
    }

    /// Check if the given bprim type is supported.
    public func isBprimTypeSupported(_ typeId: String) -> Bool {
        return IsBprimTypeSupported(swiftusd.Token(typeId))
    }

    /// Check if an rprim with the given path exists.
    public func hasRprim(_ path: swiftusd.Path) -> Bool {
        return HasRprim(path)
    }

    /// Get the number of rprims in the index.
    public var rprimCount: Int {
        return Int(GetRprimCount())
    }
}

// MARK: - HdEngineRef Extensions

extension swiftusd.HdEngineRef {
    /// Create a new Hydra engine.
    public static func create() -> swiftusd.HdEngineRef? {
        return swiftusd.HdEngineRef.Create()
    }

    /// Set a string value in the task context.
    public func setTaskContextData(id: String, value: String) {
        SetTaskContextData(swiftusd.Token(id), value)
    }

    /// Remove a value from the task context.
    public func removeTaskContextData(id: String) {
        RemoveTaskContextData(swiftusd.Token(id))
    }
}

// MARK: - HdChangeTrackerRef Extensions

extension swiftusd.HdChangeTrackerRef {
    /// Mark an rprim dirty.
    public func markRprimDirty(_ id: swiftusd.Path, bits: UInt32 = swiftusd.HdDirtyBitsAllDirty) {
        MarkRprimDirty(id, bits)
    }

    /// Mark an sprim dirty.
    public func markSprimDirty(_ id: swiftusd.Path, bits: UInt32 = swiftusd.HdDirtyBitsAllDirty) {
        MarkSprimDirty(id, bits)
    }

    /// Mark a bprim dirty.
    public func markBprimDirty(_ id: swiftusd.Path, bits: UInt32 = swiftusd.HdDirtyBitsAllDirty) {
        MarkBprimDirty(id, bits)
    }

    /// Check if an rprim is dirty.
    public func isRprimDirty(_ id: swiftusd.Path) -> Bool {
        return IsRprimDirty(id)
    }
}

// MARK: - HdRenderDelegateRef Extensions

extension swiftusd.HdRenderDelegateRef {
    /// Get supported rprim types as an array of Tokens.
    public var supportedRprimTypes: [swiftusd.Token] {
        return GetSupportedRprimTypes()
    }

    /// Get supported sprim types as an array of Tokens.
    public var supportedSprimTypes: [swiftusd.Token] {
        return GetSupportedSprimTypes()
    }

    /// Get supported bprim types as an array of Tokens.
    public var supportedBprimTypes: [swiftusd.Token] {
        return GetSupportedBprimTypes()
    }

    /// Check if the delegate supports pausing.
    public var isPauseSupported: Bool {
        return IsPauseSupported()
    }

    /// Check if the delegate is currently paused.
    public var isPaused: Bool {
        return IsPaused()
    }

    /// Check if the delegate supports stopping.
    public var isStopSupported: Bool {
        return IsStopSupported()
    }

    /// Check if the delegate is currently stopped.
    public var isStopped: Bool {
        return IsStopped()
    }

    /// Pause the render delegate's background threads.
    @discardableResult
    public func pause() -> Bool {
        return Pause()
    }

    /// Resume the render delegate's background threads.
    @discardableResult
    public func resume() -> Bool {
        return Resume()
    }

    /// Stop the render delegate's background threads.
    @discardableResult
    public func stop(blocking: Bool = true) -> Bool {
        return Stop(blocking)
    }

    /// Restart the render delegate's background threads.
    @discardableResult
    public func restart() -> Bool {
        return Restart()
    }
}

// MARK: - HdDisplayStyle Extensions

extension swiftusd.HdDisplayStyle {
    /// Create a default display style.
    public static func makeDefault() -> swiftusd.HdDisplayStyle {
        return swiftusd.HdDisplayStyle()
    }

    /// Create a display style with the given refine level.
    public static func make(refineLevel: Int32, flatShading: Bool = false, displacement: Bool = true) -> swiftusd.HdDisplayStyle {
        return swiftusd.HdDisplayStyle(refineLevel, flatShading, displacement)
    }
}

// MARK: - HdReprSelector Extensions

extension swiftusd.HdReprSelector {
    /// Create an empty repr selector.
    public static func makeEmpty() -> swiftusd.HdReprSelector {
        return swiftusd.HdReprSelector()
    }

    /// Create a repr selector with the refined representation.
    public static func makeRefined(_ refined: swiftusd.Token) -> swiftusd.HdReprSelector {
        return swiftusd.HdReprSelector(refined)
    }

    /// Create a repr selector with refined and unrefined representations.
    public static func make(refined: swiftusd.Token, unrefined: swiftusd.Token) -> swiftusd.HdReprSelector {
        return swiftusd.HdReprSelector(refined, unrefined)
    }

    /// Create a repr selector with all three representations.
    public static func make(refined: swiftusd.Token, unrefined: swiftusd.Token, points: swiftusd.Token) -> swiftusd.HdReprSelector {
        return swiftusd.HdReprSelector(refined, unrefined, points)
    }

    /// Check if this selector is empty.
    public var isEmpty: Bool {
        return IsEmpty()
    }

    /// Check if this selector contains the given repr token.
    public func contains(_ reprToken: swiftusd.Token) -> Bool {
        return Contains(reprToken)
    }
}

// MARK: - HdPrimvarDescriptor Extensions

extension swiftusd.HdPrimvarDescriptor {
    /// Create a primvar descriptor with the given name and interpolation.
    public static func make(name: swiftusd.Token, interpolation: swiftusd.HdInterpolation, role: swiftusd.Token = swiftusd.Token()) -> swiftusd.HdPrimvarDescriptor {
        return swiftusd.HdPrimvarDescriptor(name, interpolation, role)
    }
}

// MARK: - Token Convenience

extension swiftusd.Token {
    // Standard Hydra representation tokens
    public static var refined: swiftusd.Token { swiftusd.HdTokens.Refined() }
    public static var hull: swiftusd.Token { swiftusd.HdTokens.Unrefined() }
    public static var points: swiftusd.Token { swiftusd.HdTokens.Points() }
    public static var wire: swiftusd.Token { swiftusd.HdTokens.Wire() }
    public static var wireOnSurf: swiftusd.Token { swiftusd.HdTokens.WireOnSurf() }

    // Standard prim type tokens
    public static var mesh: swiftusd.Token { swiftusd.HdTokens.Mesh() }
    public static var basisCurves: swiftusd.Token { swiftusd.HdTokens.BasisCurves() }
    public static var volume: swiftusd.Token { swiftusd.HdTokens.Volume() }

    // Sprim type tokens
    public static var camera: swiftusd.Token { swiftusd.HdTokens.Camera() }
    public static var light: swiftusd.Token { swiftusd.HdTokens.Light() }
    public static var material: swiftusd.Token { swiftusd.HdTokens.Material() }

    // Bprim type tokens
    public static var texture: swiftusd.Token { swiftusd.HdTokens.Texture() }
    public static var renderBuffer: swiftusd.Token { swiftusd.HdTokens.RenderBuffer() }

    // Driver tokens
    public static var renderDriver: swiftusd.Token { swiftusd.HdTokens.RenderDriver() }
    public static var hgiDriver: swiftusd.Token { swiftusd.HdTokens.HgiDriver() }
}

// MARK: - Interpolation Helpers

extension swiftusd.HdInterpolation {
    /// Get the string name for this interpolation mode.
    public var name: String {
        return String(cString: swiftusd.HdGetInterpolationName(self))
    }
}

// MARK: - CullStyle Helpers

extension swiftusd.HdCullStyle {
    /// Get the string name for this cull style.
    public var name: String {
        return String(cString: swiftusd.HdGetCullStyleName(self))
    }
}
