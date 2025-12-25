// HdStCxxTests.swift - Tests for Storm Renderer Swift bindings

import XCTest
import USDCxx
@testable import SwiftUSD

final class HdStCxxTests: XCTestCase {

    // MARK: - HdStRenderDelegate Tests

    func testRenderDelegateCreation() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.Create() else {
            XCTFail("Failed to create HdStRenderDelegateRef")
            return
        }

        XCTAssertNotNil(delegate)
    }

    func testRenderDelegateConvenienceCreation() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.create() else {
            XCTFail("Failed to create render delegate with convenience method")
            return
        }

        XCTAssertNotNil(delegate)
    }

    func testRenderDelegateIsSupported() {
        // In standalone mode, IsSupported should return true
        let isSupported = swiftusd.HdStRenderDelegateRef.IsSupported()
        XCTAssertTrue(isSupported)
    }

    func testRenderDelegateSupportedRprimTypes() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.Create() else {
            XCTFail("Failed to create render delegate")
            return
        }

        let rprimTypes = delegate.supportedRprimTypes
        XCTAssertFalse(rprimTypes.isEmpty)

        // Storm should support mesh
        let meshToken = swiftusd.Token("mesh")
        let hasMesh = rprimTypes.contains { String(cString: $0.GetText()) == "mesh" }
        XCTAssertTrue(hasMesh, "Storm should support mesh rprim type")
    }

    func testRenderDelegateSupportedSprimTypes() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.Create() else {
            XCTFail("Failed to create render delegate")
            return
        }

        let sprimTypes = delegate.supportedSprimTypes
        XCTAssertFalse(sprimTypes.isEmpty)

        // Storm should support camera and light
        let hasCamera = sprimTypes.contains { String(cString: $0.GetText()) == "camera" }
        let hasLight = sprimTypes.contains { String(cString: $0.GetText()) == "light" }
        XCTAssertTrue(hasCamera, "Storm should support camera sprim type")
        XCTAssertTrue(hasLight, "Storm should support light sprim type")
    }

    func testRenderDelegateSupportedBprimTypes() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.Create() else {
            XCTFail("Failed to create render delegate")
            return
        }

        let bprimTypes = delegate.supportedBprimTypes
        XCTAssertFalse(bprimTypes.isEmpty)

        // Storm should support renderBuffer
        let hasRenderBuffer = bprimTypes.contains { String(cString: $0.GetText()) == "renderBuffer" }
        XCTAssertTrue(hasRenderBuffer, "Storm should support renderBuffer bprim type")
    }

    func testRenderDelegateMaterialContexts() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.Create() else {
            XCTFail("Failed to create render delegate")
            return
        }

        let contexts = delegate.materialRenderContexts
        XCTAssertFalse(contexts.isEmpty)

        // Storm should support glslfx
        let hasGlslfx = contexts.contains { String(cString: $0.GetText()) == "glslfx" }
        XCTAssertTrue(hasGlslfx, "Storm should support glslfx material context")
    }

    func testRenderDelegatePrimvarFiltering() {
        guard let delegate = swiftusd.HdStRenderDelegateRef.Create() else {
            XCTFail("Failed to create render delegate")
            return
        }

        // Storm typically requires primvar filtering
        let needsFiltering = delegate.isPrimvarFilteringNeeded
        XCTAssertTrue(needsFiltering)
    }

    // MARK: - HdStTextureType Tests

    func testTextureTypeValues() {
        XCTAssertEqual(swiftusd.HdStTextureType.Uv.rawValue, 0)
        XCTAssertEqual(swiftusd.HdStTextureType.Field.rawValue, 1)
        XCTAssertEqual(swiftusd.HdStTextureType.Ptex.rawValue, 2)
        XCTAssertEqual(swiftusd.HdStTextureType.Udim.rawValue, 3)
    }

    func testTextureTypeNames() {
        XCTAssertEqual(swiftusd.HdStTextureType.Uv.name, "uv")
        XCTAssertEqual(swiftusd.HdStTextureType.Field.name, "field")
        XCTAssertEqual(swiftusd.HdStTextureType.Ptex.name, "ptex")
        XCTAssertEqual(swiftusd.HdStTextureType.Udim.name, "udim")
    }

    // MARK: - HdStMeshTopologyInfo Tests

    func testMeshTopologyInfoDefault() {
        let topoInfo = swiftusd.HdStMeshTopologyInfo()

        XCTAssertEqual(topoInfo.numFaces, 0)
        XCTAssertEqual(topoInfo.numPoints, 0)
        XCTAssertEqual(topoInfo.numFaceVertices, 0)
        XCTAssertFalse(topoInfo.isLeftHanded)
        XCTAssertEqual(String(cString: topoInfo.scheme.GetText()), "none")
    }

    func testMeshTopologyInfoWithValues() {
        let scheme = swiftusd.Token("catmullClark")
        let topoInfo = swiftusd.HdStMeshTopologyInfo(6, 8, 24, false, scheme)

        XCTAssertEqual(topoInfo.numFaces, 6)
        XCTAssertEqual(topoInfo.numPoints, 8)
        XCTAssertEqual(topoInfo.numFaceVertices, 24)
        XCTAssertFalse(topoInfo.isLeftHanded)
        XCTAssertEqual(String(cString: topoInfo.scheme.GetText()), "catmullClark")
    }

    func testMeshTopologyInfoConvenienceMethod() {
        let topoInfo = swiftusd.HdStMeshTopologyInfo.make(
            numFaces: 12,
            numPoints: 20,
            numFaceVertices: 36,
            isLeftHanded: true,
            scheme: swiftusd.HdStMeshTopologyInfo.catmullClark
        )

        XCTAssertEqual(topoInfo.numFaces, 12)
        XCTAssertEqual(topoInfo.numPoints, 20)
        XCTAssertEqual(topoInfo.numFaceVertices, 36)
        XCTAssertTrue(topoInfo.isLeftHanded)
        XCTAssertEqual(String(cString: topoInfo.scheme.GetText()), "catmullClark")
    }

    // MARK: - HdStMaterialParam Tests

    func testMaterialParamDefault() {
        let param = swiftusd.HdStMaterialParam()

        XCTAssertTrue(param.name.IsEmpty())
        XCTAssertTrue(param.role.IsEmpty())
        XCTAssertTrue(param.fallbackValue.isEmpty)
        XCTAssertFalse(param.isPrimvar)
        XCTAssertFalse(param.isTexture)
    }

    func testMaterialParamWithValues() {
        let name = swiftusd.Token("diffuseColor")
        let role = swiftusd.Token("color")
        let param = swiftusd.HdStMaterialParam(name, role, "(0.5, 0.5, 0.5)")

        XCTAssertEqual(String(cString: param.name.GetText()), "diffuseColor")
        XCTAssertEqual(String(cString: param.role.GetText()), "color")
        XCTAssertEqual(param.fallbackValue, "(0.5, 0.5, 0.5)")
    }

    // MARK: - HdStMaterialBinding Tests

    func testMaterialBindingDefault() {
        let binding = swiftusd.HdStMaterialBinding()

        XCTAssertFalse(binding.isValid)
        XCTAssertTrue(binding.materialPath.IsEmpty())
    }

    func testMaterialBindingWithPath() {
        let path = swiftusd.Path("/Materials/Gold")
        let tag = swiftusd.Token("defaultMaterialTag")
        let binding = swiftusd.HdStMaterialBinding(path, tag)

        XCTAssertTrue(binding.isValid)
        XCTAssertEqual(String(cString: binding.materialPath.GetText()), "/Materials/Gold")
        XCTAssertEqual(String(cString: binding.materialTag.GetText()), "defaultMaterialTag")
    }

    func testMaterialBindingConvenienceMethod() {
        let path = swiftusd.Path("/Materials/Glass")
        let binding = swiftusd.HdStMaterialBinding.make(
            path: path,
            tag: swiftusd.Token.translucentMaterial
        )

        XCTAssertTrue(binding.isValid)
        XCTAssertEqual(String(cString: binding.materialPath.GetText()), "/Materials/Glass")
        XCTAssertEqual(String(cString: binding.materialTag.GetText()), "translucent")
    }

    // MARK: - HdStDrawItemInfo Tests

    func testDrawItemInfoDefault() {
        let info = swiftusd.HdStDrawItemInfo()

        XCTAssertTrue(info.rprimId.IsEmpty())
        XCTAssertTrue(info.reprToken.IsEmpty())
        XCTAssertTrue(info.materialTag.IsEmpty())
        XCTAssertEqual(info.drawCoordIndex, -1)
        XCTAssertTrue(info.isVisible)
    }

    func testDrawItemInfoWithValues() {
        let path = swiftusd.Path("/World/Mesh")
        let repr = swiftusd.Token("refined")
        let matTag = swiftusd.Token("defaultMaterialTag")
        let info = swiftusd.HdStDrawItemInfo(path, repr, matTag, true)

        XCTAssertEqual(String(cString: info.rprimId.GetText()), "/World/Mesh")
        XCTAssertEqual(String(cString: info.reprToken.GetText()), "refined")
        XCTAssertEqual(String(cString: info.materialTag.GetText()), "defaultMaterialTag")
        XCTAssertTrue(info.isVisible)
    }

    // MARK: - HdStRenderPassStateConfig Tests

    func testRenderPassStateConfigDefault() {
        let config = swiftusd.HdStRenderPassStateConfig()

        XCTAssertEqual(config.alphaThreshold, 0.5, accuracy: 0.001)
        XCTAssertTrue(config.enableDepthTest)
        XCTAssertTrue(config.enableDepthWrite)
        XCTAssertFalse(config.enableStencil)
        XCTAssertEqual(config.cullStyle, .BackUnlessDoubleSided)
        XCTAssertTrue(config.enableSceneMaterials)
    }

    func testRenderPassStateConfigConvenienceMethod() {
        let config = swiftusd.HdStRenderPassStateConfig.makeDefault()

        XCTAssertEqual(config.alphaThreshold, 0.5, accuracy: 0.001)
        XCTAssertTrue(config.enableDepthTest)
    }

    // MARK: - HdStAovDescriptor Tests

    func testAovDescriptorDefault() {
        let aov = swiftusd.HdStAovDescriptor()

        XCTAssertTrue(aov.aovName.IsEmpty())
        XCTAssertTrue(aov.format.IsEmpty())
        XCTAssertEqual(aov.clearValue.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(aov.clearValue.y, 0.0, accuracy: 0.001)
        XCTAssertEqual(aov.clearValue.z, 0.0, accuracy: 0.001)
        XCTAssertEqual(aov.clearValue.w, 1.0, accuracy: 0.001)
        XCTAssertFalse(aov.multiSampled)
    }

    func testAovDescriptorWithValues() {
        let clearColor = swiftusd.Vec4f(0.2, 0.3, 0.4, 1.0)
        let aov = swiftusd.HdStAovDescriptor(
            swiftusd.Token("color"),
            swiftusd.Token("float16Vec4"),
            clearColor,
            true
        )

        XCTAssertEqual(String(cString: aov.aovName.GetText()), "color")
        XCTAssertEqual(String(cString: aov.format.GetText()), "float16Vec4")
        XCTAssertEqual(aov.clearValue.x, 0.2, accuracy: 0.001)
        XCTAssertEqual(aov.clearValue.y, 0.3, accuracy: 0.001)
        XCTAssertEqual(aov.clearValue.z, 0.4, accuracy: 0.001)
        XCTAssertEqual(aov.clearValue.w, 1.0, accuracy: 0.001)
        XCTAssertTrue(aov.multiSampled)
    }

    func testAovDescriptorColorAov() {
        let aov = swiftusd.HdStAovDescriptor.colorAov()

        XCTAssertEqual(String(cString: aov.aovName.GetText()), "color")
        XCTAssertEqual(String(cString: aov.format.GetText()), "float16Vec4")
    }

    func testAovDescriptorDepthAov() {
        let aov = swiftusd.HdStAovDescriptor.depthAov()

        XCTAssertEqual(String(cString: aov.aovName.GetText()), "depth")
        XCTAssertEqual(String(cString: aov.format.GetText()), "float32")
    }

    // MARK: - HdStTokens Tests

    func testHdStTextureTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.UV().GetText()), "uv")
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.Field().GetText()), "field")
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.Ptex().GetText()), "ptex")
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.Udim().GetText()), "udim")
    }

    func testHdStShaderTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.DisplacementShader().GetText()), "displacementShader")
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.SurfaceShader().GetText()), "surfaceShader")
        XCTAssertEqual(String(cString: swiftusd.HdStTokens.VolumeShader().GetText()), "volumeShader")
    }

    func testHdStMaterialTagTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdStMaterialTagTokens.Masked().GetText()), "masked")
        XCTAssertEqual(String(cString: swiftusd.HdStMaterialTagTokens.Additive().GetText()), "additive")
        XCTAssertEqual(String(cString: swiftusd.HdStMaterialTagTokens.Translucent().GetText()), "translucent")
        XCTAssertEqual(String(cString: swiftusd.HdStMaterialTagTokens.Volume().GetText()), "volume")
    }

    func testHdStAovTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdStAovTokens.Color().GetText()), "color")
        XCTAssertEqual(String(cString: swiftusd.HdStAovTokens.Depth().GetText()), "depth")
        XCTAssertEqual(String(cString: swiftusd.HdStAovTokens.CameraDepth().GetText()), "cameraDepth")
        XCTAssertEqual(String(cString: swiftusd.HdStAovTokens.PrimId().GetText()), "primId")
        XCTAssertEqual(String(cString: swiftusd.HdStAovTokens.InstanceId().GetText()), "instanceId")
        XCTAssertEqual(String(cString: swiftusd.HdStAovTokens.Normal().GetText()), "normal")
    }

    // MARK: - Token Extensions Tests

    func testTokenExtensionsTexture() {
        XCTAssertEqual(String(cString: swiftusd.Token.uvTexture.GetText()), "uv")
        XCTAssertEqual(String(cString: swiftusd.Token.fieldTexture.GetText()), "field")
        XCTAssertEqual(String(cString: swiftusd.Token.ptexTexture.GetText()), "ptex")
        XCTAssertEqual(String(cString: swiftusd.Token.udimTexture.GetText()), "udim")
    }

    func testTokenExtensionsShader() {
        XCTAssertEqual(String(cString: swiftusd.Token.displacementShader.GetText()), "displacementShader")
        XCTAssertEqual(String(cString: swiftusd.Token.surfaceShader.GetText()), "surfaceShader")
        XCTAssertEqual(String(cString: swiftusd.Token.volumeShader.GetText()), "volumeShader")
    }

    func testTokenExtensionsMaterialTag() {
        XCTAssertEqual(String(cString: swiftusd.Token.maskedMaterial.GetText()), "masked")
        XCTAssertEqual(String(cString: swiftusd.Token.additiveMaterial.GetText()), "additive")
        XCTAssertEqual(String(cString: swiftusd.Token.translucentMaterial.GetText()), "translucent")
        XCTAssertEqual(String(cString: swiftusd.Token.volumeMaterial.GetText()), "volume")
    }

    func testTokenExtensionsAov() {
        XCTAssertEqual(String(cString: swiftusd.Token.colorAov.GetText()), "color")
        XCTAssertEqual(String(cString: swiftusd.Token.depthAov.GetText()), "depth")
        XCTAssertEqual(String(cString: swiftusd.Token.cameraDepthAov.GetText()), "cameraDepth")
        XCTAssertEqual(String(cString: swiftusd.Token.primIdAov.GetText()), "primId")
        XCTAssertEqual(String(cString: swiftusd.Token.instanceIdAov.GetText()), "instanceId")
        XCTAssertEqual(String(cString: swiftusd.Token.normalAov.GetText()), "normal")
    }

    // MARK: - HdStComputeQueuePriority Tests

    func testComputeQueuePriorityValues() {
        XCTAssertEqual(swiftusd.HdStComputeQueuePriority.Zero.rawValue, 0)
        XCTAssertEqual(swiftusd.HdStComputeQueuePriority.One.rawValue, 1)
        XCTAssertEqual(swiftusd.HdStComputeQueuePriority.Two.rawValue, 2)
        XCTAssertEqual(swiftusd.HdStComputeQueuePriority.Three.rawValue, 3)
        XCTAssertEqual(swiftusd.HdStComputeQueuePriority.Count.rawValue, 4)
    }

    // MARK: - Global Function Tests

    func testHdStIsSupported() {
        let isSupported = hdStIsSupported()
        XCTAssertTrue(isSupported)
    }

    func testHdStIsSupportedNative() {
        let isSupported = swiftusd.HdStIsSupported()
        XCTAssertTrue(isSupported)
    }

    // MARK: - Subdivision Scheme Tokens

    func testSubdivisionSchemeTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdStMeshTopologyInfo.catmullClark.GetText()), "catmullClark")
        XCTAssertEqual(String(cString: swiftusd.HdStMeshTopologyInfo.loop.GetText()), "loop")
        XCTAssertEqual(String(cString: swiftusd.HdStMeshTopologyInfo.bilinear.GetText()), "bilinear")
        XCTAssertEqual(String(cString: swiftusd.HdStMeshTopologyInfo.noSubdivision.GetText()), "none")
    }
}
