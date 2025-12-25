// HdStTests.swift - Tests for Storm Renderer namespace and errors

import XCTest
@testable import SwiftUSD

final class HdStTests: XCTestCase {

    // MARK: - HdStError Tests

    func testHdStErrorCreationFailed() {
        let error = HdStError.creationFailed("test message")
        XCTAssertEqual(error.description, "HdSt creation failed: test message")
    }

    func testHdStErrorOperationFailed() {
        let error = HdStError.operationFailed("operation failed")
        XCTAssertEqual(error.description, "HdSt operation failed: operation failed")
    }

    func testHdStErrorInvalidArgument() {
        let error = HdStError.invalidArgument("bad argument")
        XCTAssertEqual(error.description, "HdSt invalid argument: bad argument")
    }

    func testHdStErrorRenderFailed() {
        let error = HdStError.renderFailed("render error")
        XCTAssertEqual(error.description, "HdSt render failed: render error")
    }

    func testHdStErrorNotSupported() {
        let error = HdStError.notSupported("GPU not supported")
        XCTAssertEqual(error.description, "HdSt not supported: GPU not supported")
    }

    func testHdStErrorResourceAllocationFailed() {
        let error = HdStError.resourceAllocationFailed("out of memory")
        XCTAssertEqual(error.description, "HdSt resource allocation failed: out of memory")
    }

    func testHdStErrorIsError() {
        let error: Error = HdStError.creationFailed("test")
        XCTAssertTrue(error is HdStError)
    }

    func testHdStErrorCustomStringConvertible() {
        let error = HdStError.operationFailed("test")
        let description = String(describing: error)
        XCTAssertTrue(description.contains("HdSt operation failed"))
    }

    // MARK: - Namespace Tests

    func testHdStNamespaceExists() {
        // Verify the HdSt namespace enum exists and can be referenced
        _ = HdSt.self
    }

    // MARK: - Global Function Tests

    func testHdStIsSupportedFunction() {
        // Verify the global function is accessible
        let isSupported = hdStIsSupported()
        // In standalone mode, should return true
        XCTAssertTrue(isSupported)
    }
}
