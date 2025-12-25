// HdEngineTests.swift - Tests for HdEngine Swift bindings

import XCTest
import USDCxx
@testable import SwiftUSD

final class HdEngineTests: XCTestCase {

    // MARK: - HdEngine Creation Tests

    func testEngineCreation() {
        guard let engine = swiftusd.HdEngine.Create() else {
            XCTFail("Failed to create HdEngine")
            return
        }

        XCTAssertNotNil(engine)
    }

    func testEngineCreationWithConvenienceMethod() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine with convenience method")
            return
        }

        XCTAssertNotNil(engine)
    }

    // MARK: - Task Context Tests

    func testTaskContextSetAndClear() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Set some context data
        engine.setTaskContextData(key: "testKey", value: "testValue")

        // Verify it was set
        XCTAssertTrue(engine.hasTaskContextData(key: "testKey"))

        // Clear all context data
        engine.clearTaskContext()

        // Verify it's gone
        XCTAssertFalse(engine.hasTaskContextData(key: "testKey"))
    }

    func testTaskContextGetValue() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Set a value
        engine.setTaskContextData(key: "camera", value: "perspective")

        // Get it back
        let value = engine.getTaskContextData(key: "camera")
        XCTAssertEqual(value, "perspective")

        // Non-existent key should return empty string
        let missing = engine.getTaskContextData(key: "nonexistent")
        XCTAssertEqual(missing, "")
    }

    func testTaskContextRemove() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Set multiple values
        engine.setTaskContextData(key: "key1", value: "value1")
        engine.setTaskContextData(key: "key2", value: "value2")

        XCTAssertTrue(engine.hasTaskContextData(key: "key1"))
        XCTAssertTrue(engine.hasTaskContextData(key: "key2"))

        // Remove one
        engine.removeTaskContextData(key: "key1")

        XCTAssertFalse(engine.hasTaskContextData(key: "key1"))
        XCTAssertTrue(engine.hasTaskContextData(key: "key2"))
    }

    func testTaskContextWithTokens() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        let token = swiftusd.Token("viewport")
        engine.setTaskContextData(id: token, value: "1920x1080")

        XCTAssertTrue(engine.hasTaskContextData(id: token))
        XCTAssertEqual(engine.getTaskContextData(id: token), "1920x1080")
    }

    func testTaskContextSize() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Initially empty
        XCTAssertEqual(engine.taskContextSize, 0)

        // Add items
        engine.setTaskContextData(key: "key1", value: "value1")
        engine.setTaskContextData(key: "key2", value: "value2")
        engine.setTaskContextData(key: "key3", value: "value3")

        XCTAssertEqual(engine.taskContextSize, 3)

        // Clear and verify
        engine.clearTaskContext()
        XCTAssertEqual(engine.taskContextSize, 0)
    }

    // MARK: - Execution Tests

    func testExecuteWithNullIndex() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Execute with null index should not crash
        // This is a no-op in the implementation
        engine.Execute(nil)

        XCTAssertEqual(engine.lastExecutedTaskCount, 0)
    }

    func testExecuteTaskPathsEmpty() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Execute with empty task paths
        let emptyPaths: [swiftusd.Path] = []
        engine.ExecuteTaskPaths(nil, nil, 0)

        XCTAssertEqual(engine.lastExecutedTaskCount, 0)
    }

    func testAreTasksConvergedEmpty() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Empty task list should be considered converged
        let emptyPaths: [swiftusd.Path] = []
        let converged = engine.areTasksConverged(renderIndex: nil!, taskPaths: emptyPaths)

        // With nil index, should return true (default)
        XCTAssertTrue(converged)
    }

    // MARK: - Reference Counting Tests

    func testEngineReferenceCountingBasic() {
        // Create an engine
        guard let engine = swiftusd.HdEngine.Create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Retain and release should work without crashing
        engine.retain()
        engine.release()

        // Engine should still be valid
        XCTAssertNotNil(engine)
    }

    // MARK: - HdTaskRef Tests

    func testTaskRefProperties() {
        // HdTaskRef requires internal creation, test basic properties
        // In standalone mode, we can't easily create tasks
        // This tests the type exists and compiles
        _ = swiftusd.HdTaskRef.self
    }

    // MARK: - Integration Tests

    func testEngineWorkflow() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Simulate a typical workflow

        // 1. Set up task context with render parameters
        engine.setTaskContextData(key: "camera", value: "/World/Camera")
        engine.setTaskContextData(key: "resolution", value: "1920x1080")
        engine.setTaskContextData(key: "samples", value: "64")

        // 2. Verify context is set up
        XCTAssertEqual(engine.taskContextSize, 3)
        XCTAssertTrue(engine.hasTaskContextData(key: "camera"))
        XCTAssertEqual(engine.getTaskContextData(key: "resolution"), "1920x1080")

        // 3. Execute would normally happen here with a render index
        // engine.execute(renderIndex: index)

        // 4. Clear context for next frame
        engine.clearTaskContext()
        XCTAssertEqual(engine.taskContextSize, 0)
    }

    func testMultipleEngines() {
        // Test that multiple engines can coexist
        guard let engine1 = swiftusd.HdEngine.create(),
              let engine2 = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engines")
            return
        }

        // Each engine should have independent task context
        engine1.setTaskContextData(key: "renderer", value: "engine1")
        engine2.setTaskContextData(key: "renderer", value: "engine2")

        XCTAssertEqual(engine1.getTaskContextData(key: "renderer"), "engine1")
        XCTAssertEqual(engine2.getTaskContextData(key: "renderer"), "engine2")

        // Clearing one shouldn't affect the other
        engine1.clearTaskContext()
        XCTAssertEqual(engine1.taskContextSize, 0)
        XCTAssertEqual(engine2.getTaskContextData(key: "renderer"), "engine2")
    }

    func testTaskContextOverwrite() {
        guard let engine = swiftusd.HdEngine.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Set initial value
        engine.setTaskContextData(key: "setting", value: "initial")
        XCTAssertEqual(engine.getTaskContextData(key: "setting"), "initial")

        // Overwrite with new value
        engine.setTaskContextData(key: "setting", value: "updated")
        XCTAssertEqual(engine.getTaskContextData(key: "setting"), "updated")

        // Size should still be 1
        XCTAssertEqual(engine.taskContextSize, 1)
    }
}
