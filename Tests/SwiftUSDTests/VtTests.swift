// VtTests.swift - Tests for Value Types module

import XCTest
import SwiftUSD
@testable import CxxFacade

final class VtTests: XCTestCase {

    // MARK: - VtIntArray Tests

    func testIntArrayBasics() {
        var arr = cxxfacade.vt.VtIntArray()
        XCTAssertTrue(arr.empty())
        XCTAssertEqual(arr.size(), 0)

        arr.push_back(1)
        arr.push_back(2)
        arr.push_back(3)
        XCTAssertFalse(arr.empty())
        XCTAssertEqual(arr.size(), 3)
        XCTAssertEqual(arr[0], 1)
        XCTAssertEqual(arr[1], 2)
        XCTAssertEqual(arr[2], 3)
    }

    func testIntArraySized() {
        let arr = cxxfacade.vt.VtIntArray(5)
        XCTAssertEqual(arr.size(), 5)
    }

    func testIntArrayResize() {
        var arr = cxxfacade.vt.VtIntArray(5)
        XCTAssertEqual(arr.size(), 5)

        arr.resize(10)
        XCTAssertEqual(arr.size(), 10)

        arr.clear()
        XCTAssertTrue(arr.empty())
    }

    func testIntArrayData() {
        var arr = cxxfacade.vt.VtIntArray()
        arr.push_back(10)
        arr.push_back(20)
        arr.push_back(30)

        let ptr = arr.data()
        XCTAssertNotNil(ptr)
    }

    // MARK: - VtFloatArray Tests

    func testFloatArrayBasics() {
        var arr = cxxfacade.vt.VtFloatArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(1.5)
        arr.push_back(2.5)
        XCTAssertEqual(arr.size(), 2)
        XCTAssertEqual(arr[0], 1.5, accuracy: 0.001)
        XCTAssertEqual(arr[1], 2.5, accuracy: 0.001)
    }

    func testFloatArraySized() {
        let arr = cxxfacade.vt.VtFloatArray(3)
        XCTAssertEqual(arr.size(), 3)
    }

    func testFloatArrayReserve() {
        var arr = cxxfacade.vt.VtFloatArray()
        arr.reserve(100)
        XCTAssertEqual(arr.size(), 0)  // Reserve doesn't change size

        arr.push_back(1.0)
        arr.push_back(2.0)
        XCTAssertEqual(arr.size(), 2)
    }

    // MARK: - VtDoubleArray Tests

    func testDoubleArrayBasics() {
        var arr = cxxfacade.vt.VtDoubleArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(1.5)
        arr.push_back(2.5)
        XCTAssertEqual(arr.size(), 2)
        XCTAssertEqual(arr[0], 1.5, accuracy: 0.001)
        XCTAssertEqual(arr[1], 2.5, accuracy: 0.001)
    }

    func testDoubleArraySized() {
        let arr = cxxfacade.vt.VtDoubleArray(5)
        XCTAssertEqual(arr.size(), 5)
    }

    // MARK: - VtVec2fArray Tests

    func testVec2fArrayBasics() {
        var arr = cxxfacade.vt.VtVec2fArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(cxxfacade.gf.Vec2f(0.0, 0.0))
        arr.push_back(cxxfacade.gf.Vec2f(1.0, 0.0))
        arr.push_back(cxxfacade.gf.Vec2f(0.5, 0.5))

        XCTAssertEqual(arr.size(), 3)
        XCTAssertEqual(arr[0].x, 0.0)
        XCTAssertEqual(arr[1].x, 1.0)
        XCTAssertEqual(arr[2].x, 0.5)
        XCTAssertEqual(arr[2].y, 0.5)
    }

    func testVec2fArrayUVCoordinates() {
        // Typical UV coordinate use case
        var uvs = cxxfacade.vt.VtVec2fArray()
        uvs.push_back(cxxfacade.gf.Vec2f(0.0, 0.0))  // bottom-left
        uvs.push_back(cxxfacade.gf.Vec2f(1.0, 0.0))  // bottom-right
        uvs.push_back(cxxfacade.gf.Vec2f(1.0, 1.0))  // top-right
        uvs.push_back(cxxfacade.gf.Vec2f(0.0, 1.0))  // top-left

        XCTAssertEqual(uvs.size(), 4)
        XCTAssertEqual(uvs[2].x, 1.0)
        XCTAssertEqual(uvs[2].y, 1.0)
    }

    func testVec2fArraySized() {
        let arr = cxxfacade.vt.VtVec2fArray(4)
        XCTAssertEqual(arr.size(), 4)
    }

    // MARK: - VtVec3fArray Tests

    func testVec3fArrayBasics() {
        var arr = cxxfacade.vt.VtVec3fArray()
        arr.push_back(cxxfacade.gf.Vec3f(1.0, 0.0, 0.0))
        arr.push_back(cxxfacade.gf.Vec3f(0.0, 1.0, 0.0))
        arr.push_back(cxxfacade.gf.Vec3f(0.0, 0.0, 1.0))

        XCTAssertEqual(arr.size(), 3)
        XCTAssertEqual(arr[0].x, 1.0)
        XCTAssertEqual(arr[1].y, 1.0)
        XCTAssertEqual(arr[2].z, 1.0)
    }

    func testVec3fArrayMeshPositions() {
        // Typical mesh vertex positions
        var positions = cxxfacade.vt.VtVec3fArray()
        positions.reserve(4)

        positions.push_back(cxxfacade.gf.Vec3f(-0.5, -0.5, 0.0))
        positions.push_back(cxxfacade.gf.Vec3f( 0.5, -0.5, 0.0))
        positions.push_back(cxxfacade.gf.Vec3f( 0.5,  0.5, 0.0))
        positions.push_back(cxxfacade.gf.Vec3f(-0.5,  0.5, 0.0))

        XCTAssertEqual(positions.size(), 4)
        XCTAssertEqual(positions[0].x, -0.5, accuracy: 0.001)
        XCTAssertEqual(positions[2].y, 0.5, accuracy: 0.001)
    }

    func testVec3fArraySized() {
        let arr = cxxfacade.vt.VtVec3fArray(10)
        XCTAssertEqual(arr.size(), 10)
    }

    // MARK: - VtVec4fArray Tests

    func testVec4fArrayBasics() {
        var arr = cxxfacade.vt.VtVec4fArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(cxxfacade.gf.Vec4f(1.0, 0.0, 0.0, 1.0))  // Red
        arr.push_back(cxxfacade.gf.Vec4f(0.0, 1.0, 0.0, 1.0))  // Green
        arr.push_back(cxxfacade.gf.Vec4f(0.0, 0.0, 1.0, 1.0))  // Blue

        XCTAssertEqual(arr.size(), 3)
        XCTAssertEqual(arr[0].x, 1.0)  // Red channel
        XCTAssertEqual(arr[0].w, 1.0)  // Alpha channel
    }

    func testVec4fArrayColors() {
        // RGBA color use case
        var colors = cxxfacade.vt.VtVec4fArray()
        colors.push_back(cxxfacade.gf.Vec4f(1.0, 0.0, 0.0, 1.0))  // Red
        colors.push_back(cxxfacade.gf.Vec4f(0.0, 1.0, 0.0, 0.5))  // Semi-transparent green
        colors.push_back(cxxfacade.gf.Vec4f(0.0, 0.0, 1.0, 0.0))  // Fully transparent blue

        XCTAssertEqual(colors.size(), 3)
        XCTAssertEqual(colors[1].y, 1.0)  // Green channel
        XCTAssertEqual(colors[1].w, 0.5, accuracy: 0.001)  // Alpha
    }

    func testVec4fArraySized() {
        let arr = cxxfacade.vt.VtVec4fArray(8)
        XCTAssertEqual(arr.size(), 8)
    }

    // MARK: - VtMatrix4dArray Tests

    func testMatrix4dArrayBasics() {
        var arr = cxxfacade.vt.VtMatrix4dArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(cxxfacade.gf.Matrix4d.Identity())
        arr.push_back(cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)))

        XCTAssertEqual(arr.size(), 2)
    }

    func testMatrix4dArraySkeletal() {
        // Typical use case: skeletal animation transforms
        let jointCount = 4
        var jointTransforms = cxxfacade.vt.VtMatrix4dArray(jointCount)

        // Initialize to identity
        for i in 0..<jointCount {
            jointTransforms[i] = cxxfacade.gf.Matrix4d.Identity()
        }

        // Set some transforms
        jointTransforms[0] = cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0))
        jointTransforms[1] = cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(0.0, 1.0, 0.0))
        jointTransforms[2] = cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(0.0, 2.0, 0.0))
        jointTransforms[3] = cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(0.0, 3.0, 0.0))

        XCTAssertEqual(jointTransforms.size(), 4)
    }

    func testMatrix4dArraySized() {
        let arr = cxxfacade.vt.VtMatrix4dArray(6)
        XCTAssertEqual(arr.size(), 6)
    }

    // MARK: - VtTokenArray Tests

    func testTokenArrayBasics() {
        var arr = cxxfacade.vt.VtTokenArray()
        XCTAssertTrue(arr.empty())

        arr.push_back(cxxfacade.tf.TfToken("vertex"))
        arr.push_back(cxxfacade.tf.TfToken("faceVarying"))

        XCTAssertEqual(arr.size(), 2)
    }

    func testTokenArrayGetSet() {
        var arr = cxxfacade.vt.VtTokenArray(3)

        arr.Set(0, cxxfacade.tf.TfToken("constant"))
        arr.Set(1, cxxfacade.tf.TfToken("uniform"))
        arr.Set(2, cxxfacade.tf.TfToken("vertex"))

        let token0 = arr.Get(0)
        let token1 = arr.Get(1)
        let token2 = arr.Get(2)

        XCTAssertEqual(String(cString: token0.GetText()), "constant")
        XCTAssertEqual(String(cString: token1.GetText()), "uniform")
        XCTAssertEqual(String(cString: token2.GetText()), "vertex")
    }

    func testTokenArraySized() {
        let arr = cxxfacade.vt.VtTokenArray(5)
        XCTAssertEqual(arr.size(), 5)
    }

    func testTokenArrayClear() {
        var arr = cxxfacade.vt.VtTokenArray()
        arr.push_back(cxxfacade.tf.TfToken("test"))
        arr.push_back(cxxfacade.tf.TfToken("test2"))
        XCTAssertEqual(arr.size(), 2)

        arr.clear()
        XCTAssertTrue(arr.empty())
    }

    // MARK: - VtValue Tests

    func testValueBool() {
        let val = cxxfacade.vt.VtValue(true)
        XCTAssertFalse(val.IsEmpty())
        XCTAssertTrue(val.GetBool())

        let valFalse = cxxfacade.vt.VtValue(false)
        XCTAssertFalse(valFalse.GetBool())
    }

    func testValueInt() {
        let val = cxxfacade.vt.VtValue(42 as Int32)
        XCTAssertFalse(val.IsEmpty())
        XCTAssertEqual(val.GetInt(), 42)
    }

    func testValueFloat() {
        let val = cxxfacade.vt.VtValue(3.14 as Float)
        XCTAssertFalse(val.IsEmpty())
        XCTAssertEqual(val.GetFloat(), 3.14, accuracy: 0.001)
    }

    func testValueDouble() {
        let val = cxxfacade.vt.VtValue(2.71828)
        XCTAssertFalse(val.IsEmpty())
        XCTAssertEqual(val.GetDouble(), 2.71828, accuracy: 0.00001)
    }

    func testValueString() {
        let val = cxxfacade.vt.VtValue(std.string("Hello USD"))
        XCTAssertFalse(val.IsEmpty())
        XCTAssertEqual(String(val.GetString()), "Hello USD")
    }

    func testValueEmpty() {
        let val = cxxfacade.vt.VtValue()
        XCTAssertTrue(val.IsEmpty())
    }

    func testValueCopy() {
        let original = cxxfacade.vt.VtValue(123 as Int32)
        let copy = original
        XCTAssertEqual(copy.GetInt(), 123)
    }

    // MARK: - Copy Semantics Tests

    func testIntArrayCopy() {
        var original = cxxfacade.vt.VtIntArray()
        original.push_back(1)
        original.push_back(2)
        original.push_back(3)

        var copy = original

        // Modify copy
        copy.push_back(4)

        // Original should be independent (copy-on-write)
        XCTAssertEqual(original.size(), 3)
        XCTAssertEqual(copy.size(), 4)
    }

    func testVec3fArrayCopy() {
        var original = cxxfacade.vt.VtVec3fArray()
        original.push_back(cxxfacade.gf.Vec3f(1.0, 0.0, 0.0))

        var copy = original
        copy.push_back(cxxfacade.gf.Vec3f(0.0, 1.0, 0.0))

        XCTAssertEqual(original.size(), 1)
        XCTAssertEqual(copy.size(), 2)
    }

    // MARK: - Move Semantics Tests

    func testIntArrayMove() {
        var arr = cxxfacade.vt.VtIntArray()
        arr.push_back(1)
        arr.push_back(2)

        // Move semantics are tested implicitly through normal Swift operations
        var arr2 = arr
        arr2.push_back(3)

        XCTAssertEqual(arr.size(), 2)
        XCTAssertEqual(arr2.size(), 3)
    }

    // MARK: - Resize and Clear Tests

    func testArrayOperations() {
        var arr = cxxfacade.vt.VtFloatArray()

        // Reserve doesn't change size
        arr.reserve(100)
        XCTAssertEqual(arr.size(), 0)

        // Resize changes size
        arr.resize(50)
        XCTAssertEqual(arr.size(), 50)

        // Push back increases size
        arr.push_back(1.0)
        XCTAssertEqual(arr.size(), 51)

        // Clear removes all elements
        arr.clear()
        XCTAssertEqual(arr.size(), 0)
        XCTAssertTrue(arr.empty())
    }

    // MARK: - Data Pointer Tests

    func testFloatArrayDataPointer() {
        var arr = cxxfacade.vt.VtFloatArray()
        arr.push_back(1.0)
        arr.push_back(2.0)
        arr.push_back(3.0)

        let ptr = arr.data()
        XCTAssertNotNil(ptr)

        // Verify we can read through the pointer
        if let ptr = ptr {
            XCTAssertEqual(ptr[0], 1.0, accuracy: 0.001)
            XCTAssertEqual(ptr[1], 2.0, accuracy: 0.001)
            XCTAssertEqual(ptr[2], 3.0, accuracy: 0.001)
        }
    }

    func testVec3fArrayDataPointer() {
        var arr = cxxfacade.vt.VtVec3fArray()
        arr.push_back(cxxfacade.gf.Vec3f(1.0, 2.0, 3.0))
        arr.push_back(cxxfacade.gf.Vec3f(4.0, 5.0, 6.0))

        let ptr = arr.data()
        XCTAssertNotNil(ptr)

        if let ptr = ptr {
            XCTAssertEqual(ptr[0].x, 1.0, accuracy: 0.001)
            XCTAssertEqual(ptr[1].x, 4.0, accuracy: 0.001)
        }
    }
}
