// SdfCxxTests.swift - Tests for Sdf C++ interop types

import XCTest
import USDCxx
@testable import SwiftUSD

final class SdfCxxTests: XCTestCase {

    // MARK: - Token Tests

    func testTokenCreation() {
        let token = swiftusd.Token("testToken")
        XCTAssertFalse(token.isEmpty)
        XCTAssertEqual(token.text, "testToken")
    }

    func testTokenEmpty() {
        let empty = swiftusd.Token.Empty()
        XCTAssertTrue(empty.isEmpty)
        XCTAssertEqual(empty.text, "")
    }

    func testTokenEquality() {
        let token1 = swiftusd.Token("hello")
        let token2 = swiftusd.Token("hello")
        let token3 = swiftusd.Token("world")

        XCTAssertEqual(token1, token2)
        XCTAssertNotEqual(token1, token3)
    }

    func testTokenStringLiteral() {
        let token: swiftusd.Token = "myToken"
        XCTAssertEqual(token.text, "myToken")
    }

    func testTokenHashing() {
        let token1 = swiftusd.Token("test")
        let token2 = swiftusd.Token("test")

        var set = Set<swiftusd.Token>()
        set.insert(token1)
        XCTAssertTrue(set.contains(token2))
    }

    // MARK: - Path Tests

    func testPathCreation() {
        let path = swiftusd.Path("/World/Mesh")
        XCTAssertFalse(path.isEmpty)
        XCTAssertEqual(path.text, "/World/Mesh")
    }

    func testPathEmpty() {
        let empty = swiftusd.Path.Empty()
        XCTAssertTrue(empty.isEmpty)
    }

    func testAbsoluteRootPath() {
        let root = swiftusd.Path.AbsoluteRoot()
        XCTAssertTrue(root.IsAbsoluteRootPath())
        XCTAssertEqual(root.text, "/")
    }

    func testPathIsAbsolute() {
        let absolute = swiftusd.Path("/World")
        let relative = swiftusd.Path("World")

        XCTAssertTrue(absolute.isAbsolute)
        XCTAssertFalse(relative.isAbsolute)
    }

    func testPathIsPrim() {
        let primPath = swiftusd.Path("/World/Mesh")
        let propPath = swiftusd.Path("/World/Mesh.points")

        XCTAssertTrue(primPath.isPrim)
        XCTAssertFalse(propPath.isPrim)
    }

    func testPathIsProperty() {
        let primPath = swiftusd.Path("/World/Mesh")
        let propPath = swiftusd.Path("/World/Mesh.points")

        XCTAssertFalse(primPath.isProperty)
        XCTAssertTrue(propPath.isProperty)
    }

    func testPathName() {
        let path = swiftusd.Path("/World/Mesh")
        XCTAssertEqual(path.name, "Mesh")

        let propPath = swiftusd.Path("/World/Mesh.points")
        XCTAssertEqual(propPath.name, "points")
    }

    func testPathParent() {
        let path = swiftusd.Path("/World/Mesh")
        let parent = path.parent
        XCTAssertEqual(parent.text, "/World")

        let root = swiftusd.Path("/World")
        let rootParent = root.parent
        XCTAssertEqual(rootParent.text, "/")
    }

    func testPathAppendChild() {
        let path = swiftusd.Path("/World")
        let child = path.appending(child: "Mesh")
        XCTAssertEqual(child.text, "/World/Mesh")
    }

    func testPathAppendProperty() {
        let path = swiftusd.Path("/World/Mesh")
        let prop = path.appending(property: "points")
        XCTAssertEqual(prop.text, "/World/Mesh.points")
    }

    func testPathPrimPath() {
        let propPath = swiftusd.Path("/World/Mesh.points")
        let primPath = propPath.primPath
        XCTAssertEqual(primPath.text, "/World/Mesh")
    }

    func testPathEquality() {
        let path1 = swiftusd.Path("/World/Mesh")
        let path2 = swiftusd.Path("/World/Mesh")
        let path3 = swiftusd.Path("/World/Other")

        XCTAssertEqual(path1, path2)
        XCTAssertNotEqual(path1, path3)
    }

    func testPathStringLiteral() {
        let path: swiftusd.Path = "/World/Mesh"
        XCTAssertEqual(path.text, "/World/Mesh")
    }

    func testPathHashing() {
        let path1 = swiftusd.Path("/World/Mesh")
        let path2 = swiftusd.Path("/World/Mesh")

        var set = Set<swiftusd.Path>()
        set.insert(path1)
        XCTAssertTrue(set.contains(path2))
    }

    func testPathHasPrefix() {
        let path = swiftusd.Path("/World/Mesh/Child")
        let prefix = swiftusd.Path("/World/Mesh")

        XCTAssertTrue(path.hasPrefix(prefix))
        XCTAssertFalse(prefix.hasPrefix(path))
    }

    func testPathRootPrim() {
        let rootPrim = swiftusd.Path("/World")
        let nested = swiftusd.Path("/World/Mesh")

        XCTAssertTrue(rootPrim.IsRootPrimPath())
        XCTAssertFalse(nested.IsRootPrimPath())
    }

    func testPathValidation() {
        XCTAssertTrue(swiftusd.Path.IsValidPathString("/World"))
        XCTAssertTrue(swiftusd.Path.IsValidPathString("/World/Mesh"))
        XCTAssertTrue(swiftusd.Path.IsValidPathString(""))  // Empty is valid
    }

    // MARK: - AssetPath Tests

    func testAssetPathCreation() {
        let assetPath = swiftusd.AssetPath("./models/cube.usd")
        XCTAssertFalse(assetPath.isEmpty)
        XCTAssertEqual(assetPath.path, "./models/cube.usd")
    }

    func testAssetPathEmpty() {
        let empty = swiftusd.AssetPath()
        XCTAssertTrue(empty.isEmpty)
    }

    func testAssetPathWithResolved() {
        let assetPath = swiftusd.AssetPath("./models/cube.usd", "/absolute/path/models/cube.usd")
        XCTAssertEqual(assetPath.path, "./models/cube.usd")
        XCTAssertEqual(assetPath.resolvedPath, "/absolute/path/models/cube.usd")
    }

    func testAssetPathEquality() {
        let path1 = swiftusd.AssetPath("./cube.usd")
        let path2 = swiftusd.AssetPath("./cube.usd")
        let path3 = swiftusd.AssetPath("./sphere.usd")

        XCTAssertEqual(path1, path2)
        XCTAssertNotEqual(path1, path3)
    }

    func testAssetPathStringLiteral() {
        let assetPath: swiftusd.AssetPath = "./models/cube.usd"
        XCTAssertEqual(assetPath.path, "./models/cube.usd")
    }

    func testAssetPathHashing() {
        let path1 = swiftusd.AssetPath("./cube.usd")
        let path2 = swiftusd.AssetPath("./cube.usd")

        var set = Set<swiftusd.AssetPath>()
        set.insert(path1)
        XCTAssertTrue(set.contains(path2))
    }

    // MARK: - LayerRef Tests

    func testLayerRefCreateAnonymous() {
        guard let layer = swiftusd.LayerRef.createAnonymous(tag: "test") else {
            XCTFail("Failed to create anonymous layer")
            return
        }
        XCTAssertTrue(layer.isAnonymous)
        XCTAssertFalse(layer.isDirty)
    }

    func testLayerRefIdentifier() {
        guard let layer = swiftusd.LayerRef.createAnonymous(tag: "myLayer") else {
            XCTFail("Failed to create layer")
            return
        }
        XCTAssertFalse(layer.identifier.isEmpty)
    }

    func testLayerRefTimeMetadata() {
        guard let layer = swiftusd.LayerRef.createAnonymous() else {
            XCTFail("Failed to create layer")
            return
        }

        layer.startTimeCode = 1.0
        layer.endTimeCode = 100.0
        layer.timeCodesPerSecond = 24.0
        layer.framesPerSecond = 24.0

        XCTAssertEqual(layer.startTimeCode, 1.0)
        XCTAssertEqual(layer.endTimeCode, 100.0)
        XCTAssertEqual(layer.timeCodesPerSecond, 24.0)
        XCTAssertEqual(layer.framesPerSecond, 24.0)
    }

    func testLayerRefDefaultPrim() {
        guard let layer = swiftusd.LayerRef.createAnonymous() else {
            XCTFail("Failed to create layer")
            return
        }

        let defaultPrim = swiftusd.Token("MyPrim")
        layer.defaultPrim = defaultPrim

        XCTAssertEqual(layer.defaultPrim.text, "MyPrim")
    }

    func testLayerRefClear() {
        guard let layer = swiftusd.LayerRef.createAnonymous() else {
            XCTFail("Failed to create layer")
            return
        }

        layer.clear()
        XCTAssertTrue(layer.isEmpty)
    }

    func testLayerRefSubLayers() {
        guard let layer = swiftusd.LayerRef.createAnonymous() else {
            XCTFail("Failed to create layer")
            return
        }

        XCTAssertEqual(layer.subLayerCount, 0)

        layer.insertSubLayer("./sub1.usda")
        XCTAssertEqual(layer.subLayerCount, 1)

        layer.insertSubLayer("./sub2.usda")
        XCTAssertEqual(layer.subLayerCount, 2)

        layer.removeSubLayer(at: 0)
        XCTAssertEqual(layer.subLayerCount, 1)
    }

    func testLayerRefHasSpec() {
        guard let layer = swiftusd.LayerRef.createAnonymous() else {
            XCTFail("Failed to create layer")
            return
        }

        let path = swiftusd.Path("/NonExistent")
        XCTAssertFalse(layer.hasSpec(at: path))
    }

    // MARK: - Integration Tests

    func testPathWithToken() {
        let token = swiftusd.Token("Child")
        let path = swiftusd.Path("/World")
        let childPath = path.AppendChild(token)

        XCTAssertEqual(childPath.text, "/World/Child")
    }

    func testLayerWithPath() {
        guard let layer = swiftusd.LayerRef.createAnonymous() else {
            XCTFail("Failed to create layer")
            return
        }

        let path = swiftusd.Path("/TestPrim")
        // In standalone mode without prims, HasSpec should return false
        XCTAssertFalse(layer.hasSpec(at: path))
    }
}
