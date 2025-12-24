// UsdSkel.swift - UsdSkel module Swift wrappers
// Mirrors: pxr/usd/usdSkel/*.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - SkinningMethod

/// Skinning method for skeletal deformation.
public enum SkinningMethod: Int, Sendable {
    /// Classic linear blend skinning (LBS).
    case classicLinear = 0
    /// Dual quaternion skinning for better volume preservation.
    case dualQuaternion = 1

    internal var cValue: UsdSkelSkinningMethod {
        UsdSkelSkinningMethod(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdSkelSkinningMethod) {
        self = SkinningMethod(rawValue: Int(cValue.rawValue)) ?? .classicLinear
    }
}

// MARK: - SkelError

/// Errors that can occur when working with UsdSkel types.
public enum SkelError: Error, CustomStringConvertible {

    /// Failed to define a skeleton prim.
    case definitionFailed(String)

    /// A skeleton operation failed.
    case operationFailed(String)

    /// Invalid binding configuration.
    case invalidBinding(String)

    public var description: String {
        switch self {
        case .definitionFailed(let message):
            return "Skeleton definition failed: \(message)"
        case .operationFailed(let message):
            return "Skeleton operation failed: \(message)"
        case .invalidBinding(let message):
            return "Invalid binding: \(message)"
        }
    }
}

// MARK: - SkelRoot

/// A UsdSkelRoot prim marks the root of a skeleton hierarchy.
///
/// SkelRoot prims are used to encapsulate a skeleton and all of its
/// skinnable geometry. They serve as the binding scope for skeleton
/// relationships.
///
/// Mirrors `pxr::UsdSkelRoot` from the USD C++ API.
public final class SkelRoot: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdSkelRootRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdSkelRootRef) {
        self.handle = handle
    }

    deinit {
        UsdSkelRoot_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new SkelRoot prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new SkelRoot prim.
    /// - Returns: The newly defined SkelRoot.
    /// - Throws: `SkelError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> SkelRoot {
        guard let ref = UsdSkelRoot_Define(stage.handle, path.handle) else {
            throw SkelError.definitionFailed("Failed to define SkelRoot at '\(path)'")
        }
        return SkelRoot(handle: ref)
    }

    /// Creates a SkelRoot schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A SkelRoot wrapper, or `nil` if the prim is not a valid SkelRoot.
    public static func from(prim: Prim) -> SkelRoot? {
        guard let ref = UsdSkelRoot_FromPrim(prim.handle) else {
            return nil
        }
        let root = SkelRoot(handle: ref)
        return root.isValid ? root : nil
    }

    /// Finds the SkelRoot at or above the given prim.
    ///
    /// - Parameter prim: The prim to search from.
    /// - Returns: The SkelRoot, or `nil` if none is found.
    public static func find(from prim: Prim) -> SkelRoot? {
        guard let ref = UsdSkelRoot_Find(prim.handle) else {
            return nil
        }
        return SkelRoot(handle: ref)
    }

    // MARK: - Validity

    /// Returns `true` if the SkelRoot is valid.
    public var isValid: Bool {
        UsdSkelRoot_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdSkelRoot_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }
}

// MARK: - CustomStringConvertible

extension SkelRoot: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid SkelRoot>"
    }
}

// MARK: - Skeleton

/// A UsdSkelSkeleton prim defines a skeletal hierarchy.
///
/// A Skeleton contains the joint hierarchy (topology), bind poses (world space
/// transforms at bind time), and rest poses (local space rest transforms).
///
/// Mirrors `pxr::UsdSkelSkeleton` from the USD C++ API.
public final class Skeleton: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdSkelSkeletonRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdSkelSkeletonRef) {
        self.handle = handle
    }

    deinit {
        UsdSkelSkeleton_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Skeleton prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Skeleton prim.
    /// - Returns: The newly defined Skeleton.
    /// - Throws: `SkelError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> Skeleton {
        guard let ref = UsdSkelSkeleton_Define(stage.handle, path.handle) else {
            throw SkelError.definitionFailed("Failed to define Skeleton at '\(path)'")
        }
        return Skeleton(handle: ref)
    }

    /// Creates a Skeleton schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A Skeleton wrapper, or `nil` if the prim is not a valid Skeleton.
    public static func from(prim: Prim) -> Skeleton? {
        guard let ref = UsdSkelSkeleton_FromPrim(prim.handle) else {
            return nil
        }
        let skeleton = Skeleton(handle: ref)
        return skeleton.isValid ? skeleton : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Skeleton is valid.
    public var isValid: Bool {
        UsdSkelSkeleton_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdSkelSkeleton_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Joints

    /// The number of joints in the skeleton.
    public var jointCount: Int {
        Int(UsdSkelSkeleton_GetJointCount(handle))
    }

    /// Gets the joint paths as tokens.
    ///
    /// - Returns: An array of tokens representing the joint paths.
    public func joints() -> [Token] {
        let count = jointCount
        guard count > 0 else { return [] }

        var tokens = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = tokens.withUnsafeMutableBufferPointer { buffer in
            UsdSkelSkeleton_GetJoints(handle, buffer.baseAddress, count)
        }

        return tokens.prefix(Int(actualCount)).compactMap { tokenRef in
            guard let ref = tokenRef else { return nil }
            return Token(handle: ref)
        }
    }

    /// Sets the joint paths.
    ///
    /// - Parameter joints: An array of tokens representing the joint paths.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setJoints(_ joints: [Token]) throws {
        var tokenRefs: [TfTokenRef?] = joints.map { $0.handle }
        let result = tokenRefs.withUnsafeMutableBufferPointer { buffer in
            UsdSkelSkeleton_SetJoints(handle, buffer.baseAddress, joints.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set skeleton joints")
        }
    }

    // MARK: - Bind Transforms

    /// Gets the bind transforms (world space).
    ///
    /// - Returns: An array of 4x4 matrices as arrays of 16 doubles each.
    public func bindTransforms() -> [[Double]] {
        let count = jointCount
        guard count > 0 else { return [] }

        var transforms = [Double](repeating: 0, count: count * 16)
        let actualCount = transforms.withUnsafeMutableBufferPointer { buffer in
            UsdSkelSkeleton_GetBindTransforms(handle, buffer.baseAddress, count)
        }

        var result: [[Double]] = []
        for i in 0..<Int(actualCount) {
            let start = i * 16
            result.append(Array(transforms[start..<start + 16]))
        }
        return result
    }

    /// Sets the bind transforms (world space).
    ///
    /// - Parameter transforms: An array of 4x4 matrices as arrays of 16 doubles each.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setBindTransforms(_ transforms: [[Double]]) throws {
        let flatTransforms = transforms.flatMap { $0 }
        let result = flatTransforms.withUnsafeBufferPointer { buffer in
            UsdSkelSkeleton_SetBindTransforms(handle, buffer.baseAddress, transforms.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set bind transforms")
        }
    }

    // MARK: - Rest Transforms

    /// Gets the rest transforms (local space).
    ///
    /// - Returns: An array of 4x4 matrices as arrays of 16 doubles each.
    public func restTransforms() -> [[Double]] {
        let count = jointCount
        guard count > 0 else { return [] }

        var transforms = [Double](repeating: 0, count: count * 16)
        let actualCount = transforms.withUnsafeMutableBufferPointer { buffer in
            UsdSkelSkeleton_GetRestTransforms(handle, buffer.baseAddress, count)
        }

        var result: [[Double]] = []
        for i in 0..<Int(actualCount) {
            let start = i * 16
            result.append(Array(transforms[start..<start + 16]))
        }
        return result
    }

    /// Sets the rest transforms (local space).
    ///
    /// - Parameter transforms: An array of 4x4 matrices as arrays of 16 doubles each.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setRestTransforms(_ transforms: [[Double]]) throws {
        let flatTransforms = transforms.flatMap { $0 }
        let result = flatTransforms.withUnsafeBufferPointer { buffer in
            UsdSkelSkeleton_SetRestTransforms(handle, buffer.baseAddress, transforms.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set rest transforms")
        }
    }
}

// MARK: - CustomStringConvertible

extension Skeleton: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid Skeleton>"
    }
}

// MARK: - SkelAnimation

/// A UsdSkelAnimation prim contains animation data for a skeleton.
///
/// Animation data includes per-joint translations, rotations, and scales,
/// as well as blend shape weights.
///
/// Mirrors `pxr::UsdSkelAnimation` from the USD C++ API.
public final class SkelAnimation: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdSkelAnimationRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdSkelAnimationRef) {
        self.handle = handle
    }

    deinit {
        UsdSkelAnimation_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new SkelAnimation prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new SkelAnimation prim.
    /// - Returns: The newly defined SkelAnimation.
    /// - Throws: `SkelError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> SkelAnimation {
        guard let ref = UsdSkelAnimation_Define(stage.handle, path.handle) else {
            throw SkelError.definitionFailed("Failed to define SkelAnimation at '\(path)'")
        }
        return SkelAnimation(handle: ref)
    }

    /// Creates a SkelAnimation schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A SkelAnimation wrapper, or `nil` if the prim is not a valid SkelAnimation.
    public static func from(prim: Prim) -> SkelAnimation? {
        guard let ref = UsdSkelAnimation_FromPrim(prim.handle) else {
            return nil
        }
        let anim = SkelAnimation(handle: ref)
        return anim.isValid ? anim : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Animation is valid.
    public var isValid: Bool {
        UsdSkelAnimation_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdSkelAnimation_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Joints

    /// The number of joints in the animation.
    public var jointCount: Int {
        Int(UsdSkelAnimation_GetJointCount(handle))
    }

    /// Gets the joint tokens.
    ///
    /// - Returns: An array of tokens representing the animated joints.
    public func joints() -> [Token] {
        let count = jointCount
        guard count > 0 else { return [] }

        var tokens = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = tokens.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_GetJoints(handle, buffer.baseAddress, count)
        }

        return tokens.prefix(Int(actualCount)).compactMap { tokenRef in
            guard let ref = tokenRef else { return nil }
            return Token(handle: ref)
        }
    }

    /// Sets the joint tokens.
    ///
    /// - Parameter joints: An array of tokens representing the animated joints.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setJoints(_ joints: [Token]) throws {
        var tokenRefs: [TfTokenRef?] = joints.map { $0.handle }
        let result = tokenRefs.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_SetJoints(handle, buffer.baseAddress, joints.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set animation joints")
        }
    }

    // MARK: - Translations

    /// Gets the translations at a given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of (x, y, z) translation tuples.
    public func translations(at time: TimeCode = .default) -> [(x: Float, y: Float, z: Float)] {
        let count = jointCount
        guard count > 0 else { return [] }

        var data = [Float](repeating: 0, count: count * 3)
        let actualCount = data.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_GetTranslations(handle, time.cTimeCode, buffer.baseAddress, count)
        }

        var result: [(x: Float, y: Float, z: Float)] = []
        for i in 0..<Int(actualCount) {
            result.append((data[i * 3], data[i * 3 + 1], data[i * 3 + 2]))
        }
        return result
    }

    /// Sets the translations at a given time.
    ///
    /// - Parameters:
    ///   - translations: An array of (x, y, z) translation tuples.
    ///   - time: The time code at which to set the value.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setTranslations(_ translations: [(x: Float, y: Float, z: Float)], at time: TimeCode = .default) throws {
        let flat = translations.flatMap { [$0.x, $0.y, $0.z] }
        let result = flat.withUnsafeBufferPointer { buffer in
            UsdSkelAnimation_SetTranslations(handle, time.cTimeCode, buffer.baseAddress, translations.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set translations")
        }
    }

    // MARK: - Rotations

    /// Gets the rotations at a given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of (w, x, y, z) quaternion tuples.
    public func rotations(at time: TimeCode = .default) -> [(w: Float, x: Float, y: Float, z: Float)] {
        let count = jointCount
        guard count > 0 else { return [] }

        var data = [Float](repeating: 0, count: count * 4)
        let actualCount = data.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_GetRotations(handle, time.cTimeCode, buffer.baseAddress, count)
        }

        var result: [(w: Float, x: Float, y: Float, z: Float)] = []
        for i in 0..<Int(actualCount) {
            result.append((data[i * 4], data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]))
        }
        return result
    }

    /// Sets the rotations at a given time.
    ///
    /// - Parameters:
    ///   - rotations: An array of (w, x, y, z) quaternion tuples.
    ///   - time: The time code at which to set the value.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setRotations(_ rotations: [(w: Float, x: Float, y: Float, z: Float)], at time: TimeCode = .default) throws {
        let flat = rotations.flatMap { [$0.w, $0.x, $0.y, $0.z] }
        let result = flat.withUnsafeBufferPointer { buffer in
            UsdSkelAnimation_SetRotations(handle, time.cTimeCode, buffer.baseAddress, rotations.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set rotations")
        }
    }

    // MARK: - Scales

    /// Gets the scales at a given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of (x, y, z) scale tuples.
    public func scales(at time: TimeCode = .default) -> [(x: Float, y: Float, z: Float)] {
        let count = jointCount
        guard count > 0 else { return [] }

        var data = [Float](repeating: 0, count: count * 3)
        let actualCount = data.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_GetScales(handle, time.cTimeCode, buffer.baseAddress, count)
        }

        var result: [(x: Float, y: Float, z: Float)] = []
        for i in 0..<Int(actualCount) {
            result.append((data[i * 3], data[i * 3 + 1], data[i * 3 + 2]))
        }
        return result
    }

    /// Sets the scales at a given time.
    ///
    /// - Parameters:
    ///   - scales: An array of (x, y, z) scale tuples.
    ///   - time: The time code at which to set the value.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setScales(_ scales: [(x: Float, y: Float, z: Float)], at time: TimeCode = .default) throws {
        let flat = scales.flatMap { [$0.x, $0.y, $0.z] }
        let result = flat.withUnsafeBufferPointer { buffer in
            UsdSkelAnimation_SetScales(handle, time.cTimeCode, buffer.baseAddress, scales.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set scales")
        }
    }

    // MARK: - Blend Shapes

    /// The number of blend shapes in the animation.
    public var blendShapeCount: Int {
        Int(UsdSkelAnimation_GetBlendShapeCount(handle))
    }

    /// Gets the blend shape tokens.
    ///
    /// - Returns: An array of tokens representing the blend shapes.
    public func blendShapes() -> [Token] {
        let count = blendShapeCount
        guard count > 0 else { return [] }

        var tokens = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = tokens.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_GetBlendShapes(handle, buffer.baseAddress, count)
        }

        return tokens.prefix(Int(actualCount)).compactMap { tokenRef in
            guard let ref = tokenRef else { return nil }
            return Token(handle: ref)
        }
    }

    /// Sets the blend shape tokens.
    ///
    /// - Parameter blendShapes: An array of tokens representing the blend shapes.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setBlendShapes(_ blendShapes: [Token]) throws {
        var tokenRefs: [TfTokenRef?] = blendShapes.map { $0.handle }
        let result = tokenRefs.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_SetBlendShapes(handle, buffer.baseAddress, blendShapes.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set blend shapes")
        }
    }

    /// Gets the blend shape weights at a given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: An array of weight values.
    public func blendShapeWeights(at time: TimeCode = .default) -> [Float] {
        let count = blendShapeCount
        guard count > 0 else { return [] }

        var weights = [Float](repeating: 0, count: count)
        let actualCount = weights.withUnsafeMutableBufferPointer { buffer in
            UsdSkelAnimation_GetBlendShapeWeights(handle, time.cTimeCode, buffer.baseAddress, count)
        }
        return Array(weights.prefix(Int(actualCount)))
    }

    /// Sets the blend shape weights at a given time.
    ///
    /// - Parameters:
    ///   - weights: An array of weight values.
    ///   - time: The time code at which to set the value.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setBlendShapeWeights(_ weights: [Float], at time: TimeCode = .default) throws {
        let result = weights.withUnsafeBufferPointer { buffer in
            UsdSkelAnimation_SetBlendShapeWeights(handle, time.cTimeCode, buffer.baseAddress, weights.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set blend shape weights")
        }
    }
}

// MARK: - CustomStringConvertible

extension SkelAnimation: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid SkelAnimation>"
    }
}

// MARK: - BlendShape

/// A UsdSkelBlendShape prim defines a blend shape target.
///
/// Blend shapes store position and normal offsets that can be applied
/// to geometry to create facial expressions or other deformations.
///
/// Mirrors `pxr::UsdSkelBlendShape` from the USD C++ API.
public final class BlendShape: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdSkelBlendShapeRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdSkelBlendShapeRef) {
        self.handle = handle
    }

    deinit {
        UsdSkelBlendShape_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new BlendShape prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new BlendShape prim.
    /// - Returns: The newly defined BlendShape.
    /// - Throws: `SkelError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> BlendShape {
        guard let ref = UsdSkelBlendShape_Define(stage.handle, path.handle) else {
            throw SkelError.definitionFailed("Failed to define BlendShape at '\(path)'")
        }
        return BlendShape(handle: ref)
    }

    /// Creates a BlendShape schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A BlendShape wrapper, or `nil` if the prim is not a valid BlendShape.
    public static func from(prim: Prim) -> BlendShape? {
        guard let ref = UsdSkelBlendShape_FromPrim(prim.handle) else {
            return nil
        }
        let blendShape = BlendShape(handle: ref)
        return blendShape.isValid ? blendShape : nil
    }

    // MARK: - Validity

    /// Returns `true` if the BlendShape is valid.
    public var isValid: Bool {
        UsdSkelBlendShape_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdSkelBlendShape_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Offsets

    /// The number of offsets.
    public var offsetsCount: Int {
        Int(UsdSkelBlendShape_GetOffsetsCount(handle))
    }

    /// Gets the position offsets.
    ///
    /// - Returns: An array of floats (x, y, z interleaved).
    public func offsets() -> [Float] {
        let count = offsetsCount
        guard count > 0 else { return [] }

        var data = [Float](repeating: 0, count: count * 3)
        let actualCount = data.withUnsafeMutableBufferPointer { buffer in
            UsdSkelBlendShape_GetOffsets(handle, buffer.baseAddress, count)
        }
        return Array(data.prefix(Int(actualCount) * 3))
    }

    /// Sets the position offsets.
    ///
    /// - Parameter offsets: An array of floats (x, y, z interleaved).
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setOffsets(_ offsets: [Float]) throws {
        let count = offsets.count / 3
        let result = offsets.withUnsafeBufferPointer { buffer in
            UsdSkelBlendShape_SetOffsets(handle, buffer.baseAddress, count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set blend shape offsets")
        }
    }

    /// Gets the normal offsets.
    ///
    /// - Returns: An array of floats (x, y, z interleaved).
    public func normalOffsets() -> [Float] {
        let count = offsetsCount
        guard count > 0 else { return [] }

        var data = [Float](repeating: 0, count: count * 3)
        let actualCount = data.withUnsafeMutableBufferPointer { buffer in
            UsdSkelBlendShape_GetNormalOffsets(handle, buffer.baseAddress, count)
        }
        return Array(data.prefix(Int(actualCount) * 3))
    }

    /// Sets the normal offsets.
    ///
    /// - Parameter normalOffsets: An array of floats (x, y, z interleaved).
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setNormalOffsets(_ normalOffsets: [Float]) throws {
        let count = normalOffsets.count / 3
        let result = normalOffsets.withUnsafeBufferPointer { buffer in
            UsdSkelBlendShape_SetNormalOffsets(handle, buffer.baseAddress, count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set blend shape normal offsets")
        }
    }

    // MARK: - Point Indices

    /// The number of point indices.
    public var pointIndicesCount: Int {
        Int(UsdSkelBlendShape_GetPointIndicesCount(handle))
    }

    /// Gets the point indices.
    ///
    /// - Returns: An array of indices indicating which points the offsets apply to.
    public func pointIndices() -> [Int32] {
        let count = pointIndicesCount
        guard count > 0 else { return [] }

        var indices = [Int32](repeating: 0, count: count)
        let actualCount = indices.withUnsafeMutableBufferPointer { buffer in
            UsdSkelBlendShape_GetPointIndices(handle, buffer.baseAddress, count)
        }
        return Array(indices.prefix(Int(actualCount)))
    }

    /// Sets the point indices.
    ///
    /// - Parameter indices: An array of indices indicating which points the offsets apply to.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setPointIndices(_ indices: [Int32]) throws {
        let result = indices.withUnsafeBufferPointer { buffer in
            UsdSkelBlendShape_SetPointIndices(handle, buffer.baseAddress, indices.count)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set blend shape point indices")
        }
    }
}

// MARK: - CustomStringConvertible

extension BlendShape: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid BlendShape>"
    }
}

// MARK: - SkelBindingAPI

/// A UsdSkelBindingAPI provides API for skeletal binding on prims.
///
/// This API schema is applied to geometry prims to bind them to skeletons
/// and specify joint influences for skinning.
///
/// Mirrors `pxr::UsdSkelBindingAPI` from the USD C++ API.
public final class SkelBindingAPI: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdSkelBindingAPIRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdSkelBindingAPIRef) {
        self.handle = handle
    }

    deinit {
        UsdSkelBindingAPI_Release(handle)
    }

    // MARK: - Factory Methods

    /// Gets the BindingAPI from a prim (if already applied).
    ///
    /// - Parameter prim: The prim to get the API from.
    /// - Returns: A SkelBindingAPI wrapper, or `nil` if the API is not applied.
    public static func get(from prim: Prim) -> SkelBindingAPI? {
        guard let ref = UsdSkelBindingAPI_Get(prim.handle) else {
            return nil
        }
        let api = SkelBindingAPI(handle: ref)
        return api.isValid ? api : nil
    }

    /// Applies the BindingAPI to a prim.
    ///
    /// - Parameter prim: The prim to apply the API to.
    /// - Returns: The applied SkelBindingAPI.
    /// - Throws: `SkelError.operationFailed` if the API cannot be applied.
    public static func apply(to prim: Prim) throws -> SkelBindingAPI {
        guard let ref = UsdSkelBindingAPI_Apply(prim.handle) else {
            throw SkelError.operationFailed("Failed to apply SkelBindingAPI")
        }
        return SkelBindingAPI(handle: ref)
    }

    // MARK: - Validity

    /// Returns `true` if the BindingAPI is valid.
    public var isValid: Bool {
        UsdSkelBindingAPI_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdSkelBindingAPI_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Skeleton Relationship

    /// Gets the path to the bound skeleton.
    public var skeletonPath: Path? {
        guard let ref = UsdSkelBindingAPI_GetSkeletonPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Sets the skeleton relationship target.
    ///
    /// - Parameter path: The path to the skeleton.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setSkeletonPath(_ path: Path) throws {
        let result = UsdSkelBindingAPI_SetSkeletonPath(handle, path.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set skeleton path")
        }
    }

    // MARK: - Animation Source Relationship

    /// Gets the path to the animation source.
    public var animationSourcePath: Path? {
        guard let ref = UsdSkelBindingAPI_GetAnimationSourcePath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Sets the animation source relationship target.
    ///
    /// - Parameter path: The path to the animation source.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setAnimationSourcePath(_ path: Path) throws {
        let result = UsdSkelBindingAPI_SetAnimationSourcePath(handle, path.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set animation source path")
        }
    }

    // MARK: - Joint Influences

    /// Gets the joint indices.
    ///
    /// - Returns: An array of joint indices for skinning.
    public func jointIndices() -> [Int32] {
        let count = Int(UsdSkelBindingAPI_GetJointIndicesCount(handle))
        guard count > 0 else { return [] }

        var indices = [Int32](repeating: 0, count: count)
        let actualCount = indices.withUnsafeMutableBufferPointer { buffer in
            UsdSkelBindingAPI_GetJointIndices(handle, buffer.baseAddress, count)
        }
        return Array(indices.prefix(Int(actualCount)))
    }

    /// Sets the joint indices.
    ///
    /// - Parameters:
    ///   - indices: An array of joint indices.
    ///   - constant: If true, uses constant interpolation (rigid binding).
    ///   - elementSize: Number of influences per point (-1 for auto).
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setJointIndices(_ indices: [Int32], constant: Bool = false, elementSize: Int = -1) throws {
        let result = indices.withUnsafeBufferPointer { buffer in
            UsdSkelBindingAPI_SetJointIndices(handle, buffer.baseAddress, indices.count, constant, Int32(elementSize))
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set joint indices")
        }
    }

    /// Gets the joint weights.
    ///
    /// - Returns: An array of joint weights for skinning.
    public func jointWeights() -> [Float] {
        let count = Int(UsdSkelBindingAPI_GetJointWeightsCount(handle))
        guard count > 0 else { return [] }

        var weights = [Float](repeating: 0, count: count)
        let actualCount = weights.withUnsafeMutableBufferPointer { buffer in
            UsdSkelBindingAPI_GetJointWeights(handle, buffer.baseAddress, count)
        }
        return Array(weights.prefix(Int(actualCount)))
    }

    /// Sets the joint weights.
    ///
    /// - Parameters:
    ///   - weights: An array of joint weights.
    ///   - constant: If true, uses constant interpolation (rigid binding).
    ///   - elementSize: Number of influences per point (-1 for auto).
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setJointWeights(_ weights: [Float], constant: Bool = false, elementSize: Int = -1) throws {
        let result = weights.withUnsafeBufferPointer { buffer in
            UsdSkelBindingAPI_SetJointWeights(handle, buffer.baseAddress, weights.count, constant, Int32(elementSize))
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set joint weights")
        }
    }

    /// Sets a rigid joint influence (single joint binding).
    ///
    /// - Parameters:
    ///   - jointIndex: The index of the joint to bind to.
    ///   - weight: The influence weight (default 1.0).
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setRigidJointInfluence(jointIndex: Int, weight: Float = 1.0) throws {
        let result = UsdSkelBindingAPI_SetRigidJointInfluence(handle, Int32(jointIndex), weight)
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set rigid joint influence")
        }
    }

    // MARK: - Geometry Bind Transform

    /// Gets the geometry bind transform.
    ///
    /// - Returns: A 4x4 matrix as an array of 16 doubles.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func geomBindTransform() throws -> [Double] {
        var matrix = [Double](repeating: 0, count: 16)
        let result = matrix.withUnsafeMutableBufferPointer { buffer in
            UsdSkelBindingAPI_GetGeomBindTransform(handle, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to get geom bind transform")
        }
        return matrix
    }

    /// Sets the geometry bind transform.
    ///
    /// - Parameter matrix: A 4x4 matrix as an array of 16 doubles.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setGeomBindTransform(_ matrix: [Double]) throws {
        guard matrix.count == 16 else {
            throw SkelError.invalidBinding("Matrix must have 16 elements")
        }
        let result = matrix.withUnsafeBufferPointer { buffer in
            UsdSkelBindingAPI_SetGeomBindTransform(handle, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set geom bind transform")
        }
    }

    // MARK: - Skinning Method

    /// Gets the skinning method.
    public var skinningMethod: SkinningMethod {
        SkinningMethod(cValue: UsdSkelBindingAPI_GetSkinningMethod(handle))
    }

    /// Sets the skinning method.
    ///
    /// - Parameter method: The skinning method to use.
    /// - Throws: `SkelError.operationFailed` if the operation fails.
    public func setSkinningMethod(_ method: SkinningMethod) throws {
        let result = UsdSkelBindingAPI_SetSkinningMethod(handle, method.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw SkelError.operationFailed("Failed to set skinning method")
        }
    }
}

// MARK: - CustomStringConvertible

extension SkelBindingAPI: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid SkelBindingAPI>"
    }
}

// MARK: - Type Aliases

/// Type alias for UsdSkelRoot
public typealias UsdSkelRoot = SkelRoot

/// Type alias for UsdSkelSkeleton
public typealias UsdSkelSkeleton = Skeleton

/// Type alias for UsdSkelAnimation
public typealias UsdSkelAnimation = SkelAnimation

/// Type alias for UsdSkelBlendShape
public typealias UsdSkelBlendShape = BlendShape

/// Type alias for UsdSkelBindingAPI
public typealias UsdSkelBindingAPI = SkelBindingAPI
