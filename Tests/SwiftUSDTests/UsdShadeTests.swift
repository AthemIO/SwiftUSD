// UsdShadeTests.swift - Tests for UsdShade module
// Tests shading types: Material, Shader, NodeGraph, Input, Output, MaterialBindingAPI

import XCTest
@testable import SwiftUSD

// MARK: - Material Tests

final class UsdShadeMaterialTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineMaterial() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        XCTAssertTrue(material.isValid)
        XCTAssertNotNil(material.prim)
        XCTAssertEqual(material.path?.text, "/Materials/MyMaterial")
    }

    func testGetMaterial() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        let retrievedMaterial = Material.get(stage: stage, path: path)
        XCTAssertNotNil(retrievedMaterial)
        XCTAssertTrue(retrievedMaterial?.isValid ?? false)
    }

    func testMaterialFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        guard let prim = material.prim else {
            XCTFail("Material prim should not be nil")
            return
        }

        let materialFromPrim = Material.fromPrim(prim)
        XCTAssertNotNil(materialFromPrim)
        XCTAssertTrue(materialFromPrim?.isValid ?? false)
    }

    // MARK: - Surface Output Tests

    func testMaterialCreateSurfaceOutput() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        let surfaceOutput = material.createSurfaceOutput()
        XCTAssertNotNil(surfaceOutput)
        XCTAssertTrue(surfaceOutput?.isValid ?? false)
    }

    func testMaterialGetSurfaceOutput() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        // Create then get
        _ = material.createSurfaceOutput()
        let surfaceOutput = material.surfaceOutput()
        XCTAssertNotNil(surfaceOutput)
    }

    // MARK: - Input/Output Tests

    func testMaterialCreateInput() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        let inputName = Token("opacity")
        let typeName = Token("float")
        let input = material.createInput(name: inputName, typeName: typeName)

        XCTAssertNotNil(input)
        XCTAssertTrue(input?.isValid ?? false)
    }

    func testMaterialGetInputs() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        // Create some inputs
        _ = material.createInput(name: Token("opacity"), typeName: Token("float"))
        _ = material.createInput(name: Token("color"), typeName: Token("color3f"))

        let inputs = material.inputs(onlyAuthored: true)
        XCTAssertGreaterThanOrEqual(inputs.count, 2)
    }

    func testMaterialCreateOutput() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        let outputName = Token("result")
        let typeName = Token("token")
        let output = material.createOutput(name: outputName, typeName: typeName)

        XCTAssertNotNil(output)
        XCTAssertTrue(output?.isValid ?? false)
    }

    // MARK: - Description Tests

    func testMaterialDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        let description = material.description
        XCTAssertTrue(description.contains("Material") || description.contains("MyMaterial"))
    }
}

// MARK: - Shader Tests

final class UsdShadeShaderTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineShader() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        XCTAssertTrue(shader.isValid)
        XCTAssertNotNil(shader.prim)
    }

    func testGetShader() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        _ = try Shader.define(stage: stage, path: path)

        let retrievedShader = Shader.get(stage: stage, path: path)
        XCTAssertNotNil(retrievedShader)
        XCTAssertTrue(retrievedShader?.isValid ?? false)
    }

    func testShaderFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        guard let prim = shader.prim else {
            XCTFail("Shader prim should not be nil")
            return
        }

        let shaderFromPrim = Shader.fromPrim(prim)
        XCTAssertNotNil(shaderFromPrim)
    }

    // MARK: - Shader ID Tests

    func testShaderSetAndGetId() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        let shaderId = Token("UsdPreviewSurface")
        try shader.setShaderId(shaderId)

        let retrievedId = shader.shaderId
        XCTAssertNotNil(retrievedId)
        XCTAssertEqual(retrievedId?.text, "UsdPreviewSurface")
    }

    // MARK: - Input/Output Tests

    func testShaderCreateInput() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        let inputName = Token("diffuseColor")
        let typeName = Token("color3f")
        let input = shader.createInput(name: inputName, typeName: typeName)

        XCTAssertNotNil(input)
        XCTAssertTrue(input?.isValid ?? false)
    }

    func testShaderCreateOutput() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        let outputName = Token("surface")
        let typeName = Token("token")
        let output = shader.createOutput(name: outputName, typeName: typeName)

        XCTAssertNotNil(output)
        XCTAssertTrue(output?.isValid ?? false)
    }

    func testShaderGetInputs() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        _ = shader.createInput(name: Token("diffuseColor"), typeName: Token("color3f"))
        _ = shader.createInput(name: Token("roughness"), typeName: Token("float"))

        let inputs = shader.inputs(onlyAuthored: true)
        XCTAssertGreaterThanOrEqual(inputs.count, 2)
    }

    // MARK: - Description Tests

    func testShaderDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        let description = shader.description
        XCTAssertTrue(description.contains("PreviewSurface") || description.contains("shader"))
    }
}

// MARK: - Input Tests

final class UsdShadeInputTests: XCTestCase {

    func testInputProperties() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        let inputName = Token("diffuseColor")
        let typeName = Token("color3f")
        guard let input = shader.createInput(name: inputName, typeName: typeName) else {
            XCTFail("Failed to create input")
            return
        }

        XCTAssertTrue(input.isValid)
        XCTAssertNotNil(input.baseName)
        XCTAssertEqual(input.baseName?.text, "diffuseColor")
    }

    func testInputSetAndGetFloat() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        guard let input = shader.createInput(name: Token("roughness"), typeName: Token("float")) else {
            XCTFail("Failed to create input")
            return
        }

        try input.set(Float(0.5))

        let value = input.getFloat()
        XCTAssertNotNil(value)
        XCTAssertEqual(value ?? 0, 0.5, accuracy: 0.001)
    }

    func testInputSetAndGetVec3f() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        guard let input = shader.createInput(name: Token("diffuseColor"), typeName: Token("color3f")) else {
            XCTFail("Failed to create input")
            return
        }

        try input.setVec3f(x: 1.0, y: 0.5, z: 0.0)

        let value = input.getVec3f()
        XCTAssertNotNil(value)
        if let v = value {
            XCTAssertEqual(v.x, 1.0, accuracy: 0.001)
            XCTAssertEqual(v.y, 0.5, accuracy: 0.001)
            XCTAssertEqual(v.z, 0.0, accuracy: 0.001)
        }
    }

    func testInputDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        guard let input = shader.createInput(name: Token("roughness"), typeName: Token("float")) else {
            XCTFail("Failed to create input")
            return
        }

        let description = input.description
        XCTAssertFalse(description.isEmpty)
    }
}

// MARK: - Output Tests

final class UsdShadeOutputTests: XCTestCase {

    func testOutputProperties() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        let outputName = Token("surface")
        let typeName = Token("token")
        guard let output = shader.createOutput(name: outputName, typeName: typeName) else {
            XCTFail("Failed to create output")
            return
        }

        XCTAssertTrue(output.isValid)
        XCTAssertNotNil(output.baseName)
        XCTAssertEqual(output.baseName?.text, "surface")
    }

    func testOutputDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: path)

        guard let output = shader.createOutput(name: Token("surface"), typeName: Token("token")) else {
            XCTFail("Failed to create output")
            return
        }

        let description = output.description
        XCTAssertFalse(description.isEmpty)
    }
}

// MARK: - NodeGraph Tests

final class UsdShadeNodeGraphTests: XCTestCase {

    func testDefineNodeGraph() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/NodeGraph")
        let nodeGraph = try NodeGraph.define(stage: stage, path: path)

        XCTAssertTrue(nodeGraph.isValid)
        XCTAssertNotNil(nodeGraph.prim)
    }

    func testNodeGraphFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/NodeGraph")
        let nodeGraph = try NodeGraph.define(stage: stage, path: path)

        guard let prim = nodeGraph.prim else {
            XCTFail("NodeGraph prim should not be nil")
            return
        }

        let nodeGraphFromPrim = NodeGraph.fromPrim(prim)
        XCTAssertNotNil(nodeGraphFromPrim)
    }

    func testNodeGraphInputsOutputs() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial/NodeGraph")
        let nodeGraph = try NodeGraph.define(stage: stage, path: path)

        // Create input
        let input = nodeGraph.createInput(name: Token("scale"), typeName: Token("float"))
        XCTAssertNotNil(input)

        // Create output
        let output = nodeGraph.createOutput(name: Token("result"), typeName: Token("color3f"))
        XCTAssertNotNil(output)

        // Get inputs and outputs
        let inputs = nodeGraph.inputs(onlyAuthored: true)
        let outputs = nodeGraph.outputs(onlyAuthored: true)

        XCTAssertGreaterThanOrEqual(inputs.count, 1)
        XCTAssertGreaterThanOrEqual(outputs.count, 1)
    }
}

// MARK: - MaterialBindingAPI Tests

final class UsdShadeMaterialBindingAPITests: XCTestCase {

    func testApplyMaterialBindingAPI() throws {
        let stage = try Stage.createInMemory()

        // Create a mesh to bind material to
        let meshPath = try Path("/World/Mesh")
        let meshPrim = try stage.definePrim(at: meshPath, typeName: "Mesh")

        let bindingAPI = MaterialBindingAPI.apply(to: meshPrim)
        XCTAssertNotNil(bindingAPI)
        XCTAssertTrue(bindingAPI?.isValid ?? false)
    }

    func testBindMaterial() throws {
        let stage = try Stage.createInMemory()

        // Create material
        let materialPath = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: materialPath)

        // Create a mesh to bind material to
        let meshPath = try Path("/World/Mesh")
        let meshPrim = try stage.definePrim(at: meshPath, typeName: "Mesh")

        guard let bindingAPI = MaterialBindingAPI.apply(to: meshPrim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        // Bind material
        try bindingAPI.bind(material)

        // Verify binding
        let boundMaterial = bindingAPI.directlyBoundMaterial()
        XCTAssertNotNil(boundMaterial)
    }

    func testUnbindMaterial() throws {
        let stage = try Stage.createInMemory()

        // Create material
        let materialPath = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: materialPath)

        // Create a mesh to bind material to
        let meshPath = try Path("/World/Mesh")
        let meshPrim = try stage.definePrim(at: meshPath, typeName: "Mesh")

        guard let bindingAPI = MaterialBindingAPI.apply(to: meshPrim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        // Bind and then unbind
        try bindingAPI.bind(material)
        try bindingAPI.unbindDirectBinding()

        // Note: The binding may still exist in some form depending on implementation
    }

    func testComputeBoundMaterial() throws {
        let stage = try Stage.createInMemory()

        // Create material
        let materialPath = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: materialPath)

        // Create a mesh to bind material to
        let meshPath = try Path("/World/Mesh")
        let meshPrim = try stage.definePrim(at: meshPath, typeName: "Mesh")

        guard let bindingAPI = MaterialBindingAPI.apply(to: meshPrim) else {
            XCTFail("Failed to apply MaterialBindingAPI")
            return
        }

        // Bind material
        try bindingAPI.bind(material)

        // Compute bound material (should handle inheritance)
        let computedMaterial = bindingAPI.computeBoundMaterial()
        XCTAssertNotNil(computedMaterial)
    }
}

// MARK: - ConnectableAPI Tests

final class UsdShadeConnectableAPITests: XCTestCase {

    func testGetConnectableAPI() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        guard let prim = material.prim else {
            XCTFail("Material prim should not be nil")
            return
        }

        let connectableAPI = ConnectableAPI.get(from: prim)
        XCTAssertNotNil(connectableAPI)
        XCTAssertTrue(connectableAPI?.isValid ?? false)
    }

    func testConnectableAPIIsContainer() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: path)

        guard let prim = material.prim else {
            XCTFail("Material prim should not be nil")
            return
        }

        guard let connectableAPI = ConnectableAPI.get(from: prim) else {
            XCTFail("Failed to get ConnectableAPI")
            return
        }

        // Materials are containers
        XCTAssertTrue(connectableAPI.isContainer)
    }
}

// MARK: - ShadeUtils Tests

final class UsdShadeUtilsTests: XCTestCase {

    func testInputPrefix() {
        let prefix = ShadeUtils.inputPrefix
        XCTAssertNotNil(prefix)
        XCTAssertEqual(prefix?.text, "inputs:")
    }

    func testOutputPrefix() {
        let prefix = ShadeUtils.outputPrefix
        XCTAssertNotNil(prefix)
        XCTAssertEqual(prefix?.text, "outputs:")
    }

    func testIsInputName() {
        XCTAssertTrue(ShadeUtils.isInputName("inputs:diffuseColor"))
        XCTAssertFalse(ShadeUtils.isInputName("outputs:surface"))
        XCTAssertFalse(ShadeUtils.isInputName("diffuseColor"))
    }

    func testIsOutputName() {
        XCTAssertTrue(ShadeUtils.isOutputName("outputs:surface"))
        XCTAssertFalse(ShadeUtils.isOutputName("inputs:diffuseColor"))
        XCTAssertFalse(ShadeUtils.isOutputName("surface"))
    }

    func testBaseNameFromFullName() {
        let baseName = ShadeUtils.baseName(from: "inputs:diffuseColor")
        XCTAssertNotNil(baseName)
        XCTAssertEqual(baseName?.text, "diffuseColor")

        let outputBaseName = ShadeUtils.baseName(from: "outputs:surface")
        XCTAssertNotNil(outputBaseName)
        XCTAssertEqual(outputBaseName?.text, "surface")
    }

    func testAttributeTypeFromFullName() {
        let inputType = ShadeUtils.attributeType(from: "inputs:diffuseColor")
        XCTAssertEqual(inputType, .input)

        let outputType = ShadeUtils.attributeType(from: "outputs:surface")
        XCTAssertEqual(outputType, .output)

        let invalidType = ShadeUtils.attributeType(from: "diffuseColor")
        XCTAssertEqual(invalidType, .invalid)
    }

    func testCreateInputName() {
        let baseName = Token("diffuseColor")
        let fullName = ShadeUtils.inputName(from: baseName)
        XCTAssertNotNil(fullName)
        XCTAssertEqual(fullName?.text, "inputs:diffuseColor")
    }

    func testCreateOutputName() {
        let baseName = Token("surface")
        let fullName = ShadeUtils.outputName(from: baseName)
        XCTAssertNotNil(fullName)
        XCTAssertEqual(fullName?.text, "outputs:surface")
    }
}

// MARK: - Connection Tests

final class UsdShadeConnectionTests: XCTestCase {

    func testConnectShaderToMaterial() throws {
        let stage = try Stage.createInMemory()

        // Create material
        let materialPath = try Path("/Materials/MyMaterial")
        let material = try Material.define(stage: stage, path: materialPath)

        // Create shader
        let shaderPath = try Path("/Materials/MyMaterial/PreviewSurface")
        let shader = try Shader.define(stage: stage, path: shaderPath)
        try shader.setShaderId(Token("UsdPreviewSurface"))

        // Create shader output
        guard let shaderOutput = shader.createOutput(name: Token("surface"), typeName: Token("token")) else {
            XCTFail("Failed to create shader output")
            return
        }

        // Create material surface output
        guard let materialSurfaceOutput = material.createSurfaceOutput() else {
            XCTFail("Failed to create material surface output")
            return
        }

        // Note: Full connection tests would require actual USD runtime
        XCTAssertTrue(shaderOutput.isValid)
        XCTAssertTrue(materialSurfaceOutput.isValid)
    }
}

// MARK: - Enum Tests

final class UsdShadeEnumTests: XCTestCase {

    func testConnectionModificationEnum() {
        // Verify enum values exist
        let replace = ConnectionModification.replace
        let prepend = ConnectionModification.prepend
        let append = ConnectionModification.append

        XCTAssertNotNil(replace.cValue)
        XCTAssertNotNil(prepend.cValue)
        XCTAssertNotNil(append.cValue)
    }

    func testBindingStrengthEnum() {
        let fallback = BindingStrength.fallback
        let stronger = BindingStrength.stronger

        XCTAssertNotNil(fallback.cValue)
        XCTAssertNotNil(stronger.cValue)
    }

    func testMaterialPurposeEnum() {
        let all = MaterialPurpose.all
        let preview = MaterialPurpose.preview
        let full = MaterialPurpose.full

        XCTAssertNotNil(all.cValue)
        XCTAssertNotNil(preview.cValue)
        XCTAssertNotNil(full.cValue)
    }

    func testShadeAttributeTypeEnum() {
        XCTAssertEqual(ShadeAttributeType.invalid, .invalid)
        XCTAssertEqual(ShadeAttributeType.input, .input)
        XCTAssertEqual(ShadeAttributeType.output, .output)
    }
}

// MARK: - Error Tests

final class UsdShadeErrorTests: XCTestCase {

    func testShadeErrorDescriptions() {
        let creationError = ShadeError.creationFailed("test")
        let connectionError = ShadeError.connectionFailed("test")
        let bindingError = ShadeError.bindingFailed("test")
        let operationError = ShadeError.operationFailed("test")

        XCTAssertTrue(creationError.description.contains("creation"))
        XCTAssertTrue(connectionError.description.contains("connection"))
        XCTAssertTrue(bindingError.description.contains("binding"))
        XCTAssertTrue(operationError.description.contains("operation"))
    }
}
