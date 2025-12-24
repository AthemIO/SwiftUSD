// PxOsdTests.swift - Tests for PxOsd module

import XCTest
@testable import SwiftUSD

final class PxOsdTests: XCTestCase {

    // MARK: - SubdivTags Tests

    func testSubdivTagsCreation() throws {
        let tags = try PxOsd.SubdivTags()

        // Check default values
        XCTAssertEqual(tags.vertexInterpolationRule, .edgeAndCorner)
        XCTAssertEqual(tags.creaseMethod, .uniform)
        XCTAssertEqual(tags.triangleSubdivision, .catmullClark)
    }

    func testSubdivTagsInterpolationRules() throws {
        let tags = try PxOsd.SubdivTags()

        // Test vertex interpolation rule
        tags.vertexInterpolationRule = .none
        XCTAssertEqual(tags.vertexInterpolationRule, .none)

        tags.vertexInterpolationRule = .edgeOnly
        XCTAssertEqual(tags.vertexInterpolationRule, .edgeOnly)

        tags.vertexInterpolationRule = .edgeAndCorner
        XCTAssertEqual(tags.vertexInterpolationRule, .edgeAndCorner)

        // Test face-varying interpolation rule
        tags.faceVaryingInterpolationRule = .none
        XCTAssertEqual(tags.faceVaryingInterpolationRule, .none)

        tags.faceVaryingInterpolationRule = .all
        XCTAssertEqual(tags.faceVaryingInterpolationRule, .all)

        // Test crease method
        tags.creaseMethod = .chaikin
        XCTAssertEqual(tags.creaseMethod, .chaikin)

        tags.creaseMethod = .uniform
        XCTAssertEqual(tags.creaseMethod, .uniform)

        // Test triangle subdivision
        tags.triangleSubdivision = .smooth
        XCTAssertEqual(tags.triangleSubdivision, .smooth)
    }

    func testSubdivTagsCreaseData() throws {
        let tags = try PxOsd.SubdivTags()

        // Set crease data
        let lengths: [Int32] = [2, 3]
        let indices: [Int32] = [0, 1, 2, 3, 4]
        let weights: [Float] = [2.0, 3.0]

        tags.creaseLengths = lengths
        tags.creaseIndices = indices
        tags.creaseWeights = weights

        XCTAssertEqual(tags.creaseLengths, lengths)
        XCTAssertEqual(tags.creaseIndices, indices)
        XCTAssertEqual(tags.creaseWeights, weights)
    }

    func testSubdivTagsCornerData() throws {
        let tags = try PxOsd.SubdivTags()

        // Set corner data
        let indices: [Int32] = [0, 5, 10]
        let weights: [Float] = [1.0, 2.0, 3.0]

        tags.cornerIndices = indices
        tags.cornerWeights = weights

        XCTAssertEqual(tags.cornerIndices, indices)
        XCTAssertEqual(tags.cornerWeights, weights)
    }

    func testSubdivTagsCopy() throws {
        let original = try PxOsd.SubdivTags()
        original.vertexInterpolationRule = .edgeOnly
        original.creaseLengths = [2]
        original.creaseIndices = [0, 1]
        original.creaseWeights = [5.0]

        let copy = try original.copy()

        XCTAssertEqual(copy.vertexInterpolationRule, .edgeOnly)
        XCTAssertEqual(copy.creaseLengths, [2])
        XCTAssertEqual(copy.creaseIndices, [0, 1])
        XCTAssertEqual(copy.creaseWeights, [5.0])
    }

    func testSubdivTagsEquality() throws {
        let tags1 = try PxOsd.SubdivTags()
        let tags2 = try PxOsd.SubdivTags()

        // Same defaults should be equal
        XCTAssertEqual(tags1, tags2)

        // Different settings should not be equal
        tags1.vertexInterpolationRule = .none
        XCTAssertNotEqual(tags1, tags2)
    }

    func testSubdivTagsHash() throws {
        let tags1 = try PxOsd.SubdivTags()
        let tags2 = try PxOsd.SubdivTags()

        // Same defaults should have same hash
        XCTAssertEqual(tags1.computeHash(), tags2.computeHash())

        // Different settings should have different hash
        tags1.creaseLengths = [2]
        tags1.creaseIndices = [0, 1]
        XCTAssertNotEqual(tags1.computeHash(), tags2.computeHash())
    }

    func testSubdivTagsDescription() throws {
        let tags = try PxOsd.SubdivTags()
        let desc = tags.description
        XCTAssertTrue(desc.contains("SubdivTags"))
    }

    // MARK: - MeshTopology Tests

    func testMeshTopologyEmptyCreation() throws {
        let topology = try PxOsd.MeshTopology()

        XCTAssertEqual(topology.scheme, .bilinear)
        XCTAssertEqual(topology.orientation, .rightHanded)
        XCTAssertEqual(topology.faceCount, 0)
        XCTAssertEqual(topology.faceVertexIndexCount, 0)
    }

    func testMeshTopologyQuadCreation() throws {
        let counts: [Int32] = [4]
        let indices: [Int32] = [0, 1, 2, 3]

        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: counts,
            faceVertexIndices: indices
        )

        XCTAssertEqual(topology.scheme, .catmullClark)
        XCTAssertEqual(topology.orientation, .rightHanded)
        XCTAssertEqual(topology.faceCount, 1)
        XCTAssertEqual(topology.faceVertexCounts, counts)
        XCTAssertEqual(topology.faceVertexIndexCount, 4)
        XCTAssertEqual(topology.faceVertexIndices, indices)
    }

    func testMeshTopologyTriangleCreation() throws {
        let counts: [Int32] = [3, 3]
        let indices: [Int32] = [0, 1, 2, 1, 3, 2]

        let topology = try PxOsd.MeshTopology(
            scheme: .loop,
            orientation: .leftHanded,
            faceVertexCounts: counts,
            faceVertexIndices: indices
        )

        XCTAssertEqual(topology.scheme, .loop)
        XCTAssertEqual(topology.orientation, .leftHanded)
        XCTAssertEqual(topology.faceCount, 2)
        XCTAssertEqual(topology.faceVertexCounts, counts)
        XCTAssertEqual(topology.faceVertexIndices, indices)
    }

    func testMeshTopologyWithHoles() throws {
        let counts: [Int32] = [4, 4, 4]
        let indices: [Int32] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
        let holes: [Int32] = [1]  // Second face is a hole

        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: counts,
            faceVertexIndices: indices,
            holeIndices: holes
        )

        XCTAssertEqual(topology.holeIndexCount, 1)
        XCTAssertEqual(topology.holeIndices, holes)
    }

    func testMeshTopologyWithSubdivTags() throws {
        let tags = try PxOsd.SubdivTags()
        tags.vertexInterpolationRule = .edgeOnly
        tags.creaseLengths = [2]
        tags.creaseIndices = [0, 1]
        tags.creaseWeights = [3.0]

        let counts: [Int32] = [4]
        let indices: [Int32] = [0, 1, 2, 3]

        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: counts,
            faceVertexIndices: indices,
            subdivTags: tags
        )

        let retrievedTags = topology.subdivTags
        XCTAssertNotNil(retrievedTags)
        XCTAssertEqual(retrievedTags?.vertexInterpolationRule, .edgeOnly)
        XCTAssertEqual(retrievedTags?.creaseLengths, [2])
    }

    func testMeshTopologyWithScheme() throws {
        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let bilinear = try topology.withScheme(.bilinear)
        XCTAssertEqual(bilinear.scheme, .bilinear)
        XCTAssertEqual(bilinear.faceVertexCounts, topology.faceVertexCounts)

        let loop = try topology.withScheme(.loop)
        XCTAssertEqual(loop.scheme, .loop)
    }

    func testMeshTopologyWithOrientation() throws {
        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let leftHanded = try topology.withOrientation(.leftHanded)
        XCTAssertEqual(leftHanded.orientation, .leftHanded)
        XCTAssertEqual(leftHanded.faceVertexCounts, topology.faceVertexCounts)
    }

    func testMeshTopologyWithHoleIndices() throws {
        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4, 4],
            faceVertexIndices: [0, 1, 2, 3, 4, 5, 6, 7]
        )

        let withHoles = try topology.withHoleIndices([0])
        XCTAssertEqual(withHoles.holeIndices, [0])
        XCTAssertEqual(withHoles.faceVertexCounts, topology.faceVertexCounts)
    }

    func testMeshTopologyValidation() throws {
        // Valid topology
        let valid = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let validation = valid.validate()
        XCTAssertTrue(validation.isValid)
        XCTAssertEqual(validation.errors.count, 0)
    }

    func testMeshTopologyCopy() throws {
        let original = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let copy = try original.copy()

        XCTAssertEqual(copy.scheme, original.scheme)
        XCTAssertEqual(copy.orientation, original.orientation)
        XCTAssertEqual(copy.faceVertexCounts, original.faceVertexCounts)
        XCTAssertEqual(copy.faceVertexIndices, original.faceVertexIndices)
    }

    func testMeshTopologyEquality() throws {
        let topology1 = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let topology2 = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let topology3 = try PxOsd.MeshTopology(
            scheme: .loop,
            orientation: .rightHanded,
            faceVertexCounts: [3],
            faceVertexIndices: [0, 1, 2]
        )

        XCTAssertEqual(topology1, topology2)
        XCTAssertNotEqual(topology1, topology3)
    }

    func testMeshTopologyHash() throws {
        let topology1 = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let topology2 = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        XCTAssertEqual(topology1.computeHash(), topology2.computeHash())
    }

    func testMeshTopologyDescription() throws {
        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let desc = topology.description
        XCTAssertTrue(desc.contains("MeshTopology"))
        XCTAssertTrue(desc.contains("catmullClark"))
    }

    // MARK: - Enum String Tests

    func testSubdivSchemeDescription() {
        XCTAssertEqual(PxOsd.SubdivScheme.catmullClark.description, "catmullClark")
        XCTAssertEqual(PxOsd.SubdivScheme.loop.description, "loop")
        XCTAssertEqual(PxOsd.SubdivScheme.bilinear.description, "bilinear")
    }

    func testOrientationDescription() {
        XCTAssertEqual(PxOsd.Orientation.rightHanded.description, "rightHanded")
        XCTAssertEqual(PxOsd.Orientation.leftHanded.description, "leftHanded")
    }

    func testVertexInterpolationRuleDescription() {
        XCTAssertEqual(PxOsd.VertexInterpolationRule.none.description, "none")
        XCTAssertEqual(PxOsd.VertexInterpolationRule.edgeOnly.description, "edgeOnly")
        XCTAssertEqual(PxOsd.VertexInterpolationRule.edgeAndCorner.description, "edgeAndCorner")
    }

    func testCreaseMethodDescription() {
        XCTAssertEqual(PxOsd.CreaseMethod.uniform.description, "uniform")
        XCTAssertEqual(PxOsd.CreaseMethod.chaikin.description, "chaikin")
    }

    func testTriangleSubdivisionDescription() {
        XCTAssertEqual(PxOsd.TriangleSubdivision.catmullClark.description, "catmullClark")
        XCTAssertEqual(PxOsd.TriangleSubdivision.smooth.description, "smooth")
    }

    // MARK: - Validation Description Tests

    func testValidationDescription() throws {
        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: [4],
            faceVertexIndices: [0, 1, 2, 3]
        )

        let validation = topology.validate()
        XCTAssertTrue(validation.description.contains("valid"))
    }

    // MARK: - Full Workflow Test

    func testCubeTopologyWorkflow() throws {
        // Create subdivision tags with creases on all edges
        let tags = try PxOsd.SubdivTags()
        tags.vertexInterpolationRule = .edgeAndCorner
        tags.creaseMethod = .uniform

        // Define cube topology (6 faces, 8 vertices)
        let faceVertexCounts: [Int32] = [4, 4, 4, 4, 4, 4]
        let faceVertexIndices: [Int32] = [
            0, 1, 2, 3,  // front
            4, 5, 6, 7,  // back
            0, 4, 5, 1,  // bottom
            2, 6, 7, 3,  // top
            0, 3, 7, 4,  // left
            1, 5, 6, 2   // right
        ]

        // Create topology
        let topology = try PxOsd.MeshTopology(
            scheme: .catmullClark,
            orientation: .rightHanded,
            faceVertexCounts: faceVertexCounts,
            faceVertexIndices: faceVertexIndices,
            subdivTags: tags
        )

        // Validate
        let validation = topology.validate()
        XCTAssertTrue(validation.isValid)

        // Check properties
        XCTAssertEqual(topology.faceCount, 6)
        XCTAssertEqual(topology.faceVertexIndexCount, 24)
        XCTAssertEqual(topology.scheme, .catmullClark)

        // Create modified version
        let bilinear = try topology.withScheme(.bilinear)
        XCTAssertEqual(bilinear.scheme, .bilinear)
        XCTAssertEqual(bilinear.faceCount, 6)

        // Hash for caching
        let hash = topology.computeHash()
        XCTAssertNotEqual(hash, 0)
    }
}
