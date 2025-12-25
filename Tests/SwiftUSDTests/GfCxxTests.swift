// GfCxxTests.swift - Tests for Gf C++ interop types
// Tests: Vec2f/d/i, Vec3f/d/i, Vec4f/d, Matrix4d/f, Quatd/f, Range3d/f, BBox3d

import XCTest
@testable import SwiftUSD
import USDCxx

// Create local type aliases to swiftusd types for testing
typealias GfVec2f = swiftusd.Vec2f
typealias GfVec2d = swiftusd.Vec2d
typealias GfVec2i = swiftusd.Vec2i
typealias GfVec3f = swiftusd.Vec3f
typealias GfVec3d = swiftusd.Vec3d
typealias GfVec3i = swiftusd.Vec3i
typealias GfVec4f = swiftusd.Vec4f
typealias GfVec4d = swiftusd.Vec4d
typealias GfMatrix4d = swiftusd.Matrix4d
typealias GfMatrix4f = swiftusd.Matrix4f
typealias GfQuatd = swiftusd.Quatd
typealias GfQuatf = swiftusd.Quatf
typealias GfRange3d = swiftusd.Range3d
typealias GfRange3f = swiftusd.Range3f
typealias GfBBox3d = swiftusd.BBox3d

final class GfCxxTests: XCTestCase {

    // MARK: - Vec2f Tests

    func testVec2fCreation() {
        let v = GfVec2f(1.0, 2.0)
        XCTAssertEqual(v.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 2.0, accuracy: 1e-6)
    }

    func testVec2fZero() {
        let v = GfVec2f.Zero()
        XCTAssertEqual(v.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 0.0, accuracy: 1e-6)
    }

    func testVec2fLength() {
        let v = GfVec2f(3.0, 4.0)
        XCTAssertEqual(v.GetLength(), 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.GetLengthSq(), 25.0, accuracy: 1e-6)
    }

    func testVec2fNormalize() {
        var v = GfVec2f(3.0, 4.0)
        let len = v.Normalize()
        XCTAssertEqual(len, 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.GetLength(), 1.0, accuracy: 1e-6)
    }

    func testVec2fArithmetic() {
        let v1 = GfVec2f(1.0, 2.0)
        let v2 = GfVec2f(3.0, 4.0)

        let sum = v1 + v2
        XCTAssertEqual(sum.x, 4.0, accuracy: 1e-6)
        XCTAssertEqual(sum.y, 6.0, accuracy: 1e-6)

        let diff = v2 - v1
        XCTAssertEqual(diff.x, 2.0, accuracy: 1e-6)
        XCTAssertEqual(diff.y, 2.0, accuracy: 1e-6)

        let scaled = v1 * 2.0
        XCTAssertEqual(scaled.x, 2.0, accuracy: 1e-6)
        XCTAssertEqual(scaled.y, 4.0, accuracy: 1e-6)
    }

    // MARK: - Vec3f Tests

    func testVec3fCreation() {
        let v = GfVec3f(1.0, 2.0, 3.0)
        XCTAssertEqual(v.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 2.0, accuracy: 1e-6)
        XCTAssertEqual(v.z, 3.0, accuracy: 1e-6)
    }

    func testVec3fStaticFactories() {
        let zero = GfVec3f.Zero()
        XCTAssertEqual(zero.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(zero.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(zero.z, 0.0, accuracy: 1e-6)

        let xAxis = GfVec3f.XAxis()
        XCTAssertEqual(xAxis.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(xAxis.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(xAxis.z, 0.0, accuracy: 1e-6)

        let yAxis = GfVec3f.YAxis()
        XCTAssertEqual(yAxis.y, 1.0, accuracy: 1e-6)

        let zAxis = GfVec3f.ZAxis()
        XCTAssertEqual(zAxis.z, 1.0, accuracy: 1e-6)
    }

    func testVec3fLength() {
        let v = GfVec3f(3.0, 4.0, 0.0)
        XCTAssertEqual(v.GetLength(), 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.GetLengthSq(), 25.0, accuracy: 1e-6)
    }

    func testVec3fNormalize() {
        let v = GfVec3f(3.0, 4.0, 0.0)
        let normalized = v.GetNormalized()
        XCTAssertEqual(normalized.GetLength(), 1.0, accuracy: 1e-6)
        XCTAssertEqual(normalized.x, 0.6, accuracy: 1e-6)
        XCTAssertEqual(normalized.y, 0.8, accuracy: 1e-6)
    }

    func testVec3fDotProduct() {
        let v1 = GfVec3f(1.0, 0.0, 0.0)
        let v2 = GfVec3f(0.0, 1.0, 0.0)
        XCTAssertEqual(v1.Dot(v2), 0.0, accuracy: 1e-6)

        let v3 = GfVec3f(1.0, 2.0, 3.0)
        let v4 = GfVec3f(4.0, 5.0, 6.0)
        XCTAssertEqual(v3.Dot(v4), 32.0, accuracy: 1e-6) // 1*4 + 2*5 + 3*6
    }

    func testVec3fCrossProduct() {
        let v1 = GfVec3f(1.0, 0.0, 0.0)
        let v2 = GfVec3f(0.0, 1.0, 0.0)
        let cross = v1.Cross(v2)
        XCTAssertEqual(cross.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(cross.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(cross.z, 1.0, accuracy: 1e-6)
    }

    func testVec3fArithmetic() {
        let v1 = GfVec3f(1.0, 2.0, 3.0)
        let v2 = GfVec3f(4.0, 5.0, 6.0)

        let sum = v1 + v2
        XCTAssertEqual(sum.x, 5.0, accuracy: 1e-6)
        XCTAssertEqual(sum.y, 7.0, accuracy: 1e-6)
        XCTAssertEqual(sum.z, 9.0, accuracy: 1e-6)

        let diff = v2 - v1
        XCTAssertEqual(diff.x, 3.0, accuracy: 1e-6)
        XCTAssertEqual(diff.y, 3.0, accuracy: 1e-6)
        XCTAssertEqual(diff.z, 3.0, accuracy: 1e-6)

        let scaled = v1 * 2.0
        XCTAssertEqual(scaled.x, 2.0, accuracy: 1e-6)
        XCTAssertEqual(scaled.y, 4.0, accuracy: 1e-6)
        XCTAssertEqual(scaled.z, 6.0, accuracy: 1e-6)
    }

    func testVec3fEquality() {
        let v1 = GfVec3f(1.0, 2.0, 3.0)
        let v2 = GfVec3f(1.0, 2.0, 3.0)
        let v3 = GfVec3f(1.0, 2.0, 4.0)

        XCTAssertEqual(v1, v2)
        XCTAssertNotEqual(v1, v3)
    }

    func testVec3fIsClose() {
        let v1 = GfVec3f(1.0, 2.0, 3.0)
        let v2 = GfVec3f(1.001, 2.001, 3.001)

        XCTAssertTrue(v1.isClose(to: v2, tolerance: 0.01))
        XCTAssertFalse(v1.isClose(to: v2, tolerance: 0.0001))
    }

    // MARK: - Vec3d Tests

    func testVec3dCreation() {
        let v = GfVec3d(1.0, 2.0, 3.0)
        XCTAssertEqual(v.x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(v.y, 2.0, accuracy: 1e-10)
        XCTAssertEqual(v.z, 3.0, accuracy: 1e-10)
    }

    func testVec3dLength() {
        let v = GfVec3d(3.0, 4.0, 0.0)
        XCTAssertEqual(v.GetLength(), 5.0, accuracy: 1e-10)
    }

    func testVec3dCrossProduct() {
        let v1 = GfVec3d(1.0, 0.0, 0.0)
        let v2 = GfVec3d(0.0, 1.0, 0.0)
        let cross = v1.Cross(v2)
        XCTAssertEqual(cross.z, 1.0, accuracy: 1e-10)
    }

    // MARK: - Vec4f Tests

    func testVec4fCreation() {
        let v = GfVec4f(1.0, 2.0, 3.0, 4.0)
        XCTAssertEqual(v.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(v.y, 2.0, accuracy: 1e-6)
        XCTAssertEqual(v.z, 3.0, accuracy: 1e-6)
        XCTAssertEqual(v.w, 4.0, accuracy: 1e-6)
    }

    func testVec4fGetXYZ() {
        let v = GfVec4f(1.0, 2.0, 3.0, 4.0)
        let xyz = v.GetXYZ()
        XCTAssertEqual(xyz.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(xyz.y, 2.0, accuracy: 1e-6)
        XCTAssertEqual(xyz.z, 3.0, accuracy: 1e-6)
    }

    // MARK: - Matrix4d Tests

    func testMatrix4dIdentity() {
        let m = GfMatrix4d.Identity()
        XCTAssertEqual(m.Get(0, 0), 1.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(1, 1), 1.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(2, 2), 1.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(3, 3), 1.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(0, 1), 0.0, accuracy: 1e-10)
    }

    func testMatrix4dTranslation() {
        let m = GfMatrix4d.translation(1.0, 2.0, 3.0)
        let t = m.ExtractTranslation()
        XCTAssertEqual(t.x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(t.y, 2.0, accuracy: 1e-10)
        XCTAssertEqual(t.z, 3.0, accuracy: 1e-10)
    }

    func testMatrix4dScale() {
        let m = GfMatrix4d.scale(2.0)
        XCTAssertEqual(m.Get(0, 0), 2.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(1, 1), 2.0, accuracy: 1e-10)
        XCTAssertEqual(m.Get(2, 2), 2.0, accuracy: 1e-10)
    }

    func testMatrix4dMultiplication() {
        let m1 = GfMatrix4d.translation(1.0, 0.0, 0.0)
        let m2 = GfMatrix4d.translation(0.0, 2.0, 0.0)

        let product = m1 * m2
        let t = product.ExtractTranslation()
        XCTAssertEqual(t.x, 1.0, accuracy: 1e-10)
        XCTAssertEqual(t.y, 2.0, accuracy: 1e-10)
    }

    func testMatrix4dDeterminant() {
        let identity = GfMatrix4d.Identity()
        XCTAssertEqual(identity.GetDeterminant(), 1.0, accuracy: 1e-10)

        let scale = GfMatrix4d.scale(2.0)
        XCTAssertEqual(scale.GetDeterminant(), 8.0, accuracy: 1e-10) // 2^3
    }

    func testMatrix4dTransform() {
        let translate = GfMatrix4d.translation(10.0, 20.0, 30.0)
        let point = GfVec3d(0.0, 0.0, 0.0)
        let result = translate.TransformPoint(point)
        XCTAssertEqual(result.x, 10.0, accuracy: 1e-10)
        XCTAssertEqual(result.y, 20.0, accuracy: 1e-10)
        XCTAssertEqual(result.z, 30.0, accuracy: 1e-10)
    }

    func testMatrix4dInverse() {
        let translate = GfMatrix4d.translation(5.0, 10.0, 15.0)
        let inverse = translate.GetInverse()
        let product = translate * inverse

        // Should be approximately identity
        XCTAssertTrue(product.isClose(to: GfMatrix4d.Identity(), tolerance: 1e-10))
    }

    func testMatrix4dTranspose() {
        let m = GfMatrix4d(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        )
        let t = m.GetTranspose()
        XCTAssertEqual(t.Get(0, 1), 5.0, accuracy: 1e-10)
        XCTAssertEqual(t.Get(1, 0), 2.0, accuracy: 1e-10)
    }

    // MARK: - Quatd Tests

    func testQuatdIdentity() {
        let q = GfQuatd.Identity()
        XCTAssertEqual(q.real, 1.0, accuracy: 1e-10)
        XCTAssertEqual(q.i, 0.0, accuracy: 1e-10)
        XCTAssertEqual(q.j, 0.0, accuracy: 1e-10)
        XCTAssertEqual(q.k, 0.0, accuracy: 1e-10)
    }

    func testQuatdLength() {
        let q = GfQuatd(1.0, 0.0, 0.0, 0.0)
        XCTAssertEqual(q.GetLength(), 1.0, accuracy: 1e-10)
    }

    func testQuatdNormalize() {
        let q = GfQuatd(2.0, 0.0, 0.0, 0.0)
        let normalized = q.GetNormalized()
        XCTAssertEqual(normalized.GetLength(), 1.0, accuracy: 1e-10)
    }

    func testQuatdConjugate() {
        let q = GfQuatd(1.0, 2.0, 3.0, 4.0)
        let conj = q.GetConjugate()
        XCTAssertEqual(conj.real, 1.0, accuracy: 1e-10)
        XCTAssertEqual(conj.i, -2.0, accuracy: 1e-10)
        XCTAssertEqual(conj.j, -3.0, accuracy: 1e-10)
        XCTAssertEqual(conj.k, -4.0, accuracy: 1e-10)
    }

    func testQuatdMultiplication() {
        let q1 = GfQuatd.Identity()
        let q2 = GfQuatd(0.0, 1.0, 0.0, 0.0)

        let product = q1 * q2
        XCTAssertEqual(product.real, 0.0, accuracy: 1e-10)
        XCTAssertEqual(product.i, 1.0, accuracy: 1e-10)
    }

    func testQuatdSlerp() {
        let q1 = GfQuatd.Identity()
        let q2 = GfQuatd(0.0, 0.0, 0.0, 1.0)

        let mid = GfQuatd.slerp(from: q1, to: q2, t: 0.5)
        XCTAssertEqual(mid.GetLength(), 1.0, accuracy: 0.01)
    }

    func testQuatdFromAxisAngle() {
        let axis = GfVec3d(0.0, 1.0, 0.0)
        let q = GfQuatd.fromAxisAngle(axis: axis, radians: .pi / 2)
        XCTAssertEqual(q.GetLength(), 1.0, accuracy: 1e-10)
    }

    func testQuatdTransform() {
        // Rotate 90 degrees around Y axis
        let axis = GfVec3d(0.0, 1.0, 0.0)
        let q = GfQuatd.fromAxisAngle(axis: axis, radians: .pi / 2)

        let point = GfVec3d(1.0, 0.0, 0.0)
        let rotated = q.Transform(point)

        // X axis should become Z axis after 90 degree rotation around Y
        XCTAssertEqual(rotated.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(rotated.y, 0.0, accuracy: 1e-10)
        XCTAssertEqual(rotated.z, -1.0, accuracy: 1e-10)
    }

    // MARK: - Range3d Tests

    func testRange3dCreation() {
        let r = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 2, maxZ: 3)
        XCTAssertEqual(r.min.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(r.max.z, 3.0, accuracy: 1e-10)
    }

    func testRange3dEmpty() {
        let r = GfRange3d.Empty()
        XCTAssertTrue(r.IsEmpty())
    }

    func testRange3dUnitCube() {
        let r = GfRange3d.UnitCube()
        XCTAssertEqual(r.min.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(r.max.x, 1.0, accuracy: 1e-10)
    }

    func testRange3dContainsPoint() {
        let r = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 10, maxY: 10, maxZ: 10)
        XCTAssertTrue(r.Contains(GfVec3d(5.0, 5.0, 5.0)))
        XCTAssertFalse(r.Contains(GfVec3d(15.0, 5.0, 5.0)))
    }

    func testRange3dSize() {
        let r = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 2, maxY: 3, maxZ: 4)
        let size = r.GetSize()
        XCTAssertEqual(size.x, 2.0, accuracy: 1e-10)
        XCTAssertEqual(size.y, 3.0, accuracy: 1e-10)
        XCTAssertEqual(size.z, 4.0, accuracy: 1e-10)
    }

    func testRange3dMidpoint() {
        let r = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 10, maxY: 10, maxZ: 10)
        let mid = r.GetMidpoint()
        XCTAssertEqual(mid.x, 5.0, accuracy: 1e-10)
        XCTAssertEqual(mid.y, 5.0, accuracy: 1e-10)
        XCTAssertEqual(mid.z, 5.0, accuracy: 1e-10)
    }

    func testRange3dUnion() {
        let r1 = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let r2 = GfRange3d(minX: 2, minY: 2, minZ: 2, maxX: 3, maxY: 3, maxZ: 3)
        let u = GfRange3d.union(r1, r2)
        XCTAssertEqual(u.min.x, 0.0, accuracy: 1e-10)
        XCTAssertEqual(u.max.x, 3.0, accuracy: 1e-10)
    }

    func testRange3dCorner() {
        let r = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let corner0 = r.GetCorner(0) // min corner
        XCTAssertEqual(corner0.x, 0.0, accuracy: 1e-10)
        let corner7 = r.GetCorner(7) // max corner
        XCTAssertEqual(corner7.x, 1.0, accuracy: 1e-10)
    }

    // MARK: - BBox3d Tests

    func testBBox3dCreation() {
        let bbox = GfBBox3d()
        XCTAssertNotNil(bbox)
    }

    func testBBox3dFromRange() {
        let range = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let bbox = GfBBox3d(range: range)
        XCTAssertFalse(bbox.GetRange().IsEmpty())
    }

    func testBBox3dVolume() {
        let range = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 2, maxY: 3, maxZ: 4)
        let bbox = GfBBox3d(range: range)
        XCTAssertEqual(bbox.GetVolume(), 24.0, accuracy: 1e-10) // 2*3*4
    }

    func testBBox3dCentroid() {
        let range = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 10, maxY: 10, maxZ: 10)
        let bbox = GfBBox3d(range: range)
        let c = bbox.ComputeCentroid()
        XCTAssertEqual(c.x, 5.0, accuracy: 1e-10)
        XCTAssertEqual(c.y, 5.0, accuracy: 1e-10)
        XCTAssertEqual(c.z, 5.0, accuracy: 1e-10)
    }

    func testBBox3dCombine() {
        let r1 = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        let r2 = GfRange3d(minX: 2, minY: 2, minZ: 2, maxX: 3, maxY: 3, maxZ: 3)
        let b1 = GfBBox3d(range: r1)
        let b2 = GfBBox3d(range: r2)
        let combined = GfBBox3d.combine(b1, b2)
        XCTAssertFalse(combined.GetRange().IsEmpty())
    }

    func testBBox3dTransform() {
        let range = GfRange3d(minX: 0, minY: 0, minZ: 0, maxX: 1, maxY: 1, maxZ: 1)
        var bbox = GfBBox3d(range: range)
        let translation = GfMatrix4d.translation(10.0, 0.0, 0.0)
        bbox.Transform(translation)

        let centroid = bbox.ComputeCentroid()
        XCTAssertEqual(centroid.x, 10.5, accuracy: 1e-10) // 0.5 + 10
    }

    // MARK: - Utility Function Tests

    func testLerp() {
        XCTAssertEqual(Gf.lerp(0.0, 10.0, 0.5), 5.0, accuracy: 1e-10)
        XCTAssertEqual(Gf.lerp(0.0, 10.0, 0.0), 0.0, accuracy: 1e-10)
        XCTAssertEqual(Gf.lerp(0.0, 10.0, 1.0), 10.0, accuracy: 1e-10)
    }

    func testClamp() {
        XCTAssertEqual(Gf.clamp(5.0, min: 0.0, max: 10.0), 5.0, accuracy: 1e-10)
        XCTAssertEqual(Gf.clamp(-5.0, min: 0.0, max: 10.0), 0.0, accuracy: 1e-10)
        XCTAssertEqual(Gf.clamp(15.0, min: 0.0, max: 10.0), 10.0, accuracy: 1e-10)
    }

    func testDegreesRadians() {
        XCTAssertEqual(Gf.degreesToRadians(180.0), .pi, accuracy: 1e-10)
        XCTAssertEqual(Gf.radiansToDegrees(.pi), 180.0, accuracy: 1e-10)
    }

    func testIsClose() {
        XCTAssertTrue(Gf.isClose(1.0, 1.0 + 1e-11))
        XCTAssertFalse(Gf.isClose(1.0, 1.0 + 1e-9))
    }

    // MARK: - Hashable Tests

    func testVec3fHashable() {
        let v1 = GfVec3f(1.0, 2.0, 3.0)
        let v2 = GfVec3f(1.0, 2.0, 3.0)
        let v3 = GfVec3f(4.0, 5.0, 6.0)

        var set = Set<GfVec3f>()
        set.insert(v1)
        set.insert(v2) // Should not increase count
        set.insert(v3)

        XCTAssertEqual(set.count, 2)
    }

    func testQuatdHashable() {
        let q1 = GfQuatd(1.0, 0.0, 0.0, 0.0)
        let q2 = GfQuatd(1.0, 0.0, 0.0, 0.0)

        var set = Set<GfQuatd>()
        set.insert(q1)
        set.insert(q2)

        XCTAssertEqual(set.count, 1)
    }

    // MARK: - CustomStringConvertible Tests

    func testVec3fDescription() {
        let v = GfVec3f(1.0, 2.0, 3.0)
        let desc = v.description
        XCTAssertTrue(desc.contains("Vec3f"))
        XCTAssertTrue(desc.contains("1"))
        XCTAssertTrue(desc.contains("2"))
        XCTAssertTrue(desc.contains("3"))
    }

    func testQuatdDescription() {
        let q = GfQuatd(1.0, 2.0, 3.0, 4.0)
        let desc = q.description
        XCTAssertTrue(desc.contains("Quatd"))
        XCTAssertTrue(desc.contains("real"))
    }
}
