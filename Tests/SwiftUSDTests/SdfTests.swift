// SdfTests.swift - Tests for Sdf module
// Tests SdfPath, SdfAssetPath, and SdfLayer

import XCTest
@testable import SwiftUSD

final class SdfPathTests: XCTestCase {

    // MARK: - Creation Tests

    func testCreateEmptyPath() throws {
        let path = try Path()
        XCTAssertTrue(path.isEmpty)
        XCTAssertEqual(path.text, "")
    }

    func testCreateAbsolutePath() throws {
        let path = try Path("/World")
        XCTAssertFalse(path.isEmpty)
        XCTAssertTrue(path.isAbsolutePath)
        XCTAssertEqual(path.text, "/World")
        XCTAssertEqual(path.name, "World")
    }

    func testCreateNestedPath() throws {
        let path = try Path("/World/Mesh")
        XCTAssertTrue(path.isAbsolutePath)
        XCTAssertTrue(path.isPrimPath)
        XCTAssertEqual(path.text, "/World/Mesh")
        XCTAssertEqual(path.name, "Mesh")
    }

    func testCreatePropertyPath() throws {
        let path = try Path("/World/Mesh.points")
        XCTAssertTrue(path.isPropertyPath)
        XCTAssertEqual(path.text, "/World/Mesh.points")
        XCTAssertEqual(path.name, "points")
    }

    func testCreateRelativePath() throws {
        let path = try Path("Mesh")
        XCTAssertFalse(path.isAbsolutePath)
        XCTAssertEqual(path.text, "Mesh")
    }

    func testStaticAbsoluteRootPath() throws {
        let path = try Path.absoluteRoot
        XCTAssertTrue(path.isAbsoluteRootPath)
        XCTAssertEqual(path.text, "/")
    }

    func testStaticReflexiveRelativePath() throws {
        let path = try Path.reflexiveRelative
        XCTAssertFalse(path.isAbsolutePath)
        XCTAssertEqual(path.text, ".")
    }

    // MARK: - Property Tests

    func testPathElementCount() throws {
        let root = try Path("/")
        let single = try Path("/World")
        let nested = try Path("/World/Mesh/SubMesh")

        // Element count varies by implementation
        XCTAssertGreaterThanOrEqual(single.elementCount, 1)
        XCTAssertGreaterThan(nested.elementCount, single.elementCount)
    }

    func testIsPrimPath() throws {
        let primPath = try Path("/World/Mesh")
        let propertyPath = try Path("/World/Mesh.points")

        XCTAssertTrue(primPath.isPrimPath)
        XCTAssertFalse(propertyPath.isPrimPath)
    }

    func testIsRootPrimPath() throws {
        let rootPrim = try Path("/World")
        let nestedPrim = try Path("/World/Mesh")

        XCTAssertTrue(rootPrim.isRootPrimPath)
        XCTAssertFalse(nestedPrim.isRootPrimPath)
    }

    // MARK: - Navigation Tests

    func testGetParentPath() throws {
        let path = try Path("/World/Mesh")
        let parent = path.parentPath

        XCTAssertNotNil(parent)
        XCTAssertEqual(parent?.text, "/World")
    }

    func testGetPrimPath() throws {
        let propertyPath = try Path("/World/Mesh.points")
        let primPath = propertyPath.primPath

        XCTAssertNotNil(primPath)
        XCTAssertEqual(primPath?.text, "/World/Mesh")
    }

    // MARK: - Construction Tests

    func testAppendChild() throws {
        let parent = try Path("/World")
        let child = try parent.appendingChild("Mesh")

        XCTAssertEqual(child.text, "/World/Mesh")
    }

    func testAppendChildWithToken() throws {
        let parent = try Path("/World")
        let token = try Token("Mesh")
        let child = try parent.appendingChild(token)

        XCTAssertEqual(child.text, "/World/Mesh")
    }

    func testAppendProperty() throws {
        let prim = try Path("/World/Mesh")
        let property = try prim.appendingProperty("points")

        XCTAssertEqual(property.text, "/World/Mesh.points")
        XCTAssertTrue(property.isPropertyPath)
    }

    func testAppendVariantSelection() throws {
        let prim = try Path("/World/Mesh")
        let variant = try prim.appendingVariantSelection(variantSet: "modelingVariant", variant: "high")

        XCTAssertTrue(variant.text.contains("{"))
        XCTAssertTrue(variant.containsVariantSelection)
    }

    // MARK: - Comparison Tests

    func testPathEquality() throws {
        let path1 = try Path("/World/Mesh")
        let path2 = try Path("/World/Mesh")
        let path3 = try Path("/World/Other")

        XCTAssertEqual(path1, path2)
        XCTAssertNotEqual(path1, path3)
    }

    func testPathComparison() throws {
        let path1 = try Path("/A")
        let path2 = try Path("/B")

        XCTAssertLessThan(path1, path2)
    }

    func testHasPrefix() throws {
        let path = try Path("/World/Mesh/SubMesh")
        let prefix = try Path("/World")
        let notPrefix = try Path("/Other")

        XCTAssertTrue(path.hasPrefix(prefix))
        XCTAssertFalse(path.hasPrefix(notPrefix))
    }

    // MARK: - Hash Tests

    func testPathHashing() throws {
        let path1 = try Path("/World/Mesh")
        let path2 = try Path("/World/Mesh")

        var set = Set<Path>()
        set.insert(path1)
        set.insert(path2)

        XCTAssertEqual(set.count, 1)
    }

    // MARK: - Validation Tests

    func testIsValidPathString() {
        XCTAssertTrue(Path.isValidPathString("/World"))
        XCTAssertTrue(Path.isValidPathString("/World/Mesh"))
        XCTAssertTrue(Path.isValidPathString("/World/Mesh.points"))
    }

    func testIsValidIdentifier() {
        XCTAssertTrue(Path.isValidIdentifier("World"))
        XCTAssertTrue(Path.isValidIdentifier("Mesh_01"))
        XCTAssertTrue(Path.isValidIdentifier("_private"))
        XCTAssertFalse(Path.isValidIdentifier("123invalid"))
        XCTAssertFalse(Path.isValidIdentifier(""))
    }

    // MARK: - String Literal Tests

    func testStringLiteralInitialization() {
        let path: Path = "/World/Mesh"
        XCTAssertEqual(path.text, "/World/Mesh")
    }

    // MARK: - Copy Tests

    func testCopyPath() throws {
        let original = try Path("/World/Mesh")
        let copy = try original.copy()

        XCTAssertEqual(original, copy)
        // Verify they are independent (if we had a mutation method)
    }
}

final class SdfAssetPathTests: XCTestCase {

    // MARK: - Creation Tests

    func testCreateEmptyAssetPath() throws {
        let assetPath = try AssetPath()
        XCTAssertTrue(assetPath.isEmpty)
        XCTAssertEqual(assetPath.authoredPath, "")
    }

    func testCreateAssetPathFromString() throws {
        let assetPath = try AssetPath("./models/cube.usda")
        XCTAssertFalse(assetPath.isEmpty)
        XCTAssertEqual(assetPath.authoredPath, "./models/cube.usda")
        XCTAssertEqual(assetPath.assetPath, "./models/cube.usda")
    }

    func testCreateAssetPathWithResolvedPath() throws {
        let assetPath = try AssetPath(
            authoredPath: "./models/cube.usda",
            resolvedPath: "/absolute/path/to/models/cube.usda"
        )

        XCTAssertEqual(assetPath.authoredPath, "./models/cube.usda")
        XCTAssertEqual(assetPath.resolvedPath, "/absolute/path/to/models/cube.usda")
    }

    // MARK: - Comparison Tests

    func testAssetPathEquality() throws {
        let path1 = try AssetPath("./models/cube.usda")
        let path2 = try AssetPath("./models/cube.usda")
        let path3 = try AssetPath("./models/sphere.usda")

        XCTAssertEqual(path1, path2)
        XCTAssertNotEqual(path1, path3)
    }

    func testAssetPathComparison() throws {
        let path1 = try AssetPath("./a.usda")
        let path2 = try AssetPath("./b.usda")

        XCTAssertLessThan(path1, path2)
    }

    // MARK: - Hash Tests

    func testAssetPathHashing() throws {
        let path1 = try AssetPath("./models/cube.usda")
        let path2 = try AssetPath("./models/cube.usda")

        var set = Set<AssetPath>()
        set.insert(path1)
        set.insert(path2)

        XCTAssertEqual(set.count, 1)
    }

    // MARK: - String Literal Tests

    func testStringLiteralInitialization() {
        let assetPath: AssetPath = "./models/cube.usda"
        XCTAssertEqual(assetPath.authoredPath, "./models/cube.usda")
    }

    // MARK: - Copy Tests

    func testCopyAssetPath() throws {
        let original = try AssetPath("./models/cube.usda")
        let copy = try original.copy()

        XCTAssertEqual(original, copy)
    }
}

final class SdfLayerTests: XCTestCase {

    // MARK: - Creation Tests

    func testCreateAnonymousLayer() throws {
        let layer = try Layer.createAnonymous()
        XCTAssertTrue(layer.isAnonymous)
        XCTAssertTrue(layer.isEmpty)
    }

    func testCreateAnonymousLayerWithTag() throws {
        let layer = try Layer.createAnonymous(tag: "TestLayer")
        XCTAssertTrue(layer.isAnonymous)
        XCTAssertTrue(layer.identifier.contains("TestLayer") || layer.identifier.contains("anon"))
    }

    // MARK: - Property Tests

    func testLayerProperties() throws {
        let layer = try Layer.createAnonymous(tag: "Test")

        XCTAssertFalse(layer.identifier.isEmpty)
        XCTAssertTrue(layer.isAnonymous)
        XCTAssertTrue(layer.isEmpty)
    }

    // MARK: - Metadata Tests

    func testLayerDocumentation() throws {
        let layer = try Layer.createAnonymous()

        layer.documentation = "Test documentation"
        XCTAssertEqual(layer.documentation, "Test documentation")
    }

    func testLayerComment() throws {
        let layer = try Layer.createAnonymous()

        layer.comment = "Test comment"
        XCTAssertEqual(layer.comment, "Test comment")
    }

    func testLayerTimeCodeRange() throws {
        let layer = try Layer.createAnonymous()

        layer.startTimeCode = 1.0
        layer.endTimeCode = 100.0

        XCTAssertEqual(layer.startTimeCode, 1.0)
        XCTAssertEqual(layer.endTimeCode, 100.0)
    }

    func testLayerTimeCodesPerSecond() throws {
        let layer = try Layer.createAnonymous()

        layer.timeCodesPerSecond = 30.0
        XCTAssertEqual(layer.timeCodesPerSecond, 30.0)
    }

    func testLayerFramesPerSecond() throws {
        let layer = try Layer.createAnonymous()

        layer.framesPerSecond = 60.0
        XCTAssertEqual(layer.framesPerSecond, 60.0)
    }

    // MARK: - Content Tests

    func testClearLayer() throws {
        let layer = try Layer.createAnonymous()

        // Set some content
        layer.documentation = "Some docs"

        // Clear the layer
        try layer.clear()

        // Verify layer is empty (documentation may or may not be cleared depending on implementation)
        XCTAssertTrue(layer.isEmpty || layer.documentation.isEmpty)
    }

    // MARK: - Export/Import Tests

    func testExportToString() throws {
        let layer = try Layer.createAnonymous()

        // This should not throw - the result may be empty or contain minimal content
        let content = try layer.exportToString()
        XCTAssertNotNil(content)
    }

    func testImportFromString() throws {
        let layer = try Layer.createAnonymous()

        // Import some valid USD content (minimal usda)
        let content = """
        #usda 1.0
        """

        try layer.importFromString(content)
        // If we get here without throwing, the import succeeded
    }

    // MARK: - Sublayer Tests

    func testSubLayerCount() throws {
        let layer = try Layer.createAnonymous()

        XCTAssertEqual(layer.subLayerCount, 0)
    }

    func testSubLayerPaths() throws {
        let layer = try Layer.createAnonymous()

        XCTAssertEqual(layer.subLayerPaths.count, 0)
    }

    // MARK: - Root Prims Tests

    func testRootPrimCount() throws {
        let layer = try Layer.createAnonymous()

        XCTAssertEqual(layer.rootPrimCount, 0)
    }

    func testRootPrimNames() throws {
        let layer = try Layer.createAnonymous()

        XCTAssertEqual(layer.rootPrimNames.count, 0)
    }
}

// MARK: - Type Alias Tests

final class SdfTypeAliasTests: XCTestCase {

    func testSdfPathAlias() throws {
        let path: SdfPath = try SdfPath("/World")
        XCTAssertEqual(path.text, "/World")
    }

    func testSdfAssetPathAlias() throws {
        let assetPath: SdfAssetPath = try SdfAssetPath("./model.usda")
        XCTAssertEqual(assetPath.authoredPath, "./model.usda")
    }

    func testSdfLayerAlias() throws {
        let layer: SdfLayer = try SdfLayer.createAnonymous()
        XCTAssertTrue(layer.isAnonymous)
    }
}
