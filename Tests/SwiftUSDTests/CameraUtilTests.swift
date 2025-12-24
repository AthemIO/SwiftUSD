import XCTest
@testable import SwiftUSD

final class CameraUtilTests: XCTestCase {

    // MARK: - Range2f Tests

    func testRange2fCreation() {
        let range = CameraUtil.Range2f(minX: 0, minY: 0, maxX: 1920, maxY: 1080)
        XCTAssertEqual(range.minX, 0)
        XCTAssertEqual(range.minY, 0)
        XCTAssertEqual(range.maxX, 1920)
        XCTAssertEqual(range.maxY, 1080)
    }

    func testRange2fProperties() {
        let range = CameraUtil.Range2f(minX: 100, minY: 50, maxX: 1020, maxY: 630)
        XCTAssertEqual(range.width, 920)
        XCTAssertEqual(range.height, 580)
        XCTAssertEqual(range.aspectRatio, 920.0 / 580.0, accuracy: 0.001)
        XCTAssertFalse(range.isEmpty)
    }

    func testRange2fEmpty() {
        let empty = CameraUtil.Range2f()
        XCTAssertTrue(empty.isEmpty)
    }

    func testRange2fEquality() {
        let r1 = CameraUtil.Range2f(minX: 0, minY: 0, maxX: 100, maxY: 100)
        let r2 = CameraUtil.Range2f(minX: 0, minY: 0, maxX: 100, maxY: 100)
        let r3 = CameraUtil.Range2f(minX: 0, minY: 0, maxX: 200, maxY: 100)
        XCTAssertEqual(r1, r2)
        XCTAssertNotEqual(r1, r3)
    }

    func testRange2fDescription() {
        let range = CameraUtil.Range2f(minX: 0, minY: 0, maxX: 100, maxY: 100)
        XCTAssertTrue(range.description.contains("Range2f"))
    }

    // MARK: - Rect2i Tests

    func testRect2iCreation() {
        let rect = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        XCTAssertEqual(rect.minX, 0)
        XCTAssertEqual(rect.minY, 0)
        XCTAssertEqual(rect.width, 1920)
        XCTAssertEqual(rect.height, 1080)
    }

    func testRect2iProperties() {
        let rect = CameraUtil.Rect2i(minX: 100, minY: 50, width: 800, height: 600)
        XCTAssertEqual(rect.maxX, 900)
        XCTAssertEqual(rect.maxY, 650)
        XCTAssertEqual(rect.aspectRatio, 800.0 / 600.0, accuracy: 0.001)
        XCTAssertFalse(rect.isEmpty)
    }

    func testRect2iEmpty() {
        let empty = CameraUtil.Rect2i()
        XCTAssertTrue(empty.isEmpty)
    }

    func testRect2iIntInitializer() {
        let rect = CameraUtil.Rect2i(minX: 10, minY: 20, width: 100, height: 200)
        XCTAssertEqual(rect.minX, 10)
        XCTAssertEqual(rect.minY, 20)
    }

    // MARK: - ScreenWindow Tests

    func testScreenWindowCreation() {
        let sw = CameraUtil.ScreenWindow(left: -1, right: 1, bottom: -1, top: 1)
        XCTAssertEqual(sw.left, -1)
        XCTAssertEqual(sw.right, 1)
        XCTAssertEqual(sw.bottom, -1)
        XCTAssertEqual(sw.top, 1)
    }

    func testScreenWindowProperties() {
        let sw = CameraUtil.ScreenWindow(left: -2, right: 2, bottom: -1, top: 1)
        XCTAssertEqual(sw.width, 4)
        XCTAssertEqual(sw.height, 2)
        XCTAssertEqual(sw.aspectRatio, 2.0, accuracy: 0.001)
    }

    // MARK: - ConformWindowPolicy Tests

    func testConformWindowPolicyDescription() {
        XCTAssertEqual(CameraUtil.ConformWindowPolicy.matchVertically.description, "Match Vertically")
        XCTAssertEqual(CameraUtil.ConformWindowPolicy.matchHorizontally.description, "Match Horizontally")
        XCTAssertEqual(CameraUtil.ConformWindowPolicy.fit.description, "Fit")
        XCTAssertEqual(CameraUtil.ConformWindowPolicy.crop.description, "Crop")
        XCTAssertEqual(CameraUtil.ConformWindowPolicy.dontConform.description, "Don't Conform")
    }

    // MARK: - Framing Tests

    func testFramingEmptyCreation() throws {
        let framing = try CameraUtil.Framing()
        XCTAssertFalse(framing.isValid)
    }

    func testFramingWithWindows() throws {
        let displayWindow = CameraUtil.Range2f(minX: 0, minY: 0, maxX: 1920, maxY: 1080)
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)

        let framing = try CameraUtil.Framing(
            displayWindow: displayWindow,
            dataWindow: dataWindow,
            pixelAspectRatio: 1.0
        )

        XCTAssertTrue(framing.isValid)
        XCTAssertEqual(framing.displayWindow.maxX, 1920)
        XCTAssertEqual(framing.dataWindow.width, 1920)
        XCTAssertEqual(framing.pixelAspectRatio, 1.0, accuracy: 0.001)
    }

    func testFramingFromDataWindow() throws {
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        let framing = try CameraUtil.Framing(dataWindow: dataWindow)

        XCTAssertTrue(framing.isValid)
        XCTAssertEqual(framing.dataWindow.width, 1920)
        XCTAssertEqual(framing.dataWindow.height, 1080)
        XCTAssertEqual(framing.pixelAspectRatio, 1.0)
    }

    func testFramingWithOverscan() throws {
        // Create framing with overscan (data window larger than display window)
        let displayWindow = CameraUtil.Range2f(minX: 100, minY: 50, maxX: 2020, maxY: 1130)
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 2120, height: 1180)

        let framing = try CameraUtil.Framing(
            displayWindow: displayWindow,
            dataWindow: dataWindow
        )

        XCTAssertTrue(framing.isValid)
        XCTAssertEqual(framing.displayWindow.width, 1920)
        XCTAssertEqual(framing.displayWindow.height, 1080)
        XCTAssertEqual(framing.dataWindow.width, 2120)
    }

    func testFramingPropertySetters() throws {
        let framing = try CameraUtil.Framing(
            dataWindow: CameraUtil.Rect2i(minX: 0, minY: 0, width: 800, height: 600)
        )

        // Update display window
        framing.displayWindow = CameraUtil.Range2f(minX: 10, minY: 10, maxX: 810, maxY: 610)
        XCTAssertEqual(framing.displayWindow.minX, 10)

        // Update data window
        framing.dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        XCTAssertEqual(framing.dataWindow.width, 1920)

        // Update pixel aspect ratio
        framing.pixelAspectRatio = 1.5
        XCTAssertEqual(framing.pixelAspectRatio, 1.5, accuracy: 0.001)
    }

    func testFramingComputeFilmbackWindow() throws {
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        let framing = try CameraUtil.Framing(dataWindow: dataWindow)

        // Compute filmback for a 16:9 camera (same as display)
        let filmback = try framing.computeFilmbackWindow(
            cameraAspectRatio: 16.0 / 9.0,
            policy: .fit
        )

        XCTAssertFalse(filmback.isEmpty)
        print("Filmback window: \(filmback)")
    }

    func testFramingApplyToProjectionMatrix() throws {
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        let framing = try CameraUtil.Framing(dataWindow: dataWindow)

        // Create an identity projection matrix
        var projMatrix = [Double](repeating: 0, count: 16)
        projMatrix[0] = 1; projMatrix[5] = 1; projMatrix[10] = 1; projMatrix[15] = 1

        let result = try framing.applyToProjectionMatrix(projMatrix, policy: .fit)
        XCTAssertEqual(result.count, 16)
    }

    func testFramingCopy() throws {
        let original = try CameraUtil.Framing(
            dataWindow: CameraUtil.Rect2i(minX: 0, minY: 0, width: 800, height: 600)
        )
        let copy = try original.copy()

        XCTAssertEqual(original, copy)
        XCTAssertEqual(original.dataWindow, copy.dataWindow)
    }

    func testFramingEquality() throws {
        let f1 = try CameraUtil.Framing(
            dataWindow: CameraUtil.Rect2i(minX: 0, minY: 0, width: 800, height: 600)
        )
        let f2 = try CameraUtil.Framing(
            dataWindow: CameraUtil.Rect2i(minX: 0, minY: 0, width: 800, height: 600)
        )
        let f3 = try CameraUtil.Framing(
            dataWindow: CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        )

        XCTAssertEqual(f1, f2)
        XCTAssertNotEqual(f1, f3)
    }

    func testFramingDescription() throws {
        let framing = try CameraUtil.Framing(
            dataWindow: CameraUtil.Rect2i(minX: 0, minY: 0, width: 800, height: 600)
        )
        XCTAssertTrue(framing.description.contains("Framing"))
    }

    // MARK: - ScreenWindowParameters Tests

    func testScreenWindowParametersCreation() throws {
        let params = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 50.0,
            projection: .perspective
        )

        XCTAssertGreaterThan(params.fieldOfView, 0)
        XCTAssertLessThan(params.fieldOfView, 180)
        print("FOV: \(params.fieldOfView)")
        print("Screen window: \(params.screenWindow)")
    }

    func testScreenWindowParametersOrthographic() throws {
        let params = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 50.0,
            projection: .orthographic
        )

        // Orthographic projection should still compute a screen window
        XCTAssertNotEqual(params.screenWindow.width, 0)
    }

    func testScreenWindowParametersWithOffset() throws {
        let params = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            horizontalApertureOffset: 5.0,
            verticalApertureOffset: 3.0,
            focalLength: 50.0
        )

        // With offset, the screen window should be asymmetric
        let sw = params.screenWindow
        let centerX = (sw.left + sw.right) / 2.0
        print("Screen window center X: \(centerX)")
    }

    func testScreenWindowParametersFitDirection() throws {
        let paramsH = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 50.0,
            fitDirection: .horizontal
        )

        let paramsV = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 50.0,
            fitDirection: .vertical
        )

        // Different fit directions should produce different screen windows
        print("Horizontal fit: \(paramsH.screenWindow)")
        print("Vertical fit: \(paramsV.screenWindow)")
    }

    func testScreenWindowParametersZFacingViewMatrix() throws {
        let params = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 50.0
        )

        let matrix = params.zFacingViewMatrix
        XCTAssertEqual(matrix.count, 16)

        // The z-facing view matrix should be a valid transformation
        // (not all zeros)
        let hasNonZero = matrix.contains { $0 != 0 }
        XCTAssertTrue(hasNonZero)
    }

    func testScreenWindowParametersCopy() throws {
        let original = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 50.0
        )

        let copy = try original.copy()
        XCTAssertEqual(original.fieldOfView, copy.fieldOfView, accuracy: 0.001)
        XCTAssertEqual(original.screenWindow.left, copy.screenWindow.left, accuracy: 0.001)
    }

    // MARK: - Conform Window Size Tests

    func testConformWindowSizeMatchVertically() throws {
        let result = try CameraUtil.conformWindow(
            width: 1920, height: 1080,
            policy: .matchVertically,
            targetAspect: 2.35
        )

        // Height stays the same, width adjusts
        XCTAssertEqual(result.height, 1080, accuracy: 0.001)
        XCTAssertEqual(result.width, 1080 * 2.35, accuracy: 0.001)
    }

    func testConformWindowSizeMatchHorizontally() throws {
        let result = try CameraUtil.conformWindow(
            width: 1920, height: 1080,
            policy: .matchHorizontally,
            targetAspect: 2.35
        )

        // Width stays the same, height adjusts
        XCTAssertEqual(result.width, 1920, accuracy: 0.001)
        XCTAssertEqual(result.height, 1920 / 2.35, accuracy: 0.001)
    }

    func testConformWindowSizeFit() throws {
        // 16:9 window, target 2.35:1 (wider)
        let result = try CameraUtil.conformWindow(
            width: 1920, height: 1080,
            policy: .fit,
            targetAspect: 2.35
        )

        // Should increase width to fit
        let actualAspect = result.width / result.height
        XCTAssertEqual(actualAspect, 2.35, accuracy: 0.01)
    }

    func testConformWindowSizeCrop() throws {
        // 16:9 window, target 4:3 (narrower)
        let result = try CameraUtil.conformWindow(
            width: 1920, height: 1080,
            policy: .crop,
            targetAspect: 4.0 / 3.0
        )

        // Should decrease width
        let actualAspect = result.width / result.height
        XCTAssertEqual(actualAspect, 4.0 / 3.0, accuracy: 0.01)
    }

    func testConformWindowSizeDontConform() throws {
        let result = try CameraUtil.conformWindow(
            width: 1920, height: 1080,
            policy: .dontConform,
            targetAspect: 2.35
        )

        // Should remain unchanged
        XCTAssertEqual(result.width, 1920, accuracy: 0.001)
        XCTAssertEqual(result.height, 1080, accuracy: 0.001)
    }

    // MARK: - Conform Window Range Tests

    func testConformWindowRange() throws {
        let result = try CameraUtil.conformWindow(
            minX: 0, minY: 0, maxX: 1920, maxY: 1080,
            policy: .fit,
            targetAspect: 2.35
        )

        // Should maintain center while adjusting size
        let width = result.maxX - result.minX
        let height = result.maxY - result.minY
        let aspect = width / height
        XCTAssertEqual(aspect, 2.35, accuracy: 0.01)
    }

    // MARK: - Conform Screen Window Tests

    func testConformScreenWindow() throws {
        let original = CameraUtil.ScreenWindow(left: -1, right: 1, bottom: -1, top: 1)

        let result = try CameraUtil.conformScreenWindow(
            original,
            policy: .matchVertically,
            targetAspect: 2.0
        )

        // Horizontal extent should be 2x vertical
        XCTAssertEqual(result.width, result.height * 2.0, accuracy: 0.001)
    }

    // MARK: - Conform Projection Matrix Tests

    func testConformProjectionMatrix() throws {
        var projMatrix = [Double](repeating: 0, count: 16)
        projMatrix[0] = 1; projMatrix[5] = 1; projMatrix[10] = 1; projMatrix[15] = 1

        let result = try CameraUtil.conformProjectionMatrix(
            projMatrix,
            policy: .fit,
            targetAspect: 2.0
        )

        XCTAssertEqual(result.count, 16)
    }

    func testConformProjectionMatrixInvalidSize() throws {
        let invalidMatrix = [Double](repeating: 0, count: 9) // 3x3 instead of 4x4

        XCTAssertThrowsError(try CameraUtil.conformProjectionMatrix(
            invalidMatrix,
            policy: .fit,
            targetAspect: 2.0
        )) { error in
            XCTAssertTrue(error is CameraUtilError)
        }
    }

    // MARK: - Error Tests

    func testCameraUtilError() {
        let creationError = CameraUtilError.creationFailed("Test")
        XCTAssertTrue(creationError.description.contains("creation failed"))

        let operationError = CameraUtilError.operationFailed("Test")
        XCTAssertTrue(operationError.description.contains("operation failed"))

        let argumentError = CameraUtilError.invalidArgument("Test")
        XCTAssertTrue(argumentError.description.contains("invalid argument"))
    }

    // MARK: - Integration Tests

    func testTypicalRenderingWorkflow() throws {
        // 1. Create framing for a 1920x1080 render
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
        let framing = try CameraUtil.Framing(dataWindow: dataWindow)

        // 2. Compute filmback for a 2.35:1 camera
        let filmback = try framing.computeFilmbackWindow(
            cameraAspectRatio: 2.35,
            policy: .fit
        )

        print("Render workflow:")
        print("  Data window: \(framing.dataWindow)")
        print("  Display window: \(framing.displayWindow)")
        print("  Filmback: \(filmback)")

        // 3. Create screen window parameters for RenderMan
        let screenParams = try CameraUtil.ScreenWindowParameters(
            horizontalAperture: 36.0,
            verticalAperture: 24.0,
            focalLength: 35.0
        )

        print("  Screen window: \(screenParams.screenWindow)")
        print("  FOV: \(screenParams.fieldOfView)")
    }

    func testCinemaScopeWorkflow() throws {
        // Create a CinemaScope (2.35:1) framing in a 16:9 container
        let displayWindow = CameraUtil.Range2f(minX: 0, minY: 120, maxX: 1920, maxY: 960)
        let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)

        let framing = try CameraUtil.Framing(
            displayWindow: displayWindow,
            dataWindow: dataWindow
        )

        XCTAssertTrue(framing.isValid)

        // The display window should have approximately 2.29:1 aspect ratio
        let aspect = framing.displayWindow.aspectRatio
        XCTAssertEqual(aspect, 1920.0 / 840.0, accuracy: 0.01)

        print("CinemaScope workflow:")
        print("  Display aspect: \(aspect)")
    }
}
