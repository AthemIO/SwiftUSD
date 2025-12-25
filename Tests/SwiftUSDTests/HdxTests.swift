// HdxTests.swift - Tests for Hydra Extension Framework namespace and errors

import XCTest
@testable import SwiftUSD

final class HdxTests: XCTestCase {

    // MARK: - HdxError Tests

    func testHdxErrorCreationFailed() {
        let error = HdxError.creationFailed("test message")
        XCTAssertEqual(error.description, "Hdx creation failed: test message")
    }

    func testHdxErrorOperationFailed() {
        let error = HdxError.operationFailed("operation failed")
        XCTAssertEqual(error.description, "Hdx operation failed: operation failed")
    }

    func testHdxErrorInvalidArgument() {
        let error = HdxError.invalidArgument("bad argument")
        XCTAssertEqual(error.description, "Hdx invalid argument: bad argument")
    }

    func testHdxErrorRenderFailed() {
        let error = HdxError.renderFailed("render error")
        XCTAssertEqual(error.description, "Hdx render failed: render error")
    }

    func testHdxErrorPickFailed() {
        let error = HdxError.pickFailed("pick error")
        XCTAssertEqual(error.description, "Hdx pick failed: pick error")
    }

    func testHdxErrorSelectionFailed() {
        let error = HdxError.selectionFailed("selection error")
        XCTAssertEqual(error.description, "Hdx selection failed: selection error")
    }

    func testHdxErrorIsError() {
        let error: Error = HdxError.creationFailed("test")
        XCTAssertTrue(error is HdxError)
    }

    func testHdxErrorCustomStringConvertible() {
        let error = HdxError.operationFailed("test")
        let description = String(describing: error)
        XCTAssertTrue(description.contains("Hdx operation failed"))
    }

    // MARK: - Namespace Tests

    func testHdxNamespaceExists() {
        // Verify the Hdx namespace enum exists and can be referenced
        _ = Hdx.self
    }
}
