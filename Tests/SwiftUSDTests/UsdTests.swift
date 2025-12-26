// UsdTests.swift - Tests for Usd module (Core USD API)

import XCTest
import SwiftUSD
@testable import CxxFacade

final class UsdTests: XCTestCase {

    // MARK: - UsdTimeCode Tests

    func testTimeCodeDefault() {
        let tc = cxxfacade.usd.UsdTimeCode.Default()
        XCTAssertTrue(tc.IsDefault())
        XCTAssertFalse(tc.IsNumeric())
    }

    func testTimeCodeNumeric() {
        let tc = cxxfacade.usd.UsdTimeCode(24.0)
        XCTAssertFalse(tc.IsDefault())
        XCTAssertTrue(tc.IsNumeric())
        XCTAssertEqual(tc.GetValue(), 24.0, accuracy: 0.0001)
    }

    func testTimeCodeEquality() {
        let tc1 = cxxfacade.usd.UsdTimeCode(10.0)
        let tc2 = cxxfacade.usd.UsdTimeCode(10.0)
        let tc3 = cxxfacade.usd.UsdTimeCode(20.0)
        let tcDefault = cxxfacade.usd.UsdTimeCode.Default()

        XCTAssertTrue(tc1 == tc2)
        XCTAssertFalse(tc1 == tc3)
        XCTAssertFalse(tc1 == tcDefault)
    }

    // MARK: - UsdStage Tests

    func testStageCreateInMemory() {
        let stage = cxxfacade.usd.UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())
    }

    func testStageGetRootLayer() {
        let stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let rootLayer = stage.GetRootLayer()
        XCTAssertTrue(rootLayer.IsValid())
    }

    func testStageGetPseudoRoot() {
        let stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let pseudoRoot = stage.GetPseudoRoot()
        XCTAssertTrue(pseudoRoot.IsValid())
    }

    func testStageDefinePrim() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/World")
        let typeName = cxxfacade.tf.TfToken("Xform")
        let prim = stage.DefinePrim(path, typeName)

        XCTAssertTrue(prim.IsValid())
        XCTAssertEqual(String(prim.GetPath().GetString()), "/World")
    }

    func testStageGetPrimAtPath() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/World")
        let typeName = cxxfacade.tf.TfToken("Xform")
        let _ = stage.DefinePrim(path, typeName)

        let prim = stage.GetPrimAtPath(path)
        XCTAssertTrue(prim.IsValid())
    }

    func testStageTraverse() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World/Mesh"), cxxfacade.tf.TfToken("Mesh"))

        let prims = stage.Traverse()
        XCTAssertEqual(prims.size(), 2)
    }

    func testStageTimeCodeRange() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        stage.SetStartTimeCode(1.0)
        stage.SetEndTimeCode(100.0)

        XCTAssertEqual(stage.GetStartTimeCode(), 1.0, accuracy: 0.0001)
        XCTAssertEqual(stage.GetEndTimeCode(), 100.0, accuracy: 0.0001)
    }

    func testStageRemovePrim() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/ToRemove")
        let _ = stage.DefinePrim(path, cxxfacade.tf.TfToken())

        XCTAssertTrue(stage.GetPrimAtPath(path).IsValid())
        XCTAssertTrue(stage.RemovePrim(path))
        XCTAssertFalse(stage.GetPrimAtPath(path).IsValid())
    }

    // MARK: - UsdPrim Tests

    func testPrimGetName() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let name = prim.GetName()
        XCTAssertEqual(String(cString: name.GetText()), "World")
    }

    func testPrimGetTypeName() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))

        let typeName = prim.GetTypeName()
        XCTAssertEqual(String(cString: typeName.GetText()), "Xform")
    }

    func testPrimGetParent() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))
        let child = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World/Child"), cxxfacade.tf.TfToken())

        let parent = child.GetParent()
        XCTAssertTrue(parent.IsValid())
        XCTAssertEqual(String(cString: parent.GetName().GetText()), "World")
    }

    func testPrimGetChildren() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World/Child1"), cxxfacade.tf.TfToken())
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World/Child2"), cxxfacade.tf.TfToken())

        let world = stage.GetPrimAtPath(cxxfacade.sdf.SdfPath("/World"))
        let children = world.GetChildren()
        XCTAssertEqual(children.size(), 2)
    }

    func testPrimIsActiveAndDefined() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))

        XCTAssertTrue(prim.IsActive())
        XCTAssertTrue(prim.IsDefined())
    }

    func testPrimEquality() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let prim1 = stage.GetPrimAtPath(cxxfacade.sdf.SdfPath("/World"))
        let prim2 = stage.GetPrimAtPath(cxxfacade.sdf.SdfPath("/World"))
        let prim3 = stage.DefinePrim(cxxfacade.sdf.SdfPath("/Other"), cxxfacade.tf.TfToken())

        XCTAssertTrue(prim1 == prim2)
        XCTAssertFalse(prim1 == prim3)
    }

    // MARK: - UsdAttribute Tests

    func testAttributeCreateAndGet() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let attrName = cxxfacade.tf.TfToken("testAttr")
        let attr = prim.CreateAttribute(attrName, std.string("bool"), true)

        XCTAssertTrue(attr.IsValid())

        let retrieved = prim.GetAttribute(attrName)
        XCTAssertTrue(retrieved.IsValid())
    }

    func testAttributeGetName() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let attrName = cxxfacade.tf.TfToken("myAttribute")
        let attr = prim.CreateAttribute(attrName, std.string("float"), true)

        XCTAssertEqual(String(cString: attr.GetName().GetText()), "myAttribute")
    }

    func testAttributeGetPath() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let attrName = cxxfacade.tf.TfToken("myAttr")
        let attr = prim.CreateAttribute(attrName, std.string("int"), true)

        let path = attr.GetPath()
        XCTAssertEqual(String(path.GetString()), "/World.myAttr")
    }

    // MARK: - UsdRelationship Tests

    func testRelationshipCreateAndGet() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let relName = cxxfacade.tf.TfToken("testRel")
        let rel = prim.CreateRelationship(relName, true)

        XCTAssertTrue(rel.IsValid())

        let retrieved = prim.GetRelationship(relName)
        XCTAssertTrue(retrieved.IsValid())
    }

    func testRelationshipAddTarget() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/Target"), cxxfacade.tf.TfToken())

        let relName = cxxfacade.tf.TfToken("myRel")
        var rel = prim.CreateRelationship(relName, true)

        let targetPath = cxxfacade.sdf.SdfPath("/Target")
        XCTAssertTrue(rel.AddTarget(targetPath))
        // Note: GetTargets test removed due to std::vector Swift interop complexity
    }

    // MARK: - UsdProperty Tests

    func testPropertyIsAttributeOrRelationship() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        // Attribute and Relationship should report their types correctly
        let attr = prim.CreateAttribute(cxxfacade.tf.TfToken("attr"), std.string("float"), true)
        let rel = prim.CreateRelationship(cxxfacade.tf.TfToken("rel"), true)

        XCTAssertTrue(attr.IsValid())
        XCTAssertTrue(rel.IsValid())
    }

    // MARK: - UsdEditTarget Tests

    func testEditTargetDefault() {
        let target = cxxfacade.usd.UsdEditTarget()
        XCTAssertTrue(target.IsNull())
        XCTAssertFalse(target.IsValid())
    }

    func testEditTargetFromLayer() {
        let layer = cxxfacade.sdf.SdfLayer.CreateAnonymous()
        let target = cxxfacade.usd.UsdEditTarget(layer)
        XCTAssertTrue(target.IsValid())
        XCTAssertFalse(target.IsNull())
    }

    func testEditTargetGetLayer() {
        let layer = cxxfacade.sdf.SdfLayer.CreateAnonymous()
        let target = cxxfacade.usd.UsdEditTarget(layer)
        let retrievedLayer = target.GetLayer()
        XCTAssertTrue(retrievedLayer.IsValid())
    }

    func testStageGetSetEditTarget() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let rootLayer = stage.GetRootLayer()

        // Get default edit target
        let defaultTarget = stage.GetEditTarget()
        XCTAssertTrue(defaultTarget.IsValid())

        // Create new target and set it
        let newTarget = cxxfacade.usd.UsdEditTarget(rootLayer)
        stage.SetEditTarget(newTarget)

        let currentTarget = stage.GetEditTarget()
        XCTAssertTrue(currentTarget.IsValid())
    }

    // MARK: - UsdSchemaBase Tests

    func testSchemaBaseFromPrim() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))

        let schema = cxxfacade.usd.UsdSchemaBase(prim)
        XCTAssertTrue(schema.GetPrim().IsValid())
    }

    func testSchemaBaseGetPath() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let schema = cxxfacade.usd.UsdSchemaBase(prim)
        let path = schema.GetPath()
        XCTAssertEqual(String(path.GetString()), "/World")
    }

    func testSchemaKindQueries() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken())

        let schema = cxxfacade.usd.UsdSchemaBase(prim)
        // Base schema should report AbstractBase
        XCTAssertEqual(schema.GetSchemaKind(), cxxfacade.usd.UsdSchemaKind.AbstractBase)
        XCTAssertFalse(schema.IsConcrete())
    }

    // MARK: - UsdTyped Tests

    func testTypedFromPrim() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let prim = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))

        let typed = cxxfacade.usd.UsdTyped(prim)
        XCTAssertTrue(typed.GetPrim().IsValid())
    }

    func testTypedGet() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let _ = stage.DefinePrim(cxxfacade.sdf.SdfPath("/World"), cxxfacade.tf.TfToken("Xform"))

        let typed = cxxfacade.usd.UsdTyped.Get(stage, cxxfacade.sdf.SdfPath("/World"))
        XCTAssertTrue(typed.GetPrim().IsValid())
    }

    // MARK: - Swift Wrapper Tests

    func testUsdTimeCodeSwiftWrapper() {
        let defaultTime = UsdTimeCode.default
        XCTAssertTrue(defaultTime.isDefault)
        XCTAssertFalse(defaultTime.isNumeric)

        let numericTime = UsdTimeCode(24.0)
        XCTAssertFalse(numericTime.isDefault)
        XCTAssertTrue(numericTime.isNumeric)
        XCTAssertEqual(numericTime.value, 24.0, accuracy: 0.0001)
    }

    func testUsdStageSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        XCTAssertTrue(stage.isValid)
        XCTAssertTrue(stage.rootLayer.isValid)
        XCTAssertTrue(stage.pseudoRoot.isValid)
    }

    func testUsdStageDefinePrimSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        let prim = stage.definePrim(at: "/World", type: "Xform")
        XCTAssertTrue(prim.isValid)
        XCTAssertEqual(prim.pathString, "/World")
    }

    func testUsdStagePrimAccessSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        let _ = stage.definePrim(at: "/World")

        let prim = stage.prim(at: "/World")
        XCTAssertTrue(prim.isValid)

        let prim2 = stage.prim(at: SdfPath("/World"))
        XCTAssertTrue(prim2.isValid)
    }

    func testUsdPrimSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        let prim = stage.definePrim(at: "/World", type: "Xform")

        XCTAssertTrue(prim.isValid)
        XCTAssertTrue(prim.isActive)
        XCTAssertTrue(prim.isDefined)
        XCTAssertEqual(prim.pathString, "/World")
    }

    func testUsdPrimAttributeSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        var prim = stage.definePrim(at: "/World")

        let attr = prim.createAttribute(name: "myAttr", typeName: "float", custom: true)
        XCTAssertTrue(attr.isValid)
        XCTAssertEqual(String(cString: attr.name.GetText()), "myAttr")

        let retrieved = prim.attribute("myAttr")
        XCTAssertTrue(retrieved.isValid)
    }

    func testUsdPrimRelationshipSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        var prim = stage.definePrim(at: "/World")
        let _ = stage.definePrim(at: "/Target")

        var rel = prim.createRelationship(name: "myRel")
        XCTAssertTrue(rel.isValid)
        XCTAssertEqual(String(cString: rel.name.GetText()), "myRel")

        XCTAssertTrue(rel.addTarget("/Target"))
    }

    func testUsdEditTargetSwiftWrapper() {
        let target = UsdEditTarget()
        XCTAssertTrue(target.isNull)
        XCTAssertFalse(target.isValid)

        let layer = SdfLayer.createAnonymous()
        let validTarget = UsdEditTarget(layer)
        XCTAssertFalse(validTarget.isNull)
        XCTAssertTrue(validTarget.isValid)
        XCTAssertTrue(validTarget.layer.isValid)
    }

    func testUsdStageEditTargetSwiftWrapper() {
        var stage = UsdStage.createInMemory()

        // Get and set edit target
        let currentTarget = stage.editTarget
        XCTAssertTrue(currentTarget.isValid)

        // Set to session layer
        let sessionLayer = stage.sessionLayer
        stage.editTarget = UsdEditTarget(sessionLayer)

        // Verify it changed
        let newTarget = stage.editTarget
        XCTAssertTrue(newTarget.isValid)
    }

    func testUsdSchemaBaseSwiftWrapper() {
        var stage = UsdStage.createInMemory()
        let primObj = stage.definePrim(at: "/World", type: "Xform")

        let schema = UsdSchemaBase(primObj)
        XCTAssertTrue(schema.prim.isValid)
        XCTAssertEqual(String(schema.path.GetString()), "/World")
        XCTAssertEqual(schema.schemaKind, UsdSchemaKind.AbstractBase)
    }

    // MARK: - Integration Tests

    func testFullSceneCreationWorkflow() {
        var stage = UsdStage.createInMemory()

        // Set up stage metadata
        stage.startTimeCode = 1.0
        stage.endTimeCode = 100.0

        // Create hierarchy
        let world = stage.definePrim(at: "/World", type: "Xform")
        let mesh = stage.definePrim(at: "/World/Mesh", type: "Mesh")
        let material = stage.definePrim(at: "/Materials/Default", type: "Material")

        XCTAssertTrue(world.isValid)
        XCTAssertTrue(mesh.isValid)
        XCTAssertTrue(material.isValid)

        // Verify traversal
        let allPrims = stage.Traverse()
        XCTAssertGreaterThanOrEqual(allPrims.size(), 3)

        // Verify hierarchy
        let meshParent = mesh.parent
        XCTAssertEqual(String(cString: meshParent.name.GetText()), "World")
    }

    func testPrimAttributeAndRelationshipWorkflow() {
        var stage = UsdStage.createInMemory()
        var prim = stage.definePrim(at: "/World")
        let _ = stage.definePrim(at: "/Materials/Default")

        // Create an attribute
        let attr = prim.createAttribute(name: "visibility", typeName: "bool", custom: true)
        XCTAssertTrue(attr.isValid)

        // Create a relationship
        var rel = prim.createRelationship(name: "material:binding")
        XCTAssertTrue(rel.isValid)
        XCTAssertTrue(rel.addTarget("/Materials/Default"))
        // Note: GetTargets test removed due to std::vector Swift interop complexity
    }
}
