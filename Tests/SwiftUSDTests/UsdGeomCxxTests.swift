// UsdGeomCxxTests.swift - Tests for UsdGeom C++ interop types

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdGeomCxxTests: XCTestCase {

    // MARK: - Enum Tests

    func testGeomInterpolationValues() {
        let constant = swiftusd.GeomInterpolation.constant
        let uniform = swiftusd.GeomInterpolation.uniform
        let varying = swiftusd.GeomInterpolation.varying
        let vertex = swiftusd.GeomInterpolation.vertex
        let faceVarying = swiftusd.GeomInterpolation.faceVarying

        XCTAssertEqual(constant.value.rawValue, 0)
        XCTAssertEqual(uniform.value.rawValue, 1)
        XCTAssertEqual(varying.value.rawValue, 2)
        XCTAssertEqual(vertex.value.rawValue, 3)
        XCTAssertEqual(faceVarying.value.rawValue, 4)
    }

    func testSubdivisionSchemeValues() {
        let none = swiftusd.SubdivisionScheme.none
        let catmullClark = swiftusd.SubdivisionScheme.catmullClark
        let loop = swiftusd.SubdivisionScheme.loop
        let bilinear = swiftusd.SubdivisionScheme.bilinear

        XCTAssertEqual(none.value.rawValue, 0)
        XCTAssertEqual(catmullClark.value.rawValue, 1)
        XCTAssertEqual(loop.value.rawValue, 2)
        XCTAssertEqual(bilinear.value.rawValue, 3)
    }

    func testGeomAxisValues() {
        let x = swiftusd.GeomAxis.x
        let y = swiftusd.GeomAxis.y
        let z = swiftusd.GeomAxis.z

        XCTAssertEqual(x.value.rawValue, 0)
        XCTAssertEqual(y.value.rawValue, 1)
        XCTAssertEqual(z.value.rawValue, 2)
    }

    func testGeomPurposeValues() {
        let defaultPurpose = swiftusd.GeomPurpose.defaultPurpose
        let render = swiftusd.GeomPurpose.render
        let proxy = swiftusd.GeomPurpose.proxy
        let guide = swiftusd.GeomPurpose.guide

        XCTAssertEqual(defaultPurpose.value.rawValue, 0)
        XCTAssertEqual(render.value.rawValue, 1)
        XCTAssertEqual(proxy.value.rawValue, 2)
        XCTAssertEqual(guide.value.rawValue, 3)
    }

    func testCurveTypeValues() {
        let linear = swiftusd.CurveType.linear
        let cubic = swiftusd.CurveType.cubic

        XCTAssertEqual(linear.value.rawValue, 0)
        XCTAssertEqual(cubic.value.rawValue, 1)
    }

    func testCurveBasisValues() {
        let bezier = swiftusd.CurveBasis.bezier
        let bspline = swiftusd.CurveBasis.bspline
        let catmullRom = swiftusd.CurveBasis.catmullRom

        XCTAssertEqual(bezier.value.rawValue, 0)
        XCTAssertEqual(bspline.value.rawValue, 1)
        XCTAssertEqual(catmullRom.value.rawValue, 2)
    }

    func testCurveWrapValues() {
        let nonperiodic = swiftusd.CurveWrap.nonperiodic
        let periodic = swiftusd.CurveWrap.periodic
        let pinned = swiftusd.CurveWrap.pinned

        XCTAssertEqual(nonperiodic.value.rawValue, 0)
        XCTAssertEqual(periodic.value.rawValue, 1)
        XCTAssertEqual(pinned.value.rawValue, 2)
    }

    // MARK: - GeomXformRef Tests

    func testGeomXformDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Xform")
        guard let xform = swiftusd.GeomXformRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Xform")
            return
        }

        XCTAssertTrue(xform.IsValid())
    }

    func testGeomXformTransformOps() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Xform")
        guard let xform = swiftusd.GeomXformRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Xform")
            return
        }

        XCTAssertTrue(xform.AddTranslateOp(10, 20, 30))
        XCTAssertTrue(xform.AddRotateXYZOp(0, 45, 0))
        XCTAssertTrue(xform.AddScaleOp(2, 2, 2))
        XCTAssertTrue(xform.ClearXformOpOrder())
    }

    func testGeomXformComputeTransform() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Xform")
        guard let xform = swiftusd.GeomXformRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Xform")
            return
        }

        let matrix = xform.localToWorldTransform()
        // Identity matrix expected for new xform
        XCTAssertEqual(matrix.Get(0, 0), 1.0, accuracy: 1e-10)
        XCTAssertEqual(matrix.Get(1, 1), 1.0, accuracy: 1e-10)
        XCTAssertEqual(matrix.Get(2, 2), 1.0, accuracy: 1e-10)
        XCTAssertEqual(matrix.Get(3, 3), 1.0, accuracy: 1e-10)
    }

    // MARK: - GeomMeshRef Tests

    func testGeomMeshDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Mesh")
        guard let mesh = swiftusd.GeomMeshRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Mesh")
            return
        }

        XCTAssertTrue(mesh.IsValid())
    }

    func testGeomMeshPoints() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Mesh")
        guard let mesh = swiftusd.GeomMeshRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Mesh")
            return
        }

        // Set points for a triangle
        let points = [
            swiftusd.Vec3f(0, 0, 0),
            swiftusd.Vec3f(1, 0, 0),
            swiftusd.Vec3f(0.5, 1, 0)
        ]
        XCTAssertTrue(mesh.setPoints(points))

        // Get points back
        let retrieved = mesh.getPointsAsVec3f()
        XCTAssertEqual(retrieved.count, 3)
        XCTAssertEqual(retrieved[0].x, 0, accuracy: 1e-6)
        XCTAssertEqual(retrieved[1].x, 1, accuracy: 1e-6)
        XCTAssertEqual(retrieved[2].y, 1, accuracy: 1e-6)
    }

    func testGeomMeshTopology() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Mesh")
        guard let mesh = swiftusd.GeomMeshRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Mesh")
            return
        }

        // Set face vertex counts (one triangle)
        let counts: [Int32] = [3]
        XCTAssertTrue(mesh.setFaceVertexCounts(counts))

        // Set face vertex indices
        let indices: [Int32] = [0, 1, 2]
        XCTAssertTrue(mesh.setFaceVertexIndices(indices))

        // Retrieve and verify
        let retrievedCounts = mesh.getFaceVertexCounts()
        XCTAssertEqual(retrievedCounts.count, 1)
        XCTAssertEqual(retrievedCounts[0], 3)

        let retrievedIndices = mesh.getFaceVertexIndices()
        XCTAssertEqual(retrievedIndices.count, 3)
        XCTAssertEqual(retrievedIndices[0], 0)
        XCTAssertEqual(retrievedIndices[1], 1)
        XCTAssertEqual(retrievedIndices[2], 2)
    }

    func testGeomMeshSubdivisionScheme() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Mesh")
        guard let mesh = swiftusd.GeomMeshRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Mesh")
            return
        }

        let catmullClark = swiftusd.SubdivisionScheme.catmullClark
        XCTAssertTrue(mesh.SetSubdivisionScheme(catmullClark))

        let retrieved = mesh.GetSubdivisionScheme()
        XCTAssertEqual(retrieved.value.rawValue, 1) // CatmullClark
    }

    func testGeomMeshDoubleSided() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Mesh")
        guard let mesh = swiftusd.GeomMeshRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Mesh")
            return
        }

        XCTAssertFalse(mesh.GetDoubleSided())
        XCTAssertTrue(mesh.SetDoubleSided(true))
        XCTAssertTrue(mesh.GetDoubleSided())
    }

    // MARK: - GeomPointsRef Tests

    func testGeomPointsDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Points")
        guard let points = swiftusd.GeomPointsRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Points")
            return
        }

        XCTAssertTrue(points.IsValid())
    }

    func testGeomPointsData() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Points")
        guard let pointsPrim = swiftusd.GeomPointsRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Points")
            return
        }

        // Set point positions
        let positions = [
            swiftusd.Vec3f(0, 0, 0),
            swiftusd.Vec3f(1, 1, 1),
            swiftusd.Vec3f(2, 2, 2)
        ]
        XCTAssertTrue(pointsPrim.setPoints(positions))

        // Set widths
        let widths: [Float] = [0.1, 0.2, 0.3]
        XCTAssertTrue(pointsPrim.setWidths(widths))

        // Set IDs
        let ids: [Int64] = [100, 101, 102]
        XCTAssertTrue(pointsPrim.setIds(ids))

        // Retrieve and verify
        let retrievedPoints = pointsPrim.getPointsAsVec3f()
        XCTAssertEqual(retrievedPoints.count, 3)

        let retrievedWidths = pointsPrim.getWidths()
        XCTAssertEqual(retrievedWidths.count, 3)
        XCTAssertEqual(retrievedWidths[0], 0.1, accuracy: 1e-6)

        let retrievedIds = pointsPrim.getIds()
        XCTAssertEqual(retrievedIds.count, 3)
        XCTAssertEqual(retrievedIds[0], 100)
    }

    // MARK: - GeomBasisCurvesRef Tests

    func testGeomBasisCurvesDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Curves")
        guard let curves = swiftusd.GeomBasisCurvesRef.define(on: stage, at: path) else {
            XCTFail("Failed to define BasisCurves")
            return
        }

        XCTAssertTrue(curves.IsValid())
    }

    func testGeomBasisCurvesProperties() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Curves")
        guard let curves = swiftusd.GeomBasisCurvesRef.define(on: stage, at: path) else {
            XCTFail("Failed to define BasisCurves")
            return
        }

        // Set curve type
        let cubic = swiftusd.CurveType.cubic
        XCTAssertTrue(curves.SetType(cubic))
        XCTAssertEqual(curves.GetType().value.rawValue, 1) // Cubic

        // Set basis
        let bezier = swiftusd.CurveBasis.bezier
        XCTAssertTrue(curves.SetBasis(bezier))
        XCTAssertEqual(curves.GetBasis().value.rawValue, 0) // Bezier

        // Set wrap
        let periodic = swiftusd.CurveWrap.periodic
        XCTAssertTrue(curves.SetWrap(periodic))
        XCTAssertEqual(curves.GetWrap().value.rawValue, 1) // Periodic
    }

    func testGeomBasisCurvesData() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Curves")
        guard let curves = swiftusd.GeomBasisCurvesRef.define(on: stage, at: path) else {
            XCTFail("Failed to define BasisCurves")
            return
        }

        // Set curve vertex counts
        let counts: [Int32] = [4, 4]
        XCTAssertTrue(curves.setCurveVertexCounts(counts))

        // Set points
        let points = [
            swiftusd.Vec3f(0, 0, 0), swiftusd.Vec3f(1, 1, 0),
            swiftusd.Vec3f(2, 0, 0), swiftusd.Vec3f(3, 1, 0),
            swiftusd.Vec3f(0, 0, 1), swiftusd.Vec3f(1, 1, 1),
            swiftusd.Vec3f(2, 0, 1), swiftusd.Vec3f(3, 1, 1)
        ]
        XCTAssertTrue(curves.setPoints(points))

        // Set widths
        let widths: [Float] = [0.1, 0.2]
        XCTAssertTrue(curves.setWidths(widths))

        // Retrieve and verify
        let retrievedCounts = curves.getCurveVertexCounts()
        XCTAssertEqual(retrievedCounts.count, 2)
        XCTAssertEqual(retrievedCounts[0], 4)

        let retrievedPoints = curves.getPointsAsVec3f()
        XCTAssertEqual(retrievedPoints.count, 8)

        let retrievedWidths = curves.getWidths()
        XCTAssertEqual(retrievedWidths.count, 2)
    }

    // MARK: - GeomCameraRef Tests

    func testGeomCameraDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Camera")
        guard let camera = swiftusd.GeomCameraRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Camera")
            return
        }

        XCTAssertTrue(camera.IsValid())
    }

    func testGeomCameraProperties() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Camera")
        guard let camera = swiftusd.GeomCameraRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Camera")
            return
        }

        let time = swiftusd.TimeCode.Default()

        // Focal length
        XCTAssertTrue(camera.SetFocalLength(time, 35.0))
        XCTAssertEqual(camera.GetFocalLength(), 35.0, accuracy: 0.01)

        // Horizontal aperture
        XCTAssertTrue(camera.SetHorizontalAperture(time, 36.0))
        XCTAssertEqual(camera.GetHorizontalAperture(), 36.0, accuracy: 0.01)

        // Vertical aperture
        XCTAssertTrue(camera.SetVerticalAperture(time, 24.0))
        XCTAssertEqual(camera.GetVerticalAperture(), 24.0, accuracy: 0.01)

        // Clipping range
        XCTAssertTrue(camera.SetClippingRange(time, 0.1, 10000.0))
        if let range = camera.getClippingRange() {
            XCTAssertEqual(range.near, 0.1, accuracy: 0.001)
            XCTAssertEqual(range.far, 10000.0, accuracy: 1.0)
        } else {
            XCTFail("Failed to get clipping range")
        }
    }

    // MARK: - Primitive Shape Tests

    func testGeomSphereDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Sphere")
        guard let sphere = swiftusd.GeomSphereRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Sphere")
            return
        }

        XCTAssertTrue(sphere.IsValid())

        let time = swiftusd.TimeCode.Default()
        XCTAssertTrue(sphere.SetRadius(time, 2.5))
        XCTAssertEqual(sphere.GetRadius(), 2.5, accuracy: 1e-10)
    }

    func testGeomCubeDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Cube")
        guard let cube = swiftusd.GeomCubeRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Cube")
            return
        }

        XCTAssertTrue(cube.IsValid())

        let time = swiftusd.TimeCode.Default()
        XCTAssertTrue(cube.SetSize(time, 3.0))
        XCTAssertEqual(cube.GetSize(), 3.0, accuracy: 1e-10)
    }

    func testGeomCylinderDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Cylinder")
        guard let cylinder = swiftusd.GeomCylinderRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Cylinder")
            return
        }

        XCTAssertTrue(cylinder.IsValid())

        let time = swiftusd.TimeCode.Default()
        XCTAssertTrue(cylinder.SetHeight(time, 5.0))
        XCTAssertTrue(cylinder.SetRadius(time, 1.5))
        XCTAssertTrue(cylinder.SetAxis(swiftusd.GeomAxis.y))

        XCTAssertEqual(cylinder.GetHeight(), 5.0, accuracy: 1e-10)
        XCTAssertEqual(cylinder.GetRadius(), 1.5, accuracy: 1e-10)
        XCTAssertEqual(cylinder.GetAxis().value.rawValue, 1) // Y axis
    }

    func testGeomConeDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Cone")
        guard let cone = swiftusd.GeomConeRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Cone")
            return
        }

        XCTAssertTrue(cone.IsValid())

        let time = swiftusd.TimeCode.Default()
        XCTAssertTrue(cone.SetHeight(time, 4.0))
        XCTAssertTrue(cone.SetRadius(time, 1.0))
        XCTAssertTrue(cone.SetAxis(swiftusd.GeomAxis.x))

        XCTAssertEqual(cone.GetHeight(), 4.0, accuracy: 1e-10)
        XCTAssertEqual(cone.GetRadius(), 1.0, accuracy: 1e-10)
        XCTAssertEqual(cone.GetAxis().value.rawValue, 0) // X axis
    }

    func testGeomCapsuleDefine() {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }

        let path = swiftusd.Path("/World/Capsule")
        guard let capsule = swiftusd.GeomCapsuleRef.define(on: stage, at: path) else {
            XCTFail("Failed to define Capsule")
            return
        }

        XCTAssertTrue(capsule.IsValid())

        let time = swiftusd.TimeCode.Default()
        XCTAssertTrue(capsule.SetHeight(time, 3.0))
        XCTAssertTrue(capsule.SetRadius(time, 0.75))
        XCTAssertTrue(capsule.SetAxis(swiftusd.GeomAxis.z))

        XCTAssertEqual(capsule.GetHeight(), 3.0, accuracy: 1e-10)
        XCTAssertEqual(capsule.GetRadius(), 0.75, accuracy: 1e-10)
        XCTAssertEqual(capsule.GetAxis().value.rawValue, 2) // Z axis
    }

    // MARK: - Integration Tests

    func testCompleteGeometryWorkflow() {
        guard let stage = swiftusd.StageRef.createInMemory(identifier: "geometry_test") else {
            XCTFail("Failed to create stage")
            return
        }

        // Create a hierarchy with xform and mesh
        let xformPath = swiftusd.Path("/World")
        guard let xform = swiftusd.GeomXformRef.define(on: stage, at: xformPath) else {
            XCTFail("Failed to define Xform")
            return
        }
        XCTAssertTrue(xform.IsValid())
        XCTAssertTrue(xform.AddTranslateOp(0, 10, 0))

        // Create a mesh under the xform
        let meshPath = swiftusd.Path("/World/Mesh")
        guard let mesh = swiftusd.GeomMeshRef.define(on: stage, at: meshPath) else {
            XCTFail("Failed to define Mesh")
            return
        }
        XCTAssertTrue(mesh.IsValid())

        // Set up a simple quad
        let points = [
            swiftusd.Vec3f(-1, 0, -1),
            swiftusd.Vec3f(1, 0, -1),
            swiftusd.Vec3f(1, 0, 1),
            swiftusd.Vec3f(-1, 0, 1)
        ]
        XCTAssertTrue(mesh.setPoints(points))

        let counts: [Int32] = [4]
        XCTAssertTrue(mesh.setFaceVertexCounts(counts))

        let indices: [Int32] = [0, 1, 2, 3]
        XCTAssertTrue(mesh.setFaceVertexIndices(indices))

        XCTAssertTrue(mesh.SetDoubleSided(true))

        // Verify
        XCTAssertEqual(mesh.getPointsAsVec3f().count, 4)
        XCTAssertEqual(mesh.getFaceVertexCounts().count, 1)
        XCTAssertEqual(mesh.getFaceVertexIndices().count, 4)
        XCTAssertTrue(mesh.GetDoubleSided())
    }
}
