// GeomUtilTests.swift - Tests for GeomUtil mesh generation utilities

import XCTest
@testable import SwiftUSD

final class GeomUtilTests: XCTestCase {

    // MARK: - Sphere Tests

    func testSphereGeneration() throws {
        let sphere = try GeomUtil.generateSphere(
            numRadial: 8,
            numAxial: 4,
            radius: 1.0
        )

        // Verify we got mesh data
        XCTAssertGreaterThan(sphere.pointCount, 0, "Sphere should have points")
        XCTAssertGreaterThan(sphere.normalCount, 0, "Sphere should have normals")
        XCTAssertGreaterThan(sphere.faceCount, 0, "Sphere should have faces")
        XCTAssertGreaterThan(sphere.faceVertexIndexCount, 0, "Sphere should have face indices")

        // Verify normals interpolation
        XCTAssertEqual(sphere.normalsInterpolation, .vertex, "Sphere should have vertex normals")

        // Verify points array
        let points = sphere.points
        XCTAssertEqual(points.count, sphere.pointCount, "Points count should match")

        // Verify all points are within radius bounds
        for point in points {
            let dist = sqrt(point.x * point.x + point.y * point.y + point.z * point.z)
            XCTAssertLessThanOrEqual(dist, 1.01, "Point should be within radius")
        }
    }

    func testSpherePointCount() {
        // Full sphere (closed sweep)
        let fullCount = GeomUtil.computeSpherePointCount(numRadial: 8, numAxial: 4)
        XCTAssertGreaterThan(fullCount, 0, "Should compute point count for sphere")

        // Open sweep should have more points
        let openCount = GeomUtil.computeSpherePointCount(numRadial: 8, numAxial: 4, closedSweep: false)
        XCTAssertGreaterThan(openCount, fullCount, "Open sweep should have more points")
    }

    func testSphereWithPartialSweep() throws {
        let sphere = try GeomUtil.generateSphere(
            numRadial: 8,
            numAxial: 4,
            radius: 1.0,
            sweepDegrees: 180.0
        )

        XCTAssertGreaterThan(sphere.pointCount, 0, "Half sphere should have points")
        XCTAssertGreaterThan(sphere.faceCount, 0, "Half sphere should have faces")
    }

    // MARK: - Cuboid Tests

    func testCuboidGeneration() throws {
        let box = try GeomUtil.generateCuboid(
            xLength: 2.0,
            yLength: 1.0,
            zLength: 0.5
        )

        // Verify we got mesh data
        XCTAssertEqual(box.pointCount, 8, "Cuboid should have 8 vertices")
        XCTAssertEqual(box.normalCount, 6, "Cuboid should have 6 face normals")
        XCTAssertEqual(box.faceCount, 6, "Cuboid should have 6 faces")

        // Verify normals interpolation
        XCTAssertEqual(box.normalsInterpolation, .uniform, "Cuboid should have uniform normals")

        // All faces should be quads
        for count in box.faceVertexCounts {
            XCTAssertEqual(count, 4, "Cuboid faces should be quads")
        }
    }

    func testCuboidPointCount() {
        let count = GeomUtil.computeCuboidPointCount()
        XCTAssertEqual(count, 8, "Cuboid should always have 8 points")
    }

    func testCubeGeneration() throws {
        let cube = try GeomUtil.generateCube(size: 1.0)

        XCTAssertEqual(cube.pointCount, 8, "Cube should have 8 vertices")

        // Verify all vertices are at +/- 0.5 on each axis
        let points = cube.points
        for point in points {
            XCTAssertTrue(
                abs(abs(point.x) - 0.5) < 0.001 &&
                abs(abs(point.y) - 0.5) < 0.001 &&
                abs(abs(point.z) - 0.5) < 0.001,
                "Cube vertices should be at corners"
            )
        }
    }

    // MARK: - Cylinder Tests

    func testCylinderGeneration() throws {
        let cylinder = try GeomUtil.generateCylinder(
            numRadial: 16,
            radius: 1.0,
            height: 2.0
        )

        XCTAssertGreaterThan(cylinder.pointCount, 0, "Cylinder should have points")
        XCTAssertGreaterThan(cylinder.normalCount, 0, "Cylinder should have normals")
        XCTAssertGreaterThan(cylinder.faceCount, 0, "Cylinder should have faces")
        XCTAssertEqual(cylinder.normalsInterpolation, .vertex, "Cylinder should have vertex normals")
    }

    func testCylinderWithDifferentRadii() throws {
        // Frustum-like cylinder
        let frustum = try GeomUtil.generateCylinder(
            numRadial: 12,
            bottomRadius: 2.0,
            topRadius: 1.0,
            height: 3.0
        )

        XCTAssertGreaterThan(frustum.pointCount, 0, "Frustum should have points")
        XCTAssertGreaterThan(frustum.faceCount, 0, "Frustum should have faces")
    }

    func testCylinderPointCount() {
        let closedCount = GeomUtil.computeCylinderPointCount(numRadial: 8, closedSweep: true)
        let openCount = GeomUtil.computeCylinderPointCount(numRadial: 8, closedSweep: false)

        XCTAssertGreaterThan(closedCount, 0, "Should compute cylinder point count")
        XCTAssertGreaterThan(openCount, closedCount, "Open sweep should have more points")
    }

    // MARK: - Cone Tests

    func testConeGeneration() throws {
        let cone = try GeomUtil.generateCone(
            numRadial: 16,
            radius: 1.0,
            height: 2.0
        )

        XCTAssertGreaterThan(cone.pointCount, 0, "Cone should have points")
        XCTAssertGreaterThan(cone.normalCount, 0, "Cone should have normals")
        XCTAssertGreaterThan(cone.faceCount, 0, "Cone should have faces")
        XCTAssertEqual(cone.normalsInterpolation, .vertex, "Cone should have vertex normals")
    }

    func testConePointCount() {
        let count = GeomUtil.computeConePointCount(numRadial: 8)
        XCTAssertGreaterThan(count, 0, "Should compute cone point count")
    }

    // MARK: - Capsule Tests

    func testCapsuleGeneration() throws {
        let capsule = try GeomUtil.generateCapsule(
            numRadial: 12,
            numCapAxial: 4,
            radius: 0.5,
            height: 1.0
        )

        XCTAssertGreaterThan(capsule.pointCount, 0, "Capsule should have points")
        XCTAssertGreaterThan(capsule.normalCount, 0, "Capsule should have normals")
        XCTAssertGreaterThan(capsule.faceCount, 0, "Capsule should have faces")
        XCTAssertEqual(capsule.normalsInterpolation, .vertex, "Capsule should have vertex normals")
    }

    func testCapsuleWithDifferentRadii() throws {
        let capsule = try GeomUtil.generateCapsule(
            numRadial: 8,
            numCapAxial: 2,
            bottomRadius: 1.0,
            topRadius: 0.5,
            height: 2.0
        )

        XCTAssertGreaterThan(capsule.pointCount, 0, "Asymmetric capsule should have points")
    }

    func testCapsulePointCount() {
        let count = GeomUtil.computeCapsulePointCount(numRadial: 8, numCapAxial: 4)
        XCTAssertGreaterThan(count, 0, "Should compute capsule point count")
    }

    // MARK: - Plane Tests

    func testPlaneGeneration() throws {
        let plane = try GeomUtil.generatePlane(
            xLength: 10.0,
            yLength: 5.0
        )

        XCTAssertEqual(plane.pointCount, 4, "Plane should have 4 vertices")
        XCTAssertEqual(plane.normalCount, 1, "Plane should have 1 normal (constant)")
        XCTAssertEqual(plane.faceCount, 1, "Plane should have 1 face")
        XCTAssertEqual(plane.normalsInterpolation, .constant, "Plane should have constant normals")

        // The single face should be a quad
        XCTAssertEqual(plane.faceVertexCounts[0], 4, "Plane face should be a quad")
    }

    func testPlanePointCount() {
        let count = GeomUtil.computePlanePointCount()
        XCTAssertEqual(count, 4, "Plane should always have 4 points")
    }

    func testSquareGeneration() throws {
        let square = try GeomUtil.generateSquare(size: 2.0)

        let points = square.points
        for point in points {
            XCTAssertEqual(point.z, 0.0, accuracy: 0.001, "Square points should be in XY plane")
        }
    }

    // MARK: - Disk Tests

    func testDiskGeneration() throws {
        let disk = try GeomUtil.generateDisk(
            numRadial: 16,
            radius: 1.0
        )

        XCTAssertGreaterThan(disk.pointCount, 0, "Disk should have points")
        XCTAssertEqual(disk.normalCount, 1, "Disk should have 1 normal (constant)")
        XCTAssertGreaterThan(disk.faceCount, 0, "Disk should have faces")
        XCTAssertEqual(disk.normalsInterpolation, .constant, "Disk should have constant normals")
    }

    func testDiskWithPartialSweep() throws {
        // Pie-slice disk
        let pieSlice = try GeomUtil.generateDisk(
            numRadial: 8,
            radius: 1.0,
            sweepDegrees: 90.0
        )

        XCTAssertGreaterThan(pieSlice.pointCount, 0, "Pie slice should have points")
    }

    func testDiskPointCount() {
        let closedCount = GeomUtil.computeDiskPointCount(numRadial: 8, closedSweep: true)
        let openCount = GeomUtil.computeDiskPointCount(numRadial: 8, closedSweep: false)

        XCTAssertGreaterThan(closedCount, 0, "Should compute disk point count")
        XCTAssertGreaterThan(openCount, closedCount, "Open sweep should have more points")
    }

    // MARK: - Transform Tests

    func testCuboidWithTransform() throws {
        // Identity-like transform but with translation
        var transform = [Float](repeating: 0, count: 16)
        transform[0] = 1.0   // scale X
        transform[5] = 1.0   // scale Y
        transform[10] = 1.0  // scale Z
        transform[12] = 5.0  // translate X
        transform[13] = 3.0  // translate Y
        transform[14] = 2.0  // translate Z
        transform[15] = 1.0

        let box = try GeomUtil.generateCuboid(
            xLength: 1.0,
            yLength: 1.0,
            zLength: 1.0,
            transform: transform
        )

        // Verify translation was applied
        let points = box.points
        var minX = Float.infinity
        var maxX = -Float.infinity
        for point in points {
            minX = min(minX, point.x)
            maxX = max(maxX, point.x)
        }

        XCTAssertGreaterThan(minX, 4.4, "Box should be translated in X")
        XCTAssertLessThan(maxX, 5.6, "Box should be translated in X")
    }

    func testInvalidTransformThrows() {
        XCTAssertThrowsError(try GeomUtil.generateCuboid(
            xLength: 1.0,
            yLength: 1.0,
            zLength: 1.0,
            transform: [1.0, 0.0, 0.0]  // Invalid: only 3 elements
        )) { error in
            XCTAssertTrue(error is GeomUtilError)
        }
    }

    // MARK: - MeshData Accessor Tests

    func testMeshDataAccessors() throws {
        let sphere = try GeomUtil.generateSphere(
            numRadial: 8,
            numAxial: 4,
            radius: 1.0
        )

        // Test flat accessors
        let pointsFlat = sphere.pointsFlat
        XCTAssertEqual(pointsFlat.count, sphere.pointCount * 3, "Flat points should have 3x elements")

        let normalsFlat = sphere.normalsFlat
        XCTAssertEqual(normalsFlat.count, sphere.normalCount * 3, "Flat normals should have 3x elements")

        // Verify consistency between SIMD and flat accessors
        let points = sphere.points
        for i in 0..<points.count {
            XCTAssertEqual(points[i].x, pointsFlat[i * 3 + 0], accuracy: 0.0001)
            XCTAssertEqual(points[i].y, pointsFlat[i * 3 + 1], accuracy: 0.0001)
            XCTAssertEqual(points[i].z, pointsFlat[i * 3 + 2], accuracy: 0.0001)
        }
    }

    func testMeshDataDescription() throws {
        let cube = try GeomUtil.generateCube(size: 1.0)

        let description = cube.description
        XCTAssertTrue(description.contains("MeshData"))
        XCTAssertTrue(description.contains("points: 8"))
        XCTAssertTrue(description.contains("uniform"))
        XCTAssertTrue(description.contains("faces: 6"))
    }

    // MARK: - Normals Interpolation Tests

    func testNormalsInterpolationDescription() {
        XCTAssertEqual(GeomUtil.NormalsInterpolation.constant.description, "constant")
        XCTAssertEqual(GeomUtil.NormalsInterpolation.uniform.description, "uniform")
        XCTAssertEqual(GeomUtil.NormalsInterpolation.vertex.description, "vertex")
    }

    // MARK: - Edge Cases

    func testMinimumSegments() throws {
        // Minimum radial segments for a sphere
        let minSphere = try GeomUtil.generateSphere(
            numRadial: 3,
            numAxial: 2,
            radius: 1.0
        )
        XCTAssertGreaterThan(minSphere.pointCount, 0, "Minimum sphere should still generate")

        // Minimum radial segments for a disk
        let minDisk = try GeomUtil.generateDisk(
            numRadial: 3,
            radius: 1.0
        )
        XCTAssertGreaterThan(minDisk.pointCount, 0, "Minimum disk should still generate")
    }

    func testZeroRadius() throws {
        // Zero radius sphere (degenerate case)
        let sphere = try GeomUtil.generateSphere(
            numRadial: 8,
            numAxial: 4,
            radius: 0.0
        )
        XCTAssertGreaterThan(sphere.pointCount, 0, "Zero-radius sphere should still generate points")
    }
}
