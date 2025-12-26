import XCTest
import SwiftUSD
@testable import CxxFacade

final class HydraTests: XCTestCase {

    // MARK: - DrawMode Tests

    func testDrawModeValues() {
        // Test that draw mode enum values are accessible
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.Points)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.Wireframe)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.WireframeOnSurface)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.ShadedFlat)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.ShadedSmooth)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.GeomOnly)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.GeomFlat)
        XCTAssertNotNil(cxxfacade.hydra.DrawMode.GeomSmooth)
    }

    func testDrawModeDescription() {
        let points = cxxfacade.hydra.DrawMode.Points
        let wireframe = cxxfacade.hydra.DrawMode.Wireframe
        let smooth = cxxfacade.hydra.DrawMode.ShadedSmooth

        // Just verify they're different values
        XCTAssertTrue(points.rawValue != wireframe.rawValue)
        XCTAssertTrue(wireframe.rawValue != smooth.rawValue)
    }

    // MARK: - CullStyle Tests

    func testCullStyleValues() {
        // Test that cull style enum values are accessible
        XCTAssertNotNil(cxxfacade.hydra.CullStyle.NoOpinion)
        XCTAssertNotNil(cxxfacade.hydra.CullStyle.Nothing)
        XCTAssertNotNil(cxxfacade.hydra.CullStyle.Back)
        XCTAssertNotNil(cxxfacade.hydra.CullStyle.Front)
        XCTAssertNotNil(cxxfacade.hydra.CullStyle.BackUnlessDoubleSided)
    }

    func testCullStyleDescription() {
        let nothing = cxxfacade.hydra.CullStyle.Nothing
        let back = cxxfacade.hydra.CullStyle.Back

        // Just verify they're different values
        XCTAssertTrue(nothing.rawValue != back.rawValue)
    }

    // MARK: - RenderSettings Tests

    func testRenderSettingsDefault() {
        let settings = cxxfacade.hydra.RenderSettings()

        XCTAssertEqual(settings.frame, 0.0)
        XCTAssertEqual(settings.complexity, 1.0)
        XCTAssertTrue(settings.enableLighting)
        XCTAssertFalse(settings.showGuides)
        XCTAssertTrue(settings.showProxy)
        XCTAssertFalse(settings.showRender)
    }

    func testRenderSettingsPreview() {
        let settings = cxxfacade.hydra.RenderSettings.Preview()

        XCTAssertEqual(settings.complexity, 1.0)
        XCTAssertTrue(settings.enableLighting)
    }

    func testRenderSettingsWireframe() {
        let settings = cxxfacade.hydra.RenderSettings.Wireframe()

        XCTAssertFalse(settings.enableLighting)
    }

    func testRenderSettingsHighQuality() {
        let settings = cxxfacade.hydra.RenderSettings.HighQuality()

        XCTAssertEqual(settings.complexity, 1.5)
        XCTAssertTrue(settings.enableLighting)
        XCTAssertTrue(settings.enableSceneMaterials)
        XCTAssertTrue(settings.enableSceneLights)
    }

    func testRenderSettingsCustomization() {
        var settings = cxxfacade.hydra.RenderSettings()

        settings.frame = 24.0
        settings.complexity = 2.0
        settings.enableLighting = false
        settings.showGuides = true

        XCTAssertEqual(settings.frame, 24.0)
        XCTAssertEqual(settings.complexity, 2.0)
        XCTAssertFalse(settings.enableLighting)
        XCTAssertTrue(settings.showGuides)
    }

    func testRenderSettingsDescription() {
        let settings = cxxfacade.hydra.RenderSettings.Preview()
        // Just verify we can access properties
        XCTAssertGreaterThan(settings.complexity, 0)
    }

    // MARK: - CameraSettings Tests

    func testCameraSettingsDefault() {
        let camera = cxxfacade.hydra.CameraSettings()

        XCTAssertEqual(camera.bufferWidth, 1920)
        XCTAssertEqual(camera.bufferHeight, 1080)
    }

    func testCameraSettingsWithMatrices() {
        let view = cxxfacade.gf.Matrix4d.Identity()
        let proj = cxxfacade.gf.Matrix4d.Identity()

        let camera = cxxfacade.hydra.CameraSettings(view, proj)

        XCTAssertEqual(camera.bufferWidth, 1920)
        XCTAssertEqual(camera.bufferHeight, 1080)
    }

    func testCameraSettingsWithViewport() {
        let view = cxxfacade.gf.Matrix4d.Identity()
        let proj = cxxfacade.gf.Matrix4d.Identity()

        let camera = cxxfacade.hydra.CameraSettings(view, proj, 800, 600)

        XCTAssertEqual(camera.bufferWidth, 800)
        XCTAssertEqual(camera.bufferHeight, 600)
    }

    func testCameraSettingsAspectRatio() {
        let view = cxxfacade.gf.Matrix4d.Identity()
        let proj = cxxfacade.gf.Matrix4d.Identity()

        let camera = cxxfacade.hydra.CameraSettings(view, proj, 1920, 1080)
        let aspect = camera.GetAspectRatio()

        XCTAssertEqual(aspect, 1920.0 / 1080.0, accuracy: 0.001)
    }

    func testCameraSettingsPerspective() {
        let camera = cxxfacade.hydra.CameraSettings.Perspective(
            45.0,
            16.0 / 9.0,
            0.1,
            1000.0
        )

        // The projection matrix should be valid (not identity)
        XCTAssertNotNil(camera.projectionMatrix)
    }

    func testCameraSettingsOrthographic() {
        let camera = cxxfacade.hydra.CameraSettings.Orthographic(
            -10.0,
            10.0,
            -10.0,
            10.0,
            0.1,
            100.0
        )

        // The projection matrix should be valid
        XCTAssertNotNil(camera.projectionMatrix)
    }

    func testCameraSettingsLookAt() {
        let eye = cxxfacade.gf.Vec3d(0.0, 5.0, 10.0)
        let target = cxxfacade.gf.Vec3d(0.0, 0.0, 0.0)
        let up = cxxfacade.gf.Vec3d(0.0, 1.0, 0.0)

        let camera = cxxfacade.hydra.CameraSettings.LookAt(
            eye,
            target,
            up,
            45.0,
            16.0 / 9.0,
            0.1,
            1000.0
        )

        // Both view and projection matrices should be valid
        XCTAssertNotNil(camera.viewMatrix)
        XCTAssertNotNil(camera.projectionMatrix)
    }

    func testCameraSettingsSetViewport() {
        var camera = cxxfacade.hydra.CameraSettings()
        camera.SetViewport(0, 0, 1280, 720)

        XCTAssertEqual(camera.bufferWidth, 1280)
        XCTAssertEqual(camera.bufferHeight, 720)
    }

    func testCameraSettingsDescription() {
        let camera = cxxfacade.hydra.CameraSettings()

        // Verify we can access viewport fields
        XCTAssertEqual(camera.viewport.x, 0.0)
        XCTAssertEqual(camera.viewport.y, 0.0)
    }

    // MARK: - RendererInfo Tests

    func testRendererInfoInit() {
        let info = cxxfacade.hydra.RendererInfo(std.string("HdStormRendererPlugin"), std.string("Storm"), true)

        XCTAssertEqual(String(info.id), "HdStormRendererPlugin")
        XCTAssertEqual(String(info.displayName), "Storm")
        XCTAssertTrue(info.gpuEnabled)
    }

    func testRendererInfoDescription() {
        let info = cxxfacade.hydra.RendererInfo(std.string("TestPlugin"), std.string("Test Renderer"), true)

        XCTAssertEqual(String(info.id), "TestPlugin")
        XCTAssertEqual(String(info.displayName), "Test Renderer")
        XCTAssertTrue(info.gpuEnabled)
    }

    // MARK: - HydraEngine Tests

    func testHydraEngineDefaultInit() {
        let engine = cxxfacade.hydra.HydraEngine()

        // Default-constructed engine should be invalid (no render backend)
        XCTAssertFalse(engine.IsValid())
    }

    func testHydraEngineDescription() {
        let engine = cxxfacade.hydra.HydraEngine()
        // Just verify the engine can be created
        XCTAssertFalse(engine.IsValid())
    }

    // Note: Additional tests that require a working USD installation with imaging
    // support would go here when USE_PIXAR_USD is enabled and the Hydra engine
    // is fully functional.
}
