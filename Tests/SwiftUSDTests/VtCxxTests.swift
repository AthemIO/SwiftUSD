// VtCxxTests.swift - Tests for Vt C++ interop array types
// Tests: IntArray, FloatArray, DoubleArray, Vec3fArray, Vec3dArray, Matrix4dArray

import XCTest
@testable import SwiftUSD
import USDCxx

final class VtCxxTests: XCTestCase {

    // MARK: - IntArray Tests

    func testIntArrayCreation() {
        let arr = swiftusd.IntArray()
        XCTAssertEqual(arr.count, 0)
        XCTAssertTrue(arr.isEmpty)
    }

    func testIntArrayWithSize() {
        let arr = swiftusd.IntArray(5)
        XCTAssertEqual(arr.count, 5)
        XCTAssertFalse(arr.isEmpty)
        // Default values should be 0
        XCTAssertEqual(arr[0], 0)
        XCTAssertEqual(arr[4], 0)
    }

    func testIntArrayFromSwiftArray() {
        let arr = VtIntArray.from([1, 2, 3, 4, 5])
        XCTAssertEqual(arr.count, 5)
        XCTAssertEqual(arr[0], 1)
        XCTAssertEqual(arr[4], 5)
    }

    func testIntArrayElements() {
        let arr = VtIntArray.from([10, 20, 30])
        let elements = arr.elements
        XCTAssertEqual(elements, [10, 20, 30])
    }

    func testIntArrayModification() {
        var arr = VtIntArray.from([1, 2, 3])
        arr[1] = 42
        XCTAssertEqual(arr[1], 42)
    }

    func testIntArrayAppend() {
        var arr = swiftusd.IntArray()
        arr.append(100)
        arr.append(200)
        XCTAssertEqual(arr.count, 2)
        XCTAssertEqual(arr[0], 100)
        XCTAssertEqual(arr[1], 200)
    }

    func testIntArrayClear() {
        var arr = VtIntArray.from([1, 2, 3])
        arr.clear()
        XCTAssertTrue(arr.isEmpty)
    }

    func testIntArrayEquality() {
        let arr1 = VtIntArray.from([1, 2, 3])
        let arr2 = VtIntArray.from([1, 2, 3])
        let arr3 = VtIntArray.from([1, 2, 4])
        XCTAssertEqual(arr1, arr2)
        XCTAssertNotEqual(arr1, arr3)
    }

    func testIntArrayHashable() {
        let arr1 = VtIntArray.from([1, 2, 3])
        let arr2 = VtIntArray.from([1, 2, 3])
        var set = Set<VtIntArray>()
        set.insert(arr1)
        set.insert(arr2)
        XCTAssertEqual(set.count, 1)
    }

    func testIntArrayDescription() {
        let arr = VtIntArray.from([1, 2, 3])
        let desc = arr.description
        XCTAssertTrue(desc.contains("VtIntArray"))
        XCTAssertTrue(desc.contains("1"))
        XCTAssertTrue(desc.contains("2"))
        XCTAssertTrue(desc.contains("3"))
    }

    // MARK: - FloatArray Tests

    func testFloatArrayCreation() {
        let arr = swiftusd.FloatArray()
        XCTAssertEqual(arr.count, 0)
        XCTAssertTrue(arr.isEmpty)
    }

    func testFloatArrayFromSwiftArray() {
        let arr = VtFloatArray.from([1.5, 2.5, 3.5])
        XCTAssertEqual(arr.count, 3)
        XCTAssertEqual(arr[0], 1.5, accuracy: 1e-6)
        XCTAssertEqual(arr[2], 3.5, accuracy: 1e-6)
    }

    func testFloatArrayElements() {
        let arr = VtFloatArray.from([1.0, 2.0, 3.0])
        let elements = arr.elements
        XCTAssertEqual(elements.count, 3)
        XCTAssertEqual(elements[0], 1.0, accuracy: 1e-6)
    }

    func testFloatArrayModification() {
        var arr = VtFloatArray.from([1.0, 2.0, 3.0])
        arr[1] = 42.5
        XCTAssertEqual(arr[1], 42.5, accuracy: 1e-6)
    }

    func testFloatArrayEquality() {
        let arr1 = VtFloatArray.from([1.0, 2.0, 3.0])
        let arr2 = VtFloatArray.from([1.0, 2.0, 3.0])
        XCTAssertEqual(arr1, arr2)
    }

    // MARK: - DoubleArray Tests

    func testDoubleArrayCreation() {
        let arr = swiftusd.DoubleArray()
        XCTAssertEqual(arr.count, 0)
        XCTAssertTrue(arr.isEmpty)
    }

    func testDoubleArrayFromSwiftArray() {
        let arr = VtDoubleArray.from([1.5, 2.5, 3.5])
        XCTAssertEqual(arr.count, 3)
        XCTAssertEqual(arr[0], 1.5, accuracy: 1e-10)
        XCTAssertEqual(arr[2], 3.5, accuracy: 1e-10)
    }

    func testDoubleArrayElements() {
        let arr = VtDoubleArray.from([1.0, 2.0, 3.0])
        let elements = arr.elements
        XCTAssertEqual(elements.count, 3)
        XCTAssertEqual(elements[0], 1.0, accuracy: 1e-10)
    }

    func testDoubleArrayEquality() {
        let arr1 = VtDoubleArray.from([1.0, 2.0, 3.0])
        let arr2 = VtDoubleArray.from([1.0, 2.0, 3.0])
        XCTAssertEqual(arr1, arr2)
    }

    // MARK: - Vec3fArray Tests

    func testVec3fArrayCreation() {
        let arr = swiftusd.Vec3fArray()
        XCTAssertEqual(arr.count, 0)
        XCTAssertTrue(arr.isEmpty)
    }

    func testVec3fArrayWithSize() {
        let arr = swiftusd.Vec3fArray(3)
        XCTAssertEqual(arr.count, 3)
        // Default values should be zero vectors
        let v = arr[0]
        XCTAssertEqual(v.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(v.z, 0.0, accuracy: 1e-6)
    }

    func testVec3fArrayFromSwiftArray() {
        let v1 = swiftusd.Vec3f(1.0, 2.0, 3.0)
        let v2 = swiftusd.Vec3f(4.0, 5.0, 6.0)
        let arr = VtVec3fArray.from([v1, v2])

        XCTAssertEqual(arr.count, 2)
        XCTAssertEqual(arr[0].x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(arr[1].z, 6.0, accuracy: 1e-6)
    }

    func testVec3fArrayElements() {
        let v1 = swiftusd.Vec3f(1.0, 2.0, 3.0)
        let v2 = swiftusd.Vec3f(4.0, 5.0, 6.0)
        let arr = VtVec3fArray.from([v1, v2])

        let elements = arr.elements
        XCTAssertEqual(elements.count, 2)
        XCTAssertEqual(elements[0].x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(elements[1].y, 5.0, accuracy: 1e-6)
    }

    func testVec3fArrayModification() {
        let v1 = swiftusd.Vec3f(1.0, 2.0, 3.0)
        var arr = VtVec3fArray.from([v1])
        arr[0] = swiftusd.Vec3f(10.0, 20.0, 30.0)
        XCTAssertEqual(arr[0].x, 10.0, accuracy: 1e-6)
    }

    func testVec3fArrayEquality() {
        let v1 = swiftusd.Vec3f(1.0, 2.0, 3.0)
        let arr1 = VtVec3fArray.from([v1])
        let arr2 = VtVec3fArray.from([v1])
        XCTAssertEqual(arr1, arr2)
    }

    func testVec3fArrayDescription() {
        let v1 = swiftusd.Vec3f(1.0, 2.0, 3.0)
        let arr = VtVec3fArray.from([v1])
        let desc = arr.description
        XCTAssertTrue(desc.contains("VtVec3fArray"))
    }

    // MARK: - Vec3dArray Tests

    func testVec3dArrayCreation() {
        let arr = swiftusd.Vec3dArray()
        XCTAssertEqual(arr.count, 0)
        XCTAssertTrue(arr.isEmpty)
    }

    func testVec3dArrayFromSwiftArray() {
        let v1 = swiftusd.Vec3d(1.0, 2.0, 3.0)
        let v2 = swiftusd.Vec3d(4.0, 5.0, 6.0)
        let arr = VtVec3dArray.from([v1, v2])

        XCTAssertEqual(arr.count, 2)
        XCTAssertEqual(arr[0].x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(arr[1].z, 6.0, accuracy: 1e-10)
    }

    func testVec3dArrayEquality() {
        let v1 = swiftusd.Vec3d(1.0, 2.0, 3.0)
        let arr1 = VtVec3dArray.from([v1])
        let arr2 = VtVec3dArray.from([v1])
        XCTAssertEqual(arr1, arr2)
    }

    // MARK: - Matrix4dArray Tests

    func testMatrix4dArrayCreation() {
        let arr = swiftusd.Matrix4dArray()
        XCTAssertEqual(arr.count, 0)
        XCTAssertTrue(arr.isEmpty)
    }

    func testMatrix4dArrayWithSize() {
        let arr = swiftusd.Matrix4dArray(2)
        XCTAssertEqual(arr.count, 2)
        // Default values should be identity matrices
        let m = arr[0]
        XCTAssertEqual(m.Get(0, 0), 1.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(1, 1), 1.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(0, 1), 0.0, accuracy: 1e-10)
    }

    func testMatrix4dArrayFromSwiftArray() {
        let m1 = swiftusd.Matrix4d.Identity()
        let m2 = swiftusd.Matrix4d.SetTranslate(1.0, 2.0, 3.0)
        let arr = VtMatrix4dArray.from([m1, m2])

        XCTAssertEqual(arr.count, 2)
        XCTAssertEqual(arr[0].Get(0, 0), 1.0, accuracy: 1e-10)
        let t = arr[1].ExtractTranslation()
        XCTAssertEqual(t.x, 1.0, accuracy: 1e-10)
    }

    func testMatrix4dArrayElements() {
        let m1 = swiftusd.Matrix4d.Identity()
        let arr = VtMatrix4dArray.from([m1])

        let elements = arr.elements
        XCTAssertEqual(elements.count, 1)
        XCTAssertEqual(elements[0].Get(0, 0), 1.0, accuracy: 1e-10)
    }

    func testMatrix4dArrayEquality() {
        let m1 = swiftusd.Matrix4d.Identity()
        let arr1 = VtMatrix4dArray.from([m1])
        let arr2 = VtMatrix4dArray.from([m1])
        XCTAssertEqual(arr1, arr2)
    }

    func testMatrix4dArrayDescription() {
        let m1 = swiftusd.Matrix4d.Identity()
        let arr = VtMatrix4dArray.from([m1])
        let desc = arr.description
        XCTAssertTrue(desc.contains("VtMatrix4dArray"))
        XCTAssertTrue(desc.contains("1"))
    }

    // MARK: - Resize and Reserve Tests

    func testIntArrayResize() {
        var arr = VtIntArray.from([1, 2, 3])
        arr.resize(5)
        XCTAssertEqual(arr.count, 5)
        XCTAssertEqual(arr[0], 1)  // Original value preserved
        XCTAssertEqual(arr[4], 0)  // New values are 0
    }

    func testIntArrayReserve() {
        var arr = swiftusd.IntArray()
        arr.reserve(100)
        XCTAssertGreaterThanOrEqual(Int(arr.capacity()), 100)
        XCTAssertEqual(arr.count, 0)  // Size unchanged
    }

    func testFloatArrayResize() {
        var arr = VtFloatArray.from([1.0, 2.0])
        arr.resize(4)
        XCTAssertEqual(arr.count, 4)
        XCTAssertEqual(arr[0], 1.0, accuracy: 1e-6)
    }

    // MARK: - PopBack Tests

    func testIntArrayPopBack() {
        var arr = VtIntArray.from([1, 2, 3])
        arr.popBack()
        XCTAssertEqual(arr.count, 2)
        XCTAssertEqual(arr.elements, [1, 2])
    }

    func testFloatArrayPopBack() {
        var arr = VtFloatArray.from([1.0, 2.0, 3.0])
        arr.popBack()
        XCTAssertEqual(arr.count, 2)
    }

    // MARK: - Assign Tests

    func testIntArrayAssign() {
        var arr = VtIntArray.from([1, 2, 3])
        let newData: [Int32] = [10, 20, 30, 40]
        newData.withUnsafeBufferPointer { buffer in
            arr.assign(buffer.baseAddress, buffer.count)
        }
        XCTAssertEqual(arr.elements, [10, 20, 30, 40])
    }

    // MARK: - Large Array Tests

    func testLargeIntArray() {
        let size = 10000
        let data = (0..<size).map { Int32($0) }
        let arr = VtIntArray.from(data)

        XCTAssertEqual(arr.count, size)
        XCTAssertEqual(arr[0], 0)
        XCTAssertEqual(arr[9999], 9999)
    }

    func testLargeVec3fArray() {
        let size = 1000
        let data = (0..<size).map { swiftusd.Vec3f(Float($0), Float($0 * 2), Float($0 * 3)) }
        let arr = VtVec3fArray.from(data)

        XCTAssertEqual(arr.count, size)
        XCTAssertEqual(arr[500].x, 500.0, accuracy: 1e-6)
        XCTAssertEqual(arr[500].y, 1000.0, accuracy: 1e-6)
    }
}
