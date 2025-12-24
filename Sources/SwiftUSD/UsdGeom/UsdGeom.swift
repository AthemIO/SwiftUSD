// UsdGeom.swift - UsdGeom module Swift wrappers
// Mirrors: pxr/usd/usdGeom/*.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - Interpolation

/// Primvar interpolation modes.
/// Specifies how primvar data is interpolated across a geometric primitive.
public enum GeomInterpolation: Int, Sendable {
    /// One value for the entire surface.
    case constant = 0
    /// One value per face.
    case uniform = 1
    /// One value per vertex (bilinear interpolation).
    case varying = 2
    /// One value per vertex (smooth interpolation).
    case vertex = 3
    /// One value per face-vertex.
    case faceVarying = 4

    internal var cValue: UsdGeomInterpolation {
        UsdGeomInterpolation(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomInterpolation) {
        self = GeomInterpolation(rawValue: Int(cValue.rawValue)) ?? .constant
    }
}

// MARK: - SubdivisionScheme

/// Subdivision scheme for meshes.
public enum SubdivisionScheme: Int, Sendable {
    /// No subdivision.
    case none = 0
    /// Catmull-Clark subdivision.
    case catmullClark = 1
    /// Loop subdivision.
    case loop = 2
    /// Bilinear subdivision.
    case bilinear = 3

    internal var cValue: UsdGeomSubdivisionScheme {
        UsdGeomSubdivisionScheme(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomSubdivisionScheme) {
        self = SubdivisionScheme(rawValue: Int(cValue.rawValue)) ?? .none
    }
}

// MARK: - Axis

/// Axis for geometric primitives.
public enum GeomAxis: Int, Sendable {
    case x = 0
    case y = 1
    case z = 2

    internal var cValue: UsdGeomAxis {
        UsdGeomAxis(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomAxis) {
        self = GeomAxis(rawValue: Int(cValue.rawValue)) ?? .z
    }
}

// MARK: - Purpose

/// Purpose values for visibility/rendering.
public enum GeomPurpose: Int, Sendable {
    /// Default purpose.
    case `default` = 0
    /// Render purpose (full quality).
    case render = 1
    /// Proxy purpose (low resolution).
    case proxy = 2
    /// Guide purpose (helper visualization).
    case guide = 3

    internal var cValue: UsdGeomPurpose {
        UsdGeomPurpose(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomPurpose) {
        self = GeomPurpose(rawValue: Int(cValue.rawValue)) ?? .default
    }
}

// MARK: - GeomXform

/// A UsdGeomXform is a transformable container prim.
///
/// Xform prims provide a transform (rotation, translation, scale) that
/// affects all descendant prims. They are the primary means of organizing
/// and positioning geometry in a scene.
///
/// Mirrors `pxr::UsdGeomXform` from the USD C++ API.
public final class GeomXform: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomXformRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomXformRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomXform_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Xform prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Xform prim.
    /// - Returns: The newly defined Xform.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomXform {
        guard let ref = UsdGeomXform_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Xform at '\(path)'")
        }
        return GeomXform(handle: ref)
    }

    /// Creates a GeomXform schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomXform wrapper, or `nil` if the prim is not a valid Xform.
    public static func from(prim: Prim) -> GeomXform? {
        guard let ref = UsdGeomXform_FromPrim(prim.handle) else {
            return nil
        }
        let xform = GeomXform(handle: ref)
        return xform.isValid ? xform : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Xform is valid.
    public var isValid: Bool {
        UsdGeomXform_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomXform_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Transform Operations

    /// Computes the local-to-world transform matrix at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A 4x4 matrix as an array of 16 doubles in row-major order.
    /// - Throws: `GeomError.operationFailed` if the transform cannot be computed.
    public func localToWorldTransform(at time: TimeCode = .default) throws -> [Double] {
        var matrix = [Double](repeating: 0, count: 16)
        let result = matrix.withUnsafeMutableBufferPointer { buffer in
            UsdGeomXform_ComputeLocalToWorldTransform(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute local-to-world transform")
        }
        return matrix
    }

    /// Adds a translate operation to the Xform.
    ///
    /// - Parameters:
    ///   - x: X translation.
    ///   - y: Y translation.
    ///   - z: Z translation.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func addTranslate(x: Double, y: Double, z: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomXform_AddTranslateOp(handle, x, y, z, time.cTimeCode)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to add translate operation")
        }
    }

    /// Adds a rotate XYZ operation (in degrees) to the Xform.
    ///
    /// - Parameters:
    ///   - rotX: Rotation around X axis in degrees.
    ///   - rotY: Rotation around Y axis in degrees.
    ///   - rotZ: Rotation around Z axis in degrees.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func addRotateXYZ(rotX: Double, rotY: Double, rotZ: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomXform_AddRotateXYZOp(handle, rotX, rotY, rotZ, time.cTimeCode)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to add rotate operation")
        }
    }

    /// Adds a scale operation to the Xform.
    ///
    /// - Parameters:
    ///   - scaleX: Scale factor on X axis.
    ///   - scaleY: Scale factor on Y axis.
    ///   - scaleZ: Scale factor on Z axis.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func addScale(scaleX: Double, scaleY: Double, scaleZ: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomXform_AddScaleOp(handle, scaleX, scaleY, scaleZ, time.cTimeCode)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to add scale operation")
        }
    }

    /// Clears all xform operations.
    ///
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func clearXformOpOrder() throws {
        let result = UsdGeomXform_ClearXformOpOrder(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to clear xform op order")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomXform: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid xform>"
    }
}

// MARK: - GeomMesh

/// A UsdGeomMesh is a polygonal mesh.
///
/// Meshes are defined by a set of points, face vertex counts (number of
/// vertices per face), and face vertex indices (indices into the points array).
///
/// Mirrors `pxr::UsdGeomMesh` from the USD C++ API.
public final class GeomMesh: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomMeshRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomMeshRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomMesh_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Mesh prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Mesh prim.
    /// - Returns: The newly defined Mesh.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomMesh {
        guard let ref = UsdGeomMesh_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Mesh at '\(path)'")
        }
        return GeomMesh(handle: ref)
    }

    /// Creates a GeomMesh schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomMesh wrapper, or `nil` if the prim is not a valid Mesh.
    public static func from(prim: Prim) -> GeomMesh? {
        guard let ref = UsdGeomMesh_FromPrim(prim.handle) else {
            return nil
        }
        let mesh = GeomMesh(handle: ref)
        return mesh.isValid ? mesh : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Mesh is valid.
    public var isValid: Bool {
        UsdGeomMesh_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomMesh_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Points

    /// The number of points at the given time.
    public func pointCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomMesh_GetPointCount(handle, time.cTimeCode))
    }

    /// Gets the points array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of floats (x, y, z interleaved).
    public func points(at time: TimeCode = .default) -> [Float] {
        let count = pointCount(at: time)
        guard count > 0 else { return [] }

        var points = [Float](repeating: 0, count: count * 3)
        let actualCount = points.withUnsafeMutableBufferPointer { buffer in
            UsdGeomMesh_GetPoints(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            points.removeLast((count - Int(actualCount)) * 3)
        }
        return points
    }

    /// Sets the points array at the given time.
    ///
    /// - Parameters:
    ///   - points: Array of floats (x, y, z interleaved).
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setPoints(_ points: [Float], at time: TimeCode = .default) throws {
        let count = points.count / 3
        let result = points.withUnsafeBufferPointer { buffer in
            UsdGeomMesh_SetPoints(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set mesh points")
        }
    }

    // MARK: - Face Topology

    /// The number of face vertex counts at the given time.
    public func faceVertexCountsCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomMesh_GetFaceVertexCountsCount(handle, time.cTimeCode))
    }

    /// Gets the face vertex counts array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of integers representing the number of vertices per face.
    public func faceVertexCounts(at time: TimeCode = .default) -> [Int32] {
        let count = faceVertexCountsCount(at: time)
        guard count > 0 else { return [] }

        var counts = [Int32](repeating: 0, count: count)
        let actualCount = counts.withUnsafeMutableBufferPointer { buffer in
            UsdGeomMesh_GetFaceVertexCounts(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            counts.removeLast(count - Int(actualCount))
        }
        return counts
    }

    /// Sets the face vertex counts array at the given time.
    ///
    /// - Parameters:
    ///   - counts: Array of integers representing the number of vertices per face.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setFaceVertexCounts(_ counts: [Int32], at time: TimeCode = .default) throws {
        let result = counts.withUnsafeBufferPointer { buffer in
            UsdGeomMesh_SetFaceVertexCounts(handle, time.cTimeCode, buffer.baseAddress, counts.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set face vertex counts")
        }
    }

    /// The number of face vertex indices at the given time.
    public func faceVertexIndicesCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomMesh_GetFaceVertexIndicesCount(handle, time.cTimeCode))
    }

    /// Gets the face vertex indices array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of indices into the points array.
    public func faceVertexIndices(at time: TimeCode = .default) -> [Int32] {
        let count = faceVertexIndicesCount(at: time)
        guard count > 0 else { return [] }

        var indices = [Int32](repeating: 0, count: count)
        let actualCount = indices.withUnsafeMutableBufferPointer { buffer in
            UsdGeomMesh_GetFaceVertexIndices(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            indices.removeLast(count - Int(actualCount))
        }
        return indices
    }

    /// Sets the face vertex indices array at the given time.
    ///
    /// - Parameters:
    ///   - indices: Array of indices into the points array.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setFaceVertexIndices(_ indices: [Int32], at time: TimeCode = .default) throws {
        let result = indices.withUnsafeBufferPointer { buffer in
            UsdGeomMesh_SetFaceVertexIndices(handle, time.cTimeCode, buffer.baseAddress, indices.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set face vertex indices")
        }
    }

    // MARK: - Subdivision

    /// The subdivision scheme for this mesh.
    public var subdivisionScheme: SubdivisionScheme {
        get {
            SubdivisionScheme(cValue: UsdGeomMesh_GetSubdivisionScheme(handle))
        }
    }

    /// Sets the subdivision scheme for this mesh.
    ///
    /// - Parameter scheme: The subdivision scheme to use.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setSubdivisionScheme(_ scheme: SubdivisionScheme) throws {
        let result = UsdGeomMesh_SetSubdivisionScheme(handle, scheme.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set subdivision scheme")
        }
    }

    // MARK: - Display Properties

    /// Whether the mesh is double-sided.
    public var isDoubleSided: Bool {
        get { UsdGeomMesh_GetDoubleSided(handle) }
    }

    /// Sets whether the mesh is double-sided.
    ///
    /// - Parameter doubleSided: Whether the mesh should be double-sided.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setDoubleSided(_ doubleSided: Bool) throws {
        let result = UsdGeomMesh_SetDoubleSided(handle, doubleSided)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set double-sided property")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomMesh: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid mesh>"
    }
}

// MARK: - GeomSphere

/// A UsdGeomSphere is a primitive sphere geometry.
///
/// Mirrors `pxr::UsdGeomSphere` from the USD C++ API.
public final class GeomSphere: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomSphereRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomSphereRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomSphere_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Sphere prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Sphere prim.
    /// - Returns: The newly defined Sphere.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomSphere {
        guard let ref = UsdGeomSphere_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Sphere at '\(path)'")
        }
        return GeomSphere(handle: ref)
    }

    /// Creates a GeomSphere schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomSphere wrapper, or `nil` if the prim is not a valid Sphere.
    public static func from(prim: Prim) -> GeomSphere? {
        guard let ref = UsdGeomSphere_FromPrim(prim.handle) else {
            return nil
        }
        let sphere = GeomSphere(handle: ref)
        return sphere.isValid ? sphere : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Sphere is valid.
    public var isValid: Bool {
        UsdGeomSphere_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomSphere_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Properties

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Double {
        UsdGeomSphere_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomSphere_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set sphere radius")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomSphere: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid sphere>"
    }
}

// MARK: - GeomCube

/// A UsdGeomCube is a primitive cube geometry.
///
/// Mirrors `pxr::UsdGeomCube` from the USD C++ API.
public final class GeomCube: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomCubeRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomCubeRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomCube_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Cube prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Cube prim.
    /// - Returns: The newly defined Cube.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomCube {
        guard let ref = UsdGeomCube_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Cube at '\(path)'")
        }
        return GeomCube(handle: ref)
    }

    /// Creates a GeomCube schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomCube wrapper, or `nil` if the prim is not a valid Cube.
    public static func from(prim: Prim) -> GeomCube? {
        guard let ref = UsdGeomCube_FromPrim(prim.handle) else {
            return nil
        }
        let cube = GeomCube(handle: ref)
        return cube.isValid ? cube : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Cube is valid.
    public var isValid: Bool {
        UsdGeomCube_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomCube_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Properties

    /// Gets the size at the given time.
    public func size(at time: TimeCode = .default) -> Double {
        UsdGeomCube_GetSize(handle, time.cTimeCode)
    }

    /// Sets the size at the given time.
    ///
    /// - Parameters:
    ///   - size: The size value (side length of the cube).
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setSize(_ size: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCube_SetSize(handle, time.cTimeCode, size)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cube size")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomCube: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid cube>"
    }
}

// MARK: - GeomCylinder

/// A UsdGeomCylinder is a primitive cylinder geometry.
///
/// Mirrors `pxr::UsdGeomCylinder` from the USD C++ API.
public final class GeomCylinder: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomCylinderRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomCylinderRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomCylinder_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Cylinder prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Cylinder prim.
    /// - Returns: The newly defined Cylinder.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomCylinder {
        guard let ref = UsdGeomCylinder_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Cylinder at '\(path)'")
        }
        return GeomCylinder(handle: ref)
    }

    /// Creates a GeomCylinder schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomCylinder wrapper, or `nil` if the prim is not a valid Cylinder.
    public static func from(prim: Prim) -> GeomCylinder? {
        guard let ref = UsdGeomCylinder_FromPrim(prim.handle) else {
            return nil
        }
        let cylinder = GeomCylinder(handle: ref)
        return cylinder.isValid ? cylinder : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Cylinder is valid.
    public var isValid: Bool {
        UsdGeomCylinder_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomCylinder_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Properties

    /// Gets the height at the given time.
    public func height(at time: TimeCode = .default) -> Double {
        UsdGeomCylinder_GetHeight(handle, time.cTimeCode)
    }

    /// Sets the height at the given time.
    ///
    /// - Parameters:
    ///   - height: The height value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setHeight(_ height: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCylinder_SetHeight(handle, time.cTimeCode, height)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cylinder height")
        }
    }

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Double {
        UsdGeomCylinder_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCylinder_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cylinder radius")
        }
    }

    /// The axis along which the cylinder is oriented.
    public var axis: GeomAxis {
        get { GeomAxis(cValue: UsdGeomCylinder_GetAxis(handle)) }
    }

    /// Sets the axis along which the cylinder is oriented.
    ///
    /// - Parameter axis: The axis to orient the cylinder along.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setAxis(_ axis: GeomAxis) throws {
        let result = UsdGeomCylinder_SetAxis(handle, axis.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cylinder axis")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomCylinder: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid cylinder>"
    }
}

// MARK: - GeomCone

/// A UsdGeomCone is a primitive cone geometry.
///
/// Mirrors `pxr::UsdGeomCone` from the USD C++ API.
public final class GeomCone: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomConeRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomConeRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomCone_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Cone prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Cone prim.
    /// - Returns: The newly defined Cone.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomCone {
        guard let ref = UsdGeomCone_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Cone at '\(path)'")
        }
        return GeomCone(handle: ref)
    }

    /// Creates a GeomCone schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomCone wrapper, or `nil` if the prim is not a valid Cone.
    public static func from(prim: Prim) -> GeomCone? {
        guard let ref = UsdGeomCone_FromPrim(prim.handle) else {
            return nil
        }
        let cone = GeomCone(handle: ref)
        return cone.isValid ? cone : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Cone is valid.
    public var isValid: Bool {
        UsdGeomCone_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomCone_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Properties

    /// Gets the height at the given time.
    public func height(at time: TimeCode = .default) -> Double {
        UsdGeomCone_GetHeight(handle, time.cTimeCode)
    }

    /// Sets the height at the given time.
    ///
    /// - Parameters:
    ///   - height: The height value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setHeight(_ height: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCone_SetHeight(handle, time.cTimeCode, height)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cone height")
        }
    }

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Double {
        UsdGeomCone_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCone_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cone radius")
        }
    }

    /// The axis along which the cone is oriented.
    public var axis: GeomAxis {
        get { GeomAxis(cValue: UsdGeomCone_GetAxis(handle)) }
    }

    /// Sets the axis along which the cone is oriented.
    ///
    /// - Parameter axis: The axis to orient the cone along.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setAxis(_ axis: GeomAxis) throws {
        let result = UsdGeomCone_SetAxis(handle, axis.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set cone axis")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomCone: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid cone>"
    }
}

// MARK: - GeomCapsule

/// A UsdGeomCapsule is a primitive capsule geometry (cylinder with hemispherical caps).
///
/// Mirrors `pxr::UsdGeomCapsule` from the USD C++ API.
public final class GeomCapsule: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomCapsuleRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomCapsuleRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomCapsule_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Capsule prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Capsule prim.
    /// - Returns: The newly defined Capsule.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomCapsule {
        guard let ref = UsdGeomCapsule_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Capsule at '\(path)'")
        }
        return GeomCapsule(handle: ref)
    }

    /// Creates a GeomCapsule schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomCapsule wrapper, or `nil` if the prim is not a valid Capsule.
    public static func from(prim: Prim) -> GeomCapsule? {
        guard let ref = UsdGeomCapsule_FromPrim(prim.handle) else {
            return nil
        }
        let capsule = GeomCapsule(handle: ref)
        return capsule.isValid ? capsule : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Capsule is valid.
    public var isValid: Bool {
        UsdGeomCapsule_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomCapsule_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Properties

    /// Gets the height at the given time.
    public func height(at time: TimeCode = .default) -> Double {
        UsdGeomCapsule_GetHeight(handle, time.cTimeCode)
    }

    /// Sets the height at the given time.
    ///
    /// - Parameters:
    ///   - height: The height value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setHeight(_ height: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCapsule_SetHeight(handle, time.cTimeCode, height)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set capsule height")
        }
    }

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Double {
        UsdGeomCapsule_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Double, at time: TimeCode = .default) throws {
        let result = UsdGeomCapsule_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set capsule radius")
        }
    }

    /// The axis along which the capsule is oriented.
    public var axis: GeomAxis {
        get { GeomAxis(cValue: UsdGeomCapsule_GetAxis(handle)) }
    }

    /// Sets the axis along which the capsule is oriented.
    ///
    /// - Parameter axis: The axis to orient the capsule along.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setAxis(_ axis: GeomAxis) throws {
        let result = UsdGeomCapsule_SetAxis(handle, axis.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set capsule axis")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomCapsule: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid capsule>"
    }
}

// MARK: - GeomCamera

/// A UsdGeomCamera represents a camera in the scene.
///
/// Mirrors `pxr::UsdGeomCamera` from the USD C++ API.
public final class GeomCamera: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomCameraRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomCameraRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomCamera_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Camera prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Camera prim.
    /// - Returns: The newly defined Camera.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomCamera {
        guard let ref = UsdGeomCamera_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Camera at '\(path)'")
        }
        return GeomCamera(handle: ref)
    }

    /// Creates a GeomCamera schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomCamera wrapper, or `nil` if the prim is not a valid Camera.
    public static func from(prim: Prim) -> GeomCamera? {
        guard let ref = UsdGeomCamera_FromPrim(prim.handle) else {
            return nil
        }
        let camera = GeomCamera(handle: ref)
        return camera.isValid ? camera : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Camera is valid.
    public var isValid: Bool {
        UsdGeomCamera_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomCamera_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Properties

    /// Gets the focal length in mm at the given time.
    public func focalLength(at time: TimeCode = .default) -> Float {
        UsdGeomCamera_GetFocalLength(handle, time.cTimeCode)
    }

    /// Sets the focal length in mm at the given time.
    ///
    /// - Parameters:
    ///   - focalLength: The focal length in mm.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setFocalLength(_ focalLength: Float, at time: TimeCode = .default) throws {
        let result = UsdGeomCamera_SetFocalLength(handle, time.cTimeCode, focalLength)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set camera focal length")
        }
    }

    /// Gets the horizontal aperture in mm at the given time.
    public func horizontalAperture(at time: TimeCode = .default) -> Float {
        UsdGeomCamera_GetHorizontalAperture(handle, time.cTimeCode)
    }

    /// Sets the horizontal aperture in mm at the given time.
    ///
    /// - Parameters:
    ///   - aperture: The horizontal aperture in mm.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setHorizontalAperture(_ aperture: Float, at time: TimeCode = .default) throws {
        let result = UsdGeomCamera_SetHorizontalAperture(handle, time.cTimeCode, aperture)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set camera horizontal aperture")
        }
    }

    /// Gets the vertical aperture in mm at the given time.
    public func verticalAperture(at time: TimeCode = .default) -> Float {
        UsdGeomCamera_GetVerticalAperture(handle, time.cTimeCode)
    }

    /// Sets the vertical aperture in mm at the given time.
    ///
    /// - Parameters:
    ///   - aperture: The vertical aperture in mm.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setVerticalAperture(_ aperture: Float, at time: TimeCode = .default) throws {
        let result = UsdGeomCamera_SetVerticalAperture(handle, time.cTimeCode, aperture)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set camera vertical aperture")
        }
    }

    /// Gets the clipping range at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (near, far) clipping distances, or `nil` on failure.
    public func clippingRange(at time: TimeCode = .default) -> (near: Float, far: Float)? {
        var nearClip: Float = 0
        var farClip: Float = 0
        let result = UsdGeomCamera_GetClippingRange(handle, time.cTimeCode, &nearClip, &farClip)
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (nearClip, farClip)
    }

    /// Sets the clipping range at the given time.
    ///
    /// - Parameters:
    ///   - nearClip: The near clipping distance.
    ///   - farClip: The far clipping distance.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setClippingRange(near nearClip: Float, far farClip: Float, at time: TimeCode = .default) throws {
        let result = UsdGeomCamera_SetClippingRange(handle, time.cTimeCode, nearClip, farClip)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set camera clipping range")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomCamera: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid camera>"
    }
}

// MARK: - GeomError

/// Errors that can occur when working with UsdGeom types.
public enum GeomError: Error, CustomStringConvertible {

    /// Failed to define a geometry prim.
    case definitionFailed(String)

    /// A geometry operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .definitionFailed(let message):
            return "Geometry definition failed: \(message)"
        case .operationFailed(let message):
            return "Geometry operation failed: \(message)"
        }
    }
}

// MARK: - Type Aliases

/// Type alias for UsdGeomXform
public typealias UsdGeomXform = GeomXform

/// Type alias for UsdGeomMesh
public typealias UsdGeomMesh = GeomMesh

/// Type alias for UsdGeomSphere
public typealias UsdGeomSphere = GeomSphere

/// Type alias for UsdGeomCube
public typealias UsdGeomCube = GeomCube

/// Type alias for UsdGeomCylinder
public typealias UsdGeomCylinder = GeomCylinder

/// Type alias for UsdGeomCone
public typealias UsdGeomCone = GeomCone

/// Type alias for UsdGeomCapsule
public typealias UsdGeomCapsule = GeomCapsule

/// Type alias for UsdGeomCamera
public typealias UsdGeomCamera = GeomCamera
