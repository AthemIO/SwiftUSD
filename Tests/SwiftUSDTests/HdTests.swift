// HdTests.swift - Tests for Hydra Core Framework namespace and errors

import XCTest
@testable import SwiftUSD

final class HdTests: XCTestCase {

    // MARK: - HdError Tests

    func testHdErrorCreationFailed() {
        let error = HdError.creationFailed("test message")
        XCTAssertEqual(error.description, "Hd creation failed: test message")
    }

    func testHdErrorOperationFailed() {
        let error = HdError.operationFailed("operation failed")
        XCTAssertEqual(error.description, "Hd operation failed: operation failed")
    }

    func testHdErrorInvalidArgument() {
        let error = HdError.invalidArgument("bad argument")
        XCTAssertEqual(error.description, "Hd invalid argument: bad argument")
    }

    func testHdErrorRenderFailed() {
        let error = HdError.renderFailed("render error")
        XCTAssertEqual(error.description, "Hd render failed: render error")
    }

    func testHdErrorIsError() {
        let error: Error = HdError.creationFailed("test")
        XCTAssertTrue(error is HdError)
    }

    func testHdErrorCustomStringConvertible() {
        let error = HdError.operationFailed("test")
        let description = String(describing: error)
        XCTAssertTrue(description.contains("Hd operation failed"))
    }

    // MARK: - Namespace Tests

    func testHdNamespaceExists() {
        // Verify the Hd namespace enum exists and can be referenced
        _ = Hd.self
    }
}
