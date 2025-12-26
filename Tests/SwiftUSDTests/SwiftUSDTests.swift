import XCTest
@testable import SwiftUSD
import CxxFacade

final class SwiftUSDTests: XCTestCase {

    func testVersion() {
        XCTAssertEqual(SwiftUSD.version, "2.0.0")
    }

    func testCxxFacadeImport() {
        // Verify CxxFacade module can be imported and used
        let version = String(cString: cxxfacade.GetVersion())
        XCTAssertEqual(version, "2.0.0")

        let buildInfo = String(cString: cxxfacade.GetBuildInfo())
        XCTAssertTrue(buildInfo.contains("CxxFacade"), "Build info should contain CxxFacade")
    }

    // MARK: - TfToken Tests

    func testTfTokenCreation() {
        let token = cxxfacade.tf.TfToken("hello")
        XCTAssertFalse(token.IsEmpty())
        XCTAssertEqual(token.GetString(), std.string("hello"))
    }

    func testTfTokenEmpty() {
        let token = cxxfacade.tf.TfToken()
        XCTAssertTrue(token.IsEmpty())
    }

    func testTfTokenEquality() {
        let token1 = cxxfacade.tf.TfToken("test")
        let token2 = cxxfacade.tf.TfToken("test")
        let token3 = cxxfacade.tf.TfToken("other")
        XCTAssertTrue(token1 == token2)
        XCTAssertTrue(token1 != token3)
    }

    // MARK: - Gf Math Tests

    func testVec3dCreation() {
        let vec = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        XCTAssertEqual(vec.x, 1.0)
        XCTAssertEqual(vec.y, 2.0)
        XCTAssertEqual(vec.z, 3.0)
    }

    func testVec3dOperations() {
        let v1 = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        let v2 = cxxfacade.gf.Vec3d(4.0, 5.0, 6.0)

        let sum = v1 + v2
        XCTAssertEqual(sum.x, 5.0)
        XCTAssertEqual(sum.y, 7.0)
        XCTAssertEqual(sum.z, 9.0)

        let dot = v1.Dot(v2)
        XCTAssertEqual(dot, 32.0)
    }

    func testVec3dNormalize() {
        let vec = cxxfacade.gf.Vec3d(3.0, 0.0, 0.0)
        let normalized = vec.GetNormalized()
        XCTAssertEqual(normalized.x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(normalized.y, 0.0, accuracy: 1e-10)
        XCTAssertEqual(normalized.z, 0.0, accuracy: 1e-10)
    }

    func testMatrix4dIdentity() {
        let identity = cxxfacade.gf.Matrix4d.Identity()
        XCTAssertEqual(identity.data.0.0, 1.0)
        XCTAssertEqual(identity.data.1.1, 1.0)
        XCTAssertEqual(identity.data.2.2, 1.0)
        XCTAssertEqual(identity.data.3.3, 1.0)
        XCTAssertEqual(identity.data.0.1, 0.0)
    }

    func testQuatdIdentity() {
        let identity = cxxfacade.gf.Quatd.Identity()
        XCTAssertEqual(identity.w, 1.0)
        XCTAssertEqual(identity.x, 0.0)
        XCTAssertEqual(identity.y, 0.0)
        XCTAssertEqual(identity.z, 0.0)
    }

    // MARK: - Vt Array Tests

    func testVtIntArray() {
        var arr = cxxfacade.vt.VtIntArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(10)
        arr.push_back(20)
        arr.push_back(30)

        XCTAssertEqual(arr.size(), 3)
        XCTAssertFalse(arr.empty())
    }

    func testVtFloatArray() {
        let arr = cxxfacade.vt.VtFloatArray(5)
        XCTAssertEqual(arr.size(), 5)
    }

    func testVtValue() {
        let intValue = cxxfacade.vt.VtValue(Int32(42))
        XCTAssertFalse(intValue.IsEmpty())
        XCTAssertEqual(intValue.GetInt(), 42)

        let floatValue = cxxfacade.vt.VtValue(Float(3.14))
        XCTAssertFalse(floatValue.IsEmpty())
        XCTAssertEqual(floatValue.GetFloat(), 3.14, accuracy: 0.001)
    }

    // MARK: - Sdf Tests

    func testSdfPathCreation() {
        let path = cxxfacade.sdf.SdfPath("/World/Cube")
        XCTAssertFalse(path.IsEmpty())
        XCTAssertTrue(path.IsAbsolutePath())
        XCTAssertTrue(path.IsPrimPath())
        XCTAssertEqual(path.GetString(), std.string("/World/Cube"))
    }

    func testSdfPathOperations() {
        let root = cxxfacade.sdf.SdfPath("/World")
        let childName = cxxfacade.tf.TfToken("Child")
        let childPath = root.AppendChild(childName)
        XCTAssertEqual(childPath.GetString(), std.string("/World/Child"))

        let parent = childPath.GetParentPath()
        XCTAssertEqual(parent.GetString(), std.string("/World"))
    }

    func testSdfAssetPath() {
        let assetPath = cxxfacade.sdf.SdfAssetPath("./textures/wood.png")
        XCTAssertEqual(assetPath.GetAssetPath(), std.string("./textures/wood.png"))
    }
}
