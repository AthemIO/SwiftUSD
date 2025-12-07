//
// Copyright 2025 Afloat Technologies. All Rights Reserved.
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_IMAGING_PX_OSD_SWIFT_INTEROP_H
#define PXR_IMAGING_PX_OSD_SWIFT_INTEROP_H

/// \file pxOsd/swiftInterop.h
///
/// Swift C++ Interop Bridge for PxOsd
///
/// This header provides wrapper functions that return by value instead of
/// by const reference. Swift C++ interop cannot handle methods that return
/// const& (const references) - they cause compiler crashes.
///
/// These inline functions are lightweight wrappers that copy the returned
/// values, enabling Swift to safely access PxOsd data.

#include "pxr/pxrns.h"
#include "PxOsd/meshTopology.h"
#include "PxOsd/subdivTags.h"

PXR_NAMESPACE_OPEN_SCOPE

// ============================================================================
// PxOsdMeshTopology Swift Bridge Functions
// ============================================================================

/// Returns face vertex counts by value (Swift-compatible)
inline VtIntArray
PxOsdMeshTopology_GetFaceVertexCounts(const PxOsdMeshTopology& topology) {
    return topology.GetFaceVertexCounts();
}

/// Returns face vertex indices by value (Swift-compatible)
inline VtIntArray
PxOsdMeshTopology_GetFaceVertexIndices(const PxOsdMeshTopology& topology) {
    return topology.GetFaceVertexIndices();
}

/// Returns orientation token by value (Swift-compatible)
inline TfToken
PxOsdMeshTopology_GetOrientation(const PxOsdMeshTopology& topology) {
    return topology.GetOrientation();
}

/// Returns hole indices by value (Swift-compatible)
inline VtIntArray
PxOsdMeshTopology_GetHoleIndices(const PxOsdMeshTopology& topology) {
    return topology.GetHoleIndices();
}

/// Returns subdivision tags by value (Swift-compatible)
inline PxOsdSubdivTags
PxOsdMeshTopology_GetSubdivTags(const PxOsdMeshTopology& topology) {
    return topology.GetSubdivTags();
}

/// Returns whether the topology is valid
inline bool
PxOsdMeshTopology_IsValid(const PxOsdMeshTopology& topology) {
    return static_cast<bool>(topology.Validate());
}

// ============================================================================
// PxOsdSubdivTags Swift Bridge Functions
// ============================================================================

/// Returns crease indices by value (Swift-compatible)
inline VtIntArray
PxOsdSubdivTags_GetCreaseIndices(const PxOsdSubdivTags& tags) {
    return tags.GetCreaseIndices();
}

/// Returns crease lengths by value (Swift-compatible)
inline VtIntArray
PxOsdSubdivTags_GetCreaseLengths(const PxOsdSubdivTags& tags) {
    return tags.GetCreaseLengths();
}

/// Returns crease weights by value (Swift-compatible)
inline VtFloatArray
PxOsdSubdivTags_GetCreaseWeights(const PxOsdSubdivTags& tags) {
    return tags.GetCreaseWeights();
}

/// Returns corner indices by value (Swift-compatible)
inline VtIntArray
PxOsdSubdivTags_GetCornerIndices(const PxOsdSubdivTags& tags) {
    return tags.GetCornerIndices();
}

/// Returns corner weights by value (Swift-compatible)
inline VtFloatArray
PxOsdSubdivTags_GetCornerWeights(const PxOsdSubdivTags& tags) {
    return tags.GetCornerWeights();
}

// ============================================================================
// Factory Functions for Swift
// ============================================================================

/// Create a mesh topology with the basic parameters
inline PxOsdMeshTopology
PxOsdMeshTopology_Create(
    const TfToken& scheme,
    const TfToken& orientation,
    const VtIntArray& faceVertexCounts,
    const VtIntArray& faceVertexIndices)
{
    return PxOsdMeshTopology(scheme, orientation, faceVertexCounts, faceVertexIndices);
}

/// Create a mesh topology with holes
inline PxOsdMeshTopology
PxOsdMeshTopology_CreateWithHoles(
    const TfToken& scheme,
    const TfToken& orientation,
    const VtIntArray& faceVertexCounts,
    const VtIntArray& faceVertexIndices,
    const VtIntArray& holeIndices)
{
    return PxOsdMeshTopology(scheme, orientation, faceVertexCounts,
                             faceVertexIndices, holeIndices);
}

/// Create a mesh topology with subdiv tags
inline PxOsdMeshTopology
PxOsdMeshTopology_CreateWithSubdivTags(
    const TfToken& scheme,
    const TfToken& orientation,
    const VtIntArray& faceVertexCounts,
    const VtIntArray& faceVertexIndices,
    const PxOsdSubdivTags& subdivTags)
{
    return PxOsdMeshTopology(scheme, orientation, faceVertexCounts,
                             faceVertexIndices, subdivTags);
}

/// Create a mesh topology with holes and subdiv tags
inline PxOsdMeshTopology
PxOsdMeshTopology_CreateFull(
    const TfToken& scheme,
    const TfToken& orientation,
    const VtIntArray& faceVertexCounts,
    const VtIntArray& faceVertexIndices,
    const VtIntArray& holeIndices,
    const PxOsdSubdivTags& subdivTags)
{
    return PxOsdMeshTopology(scheme, orientation, faceVertexCounts,
                             faceVertexIndices, holeIndices, subdivTags);
}

/// Create subdiv tags with all parameters
inline PxOsdSubdivTags
PxOsdSubdivTags_Create(
    const TfToken& vertexInterpolationRule,
    const TfToken& faceVaryingInterpolationRule,
    const TfToken& creaseMethod,
    const TfToken& triangleSubdivision,
    const VtIntArray& creaseIndices,
    const VtIntArray& creaseLengths,
    const VtFloatArray& creaseWeights,
    const VtIntArray& cornerIndices,
    const VtFloatArray& cornerWeights)
{
    return PxOsdSubdivTags(
        vertexInterpolationRule,
        faceVaryingInterpolationRule,
        creaseMethod,
        triangleSubdivision,
        creaseIndices,
        creaseLengths,
        creaseWeights,
        cornerIndices,
        cornerWeights
    );
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_IMAGING_PX_OSD_SWIFT_INTEROP_H
