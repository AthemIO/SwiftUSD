// Camera.swift - UsdGeomCamera wrapper
// Mirrors: pxr/usdGeom/camera.h

import OpenUSDInterop

/// A camera prim defines viewing parameters.
/// Mirrors pxr::UsdGeomCamera from the USD C++ API.
public struct UsdGeomCamera: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Returns true if the prim is a valid camera.
    public var isValid: Bool {
        return prim.isValid
    }

    // MARK: - Define

    /// Defines a new camera at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdGeomCamera? {
        guard let prim = stage.definePrim(at: path, type: "Camera") else {
            return nil
        }
        return UsdGeomCamera(prim)
    }

    // MARK: - Projection

    /// Returns the projection attribute.
    public var projectionAttr: UsdAttribute? {
        return prim.attribute("projection")
    }

    /// Gets the projection type.
    public func getProjection(at time: UsdTimeCode = .default) -> Projection {
        // TODO: Implement via C interop
        return .perspective
    }

    /// Sets the projection type.
    public func setProjection(_ projection: Projection, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Focal Length

    /// Returns the focal length attribute.
    public var focalLengthAttr: UsdAttribute? {
        return prim.attribute("focalLength")
    }

    /// Gets the focal length in mm.
    public func getFocalLength(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 50.0
    }

    /// Sets the focal length in mm.
    public func setFocalLength(_ length: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Aperture

    /// Gets the horizontal aperture in mm.
    public func getHorizontalAperture(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 36.0
    }

    /// Sets the horizontal aperture in mm.
    public func setHorizontalAperture(_ aperture: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Gets the vertical aperture in mm.
    public func getVerticalAperture(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 24.0
    }

    /// Sets the vertical aperture in mm.
    public func setVerticalAperture(_ aperture: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Clipping

    /// Gets the near clipping plane distance.
    public func getClippingRange(at time: UsdTimeCode = .default) -> (near: Float, far: Float) {
        // TODO: Implement via C interop
        return (0.1, 10000.0)
    }

    /// Sets the clipping range.
    public func setClippingRange(near: Float, far: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Supporting Types

    /// Camera projection type.
    public enum Projection: String, Sendable {
        case perspective
        case orthographic
    }
}
