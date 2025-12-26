// UsdSkelTests.swift - Tests for UsdSkel skeletal animation support

import XCTest
@testable import SwiftUSD
@testable import CxxFacade

final class UsdSkelTests: XCTestCase {

    // MARK: - Skeleton Tests

    func testSkeletonCreation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/TestSkeleton")

        var skeleton = cxxfacade.usdSkel.UsdSkelSkeleton.Define(&stage, path)

        XCTAssertTrue(skeleton.GetPrim().IsValid())
        XCTAssertEqual(String(skeleton.GetPrim().GetPath().GetString()), "/TestSkeleton")

        // Test attribute creation
        let jointsAttr = skeleton.CreateJointsAttr()
        XCTAssertTrue(jointsAttr.IsValid())

        let bindAttr = skeleton.CreateBindTransformsAttr()
        XCTAssertTrue(bindAttr.IsValid())

        let restAttr = skeleton.CreateRestTransformsAttr()
        XCTAssertTrue(restAttr.IsValid())
    }

    func testSkeletonGet() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/MySkeleton")

        _ = cxxfacade.usdSkel.UsdSkelSkeleton.Define(&stage, path)

        let retrieved = cxxfacade.usdSkel.UsdSkelSkeleton.Get(stage, path)
        XCTAssertTrue(retrieved.GetPrim().IsValid())
    }

    // MARK: - SkelRoot Tests

    func testSkelRootCreation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/TestRoot")

        let root = cxxfacade.usdSkel.UsdSkelRoot.Define(&stage, path)

        XCTAssertTrue(root.GetPrim().IsValid())
        XCTAssertEqual(String(root.GetPrim().GetPath().GetString()), "/TestRoot")
    }

    func testSkelRootFind() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create a hierarchy: SkelRoot -> Skeleton
        let rootPath = cxxfacade.sdf.SdfPath("/MyRoot")
        _ = cxxfacade.usdSkel.UsdSkelRoot.Define(&stage, rootPath)

        let skelPath = cxxfacade.sdf.SdfPath("/MyRoot/MySkeleton")
        let skeleton = cxxfacade.usdSkel.UsdSkelSkeleton.Define(&stage, skelPath)

        // Find root from skeleton prim
        let foundRoot = cxxfacade.usdSkel.UsdSkelRoot.Find(skeleton.GetPrim())
        XCTAssertTrue(foundRoot.GetPrim().IsValid())
        XCTAssertEqual(String(foundRoot.GetPrim().GetPath().GetString()), "/MyRoot")
    }

    // MARK: - SkelAnimation Tests

    func testAnimationCreation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/TestAnim")

        var anim = cxxfacade.usdSkel.UsdSkelAnimation.Define(&stage, path)

        XCTAssertTrue(anim.GetPrim().IsValid())
        XCTAssertEqual(String(anim.GetPrim().GetPath().GetString()), "/TestAnim")

        // Test attribute creation
        let jointsAttr = anim.CreateJointsAttr()
        XCTAssertTrue(jointsAttr.IsValid())

        let translationsAttr = anim.CreateTranslationsAttr()
        XCTAssertTrue(translationsAttr.IsValid())

        let rotationsAttr = anim.CreateRotationsAttr()
        XCTAssertTrue(rotationsAttr.IsValid())

        let scalesAttr = anim.CreateScalesAttr()
        XCTAssertTrue(scalesAttr.IsValid())
    }

    // MARK: - SkelBindingAPI Tests

    func testBindingAPIApply() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let meshPath = cxxfacade.sdf.SdfPath("/TestMesh")

        // Create a simple prim to apply the API to
        let prim = stage.DefinePrim(meshPath, cxxfacade.tf.TfToken("Mesh"))
        XCTAssertTrue(prim.IsValid())

        // Check if we can apply
        let canApply = cxxfacade.usdSkel.UsdSkelBindingAPI.CanApply(prim, nil)
        XCTAssertTrue(canApply)

        // Apply the API
        let bindingAPI = cxxfacade.usdSkel.UsdSkelBindingAPI.Apply(prim)
        XCTAssertTrue(bindingAPI.GetPrim().IsValid())
    }

    func testBindingAPIRelationships() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let meshPath = cxxfacade.sdf.SdfPath("/TestMesh")

        let prim = stage.DefinePrim(meshPath, cxxfacade.tf.TfToken("Mesh"))
        var bindingAPI = cxxfacade.usdSkel.UsdSkelBindingAPI.Apply(prim)

        // Create relationships
        let skelRel = bindingAPI.CreateSkeletonRel()
        XCTAssertTrue(skelRel.IsValid())

        let animRel = bindingAPI.CreateAnimationSourceRel()
        XCTAssertTrue(animRel.IsValid())
    }

    func testBindingAPIRigidJointInfluence() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let meshPath = cxxfacade.sdf.SdfPath("/TestMesh")

        let prim = stage.DefinePrim(meshPath, cxxfacade.tf.TfToken("Mesh"))
        var bindingAPI = cxxfacade.usdSkel.UsdSkelBindingAPI.Apply(prim)

        // Set rigid joint influence
        let success = bindingAPI.SetRigidJointInfluence(0, 1.0)
        XCTAssertTrue(success)
    }

    // MARK: - BlendShape Tests

    func testBlendShapeCreation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/TestBlendShape")

        var blendShape = cxxfacade.usdSkel.UsdSkelBlendShape.Define(&stage, path)

        XCTAssertTrue(blendShape.GetPrim().IsValid())
        XCTAssertEqual(String(blendShape.GetPrim().GetPath().GetString()), "/TestBlendShape")

        // Test attribute creation
        let offsetsAttr = blendShape.CreateOffsetsAttr()
        XCTAssertTrue(offsetsAttr.IsValid())

        let normalOffsetsAttr = blendShape.CreateNormalOffsetsAttr()
        XCTAssertTrue(normalOffsetsAttr.IsValid())

        let pointIndicesAttr = blendShape.CreatePointIndicesAttr()
        XCTAssertTrue(pointIndicesAttr.IsValid())
    }

    // MARK: - SkelCache Tests

    func testCacheCreation() {
        let cache = cxxfacade.usdSkel.UsdSkelCache()
        XCTAssertNotNil(cache)
    }

    func testCachePopulateAndQuery() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create a skeleton hierarchy
        let rootPath = cxxfacade.sdf.SdfPath("/Root")
        let skelPath = cxxfacade.sdf.SdfPath("/Root/Skeleton")

        let root = cxxfacade.usdSkel.UsdSkelRoot.Define(&stage, rootPath)
        _ = cxxfacade.usdSkel.UsdSkelSkeleton.Define(&stage, skelPath)

        // Create and populate cache
        var cache = cxxfacade.usdSkel.UsdSkelCache()
        let success = cache.Populate(root)
        XCTAssertTrue(success)

        // Clear the cache
        cache.Clear()
    }

    // MARK: - Token Tests

    func testSkelTokens() {
        let skeletonToken = cxxfacade.usdSkel.UsdSkelTokens.skeleton()
        XCTAssertEqual(String(cString: skeletonToken.GetText()), "Skeleton")

        let skelRootToken = cxxfacade.usdSkel.UsdSkelTokens.skelRoot()
        XCTAssertEqual(String(cString: skelRootToken.GetText()), "SkelRoot")

        let constantToken = cxxfacade.usdSkel.UsdSkelTokens.constant()
        XCTAssertEqual(String(cString: constantToken.GetText()), "constant")

        let vertexToken = cxxfacade.usdSkel.UsdSkelTokens.vertex()
        XCTAssertEqual(String(cString: vertexToken.GetText()), "vertex")
    }

    // MARK: - Integration Tests

    func testSkeletonWithAnimation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create a complete skeletal setup
        let rootPath = cxxfacade.sdf.SdfPath("/CharacterRoot")
        let skelPath = cxxfacade.sdf.SdfPath("/CharacterRoot/Skeleton")
        let animPath = cxxfacade.sdf.SdfPath("/CharacterRoot/Animation")
        let meshPath = cxxfacade.sdf.SdfPath("/CharacterRoot/SkinnedMesh")

        // Define the hierarchy
        let root = cxxfacade.usdSkel.UsdSkelRoot.Define(&stage, rootPath)
        XCTAssertTrue(root.GetPrim().IsValid())

        var skeleton = cxxfacade.usdSkel.UsdSkelSkeleton.Define(&stage, skelPath)
        XCTAssertTrue(skeleton.GetPrim().IsValid())

        // Create skeleton attributes
        _ = skeleton.CreateJointsAttr()
        _ = skeleton.CreateBindTransformsAttr()
        _ = skeleton.CreateRestTransformsAttr()

        var anim = cxxfacade.usdSkel.UsdSkelAnimation.Define(&stage, animPath)
        XCTAssertTrue(anim.GetPrim().IsValid())

        // Create animation attributes
        _ = anim.CreateJointsAttr()
        _ = anim.CreateTranslationsAttr()
        _ = anim.CreateRotationsAttr()
        _ = anim.CreateScalesAttr()

        // Create mesh and bind it
        let meshPrim = stage.DefinePrim(meshPath, cxxfacade.tf.TfToken("Mesh"))
        var bindingAPI = cxxfacade.usdSkel.UsdSkelBindingAPI.Apply(meshPrim)

        _ = bindingAPI.CreateSkeletonRel()
        _ = bindingAPI.CreateAnimationSourceRel()
    }

    func testQueryValidity() {
        // Test that invalid queries are properly detected
        let animQuery = cxxfacade.usdSkel.UsdSkelAnimQuery()
        XCTAssertFalse(animQuery.IsValid())

        let skelQuery = cxxfacade.usdSkel.UsdSkelSkeletonQuery()
        XCTAssertFalse(skelQuery.IsValid())

        let skinQuery = cxxfacade.usdSkel.UsdSkelSkinningQuery()
        XCTAssertFalse(skinQuery.IsValid())
    }
}
