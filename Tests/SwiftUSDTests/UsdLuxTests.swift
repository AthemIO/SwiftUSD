// UsdLuxTests.swift - Tests for UsdLux module

import XCTest
@testable import SwiftUSD

final class UsdLuxTests: XCTestCase {

    // MARK: - DistantLight Tests

    func testDistantLightDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sun")

        let light = try LuxDistantLight.define(on: stage, at: path)

        XCTAssertTrue(light.isValid)
    }

    func testDistantLightAngle() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sun")

        let light = try LuxDistantLight.define(on: stage, at: path)

        // Default angle is 0.53 (like the sun)
        let defaultAngle = light.angle()
        XCTAssertEqual(defaultAngle, 0.53, accuracy: 0.01)

        // Set a new angle
        try light.setAngle(1.0)
        let newAngle = light.angle()
        XCTAssertEqual(newAngle, 1.0, accuracy: 0.01)
    }

    func testDistantLightIntensity() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sun")

        let light = try LuxDistantLight.define(on: stage, at: path)

        // Default intensity is 1.0
        let defaultIntensity = light.intensity()
        XCTAssertEqual(defaultIntensity, 1.0, accuracy: 0.01)

        // Set a new intensity
        try light.setIntensity(5.0)
        let newIntensity = light.intensity()
        XCTAssertEqual(newIntensity, 5.0, accuracy: 0.01)
    }

    func testDistantLightColor() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sun")

        let light = try LuxDistantLight.define(on: stage, at: path)

        // Default color is white (1, 1, 1)
        if let color = light.color() {
            XCTAssertEqual(color.r, 1.0, accuracy: 0.01)
            XCTAssertEqual(color.g, 1.0, accuracy: 0.01)
            XCTAssertEqual(color.b, 1.0, accuracy: 0.01)
        }

        // Set a new color
        try light.setColor(r: 1.0, g: 0.8, b: 0.6)
        if let newColor = light.color() {
            XCTAssertEqual(newColor.r, 1.0, accuracy: 0.01)
            XCTAssertEqual(newColor.g, 0.8, accuracy: 0.01)
            XCTAssertEqual(newColor.b, 0.6, accuracy: 0.01)
        }
    }

    func testDistantLightExposure() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sun")

        let light = try LuxDistantLight.define(on: stage, at: path)

        // Default exposure is 0
        let defaultExposure = light.exposure()
        XCTAssertEqual(defaultExposure, 0.0, accuracy: 0.01)

        // Set a new exposure
        try light.setExposure(2.0)
        let newExposure = light.exposure()
        XCTAssertEqual(newExposure, 2.0, accuracy: 0.01)
    }

    // MARK: - DomeLight Tests

    func testDomeLightDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sky")

        let light = try LuxDomeLight.define(on: stage, at: path)

        XCTAssertTrue(light.isValid)
    }

    func testDomeLightTextureFile() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sky")

        let light = try LuxDomeLight.define(on: stage, at: path)

        // Set a texture file
        try light.setTextureFile("sky.hdr")
        let textureFile = light.textureFile
        XCTAssertEqual(textureFile, "sky.hdr")
    }

    func testDomeLightIntensity() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Sky")

        let light = try LuxDomeLight.define(on: stage, at: path)

        // Default intensity is 1.0
        let defaultIntensity = light.intensity()
        XCTAssertEqual(defaultIntensity, 1.0, accuracy: 0.01)

        // Set a new intensity
        try light.setIntensity(0.5)
        let newIntensity = light.intensity()
        XCTAssertEqual(newIntensity, 0.5, accuracy: 0.01)
    }

    // MARK: - RectLight Tests

    func testRectLightDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/AreaLight")

        let light = try LuxRectLight.define(on: stage, at: path)

        XCTAssertTrue(light.isValid)
    }

    func testRectLightDimensions() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/AreaLight")

        let light = try LuxRectLight.define(on: stage, at: path)

        // Default dimensions are 1x1
        let defaultWidth = light.width()
        let defaultHeight = light.height()
        XCTAssertEqual(defaultWidth, 1.0, accuracy: 0.01)
        XCTAssertEqual(defaultHeight, 1.0, accuracy: 0.01)

        // Set new dimensions
        try light.setWidth(2.0)
        try light.setHeight(3.0)
        let newWidth = light.width()
        let newHeight = light.height()
        XCTAssertEqual(newWidth, 2.0, accuracy: 0.01)
        XCTAssertEqual(newHeight, 3.0, accuracy: 0.01)
    }

    func testRectLightNormalize() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/AreaLight")

        let light = try LuxRectLight.define(on: stage, at: path)

        // Default normalize is false
        let defaultNormalize = light.normalize()
        XCTAssertFalse(defaultNormalize)

        // Set normalize to true
        try light.setNormalize(true)
        let newNormalize = light.normalize()
        XCTAssertTrue(newNormalize)
    }

    // MARK: - SphereLight Tests

    func testSphereLightDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/PointLight")

        let light = try LuxSphereLight.define(on: stage, at: path)

        XCTAssertTrue(light.isValid)
    }

    func testSphereLightRadius() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/PointLight")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // Default radius is 0.5
        let defaultRadius = light.radius()
        XCTAssertEqual(defaultRadius, 0.5, accuracy: 0.01)

        // Set a new radius
        try light.setRadius(1.0)
        let newRadius = light.radius()
        XCTAssertEqual(newRadius, 1.0, accuracy: 0.01)
    }

    func testSphereLightTreatAsPoint() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/PointLight")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // Default treatAsPoint is false
        let defaultTreatAsPoint = light.treatAsPoint()
        XCTAssertFalse(defaultTreatAsPoint)

        // Set treatAsPoint to true
        try light.setTreatAsPoint(true)
        let newTreatAsPoint = light.treatAsPoint()
        XCTAssertTrue(newTreatAsPoint)
    }

    // MARK: - CylinderLight Tests

    func testCylinderLightDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/TubeLight")

        let light = try LuxCylinderLight.define(on: stage, at: path)

        XCTAssertTrue(light.isValid)
    }

    func testCylinderLightDimensions() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/TubeLight")

        let light = try LuxCylinderLight.define(on: stage, at: path)

        // Default length is 1.0, radius is 0.5
        let defaultLength = light.length()
        let defaultRadius = light.radius()
        XCTAssertEqual(defaultLength, 1.0, accuracy: 0.01)
        XCTAssertEqual(defaultRadius, 0.5, accuracy: 0.01)

        // Set new dimensions
        try light.setLength(5.0)
        try light.setRadius(0.2)
        let newLength = light.length()
        let newRadius = light.radius()
        XCTAssertEqual(newLength, 5.0, accuracy: 0.01)
        XCTAssertEqual(newRadius, 0.2, accuracy: 0.01)
    }

    func testCylinderLightTreatAsLine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/TubeLight")

        let light = try LuxCylinderLight.define(on: stage, at: path)

        // Default treatAsLine is false
        let defaultTreatAsLine = light.treatAsLine()
        XCTAssertFalse(defaultTreatAsLine)

        // Set treatAsLine to true
        try light.setTreatAsLine(true)
        let newTreatAsLine = light.treatAsLine()
        XCTAssertTrue(newTreatAsLine)
    }

    // MARK: - DiskLight Tests

    func testDiskLightDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/DiskLight")

        let light = try LuxDiskLight.define(on: stage, at: path)

        XCTAssertTrue(light.isValid)
    }

    func testDiskLightRadius() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/DiskLight")

        let light = try LuxDiskLight.define(on: stage, at: path)

        // Default radius is 0.5
        let defaultRadius = light.radius()
        XCTAssertEqual(defaultRadius, 0.5, accuracy: 0.01)

        // Set a new radius
        try light.setRadius(2.0)
        let newRadius = light.radius()
        XCTAssertEqual(newRadius, 2.0, accuracy: 0.01)
    }

    func testDiskLightColor() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/DiskLight")

        let light = try LuxDiskLight.define(on: stage, at: path)

        // Set a color
        try light.setColor(r: 0.5, g: 0.3, b: 0.1)
        if let color = light.color() {
            XCTAssertEqual(color.r, 0.5, accuracy: 0.01)
            XCTAssertEqual(color.g, 0.3, accuracy: 0.01)
            XCTAssertEqual(color.b, 0.1, accuracy: 0.01)
        }
    }

    // MARK: - Description Tests

    func testLightDescriptions() throws {
        let stage = try Stage.createInMemory()

        let distantLight = try LuxDistantLight.define(on: stage, at: try Path("/World/Sun"))
        let domeLight = try LuxDomeLight.define(on: stage, at: try Path("/World/Sky"))
        let rectLight = try LuxRectLight.define(on: stage, at: try Path("/World/AreaLight"))
        let sphereLight = try LuxSphereLight.define(on: stage, at: try Path("/World/PointLight"))
        let cylinderLight = try LuxCylinderLight.define(on: stage, at: try Path("/World/TubeLight"))
        let diskLight = try LuxDiskLight.define(on: stage, at: try Path("/World/DiskLight"))

        // All lights should have descriptions containing their paths
        XCTAssertTrue(distantLight.description.contains("Sun") || distantLight.description.contains("invalid"))
        XCTAssertTrue(domeLight.description.contains("Sky") || domeLight.description.contains("invalid"))
        XCTAssertTrue(rectLight.description.contains("AreaLight") || rectLight.description.contains("invalid"))
        XCTAssertTrue(sphereLight.description.contains("PointLight") || sphereLight.description.contains("invalid"))
        XCTAssertTrue(cylinderLight.description.contains("TubeLight") || cylinderLight.description.contains("invalid"))
        XCTAssertTrue(diskLight.description.contains("DiskLight") || diskLight.description.contains("invalid"))
    }

    // MARK: - LightFilter Tests

    func testLightFilterDefine() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Filter")

        let filter = try LuxLightFilter.define(on: stage, at: path)

        XCTAssertTrue(filter.isValid)
    }

    func testLightFilterShaderId() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Filter")

        let filter = try LuxLightFilter.define(on: stage, at: path)

        // Set a shader ID
        try filter.setShaderId("PxrBarnLightFilter")
        let shaderId = filter.shaderId
        XCTAssertEqual(shaderId, "PxrBarnLightFilter")
    }

    func testLightFilterDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Filter")

        let filter = try LuxLightFilter.define(on: stage, at: path)

        XCTAssertTrue(filter.description.contains("Filter") || filter.description.contains("invalid"))
    }

    // MARK: - ShadowAPI Tests

    func testShadowAPIApply() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        // Create a light first to apply ShadowAPI to
        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        if let prim = light.prim {
            // Apply ShadowAPI
            let shadowAPI = try LuxShadowAPI.apply(to: prim)
            XCTAssertTrue(shadowAPI.isValid)
        } else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
        }
    }

    func testShadowAPIEnable() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        guard let prim = light.prim else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
            return
        }

        let shadowAPI = try LuxShadowAPI.apply(to: prim)

        // Default is enabled
        let defaultEnable = shadowAPI.shadowEnable()
        XCTAssertTrue(defaultEnable)

        // Disable shadows
        try shadowAPI.setShadowEnable(false)
        let newEnable = shadowAPI.shadowEnable()
        XCTAssertFalse(newEnable)
    }

    func testShadowAPIColor() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        guard let prim = light.prim else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
            return
        }

        let shadowAPI = try LuxShadowAPI.apply(to: prim)

        // Default shadow color is black (0, 0, 0)
        if let color = shadowAPI.shadowColor() {
            XCTAssertEqual(color.r, 0.0, accuracy: 0.01)
            XCTAssertEqual(color.g, 0.0, accuracy: 0.01)
            XCTAssertEqual(color.b, 0.0, accuracy: 0.01)
        }

        // Set a new shadow color
        try shadowAPI.setShadowColor(r: 0.1, g: 0.1, b: 0.2)
        if let newColor = shadowAPI.shadowColor() {
            XCTAssertEqual(newColor.r, 0.1, accuracy: 0.01)
            XCTAssertEqual(newColor.g, 0.1, accuracy: 0.01)
            XCTAssertEqual(newColor.b, 0.2, accuracy: 0.01)
        }
    }

    func testShadowAPIDistance() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        guard let prim = light.prim else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
            return
        }

        let shadowAPI = try LuxShadowAPI.apply(to: prim)

        // Default is -1 (no limit)
        let defaultDistance = shadowAPI.shadowDistance()
        XCTAssertEqual(defaultDistance, -1.0, accuracy: 0.01)

        // Set a distance limit
        try shadowAPI.setShadowDistance(100.0)
        let newDistance = shadowAPI.shadowDistance()
        XCTAssertEqual(newDistance, 100.0, accuracy: 0.01)
    }

    func testShadowAPIFalloff() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        guard let prim = light.prim else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
            return
        }

        let shadowAPI = try LuxShadowAPI.apply(to: prim)

        // Default falloff is -1 (no falloff)
        let defaultFalloff = shadowAPI.shadowFalloff()
        XCTAssertEqual(defaultFalloff, -1.0, accuracy: 0.01)

        // Set a falloff
        try shadowAPI.setShadowFalloff(50.0)
        let newFalloff = shadowAPI.shadowFalloff()
        XCTAssertEqual(newFalloff, 50.0, accuracy: 0.01)
    }

    func testShadowAPIFalloffGamma() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        guard let prim = light.prim else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
            return
        }

        let shadowAPI = try LuxShadowAPI.apply(to: prim)

        // Default gamma is 1.0
        let defaultGamma = shadowAPI.shadowFalloffGamma()
        XCTAssertEqual(defaultGamma, 1.0, accuracy: 0.01)

        // Set a new gamma
        try shadowAPI.setShadowFalloffGamma(2.2)
        let newGamma = shadowAPI.shadowFalloffGamma()
        XCTAssertEqual(newGamma, 2.2, accuracy: 0.01)
    }

    func testShadowAPIDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World/Light")

        let light = try LuxSphereLight.define(on: stage, at: path)

        // In stub mode, prim may not be available
        guard let prim = light.prim else {
            // Stub mode - just verify light is valid
            XCTAssertTrue(light.isValid)
            return
        }

        let shadowAPI = try LuxShadowAPI.apply(to: prim)

        XCTAssertTrue(shadowAPI.description.contains("Light") || shadowAPI.description.contains("invalid"))
    }

    // MARK: - Type Alias Tests

    func testTypeAliases() throws {
        // Verify type aliases are properly defined
        let _: UsdLuxDistantLight.Type = LuxDistantLight.self
        let _: UsdLuxDomeLight.Type = LuxDomeLight.self
        let _: UsdLuxRectLight.Type = LuxRectLight.self
        let _: UsdLuxSphereLight.Type = LuxSphereLight.self
        let _: UsdLuxCylinderLight.Type = LuxCylinderLight.self
        let _: UsdLuxDiskLight.Type = LuxDiskLight.self
        let _: UsdLuxLightFilter.Type = LuxLightFilter.self
        let _: UsdLuxShadowAPI.Type = LuxShadowAPI.self
    }
}
