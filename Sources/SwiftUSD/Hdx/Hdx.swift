// Hdx.swift - Hdx module namespace (Hydra Extension Framework)
// Mirrors: pxr/imaging/hdx/

import USDCxx

// MARK: - Hdx Namespace

/// Hydra Extension (Hdx) module namespace - high-level rendering utilities.
///
/// The Hdx module provides high-level rendering tasks and utilities built
/// on top of the core Hydra (Hd) framework:
/// - `HdxTaskController`: High-level task management for rendering
/// - `HdxSelectionTracker`: Selection state management
/// - `HdxPickHit`: Pick hit result structure
/// - `HdxRenderTaskParams`: Render task configuration
/// - `HdxSelectionTaskParams`: Selection highlighting configuration
/// - `HdxPickTaskParams`: Pick task configuration
/// - `HdxPickTaskContextParams`: Pick operation context
/// - `HdxColorCorrectionTaskParams`: Color correction configuration
/// - `HdxBoundingBoxTaskParams`: Bounding box visualization
/// - `HdxShadowTaskParams`: Shadow rendering configuration
/// - `HdxPickTarget`: Pick operation target enumeration
/// - `HdxPickResolveMode`: Pick resolution strategy enumeration
///
/// Mirrors `pxr::Hdx` namespace from the USD C++ API.
///
/// ## Usage
/// ```swift
/// // Create a selection tracker
/// guard let tracker = HdxSelectionTracker.create() else { return }
/// tracker.addSelection(swiftusd.Path("/World/Mesh"))
///
/// // Create render task params
/// let params = HdxRenderTaskParams.make(
///     viewport: swiftusd.Vec4d(0, 0, 1920, 1080),
///     enableLighting: true
/// )
///
/// // Create selection task params with custom colors
/// let selectionParams = HdxSelectionTaskParams.make(
///     selectionColor: swiftusd.Vec4f(1.0, 1.0, 0.0, 0.5),  // Yellow
///     locateColor: swiftusd.Vec4f(0.0, 1.0, 1.0, 0.5)      // Cyan
/// )
/// ```
public enum Hdx {
    // This enum serves as a namespace for Hdx-related types and utilities.
    // All types are defined in HdxCxx.swift using the C++ interop layer.
}

// MARK: - HdxError

/// Errors that can occur when working with Hdx types.
public enum HdxError: Error, CustomStringConvertible {

    /// A creation operation failed.
    case creationFailed(String)

    /// An operation failed.
    case operationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    /// A render operation failed.
    case renderFailed(String)

    /// A picking operation failed.
    case pickFailed(String)

    /// A selection operation failed.
    case selectionFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Hdx creation failed: \(message)"
        case .operationFailed(let message):
            return "Hdx operation failed: \(message)"
        case .invalidArgument(let message):
            return "Hdx invalid argument: \(message)"
        case .renderFailed(let message):
            return "Hdx render failed: \(message)"
        case .pickFailed(let message):
            return "Hdx pick failed: \(message)"
        case .selectionFailed(let message):
            return "Hdx selection failed: \(message)"
        }
    }
}
