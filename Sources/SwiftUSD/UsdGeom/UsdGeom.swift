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

// MARK: - CurveType

/// Curve type for basis curves.
public enum CurveType: Int, Sendable {
    /// Linear curve segments.
    case linear = 0
    /// Cubic curve segments.
    case cubic = 1

    internal var cValue: UsdGeomCurveType {
        UsdGeomCurveType(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomCurveType) {
        self = CurveType(rawValue: Int(cValue.rawValue)) ?? .linear
    }
}

// MARK: - CurveBasis

/// Basis for cubic curves.
public enum CurveBasis: Int, Sendable {
    /// Bezier basis.
    case bezier = 0
    /// B-spline basis.
    case bspline = 1
    /// Catmull-Rom basis.
    case catmullRom = 2

    internal var cValue: UsdGeomBasis {
        UsdGeomBasis(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomBasis) {
        self = CurveBasis(rawValue: Int(cValue.rawValue)) ?? .bezier
    }
}

// MARK: - CurveWrap

/// Wrap mode for curves.
public enum CurveWrap: Int, Sendable {
    /// Non-periodic curves.
    case nonperiodic = 0
    /// Periodic (closed) curves.
    case periodic = 1
    /// Pinned curves (endpoints match control points).
    case pinned = 2

    internal var cValue: UsdGeomWrap {
        UsdGeomWrap(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdGeomWrap) {
        self = CurveWrap(rawValue: Int(cValue.rawValue)) ?? .nonperiodic
    }
}

// MARK: - GeomPoints

/// A UsdGeomPoints is a point cloud primitive.
///
/// Points prims are defined by a set of point positions, optional widths,
/// and optional IDs for tracking points across time.
///
/// Mirrors `pxr::UsdGeomPoints` from the USD C++ API.
public final class GeomPoints: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomPointsRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomPointsRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomPoints_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Points prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Points prim.
    /// - Returns: The newly defined Points.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomPoints {
        guard let ref = UsdGeomPoints_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define Points at '\(path)'")
        }
        return GeomPoints(handle: ref)
    }

    /// Creates a GeomPoints schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomPoints wrapper, or `nil` if the prim is not a valid Points.
    public static func from(prim: Prim) -> GeomPoints? {
        guard let ref = UsdGeomPoints_FromPrim(prim.handle) else {
            return nil
        }
        let points = GeomPoints(handle: ref)
        return points.isValid ? points : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Points is valid.
    public var isValid: Bool {
        UsdGeomPoints_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomPoints_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Points

    /// The number of points at the given time.
    public func pointCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomPoints_GetPointCount(handle, time.cTimeCode))
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
            UsdGeomPoints_GetPoints(handle, time.cTimeCode, buffer.baseAddress, count)
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
            UsdGeomPoints_SetPoints(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set points")
        }
    }

    // MARK: - Widths

    /// The number of widths at the given time.
    public func widthsCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomPoints_GetWidthsCount(handle, time.cTimeCode))
    }

    /// Gets the widths array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of float widths.
    public func widths(at time: TimeCode = .default) -> [Float] {
        let count = widthsCount(at: time)
        guard count > 0 else { return [] }

        var widths = [Float](repeating: 0, count: count)
        let actualCount = widths.withUnsafeMutableBufferPointer { buffer in
            UsdGeomPoints_GetWidths(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            widths.removeLast(count - Int(actualCount))
        }
        return widths
    }

    /// Sets the widths array at the given time.
    ///
    /// - Parameters:
    ///   - widths: Array of float widths.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setWidths(_ widths: [Float], at time: TimeCode = .default) throws {
        let result = widths.withUnsafeBufferPointer { buffer in
            UsdGeomPoints_SetWidths(handle, time.cTimeCode, buffer.baseAddress, widths.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set widths")
        }
    }

    // MARK: - IDs

    /// The number of IDs at the given time.
    public func idsCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomPoints_GetIdsCount(handle, time.cTimeCode))
    }

    /// Gets the IDs array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of Int64 IDs.
    public func ids(at time: TimeCode = .default) -> [Int64] {
        let count = idsCount(at: time)
        guard count > 0 else { return [] }

        var ids = [Int64](repeating: 0, count: count)
        let actualCount = ids.withUnsafeMutableBufferPointer { buffer in
            UsdGeomPoints_GetIds(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            ids.removeLast(count - Int(actualCount))
        }
        return ids
    }

    /// Sets the IDs array at the given time.
    ///
    /// - Parameters:
    ///   - ids: Array of Int64 IDs.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setIds(_ ids: [Int64], at time: TimeCode = .default) throws {
        let result = ids.withUnsafeBufferPointer { buffer in
            UsdGeomPoints_SetIds(handle, time.cTimeCode, buffer.baseAddress, ids.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set IDs")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomPoints: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid points>"
    }
}

// MARK: - GeomBasisCurves

/// A UsdGeomBasisCurves is a collection of parametric curves.
///
/// Basis curves can be linear or cubic, with various basis types
/// (Bezier, B-spline, Catmull-Rom) and wrap modes.
///
/// Mirrors `pxr::UsdGeomBasisCurves` from the USD C++ API.
public final class GeomBasisCurves: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomBasisCurvesRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomBasisCurvesRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomBasisCurves_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new BasisCurves prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new BasisCurves prim.
    /// - Returns: The newly defined BasisCurves.
    /// - Throws: `GeomError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> GeomBasisCurves {
        guard let ref = UsdGeomBasisCurves_Define(stage.handle, path.handle) else {
            throw GeomError.definitionFailed("Failed to define BasisCurves at '\(path)'")
        }
        return GeomBasisCurves(handle: ref)
    }

    /// Creates a GeomBasisCurves schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A GeomBasisCurves wrapper, or `nil` if the prim is not a valid BasisCurves.
    public static func from(prim: Prim) -> GeomBasisCurves? {
        guard let ref = UsdGeomBasisCurves_FromPrim(prim.handle) else {
            return nil
        }
        let curves = GeomBasisCurves(handle: ref)
        return curves.isValid ? curves : nil
    }

    // MARK: - Validity

    /// Returns `true` if the BasisCurves is valid.
    public var isValid: Bool {
        UsdGeomBasisCurves_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdGeomBasisCurves_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Curve Type

    /// The curve type (linear or cubic).
    public var curveType: CurveType {
        get { CurveType(cValue: UsdGeomBasisCurves_GetType(handle)) }
    }

    /// Sets the curve type.
    ///
    /// - Parameter type: The curve type.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setCurveType(_ type: CurveType) throws {
        let result = UsdGeomBasisCurves_SetType(handle, type.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set curve type")
        }
    }

    // MARK: - Basis

    /// The basis for cubic curves.
    public var basis: CurveBasis {
        get { CurveBasis(cValue: UsdGeomBasisCurves_GetBasis(handle)) }
    }

    /// Sets the curve basis.
    ///
    /// - Parameter basis: The curve basis.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setBasis(_ basis: CurveBasis) throws {
        let result = UsdGeomBasisCurves_SetBasis(handle, basis.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set curve basis")
        }
    }

    // MARK: - Wrap

    /// The wrap mode for curves.
    public var wrap: CurveWrap {
        get { CurveWrap(cValue: UsdGeomBasisCurves_GetWrap(handle)) }
    }

    /// Sets the wrap mode.
    ///
    /// - Parameter wrap: The wrap mode.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setWrap(_ wrap: CurveWrap) throws {
        let result = UsdGeomBasisCurves_SetWrap(handle, wrap.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set wrap mode")
        }
    }

    // MARK: - Curve Vertex Counts

    /// The number of curve vertex counts at the given time.
    public func curveVertexCountsCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomBasisCurves_GetCurveVertexCountsCount(handle, time.cTimeCode))
    }

    /// Gets the curve vertex counts array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of integers representing vertices per curve.
    public func curveVertexCounts(at time: TimeCode = .default) -> [Int32] {
        let count = curveVertexCountsCount(at: time)
        guard count > 0 else { return [] }

        var counts = [Int32](repeating: 0, count: count)
        let actualCount = counts.withUnsafeMutableBufferPointer { buffer in
            UsdGeomBasisCurves_GetCurveVertexCounts(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            counts.removeLast(count - Int(actualCount))
        }
        return counts
    }

    /// Sets the curve vertex counts array at the given time.
    ///
    /// - Parameters:
    ///   - counts: Array of integers representing vertices per curve.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setCurveVertexCounts(_ counts: [Int32], at time: TimeCode = .default) throws {
        let result = counts.withUnsafeBufferPointer { buffer in
            UsdGeomBasisCurves_SetCurveVertexCounts(handle, time.cTimeCode, buffer.baseAddress, counts.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set curve vertex counts")
        }
    }

    // MARK: - Points

    /// The number of points at the given time.
    public func pointCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomBasisCurves_GetPointCount(handle, time.cTimeCode))
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
            UsdGeomBasisCurves_GetPoints(handle, time.cTimeCode, buffer.baseAddress, count)
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
            UsdGeomBasisCurves_SetPoints(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set points")
        }
    }

    // MARK: - Widths

    /// The number of widths at the given time.
    public func widthsCount(at time: TimeCode = .default) -> Int {
        Int(UsdGeomBasisCurves_GetWidthsCount(handle, time.cTimeCode))
    }

    /// Gets the widths array at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of float widths.
    public func widths(at time: TimeCode = .default) -> [Float] {
        let count = widthsCount(at: time)
        guard count > 0 else { return [] }

        var widths = [Float](repeating: 0, count: count)
        let actualCount = widths.withUnsafeMutableBufferPointer { buffer in
            UsdGeomBasisCurves_GetWidths(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        if actualCount < count {
            widths.removeLast(count - Int(actualCount))
        }
        return widths
    }

    /// Sets the widths array at the given time.
    ///
    /// - Parameters:
    ///   - widths: Array of float widths.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setWidths(_ widths: [Float], at time: TimeCode = .default) throws {
        let result = widths.withUnsafeBufferPointer { buffer in
            UsdGeomBasisCurves_SetWidths(handle, time.cTimeCode, buffer.baseAddress, widths.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set widths")
        }
    }
}

// MARK: - CustomStringConvertible

extension GeomBasisCurves: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid basis curves>"
    }
}

// MARK: - GeomPrimvar

/// A primvar is a special kind of attribute that can have interpolation metadata.
///
/// Primvars are used to specify data that varies across the surface of a geometric
/// primitive (like colors, UVs, normals).
///
/// Mirrors `pxr::UsdGeomPrimvar` from the USD C++ API.
public final class GeomPrimvar: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomPrimvarRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomPrimvarRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomPrimvar_Release(handle)
    }

    // MARK: - Factory Methods

    /// Creates a GeomPrimvar from an attribute.
    ///
    /// - Parameter attribute: The attribute to wrap.
    /// - Returns: A GeomPrimvar wrapper, or `nil` if the attribute is not a valid primvar.
    public static func from(attribute: Attribute) -> GeomPrimvar? {
        guard let ref = UsdGeomPrimvar_FromAttribute(attribute.handle) else {
            return nil
        }
        let primvar = GeomPrimvar(handle: ref)
        return primvar.isValid ? primvar : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Primvar is valid.
    public var isValid: Bool {
        UsdGeomPrimvar_IsValid(handle)
    }

    /// Returns `true` if the primvar has a value.
    public var hasValue: Bool {
        UsdGeomPrimvar_HasValue(handle)
    }

    /// Returns `true` if the primvar has an authored value.
    public var hasAuthoredValue: Bool {
        UsdGeomPrimvar_HasAuthoredValue(handle)
    }

    // MARK: - Properties

    /// The full name of the primvar (including namespace prefix).
    public var name: Token? {
        guard let ref = UsdGeomPrimvar_GetName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The base name of the primvar (without namespace prefix).
    public var baseName: Token? {
        guard let ref = UsdGeomPrimvar_GetBaseName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The interpolation mode.
    public var interpolation: GeomInterpolation {
        get { GeomInterpolation(cValue: UsdGeomPrimvar_GetInterpolation(handle)) }
    }

    /// Sets the interpolation mode.
    ///
    /// - Parameter interpolation: The interpolation mode.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setInterpolation(_ interpolation: GeomInterpolation) throws {
        let result = UsdGeomPrimvar_SetInterpolation(handle, interpolation.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set interpolation")
        }
    }

    /// The element size (for array-valued primvars).
    public var elementSize: Int {
        get { Int(UsdGeomPrimvar_GetElementSize(handle)) }
    }

    /// Sets the element size.
    ///
    /// - Parameter size: The element size.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setElementSize(_ size: Int) throws {
        let result = UsdGeomPrimvar_SetElementSize(handle, Int32(size))
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set element size")
        }
    }

    /// Returns `true` if the primvar is indexed.
    public var isIndexed: Bool {
        UsdGeomPrimvar_IsIndexed(handle)
    }

    /// Gets the underlying attribute.
    public var attribute: Attribute? {
        guard let ref = UsdGeomPrimvar_GetAttr(handle) else {
            return nil
        }
        return Attribute(handle: ref)
    }
}

// MARK: - CustomStringConvertible

extension GeomPrimvar: CustomStringConvertible {

    public var description: String {
        name?.text ?? "<invalid primvar>"
    }
}

// MARK: - GeomPrimvarsAPI

/// API schema for accessing and creating primvars on a prim.
///
/// Mirrors `pxr::UsdGeomPrimvarsAPI` from the USD C++ API.
public final class GeomPrimvarsAPI: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomPrimvarsAPIRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdGeomPrimvarsAPIRef) {
        self.handle = handle
    }

    deinit {
        UsdGeomPrimvarsAPI_Release(handle)
    }

    // MARK: - Factory Methods

    /// Gets the PrimvarsAPI for a prim.
    ///
    /// - Parameter prim: The prim to get the API for.
    /// - Returns: The PrimvarsAPI, or `nil` if it cannot be created.
    public static func get(prim: Prim) -> GeomPrimvarsAPI? {
        guard let ref = UsdGeomPrimvarsAPI_Get(prim.handle) else {
            return nil
        }
        let api = GeomPrimvarsAPI(handle: ref)
        return api.isValid ? api : nil
    }

    // MARK: - Validity

    /// Returns `true` if the API is valid.
    public var isValid: Bool {
        UsdGeomPrimvarsAPI_IsValid(handle)
    }

    // MARK: - Primvar Operations

    /// Creates a primvar on the prim.
    ///
    /// - Parameters:
    ///   - name: The name of the primvar.
    ///   - typeName: The type name (e.g., "float3", "color3f").
    ///   - interpolation: The interpolation mode.
    /// - Returns: The created primvar, or `nil` on failure.
    public func createPrimvar(
        name: Token,
        typeName: Token,
        interpolation: GeomInterpolation
    ) -> GeomPrimvar? {
        guard let ref = UsdGeomPrimvarsAPI_CreatePrimvar(
            handle, name.handle, typeName.handle, interpolation.cValue
        ) else {
            return nil
        }
        return GeomPrimvar(handle: ref)
    }

    /// Creates a primvar with element size.
    ///
    /// - Parameters:
    ///   - name: The name of the primvar.
    ///   - typeName: The type name (e.g., "float3", "color3f").
    ///   - interpolation: The interpolation mode.
    ///   - elementSize: The element size for array values.
    /// - Returns: The created primvar, or `nil` on failure.
    public func createPrimvar(
        name: Token,
        typeName: Token,
        interpolation: GeomInterpolation,
        elementSize: Int
    ) -> GeomPrimvar? {
        guard let ref = UsdGeomPrimvarsAPI_CreatePrimvarWithElementSize(
            handle, name.handle, typeName.handle, interpolation.cValue, Int32(elementSize)
        ) else {
            return nil
        }
        return GeomPrimvar(handle: ref)
    }

    /// Gets a primvar by name.
    ///
    /// - Parameter name: The name of the primvar.
    /// - Returns: The primvar, or `nil` if not found.
    public func getPrimvar(named name: Token) -> GeomPrimvar? {
        guard let ref = UsdGeomPrimvarsAPI_GetPrimvar(handle, name.handle) else {
            return nil
        }
        return GeomPrimvar(handle: ref)
    }

    /// Returns `true` if the prim has a primvar with the given name.
    public func hasPrimvar(named name: Token) -> Bool {
        UsdGeomPrimvarsAPI_HasPrimvar(handle, name.handle)
    }

    /// Removes a primvar.
    ///
    /// - Parameter name: The name of the primvar to remove.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func removePrimvar(named name: Token) throws {
        let result = UsdGeomPrimvarsAPI_RemovePrimvar(handle, name.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to remove primvar '\(name)'")
        }
    }

    /// The number of primvars on the prim.
    public var primvarCount: Int {
        Int(UsdGeomPrimvarsAPI_GetPrimvarCount(handle))
    }

    /// Gets all primvars on the prim.
    ///
    /// - Returns: An array of all primvars.
    public var primvars: [GeomPrimvar] {
        let count = primvarCount
        guard count > 0 else { return [] }

        var refs = [UsdGeomPrimvarRef?](repeating: nil, count: count)
        let actualCount = refs.withUnsafeMutableBufferPointer { buffer in
            UsdGeomPrimvarsAPI_GetPrimvars(handle, buffer.baseAddress, count)
        }
        return refs.prefix(Int(actualCount)).compactMap { ref in
            ref.map { GeomPrimvar(handle: $0) }
        }
    }
}

// MARK: - GeomXformCache

/// A cache for xform transforms to avoid repeated computation.
///
/// XformCache provides efficient access to transform matrices by caching
/// computed results.
///
/// Mirrors `pxr::UsdGeomXformCache` from the USD C++ API.
public final class GeomXformCache: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomXformCacheRef

    // MARK: - Initialization

    /// Creates an xform cache for the given time.
    ///
    /// - Parameter time: The time code for transform evaluation.
    public init(at time: TimeCode = .default) {
        self.handle = UsdGeomXformCache_Create(time.cTimeCode)
    }

    deinit {
        UsdGeomXformCache_Release(handle)
    }

    // MARK: - Transform Queries

    /// Computes the local-to-world transform for a prim.
    ///
    /// - Parameter prim: The prim to compute the transform for.
    /// - Returns: A 4x4 matrix as 16 doubles in row-major order.
    /// - Throws: `GeomError.operationFailed` if the transform cannot be computed.
    public func localToWorldTransform(for prim: Prim) throws -> [Double] {
        var matrix = [Double](repeating: 0, count: 16)
        let result = matrix.withUnsafeMutableBufferPointer { buffer in
            UsdGeomXformCache_GetLocalToWorldTransform(handle, prim.handle, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute local-to-world transform")
        }
        return matrix
    }

    /// Computes the parent-to-world transform for a prim.
    ///
    /// - Parameter prim: The prim to compute the transform for.
    /// - Returns: A 4x4 matrix as 16 doubles in row-major order.
    /// - Throws: `GeomError.operationFailed` if the transform cannot be computed.
    public func parentToWorldTransform(for prim: Prim) throws -> [Double] {
        var matrix = [Double](repeating: 0, count: 16)
        let result = matrix.withUnsafeMutableBufferPointer { buffer in
            UsdGeomXformCache_GetParentToWorldTransform(handle, prim.handle, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute parent-to-world transform")
        }
        return matrix
    }

    /// Computes the local transformation for a prim.
    ///
    /// - Parameter prim: The prim to compute the transform for.
    /// - Returns: A tuple of the transform matrix and whether the xform stack is reset.
    /// - Throws: `GeomError.operationFailed` if the transform cannot be computed.
    public func localTransformation(for prim: Prim) throws -> (matrix: [Double], resetsXformStack: Bool) {
        var matrix = [Double](repeating: 0, count: 16)
        var resetsStack = false
        let result = matrix.withUnsafeMutableBufferPointer { buffer in
            UsdGeomXformCache_GetLocalTransformation(handle, prim.handle, buffer.baseAddress, &resetsStack)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute local transformation")
        }
        return (matrix, resetsStack)
    }

    /// Computes the relative transform from one prim to an ancestor.
    ///
    /// - Parameters:
    ///   - prim: The prim to compute the transform for.
    ///   - ancestor: The ancestor prim.
    /// - Returns: A 4x4 matrix as 16 doubles in row-major order.
    /// - Throws: `GeomError.operationFailed` if the transform cannot be computed.
    public func relativeTransform(from prim: Prim, to ancestor: Prim) throws -> [Double] {
        var matrix = [Double](repeating: 0, count: 16)
        let result = matrix.withUnsafeMutableBufferPointer { buffer in
            UsdGeomXformCache_ComputeRelativeTransform(handle, prim.handle, ancestor.handle, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute relative transform")
        }
        return matrix
    }

    // MARK: - Cache Control

    /// Sets the time for this cache, invalidating cached values.
    public func setTime(_ time: TimeCode) {
        UsdGeomXformCache_SetTime(handle, time.cTimeCode)
    }

    /// Gets the current time of the cache.
    public var time: TimeCode {
        let cTime = UsdGeomXformCache_GetTime(handle)
        return cTime.isDefault ? .default : TimeCode(cTime.time)
    }

    /// Clears all cached data.
    public func clear() {
        UsdGeomXformCache_Clear(handle)
    }
}

// MARK: - GeomBBoxCache

/// A cache for bounding box computations.
///
/// BBoxCache provides efficient access to bounding boxes by caching
/// computed results.
///
/// Mirrors `pxr::UsdGeomBBoxCache` from the USD C++ API.
public final class GeomBBoxCache: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdGeomBBoxCacheRef

    // MARK: - Initialization

    /// Creates a bounding box cache.
    ///
    /// - Parameters:
    ///   - time: The time code for bounds evaluation.
    ///   - purposes: The purposes to include in bounds computation.
    ///   - useExtentsHint: Whether to use extents hints if available.
    ///   - ignoreVisibility: Whether to ignore visibility when computing bounds.
    public init(
        at time: TimeCode = .default,
        purposes: [GeomPurpose] = [.default, .render],
        useExtentsHint: Bool = true,
        ignoreVisibility: Bool = false
    ) {
        let cPurposes = purposes.map { $0.cValue }
        self.handle = cPurposes.withUnsafeBufferPointer { buffer in
            UsdGeomBBoxCache_Create(time.cTimeCode, buffer.baseAddress, buffer.count, useExtentsHint, ignoreVisibility)
        }
    }

    deinit {
        UsdGeomBBoxCache_Release(handle)
    }

    // MARK: - Bounds Queries

    /// Computes the world-space bounding box for a prim.
    ///
    /// - Parameter prim: The prim to compute bounds for.
    /// - Returns: A tuple of (min, max) points, each as [x, y, z].
    /// - Throws: `GeomError.operationFailed` if bounds cannot be computed.
    public func computeWorldBound(for prim: Prim) throws -> (min: [Double], max: [Double]) {
        var minPoint = [Double](repeating: 0, count: 3)
        var maxPoint = [Double](repeating: 0, count: 3)
        let result = minPoint.withUnsafeMutableBufferPointer { minBuffer in
            maxPoint.withUnsafeMutableBufferPointer { maxBuffer in
                UsdGeomBBoxCache_ComputeWorldBound(handle, prim.handle, minBuffer.baseAddress, maxBuffer.baseAddress)
            }
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute world bounds")
        }
        return (minPoint, maxPoint)
    }

    /// Computes the local-space bounding box for a prim.
    ///
    /// - Parameter prim: The prim to compute bounds for.
    /// - Returns: A tuple of (min, max) points, each as [x, y, z].
    /// - Throws: `GeomError.operationFailed` if bounds cannot be computed.
    public func computeLocalBound(for prim: Prim) throws -> (min: [Double], max: [Double]) {
        var minPoint = [Double](repeating: 0, count: 3)
        var maxPoint = [Double](repeating: 0, count: 3)
        let result = minPoint.withUnsafeMutableBufferPointer { minBuffer in
            maxPoint.withUnsafeMutableBufferPointer { maxBuffer in
                UsdGeomBBoxCache_ComputeLocalBound(handle, prim.handle, minBuffer.baseAddress, maxBuffer.baseAddress)
            }
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute local bounds")
        }
        return (minPoint, maxPoint)
    }

    /// Computes the untransformed bounding box for a prim.
    ///
    /// - Parameter prim: The prim to compute bounds for.
    /// - Returns: A tuple of (min, max) points, each as [x, y, z].
    /// - Throws: `GeomError.operationFailed` if bounds cannot be computed.
    public func computeUntransformedBound(for prim: Prim) throws -> (min: [Double], max: [Double]) {
        var minPoint = [Double](repeating: 0, count: 3)
        var maxPoint = [Double](repeating: 0, count: 3)
        let result = minPoint.withUnsafeMutableBufferPointer { minBuffer in
            maxPoint.withUnsafeMutableBufferPointer { maxBuffer in
                UsdGeomBBoxCache_ComputeUntransformedBound(handle, prim.handle, minBuffer.baseAddress, maxBuffer.baseAddress)
            }
        }
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to compute untransformed bounds")
        }
        return (minPoint, maxPoint)
    }

    // MARK: - Cache Control

    /// Sets the time for this cache, invalidating cached values.
    public func setTime(_ time: TimeCode) {
        UsdGeomBBoxCache_SetTime(handle, time.cTimeCode)
    }

    /// Gets the current time of the cache.
    public var time: TimeCode {
        let cTime = UsdGeomBBoxCache_GetTime(handle)
        return cTime.isDefault ? .default : TimeCode(cTime.time)
    }

    /// Clears all cached data.
    public func clear() {
        UsdGeomBBoxCache_Clear(handle)
    }
}

// MARK: - Visibility and Purpose Helpers

extension Prim {

    /// Gets the visibility of this prim at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: `true` if visible, `false` if invisible.
    public func isVisible(at time: TimeCode = .default) -> Bool {
        UsdGeom_GetVisibility(handle, time.cTimeCode)
    }

    /// Sets the visibility of this prim at the given time.
    ///
    /// - Parameters:
    ///   - visible: Whether the prim should be visible.
    ///   - time: The time code at which to set the value.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setVisibility(_ visible: Bool, at time: TimeCode = .default) throws {
        let result = UsdGeom_SetVisibility(handle, time.cTimeCode, visible)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set visibility")
        }
    }

    /// Gets the purpose of this prim.
    public var purpose: GeomPurpose {
        GeomPurpose(cValue: UsdGeom_GetPurpose(handle))
    }

    /// Sets the purpose of this prim.
    ///
    /// - Parameter purpose: The purpose to set.
    /// - Throws: `GeomError.operationFailed` if the operation fails.
    public func setPurpose(_ purpose: GeomPurpose) throws {
        let result = UsdGeom_SetPurpose(handle, purpose.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw GeomError.operationFailed("Failed to set purpose")
        }
    }
}

// MARK: - Type Aliases

/// Type alias for UsdGeomXform
public typealias UsdGeomXform = GeomXform

/// Type alias for UsdGeomMesh
public typealias UsdGeomMesh = GeomMesh

/// Type alias for UsdGeomPoints
public typealias UsdGeomPoints = GeomPoints

/// Type alias for UsdGeomBasisCurves
public typealias UsdGeomBasisCurves = GeomBasisCurves

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

/// Type alias for UsdGeomPrimvar
public typealias UsdGeomPrimvar = GeomPrimvar

/// Type alias for UsdGeomPrimvarsAPI
public typealias UsdGeomPrimvarsAPI = GeomPrimvarsAPI

/// Type alias for UsdGeomXformCache
public typealias UsdGeomXformCache = GeomXformCache

/// Type alias for UsdGeomBBoxCache
public typealias UsdGeomBBoxCache = GeomBBoxCache
