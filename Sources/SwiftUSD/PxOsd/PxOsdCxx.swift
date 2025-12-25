// PxOsdCxx.swift - OpenSubdiv Integration wrapper using C++ interop
// Mirrors: pxr/imaging/pxOsd/*.h (Subdivision surfaces)

import USDCxx

// MARK: - Type Aliases for C++ Types

/// Subdivision tags for non-hierarchical subdivision surfaces.
public typealias PxOsdSubdivTags = swiftusd.PxOsdSubdivTagsRef

/// Mesh topology data for subdivision meshes.
public typealias PxOsdMeshTopology = swiftusd.PxOsdMeshTopologyRef

/// Validation result for mesh topology.
public typealias PxOsdMeshTopologyValidation = swiftusd.PxOsdMeshTopologyValidationRef

/// Subdivision scheme types.
public typealias PxOsdSubdivScheme = swiftusd.PxOsdSubdivScheme

/// Interpolation rules for boundaries.
public typealias PxOsdInterpolationRule = swiftusd.PxOsdInterpolationRule

/// Crease computation methods.
public typealias PxOsdCreaseMethod = swiftusd.PxOsdCreaseMethod

/// Triangle subdivision methods.
public typealias PxOsdTriangleSubdivision = swiftusd.PxOsdTriangleSubdivision

/// Face winding orientation.
public typealias PxOsdOrientation = swiftusd.PxOsdOrientation

/// Validation error codes.
public typealias PxOsdValidationCode = swiftusd.PxOsdMeshTopologyValidationCode

// MARK: - PxOsdSubdivTags Extensions

extension swiftusd.PxOsdSubdivTagsRef {

    /// Create empty subdivision tags.
    public static func create() -> swiftusd.PxOsdSubdivTagsRef? {
        return swiftusd.PxOsdSubdivTagsRef.Create()
    }

    /// Create subdivision tags with all parameters.
    public static func create(
        vertexInterpolationRule: swiftusd.PxOsdInterpolationRule,
        faceVaryingInterpolationRule: swiftusd.PxOsdInterpolationRule,
        creaseMethod: swiftusd.PxOsdCreaseMethod,
        triangleSubdivision: swiftusd.PxOsdTriangleSubdivision,
        creaseIndices: swiftusd.IntArray?,
        creaseLengths: swiftusd.IntArray?,
        creaseWeights: swiftusd.FloatArray?,
        cornerIndices: swiftusd.IntArray?,
        cornerWeights: swiftusd.FloatArray?
    ) -> swiftusd.PxOsdSubdivTagsRef? {
        return swiftusd.PxOsdSubdivTagsRef.Create(
            vertexInterpolationRule,
            faceVaryingInterpolationRule,
            creaseMethod,
            triangleSubdivision,
            creaseIndices,
            creaseLengths,
            creaseWeights,
            cornerIndices,
            cornerWeights
        )
    }

    /// Vertex interpolation rule.
    public var vertexInterpolation: swiftusd.PxOsdInterpolationRule {
        get { GetVertexInterpolationRule() }
        set { SetVertexInterpolationRule(newValue) }
    }

    /// Face-varying interpolation rule.
    public var faceVaryingInterpolation: swiftusd.PxOsdInterpolationRule {
        get { GetFaceVaryingInterpolationRule() }
        set { SetFaceVaryingInterpolationRule(newValue) }
    }

    /// Crease method.
    public var crease: swiftusd.PxOsdCreaseMethod {
        get { GetCreaseMethod() }
        set { SetCreaseMethod(newValue) }
    }

    /// Triangle subdivision method.
    public var triangleSubdiv: swiftusd.PxOsdTriangleSubdivision {
        get { GetTriangleSubdivision() }
        set { SetTriangleSubdivision(newValue) }
    }

    /// Compute hash for instancing.
    public var hash: UInt64 {
        ComputeHash()
    }
}

// MARK: - PxOsdMeshTopology Extensions

extension swiftusd.PxOsdMeshTopologyRef {

    /// Create empty mesh topology.
    public static func create() -> swiftusd.PxOsdMeshTopologyRef? {
        return swiftusd.PxOsdMeshTopologyRef.Create()
    }

    /// Create mesh topology with basic parameters.
    public static func create(
        scheme: swiftusd.PxOsdSubdivScheme,
        orientation: swiftusd.PxOsdOrientation,
        faceVertexCounts: swiftusd.IntArray,
        faceVertexIndices: swiftusd.IntArray
    ) -> swiftusd.PxOsdMeshTopologyRef? {
        return swiftusd.PxOsdMeshTopologyRef.Create(
            scheme,
            orientation,
            faceVertexCounts,
            faceVertexIndices
        )
    }

    /// Create mesh topology with holes.
    public static func create(
        scheme: swiftusd.PxOsdSubdivScheme,
        orientation: swiftusd.PxOsdOrientation,
        faceVertexCounts: swiftusd.IntArray,
        faceVertexIndices: swiftusd.IntArray,
        holeIndices: swiftusd.IntArray?
    ) -> swiftusd.PxOsdMeshTopologyRef? {
        return swiftusd.PxOsdMeshTopologyRef.Create(
            scheme,
            orientation,
            faceVertexCounts,
            faceVertexIndices,
            holeIndices
        )
    }

    /// Create mesh topology with holes and subdiv tags.
    public static func create(
        scheme: swiftusd.PxOsdSubdivScheme,
        orientation: swiftusd.PxOsdOrientation,
        faceVertexCounts: swiftusd.IntArray,
        faceVertexIndices: swiftusd.IntArray,
        holeIndices: swiftusd.IntArray?,
        subdivTags: swiftusd.PxOsdSubdivTagsRef?
    ) -> swiftusd.PxOsdMeshTopologyRef? {
        return swiftusd.PxOsdMeshTopologyRef.Create(
            scheme,
            orientation,
            faceVertexCounts,
            faceVertexIndices,
            holeIndices,
            subdivTags
        )
    }

    /// The subdivision scheme.
    public var scheme: swiftusd.PxOsdSubdivScheme {
        GetScheme()
    }

    /// The face orientation.
    public var orientation: swiftusd.PxOsdOrientation {
        GetOrientation()
    }

    /// Get face vertex counts array.
    public var faceVertexCounts: swiftusd.IntArray? {
        GetFaceVertexCounts()
    }

    /// Get face vertex indices array.
    public var faceVertexIndices: swiftusd.IntArray? {
        GetFaceVertexIndices()
    }

    /// Get hole indices array.
    public var holeIndices: swiftusd.IntArray? {
        GetHoleIndices()
    }

    /// Get subdivision tags.
    public var subdivTags: swiftusd.PxOsdSubdivTagsRef? {
        GetSubdivTags()
    }

    /// Number of faces.
    public var faceCount: Int {
        Int(GetFaceCount())
    }

    /// Total number of vertex indices.
    public var vertexIndexCount: Int {
        Int(GetVertexIndexCount())
    }

    /// Validate the topology.
    public var validation: swiftusd.PxOsdMeshTopologyValidationRef? {
        Validate()
    }

    /// Compute hash for instancing.
    public var hash: UInt64 {
        ComputeHash()
    }

    /// Create a copy with a different scheme.
    public func with(scheme: swiftusd.PxOsdSubdivScheme) -> swiftusd.PxOsdMeshTopologyRef? {
        WithScheme(scheme)
    }

    /// Create a copy with a different orientation.
    public func with(orientation: swiftusd.PxOsdOrientation) -> swiftusd.PxOsdMeshTopologyRef? {
        WithOrientation(orientation)
    }

    /// Create a copy with different hole indices.
    public func with(holeIndices: swiftusd.IntArray?) -> swiftusd.PxOsdMeshTopologyRef? {
        WithHoleIndices(holeIndices)
    }

    /// Create a copy with different subdiv tags.
    public func with(subdivTags: swiftusd.PxOsdSubdivTagsRef?) -> swiftusd.PxOsdMeshTopologyRef? {
        WithSubdivTags(subdivTags)
    }

    /// Check equality with another topology.
    public func equals(_ other: swiftusd.PxOsdMeshTopologyRef?) -> Bool {
        Equals(other)
    }
}

// MARK: - PxOsdMeshTopologyValidation Extensions

extension swiftusd.PxOsdMeshTopologyValidationRef {

    /// Whether the topology is valid.
    public var isValid: Bool {
        IsValid()
    }

    /// Number of validation errors.
    public var errorCount: Int {
        Int(GetErrorCount())
    }

    /// Get error code at index.
    public func errorCode(at index: Int) -> swiftusd.PxOsdMeshTopologyValidationCode {
        GetErrorCode(index)
    }

    /// Get error message at index.
    public func errorMessage(at index: Int) -> String {
        String(cString: GetErrorMessage(index))
    }

    /// Get all error messages.
    public var errorMessages: [String] {
        (0..<errorCount).map { errorMessage(at: $0) }
    }
}

// MARK: - Token Constants

/// Standard tokens for PxOsd subdivision schemes and rules.
public enum PxOsdTokensCxx {
    // Subdivision schemes
    public static var catmullClark: swiftusd.Token { swiftusd.PxOsdTokens.CatmullClark() }
    public static var loop: swiftusd.Token { swiftusd.PxOsdTokens.Loop() }
    public static var bilinear: swiftusd.Token { swiftusd.PxOsdTokens.Bilinear() }

    // Interpolation rules
    public static var none: swiftusd.Token { swiftusd.PxOsdTokens.None() }
    public static var all: swiftusd.Token { swiftusd.PxOsdTokens.All() }
    public static var cornersOnly: swiftusd.Token { swiftusd.PxOsdTokens.CornersOnly() }
    public static var cornersPlus1: swiftusd.Token { swiftusd.PxOsdTokens.CornersPlus1() }
    public static var cornersPlus2: swiftusd.Token { swiftusd.PxOsdTokens.CornersPlus2() }
    public static var boundaries: swiftusd.Token { swiftusd.PxOsdTokens.Boundaries() }
    public static var edgeOnly: swiftusd.Token { swiftusd.PxOsdTokens.EdgeOnly() }
    public static var edgeAndCorner: swiftusd.Token { swiftusd.PxOsdTokens.EdgeAndCorner() }

    // Crease methods
    public static var uniform: swiftusd.Token { swiftusd.PxOsdTokens.Uniform() }
    public static var chaikin: swiftusd.Token { swiftusd.PxOsdTokens.Chaikin() }

    // Orientations
    public static var leftHanded: swiftusd.Token { swiftusd.PxOsdTokens.LeftHanded() }
    public static var rightHanded: swiftusd.Token { swiftusd.PxOsdTokens.RightHanded() }

    // Triangle subdivision
    public static var smooth: swiftusd.Token { swiftusd.PxOsdTokens.Smooth() }
}
