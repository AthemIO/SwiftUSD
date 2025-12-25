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

extension UsdSkelImagingSkeletonAdapter {
    /// Create a new skeleton adapter
    public static func create() -> UsdSkelImagingSkeletonAdapter? {
        return UsdSkelImagingSkeletonAdapterRef.Create()
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

extension UsdSkelImagingSkelRootAdapter {
    /// Create a new SkelRoot adapter
    public static func create() -> UsdSkelImagingSkelRootAdapter? {
        return UsdSkelImagingSkelRootAdapterRef.Create()
    }
}

// MARK: - UsdSkelImagingBoneTopology Extensions

extension UsdSkelImagingBoneTopology {
    /// Check if the topology is valid
    public var isValid: Bool {
        return IsValid()
    }

    /// Create a bone topology from joint count
    public static func fromJointCount(_ jointCount: Int) -> UsdSkelImagingBoneTopology {
        return swiftusd.UsdSkelImagingComputeBoneTopology(jointCount)
    }
}

// MARK: - UsdSkelImagingSkinningParams Extensions

extension UsdSkelImagingSkinningParams {
    /// Create skinning params with default values
    public static func defaultParams() -> UsdSkelImagingSkinningParams {
        return UsdSkelImagingSkinningParams()
    }

    /// Create skinning params with custom settings
    public static func custom(
        skinningMethod: SkelSkinningMethod = SkelSkinningMethod(),
        numInfluencesPerComponent: Int32 = 4,
        useGpuSkinning: Bool = true,
        blendShapesEnabled: Bool = false
    ) -> UsdSkelImagingSkinningParams {
        var params = UsdSkelImagingSkinningParams()
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
    public static var skeleton: Token { swiftusd.UsdSkelImagingTokens.Skeleton() }
    public static var skelAnimation: Token { swiftusd.UsdSkelImagingTokens.SkelAnimation() }
    public static var skelBlendShape: Token { swiftusd.UsdSkelImagingTokens.SkelBlendShape() }

    /// Computation tokens
    public static var skinningInputAggregatorComputation: Token {
        swiftusd.UsdSkelImagingTokens.SkinningInputAggregatorComputation()
    }
    public static var skinningComputation: Token {
        swiftusd.UsdSkelImagingTokens.SkinningComputation()
    }

    /// Skinning input tokens
    public static var blendShapeWeights: Token { swiftusd.UsdSkelImagingTokens.BlendShapeWeights() }
    public static var skinningXforms: Token { swiftusd.UsdSkelImagingTokens.SkinningXforms() }
    public static var skinningScaleXforms: Token { swiftusd.UsdSkelImagingTokens.SkinningScaleXforms() }
    public static var skinningDualQuats: Token { swiftusd.UsdSkelImagingTokens.SkinningDualQuats() }
    public static var skelLocalToCommonSpace: Token {
        swiftusd.UsdSkelImagingTokens.SkelLocalToCommonSpace()
    }
    public static var commonSpaceToPrimLocal: Token {
        swiftusd.UsdSkelImagingTokens.CommonSpaceToPrimLocal()
    }

    /// Skinning output tokens
    public static var skinnedPoints: Token { swiftusd.UsdSkelImagingTokens.SkinnedPoints() }

    /// Primvar tokens
    public static var primvarJointIndices: Token {
        swiftusd.UsdSkelImagingTokens.PrimvarJointIndices()
    }
    public static var primvarJointWeights: Token {
        swiftusd.UsdSkelImagingTokens.PrimvarJointWeights()
    }
    public static var primvarSkinningMethod: Token {
        swiftusd.UsdSkelImagingTokens.PrimvarSkinningMethod()
    }
    public static var primvarGeomBindTransform: Token {
        swiftusd.UsdSkelImagingTokens.PrimvarGeomBindTransform()
    }

    /// Skinning method tokens
    public static var classicLinear: Token { swiftusd.UsdSkelImagingTokens.ClassicLinear() }
    public static var dualQuaternion: Token { swiftusd.UsdSkelImagingTokens.DualQuaternion() }
}

// MARK: - Utility Functions

/// Get the default bone width for skeleton visualization
public func usdSkelImagingGetDefaultBoneWidth() -> Float {
    return swiftusd.UsdSkelImagingGetDefaultBoneWidth()
}

/// Check if a skinning method token is valid
public func usdSkelImagingIsValidSkinningMethod(_ method: Token) -> Bool {
    return swiftusd.UsdSkelImagingIsValidSkinningMethod(method)
}

/// Get the skinning method token for a SkelSkinningMethod value
public func usdSkelImagingGetSkinningMethodToken(_ method: SkelSkinningMethod) -> Token {
    return swiftusd.UsdSkelImagingGetSkinningMethodToken(method)
}

/// Parse a skinning method token to a SkelSkinningMethod value
public func usdSkelImagingParseSkinningMethod(_ token: Token) -> SkelSkinningMethod {
    return swiftusd.UsdSkelImagingParseSkinningMethod(token)
}
