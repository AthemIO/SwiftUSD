// BBox3.swift - GfBBox3d wrapper
// Mirrors: pxr/base/gf/bbox3d.h

import OpenUSDInterop

// MARK: - BBox3d

/// A 3D bounding box with an associated transformation matrix.
///
/// `BBox3d` represents an axis-aligned box combined with a transformation
/// matrix. This is more useful than using just `Range3d` because:
/// - When a bounding box is transformed multiple times, storing the
///   transformation separately results in a tighter fit.
/// - When combining bounding boxes, there's a better opportunity to
///   choose an optimal coordinate space.
///
/// Mirrors `pxr::GfBBox3d` from the USD C++ API.
public final class BBox3d: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: GfBBox3dRef

    // MARK: - Initialization

    /// Creates an empty bounding box with identity transformation.
    ///
    /// - Throws: `GfError.creationFailed` if the bbox cannot be created.
    public init() throws {
        guard let ref = GfBBox3d_Create() else {
            throw GfError.creationFailed("Failed to create BBox3d")
        }
        self.handle = ref
    }

    /// Creates a bounding box from a range with identity transformation.
    ///
    /// - Parameter range: The axis-aligned box.
    /// - Throws: `GfError.creationFailed` if the bbox cannot be created.
    public init(range: Range3d) throws {
        guard let ref = GfBBox3d_CreateFromRange(range.handle) else {
            throw GfError.creationFailed("Failed to create BBox3d from range")
        }
        self.handle = ref
    }

    /// Creates a bounding box from a range and transformation matrix.
    ///
    /// - Parameters:
    ///   - range: The axis-aligned box.
    ///   - matrix: The transformation matrix.
    /// - Throws: `GfError.creationFailed` if the bbox cannot be created.
    public init(range: Range3d, matrix: Matrix4d) throws {
        guard let ref = GfBBox3d_CreateFromRangeAndMatrix(range.handle, matrix.handle) else {
            throw GfError.creationFailed("Failed to create BBox3d from range and matrix")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: GfBBox3dRef) {
        self.handle = handle
    }

    deinit {
        GfBBox3d_Release(handle)
    }

    // MARK: - Properties

    /// Gets the axis-aligned range (untransformed box).
    ///
    /// - Returns: The range, or nil if retrieval fails.
    public var range: Range3d? {
        guard let ref = GfBBox3d_GetRange(handle) else { return nil }
        return Range3d(handle: ref)
    }

    /// Gets the transformation matrix.
    ///
    /// - Returns: The matrix, or nil if retrieval fails.
    public var matrix: Matrix4d? {
        guard let ref = GfBBox3d_GetMatrix(handle) else { return nil }
        return Matrix4d(handle: ref)
    }

    /// Gets the inverse of the transformation matrix.
    ///
    /// - Returns: The inverse matrix, or nil if retrieval fails.
    public var inverseMatrix: Matrix4d? {
        guard let ref = GfBBox3d_GetInverseMatrix(handle) else { return nil }
        return Matrix4d(handle: ref)
    }

    /// Sets the range.
    ///
    /// - Parameter range: The new range.
    public func setRange(_ range: Range3d) {
        GfBBox3d_SetRange(handle, range.handle)
    }

    /// Sets the transformation matrix.
    ///
    /// - Parameter matrix: The new matrix.
    public func setMatrix(_ matrix: Matrix4d) {
        GfBBox3d_SetMatrix(handle, matrix.handle)
    }

    /// Gets the zero-area primitives flag.
    ///
    /// When true, the bounding box contains primitives with zero area
    /// (like points or lines) that may need special handling during
    /// intersection tests.
    public var hasZeroAreaPrimitives: Bool {
        get { GfBBox3d_HasZeroAreaPrimitives(handle) }
        set { GfBBox3d_SetHasZeroAreaPrimitives(handle, newValue) }
    }

    // MARK: - Operations

    /// Returns the volume of the bounding box.
    ///
    /// Returns 0 for an empty box.
    public var volume: Double {
        GfBBox3d_GetVolume(handle)
    }

    /// Transforms this bounding box by a matrix.
    ///
    /// The matrix is assumed to be a global transformation, so it is
    /// post-multiplied with the box's existing matrix.
    ///
    /// - Parameter matrix: The transformation matrix.
    public func transform(by matrix: Matrix4d) {
        GfBBox3d_Transform(handle, matrix.handle)
    }

    /// Computes the axis-aligned range by applying the transformation.
    ///
    /// This transforms all corners of the box and returns the axis-aligned
    /// bounding box of the result.
    ///
    /// - Returns: The aligned range, or nil if computation fails.
    public func computeAlignedRange() -> Range3d? {
        guard let ref = GfBBox3d_ComputeAlignedRange(handle) else { return nil }
        return Range3d(handle: ref)
    }

    /// Computes the centroid of the bounding box.
    ///
    /// The centroid is the transformed center of the range.
    ///
    /// - Returns: The centroid coordinates.
    public var centroid: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfBBox3d_ComputeCentroid(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Combines two bounding boxes.
    ///
    /// The result uses the coordinate space that produces the smaller
    /// combined box.
    ///
    /// - Parameters:
    ///   - b1: The first bounding box.
    ///   - b2: The second bounding box.
    /// - Returns: The combined bounding box.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public static func combine(_ b1: BBox3d, _ b2: BBox3d) throws -> BBox3d {
        guard let ref = GfBBox3d_Combine(b1.handle, b2.handle) else {
            throw GfError.creationFailed("Failed to combine bounding boxes")
        }
        return BBox3d(handle: ref)
    }

    /// Creates a copy of this bounding box.
    ///
    /// - Returns: A new bounding box with the same state.
    /// - Throws: `GfError.creationFailed` if the copy fails.
    public func copy() throws -> BBox3d {
        guard let ref = GfBBox3d_Copy(handle) else {
            throw GfError.creationFailed("Failed to copy BBox3d")
        }
        return BBox3d(handle: ref)
    }
}

// MARK: - BBox3d Protocol Conformances

extension BBox3d: Equatable {
    public static func == (lhs: BBox3d, rhs: BBox3d) -> Bool {
        GfBBox3d_Equal(lhs.handle, rhs.handle)
    }
}

extension BBox3d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(GfBBox3d_Hash(handle))
    }
}

extension BBox3d: CustomStringConvertible {
    public var description: String {
        let c = centroid
        return "BBox3d(centroid: (\(c.x), \(c.y), \(c.z)), hasZeroArea: \(hasZeroAreaPrimitives))"
    }
}

extension BBox3d: CustomDebugStringConvertible {
    public var debugDescription: String {
        description
    }
}
