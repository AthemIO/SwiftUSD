// SwiftRenderingTests.swift - Tests for SwiftRendering module

import XCTest
import USDCxx
@testable import SwiftUSD

final class SwiftRenderingTests: XCTestCase {

    // MARK: - RenderingError Tests

    func testRenderingErrorCreationFailed() {
        let error = RenderingError.creationFailed("test message")
        XCTAssertEqual(error.description, "Rendering creation failed: test message")
    }

    func testRenderingErrorRenderFailed() {
        let error = RenderingError.renderFailed("render error")
        XCTAssertEqual(error.description, "Rendering failed: render error")
    }

    func testRenderingErrorStageFailed() {
        let error = RenderingError.stageFailed("stage error")
        XCTAssertEqual(error.description, "Stage setup failed: stage error")
    }

    func testRenderingErrorCameraFailed() {
        let error = RenderingError.cameraFailed("camera error")
        XCTAssertEqual(error.description, "Camera configuration failed: camera error")
    }

    func testRenderingErrorPickFailed() {
        let error = RenderingError.pickFailed("pick error")
        XCTAssertEqual(error.description, "Picking failed: pick error")
    }

    func testRenderingErrorInvalidArgument() {
        let error = RenderingError.invalidArgument("bad argument")
        XCTAssertEqual(error.description, "Rendering invalid argument: bad argument")
    }

    func testRenderingErrorUnsupportedPlatform() {
        let error = RenderingError.unsupportedPlatform("platform error")
        XCTAssertEqual(error.description, "Unsupported platform: platform error")
    }

    func testRenderingErrorResourceAllocationFailed() {
        let error = RenderingError.resourceAllocationFailed("allocation error")
        XCTAssertEqual(error.description, "Resource allocation failed: allocation error")
    }

    func testRenderingErrorIsError() {
        let error: Error = RenderingError.creationFailed("test")
        XCTAssertTrue(error is RenderingError)
    }

    func testRenderingErrorCustomStringConvertible() {
        let error = RenderingError.renderFailed("test")
        let description = String(describing: error)
        XCTAssertTrue(description.contains("Rendering failed"))
    }

    // MARK: - Namespace Tests

    func testRenderingNamespaceExists() {
        // Verify the Rendering namespace enum exists and can be referenced
        _ = Rendering.self
    }

    // MARK: - SwiftRenderingParams Tests

    func testSwiftRenderingParamsDefault() {
        let params = swiftusd.SwiftRenderingParams()

        // Check default viewport (1920x1080)
        XCTAssertEqual(params.viewport.x, 0)
        XCTAssertEqual(params.viewport.y, 0)
        XCTAssertEqual(params.viewport.z, 1920)
        XCTAssertEqual(params.viewport.w, 1080)

        // Check default clear color (18% gray)
        XCTAssertEqual(params.clearColor.x, 0.18, accuracy: 0.01)
        XCTAssertEqual(params.clearColor.y, 0.18, accuracy: 0.01)
        XCTAssertEqual(params.clearColor.z, 0.18, accuracy: 0.01)
        XCTAssertEqual(params.clearColor.w, 1.0, accuracy: 0.01)

        // Check default draw mode
        XCTAssertEqual(params.drawMode, swiftusd.SwiftRenderingDrawMode.Shaded)

        // Check default color space
        XCTAssertEqual(params.colorSpace, swiftusd.SwiftRenderingColorSpace.SRGB)

        // Check default flags
        XCTAssertTrue(params.enableDepth)
        XCTAssertFalse(params.enableShadows)
        XCTAssertTrue(params.enableSelection)
        XCTAssertTrue(params.enableSceneLights)
        XCTAssertTrue(params.enableSceneMaterials)
        XCTAssertTrue(params.enableCameraLight)
        XCTAssertTrue(params.enableDomeLightVisibility)

        // Check default refinement
        XCTAssertEqual(params.refineLevel, 0)
        XCTAssertEqual(params.complexity, 1.0, accuracy: 0.01)
    }

    func testSwiftRenderingParamsWithSize() {
        let params = swiftusd.SwiftRenderingParams(800, 600)

        XCTAssertEqual(params.viewport.x, 0)
        XCTAssertEqual(params.viewport.y, 0)
        XCTAssertEqual(params.viewport.z, 800)
        XCTAssertEqual(params.viewport.w, 600)
    }

    // MARK: - SwiftRenderingCameraParams Tests

    func testSwiftRenderingCameraParamsDefault() {
        let camera = swiftusd.SwiftRenderingCameraParams()

        // Default should be identity matrices and free camera
        XCTAssertFalse(camera.useSceneCamera)
    }

    func testSwiftRenderingCameraParamsWithMatrices() {
        let view = swiftusd.Matrix4d.Identity()
        let proj = swiftusd.Matrix4d.Identity()
        let camera = swiftusd.SwiftRenderingCameraParams(view, proj)

        XCTAssertFalse(camera.useSceneCamera)
    }

    func testSwiftRenderingCameraParamsWithPath() {
        let cameraPath = swiftusd.Path("/World/Camera")
        let camera = swiftusd.SwiftRenderingCameraParams(cameraPath)

        XCTAssertTrue(camera.useSceneCamera)
        XCTAssertEqual(String(cString: camera.cameraPath.GetText()), "/World/Camera")
    }

    // MARK: - SwiftRenderingColorSpace Tests

    func testSwiftRenderingColorSpaceLinear() {
        let colorSpace = swiftusd.SwiftRenderingColorSpace.Linear
        XCTAssertEqual(colorSpace.name, "Linear")
        XCTAssertEqual(String(cString: colorSpace.token.GetText()), "linear")
    }

    func testSwiftRenderingColorSpaceSRGB() {
        let colorSpace = swiftusd.SwiftRenderingColorSpace.SRGB
        XCTAssertEqual(colorSpace.name, "sRGB")
        XCTAssertEqual(String(cString: colorSpace.token.GetText()), "sRGB")
    }

    func testSwiftRenderingColorSpaceDisplayP3() {
        let colorSpace = swiftusd.SwiftRenderingColorSpace.DisplayP3
        XCTAssertEqual(colorSpace.name, "Display P3")
        XCTAssertEqual(String(cString: colorSpace.token.GetText()), "displayP3")
    }

    func testSwiftRenderingColorSpaceRaw() {
        let colorSpace = swiftusd.SwiftRenderingColorSpace.Raw
        XCTAssertEqual(colorSpace.name, "Raw")
        XCTAssertEqual(String(cString: colorSpace.token.GetText()), "raw")
    }

    // MARK: - SwiftRenderingDrawMode Tests

    func testSwiftRenderingDrawModeShaded() {
        let drawMode = swiftusd.SwiftRenderingDrawMode.Shaded
        XCTAssertEqual(drawMode.name, "Shaded")
        XCTAssertEqual(String(cString: drawMode.token.GetText()), "shaded")
    }

    func testSwiftRenderingDrawModeWireframe() {
        let drawMode = swiftusd.SwiftRenderingDrawMode.Wireframe
        XCTAssertEqual(drawMode.name, "Wireframe")
        XCTAssertEqual(String(cString: drawMode.token.GetText()), "wireframe")
    }

    func testSwiftRenderingDrawModeShadedWireframe() {
        let drawMode = swiftusd.SwiftRenderingDrawMode.ShadedWireframe
        XCTAssertEqual(drawMode.name, "Shaded+Wireframe")
        XCTAssertEqual(String(cString: drawMode.token.GetText()), "shadedWireframe")
    }

    func testSwiftRenderingDrawModePoints() {
        let drawMode = swiftusd.SwiftRenderingDrawMode.Points
        XCTAssertEqual(drawMode.name, "Points")
        XCTAssertEqual(String(cString: drawMode.token.GetText()), "points")
    }

    func testSwiftRenderingDrawModeGeom() {
        let drawMode = swiftusd.SwiftRenderingDrawMode.Geom
        XCTAssertEqual(drawMode.name, "Geometry")
        XCTAssertEqual(String(cString: drawMode.token.GetText()), "geom")
    }

    func testSwiftRenderingDrawModeFlatShaded() {
        let drawMode = swiftusd.SwiftRenderingDrawMode.FlatShaded
        XCTAssertEqual(drawMode.name, "Flat Shaded")
        XCTAssertEqual(String(cString: drawMode.token.GetText()), "flatShaded")
    }

    // MARK: - Token Tests

    func testTokenStorm() {
        let token = swiftusd.SwiftRenderingTokens.Storm()
        XCTAssertEqual(String(cString: token.GetText()), "HdStormRendererPlugin")
    }

    func testTokenEmbree() {
        let token = swiftusd.SwiftRenderingTokens.Embree()
        XCTAssertEqual(String(cString: token.GetText()), "HdEmbreeRendererPlugin")
    }

    func testTokenPrman() {
        let token = swiftusd.SwiftRenderingTokens.Prman()
        XCTAssertEqual(String(cString: token.GetText()), "HdPrmanRendererPlugin")
    }

    func testTokenColorAov() {
        let token = swiftusd.SwiftRenderingTokens.ColorAov()
        XCTAssertEqual(String(cString: token.GetText()), "color")
    }

    func testTokenDepthAov() {
        let token = swiftusd.SwiftRenderingTokens.DepthAov()
        XCTAssertEqual(String(cString: token.GetText()), "depth")
    }

    func testTokenNormalAov() {
        let token = swiftusd.SwiftRenderingTokens.NormalAov()
        XCTAssertEqual(String(cString: token.GetText()), "normal")
    }

    // MARK: - HydraRenderer Tests

    func testHydraRendererCreate() {
        let renderer = swiftusd.HydraRendererRef.Create()
        XCTAssertNotNil(renderer)

        if let renderer = renderer {
            XCTAssertTrue(renderer.IsValid())
            XCTAssertEqual(
                String(cString: renderer.GetRendererPluginId().GetText()),
                "HdStormRendererPlugin"
            )
        }
    }

    func testHydraRendererCreateWithPlugin() {
        let renderer = swiftusd.HydraRendererRef.Create(swiftusd.SwiftRenderingTokens.Storm())
        XCTAssertNotNil(renderer)

        if let renderer = renderer {
            XCTAssertTrue(renderer.IsValid())
        }
    }

    func testHydraRendererViewport() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetViewport(1280, 720)
        XCTAssertEqual(Int(renderer.GetViewportWidth()), 1280)
        XCTAssertEqual(Int(renderer.GetViewportHeight()), 720)
    }

    func testHydraRendererTime() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetTime(2.5)
        XCTAssertEqual(renderer.GetTime(), 2.5, accuracy: 0.001)
    }

    func testHydraRendererParams() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        var params = swiftusd.SwiftRenderingParams(640, 480)
        params.enableShadows = true
        params.refineLevel = 2
        renderer.SetParams(params)

        let retrievedParams = renderer.GetParams()
        XCTAssertEqual(Int(retrievedParams.viewport.z), 640)
        XCTAssertEqual(Int(retrievedParams.viewport.w), 480)
        XCTAssertTrue(retrievedParams.enableShadows)
        XCTAssertEqual(retrievedParams.refineLevel, 2)
    }

    func testHydraRendererCamera() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        let cameraPath = swiftusd.Path("/World/Camera")
        let camera = swiftusd.SwiftRenderingCameraParams(cameraPath)
        renderer.SetCamera(camera)

        let retrievedCamera = renderer.GetCamera()
        XCTAssertTrue(retrievedCamera.useSceneCamera)
    }

    func testHydraRendererSelection() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        let paths = [
            swiftusd.Path("/World/Mesh1"),
            swiftusd.Path("/World/Mesh2")
        ]
        paths.withUnsafeBufferPointer { buffer in
            renderer.SetSelection(buffer.baseAddress, buffer.count)
        }

        XCTAssertEqual(Int(renderer.GetSelectionCount()), 2)

        renderer.ClearSelection()
        XCTAssertEqual(Int(renderer.GetSelectionCount()), 0)
    }

    func testHydraRendererRender() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetViewport(64, 64)

        let frame = renderer.Render()
        XCTAssertNotNil(frame)

        if let frame = frame {
            XCTAssertEqual(Int(frame.GetWidth()), 64)
            XCTAssertEqual(Int(frame.GetHeight()), 64)
            XCTAssertTrue(frame.IsValid())
            XCTAssertNotNil(frame.GetColorData())
        }
    }

    func testHydraRendererStats() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetViewport(32, 32)
        _ = renderer.Render()

        let stats = renderer.GetStats()
        // Stats should be valid after render
        XCTAssertTrue(stats.isConverged)
    }

    func testHydraRendererInvalidate() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        // Should not throw
        renderer.Invalidate()
        renderer.GarbageCollect()
    }

    // MARK: - RenderFrame Tests

    func testRenderFrameProperties() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetViewport(128, 64)

        guard let frame = renderer.Render() else {
            XCTFail("Failed to render frame")
            return
        }

        XCTAssertEqual(Int(frame.GetWidth()), 128)
        XCTAssertEqual(Int(frame.GetHeight()), 64)
        XCTAssertTrue(frame.IsValid())
        XCTAssertTrue(frame.GetColorDataSize() > 0)
        XCTAssertTrue(frame.GetDepthDataSize() > 0)
        XCTAssertEqual(Int(frame.GetBytesPerRow()), 128 * 4 * MemoryLayout<Float>.size)
    }

    func testRenderFrameColorData() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetViewport(8, 8)

        guard let frame = renderer.Render() else {
            XCTFail("Failed to render frame")
            return
        }

        // Verify color data is accessible
        if let colorData = frame.GetColorData() {
            // Check first pixel RGBA values are valid floats
            XCTAssertTrue(colorData[0] >= 0 && colorData[0] <= 1)  // R
            XCTAssertTrue(colorData[1] >= 0 && colorData[1] <= 1)  // G
            XCTAssertTrue(colorData[2] >= 0 && colorData[2] <= 1)  // B
            XCTAssertTrue(colorData[3] >= 0 && colorData[3] <= 1)  // A
        } else {
            XCTFail("Color data is nil")
        }
    }

    func testRenderFrameDepthData() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        renderer.SetViewport(8, 8)

        guard let frame = renderer.Render() else {
            XCTFail("Failed to render frame")
            return
        }

        // Verify depth data is accessible
        if let depthData = frame.GetDepthData() {
            // Depth should be 1.0 (far plane) for empty scene
            XCTAssertEqual(depthData[0], 1.0, accuracy: 0.001)
        } else {
            XCTFail("Depth data is nil")
        }
    }

    // MARK: - Global Function Tests

    func testSwiftRenderingGetColorSpaceToken() {
        let token = swiftusd.SwiftRenderingGetColorSpaceToken(.SRGB)
        XCTAssertEqual(String(cString: token.GetText()), "sRGB")
    }

    func testSwiftRenderingGetColorSpaceName() {
        let name = String(cString: swiftusd.SwiftRenderingGetColorSpaceName(.Linear))
        XCTAssertEqual(name, "Linear")
    }

    func testSwiftRenderingGetDrawModeToken() {
        let token = swiftusd.SwiftRenderingGetDrawModeToken(.Wireframe)
        XCTAssertEqual(String(cString: token.GetText()), "wireframe")
    }

    func testSwiftRenderingGetDrawModeName() {
        let name = String(cString: swiftusd.SwiftRenderingGetDrawModeName(.Shaded))
        XCTAssertEqual(name, "Shaded")
    }

#if os(macOS) || os(iOS) || os(visionOS)
    // MARK: - PlatformView Tests (Apple platforms only)

    func testPlatformViewCreate() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        let view = swiftusd.PlatformViewRef.Create(renderer)
        XCTAssertNotNil(view)

        if let view = view {
            XCTAssertNotNil(view.GetRenderer())
        }
    }

    func testPlatformViewDrawableSize() {
        guard let renderer = swiftusd.HydraRendererRef.Create() else {
            XCTFail("Failed to create renderer")
            return
        }

        guard let view = swiftusd.PlatformViewRef.Create(renderer) else {
            XCTFail("Failed to create platform view")
            return
        }

        view.SetDrawableSize(1920, 1080)
        XCTAssertEqual(Int(view.GetDrawableWidth()), 1920)
        XCTAssertEqual(Int(view.GetDrawableHeight()), 1080)
    }

    func testHydraRendererCreateMetal() {
        let renderer = swiftusd.HydraRendererRef.CreateMetal(nil)
        XCTAssertNotNil(renderer)

        if let renderer = renderer {
            XCTAssertTrue(renderer.IsValid())
            XCTAssertEqual(String(cString: renderer.GetGraphicsAPIName().GetText()), "Metal")
        }
    }
#endif
}
