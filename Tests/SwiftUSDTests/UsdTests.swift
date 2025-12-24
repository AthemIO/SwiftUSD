// UsdTests.swift - Tests for Usd module
// Tests Stage, Prim, Attribute, Relationship, Property
// Note: These tests are designed to pass in stub mode (without full USD library).
// More comprehensive tests require USE_PIXAR_USD to be defined.

import XCTest
@testable import SwiftUSD

// MARK: - TimeCode Tests

final class TimeCodeTests: XCTestCase {

    func testDefaultTimeCode() {
        let time = TimeCode.default
        XCTAssertTrue(time.isDefault)
    }

    func testTimeCodeWithValue() {
        let time = TimeCode(1.0)
        XCTAssertFalse(time.isDefault)
        XCTAssertEqual(time.time, 1.0)
    }

    func testTimeCodeFloatLiteral() {
        let time: TimeCode = 24.0
        XCTAssertEqual(time.time, 24.0)
        XCTAssertFalse(time.isDefault)
    }

    func testTimeCodeIntegerLiteral() {
        let time: TimeCode = 30
        XCTAssertEqual(time.time, 30.0)
        XCTAssertFalse(time.isDefault)
    }

    func testTimeCodeEquality() {
        let time1 = TimeCode(1.0)
        let time2 = TimeCode(1.0)
        let time3 = TimeCode(2.0)

        XCTAssertEqual(time1, time2)
        XCTAssertNotEqual(time1, time3)
    }

    func testTimeCodeHashing() {
        let time1 = TimeCode(1.0)
        let time2 = TimeCode(1.0)

        var set = Set<TimeCode>()
        set.insert(time1)
        set.insert(time2)

        XCTAssertEqual(set.count, 1)
    }
}

// MARK: - Specifier Tests

final class SpecifierTests: XCTestCase {

    func testSpecifierDescriptions() {
        XCTAssertEqual(Specifier.def.description, "def")
        XCTAssertEqual(Specifier.over.description, "over")
        XCTAssertEqual(Specifier.class.description, "class")
    }
}

// MARK: - Variability Tests

final class VariabilityTests: XCTestCase {

    func testVariabilityDescriptions() {
        XCTAssertEqual(Variability.varying.description, "varying")
        XCTAssertEqual(Variability.uniform.description, "uniform")
    }
}

// MARK: - ListPosition Tests

final class ListPositionTests: XCTestCase {

    func testListPositionDescriptions() {
        XCTAssertEqual(ListPosition.frontOfPrependList.description, "frontOfPrependList")
        XCTAssertEqual(ListPosition.backOfPrependList.description, "backOfPrependList")
        XCTAssertEqual(ListPosition.frontOfAppendList.description, "frontOfAppendList")
        XCTAssertEqual(ListPosition.backOfAppendList.description, "backOfAppendList")
    }
}

// MARK: - Stage Tests

final class StageTests: XCTestCase {

    func testCreateInMemoryStage() throws {
        let stage = try Stage.createInMemory()
        XCTAssertNotNil(stage)
    }

    func testCreateInMemoryStageWithIdentifier() throws {
        let stage = try Stage.createInMemory(identifier: "test.usda")
        XCTAssertNotNil(stage)
    }

    func testStageTimeProperties() throws {
        let stage = try Stage.createInMemory()

        stage.startTimeCode = 1.0
        stage.endTimeCode = 100.0
        stage.timeCodesPerSecond = 24.0
        stage.framesPerSecond = 24.0

        XCTAssertEqual(stage.startTimeCode, 1.0)
        XCTAssertEqual(stage.endTimeCode, 100.0)
        XCTAssertEqual(stage.timeCodesPerSecond, 24.0)
        XCTAssertEqual(stage.framesPerSecond, 24.0)
    }

    func testStageDescription() throws {
        let stage = try Stage.createInMemory(identifier: "test.usda")
        // Description should work even in stub mode
        let _ = stage.description
        let _ = stage.debugDescription
    }
}

// MARK: - Error Type Tests

final class ErrorTypeTests: XCTestCase {

    func testStageErrorDescriptions() {
        let error1 = StageError.creationFailed("test")
        XCTAssertTrue(error1.description.contains("creation failed"))

        let error2 = StageError.openFailed("test")
        XCTAssertTrue(error2.description.contains("open failed"))

        let error3 = StageError.primCreationFailed("test")
        XCTAssertTrue(error3.description.contains("creation failed"))

        let error4 = StageError.saveFailed("test")
        XCTAssertTrue(error4.description.contains("save failed"))

        let error5 = StageError.exportFailed("test")
        XCTAssertTrue(error5.description.contains("export failed"))

        let error6 = StageError.reloadFailed("test")
        XCTAssertTrue(error6.description.contains("reload failed"))

        let error7 = StageError.operationFailed("test")
        XCTAssertTrue(error7.description.contains("operation failed"))
    }

    func testPrimErrorDescriptions() {
        let error1 = PrimError.attributeCreationFailed("test")
        XCTAssertTrue(error1.description.contains("Attribute creation failed"))

        let error2 = PrimError.relationshipCreationFailed("test")
        XCTAssertTrue(error2.description.contains("Relationship creation failed"))

        let error3 = PrimError.operationFailed("test")
        XCTAssertTrue(error3.description.contains("operation failed"))
    }

    func testAttributeErrorDescriptions() {
        let error1 = AttributeError.getFailed("test")
        XCTAssertTrue(error1.description.contains("get failed"))

        let error2 = AttributeError.setFailed("test")
        XCTAssertTrue(error2.description.contains("set failed"))

        let error3 = AttributeError.typeMismatch("test")
        XCTAssertTrue(error3.description.contains("type mismatch"))
    }

    func testRelationshipErrorDescriptions() {
        let error1 = RelationshipError.addTargetFailed("test")
        XCTAssertTrue(error1.description.contains("add target failed"))

        let error2 = RelationshipError.removeTargetFailed("test")
        XCTAssertTrue(error2.description.contains("remove target failed"))

        let error3 = RelationshipError.setTargetsFailed("test")
        XCTAssertTrue(error3.description.contains("set targets failed"))
    }
}
