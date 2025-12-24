// Matrix4.swift - GfMatrix4d wrapper
// Mirrors: pxr/base/gf/matrix4d.h

import OpenUSDInterop

// MARK: - Matrix4d

/// A 4x4 matrix of double-precision floating-point values.
///
/// `Matrix4d` is commonly used for 3D transformations including translation,
/// rotation, and scale. The matrix is stored in row-major order, so
/// `matrix[row][col]` accesses element in row `row` and column `col`.
///
/// Matrices are organized for row vectors, so transformation matrices are
/// applied left-to-right (e.g., `point * M` transforms `point` by `M`).
///
/// Mirrors `pxr::GfMatrix4d` from the USD C++ API.
public final class Matrix4d: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: GfMatrix4dRef

    // MARK: - Initialization

    /// Creates a matrix from 16 values in row-major order.
    ///
    /// - Parameters:
    ///   - m00...m33: Matrix elements where mXY is in row X and column Y.
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public init(
        _ m00: Double, _ m01: Double, _ m02: Double, _ m03: Double,
        _ m10: Double, _ m11: Double, _ m12: Double, _ m13: Double,
        _ m20: Double, _ m21: Double, _ m22: Double, _ m23: Double,
        _ m30: Double, _ m31: Double, _ m32: Double, _ m33: Double
    ) throws {
        guard let ref = GfMatrix4d_Create(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33
        ) else {
            throw GfError.creationFailed("Failed to create Matrix4d")
        }
        self.handle = ref
    }

    /// Creates an identity matrix.
    ///
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public init() throws {
        guard let ref = GfMatrix4d_CreateIdentity() else {
            throw GfError.creationFailed("Failed to create identity Matrix4d")
        }
        self.handle = ref
    }

    /// Creates a diagonal matrix with the given scale factor.
    ///
    /// - Parameter diagonal: The diagonal value (creates `diagonal * identity`).
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public init(diagonal: Double) throws {
        guard let ref = GfMatrix4d_CreateDiagonal(diagonal) else {
            throw GfError.creationFailed("Failed to create diagonal Matrix4d")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: GfMatrix4dRef) {
        self.handle = handle
    }

    deinit {
        GfMatrix4d_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Returns the identity matrix.
    public static var identity: Matrix4d {
        get throws {
            try Matrix4d()
        }
    }

    /// Returns the zero matrix.
    public static var zero: Matrix4d {
        get throws {
            guard let ref = GfMatrix4d_CreateZero() else {
                throw GfError.creationFailed("Failed to create zero Matrix4d")
            }
            return Matrix4d(handle: ref)
        }
    }

    /// Creates a translation matrix.
    ///
    /// - Parameters:
    ///   - x: X translation.
    ///   - y: Y translation.
    ///   - z: Z translation.
    /// - Returns: A translation matrix.
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public static func translation(_ x: Double, _ y: Double, _ z: Double) throws -> Matrix4d {
        guard let ref = GfMatrix4d_SetTranslate(x, y, z) else {
            throw GfError.creationFailed("Failed to create translation matrix")
        }
        return Matrix4d(handle: ref)
    }

    /// Creates a translation matrix from a vector.
    ///
    /// - Parameter translation: The translation vector.
    /// - Returns: A translation matrix.
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public static func translation(_ translation: Vec3d) throws -> Matrix4d {
        try self.translation(translation.x, translation.y, translation.z)
    }

    /// Creates a uniform scale matrix.
    ///
    /// - Parameter scale: The scale factor for all axes.
    /// - Returns: A scale matrix.
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public static func scale(_ scale: Double) throws -> Matrix4d {
        guard let ref = GfMatrix4d_SetScale(scale) else {
            throw GfError.creationFailed("Failed to create scale matrix")
        }
        return Matrix4d(handle: ref)
    }

    /// Creates a non-uniform scale matrix.
    ///
    /// - Parameters:
    ///   - x: X scale factor.
    ///   - y: Y scale factor.
    ///   - z: Z scale factor.
    /// - Returns: A scale matrix.
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public static func scale(_ x: Double, _ y: Double, _ z: Double) throws -> Matrix4d {
        guard let ref = GfMatrix4d_SetScaleVec(x, y, z) else {
            throw GfError.creationFailed("Failed to create non-uniform scale matrix")
        }
        return Matrix4d(handle: ref)
    }

    /// Creates a rotation matrix from a quaternion.
    ///
    /// - Parameter quaternion: The rotation quaternion.
    /// - Returns: A rotation matrix.
    /// - Throws: `GfError.creationFailed` if the matrix cannot be created.
    public static func rotation(_ quaternion: Quatd) throws -> Matrix4d {
        guard let ref = GfMatrix4d_SetRotateQuat(quaternion.handle) else {
            throw GfError.creationFailed("Failed to create rotation matrix")
        }
        return Matrix4d(handle: ref)
    }

    // MARK: - Element Access

    /// Gets the element at the specified row and column.
    ///
    /// - Parameters:
    ///   - row: Row index (0-3).
    ///   - col: Column index (0-3).
    /// - Returns: The element value.
    public subscript(row: Int, col: Int) -> Double {
        get { GfMatrix4d_GetElement(handle, Int32(row), Int32(col)) }
        set { GfMatrix4d_SetElement(handle, Int32(row), Int32(col), newValue) }
    }

    /// Gets all 16 elements as an array in row-major order.
    public var elements: [Double] {
        var arr = [Double](repeating: 0, count: 16)
        GfMatrix4d_GetElements(handle, &arr)
        return arr
    }

    /// Sets all 16 elements from an array in row-major order.
    public func setElements(_ elements: [Double]) {
        guard elements.count == 16 else { return }
        GfMatrix4d_SetElements(handle, elements)
    }

    /// Gets a row as a tuple.
    public func row(_ index: Int) -> (Double, Double, Double, Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0, w: Double = 0
        GfMatrix4d_GetRow(handle, Int32(index), &x, &y, &z, &w)
        return (x, y, z, w)
    }

    /// Gets a column as a tuple.
    public func column(_ index: Int) -> (Double, Double, Double, Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0, w: Double = 0
        GfMatrix4d_GetColumn(handle, Int32(index), &x, &y, &z, &w)
        return (x, y, z, w)
    }

    // MARK: - Matrix Operations

    /// Sets this matrix to identity.
    public func setIdentity() {
        GfMatrix4d_SetIdentity(handle)
    }

    /// Sets this matrix to zero.
    public func setZero() {
        GfMatrix4d_SetZero(handle)
    }

    /// Returns the transpose of this matrix.
    ///
    /// - Returns: The transposed matrix.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func transposed() throws -> Matrix4d {
        guard let ref = GfMatrix4d_GetTranspose(handle) else {
            throw GfError.creationFailed("Failed to transpose matrix")
        }
        return Matrix4d(handle: ref)
    }

    /// Returns the inverse of this matrix.
    ///
    /// - Returns: The inverse matrix, or identity if singular.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public func inverse() throws -> Matrix4d {
        guard let ref = GfMatrix4d_GetInverse(handle) else {
            throw GfError.creationFailed("Failed to invert matrix")
        }
        return Matrix4d(handle: ref)
    }

    /// Returns the determinant of this matrix.
    public var determinant: Double {
        GfMatrix4d_GetDeterminant(handle)
    }

    /// Creates a copy of this matrix.
    ///
    /// - Returns: A new matrix with the same elements.
    /// - Throws: `GfError.creationFailed` if the copy fails.
    public func copy() throws -> Matrix4d {
        guard let ref = GfMatrix4d_Copy(handle) else {
            throw GfError.creationFailed("Failed to copy Matrix4d")
        }
        return Matrix4d(handle: ref)
    }

    // MARK: - Transform Operations

    /// Extracts the translation component from this matrix.
    ///
    /// - Returns: The translation vector.
    public func extractTranslation() -> (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfMatrix4d_ExtractTranslation(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Transforms a point by this matrix (with translation).
    ///
    /// - Parameter point: The point to transform.
    /// - Returns: The transformed point.
    public func transform(_ point: Vec3d) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfMatrix4d_Transform(handle, point.x, point.y, point.z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Transforms a point by this matrix (with translation).
    ///
    /// - Parameters:
    ///   - x: X coordinate.
    ///   - y: Y coordinate.
    ///   - z: Z coordinate.
    /// - Returns: The transformed point.
    public func transform(_ x: Double, _ y: Double, _ z: Double) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfMatrix4d_Transform(handle, x, y, z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Transforms a direction by this matrix (without translation).
    ///
    /// - Parameter direction: The direction to transform.
    /// - Returns: The transformed direction.
    public func transformDirection(_ direction: Vec3d) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfMatrix4d_TransformDir(handle, direction.x, direction.y, direction.z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Transforms a direction by this matrix (without translation).
    ///
    /// - Parameters:
    ///   - x: X component.
    ///   - y: Y component.
    ///   - z: Z component.
    /// - Returns: The transformed direction.
    public func transformDirection(_ x: Double, _ y: Double, _ z: Double) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfMatrix4d_TransformDir(handle, x, y, z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    /// Transforms a point using affine transformation (assumes w=1).
    ///
    /// - Parameters:
    ///   - x: X coordinate.
    ///   - y: Y coordinate.
    ///   - z: Z coordinate.
    /// - Returns: The transformed point.
    public func transformAffine(_ x: Double, _ y: Double, _ z: Double) -> (x: Double, y: Double, z: Double) {
        var outX: Double = 0, outY: Double = 0, outZ: Double = 0
        GfMatrix4d_TransformAffine(handle, x, y, z, &outX, &outY, &outZ)
        return (outX, outY, outZ)
    }

    // MARK: - Comparison

    /// Returns true if this matrix is close to another within a tolerance.
    ///
    /// - Parameters:
    ///   - other: The other matrix.
    ///   - tolerance: The maximum allowed difference per element.
    /// - Returns: True if the matrices are close.
    public func isClose(to other: Matrix4d, tolerance: Double) -> Bool {
        GfMatrix4d_IsClose(handle, other.handle, tolerance)
    }
}

// MARK: - Matrix4d Operators

extension Matrix4d {

    /// Returns the product of two matrices.
    public static func * (lhs: Matrix4d, rhs: Matrix4d) -> Matrix4d? {
        guard let ref = GfMatrix4d_Multiply(lhs.handle, rhs.handle) else { return nil }
        return Matrix4d(handle: ref)
    }

    /// Returns the matrix multiplied by a scalar.
    public static func * (lhs: Matrix4d, rhs: Double) -> Matrix4d? {
        guard let ref = GfMatrix4d_MultiplyScalar(lhs.handle, rhs) else { return nil }
        return Matrix4d(handle: ref)
    }

    /// Returns the matrix multiplied by a scalar.
    public static func * (lhs: Double, rhs: Matrix4d) -> Matrix4d? {
        rhs * lhs
    }

    /// Returns the sum of two matrices.
    public static func + (lhs: Matrix4d, rhs: Matrix4d) -> Matrix4d? {
        guard let ref = GfMatrix4d_Add(lhs.handle, rhs.handle) else { return nil }
        return Matrix4d(handle: ref)
    }

    /// Returns the difference of two matrices.
    public static func - (lhs: Matrix4d, rhs: Matrix4d) -> Matrix4d? {
        guard let ref = GfMatrix4d_Subtract(lhs.handle, rhs.handle) else { return nil }
        return Matrix4d(handle: ref)
    }
}

// MARK: - Matrix4d Protocol Conformances

extension Matrix4d: Equatable {
    public static func == (lhs: Matrix4d, rhs: Matrix4d) -> Bool {
        GfMatrix4d_Equal(lhs.handle, rhs.handle)
    }
}

extension Matrix4d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(GfMatrix4d_Hash(handle))
    }
}

extension Matrix4d: CustomStringConvertible {
    public var description: String {
        let e = elements
        return """
        [[\(e[0]), \(e[1]), \(e[2]), \(e[3])],
         [\(e[4]), \(e[5]), \(e[6]), \(e[7])],
         [\(e[8]), \(e[9]), \(e[10]), \(e[11])],
         [\(e[12]), \(e[13]), \(e[14]), \(e[15])]]
        """
    }
}

extension Matrix4d: CustomDebugStringConvertible {
    public var debugDescription: String {
        "Matrix4d(\(description))"
    }
}
