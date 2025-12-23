// mesh.h - UsdGeomMesh C wrapper
// Mirrors: pxr/usdGeom/mesh.h

#ifndef OPENUSDINTEROP_USDGEOM_MESH_H
#define OPENUSDINTEROP_USDGEOM_MESH_H

#include "../OpenUSDInterop.h"
#include "../base/gf.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Mesh Definition

/// Defines a new mesh at the given path.
USD_INTEROP_API UsdResult UsdGeomMesh_Define(UsdStageRef stage, UsdSdfPathRef path, UsdPrimRef* outPrim);

/// Returns true if the prim is a mesh.
USD_INTEROP_API bool UsdGeomMesh_IsMesh(UsdPrimRef prim);

// MARK: - Points

/// Gets the points attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_GetPointsAttr(UsdPrimRef prim, UsdAttributeRef* outAttr);

/// Creates the points attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_CreatePointsAttr(UsdPrimRef prim, UsdAttributeRef* outAttr);

/// Gets the points array.
USD_INTEROP_API UsdResult UsdGeomMesh_GetPoints(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef* outArray);

/// Sets the points array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetPoints(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef array);

// MARK: - Normals

/// Gets the normals attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_GetNormalsAttr(UsdPrimRef prim, UsdAttributeRef* outAttr);

/// Creates the normals attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_CreateNormalsAttr(UsdPrimRef prim, UsdAttributeRef* outAttr);

/// Gets the normals array.
USD_INTEROP_API UsdResult UsdGeomMesh_GetNormals(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef* outArray);

/// Sets the normals array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetNormals(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef array);

// MARK: - Face Topology

/// Gets the face vertex counts attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_GetFaceVertexCountsAttr(UsdPrimRef prim, UsdAttributeRef* outAttr);

/// Gets the face vertex counts array.
USD_INTEROP_API UsdResult UsdGeomMesh_GetFaceVertexCounts(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef* outArray);

/// Sets the face vertex counts array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetFaceVertexCounts(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef array);

/// Gets the face vertex indices attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_GetFaceVertexIndicesAttr(UsdPrimRef prim, UsdAttributeRef* outAttr);

/// Gets the face vertex indices array.
USD_INTEROP_API UsdResult UsdGeomMesh_GetFaceVertexIndices(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef* outArray);

/// Sets the face vertex indices array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetFaceVertexIndices(UsdPrimRef prim, UsdTimeCode time, UsdVtArrayRef array);

// MARK: - Subdivision

/// Subdivision scheme tokens.
typedef enum {
    USD_GEOM_MESH_SUBDIV_NONE,
    USD_GEOM_MESH_SUBDIV_CATMULL_CLARK,
    USD_GEOM_MESH_SUBDIV_LOOP,
    USD_GEOM_MESH_SUBDIV_BILINEAR
} UsdGeomMeshSubdivScheme;

/// Gets the subdivision scheme.
USD_INTEROP_API UsdResult UsdGeomMesh_GetSubdivisionScheme(UsdPrimRef prim, UsdGeomMeshSubdivScheme* outScheme);

/// Sets the subdivision scheme.
USD_INTEROP_API UsdResult UsdGeomMesh_SetSubdivisionScheme(UsdPrimRef prim, UsdGeomMeshSubdivScheme scheme);

// MARK: - Extent

/// Computes the bounding box extent.
USD_INTEROP_API UsdResult UsdGeomMesh_ComputeExtent(UsdPrimRef prim, UsdTimeCode time, UsdGfVec3f* outMin, UsdGfVec3f* outMax);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDGEOM_MESH_H
