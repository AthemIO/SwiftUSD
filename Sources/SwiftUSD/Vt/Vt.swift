// Vt.swift - Vt module exports and common types
// Mirrors: pxr/base/vt/

import OpenUSDInterop

// MARK: - ArrayError

/// Errors that can occur when working with Vt arrays.
public enum ArrayError: Error, CustomStringConvertible {

    /// Array creation failed.
    case creationFailed(String)

    /// Index out of bounds.
    case indexOutOfBounds(Int, Int)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Array creation failed: \(message)"
        case .indexOutOfBounds(let index, let count):
            return "Index \(index) out of bounds (count: \(count))"
        }
    }
}

// MARK: - Type Aliases for Convenience

/// Alias for ArrayInt for naming consistency with USD.
public typealias VtArrayInt = ArrayInt

/// Alias for ArrayFloat for naming consistency with USD.
public typealias VtArrayFloat = ArrayFloat

/// Alias for ArrayDouble for naming consistency with USD.
public typealias VtArrayDouble = ArrayDouble

/// Alias for ArrayVec3f for naming consistency with USD.
public typealias VtArrayVec3f = ArrayVec3f

/// Alias for ArrayVec3d for naming consistency with USD.
public typealias VtArrayVec3d = ArrayVec3d

/// Alias for Value for naming consistency with USD.
public typealias VtValue = Value

/// Alias for VtDict for naming consistency with USD.
public typealias VtDictionary = VtDict
