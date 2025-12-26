// UsdShadeTests.swift - Tests for UsdShade module (Shading Schemas)

import XCTest
import SwiftUSD
@testable import CxxFacade

final class UsdShadeTests: XCTestCase {

    // MARK: - UsdShadeMaterial Tests

    func testMaterialDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Materials/MyMaterial")
        let material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, path)

        XCTAssertTrue(material.GetPrim().IsValid())
        XCTAssertEqual(String(cString: material.GetPrim().GetTypeName().GetText()), "Material")
    }

    func testMaterialSurfaceOutput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial"))

        let surfaceOutput = material.CreateSurfaceOutput(cxxfacade.tf.TfToken())
        XCTAssertTrue(surfaceOutput.IsValid())
    }

    func testMaterialDisplacementOutput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial"))

        let displacementOutput = material.CreateDisplacementOutput(cxxfacade.tf.TfToken())
        XCTAssertTrue(displacementOutput.IsValid())
    }

    func testMaterialVolumeOutput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial"))

        let volumeOutput = material.CreateVolumeOutput(cxxfacade.tf.TfToken())
        XCTAssertTrue(volumeOutput.IsValid())
    }

    // MARK: - UsdShadeShader Tests

    func testShaderDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface")
        let shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, path)

        XCTAssertTrue(shader.GetPrim().IsValid())
        XCTAssertEqual(String(cString: shader.GetPrim().GetTypeName().GetText()), "Shader")
    }

    func testShaderSetShaderId() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let shaderId = cxxfacade.tf.TfToken("UsdPreviewSurface")
        let success = shader.SetShaderId(shaderId)
        XCTAssertTrue(success)

        var retrievedId = cxxfacade.tf.TfToken()
        let gotId = shader.GetShaderId(&retrievedId)
        XCTAssertTrue(gotId)
        XCTAssertEqual(String(cString: retrievedId.GetText()), "UsdPreviewSurface")
    }

    func testShaderCreateInput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let colorInput = shader.CreateInput(cxxfacade.tf.TfToken("diffuseColor"), std.string("color3f"))
        XCTAssertTrue(colorInput.IsValid())

        let fullName = colorInput.GetFullName()
        XCTAssertEqual(String(cString: fullName.GetText()), "inputs:diffuseColor")
    }

    func testShaderCreateOutput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let surfaceOutput = shader.CreateOutput(cxxfacade.tf.TfToken("surface"), std.string("token"))
        XCTAssertTrue(surfaceOutput.IsValid())

        let fullName = surfaceOutput.GetFullName()
        XCTAssertEqual(String(cString: fullName.GetText()), "outputs:surface")
    }

    func testShaderGetInputs() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let _ = shader.CreateInput(cxxfacade.tf.TfToken("diffuseColor"), std.string("color3f"))
        let _ = shader.CreateInput(cxxfacade.tf.TfToken("roughness"), std.string("float"))

        let inputs = shader.GetInputs(true)
        XCTAssertEqual(inputs.size(), 2)
    }

    func testShaderGetOutputs() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let _ = shader.CreateOutput(cxxfacade.tf.TfToken("surface"), std.string("token"))

        let outputs = shader.GetOutputs(true)
        XCTAssertEqual(outputs.size(), 1)
    }

    // MARK: - UsdShadeNodeGraph Tests

    func testNodeGraphDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Materials/MyNodeGraph")
        let nodeGraph = cxxfacade.usdShade.UsdShadeNodeGraph.Define(&stage, path)

        XCTAssertTrue(nodeGraph.GetPrim().IsValid())
        XCTAssertEqual(String(cString: nodeGraph.GetPrim().GetTypeName().GetText()), "NodeGraph")
    }

    func testNodeGraphInputsOutputs() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var nodeGraph = cxxfacade.usdShade.UsdShadeNodeGraph.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyNodeGraph"))

        let input = nodeGraph.CreateInput(cxxfacade.tf.TfToken("color"), std.string("color3f"))
        XCTAssertTrue(input.IsValid())

        let output = nodeGraph.CreateOutput(cxxfacade.tf.TfToken("result"), std.string("color3f"))
        XCTAssertTrue(output.IsValid())
    }

    // MARK: - UsdShadeConnectableAPI Tests

    func testConnectableAPI() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))
        let prim = shader.GetPrim()

        let api = cxxfacade.usdShade.UsdShadeConnectableAPI(prim)
        XCTAssertTrue(api.GetPrim().IsValid())
    }

    func testConnectableAPICreateInput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))
        var api = cxxfacade.usdShade.UsdShadeConnectableAPI(shader.GetPrim())

        let input = api.CreateInput(cxxfacade.tf.TfToken("testInput"), std.string("float"))
        XCTAssertTrue(input.IsValid())
    }

    func testConnectableAPICreateOutput() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))
        var api = cxxfacade.usdShade.UsdShadeConnectableAPI(shader.GetPrim())

        let output = api.CreateOutput(cxxfacade.tf.TfToken("testOutput"), std.string("token"))
        XCTAssertTrue(output.IsValid())
    }

    // MARK: - UsdShadeMaterialBindingAPI Tests

    func testMaterialBindingAPIApply() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/World/Mesh"))
        let prim = mesh.GetPrim()

        let bindingAPI = cxxfacade.usdShade.UsdShadeMaterialBindingAPI.Apply(prim)
        XCTAssertTrue(bindingAPI.GetPrim().IsValid())
    }

    func testMaterialBindingAPICanApply() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/World/Mesh"))
        let prim = mesh.GetPrim()

        let canApply = cxxfacade.usdShade.UsdShadeMaterialBindingAPI.CanApply(prim, nil)
        XCTAssertTrue(canApply)
    }

    func testMaterialBindingAPIBind() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create a material
        let material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial"))

        // Create a mesh
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/World/Mesh"))
        let meshPrim = mesh.GetPrim()

        // Apply binding API and bind material
        var bindingAPI = cxxfacade.usdShade.UsdShadeMaterialBindingAPI.Apply(meshPrim)
        let success = bindingAPI.Bind(material, cxxfacade.tf.TfToken(), cxxfacade.tf.TfToken())
        XCTAssertTrue(success)

        // Verify the binding
        let boundMaterial = bindingAPI.ComputeBoundMaterial(cxxfacade.tf.TfToken(), nil)
        XCTAssertTrue(boundMaterial.GetPrim().IsValid())
    }

    func testMaterialBindingAPIUnbind() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create a material
        let material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial"))

        // Create a mesh
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/World/Mesh"))
        let meshPrim = mesh.GetPrim()

        // Apply binding API and bind material
        var bindingAPI = cxxfacade.usdShade.UsdShadeMaterialBindingAPI.Apply(meshPrim)
        let _ = bindingAPI.Bind(material, cxxfacade.tf.TfToken(), cxxfacade.tf.TfToken())

        // Unbind
        let unbound = bindingAPI.UnbindDirectBinding(cxxfacade.tf.TfToken())
        XCTAssertTrue(unbound)
    }

    // MARK: - Connection Tests

    func testInputOutputConnection() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create material and shader
        var material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial"))
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))
        let _ = shader.SetShaderId(cxxfacade.tf.TfToken("UsdPreviewSurface"))

        // Create shader output
        let shaderOutput = shader.CreateOutput(cxxfacade.tf.TfToken("surface"), std.string("token"))
        XCTAssertTrue(shaderOutput.IsValid())

        // Create material surface output and connect to shader
        let surfaceOutput = material.CreateSurfaceOutput(cxxfacade.tf.TfToken())

        // Connect using path
        let sourcePath = shaderOutput.GetAttr().GetPath()
        let connected = surfaceOutput.ConnectToSource(sourcePath)
        XCTAssertTrue(connected)

        // Verify connection
        XCTAssertTrue(surfaceOutput.HasConnectedSource())
    }

    // MARK: - Token Tests

    func testShadeTokens() {
        // Test that token functions work
        let idToken = cxxfacade.usdShade.UsdShadeTokens.id()
        XCTAssertTrue(strlen(idToken.GetText()) > 0)

        let surfaceToken = cxxfacade.usdShade.UsdShadeTokens.surface()
        XCTAssertTrue(strlen(surfaceToken.GetText()) > 0)
        XCTAssertEqual(String(cString: surfaceToken.GetText()), "surface")

        let displacementToken = cxxfacade.usdShade.UsdShadeTokens.displacement()
        XCTAssertTrue(strlen(displacementToken.GetText()) > 0)
        XCTAssertEqual(String(cString: displacementToken.GetText()), "displacement")

        let volumeToken = cxxfacade.usdShade.UsdShadeTokens.volume()
        XCTAssertTrue(strlen(volumeToken.GetText()) > 0)
        XCTAssertEqual(String(cString: volumeToken.GetText()), "volume")

        let fullToken = cxxfacade.usdShade.UsdShadeTokens.full()
        XCTAssertTrue(strlen(fullToken.GetText()) > 0)
    }

    // MARK: - UsdShadeInput Tests

    func testInputProperties() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let input = shader.CreateInput(cxxfacade.tf.TfToken("diffuseColor"), std.string("color3f"))
        XCTAssertTrue(input.IsValid())

        let baseName = input.GetBaseName()
        XCTAssertEqual(String(cString: baseName.GetText()), "diffuseColor")

        let typeName = input.GetTypeName()
        XCTAssertEqual(String(typeName), "color3f")

        XCTAssertTrue(input.GetPrim().IsValid())
        XCTAssertTrue(input.GetAttr().IsValid())
    }

    func testInputDocumentation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        var input = shader.CreateInput(cxxfacade.tf.TfToken("diffuseColor"), std.string("color3f"))

        let success = input.SetDocumentation(std.string("The diffuse color of the material"))
        XCTAssertTrue(success)

        let docs = input.GetDocumentation()
        XCTAssertEqual(String(docs), "The diffuse color of the material")
    }

    // MARK: - UsdShadeOutput Tests

    func testOutputProperties() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/MyMaterial/Surface"))

        let output = shader.CreateOutput(cxxfacade.tf.TfToken("surface"), std.string("token"))
        XCTAssertTrue(output.IsValid())

        let baseName = output.GetBaseName()
        XCTAssertEqual(String(cString: baseName.GetText()), "surface")

        XCTAssertTrue(output.GetPrim().IsValid())
        XCTAssertTrue(output.GetAttr().IsValid())
    }

    // MARK: - Complete Material Network Test

    func testCompleteMaterialNetwork() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()

        // Create material
        var material = cxxfacade.usdShade.UsdShadeMaterial.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/SimpleMaterial"))
        XCTAssertTrue(material.GetPrim().IsValid())

        // Create shader
        var shader = cxxfacade.usdShade.UsdShadeShader.Define(&stage, cxxfacade.sdf.SdfPath("/Materials/SimpleMaterial/Surface"))
        XCTAssertTrue(shader.SetShaderId(cxxfacade.tf.TfToken("UsdPreviewSurface")))

        // Create inputs
        let diffuseColorInput = shader.CreateInput(cxxfacade.tf.TfToken("diffuseColor"), std.string("color3f"))
        XCTAssertTrue(diffuseColorInput.IsValid())

        let roughnessInput = shader.CreateInput(cxxfacade.tf.TfToken("roughness"), std.string("float"))
        XCTAssertTrue(roughnessInput.IsValid())

        // Create surface output
        let surfaceOutput = shader.CreateOutput(cxxfacade.tf.TfToken("surface"), std.string("token"))
        XCTAssertTrue(surfaceOutput.IsValid())

        // Connect material surface to shader output
        let materialSurface = material.CreateSurfaceOutput(cxxfacade.tf.TfToken())
        let shaderOutputPath = surfaceOutput.GetAttr().GetPath()
        let connected = materialSurface.ConnectToSource(shaderOutputPath)
        XCTAssertTrue(connected)

        // Verify connection
        XCTAssertTrue(materialSurface.HasConnectedSource())

        // Create mesh and bind material
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/World/Mesh"))
        var bindingAPI = cxxfacade.usdShade.UsdShadeMaterialBindingAPI.Apply(mesh.GetPrim())
        let bound = bindingAPI.Bind(material, cxxfacade.tf.TfToken(), cxxfacade.tf.TfToken())
        XCTAssertTrue(bound)

        // Verify binding
        let boundMaterial = bindingAPI.ComputeBoundMaterial(cxxfacade.tf.TfToken(), nil)
        XCTAssertTrue(boundMaterial.GetPrim().IsValid())
        XCTAssertEqual(String(boundMaterial.GetPrim().GetPath().GetString()), "/Materials/SimpleMaterial")
    }
}
