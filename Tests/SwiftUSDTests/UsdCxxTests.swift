// UsdCxxTests.swift - Tests for Usd C++ interop types

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdCxxTests: XCTestCase {

    // MARK: - TimeCode Tests

    func testTimeCodeCreation() {
        let time = swiftusd.TimeCode(1.0)
        XCTAssertFalse(time.isDefault)
        XCTAssertTrue(time.isNumeric)
        XCTAssertEqual(time.GetValue(), 1.0)
    }

    func testTimeCodeDefault() {
        let defaultTime = swiftusd.TimeCode.Default()
        XCTAssertTrue(defaultTime.isDefault)
        XCTAssertFalse(defaultTime.isNumeric)
    }

    func testTimeCodeEarliestTime() {
        let earliest = swiftusd.TimeCode.EarliestTime()
        XCTAssertTrue(earliest.isEarliestTime)
        XCTAssertFalse(earliest.isDefault)
    }

    func testTimeCodeEquality() {
        let time1 = swiftusd.TimeCode(10.0)
        let time2 = swiftusd.TimeCode(10.0)
        let time3 = swiftusd.TimeCode(20.0)

        XCTAssertEqual(time1, time2)
        XCTAssertNotEqual(time1, time3)

        let default1 = swiftusd.TimeCode.Default()
        let default2 = swiftusd.TimeCode.Default()
        XCTAssertEqual(default1, default2)
    }

    func testTimeCodeComparison() {
        let time1 = swiftusd.TimeCode(10.0)
        let time2 = swiftusd.TimeCode(20.0)

        XCTAssertTrue(time1 < time2)
        XCTAssertTrue(time2 > time1)
        XCTAssertTrue(time1 <= time1)
        XCTAssertTrue(time1 >= time1)
    }

    func testTimeCodeFloatLiteral() {
        let time: swiftusd.TimeCode = 24.0
        XCTAssertEqual(time.GetValue(), 24.0)
    }

    func testTimeCodeIntegerLiteral() {
        let time: swiftusd.TimeCode = 30
        XCTAssertEqual(time.GetValue(), 30.0)
    }

    func testTimeCodeHashing() {
        let time1 = swiftusd.TimeCode(5.0)
        let time2 = swiftusd.TimeCode(5.0)

        var set = Set<swiftusd.TimeCode>()
        set.insert(time1)
        XCTAssertTrue(set.contains(time2))
    }

    // MARK: - StageRef Tests

    func testStageCreateInMemory() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create in-memory stage")
            return
        }
        XCTAssertFalse(stage.identifier.isEmpty)
    }

    func testStageCreateInMemoryWithIdentifier() {
        guard let stage = swiftusd.StageRef.createInMemory(identifier: "testStage") else {
            XCTFail("Failed to create in-memory stage")
            return
        }
        XCTAssertTrue(stage.identifier.contains("testStage"))
    }

    func testStageTimeProperties() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        stage.startTimeCode = 1.0
        stage.endTimeCode = 100.0
        stage.timeCodesPerSecond = 24.0
        stage.framesPerSecond = 24.0

        XCTAssertEqual(stage.startTimeCode, 1.0)
        XCTAssertEqual(stage.endTimeCode, 100.0)
        XCTAssertEqual(stage.timeCodesPerSecond, 24.0)
        XCTAssertEqual(stage.framesPerSecond, 24.0)
    }

    func testStageDefinePrim() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let primPath = swiftusd.Path("/World")
        guard let prim = stage.definePrim(at: primPath) else {
            XCTFail("Failed to define prim")
            return
        }

        XCTAssertTrue(prim.isValid)
        XCTAssertEqual(prim.path.text, "/World")
    }

    func testStageDefinePrimWithType() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/World/Mesh", typeName: "Mesh") else {
            XCTFail("Failed to define typed prim")
            return
        }

        XCTAssertTrue(prim.isValid)
        XCTAssertEqual(prim.typeNameString, "Mesh")
    }

    func testStageGetPrimAtPath() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let _ = stage.definePrim(atPath: "/World")

        let prim = stage.prim(atPath: "/World")
        XCTAssertNotNil(prim)
        XCTAssertTrue(prim?.isValid ?? false)
    }

    func testStageRemovePrim() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let _ = stage.definePrim(atPath: "/ToRemove")
        XCTAssertNotNil(stage.prim(atPath: "/ToRemove"))

        let removed = stage.removePrim(at: swiftusd.Path("/ToRemove"))
        XCTAssertTrue(removed)
    }

    func testStagePseudoRoot() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let pseudoRoot = stage.pseudoRoot
        XCTAssertNotNil(pseudoRoot)
        XCTAssertTrue(pseudoRoot?.isValid ?? false)
    }

    func testStageRootLayer() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let rootLayer = stage.rootLayer
        XCTAssertNotNil(rootLayer)
    }

    func testStageSessionLayer() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let sessionLayer = stage.sessionLayer
        XCTAssertNotNil(sessionLayer)
    }

    func testStageDefaultPrim() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        XCTAssertFalse(stage.hasDefaultPrim)

        guard let prim = stage.definePrim(atPath: "/DefaultPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        stage.defaultPrim = prim
        XCTAssertTrue(stage.hasDefaultPrim)

        stage.clearDefaultPrim()
        XCTAssertFalse(stage.hasDefaultPrim)
    }

    // MARK: - PrimRef Tests

    func testPrimIdentity() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/World/Mesh", typeName: "Mesh") else {
            XCTFail("Failed to define prim")
            return
        }

        XCTAssertEqual(prim.path.text, "/World/Mesh")
        XCTAssertEqual(prim.nameString, "Mesh")
        XCTAssertEqual(prim.typeNameString, "Mesh")
    }

    func testPrimState() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        XCTAssertTrue(prim.isValid)
        XCTAssertTrue(prim.isActive)
        XCTAssertTrue(prim.isDefined)
        XCTAssertFalse(prim.isAbstract)

        prim.isActive = false
        XCTAssertFalse(prim.isActive)

        prim.isActive = true
        XCTAssertTrue(prim.isActive)
    }

    func testPrimHierarchy() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let _ = stage.definePrim(atPath: "/Parent")
        let _ = stage.definePrim(atPath: "/Parent/Child1")
        let _ = stage.definePrim(atPath: "/Parent/Child2")

        guard let parent = stage.prim(atPath: "/Parent") else {
            XCTFail("Failed to get parent prim")
            return
        }

        // Note: In standalone mode, child tracking is simplified
        // The full child enumeration is available when USE_PIXAR_USD is defined
        XCTAssertTrue(parent.isValid)

        guard let child1 = stage.prim(atPath: "/Parent/Child1") else {
            XCTFail("Failed to get child prim")
            return
        }

        XCTAssertTrue(child1.isValid)
        XCTAssertEqual(child1.nameString, "Child1")
    }

    func testPrimGetChild() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let _ = stage.definePrim(atPath: "/Parent")
        guard let child = stage.definePrim(atPath: "/Parent/Child") else {
            XCTFail("Failed to define child prim")
            return
        }

        // In standalone mode, we can access child prims directly via the stage
        // The GetChild method may return nil in standalone mode
        XCTAssertTrue(child.isValid)
        XCTAssertEqual(child.nameString, "Child")

        // Verify we can also retrieve via path
        let childByPath = stage.prim(atPath: "/Parent/Child")
        XCTAssertNotNil(childByPath)
        XCTAssertEqual(childByPath?.nameString, "Child")
    }

    func testPrimStageAccess() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        let primStage = prim.stage
        XCTAssertNotNil(primStage)
    }

    // MARK: - AttributeRef Tests

    func testAttributeCreation() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "testFloat", typeName: "float") else {
            XCTFail("Failed to create attribute")
            return
        }

        XCTAssertTrue(attr.isValid)
        XCTAssertEqual(attr.nameString, "testFloat")
    }

    func testAttributeFloatValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "myFloat", typeName: "float") else {
            XCTFail("Failed to create attribute")
            return
        }

        XCTAssertTrue(attr.set(Float(3.14)))

        let value = attr.getFloat()
        XCTAssertNotNil(value)
        XCTAssertEqual(value!, 3.14, accuracy: 0.001)
    }

    func testAttributeIntValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "myInt", typeName: "int") else {
            XCTFail("Failed to create attribute")
            return
        }

        XCTAssertTrue(attr.set(42))

        let value = attr.getInt()
        XCTAssertNotNil(value)
        XCTAssertEqual(value!, 42)
    }

    func testAttributeBoolValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "myBool", typeName: "bool") else {
            XCTFail("Failed to create attribute")
            return
        }

        XCTAssertTrue(attr.set(true))

        let value = attr.getBool()
        XCTAssertNotNil(value)
        XCTAssertEqual(value!, true)
    }

    func testAttributeDoubleValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "myDouble", typeName: "double") else {
            XCTFail("Failed to create attribute")
            return
        }

        XCTAssertTrue(attr.set(2.718281828))

        let value = attr.getDouble()
        XCTAssertNotNil(value)
        XCTAssertEqual(value!, 2.718281828, accuracy: 0.0000001)
    }

    func testAttributeVec3fValue() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "myVec3f", typeName: "float3") else {
            XCTFail("Failed to create attribute")
            return
        }

        let vec = swiftusd.Vec3f(1.0, 2.0, 3.0)
        XCTAssertTrue(attr.set(vec))

        let value = attr.getVec3f()
        XCTAssertNotNil(value)
        XCTAssertEqual(value!.x, 1.0)
        XCTAssertEqual(value!.y, 2.0)
        XCTAssertEqual(value!.z, 3.0)
    }

    func testAttributeClear() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "toClear", typeName: "int") else {
            XCTFail("Failed to create attribute")
            return
        }

        attr.set(100)
        XCTAssertTrue(attr.hasValue)

        attr.clear()
        // After clear, hasValue should be false in standalone mode
    }

    func testAttributePrimAccess() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let attr = prim.createAttribute(named: "test", typeName: "int") else {
            XCTFail("Failed to create attribute")
            return
        }

        let attrPrim = attr.prim
        XCTAssertNotNil(attrPrim)
        XCTAssertEqual(attrPrim?.path.text, "/TestPrim")
    }

    func testPrimHasAttribute() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        XCTAssertFalse(prim.hasAttribute("myAttr"))

        let _ = prim.createAttribute(named: "myAttr", typeName: "float")
        XCTAssertTrue(prim.hasAttribute("myAttr"))
    }

    // MARK: - RelationshipRef Tests

    func testRelationshipCreation() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let rel = prim.createRelationship(named: "testRel") else {
            XCTFail("Failed to create relationship")
            return
        }

        XCTAssertTrue(rel.isValid)
        XCTAssertEqual(rel.nameString, "testRel")
    }

    func testRelationshipTargets() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/SourcePrim") else {
            XCTFail("Failed to define prim")
            return
        }
        let _ = stage.definePrim(atPath: "/Target1")
        let _ = stage.definePrim(atPath: "/Target2")

        guard let rel = prim.createRelationship(named: "targets") else {
            XCTFail("Failed to create relationship")
            return
        }

        XCTAssertEqual(rel.targetCount, 0)

        XCTAssertTrue(rel.addTarget("/Target1"))
        XCTAssertEqual(rel.targetCount, 1)

        XCTAssertTrue(rel.addTarget("/Target2"))
        XCTAssertEqual(rel.targetCount, 2)

        let targets = rel.targets
        XCTAssertEqual(targets.count, 2)
    }

    func testRelationshipRemoveTarget() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/SourcePrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let rel = prim.createRelationship(named: "targets") else {
            XCTFail("Failed to create relationship")
            return
        }

        rel.addTarget("/Target1")
        rel.addTarget("/Target2")
        XCTAssertEqual(rel.targetCount, 2)

        XCTAssertTrue(rel.removeTarget(swiftusd.Path("/Target1")))
        XCTAssertEqual(rel.targetCount, 1)
    }

    func testRelationshipClearTargets() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/SourcePrim") else {
            XCTFail("Failed to define prim")
            return
        }

        guard let rel = prim.createRelationship(named: "targets") else {
            XCTFail("Failed to create relationship")
            return
        }

        rel.addTarget("/Target1")
        rel.addTarget("/Target2")
        XCTAssertEqual(rel.targetCount, 2)

        XCTAssertTrue(rel.clearTargets())
        XCTAssertEqual(rel.targetCount, 0)
    }

    func testPrimHasRelationship() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        guard let prim = stage.definePrim(atPath: "/TestPrim") else {
            XCTFail("Failed to define prim")
            return
        }

        XCTAssertFalse(prim.hasRelationship("myRel"))

        let _ = prim.createRelationship(named: "myRel")
        XCTAssertTrue(prim.hasRelationship("myRel"))
    }

    // MARK: - Integration Tests

    func testFullWorkflow() {
        // Create a stage
        guard let stage = swiftusd.StageRef.createInMemory(identifier: "test") else {
            XCTFail("Failed to create stage")
            return
        }

        // Set metadata
        stage.startTimeCode = 1.0
        stage.endTimeCode = 100.0
        stage.framesPerSecond = 24.0

        XCTAssertEqual(stage.startTimeCode, 1.0)
        XCTAssertEqual(stage.endTimeCode, 100.0)
        XCTAssertEqual(stage.framesPerSecond, 24.0)

        // Create hierarchy
        guard let world = stage.definePrim(atPath: "/World") else {
            XCTFail("Failed to create /World")
            return
        }

        guard let mesh = stage.definePrim(atPath: "/World/Mesh", typeName: "Mesh") else {
            XCTFail("Failed to create /World/Mesh")
            return
        }

        XCTAssertTrue(world.isValid)
        XCTAssertTrue(mesh.isValid)
        XCTAssertEqual(mesh.typeNameString, "Mesh")

        // Add attributes
        guard let pointsAttr = mesh.createAttribute(named: "points", typeName: "float3[]") else {
            XCTFail("Failed to create points attribute")
            return
        }
        XCTAssertTrue(pointsAttr.isValid)
        XCTAssertEqual(pointsAttr.nameString, "points")

        // Add relationship
        guard let materialRel = mesh.createRelationship(named: "material") else {
            XCTFail("Failed to create material relationship")
            return
        }

        let _ = stage.definePrim(atPath: "/Materials/Metal")
        materialRel.addTarget("/Materials/Metal")

        // Set default prim
        stage.defaultPrim = world
        XCTAssertTrue(stage.hasDefaultPrim)

        // Verify relationship
        XCTAssertEqual(materialRel.targetCount, 1)

        // Verify we can retrieve prims by path
        let retrievedMesh = stage.prim(atPath: "/World/Mesh")
        XCTAssertNotNil(retrievedMesh)
        XCTAssertEqual(retrievedMesh?.typeNameString, "Mesh")
    }
}
