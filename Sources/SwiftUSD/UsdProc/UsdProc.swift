// UsdProc.swift - UsdProc module Swift wrappers
// Mirrors: pxr/usd/usdProc/*.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - ProcError

/// Errors that can occur when working with UsdProc types.
public enum ProcError: Error, CustomStringConvertible {

    /// Failed to define a procedural prim.
    case definitionFailed(String)

    /// Failed to get a procedural prim.
    case getFailed(String)

    /// A procedural operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .definitionFailed(let message):
            return "Procedural definition failed: \(message)"
        case .getFailed(let message):
            return "Procedural get failed: \(message)"
        case .operationFailed(let message):
            return "Procedural operation failed: \(message)"
        }
    }
}

// MARK: - ProcGenerativeProcedural

/// A UsdProcGenerativeProcedural represents an abstract generative procedural prim.
///
/// This schema delivers its input parameters via properties (including relationships)
/// within the "primvars:" namespace. It does not itself have any awareness or
/// participation in the execution of the procedural but rather serves as a means
/// of delivering a procedural's definition and input parameters.
///
/// The value of its "proceduralSystem" property (either authored or provided
/// by API schema fallback) indicates to which system the procedural definition
/// is meaningful.
///
/// Mirrors `pxr::UsdProcGenerativeProcedural` from the USD C++ API.
public final class ProcGenerativeProcedural: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdProcGenerativeProceduralRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdProcGenerativeProceduralRef) {
        self.handle = handle
    }

    deinit {
        UsdProcGenerativeProcedural_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new GenerativeProcedural prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new GenerativeProcedural prim.
    /// - Returns: The newly defined GenerativeProcedural.
    /// - Throws: `ProcError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> ProcGenerativeProcedural {
        guard let ref = UsdProcGenerativeProcedural_Define(stage.handle, path.handle) else {
            throw ProcError.definitionFailed("Failed to define GenerativeProcedural at '\(path)'")
        }
        return ProcGenerativeProcedural(handle: ref)
    }

    /// Gets a GenerativeProcedural at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to get the prim from.
    ///   - path: The path of the GenerativeProcedural prim.
    /// - Returns: The GenerativeProcedural at the path.
    /// - Throws: `ProcError.getFailed` if the prim cannot be found or is not a GenerativeProcedural.
    public static func get(on stage: Stage, at path: Path) throws -> ProcGenerativeProcedural {
        guard let ref = UsdProcGenerativeProcedural_Get(stage.handle, path.handle) else {
            throw ProcError.getFailed("Failed to get GenerativeProcedural at '\(path)'")
        }
        let proc = ProcGenerativeProcedural(handle: ref)
        guard proc.isValid else {
            throw ProcError.getFailed("GenerativeProcedural at '\(path)' is not valid")
        }
        return proc
    }

    /// Creates a ProcGenerativeProcedural schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A ProcGenerativeProcedural wrapper, or `nil` if the prim is not a valid GenerativeProcedural.
    public static func from(prim: Prim) -> ProcGenerativeProcedural? {
        guard let ref = UsdProcGenerativeProcedural_FromPrim(prim.handle) else {
            return nil
        }
        let proc = ProcGenerativeProcedural(handle: ref)
        return proc.isValid ? proc : nil
    }

    // MARK: - Validity

    /// Returns `true` if the procedural is valid.
    public var isValid: Bool {
        UsdProcGenerativeProcedural_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdProcGenerativeProcedural_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - ProceduralSystem Attribute

    /// Gets the proceduralSystem token value at the given time.
    ///
    /// The procedural system is the name or convention of the system responsible
    /// for evaluating the procedural.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: The procedural system name, or `nil` if not set.
    public func proceduralSystem(at time: TimeCode = .default) -> String? {
        guard let cStr = UsdProcGenerativeProcedural_GetProceduralSystem(handle, time.cTimeCode) else {
            return nil
        }
        let result = String(cString: cStr)
        free(cStr)
        return result.isEmpty ? nil : result
    }

    /// Sets the proceduralSystem token value at the given time.
    ///
    /// - Parameters:
    ///   - system: The name of the procedural system.
    ///   - time: The time code at which to set the value.
    /// - Throws: `ProcError.operationFailed` if the operation fails.
    public func setProceduralSystem(_ system: String, at time: TimeCode = .default) throws {
        let result = UsdProcGenerativeProcedural_SetProceduralSystem(handle, system, time.cTimeCode)
        guard result == USD_RESULT_SUCCESS else {
            throw ProcError.operationFailed("Failed to set procedural system")
        }
    }

    /// Creates the proceduralSystem attribute if it doesn't exist.
    ///
    /// - Parameter defaultValue: Optional default value for the attribute.
    /// - Returns: `true` if the attribute was successfully created or already exists.
    @discardableResult
    public func createProceduralSystemAttr(defaultValue: String? = nil) -> Bool {
        UsdProcGenerativeProcedural_CreateProceduralSystemAttr(handle, defaultValue)
    }

    /// Checks if the proceduralSystem attribute has an authored value.
    public var hasProceduralSystem: Bool {
        UsdProcGenerativeProcedural_HasProceduralSystem(handle)
    }

    // MARK: - Extent (Bounding Box)

    /// A struct representing a 3D bounding box extent.
    public struct Extent: Equatable, Sendable {
        /// Minimum corner of the bounding box.
        public var min: (x: Float, y: Float, z: Float)
        /// Maximum corner of the bounding box.
        public var max: (x: Float, y: Float, z: Float)

        public init(min: (x: Float, y: Float, z: Float), max: (x: Float, y: Float, z: Float)) {
            self.min = min
            self.max = max
        }

        public static func == (lhs: Extent, rhs: Extent) -> Bool {
            lhs.min.x == rhs.min.x && lhs.min.y == rhs.min.y && lhs.min.z == rhs.min.z &&
            lhs.max.x == rhs.max.x && lhs.max.y == rhs.max.y && lhs.max.z == rhs.max.z
        }
    }

    /// Gets the extent (bounding box) at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: The extent, or `nil` on failure.
    public func extent(at time: TimeCode = .default) -> Extent? {
        var values: [Float] = [0, 0, 0, 0, 0, 0]
        let result = values.withUnsafeMutableBufferPointer { buffer in
            UsdProcGenerativeProcedural_GetExtent(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return Extent(
            min: (values[0], values[1], values[2]),
            max: (values[3], values[4], values[5])
        )
    }

    /// Sets the extent (bounding box) at the given time.
    ///
    /// - Parameters:
    ///   - extent: The bounding box extent.
    ///   - time: The time code at which to set the value.
    /// - Throws: `ProcError.operationFailed` if the operation fails.
    public func setExtent(_ extent: Extent, at time: TimeCode = .default) throws {
        let values: [Float] = [
            extent.min.x, extent.min.y, extent.min.z,
            extent.max.x, extent.max.y, extent.max.z
        ]
        let result = values.withUnsafeBufferPointer { buffer in
            UsdProcGenerativeProcedural_SetExtent(handle, buffer.baseAddress, time.cTimeCode)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw ProcError.operationFailed("Failed to set extent")
        }
    }
}

// MARK: - CustomStringConvertible

extension ProcGenerativeProcedural: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid generative procedural>"
    }
}

// MARK: - Type Aliases

/// Type alias for UsdProcGenerativeProcedural
public typealias UsdProcGenerativeProcedural = ProcGenerativeProcedural
