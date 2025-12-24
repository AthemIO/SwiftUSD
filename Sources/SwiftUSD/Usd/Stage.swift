// Stage.swift - UsdStage wrapper
// Mirrors: pxr/usd/usd/stage.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - InitialLoadSet

/// Specifies which prims to load when opening a stage.
public enum InitialLoadSet: Sendable {
    /// Load all loadable prims.
    case all
    /// Load no loadable prims (deferred loading).
    case none

    internal var cValue: UsdInitialLoadSet {
        switch self {
        case .all: return USD_LOAD_ALL
        case .none: return USD_LOAD_NONE
        }
    }
}

// MARK: - Stage

/// The outermost container for scene description.
///
/// A `Stage` provides the complete namespace hierarchy of prims,
/// composed from one or more layers. Layers are organized into a layer stack.
///
/// Stages can be created from scratch, opened from files, or opened
/// from existing layers. Once opened, you can traverse, query, and author
/// prims, attributes, and other scene description.
///
/// Mirrors `pxr::UsdStage` from the USD C++ API.
public final class Stage: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdStageRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdStageRef) {
        self.handle = handle
    }

    deinit {
        UsdStage_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Creates a new stage at the given file path.
    ///
    /// - Parameters:
    ///   - identifier: The file path for the new stage.
    ///   - load: Which prims to load initially.
    /// - Returns: A new stage.
    /// - Throws: `StageError.creationFailed` if the stage cannot be created.
    public static func createNew(
        identifier: String,
        load: InitialLoadSet = .all
    ) throws -> Stage {
        guard let ref = UsdStage_CreateNew(identifier, load.cValue) else {
            throw StageError.creationFailed("Failed to create stage at '\(identifier)'")
        }
        return Stage(handle: ref)
    }

    /// Creates an in-memory stage.
    ///
    /// - Parameters:
    ///   - identifier: An optional identifier for the stage.
    ///   - load: Which prims to load initially.
    /// - Returns: A new in-memory stage.
    /// - Throws: `StageError.creationFailed` if the stage cannot be created.
    public static func createInMemory(
        identifier: String? = nil,
        load: InitialLoadSet = .all
    ) throws -> Stage {
        guard let ref = UsdStage_CreateInMemory(identifier, load.cValue) else {
            throw StageError.creationFailed("Failed to create in-memory stage")
        }
        return Stage(handle: ref)
    }

    /// Opens an existing stage from a file path.
    ///
    /// - Parameters:
    ///   - filePath: The path to the USD file.
    ///   - load: Which prims to load initially.
    /// - Returns: The opened stage.
    /// - Throws: `StageError.openFailed` if the stage cannot be opened.
    public static func open(
        filePath: String,
        load: InitialLoadSet = .all
    ) throws -> Stage {
        guard let ref = UsdStage_Open(filePath, load.cValue) else {
            throw StageError.openFailed("Failed to open stage at '\(filePath)'")
        }
        return Stage(handle: ref)
    }

    /// Opens a stage from an existing root layer.
    ///
    /// - Parameters:
    ///   - rootLayer: The layer to use as the root.
    ///   - load: Which prims to load initially.
    /// - Returns: The opened stage.
    /// - Throws: `StageError.openFailed` if the stage cannot be opened.
    public static func open(
        rootLayer: Layer,
        load: InitialLoadSet = .all
    ) throws -> Stage {
        guard let ref = UsdStage_OpenLayer(rootLayer.handle, load.cValue) else {
            throw StageError.openFailed("Failed to open stage from layer")
        }
        return Stage(handle: ref)
    }

    // MARK: - Layer Access

    /// The stage's root layer.
    public var rootLayer: Layer? {
        guard let ref = UsdStage_GetRootLayer(handle) else {
            return nil
        }
        return Layer(handle: ref)
    }

    /// The stage's session layer.
    public var sessionLayer: Layer? {
        guard let ref = UsdStage_GetSessionLayer(handle) else {
            return nil
        }
        return Layer(handle: ref)
    }

    /// The layers in the stage's layer stack.
    ///
    /// - Parameter includeSessionLayers: Whether to include session layers.
    /// - Returns: Array of layers in the stack.
    public func layerStack(includeSessionLayers: Bool = true) -> [Layer] {
        let count = UsdStage_GetLayerStackCount(handle, includeSessionLayers)
        guard count > 0 else { return [] }

        var handles = [SdfLayerRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdStage_GetLayerStack(handle, buffer.baseAddress, count, includeSessionLayers)
        }

        var result = [Layer]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Layer(handle: h))
            }
        }
        return result
    }

    // MARK: - Time Properties

    /// The start time code of the stage.
    public var startTimeCode: Double {
        get { UsdStage_GetStartTimeCode(handle) }
        set { _ = UsdStage_SetStartTimeCode(handle, newValue) }
    }

    /// The end time code of the stage.
    public var endTimeCode: Double {
        get { UsdStage_GetEndTimeCode(handle) }
        set { _ = UsdStage_SetEndTimeCode(handle, newValue) }
    }

    /// The time codes per second.
    public var timeCodesPerSecond: Double {
        get { UsdStage_GetTimeCodesPerSecond(handle) }
        set { _ = UsdStage_SetTimeCodesPerSecond(handle, newValue) }
    }

    /// The frames per second.
    public var framesPerSecond: Double {
        get { UsdStage_GetFramesPerSecond(handle) }
        set { _ = UsdStage_SetFramesPerSecond(handle, newValue) }
    }

    // MARK: - Prim Access

    /// The pseudo-root prim (parent of all root prims).
    public var pseudoRoot: Prim? {
        guard let ref = UsdStage_GetPseudoRoot(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    /// The default prim for the stage.
    ///
    /// This is the prim that will be targeted by references or payloads
    /// that don't specify a prim path.
    public var defaultPrim: Prim? {
        get {
            guard let ref = UsdStage_GetDefaultPrim(handle) else {
                return nil
            }
            return Prim(handle: ref)
        }
        set {
            if let prim = newValue {
                _ = UsdStage_SetDefaultPrim(handle, prim.handle)
            } else {
                _ = UsdStage_ClearDefaultPrim(handle)
            }
        }
    }

    /// Whether the stage has a default prim set.
    public var hasDefaultPrim: Bool {
        UsdStage_HasDefaultPrim(handle)
    }

    /// Clears the default prim.
    public func clearDefaultPrim() {
        _ = UsdStage_ClearDefaultPrim(handle)
    }

    /// Gets a prim at the specified path.
    ///
    /// - Parameter path: The path to the prim.
    /// - Returns: The prim at the path, or `nil` if not found.
    public func prim(at path: Path) -> Prim? {
        guard let ref = UsdStage_GetPrimAtPath(handle, path.handle) else {
            return nil
        }
        let prim = Prim(handle: ref)
        return prim.isValid ? prim : nil
    }

    /// Gets a prim at the specified path string.
    ///
    /// - Parameter path: The path string to the prim.
    /// - Returns: The prim at the path, or `nil` if not found.
    public func prim(at path: String) -> Prim? {
        guard let ref = UsdStage_GetPrimAtPathString(handle, path) else {
            return nil
        }
        let prim = Prim(handle: ref)
        return prim.isValid ? prim : nil
    }

    // MARK: - Prim Creation

    /// Defines a prim at the given path with the specified type.
    ///
    /// Creates the prim and any necessary ancestors.
    ///
    /// - Parameters:
    ///   - path: The path for the new prim.
    ///   - typeName: The type name for the prim (e.g., "Mesh", "Xform").
    /// - Returns: The newly defined prim.
    /// - Throws: `StageError.primCreationFailed` if the prim cannot be created.
    public func definePrim(at path: Path, typeName: Token? = nil) throws -> Prim {
        guard let ref = UsdStage_DefinePrim(handle, path.handle, typeName?.handle) else {
            throw StageError.primCreationFailed("Failed to define prim at '\(path)'")
        }
        return Prim(handle: ref)
    }

    /// Defines a prim at the given path string with the specified type.
    ///
    /// - Parameters:
    ///   - path: The path string for the new prim.
    ///   - typeName: The type name for the prim (e.g., "Mesh", "Xform").
    /// - Returns: The newly defined prim.
    /// - Throws: `StageError.primCreationFailed` if the prim cannot be created.
    public func definePrim(at path: String, typeName: String? = nil) throws -> Prim {
        guard let ref = UsdStage_DefinePrimString(handle, path, typeName) else {
            throw StageError.primCreationFailed("Failed to define prim at '\(path)'")
        }
        return Prim(handle: ref)
    }

    /// Creates an override prim at the given path.
    ///
    /// Override prims are used to modify existing prims from other layers
    /// without defining a concrete prim.
    ///
    /// - Parameter path: The path for the override prim.
    /// - Returns: The newly created override prim.
    /// - Throws: `StageError.primCreationFailed` if the prim cannot be created.
    public func overridePrim(at path: Path) throws -> Prim {
        guard let ref = UsdStage_OverridePrim(handle, path.handle) else {
            throw StageError.primCreationFailed("Failed to override prim at '\(path)'")
        }
        return Prim(handle: ref)
    }

    /// Creates an override prim at the given path string.
    ///
    /// - Parameter path: The path string for the override prim.
    /// - Returns: The newly created override prim.
    /// - Throws: `StageError.primCreationFailed` if the prim cannot be created.
    public func overridePrim(at path: String) throws -> Prim {
        guard let ref = UsdStage_OverridePrimString(handle, path) else {
            throw StageError.primCreationFailed("Failed to override prim at '\(path)'")
        }
        return Prim(handle: ref)
    }

    /// Creates a class prim at the given root path.
    ///
    /// Class prims define abstract classes that can be inherited.
    ///
    /// - Parameter path: The root path for the class prim.
    /// - Returns: The newly created class prim.
    /// - Throws: `StageError.primCreationFailed` if the prim cannot be created.
    public func createClassPrim(at path: Path) throws -> Prim {
        guard let ref = UsdStage_CreateClassPrim(handle, path.handle) else {
            throw StageError.primCreationFailed("Failed to create class prim at '\(path)'")
        }
        return Prim(handle: ref)
    }

    /// Removes a prim at the given path from the current edit target.
    ///
    /// - Parameter path: The path of the prim to remove.
    /// - Throws: `StageError.operationFailed` if the removal fails.
    public func removePrim(at path: Path) throws {
        let result = UsdStage_RemovePrim(handle, path.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw StageError.operationFailed("Failed to remove prim at '\(path)'")
        }
    }

    // MARK: - Traversal

    /// The root prims of the stage.
    public var rootPrims: [Prim] {
        let count = UsdStage_GetRootPrimCount(handle)
        guard count > 0 else { return [] }

        var handles = [UsdPrimRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdStage_GetRootPrims(handle, buffer.baseAddress, count)
        }

        var result = [Prim]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Prim(handle: h))
            }
        }
        return result
    }

    /// Traverses all prims in the stage.
    ///
    /// Returns all prims reachable via the default traversal predicate
    /// (active, loaded, defined, non-abstract).
    ///
    /// - Returns: Array of all traversed prims.
    public func traverse() -> [Prim] {
        let count = UsdStage_GetTraversalCount(handle)
        guard count > 0 else { return [] }

        var handles = [UsdPrimRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdStage_Traverse(handle, buffer.baseAddress, count)
        }

        var result = [Prim]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Prim(handle: h))
            }
        }
        return result
    }

    // MARK: - Serialization

    /// Saves all dirty layers in the stage's layer stack.
    ///
    /// - Throws: `StageError.saveFailed` if the save fails.
    public func save() throws {
        let result = UsdStage_Save(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw StageError.saveFailed("Failed to save stage")
        }
    }

    /// Exports the flattened stage to a file.
    ///
    /// - Parameters:
    ///   - filename: The path to export to.
    ///   - addSourceFileComment: Whether to add a source file comment.
    /// - Throws: `StageError.exportFailed` if the export fails.
    public func export(
        to filename: String,
        addSourceFileComment: Bool = true
    ) throws {
        let result = UsdStage_Export(handle, filename, addSourceFileComment)
        guard result == USD_RESULT_SUCCESS else {
            throw StageError.exportFailed("Failed to export stage to '\(filename)'")
        }
    }

    /// Exports the flattened stage to a string.
    ///
    /// - Returns: The stage content as a string.
    /// - Throws: `StageError.exportFailed` if the export fails.
    public func exportToString() throws -> String {
        guard let cstr = UsdStage_ExportToString(handle) else {
            throw StageError.exportFailed("Failed to export stage to string")
        }
        defer { UsdStage_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// Reloads all non-session layers in the stage.
    ///
    /// - Throws: `StageError.reloadFailed` if the reload fails.
    public func reload() throws {
        let result = UsdStage_Reload(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw StageError.reloadFailed("Failed to reload stage")
        }
    }
}

// MARK: - CustomStringConvertible

extension Stage: CustomStringConvertible {

    public var description: String {
        rootLayer?.identifier ?? "<anonymous stage>"
    }
}

// MARK: - CustomDebugStringConvertible

extension Stage: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Stage(\"\(rootLayer?.identifier ?? "<anonymous>")\")"
    }
}

// MARK: - StageError

/// Errors that can occur when working with stages.
public enum StageError: Error, CustomStringConvertible {

    /// Stage creation failed.
    case creationFailed(String)

    /// Stage open failed.
    case openFailed(String)

    /// Prim creation failed.
    case primCreationFailed(String)

    /// Stage save failed.
    case saveFailed(String)

    /// Stage export failed.
    case exportFailed(String)

    /// Stage reload failed.
    case reloadFailed(String)

    /// A general operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Stage creation failed: \(message)"
        case .openFailed(let message):
            return "Stage open failed: \(message)"
        case .primCreationFailed(let message):
            return "Prim creation failed: \(message)"
        case .saveFailed(let message):
            return "Stage save failed: \(message)"
        case .exportFailed(let message):
            return "Stage export failed: \(message)"
        case .reloadFailed(let message):
            return "Stage reload failed: \(message)"
        case .operationFailed(let message):
            return "Stage operation failed: \(message)"
        }
    }
}
