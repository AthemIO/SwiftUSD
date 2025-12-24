// Range3.swift - GfRange3d wrapper
// Mirrors: pxr/base/gf/range3d.h

import OpenUSDInterop

// MARK: - Range3d

/// A 3-dimensional range (or interval) with double-precision endpoints.
///
/// `Range3d` represents an axis-aligned box defined by minimum and maximum
/// points. It is commonly used for bounding boxes in 3D graphics.
/// An empty range is one where max < min.
///
/// Mirrors `pxr::GfRange3d` from the USD C++ API.
public final class Range3d: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: GfRange3dRef

    // MARK: - Initialization

    /// Creates a range with the specified min and max points.
    ///
    /// - Parameters:
    ///   - minX, minY, minZ: The minimum point coordinates.
    ///   - maxX, maxY, maxZ: The maximum point coordinates.
    /// - Throws: `GfError.creationFailed` if the range cannot be created.
    public init(
        minX: Double, minY: Double, minZ: Double,
        maxX: Double, maxY: Double, maxZ: Double
    ) throws {
        guard let ref = GfRange3d_Create(minX, minY, minZ, maxX, maxY, maxZ) else {
            throw GfError.creationFailed("Failed to create Range3d")
        }
        self.handle = ref
    }

    /// Creates a range from min and max vectors.
    ///
    /// - Parameters:
    ///   - min: The minimum point.
    ///   - max: The maximum point.
    /// - Throws: `GfError.creationFailed` if the range cannot be created.
    public init(min: Vec3d, max: Vec3d) throws {
        guard let ref = GfRange3d_Create(min.x, min.y, min.z, max.x, max.y, max.z) else {
            throw GfError.creationFailed("Failed to create Range3d")
        }
        self.handle = ref
    }

    /// Creates an empty range.
    ///
    /// An empty range has max < min, meaning it contains no points.
    ///
    /// - Throws: `GfError.creationFailed` if the range cannot be created.
    public init() throws {
        guard let ref = GfRange3d_CreateEmpty() else {
            throw GfError.creationFailed("Failed to create empty Range3d")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: GfRange3dRef) {
        self.handle = handle
    }

    deinit {
        GfRange3d_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Returns an empty range.
    public static var empty: Range3d {
        get throws {
            try Range3d()
        }
    }

    /// Returns the unit cube [0,1] in each dimension.
    public static var unitCube: Range3d {
        get throws {
            guard let ref = GfRange3d_CreateUnitCube() else {
                throw GfError.creationFailed("Failed to create unit cube")
            }
            return Range3d(handle: ref)
        }
    }

    // MARK: - Properties

    /// Gets the minimum point of the range.
    public var min: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfRange3d_GetMin(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Gets the maximum point of the range.
    public var max: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfRange3d_GetMax(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Sets the minimum point.
    public func setMin(_ x: Double, _ y: Double, _ z: Double) {
        GfRange3d_SetMin(handle, x, y, z)
    }

    /// Sets the maximum point.
    public func setMax(_ x: Double, _ y: Double, _ z: Double) {
        GfRange3d_SetMax(handle, x, y, z)
    }

    /// Gets the size (extent) of the range.
    public var size: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfRange3d_GetSize(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Gets the midpoint (center) of the range.
    public var midpoint: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfRange3d_GetMidpoint(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Returns true if the range is empty (max < min in any dimension).
    public var isEmpty: Bool {
        GfRange3d_IsEmpty(handle)
    }

    /// Sets the range to empty.
    public func setEmpty() {
        GfRange3d_SetEmpty(handle)
    }

    // MARK: - Containment Tests

    /// Returns true if the range contains the given point.
    ///
    /// - Parameters:
    ///   - x, y, z: The point coordinates.
    /// - Returns: True if the point is inside the range.
    public func contains(_ x: Double, _ y: Double, _ z: Double) -> Bool {
        GfRange3d_ContainsPoint(handle, x, y, z)
    }

    /// Returns true if the range contains the given point.
    ///
    /// - Parameter point: The point to test.
    /// - Returns: True if the point is inside the range.
    public func contains(_ point: Vec3d) -> Bool {
        GfRange3d_ContainsPoint(handle, point.x, point.y, point.z)
    }

    /// Returns true if this range entirely contains another range.
    ///
    /// - Parameter other: The other range to test.
    /// - Returns: True if the other range is entirely inside this range.
    public func contains(_ other: Range3d) -> Bool {
        GfRange3d_ContainsRange(handle, other.handle)
    }

    /// Returns true if this range is entirely outside another range.
    ///
    /// - Parameter other: The other range to test.
    /// - Returns: True if the ranges do not overlap.
    public func isOutside(_ other: Range3d) -> Bool {
        GfRange3d_IsOutside(handle, other.handle)
    }

    // MARK: - Union Operations

    /// Extends this range to include the given point.
    ///
    /// - Parameters:
    ///   - x, y, z: The point coordinates.
    public func unionWith(_ x: Double, _ y: Double, _ z: Double) {
        GfRange3d_UnionWithPoint(handle, x, y, z)
    }

    /// Extends this range to include the given point.
    ///
    /// - Parameter point: The point to include.
    public func unionWith(_ point: Vec3d) {
        GfRange3d_UnionWithPoint(handle, point.x, point.y, point.z)
    }

    /// Extends this range to include another range.
    ///
    /// - Parameter other: The other range to include.
    public func unionWith(_ other: Range3d) {
        GfRange3d_UnionWithRange(handle, other.handle)
    }

    /// Returns the union of two ranges.
    ///
    /// - Parameters:
    ///   - r1: The first range.
    ///   - r2: The second range.
    /// - Returns: The smallest range containing both.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public static func union(_ r1: Range3d, _ r2: Range3d) throws -> Range3d {
        guard let ref = GfRange3d_GetUnion(r1.handle, r2.handle) else {
            throw GfError.creationFailed("Failed to compute union")
        }
        return Range3d(handle: ref)
    }

    /// Returns the intersection of two ranges.
    ///
    /// - Parameters:
    ///   - r1: The first range.
    ///   - r2: The second range.
    /// - Returns: The intersection range.
    /// - Throws: `GfError.creationFailed` if the operation fails.
    public static func intersection(_ r1: Range3d, _ r2: Range3d) throws -> Range3d {
        guard let ref = GfRange3d_GetIntersection(r1.handle, r2.handle) else {
            throw GfError.creationFailed("Failed to compute intersection")
        }
        return Range3d(handle: ref)
    }

    // MARK: - Distance Operations

    /// Returns the squared distance from a point to this range.
    ///
    /// - Parameters:
    ///   - x, y, z: The point coordinates.
    /// - Returns: The squared distance (0 if point is inside).
    public func distanceSquared(to x: Double, _ y: Double, _ z: Double) -> Double {
        GfRange3d_GetDistanceSquared(handle, x, y, z)
    }

    /// Returns the squared distance from a point to this range.
    ///
    /// - Parameter point: The point.
    /// - Returns: The squared distance (0 if point is inside).
    public func distanceSquared(to point: Vec3d) -> Double {
        GfRange3d_GetDistanceSquared(handle, point.x, point.y, point.z)
    }

    /// Returns the ith corner of the range (0-7).
    ///
    /// Corners are ordered: LDB, RDB, LUB, RUB, LDF, RDF, LUF, RUF
    /// (L/R = left/right, D/U = down/up, B/F = back/front).
    ///
    /// - Parameter index: Corner index (0-7).
    /// - Returns: The corner coordinates.
    public func corner(_ index: Int) -> (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        GfRange3d_GetCorner(handle, index, &x, &y, &z)
        return (x, y, z)
    }

    // MARK: - Copy

    /// Creates a copy of this range.
    ///
    /// - Returns: A new range with the same bounds.
    /// - Throws: `GfError.creationFailed` if the copy fails.
    public func copy() throws -> Range3d {
        guard let ref = GfRange3d_Copy(handle) else {
            throw GfError.creationFailed("Failed to copy Range3d")
        }
        return Range3d(handle: ref)
    }
}

// MARK: - Range3d Protocol Conformances

extension Range3d: Equatable {
    public static func == (lhs: Range3d, rhs: Range3d) -> Bool {
        GfRange3d_Equal(lhs.handle, rhs.handle)
    }
}

extension Range3d: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(GfRange3d_Hash(handle))
    }
}

extension Range3d: CustomStringConvertible {
    public var description: String {
        let m = min
        let M = max
        return "[(\(m.x), \(m.y), \(m.z)) - (\(M.x), \(M.y), \(M.z))]"
    }
}

extension Range3d: CustomDebugStringConvertible {
    public var debugDescription: String {
        "Range3d(\(description))"
    }
}
