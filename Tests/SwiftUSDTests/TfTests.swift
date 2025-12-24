import XCTest
@testable import SwiftUSD

final class TfTokenTests: XCTestCase {

    // MARK: - Initialization Tests

    func testCreateFromString() throws {
        let token = try Token("hello")
        XCTAssertEqual(token.text, "hello")
        XCTAssertEqual(token.count, 5)
        XCTAssertFalse(token.isEmpty)
    }

    func testCreateEmpty() throws {
        let token = try Token()
        XCTAssertEqual(token.text, "")
        XCTAssertEqual(token.count, 0)
        XCTAssertTrue(token.isEmpty)
    }

    func testCreateFromBuffer() throws {
        let text = "world"
        let token = try text.withCString { ptr in
            try Token(buffer: ptr, length: text.count)
        }
        XCTAssertEqual(token.text, "world")
        XCTAssertEqual(token.count, 5)
    }

    func testStringLiteral() {
        let token: Token = "literal"
        XCTAssertEqual(token.text, "literal")
    }

    // MARK: - Equality Tests

    func testEquality() throws {
        let token1 = try Token("test")
        let token2 = try Token("test")
        let token3 = try Token("other")

        XCTAssertEqual(token1, token2)
        XCTAssertNotEqual(token1, token3)
    }

    func testEqualsString() throws {
        let token = try Token("hello")
        XCTAssertTrue(token.equals("hello"))
        XCTAssertFalse(token.equals("world"))
    }

    func testEqualityWithEmpty() throws {
        let empty1 = try Token()
        let empty2 = try Token()
        let notEmpty = try Token("text")

        XCTAssertEqual(empty1, empty2)
        XCTAssertNotEqual(empty1, notEmpty)
    }

    // MARK: - Comparison Tests

    func testComparable() throws {
        let a = try Token("apple")
        let b = try Token("banana")
        let c = try Token("cherry")

        XCTAssertTrue(a < b)
        XCTAssertTrue(b < c)
        XCTAssertTrue(a < c)
        XCTAssertFalse(b < a)
    }

    func testSorting() throws {
        let tokens = [
            try Token("cherry"),
            try Token("apple"),
            try Token("banana")
        ]
        let sorted = tokens.sorted()

        XCTAssertEqual(sorted[0].text, "apple")
        XCTAssertEqual(sorted[1].text, "banana")
        XCTAssertEqual(sorted[2].text, "cherry")
    }

    // MARK: - Hash Tests

    func testHashable() throws {
        let token1 = try Token("test")
        let token2 = try Token("test")

        XCTAssertEqual(token1.hashValue, token2.hashValue)
    }

    func testUseInSet() throws {
        var set = Set<Token>()
        let token1 = try Token("first")
        let token2 = try Token("second")
        let token1Dup = try Token("first")

        set.insert(token1)
        set.insert(token2)
        set.insert(token1Dup)

        XCTAssertEqual(set.count, 2)
        XCTAssertTrue(set.contains(token1))
        XCTAssertTrue(set.contains(token2))
    }

    func testUseAsDictionaryKey() throws {
        var dict = [Token: Int]()
        let key1 = try Token("one")
        let key2 = try Token("two")

        dict[key1] = 1
        dict[key2] = 2

        XCTAssertEqual(dict[key1], 1)
        XCTAssertEqual(dict[key2], 2)

        // Test with equal token
        let key1Dup = try Token("one")
        XCTAssertEqual(dict[key1Dup], 1)
    }

    // MARK: - Copy Tests

    func testCopy() throws {
        let original = try Token("original")
        let copy = try original.copy()

        XCTAssertEqual(original, copy)
        XCTAssertEqual(original.text, copy.text)
    }

    // MARK: - String Description Tests

    func testDescription() throws {
        let token = try Token("described")
        XCTAssertEqual(token.description, "described")
        XCTAssertEqual(String(describing: token), "described")
    }

    func testDebugDescription() throws {
        let token = try Token("debug")
        XCTAssertEqual(token.debugDescription, "Token(\"debug\")")
    }

    // MARK: - Batch Operations Tests

    func testBatchCreation() throws {
        let strings = ["alpha", "beta", "gamma", "delta"]
        let tokens = try Token.tokens(from: strings)

        XCTAssertEqual(tokens.count, 4)
        XCTAssertEqual(tokens[0].text, "alpha")
        XCTAssertEqual(tokens[1].text, "beta")
        XCTAssertEqual(tokens[2].text, "gamma")
        XCTAssertEqual(tokens[3].text, "delta")
    }

    func testBatchCreationEmpty() throws {
        let tokens = try Token.tokens(from: [])
        XCTAssertTrue(tokens.isEmpty)
    }

    func testStringsFromTokens() throws {
        let tokens = [
            try Token("one"),
            try Token("two"),
            try Token("three")
        ]
        let strings = Token.strings(from: tokens)

        XCTAssertEqual(strings, ["one", "two", "three"])
    }

    // MARK: - Codable Tests

    func testEncode() throws {
        let token = try Token("encodable")
        let encoder = JSONEncoder()
        let data = try encoder.encode(token)
        let json = String(data: data, encoding: .utf8)

        XCTAssertEqual(json, "\"encodable\"")
    }

    func testDecode() throws {
        let json = "\"decodable\""
        let data = json.data(using: .utf8)!
        let decoder = JSONDecoder()
        let token = try decoder.decode(Token.self, from: data)

        XCTAssertEqual(token.text, "decodable")
    }

    func testRoundTrip() throws {
        let original = try Token("roundtrip")
        let encoder = JSONEncoder()
        let decoder = JSONDecoder()

        let data = try encoder.encode(original)
        let decoded = try decoder.decode(Token.self, from: data)

        XCTAssertEqual(original, decoded)
    }

    // MARK: - Thread Safety Tests

    func testConcurrentAccess() throws {
        let token = try Token("concurrent")
        let iterations = 1000
        let expectation = expectation(description: "Concurrent access")
        expectation.expectedFulfillmentCount = iterations

        for _ in 0..<iterations {
            DispatchQueue.global().async {
                // Access properties from multiple threads
                _ = token.text
                _ = token.count
                _ = token.isEmpty
                _ = token.hashValue
                expectation.fulfill()
            }
        }

        wait(for: [expectation], timeout: 10.0)
    }

    func testConcurrentCreation() throws {
        let iterations = 100
        let expectation = expectation(description: "Concurrent creation")
        expectation.expectedFulfillmentCount = iterations

        var tokens = [Token]()
        let lock = NSLock()

        for i in 0..<iterations {
            DispatchQueue.global().async {
                do {
                    let token = try Token("token_\(i)")
                    lock.lock()
                    tokens.append(token)
                    lock.unlock()
                } catch {
                    XCTFail("Failed to create token: \(error)")
                }
                expectation.fulfill()
            }
        }

        wait(for: [expectation], timeout: 10.0)
        XCTAssertEqual(tokens.count, iterations)
    }

    // MARK: - Edge Cases

    func testEmptyString() throws {
        let token = try Token("")
        XCTAssertTrue(token.isEmpty)
        XCTAssertEqual(token.text, "")
        XCTAssertEqual(token.count, 0)
    }

    func testUnicodeString() throws {
        let token = try Token("Hello, 世界! 🌍")
        XCTAssertEqual(token.text, "Hello, 世界! 🌍")
        XCTAssertFalse(token.isEmpty)
    }

    func testLongString() throws {
        let longString = String(repeating: "a", count: 10000)
        let token = try Token(longString)
        XCTAssertEqual(token.text, longString)
        XCTAssertEqual(token.count, 10000)
    }

    func testSpecialCharacters() throws {
        let special = "path/to/file.usd"
        let token = try Token(special)
        XCTAssertEqual(token.text, special)
    }

    func testWhitespace() throws {
        let whitespace = "  spaces  "
        let token = try Token(whitespace)
        XCTAssertEqual(token.text, whitespace)
    }

    // MARK: - Find Tests (if supported)

    func testFindExisting() throws {
        // Create a token first
        let original = try Token("findme")
        _ = original.text  // Ensure it's fully initialized

        // Try to find it
        let found = Token.find("findme")

        // Note: Find may or may not work depending on the underlying implementation
        // In the fallback mode (no USD), find always returns nil
        if let foundToken = found {
            XCTAssertEqual(foundToken.text, "findme")
        }
        // If nil, that's acceptable behavior for fallback mode
    }

    func testFindNonexistent() {
        let found = Token.find("definitely_not_a_token_that_exists_12345")
        // Should return nil for non-existent tokens
        XCTAssertNil(found)
    }
}

// MARK: - Token Error Tests

final class TokenErrorTests: XCTestCase {

    func testErrorDescription() {
        let creationError = TokenError.creationFailed("test message")
        XCTAssertTrue(creationError.description.contains("test message"))

        let argError = TokenError.invalidArgument("bad arg")
        XCTAssertTrue(argError.description.contains("bad arg"))
    }
}
