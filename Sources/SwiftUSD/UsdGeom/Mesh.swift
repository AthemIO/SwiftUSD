// Mesh.swift - UsdGeomMesh wrapper
// Mirrors: pxr/usdGeom/mesh.h

import OpenUSDInterop

/// A mesh is a polygonal geometry prim.
/// Mirrors pxr::UsdGeomMesh from the USD C++ API.
public struct UsdGeomMesh: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Returns true if the prim is a valid mesh.
    public var isValid: Bool {
        return prim.isValid
    }

    // MARK: - Define

    /// Defines a new mesh at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdGeomMesh? {
        guard let prim = stage.definePrim(at: path, type: "Mesh") else {
            return nil
        }
        return UsdGeomMesh(prim)
    }

    // MARK: - Points

    /// Returns the points attribute.
    public var pointsAttr: UsdAttribute? {
        return prim.attribute("points")
    }

    /// Gets the mesh points.
    public func getPoints(at time: UsdTimeCode = .default) -> VtVec3fArray? {
        // TODO: Implement via C interop
        return nil
    }

    /// Sets the mesh points.
    public func setPoints(_ points: VtVec3fArray, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Normals

    /// Returns the normals attribute.
    public var normalsAttr: UsdAttribute? {
        return prim.attribute("normals")
    }

    /// Gets the mesh normals.
    public func getNormals(at time: UsdTimeCode = .default) -> VtVec3fArray? {
        // TODO: Implement via C interop
        return nil
    }

    /// Sets the mesh normals.
    public func setNormals(_ normals: VtVec3fArray, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Face Vertex Counts

    /// Returns the face vertex counts attribute.
    public var faceVertexCountsAttr: UsdAttribute? {
        return prim.attribute("faceVertexCounts")
    }

    /// Gets the face vertex counts.
    public func getFaceVertexCounts(at time: UsdTimeCode = .default) -> VtIntArray? {
        // TODO: Implement via C interop
        return nil
    }

    /// Sets the face vertex counts.
    public func setFaceVertexCounts(_ counts: VtIntArray, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Face Vertex Indices

    /// Returns the face vertex indices attribute.
    public var faceVertexIndicesAttr: UsdAttribute? {
        return prim.attribute("faceVertexIndices")
    }

    /// Gets the face vertex indices.
    public func getFaceVertexIndices(at time: UsdTimeCode = .default) -> VtIntArray? {
        // TODO: Implement via C interop
        return nil
    }

    /// Sets the face vertex indices.
    public func setFaceVertexIndices(_ indices: VtIntArray, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}
