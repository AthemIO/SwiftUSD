// Gf.swift - Gf module namespace and utilities
// Mirrors: pxr/base/gf/

import OpenUSDInterop

// MARK: - Gf Namespace

/// Graphics Foundations (Gf) module namespace.
///
/// The Gf module provides basic math types for 3D graphics:
/// - Vectors: `Vec3f`, `Vec3d`
/// - Matrices: `Matrix4d`
/// - Quaternions: `Quatd`
/// - Ranges: `Range3d`
/// - Bounding boxes: `BBox3d`
///
/// Mirrors `pxr::Gf` namespace from the USD C++ API.
public enum Gf {
    // This enum serves as a namespace for Gf-related types and utilities.
    // All types are defined at the module level for convenience.
}

// MARK: - GfError

/// Errors that can occur when working with Gf types.
public enum GfError: Error, CustomStringConvertible {

    /// A creation operation failed.
    case creationFailed(String)

    /// An operation failed.
    case operationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Gf creation failed: \(message)"
        case .operationFailed(let message):
            return "Gf operation failed: \(message)"
        case .invalidArgument(let message):
            return "Gf invalid argument: \(message)"
        }
    }
}

// MARK: - Type Aliases

/// Type alias for clarity when using float vectors.
public typealias GfVec3f = Vec3f

/// Type alias for clarity when using double vectors.
public typealias GfVec3d = Vec3d

/// Type alias for clarity when using matrices.
public typealias GfMatrix4d = Matrix4d

/// Type alias for clarity when using quaternions.
public typealias GfQuatd = Quatd

/// Type alias for clarity when using ranges.
public typealias GfRange3d = Range3d

/// Type alias for clarity when using bounding boxes.
public typealias GfBBox3d = BBox3d
