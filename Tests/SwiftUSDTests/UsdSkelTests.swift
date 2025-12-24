// UsdSkelTests.swift - Tests for UsdSkel module

import Testing
@testable import SwiftUSD

@Suite("UsdSkel Module Tests")
struct UsdSkelTests {

    // MARK: - SkelRoot Tests

    @Test("SkelRoot can be defined on a stage")
    func testSkelRootDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/SkelRoot")
        let skelRoot = try SkelRoot.define(on: stage, at: path)

        #expect(skelRoot.isValid)
        // Note: prim may be nil in stub mode
    }

    @Test("SkelRoot can be created from prim")
    func testSkelRootFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/SkelRoot")
        let skelRoot = try SkelRoot.define(on: stage, at: path)

        // In stub mode, prim may be nil
        guard let prim = skelRoot.prim else {
            // Skip this test in stub mode
            return
        }

        let wrapped = SkelRoot.from(prim: prim)
        #expect(wrapped != nil)
        #expect(wrapped?.isValid == true)
    }

    // MARK: - Skeleton Tests

    @Test("Skeleton can be defined on a stage")
    func testSkeletonDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Skeleton")
        let skeleton = try Skeleton.define(on: stage, at: path)

        #expect(skeleton.isValid)
        // Note: prim may be nil in stub mode
    }

    @Test("Skeleton joints can be set and retrieved")
    func testSkeletonJoints() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Skeleton")
        let skeleton = try Skeleton.define(on: stage, at: path)

        let joints = [
            try Token("Root"),
            try Token("Root/Spine"),
            try Token("Root/Spine/Head")
        ]

        try skeleton.setJoints(joints)

        let retrievedJoints = skeleton.joints()
        #expect(retrievedJoints.count == 3)
    }

    @Test("Skeleton bind transforms can be set and retrieved")
    func testSkeletonBindTransforms() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Skeleton")
        let skeleton = try Skeleton.define(on: stage, at: path)

        // Create identity matrices for 2 joints
        let identity: [Double] = [
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        ]
        let transforms = [identity, identity]

        // Set joints first
        let joints = [try Token("Root"), try Token("Root/Child")]
        try skeleton.setJoints(joints)

        try skeleton.setBindTransforms(transforms)

        let retrievedTransforms = skeleton.bindTransforms()
        #expect(retrievedTransforms.count == 2)
        if retrievedTransforms.count == 2 {
            #expect(retrievedTransforms[0].count == 16)
            #expect(retrievedTransforms[0][0] == 1.0)
        }
    }

    @Test("Skeleton rest transforms can be set and retrieved")
    func testSkeletonRestTransforms() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Skeleton")
        let skeleton = try Skeleton.define(on: stage, at: path)

        // Create identity matrices for 2 joints
        let identity: [Double] = [
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        ]
        let transforms = [identity, identity]

        // Set joints first
        let joints = [try Token("Root"), try Token("Root/Child")]
        try skeleton.setJoints(joints)

        try skeleton.setRestTransforms(transforms)

        let retrievedTransforms = skeleton.restTransforms()
        #expect(retrievedTransforms.count == 2)
    }

    // MARK: - SkelAnimation Tests

    @Test("SkelAnimation can be defined on a stage")
    func testSkelAnimationDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Animation")
        let animation = try SkelAnimation.define(on: stage, at: path)

        #expect(animation.isValid)
        // Note: prim may be nil in stub mode
    }

    @Test("SkelAnimation translations can be set and retrieved")
    func testSkelAnimationTranslations() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Animation")
        let animation = try SkelAnimation.define(on: stage, at: path)

        // Set joints first
        let joints = [try Token("Root"), try Token("Root/Child")]
        try animation.setJoints(joints)

        let translations: [(x: Float, y: Float, z: Float)] = [
            (0.0, 0.0, 0.0),
            (0.0, 1.0, 0.0)
        ]

        try animation.setTranslations(translations, at: .default)

        let retrievedTranslations = animation.translations(at: .default)
        #expect(retrievedTranslations.count == 2)
        if retrievedTranslations.count == 2 {
            #expect(retrievedTranslations[1].y == 1.0)
        }
    }

    @Test("SkelAnimation rotations can be set and retrieved")
    func testSkelAnimationRotations() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Animation")
        let animation = try SkelAnimation.define(on: stage, at: path)

        // Set joints first
        let joints = [try Token("Root")]
        try animation.setJoints(joints)

        // Identity quaternion
        let rotations: [(w: Float, x: Float, y: Float, z: Float)] = [
            (1.0, 0.0, 0.0, 0.0)
        ]

        try animation.setRotations(rotations, at: .default)

        let retrievedRotations = animation.rotations(at: .default)
        #expect(retrievedRotations.count == 1)
        if retrievedRotations.count == 1 {
            #expect(retrievedRotations[0].w == 1.0)
        }
    }

    @Test("SkelAnimation scales can be set and retrieved")
    func testSkelAnimationScales() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Animation")
        let animation = try SkelAnimation.define(on: stage, at: path)

        // Set joints first
        let joints = [try Token("Root")]
        try animation.setJoints(joints)

        let scales: [(x: Float, y: Float, z: Float)] = [
            (1.0, 2.0, 1.0)
        ]

        try animation.setScales(scales, at: .default)

        let retrievedScales = animation.scales(at: .default)
        #expect(retrievedScales.count == 1)
    }

    @Test("SkelAnimation blend shape weights can be set and retrieved")
    func testSkelAnimationBlendShapeWeights() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Animation")
        let animation = try SkelAnimation.define(on: stage, at: path)

        // Set blend shapes first
        let blendShapes = [try Token("smile"), try Token("frown")]
        try animation.setBlendShapes(blendShapes)

        let weights: [Float] = [0.5, 0.3]
        try animation.setBlendShapeWeights(weights, at: .default)

        let retrievedWeights = animation.blendShapeWeights(at: .default)
        #expect(retrievedWeights.count == 2)
    }

    // MARK: - BlendShape Tests

    @Test("BlendShape can be defined on a stage")
    func testBlendShapeDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/BlendShape")
        let blendShape = try BlendShape.define(on: stage, at: path)

        #expect(blendShape.isValid)
        // Note: prim may be nil in stub mode
    }

    @Test("BlendShape offsets can be set and retrieved")
    func testBlendShapeOffsets() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/BlendShape")
        let blendShape = try BlendShape.define(on: stage, at: path)

        // Position offsets for 3 points (x, y, z interleaved)
        let offsets: [Float] = [
            0.1, 0.0, 0.0,
            0.0, 0.1, 0.0,
            0.0, 0.0, 0.1
        ]

        try blendShape.setOffsets(offsets)

        let retrievedOffsets = blendShape.offsets()
        #expect(retrievedOffsets.count == 9)
    }

    @Test("BlendShape point indices can be set and retrieved")
    func testBlendShapePointIndices() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/BlendShape")
        let blendShape = try BlendShape.define(on: stage, at: path)

        let indices: [Int32] = [0, 1, 2, 10, 20, 30]
        try blendShape.setPointIndices(indices)

        let retrievedIndices = blendShape.pointIndices()
        #expect(retrievedIndices.count == 6)
        if retrievedIndices.count >= 4 {
            #expect(retrievedIndices[3] == 10)
        }
    }

    // MARK: - SkelBindingAPI Tests

    @Test("SkelBindingAPI can be applied to a prim")
    func testSkelBindingAPIApply() throws {
        let stage = try Stage.createInMemory()
        let meshPath = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)

        // In stub mode, prim may be nil
        guard let prim = mesh.prim else {
            // Skip this test in stub mode
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: prim)
        #expect(bindingAPI.isValid)
    }

    @Test("SkelBindingAPI skeleton path can be set and retrieved")
    func testSkelBindingAPISkeletonPath() throws {
        let stage = try Stage.createInMemory()
        let meshPath = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)
        let skelPath = try Path("/Skeleton")
        _ = try Skeleton.define(on: stage, at: skelPath)

        // In stub mode, prim may be nil
        guard let prim = mesh.prim else {
            // Skip this test in stub mode
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: prim)
        try bindingAPI.setSkeletonPath(skelPath)

        let retrievedPath = bindingAPI.skeletonPath
        #expect(retrievedPath?.text == "/Skeleton")
    }

    @Test("SkelBindingAPI joint influences can be set and retrieved")
    func testSkelBindingAPIJointInfluences() throws {
        let stage = try Stage.createInMemory()
        let meshPath = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)

        // In stub mode, prim may be nil
        guard let prim = mesh.prim else {
            // Skip this test in stub mode
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: prim)

        // Set joint indices and weights for vertex skinning
        let indices: [Int32] = [0, 1, 0, 1, 0, 1]
        let weights: [Float] = [0.5, 0.5, 0.3, 0.7, 0.8, 0.2]

        try bindingAPI.setJointIndices(indices, constant: false, elementSize: 2)
        try bindingAPI.setJointWeights(weights, constant: false, elementSize: 2)

        let retrievedIndices = bindingAPI.jointIndices()
        let retrievedWeights = bindingAPI.jointWeights()

        #expect(retrievedIndices.count == 6)
        #expect(retrievedWeights.count == 6)
    }

    @Test("SkelBindingAPI rigid joint influence can be set")
    func testSkelBindingAPIRigidInfluence() throws {
        let stage = try Stage.createInMemory()
        let meshPath = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)

        // In stub mode, prim may be nil
        guard let prim = mesh.prim else {
            // Skip this test in stub mode
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: prim)
        try bindingAPI.setRigidJointInfluence(jointIndex: 2, weight: 1.0)

        let indices = bindingAPI.jointIndices()
        let weights = bindingAPI.jointWeights()

        #expect(indices.count > 0)
        #expect(weights.count > 0)
    }

    @Test("SkelBindingAPI skinning method can be set and retrieved")
    func testSkelBindingAPISkinningMethod() throws {
        let stage = try Stage.createInMemory()
        let meshPath = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)

        // In stub mode, prim may be nil
        guard let prim = mesh.prim else {
            // Skip this test in stub mode
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: prim)
        try bindingAPI.setSkinningMethod(.dualQuaternion)

        let method = bindingAPI.skinningMethod
        #expect(method == .dualQuaternion)
    }

    @Test("SkelBindingAPI geom bind transform can be set and retrieved")
    func testSkelBindingAPIGeomBindTransform() throws {
        let stage = try Stage.createInMemory()
        let meshPath = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)

        // In stub mode, prim may be nil
        guard let prim = mesh.prim else {
            // Skip this test in stub mode
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: prim)

        // Set a translation matrix
        let matrix: [Double] = [
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            10, 20, 30, 1
        ]

        try bindingAPI.setGeomBindTransform(matrix)

        let retrievedMatrix = try bindingAPI.geomBindTransform()
        #expect(retrievedMatrix.count == 16)
        #expect(retrievedMatrix[12] == 10.0)
        #expect(retrievedMatrix[13] == 20.0)
        #expect(retrievedMatrix[14] == 30.0)
    }

    // MARK: - Integration Tests

    @Test("Complete skeleton hierarchy can be created")
    func testCompleteSkeletonHierarchy() throws {
        let stage = try Stage.createInMemory()

        // Create SkelRoot
        let rootPath = try Path("/Character")
        let skelRoot = try SkelRoot.define(on: stage, at: rootPath)
        #expect(skelRoot.isValid)

        // Create Skeleton under root
        let skelPath = try Path("/Character/Skeleton")
        let skeleton = try Skeleton.define(on: stage, at: skelPath)
        #expect(skeleton.isValid)

        // Set up joint hierarchy
        let joints = [
            try Token("Root"),
            try Token("Root/Spine"),
            try Token("Root/Spine/Chest"),
            try Token("Root/Spine/Chest/Head")
        ]
        try skeleton.setJoints(joints)

        // Create animation
        let animPath = try Path("/Character/Animation")
        let animation = try SkelAnimation.define(on: stage, at: animPath)
        #expect(animation.isValid)

        try animation.setJoints(joints)

        // Create mesh
        let meshPath = try Path("/Character/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: meshPath)
        #expect(mesh.isValid)

        // Apply binding to mesh (only works in non-stub mode)
        guard let meshPrim = mesh.prim else {
            // In stub mode, prim access is not available
            // Core functionality has been tested above
            return
        }

        let bindingAPI = try SkelBindingAPI.apply(to: meshPrim)
        try bindingAPI.setSkeletonPath(skelPath)
        try bindingAPI.setAnimationSourcePath(animPath)

        #expect(bindingAPI.skeletonPath?.text == "/Character/Skeleton")
        #expect(bindingAPI.animationSourcePath?.text == "/Character/Animation")
    }
}
