// ArTests.swift - Tests for the Ar (Asset Resolution) module

import XCTest
@testable import SwiftUSD

final class ArTests: XCTestCase {

    // MARK: - ResolvedPath Tests

    func testResolvedPathCreation() throws {
        let path = try ResolvedPath("/path/to/asset.usd")
        XCTAssertEqual(path.pathString, "/path/to/asset.usd")
        XCTAssertFalse(path.isEmpty)
        XCTAssertTrue(path.isValid)
    }

    func testResolvedPathEmpty() throws {
        let path = try ResolvedPath()
        XCTAssertTrue(path.isEmpty)
        XCTAssertFalse(path.isValid)
        XCTAssertEqual(path.pathString, "")
    }

    func testResolvedPathCopy() throws {
        let original = try ResolvedPath("/original/path.usd")
        let copy = try original.copy()
        XCTAssertEqual(original, copy)
        XCTAssertEqual(original.pathString, copy.pathString)
    }

    func testResolvedPathEquality() throws {
        let path1 = try ResolvedPath("/same/path.usd")
        let path2 = try ResolvedPath("/same/path.usd")
        let path3 = try ResolvedPath("/different/path.usd")

        XCTAssertEqual(path1, path2)
        XCTAssertNotEqual(path1, path3)
    }

    func testResolvedPathComparison() throws {
        let pathA = try ResolvedPath("/a/path.usd")
        let pathB = try ResolvedPath("/b/path.usd")

        XCTAssertTrue(pathA < pathB)
        XCTAssertFalse(pathB < pathA)
    }

    func testResolvedPathHashable() throws {
        let path1 = try ResolvedPath("/test/path.usd")
        let path2 = try ResolvedPath("/test/path.usd")

        var set = Set<ResolvedPath>()
        set.insert(path1)
        set.insert(path2)

        XCTAssertEqual(set.count, 1)
    }

    func testResolvedPathStringLiteral() {
        let path: ResolvedPath = "/literal/path.usd"
        XCTAssertEqual(path.pathString, "/literal/path.usd")
    }

    func testResolvedPathCodable() throws {
        let original = try ResolvedPath("/codable/test.usd")
        let encoder = JSONEncoder()
        let data = try encoder.encode(original)

        let decoder = JSONDecoder()
        let decoded = try decoder.decode(ResolvedPath.self, from: data)

        XCTAssertEqual(original, decoded)
    }

    // MARK: - Timestamp Tests

    func testTimestampCreation() {
        let time: Double = 1234567890.123
        let timestamp = Timestamp(time: time)
        XCTAssertTrue(timestamp.isValid)
        XCTAssertEqual(timestamp.time, time, accuracy: 0.001)
    }

    func testTimestampInvalid() {
        let timestamp = Timestamp()
        XCTAssertFalse(timestamp.isValid)
        XCTAssertTrue(timestamp.time.isNaN)
    }

    func testTimestampStaticInvalid() {
        let timestamp = Timestamp.invalid
        XCTAssertFalse(timestamp.isValid)
    }

    func testTimestampNow() {
        let before = Date().timeIntervalSince1970
        let timestamp = Timestamp.now
        let after = Date().timeIntervalSince1970

        XCTAssertTrue(timestamp.isValid)
        XCTAssertGreaterThanOrEqual(timestamp.time, before)
        XCTAssertLessThanOrEqual(timestamp.time, after)
    }

    func testTimestampDate() {
        let time: Double = 1234567890.0
        let timestamp = Timestamp(time: time)
        let date = timestamp.date

        XCTAssertNotNil(date)
        if let date = date {
            XCTAssertEqual(date.timeIntervalSince1970, time, accuracy: 0.001)
        }
    }

    func testTimestampDateInvalid() {
        let timestamp = Timestamp.invalid
        XCTAssertNil(timestamp.date)
    }

    func testTimestampEquality() {
        let ts1 = Timestamp(time: 1000.0)
        let ts2 = Timestamp(time: 1000.0)
        let ts3 = Timestamp(time: 2000.0)
        let invalid1 = Timestamp.invalid
        let invalid2 = Timestamp.invalid

        XCTAssertEqual(ts1, ts2)
        XCTAssertNotEqual(ts1, ts3)
        XCTAssertEqual(invalid1, invalid2)
        XCTAssertNotEqual(ts1, invalid1)
    }

    func testTimestampComparison() {
        let ts1 = Timestamp(time: 1000.0)
        let ts2 = Timestamp(time: 2000.0)
        let invalid = Timestamp.invalid

        XCTAssertTrue(ts1 < ts2)
        XCTAssertFalse(ts2 < ts1)
        XCTAssertTrue(invalid < ts1)  // Invalid is less than valid
    }

    func testTimestampCodable() throws {
        let original = Timestamp(time: 1234567890.5)
        let encoder = JSONEncoder()
        let data = try encoder.encode(original)

        let decoder = JSONDecoder()
        let decoded = try decoder.decode(Timestamp.self, from: data)

        XCTAssertEqual(original.time, decoded.time, accuracy: 0.001)
    }

    // MARK: - ResolverContext Tests

    func testResolverContextCreation() throws {
        let context = try ResolverContext()
        XCTAssertTrue(context.isEmpty)
    }

    func testResolverContextCopy() throws {
        let original = try ResolverContext()
        let copy = try original.copy()
        XCTAssertEqual(original, copy)
    }

    func testResolverContextEquality() throws {
        let ctx1 = try ResolverContext()
        let ctx2 = try ResolverContext()
        XCTAssertEqual(ctx1, ctx2)
    }

    func testResolverContextHashable() throws {
        let ctx1 = try ResolverContext()
        let ctx2 = try ResolverContext()

        var set = Set<ResolverContext>()
        set.insert(ctx1)
        set.insert(ctx2)

        // Both are empty contexts, so they should be equal
        XCTAssertEqual(set.count, 1)
    }

    // MARK: - DefaultResolverContext Tests

    func testDefaultResolverContextCreation() throws {
        let context = try DefaultResolverContext()
        XCTAssertEqual(context.searchPaths, [])
    }

    func testDefaultResolverContextWithSearchPaths() throws {
        let paths = ["/path/one", "/path/two", "/path/three"]
        let context = try DefaultResolverContext(searchPaths: paths)
        XCTAssertEqual(context.searchPaths, paths)
    }

    func testDefaultResolverContextCopy() throws {
        let paths = ["/original/path"]
        let original = try DefaultResolverContext(searchPaths: paths)
        let copy = try original.copy()

        XCTAssertEqual(original, copy)
        XCTAssertEqual(original.searchPaths, copy.searchPaths)
    }

    func testDefaultResolverContextEquality() throws {
        let paths = ["/test/path"]
        let ctx1 = try DefaultResolverContext(searchPaths: paths)
        let ctx2 = try DefaultResolverContext(searchPaths: paths)
        let ctx3 = try DefaultResolverContext(searchPaths: ["/different/path"])

        XCTAssertEqual(ctx1, ctx2)
        XCTAssertNotEqual(ctx1, ctx3)
    }

    func testDefaultResolverContextToResolverContext() throws {
        let paths = ["/search/path"]
        let defaultContext = try DefaultResolverContext(searchPaths: paths)
        let resolverContext = try defaultContext.toResolverContext()

        XCTAssertNotNil(resolverContext)
    }

    func testDefaultResolverContextAsString() throws {
        let paths = ["/path/a", "/path/b"]
        let context = try DefaultResolverContext(searchPaths: paths)
        let str = context.asString

        // The string should contain the paths
        XCTAssertFalse(str.isEmpty)
    }

    // MARK: - Resolver Tests

    func testResolverCreateIdentifier() {
        let identifier = Resolver.createIdentifier(for: "test/asset.usd")
        // The identifier should not be nil and should relate to the input
        XCTAssertNotNil(identifier)
    }

    func testResolverCreateIdentifierWithAnchor() throws {
        let anchor = try ResolvedPath("/root/path/")
        let identifier = Resolver.createIdentifier(for: "relative/asset.usd", anchorAssetPath: anchor)
        XCTAssertNotNil(identifier)
    }

    func testResolverGetExtension() {
        let ext1 = Resolver.getExtension(for: "model.usd")
        let ext2 = Resolver.getExtension(for: "model.usda")
        let ext3 = Resolver.getExtension(for: "model.usdc")
        let ext4 = Resolver.getExtension(for: "noextension")

        XCTAssertEqual(ext1, "usd")
        XCTAssertEqual(ext2, "usda")
        XCTAssertEqual(ext3, "usdc")
        XCTAssertEqual(ext4, "")
    }

    func testResolverGetExtensionWithPath() {
        let ext = Resolver.getExtension(for: "/path/to/file.usda")
        XCTAssertEqual(ext, "usda")
    }

    func testResolverIsContextDependentPath() {
        // Absolute paths are typically not context-dependent
        let result = Resolver.isContextDependentPath("/absolute/path.usd")
        // This depends on the resolver implementation, just verify it doesn't crash
        _ = result
    }

    func testResolverCreateDefaultContext() throws {
        let context = try Resolver.createDefaultContext()
        XCTAssertNotNil(context)
    }

    func testResolverCreateDefaultContextForAsset() throws {
        let context = try Resolver.createDefaultContext(for: "/some/asset.usd")
        XCTAssertNotNil(context)
    }

    func testResolverCreateContextFromString() throws {
        let context = try Resolver.createContext(from: "/search/path")
        XCTAssertNotNil(context)
    }

    func testResolverCurrentContext() {
        let context = Resolver.currentContext
        // Current context should always be available
        XCTAssertNotNil(context)
    }

    func testResolverRegisteredURISchemes() {
        let schemes = Resolver.registeredURISchemes
        // This should return an array (possibly empty)
        XCTAssertNotNil(schemes)
    }

    func testResolverCanWriteAsset() throws {
        let resolvedPath = try ResolvedPath("/tmp/test_write.usd")
        let (canWrite, reason) = Resolver.canWriteAsset(to: resolvedPath)
        // Just verify the API works, result depends on system state
        _ = canWrite
        _ = reason
    }

    func testResolverGetModificationTimestamp() throws {
        let resolvedPath = try ResolvedPath("/nonexistent/file.usd")
        let timestamp = Resolver.getModificationTimestamp(for: "/nonexistent/file.usd", resolvedPath: resolvedPath)
        // For a nonexistent file, timestamp should be invalid
        // (though this depends on the resolver implementation)
        _ = timestamp
    }

    // MARK: - Integration Tests

    func testResolveWorkflow() throws {
        // Test a typical asset resolution workflow
        let assetPath = "test_model.usd"

        // Create an identifier
        let identifier = Resolver.createIdentifier(for: assetPath)
        XCTAssertNotNil(identifier)

        // Try to resolve (may return nil if asset doesn't exist)
        let resolved = Resolver.resolve(assetPath)
        // This is expected to fail since the file doesn't exist
        _ = resolved

        // Get extension
        let ext = Resolver.getExtension(for: assetPath)
        XCTAssertEqual(ext, "usd")
    }

    func testContextWorkflow() throws {
        // Test creating and using resolver contexts
        let searchPaths = ["/assets/models", "/assets/textures"]
        let defaultContext = try DefaultResolverContext(searchPaths: searchPaths)

        XCTAssertEqual(defaultContext.searchPaths, searchPaths)

        // Convert to generic context
        let resolverContext = try defaultContext.toResolverContext()
        XCTAssertNotNil(resolverContext)

        // Create a copy
        let contextCopy = try defaultContext.copy()
        XCTAssertEqual(defaultContext, contextCopy)
    }
}
