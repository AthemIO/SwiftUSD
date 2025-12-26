// UsdLuxTests.swift - Tests for UsdLux module

import XCTest
import SwiftUSD
@testable import CxxFacade

final class UsdLuxTests: XCTestCase {

    // MARK: - Token Tests

    func testTokens() {
        // Test light type tokens
        let distantToken = cxxfacade.usdLux.UsdLuxTokens.distantLight()
        XCTAssertEqual(distantToken.GetString(), "DistantLight")

        let domeToken = cxxfacade.usdLux.UsdLuxTokens.domeLight()
        XCTAssertEqual(domeToken.GetString(), "DomeLight")

        let rectToken = cxxfacade.usdLux.UsdLuxTokens.rectLight()
        XCTAssertEqual(rectToken.GetString(), "RectLight")

        let sphereToken = cxxfacade.usdLux.UsdLuxTokens.sphereLight()
        XCTAssertEqual(sphereToken.GetString(), "SphereLight")

        let cylinderToken = cxxfacade.usdLux.UsdLuxTokens.cylinderLight()
        XCTAssertEqual(cylinderToken.GetString(), "CylinderLight")

        let diskToken = cxxfacade.usdLux.UsdLuxTokens.diskLight()
        XCTAssertEqual(diskToken.GetString(), "DiskLight")

        // Test texture format tokens
        let automaticToken = cxxfacade.usdLux.UsdLuxTokens.automatic()
        XCTAssertFalse(automaticToken.GetString().isEmpty)

        let latlongToken = cxxfacade.usdLux.UsdLuxTokens.latlong()
        XCTAssertFalse(latlongToken.GetString().isEmpty)
    }

    // MARK: - Blackbody Temperature Tests

    func testBlackbodyTemperature() {
        // Test daylight (6500K)
        let daylight = blackbodyTemperatureAsRgb(6500)
        XCTAssertGreaterThan(daylight.x, 0)
        XCTAssertGreaterThan(daylight.y, 0)
        XCTAssertGreaterThan(daylight.z, 0)

        // Test warm (3000K - incandescent)
        let warm = blackbodyTemperatureAsRgb(3000)
        XCTAssertGreaterThan(warm.x, 0)
        // Warm light should be more red than blue
        XCTAssertGreaterThan(warm.x, warm.z)

        // Test cool (10000K)
        let cool = blackbodyTemperatureAsRgb(10000)
        XCTAssertGreaterThan(cool.x, 0)
        XCTAssertGreaterThan(cool.y, 0)
        XCTAssertGreaterThan(cool.z, 0)
    }

    // MARK: - Light Type Creation Tests

    func testDistantLightDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let light = UsdLuxDistantLight.define(&stage, path: "/lights/sun")
        XCTAssertTrue(light.GetPrim().IsValid())
        XCTAssertEqual(light.GetPrim().GetPath().GetString(), "/lights/sun")

        // Test angle attribute
        let angleAttr = light.angle
        XCTAssertTrue(angleAttr.IsValid())
    }

    func testDomeLightDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let dome = UsdLuxDomeLight.define(&stage, path: "/lights/dome")
        XCTAssertTrue(dome.GetPrim().IsValid())
        XCTAssertEqual(dome.GetPrim().GetPath().GetString(), "/lights/dome")

        // Test texture file attribute
        let textureFileAttr = dome.textureFile
        XCTAssertTrue(textureFileAttr.IsValid())

        // Test texture format attribute
        let textureFormatAttr = dome.textureFormat
        XCTAssertTrue(textureFormatAttr.IsValid())

        // Test guide radius attribute
        let guideRadiusAttr = dome.guideRadius
        XCTAssertTrue(guideRadiusAttr.IsValid())
    }

    func testRectLightDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let rect = UsdLuxRectLight.define(&stage, path: "/lights/rect")
        XCTAssertTrue(rect.GetPrim().IsValid())

        // Test width and height attributes
        let widthAttr = rect.width
        XCTAssertTrue(widthAttr.IsValid())

        let heightAttr = rect.height
        XCTAssertTrue(heightAttr.IsValid())
    }

    func testSphereLightDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let sphere = UsdLuxSphereLight.define(&stage, path: "/lights/point")
        XCTAssertTrue(sphere.GetPrim().IsValid())

        // Test radius attribute
        let radiusAttr = sphere.radius
        XCTAssertTrue(radiusAttr.IsValid())

        // Test treatAsPoint attribute
        let treatAsPointAttr = sphere.treatAsPoint
        XCTAssertTrue(treatAsPointAttr.IsValid())
    }

    func testCylinderLightDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let cylinder = UsdLuxCylinderLight.define(&stage, path: "/lights/tube")
        XCTAssertTrue(cylinder.GetPrim().IsValid())

        // Test length and radius attributes
        let lengthAttr = cylinder.length
        XCTAssertTrue(lengthAttr.IsValid())

        let radiusAttr = cylinder.radius
        XCTAssertTrue(radiusAttr.IsValid())

        // Test treatAsLine attribute
        let treatAsLineAttr = cylinder.treatAsLine
        XCTAssertTrue(treatAsLineAttr.IsValid())
    }

    func testDiskLightDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let disk = UsdLuxDiskLight.define(&stage, path: "/lights/disk")
        XCTAssertTrue(disk.GetPrim().IsValid())

        // Test radius attribute
        let radiusAttr = disk.radius
        XCTAssertTrue(radiusAttr.IsValid())
    }

    // MARK: - LightAPI Tests

    func testLightAPIAttributes() {
        var stage = UsdStage.CreateInMemory()
        let sphere = UsdLuxSphereLight.define(&stage, path: "/lights/sphere")

        let lightAPI = sphere.lightAPI()

        // Test intensity attribute
        let intensityAttr = lightAPI.intensity
        XCTAssertTrue(intensityAttr.IsValid())

        // Test exposure attribute
        let exposureAttr = lightAPI.exposure
        XCTAssertTrue(exposureAttr.IsValid())

        // Test color attribute
        let colorAttr = lightAPI.color
        XCTAssertTrue(colorAttr.IsValid())

        // Test diffuse attribute
        let diffuseAttr = lightAPI.diffuse
        XCTAssertTrue(diffuseAttr.IsValid())

        // Test specular attribute
        let specularAttr = lightAPI.specular
        XCTAssertTrue(specularAttr.IsValid())

        // Test normalize attribute
        let normalizeAttr = lightAPI.normalize
        XCTAssertTrue(normalizeAttr.IsValid())

        // Test color temperature attributes
        let enableColorTempAttr = lightAPI.enableColorTemperature
        XCTAssertTrue(enableColorTempAttr.IsValid())

        let colorTempAttr = lightAPI.colorTemperature
        XCTAssertTrue(colorTempAttr.IsValid())
    }

    // MARK: - ShadowAPI Tests

    func testShadowAPI() {
        var stage = UsdStage.CreateInMemory()
        let sphere = UsdLuxSphereLight.define(&stage, path: "/lights/sphere")
        let prim = sphere.GetPrim()

        // Apply ShadowAPI
        let shadowAPI = UsdLuxShadowAPI.apply(prim)
        XCTAssertTrue(shadowAPI.GetPrim().IsValid())

        // Test shadow enable attribute
        let enableAttr = shadowAPI.shadowEnable
        XCTAssertTrue(enableAttr.IsValid())

        // Test shadow color attribute
        let colorAttr = shadowAPI.shadowColor
        XCTAssertTrue(colorAttr.IsValid())

        // Test shadow distance attribute
        let distanceAttr = shadowAPI.shadowDistance
        XCTAssertTrue(distanceAttr.IsValid())

        // Test shadow falloff attribute
        let falloffAttr = shadowAPI.shadowFalloff
        XCTAssertTrue(falloffAttr.IsValid())

        // Test shadow falloff gamma attribute
        let falloffGammaAttr = shadowAPI.shadowFalloffGamma
        XCTAssertTrue(falloffGammaAttr.IsValid())
    }

    // MARK: - ShapingAPI Tests

    func testShapingAPI() {
        var stage = UsdStage.CreateInMemory()
        let sphere = UsdLuxSphereLight.define(&stage, path: "/lights/sphere")
        let prim = sphere.GetPrim()

        // Apply ShapingAPI
        let shapingAPI = UsdLuxShapingAPI.apply(prim)
        XCTAssertTrue(shapingAPI.GetPrim().IsValid())

        // Test focus attribute
        let focusAttr = shapingAPI.focus
        XCTAssertTrue(focusAttr.IsValid())

        // Test focus tint attribute
        let focusTintAttr = shapingAPI.focusTint
        XCTAssertTrue(focusTintAttr.IsValid())

        // Test cone angle attribute
        let coneAngleAttr = shapingAPI.coneAngle
        XCTAssertTrue(coneAngleAttr.IsValid())

        // Test cone softness attribute
        let coneSoftnessAttr = shapingAPI.coneSoftness
        XCTAssertTrue(coneSoftnessAttr.IsValid())

        // Test IES file attribute
        let iesFileAttr = shapingAPI.iesFile
        XCTAssertTrue(iesFileAttr.IsValid())

        // Test IES angle scale attribute
        let iesAngleScaleAttr = shapingAPI.iesAngleScale
        XCTAssertTrue(iesAngleScaleAttr.IsValid())

        // Test IES normalize attribute
        let iesNormalizeAttr = shapingAPI.iesNormalize
        XCTAssertTrue(iesNormalizeAttr.IsValid())
    }

    // MARK: - Light Filter Tests

    func testLightFilterDefinition() {
        var stage = UsdStage.CreateInMemory()
        XCTAssertTrue(stage.IsValid())

        let filter = UsdLuxLightFilter.define(&stage, path: "/lights/filter")
        XCTAssertTrue(filter.GetPrim().IsValid())

        // Test shader ID attribute
        let shaderIdAttr = filter.shaderId
        XCTAssertTrue(shaderIdAttr.IsValid())
    }

    // MARK: - Multiple Lights Test

    func testMultipleLights() {
        var stage = UsdStage.CreateInMemory()

        // Create various light types
        let sun = UsdLuxDistantLight.define(&stage, path: "/lights/sun")
        let sky = UsdLuxDomeLight.define(&stage, path: "/lights/sky")
        let key = UsdLuxRectLight.define(&stage, path: "/lights/key")
        let fill = UsdLuxSphereLight.define(&stage, path: "/lights/fill")
        let rim = UsdLuxCylinderLight.define(&stage, path: "/lights/rim")
        let accent = UsdLuxDiskLight.define(&stage, path: "/lights/accent")

        // Verify all lights were created
        XCTAssertTrue(sun.GetPrim().IsValid())
        XCTAssertTrue(sky.GetPrim().IsValid())
        XCTAssertTrue(key.GetPrim().IsValid())
        XCTAssertTrue(fill.GetPrim().IsValid())
        XCTAssertTrue(rim.GetPrim().IsValid())
        XCTAssertTrue(accent.GetPrim().IsValid())

        // Verify paths
        XCTAssertEqual(sun.GetPrim().GetPath().GetString(), "/lights/sun")
        XCTAssertEqual(sky.GetPrim().GetPath().GetString(), "/lights/sky")
        XCTAssertEqual(key.GetPrim().GetPath().GetString(), "/lights/key")
        XCTAssertEqual(fill.GetPrim().GetPath().GetString(), "/lights/fill")
        XCTAssertEqual(rim.GetPrim().GetPath().GetString(), "/lights/rim")
        XCTAssertEqual(accent.GetPrim().GetPath().GetString(), "/lights/accent")
    }
}
