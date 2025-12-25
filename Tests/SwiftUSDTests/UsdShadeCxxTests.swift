// UsdShadeCxxTests.swift - Tests for UsdShade C++ interop types

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdShadeCxxTests: XCTestCase {

    // MARK: - ShadeShaderRef Tests

    func testShadeShaderDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Shader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Shader")
            return
        }

        XCTAssertTrue(shader.isValid)
    }

    func testShadeShaderShaderId() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Shader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Shader")
            return
        }

        // Set shader ID
        let shaderId = swiftusd.Token("UsdPreviewSurface")
        XCTAssertTrue(shader.SetShaderId(shaderId))

        // Get shader ID back
        let retrievedId = shader.GetShaderId()
        XCTAssertEqual(String(cString: retrievedId.GetText()), "UsdPreviewSurface")
    }

    func testShadeShaderInputs() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Shader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Shader")
            return
        }

        // Create an input using direct C++ method call
        guard let input = shader.CreateInput(swiftusd.Token("diffuseColor"), swiftusd.Token("color3f")) else {
            XCTFail("Failed to create input")
            return
        }

        XCTAssertTrue(input.IsValid())
        XCTAssertEqual(String(cString: input.GetBaseName().GetText()), "diffuseColor")
    }

    func testShadeShaderOutputs() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Shader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Shader")
            return
        }

        // Create an output
        guard let output = shader.createOutput(named: "surface", typeName: "token") else {
            XCTFail("Failed to create output")
            return
        }

        XCTAssertTrue(output.isValid)
        XCTAssertEqual(String(cString: output.baseName.GetText()), "surface")
    }

    // MARK: - ShadeNodeGraphRef Tests

    func testShadeNodeGraphDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/NodeGraph")
        guard let nodeGraph = swiftusd.ShadeNodeGraphRef.define(on: stage, at: path) else {
            XCTFail("Failed to define NodeGraph")
            return
        }

        XCTAssertTrue(nodeGraph.isValid)
    }

    func testShadeNodeGraphInputsOutputs() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/NodeGraph")
        guard let nodeGraph = swiftusd.ShadeNodeGraphRef.define(on: stage, at: path) else {
            XCTFail("Failed to define NodeGraph")
            return
        }

        // Create interface input
        guard let input = nodeGraph.createInput(named: "baseColor", typeName: "color3f") else {
            XCTFail("Failed to create input")
            return
        }
        XCTAssertTrue(input.isValid)

        // Create interface output
        guard let output = nodeGraph.createOutput(named: "surface", typeName: "token") else {
            XCTFail("Failed to create output")
            return
        }
        XCTAssertTrue(output.isValid)
    }

    // MARK: - ShadeMaterialRef Tests

    func testShadeMaterialDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Material")
            return
        }

        XCTAssertTrue(material.isValid)
    }

    func testShadeMaterialSurfaceOutput() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Material")
            return
        }

        // Create surface output
        guard let surfaceOutput = material.createSurfaceOutput() else {
            XCTFail("Failed to create surface output")
            return
        }

        XCTAssertTrue(surfaceOutput.isValid)
    }

    func testShadeMaterialDisplacementOutput() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Material")
            return
        }

        // Create displacement output
        guard let displacementOutput = material.createDisplacementOutput() else {
            XCTFail("Failed to create displacement output")
            return
        }

        XCTAssertTrue(displacementOutput.isValid)
    }

    func testShadeMaterialInputs() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Material")
            return
        }

        // Create interface input on material
        guard let input = material.createInput(named: "opacity", typeName: "float") else {
            XCTFail("Failed to create input")
            return
        }

        XCTAssertTrue(input.isValid)
        XCTAssertEqual(String(cString: input.baseName.GetText()), "opacity")
    }

    // MARK: - ShadeMaterialBindingAPIRef Tests

    func testShadeMaterialBindingAPIApply() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        // Create a mesh to bind to
        let meshPath = swiftusd.Path("/World/Mesh")
        guard let prim = stage.definePrim(at: meshPath, typeName: "Mesh") else {
            XCTFail("Failed to define prim")
            return
        }

        // Apply MaterialBindingAPI
        guard let bindingAPI = swiftusd.ShadeMaterialBindingAPIRef.apply(to: prim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        XCTAssertTrue(bindingAPI.isValid)
    }

    func testShadeMaterialBindingAPIBind() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        // Create a material
        let materialPath = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: materialPath) else {
            XCTFail("Failed to define Material")
            return
        }

        // Create a mesh to bind to
        let meshPath = swiftusd.Path("/World/Mesh")
        guard let prim = stage.definePrim(at: meshPath, typeName: "Mesh") else {
            XCTFail("Failed to define prim")
            return
        }

        // Apply MaterialBindingAPI and bind
        guard let bindingAPI = swiftusd.ShadeMaterialBindingAPIRef.apply(to: prim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        XCTAssertTrue(bindingAPI.bind(material: material))
        XCTAssertTrue(bindingAPI.hasBoundMaterial())
    }

    func testShadeMaterialBindingAPIUnbind() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        // Create a material
        let materialPath = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: materialPath) else {
            XCTFail("Failed to define Material")
            return
        }

        // Create a mesh to bind to
        let meshPath = swiftusd.Path("/World/Mesh")
        guard let prim = stage.definePrim(at: meshPath, typeName: "Mesh") else {
            XCTFail("Failed to define prim")
            return
        }

        // Apply MaterialBindingAPI, bind and unbind
        guard let bindingAPI = swiftusd.ShadeMaterialBindingAPIRef.apply(to: prim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        XCTAssertTrue(bindingAPI.bind(material: material))
        XCTAssertTrue(bindingAPI.hasBoundMaterial())
        XCTAssertTrue(bindingAPI.unbind())
        XCTAssertFalse(bindingAPI.hasBoundMaterial())
    }

    // MARK: - ShadeInputRef Value Tests

    func testShadeInputFloatValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Shader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Shader")
            return
        }

        // Create a float input
        guard let input = shader.createInput(named: "roughness", typeName: "float") else {
            XCTFail("Failed to create input")
            return
        }

        // Set and get float value
        XCTAssertTrue(input.set(0.5))

        if let value = input.getFloat() {
            XCTAssertEqual(value, 0.5, accuracy: 1e-6)
        } else {
            XCTFail("Failed to get float value")
        }
    }

    func testShadeInputVec3fValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Shader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Shader")
            return
        }

        // Create a color input
        guard let input = shader.createInput(named: "diffuseColor", typeName: "color3f") else {
            XCTFail("Failed to create input")
            return
        }

        // Set and get Vec3f value
        let color = swiftusd.Vec3f(1.0, 0.5, 0.0)
        XCTAssertTrue(input.set(color))

        if let value = input.getVec3f() {
            XCTAssertEqual(value.x, 1.0, accuracy: 1e-6)
            XCTAssertEqual(value.y, 0.5, accuracy: 1e-6)
            XCTAssertEqual(value.z, 0.0, accuracy: 1e-6)
        } else {
            XCTFail("Failed to get Vec3f value")
        }
    }

    // MARK: - ShadeConnectionSourceInfo Tests

    func testShadeConnectionSourceInfo() {
        let info = swiftusd.ShadeConnectionSourceInfo()
        XCTAssertFalse(info.isValid)

        let validInfo = swiftusd.ShadeConnectionSourceInfo(
            swiftusd.Path("/World/Shader.outputs:surface"),
            swiftusd.Token("surface"),
            swiftusd.Token("shader")
        )
        XCTAssertTrue(validInfo.isValid)
    }

    // MARK: - Integration Tests

    func testCompleteShadingWorkflow() {
        guard let stage = swiftusd.StageRef.createInMemory(identifier: "shading_test") else {
            XCTFail("Failed to create stage")
            return
        }

        // Create a material
        let materialPath = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: materialPath) else {
            XCTFail("Failed to define Material")
            return
        }
        XCTAssertTrue(material.isValid)

        // Create surface output on material
        guard let surfaceOutput = material.createSurfaceOutput() else {
            XCTFail("Failed to create surface output")
            return
        }
        XCTAssertTrue(surfaceOutput.isValid)

        // Create a shader inside the material
        let shaderPath = swiftusd.Path("/World/Material/PreviewShader")
        guard let shader = swiftusd.ShadeShaderRef.define(on: stage, at: shaderPath) else {
            XCTFail("Failed to define Shader")
            return
        }
        XCTAssertTrue(shader.isValid)

        // Set shader ID
        shader.shaderId = swiftusd.Token("UsdPreviewSurface")
        XCTAssertEqual(String(cString: shader.shaderId.GetText()), "UsdPreviewSurface")

        // Create inputs on the shader
        guard let diffuseInput = shader.createInput(named: "diffuseColor", typeName: "color3f") else {
            XCTFail("Failed to create diffuseColor input")
            return
        }
        XCTAssertTrue(diffuseInput.set(swiftusd.Vec3f(0.18, 0.18, 0.18)))

        guard let roughnessInput = shader.createInput(named: "roughness", typeName: "float") else {
            XCTFail("Failed to create roughness input")
            return
        }
        XCTAssertTrue(roughnessInput.set(0.5))

        guard let metallicInput = shader.createInput(named: "metallic", typeName: "float") else {
            XCTFail("Failed to create metallic input")
            return
        }
        XCTAssertTrue(metallicInput.set(0.0))

        // Create surface output on shader
        guard let shaderOutput = shader.createOutput(named: "surface", typeName: "token") else {
            XCTFail("Failed to create shader output")
            return
        }
        XCTAssertTrue(shaderOutput.isValid)

        // Create geometry and bind material
        let meshPath = swiftusd.Path("/World/Mesh")
        guard let meshPrim = stage.definePrim(at: meshPath, typeName: "Mesh") else {
            XCTFail("Failed to define Mesh prim")
            return
        }

        guard let bindingAPI = swiftusd.ShadeMaterialBindingAPIRef.apply(to: meshPrim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        XCTAssertTrue(bindingAPI.bind(material: material))
        XCTAssertTrue(bindingAPI.hasBoundMaterial())

        // Verify material binding
        if let boundMaterial = bindingAPI.boundMaterial() {
            XCTAssertTrue(boundMaterial.isValid)
        } else {
            XCTFail("Failed to get bound material")
        }
    }

    func testMaterialWithNodeGraph() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        // Create material
        let materialPath = swiftusd.Path("/World/Material")
        guard let material = swiftusd.ShadeMaterialRef.define(on: stage, at: materialPath) else {
            XCTFail("Failed to define Material")
            return
        }

        // Create node graph inside material
        let nodeGraphPath = swiftusd.Path("/World/Material/Textures")
        guard let nodeGraph = swiftusd.ShadeNodeGraphRef.define(on: stage, at: nodeGraphPath) else {
            XCTFail("Failed to define NodeGraph")
            return
        }
        XCTAssertTrue(nodeGraph.isValid)

        // Create interface inputs on node graph
        guard let uvInput = nodeGraph.createInput(named: "uv", typeName: "float2") else {
            XCTFail("Failed to create UV input")
            return
        }
        XCTAssertTrue(uvInput.isValid)

        // Create interface output on node graph
        guard let colorOutput = nodeGraph.createOutput(named: "diffuseColor", typeName: "color3f") else {
            XCTFail("Failed to create color output")
            return
        }
        XCTAssertTrue(colorOutput.isValid)

        // Verify counts
        XCTAssertEqual(nodeGraph.inputCount, 1)
        XCTAssertEqual(nodeGraph.outputCount, 1)
    }
}
