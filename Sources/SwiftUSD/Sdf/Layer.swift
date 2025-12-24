// Layer.swift - SdfLayer wrapper
// Mirrors: pxr/usd/sdf/layer.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - SdfLayer

/// A scene description container that can combine with other containers
/// to form assets and aggregates.
///
/// The contents of an `Layer` adhere to the Sdf data model. A layer can be
/// ephemeral, or be an asset accessed and serialized through the asset system.
///
/// Layers can have a time code range (startTimeCode and endTimeCode). This range
/// represents the suggested playback range.
///
/// Mirrors `pxr::SdfLayer` from the USD C++ API.
public final class Layer: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: SdfLayerRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: SdfLayerRef) {
        self.handle = handle
    }

    deinit {
        SdfLayer_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Creates a new empty layer with the given identifier.
    ///
    /// - Parameter identifier: The layer identifier (typically a file path).
    /// - Returns: A new layer.
    /// - Throws: `LayerError.creationFailed` if the layer cannot be created.
    public static func createNew(identifier: String) throws -> Layer {
        guard let ref = SdfLayer_CreateNew(identifier) else {
            throw LayerError.creationFailed("Failed to create layer at '\(identifier)'")
        }
        return Layer(handle: ref)
    }

    /// Creates a new anonymous layer with an optional tag.
    ///
    /// Anonymous layers cannot be saved to disk via `save()`.
    ///
    /// - Parameter tag: An optional tag for debugging purposes.
    /// - Returns: A new anonymous layer.
    /// - Throws: `LayerError.creationFailed` if the layer cannot be created.
    public static func createAnonymous(tag: String? = nil) throws -> Layer {
        guard let ref = SdfLayer_CreateAnonymous(tag) else {
            throw LayerError.creationFailed("Failed to create anonymous layer")
        }
        return Layer(handle: ref)
    }

    /// Opens an existing layer or creates it if it doesn't exist.
    ///
    /// - Parameter identifier: The layer identifier.
    /// - Returns: The found or opened layer.
    /// - Throws: `LayerError.openFailed` if the layer cannot be opened.
    public static func findOrOpen(identifier: String) throws -> Layer {
        guard let ref = SdfLayer_FindOrOpen(identifier) else {
            throw LayerError.openFailed("Failed to find or open layer '\(identifier)'")
        }
        return Layer(handle: ref)
    }

    /// Finds an existing layer by identifier.
    ///
    /// - Parameter identifier: The layer identifier.
    /// - Returns: The found layer, or `nil` if not found.
    public static func find(identifier: String) -> Layer? {
        guard let ref = SdfLayer_Find(identifier) else {
            return nil
        }
        return Layer(handle: ref)
    }

    // MARK: - Properties

    /// The layer's identifier.
    public var identifier: String {
        String(cString: SdfLayer_GetIdentifier(handle))
    }

    /// The layer's display name (base filename of the identifier).
    public var displayName: String {
        guard let cstr = SdfLayer_GetDisplayName(handle) else {
            return ""
        }
        defer { SdfLayer_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// The layer's resolved path.
    public var realPath: String {
        String(cString: SdfLayer_GetRealPath(handle))
    }

    /// The layer's file extension.
    public var fileExtension: String {
        guard let cstr = SdfLayer_GetFileExtension(handle) else {
            return ""
        }
        defer { SdfLayer_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// Returns `true` if the layer is anonymous.
    public var isAnonymous: Bool {
        SdfLayer_IsAnonymous(handle)
    }

    /// Returns `true` if the layer is dirty (has unsaved changes).
    public var isDirty: Bool {
        SdfLayer_IsDirty(handle)
    }

    /// Returns `true` if the layer is empty.
    public var isEmpty: Bool {
        SdfLayer_IsEmpty(handle)
    }

    /// Returns `true` if the layer is muted.
    public var isMuted: Bool {
        SdfLayer_IsMuted(handle)
    }

    // MARK: - Content

    /// Clears all content from the layer.
    ///
    /// - Throws: `LayerError.operationFailed` if the operation fails.
    public func clear() throws {
        let result = SdfLayer_Clear(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.operationFailed("Failed to clear layer")
        }
    }

    /// Saves the layer to its current location.
    ///
    /// - Parameter force: If `true`, forces the save even if the layer is not dirty.
    /// - Throws: `LayerError.saveFailed` if the save fails.
    public func save(force: Bool = false) throws {
        let result = SdfLayer_Save(handle, force)
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.saveFailed("Failed to save layer")
        }
    }

    /// Exports the layer to a new file.
    ///
    /// - Parameters:
    ///   - filename: The path to export to.
    ///   - comment: An optional comment to include.
    /// - Throws: `LayerError.exportFailed` if the export fails.
    public func export(to filename: String, comment: String = "") throws {
        let result = SdfLayer_Export(handle, filename, comment)
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.exportFailed("Failed to export layer to '\(filename)'")
        }
    }

    /// Exports the layer content to a string.
    ///
    /// - Returns: The layer content as a string.
    /// - Throws: `LayerError.exportFailed` if the export fails.
    public func exportToString() throws -> String {
        guard let cstr = SdfLayer_ExportToString(handle) else {
            throw LayerError.exportFailed("Failed to export layer to string")
        }
        defer { SdfLayer_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// Imports content from a string.
    ///
    /// - Parameter content: The content to import.
    /// - Throws: `LayerError.importFailed` if the import fails.
    public func importFromString(_ content: String) throws {
        let result = SdfLayer_ImportFromString(handle, content)
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.importFailed("Failed to import content from string")
        }
    }

    /// Reloads the layer from disk.
    ///
    /// - Parameter force: If `true`, forces the reload even if unchanged.
    /// - Throws: `LayerError.reloadFailed` if the reload fails.
    public func reload(force: Bool = false) throws {
        let result = SdfLayer_Reload(handle, force)
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.reloadFailed("Failed to reload layer")
        }
    }

    // MARK: - Metadata

    /// The default prim name.
    ///
    /// This is the prim that will be targeted by a reference or payload
    /// that doesn't specify a prim path.
    public var defaultPrim: Token? {
        get {
            guard let ref = SdfLayer_GetDefaultPrim(handle) else {
                return nil
            }
            return Token(handle: ref)
        }
        set {
            _ = SdfLayer_SetDefaultPrim(handle, newValue?.handle)
        }
    }

    /// The layer's documentation string.
    public var documentation: String {
        get {
            guard let cstr = SdfLayer_GetDocumentation(handle) else {
                return ""
            }
            defer { SdfLayer_FreeString(cstr) }
            return String(cString: cstr)
        }
        set {
            _ = SdfLayer_SetDocumentation(handle, newValue)
        }
    }

    /// The layer's comment string.
    public var comment: String {
        get {
            guard let cstr = SdfLayer_GetComment(handle) else {
                return ""
            }
            defer { SdfLayer_FreeString(cstr) }
            return String(cString: cstr)
        }
        set {
            _ = SdfLayer_SetComment(handle, newValue)
        }
    }

    /// The layer's start time code.
    public var startTimeCode: Double {
        get { SdfLayer_GetStartTimeCode(handle) }
        set { _ = SdfLayer_SetStartTimeCode(handle, newValue) }
    }

    /// Returns `true` if the layer has a start time code.
    public var hasStartTimeCode: Bool {
        SdfLayer_HasStartTimeCode(handle)
    }

    /// The layer's end time code.
    public var endTimeCode: Double {
        get { SdfLayer_GetEndTimeCode(handle) }
        set { _ = SdfLayer_SetEndTimeCode(handle, newValue) }
    }

    /// Returns `true` if the layer has an end time code.
    public var hasEndTimeCode: Bool {
        SdfLayer_HasEndTimeCode(handle)
    }

    /// The layer's time codes per second.
    public var timeCodesPerSecond: Double {
        get { SdfLayer_GetTimeCodesPerSecond(handle) }
        set { _ = SdfLayer_SetTimeCodesPerSecond(handle, newValue) }
    }

    /// Returns `true` if the layer has time codes per second set.
    public var hasTimeCodesPerSecond: Bool {
        SdfLayer_HasTimeCodesPerSecond(handle)
    }

    /// The layer's frames per second.
    public var framesPerSecond: Double {
        get { SdfLayer_GetFramesPerSecond(handle) }
        set { _ = SdfLayer_SetFramesPerSecond(handle, newValue) }
    }

    /// Returns `true` if the layer has frames per second set.
    public var hasFramesPerSecond: Bool {
        SdfLayer_HasFramesPerSecond(handle)
    }

    // MARK: - Sublayers

    /// The number of sublayers.
    public var subLayerCount: Int {
        Int(SdfLayer_GetNumSubLayerPaths(handle))
    }

    /// The sublayer paths.
    public var subLayerPaths: [String] {
        let count = subLayerCount
        guard count > 0 else { return [] }

        var paths = [UnsafeMutablePointer<CChar>?](repeating: nil, count: count)
        let actualCount = paths.withUnsafeMutableBufferPointer { buffer in
            SdfLayer_GetSubLayerPaths(handle, buffer.baseAddress, count)
        }

        var result = [String]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let cstr = paths[i] {
                result.append(String(cString: cstr))
                free(cstr)
            }
        }
        return result
    }

    /// Inserts a sublayer path at the given index.
    ///
    /// - Parameters:
    ///   - path: The sublayer path to insert.
    ///   - index: The index at which to insert. Use -1 to append.
    /// - Throws: `LayerError.operationFailed` if the operation fails.
    public func insertSubLayerPath(_ path: String, at index: Int = -1) throws {
        let result = SdfLayer_InsertSubLayerPath(handle, path, Int32(index))
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.operationFailed("Failed to insert sublayer path")
        }
    }

    /// Removes a sublayer path at the given index.
    ///
    /// - Parameter index: The index of the sublayer to remove.
    /// - Throws: `LayerError.operationFailed` if the operation fails.
    public func removeSubLayerPath(at index: Int) throws {
        let result = SdfLayer_RemoveSubLayerPath(handle, Int32(index))
        guard result == USD_RESULT_SUCCESS else {
            throw LayerError.operationFailed("Failed to remove sublayer path at index \(index)")
        }
    }

    // MARK: - Path Operations

    /// Returns `true` if a spec exists at the given path.
    ///
    /// - Parameter path: The path to check.
    /// - Returns: `true` if a spec exists at the path.
    public func hasSpec(at path: Path) -> Bool {
        SdfLayer_HasSpec(handle, path.handle)
    }

    // MARK: - Root Prims

    /// The number of root prims.
    public var rootPrimCount: Int {
        Int(SdfLayer_GetRootPrimCount(handle))
    }

    /// The names of the root prims.
    public var rootPrimNames: [Token] {
        let count = rootPrimCount
        guard count > 0 else { return [] }

        var handles = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            SdfLayer_GetRootPrimNames(handle, buffer.baseAddress, count)
        }

        var result = [Token]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Token(handle: h))
            }
        }
        return result
    }
}

// MARK: - CustomStringConvertible

extension Layer: CustomStringConvertible {

    public var description: String {
        identifier
    }
}

// MARK: - CustomDebugStringConvertible

extension Layer: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Layer(\"\(identifier)\")"
    }
}

// MARK: - LayerError

/// Errors that can occur when working with layers.
public enum LayerError: Error, CustomStringConvertible {

    /// Layer creation failed.
    case creationFailed(String)

    /// Layer open failed.
    case openFailed(String)

    /// Layer save failed.
    case saveFailed(String)

    /// Layer export failed.
    case exportFailed(String)

    /// Layer import failed.
    case importFailed(String)

    /// Layer reload failed.
    case reloadFailed(String)

    /// A general operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Layer creation failed: \(message)"
        case .openFailed(let message):
            return "Layer open failed: \(message)"
        case .saveFailed(let message):
            return "Layer save failed: \(message)"
        case .exportFailed(let message):
            return "Layer export failed: \(message)"
        case .importFailed(let message):
            return "Layer import failed: \(message)"
        case .reloadFailed(let message):
            return "Layer reload failed: \(message)"
        case .operationFailed(let message):
            return "Layer operation failed: \(message)"
        }
    }
}
