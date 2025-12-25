// UsdGeomCxx.swift - Swift extensions for UsdGeom C++ interop types
// This provides Swift convenience APIs on top of the C++ interop layer (USDCxx.UsdGeom)

import USDCxx

// MARK: - Type Aliases for C++ Geometry Types

/// Typealias for the C++ GeomXformRef type
public typealias GeomXformCxx = swiftusd.GeomXformRef

/// Typealias for the C++ GeomMeshRef type
public typealias GeomMeshCxx = swiftusd.GeomMeshRef

/// Typealias for the C++ GeomPointsRef type
public typealias GeomPointsCxx = swiftusd.GeomPointsRef

/// Typealias for the C++ GeomBasisCurvesRef type
public typealias GeomBasisCurvesCxx = swiftusd.GeomBasisCurvesRef

/// Typealias for the C++ GeomCameraRef type
public typealias GeomCameraCxx = swiftusd.GeomCameraRef

/// Typealias for the C++ GeomSphereRef type
public typealias GeomSphereCxx = swiftusd.GeomSphereRef

/// Typealias for the C++ GeomCubeRef type
public typealias GeomCubeCxx = swiftusd.GeomCubeRef

/// Typealias for the C++ GeomCylinderRef type
public typealias GeomCylinderCxx = swiftusd.GeomCylinderRef

/// Typealias for the C++ GeomConeRef type
public typealias GeomConeCxx = swiftusd.GeomConeRef

/// Typealias for the C++ GeomCapsuleRef type
public typealias GeomCapsuleCxx = swiftusd.GeomCapsuleRef

// MARK: - Enum Type Aliases

/// Typealias for the C++ GeomInterpolation type
public typealias GeomInterpolationCxx = swiftusd.GeomInterpolation

/// Typealias for the C++ SubdivisionScheme type
public typealias SubdivisionSchemeCxx = swiftusd.SubdivisionScheme

/// Typealias for the C++ GeomAxis type
public typealias GeomAxisCxx = swiftusd.GeomAxis

/// Typealias for the C++ GeomPurpose type
public typealias GeomPurposeCxx = swiftusd.GeomPurpose

/// Typealias for the C++ CurveType type
public typealias CurveTypeCxx = swiftusd.CurveType

/// Typealias for the C++ CurveBasis type
public typealias CurveBasisCxx = swiftusd.CurveBasis

/// Typealias for the C++ CurveWrap type
public typealias CurveWrapCxx = swiftusd.CurveWrap

// MARK: - Enum Convenience Extensions

extension swiftusd.GeomAxis {
    /// X axis (value = 0)
    public static var x: swiftusd.GeomAxis { swiftusd.GeomAxis(swiftusd.GeomAxis.Value(rawValue: 0)) }
    /// Y axis (value = 1)
    public static var y: swiftusd.GeomAxis { swiftusd.GeomAxis(swiftusd.GeomAxis.Value(rawValue: 1)) }
    /// Z axis (value = 2)
    public static var z: swiftusd.GeomAxis { swiftusd.GeomAxis(swiftusd.GeomAxis.Value(rawValue: 2)) }
}

extension swiftusd.GeomInterpolation {
    /// Constant interpolation (value = 0)
    public static var constant: swiftusd.GeomInterpolation { swiftusd.GeomInterpolation(swiftusd.GeomInterpolation.Value(rawValue: 0)) }
    /// Uniform interpolation (value = 1)
    public static var uniform: swiftusd.GeomInterpolation { swiftusd.GeomInterpolation(swiftusd.GeomInterpolation.Value(rawValue: 1)) }
    /// Varying interpolation (value = 2)
    public static var varying: swiftusd.GeomInterpolation { swiftusd.GeomInterpolation(swiftusd.GeomInterpolation.Value(rawValue: 2)) }
    /// Vertex interpolation (value = 3)
    public static var vertex: swiftusd.GeomInterpolation { swiftusd.GeomInterpolation(swiftusd.GeomInterpolation.Value(rawValue: 3)) }
    /// Face-varying interpolation (value = 4)
    public static var faceVarying: swiftusd.GeomInterpolation { swiftusd.GeomInterpolation(swiftusd.GeomInterpolation.Value(rawValue: 4)) }
}

extension swiftusd.SubdivisionScheme {
    /// No subdivision (value = 0)
    public static var none: swiftusd.SubdivisionScheme { swiftusd.SubdivisionScheme(swiftusd.SubdivisionScheme.Value(rawValue: 0)) }
    /// Catmull-Clark subdivision (value = 1)
    public static var catmullClark: swiftusd.SubdivisionScheme { swiftusd.SubdivisionScheme(swiftusd.SubdivisionScheme.Value(rawValue: 1)) }
    /// Loop subdivision (value = 2)
    public static var loop: swiftusd.SubdivisionScheme { swiftusd.SubdivisionScheme(swiftusd.SubdivisionScheme.Value(rawValue: 2)) }
    /// Bilinear subdivision (value = 3)
    public static var bilinear: swiftusd.SubdivisionScheme { swiftusd.SubdivisionScheme(swiftusd.SubdivisionScheme.Value(rawValue: 3)) }
}

extension swiftusd.GeomPurpose {
    /// Default purpose (value = 0)
    public static var defaultPurpose: swiftusd.GeomPurpose { swiftusd.GeomPurpose(swiftusd.GeomPurpose.Value(rawValue: 0)) }
    /// Render purpose (value = 1)
    public static var render: swiftusd.GeomPurpose { swiftusd.GeomPurpose(swiftusd.GeomPurpose.Value(rawValue: 1)) }
    /// Proxy purpose (value = 2)
    public static var proxy: swiftusd.GeomPurpose { swiftusd.GeomPurpose(swiftusd.GeomPurpose.Value(rawValue: 2)) }
    /// Guide purpose (value = 3)
    public static var guide: swiftusd.GeomPurpose { swiftusd.GeomPurpose(swiftusd.GeomPurpose.Value(rawValue: 3)) }
}

extension swiftusd.CurveType {
    /// Linear curves (value = 0)
    public static var linear: swiftusd.CurveType { swiftusd.CurveType(swiftusd.CurveType.Value(rawValue: 0)) }
    /// Cubic curves (value = 1)
    public static var cubic: swiftusd.CurveType { swiftusd.CurveType(swiftusd.CurveType.Value(rawValue: 1)) }
}

extension swiftusd.CurveBasis {
    /// Bezier basis (value = 0)
    public static var bezier: swiftusd.CurveBasis { swiftusd.CurveBasis(swiftusd.CurveBasis.Value(rawValue: 0)) }
    /// B-spline basis (value = 1)
    public static var bspline: swiftusd.CurveBasis { swiftusd.CurveBasis(swiftusd.CurveBasis.Value(rawValue: 1)) }
    /// Catmull-Rom basis (value = 2)
    public static var catmullRom: swiftusd.CurveBasis { swiftusd.CurveBasis(swiftusd.CurveBasis.Value(rawValue: 2)) }
}

extension swiftusd.CurveWrap {
    /// Non-periodic wrap (value = 0)
    public static var nonperiodic: swiftusd.CurveWrap { swiftusd.CurveWrap(swiftusd.CurveWrap.Value(rawValue: 0)) }
    /// Periodic wrap (value = 1)
    public static var periodic: swiftusd.CurveWrap { swiftusd.CurveWrap(swiftusd.CurveWrap.Value(rawValue: 1)) }
    /// Pinned wrap (value = 2)
    public static var pinned: swiftusd.CurveWrap { swiftusd.CurveWrap(swiftusd.CurveWrap.Value(rawValue: 2)) }
}

// MARK: - Factory Method Extensions

extension swiftusd.GeomXformRef {
    /// Defines a new Xform prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomXformRef? {
        swiftusd.GeomXformRef.Define(stage, path)
    }
}

extension swiftusd.GeomMeshRef {
    /// Defines a new Mesh prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomMeshRef? {
        swiftusd.GeomMeshRef.Define(stage, path)
    }
}

extension swiftusd.GeomPointsRef {
    /// Defines a new Points prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomPointsRef? {
        swiftusd.GeomPointsRef.Define(stage, path)
    }
}

extension swiftusd.GeomBasisCurvesRef {
    /// Defines a new BasisCurves prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomBasisCurvesRef? {
        swiftusd.GeomBasisCurvesRef.Define(stage, path)
    }
}

extension swiftusd.GeomCameraRef {
    /// Defines a new Camera prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomCameraRef? {
        swiftusd.GeomCameraRef.Define(stage, path)
    }
}

extension swiftusd.GeomSphereRef {
    /// Defines a new Sphere prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomSphereRef? {
        swiftusd.GeomSphereRef.Define(stage, path)
    }
}

extension swiftusd.GeomCubeRef {
    /// Defines a new Cube prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomCubeRef? {
        swiftusd.GeomCubeRef.Define(stage, path)
    }
}

extension swiftusd.GeomCylinderRef {
    /// Defines a new Cylinder prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomCylinderRef? {
        swiftusd.GeomCylinderRef.Define(stage, path)
    }
}

extension swiftusd.GeomConeRef {
    /// Defines a new Cone prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomConeRef? {
        swiftusd.GeomConeRef.Define(stage, path)
    }
}

extension swiftusd.GeomCapsuleRef {
    /// Defines a new Capsule prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.GeomCapsuleRef? {
        swiftusd.GeomCapsuleRef.Define(stage, path)
    }
}

// MARK: - GeomMeshRef Extensions

extension swiftusd.GeomMeshRef {
    /// Gets the points as an array of Vec3f.
    public func getPointsAsVec3f(at time: swiftusd.TimeCode = .Default()) -> [swiftusd.Vec3f] {
        let count = Int(GetPointCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetPoints(time, ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the points from an array of Vec3f.
    @discardableResult
    public func setPoints(_ points: [swiftusd.Vec3f], at time: swiftusd.TimeCode = .Default()) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(points.count * 3)
        for p in points {
            buffer.append(p.x)
            buffer.append(p.y)
            buffer.append(p.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetPoints(time, ptr.baseAddress, points.count)
        }
    }

    /// Gets the face vertex counts as an array.
    public func getFaceVertexCounts(at time: swiftusd.TimeCode = .Default()) -> [Int32] {
        let count = Int(GetFaceVertexCountsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Int32](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetFaceVertexCounts(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the face vertex counts.
    @discardableResult
    public func setFaceVertexCounts(_ counts: [Int32], at time: swiftusd.TimeCode = .Default()) -> Bool {
        counts.withUnsafeBufferPointer { ptr in
            SetFaceVertexCounts(time, ptr.baseAddress, counts.count)
        }
    }

    /// Gets the face vertex indices as an array.
    public func getFaceVertexIndices(at time: swiftusd.TimeCode = .Default()) -> [Int32] {
        let count = Int(GetFaceVertexIndicesCount(time))
        guard count > 0 else { return [] }

        var buffer = [Int32](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetFaceVertexIndices(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the face vertex indices.
    @discardableResult
    public func setFaceVertexIndices(_ indices: [Int32], at time: swiftusd.TimeCode = .Default()) -> Bool {
        indices.withUnsafeBufferPointer { ptr in
            SetFaceVertexIndices(time, ptr.baseAddress, indices.count)
        }
    }
}

// MARK: - GeomPointsRef Extensions

extension swiftusd.GeomPointsRef {
    /// Gets the points as an array of Vec3f.
    public func getPointsAsVec3f(at time: swiftusd.TimeCode = .Default()) -> [swiftusd.Vec3f] {
        let count = Int(GetPointCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetPoints(time, ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the points from an array of Vec3f.
    @discardableResult
    public func setPoints(_ points: [swiftusd.Vec3f], at time: swiftusd.TimeCode = .Default()) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(points.count * 3)
        for p in points {
            buffer.append(p.x)
            buffer.append(p.y)
            buffer.append(p.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetPoints(time, ptr.baseAddress, points.count)
        }
    }

    /// Gets the widths as an array.
    public func getWidths(at time: swiftusd.TimeCode = .Default()) -> [Float] {
        let count = Int(GetWidthsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetWidths(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the widths.
    @discardableResult
    public func setWidths(_ widths: [Float], at time: swiftusd.TimeCode = .Default()) -> Bool {
        widths.withUnsafeBufferPointer { ptr in
            SetWidths(time, ptr.baseAddress, widths.count)
        }
    }

    /// Gets the IDs as an array.
    public func getIds(at time: swiftusd.TimeCode = .Default()) -> [Int64] {
        let count = Int(GetIdsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Int64](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetIds(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the IDs.
    @discardableResult
    public func setIds(_ ids: [Int64], at time: swiftusd.TimeCode = .Default()) -> Bool {
        ids.withUnsafeBufferPointer { ptr in
            SetIds(time, ptr.baseAddress, ids.count)
        }
    }
}

// MARK: - GeomBasisCurvesRef Extensions

extension swiftusd.GeomBasisCurvesRef {
    /// Gets the curve vertex counts as an array.
    public func getCurveVertexCounts(at time: swiftusd.TimeCode = .Default()) -> [Int32] {
        let count = Int(GetCurveVertexCountsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Int32](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetCurveVertexCounts(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the curve vertex counts.
    @discardableResult
    public func setCurveVertexCounts(_ counts: [Int32], at time: swiftusd.TimeCode = .Default()) -> Bool {
        counts.withUnsafeBufferPointer { ptr in
            SetCurveVertexCounts(time, ptr.baseAddress, counts.count)
        }
    }

    /// Gets the points as an array of Vec3f.
    public func getPointsAsVec3f(at time: swiftusd.TimeCode = .Default()) -> [swiftusd.Vec3f] {
        let count = Int(GetPointCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetPoints(time, ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the points from an array of Vec3f.
    @discardableResult
    public func setPoints(_ points: [swiftusd.Vec3f], at time: swiftusd.TimeCode = .Default()) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(points.count * 3)
        for p in points {
            buffer.append(p.x)
            buffer.append(p.y)
            buffer.append(p.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetPoints(time, ptr.baseAddress, points.count)
        }
    }

    /// Gets the widths as an array.
    public func getWidths(at time: swiftusd.TimeCode = .Default()) -> [Float] {
        let count = Int(GetWidthsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetWidths(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the widths.
    @discardableResult
    public func setWidths(_ widths: [Float], at time: swiftusd.TimeCode = .Default()) -> Bool {
        widths.withUnsafeBufferPointer { ptr in
            SetWidths(time, ptr.baseAddress, widths.count)
        }
    }
}

// MARK: - GeomCameraRef Extensions

extension swiftusd.GeomCameraRef {
    /// Gets the clipping range as a tuple.
    public func getClippingRange(at time: swiftusd.TimeCode = .Default()) -> (near: Float, far: Float)? {
        var nearClip: Float = 0
        var farClip: Float = 0
        if GetClippingRange(time, &nearClip, &farClip) {
            return (nearClip, farClip)
        }
        return nil
    }
}

// MARK: - GeomXformRef Extensions

extension swiftusd.GeomXformRef {
    /// Computes the local-to-world transform as a Matrix4d.
    public func localToWorldTransform(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Matrix4d {
        var matrix = [Double](repeating: 0, count: 16)
        _ = matrix.withUnsafeMutableBufferPointer { buffer in
            ComputeLocalToWorldTransform(time, buffer.baseAddress)
        }
        return swiftusd.Matrix4d(
            matrix[0],  matrix[1],  matrix[2],  matrix[3],
            matrix[4],  matrix[5],  matrix[6],  matrix[7],
            matrix[8],  matrix[9],  matrix[10], matrix[11],
            matrix[12], matrix[13], matrix[14], matrix[15]
        )
    }
}
