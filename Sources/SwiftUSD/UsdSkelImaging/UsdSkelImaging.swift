// UsdSkelImaging.swift - Swift wrapper for UsdSkelImaging module
// Provides skeleton visualization and skinning adapter support for Hydra

import USDCxx

// MARK: - Type Aliases

/// Skeleton imaging adapter for rendering skeleton bone meshes
public typealias UsdSkelImagingSkeletonAdapter = swiftusd.UsdSkelImagingSkeletonAdapterRef

/// SkelRoot imaging adapter for managing skeleton scopes
public typealias UsdSkelImagingSkelRootAdapter = swiftusd.UsdSkelImagingSkelRootAdapterRef

/// Bone mesh topology description
public typealias UsdSkelImagingBoneTopology = swiftusd.UsdSkelImagingBoneTopology

/// Skinning computation parameters
public typealias UsdSkelImagingSkinningParams = swiftusd.UsdSkelImagingSkinningParams

// MARK: - UsdSkelImagingSkeletonAdapter Extensions

extension swiftusd.UsdSkelImagingSkeletonAdapterRef {
    /// Create a new skeleton adapter
    public static func create() -> swiftusd.UsdSkelImagingSkeletonAdapterRef? {
        return swiftusd.UsdSkelImagingSkeletonAdapterRef.Create()
    }

    /// Compute bone mesh points for skeleton visualization
    /// - Parameters:
    ///   - restTransforms: Array of 4x4 rest pose matrices (flattened, 16 doubles per matrix)
    ///   - boneWidth: Width of bone visualization (default: 0.1)
    /// - Returns: Array of 3D points (flattened, 3 floats per point)
    public func computeBonePoints(restTransforms: [Double], boneWidth: Float = 0.1) -> [Float] {
        let jointCount = restTransforms.count / 16
        guard jointCount > 0 else { return [] }

        let topology = ComputeBoneTopology(jointCount)
        guard topology.IsValid() else { return [] }

        var points = [Float](repeating: 0, count: Int(topology.pointCount) * 3)
        let count = restTransforms.withUnsafeBufferPointer { transforms in
            points.withUnsafeMutableBufferPointer { buffer in
                ComputeBonePoints(
                    transforms.baseAddress,
                    jointCount,
                    boneWidth,
                    buffer.baseAddress,
                    buffer.count / 3
                )
            }
        }
        if count > 0 {
            return Array(points.prefix(Int(count) * 3))
        }
        return []
    }
}

// MARK: - UsdSkelImagingSkelRootAdapter Extensions

extension swiftusd.UsdSkelImagingSkelRootAdapterRef {
    /// Create a new SkelRoot adapter
    public static func create() -> swiftusd.UsdSkelImagingSkelRootAdapterRef? {
        return swiftusd.UsdSkelImagingSkelRootAdapterRef.Create()
    }
}

// MARK: - UsdSkelImagingBoneTopology Extensions

extension swiftusd.UsdSkelImagingBoneTopology {
    /// Check if the topology is valid
    public var isValid: Bool {
        return IsValid()
    }

    /// Create a bone topology from joint count
    public static func fromJointCount(_ jointCount: Int) -> swiftusd.UsdSkelImagingBoneTopology {
        return swiftusd.UsdSkelImagingComputeBoneTopology(jointCount)
    }
}

// MARK: - UsdSkelImagingSkinningParams Extensions

extension swiftusd.UsdSkelImagingSkinningParams {
    /// Create skinning params with default values
    public static func defaultParams() -> swiftusd.UsdSkelImagingSkinningParams {
        return swiftusd.UsdSkelImagingSkinningParams()
    }

    /// Create skinning params with custom settings
    public static func custom(
        skinningMethod: swiftusd.SkelSkinningMethod = swiftusd.SkelSkinningMethod(),
        numInfluencesPerComponent: Int32 = 4,
        useGpuSkinning: Bool = true,
        blendShapesEnabled: Bool = false
    ) -> swiftusd.UsdSkelImagingSkinningParams {
        var params = swiftusd.UsdSkelImagingSkinningParams()
        params.skinningMethod = skinningMethod
        params.numInfluencesPerComponent = numInfluencesPerComponent
        params.useGpuSkinning = useGpuSkinning
        params.blendShapesEnabled = blendShapesEnabled
        return params
    }
}

// MARK: - Token Helpers

/// UsdSkelImaging tokens namespace
public enum UsdSkelImagingTokens {
    /// Prim type tokens
    public static var skeleton: swiftusd.Token { swiftusd.UsdSkelImagingTokens.Skeleton() }
    public static var skelAnimation: swiftusd.Token { swiftusd.UsdSkelImagingTokens.SkelAnimation() }
    public static var skelBlendShape: swiftusd.Token { swiftusd.UsdSkelImagingTokens.SkelBlendShape() }

    /// Computation tokens
    public static var skinningInputAggregatorComputation: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.SkinningInputAggregatorComputation()
    }
    public static var skinningComputation: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.SkinningComputation()
    }

    /// Skinning input tokens
    public static var blendShapeWeights: swiftusd.Token { swiftusd.UsdSkelImagingTokens.BlendShapeWeights() }
    public static var skinningXforms: swiftusd.Token { swiftusd.UsdSkelImagingTokens.SkinningXforms() }
    public static var skinningScaleXforms: swiftusd.Token { swiftusd.UsdSkelImagingTokens.SkinningScaleXforms() }
    public static var skinningDualQuats: swiftusd.Token { swiftusd.UsdSkelImagingTokens.SkinningDualQuats() }
    public static var skelLocalToCommonSpace: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.SkelLocalToCommonSpace()
    }
    public static var commonSpaceToPrimLocal: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.CommonSpaceToPrimLocal()
    }

    /// Skinning output tokens
    public static var skinnedPoints: swiftusd.Token { swiftusd.UsdSkelImagingTokens.SkinnedPoints() }

    /// Primvar tokens
    public static var primvarJointIndices: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.PrimvarJointIndices()
    }
    public static var primvarJointWeights: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.PrimvarJointWeights()
    }
    public static var primvarSkinningMethod: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.PrimvarSkinningMethod()
    }
    public static var primvarGeomBindTransform: swiftusd.Token {
        swiftusd.UsdSkelImagingTokens.PrimvarGeomBindTransform()
    }

    /// Skinning method tokens
    public static var classicLinear: swiftusd.Token { swiftusd.UsdSkelImagingTokens.ClassicLinear() }
    public static var dualQuaternion: swiftusd.Token { swiftusd.UsdSkelImagingTokens.DualQuaternion() }
}

// MARK: - Utility Functions

/// Get the default bone width for skeleton visualization
public func usdSkelImagingGetDefaultBoneWidth() -> Float {
    return swiftusd.UsdSkelImagingGetDefaultBoneWidth()
}

/// Check if a skinning method token is valid
public func usdSkelImagingIsValidSkinningMethod(_ method: swiftusd.Token) -> Bool {
    return swiftusd.UsdSkelImagingIsValidSkinningMethod(method)
}

/// Get the skinning method token for a SkelSkinningMethod value
public func usdSkelImagingGetSkinningMethodToken(_ method: swiftusd.SkelSkinningMethod) -> swiftusd.Token {
    return swiftusd.UsdSkelImagingGetSkinningMethodToken(method)
}

/// Parse a skinning method token to a SkelSkinningMethod value
public func usdSkelImagingParseSkinningMethod(_ token: swiftusd.Token) -> swiftusd.SkelSkinningMethod {
    return swiftusd.UsdSkelImagingParseSkinningMethod(token)
}
