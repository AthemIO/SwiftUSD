// UsdImagingTests.swift - Tests for UsdImaging module

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdImagingTests: XCTestCase {

    // MARK: - UsdImagingDelegateParams Tests

    func testDelegateParamsDefaultValues() {
        let params = swiftusd.UsdImagingDelegateParams()

        XCTAssertTrue(params.resolveInstanceProxies)
        XCTAssertFalse(params.excludeInvisiblePrims)
        XCTAssertEqual(params.refineLevel, 0)
        XCTAssertTrue(params.sceneMaterialsEnabled)
        XCTAssertTrue(params.sceneLightsEnabled)
    }

    func testDelegateParamsEquality() {
        let params1 = swiftusd.UsdImagingDelegateParams()
        var params2 = swiftusd.UsdImagingDelegateParams()

        // Use the C++ operator== via the == method
        XCTAssertTrue(params1 == params2)

        params2.refineLevel = 3
        XCTAssertTrue(params1 != params2)
    }

    func testDelegateParamsMake() {
        let params = swiftusd.UsdImagingDelegateParams.make(
            refineLevel: 4,
            sceneMaterialsEnabled: false,
            sceneLightsEnabled: true
        )

        XCTAssertEqual(params.refineLevel, 4)
        XCTAssertFalse(params.sceneMaterialsEnabled)
        XCTAssertTrue(params.sceneLightsEnabled)
    }

    // MARK: - UsdImagingTokens Tests

    func testMaterialPurposeTokens() {
        let allPurpose = swiftusd.UsdImagingTokens.AllPurpose()
        let preview = swiftusd.UsdImagingTokens.Preview()
        let full = swiftusd.UsdImagingTokens.Full()

        XCTAssertEqual(String(cString: allPurpose.GetText()), "allPurpose")
        XCTAssertEqual(String(cString: preview.GetText()), "preview")
        XCTAssertEqual(String(cString: full.GetText()), "full")
    }

    func testVisibilityTokens() {
        let invisible = swiftusd.UsdImagingTokens.Invisible()
        let visible = swiftusd.UsdImagingTokens.Visible()
        let inherited = swiftusd.UsdImagingTokens.Inherited()

        XCTAssertEqual(String(cString: invisible.GetText()), "invisible")
        XCTAssertEqual(String(cString: visible.GetText()), "visible")
        XCTAssertEqual(String(cString: inherited.GetText()), "inherited")
    }

    func testRenderTagTokens() {
        let geometry = swiftusd.UsdImagingTokens.Geometry()
        let guide = swiftusd.UsdImagingTokens.Guide()
        let proxy = swiftusd.UsdImagingTokens.Proxy()
        let render = swiftusd.UsdImagingTokens.Render()

        XCTAssertEqual(String(cString: geometry.GetText()), "geometry")
        XCTAssertEqual(String(cString: guide.GetText()), "guide")
        XCTAssertEqual(String(cString: proxy.GetText()), "proxy")
        XCTAssertEqual(String(cString: render.GetText()), "render")
    }

    func testPrimTypeTokens() {
        let mesh = swiftusd.UsdImagingTokens.Mesh()
        let basisCurves = swiftusd.UsdImagingTokens.BasisCurves()
        let points = swiftusd.UsdImagingTokens.Points()
        let sphere = swiftusd.UsdImagingTokens.Sphere()
        let cube = swiftusd.UsdImagingTokens.Cube()

        XCTAssertEqual(String(cString: mesh.GetText()), "Mesh")
        XCTAssertEqual(String(cString: basisCurves.GetText()), "BasisCurves")
        XCTAssertEqual(String(cString: points.GetText()), "Points")
        XCTAssertEqual(String(cString: sphere.GetText()), "Sphere")
        XCTAssertEqual(String(cString: cube.GetText()), "Cube")
    }

    func testLightTypeTokens() {
        let domeLight = swiftusd.UsdImagingTokens.DomeLight()
        let distantLight = swiftusd.UsdImagingTokens.DistantLight()
        let sphereLight = swiftusd.UsdImagingTokens.SphereLight()
        let rectLight = swiftusd.UsdImagingTokens.RectLight()

        XCTAssertEqual(String(cString: domeLight.GetText()), "DomeLight")
        XCTAssertEqual(String(cString: distantLight.GetText()), "DistantLight")
        XCTAssertEqual(String(cString: sphereLight.GetText()), "SphereLight")
        XCTAssertEqual(String(cString: rectLight.GetText()), "RectLight")
    }

    func testCameraTokens() {
        let camera = swiftusd.UsdImagingTokens.Camera()
        let perspective = swiftusd.UsdImagingTokens.Perspective()
        let orthographic = swiftusd.UsdImagingTokens.Orthographic()

        XCTAssertEqual(String(cString: camera.GetText()), "Camera")
        XCTAssertEqual(String(cString: perspective.GetText()), "perspective")
        XCTAssertEqual(String(cString: orthographic.GetText()), "orthographic")
    }

    // MARK: - Utility Function Tests

    func testMaterialPurposeUtilityFunctions() {
        let defaultPurpose = swiftusd.UsdImagingGetDefaultMaterialPurpose()
        let previewPurpose = swiftusd.UsdImagingGetPreviewMaterialPurpose()
        let fullPurpose = swiftusd.UsdImagingGetFullMaterialPurpose()

        XCTAssertEqual(String(cString: defaultPurpose.GetText()), "allPurpose")
        XCTAssertEqual(String(cString: previewPurpose.GetText()), "preview")
        XCTAssertEqual(String(cString: fullPurpose.GetText()), "full")
    }

    // MARK: - Swift Extension Tests

    func testSwiftTokenExtensions() {
        // Material purpose tokens
        XCTAssertEqual(String(cString: swiftusd.Token.allPurpose.GetText()), "allPurpose")
        XCTAssertEqual(String(cString: swiftusd.Token.preview.GetText()), "preview")
        XCTAssertEqual(String(cString: swiftusd.Token.full.GetText()), "full")

        // Visibility tokens
        XCTAssertEqual(String(cString: swiftusd.Token.invisible.GetText()), "invisible")
        XCTAssertEqual(String(cString: swiftusd.Token.visible.GetText()), "visible")
        XCTAssertEqual(String(cString: swiftusd.Token.inherited.GetText()), "inherited")

        // Render tag tokens
        XCTAssertEqual(String(cString: swiftusd.Token.geometry.GetText()), "geometry")
        XCTAssertEqual(String(cString: swiftusd.Token.guide.GetText()), "guide")
        XCTAssertEqual(String(cString: swiftusd.Token.proxy.GetText()), "proxy")
        XCTAssertEqual(String(cString: swiftusd.Token.render.GetText()), "render")

        // Prim type tokens
        XCTAssertEqual(String(cString: swiftusd.Token.meshType.GetText()), "Mesh")
        XCTAssertEqual(String(cString: swiftusd.Token.sphereType.GetText()), "Sphere")
        XCTAssertEqual(String(cString: swiftusd.Token.cubeType.GetText()), "Cube")

        // Light tokens
        XCTAssertEqual(String(cString: swiftusd.Token.domeLight.GetText()), "DomeLight")
        XCTAssertEqual(String(cString: swiftusd.Token.distantLight.GetText()), "DistantLight")

        // Camera tokens
        XCTAssertEqual(String(cString: swiftusd.Token.cameraType.GetText()), "Camera")
        XCTAssertEqual(String(cString: swiftusd.Token.perspective.GetText()), "perspective")
    }

    func testSwiftHelperFunctions() {
        let defaultPurpose = usdImagingDefaultMaterialPurpose()
        let previewPurpose = usdImagingPreviewMaterialPurpose()
        let fullPurpose = usdImagingFullMaterialPurpose()

        XCTAssertEqual(String(cString: defaultPurpose.GetText()), "allPurpose")
        XCTAssertEqual(String(cString: previewPurpose.GetText()), "preview")
        XCTAssertEqual(String(cString: fullPurpose.GetText()), "full")
    }

    // MARK: - Type Alias Tests

    func testTypeAliases() {
        // Verify type aliases compile correctly
        let _: UsdImagingDelegate.Type = swiftusd.UsdImagingDelegateRef.self
        let _: UsdImagingDelegateParams.Type = swiftusd.UsdImagingDelegateParams.self
    }
}
