// GfCxx.swift - Swift extensions for Gf types via C++ interop
// Provides Swift-friendly API for swiftusd::Vec*, Matrix*, Quat*, Range*, BBox* types

import USDCxx

// MARK: - Type Aliases

/// Type aliases that map to the C++ types in swiftusd namespace
public typealias GfVec2f = swiftusd.Vec2f
public typealias GfVec2d = swiftusd.Vec2d
public typealias GfVec2i = swiftusd.Vec2i
public typealias GfVec3f = swiftusd.Vec3f
public typealias GfVec3d = swiftusd.Vec3d
public typealias GfVec3i = swiftusd.Vec3i
public typealias GfVec4f = swiftusd.Vec4f
public typealias GfVec4d = swiftusd.Vec4d
public typealias GfMatrix4d = swiftusd.Matrix4d
public typealias GfMatrix4f = swiftusd.Matrix4f
public typealias GfQuatd = swiftusd.Quatd
public typealias GfQuatf = swiftusd.Quatf
public typealias GfRange3d = swiftusd.Range3d
public typealias GfRange3f = swiftusd.Range3f
public typealias GfBBox3d = swiftusd.BBox3d

// MARK: - Vec2f Extensions

extension swiftusd.Vec2f: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y
    }
}

extension swiftusd.Vec2f: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
    }
}

extension swiftusd.Vec2f: CustomStringConvertible {
    public var description: String {
        "Vec2f(\(x), \(y))"
    }
}

extension swiftusd.Vec2f: @unchecked Sendable {}

// MARK: - Vec2d Extensions

extension swiftusd.Vec2d: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y
    }
}

extension swiftusd.Vec2d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
    }
}

extension swiftusd.Vec2d: CustomStringConvertible {
    public var description: String {
        "Vec2d(\(x), \(y))"
    }
}

extension swiftusd.Vec2d: @unchecked Sendable {}

// MARK: - Vec2i Extensions

extension swiftusd.Vec2i: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y
    }
}

extension swiftusd.Vec2i: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
    }
}

extension swiftusd.Vec2i: CustomStringConvertible {
    public var description: String {
        "Vec2i(\(x), \(y))"
    }
}

extension swiftusd.Vec2i: @unchecked Sendable {}

// MARK: - Vec3f Extensions

extension swiftusd.Vec3f: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension swiftusd.Vec3f: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
        hasher.combine(z)
    }
}

extension swiftusd.Vec3f: CustomStringConvertible {
    public var description: String {
        "Vec3f(\(x), \(y), \(z))"
    }
}

extension swiftusd.Vec3f: @unchecked Sendable {}

extension swiftusd.Vec3f {
    /// Check if two vectors are approximately equal within a tolerance.
    public func isClose(to other: Self, tolerance: Float = 1e-6) -> Bool {
        let diff = self - other
        return diff.GetLengthSq() <= tolerance * tolerance
    }
}

// MARK: - Vec3d Extensions

extension swiftusd.Vec3d: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension swiftusd.Vec3d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
        hasher.combine(z)
    }
}

extension swiftusd.Vec3d: CustomStringConvertible {
    public var description: String {
        "Vec3d(\(x), \(y), \(z))"
    }
}

extension swiftusd.Vec3d: @unchecked Sendable {}

extension swiftusd.Vec3d {
    /// Check if two vectors are approximately equal within a tolerance.
    public func isClose(to other: Self, tolerance: Double = 1e-10) -> Bool {
        let diff = self - other
        return diff.GetLengthSq() <= tolerance * tolerance
    }
}

// MARK: - Vec3i Extensions

extension swiftusd.Vec3i: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}

extension swiftusd.Vec3i: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
        hasher.combine(z)
    }
}

extension swiftusd.Vec3i: CustomStringConvertible {
    public var description: String {
        "Vec3i(\(x), \(y), \(z))"
    }
}

extension swiftusd.Vec3i: @unchecked Sendable {}

// MARK: - Vec4f Extensions

extension swiftusd.Vec4f: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}

extension swiftusd.Vec4f: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
        hasher.combine(z)
        hasher.combine(w)
    }
}

extension swiftusd.Vec4f: CustomStringConvertible {
    public var description: String {
        "Vec4f(\(x), \(y), \(z), \(w))"
    }
}

extension swiftusd.Vec4f: @unchecked Sendable {}

// MARK: - Vec4d Extensions

extension swiftusd.Vec4d: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}

extension swiftusd.Vec4d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(x)
        hasher.combine(y)
        hasher.combine(z)
        hasher.combine(w)
    }
}

extension swiftusd.Vec4d: CustomStringConvertible {
    public var description: String {
        "Vec4d(\(x), \(y), \(z), \(w))"
    }
}

extension swiftusd.Vec4d: @unchecked Sendable {}

// MARK: - Matrix4d Extensions

extension swiftusd.Matrix4d: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        let m1 = lhs.m
        let m2 = rhs.m
        return m1.0 == m2.0 && m1.1 == m2.1 && m1.2 == m2.2 && m1.3 == m2.3 &&
               m1.4 == m2.4 && m1.5 == m2.5 && m1.6 == m2.6 && m1.7 == m2.7 &&
               m1.8 == m2.8 && m1.9 == m2.9 && m1.10 == m2.10 && m1.11 == m2.11 &&
               m1.12 == m2.12 && m1.13 == m2.13 && m1.14 == m2.14 && m1.15 == m2.15
    }
}

extension swiftusd.Matrix4d: CustomStringConvertible {
    public var description: String {
        let m = self.m
        return """
        Matrix4d([
          [\(m.0), \(m.1), \(m.2), \(m.3)],
          [\(m.4), \(m.5), \(m.6), \(m.7)],
          [\(m.8), \(m.9), \(m.10), \(m.11)],
          [\(m.12), \(m.13), \(m.14), \(m.15)]
        ])
        """
    }
}

extension swiftusd.Matrix4d: @unchecked Sendable {}

extension swiftusd.Matrix4d {
    /// Check if two matrices are approximately equal within a tolerance.
    public func isClose(to other: Self, tolerance: Double = 1e-10) -> Bool {
        let m1 = self.m
        let m2 = other.m
        return Swift.abs(m1.0 - m2.0) <= tolerance &&
               Swift.abs(m1.1 - m2.1) <= tolerance &&
               Swift.abs(m1.2 - m2.2) <= tolerance &&
               Swift.abs(m1.3 - m2.3) <= tolerance &&
               Swift.abs(m1.4 - m2.4) <= tolerance &&
               Swift.abs(m1.5 - m2.5) <= tolerance &&
               Swift.abs(m1.6 - m2.6) <= tolerance &&
               Swift.abs(m1.7 - m2.7) <= tolerance &&
               Swift.abs(m1.8 - m2.8) <= tolerance &&
               Swift.abs(m1.9 - m2.9) <= tolerance &&
               Swift.abs(m1.10 - m2.10) <= tolerance &&
               Swift.abs(m1.11 - m2.11) <= tolerance &&
               Swift.abs(m1.12 - m2.12) <= tolerance &&
               Swift.abs(m1.13 - m2.13) <= tolerance &&
               Swift.abs(m1.14 - m2.14) <= tolerance &&
               Swift.abs(m1.15 - m2.15) <= tolerance
    }

    /// Create a translation matrix.
    public static func translation(_ x: Double, _ y: Double, _ z: Double) -> Self {
        SetTranslate(x, y, z)
    }

    /// Create a translation matrix from a vector.
    public static func translation(_ v: swiftusd.Vec3d) -> Self {
        SetTranslate(v)
    }

    /// Create a uniform scale matrix.
    public static func scale(_ s: Double) -> Self {
        SetScale(s)
    }

    /// Create a non-uniform scale matrix.
    public static func scale(_ x: Double, _ y: Double, _ z: Double) -> Self {
        SetScale(x, y, z)
    }

    /// Create a scale matrix from a vector.
    public static func scale(_ v: swiftusd.Vec3d) -> Self {
        SetScale(v)
    }
}

// MARK: - Matrix4f Extensions

extension swiftusd.Matrix4f: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        let m1 = lhs.m
        let m2 = rhs.m
        return m1.0 == m2.0 && m1.1 == m2.1 && m1.2 == m2.2 && m1.3 == m2.3 &&
               m1.4 == m2.4 && m1.5 == m2.5 && m1.6 == m2.6 && m1.7 == m2.7 &&
               m1.8 == m2.8 && m1.9 == m2.9 && m1.10 == m2.10 && m1.11 == m2.11 &&
               m1.12 == m2.12 && m1.13 == m2.13 && m1.14 == m2.14 && m1.15 == m2.15
    }
}

extension swiftusd.Matrix4f: CustomStringConvertible {
    public var description: String {
        let m = self.m
        return """
        Matrix4f([
          [\(m.0), \(m.1), \(m.2), \(m.3)],
          [\(m.4), \(m.5), \(m.6), \(m.7)],
          [\(m.8), \(m.9), \(m.10), \(m.11)],
          [\(m.12), \(m.13), \(m.14), \(m.15)]
        ])
        """
    }
}

extension swiftusd.Matrix4f: @unchecked Sendable {}

// MARK: - Quatd Extensions

extension swiftusd.Quatd: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.real == rhs.real && lhs.i == rhs.i && lhs.j == rhs.j && lhs.k == rhs.k
    }
}

extension swiftusd.Quatd: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(real)
        hasher.combine(i)
        hasher.combine(j)
        hasher.combine(k)
    }
}

extension swiftusd.Quatd: CustomStringConvertible {
    public var description: String {
        "Quatd(real: \(real), i: \(i), j: \(j), k: \(k))"
    }
}

extension swiftusd.Quatd: @unchecked Sendable {}

extension swiftusd.Quatd {
    /// Perform spherical linear interpolation.
    public static func slerp(from q0: Self, to q1: Self, t: Double) -> Self {
        Slerp(q0, q1, t)
    }

    /// Create a rotation quaternion from axis and angle.
    public static func fromAxisAngle(axis: swiftusd.Vec3d, radians: Double) -> Self {
        FromAxisAngle(axis, radians)
    }
}

// MARK: - Quatf Extensions

extension swiftusd.Quatf: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.real == rhs.real && lhs.i == rhs.i && lhs.j == rhs.j && lhs.k == rhs.k
    }
}

extension swiftusd.Quatf: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(real)
        hasher.combine(i)
        hasher.combine(j)
        hasher.combine(k)
    }
}

extension swiftusd.Quatf: CustomStringConvertible {
    public var description: String {
        "Quatf(real: \(real), i: \(i), j: \(j), k: \(k))"
    }
}

extension swiftusd.Quatf: @unchecked Sendable {}

// MARK: - Range3d Extensions

extension swiftusd.Range3d: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.min == rhs.min && lhs.max == rhs.max
    }
}

extension swiftusd.Range3d: CustomStringConvertible {
    public var description: String {
        "Range3d(min: \(min), max: \(max))"
    }
}

extension swiftusd.Range3d: @unchecked Sendable {}

extension swiftusd.Range3d {
    /// Create a range from min/max coordinate values.
    public init(
        minX: Double, minY: Double, minZ: Double,
        maxX: Double, maxY: Double, maxZ: Double
    ) {
        self.init(
            swiftusd.Vec3d(minX, minY, minZ),
            swiftusd.Vec3d(maxX, maxY, maxZ)
        )
    }

    /// Return the union of two ranges.
    public static func union(_ a: Self, _ b: Self) -> Self {
        GetUnion(a, b)
    }

    /// Return the intersection of two ranges.
    public static func intersection(_ a: Self, _ b: Self) -> Self {
        GetIntersection(a, b)
    }
}

// MARK: - Range3f Extensions

extension swiftusd.Range3f: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.min == rhs.min && lhs.max == rhs.max
    }
}

extension swiftusd.Range3f: CustomStringConvertible {
    public var description: String {
        "Range3f(min: \(min), max: \(max))"
    }
}

extension swiftusd.Range3f: @unchecked Sendable {}

// MARK: - BBox3d Extensions

extension swiftusd.BBox3d: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.range == rhs.range && lhs.matrix == rhs.matrix &&
        lhs.hasZeroAreaPrimitives == rhs.hasZeroAreaPrimitives
    }
}

extension swiftusd.BBox3d: CustomStringConvertible {
    public var description: String {
        "BBox3d(range: \(range), hasZeroArea: \(hasZeroAreaPrimitives))"
    }
}

extension swiftusd.BBox3d: @unchecked Sendable {}

extension swiftusd.BBox3d {
    /// Create a BBox3d from a range with identity transform.
    public init(range: swiftusd.Range3d) {
        self.init(range)
    }

    /// Create a BBox3d from a range and transform matrix.
    public init(range: swiftusd.Range3d, matrix: swiftusd.Matrix4d) {
        self.init(range, matrix)
    }

    /// Combine two bounding boxes.
    public static func combine(_ b1: Self, _ b2: Self) -> Self {
        Combine(b1, b2)
    }
}

// MARK: - Utility Extensions

/// Utility functions from the Gf module
public enum Gf {
    /// Linearly interpolate between two values.
    public static func lerp(_ a: Double, _ b: Double, _ t: Double) -> Double {
        swiftusd.Lerp(a, b, t)
    }

    /// Clamp a value to a range.
    public static func clamp(_ value: Double, min: Double, max: Double) -> Double {
        swiftusd.Clamp(value, min, max)
    }

    /// Convert degrees to radians.
    public static func degreesToRadians(_ degrees: Double) -> Double {
        swiftusd.DegreesToRadians(degrees)
    }

    /// Convert radians to degrees.
    public static func radiansToDegrees(_ radians: Double) -> Double {
        swiftusd.RadiansToDegrees(radians)
    }

    /// Check if two values are approximately equal.
    public static func isClose(_ a: Double, _ b: Double, tolerance: Double = 1e-10) -> Bool {
        swiftusd.IsClose(a, b, tolerance)
    }
}
