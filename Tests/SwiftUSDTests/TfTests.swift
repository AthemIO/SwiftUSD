// TfTests.swift - Tests for Tf module (Token/Type Foundation)

import XCTest
import SwiftUSD
@testable import CxxFacade

final class TfTests: XCTestCase {

    // MARK: - TfToken Creation Tests

    func testTokenCreationFromString() {
        let token = cxxfacade.tf.TfToken("hello")
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(String(cString: token.GetText()), "hello")
    }

    func testTokenCreationFromStdString() {
        let stdStr = std.string("world")
        let token = cxxfacade.tf.TfToken(stdStr)
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(token.GetString(), stdStr)
    }

    func testEmptyTokenCreation() {
        let token = cxxfacade.tf.TfToken()
        XCTAssertTrue(token.IsEmpty())
        XCTAssertEqual(String(cString: token.GetText()), "")
        XCTAssertEqual(token.size(), 0)
    }

    // MARK: - Equality Tests

    func testTokenEquality() {
        let token1 = cxxfacade.tf.TfToken("test")
        let token2 = cxxfacade.tf.TfToken("test")
        let token3 = cxxfacade.tf.TfToken("other")

        XCTAssertTrue(token1 == token2)
        XCTAssertFalse(token1 == token3)
        XCTAssertTrue(token1 != token3)
    }

    func testEmptyTokenEquality() {
        let empty1 = cxxfacade.tf.TfToken()
        let empty2 = cxxfacade.tf.TfToken()
        let nonEmpty = cxxfacade.tf.TfToken("something")

        XCTAssertTrue(empty1 == empty2)
        XCTAssertFalse(empty1 == nonEmpty)
    }

    // MARK: - Comparison Tests

    func testTokenLessThan() {
        var tokenA = cxxfacade.tf.TfToken("apple")
        var tokenB = cxxfacade.tf.TfToken("banana")
        var tokenC = cxxfacade.tf.TfToken("cherry")

        XCTAssertTrue(tokenA < tokenB)
        XCTAssertTrue(tokenB < tokenC)
        XCTAssertFalse(tokenC < tokenA)
    }

    func testTokenGreaterThan() {
        var tokenA = cxxfacade.tf.TfToken("apple")
        var tokenB = cxxfacade.tf.TfToken("banana")

        XCTAssertTrue(tokenB > tokenA)
        XCTAssertFalse(tokenA > tokenB)
    }

    func testTokenLessThanOrEqual() {
        var tokenA = cxxfacade.tf.TfToken("apple")
        var tokenB = cxxfacade.tf.TfToken("apple")
        var tokenC = cxxfacade.tf.TfToken("banana")

        XCTAssertTrue(tokenA <= tokenB)
        XCTAssertTrue(tokenA <= tokenC)
        XCTAssertFalse(tokenC <= tokenA)
    }

    func testTokenGreaterThanOrEqual() {
        var tokenA = cxxfacade.tf.TfToken("apple")
        var tokenB = cxxfacade.tf.TfToken("apple")
        var tokenC = cxxfacade.tf.TfToken("banana")

        XCTAssertTrue(tokenA >= tokenB)
        XCTAssertTrue(tokenC >= tokenA)
        XCTAssertFalse(tokenA >= tokenC)
    }

    // MARK: - Hash Tests

    func testTokenHash() {
        let token1 = cxxfacade.tf.TfToken("test")
        let token2 = cxxfacade.tf.TfToken("test")

        XCTAssertEqual(token1.Hash(), token2.Hash())
    }

    func testDifferentTokensDifferentHash() {
        let token1 = cxxfacade.tf.TfToken("hello")
        let token2 = cxxfacade.tf.TfToken("world")

        // Different tokens should generally have different hashes
        // (though collision is theoretically possible)
        XCTAssertNotEqual(token1.Hash(), token2.Hash())
    }

    // MARK: - Size Tests

    func testTokenSize() {
        let token = cxxfacade.tf.TfToken("hello")
        XCTAssertEqual(token.size(), 5)
    }

    func testEmptyTokenSize() {
        let token = cxxfacade.tf.TfToken()
        XCTAssertEqual(token.size(), 0)
    }

    func testLongTokenSize() {
        let longString = String(repeating: "a", count: 1000)
        let token = cxxfacade.tf.TfToken(longString)
        XCTAssertEqual(token.size(), 1000)
    }

    // MARK: - Copy Tests

    func testTokenCopy() {
        let original = cxxfacade.tf.TfToken("original")
        let copy = original

        XCTAssertEqual(String(cString: original.GetText()), String(cString: copy.GetText()))
        XCTAssertTrue(original == copy)
    }

    // MARK: - String Comparison Tests

    func testTokenStringEquality() {
        var token = cxxfacade.tf.TfToken("hello")
        let str = std.string("hello")
        let wrongStr = std.string("world")

        XCTAssertTrue(token == str)
        XCTAssertFalse(token == wrongStr)
        XCTAssertTrue(token != wrongStr)
    }

    // MARK: - Swap Tests

    func testTokenSwap() {
        var token1 = cxxfacade.tf.TfToken("first")
        var token2 = cxxfacade.tf.TfToken("second")

        token1.Swap(&token2)

        XCTAssertEqual(String(cString: token1.GetText()), "second")
        XCTAssertEqual(String(cString: token2.GetText()), "first")
    }

    // MARK: - Find Tests

    func testTokenFind() {
        // First create a token to register it
        let _ = cxxfacade.tf.TfToken("registeredToken")

        // Now try to find it
        let found = cxxfacade.tf.TfToken.Find(std.string("registeredToken"))
        XCTAssertFalse(found.IsEmpty())
        XCTAssertEqual(String(cString: found.GetText()), "registeredToken")
    }

    func testTokenFindNotFound() {
        // Try to find a token that was never created with a unique name
        let found = cxxfacade.tf.TfToken.Find(std.string("uniqueNeverCreatedToken98765"))
        // If not found, USD returns an empty token
        XCTAssertTrue(found.IsEmpty())
    }

    // MARK: - Edge Cases

    func testTokenWithSpecialCharacters() {
        let token = cxxfacade.tf.TfToken("hello_world:property.name")
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(String(cString: token.GetText()), "hello_world:property.name")
    }

    func testTokenWithUnicode() {
        let token = cxxfacade.tf.TfToken("日本語")
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(String(cString: token.GetText()), "日本語")
    }

    func testTokenWithSpaces() {
        let token = cxxfacade.tf.TfToken("hello world")
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(String(cString: token.GetText()), "hello world")
    }

    func testTokenWithNumbers() {
        let token = cxxfacade.tf.TfToken("item123")
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(String(cString: token.GetText()), "item123")
    }

    // MARK: - Interoperability Tests

    func testTokenWithSdfPath() {
        // Test that TfToken works correctly with SdfPath operations
        let pathToken = cxxfacade.tf.TfToken("World")
        let root = cxxfacade.sdf.SdfPath("/")
        let childPath = root.AppendChild(pathToken)

        XCTAssertEqual(childPath.GetString(), std.string("/World"))
    }

    func testTokenIntegrationWithUSD() {
        // Test token usage in typical USD scenarios
        let xformToken = cxxfacade.tf.TfToken("Xform")
        let meshToken = cxxfacade.tf.TfToken("Mesh")
        let sphereToken = cxxfacade.tf.TfToken("Sphere")

        // Tokens should be efficiently comparable
        XCTAssertFalse(xformToken == meshToken)
        XCTAssertFalse(meshToken == sphereToken)
        XCTAssertTrue(xformToken == cxxfacade.tf.TfToken("Xform"))
    }
}
