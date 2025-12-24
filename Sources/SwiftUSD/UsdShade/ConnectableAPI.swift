// ConnectableAPI.swift - UsdShadeConnectableAPI wrapper
// Mirrors: pxr/usd/usdShade/connectableAPI.h

import OpenUSDInterop

// MARK: - ConnectableAPI

/// API for managing connections in shading networks.
///
/// The ConnectableAPI provides a generic interface for working with
/// shader connections. It is automatically available on all shaders,
/// materials, and node graphs.
///
/// Mirrors `pxr::UsdShadeConnectableAPI` from the USD C++ API.
public final class ConnectableAPI: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeConnectableAPIRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeConnectableAPIRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeConnectableAPI_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Gets the ConnectableAPI from a prim.
    ///
    /// - Parameter prim: The prim to get the API from.
    /// - Returns: The API, or `nil` if not available.
    public static func get(from prim: Prim) -> ConnectableAPI? {
        guard let ref = UsdShadeConnectableAPI_Get(prim.handle) else {
            return nil
        }
        return ConnectableAPI(handle: ref)
    }

    // MARK: - Properties

    /// Whether the API is valid.
    public var isValid: Bool {
        UsdShadeConnectableAPI_IsValid(handle)
    }

    /// Whether the prim is a container (NodeGraph or Material).
    public var isContainer: Bool {
        UsdShadeConnectableAPI_IsContainer(handle)
    }

    /// Whether the prim requires encapsulation.
    public var requiresEncapsulation: Bool {
        UsdShadeConnectableAPI_RequiresEncapsulation(handle)
    }

    /// The underlying prim.
    public var prim: Prim? {
        guard let ref = UsdShadeConnectableAPI_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Static Connection Utilities

    /// Checks if an input can connect to a source output.
    ///
    /// - Parameters:
    ///   - input: The input to check.
    ///   - source: The source output.
    /// - Returns: `true` if the connection is allowed.
    public static func canConnect(input: Input, to source: Output) -> Bool {
        UsdShadeConnectableAPI_CanConnectInput(input.handle, source.handle)
    }

    /// Checks if an output can connect to a source output.
    ///
    /// - Parameters:
    ///   - output: The output to check.
    ///   - source: The source output.
    /// - Returns: `true` if the connection is allowed.
    public static func canConnect(output: Output, to source: Output) -> Bool {
        UsdShadeConnectableAPI_CanConnectOutput(output.handle, source.handle)
    }
}

// MARK: - CustomStringConvertible

extension ConnectableAPI: CustomStringConvertible {

    public var description: String {
        if let p = prim {
            return "ConnectableAPI(\(p.path.description))"
        }
        return "<invalid ConnectableAPI>"
    }
}
