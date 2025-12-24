// GeomUtil.swift - Mesh Generation Utilities wrapper
// Mirrors: pxr/imaging/geomUtil/*.h (Mesh generation for primitives)

import OpenUSDInterop

// MARK: - GeomUtil Namespace

/// Mesh generation utilities for procedural primitive geometry.
/// Mirrors pxr::GeomUtil from the USD imaging library.
///
/// GeomUtil provides static mesh generators for common primitive shapes:
/// - Sphere, Cuboid, Cylinder, Cone, Capsule, Plane, Disk
///
/// All generators produce mesh data including points, normals, and topology
/// (face vertex counts and indices).
///
/// ## Usage
/// ```swift
/// // Generate a sphere mesh
/// let sphere = try GeomUtil.generateSphere(
///     numRadial: 32, numAxial: 16,
///     radius: 1.0
/// )
/// print("Points: \(sphere.points.count)")
/// print("Faces: \(sphere.faceVertexCounts.count)")
///
/// // Generate a cuboid with transformation
/// var transform = [Float](repeating: 0, count: 16)
/// // ... set up transform matrix ...
/// let box = try GeomUtil.generateCuboid(
///     xLength: 2.0, yLength: 1.0, zLength: 0.5,
///     transform: transform
/// )
/// ```
public enum GeomUtil {

    // MARK: - Normal Interpolation

    /// Normal interpolation mode for mesh data.
    /// Mirrors pxr::GeomUtilMeshGeneratorBase normal interpolation tokens.
    public enum NormalsInterpolation: Int, Sendable, CustomStringConvertible {
        /// Single normal for entire geometry (e.g., plane, disk).
        case constant = 0
        /// One normal per face (e.g., cuboid).
        case uniform = 1
        /// One normal per vertex (e.g., sphere, cylinder, cone, capsule).
        case vertex = 2

        public var description: String {
            switch self {
            case .constant: return "constant"
            case .uniform: return "uniform"
            case .vertex: return "vertex"
            }
        }

        internal init(_ cValue: GeomUtilNormalsInterpolation) {
            switch cValue {
            case GEOMUTIL_NORMALS_CONSTANT:
                self = .constant
            case GEOMUTIL_NORMALS_UNIFORM:
                self = .uniform
            case GEOMUTIL_NORMALS_VERTEX:
                self = .vertex
            default:
                self = .vertex
            }
        }
    }
}

// MARK: - GeomUtil Error

/// Errors that can occur in GeomUtil operations.
public enum GeomUtilError: Error, CustomStringConvertible {
    case generationFailed(String)
    case invalidArgument(String)

    public var description: String {
        switch self {
        case .generationFailed(let message):
            return "GeomUtil generation failed: \(message)"
        case .invalidArgument(let message):
            return "GeomUtil invalid argument: \(message)"
        }
    }
}

// MARK: - MeshData

extension GeomUtil {

    /// Generated mesh data containing points, normals, and topology.
    ///
    /// MeshData holds all the information needed to render or process
    /// a generated primitive mesh:
    /// - `points`: Vertex positions as [x, y, z] triplets
    /// - `normals`: Normal vectors as [x, y, z] triplets
    /// - `normalsInterpolation`: How normals map to the mesh
    /// - `faceVertexCounts`: Number of vertices per face
    /// - `faceVertexIndices`: Vertex indices for each face
    public final class MeshData: @unchecked Sendable {
        internal let handle: GeomUtilMeshDataRef

        internal init(handle: GeomUtilMeshDataRef) {
            self.handle = handle
        }

        deinit {
            GeomUtilMeshData_Release(handle)
        }

        // MARK: Points

        /// The number of points (vertices) in the mesh.
        public var pointCount: Int {
            Int(GeomUtilMeshData_GetPointCount(handle))
        }

        /// The points (vertices) as an array of SIMD3<Float>.
        public var points: [SIMD3<Float>] {
            let count = pointCount
            guard count > 0 else { return [] }

            var buffer = [Float](repeating: 0, count: count * 3)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                GeomUtilMeshData_GetPoints(handle, ptr.baseAddress!, count)
            }

            var result = [SIMD3<Float>]()
            result.reserveCapacity(Int(copied))
            for i in 0..<Int(copied) {
                result.append(SIMD3<Float>(
                    buffer[i * 3 + 0],
                    buffer[i * 3 + 1],
                    buffer[i * 3 + 2]
                ))
            }
            return result
        }

        /// The points as a flat array of floats [x0, y0, z0, x1, y1, z1, ...].
        public var pointsFlat: [Float] {
            let count = pointCount
            guard count > 0 else { return [] }

            var buffer = [Float](repeating: 0, count: count * 3)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                GeomUtilMeshData_GetPoints(handle, ptr.baseAddress!, count)
            }

            if copied < count {
                return Array(buffer.prefix(Int(copied) * 3))
            }
            return buffer
        }

        // MARK: Normals

        /// The number of normals in the mesh.
        public var normalCount: Int {
            Int(GeomUtilMeshData_GetNormalCount(handle))
        }

        /// The normals as an array of SIMD3<Float>.
        public var normals: [SIMD3<Float>] {
            let count = normalCount
            guard count > 0 else { return [] }

            var buffer = [Float](repeating: 0, count: count * 3)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                GeomUtilMeshData_GetNormals(handle, ptr.baseAddress!, count)
            }

            var result = [SIMD3<Float>]()
            result.reserveCapacity(Int(copied))
            for i in 0..<Int(copied) {
                result.append(SIMD3<Float>(
                    buffer[i * 3 + 0],
                    buffer[i * 3 + 1],
                    buffer[i * 3 + 2]
                ))
            }
            return result
        }

        /// The normals as a flat array of floats [nx0, ny0, nz0, nx1, ny1, nz1, ...].
        public var normalsFlat: [Float] {
            let count = normalCount
            guard count > 0 else { return [] }

            var buffer = [Float](repeating: 0, count: count * 3)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                GeomUtilMeshData_GetNormals(handle, ptr.baseAddress!, count)
            }

            if copied < count {
                return Array(buffer.prefix(Int(copied) * 3))
            }
            return buffer
        }

        /// The normal interpolation mode.
        public var normalsInterpolation: NormalsInterpolation {
            NormalsInterpolation(GeomUtilMeshData_GetNormalsInterpolation(handle))
        }

        // MARK: Topology

        /// The number of faces in the mesh.
        public var faceCount: Int {
            Int(GeomUtilMeshData_GetFaceCount(handle))
        }

        /// The face vertex counts (number of vertices per face).
        public var faceVertexCounts: [Int32] {
            let count = faceCount
            guard count > 0 else { return [] }

            var buffer = [Int32](repeating: 0, count: count)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                GeomUtilMeshData_GetFaceVertexCounts(handle, ptr.baseAddress!, count)
            }

            if copied < count {
                return Array(buffer.prefix(Int(copied)))
            }
            return buffer
        }

        /// The total number of face vertex indices.
        public var faceVertexIndexCount: Int {
            Int(GeomUtilMeshData_GetFaceVertexIndexCount(handle))
        }

        /// The face vertex indices.
        public var faceVertexIndices: [Int32] {
            let count = faceVertexIndexCount
            guard count > 0 else { return [] }

            var buffer = [Int32](repeating: 0, count: count)
            let copied = buffer.withUnsafeMutableBufferPointer { ptr in
                GeomUtilMeshData_GetFaceVertexIndices(handle, ptr.baseAddress!, count)
            }

            if copied < count {
                return Array(buffer.prefix(Int(copied)))
            }
            return buffer
        }
    }
}

// MARK: - MeshData CustomStringConvertible

extension GeomUtil.MeshData: CustomStringConvertible {
    public var description: String {
        "MeshData(points: \(pointCount), normals: \(normalCount) [\(normalsInterpolation)], faces: \(faceCount))"
    }
}

// MARK: - Sphere Generation

extension GeomUtil {

    /// Computes the number of points for a sphere mesh.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - numAxial: Number of axial segments (minimum 2).
    ///   - closedSweep: Whether the sweep is closed (360 degrees).
    /// - Returns: The number of points that will be generated.
    public static func computeSpherePointCount(
        numRadial: Int,
        numAxial: Int,
        closedSweep: Bool = true
    ) -> Int {
        Int(GeomUtil_ComputeSpherePointCount(numRadial, numAxial, closedSweep))
    }

    /// Generates a sphere mesh.
    ///
    /// Creates a sphere centered at the origin with circular cross-sections
    /// in the XY plane. The sphere extends from Z = -radius to Z = +radius.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - numAxial: Number of axial segments (minimum 2).
    ///   - radius: The radius of the sphere.
    ///   - sweepDegrees: Sweep angle in degrees (360 for full sphere).
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generateSphere(
        numRadial: Int,
        numAxial: Int,
        radius: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GenerateSphereMesh(
            numRadial, numAxial, radius, sweepDegrees, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate sphere mesh")
        }

        return MeshData(handle: handle)
    }
}

// MARK: - Cuboid Generation

extension GeomUtil {

    /// Computes the number of points for a cuboid mesh.
    /// - Returns: The number of points (always 8 for a cuboid).
    public static func computeCuboidPointCount() -> Int {
        Int(GeomUtil_ComputeCuboidPointCount())
    }

    /// Generates a cuboid (rectangular box) mesh.
    ///
    /// Creates a box centered at the origin with the specified dimensions
    /// along each axis.
    ///
    /// - Parameters:
    ///   - xLength: Length along the X axis.
    ///   - yLength: Length along the Y axis.
    ///   - zLength: Length along the Z axis.
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generateCuboid(
        xLength: Float,
        yLength: Float,
        zLength: Float,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GenerateCuboidMesh(
            xLength, yLength, zLength, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate cuboid mesh")
        }

        return MeshData(handle: handle)
    }

    /// Convenience method to generate a cube (equal dimensions on all axes).
    ///
    /// - Parameters:
    ///   - size: The length of each side.
    ///   - transform: Optional 4x4 transformation matrix.
    /// - Returns: The generated mesh data.
    public static func generateCube(
        size: Float,
        transform: [Float]? = nil
    ) throws -> MeshData {
        try generateCuboid(xLength: size, yLength: size, zLength: size, transform: transform)
    }
}

// MARK: - Cylinder Generation

extension GeomUtil {

    /// Computes the number of points for a cylinder mesh.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - closedSweep: Whether the sweep is closed (360 degrees).
    /// - Returns: The number of points that will be generated.
    public static func computeCylinderPointCount(
        numRadial: Int,
        closedSweep: Bool = true
    ) -> Int {
        Int(GeomUtil_ComputeCylinderPointCount(numRadial, closedSweep))
    }

    /// Generates a cylinder mesh.
    ///
    /// Creates a cylinder with circular cross-sections in the XY plane,
    /// centered at the origin. The cylinder extends from Z = -height/2
    /// to Z = +height/2.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - bottomRadius: Radius at the bottom (Z = -height/2).
    ///   - topRadius: Radius at the top (Z = +height/2).
    ///   - height: Height of the cylinder.
    ///   - sweepDegrees: Sweep angle in degrees (360 for full cylinder).
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generateCylinder(
        numRadial: Int,
        bottomRadius: Float,
        topRadius: Float,
        height: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GenerateCylinderMesh(
            numRadial, bottomRadius, topRadius, height, sweepDegrees, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate cylinder mesh")
        }

        return MeshData(handle: handle)
    }

    /// Convenience method to generate a cylinder with uniform radius.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments.
    ///   - radius: Radius of the cylinder.
    ///   - height: Height of the cylinder.
    ///   - sweepDegrees: Sweep angle in degrees.
    ///   - transform: Optional transformation matrix.
    /// - Returns: The generated mesh data.
    public static func generateCylinder(
        numRadial: Int,
        radius: Float,
        height: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        try generateCylinder(
            numRadial: numRadial,
            bottomRadius: radius,
            topRadius: radius,
            height: height,
            sweepDegrees: sweepDegrees,
            transform: transform
        )
    }
}

// MARK: - Cone Generation

extension GeomUtil {

    /// Computes the number of points for a cone mesh.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - closedSweep: Whether the sweep is closed (360 degrees).
    /// - Returns: The number of points that will be generated.
    public static func computeConePointCount(
        numRadial: Int,
        closedSweep: Bool = true
    ) -> Int {
        Int(GeomUtil_ComputeConePointCount(numRadial, closedSweep))
    }

    /// Generates a cone mesh.
    ///
    /// Creates a cone with a circular base in the XY plane, centered at
    /// the origin. The base is at Z = -height/2 and the apex at Z = +height/2.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - radius: Base radius.
    ///   - height: Height of the cone.
    ///   - sweepDegrees: Sweep angle in degrees (360 for full cone).
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generateCone(
        numRadial: Int,
        radius: Float,
        height: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GenerateConeMesh(
            numRadial, radius, height, sweepDegrees, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate cone mesh")
        }

        return MeshData(handle: handle)
    }
}

// MARK: - Capsule Generation

extension GeomUtil {

    /// Computes the number of points for a capsule mesh.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - numCapAxial: Number of axial segments per hemisphere cap (minimum 1).
    ///   - closedSweep: Whether the sweep is closed (360 degrees).
    /// - Returns: The number of points that will be generated.
    public static func computeCapsulePointCount(
        numRadial: Int,
        numCapAxial: Int,
        closedSweep: Bool = true
    ) -> Int {
        Int(GeomUtil_ComputeCapsulePointCount(numRadial, numCapAxial, closedSweep))
    }

    /// Generates a capsule mesh (cylinder with hemispherical caps).
    ///
    /// Creates a capsule centered at the origin. The cylindrical portion
    /// extends from Z = -height/2 to Z = +height/2, with hemispheres on
    /// each end extending the total height by bottomRadius and topRadius.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - numCapAxial: Number of axial segments per hemisphere cap (minimum 1).
    ///   - bottomRadius: Radius of the bottom hemisphere.
    ///   - topRadius: Radius of the top hemisphere.
    ///   - height: Height of the cylindrical portion.
    ///   - sweepDegrees: Sweep angle in degrees (360 for full capsule).
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generateCapsule(
        numRadial: Int,
        numCapAxial: Int,
        bottomRadius: Float,
        topRadius: Float,
        height: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GenerateCapsuleMesh(
            numRadial, numCapAxial, bottomRadius, topRadius, height, sweepDegrees, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate capsule mesh")
        }

        return MeshData(handle: handle)
    }

    /// Convenience method to generate a capsule with uniform radius.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments.
    ///   - numCapAxial: Number of axial segments per cap.
    ///   - radius: Radius of the capsule.
    ///   - height: Height of the cylindrical portion.
    ///   - sweepDegrees: Sweep angle in degrees.
    ///   - transform: Optional transformation matrix.
    /// - Returns: The generated mesh data.
    public static func generateCapsule(
        numRadial: Int,
        numCapAxial: Int,
        radius: Float,
        height: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        try generateCapsule(
            numRadial: numRadial,
            numCapAxial: numCapAxial,
            bottomRadius: radius,
            topRadius: radius,
            height: height,
            sweepDegrees: sweepDegrees,
            transform: transform
        )
    }
}

// MARK: - Plane Generation

extension GeomUtil {

    /// Computes the number of points for a plane mesh.
    /// - Returns: The number of points (always 4 for a plane).
    public static func computePlanePointCount() -> Int {
        Int(GeomUtil_ComputePlanePointCount())
    }

    /// Generates a plane mesh.
    ///
    /// Creates a rectangular plane in the XY plane, centered at the origin.
    /// The normal points in the +Z direction.
    ///
    /// - Parameters:
    ///   - xLength: Length along the X axis.
    ///   - yLength: Length along the Y axis.
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generatePlane(
        xLength: Float,
        yLength: Float,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GeneratePlaneMesh(
            xLength, yLength, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate plane mesh")
        }

        return MeshData(handle: handle)
    }

    /// Convenience method to generate a square plane.
    ///
    /// - Parameters:
    ///   - size: The length of each side.
    ///   - transform: Optional transformation matrix.
    /// - Returns: The generated mesh data.
    public static func generateSquare(
        size: Float,
        transform: [Float]? = nil
    ) throws -> MeshData {
        try generatePlane(xLength: size, yLength: size, transform: transform)
    }
}

// MARK: - Disk Generation

extension GeomUtil {

    /// Computes the number of points for a disk mesh.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - closedSweep: Whether the sweep is closed (360 degrees).
    /// - Returns: The number of points that will be generated.
    public static func computeDiskPointCount(
        numRadial: Int,
        closedSweep: Bool = true
    ) -> Int {
        Int(GeomUtil_ComputeDiskPointCount(numRadial, closedSweep))
    }

    /// Generates a disk mesh.
    ///
    /// Creates a circular disk in the XY plane, centered at the origin.
    /// The normal points in the +Z direction.
    ///
    /// - Parameters:
    ///   - numRadial: Number of radial segments (minimum 3).
    ///   - radius: Radius of the disk.
    ///   - sweepDegrees: Sweep angle in degrees (360 for full disk).
    ///   - transform: Optional 4x4 transformation matrix (16 floats, row-major).
    /// - Returns: The generated mesh data.
    /// - Throws: `GeomUtilError.generationFailed` if generation fails.
    public static func generateDisk(
        numRadial: Int,
        radius: Float,
        sweepDegrees: Float = 360.0,
        transform: [Float]? = nil
    ) throws -> MeshData {
        let transformPtr: UnsafePointer<Float>?
        if let transform = transform {
            guard transform.count == 16 else {
                throw GeomUtilError.invalidArgument("Transform must have 16 elements")
            }
            transformPtr = transform.withUnsafeBufferPointer { $0.baseAddress }
        } else {
            transformPtr = nil
        }

        guard let handle = GeomUtil_GenerateDiskMesh(
            numRadial, radius, sweepDegrees, transformPtr
        ) else {
            throw GeomUtilError.generationFailed("Failed to generate disk mesh")
        }

        return MeshData(handle: handle)
    }
}
