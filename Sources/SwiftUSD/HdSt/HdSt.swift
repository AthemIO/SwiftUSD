// HdSt.swift - HdSt module namespace (Storm Renderer)
// Mirrors: pxr/imaging/hdSt/

import USDCxx

// MARK: - HdSt Namespace

/// Storm (HdSt) module namespace - Pixar's reference GPU renderer.
///
/// The HdSt module provides Storm, Pixar's reference GPU renderer built on Hydra:
/// - `HdStRenderDelegate`: Storm's implementation of HdRenderDelegate
/// - `HdStResourceRegistry`: GPU resource management for Storm
/// - `HdStMeshTopologyInfo`: Mesh topology data for subdivision
/// - `HdStMaterialParam`: Material parameter description
/// - `HdStMaterialBinding`: Material binding information
/// - `HdStDrawItemInfo`: Draw item metadata
/// - `HdStRenderPassStateConfig`: Render pass configuration
/// - `HdStAovDescriptor`: AOV (render output) descriptor
/// - `HdStTextureType`: Texture classification
/// - `HdStComputeQueuePriority`: GPU compute ordering
///
/// Mirrors `pxr::HdSt` namespace from the USD C++ API.
///
/// ## Usage
/// ```swift
/// // Check if Storm is supported
/// if hdStIsSupported() {
///     // Create Storm render delegate
///     guard let delegate = HdStRenderDelegate.create() else { return }
///
///     // Check supported prim types
///     let rprimTypes = delegate.supportedRprimTypes
///
///     // Create mesh topology info
///     let topology = HdStMeshTopologyInfo.make(
///         numFaces: 6, numPoints: 8, numFaceVertices: 24,
///         scheme: HdStMeshTopologyInfo.catmullClark
///     )
///
///     // Create AOV descriptors
///     let colorAov = HdStAovDescriptor.colorAov()
///     let depthAov = HdStAovDescriptor.depthAov()
/// }
/// ```
public enum HdSt {
    // This enum serves as a namespace for HdSt-related types and utilities.
    // All types are defined in HdStCxx.swift using the C++ interop layer.
}

// MARK: - HdStError

/// Errors that can occur when working with HdSt types.
public enum HdStError: Error, CustomStringConvertible {

    /// A creation operation failed.
    case creationFailed(String)

    /// An operation failed.
    case operationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    /// A render operation failed.
    case renderFailed(String)

    /// Storm is not supported on this hardware.
    case notSupported(String)

    /// A resource allocation failed.
    case resourceAllocationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "HdSt creation failed: \(message)"
        case .operationFailed(let message):
            return "HdSt operation failed: \(message)"
        case .invalidArgument(let message):
            return "HdSt invalid argument: \(message)"
        case .renderFailed(let message):
            return "HdSt render failed: \(message)"
        case .notSupported(let message):
            return "HdSt not supported: \(message)"
        case .resourceAllocationFailed(let message):
            return "HdSt resource allocation failed: \(message)"
        }
    }
}
