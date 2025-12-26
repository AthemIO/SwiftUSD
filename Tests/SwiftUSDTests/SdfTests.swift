// SdfTests.swift - Tests for Sdf module (Scene Description Foundation)

import XCTest
import SwiftUSD
@testable import CxxFacade

final class SdfTests: XCTestCase {

    // MARK: - SdfPath Creation Tests

    func testPathCreationFromString() {
        let path = cxxfacade.sdf.SdfPath("/World/Mesh")
        XCTAssertFalse(path.IsEmpty())
        XCTAssertEqual(String(path.GetString()), "/World/Mesh")
    }

    func testPathCreationFromStdString() {
        let stdStr = std.string("/World/Cube")
        let path = cxxfacade.sdf.SdfPath(stdStr)
        XCTAssertFalse(path.IsEmpty())
        XCTAssertEqual(path.GetString(), stdStr)
    }

    func testEmptyPathCreation() {
        let path = cxxfacade.sdf.SdfPath()
        XCTAssertTrue(path.IsEmpty())
    }

    func testAbsoluteRootPath() {
        let root = cxxfacade.sdf.SdfPath.AbsoluteRootPath()
        XCTAssertFalse(root.IsEmpty())
        XCTAssertTrue(root.IsAbsolutePath())
        XCTAssertEqual(String(root.GetString()), "/")
    }

    // MARK: - SdfPath Properties Tests

    func testPathIsAbsolute() {
        let absolute = cxxfacade.sdf.SdfPath("/World")
        XCTAssertTrue(absolute.IsAbsolutePath())
    }

    func testPathIsPrimPath() {
        let primPath = cxxfacade.sdf.SdfPath("/World/Mesh")
        XCTAssertTrue(primPath.IsPrimPath())
        XCTAssertFalse(primPath.IsPropertyPath())
    }

    func testPathIsPropertyPath() {
        let primPath = cxxfacade.sdf.SdfPath("/World/Mesh")
        let propToken = cxxfacade.tf.TfToken("visibility")
        let propertyPath = primPath.AppendProperty(propToken)
        XCTAssertTrue(propertyPath.IsPropertyPath())
        XCTAssertFalse(propertyPath.IsPrimPath())
    }

    // MARK: - SdfPath Hierarchy Tests

    func testPathAppendChild() {
        let root = cxxfacade.sdf.SdfPath("/")
        let childToken = cxxfacade.tf.TfToken("World")
        let childPath = root.AppendChild(childToken)
        XCTAssertEqual(String(childPath.GetString()), "/World")
    }

    func testPathGetParent() {
        let childPath = cxxfacade.sdf.SdfPath("/World/Mesh")
        let parentPath = childPath.GetParentPath()
        XCTAssertEqual(String(parentPath.GetString()), "/World")
    }

    func testPathGetName() {
        let path = cxxfacade.sdf.SdfPath("/World/Mesh")
        let name = path.GetName()
        XCTAssertEqual(String(cString: name.GetText()), "Mesh")
    }

    // MARK: - SdfPath Equality Tests

    func testPathEquality() {
        let path1 = cxxfacade.sdf.SdfPath("/World/Mesh")
        let path2 = cxxfacade.sdf.SdfPath("/World/Mesh")
        let path3 = cxxfacade.sdf.SdfPath("/World/Cube")

        XCTAssertTrue(path1 == path2)
        XCTAssertFalse(path1 == path3)
        XCTAssertTrue(path1 != path3)
    }

    func testEmptyPathEquality() {
        let empty1 = cxxfacade.sdf.SdfPath()
        let empty2 = cxxfacade.sdf.SdfPath.EmptyPath()
        let nonEmpty = cxxfacade.sdf.SdfPath("/World")

        XCTAssertTrue(empty1 == empty2)
        XCTAssertFalse(empty1 == nonEmpty)
    }

    // MARK: - SdfPath Comparison Tests

    func testPathLessThan() {
        var path1 = cxxfacade.sdf.SdfPath("/A")
        var path2 = cxxfacade.sdf.SdfPath("/B")

        XCTAssertTrue(path1 < path2)
        XCTAssertFalse(path2 < path1)
    }

    // MARK: - SdfAssetPath Tests

    func testAssetPathCreation() {
        let assetPath = cxxfacade.sdf.SdfAssetPath("textures/diffuse.png")
        XCTAssertEqual(String(assetPath.GetAssetPath()), "textures/diffuse.png")
    }

    func testAssetPathWithResolvedPath() {
        let assetPath = cxxfacade.sdf.SdfAssetPath(
            std.string("textures/diffuse.png"),
            std.string("/absolute/path/textures/diffuse.png")
        )
        XCTAssertEqual(String(assetPath.GetAssetPath()), "textures/diffuse.png")
        XCTAssertEqual(String(assetPath.GetResolvedPath()), "/absolute/path/textures/diffuse.png")
    }

    func testAssetPathEquality() {
        let path1 = cxxfacade.sdf.SdfAssetPath("textures/a.png")
        let path2 = cxxfacade.sdf.SdfAssetPath("textures/a.png")
        let path3 = cxxfacade.sdf.SdfAssetPath("textures/b.png")

        XCTAssertTrue(path1 == path2)
        XCTAssertFalse(path1 == path3)
    }

    // MARK: - SdfLayerOffset Tests

    func testLayerOffsetDefaultCreation() {
        let offset = cxxfacade.sdf.SdfLayerOffset()
        XCTAssertTrue(offset.IsIdentity())
        XCTAssertTrue(offset.IsValid())
        XCTAssertEqual(offset.GetOffset(), 0.0)
        XCTAssertEqual(offset.GetScale(), 1.0)
    }

    func testLayerOffsetCustomCreation() {
        let offset = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        XCTAssertFalse(offset.IsIdentity())
        XCTAssertTrue(offset.IsValid())
        XCTAssertEqual(offset.GetOffset(), 24.0)
        XCTAssertEqual(offset.GetScale(), 2.0)
    }

    func testLayerOffsetSetters() {
        var offset = cxxfacade.sdf.SdfLayerOffset()
        offset.SetOffset(10.0)
        offset.SetScale(0.5)

        XCTAssertEqual(offset.GetOffset(), 10.0)
        XCTAssertEqual(offset.GetScale(), 0.5)
        XCTAssertFalse(offset.IsIdentity())
    }

    func testLayerOffsetApplyToTime() {
        // offset = 24, scale = 2
        // result = offset + scale * time = 24 + 2 * 10 = 44
        let offset = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let result = offset * 10.0
        XCTAssertEqual(result, 44.0, accuracy: 0.0001)
    }

    func testLayerOffsetComposition() {
        // First offset: offset=10, scale=2
        // Second offset: offset=5, scale=3
        // Composed: first apply second, then first
        // For time t: second(t) = 5 + 3*t, then first(second(t)) = 10 + 2*(5 + 3*t) = 10 + 10 + 6*t = 20 + 6*t
        let first = cxxfacade.sdf.SdfLayerOffset(10.0, 2.0)
        let second = cxxfacade.sdf.SdfLayerOffset(5.0, 3.0)
        let composed = first * second

        // composed.GetOffset() should be 10 + 2*5 = 20
        // composed.GetScale() should be 2 * 3 = 6
        XCTAssertEqual(composed.GetOffset(), 20.0, accuracy: 0.0001)
        XCTAssertEqual(composed.GetScale(), 6.0, accuracy: 0.0001)

        // Verify by applying to a time value
        let time = 10.0
        let directResult = composed * time
        let manualResult = 20.0 + 6.0 * time
        XCTAssertEqual(directResult, manualResult, accuracy: 0.0001)
    }

    func testLayerOffsetInverse() {
        let offset = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let inverse = offset.GetInverse()

        // Applying offset then inverse should give identity
        let composed = inverse * offset
        XCTAssertTrue(composed.IsIdentity())
    }

    func testLayerOffsetEquality() {
        let offset1 = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let offset2 = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let offset3 = cxxfacade.sdf.SdfLayerOffset(10.0, 2.0)

        XCTAssertTrue(offset1 == offset2)
        XCTAssertFalse(offset1 == offset3)
        XCTAssertTrue(offset1 != offset3)
    }

    func testLayerOffsetComparison() {
        var offset1 = cxxfacade.sdf.SdfLayerOffset(10.0, 1.0)
        var offset2 = cxxfacade.sdf.SdfLayerOffset(20.0, 1.0)

        // Less than comparison is somewhat arbitrary but consistent
        XCTAssertTrue(offset1 < offset2 || offset2 < offset1 || offset1 == offset2)
    }

    func testLayerOffsetHash() {
        let offset1 = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let offset2 = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let offset3 = cxxfacade.sdf.SdfLayerOffset(10.0, 3.0)

        XCTAssertEqual(offset1.GetHash(), offset2.GetHash())
        XCTAssertNotEqual(offset1.GetHash(), offset3.GetHash())
    }

    func testLayerOffsetCopy() {
        let original = cxxfacade.sdf.SdfLayerOffset(24.0, 2.0)
        let copy = original

        XCTAssertEqual(original.GetOffset(), copy.GetOffset())
        XCTAssertEqual(original.GetScale(), copy.GetScale())
        XCTAssertTrue(original == copy)
    }

    // MARK: - SdfLayer Tests

    func testLayerCreateAnonymous() {
        let layer = cxxfacade.sdf.SdfLayer.CreateAnonymous()
        XCTAssertTrue(layer.IsValid())
        XCTAssertTrue(layer.IsAnonymous())
    }

    func testLayerInvalidAfterDefault() {
        let layer = cxxfacade.sdf.SdfLayer()
        XCTAssertFalse(layer.IsValid())
    }

    func testLayerClear() {
        var layer = cxxfacade.sdf.SdfLayer.CreateAnonymous()
        layer.Clear()
        XCTAssertTrue(layer.IsValid())  // Still valid after clear
    }

    // MARK: - Swift Wrapper Tests

    func testSdfPathSwiftWrapper() {
        let path: SdfPath = "/World/Mesh"
        XCTAssertFalse(path.isEmpty)
        XCTAssertTrue(path.isAbsolute)
        XCTAssertTrue(path.isPrimPath)
        XCTAssertEqual(path.pathString, "/World/Mesh")
    }

    func testSdfPathAppending() {
        let root = SdfPath.absoluteRoot
        let world = root.appending(child: "World")
        let mesh = world.appending(child: "Mesh")

        XCTAssertEqual(world.pathString, "/World")
        XCTAssertEqual(mesh.pathString, "/World/Mesh")
        XCTAssertEqual(mesh.parent.pathString, "/World")
    }

    func testSdfAssetPathSwiftWrapper() {
        let assetPath: SdfAssetPath = "textures/diffuse.png"
        XCTAssertEqual(assetPath.assetPath, "textures/diffuse.png")
    }

    func testSdfLayerOffsetSwiftWrapper() {
        var offset = SdfLayerOffset(offset: 24.0, scale: 2.0)
        XCTAssertEqual(offset.offset, 24.0)
        XCTAssertEqual(offset.scale, 2.0)
        XCTAssertFalse(offset.isIdentity)
        XCTAssertTrue(offset.isValid)

        // Test apply
        let result = offset.apply(to: 10.0)
        XCTAssertEqual(result, 44.0, accuracy: 0.0001)

        // Test inverse
        let inv = offset.inverse
        XCTAssertNotEqual(inv.offset, offset.offset)

        // Test identity
        let identity = SdfLayerOffset.identity
        XCTAssertTrue(identity.isIdentity)
    }

    func testSdfLayerSwiftWrapper() {
        let layer = SdfLayer.createAnonymous()
        XCTAssertTrue(layer.isValid)
        XCTAssertTrue(layer.isAnonymous)
        XCTAssertFalse(layer.identifier.isEmpty)
    }

    // MARK: - Integration Tests

    func testPathWithToken() {
        let worldToken = TfToken("World")
        let meshToken = TfToken("Mesh")

        let root = SdfPath.absoluteRoot
        let worldPath = root.AppendChild(worldToken)
        let meshPath = worldPath.AppendChild(meshToken)

        XCTAssertEqual(meshPath.pathString, "/World/Mesh")
        XCTAssertEqual(meshPath.name.text, "Mesh")
    }

    func testPathPropertyAppend() {
        let primPath = SdfPath("/World/Mesh")
        let propPath = primPath.appending(property: "visibility")

        XCTAssertTrue(propPath.isPropertyPath)
        XCTAssertFalse(propPath.isPrimPath)
    }

    func testLayerOffsetChaining() {
        // Create a chain of offsets
        let offset1 = SdfLayerOffset(offset: 10.0, scale: 2.0)
        let offset2 = SdfLayerOffset(offset: 5.0, scale: 1.5)

        let composed = offset1 * offset2

        // Apply to time and verify
        let time = 100.0
        let composedResult = composed.apply(to: time)

        // Manual calculation: offset1(offset2(100)) = offset1(5 + 1.5*100) = offset1(155) = 10 + 2*155 = 320
        let manualResult = 10.0 + 2.0 * (5.0 + 1.5 * time)
        XCTAssertEqual(composedResult, manualResult, accuracy: 0.0001)
    }
}
