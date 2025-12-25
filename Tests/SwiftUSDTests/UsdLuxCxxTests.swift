// UsdLuxCxxTests.swift - Tests for UsdLux C++ interop

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdLuxCxxTests: XCTestCase {

    // MARK: - DistantLight Tests

    func testDistantLightCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Sun")
        guard let light = swiftusd.LuxDistantLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create DistantLight")
            return
        }
        defer { light.release() }

        XCTAssertTrue(light.IsValid())
    }

    func testDistantLightAngle() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Sun")
        guard let light = swiftusd.LuxDistantLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create DistantLight")
            return
        }
        defer { light.release() }

        // Test Swift property wrapper for angle
        light.angle = 1.0
        XCTAssertEqual(light.angle, 1.0, accuracy: 0.001)
    }

    // MARK: - DomeLight Tests

    func testDomeLightCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Sky")
        guard let light = swiftusd.LuxDomeLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create DomeLight")
            return
        }
        defer { light.release() }

        XCTAssertTrue(light.IsValid())
    }

    func testDomeLightProperties() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Sky")
        guard let light = swiftusd.LuxDomeLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create DomeLight")
            return
        }
        defer { light.release() }

        // Test guide radius via Swift property wrapper
        light.guideRadius = 50000.0
        XCTAssertEqual(light.guideRadius, 50000.0, accuracy: 0.1)
    }

    // MARK: - RectLight Tests

    func testRectLightCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/AreaLight")
        guard let light = swiftusd.LuxRectLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create RectLight")
            return
        }
        defer { light.release() }

        XCTAssertTrue(light.IsValid())
    }

    func testRectLightDimensions() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/AreaLight")
        guard let light = swiftusd.LuxRectLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create RectLight")
            return
        }
        defer { light.release() }

        // Test Swift property wrappers
        light.width = 2.0
        light.height = 1.5
        XCTAssertEqual(light.width, 2.0, accuracy: 0.001)
        XCTAssertEqual(light.height, 1.5, accuracy: 0.001)
    }

    // MARK: - SphereLight Tests

    func testSphereLightCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/PointLight")
        guard let light = swiftusd.LuxSphereLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create SphereLight")
            return
        }
        defer { light.release() }

        XCTAssertTrue(light.IsValid())
    }

    func testSphereLightProperties() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/PointLight")
        guard let light = swiftusd.LuxSphereLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create SphereLight")
            return
        }
        defer { light.release() }

        // Test Swift property wrappers
        light.radius = 1.0
        XCTAssertEqual(light.radius, 1.0, accuracy: 0.001)

        light.treatAsPoint = true
        XCTAssertTrue(light.treatAsPoint)
    }

    // MARK: - CylinderLight Tests

    func testCylinderLightCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TubeLight")
        guard let light = swiftusd.LuxCylinderLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create CylinderLight")
            return
        }
        defer { light.release() }

        XCTAssertTrue(light.IsValid())
    }

    func testCylinderLightProperties() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TubeLight")
        guard let light = swiftusd.LuxCylinderLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create CylinderLight")
            return
        }
        defer { light.release() }

        // Test Swift property wrappers
        light.length = 2.0
        light.radius = 0.25
        XCTAssertEqual(light.length, 2.0, accuracy: 0.001)
        XCTAssertEqual(light.radius, 0.25, accuracy: 0.001)

        light.treatAsLine = true
        XCTAssertTrue(light.treatAsLine)
    }

    // MARK: - DiskLight Tests

    func testDiskLightCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DiskLight")
        guard let light = swiftusd.LuxDiskLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create DiskLight")
            return
        }
        defer { light.release() }

        XCTAssertTrue(light.IsValid())
    }

    func testDiskLightProperties() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DiskLight")
        guard let light = swiftusd.LuxDiskLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create DiskLight")
            return
        }
        defer { light.release() }

        // Test Swift property wrapper
        light.radius = 1.0
        XCTAssertEqual(light.radius, 1.0, accuracy: 0.001)
    }

    // MARK: - LightFilter Tests

    func testLightFilterCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Filter")
        guard let filter = swiftusd.LuxLightFilterRef.define(on: stage, at: path) else {
            XCTFail("Failed to create LightFilter")
            return
        }
        defer { filter.release() }

        XCTAssertTrue(filter.IsValid())
    }

    func testLightFilterProperties() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Filter")
        guard let filter = swiftusd.LuxLightFilterRef.define(on: stage, at: path) else {
            XCTFail("Failed to create LightFilter")
            return
        }
        defer { filter.release() }

        // Test Swift property wrapper
        filter.shaderId = swiftusd.Token("myFilter")
        XCTAssertEqual(String(cString: filter.shaderId.GetText()), "myFilter")
    }

    // MARK: - Reference Counting Tests

    func testReferenceCountingLight() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TestLight")
        guard let light = swiftusd.LuxSphereLightRef.define(on: stage, at: path) else {
            XCTFail("Failed to create light")
            return
        }

        // Manually retain and release
        light.retain()
        light.release()

        // Light should still be valid
        XCTAssertTrue(light.IsValid())

        // Final release
        light.release()
    }

    // MARK: - Integration Tests

    func testMultipleLightsOnStage() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        // Create multiple light types on the same stage
        let sunPath = swiftusd.Path("/World/Sun")
        guard let sun = swiftusd.LuxDistantLightRef.define(on: stage, at: sunPath) else {
            XCTFail("Failed to create DistantLight")
            return
        }
        defer { sun.release() }
        sun.angle = 0.53

        let skyPath = swiftusd.Path("/World/Sky")
        guard let sky = swiftusd.LuxDomeLightRef.define(on: stage, at: skyPath) else {
            XCTFail("Failed to create DomeLight")
            return
        }
        defer { sky.release() }
        sky.guideRadius = 100000.0

        let keyPath = swiftusd.Path("/World/KeyLight")
        guard let key = swiftusd.LuxRectLightRef.define(on: stage, at: keyPath) else {
            XCTFail("Failed to create RectLight")
            return
        }
        defer { key.release() }
        key.width = 2.0
        key.height = 1.0

        let fillPath = swiftusd.Path("/World/FillLight")
        guard let fill = swiftusd.LuxSphereLightRef.define(on: stage, at: fillPath) else {
            XCTFail("Failed to create SphereLight")
            return
        }
        defer { fill.release() }
        fill.radius = 0.5

        // Verify all are valid
        XCTAssertTrue(sun.IsValid())
        XCTAssertTrue(sky.IsValid())
        XCTAssertTrue(key.IsValid())
        XCTAssertTrue(fill.IsValid())
    }
}
