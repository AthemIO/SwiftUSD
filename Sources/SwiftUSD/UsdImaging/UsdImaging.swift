// UsdImaging.swift - UsdImaging module namespace (USD Imaging Delegate)
// Mirrors: pxr/usdImaging/usdImaging/

import USDCxx

// MARK: - UsdImaging Namespace

/// UsdImaging module namespace - the bridge between USD and Hydra rendering.
///
/// The UsdImaging module provides the scene delegate that translates USD scene
/// data into Hydra render primitives:
/// - `UsdImagingDelegate`: Main adapter connecting USD to Hydra
/// - `UsdImagingDelegateParams`: Configuration parameters for the delegate
/// - `UsdImagingTokens`: Common tokens used in UsdImaging
///
/// Mirrors `pxr::UsdImaging` namespace from the USD C++ API.
///
/// ## Usage
/// ```swift
/// // Create an imaging delegate with a render index
/// guard let renderIndex = HdRenderIndex.create(renderDelegate: delegate) else { return }
/// guard let imagingDelegate = UsdImagingDelegate.create(
///     renderIndex: renderIndex,
///     delegateId: "/imaging"
/// ) else { return }
///
/// // Populate the delegate from a USD stage
/// imagingDelegate.populate(stage: stage, rootPrimPath: "/")
///
/// // Set time for animation
/// imagingDelegate.time = 1.0
/// imagingDelegate.applyPendingUpdates()
/// ```
public enum UsdImaging {
    // This enum serves as a namespace for UsdImaging-related types and utilities.
    // All types are defined in UsdImagingCxx.swift using the C++ interop layer.
}

// MARK: - UsdImagingError

/// Errors that can occur when working with UsdImaging types.
public enum UsdImagingError: Error, CustomStringConvertible {

    /// A creation operation failed.
    case creationFailed(String)

    /// An operation failed.
    case operationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    /// A population operation failed.
    case populationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "UsdImaging creation failed: \(message)"
        case .operationFailed(let message):
            return "UsdImaging operation failed: \(message)"
        case .invalidArgument(let message):
            return "UsdImaging invalid argument: \(message)"
        case .populationFailed(let message):
            return "UsdImaging population failed: \(message)"
        }
    }
}
