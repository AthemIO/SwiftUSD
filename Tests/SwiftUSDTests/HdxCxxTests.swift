// HdxCxxTests.swift - Tests for Hydra Extension Framework Swift bindings

import XCTest
import USDCxx
@testable import SwiftUSD

final class HdxCxxTests: XCTestCase {

    // MARK: - HdxPickTarget Tests

    func testPickTargetValues() {
        XCTAssertEqual(swiftusd.HdxPickTarget.PrimsAndInstances.rawValue, 0)
        XCTAssertEqual(swiftusd.HdxPickTarget.Faces.rawValue, 1)
        XCTAssertEqual(swiftusd.HdxPickTarget.Edges.rawValue, 2)
        XCTAssertEqual(swiftusd.HdxPickTarget.Points.rawValue, 3)
        XCTAssertEqual(swiftusd.HdxPickTarget.PointsAndInstances.rawValue, 4)
    }

    func testPickTargetNames() {
        XCTAssertEqual(swiftusd.HdxPickTarget.PrimsAndInstances.name, "primsAndInstances")
        XCTAssertEqual(swiftusd.HdxPickTarget.Faces.name, "faces")
        XCTAssertEqual(swiftusd.HdxPickTarget.Edges.name, "edges")
        XCTAssertEqual(swiftusd.HdxPickTarget.Points.name, "points")
        XCTAssertEqual(swiftusd.HdxPickTarget.PointsAndInstances.name, "pointsAndInstances")
    }

    func testPickTargetTokens() {
        XCTAssertEqual(
            String(cString: swiftusd.HdxPickTarget.PrimsAndInstances.token.GetText()),
            "pickPrimsAndInstances"
        )
        XCTAssertEqual(
            String(cString: swiftusd.HdxPickTarget.Faces.token.GetText()),
            "pickFaces"
        )
        XCTAssertEqual(
            String(cString: swiftusd.HdxPickTarget.Edges.token.GetText()),
            "pickEdges"
        )
        XCTAssertEqual(
            String(cString: swiftusd.HdxPickTarget.Points.token.GetText()),
            "pickPoints"
        )
    }

    // MARK: - HdxPickResolveMode Tests

    func testPickResolveModeValues() {
        XCTAssertEqual(swiftusd.HdxPickResolveMode.NearestToCamera.rawValue, 0)
        XCTAssertEqual(swiftusd.HdxPickResolveMode.NearestToCenter.rawValue, 1)
        XCTAssertEqual(swiftusd.HdxPickResolveMode.Unique.rawValue, 2)
        XCTAssertEqual(swiftusd.HdxPickResolveMode.All.rawValue, 3)
        XCTAssertEqual(swiftusd.HdxPickResolveMode.Deep.rawValue, 4)
    }

    func testPickResolveModeNames() {
        XCTAssertEqual(swiftusd.HdxPickResolveMode.NearestToCamera.name, "nearestToCamera")
        XCTAssertEqual(swiftusd.HdxPickResolveMode.NearestToCenter.name, "nearestToCenter")
        XCTAssertEqual(swiftusd.HdxPickResolveMode.Unique.name, "unique")
        XCTAssertEqual(swiftusd.HdxPickResolveMode.All.name, "all")
        XCTAssertEqual(swiftusd.HdxPickResolveMode.Deep.name, "deep")
    }

    func testPickResolveModeTokens() {
        XCTAssertEqual(
            String(cString: swiftusd.HdxPickResolveMode.NearestToCamera.token.GetText()),
            "resolveNearestToCamera"
        )
        XCTAssertEqual(
            String(cString: swiftusd.HdxPickResolveMode.Unique.token.GetText()),
            "resolveUnique"
        )
    }

    // MARK: - HdxPickHit Tests

    func testPickHitDefault() {
        let hit = swiftusd.HdxPickHit()

        XCTAssertTrue(hit.delegateId.IsEmpty())
        XCTAssertTrue(hit.objectId.IsEmpty())
        XCTAssertTrue(hit.instancerId.IsEmpty())
        XCTAssertEqual(hit.instanceIndex, -1)
        XCTAssertEqual(hit.elementIndex, -1)
        XCTAssertEqual(hit.edgeIndex, -1)
        XCTAssertEqual(hit.pointIndex, -1)
        XCTAssertEqual(hit.normalizedDepth, 0.0, accuracy: 0.001)
        XCTAssertFalse(hit.isValid)
    }

    func testPickHitWithValues() {
        let objectId = swiftusd.Path("/World/Mesh")
        let hitPoint = swiftusd.Vec3d(1.0, 2.0, 3.0)
        let hit = swiftusd.HdxPickHit(objectId, hitPoint, 0.5)

        XCTAssertEqual(String(cString: hit.objectId.GetText()), "/World/Mesh")
        XCTAssertEqual(hit.worldSpaceHitPoint.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(hit.worldSpaceHitPoint.y, 2.0, accuracy: 0.001)
        XCTAssertEqual(hit.worldSpaceHitPoint.z, 3.0, accuracy: 0.001)
        XCTAssertEqual(hit.normalizedDepth, 0.5, accuracy: 0.001)
        XCTAssertTrue(hit.isValid)
    }

    func testPickHitConvenienceMethod() {
        let hit = swiftusd.HdxPickHit.make(
            objectId: swiftusd.Path("/World/Cube"),
            hitPoint: swiftusd.Vec3d(0.0, 1.0, 0.0),
            depth: 0.25
        )

        XCTAssertEqual(String(cString: hit.objectId.GetText()), "/World/Cube")
        XCTAssertEqual(hit.normalizedDepth, 0.25, accuracy: 0.001)
        XCTAssertTrue(hit.isValid)
    }

    // MARK: - HdxRenderTaskParams Tests

    func testRenderTaskParamsDefault() {
        let params = swiftusd.HdxRenderTaskParams()

        XCTAssertEqual(params.viewport.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.y, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.z, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.w, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.cullStyle, .BackUnlessDoubleSided)
        XCTAssertEqual(params.alphaThreshold, 0.5, accuracy: 0.001)
        XCTAssertTrue(params.enableLighting)
        XCTAssertTrue(params.enableSceneMaterials)
        XCTAssertFalse(params.enableIdRender)
    }

    func testRenderTaskParamsWithViewport() {
        let viewport = swiftusd.Vec4d(0, 0, 1920, 1080)
        let params = swiftusd.HdxRenderTaskParams(viewport, true)

        XCTAssertEqual(params.viewport.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.y, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.z, 1920.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.w, 1080.0, accuracy: 0.001)
        XCTAssertTrue(params.enableLighting)
    }

    func testRenderTaskParamsConvenienceMethod() {
        let params = swiftusd.HdxRenderTaskParams.make(
            viewport: swiftusd.Vec4d(100, 100, 800, 600),
            enableLighting: false
        )

        XCTAssertEqual(params.viewport.x, 100.0, accuracy: 0.001)
        XCTAssertEqual(params.viewport.z, 800.0, accuracy: 0.001)
        XCTAssertFalse(params.enableLighting)
    }

    // MARK: - HdxSelectionTaskParams Tests

    func testSelectionTaskParamsDefault() {
        let params = swiftusd.HdxSelectionTaskParams()

        XCTAssertTrue(params.enableSelectionHighlight)
        XCTAssertTrue(params.enableLocateHighlight)
        XCTAssertEqual(params.occludedSelectionOpacity, 0.5, accuracy: 0.001)
        // Yellow selection color
        XCTAssertEqual(params.selectionColor.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.selectionColor.y, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.selectionColor.z, 0.0, accuracy: 0.001)
        // Cyan locate color
        XCTAssertEqual(params.locateColor.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.locateColor.y, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.locateColor.z, 1.0, accuracy: 0.001)
    }

    func testSelectionTaskParamsWithColors() {
        let selColor = swiftusd.Vec4f(1.0, 0.0, 0.0, 1.0)  // Red
        let locColor = swiftusd.Vec4f(0.0, 1.0, 0.0, 1.0)  // Green
        let params = swiftusd.HdxSelectionTaskParams(selColor, locColor)

        XCTAssertEqual(params.selectionColor.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.selectionColor.y, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.locateColor.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.locateColor.y, 1.0, accuracy: 0.001)
    }

    func testSelectionTaskParamsConvenienceMethod() {
        let params = swiftusd.HdxSelectionTaskParams.make(
            selectionColor: swiftusd.Vec4f(0.5, 0.5, 0.5, 1.0),
            locateColor: swiftusd.Vec4f(0.8, 0.8, 0.8, 1.0)
        )

        XCTAssertEqual(params.selectionColor.x, 0.5, accuracy: 0.001)
        XCTAssertEqual(params.locateColor.x, 0.8, accuracy: 0.001)
    }

    // MARK: - HdxPickTaskParams Tests

    func testPickTaskParamsDefault() {
        let params = swiftusd.HdxPickTaskParams()

        XCTAssertEqual(params.cullStyle, .Nothing)
    }

    // MARK: - HdxPickTaskContextParams Tests

    func testPickTaskContextParamsDefault() {
        let params = swiftusd.HdxPickTaskContextParams()

        XCTAssertEqual(params.resolution.x, 128)
        XCTAssertEqual(params.resolution.y, 128)
        XCTAssertEqual(params.maxNumDeepEntries, 32000)
        XCTAssertEqual(params.pickTarget, .PrimsAndInstances)
        XCTAssertEqual(params.resolveMode, .NearestToCamera)
        XCTAssertFalse(params.doUnpickablesOcclude)
        XCTAssertEqual(params.alphaThreshold, 0.0001, accuracy: 0.00001)
    }

    func testPickTaskContextParamsWithMatrices() {
        let resolution = swiftusd.Vec2i(256, 256)
        let viewMatrix = swiftusd.Matrix4d()
        let projMatrix = swiftusd.Matrix4d()
        let params = swiftusd.HdxPickTaskContextParams(resolution, viewMatrix, projMatrix)

        XCTAssertEqual(params.resolution.x, 256)
        XCTAssertEqual(params.resolution.y, 256)
    }

    func testPickTaskContextParamsConvenienceMethod() {
        let params = swiftusd.HdxPickTaskContextParams.make(
            resolution: swiftusd.Vec2i(512, 512),
            viewMatrix: swiftusd.Matrix4d(),
            projectionMatrix: swiftusd.Matrix4d()
        )

        XCTAssertEqual(params.resolution.x, 512)
        XCTAssertEqual(params.resolution.y, 512)
    }

    // MARK: - HdxColorCorrectionTaskParams Tests

    func testColorCorrectionTaskParamsDefault() {
        let params = swiftusd.HdxColorCorrectionTaskParams()

        XCTAssertEqual(String(cString: params.colorCorrectionMode.GetText()), "disabled")
    }

    func testColorCorrectionTaskParamsWithMode() {
        let params = swiftusd.HdxColorCorrectionTaskParams(swiftusd.Token("sRGB"))

        XCTAssertEqual(String(cString: params.colorCorrectionMode.GetText()), "sRGB")
    }

    func testColorCorrectionTaskParamsConvenienceMethods() {
        let disabledParams = swiftusd.HdxColorCorrectionTaskParams.disabled()
        XCTAssertEqual(String(cString: disabledParams.colorCorrectionMode.GetText()), "disabled")

        let sRGBParams = swiftusd.HdxColorCorrectionTaskParams.sRGB()
        XCTAssertEqual(String(cString: sRGBParams.colorCorrectionMode.GetText()), "sRGB")
    }

    // MARK: - HdxBoundingBoxTaskParams Tests

    func testBoundingBoxTaskParamsDefault() {
        let params = swiftusd.HdxBoundingBoxTaskParams()

        // Yellow default color
        XCTAssertEqual(params.bboxColor.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.bboxColor.y, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.bboxColor.z, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.bboxColor.w, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.dashSize, 3.0, accuracy: 0.001)
    }

    func testBoundingBoxTaskParamsWithColor() {
        let color = swiftusd.Vec4f(1.0, 0.0, 0.0, 1.0)  // Red
        let params = swiftusd.HdxBoundingBoxTaskParams(color, 5.0)

        XCTAssertEqual(params.bboxColor.x, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.bboxColor.y, 0.0, accuracy: 0.001)
        XCTAssertEqual(params.dashSize, 5.0, accuracy: 0.001)
    }

    func testBoundingBoxTaskParamsConvenienceMethod() {
        let params = swiftusd.HdxBoundingBoxTaskParams.make(
            color: swiftusd.Vec4f(0.0, 0.0, 1.0, 1.0),  // Blue
            dashSize: 10.0
        )

        XCTAssertEqual(params.bboxColor.z, 1.0, accuracy: 0.001)
        XCTAssertEqual(params.dashSize, 10.0, accuracy: 0.001)
    }

    // MARK: - HdxShadowTaskParams Tests

    func testShadowTaskParamsDefault() {
        let params = swiftusd.HdxShadowTaskParams()

        XCTAssertTrue(params.enable)
        XCTAssertEqual(params.resolution, 2048)
    }

    func testShadowTaskParamsWithValues() {
        let params = swiftusd.HdxShadowTaskParams(false, 4096)

        XCTAssertFalse(params.enable)
        XCTAssertEqual(params.resolution, 4096)
    }

    func testShadowTaskParamsConvenienceMethod() {
        let params = swiftusd.HdxShadowTaskParams.make(enabled: true, resolution: 1024)

        XCTAssertTrue(params.enable)
        XCTAssertEqual(params.resolution, 1024)
    }

    // MARK: - HdxSelectionTrackerRef Tests

    func testSelectionTrackerCreation() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.Create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        XCTAssertNotNil(tracker)
        XCTAssertEqual(tracker.selectionCount, 0)
    }

    func testSelectionTrackerAddSelection() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        let path1 = swiftusd.Path("/World/Mesh1")
        let path2 = swiftusd.Path("/World/Mesh2")

        tracker.addSelection(path1)
        XCTAssertEqual(tracker.selectionCount, 1)
        XCTAssertTrue(tracker.isSelected(path1))
        XCTAssertFalse(tracker.isSelected(path2))

        tracker.addSelection(path2)
        XCTAssertEqual(tracker.selectionCount, 2)
        XCTAssertTrue(tracker.isSelected(path2))
    }

    func testSelectionTrackerRemoveSelection() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        let path = swiftusd.Path("/World/Mesh")
        tracker.addSelection(path)
        XCTAssertEqual(tracker.selectionCount, 1)

        tracker.removeSelection(path)
        XCTAssertEqual(tracker.selectionCount, 0)
        XCTAssertFalse(tracker.isSelected(path))
    }

    func testSelectionTrackerClear() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        tracker.addSelection(swiftusd.Path("/World/Mesh1"))
        tracker.addSelection(swiftusd.Path("/World/Mesh2"))
        tracker.addSelection(swiftusd.Path("/World/Mesh3"))
        XCTAssertEqual(tracker.selectionCount, 3)

        tracker.clear()
        XCTAssertEqual(tracker.selectionCount, 0)
    }

    func testSelectionTrackerVersion() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        let initialVersion = tracker.version
        tracker.addSelection(swiftusd.Path("/World/Mesh"))
        XCTAssertGreaterThan(tracker.version, initialVersion)

        let versionAfterAdd = tracker.version
        tracker.incrementVersion()
        XCTAssertGreaterThan(tracker.version, versionAfterAdd)
    }

    func testSelectionTrackerGetSelection() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        let path1 = swiftusd.Path("/World/Mesh1")
        let path2 = swiftusd.Path("/World/Mesh2")
        tracker.addSelection(path1)
        tracker.addSelection(path2)

        let retrieved = tracker.selection(at: 0)
        XCTAssertEqual(String(cString: retrieved.GetText()), "/World/Mesh1")
    }

    func testSelectionTrackerAllSelections() {
        guard let tracker = swiftusd.HdxSelectionTrackerRef.create() else {
            XCTFail("Failed to create selection tracker")
            return
        }

        tracker.addSelection(swiftusd.Path("/World/Mesh1"))
        tracker.addSelection(swiftusd.Path("/World/Mesh2"))

        let selections = tracker.allSelections
        XCTAssertEqual(selections.count, 2)
    }

    // MARK: - HdxTokens Tests

    func testHdxTaskContextTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickParams().GetText()), "pickParams")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickHits().GetText()), "pickHits")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.RenderParams().GetText()), "renderParams")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.SelectionState().GetText()), "selectionState")
    }

    func testHdxPickTargetTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickPrimsAndInstances().GetText()), "pickPrimsAndInstances")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickFaces().GetText()), "pickFaces")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickEdges().GetText()), "pickEdges")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickPoints().GetText()), "pickPoints")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.PickPointsAndInstances().GetText()), "pickPointsAndInstances")
    }

    func testHdxResolveModeTokens() {
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.ResolveNearestToCamera().GetText()), "resolveNearestToCamera")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.ResolveNearestToCenter().GetText()), "resolveNearestToCenter")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.ResolveUnique().GetText()), "resolveUnique")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.ResolveAll().GetText()), "resolveAll")
        XCTAssertEqual(String(cString: swiftusd.HdxTokens.ResolveDeep().GetText()), "resolveDeep")
    }

    // MARK: - Token Extensions Tests

    func testTokenExtensionsPickTargets() {
        XCTAssertEqual(String(cString: swiftusd.Token.pickPrimsAndInstances.GetText()), "pickPrimsAndInstances")
        XCTAssertEqual(String(cString: swiftusd.Token.pickFaces.GetText()), "pickFaces")
        XCTAssertEqual(String(cString: swiftusd.Token.pickEdges.GetText()), "pickEdges")
        XCTAssertEqual(String(cString: swiftusd.Token.pickPoints.GetText()), "pickPoints")
    }

    func testTokenExtensionsResolveModes() {
        XCTAssertEqual(String(cString: swiftusd.Token.resolveNearestToCamera.GetText()), "resolveNearestToCamera")
        XCTAssertEqual(String(cString: swiftusd.Token.resolveUnique.GetText()), "resolveUnique")
        XCTAssertEqual(String(cString: swiftusd.Token.resolveAll.GetText()), "resolveAll")
    }

    func testTokenExtensionsTaskContext() {
        XCTAssertEqual(String(cString: swiftusd.Token.pickParams.GetText()), "pickParams")
        XCTAssertEqual(String(cString: swiftusd.Token.pickHits.GetText()), "pickHits")
        XCTAssertEqual(String(cString: swiftusd.Token.renderParams.GetText()), "renderParams")
        XCTAssertEqual(String(cString: swiftusd.Token.selectionState.GetText()), "selectionState")
    }

    // MARK: - Global Functions Tests

    func testGlobalPickTargetToken() {
        let token = hdxGetPickTargetToken(.Faces)
        XCTAssertEqual(String(cString: token.GetText()), "pickFaces")
    }

    func testGlobalResolveModeToken() {
        let token = hdxGetResolveModeToken(.Unique)
        XCTAssertEqual(String(cString: token.GetText()), "resolveUnique")
    }

    func testGlobalPickTargetName() {
        XCTAssertEqual(hdxGetPickTargetName(.PrimsAndInstances), "primsAndInstances")
        XCTAssertEqual(hdxGetPickTargetName(.Edges), "edges")
    }

    func testGlobalResolveModeName() {
        XCTAssertEqual(hdxGetResolveModeName(.NearestToCamera), "nearestToCamera")
        XCTAssertEqual(hdxGetResolveModeName(.Deep), "deep")
    }
}
