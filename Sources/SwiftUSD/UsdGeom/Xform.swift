// Xform.swift - UsdGeomXform wrapper
// Mirrors: pxr/usdGeom/xform.h

import OpenUSDInterop

/// An Xform is a transformable prim (provides local transformation).
/// Mirrors pxr::UsdGeomXform from the USD C++ API.
public struct UsdGeomXform: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Returns true if the prim is a valid Xform.
    public var isValid: Bool {
        return prim.isValid
    }

    // MARK: - Define

    /// Defines a new Xform at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdGeomXform? {
        guard let prim = stage.definePrim(at: path, type: "Xform") else {
            return nil
        }
        return UsdGeomXform(prim)
    }

    // MARK: - Transform Operations

    /// Adds a translate operation.
    public func addTranslateOp() -> UsdGeomXformOp? {
        // TODO: Implement via C interop
        return nil
    }

    /// Adds a rotate XYZ operation.
    public func addRotateXYZOp() -> UsdGeomXformOp? {
        // TODO: Implement via C interop
        return nil
    }

    /// Adds a scale operation.
    public func addScaleOp() -> UsdGeomXformOp? {
        // TODO: Implement via C interop
        return nil
    }

    /// Adds a 4x4 transform operation.
    public func addTransformOp() -> UsdGeomXformOp? {
        // TODO: Implement via C interop
        return nil
    }

    /// Gets the ordered transform operations.
    public func getOrderedXformOps() -> [UsdGeomXformOp] {
        // TODO: Implement via C interop
        return []
    }

    /// Gets the local transformation matrix at the given time.
    public func getLocalTransformation(at time: UsdTimeCode = .default) -> GfMatrix4d {
        // TODO: Implement via C interop
        return GfMatrix4d.identity
    }

    /// Sets the xformOp order reset flag.
    public func setResetXformStack(_ reset: Bool) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - XformOp

/// Represents a single transform operation.
/// Mirrors pxr::UsdGeomXformOp from the USD C++ API.
public struct UsdGeomXformOp: Sendable {
    internal var handle: OpaquePointer?

    internal init(handle: OpaquePointer?) {
        self.handle = handle
    }

    /// Returns the underlying attribute.
    public var attr: UsdAttribute {
        // TODO: Implement via C interop
        return UsdAttribute(handle: nil)
    }

    /// Gets the operation type.
    public var opType: OpType {
        // TODO: Implement via C interop
        return .translate
    }

    /// Sets the value at the given time.
    public func set<T>(_ value: T, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Gets the value at the given time.
    public func get<T>(at time: UsdTimeCode = .default) -> T? {
        // TODO: Implement via C interop
        return nil
    }

    /// The type of transform operation.
    public enum OpType: Sendable {
        case translate
        case scale
        case rotateX
        case rotateY
        case rotateZ
        case rotateXYZ
        case rotateXZY
        case rotateYXZ
        case rotateYZX
        case rotateZXY
        case rotateZYX
        case orient
        case transform
    }
}
