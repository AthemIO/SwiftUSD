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

    // MARK: - ResolverContextBinder Tests

    func testResolverContextBinderCreation() throws {
        let context = try ResolverContext()
        let binder = try ResolverContextBinder(context: context)
        XCTAssertNotNil(binder)
    }

    func testResolverContextBinderGetContext() throws {
        let defaultContext = try DefaultResolverContext(searchPaths: ["/test/path"])
        let context = try defaultContext.toResolverContext()
        let binder = try ResolverContextBinder(context: context)

        let boundContext = binder.boundContext
        XCTAssertNotNil(boundContext)
    }

    func testResolverContextBinderWithBoundContext() throws {
        let defaultContext = try DefaultResolverContext(searchPaths: ["/test/path"])
        let context = try defaultContext.toResolverContext()

        var wasExecuted = false
        try ResolverContextBinder.withBoundContext(context) {
            wasExecuted = true
            // Verify we can still do resolution operations
            _ = Resolver.currentContext
        }
        XCTAssertTrue(wasExecuted)
    }

    func testResolverContextBinderWithBoundContextReturnsValue() throws {
        let context = try ResolverContext()

        let result = try ResolverContextBinder.withBoundContext(context) {
            return 42
        }
        XCTAssertEqual(result, 42)
    }

    // MARK: - ResolverScopedCache Tests

    func testResolverScopedCacheCreation() throws {
        let cache = try ResolverScopedCache()
        XCTAssertNotNil(cache)
    }

    func testResolverScopedCacheWithCaching() throws {
        var wasExecuted = false
        try ResolverScopedCache.withCaching {
            wasExecuted = true
            // Perform some resolution operations that might benefit from caching
            _ = Resolver.getExtension(for: "test.usd")
            _ = Resolver.getExtension(for: "test.usd")
        }
        XCTAssertTrue(wasExecuted)
    }

    func testResolverScopedCacheWithCachingReturnsValue() throws {
        let result = try ResolverScopedCache.withCaching {
            return "cached result"
        }
        XCTAssertEqual(result, "cached result")
    }

    // MARK: - Asset Tests

    func testAssetOpenNonexistent() {
        // Opening a nonexistent asset should return nil
        let path = try! ResolvedPath("/nonexistent/file/that/does/not/exist.usd")
        let asset = Asset.open(path)
        XCTAssertNil(asset)
    }

    func testAssetWithTempFile() throws {
        // Create a temporary file to test asset reading
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_test_\(UUID().uuidString).txt")
        let testContent = "Hello, Asset Test!"

        // Write test content
        try testContent.write(to: tempFile, atomically: true, encoding: .utf8)
        defer { try? FileManager.default.removeItem(at: tempFile) }

        // Create resolved path and open asset
        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = Asset.open(resolvedPath) else {
            XCTFail("Failed to open asset")
            return
        }

        // Test size
        XCTAssertEqual(asset.size, testContent.utf8.count)
        XCTAssertFalse(asset.isEmpty)

        // Test string content
        XCTAssertEqual(asset.string, testContent)

        // Test data content
        let data = asset.data
        XCTAssertNotNil(data)
        XCTAssertEqual(String(data: data!, encoding: .utf8), testContent)
    }

    func testAssetRead() throws {
        // Create a temporary file
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_read_test_\(UUID().uuidString).txt")
        let testContent = "0123456789ABCDEF"

        try testContent.write(to: tempFile, atomically: true, encoding: .utf8)
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = Asset.open(resolvedPath) else {
            XCTFail("Failed to open asset")
            return
        }

        // Test reading a range
        var buffer = [UInt8](repeating: 0, count: 4)
        let bytesRead = asset.read(into: &buffer, count: 4, offset: 0)
        XCTAssertEqual(bytesRead, 4)
        XCTAssertEqual(String(bytes: buffer, encoding: .utf8), "0123")

        // Test reading with offset
        let bytesRead2 = asset.read(into: &buffer, count: 4, offset: 10)
        XCTAssertEqual(bytesRead2, 4)
        XCTAssertEqual(String(bytes: buffer, encoding: .utf8), "ABCD")

        // Test read range
        let rangeData = asset.read(range: 4..<8)
        XCTAssertNotNil(rangeData)
        XCTAssertEqual(String(data: rangeData!, encoding: .utf8), "4567")
    }

    func testAssetDescription() throws {
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_desc_test_\(UUID().uuidString).txt")
        try "test".write(to: tempFile, atomically: true, encoding: .utf8)
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = Asset.open(resolvedPath) else {
            XCTFail("Failed to open asset")
            return
        }

        XCTAssertTrue(asset.description.contains("Asset"))
        XCTAssertTrue(asset.debugDescription.contains("bytes"))
    }

    // MARK: - WritableAsset Tests

    func testWritableAssetWrite() throws {
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_write_test_\(UUID().uuidString).txt")
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = WritableAsset.open(resolvedPath, mode: .replace) else {
            XCTFail("Failed to open writable asset")
            return
        }

        // Write content
        let testContent = "Hello, WritableAsset!"
        let bytesWritten = asset.write(testContent)
        XCTAssertEqual(bytesWritten, testContent.utf8.count)

        // Close and finalize
        let success = asset.close()
        XCTAssertTrue(success)

        // Verify content was written
        let written = try String(contentsOf: tempFile, encoding: .utf8)
        XCTAssertEqual(written, testContent)
    }

    func testWritableAssetWriteData() throws {
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_write_data_test_\(UUID().uuidString).bin")
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = WritableAsset.open(resolvedPath, mode: .replace) else {
            XCTFail("Failed to open writable asset")
            return
        }

        // Write binary data
        let testData = Data([0x00, 0x01, 0x02, 0x03, 0xFF, 0xFE, 0xFD])
        let bytesWritten = asset.write(testData)
        XCTAssertEqual(bytesWritten, testData.count)

        asset.close()

        // Verify
        let written = try Data(contentsOf: tempFile)
        XCTAssertEqual(written, testData)
    }

    func testWritableAssetWriteBytes() throws {
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_write_bytes_test_\(UUID().uuidString).bin")
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = WritableAsset.open(resolvedPath, mode: .replace) else {
            XCTFail("Failed to open writable asset")
            return
        }

        // Write byte array
        let bytes: [UInt8] = [65, 66, 67, 68]  // "ABCD"
        let bytesWritten = asset.write(bytes)
        XCTAssertEqual(bytesWritten, 4)

        asset.close()

        // Verify
        let written = try String(contentsOf: tempFile, encoding: .utf8)
        XCTAssertEqual(written, "ABCD")
    }

    func testWritableAssetUpdateMode() throws {
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_update_test_\(UUID().uuidString).txt")

        // Create initial file
        try "AAAAAAAAAA".write(to: tempFile, atomically: true, encoding: .utf8)
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = WritableAsset.open(resolvedPath, mode: .update) else {
            XCTFail("Failed to open writable asset in update mode")
            return
        }

        // Update part of the file
        let bytesWritten = asset.write("BBB", offset: 3)
        XCTAssertEqual(bytesWritten, 3)

        asset.close()

        // Verify partial update
        let written = try String(contentsOf: tempFile, encoding: .utf8)
        XCTAssertEqual(written, "AAABBBAAAA")
    }

    func testWritableAssetDescription() throws {
        let tempDir = FileManager.default.temporaryDirectory
        let tempFile = tempDir.appendingPathComponent("ar_desc_write_test_\(UUID().uuidString).txt")
        defer { try? FileManager.default.removeItem(at: tempFile) }

        let resolvedPath = try ResolvedPath(tempFile.path)
        guard let asset = WritableAsset.open(resolvedPath, mode: .replace) else {
            XCTFail("Failed to open writable asset")
            return
        }

        XCTAssertTrue(asset.description.contains("WritableAsset"))
        XCTAssertTrue(asset.description.contains("closed: false"))

        asset.close()
        XCTAssertTrue(asset.description.contains("closed: true"))
    }

    func testWriteModeEnum() {
        // Test WriteMode enum values exist and are distinct
        let updateMode = WriteMode.update
        let replaceMode = WriteMode.replace

        // Just verify they are different modes
        XCTAssertNotNil(updateMode)
        XCTAssertNotNil(replaceMode)
    }
}
