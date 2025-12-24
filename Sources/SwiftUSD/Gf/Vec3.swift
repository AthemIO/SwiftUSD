// Vec3.swift - GfVec3f and GfVec3d wrappers
// Mirrors: pxr/base/gf/vec3f.h, pxr/base/gf/vec3d.h

import OpenUSDInterop

// MARK: - Vec3f

/// A 3D vector of single-precision floating-point values.
///
/// `Vec3f` is commonly used for positions, directions, and normals in 3D graphics.
/// It provides efficient vector operations including dot product, cross product,
/// normalization, and component-wise arithmetic.
///
/// Mirrors `pxr::GfVec3f` from the USD C++ API.
public final class Vec3f: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: GfVec3fRef

    // MARK: - Initialization

    /// Creates a vector with the specified components.
    ///
    /// - Parameters:
    ///   - x: The X component.
    ///   - y: The Y component.
    ///   - z: The Z component.
    /// - Throws: `GfError.creationFailed` if the vector cannot be created.
    public init(_ x: Float, _ y: Float, _ z: Float) throws {
        guard let ref = GfVec3f_Create(x, y, z) else {
            throw GfError.creationFailed("Failed to create Vec3f")
        }
        self.handle = ref
    }

    /// Creates a vector with all components set to the same value.
    ///
    /// - Parameter scalar: The value for all components.
    /// - Throws: `GfError.creationFailed` if the vector cannot be created.
    public init(scalar: Float) throws {
        guard let ref = GfVec3f_CreateScalar(scalar) else {
            throw GfError.creationFailed("Failed to create Vec3f from scalar")
        }
        self.handle = ref
    }

    /// Creates a zero vector (0, 0, 0).
    ///
    /// - Throws: `GfError.creationFailed` if the vector cannot be created.
    public init() throws {
        guard let ref = GfVec3f_CreateZero() else {
            throw GfError.creationFailed("Failed to create zero Vec3f")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: GfVec3fRef) {
        self.handle = handle
    }

    deinit {
        GfVec3f_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Returns a zero vector (0, 0, 0).
    public static var zero: Vec3f {
        get throws {
            try Vec3f()
        }
    }

    /// Returns the X axis unit vector (1, 0, 0).
    public static var xAxis: Vec3f {
        get throws {
            guard let ref = GfVec3f_XAxis() else {
                throw GfError.creationFailed("Failed to create X axis")
            }
            return Vec3f(handle: ref)
        }
    }

    /// Returns the Y axis unit vector (0, 1, 0).
    public static var yAxis: Vec3f {
        get throws {
            guard let ref = GfVec3f_YAxis() else {
                throw GfError.creationFailed("Failed to create Y axis")
            }
            return Vec3f(handle: ref)
        }
    }

    /// Returns the Z axis unit vector (0, 0, 1).
    public static var zAxis: Vec3f {
        get throws {
            guard let ref = GfVec3f_ZAxis() else {
                throw GfError.creationFailed("Failed to create Z axis")
            }
            return Vec3f(handle: ref)
        }
    }

    // MARK: - Component Access

    /// The X component of the vector.
    public var x: Float {
        get { GfVec3f_GetX(handle) }
        set { GfVec3f_Set(handle, newValue, y, z) }
    }

    /// The Y component of the vector.
    public var y: Float {
        get { GfVec3f_GetY(handle) }
        set { GfVec3f_Set(handle, x, newValue, z) }
    }

    /// The Z component of the vector.
    public var z: Float {
        get { GfVec3f_GetZ(handle) }
        set { GfVec3f_Set(handle, x, y, newValue) }
    }

    /// Gets all components as a tuple.
    public var components: (x: Float, y: Float, z: Float) {
        var outX: Float = 0, outY: Float = 0, outZ: Float = 0
        GfVec3f_GetComponents(handle, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Sets all components.
    public func set(_ x: Float, _ y: Float, _ z: Float) {
        GfVec3f_Set(handle, x, y, z)
    }

    // MARK: - Properties

    /// The length (magnitude) of the vector.
    public var length: Float {
        GfVec3f_GetLength(handle)
    }

    /// The squared length of the vector (faster than `length`).
    public var lengthSquared: Float {
        GfVec3f_GetLengthSq(handle)
    }

    // MARK: - Operations

    /// Returns a normalized copy of the vector.
    ///
    /// - Returns: A unit vector in the same direction.
    /// - Throws: `GfError.creationFailed` if normalization fails.
    public func normalized() throws -> Vec3f {
        guard let ref = GfVec3f_GetNormalized(handle) else {
            throw GfError.creationFailed("Failed to normalize Vec3f")
        }
        return Vec3f(handle: ref)
    }

    /// Normalizes the vector in place.
    ///
    /// - Returns: The original length before normalization.
    @discardableResult
    public func normalize() -> Float {
        GfVec3f_Normalize(handle)
    }

    /// Returns the dot product with another vector.
    public func dot(_ other: Vec3f) -> Float {
        GfVec3f_Dot(handle, other.handle)
    }

    /// Returns the cross product with another vector.
    ///
    /// - Parameter other: The other vector.
    /// - Returns: The cross product vector.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func cross(_ other: Vec3f) throws -> Vec3f {
        guard let ref = GfVec3f_Cross(handle, other.handle) else {
            throw GfError.creationFailed("Failed to compute cross product")
        }
        return Vec3f(handle: ref)
    }

    /// Returns the projection of this vector onto another.
    ///
    /// - Parameter onto: The vector to project onto.
    /// - Returns: The projection vector.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func projection(onto: Vec3f) throws -> Vec3f {
        guard let ref = GfVec3f_GetProjection(handle, onto.handle) else {
            throw GfError.creationFailed("Failed to compute projection")
        }
        return Vec3f(handle: ref)
    }

    /// Returns the orthogonal complement of this vector's projection onto another.
    ///
    /// - Parameter onto: The vector to compute complement with respect to.
    /// - Returns: The complement vector.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func complement(onto: Vec3f) throws -> Vec3f {
        guard let ref = GfVec3f_GetComplement(handle, onto.handle) else {
            throw GfError.creationFailed("Failed to compute complement")
        }
        return Vec3f(handle: ref)
    }

    /// Creates a copy of this vector.
    ///
    /// - Returns: A new vector with the same components.
    /// - Throws: `GfError.creationFailed` if the copy fails.
    public func copy() throws -> Vec3f {
        guard let ref = GfVec3f_Copy(handle) else {
            throw GfError.creationFailed("Failed to copy Vec3f")
        }
        return Vec3f(handle: ref)
    }

    /// Returns true if this vector is close to another within a tolerance.
    ///
    /// - Parameters:
    ///   - other: The other vector.
    ///   - tolerance: The maximum allowed difference in length.
    /// - Returns: True if the vectors are close.
    public func isClose(to other: Vec3f, tolerance: Float) -> Bool {
        GfVec3f_IsClose(handle, other.handle, tolerance)
    }
}

// MARK: - Vec3f Operators

extension Vec3f {

    /// Returns the sum of two vectors.
    public static func + (lhs: Vec3f, rhs: Vec3f) -> Vec3f? {
        guard let ref = GfVec3f_Add(lhs.handle, rhs.handle) else { return nil }
        return Vec3f(handle: ref)
    }

    /// Returns the difference of two vectors.
    public static func - (lhs: Vec3f, rhs: Vec3f) -> Vec3f? {
        guard let ref = GfVec3f_Subtract(lhs.handle, rhs.handle) else { return nil }
        return Vec3f(handle: ref)
    }

    /// Returns the vector multiplied by a scalar.
    public static func * (lhs: Vec3f, rhs: Float) -> Vec3f? {
        guard let ref = GfVec3f_Multiply(lhs.handle, rhs) else { return nil }
        return Vec3f(handle: ref)
    }

    /// Returns the vector multiplied by a scalar.
    public static func * (lhs: Float, rhs: Vec3f) -> Vec3f? {
        rhs * lhs
    }

    /// Returns the vector divided by a scalar.
    public static func / (lhs: Vec3f, rhs: Float) -> Vec3f? {
        guard let ref = GfVec3f_Divide(lhs.handle, rhs) else { return nil }
        return Vec3f(handle: ref)
    }

    /// Returns the negation of the vector.
    public static prefix func - (vec: Vec3f) -> Vec3f? {
        guard let ref = GfVec3f_Negate(vec.handle) else { return nil }
        return Vec3f(handle: ref)
    }
}

// MARK: - Vec3f Protocol Conformances

extension Vec3f: Equatable {
    public static func == (lhs: Vec3f, rhs: Vec3f) -> Bool {
        GfVec3f_Equal(lhs.handle, rhs.handle)
    }
}

extension Vec3f: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(GfVec3f_Hash(handle))
    }
}

extension Vec3f: CustomStringConvertible {
    public var description: String {
        "(\(x), \(y), \(z))"
    }
}

extension Vec3f: CustomDebugStringConvertible {
    public var debugDescription: String {
        "Vec3f(\(x), \(y), \(z))"
    }
}

// MARK: - Vec3d

/// A 3D vector of double-precision floating-point values.
///
/// `Vec3d` provides higher precision than `Vec3f` and is commonly used for
/// positions, directions, and normals in 3D graphics where precision is critical.
///
/// Mirrors `pxr::GfVec3d` from the USD C++ API.
public final class Vec3d: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: GfVec3dRef

    // MARK: - Initialization

    /// Creates a vector with the specified components.
    ///
    /// - Parameters:
    ///   - x: The X component.
    ///   - y: The Y component.
    ///   - z: The Z component.
    /// - Throws: `GfError.creationFailed` if the vector cannot be created.
    public init(_ x: Double, _ y: Double, _ z: Double) throws {
        guard let ref = GfVec3d_Create(x, y, z) else {
            throw GfError.creationFailed("Failed to create Vec3d")
        }
        self.handle = ref
    }

    /// Creates a vector with all components set to the same value.
    ///
    /// - Parameter scalar: The value for all components.
    /// - Throws: `GfError.creationFailed` if the vector cannot be created.
    public init(scalar: Double) throws {
        guard let ref = GfVec3d_CreateScalar(scalar) else {
            throw GfError.creationFailed("Failed to create Vec3d from scalar")
        }
        self.handle = ref
    }

    /// Creates a zero vector (0, 0, 0).
    ///
    /// - Throws: `GfError.creationFailed` if the vector cannot be created.
    public init() throws {
        guard let ref = GfVec3d_CreateZero() else {
            throw GfError.creationFailed("Failed to create zero Vec3d")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: GfVec3dRef) {
        self.handle = handle
    }

    deinit {
        GfVec3d_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Returns a zero vector (0, 0, 0).
    public static var zero: Vec3d {
        get throws {
            try Vec3d()
        }
    }

    /// Returns the X axis unit vector (1, 0, 0).
    public static var xAxis: Vec3d {
        get throws {
            guard let ref = GfVec3d_XAxis() else {
                throw GfError.creationFailed("Failed to create X axis")
            }
            return Vec3d(handle: ref)
        }
    }

    /// Returns the Y axis unit vector (0, 1, 0).
    public static var yAxis: Vec3d {
        get throws {
            guard let ref = GfVec3d_YAxis() else {
                throw GfError.creationFailed("Failed to create Y axis")
            }
            return Vec3d(handle: ref)
        }
    }

    /// Returns the Z axis unit vector (0, 0, 1).
    public static var zAxis: Vec3d {
        get throws {
            guard let ref = GfVec3d_ZAxis() else {
                throw GfError.creationFailed("Failed to create Z axis")
            }
            return Vec3d(handle: ref)
        }
    }

    // MARK: - Component Access

    /// The X component of the vector.
    public var x: Double {
        get { GfVec3d_GetX(handle) }
        set { GfVec3d_Set(handle, newValue, y, z) }
    }

    /// The Y component of the vector.
    public var y: Double {
        get { GfVec3d_GetY(handle) }
        set { GfVec3d_Set(handle, x, newValue, z) }
    }

    /// The Z component of the vector.
    public var z: Double {
        get { GfVec3d_GetZ(handle) }
        set { GfVec3d_Set(handle, x, y, newValue) }
    }

    /// Gets all components as a tuple.
    public var components: (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfVec3d_GetComponents(handle, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Sets all components.
    public func set(_ x: Double, _ y: Double, _ z: Double) {
        GfVec3d_Set(handle, x, y, z)
    }

    // MARK: - Properties

    /// The length (magnitude) of the vector.
    public var length: Double {
        GfVec3d_GetLength(handle)
    }

    /// The squared length of the vector (faster than `length`).
    public var lengthSquared: Double {
        GfVec3d_GetLengthSq(handle)
    }

    // MARK: - Operations

    /// Returns a normalized copy of the vector.
    ///
    /// - Returns: A unit vector in the same direction.
    /// - Throws: `GfError.creationFailed` if normalization fails.
    public func normalized() throws -> Vec3d {
        guard let ref = GfVec3d_GetNormalized(handle) else {
            throw GfError.creationFailed("Failed to normalize Vec3d")
        }
        return Vec3d(handle: ref)
    }

    /// Normalizes the vector in place.
    ///
    /// - Returns: The original length before normalization.
    @discardableResult
    public func normalize() -> Double {
        GfVec3d_Normalize(handle)
    }

    /// Returns the dot product with another vector.
    public func dot(_ other: Vec3d) -> Double {
        GfVec3d_Dot(handle, other.handle)
    }

    /// Returns the cross product with another vector.
    ///
    /// - Parameter other: The other vector.
    /// - Returns: The cross product vector.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func cross(_ other: Vec3d) throws -> Vec3d {
        guard let ref = GfVec3d_Cross(handle, other.handle) else {
            throw GfError.creationFailed("Failed to compute cross product")
        }
        return Vec3d(handle: ref)
    }

    /// Returns the projection of this vector onto another.
    ///
    /// - Parameter onto: The vector to project onto.
    /// - Returns: The projection vector.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func projection(onto: Vec3d) throws -> Vec3d {
        guard let ref = GfVec3d_GetProjection(handle, onto.handle) else {
            throw GfError.creationFailed("Failed to compute projection")
        }
        return Vec3d(handle: ref)
    }

    /// Returns the orthogonal complement of this vector's projection onto another.
    ///
    /// - Parameter onto: The vector to compute complement with respect to.
    /// - Returns: The complement vector.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func complement(onto: Vec3d) throws -> Vec3d {
        guard let ref = GfVec3d_GetComplement(handle, onto.handle) else {
            throw GfError.creationFailed("Failed to compute complement")
        }
        return Vec3d(handle: ref)
    }

    /// Creates a copy of this vector.
    ///
    /// - Returns: A new vector with the same components.
    /// - Throws: `GfError.creationFailed` if the copy fails.
    public func copy() throws -> Vec3d {
        guard let ref = GfVec3d_Copy(handle) else {
            throw GfError.creationFailed("Failed to copy Vec3d")
        }
        return Vec3d(handle: ref)
    }

    /// Returns true if this vector is close to another within a tolerance.
    ///
    /// - Parameters:
    ///   - other: The other vector.
    ///   - tolerance: The maximum allowed difference in length.
    /// - Returns: True if the vectors are close.
    public func isClose(to other: Vec3d, tolerance: Double) -> Bool {
        GfVec3d_IsClose(handle, other.handle, tolerance)
    }
}

// MARK: - Vec3d Operators

extension Vec3d {

    /// Returns the sum of two vectors.
    public static func + (lhs: Vec3d, rhs: Vec3d) -> Vec3d? {
        guard let ref = GfVec3d_Add(lhs.handle, rhs.handle) else { return nil }
        return Vec3d(handle: ref)
    }

    /// Returns the difference of two vectors.
    public static func - (lhs: Vec3d, rhs: Vec3d) -> Vec3d? {
        guard let ref = GfVec3d_Subtract(lhs.handle, rhs.handle) else { return nil }
        return Vec3d(handle: ref)
    }

    /// Returns the vector multiplied by a scalar.
    public static func * (lhs: Vec3d, rhs: Double) -> Vec3d? {
        guard let ref = GfVec3d_Multiply(lhs.handle, rhs) else { return nil }
        return Vec3d(handle: ref)
    }

    /// Returns the vector multiplied by a scalar.
    public static func * (lhs: Double, rhs: Vec3d) -> Vec3d? {
        rhs * lhs
    }

    /// Returns the vector divided by a scalar.
    public static func / (lhs: Vec3d, rhs: Double) -> Vec3d? {
        guard let ref = GfVec3d_Divide(lhs.handle, rhs) else { return nil }
        return Vec3d(handle: ref)
    }

    /// Returns the negation of the vector.
    public static prefix func - (vec: Vec3d) -> Vec3d? {
        guard let ref = GfVec3d_Negate(vec.handle) else { return nil }
        return Vec3d(handle: ref)
    }
}

// MARK: - Vec3d Protocol Conformances

extension Vec3d: Equatable {
    public static func == (lhs: Vec3d, rhs: Vec3d) -> Bool {
        GfVec3d_Equal(lhs.handle, rhs.handle)
    }
}

extension Vec3d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(GfVec3d_Hash(handle))
    }
}

extension Vec3d: CustomStringConvertible {
    public var description: String {
        "(\(x), \(y), \(z))"
    }
}

extension Vec3d: CustomDebugStringConvertible {
    public var debugDescription: String {
        "Vec3d(\(x), \(y), \(z))"
    }
}
