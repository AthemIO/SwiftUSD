// GfTests.swift - Tests for Gf module (Graphics Foundation)

import XCTest
import SwiftUSD
@testable import CxxFacade

final class GfTests: XCTestCase {

    // MARK: - Vec2f Tests

    func testVec2fCreation() {
        let v = cxxfacade.gf.Vec2f(1.0, 2.0)
        XCTAssertEqual(v.x, 1.0)
        XCTAssertEqual(v.y, 2.0)
    }

    func testVec2fDefaultConstructor() {
        let v = cxxfacade.gf.Vec2f()
        XCTAssertEqual(v.x, 0.0)
        XCTAssertEqual(v.y, 0.0)
    }

    func testVec2fArithmetic() {
        let a = cxxfacade.gf.Vec2f(1.0, 2.0)
        let b = cxxfacade.gf.Vec2f(3.0, 4.0)

        let sum = a + b
        XCTAssertEqual(sum.x, 4.0)
        XCTAssertEqual(sum.y, 6.0)

        let diff = b - a
        XCTAssertEqual(diff.x, 2.0)
        XCTAssertEqual(diff.y, 2.0)

        let scaled = a * 2.0
        XCTAssertEqual(scaled.x, 2.0)
        XCTAssertEqual(scaled.y, 4.0)
    }

    func testVec2fDot() {
        let a = cxxfacade.gf.Vec2f(1.0, 0.0)
        let b = cxxfacade.gf.Vec2f(0.0, 1.0)
        XCTAssertEqual(a.Dot(b), 0.0)

        let c = cxxfacade.gf.Vec2f(1.0, 2.0)
        let d = cxxfacade.gf.Vec2f(3.0, 4.0)
        XCTAssertEqual(c.Dot(d), 11.0)  // 1*3 + 2*4
    }

    func testVec2fLength() {
        let v = cxxfacade.gf.Vec2f(3.0, 4.0)
        XCTAssertEqual(v.GetLength(), 5.0, accuracy: 1e-6)
        XCTAssertEqual(v.GetLengthSq(), 25.0)
    }

    func testVec2fNormalize() {
        let v = cxxfacade.gf.Vec2f(3.0, 4.0)
        let n = v.GetNormalized()
        XCTAssertEqual(n.GetLength(), 1.0, accuracy: 1e-6)
        XCTAssertEqual(n.x, 0.6, accuracy: 1e-6)
        XCTAssertEqual(n.y, 0.8, accuracy: 1e-6)
    }

    // MARK: - Vec3f Tests

    func testVec3fCreation() {
        let v = cxxfacade.gf.Vec3f(1.0, 2.0, 3.0)
        XCTAssertEqual(v.x, 1.0)
        XCTAssertEqual(v.y, 2.0)
        XCTAssertEqual(v.z, 3.0)
    }

    func testVec3fDefaultConstructor() {
        let v = cxxfacade.gf.Vec3f()
        XCTAssertEqual(v.x, 0.0)
        XCTAssertEqual(v.y, 0.0)
        XCTAssertEqual(v.z, 0.0)
    }

    func testVec3fArithmetic() {
        let a = cxxfacade.gf.Vec3f(1.0, 2.0, 3.0)
        let b = cxxfacade.gf.Vec3f(4.0, 5.0, 6.0)

        let sum = a + b
        XCTAssertEqual(sum.x, 5.0)
        XCTAssertEqual(sum.y, 7.0)
        XCTAssertEqual(sum.z, 9.0)

        let diff = b - a
        XCTAssertEqual(diff.x, 3.0)
        XCTAssertEqual(diff.y, 3.0)
        XCTAssertEqual(diff.z, 3.0)

        let scaled = a * 2.0
        XCTAssertEqual(scaled.x, 2.0)
        XCTAssertEqual(scaled.y, 4.0)
        XCTAssertEqual(scaled.z, 6.0)
    }

    func testVec3fCross() {
        let x = cxxfacade.gf.Vec3f(1.0, 0.0, 0.0)
        let y = cxxfacade.gf.Vec3f(0.0, 1.0, 0.0)
        let z = x.Cross(y)
        XCTAssertEqual(z.x, 0.0, accuracy: 1e-6)
        XCTAssertEqual(z.y, 0.0, accuracy: 1e-6)
        XCTAssertEqual(z.z, 1.0, accuracy: 1e-6)
    }

    func testVec3fDot() {
        let a = cxxfacade.gf.Vec3f(1.0, 2.0, 3.0)
        let b = cxxfacade.gf.Vec3f(4.0, 5.0, 6.0)
        XCTAssertEqual(a.Dot(b), 32.0)  // 1*4 + 2*5 + 3*6
    }

    func testVec3fLength() {
        let v = cxxfacade.gf.Vec3f(1.0, 2.0, 2.0)
        XCTAssertEqual(v.GetLength(), 3.0, accuracy: 1e-6)
    }

    func testVec3fNormalize() {
        let v = cxxfacade.gf.Vec3f(1.0, 2.0, 2.0)
        let n = v.GetNormalized()
        XCTAssertEqual(n.GetLength(), 1.0, accuracy: 1e-6)
    }

    func testVec3fStaticConstructors() {
        let x = cxxfacade.gf.Vec3f.XAxis()
        XCTAssertEqual(x.x, 1.0)
        XCTAssertEqual(x.y, 0.0)
        XCTAssertEqual(x.z, 0.0)

        let zero = cxxfacade.gf.Vec3f.Zero()
        XCTAssertEqual(zero.x, 0.0)
        XCTAssertEqual(zero.y, 0.0)
        XCTAssertEqual(zero.z, 0.0)
    }

    // MARK: - Vec3d Tests

    func testVec3dCreation() {
        let v = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        XCTAssertEqual(v.x, 1.0)
        XCTAssertEqual(v.y, 2.0)
        XCTAssertEqual(v.z, 3.0)
    }

    func testVec3dArithmetic() {
        let a = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        let b = cxxfacade.gf.Vec3d(4.0, 5.0, 6.0)

        let sum = a + b
        XCTAssertEqual(sum.x, 5.0)
        XCTAssertEqual(sum.y, 7.0)
        XCTAssertEqual(sum.z, 9.0)
    }

    func testVec3dCross() {
        let x = cxxfacade.gf.Vec3d(1.0, 0.0, 0.0)
        let y = cxxfacade.gf.Vec3d(0.0, 1.0, 0.0)
        let z = x.Cross(y)
        XCTAssertEqual(z.z, 1.0, accuracy: 1e-12)
    }

    // MARK: - Vec4f Tests

    func testVec4fCreation() {
        let v = cxxfacade.gf.Vec4f(1.0, 2.0, 3.0, 4.0)
        XCTAssertEqual(v.x, 1.0)
        XCTAssertEqual(v.y, 2.0)
        XCTAssertEqual(v.z, 3.0)
        XCTAssertEqual(v.w, 4.0)
    }

    func testVec4fFromVec3() {
        let v3 = cxxfacade.gf.Vec3f(1.0, 2.0, 3.0)
        let v4 = cxxfacade.gf.Vec4f(v3, 1.0)
        XCTAssertEqual(v4.x, 1.0)
        XCTAssertEqual(v4.y, 2.0)
        XCTAssertEqual(v4.z, 3.0)
        XCTAssertEqual(v4.w, 1.0)
    }

    func testVec4fGetVec3() {
        let v4 = cxxfacade.gf.Vec4f(1.0, 2.0, 3.0, 4.0)
        let v3 = v4.GetVec3()
        XCTAssertEqual(v3.x, 1.0)
        XCTAssertEqual(v3.y, 2.0)
        XCTAssertEqual(v3.z, 3.0)
    }

    // MARK: - Matrix4f Tests

    func testMatrix4fIdentity() {
        let m = cxxfacade.gf.Matrix4f.Identity()
        XCTAssertEqual(m.data.0.0, 1.0)
        XCTAssertEqual(m.data.1.1, 1.0)
        XCTAssertEqual(m.data.2.2, 1.0)
        XCTAssertEqual(m.data.3.3, 1.0)
        XCTAssertEqual(m.data.0.1, 0.0)
        XCTAssertEqual(m.data.1.0, 0.0)
    }

    func testMatrix4fTranslation() {
        let t = cxxfacade.gf.Vec3f(1.0, 2.0, 3.0)
        let m = cxxfacade.gf.Matrix4f.FromTranslation(t)

        let p = cxxfacade.gf.Vec3f(0.0, 0.0, 0.0)
        let result = m.TransformPoint(p)

        XCTAssertEqual(result.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(result.y, 2.0, accuracy: 1e-6)
        XCTAssertEqual(result.z, 3.0, accuracy: 1e-6)
    }

    func testMatrix4fScale() {
        let s = cxxfacade.gf.Vec3f(2.0, 3.0, 4.0)
        let m = cxxfacade.gf.Matrix4f.FromScale(s)

        let p = cxxfacade.gf.Vec3f(1.0, 1.0, 1.0)
        let result = m.TransformPoint(p)

        XCTAssertEqual(result.x, 2.0, accuracy: 1e-6)
        XCTAssertEqual(result.y, 3.0, accuracy: 1e-6)
        XCTAssertEqual(result.z, 4.0, accuracy: 1e-6)
    }

    func testMatrix4fMultiplication() {
        let a = cxxfacade.gf.Matrix4f.FromTranslation(cxxfacade.gf.Vec3f(1.0, 0.0, 0.0))
        let b = cxxfacade.gf.Matrix4f.FromTranslation(cxxfacade.gf.Vec3f(0.0, 1.0, 0.0))
        let c = a * b

        let p = cxxfacade.gf.Vec3f(0.0, 0.0, 0.0)
        let result = c.TransformPoint(p)

        XCTAssertEqual(result.x, 1.0, accuracy: 1e-6)
        XCTAssertEqual(result.y, 1.0, accuracy: 1e-6)
    }

    func testMatrix4fInverse() {
        let t = cxxfacade.gf.Vec3f(1.0, 2.0, 3.0)
        let m = cxxfacade.gf.Matrix4f.FromTranslation(t)
        let inv = m.GetInverse()
        let identity = m * inv

        XCTAssertEqual(identity.data.0.0, 1.0, accuracy: 1e-5)
        XCTAssertEqual(identity.data.1.1, 1.0, accuracy: 1e-5)
        XCTAssertEqual(identity.data.2.2, 1.0, accuracy: 1e-5)
        XCTAssertEqual(identity.data.3.3, 1.0, accuracy: 1e-5)
    }

    func testMatrix4fTranspose() {
        var m = cxxfacade.gf.Matrix4f()
        m.data.0.1 = 5.0
        let t = m.GetTranspose()
        XCTAssertEqual(t.data.1.0, 5.0)
    }

    // MARK: - Matrix4d Tests

    func testMatrix4dIdentity() {
        let m = cxxfacade.gf.Matrix4d.Identity()
        XCTAssertEqual(m.data.0.0, 1.0)
        XCTAssertEqual(m.data.1.1, 1.0)
        XCTAssertEqual(m.data.2.2, 1.0)
        XCTAssertEqual(m.data.3.3, 1.0)
    }

    func testMatrix4dTranslation() {
        let t = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        let m = cxxfacade.gf.Matrix4d.FromTranslation(t)

        let p = cxxfacade.gf.Vec3d(0.0, 0.0, 0.0)
        let result = m.TransformPoint(p)

        XCTAssertEqual(result.x, 1.0, accuracy: 1e-12)
        XCTAssertEqual(result.y, 2.0, accuracy: 1e-12)
        XCTAssertEqual(result.z, 3.0, accuracy: 1e-12)
    }

    func testMatrix4dToFloat() {
        let md = cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(1.0, 2.0, 3.0))
        let mf = md.ToFloat()
        XCTAssertEqual(mf.data.3.0, 1.0, accuracy: 1e-5)
        XCTAssertEqual(mf.data.3.1, 2.0, accuracy: 1e-5)
        XCTAssertEqual(mf.data.3.2, 3.0, accuracy: 1e-5)
    }

    // MARK: - Quatf Tests

    func testQuatfIdentity() {
        let q = cxxfacade.gf.Quatf.Identity()
        XCTAssertEqual(q.w, 1.0)
        XCTAssertEqual(q.x, 0.0)
        XCTAssertEqual(q.y, 0.0)
        XCTAssertEqual(q.z, 0.0)
    }

    func testQuatfFromAxisAngle() {
        let axis = cxxfacade.gf.Vec3f(0.0, 0.0, 1.0)
        let angle: Float = .pi / 2  // 90 degrees
        let q = cxxfacade.gf.Quatf.FromAxisAngle(axis, angle)

        XCTAssertEqual(q.GetLength(), 1.0, accuracy: 1e-6)
    }

    func testQuatfRotateVector() {
        let axis = cxxfacade.gf.Vec3f(0.0, 0.0, 1.0)
        let angle: Float = .pi / 2  // 90 degrees around Z
        let q = cxxfacade.gf.Quatf.FromAxisAngle(axis, angle)

        let v = cxxfacade.gf.Vec3f(1.0, 0.0, 0.0)  // X axis
        let rotated = q.RotateVector(v)

        // X axis rotated 90 degrees around Z should be Y axis
        XCTAssertEqual(rotated.x, 0.0, accuracy: 1e-5)
        XCTAssertEqual(rotated.y, 1.0, accuracy: 1e-5)
        XCTAssertEqual(rotated.z, 0.0, accuracy: 1e-5)
    }

    func testQuatfMultiplication() {
        let axis = cxxfacade.gf.Vec3f(0.0, 0.0, 1.0)
        let q1 = cxxfacade.gf.Quatf.FromAxisAngle(axis, Float.pi / 4)  // 45 degrees
        let q2 = cxxfacade.gf.Quatf.FromAxisAngle(axis, Float.pi / 4)  // 45 degrees
        let q3 = q1 * q2  // Should be 90 degrees

        let v = cxxfacade.gf.Vec3f(1.0, 0.0, 0.0)
        let rotated = q3.RotateVector(v)

        XCTAssertEqual(rotated.x, 0.0, accuracy: 1e-5)
        XCTAssertEqual(rotated.y, 1.0, accuracy: 1e-5)
    }

    func testQuatfToMatrix() {
        let axis = cxxfacade.gf.Vec3f(0.0, 0.0, 1.0)
        let q = cxxfacade.gf.Quatf.FromAxisAngle(axis, Float.pi / 2)
        let m = q.ToMatrix()

        let v = cxxfacade.gf.Vec3f(1.0, 0.0, 0.0)
        let rotated = m.TransformDirection(v)

        XCTAssertEqual(rotated.x, 0.0, accuracy: 1e-5)
        XCTAssertEqual(rotated.y, 1.0, accuracy: 1e-5)
    }

    func testQuatfSlerp() {
        let axis = cxxfacade.gf.Vec3f(0.0, 0.0, 1.0)
        let q0 = cxxfacade.gf.Quatf.Identity()
        let q1 = cxxfacade.gf.Quatf.FromAxisAngle(axis, Float.pi / 2)

        let qHalf = cxxfacade.gf.Quatf.Slerp(q0, q1, 0.5)
        let v = cxxfacade.gf.Vec3f(1.0, 0.0, 0.0)
        let rotated = qHalf.RotateVector(v)

        // At t=0.5, should be 45 degrees
        let expected = Float.pi / 4
        XCTAssertEqual(rotated.x, cos(expected), accuracy: 1e-5)
        XCTAssertEqual(rotated.y, sin(expected), accuracy: 1e-5)
    }

    // MARK: - Quatd Tests

    func testQuatdIdentity() {
        let q = cxxfacade.gf.Quatd.Identity()
        XCTAssertEqual(q.w, 1.0)
        XCTAssertEqual(q.x, 0.0)
        XCTAssertEqual(q.y, 0.0)
        XCTAssertEqual(q.z, 0.0)
    }

    func testQuatdFromAxisAngle() {
        let axis = cxxfacade.gf.Vec3d(0.0, 0.0, 1.0)
        let angle: Double = .pi / 2
        let q = cxxfacade.gf.Quatd.FromAxisAngle(axis, angle)

        XCTAssertEqual(q.GetLength(), 1.0, accuracy: 1e-12)
    }

    func testQuatdToFloat() {
        let qd = cxxfacade.gf.Quatd(1.0, 0.0, 0.0, 0.0)
        let qf = qd.ToFloat()
        XCTAssertEqual(qf.w, 1.0)
    }

    // MARK: - Range3d Tests

    func testRange3dEmpty() {
        let r = cxxfacade.gf.Range3d()
        XCTAssertTrue(r.IsEmpty())
    }

    func testRange3dFromMinMax() {
        let min = cxxfacade.gf.Vec3d(0.0, 0.0, 0.0)
        let max = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        let r = cxxfacade.gf.Range3d(min, max)

        XCTAssertFalse(r.IsEmpty())
        XCTAssertEqual(r.min.x, 0.0)
        XCTAssertEqual(r.max.x, 1.0)
    }

    func testRange3dSize() {
        let min = cxxfacade.gf.Vec3d(0.0, 0.0, 0.0)
        let max = cxxfacade.gf.Vec3d(1.0, 2.0, 3.0)
        let r = cxxfacade.gf.Range3d(min, max)
        let size = r.GetSize()

        XCTAssertEqual(size.x, 1.0)
        XCTAssertEqual(size.y, 2.0)
        XCTAssertEqual(size.z, 3.0)
    }

    func testRange3dMidpoint() {
        let min = cxxfacade.gf.Vec3d(0.0, 0.0, 0.0)
        let max = cxxfacade.gf.Vec3d(2.0, 4.0, 6.0)
        let r = cxxfacade.gf.Range3d(min, max)
        let mid = r.GetMidpoint()

        XCTAssertEqual(mid.x, 1.0)
        XCTAssertEqual(mid.y, 2.0)
        XCTAssertEqual(mid.z, 3.0)
    }

    func testRange3dContainsPoint() {
        let min = cxxfacade.gf.Vec3d(0.0, 0.0, 0.0)
        let max = cxxfacade.gf.Vec3d(1.0, 1.0, 1.0)
        var r = cxxfacade.gf.Range3d(min, max)

        let inside = cxxfacade.gf.Vec3d(0.5, 0.5, 0.5)
        let outside = cxxfacade.gf.Vec3d(2.0, 2.0, 2.0)

        XCTAssertTrue(r.Contains(inside))
        XCTAssertFalse(r.Contains(outside))
    }

    func testRange3dUnion() {
        let r1 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(1.0, 1.0, 1.0))
        let r2 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.5, 0.5, 0.5), cxxfacade.gf.Vec3d(2.0, 2.0, 2.0))
        let u = r1.GetUnion(r2)

        XCTAssertEqual(u.min.x, 0.0)
        XCTAssertEqual(u.max.x, 2.0)
    }

    func testRange3dIntersection() {
        let r1 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(1.0, 1.0, 1.0))
        let r2 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.5, 0.5, 0.5), cxxfacade.gf.Vec3d(2.0, 2.0, 2.0))

        XCTAssertTrue(r1.Intersects(r2))

        let inter = r1.GetIntersection(r2)
        XCTAssertEqual(inter.min.x, 0.5)
        XCTAssertEqual(inter.max.x, 1.0)
    }

    func testRange3dNoIntersection() {
        let r1 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(1.0, 1.0, 1.0))
        let r2 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(2.0, 2.0, 2.0), cxxfacade.gf.Vec3d(3.0, 3.0, 3.0))

        XCTAssertFalse(r1.Intersects(r2))
    }

    // MARK: - BBox3d Tests

    func testBBox3dEmpty() {
        let b = cxxfacade.gf.BBox3d()
        XCTAssertTrue(b.IsEmpty())
    }

    func testBBox3dFromRange() {
        let range = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(1.0, 2.0, 3.0))
        let b = cxxfacade.gf.BBox3d(range)

        XCTAssertFalse(b.IsEmpty())
        XCTAssertEqual(b.GetVolume(), 6.0, accuracy: 1e-12)  // 1 * 2 * 3
    }

    func testBBox3dCentroid() {
        let range = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(2.0, 4.0, 6.0))
        let b = cxxfacade.gf.BBox3d(range)
        let c = b.GetCentroid()

        XCTAssertEqual(c.x, 1.0)
        XCTAssertEqual(c.y, 2.0)
        XCTAssertEqual(c.z, 3.0)
    }

    func testBBox3dWithTransform() {
        let range = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(1.0, 1.0, 1.0))
        let matrix = cxxfacade.gf.Matrix4d.FromTranslation(cxxfacade.gf.Vec3d(10.0, 0.0, 0.0))
        let b = cxxfacade.gf.BBox3d(range, matrix)

        let aligned = b.ComputeAlignedRange()
        XCTAssertEqual(aligned.min.x, 10.0, accuracy: 1e-12)
        XCTAssertEqual(aligned.max.x, 11.0, accuracy: 1e-12)
    }

    func testBBox3dCombine() {
        let r1 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(0.0, 0.0, 0.0), cxxfacade.gf.Vec3d(1.0, 1.0, 1.0))
        let r2 = cxxfacade.gf.Range3d(cxxfacade.gf.Vec3d(2.0, 2.0, 2.0), cxxfacade.gf.Vec3d(3.0, 3.0, 3.0))
        let b1 = cxxfacade.gf.BBox3d(r1)
        let b2 = cxxfacade.gf.BBox3d(r2)

        let combined = cxxfacade.gf.BBox3d.Combine(b1, b2)
        let aligned = combined.ComputeAlignedRange()

        XCTAssertEqual(aligned.min.x, 0.0, accuracy: 1e-12)
        XCTAssertEqual(aligned.max.x, 3.0, accuracy: 1e-12)
    }

    // MARK: - Swift Wrapper Tests

    func testSwiftVec3dExtensions() {
        let v = Vec3d(1.0, 2.0, 3.0)
        XCTAssertEqual(v.description, "Vec3d(1.0, 2.0, 3.0)")
        XCTAssertTrue(v.lengthSquared > 0)
    }

    func testSwiftMatrix4dExtensions() {
        let m = Matrix4d.identity
        XCTAssertEqual(m.determinant, 1.0, accuracy: 1e-12)

        let t = Matrix4d.translation(Vec3d(1.0, 2.0, 3.0))
        let p = t.transform(point: Vec3d.zero)
        XCTAssertEqual(p.x, 1.0)
        XCTAssertEqual(p.y, 2.0)
        XCTAssertEqual(p.z, 3.0)
    }

    func testSwiftQuatdExtensions() {
        let q = Quatd.identity
        XCTAssertEqual(q.length, 1.0)

        let axis = Vec3d.zAxis
        let rotation = Quatd.axisAngle(axis, radians: .pi / 2)
        XCTAssertEqual(rotation.length, 1.0, accuracy: 1e-12)
    }

    func testSwiftRange3dExtensions() {
        let r = Range3d.empty
        XCTAssertTrue(r.isEmpty)
    }

    func testGfMathConversions() {
        let degrees: Double = 90.0
        let radians = GfMath.radians(fromDegrees: degrees)
        XCTAssertEqual(radians, .pi / 2, accuracy: 1e-12)

        let backToDegrees = GfMath.degrees(fromRadians: radians)
        XCTAssertEqual(backToDegrees, 90.0, accuracy: 1e-12)
    }
}
