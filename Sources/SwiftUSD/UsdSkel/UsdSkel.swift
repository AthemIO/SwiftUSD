// UsdSkel.swift - Skeletal Animation Schemas - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's UsdSkel types,
// including skeletons, skeleton roots, animations, blend shapes, and
// related API schemas for skeletal animation and skinning.

import CxxFacade

// MARK: - Type Aliases

/// UsdSkelTopology - Describes skeleton topology (joint hierarchy).
///
/// UsdSkelTopology holds information about how joints are connected
/// in a skeleton hierarchy, using parent indices.
///
/// ## Usage
/// ```swift
/// // Create topology from parent indices
/// // Root joint has parent -1, others reference their parent's index
/// let parentIndices: [Int32] = [-1, 0, 1]  // Root -> Child -> Grandchild
/// var topology = UsdSkelTopology(parentIndices)
///
/// // Query topology
/// let numJoints = topology.GetNumJoints()
/// let isRoot = topology.IsRoot(0)  // true
/// ```
public typealias UsdSkelTopology = cxxfacade.usdSkel.UsdSkelTopology

/// UsdSkelSkeleton - Skeleton prim schema.
///
/// UsdSkelSkeleton describes a skeleton by defining its joint hierarchy
/// and the bind/rest poses of those joints.
///
/// ## Usage
/// ```swift
/// // Define a skeleton
/// var stage = UsdStage.createInMemory()
/// let skeleton = UsdSkelSkeleton.Define(&stage, SdfPath("/MySkeleton"))
///
/// // Set up joints and transforms
/// _ = skeleton.CreateJointsAttr()
/// _ = skeleton.CreateBindTransformsAttr()
/// _ = skeleton.CreateRestTransformsAttr()
/// ```
public typealias UsdSkelSkeleton = cxxfacade.usdSkel.UsdSkelSkeleton

/// UsdSkelRoot - Skeleton root prim schema.
///
/// UsdSkelRoot defines a scope beneath which skeletally-posed primitives
/// are defined. A SkelRoot must exist at or above any skinned primitive.
///
/// ## Usage
/// ```swift
/// // Define a skeleton root
/// var stage = UsdStage.createInMemory()
/// let root = UsdSkelRoot.Define(&stage, SdfPath("/Character"))
///
/// // Find root from a descendant prim
/// let foundRoot = UsdSkelRoot.Find(somePrim)
/// ```
public typealias UsdSkelRoot = cxxfacade.usdSkel.UsdSkelRoot

/// UsdSkelAnimation - Skeletal animation prim schema.
///
/// UsdSkelAnimation stores joint animation in vectorized form:
/// translations, rotations, scales, and blend shape weights.
///
/// ## Usage
/// ```swift
/// // Define an animation
/// var stage = UsdStage.createInMemory()
/// let anim = UsdSkelAnimation.Define(&stage, SdfPath("/Animation"))
///
/// // Set up animation channels
/// _ = anim.CreateJointsAttr()
/// _ = anim.CreateTranslationsAttr()
/// _ = anim.CreateRotationsAttr()
/// _ = anim.CreateScalesAttr()
/// ```
public typealias UsdSkelAnimation = cxxfacade.usdSkel.UsdSkelAnimation

/// UsdSkelBindingAPI - API for skeletal binding properties.
///
/// UsdSkelBindingAPI provides properties for binding skeletons and
/// animations to geometry, including joint influences.
///
/// ## Usage
/// ```swift
/// // Apply binding API to a mesh
/// let bindingAPI = UsdSkelBindingAPI.Apply(meshPrim)
///
/// // Set up skeleton binding
/// _ = bindingAPI.CreateSkeletonRel()
/// _ = bindingAPI.CreateAnimationSourceRel()
/// ```
public typealias UsdSkelBindingAPI = cxxfacade.usdSkel.UsdSkelBindingAPI

/// UsdSkelBlendShape - Blend shape prim schema.
///
/// UsdSkelBlendShape describes a target blend shape with position
/// and normal offsets from a base pose.
///
/// ## Usage
/// ```swift
/// // Define a blend shape
/// var stage = UsdStage.createInMemory()
/// let blendShape = UsdSkelBlendShape.Define(&stage, SdfPath("/BlendShape"))
///
/// // Set up offsets
/// _ = blendShape.CreateOffsetsAttr()
/// _ = blendShape.CreateNormalOffsetsAttr()
/// ```
public typealias UsdSkelBlendShape = cxxfacade.usdSkel.UsdSkelBlendShape

/// UsdSkelCache - Cache for skeleton queries.
///
/// UsdSkelCache provides thread-safe caching for skeleton query objects
/// to efficiently access skeleton, animation, and skinning data.
///
/// ## Usage
/// ```swift
/// // Create and populate cache
/// let cache = UsdSkelCache()
/// cache.Populate(skelRoot)
///
/// // Get queries
/// let skelQuery = cache.GetSkelQuery(skeleton)
/// let animQuery = cache.GetAnimQuery(animation)
/// ```
public typealias UsdSkelCache = cxxfacade.usdSkel.UsdSkelCache

/// UsdSkelAnimQuery - Animation query object.
///
/// UsdSkelAnimQuery provides efficient queries of animation primitives.
public typealias UsdSkelAnimQuery = cxxfacade.usdSkel.UsdSkelAnimQuery

/// UsdSkelSkeletonQuery - Skeleton query object.
///
/// UsdSkelSkeletonQuery provides the primary interface to reading
/// bound skeleton data.
public typealias UsdSkelSkeletonQuery = cxxfacade.usdSkel.UsdSkelSkeletonQuery

/// UsdSkelSkinningQuery - Skinning query object.
///
/// UsdSkelSkinningQuery provides queries for resolved skinning bindings.
public typealias UsdSkelSkinningQuery = cxxfacade.usdSkel.UsdSkelSkinningQuery

// MARK: - UsdSkelTopology Extensions

extension UsdSkelTopology {
    /// The number of joints in the topology.
    public var jointCount: Int {
        return Int(self.GetNumJoints())
    }

    /// Whether the topology is valid (has joints).
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The parent indices as a Swift array.
    public var parentIndices: [Int32] {
        let indices = self.GetParentIndices()
        var result: [Int32] = []
        for i in 0..<indices.size() {
            result.append(indices[i])
        }
        return result
    }
}

// MARK: - UsdSkelSkeleton Extensions

extension UsdSkelSkeleton {
    /// Define a skeleton at the given path.
    public static func define(stage: inout UsdStage, path: SdfPath) -> UsdSkelSkeleton {
        return UsdSkelSkeleton.Define(&stage, path)
    }

    /// The underlying prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether the skeleton is valid.
    public var isValid: Bool {
        return self.GetPrim().IsValid()
    }

    /// The joints attribute.
    public var jointsAttr: UsdAttribute {
        return self.GetJointsAttr()
    }

    /// The bind transforms attribute.
    public var bindTransformsAttr: UsdAttribute {
        return self.GetBindTransformsAttr()
    }

    /// The rest transforms attribute.
    public var restTransformsAttr: UsdAttribute {
        return self.GetRestTransformsAttr()
    }
}

// MARK: - UsdSkelRoot Extensions

extension UsdSkelRoot {
    /// Define a skeleton root at the given path.
    public static func define(stage: inout UsdStage, path: SdfPath) -> UsdSkelRoot {
        return UsdSkelRoot.Define(&stage, path)
    }

    /// The underlying prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether the skeleton root is valid.
    public var isValid: Bool {
        return self.GetPrim().IsValid()
    }
}

// MARK: - UsdSkelAnimation Extensions

extension UsdSkelAnimation {
    /// Define an animation at the given path.
    public static func define(stage: inout UsdStage, path: SdfPath) -> UsdSkelAnimation {
        return UsdSkelAnimation.Define(&stage, path)
    }

    /// The underlying prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether the animation is valid.
    public var isValid: Bool {
        return self.GetPrim().IsValid()
    }

    /// The joints attribute.
    public var jointsAttr: UsdAttribute {
        return self.GetJointsAttr()
    }

    /// The translations attribute.
    public var translationsAttr: UsdAttribute {
        return self.GetTranslationsAttr()
    }

    /// The rotations attribute.
    public var rotationsAttr: UsdAttribute {
        return self.GetRotationsAttr()
    }

    /// The scales attribute.
    public var scalesAttr: UsdAttribute {
        return self.GetScalesAttr()
    }

    /// The blend shapes attribute.
    public var blendShapesAttr: UsdAttribute {
        return self.GetBlendShapesAttr()
    }

    /// The blend shape weights attribute.
    public var blendShapeWeightsAttr: UsdAttribute {
        return self.GetBlendShapeWeightsAttr()
    }
}

// MARK: - UsdSkelBindingAPI Extensions

extension UsdSkelBindingAPI {
    /// Apply the binding API to a prim.
    public static func apply(to prim: UsdPrim) -> UsdSkelBindingAPI {
        return UsdSkelBindingAPI.Apply(prim)
    }

    /// The underlying prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether the binding API is valid.
    public var isValid: Bool {
        return self.GetPrim().IsValid()
    }

    /// The skeleton relationship.
    public var skeletonRel: UsdRelationship {
        return self.GetSkeletonRel()
    }

    /// The animation source relationship.
    public var animationSourceRel: UsdRelationship {
        return self.GetAnimationSourceRel()
    }

    /// The inherited skeleton.
    public var inheritedSkeleton: UsdSkelSkeleton {
        return self.GetInheritedSkeleton()
    }

    /// The inherited animation source.
    public var inheritedAnimationSource: UsdPrim {
        return self.GetInheritedAnimationSource()
    }
}

// MARK: - UsdSkelBlendShape Extensions

extension UsdSkelBlendShape {
    /// Define a blend shape at the given path.
    public static func define(stage: inout UsdStage, path: SdfPath) -> UsdSkelBlendShape {
        return UsdSkelBlendShape.Define(&stage, path)
    }

    /// The underlying prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether the blend shape is valid.
    public var isValid: Bool {
        return self.GetPrim().IsValid()
    }

    /// The offsets attribute.
    public var offsetsAttr: UsdAttribute {
        return self.GetOffsetsAttr()
    }

    /// The normal offsets attribute.
    public var normalOffsetsAttr: UsdAttribute {
        return self.GetNormalOffsetsAttr()
    }

    /// The point indices attribute.
    public var pointIndicesAttr: UsdAttribute {
        return self.GetPointIndicesAttr()
    }
}

// MARK: - UsdSkelCache Extensions

extension UsdSkelCache {
    /// Populate the cache for a skeleton root.
    @discardableResult
    public mutating func populate(_ root: UsdSkelRoot) -> Bool {
        return self.Populate(root)
    }
}

// MARK: - UsdSkelAnimQuery Extensions

extension UsdSkelAnimQuery {
    /// Whether the query is valid.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The prim this query reads from.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether joint transforms might be time-varying.
    public var jointTransformsMightBeTimeVarying: Bool {
        return self.JointTransformsMightBeTimeVarying()
    }

    /// Whether blend shape weights might be time-varying.
    public var blendShapeWeightsMightBeTimeVarying: Bool {
        return self.BlendShapeWeightsMightBeTimeVarying()
    }
}

// MARK: - UsdSkelSkeletonQuery Extensions

extension UsdSkelSkeletonQuery {
    /// Whether the query is valid.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// Whether the skeleton has a bind pose.
    public var hasBindPose: Bool {
        return self.HasBindPose()
    }

    /// Whether the skeleton has a rest pose.
    public var hasRestPose: Bool {
        return self.HasRestPose()
    }

    /// The prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// The skeleton.
    public var skeleton: UsdSkelSkeleton {
        return self.GetSkeleton()
    }

    /// The animation query.
    public var animQuery: UsdSkelAnimQuery {
        return self.GetAnimQuery()
    }

    /// The topology.
    public var topology: UsdSkelTopology {
        return self.GetTopology()
    }
}

// MARK: - UsdSkelSkinningQuery Extensions

extension UsdSkelSkinningQuery {
    /// Whether the query is valid.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The prim.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// Whether there are blend shapes.
    public var hasBlendShapes: Bool {
        return self.HasBlendShapes()
    }

    /// Whether there are joint influences.
    public var hasJointInfluences: Bool {
        return self.HasJointInfluences()
    }

    /// The number of influences per component.
    public var numInfluencesPerComponent: Int {
        return Int(self.GetNumInfluencesPerComponent())
    }

    /// The interpolation mode.
    public var interpolation: TfToken {
        return self.GetInterpolation()
    }

    /// Whether the geometry is rigidly deformed.
    public var isRigidlyDeformed: Bool {
        return self.IsRigidlyDeformed()
    }

    /// The skinning method.
    public var skinningMethod: TfToken {
        return self.GetSkinningMethod()
    }
}

// MARK: - UsdSkel Tokens

/// Common tokens used in UsdSkel.
public enum UsdSkelTokens {
    /// The "Skeleton" type name.
    public static var skeleton: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.skeleton()
    }

    /// The "SkelRoot" type name.
    public static var skelRoot: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.skelRoot()
    }

    /// The "SkelAnimation" type name.
    public static var skelAnimation: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.skelAnimation()
    }

    /// The "BlendShape" type name.
    public static var blendShape: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.blendShape()
    }

    /// The "classicLinear" skinning method.
    public static var classicLinear: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.classicLinear()
    }

    /// The "dualQuaternion" skinning method.
    public static var dualQuaternion: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.dualQuaternion()
    }

    /// The "constant" interpolation.
    public static var constant: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.constant()
    }

    /// The "vertex" interpolation.
    public static var vertex: TfToken {
        return cxxfacade.usdSkel.UsdSkelTokens.vertex()
    }
}
