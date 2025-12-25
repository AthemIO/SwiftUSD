// PxOsdCxxTests.swift - Tests for PxOsd C++ interop

import XCTest
import USDCxx
@testable import SwiftUSD

final class PxOsdCxxTests: XCTestCase {

    // MARK: - PxOsdSubdivTags Tests

    func testSubdivTagsCreation() {
        // Test empty creation
        let tags = swiftusd.PxOsdSubdivTagsRef.Create()
        XCTAssertNotNil(tags, "Should create empty subdiv tags")

        // Verify default values
        XCTAssertEqual(tags?.GetVertexInterpolationRule(), .None)
        XCTAssertEqual(tags?.GetFaceVaryingInterpolationRule(), .None)
        XCTAssertEqual(tags?.GetCreaseMethod(), .None)
        XCTAssertEqual(tags?.GetTriangleSubdivision(), .None)
    }

    func testSubdivTagsInterpolationRules() {
        guard let tags = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }

        // Test vertex interpolation rule
        tags.SetVertexInterpolationRule(.CornersPlus1)
        XCTAssertEqual(tags.GetVertexInterpolationRule(), .CornersPlus1)

        tags.SetVertexInterpolationRule(.EdgeAndCorner)
        XCTAssertEqual(tags.GetVertexInterpolationRule(), .EdgeAndCorner)

        // Test face-varying interpolation rule
        tags.SetFaceVaryingInterpolationRule(.Boundaries)
        XCTAssertEqual(tags.GetFaceVaryingInterpolationRule(), .Boundaries)

        tags.SetFaceVaryingInterpolationRule(.All)
        XCTAssertEqual(tags.GetFaceVaryingInterpolationRule(), .All)
    }

    func testSubdivTagsCreaseMethod() {
        guard let tags = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }

        tags.SetCreaseMethod(.Uniform)
        XCTAssertEqual(tags.GetCreaseMethod(), .Uniform)

        tags.SetCreaseMethod(.Chaikin)
        XCTAssertEqual(tags.GetCreaseMethod(), .Chaikin)
    }

    func testSubdivTagsTriangleSubdivision() {
        guard let tags = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }

        tags.SetTriangleSubdivision(.Smooth)
        XCTAssertEqual(tags.GetTriangleSubdivision(), .Smooth)
    }

    func testSubdivTagsCreaseData() {
        guard let tags = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }

        // Create crease data arrays
        let creaseIndices = swiftusd.IntArray.Create()
        creaseIndices?.append(0)
        creaseIndices?.append(1)
        creaseIndices?.append(2)

        let creaseLengths = swiftusd.IntArray.Create()
        creaseLengths?.append(3)

        let creaseWeights = swiftusd.FloatArray.Create()
        creaseWeights?.append(2.0)

        // Set crease data
        tags.SetCreaseIndices(creaseIndices)
        tags.SetCreaseLengths(creaseLengths)
        tags.SetCreaseWeights(creaseWeights)

        // Verify crease data
        let retrievedIndices = tags.GetCreaseIndices()
        XCTAssertNotNil(retrievedIndices)
        XCTAssertEqual(retrievedIndices?.size(), 3)

        let retrievedLengths = tags.GetCreaseLengths()
        XCTAssertNotNil(retrievedLengths)
        XCTAssertEqual(retrievedLengths?.size(), 1)

        let retrievedWeights = tags.GetCreaseWeights()
        XCTAssertNotNil(retrievedWeights)
        XCTAssertEqual(retrievedWeights?.size(), 1)
    }

    func testSubdivTagsCornerData() {
        guard let tags = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }

        // Create corner data arrays
        let cornerIndices = swiftusd.IntArray.Create()
        cornerIndices?.append(5)
        cornerIndices?.append(10)

        let cornerWeights = swiftusd.FloatArray.Create()
        cornerWeights?.append(3.0)
        cornerWeights?.append(5.0)

        // Set corner data
        tags.SetCornerIndices(cornerIndices)
        tags.SetCornerWeights(cornerWeights)

        // Verify corner data
        let retrievedIndices = tags.GetCornerIndices()
        XCTAssertNotNil(retrievedIndices)
        XCTAssertEqual(retrievedIndices?.size(), 2)

        let retrievedWeights = tags.GetCornerWeights()
        XCTAssertNotNil(retrievedWeights)
        XCTAssertEqual(retrievedWeights?.size(), 2)
    }

    func testSubdivTagsHash() {
        guard let tags1 = swiftusd.PxOsdSubdivTagsRef.Create(),
              let tags2 = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }

        // Same configuration should have same hash
        let hash1 = tags1.ComputeHash()
        let hash2 = tags2.ComputeHash()
        XCTAssertEqual(hash1, hash2)

        // Different configuration should have different hash
        tags1.SetVertexInterpolationRule(.CornersPlus1)
        let hash3 = tags1.ComputeHash()
        XCTAssertNotEqual(hash2, hash3)
    }

    // MARK: - PxOsdMeshTopology Tests

    func testMeshTopologyCreation() {
        // Test empty creation
        let topology = swiftusd.PxOsdMeshTopologyRef.Create()
        XCTAssertNotNil(topology, "Should create empty mesh topology")
    }

    func testMeshTopologyWithBasicParams() {
        // Create a simple quad topology
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        faceVertexIndices?.append(0)
        faceVertexIndices?.append(1)
        faceVertexIndices?.append(2)
        faceVertexIndices?.append(3)

        let topology = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices
        )

        XCTAssertNotNil(topology)
        XCTAssertEqual(topology?.GetScheme(), .CatmullClark)
        XCTAssertEqual(topology?.GetOrientation(), .RightHanded)
        XCTAssertEqual(topology?.GetFaceCount(), 1)
        XCTAssertEqual(topology?.GetVertexIndexCount(), 4)
    }

    func testMeshTopologyWithHoles() {
        // Create topology with 2 quads, one being a hole
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<8 {
            faceVertexIndices?.append(i)
        }

        let holeIndices = swiftusd.IntArray.Create()
        holeIndices?.append(1)  // Second face is a hole

        let topology = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices,
            holeIndices
        )

        XCTAssertNotNil(topology)
        XCTAssertEqual(topology?.GetFaceCount(), 2)

        let retrievedHoles = topology?.GetHoleIndices()
        XCTAssertNotNil(retrievedHoles)
        XCTAssertEqual(retrievedHoles?.size(), 1)
    }

    func testMeshTopologyWithSubdivTags() {
        // Create subdiv tags
        guard let tags = swiftusd.PxOsdSubdivTagsRef.Create() else {
            XCTFail("Failed to create subdiv tags")
            return
        }
        tags.SetVertexInterpolationRule(.EdgeAndCorner)

        // Create topology with subdiv tags
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices?.append(i)
        }

        let topology = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices,
            nil,
            tags
        )

        XCTAssertNotNil(topology)

        let retrievedTags = topology?.GetSubdivTags()
        XCTAssertNotNil(retrievedTags)
        XCTAssertEqual(retrievedTags?.GetVertexInterpolationRule(), .EdgeAndCorner)
    }

    func testMeshTopologyBuilderMethods() {
        // Create initial topology
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices?.append(i)
        }

        guard let topology = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices
        ) else {
            XCTFail("Failed to create initial topology")
            return
        }

        // Test WithScheme
        let loopTopology = topology.WithScheme(.Loop)
        XCTAssertNotNil(loopTopology)
        XCTAssertEqual(loopTopology?.GetScheme(), .Loop)
        XCTAssertEqual(topology.GetScheme(), .CatmullClark)  // Original unchanged

        // Test WithOrientation
        let leftHandedTopology = topology.WithOrientation(.LeftHanded)
        XCTAssertNotNil(leftHandedTopology)
        XCTAssertEqual(leftHandedTopology?.GetOrientation(), .LeftHanded)
        XCTAssertEqual(topology.GetOrientation(), .RightHanded)  // Original unchanged

        // Test WithHoleIndices
        let holeIndices = swiftusd.IntArray.Create()
        holeIndices?.append(0)

        let holedTopology = topology.WithHoleIndices(holeIndices)
        XCTAssertNotNil(holedTopology)
        XCTAssertEqual(holedTopology?.GetHoleIndices()?.size(), 1)
    }

    func testMeshTopologyValidation() {
        // Create valid topology
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices?.append(i)
        }

        guard let validTopology = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices
        ) else {
            XCTFail("Failed to create valid topology")
            return
        }

        let validation = validTopology.Validate()
        XCTAssertNotNil(validation)
        XCTAssertTrue(validation?.IsValid() ?? false)
        XCTAssertEqual(validation?.GetErrorCount(), 0)
    }

    func testMeshTopologyValidationWithErrors() {
        // Create invalid topology (face with less than 3 vertices)
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(2)  // Invalid: less than 3 vertices

        let faceVertexIndices = swiftusd.IntArray.Create()
        faceVertexIndices?.append(0)
        faceVertexIndices?.append(1)

        guard let invalidTopology = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices
        ) else {
            XCTFail("Failed to create topology")
            return
        }

        let validation = invalidTopology.Validate()
        XCTAssertNotNil(validation)
        XCTAssertFalse(validation?.IsValid() ?? true)
        XCTAssertGreaterThan(validation?.GetErrorCount() ?? 0, 0)
    }

    func testMeshTopologyHash() {
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices?.append(i)
        }

        guard let topology1 = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices
        ) else {
            XCTFail("Failed to create topology1")
            return
        }

        // Create identical topology
        let faceVertexCounts2 = swiftusd.IntArray.Create()
        faceVertexCounts2?.append(4)

        let faceVertexIndices2 = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices2?.append(i)
        }

        guard let topology2 = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts2,
            faceVertexIndices2
        ) else {
            XCTFail("Failed to create topology2")
            return
        }

        // Same topology should have same hash
        let hash1 = topology1.ComputeHash()
        let hash2 = topology2.ComputeHash()
        XCTAssertEqual(hash1, hash2)

        // Different scheme should have different hash
        let loopTopology = topology1.WithScheme(.Loop)
        let hash3 = loopTopology?.ComputeHash() ?? 0
        XCTAssertNotEqual(hash1, hash3)
    }

    func testMeshTopologyEquality() {
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(4)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices?.append(i)
        }

        guard let topology1 = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts,
            faceVertexIndices
        ) else {
            XCTFail("Failed to create topology1")
            return
        }

        // Create identical topology
        let faceVertexCounts2 = swiftusd.IntArray.Create()
        faceVertexCounts2?.append(4)

        let faceVertexIndices2 = swiftusd.IntArray.Create()
        for i: Int32 in 0..<4 {
            faceVertexIndices2?.append(i)
        }

        guard let topology2 = swiftusd.PxOsdMeshTopologyRef.Create(
            .CatmullClark,
            .RightHanded,
            faceVertexCounts2,
            faceVertexIndices2
        ) else {
            XCTFail("Failed to create topology2")
            return
        }

        XCTAssertTrue(topology1.Equals(topology2))

        // Different topology should not be equal
        let loopTopology = topology1.WithScheme(.Loop)
        XCTAssertFalse(topology1.Equals(loopTopology))
    }

    // MARK: - Token Conversion Tests

    func testSubdivSchemeTokenConversion() {
        // Test all scheme conversions
        let catmullClark = swiftusd.PxOsdSubdivSchemeToToken(.CatmullClark)
        XCTAssertEqual(String(cString: catmullClark.GetText()), "catmullClark")

        let loop = swiftusd.PxOsdSubdivSchemeToToken(.Loop)
        XCTAssertEqual(String(cString: loop.GetText()), "loop")

        let bilinear = swiftusd.PxOsdSubdivSchemeToToken(.Bilinear)
        XCTAssertEqual(String(cString: bilinear.GetText()), "bilinear")

        // Test reverse conversion
        XCTAssertEqual(swiftusd.PxOsdSubdivSchemeFromToken(catmullClark), .CatmullClark)
        XCTAssertEqual(swiftusd.PxOsdSubdivSchemeFromToken(loop), .Loop)
        XCTAssertEqual(swiftusd.PxOsdSubdivSchemeFromToken(bilinear), .Bilinear)
    }

    func testInterpolationRuleTokenConversion() {
        let cornersPlus1 = swiftusd.PxOsdInterpolationRuleToToken(.CornersPlus1)
        XCTAssertEqual(String(cString: cornersPlus1.GetText()), "cornersPlus1")

        let boundaries = swiftusd.PxOsdInterpolationRuleToToken(.Boundaries)
        XCTAssertEqual(String(cString: boundaries.GetText()), "boundaries")

        // Test reverse conversion
        XCTAssertEqual(swiftusd.PxOsdInterpolationRuleFromToken(cornersPlus1), .CornersPlus1)
        XCTAssertEqual(swiftusd.PxOsdInterpolationRuleFromToken(boundaries), .Boundaries)
    }

    func testOrientationTokenConversion() {
        let rightHanded = swiftusd.PxOsdOrientationToToken(.RightHanded)
        XCTAssertEqual(String(cString: rightHanded.GetText()), "rightHanded")

        let leftHanded = swiftusd.PxOsdOrientationToToken(.LeftHanded)
        XCTAssertEqual(String(cString: leftHanded.GetText()), "leftHanded")

        // Test reverse conversion
        XCTAssertEqual(swiftusd.PxOsdOrientationFromToken(rightHanded), .RightHanded)
        XCTAssertEqual(swiftusd.PxOsdOrientationFromToken(leftHanded), .LeftHanded)
    }

    // MARK: - Swift Extension Tests

    func testSwiftExtensions() {
        // Test type aliases and extensions work
        guard let tags = PxOsdSubdivTags.create() else {
            XCTFail("Failed to create tags via extension")
            return
        }

        tags.vertexInterpolation = .CornersPlus1
        XCTAssertEqual(tags.vertexInterpolation, .CornersPlus1)

        // Test topology extension
        let faceVertexCounts = swiftusd.IntArray.Create()
        faceVertexCounts?.append(3)

        let faceVertexIndices = swiftusd.IntArray.Create()
        for i: Int32 in 0..<3 {
            faceVertexIndices?.append(i)
        }

        guard let topology = PxOsdMeshTopology.create(
            scheme: .Loop,
            orientation: .RightHanded,
            faceVertexCounts: faceVertexCounts!,
            faceVertexIndices: faceVertexIndices!
        ) else {
            XCTFail("Failed to create topology via extension")
            return
        }

        XCTAssertEqual(topology.scheme, .Loop)
        XCTAssertEqual(topology.faceCount, 1)
        XCTAssertEqual(topology.vertexIndexCount, 3)

        // Test validation extension
        guard let validation = topology.validation else {
            XCTFail("Failed to get validation")
            return
        }

        XCTAssertTrue(validation.isValid)
        XCTAssertEqual(validation.errorCount, 0)
    }

    // MARK: - PxOsdTokens Tests

    func testPxOsdTokenConstants() {
        // Test subdivision scheme tokens
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.CatmullClark().GetText()), "catmullClark")
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.Loop().GetText()), "loop")
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.Bilinear().GetText()), "bilinear")

        // Test orientation tokens
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.RightHanded().GetText()), "rightHanded")
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.LeftHanded().GetText()), "leftHanded")

        // Test crease method tokens
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.Uniform().GetText()), "uniform")
        XCTAssertEqual(String(cString: swiftusd.PxOsdTokens.Chaikin().GetText()), "chaikin")
    }
}
