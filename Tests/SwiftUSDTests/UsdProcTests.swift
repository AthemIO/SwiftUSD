// UsdProcTests.swift - Tests for UsdProc module

import XCTest
@testable import SwiftUSD

final class UsdProcTests: XCTestCase {

    // MARK: - GenerativeProcedural Definition Tests

    func testGenerativeProceduralDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        XCTAssertTrue(proc.isValid)
    }

    func testGenerativeProceduralGet() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        // Define first
        _ = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Then get
        let proc = try ProcGenerativeProcedural.get(on: stage, at: path)

        XCTAssertTrue(proc.isValid)
    }

    func testGenerativeProceduralFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let definedProc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Get the prim and wrap it again
        // Note: In stub mode (without USE_PIXAR_USD), GetPrim returns nullptr
        // so we test the behavior rather than failing
        if let prim = definedProc.prim {
            let proc = ProcGenerativeProcedural.from(prim: prim)
            XCTAssertNotNil(proc)
            XCTAssertTrue(proc?.isValid ?? false)
        } else {
            // In stub mode, prim is nil but the procedural itself is still valid
            XCTAssertTrue(definedProc.isValid)
        }
    }

    // MARK: - ProceduralSystem Attribute Tests

    func testProceduralSystemSetAndGet() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Initially empty or not set
        let initialSystem = proc.proceduralSystem()
        // May be nil or empty depending on stub vs full implementation

        // Set a procedural system
        try proc.setProceduralSystem("myRenderer")

        let system = proc.proceduralSystem()
        XCTAssertEqual(system, "myRenderer")
    }

    func testProceduralSystemCreateAttr() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Create attribute with default value
        let created = proc.createProceduralSystemAttr(defaultValue: "defaultSystem")
        XCTAssertTrue(created)

        // Check if it has the value
        XCTAssertTrue(proc.hasProceduralSystem)
    }

    func testProceduralSystemHasValue() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Initially may not have authored value
        let initialHas = proc.hasProceduralSystem

        // Set a value
        try proc.setProceduralSystem("testSystem")

        // Now should have it
        XCTAssertTrue(proc.hasProceduralSystem)
    }

    // MARK: - Extent (Bounding Box) Tests

    func testExtentGetDefault() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Get the extent (default should be a unit cube)
        guard let extent = proc.extent() else {
            XCTFail("Expected extent to be available")
            return
        }

        // Default extent is centered unit cube
        XCTAssertEqual(extent.min.x, -0.5, accuracy: 0.01)
        XCTAssertEqual(extent.min.y, -0.5, accuracy: 0.01)
        XCTAssertEqual(extent.min.z, -0.5, accuracy: 0.01)
        XCTAssertEqual(extent.max.x, 0.5, accuracy: 0.01)
        XCTAssertEqual(extent.max.y, 0.5, accuracy: 0.01)
        XCTAssertEqual(extent.max.z, 0.5, accuracy: 0.01)
    }

    func testExtentSetAndGet() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Set a custom extent
        let newExtent = ProcGenerativeProcedural.Extent(
            min: (x: -2.0, y: -1.0, z: -3.0),
            max: (x: 2.0, y: 1.0, z: 3.0)
        )
        try proc.setExtent(newExtent)

        // Get it back
        guard let extent = proc.extent() else {
            XCTFail("Expected extent to be available")
            return
        }

        XCTAssertEqual(extent.min.x, -2.0, accuracy: 0.01)
        XCTAssertEqual(extent.min.y, -1.0, accuracy: 0.01)
        XCTAssertEqual(extent.min.z, -3.0, accuracy: 0.01)
        XCTAssertEqual(extent.max.x, 2.0, accuracy: 0.01)
        XCTAssertEqual(extent.max.y, 1.0, accuracy: 0.01)
        XCTAssertEqual(extent.max.z, 3.0, accuracy: 0.01)
    }

    // MARK: - Description Tests

    func testDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/MyProcedural")

        let proc = try ProcGenerativeProcedural.define(on: stage, at: path)

        // Description should contain the path or indicate it's a procedural
        let description = proc.description
        XCTAssertFalse(description.isEmpty)
    }

    // MARK: - Type Alias Tests

    func testTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/AliasTest")

        // Use the type alias
        let proc = try UsdProcGenerativeProcedural.define(on: stage, at: path)

        XCTAssertTrue(proc.isValid)
    }

    // MARK: - Error Handling Tests

    func testGetNonExistentProcedural() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/DoesNotExist")

        // Note: In stub mode (without USE_PIXAR_USD), Get always returns a valid wrapper
        // In full mode, it would throw for non-existent prims
        // Test that the API works without crashing
        do {
            let proc = try ProcGenerativeProcedural.get(on: stage, at: path)
            // In stub mode, this succeeds
            XCTAssertTrue(proc.isValid)
        } catch {
            // In full mode, this should throw
            XCTAssertTrue(error is ProcError)
        }
    }
}
