// UsdSkelImagingTests.swift - Tests for UsdSkelImaging module

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdSkelImagingTests: XCTestCase {

    // MARK: - SkeletonAdapter Tests

    func testSkeletonAdapterCreation() {
        let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create()
        XCTAssertNotNil(adapter, "Should be able to create skeleton adapter")
        XCTAssertTrue(adapter?.IsValid() ?? false, "Adapter should be valid")
    }

    func testSkeletonAdapterIsInstanceable() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }
        XCTAssertTrue(adapter.IsInstanceable(), "Skeleton adapter should support instancing")
    }

    func testSkeletonAdapterSupportsSkinning() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }
        XCTAssertTrue(adapter.SupportsSkinning(), "Skeleton adapter should support skinning")
    }

    func testSkeletonAdapterPrimTokens() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }

        var tokens = [swiftusd.Token](repeating: swiftusd.Token(), count: 4)
        let count = tokens.withUnsafeMutableBufferPointer { buffer in
            adapter.GetPrimTokens(buffer.baseAddress, buffer.count)
        }

        XCTAssertEqual(count, 1, "Should return 1 prim token")
        XCTAssertEqual(String(cString: tokens[0].GetText()), "skeleton", "Token should be 'skeleton'")
    }

    // MARK: - Bone Topology Tests

    func testBoneTopologyComputation() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }

        // Test with 3 joints (should give 2 bones)
        let topology = adapter.ComputeBoneTopology(3)
        XCTAssertTrue(topology.IsValid(), "Topology should be valid")
        XCTAssertGreaterThan(topology.faceCount, 0, "Should have faces")
        XCTAssertGreaterThan(topology.pointCount, 0, "Should have points")
        XCTAssertGreaterThan(topology.faceVertexCount, 0, "Should have face vertices")
    }

    func testBoneTopologyZeroJoints() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }

        let topology = adapter.ComputeBoneTopology(0)
        XCTAssertFalse(topology.IsValid(), "Topology with 0 joints should be invalid")
    }

    func testBoneTopologyFromJointCount() {
        let topology = swiftusd.UsdSkelImagingBoneTopology.fromJointCount(5)
        XCTAssertTrue(topology.isValid, "Topology should be valid for 5 joints")

        // 5 joints = 4 bones, each bone is an octahedron with 8 faces
        // 4 * 8 = 32 faces
        XCTAssertEqual(topology.faceCount, 32, "Should have 32 faces for 4 bones")
    }

    // MARK: - Bone Points Tests

    func testBonePointsComputation() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }

        // Create identity rest transforms for 3 joints
        var restTransforms: [Double] = []
        for i in 0..<3 {
            // Identity matrix with translation along Y
            restTransforms.append(contentsOf: [
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, Double(i) * 0.5, 0, 1
            ])
        }

        let points = adapter.computeBonePoints(restTransforms: restTransforms, boneWidth: 0.1)
        XCTAssertGreaterThan(points.count, 0, "Should compute some bone points")
        XCTAssertEqual(points.count % 3, 0, "Points should be in groups of 3 (x, y, z)")
    }

    func testBonePointsEmptyTransforms() {
        guard let adapter = swiftusd.UsdSkelImagingSkeletonAdapterRef.create() else {
            XCTFail("Failed to create skeleton adapter")
            return
        }

        let points = adapter.computeBonePoints(restTransforms: [], boneWidth: 0.1)
        XCTAssertTrue(points.isEmpty, "Should return empty points for empty transforms")
    }

    // MARK: - SkelRootAdapter Tests

    func testSkelRootAdapterCreation() {
        let adapter = swiftusd.UsdSkelImagingSkelRootAdapterRef.create()
        XCTAssertNotNil(adapter, "Should be able to create SkelRoot adapter")
        XCTAssertTrue(adapter?.IsValid() ?? false, "Adapter should be valid")
    }

    func testSkelRootAdapterPrimToken() {
        guard let adapter = swiftusd.UsdSkelImagingSkelRootAdapterRef.create() else {
            XCTFail("Failed to create SkelRoot adapter")
            return
        }

        let token = adapter.GetPrimToken()
        XCTAssertEqual(String(cString: token.GetText()), "SkelRoot", "Token should be 'SkelRoot'")
    }

    func testSkelRootContainsPath() {
        guard let adapter = swiftusd.UsdSkelImagingSkelRootAdapterRef.create() else {
            XCTFail("Failed to create SkelRoot adapter")
            return
        }

        let rootPath = swiftusd.Path("/World/Character")
        let childPath = swiftusd.Path("/World/Character/Skeleton")
        let siblingPath = swiftusd.Path("/World/OtherCharacter")

        XCTAssertTrue(adapter.ContainsPath(rootPath, childPath),
                      "Child path should be contained in root")
        XCTAssertFalse(adapter.ContainsPath(rootPath, siblingPath),
                       "Sibling path should not be contained in root")
    }

    // MARK: - Skinning Params Tests

    func testSkinningParamsDefault() {
        let params = swiftusd.UsdSkelImagingSkinningParams.defaultParams()
        XCTAssertEqual(params.numInfluencesPerComponent, 4, "Default influences should be 4")
        XCTAssertTrue(params.useGpuSkinning, "GPU skinning should be enabled by default")
        XCTAssertFalse(params.blendShapesEnabled, "Blend shapes should be disabled by default")
    }

    func testSkinningParamsCustom() {
        let method = swiftusd.SkelSkinningMethod(swiftusd.SkelSkinningMethod.Value(rawValue: 1))
        let params = swiftusd.UsdSkelImagingSkinningParams.custom(
            skinningMethod: method,
            numInfluencesPerComponent: 8,
            useGpuSkinning: false,
            blendShapesEnabled: true
        )

        XCTAssertEqual(params.skinningMethod.value.rawValue, 1,
                       "Skinning method should be dual quaternion")
        XCTAssertEqual(params.numInfluencesPerComponent, 8, "Should have 8 influences")
        XCTAssertFalse(params.useGpuSkinning, "GPU skinning should be disabled")
        XCTAssertTrue(params.blendShapesEnabled, "Blend shapes should be enabled")
    }

    func testSkinningParamsEquality() {
        let params1 = swiftusd.UsdSkelImagingSkinningParams.defaultParams()
        let params2 = swiftusd.UsdSkelImagingSkinningParams.defaultParams()
        let params3 = swiftusd.UsdSkelImagingSkinningParams.custom(numInfluencesPerComponent: 8)

        // Test equality using operator==
        XCTAssertTrue(params1 == params2, "Default params should be equal")
        XCTAssertTrue(params1 != params3, "Different params should not be equal")
    }

    // MARK: - Token Tests

    func testUsdSkelImagingTokens() {
        XCTAssertEqual(String(cString: UsdSkelImagingTokens.skeleton.GetText()), "skeleton")
        XCTAssertEqual(String(cString: UsdSkelImagingTokens.skelAnimation.GetText()), "skelAnimation")
        XCTAssertEqual(String(cString: UsdSkelImagingTokens.skelBlendShape.GetText()), "skelBlendShape")
        XCTAssertEqual(String(cString: UsdSkelImagingTokens.classicLinear.GetText()), "classicLinear")
        XCTAssertEqual(String(cString: UsdSkelImagingTokens.dualQuaternion.GetText()), "dualQuaternion")
        XCTAssertEqual(String(cString: UsdSkelImagingTokens.skinnedPoints.GetText()), "skinnedPoints")
    }

    func testSkinningMethodTokens() {
        let linearToken = UsdSkelImagingTokens.classicLinear
        let dqToken = UsdSkelImagingTokens.dualQuaternion

        XCTAssertTrue(usdSkelImagingIsValidSkinningMethod(linearToken),
                      "classicLinear should be valid skinning method")
        XCTAssertTrue(usdSkelImagingIsValidSkinningMethod(dqToken),
                      "dualQuaternion should be valid skinning method")

        let invalidToken = swiftusd.Token("invalid")
        XCTAssertFalse(usdSkelImagingIsValidSkinningMethod(invalidToken),
                       "Invalid token should not be valid skinning method")
    }

    func testSkinningMethodConversion() {
        let linearMethod = swiftusd.SkelSkinningMethod(swiftusd.SkelSkinningMethod.Value(rawValue: 0))
        let dqMethod = swiftusd.SkelSkinningMethod(swiftusd.SkelSkinningMethod.Value(rawValue: 1))

        let linearToken = usdSkelImagingGetSkinningMethodToken(linearMethod)
        let dqToken = usdSkelImagingGetSkinningMethodToken(dqMethod)

        XCTAssertEqual(String(cString: linearToken.GetText()), "classicLinear")
        XCTAssertEqual(String(cString: dqToken.GetText()), "dualQuaternion")

        // Round-trip
        let parsedLinear = usdSkelImagingParseSkinningMethod(linearToken)
        let parsedDq = usdSkelImagingParseSkinningMethod(dqToken)

        XCTAssertEqual(parsedLinear.value.rawValue, 0)
        XCTAssertEqual(parsedDq.value.rawValue, 1)
    }

    // MARK: - Utility Function Tests

    func testDefaultBoneWidth() {
        let width = usdSkelImagingGetDefaultBoneWidth()
        XCTAssertEqual(width, 0.1, accuracy: 0.001, "Default bone width should be 0.1")
    }

    // MARK: - Reference Counting Tests

    func testSkeletonAdapterRetainRelease() {
        var adapter: swiftusd.UsdSkelImagingSkeletonAdapterRef? = swiftusd.UsdSkelImagingSkeletonAdapterRef.create()
        XCTAssertNotNil(adapter)

        // Retain
        adapter?.retain()

        // Release twice (once for retain, once for original)
        adapter?.release()
        adapter?.release()

        // Note: We can't easily test that the object was deallocated,
        // but at least we verify the methods don't crash
        adapter = nil
    }

    func testSkelRootAdapterRetainRelease() {
        var adapter: swiftusd.UsdSkelImagingSkelRootAdapterRef? = swiftusd.UsdSkelImagingSkelRootAdapterRef.create()
        XCTAssertNotNil(adapter)

        adapter?.retain()
        adapter?.release()
        adapter?.release()

        adapter = nil
    }
}
