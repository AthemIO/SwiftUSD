// HdCxxTests.swift - Tests for Hydra Core Framework Swift bindings

import XCTest
import USDCxx
@testable import SwiftUSD

final class HdCxxTests: XCTestCase {

    // MARK: - HdDriver Tests

    func testDriverCreation() {
        // Test creating a driver with a name
        let driverName = swiftusd.Token("testDriver")
        guard let driver = swiftusd.HdDriverRef.Create(driverName) else {
            XCTFail("Failed to create HdDriverRef")
            return
        }

        XCTAssertEqual(driver.GetName().GetText(), "testDriver")
    }

    func testDriverWithConvenienceMethod() {
        // Test Swift convenience method
        guard let driver = swiftusd.HdDriverRef.create(name: "myDriver") else {
            XCTFail("Failed to create driver with convenience method")
            return
        }

        XCTAssertEqual(String(cString: driver.GetName().GetText()), "myDriver")
    }

    func testDriverHasDevice() {
        // Driver without Hgi should not have a device
        guard let driver = swiftusd.HdDriverRef.Create(swiftusd.Token("noDevice")) else {
            XCTFail("Failed to create driver")
            return
        }

        // In standalone mode, HasDevice should return false without Hgi
        let hasDevice = driver.HasDevice()
        // Note: Behavior depends on whether Hgi is attached
        XCTAssertFalse(hasDevice)
    }

    // MARK: - HdEngine Tests

    func testEngineCreation() {
        guard let engine = swiftusd.HdEngineRef.Create() else {
            XCTFail("Failed to create HdEngineRef")
            return
        }

        // Engine should be created successfully
        XCTAssertNotNil(engine)
    }

    func testEngineTaskContext() {
        guard let engine = swiftusd.HdEngineRef.create() else {
            XCTFail("Failed to create engine")
            return
        }

        // Test setting and clearing task context data
        engine.setTaskContextData(id: "testKey", value: "testValue")

        // Clear all context data
        engine.ClearTaskContextData()

        // After clear, we can set new data
        engine.setTaskContextData(id: "newKey", value: "newValue")
    }

    // MARK: - HdDisplayStyle Tests

    func testDisplayStyleDefault() {
        let style = swiftusd.HdDisplayStyle()

        XCTAssertEqual(style.refineLevel, 0)
        XCTAssertFalse(style.flatShadingEnabled)
        XCTAssertTrue(style.displacementEnabled)
        XCTAssertFalse(style.displayInOverlay)
        XCTAssertFalse(style.occludedSelectionShowsThrough)
        XCTAssertFalse(style.pointsShadingEnabled)
        XCTAssertFalse(style.materialIsFinal)
    }

    func testDisplayStyleWithRefineLevel() {
        let style = swiftusd.HdDisplayStyle.make(refineLevel: 3, flatShading: true, displacement: false)

        XCTAssertEqual(style.refineLevel, 3)
        XCTAssertTrue(style.flatShadingEnabled)
        XCTAssertFalse(style.displacementEnabled)
    }

    func testDisplayStyleRefineLevelClamping() {
        // Test that refine level is clamped to [0, 8]
        let styleLow = swiftusd.HdDisplayStyle(-5, false, true)
        XCTAssertEqual(styleLow.refineLevel, 0)

        let styleHigh = swiftusd.HdDisplayStyle(15, false, true)
        XCTAssertEqual(styleHigh.refineLevel, 8)
    }

    func testDisplayStyleEquality() {
        let style1 = swiftusd.HdDisplayStyle(2, true, false)
        let style2 = swiftusd.HdDisplayStyle(2, true, false)
        let style3 = swiftusd.HdDisplayStyle(3, true, false)

        XCTAssertEqual(style1, style2)
        XCTAssertNotEqual(style1, style3)
    }

    // MARK: - HdReprSelector Tests

    func testReprSelectorEmpty() {
        let selector = swiftusd.HdReprSelector()

        XCTAssertTrue(selector.IsEmpty())
    }

    func testReprSelectorWithRefined() {
        let refined = swiftusd.Token("refined")
        let selector = swiftusd.HdReprSelector(refined)

        XCTAssertFalse(selector.IsEmpty())
        XCTAssertTrue(selector.Contains(refined))
        XCTAssertFalse(selector.Contains(swiftusd.Token("other")))
    }

    func testReprSelectorWithMultiple() {
        let refined = swiftusd.Token("refined")
        let hull = swiftusd.Token("hull")
        let points = swiftusd.Token("points")

        let selector = swiftusd.HdReprSelector.make(refined: refined, unrefined: hull, points: points)

        XCTAssertFalse(selector.isEmpty)
        XCTAssertTrue(selector.contains(refined))
        XCTAssertTrue(selector.contains(hull))
        XCTAssertTrue(selector.contains(points))
    }

    // MARK: - HdPrimvarDescriptor Tests

    func testPrimvarDescriptorDefault() {
        let desc = swiftusd.HdPrimvarDescriptor()

        XCTAssertTrue(desc.name.IsEmpty())
        XCTAssertEqual(desc.interpolation, .Constant)
        XCTAssertFalse(desc.indexed)
    }

    func testPrimvarDescriptorWithValues() {
        let name = swiftusd.Token("points")
        let role = swiftusd.Token("point")
        let desc = swiftusd.HdPrimvarDescriptor(name, .Vertex, role)

        XCTAssertEqual(String(cString: desc.name.GetText()), "points")
        XCTAssertEqual(desc.interpolation, .Vertex)
        XCTAssertEqual(String(cString: desc.role.GetText()), "point")
        XCTAssertFalse(desc.indexed)
    }

    func testPrimvarDescriptorIndexed() {
        let name = swiftusd.Token("colors")
        let role = swiftusd.Token("color")
        let desc = swiftusd.HdPrimvarDescriptor(name, .FaceVarying, role, true)

        XCTAssertTrue(desc.indexed)
    }

    func testPrimvarDescriptorEquality() {
        let desc1 = swiftusd.HdPrimvarDescriptor(swiftusd.Token("normals"), .Vertex, swiftusd.Token("normal"))
        let desc2 = swiftusd.HdPrimvarDescriptor(swiftusd.Token("normals"), .Vertex, swiftusd.Token("normal"))
        let desc3 = swiftusd.HdPrimvarDescriptor(swiftusd.Token("colors"), .Vertex, swiftusd.Token("color"))

        XCTAssertEqual(desc1, desc2)
        XCTAssertNotEqual(desc1, desc3)
    }

    // MARK: - HdInterpolation Tests

    func testInterpolationValues() {
        XCTAssertEqual(swiftusd.HdInterpolation.Constant.rawValue, 0)
        XCTAssertEqual(swiftusd.HdInterpolation.Uniform.rawValue, 1)
        XCTAssertEqual(swiftusd.HdInterpolation.Varying.rawValue, 2)
        XCTAssertEqual(swiftusd.HdInterpolation.Vertex.rawValue, 3)
        XCTAssertEqual(swiftusd.HdInterpolation.FaceVarying.rawValue, 4)
        XCTAssertEqual(swiftusd.HdInterpolation.Instance.rawValue, 5)
    }

    func testInterpolationNames() {
        XCTAssertEqual(swiftusd.HdInterpolation.Constant.name, "constant")
        XCTAssertEqual(swiftusd.HdInterpolation.Uniform.name, "uniform")
        XCTAssertEqual(swiftusd.HdInterpolation.Varying.name, "varying")
        XCTAssertEqual(swiftusd.HdInterpolation.Vertex.name, "vertex")
        XCTAssertEqual(swiftusd.HdInterpolation.FaceVarying.name, "faceVarying")
        XCTAssertEqual(swiftusd.HdInterpolation.Instance.name, "instance")
    }

    // MARK: - HdCullStyle Tests

    func testCullStyleValues() {
        XCTAssertEqual(swiftusd.HdCullStyle.DontCare.rawValue, 0)
        XCTAssertEqual(swiftusd.HdCullStyle.Nothing.rawValue, 1)
        XCTAssertEqual(swiftusd.HdCullStyle.Back.rawValue, 2)
        XCTAssertEqual(swiftusd.HdCullStyle.Front.rawValue, 3)
    }

    func testCullStyleNames() {
        XCTAssertEqual(swiftusd.HdCullStyle.DontCare.name, "dontCare")
        XCTAssertEqual(swiftusd.HdCullStyle.Nothing.name, "nothing")
        XCTAssertEqual(swiftusd.HdCullStyle.Back.name, "back")
        XCTAssertEqual(swiftusd.HdCullStyle.Front.name, "front")
    }

    // MARK: - HdTokens Tests

    func testHdTokensRepr() {
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Refined().GetText()), "refined")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Unrefined().GetText()), "hull")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Points().GetText()), "points")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Wire().GetText()), "wire")
    }

    func testHdTokensPrimTypes() {
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Mesh().GetText()), "mesh")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.BasisCurves().GetText()), "basisCurves")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Volume().GetText()), "volume")
    }

    func testHdTokensSprimTypes() {
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Camera().GetText()), "camera")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Light().GetText()), "light")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.Material().GetText()), "material")
    }

    func testHdTokensDrivers() {
        XCTAssertEqual(String(cString: swiftusd.HdTokens.RenderDriver().GetText()), "renderDriver")
        XCTAssertEqual(String(cString: swiftusd.HdTokens.HgiDriver().GetText()), "hgi")
    }

    // MARK: - Token Extension Tests

    func testTokenExtensions() {
        XCTAssertEqual(String(cString: swiftusd.Token.refined.GetText()), "refined")
        XCTAssertEqual(String(cString: swiftusd.Token.mesh.GetText()), "mesh")
        XCTAssertEqual(String(cString: swiftusd.Token.camera.GetText()), "camera")
        XCTAssertEqual(String(cString: swiftusd.Token.renderDriver.GetText()), "renderDriver")
    }

    // MARK: - Dirty Bits Tests

    func testDirtyBitsConstants() {
        XCTAssertEqual(swiftusd.HdDirtyBitsClean, 0)
        XCTAssertEqual(swiftusd.HdDirtyBitsAllDirty, 0xffffffff)

        // Test individual bits
        XCTAssertEqual(swiftusd.HdDirtyBitsDirtyPrimId, 1 << 0)
        XCTAssertEqual(swiftusd.HdDirtyBitsDirtyExtent, 1 << 1)
        XCTAssertEqual(swiftusd.HdDirtyBitsDirtyDisplayStyle, 1 << 2)
        XCTAssertEqual(swiftusd.HdDirtyBitsDirtyPoints, 1 << 3)
        XCTAssertEqual(swiftusd.HdDirtyBitsDirtyTransform, 1 << 7)
        XCTAssertEqual(swiftusd.HdDirtyBitsDirtyVisibility, 1 << 8)
    }

    // MARK: - HdChangeTracker Tests

    func testChangeTrackerCreation() {
        // ChangeTracker is typically obtained from a RenderIndex
        // In standalone mode, we can create one directly for testing
        let tracker = swiftusd.HdChangeTrackerRef()

        XCTAssertEqual(tracker.GetVisibilityChangeCount(), 0)
        XCTAssertEqual(tracker.GetRenderTagVersion(), 0)
    }

    // MARK: - HdRenderSettingDescriptor Tests

    func testRenderSettingDescriptor() {
        let key = swiftusd.Token("quality")
        let desc = swiftusd.HdRenderSettingDescriptor("Render Quality", key, "high")

        XCTAssertEqual(desc.name, "Render Quality")
        XCTAssertEqual(String(cString: desc.key.GetText()), "quality")
        XCTAssertEqual(desc.defaultValueStr, "high")
    }

    func testRenderSettingDescriptorDefault() {
        let desc = swiftusd.HdRenderSettingDescriptor()

        XCTAssertTrue(desc.name.isEmpty)
        XCTAssertTrue(desc.key.IsEmpty())
        XCTAssertTrue(desc.defaultValueStr.isEmpty)
    }
}
