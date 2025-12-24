// Quaternion.swift - GfQuatd wrapper
// Mirrors: pxr/base/gf/quatd.h

import OpenUSDInterop

// MARK: - Quatd

/// A quaternion with double-precision floating-point components.
///
/// A quaternion is a complex number with a real coefficient and three
/// imaginary coefficients (i, j, k). It is commonly used to represent
/// 3D rotations efficiently and without gimbal lock.
///
/// Mirrors `pxr::GfQuatd` from the USD C++ API.
public final class Quatd: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: GfQuatdRef

    // MARK: - Initialization

    /// Creates a quaternion with the specified components.
    ///
    /// - Parameters:
    ///   - real: The real (scalar) component.
    ///   - i: The i imaginary component.
    ///   - j: The j imaginary component.
    ///   - k: The k imaginary component.
    /// - Throws: `GfError.creationFailed` if the quaternion cannot be created.
    public init(real: Double, i: Double, j: Double, k: Double) throws {
        guard let ref = GfQuatd_Create(real, i, j, k) else {
            throw GfError.creationFailed("Failed to create Quatd")
        }
        self.handle = ref
    }

    /// Creates a quaternion with only the real component set.
    ///
    /// - Parameter real: The real component.
    /// - Throws: `GfError.creationFailed` if the quaternion cannot be created.
    public init(real: Double) throws {
        guard let ref = GfQuatd_CreateReal(real) else {
            throw GfError.creationFailed("Failed to create Quatd from real")
        }
        self.handle = ref
    }

    /// Creates an identity quaternion (1, 0, 0, 0).
    ///
    /// The identity quaternion represents no rotation.
    ///
    /// - Throws: `GfError.creationFailed` if the quaternion cannot be created.
    public init() throws {
        guard let ref = GfQuatd_CreateIdentity() else {
            throw GfError.creationFailed("Failed to create identity Quatd")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: GfQuatdRef) {
        self.handle = handle
    }

    deinit {
        GfQuatd_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Returns the identity quaternion (1, 0, 0, 0).
    public static var identity: Quatd {
        get throws {
            try Quatd()
        }
    }

    /// Returns the zero quaternion (0, 0, 0, 0).
    public static var zero: Quatd {
        get throws {
            guard let ref = GfQuatd_CreateZero() else {
                throw GfError.creationFailed("Failed to create zero Quatd")
            }
            return Quatd(handle: ref)
        }
    }

    // MARK: - Component Access

    /// The real (scalar) component of the quaternion.
    public var real: Double {
        get { GfQuatd_GetReal(handle) }
        set { GfQuatd_SetReal(handle, newValue) }
    }

    /// Gets the imaginary components as a tuple.
    public var imaginary: (i: Double, j: Double, k: Double) {
        var i: Double = 0, j: Double = 0, k: Double = 0
        GfQuatd_GetImaginary(handle, &i, &j, &k)
        return (i, j, k)
    }

    /// The i imaginary component.
    public var i: Double {
        imaginary.i
    }

    /// The j imaginary component.
    public var j: Double {
        imaginary.j
    }

    /// The k imaginary component.
    public var k: Double {
        imaginary.k
    }

    /// Sets the imaginary components.
    public func setImaginary(_ i: Double, _ j: Double, _ k: Double) {
        GfQuatd_SetImaginary(handle, i, j, k)
    }

    // MARK: - Properties

    /// The length (magnitude) of the quaternion.
    public var length: Double {
        GfQuatd_GetLength(handle)
    }

    // MARK: - Operations

    /// Returns a normalized copy of the quaternion.
    ///
    /// - Returns: A unit quaternion in the same orientation.
    /// - Throws: `GfError.creationFailed` if normalization fails.
    public func normalized() throws -> Quatd {
        guard let ref = GfQuatd_GetNormalized(handle) else {
            throw GfError.creationFailed("Failed to normalize Quatd")
        }
        return Quatd(handle: ref)
    }

    /// Normalizes the quaternion in place.
    ///
    /// - Returns: The original length before normalization.
    @discardableResult
    public func normalize() -> Double {
        GfQuatd_Normalize(handle)
    }

    /// Returns the conjugate of this quaternion.
    ///
    /// The conjugate has the same real component but negated imaginary components.
    ///
    /// - Returns: The conjugate quaternion.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func conjugate() throws -> Quatd {
        guard let ref = GfQuatd_GetConjugate(handle) else {
            throw GfError.creationFailed("Failed to compute conjugate")
        }
        return Quatd(handle: ref)
    }

    /// Returns the inverse of this quaternion.
    ///
    /// The inverse is the conjugate divided by the squared length.
    ///
    /// - Returns: The inverse quaternion.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func inverse() throws -> Quatd {
        guard let ref = GfQuatd_GetInverse(handle) else {
            throw GfError.creationFailed("Failed to compute inverse")
        }
        return Quatd(handle: ref)
    }

    /// Returns the dot product with another quaternion.
    public func dot(_ other: Quatd) -> Double {
        GfQuatd_Dot(handle, other.handle)
    }

    /// Transforms a point by this quaternion's rotation.
    ///
    /// - Parameter point: The point to transform.
    /// - Returns: The rotated point.
    public func transform(_ point: Vec3d) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfQuatd_Transform(handle, point.x, point.y, point.z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Transforms a point by this quaternion's rotation.
    ///
    /// - Parameters:
    ///   - x: X coordinate.
    ///   - y: Y coordinate.
    ///   - z: Z coordinate.
    /// - Returns: The rotated point.
    public func transform(_ x: Double, _ y: Double, _ z: Double) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfQuatd_Transform(handle, x, y, z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Performs spherical linear interpolation between two quaternions.
    ///
    /// - Parameters:
    ///   - from: The starting quaternion (alpha=0).
    ///   - to: The ending quaternion (alpha=1).
    ///   - alpha: The interpolation parameter (0..1).
    /// - Returns: The interpolated quaternion.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public static func slerp(from: Quatd, to: Quatd, alpha: Double) throws -> Quatd {
        guard let ref = GfQuatd_Slerp(from.handle, to.handle, alpha) else {
            throw GfError.creationFailed("Failed to compute slerp")
        }
        return Quatd(handle: ref)
    }

    /// Creates a copy of this quaternion.
    ///
    /// - Returns: A new quaternion with the same components.
    /// - Throws: `GfError.creationFailed` if the copy fails.
    public func copy() throws -> Quatd {
        guard let ref = GfQuatd_Copy(handle) else {
            throw GfError.creationFailed("Failed to copy Quatd")
        }
        return Quatd(handle: ref)
    }
}

// MARK: - Quatd Operators

extension Quatd {

    /// Returns the product of two quaternions.
    public static func * (lhs: Quatd, rhs: Quatd) -> Quatd? {
        guard let ref = GfQuatd_Multiply(lhs.handle, rhs.handle) else { return nil }
        return Quatd(handle: ref)
    }

    /// Returns the quaternion multiplied by a scalar.
    public static func * (lhs: Quatd, rhs: Double) -> Quatd? {
        guard let ref = GfQuatd_MultiplyScalar(lhs.handle, rhs) else { return nil }
        return Quatd(handle: ref)
    }

    /// Returns the quaternion multiplied by a scalar.
    public static func * (lhs: Double, rhs: Quatd) -> Quatd? {
        rhs * lhs
    }

    /// Returns the sum of two quaternions.
    public static func + (lhs: Quatd, rhs: Quatd) -> Quatd? {
        guard let ref = GfQuatd_Add(lhs.handle, rhs.handle) else { return nil }
        return Quatd(handle: ref)
    }

    /// Returns the difference of two quaternions.
    public static func - (lhs: Quatd, rhs: Quatd) -> Quatd? {
        guard let ref = GfQuatd_Subtract(lhs.handle, rhs.handle) else { return nil }
        return Quatd(handle: ref)
    }

    /// Returns the negation of the quaternion.
    public static prefix func - (quat: Quatd) -> Quatd? {
        guard let ref = GfQuatd_Negate(quat.handle) else { return nil }
        return Quatd(handle: ref)
    }
}

// MARK: - Quatd Protocol Conformances

extension Quatd: Equatable {
    public static func == (lhs: Quatd, rhs: Quatd) -> Bool {
        GfQuatd_Equal(lhs.handle, rhs.handle)
    }
}

extension Quatd: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(GfQuatd_Hash(handle))
    }
}

extension Quatd: CustomStringConvertible {
    public var description: String {
        let (i, j, k) = imaginary
        return "(\(real), \(i), \(j), \(k))"
    }
}

extension Quatd: CustomDebugStringConvertible {
    public var debugDescription: String {
        let (i, j, k) = imaginary
        return "Quatd(real: \(real), i: \(i), j: \(j), k: \(k))"
    }
}
