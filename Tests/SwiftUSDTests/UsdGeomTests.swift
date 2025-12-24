// UsdGeomTests.swift - Tests for UsdGeom module
// Tests geometry types: Xform, Mesh, Sphere, Cube, Cylinder, Cone, Capsule, Camera

import XCTest
@testable import SwiftUSD

final class UsdGeomXformTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineXform() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        XCTAssertTrue(xform.isValid)
        XCTAssertNotNil(xform.prim)
        XCTAssertEqual(xform.prim?.path.text, "/World")
    }

    func testDefineNestedXform() throws {
        let stage = try Stage.createInMemory()
        let parentPath = try Path("/World")
        let childPath = try Path("/World/Child")

        let parent = try GeomXform.define(on: stage, at: parentPath)
        let child = try GeomXform.define(on: stage, at: childPath)

        XCTAssertTrue(parent.isValid)
        XCTAssertTrue(child.isValid)
    }

    func testXformFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        guard let prim = xform.prim else {
            XCTFail("Xform prim should not be nil")
            return
        }

        let xformFromPrim = GeomXform.from(prim: prim)
        XCTAssertNotNil(xformFromPrim)
        XCTAssertTrue(xformFromPrim?.isValid ?? false)
    }

    // MARK: - Transform Operation Tests

    func testAddTranslateOp() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        // Should not throw
        try xform.addTranslate(x: 1.0, y: 2.0, z: 3.0)
    }

    func testAddRotateXYZOp() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        // Should not throw
        try xform.addRotateXYZ(rotX: 45.0, rotY: 90.0, rotZ: 0.0)
    }

    func testAddScaleOp() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        // Should not throw
        try xform.addScale(scaleX: 2.0, scaleY: 2.0, scaleZ: 2.0)
    }

    func testClearXformOpOrder() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        try xform.addTranslate(x: 1.0, y: 2.0, z: 3.0)
        try xform.clearXformOpOrder()
        // If we get here without throwing, the test passes
    }

    func testLocalToWorldTransform() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        let matrix = try xform.localToWorldTransform()
        XCTAssertEqual(matrix.count, 16)

        // Identity matrix check (diagonal should be 1s)
        XCTAssertEqual(matrix[0], 1.0, accuracy: 0.0001)
        XCTAssertEqual(matrix[5], 1.0, accuracy: 0.0001)
        XCTAssertEqual(matrix[10], 1.0, accuracy: 0.0001)
        XCTAssertEqual(matrix[15], 1.0, accuracy: 0.0001)
    }

    // MARK: - Description Tests

    func testXformDescription() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/World")
        let xform = try GeomXform.define(on: stage, at: path)

        let description = xform.description
        XCTAssertTrue(description.contains("World") || description.contains("xform"))
    }
}

final class UsdGeomMeshTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineMesh() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        XCTAssertTrue(mesh.isValid)
        XCTAssertNotNil(mesh.prim)
        XCTAssertEqual(mesh.prim?.path.text, "/Mesh")
    }

    func testMeshFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        guard let prim = mesh.prim else {
            XCTFail("Mesh prim should not be nil")
            return
        }

        let meshFromPrim = GeomMesh.from(prim: prim)
        XCTAssertNotNil(meshFromPrim)
        XCTAssertTrue(meshFromPrim?.isValid ?? false)
    }

    // MARK: - Points Tests

    func testMeshSetAndGetPoints() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Triangle vertices
        let points: [Float] = [
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            0.5, 1.0, 0.0
        ]

        try mesh.setPoints(points)

        let retrievedPoints = mesh.points()
        XCTAssertEqual(retrievedPoints.count, 9)
        XCTAssertEqual(mesh.pointCount(), 3)

        // Verify first point
        XCTAssertEqual(retrievedPoints[0], 0.0, accuracy: 0.0001)
        XCTAssertEqual(retrievedPoints[1], 0.0, accuracy: 0.0001)
        XCTAssertEqual(retrievedPoints[2], 0.0, accuracy: 0.0001)
    }

    // MARK: - Topology Tests

    func testMeshSetAndGetFaceVertexCounts() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Two triangles
        let counts: [Int32] = [3, 3]
        try mesh.setFaceVertexCounts(counts)

        let retrievedCounts = mesh.faceVertexCounts()
        XCTAssertEqual(retrievedCounts.count, 2)
        XCTAssertEqual(mesh.faceVertexCountsCount(), 2)
    }

    func testMeshSetAndGetFaceVertexIndices() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Indices for two triangles
        let indices: [Int32] = [0, 1, 2, 0, 2, 3]
        try mesh.setFaceVertexIndices(indices)

        let retrievedIndices = mesh.faceVertexIndices()
        XCTAssertEqual(retrievedIndices.count, 6)
        XCTAssertEqual(mesh.faceVertexIndicesCount(), 6)
    }

    // MARK: - Subdivision Tests

    func testMeshSubdivisionScheme() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Default should be none or catmull-clark
        let initialScheme = mesh.subdivisionScheme
        XCTAssertTrue(initialScheme == .none || initialScheme == .catmullClark)

        // Set to catmull-clark
        try mesh.setSubdivisionScheme(.catmullClark)
    }

    // MARK: - Display Properties Tests

    func testMeshDoubleSided() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Set double-sided to true
        try mesh.setDoubleSided(true)
        // Note: Depending on implementation, this may or may not persist immediately
    }

    // MARK: - Complete Mesh Tests

    func testCreateTriangleMesh() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Triangle")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Triangle vertices
        let points: [Float] = [
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            0.5, 1.0, 0.0
        ]

        let faceVertexCounts: [Int32] = [3]
        let faceVertexIndices: [Int32] = [0, 1, 2]

        try mesh.setPoints(points)
        try mesh.setFaceVertexCounts(faceVertexCounts)
        try mesh.setFaceVertexIndices(faceVertexIndices)

        XCTAssertEqual(mesh.pointCount(), 3)
        XCTAssertEqual(mesh.faceVertexCountsCount(), 1)
        XCTAssertEqual(mesh.faceVertexIndicesCount(), 3)
    }

    func testCreateQuadMesh() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Quad")
        let mesh = try GeomMesh.define(on: stage, at: path)

        // Quad vertices
        let points: [Float] = [
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0
        ]

        let faceVertexCounts: [Int32] = [4]
        let faceVertexIndices: [Int32] = [0, 1, 2, 3]

        try mesh.setPoints(points)
        try mesh.setFaceVertexCounts(faceVertexCounts)
        try mesh.setFaceVertexIndices(faceVertexIndices)

        XCTAssertEqual(mesh.pointCount(), 4)
        XCTAssertEqual(mesh.faceVertexCountsCount(), 1)
        XCTAssertEqual(mesh.faceVertexIndicesCount(), 4)
    }
}

final class UsdGeomSphereTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineSphere() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Sphere")
        let sphere = try GeomSphere.define(on: stage, at: path)

        XCTAssertTrue(sphere.isValid)
        XCTAssertNotNil(sphere.prim)
    }

    func testSphereFromPrim() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Sphere")
        let sphere = try GeomSphere.define(on: stage, at: path)

        guard let prim = sphere.prim else {
            XCTFail("Sphere prim should not be nil")
            return
        }

        let sphereFromPrim = GeomSphere.from(prim: prim)
        XCTAssertNotNil(sphereFromPrim)
    }

    // MARK: - Property Tests

    func testSphereRadius() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Sphere")
        let sphere = try GeomSphere.define(on: stage, at: path)

        // Default radius is typically 1.0
        let defaultRadius = sphere.radius()
        XCTAssertEqual(defaultRadius, 1.0, accuracy: 0.0001)

        // Set new radius
        try sphere.setRadius(5.0)
        let newRadius = sphere.radius()
        XCTAssertEqual(newRadius, 5.0, accuracy: 0.0001)
    }
}

final class UsdGeomCubeTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineCube() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cube")
        let cube = try GeomCube.define(on: stage, at: path)

        XCTAssertTrue(cube.isValid)
        XCTAssertNotNil(cube.prim)
    }

    // MARK: - Property Tests

    func testCubeSize() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cube")
        let cube = try GeomCube.define(on: stage, at: path)

        // Default size is typically 2.0
        let defaultSize = cube.size()
        XCTAssertEqual(defaultSize, 2.0, accuracy: 0.0001)

        // Set new size
        try cube.setSize(4.0)
        let newSize = cube.size()
        XCTAssertEqual(newSize, 4.0, accuracy: 0.0001)
    }
}

final class UsdGeomCylinderTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineCylinder() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cylinder")
        let cylinder = try GeomCylinder.define(on: stage, at: path)

        XCTAssertTrue(cylinder.isValid)
        XCTAssertNotNil(cylinder.prim)
    }

    // MARK: - Property Tests

    func testCylinderHeight() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cylinder")
        let cylinder = try GeomCylinder.define(on: stage, at: path)

        // Set height
        try cylinder.setHeight(5.0)
        let height = cylinder.height()
        XCTAssertEqual(height, 5.0, accuracy: 0.0001)
    }

    func testCylinderRadius() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cylinder")
        let cylinder = try GeomCylinder.define(on: stage, at: path)

        // Set radius
        try cylinder.setRadius(2.0)
        let radius = cylinder.radius()
        XCTAssertEqual(radius, 2.0, accuracy: 0.0001)
    }

    func testCylinderAxis() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cylinder")
        let cylinder = try GeomCylinder.define(on: stage, at: path)

        // Default axis is typically Z
        let defaultAxis = cylinder.axis
        XCTAssertEqual(defaultAxis, .z)

        // Set axis to Y
        try cylinder.setAxis(.y)
        let newAxis = cylinder.axis
        XCTAssertEqual(newAxis, .y)
    }
}

final class UsdGeomConeTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineCone() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cone")
        let cone = try GeomCone.define(on: stage, at: path)

        XCTAssertTrue(cone.isValid)
        XCTAssertNotNil(cone.prim)
    }

    // MARK: - Property Tests

    func testConeProperties() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cone")
        let cone = try GeomCone.define(on: stage, at: path)

        try cone.setHeight(3.0)
        try cone.setRadius(1.5)
        try cone.setAxis(.x)

        XCTAssertEqual(cone.height(), 3.0, accuracy: 0.0001)
        XCTAssertEqual(cone.radius(), 1.5, accuracy: 0.0001)
        XCTAssertEqual(cone.axis, .x)
    }
}

final class UsdGeomCapsuleTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineCapsule() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Capsule")
        let capsule = try GeomCapsule.define(on: stage, at: path)

        XCTAssertTrue(capsule.isValid)
        XCTAssertNotNil(capsule.prim)
    }

    // MARK: - Property Tests

    func testCapsuleProperties() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Capsule")
        let capsule = try GeomCapsule.define(on: stage, at: path)

        try capsule.setHeight(4.0)
        try capsule.setRadius(0.75)
        try capsule.setAxis(.y)

        XCTAssertEqual(capsule.height(), 4.0, accuracy: 0.0001)
        XCTAssertEqual(capsule.radius(), 0.75, accuracy: 0.0001)
        XCTAssertEqual(capsule.axis, .y)
    }
}

final class UsdGeomCameraTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineCamera() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Camera")
        let camera = try GeomCamera.define(on: stage, at: path)

        XCTAssertTrue(camera.isValid)
        XCTAssertNotNil(camera.prim)
    }

    // MARK: - Property Tests

    func testCameraFocalLength() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Camera")
        let camera = try GeomCamera.define(on: stage, at: path)

        // Default focal length is typically 50mm
        let defaultFocalLength = camera.focalLength()
        XCTAssertEqual(defaultFocalLength, 50.0, accuracy: 0.1)

        // Set new focal length
        try camera.setFocalLength(35.0)
        let newFocalLength = camera.focalLength()
        XCTAssertEqual(newFocalLength, 35.0, accuracy: 0.1)
    }

    func testCameraAperture() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Camera")
        let camera = try GeomCamera.define(on: stage, at: path)

        // Set horizontal aperture
        try camera.setHorizontalAperture(24.0)
        let hAperture = camera.horizontalAperture()
        XCTAssertEqual(hAperture, 24.0, accuracy: 0.1)

        // Set vertical aperture
        try camera.setVerticalAperture(18.0)
        let vAperture = camera.verticalAperture()
        XCTAssertEqual(vAperture, 18.0, accuracy: 0.1)
    }

    func testCameraClippingRange() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Camera")
        let camera = try GeomCamera.define(on: stage, at: path)

        // Set clipping range
        try camera.setClippingRange(near: 0.01, far: 1000.0)

        let range = camera.clippingRange()
        XCTAssertNotNil(range)
        if let range = range {
            XCTAssertEqual(range.near, 0.01, accuracy: 0.001)
            XCTAssertEqual(range.far, 1000.0, accuracy: 0.1)
        }
    }
}

// MARK: - Enum Tests

final class UsdGeomEnumTests: XCTestCase {

    func testInterpolationEnum() {
        XCTAssertEqual(GeomInterpolation.constant.rawValue, 0)
        XCTAssertEqual(GeomInterpolation.uniform.rawValue, 1)
        XCTAssertEqual(GeomInterpolation.varying.rawValue, 2)
        XCTAssertEqual(GeomInterpolation.vertex.rawValue, 3)
        XCTAssertEqual(GeomInterpolation.faceVarying.rawValue, 4)
    }

    func testSubdivisionSchemeEnum() {
        XCTAssertEqual(SubdivisionScheme.none.rawValue, 0)
        XCTAssertEqual(SubdivisionScheme.catmullClark.rawValue, 1)
        XCTAssertEqual(SubdivisionScheme.loop.rawValue, 2)
        XCTAssertEqual(SubdivisionScheme.bilinear.rawValue, 3)
    }

    func testGeomAxisEnum() {
        XCTAssertEqual(GeomAxis.x.rawValue, 0)
        XCTAssertEqual(GeomAxis.y.rawValue, 1)
        XCTAssertEqual(GeomAxis.z.rawValue, 2)
    }

    func testGeomPurposeEnum() {
        XCTAssertEqual(GeomPurpose.default.rawValue, 0)
        XCTAssertEqual(GeomPurpose.render.rawValue, 1)
        XCTAssertEqual(GeomPurpose.proxy.rawValue, 2)
        XCTAssertEqual(GeomPurpose.guide.rawValue, 3)
    }
}

// MARK: - Type Alias Tests

final class UsdGeomTypeAliasTests: XCTestCase {

    func testXformTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Xform")
        let xform: UsdGeomXform = try UsdGeomXform.define(on: stage, at: path)
        XCTAssertTrue(xform.isValid)
    }

    func testMeshTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Mesh")
        let mesh: UsdGeomMesh = try UsdGeomMesh.define(on: stage, at: path)
        XCTAssertTrue(mesh.isValid)
    }

    func testSphereTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Sphere")
        let sphere: UsdGeomSphere = try UsdGeomSphere.define(on: stage, at: path)
        XCTAssertTrue(sphere.isValid)
    }

    func testCubeTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cube")
        let cube: UsdGeomCube = try UsdGeomCube.define(on: stage, at: path)
        XCTAssertTrue(cube.isValid)
    }

    func testCylinderTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cylinder")
        let cylinder: UsdGeomCylinder = try UsdGeomCylinder.define(on: stage, at: path)
        XCTAssertTrue(cylinder.isValid)
    }

    func testConeTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Cone")
        let cone: UsdGeomCone = try UsdGeomCone.define(on: stage, at: path)
        XCTAssertTrue(cone.isValid)
    }

    func testCapsuleTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Capsule")
        let capsule: UsdGeomCapsule = try UsdGeomCapsule.define(on: stage, at: path)
        XCTAssertTrue(capsule.isValid)
    }

    func testCameraTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = try Path("/Camera")
        let camera: UsdGeomCamera = try UsdGeomCamera.define(on: stage, at: path)
        XCTAssertTrue(camera.isValid)
    }
}
