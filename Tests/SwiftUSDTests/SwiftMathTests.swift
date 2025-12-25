// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the SwiftUSD Project

import XCTest
import simd
@testable import SwiftUSD
import USDCxx

final class SwiftMathTests: XCTestCase {

    // MARK: - Vec2 <-> SIMD2 Tests

    func testVec2fToSIMD2() {
        let vec = swiftusd.Vec2f(1.0, 2.0)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 1.0)
        XCTAssertEqual(simdVec.y, 2.0)
    }

    func testSIMD2ToVec2f() {
        let simdVec = SIMD2<Float>(3.0, 4.0)
        let vec = swiftusd.Vec2f(simdVec)

        XCTAssertEqual(vec.x, 3.0)
        XCTAssertEqual(vec.y, 4.0)
    }

    func testVec2dToSIMD2() {
        let vec = swiftusd.Vec2d(1.5, 2.5)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 1.5)
        XCTAssertEqual(simdVec.y, 2.5)
    }

    func testSIMD2ToVec2d() {
        let simdVec = SIMD2<Double>(3.5, 4.5)
        let vec = swiftusd.Vec2d(simdVec)

        XCTAssertEqual(vec.x, 3.5)
        XCTAssertEqual(vec.y, 4.5)
    }

    func testVec2iToSIMD2() {
        let vec = swiftusd.Vec2i(10, 20)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 10)
        XCTAssertEqual(simdVec.y, 20)
    }

    func testSIMD2ToVec2i() {
        let simdVec = SIMD2<Int32>(30, 40)
        let vec = swiftusd.Vec2i(simdVec)

        XCTAssertEqual(vec.x, 30)
        XCTAssertEqual(vec.y, 40)
    }

    // MARK: - Vec3 <-> SIMD3 Tests

    func testVec3fToSIMD3() {
        let vec = swiftusd.Vec3f(1.0, 2.0, 3.0)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 1.0)
        XCTAssertEqual(simdVec.y, 2.0)
        XCTAssertEqual(simdVec.z, 3.0)
    }

    func testSIMD3ToVec3f() {
        let simdVec = SIMD3<Float>(4.0, 5.0, 6.0)
        let vec = swiftusd.Vec3f(simdVec)

        XCTAssertEqual(vec.x, 4.0)
        XCTAssertEqual(vec.y, 5.0)
        XCTAssertEqual(vec.z, 6.0)
    }

    func testVec3dToSIMD3() {
        let vec = swiftusd.Vec3d(1.5, 2.5, 3.5)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 1.5)
        XCTAssertEqual(simdVec.y, 2.5)
        XCTAssertEqual(simdVec.z, 3.5)
    }

    func testSIMD3ToVec3d() {
        let simdVec = SIMD3<Double>(4.5, 5.5, 6.5)
        let vec = swiftusd.Vec3d(simdVec)

        XCTAssertEqual(vec.x, 4.5)
        XCTAssertEqual(vec.y, 5.5)
        XCTAssertEqual(vec.z, 6.5)
    }

    func testVec3iToSIMD3() {
        let vec = swiftusd.Vec3i(10, 20, 30)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 10)
        XCTAssertEqual(simdVec.y, 20)
        XCTAssertEqual(simdVec.z, 30)
    }

    func testSIMD3ToVec3i() {
        let simdVec = SIMD3<Int32>(40, 50, 60)
        let vec = swiftusd.Vec3i(simdVec)

        XCTAssertEqual(vec.x, 40)
        XCTAssertEqual(vec.y, 50)
        XCTAssertEqual(vec.z, 60)
    }

    // MARK: - Vec4 <-> SIMD4 Tests

    func testVec4fToSIMD4() {
        let vec = swiftusd.Vec4f(1.0, 2.0, 3.0, 4.0)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 1.0)
        XCTAssertEqual(simdVec.y, 2.0)
        XCTAssertEqual(simdVec.z, 3.0)
        XCTAssertEqual(simdVec.w, 4.0)
    }

    func testSIMD4ToVec4f() {
        let simdVec = SIMD4<Float>(5.0, 6.0, 7.0, 8.0)
        let vec = swiftusd.Vec4f(simdVec)

        XCTAssertEqual(vec.x, 5.0)
        XCTAssertEqual(vec.y, 6.0)
        XCTAssertEqual(vec.z, 7.0)
        XCTAssertEqual(vec.w, 8.0)
    }

    func testVec4dToSIMD4() {
        let vec = swiftusd.Vec4d(1.5, 2.5, 3.5, 4.5)
        let simdVec = vec.simd

        XCTAssertEqual(simdVec.x, 1.5)
        XCTAssertEqual(simdVec.y, 2.5)
        XCTAssertEqual(simdVec.z, 3.5)
        XCTAssertEqual(simdVec.w, 4.5)
    }

    func testSIMD4ToVec4d() {
        let simdVec = SIMD4<Double>(5.5, 6.5, 7.5, 8.5)
        let vec = swiftusd.Vec4d(simdVec)

        XCTAssertEqual(vec.x, 5.5)
        XCTAssertEqual(vec.y, 6.5)
        XCTAssertEqual(vec.z, 7.5)
        XCTAssertEqual(vec.w, 8.5)
    }

    // MARK: - Matrix4 <-> simd_float4x4/simd_double4x4 Tests

    func testMatrix4fToSimdFloat4x4() {
        // Create a simple translation matrix in row-major format
        // Identity with translation (10, 20, 30)
        let matrix = swiftusd.Matrix4f(
            1, 0, 0, 10,
            0, 1, 0, 20,
            0, 0, 1, 30,
            0, 0, 0, 1
        )

        let simdMatrix = matrix.simd

        // In column-major, translation is in the last column
        XCTAssertEqual(simdMatrix.columns.3.x, 10.0)
        XCTAssertEqual(simdMatrix.columns.3.y, 20.0)
        XCTAssertEqual(simdMatrix.columns.3.z, 30.0)
        XCTAssertEqual(simdMatrix.columns.3.w, 1.0)
    }

    func testSimdFloat4x4ToMatrix4f() {
        // Create a translation matrix using simd
        let translation = simd_float4x4(
            SIMD4<Float>(1, 0, 0, 0),
            SIMD4<Float>(0, 1, 0, 0),
            SIMD4<Float>(0, 0, 1, 0),
            SIMD4<Float>(5, 10, 15, 1)
        )

        let matrix = swiftusd.Matrix4f(translation)

        // In row-major, translation is at positions 3, 7, 11 (tuple indices)
        XCTAssertEqual(matrix.m.3, 5.0)
        XCTAssertEqual(matrix.m.7, 10.0)
        XCTAssertEqual(matrix.m.11, 15.0)
    }

    func testMatrix4dToSimdDouble4x4() {
        // Create a simple scaling matrix in row-major format
        let matrix = swiftusd.Matrix4d(
            2, 0, 0, 0,
            0, 3, 0, 0,
            0, 0, 4, 0,
            0, 0, 0, 1
        )

        let simdMatrix = matrix.simd

        // Check diagonal scaling values
        XCTAssertEqual(simdMatrix.columns.0.x, 2.0)
        XCTAssertEqual(simdMatrix.columns.1.y, 3.0)
        XCTAssertEqual(simdMatrix.columns.2.z, 4.0)
        XCTAssertEqual(simdMatrix.columns.3.w, 1.0)
    }

    func testMatrixRoundTrip() {
        // Create a matrix, convert to simd and back, verify equality
        let original = swiftusd.Matrix4f(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        )

        let simdMatrix = original.simd
        let restored = swiftusd.Matrix4f(simdMatrix)

        // Compare using tuple members
        XCTAssertEqual(original.m.0, restored.m.0, accuracy: 0.0001)
        XCTAssertEqual(original.m.1, restored.m.1, accuracy: 0.0001)
        XCTAssertEqual(original.m.2, restored.m.2, accuracy: 0.0001)
        XCTAssertEqual(original.m.3, restored.m.3, accuracy: 0.0001)
        XCTAssertEqual(original.m.4, restored.m.4, accuracy: 0.0001)
        XCTAssertEqual(original.m.5, restored.m.5, accuracy: 0.0001)
        XCTAssertEqual(original.m.6, restored.m.6, accuracy: 0.0001)
        XCTAssertEqual(original.m.7, restored.m.7, accuracy: 0.0001)
        XCTAssertEqual(original.m.8, restored.m.8, accuracy: 0.0001)
        XCTAssertEqual(original.m.9, restored.m.9, accuracy: 0.0001)
        XCTAssertEqual(original.m.10, restored.m.10, accuracy: 0.0001)
        XCTAssertEqual(original.m.11, restored.m.11, accuracy: 0.0001)
        XCTAssertEqual(original.m.12, restored.m.12, accuracy: 0.0001)
        XCTAssertEqual(original.m.13, restored.m.13, accuracy: 0.0001)
        XCTAssertEqual(original.m.14, restored.m.14, accuracy: 0.0001)
        XCTAssertEqual(original.m.15, restored.m.15, accuracy: 0.0001)
    }

    // MARK: - Quaternion <-> simd_quat Tests

    func testQuatfToSimdQuatf() {
        // Create a quaternion representing a rotation
        let quat = swiftusd.Quatf(0.707, 0.0, 0.707, 0.0)  // ~90 degree rotation around Y
        let simdQuat = quat.simd

        XCTAssertEqual(simdQuat.real, 0.707, accuracy: 0.001)
        XCTAssertEqual(simdQuat.imag.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(simdQuat.imag.y, 0.707, accuracy: 0.001)
        XCTAssertEqual(simdQuat.imag.z, 0.0, accuracy: 0.001)
    }

    func testSimdQuatfToQuatf() {
        let simdQuat = simd_quatf(ix: 0.1, iy: 0.2, iz: 0.3, r: 0.9)
        let quat = swiftusd.Quatf(simdQuat)

        XCTAssertEqual(quat.real, 0.9, accuracy: 0.001)
        XCTAssertEqual(quat.i, 0.1, accuracy: 0.001)
        XCTAssertEqual(quat.j, 0.2, accuracy: 0.001)
        XCTAssertEqual(quat.k, 0.3, accuracy: 0.001)
    }

    func testQuatdToSimdQuatd() {
        let quat = swiftusd.Quatd(1.0, 0.0, 0.0, 0.0)  // Identity quaternion
        let simdQuat = quat.simd

        XCTAssertEqual(simdQuat.real, 1.0, accuracy: 0.001)
        XCTAssertEqual(simdQuat.imag.x, 0.0, accuracy: 0.001)
        XCTAssertEqual(simdQuat.imag.y, 0.0, accuracy: 0.001)
        XCTAssertEqual(simdQuat.imag.z, 0.0, accuracy: 0.001)
    }

    func testSimdQuatdToQuatd() {
        let simdQuat = simd_quatd(ix: 0.5, iy: 0.5, iz: 0.5, r: 0.5)
        let quat = swiftusd.Quatd(simdQuat)

        XCTAssertEqual(quat.real, 0.5, accuracy: 0.001)
        XCTAssertEqual(quat.i, 0.5, accuracy: 0.001)
        XCTAssertEqual(quat.j, 0.5, accuracy: 0.001)
        XCTAssertEqual(quat.k, 0.5, accuracy: 0.001)
    }

    func testQuaternionRoundTrip() {
        let original = swiftusd.Quatf(0.5, 0.5, 0.5, 0.5)
        let simdQuat = original.simd
        let restored = swiftusd.Quatf(simdQuat)

        XCTAssertEqual(original.real, restored.real, accuracy: 0.0001)
        XCTAssertEqual(original.i, restored.i, accuracy: 0.0001)
        XCTAssertEqual(original.j, restored.j, accuracy: 0.0001)
        XCTAssertEqual(original.k, restored.k, accuracy: 0.0001)
    }

    // MARK: - Array Conversion Tests

    func testVec3fArrayToSIMD3Array() {
        let vectors = [
            swiftusd.Vec3f(1.0, 2.0, 3.0),
            swiftusd.Vec3f(4.0, 5.0, 6.0),
            swiftusd.Vec3f(7.0, 8.0, 9.0)
        ]

        let simdVectors = vectors.simd

        XCTAssertEqual(simdVectors.count, 3)
        XCTAssertEqual(simdVectors[0], SIMD3<Float>(1.0, 2.0, 3.0))
        XCTAssertEqual(simdVectors[1], SIMD3<Float>(4.0, 5.0, 6.0))
        XCTAssertEqual(simdVectors[2], SIMD3<Float>(7.0, 8.0, 9.0))
    }

    func testSIMD3ArrayToVec3fArray() {
        let simdVectors = [
            SIMD3<Float>(1.0, 2.0, 3.0),
            SIMD3<Float>(4.0, 5.0, 6.0)
        ]

        let vectors = simdVectors.gf

        XCTAssertEqual(vectors.count, 2)
        XCTAssertEqual(vectors[0].x, 1.0)
        XCTAssertEqual(vectors[0].y, 2.0)
        XCTAssertEqual(vectors[0].z, 3.0)
        XCTAssertEqual(vectors[1].x, 4.0)
        XCTAssertEqual(vectors[1].y, 5.0)
        XCTAssertEqual(vectors[1].z, 6.0)
    }

    // MARK: - Precision Conversion Tests

    func testVec3fToVec3d() {
        let floatVec = swiftusd.Vec3f(1.5, 2.5, 3.5)
        let doubleVec = floatVec.asDouble

        XCTAssertEqual(doubleVec.x, 1.5, accuracy: 0.0001)
        XCTAssertEqual(doubleVec.y, 2.5, accuracy: 0.0001)
        XCTAssertEqual(doubleVec.z, 3.5, accuracy: 0.0001)
    }

    func testVec3dToVec3f() {
        let doubleVec = swiftusd.Vec3d(1.5, 2.5, 3.5)
        let floatVec = doubleVec.asFloat

        XCTAssertEqual(floatVec.x, 1.5, accuracy: 0.0001)
        XCTAssertEqual(floatVec.y, 2.5, accuracy: 0.0001)
        XCTAssertEqual(floatVec.z, 3.5, accuracy: 0.0001)
    }

    func testQuatfToQuatd() {
        let floatQuat = swiftusd.Quatf(0.5, 0.5, 0.5, 0.5)
        let doubleQuat = floatQuat.asDouble

        XCTAssertEqual(doubleQuat.real, 0.5, accuracy: 0.0001)
        XCTAssertEqual(doubleQuat.i, 0.5, accuracy: 0.0001)
        XCTAssertEqual(doubleQuat.j, 0.5, accuracy: 0.0001)
        XCTAssertEqual(doubleQuat.k, 0.5, accuracy: 0.0001)
    }

    func testQuatdToQuatf() {
        let doubleQuat = swiftusd.Quatd(0.5, 0.5, 0.5, 0.5)
        let floatQuat = doubleQuat.asFloat

        XCTAssertEqual(floatQuat.real, 0.5, accuracy: 0.0001)
        XCTAssertEqual(floatQuat.i, 0.5, accuracy: 0.0001)
        XCTAssertEqual(floatQuat.j, 0.5, accuracy: 0.0001)
        XCTAssertEqual(floatQuat.k, 0.5, accuracy: 0.0001)
    }

    // MARK: - Type Alias Tests

    func testTypeAliases() {
        // Verify typealiases work correctly
        let vec3f: GfVec3f = swiftusd.Vec3f(1, 2, 3)
        let vec3d: GfVec3d = swiftusd.Vec3d(1, 2, 3)
        let mat4f: GfMatrix4f = swiftusd.Matrix4f()
        let mat4d: GfMatrix4d = swiftusd.Matrix4d()
        let quatf: GfQuatf = swiftusd.Quatf(1, 0, 0, 0)
        let quatd: GfQuatd = swiftusd.Quatd(1, 0, 0, 0)

        XCTAssertEqual(vec3f.x, 1)
        XCTAssertEqual(vec3d.y, 2)
        XCTAssertNotNil(mat4f.m)
        XCTAssertNotNil(mat4d.m)
        XCTAssertEqual(quatf.real, 1)
        XCTAssertEqual(quatd.real, 1)
    }

    // MARK: - SIMD Extension Initializer Tests

    func testSIMD3FloatInitFromVec3f() {
        let vec = swiftusd.Vec3f(10.0, 20.0, 30.0)
        let simdVec = SIMD3<Float>(vec)

        XCTAssertEqual(simdVec.x, 10.0)
        XCTAssertEqual(simdVec.y, 20.0)
        XCTAssertEqual(simdVec.z, 30.0)
    }

    func testSIMD3DoubleInitFromVec3d() {
        let vec = swiftusd.Vec3d(10.5, 20.5, 30.5)
        let simdVec = SIMD3<Double>(vec)

        XCTAssertEqual(simdVec.x, 10.5)
        XCTAssertEqual(simdVec.y, 20.5)
        XCTAssertEqual(simdVec.z, 30.5)
    }

    func testSimdFloat4x4InitFromMatrix4f() {
        // Create identity matrix using constructor
        let matrix = swiftusd.Matrix4f(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        )

        let simdMatrix = simd_float4x4(matrix)

        XCTAssertEqual(simdMatrix.columns.0.x, 1.0)
        XCTAssertEqual(simdMatrix.columns.1.y, 1.0)
        XCTAssertEqual(simdMatrix.columns.2.z, 1.0)
        XCTAssertEqual(simdMatrix.columns.3.w, 1.0)
    }

    func testSimdQuatfInitFromQuatf() {
        let quat = swiftusd.Quatf(1, 0, 0, 0)
        let simdQuat = simd_quatf(quat)

        XCTAssertEqual(simdQuat.real, 1.0)
        XCTAssertEqual(simdQuat.imag.x, 0.0)
        XCTAssertEqual(simdQuat.imag.y, 0.0)
        XCTAssertEqual(simdQuat.imag.z, 0.0)
    }
}
