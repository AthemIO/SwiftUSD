// UsdShade.swift - UsdShade module errors and utilities
// Mirrors: pxr/usd/usdShade/utils.h

import OpenUSDInterop

// MARK: - ShadeError

/// Errors that can occur when working with shading.
public enum ShadeError: Error, CustomStringConvertible {

    /// Material, shader, or node graph creation failed.
    case creationFailed(String)

    /// Connection failed.
    case connectionFailed(String)

    /// Material binding failed.
    case bindingFailed(String)

    /// A general operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Shade creation failed: \(message)"
        case .connectionFailed(let message):
            return "Shade connection failed: \(message)"
        case .bindingFailed(let message):
            return "Shade binding failed: \(message)"
        case .operationFailed(let message):
            return "Shade operation failed: \(message)"
        }
    }
}

// MARK: - ShadeAttributeType

/// The type of a shade attribute (input or output).
public enum ShadeAttributeType: Sendable {
    /// Invalid attribute type.
    case invalid
    /// Input attribute.
    case input
    /// Output attribute.
    case output

    internal init(cValue: UsdShadeAttributeType) {
        switch cValue {
        case USDSHADE_ATTR_TYPE_INPUT:
            self = .input
        case USDSHADE_ATTR_TYPE_OUTPUT:
            self = .output
        default:
            self = .invalid
        }
    }
}

// MARK: - ShadeUtils

/// Utility functions for working with shading.
public enum ShadeUtils {

    /// Gets the input name prefix ("inputs:").
    public static var inputPrefix: Token? {
        guard let ref = UsdShadeUtils_GetInputPrefix() else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Gets the output name prefix ("outputs:").
    public static var outputPrefix: Token? {
        guard let ref = UsdShadeUtils_GetOutputPrefix() else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Checks if a name is an input name (has "inputs:" prefix).
    ///
    /// - Parameter name: The name to check.
    /// - Returns: `true` if the name is an input name.
    public static func isInputName(_ name: String) -> Bool {
        UsdShadeUtils_IsInputName(name)
    }

    /// Checks if a name is an output name (has "outputs:" prefix).
    ///
    /// - Parameter name: The name to check.
    /// - Returns: `true` if the name is an output name.
    public static func isOutputName(_ name: String) -> Bool {
        UsdShadeUtils_IsOutputName(name)
    }

    /// Gets the base name from a full input/output name.
    ///
    /// - Parameter fullName: The full name with prefix.
    /// - Returns: The base name without prefix.
    public static func baseName(from fullName: String) -> Token? {
        guard let ref = UsdShadeUtils_GetBaseNameFromFullName(fullName) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Gets the attribute type from a full name.
    ///
    /// - Parameter fullName: The full name with prefix.
    /// - Returns: The attribute type.
    public static func attributeType(from fullName: String) -> ShadeAttributeType {
        let cValue = UsdShadeUtils_GetTypeFromFullName(fullName)
        return ShadeAttributeType(cValue: cValue)
    }

    /// Creates a full input name from a base name.
    ///
    /// - Parameter baseName: The base name.
    /// - Returns: The full name with "inputs:" prefix.
    public static func inputName(from baseName: Token) -> Token? {
        guard let ref = UsdShadeUtils_CreateInputName(baseName.handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Creates a full output name from a base name.
    ///
    /// - Parameter baseName: The base name.
    /// - Returns: The full name with "outputs:" prefix.
    public static func outputName(from baseName: Token) -> Token? {
        guard let ref = UsdShadeUtils_CreateOutputName(baseName.handle) else {
            return nil
        }
        return Token(handle: ref)
    }
}
