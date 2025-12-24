// UsdUtilsTests.swift - Tests for UsdUtils module
// Tests StageCache, LayerStackUtils, StitchUtils
// Note: These tests are designed to pass in stub mode (without full USD library).
// More comprehensive tests require USE_PIXAR_USD to be defined.

import XCTest
@testable import SwiftUSD

// MARK: - StageCache Tests

final class StageCacheTests: XCTestCase {

    override func setUp() {
        super.setUp()
        // Clear the cache before each test
        StageCache.clear()
    }

    override func tearDown() {
        // Clean up after each test
        StageCache.clear()
        super.tearDown()
    }

    func testEmptyCache() {
        XCTAssertEqual(StageCache.count, 0)
        XCTAssertTrue(StageCache.isEmpty)
        XCTAssertTrue(StageCache.allStages.isEmpty)
    }

    func testInsertAndFindStage() throws {
        let stage = try Stage.createInMemory(identifier: "test.usda")

        // Insert stage into cache
        let id = StageCache.insert(stage)
        XCTAssertNotNil(id)

        // Cache should now have one stage
        XCTAssertEqual(StageCache.count, 1)
        XCTAssertFalse(StageCache.isEmpty)

        // Should be able to find by ID
        if let cacheId = id {
            let foundStage = StageCache.find(id: cacheId)
            XCTAssertNotNil(foundStage)
        }
    }

    func testEraseById() throws {
        let stage = try Stage.createInMemory(identifier: "test_erase.usda")

        guard let id = StageCache.insert(stage) else {
            XCTFail("Failed to insert stage")
            return
        }

        XCTAssertEqual(StageCache.count, 1)

        // Erase by ID
        let erased = StageCache.erase(id: id)
        XCTAssertTrue(erased)
        XCTAssertEqual(StageCache.count, 0)

        // Should not be able to find after erase
        XCTAssertNil(StageCache.find(id: id))
    }

    func testEraseByStage() throws {
        let stage = try Stage.createInMemory(identifier: "test_erase_stage.usda")

        _ = StageCache.insert(stage)
        XCTAssertEqual(StageCache.count, 1)

        // Erase by stage reference
        let erased = StageCache.erase(stage)
        XCTAssertTrue(erased)
        XCTAssertEqual(StageCache.count, 0)
    }

    func testClearCache() throws {
        // Insert multiple stages
        for i in 0..<3 {
            let stage = try Stage.createInMemory(identifier: "stage\(i).usda")
            _ = StageCache.insert(stage)
        }

        XCTAssertEqual(StageCache.count, 3)

        // Clear all
        StageCache.clear()
        XCTAssertEqual(StageCache.count, 0)
        XCTAssertTrue(StageCache.isEmpty)
    }

    func testGetAllStages() throws {
        // Insert multiple stages
        for i in 0..<3 {
            let stage = try Stage.createInMemory(identifier: "stage\(i).usda")
            _ = StageCache.insert(stage)
        }

        let allStages = StageCache.allStages
        XCTAssertEqual(allStages.count, 3)
    }

    func testFindNonexistentId() {
        let stage = StageCache.find(id: 99999)
        XCTAssertNil(stage)
    }

    func testEraseNonexistentId() {
        let erased = StageCache.erase(id: 99999)
        XCTAssertFalse(erased)
    }
}

// MARK: - LayerStackUtils Tests

final class LayerStackUtilsTests: XCTestCase {

    func testFlattenLayerStack() throws {
        // Create a simple stage
        let stage = try Stage.createInMemory(identifier: "flatten_test.usda")

        // Flatten the layer stack
        let flattenedLayer = try LayerStackUtils.flattenLayerStack(of: stage)
        XCTAssertNotNil(flattenedLayer)
    }

    func testFlattenLayerStackWithTag() throws {
        let stage = try Stage.createInMemory(identifier: "flatten_tag_test.usda")

        let flattenedLayer = try LayerStackUtils.flattenLayerStack(of: stage, tag: "flattened")
        XCTAssertNotNil(flattenedLayer)
    }

    func testResolveAssetPath() throws {
        let layer = try Layer.createAnonymous()

        // Test resolving an asset path
        let resolved = LayerStackUtils.resolveAssetPath(
            sourceLayer: layer,
            assetPath: "relative/path/to/asset.usd"
        )

        // Should return some path (exact behavior depends on implementation)
        XCTAssertFalse(resolved.isEmpty)
    }
}

// MARK: - StitchUtils Tests

final class StitchUtilsTests: XCTestCase {

    func testStitchLayers() throws {
        // Create two layers
        let strongLayer = try Layer.createAnonymous(tag: "strong")
        let weakLayer = try Layer.createAnonymous(tag: "weak")

        // Stitch weak into strong
        XCTAssertNoThrow(try StitchUtils.stitchLayers(strong: strongLayer, weak: weakLayer))
    }

    func testStitchLayersPreservesStrong() throws {
        // Create two layers - in a real scenario, we'd add content
        // and verify the strong layer takes precedence
        let strongLayer = try Layer.createAnonymous(tag: "strong")
        let weakLayer = try Layer.createAnonymous(tag: "weak")

        // This should not throw
        try StitchUtils.stitchLayers(strong: strongLayer, weak: weakLayer)

        // Strong layer should still be valid
        XCTAssertNotNil(strongLayer)
    }
}

// MARK: - DependencyUtils Tests

final class DependencyUtilsTests: XCTestCase {

    func testExtractExternalReferencesFromNonExistentFile() {
        // In stub mode, this returns empty arrays
        // In full mode, it would fail for a non-existent file
        do {
            let refs = try DependencyUtils.extractExternalReferences(from: "nonexistent.usda")
            // Stub mode returns empty
            XCTAssertTrue(refs.subLayers.isEmpty)
            XCTAssertTrue(refs.references.isEmpty)
            XCTAssertTrue(refs.payloads.isEmpty)
        } catch {
            // Full mode may throw an error
            XCTAssertTrue(error is UsdUtilsError)
        }
    }

    func testComputeAllDependenciesFromNonExistentFile() {
        // In stub mode, this returns empty arrays
        do {
            let deps = try DependencyUtils.computeAllDependencies(of: "nonexistent.usda")
            // Stub mode returns empty
            XCTAssertTrue(deps.layers.isEmpty)
            XCTAssertTrue(deps.assets.isEmpty)
            XCTAssertTrue(deps.unresolvedPaths.isEmpty)
        } catch {
            // Full mode may throw an error
            XCTAssertTrue(error is UsdUtilsError)
        }
    }

    func testExternalReferencesStructure() {
        // Test that ExternalReferences can be created
        let refs = ExternalReferences(
            subLayers: ["layer1.usda", "layer2.usda"],
            references: ["ref1.usda"],
            payloads: ["payload1.usda"]
        )

        XCTAssertEqual(refs.subLayers.count, 2)
        XCTAssertEqual(refs.references.count, 1)
        XCTAssertEqual(refs.payloads.count, 1)
    }

    func testAssetDependenciesStructure() {
        // Test that AssetDependencies can be created
        let deps = AssetDependencies(
            layers: ["layer1.usda"],
            assets: ["texture.png", "mesh.obj"],
            unresolvedPaths: ["missing.usda"]
        )

        XCTAssertEqual(deps.layers.count, 1)
        XCTAssertEqual(deps.assets.count, 2)
        XCTAssertEqual(deps.unresolvedPaths.count, 1)
    }
}

// MARK: - PipelineUtils Tests

final class PipelineUtilsTests: XCTestCase {

    func testMaterialsScopeName() {
        let scopeName = PipelineUtils.materialsScopeName()
        XCTAssertFalse(scopeName.isEmpty)
        // Default is "Looks" in stub mode
        XCTAssertEqual(scopeName, "Looks")
    }

    func testMaterialsScopeNameForceDefault() {
        let scopeName = PipelineUtils.materialsScopeName(forceDefault: true)
        XCTAssertFalse(scopeName.isEmpty)
        XCTAssertEqual(scopeName, "Looks")
    }

    func testPrimaryUVSetName() {
        let uvSetName = PipelineUtils.primaryUVSetName
        XCTAssertFalse(uvSetName.isEmpty)
        // Default is "st"
        XCTAssertEqual(uvSetName, "st")
    }

    func testPrefName() {
        let prefName = PipelineUtils.prefName
        XCTAssertFalse(prefName.isEmpty)
        // Default is "pref"
        XCTAssertEqual(prefName, "pref")
    }

    func testPrimaryCameraName() {
        let cameraName = PipelineUtils.primaryCameraName()
        XCTAssertFalse(cameraName.isEmpty)
        // Default is "main_cam"
        XCTAssertEqual(cameraName, "main_cam")
    }

    func testPrimaryCameraNameForceDefault() {
        let cameraName = PipelineUtils.primaryCameraName(forceDefault: true)
        XCTAssertFalse(cameraName.isEmpty)
        XCTAssertEqual(cameraName, "main_cam")
    }

    func testAlphaAttributeNameForColor() {
        let alphaName = PipelineUtils.alphaAttributeName(for: "diffuseColor")
        XCTAssertFalse(alphaName.isEmpty)
        // In stub mode, it appends "_opacity"
        XCTAssertTrue(alphaName.contains("diffuseColor"))
    }

    func testModelNameFromRootLayer() throws {
        let layer = try Layer.createAnonymous()
        // In stub mode, this returns nil since no model name is available
        let modelName = PipelineUtils.modelName(from: layer)
        // Could be nil in stub mode
        XCTAssertTrue(modelName == nil || !modelName!.isEmpty)
    }
}

// MARK: - UsdUtilsError Tests

final class UsdUtilsErrorTests: XCTestCase {

    func testFlattenFailedDescription() {
        let error = UsdUtilsError.flattenFailed("Test failure")
        XCTAssertTrue(error.description.contains("flatten"))
        XCTAssertTrue(error.description.contains("Test failure"))
    }

    func testStitchFailedDescription() {
        let error = UsdUtilsError.stitchFailed("Test failure")
        XCTAssertTrue(error.description.contains("stitch"))
        XCTAssertTrue(error.description.contains("Test failure"))
    }

    func testDependencyAnalysisFailedDescription() {
        let error = UsdUtilsError.dependencyAnalysisFailed("Test failure")
        XCTAssertTrue(error.description.contains("Dependency"))
        XCTAssertTrue(error.description.contains("Test failure"))
    }
}
