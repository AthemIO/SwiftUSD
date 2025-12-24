// GfTests.swift - Tests for Gf module
// Tests: Vec3f, Vec3d, Matrix4d, Quatd, Range3d, BBox3d

import XCTest
@testable import SwiftUSD

final class GfTests: XCTestCase {

    // MARK: - Vec3f Tests

    func testVec3fCreation() throws {
        let v = try Vec3f(1.0, 2.0, 3.0)
        XCTAssertEqual(v.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 2.0, accuracy: 1e-6)
        XCTAssertEqual(v.z, 3.0, accuracy: 1e-6)
    }

    func testVec3fScalar() throws {
        let v = try Vec3f(scalar: 5.0)
        XCTAssertEqual(v.x, 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.z, 5.0, accuracy: 1e-6)
    }

    func testVec3fZero() throws {
        let v = try Vec3f.zero
        XCTAssertEqual(v.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(v.z, 0.0, accuracy: 1e-6)
    }

    func testVec3fAxes() throws {
        let xAxis = try Vec3f.xAxis
        XCTAssertEqual(xAxis.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(xAxis.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(xAxis.z, 0.0, accuracy: 1e-6)

        let yAxis = try Vec3f.yAxis
        XCTAssertEqual(yAxis.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(yAxis.y, 1.0, accuracy: 1e-6)
        XCTAssertEqual(yAxis.z, 0.0, accuracy: 1e-6)

        let zAxis = try Vec3f.zAxis
        XCTAssertEqual(zAxis.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(zAxis.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(zAxis.z, 1.0, accuracy: 1e-6)
    }

    func testVec3fLength() throws {
        let v = try Vec3f(3.0, 4.0, 0.0)
        XCTAssertEqual(v.length, 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.lengthSquared, 25.0, accuracy: 1e-6)
    }

    func testVec3fNormalize() throws {
        let v = try Vec3f(3.0, 4.0, 0.0)
        let normalized = try v.normalized()
        XCTAssertEqual(normalized.length, 1.0, accuracy: 1e-6)
        XCTAssertEqual(normalized.x, 0.6, accuracy: 1e-6)
        XCTAssertEqual(normalized.y, 0.8, accuracy: 1e-6)
    }

    func testVec3fDotProduct() throws {
        let v1 = try Vec3f(1.0, 0.0, 0.0)
        let v2 = try Vec3f(0.0, 1.0, 0.0)
        XCTAssertEqual(v1.dot(v2), 0.0, accuracy: 1e-6)

        let v3 = try Vec3f(1.0, 2.0, 3.0)
        let v4 = try Vec3f(4.0, 5.0, 6.0)
        XCTAssertEqual(v3.dot(v4), 32.0, accuracy: 1e-6) // 1*4 + 2*5 + 3*6
    }

    func testVec3fCrossProduct() throws {
        let v1 = try Vec3f(1.0, 0.0, 0.0)
        let v2 = try Vec3f(0.0, 1.0, 0.0)
        let cross = try v1.cross(v2)
        XCTAssertEqual(cross.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(cross.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(cross.z, 1.0, accuracy: 1e-6)
    }

    func testVec3fArithmetic() throws {
        let v1 = try Vec3f(1.0, 2.0, 3.0)
        let v2 = try Vec3f(4.0, 5.0, 6.0)

        if let sum = v1 + v2 {
            XCTAssertEqual(sum.x, 5.0, accuracy: 1e-6)
            XCTAssertEqual(sum.y, 7.0, accuracy: 1e-6)
            XCTAssertEqual(sum.z, 9.0, accuracy: 1e-6)
        } else {
            XCTFail("Addition failed")
        }

        if let diff = v2 - v1 {
            XCTAssertEqual(diff.x, 3.0, accuracy: 1e-6)
            XCTAssertEqual(diff.y, 3.0, accuracy: 1e-6)
            XCTAssertEqual(diff.z, 3.0, accuracy: 1e-6)
        } else {
            XCTFail("Subtraction failed")
        }

        if let scaled = v1 * 2.0 {
            XCTAssertEqual(scaled.x, 2.0, accuracy: 1e-6)
            XCTAssertEqual(scaled.y, 4.0, accuracy: 1e-6)
            XCTAssertEqual(scaled.z, 6.0, accuracy: 1e-6)
        } else {
            XCTFail("Multiplication failed")
        }
    }

    func testVec3fEquality() throws {
        let v1 = try Vec3f(1.0, 2.0, 3.0)
        let v2 = try Vec3f(1.0, 2.0, 3.0)
        let v3 = try Vec3f(1.0, 2.0, 4.0)

        XCTAssertEqual(v1, v2)
        XCTAssertNotEqual(v1, v3)
    }

    func testVec3fIsClose() throws {
        let v1 = try Vec3f(1.0, 2.0, 3.0)
        let v2 = try Vec3f(1.001, 2.001, 3.001)

        XCTAssertTrue(v1.isClose(to: v2, tolerance: 0.01))
        XCTAssertFalse(v1.isClose(to: v2, tolerance: 0.0001))
    }

    // MARK: - Vec3d Tests

    func testVec3dCreation() throws {
        let v = try Vec3d(1.0, 2.0, 3.0)
        XCTAssertEqual(v.x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(v.y, 2.0, accuracy: 1e-10)
        XCTAssertEqual(v.z, 3.0, accuracy: 1e-10)
    }

    func testVec3dLength() throws {
        let v = try Vec3d(3.0, 4.0, 0.0)
        XCTAssertEqual(v.length, 5.0, accuracy: 1e-10)
    }

    func testVec3dCrossProduct() throws {
        let v1 = try Vec3d(1.0, 0.0, 0.0)
        let v2 = try Vec3d(0.0, 1.0, 0.0)
        let cross = try v1.cross(v2)
        XCTAssertEqual(cross.z, 1.0, accuracy: 1e-10)
    }

    // MARK: - Matrix4d Tests

    func testMatrix4dIdentity() throws {
        let m = try Matrix4d.identity
        XCTAssertEqual(m[0, 0], 1.0, accuracy: 1e-10)
        XCTAssertEqual(m[1, 1], 1.0, accuracy: 1e-10)
        XCTAssertEqual(m[2, 2], 1.0, accuracy: 1e-10)
        XCTAssertEqual(m[3, 3], 1.0, accuracy: 1e-10)
        XCTAssertEqual(m[0, 1], 0.0, accuracy: 1e-10)
    }

    func testMatrix4dTranslation() throws {
        let m = try Matrix4d.translation(1.0, 2.0, 3.0)
        let t = m.extractTranslation()
        XCTAssertEqual(t.x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(t.y, 2.0, accuracy: 1e-10)
        XCTAssertEqual(t.z, 3.0, accuracy: 1e-10)
    }

    func testMatrix4dScale() throws {
        let m = try Matrix4d.scale(2.0)
        XCTAssertEqual(m[0, 0], 2.0, accuracy: 1e-10)
        XCTAssertEqual(m[1, 1], 2.0, accuracy: 1e-10)
        XCTAssertEqual(m[2, 2], 2.0, accuracy: 1e-10)
    }

    func testMatrix4dMultiplication() throws {
        let m1 = try Matrix4d.translation(1.0, 0.0, 0.0)
        let m2 = try Matrix4d.translation(0.0, 2.0, 0.0)

        if let product = m1 * m2 {
            let t = product.extractTranslation()
            XCTAssertEqual(t.x, 1.0, accuracy: 1e-10)
            XCTAssertEqual(t.y, 2.0, accuracy: 1e-10)
        } else {
            XCTFail("Matrix multiplication failed")
        }
    }

    func testMatrix4dDeterminant() throws {
        let identity = try Matrix4d.identity
        XCTAssertEqual(identity.determinant, 1.0, accuracy: 1e-10)

        let scale = try Matrix4d.scale(2.0)
        XCTAssertEqual(scale.determinant, 8.0, accuracy: 1e-10) // 2^3 (3D scale in homogeneous coords)
    }

    func testMatrix4dTransform() throws {
        let translate = try Matrix4d.translation(10.0, 20.0, 30.0)
        let result = translate.transform(0.0, 0.0, 0.0)
        XCTAssertEqual(result.x, 10.0, accuracy: 1e-10)
        XCTAssertEqual(result.y, 20.0, accuracy: 1e-10)
        XCTAssertEqual(result.z, 30.0, accuracy: 1e-10)
    }

    // MARK: - Quatd Tests

    func testQuatdIdentity() throws {
        let q = try Quatd.identity
        XCTAssertEqual(q.real, 1.0, accuracy: 1e-10)
        XCTAssertEqual(q.i, 0.0, accuracy: 1e-10)
        XCTAssertEqual(q.j, 0.0, accuracy: 1e-10)
        XCTAssertEqual(q.k, 0.0, accuracy: 1e-10)
    }

    func testQuatdLength() throws {
        let q = try Quatd(real: 1.0, i: 0.0, j: 0.0, k: 0.0)
        XCTAssertEqual(q.length, 1.0, accuracy: 1e-10)
    }

    func testQuatdNormalize() throws {
        let q = try Quatd(real: 2.0, i: 0.0, j: 0.0, k: 0.0)
        let normalized = try q.normalized()
        XCTAssertEqual(normalized.length, 1.0, accuracy: 1e-10)
    }

    func testQuatdConjugate() throws {
        let q = try Quatd(real: 1.0, i: 2.0, j: 3.0, k: 4.0)
        let conj = try q.conjugate()
        XCTAssertEqual(conj.real, 1.0, accuracy: 1e-10)
        XCTAssertEqual(conj.i, -2.0, accuracy: 1e-10)
        XCTAssertEqual(conj.j, -3.0, accuracy: 1e-10)
        XCTAssertEqual(conj.k, -4.0, accuracy: 1e-10)
    }

    func testQuatdMultiplication() throws {
        let q1 = try Quatd.identity
        let q2 = try Quatd(real: 0.0, i: 1.0, j: 0.0, k: 0.0)

        if let product = q1 * q2 {
            XCTAssertEqual(product.real, 0.0, accuracy: 1e-10)
            XCTAssertEqual(product.i, 1.0, accuracy: 1e-10)
        } else {
            XCTFail("Quaternion multiplication failed")
        }
    }

    func testQuatdSlerp() throws {
        let q1 = try Quatd.identity
        let q2 = try Quatd(real: 0.0, i: 0.0, j: 0.0, k: 1.0)

        let mid = try Quatd.slerp(from: q1, to: q2, alpha: 0.5)
        XCTAssertEqual(mid.length, 1.0, accuracy: 0.01)
    }

    // MARK: - Range3d Tests

    func testRange3dCreation() throws {
        let r = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 2, maxZ: 3)
        XCTAssertEqual(r.min.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(r.max.z, 3.0, accuracy: 1e-10)
    }

    func testRange3dEmpty() throws {
        let r = try Range3d.empty
        XCTAssertTrue(r.isEmpty)
    }

    func testRange3dUnitCube() throws {
        let r = try Range3d.unitCube
        XCTAssertEqual(r.min.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(r.max.x, 1.0, accuracy: 1e-10)
    }

    func testRange3dContainsPoint() throws {
        let r = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 10, maxY: 10, maxZ: 10)
        XCTAssertTrue(r.contains(5.0, 5.0, 5.0))
        XCTAssertFalse(r.contains(15.0, 5.0, 5.0))
    }

    func testRange3dSize() throws {
        let r = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 2, maxY: 3, maxZ: 4)
        let size = r.size
        XCTAssertEqual(size.x, 2.0, accuracy: 1e-10)
        XCTAssertEqual(size.y, 3.0, accuracy: 1e-10)
        XCTAssertEqual(size.z, 4.0, accuracy: 1e-10)
    }

    func testRange3dMidpoint() throws {
        let r = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 10, maxY: 10, maxZ: 10)
        let mid = r.midpoint
        XCTAssertEqual(mid.x, 5.0, accuracy: 1e-10)
        XCTAssertEqual(mid.y, 5.0, accuracy: 1e-10)
        XCTAssertEqual(mid.z, 5.0, accuracy: 1e-10)
    }

    func testRange3dUnion() throws {
        let r1 = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let r2 = try Range3d(minX: 2, minY: 2, minZ: 2, maxX: 3, maxY: 3, maxZ: 3)
        let u = try Range3d.union(r1, r2)
        XCTAssertEqual(u.min.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(u.max.x, 3.0, accuracy: 1e-10)
    }

    // MARK: - BBox3d Tests

    func testBBox3dCreation() throws {
        let bbox = try BBox3d()
        XCTAssertNotNil(bbox)
    }

    func testBBox3dFromRange() throws {
        let range = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let bbox = try BBox3d(range: range)
        XCTAssertNotNil(bbox.range)
    }

    func testBBox3dVolume() throws {
        let range = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 2, maxY: 3, maxZ: 4)
        let bbox = try BBox3d(range: range)
        XCTAssertEqual(bbox.volume, 24.0, accuracy: 1e-10) // 2*3*4
    }

    func testBBox3dCentroid() throws {
        let range = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 10, maxY: 10, maxZ: 10)
        let bbox = try BBox3d(range: range)
        let c = bbox.centroid
        XCTAssertEqual(c.x, 5.0, accuracy: 1e-10)
        XCTAssertEqual(c.y, 5.0, accuracy: 1e-10)
        XCTAssertEqual(c.z, 5.0, accuracy: 1e-10)
    }

    func testBBox3dCombine() throws {
        let r1 = try Range3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let r2 = try Range3d(minX: 2, minY: 2, minZ: 2, maxX: 3, maxY: 3, maxZ: 3)
        let b1 = try BBox3d(range: r1)
        let b2 = try BBox3d(range: r2)
        let combined = try BBox3d.combine(b1, b2)
        XCTAssertNotNil(combined)
    }
}
