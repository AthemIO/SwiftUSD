// UsdSkelCxx.swift - Swift extensions for UsdSkel C++ interop types
// This provides Swift convenience APIs on top of the C++ interop layer (USDCxx.UsdSkel)

import USDCxx

// MARK: - Type Aliases for C++ Skeleton Types

/// Typealias for the C++ SkelRootRef type
public typealias SkelRootCxx = swiftusd.SkelRootRef

/// Typealias for the C++ SkelSkeletonRef type
public typealias SkelSkeletonCxx = swiftusd.SkelSkeletonRef

/// Typealias for the C++ SkelAnimationRef type
public typealias SkelAnimationCxx = swiftusd.SkelAnimationRef

/// Typealias for the C++ SkelBlendShapeRef type
public typealias SkelBlendShapeCxx = swiftusd.SkelBlendShapeRef

/// Typealias for the C++ SkelBindingAPIRef type
public typealias SkelBindingAPICxx = swiftusd.SkelBindingAPIRef

// MARK: - Enum Type Aliases

/// Typealias for the C++ SkelSkinningMethod type
public typealias SkelSkinningMethodCxx = swiftusd.SkelSkinningMethod

// MARK: - Enum Convenience Extensions

extension swiftusd.SkelSkinningMethod {
    /// Classic linear blend skinning (value = 0)
    public static var classicLinear: swiftusd.SkelSkinningMethod {
        swiftusd.SkelSkinningMethod(swiftusd.SkelSkinningMethod.Value(rawValue: 0))
    }
    /// Dual quaternion skinning (value = 1)
    public static var dualQuaternion: swiftusd.SkelSkinningMethod {
        swiftusd.SkelSkinningMethod(swiftusd.SkelSkinningMethod.Value(rawValue: 1))
    }
}

// MARK: - Factory Method Extensions

extension swiftusd.SkelRootRef {
    /// Defines a new SkelRoot prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.SkelRootRef? {
        swiftusd.SkelRootRef.Define(stage, path)
    }

    /// Finds the SkelRoot at or above the given prim.
    public static func find(from prim: swiftusd.PrimRef) -> swiftusd.SkelRootRef? {
        swiftusd.SkelRootRef.Find(prim)
    }
}

extension swiftusd.SkelSkeletonRef {
    /// Defines a new Skeleton prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.SkelSkeletonRef? {
        swiftusd.SkelSkeletonRef.Define(stage, path)
    }
}

extension swiftusd.SkelAnimationRef {
    /// Defines a new SkelAnimation prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.SkelAnimationRef? {
        swiftusd.SkelAnimationRef.Define(stage, path)
    }
}

extension swiftusd.SkelBlendShapeRef {
    /// Defines a new BlendShape prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.SkelBlendShapeRef? {
        swiftusd.SkelBlendShapeRef.Define(stage, path)
    }
}

extension swiftusd.SkelBindingAPIRef {
    /// Applies SkelBindingAPI to the given prim.
    public static func apply(to prim: swiftusd.PrimRef) -> swiftusd.SkelBindingAPIRef? {
        swiftusd.SkelBindingAPIRef.Apply(prim)
    }
}

// MARK: - SkelSkeletonRef Extensions

extension swiftusd.SkelSkeletonRef {
    /// Gets the joints as an array of Tokens.
    public func getJoints() -> [swiftusd.Token] {
        let count = Int(GetJointsCount())
        guard count > 0 else { return [] }

        var buffer = [swiftusd.Token](repeating: swiftusd.Token(), count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetJoints(ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the joints from an array of Tokens.
    @discardableResult
    public func setJoints(_ joints: [swiftusd.Token]) -> Bool {
        joints.withUnsafeBufferPointer { ptr in
            SetJoints(ptr.baseAddress, joints.count)
        }
    }

    /// Gets the bind transforms as an array of Matrix4d (flattened 16 doubles each).
    public func getBindTransforms() -> [swiftusd.Matrix4d] {
        let count = Int(GetBindTransformsCount())
        guard count > 0 else { return [] }

        var buffer = [Double](repeating: 0, count: count * 16)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetBindTransforms(ptr.baseAddress, count)
        }

        var result = [swiftusd.Matrix4d]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            let offset = i * 16
            result.append(swiftusd.Matrix4d(
                buffer[offset + 0],  buffer[offset + 1],  buffer[offset + 2],  buffer[offset + 3],
                buffer[offset + 4],  buffer[offset + 5],  buffer[offset + 6],  buffer[offset + 7],
                buffer[offset + 8],  buffer[offset + 9],  buffer[offset + 10], buffer[offset + 11],
                buffer[offset + 12], buffer[offset + 13], buffer[offset + 14], buffer[offset + 15]
            ))
        }
        return result
    }

    /// Sets the bind transforms from an array of Matrix4d.
    @discardableResult
    public func setBindTransforms(_ transforms: [swiftusd.Matrix4d]) -> Bool {
        var buffer = [Double]()
        buffer.reserveCapacity(transforms.count * 16)
        for xform in transforms {
            // Get elements in row-major order
            for row in 0..<4 {
                for col in 0..<4 {
                    buffer.append(xform.Get(Int32(row), Int32(col)))
                }
            }
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetBindTransforms(ptr.baseAddress, transforms.count)
        }
    }

    /// Gets the rest transforms as an array of Matrix4d (flattened 16 doubles each).
    public func getRestTransforms() -> [swiftusd.Matrix4d] {
        let count = Int(GetRestTransformsCount())
        guard count > 0 else { return [] }

        var buffer = [Double](repeating: 0, count: count * 16)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetRestTransforms(ptr.baseAddress, count)
        }

        var result = [swiftusd.Matrix4d]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            let offset = i * 16
            result.append(swiftusd.Matrix4d(
                buffer[offset + 0],  buffer[offset + 1],  buffer[offset + 2],  buffer[offset + 3],
                buffer[offset + 4],  buffer[offset + 5],  buffer[offset + 6],  buffer[offset + 7],
                buffer[offset + 8],  buffer[offset + 9],  buffer[offset + 10], buffer[offset + 11],
                buffer[offset + 12], buffer[offset + 13], buffer[offset + 14], buffer[offset + 15]
            ))
        }
        return result
    }

    /// Sets the rest transforms from an array of Matrix4d.
    @discardableResult
    public func setRestTransforms(_ transforms: [swiftusd.Matrix4d]) -> Bool {
        var buffer = [Double]()
        buffer.reserveCapacity(transforms.count * 16)
        for xform in transforms {
            for row in 0..<4 {
                for col in 0..<4 {
                    buffer.append(xform.Get(Int32(row), Int32(col)))
                }
            }
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetRestTransforms(ptr.baseAddress, transforms.count)
        }
    }
}

// MARK: - SkelAnimationRef Extensions

extension swiftusd.SkelAnimationRef {
    /// Gets the joints as an array of Tokens.
    public func getJoints() -> [swiftusd.Token] {
        let count = Int(GetJointsCount())
        guard count > 0 else { return [] }

        var buffer = [swiftusd.Token](repeating: swiftusd.Token(), count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetJoints(ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the joints from an array of Tokens.
    @discardableResult
    public func setJoints(_ joints: [swiftusd.Token]) -> Bool {
        joints.withUnsafeBufferPointer { ptr in
            SetJoints(ptr.baseAddress, joints.count)
        }
    }

    /// Gets the translations as an array of Vec3f.
    public func getTranslations(at time: swiftusd.TimeCode = .Default()) -> [swiftusd.Vec3f] {
        let count = Int(GetTranslationsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetTranslations(time, ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the translations from an array of Vec3f.
    @discardableResult
    public func setTranslations(_ translations: [swiftusd.Vec3f], at time: swiftusd.TimeCode = .Default()) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(translations.count * 3)
        for t in translations {
            buffer.append(t.x)
            buffer.append(t.y)
            buffer.append(t.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetTranslations(time, ptr.baseAddress, translations.count)
        }
    }

    /// Gets the rotations as an array of Quatf (x, y, z, w).
    public func getRotations(at time: swiftusd.TimeCode = .Default()) -> [swiftusd.Quatf] {
        let count = Int(GetRotationsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 4)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetRotations(time, ptr.baseAddress, count)
        }

        var result = [swiftusd.Quatf]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            // Buffer is x, y, z, w order
            result.append(swiftusd.Quatf(
                buffer[i * 4 + 3],  // w (real)
                buffer[i * 4 + 0],  // x (imaginary)
                buffer[i * 4 + 1],  // y
                buffer[i * 4 + 2]   // z
            ))
        }
        return result
    }

    /// Sets the rotations from an array of Quatf.
    @discardableResult
    public func setRotations(_ rotations: [swiftusd.Quatf], at time: swiftusd.TimeCode = .Default()) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(rotations.count * 4)
        for q in rotations {
            // Store as x, y, z, w order
            let img = q.GetImaginary()
            buffer.append(img.x)
            buffer.append(img.y)
            buffer.append(img.z)
            buffer.append(q.GetReal())
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetRotations(time, ptr.baseAddress, rotations.count)
        }
    }

    /// Gets the scales as an array of Vec3f.
    public func getScales(at time: swiftusd.TimeCode = .Default()) -> [swiftusd.Vec3f] {
        let count = Int(GetScalesCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetScales(time, ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the scales from an array of Vec3f.
    @discardableResult
    public func setScales(_ scales: [swiftusd.Vec3f], at time: swiftusd.TimeCode = .Default()) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(scales.count * 3)
        for s in scales {
            buffer.append(s.x)
            buffer.append(s.y)
            buffer.append(s.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetScales(time, ptr.baseAddress, scales.count)
        }
    }

    /// Gets the blend shape weights as an array.
    public func getBlendShapeWeights(at time: swiftusd.TimeCode = .Default()) -> [Float] {
        let count = Int(GetBlendShapeWeightsCount(time))
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetBlendShapeWeights(time, ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the blend shape weights.
    @discardableResult
    public func setBlendShapeWeights(_ weights: [Float], at time: swiftusd.TimeCode = .Default()) -> Bool {
        weights.withUnsafeBufferPointer { ptr in
            SetBlendShapeWeights(time, ptr.baseAddress, weights.count)
        }
    }
}

// MARK: - SkelBlendShapeRef Extensions

extension swiftusd.SkelBlendShapeRef {
    /// Gets the offsets as an array of Vec3f.
    public func getOffsets() -> [swiftusd.Vec3f] {
        let count = Int(GetOffsetsCount())
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetOffsets(ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the offsets from an array of Vec3f.
    @discardableResult
    public func setOffsets(_ offsets: [swiftusd.Vec3f]) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(offsets.count * 3)
        for o in offsets {
            buffer.append(o.x)
            buffer.append(o.y)
            buffer.append(o.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetOffsets(ptr.baseAddress, offsets.count)
        }
    }

    /// Gets the normal offsets as an array of Vec3f.
    public func getNormalOffsets() -> [swiftusd.Vec3f] {
        let count = Int(GetNormalOffsetsCount())
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count * 3)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetNormalOffsets(ptr.baseAddress, count)
        }

        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            result.append(swiftusd.Vec3f(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]))
        }
        return result
    }

    /// Sets the normal offsets from an array of Vec3f.
    @discardableResult
    public func setNormalOffsets(_ offsets: [swiftusd.Vec3f]) -> Bool {
        var buffer = [Float]()
        buffer.reserveCapacity(offsets.count * 3)
        for o in offsets {
            buffer.append(o.x)
            buffer.append(o.y)
            buffer.append(o.z)
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetNormalOffsets(ptr.baseAddress, offsets.count)
        }
    }

    /// Gets the point indices as an array.
    public func getPointIndices() -> [Int32] {
        let count = Int(GetPointIndicesCount())
        guard count > 0 else { return [] }

        var buffer = [Int32](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetPointIndices(ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the point indices.
    @discardableResult
    public func setPointIndices(_ indices: [Int32]) -> Bool {
        indices.withUnsafeBufferPointer { ptr in
            SetPointIndices(ptr.baseAddress, indices.count)
        }
    }
}

// MARK: - SkelBindingAPIRef Extensions

extension swiftusd.SkelBindingAPIRef {
    /// Gets the geom bind transform as a Matrix4d.
    public func getGeomBindTransform() -> swiftusd.Matrix4d {
        var buffer = [Double](repeating: 0, count: 16)
        _ = buffer.withUnsafeMutableBufferPointer { ptr in
            GetGeomBindTransform(ptr.baseAddress)
        }
        return swiftusd.Matrix4d(
            buffer[0],  buffer[1],  buffer[2],  buffer[3],
            buffer[4],  buffer[5],  buffer[6],  buffer[7],
            buffer[8],  buffer[9],  buffer[10], buffer[11],
            buffer[12], buffer[13], buffer[14], buffer[15]
        )
    }

    /// Sets the geom bind transform from a Matrix4d.
    @discardableResult
    public func setGeomBindTransform(_ transform: swiftusd.Matrix4d) -> Bool {
        var buffer = [Double]()
        buffer.reserveCapacity(16)
        for row in 0..<4 {
            for col in 0..<4 {
                buffer.append(transform.Get(Int32(row), Int32(col)))
            }
        }
        return buffer.withUnsafeBufferPointer { ptr in
            SetGeomBindTransform(ptr.baseAddress)
        }
    }

    /// Gets the joint indices as an array.
    public func getJointIndices() -> [Int32] {
        let count = Int(GetJointIndicesCount())
        guard count > 0 else { return [] }

        var buffer = [Int32](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetJointIndices(ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the joint indices.
    @discardableResult
    public func setJointIndices(_ indices: [Int32]) -> Bool {
        indices.withUnsafeBufferPointer { ptr in
            SetJointIndices(ptr.baseAddress, indices.count)
        }
    }

    /// Gets the joint weights as an array.
    public func getJointWeights() -> [Float] {
        let count = Int(GetJointWeightsCount())
        guard count > 0 else { return [] }

        var buffer = [Float](repeating: 0, count: count)
        let actualCount = buffer.withUnsafeMutableBufferPointer { ptr in
            GetJointWeights(ptr.baseAddress, count)
        }
        return Array(buffer.prefix(Int(actualCount)))
    }

    /// Sets the joint weights.
    @discardableResult
    public func setJointWeights(_ weights: [Float]) -> Bool {
        weights.withUnsafeBufferPointer { ptr in
            SetJointWeights(ptr.baseAddress, weights.count)
        }
    }
}
