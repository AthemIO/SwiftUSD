// Hd.swift - Hd module namespace (Hydra Core Framework)
// Mirrors: pxr/imaging/hd/

import USDCxx

// MARK: - Hd Namespace

/// Hydra (Hd) module namespace - the core rendering framework.
///
/// The Hd module provides the core Hydra rendering framework:
/// - `HdEngine`: Application-facing entry point for Hydra rendering
/// - `HdRenderIndex`: Central flattened scene graph representation
/// - `HdRenderDelegate`: Abstract renderer backend interface
/// - `HdChangeTracker`: Change tracking for scene primitives
/// - `HdDriver`: Render device/context handle
/// - `HdDisplayStyle`: Display style settings for prim geometry
/// - `HdReprSelector`: Representation selector for prim rendering
/// - `HdPrimvarDescriptor`: Primvar descriptor metadata
/// - `HdInterpolation`: Primvar interpolation modes
/// - `HdCullStyle`: Face culling modes
///
/// Mirrors `pxr::Hd` namespace from the USD C++ API.
///
/// ## Usage
/// ```swift
/// // Create a Hydra engine
/// guard let engine = HdEngine.create() else { return }
///
/// // Create display style with refinement
/// let style = HdDisplayStyle.make(refineLevel: 2)
///
/// // Create repr selector
/// let selector = HdReprSelector.makeRefined(swiftusd.Token.refined)
/// ```
public enum Hd {
    // This enum serves as a namespace for Hd-related types and utilities.
    // All types are defined in HdCxx.swift using the C++ interop layer.
}

// MARK: - HdError

/// Errors that can occur when working with Hd types.
public enum HdError: Error, CustomStringConvertible {

    /// A creation operation failed.
    case creationFailed(String)

    /// An operation failed.
    case operationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    /// A render operation failed.
    case renderFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Hd creation failed: \(message)"
        case .operationFailed(let message):
            return "Hd operation failed: \(message)"
        case .invalidArgument(let message):
            return "Hd invalid argument: \(message)"
        case .renderFailed(let message):
            return "Hd render failed: \(message)"
        }
    }
}
