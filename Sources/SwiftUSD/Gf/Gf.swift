// Gf.swift - Graphics Foundation - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Gf types,
// including vectors, matrices, quaternions, and bounding boxes.

import CxxFacade

// MARK: - Type Aliases

/// Vec2f - 2D single-precision vector for texture coordinates, etc.
public typealias Vec2f = cxxfacade.gf.Vec2f

/// Vec2d - 2D double-precision vector
public typealias Vec2d = cxxfacade.gf.Vec2d

/// Vec3f - 3D single-precision vector for positions, normals, colors, etc.
public typealias Vec3f = cxxfacade.gf.Vec3f

/// Vec3d - 3D double-precision vector
public typealias Vec3d = cxxfacade.gf.Vec3d

/// Vec4f - 4D single-precision vector for RGBA colors, homogeneous coordinates, etc.
public typealias Vec4f = cxxfacade.gf.Vec4f

/// Vec4d - 4D double-precision vector
public typealias Vec4d = cxxfacade.gf.Vec4d

/// Matrix4f - 4x4 single-precision transformation matrix (row-major)
public typealias Matrix4f = cxxfacade.gf.Matrix4f

/// Matrix4d - 4x4 double-precision transformation matrix (row-major)
public typealias Matrix4d = cxxfacade.gf.Matrix4d

/// Quatf - Single-precision quaternion for 3D rotations
public typealias Quatf = cxxfacade.gf.Quatf

/// Quatd - Double-precision quaternion for 3D rotations
public typealias Quatd = cxxfacade.gf.Quatd

/// Range3d - Axis-aligned 3D range defined by min and max points
public typealias Range3d = cxxfacade.gf.Range3d

/// BBox3d - 3D bounding box with optional transformation matrix
public typealias BBox3d = cxxfacade.gf.BBox3d

// MARK: - Vec2f Extensions

extension Vec2f: CustomStringConvertible {
    public var description: String {
        return "Vec2f(\(x), \(y))"
    }
}

extension Vec2f: Equatable {
    public static func == (lhs: Vec2f, rhs: Vec2f) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y
    }
}

extension Vec2f {
    /// Create a vector from a tuple
    public init(_ tuple: (Float, Float)) {
        self.init(tuple.0, tuple.1)
    }

    /// The length (magnitude) of the vector
    public var length: Float { GetLength() }

    /// The squared length of the vector (faster than length)
    public var lengthSquared: Float { GetLengthSq() }

    /// A normalized copy of the vector
    public var normalized: Vec2f { GetNormalized() }

    /// Dot product with another vector
    public func dot(_ other: Vec2f) -> Float { Dot(other) }
}

// MARK: - Vec2d Extensions

extension Vec2d: CustomStringConvertible {
    public var description: String {
        return "Vec2d(\(x), \(y))"
    }
}

extension Vec2d: Equatable {
    public static func == (lhs: Vec2d, rhs: Vec2d) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y
    }
}

extension Vec2d {
    /// Create a vector from a tuple
    public init(_ tuple: (Double, Double)) {
        self.init(tuple.0, tuple.1)
    }

    /// The length (magnitude) of the vector
    public var length: Double { GetLength() }

    /// The squared length of the vector (faster than length)
    public var lengthSquared: Double { GetLengthSq() }

    /// A normalized copy of the vector
    public var normalized: Vec2d { GetNormalized() }

    /// Dot product with another vector
    public func dot(_ other: Vec2d) -> Double { Dot(other) }
}

// MARK: - Vec3f Extensions

extension Vec3f: CustomStringConvertible {
    public var description: String {
        return "Vec3f(\(x), \(y), \(z))"
    }
}

extension Vec3f: Equatable {
    public static func == (lhs: Vec3f, rhs: Vec3f) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension Vec3f {
    /// Create a vector from a tuple
    public init(_ tuple: (Float, Float, Float)) {
        self.init(tuple.0, tuple.1, tuple.2)
    }

    /// The length (magnitude) of the vector
    public var length: Float { GetLength() }

    /// The squared length of the vector (faster than length)
    public var lengthSquared: Float { GetLengthSq() }

    /// A normalized copy of the vector
    public var normalized: Vec3f { GetNormalized() }

    /// Dot product with another vector
    public func dot(_ other: Vec3f) -> Float { Dot(other) }

    /// Cross product with another vector
    public func cross(_ other: Vec3f) -> Vec3f { Cross(other) }

    /// Unit vector along the X axis
    public static var xAxis: Vec3f { Vec3f.XAxis() }

    /// Unit vector along the Y axis
    public static var yAxis: Vec3f { Vec3f.YAxis() }

    /// Unit vector along the Z axis
    public static var zAxis: Vec3f { Vec3f.ZAxis() }

    /// Zero vector
    public static var zero: Vec3f { Vec3f.Zero() }
}

// MARK: - Vec3d Extensions

extension Vec3d: CustomStringConvertible {
    public var description: String {
        return "Vec3d(\(x), \(y), \(z))"
    }
}

extension Vec3d: Equatable {
    public static func == (lhs: Vec3d, rhs: Vec3d) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension Vec3d {
    /// Create a vector from a tuple
    public init(_ tuple: (Double, Double, Double)) {
        self.init(tuple.0, tuple.1, tuple.2)
    }

    /// The length (magnitude) of the vector
    public var length: Double { GetLength() }

    /// The squared length of the vector (faster than length)
    public var lengthSquared: Double { GetLengthSq() }

    /// A normalized copy of the vector
    public var normalized: Vec3d { GetNormalized() }

    /// Dot product with another vector
    public func dot(_ other: Vec3d) -> Double { Dot(other) }

    /// Cross product with another vector
    public func cross(_ other: Vec3d) -> Vec3d { Cross(other) }

    /// Unit vector along the X axis
    public static var xAxis: Vec3d { Vec3d.XAxis() }

    /// Unit vector along the Y axis
    public static var yAxis: Vec3d { Vec3d.YAxis() }

    /// Unit vector along the Z axis
    public static var zAxis: Vec3d { Vec3d.ZAxis() }

    /// Zero vector
    public static var zero: Vec3d { Vec3d.Zero() }
}

// MARK: - Vec4f Extensions

extension Vec4f: CustomStringConvertible {
    public var description: String {
        return "Vec4f(\(x), \(y), \(z), \(w))"
    }
}

extension Vec4f: Equatable {
    public static func == (lhs: Vec4f, rhs: Vec4f) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}

extension Vec4f {
    /// Create a vector from a tuple
    public init(_ tuple: (Float, Float, Float, Float)) {
        self.init(tuple.0, tuple.1, tuple.2, tuple.3)
    }

    /// The length (magnitude) of the vector
    public var length: Float { GetLength() }

    /// The squared length of the vector (faster than length)
    public var lengthSquared: Float { GetLengthSq() }

    /// A normalized copy of the vector
    public var normalized: Vec4f { GetNormalized() }

    /// Dot product with another vector
    public func dot(_ other: Vec4f) -> Float { Dot(other) }

    /// Get the first 3 components as Vec3f
    public var xyz: Vec3f { GetVec3() }

    /// Zero vector
    public static var zero: Vec4f { Vec4f.Zero() }
}

// MARK: - Vec4d Extensions

extension Vec4d: CustomStringConvertible {
    public var description: String {
        return "Vec4d(\(x), \(y), \(z), \(w))"
    }
}

extension Vec4d: Equatable {
    public static func == (lhs: Vec4d, rhs: Vec4d) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}

extension Vec4d {
    /// Create a vector from a tuple
    public init(_ tuple: (Double, Double, Double, Double)) {
        self.init(tuple.0, tuple.1, tuple.2, tuple.3)
    }

    /// The length (magnitude) of the vector
    public var length: Double { GetLength() }

    /// The squared length of the vector (faster than length)
    public var lengthSquared: Double { GetLengthSq() }

    /// A normalized copy of the vector
    public var normalized: Vec4d { GetNormalized() }

    /// Dot product with another vector
    public func dot(_ other: Vec4d) -> Double { Dot(other) }

    /// Get the first 3 components as Vec3d
    public var xyz: Vec3d { GetVec3() }

    /// Zero vector
    public static var zero: Vec4d { Vec4d.Zero() }
}

// MARK: - Matrix4f Extensions

extension Matrix4f: CustomStringConvertible {
    public var description: String {
        return "Matrix4f(identity: \(self == Matrix4f.Identity()))"
    }
}

extension Matrix4f {
    /// Create an identity matrix
    public static var identity: Matrix4f { Matrix4f.Identity() }

    /// Create a translation matrix
    public static func translation(_ v: Vec3f) -> Matrix4f {
        return Matrix4f.FromTranslation(v)
    }

    /// Create a scale matrix
    public static func scale(_ v: Vec3f) -> Matrix4f {
        return Matrix4f.FromScale(v)
    }

    /// Create a rotation matrix around X axis
    public static func rotationX(_ radians: Float) -> Matrix4f {
        return Matrix4f.FromRotationX(radians)
    }

    /// Create a rotation matrix around Y axis
    public static func rotationY(_ radians: Float) -> Matrix4f {
        return Matrix4f.FromRotationY(radians)
    }

    /// Create a rotation matrix around Z axis
    public static func rotationZ(_ radians: Float) -> Matrix4f {
        return Matrix4f.FromRotationZ(radians)
    }

    /// The inverse of this matrix
    public var inverse: Matrix4f { GetInverse() }

    /// The transpose of this matrix
    public var transpose: Matrix4f { GetTranspose() }

    /// The determinant of this matrix
    public var determinant: Float { GetDeterminant() }

    /// Transform a point by this matrix
    public func transform(point: Vec3f) -> Vec3f {
        return TransformPoint(point)
    }

    /// Transform a direction by this matrix (ignores translation)
    public func transform(direction: Vec3f) -> Vec3f {
        return TransformDirection(direction)
    }
}

// MARK: - Matrix4d Extensions

extension Matrix4d: CustomStringConvertible {
    public var description: String {
        return "Matrix4d(identity: \(self == Matrix4d.Identity()))"
    }
}

extension Matrix4d {
    /// Create an identity matrix
    public static var identity: Matrix4d { Matrix4d.Identity() }

    /// Create a translation matrix
    public static func translation(_ v: Vec3d) -> Matrix4d {
        return Matrix4d.FromTranslation(v)
    }

    /// Create a scale matrix
    public static func scale(_ v: Vec3d) -> Matrix4d {
        return Matrix4d.FromScale(v)
    }

    /// Create a rotation matrix around X axis
    public static func rotationX(_ radians: Double) -> Matrix4d {
        return Matrix4d.FromRotationX(radians)
    }

    /// Create a rotation matrix around Y axis
    public static func rotationY(_ radians: Double) -> Matrix4d {
        return Matrix4d.FromRotationY(radians)
    }

    /// Create a rotation matrix around Z axis
    public static func rotationZ(_ radians: Double) -> Matrix4d {
        return Matrix4d.FromRotationZ(radians)
    }

    /// The inverse of this matrix
    public var inverse: Matrix4d { GetInverse() }

    /// The transpose of this matrix
    public var transpose: Matrix4d { GetTranspose() }

    /// The determinant of this matrix
    public var determinant: Double { GetDeterminant() }

    /// Transform a point by this matrix
    public func transform(point: Vec3d) -> Vec3d {
        return TransformPoint(point)
    }

    /// Transform a direction by this matrix (ignores translation)
    public func transform(direction: Vec3d) -> Vec3d {
        return TransformDirection(direction)
    }

    /// Convert to single-precision
    public var toFloat: Matrix4f { ToFloat() }
}

// MARK: - Quatf Extensions

extension Quatf: CustomStringConvertible {
    public var description: String {
        return "Quatf(w: \(w), x: \(x), y: \(y), z: \(z))"
    }
}

extension Quatf: Equatable {
    public static func == (lhs: Quatf, rhs: Quatf) -> Bool {
        return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension Quatf {
    /// Create an identity quaternion (no rotation)
    public static var identity: Quatf { Quatf.Identity() }

    /// Create a quaternion from an axis and angle
    public static func axisAngle(_ axis: Vec3f, radians: Float) -> Quatf {
        return Quatf.FromAxisAngle(axis, radians)
    }

    /// Create a quaternion from a rotation matrix
    public static func fromMatrix(_ matrix: Matrix4f) -> Quatf {
        return Quatf.FromRotationMatrix(matrix)
    }

    /// The length of the quaternion
    public var length: Float { GetLength() }

    /// The squared length of the quaternion
    public var lengthSquared: Float { GetLengthSq() }

    /// A normalized copy of the quaternion
    public var normalized: Quatf { GetNormalized() }

    /// The conjugate of the quaternion
    public var conjugate: Quatf { GetConjugate() }

    /// The inverse of the quaternion
    public var inverse: Quatf { GetInverse() }

    /// The real (scalar) part of the quaternion
    public var real: Float { GetReal() }

    /// The imaginary (vector) part of the quaternion
    public var imaginary: Vec3f { GetImaginary() }

    /// Convert to a rotation matrix
    public var matrix: Matrix4f { ToMatrix() }

    /// Rotate a vector by this quaternion
    public func rotate(_ v: Vec3f) -> Vec3f {
        return RotateVector(v)
    }

    /// Spherical linear interpolation between two quaternions
    public static func slerp(_ q0: Quatf, _ q1: Quatf, t: Float) -> Quatf {
        return Quatf.Slerp(q0, q1, t)
    }
}

// MARK: - Quatd Extensions

extension Quatd: CustomStringConvertible {
    public var description: String {
        return "Quatd(w: \(w), x: \(x), y: \(y), z: \(z))"
    }
}

extension Quatd: Equatable {
    public static func == (lhs: Quatd, rhs: Quatd) -> Bool {
        return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension Quatd {
    /// Create an identity quaternion (no rotation)
    public static var identity: Quatd { Quatd.Identity() }

    /// Create a quaternion from an axis and angle
    public static func axisAngle(_ axis: Vec3d, radians: Double) -> Quatd {
        return Quatd.FromAxisAngle(axis, radians)
    }

    /// Create a quaternion from a rotation matrix
    public static func fromMatrix(_ matrix: Matrix4d) -> Quatd {
        return Quatd.FromRotationMatrix(matrix)
    }

    /// The length of the quaternion
    public var length: Double { GetLength() }

    /// The squared length of the quaternion
    public var lengthSquared: Double { GetLengthSq() }

    /// A normalized copy of the quaternion
    public var normalized: Quatd { GetNormalized() }

    /// The conjugate of the quaternion
    public var conjugate: Quatd { GetConjugate() }

    /// The inverse of the quaternion
    public var inverse: Quatd { GetInverse() }

    /// The real (scalar) part of the quaternion
    public var real: Double { GetReal() }

    /// The imaginary (vector) part of the quaternion
    public var imaginary: Vec3d { GetImaginary() }

    /// Convert to a rotation matrix
    public var matrix: Matrix4d { ToMatrix() }

    /// Convert to single-precision
    public var toFloat: Quatf { ToFloat() }

    /// Rotate a vector by this quaternion
    public func rotate(_ v: Vec3d) -> Vec3d {
        return RotateVector(v)
    }

    /// Spherical linear interpolation between two quaternions
    public static func slerp(_ q0: Quatd, _ q1: Quatd, t: Double) -> Quatd {
        return Quatd.Slerp(q0, q1, t)
    }
}

// MARK: - Range3d Extensions

extension Range3d: CustomStringConvertible {
    public var description: String {
        if IsEmpty() {
            return "Range3d(empty)"
        }
        return "Range3d(min: \(min), max: \(max))"
    }
}

extension Range3d {
    /// Create an empty range
    public static var empty: Range3d { Range3d.Empty() }

    /// Whether this range is empty (invalid)
    public var isEmpty: Bool { IsEmpty() }

    /// The size of the range along each axis
    public var size: Vec3d { GetSize() }

    /// The center point of the range
    public var center: Vec3d { GetMidpoint() }

    /// Check if the range contains a point
    public func contains(_ point: Vec3d) -> Bool {
        return Contains(point)
    }

    /// Check if this range contains another range
    public func contains(_ other: Range3d) -> Bool {
        return Contains(other)
    }

    /// Check if this range intersects another range
    public func intersects(_ other: Range3d) -> Bool {
        return Intersects(other)
    }

    /// Get the intersection of this range with another
    public func intersection(with other: Range3d) -> Range3d {
        return GetIntersection(other)
    }

    /// Get the union of this range with another
    public func union(with other: Range3d) -> Range3d {
        return GetUnion(other)
    }

    /// Get the closest point in the range to the given point
    public func closestPoint(to point: Vec3d) -> Vec3d {
        return GetClosestPoint(point)
    }

    /// Get the distance from a point to the range (0 if inside)
    public func distance(to point: Vec3d) -> Double {
        return GetDistanceToPoint(point)
    }
}

// MARK: - BBox3d Extensions

extension BBox3d: CustomStringConvertible {
    public var description: String {
        if IsEmpty() {
            return "BBox3d(empty)"
        }
        return "BBox3d(range: \(GetRange()))"
    }
}

extension BBox3d {
    /// Whether this bounding box is empty
    public var isEmpty: Bool { IsEmpty() }

    /// The underlying range (in local space)
    public var range: Range3d { GetRange().pointee }

    /// The transformation matrix
    public var matrix: Matrix4d { GetMatrix().pointee }

    /// The volume of the box
    public var volume: Double { GetVolume() }

    /// The center of the box (in world space)
    public var centroid: Vec3d { GetCentroid() }

    /// Compute the axis-aligned bounding box in world space
    public var alignedRange: Range3d { ComputeAlignedRange() }

    /// Combine two bounding boxes
    public static func combine(_ b1: BBox3d, _ b2: BBox3d) -> BBox3d {
        return BBox3d.Combine(b1, b2)
    }
}

// MARK: - Math Constants

/// Mathematical constants for graphics calculations
public enum GfMath {
    /// Pi (single precision)
    public static let pi: Float = 3.14159265358979323846

    /// Pi (double precision)
    public static let piDouble: Double = 3.14159265358979323846

    /// Convert degrees to radians (single precision)
    public static func radians(fromDegrees degrees: Float) -> Float {
        return degrees * (pi / 180.0)
    }

    /// Convert degrees to radians (double precision)
    public static func radians(fromDegrees degrees: Double) -> Double {
        return degrees * (piDouble / 180.0)
    }

    /// Convert radians to degrees (single precision)
    public static func degrees(fromRadians radians: Float) -> Float {
        return radians * (180.0 / pi)
    }

    /// Convert radians to degrees (double precision)
    public static func degrees(fromRadians radians: Double) -> Double {
        return radians * (180.0 / piDouble)
    }
}
