// pxOsd.cpp - PxOsd C wrapper implementation
// Mirrors: pxr/imaging/pxOsd/*.h (OpenSubdiv integration, subdivision surfaces)

#include "../../include/imaging/pxOsd.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define PXOSD_USE_USD 1
    #include <pxr/imaging/pxOsd/subdivTags.h>
    #include <pxr/imaging/pxOsd/meshTopology.h>
    #include <pxr/imaging/pxOsd/meshTopologyValidation.h>
    #include <pxr/imaging/pxOsd/tokens.h>
    #include <pxr/base/vt/array.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define PXOSD_USE_USD 0
#endif

#include <atomic>
#include <vector>
#include <string>
#include <cstring>

// ============================================================================
// MARK: - PxOsdSubdivTags Wrapper
// ============================================================================

struct PxOsdSubdivTagsOpaque {
    std::atomic<int> refCount;

#if PXOSD_USE_USD
    PxOsdSubdivTags tags;
#else
    // Standalone data
    PxOsdVertexInterpolationRule vertexInterpolationRule;
    PxOsdFaceVaryingInterpolationRule faceVaryingInterpolationRule;
    PxOsdCreaseMethod creaseMethod;
    PxOsdTriangleSubdivision triangleSubdivision;
    std::vector<int> creaseLengths;
    std::vector<int> creaseIndices;
    std::vector<float> creaseWeights;
    std::vector<int> cornerIndices;
    std::vector<float> cornerWeights;
#endif

    PxOsdSubdivTagsOpaque() : refCount(1) {
#if !PXOSD_USE_USD
        vertexInterpolationRule = PXOSD_VERTEX_INTERP_EDGE_AND_CORNER;
        faceVaryingInterpolationRule = PXOSD_FVAR_INTERP_CORNERS_PLUS1;
        creaseMethod = PXOSD_CREASE_UNIFORM;
        triangleSubdivision = PXOSD_TRI_SUBDIV_CATMULL_CLARK;
#endif
    }
};

// ============================================================================
// MARK: - PxOsdMeshTopology Wrapper
// ============================================================================

struct PxOsdMeshTopologyOpaque {
    std::atomic<int> refCount;

#if PXOSD_USE_USD
    PxOsdMeshTopology topology;
#else
    // Standalone data
    PxOsdSubdivScheme scheme;
    PxOsdOrientation orientation;
    std::vector<int> faceVertexCounts;
    std::vector<int> faceVertexIndices;
    std::vector<int> holeIndices;
    PxOsdSubdivTagsRef subdivTags; // May be null
#endif

    PxOsdMeshTopologyOpaque() : refCount(1) {
#if !PXOSD_USE_USD
        scheme = PXOSD_SCHEME_BILINEAR;
        orientation = PXOSD_ORIENTATION_RIGHT_HANDED;
        subdivTags = nullptr;
#endif
    }

#if !PXOSD_USE_USD
    ~PxOsdMeshTopologyOpaque() {
        if (subdivTags) {
            PxOsdSubdivTags_Release(subdivTags);
        }
    }
#endif
};

// ============================================================================
// MARK: - PxOsdMeshTopologyValidation Wrapper
// ============================================================================

struct PxOsdMeshTopologyValidationOpaque {
    std::atomic<int> refCount;
    bool isValid;
    std::vector<PxOsdValidationError> errors;

    PxOsdMeshTopologyValidationOpaque() : refCount(1), isValid(true) {}
};

// ============================================================================
// MARK: - Helper Functions (USD Mode)
// ============================================================================

#if PXOSD_USE_USD

static TfToken SchemeToToken(PxOsdSubdivScheme scheme) {
    switch (scheme) {
        case PXOSD_SCHEME_CATMULL_CLARK: return PxOsdOpenSubdivTokens->catmullClark;
        case PXOSD_SCHEME_LOOP: return PxOsdOpenSubdivTokens->loop;
        case PXOSD_SCHEME_BILINEAR: return PxOsdOpenSubdivTokens->bilinear;
        default: return PxOsdOpenSubdivTokens->bilinear;
    }
}

static PxOsdSubdivScheme TokenToScheme(const TfToken& token) {
    if (token == PxOsdOpenSubdivTokens->catmullClark) return PXOSD_SCHEME_CATMULL_CLARK;
    if (token == PxOsdOpenSubdivTokens->loop) return PXOSD_SCHEME_LOOP;
    return PXOSD_SCHEME_BILINEAR;
}

static TfToken OrientationToToken(PxOsdOrientation orientation) {
    switch (orientation) {
        case PXOSD_ORIENTATION_LEFT_HANDED: return PxOsdOpenSubdivTokens->leftHanded;
        case PXOSD_ORIENTATION_RIGHT_HANDED:
        default: return PxOsdOpenSubdivTokens->rightHanded;
    }
}

static PxOsdOrientation TokenToOrientation(const TfToken& token) {
    if (token == PxOsdOpenSubdivTokens->leftHanded) return PXOSD_ORIENTATION_LEFT_HANDED;
    return PXOSD_ORIENTATION_RIGHT_HANDED;
}

static TfToken VertexInterpToToken(PxOsdVertexInterpolationRule rule) {
    switch (rule) {
        case PXOSD_VERTEX_INTERP_NONE: return PxOsdOpenSubdivTokens->none;
        case PXOSD_VERTEX_INTERP_EDGE_ONLY: return PxOsdOpenSubdivTokens->edgeOnly;
        case PXOSD_VERTEX_INTERP_EDGE_AND_CORNER:
        default: return PxOsdOpenSubdivTokens->edgeAndCorner;
    }
}

static PxOsdVertexInterpolationRule TokenToVertexInterp(const TfToken& token) {
    if (token == PxOsdOpenSubdivTokens->none) return PXOSD_VERTEX_INTERP_NONE;
    if (token == PxOsdOpenSubdivTokens->edgeOnly) return PXOSD_VERTEX_INTERP_EDGE_ONLY;
    return PXOSD_VERTEX_INTERP_EDGE_AND_CORNER;
}

static TfToken FVarInterpToToken(PxOsdFaceVaryingInterpolationRule rule) {
    switch (rule) {
        case PXOSD_FVAR_INTERP_NONE: return PxOsdOpenSubdivTokens->none;
        case PXOSD_FVAR_INTERP_CORNERS_ONLY: return PxOsdOpenSubdivTokens->cornersOnly;
        case PXOSD_FVAR_INTERP_CORNERS_PLUS1: return PxOsdOpenSubdivTokens->cornersPlus1;
        case PXOSD_FVAR_INTERP_CORNERS_PLUS2: return PxOsdOpenSubdivTokens->cornersPlus2;
        case PXOSD_FVAR_INTERP_BOUNDARIES: return PxOsdOpenSubdivTokens->boundaries;
        case PXOSD_FVAR_INTERP_ALL:
        default: return PxOsdOpenSubdivTokens->all;
    }
}

static PxOsdFaceVaryingInterpolationRule TokenToFVarInterp(const TfToken& token) {
    if (token == PxOsdOpenSubdivTokens->none) return PXOSD_FVAR_INTERP_NONE;
    if (token == PxOsdOpenSubdivTokens->cornersOnly) return PXOSD_FVAR_INTERP_CORNERS_ONLY;
    if (token == PxOsdOpenSubdivTokens->cornersPlus1) return PXOSD_FVAR_INTERP_CORNERS_PLUS1;
    if (token == PxOsdOpenSubdivTokens->cornersPlus2) return PXOSD_FVAR_INTERP_CORNERS_PLUS2;
    if (token == PxOsdOpenSubdivTokens->boundaries) return PXOSD_FVAR_INTERP_BOUNDARIES;
    return PXOSD_FVAR_INTERP_ALL;
}

static TfToken CreaseMethodToToken(PxOsdCreaseMethod method) {
    switch (method) {
        case PXOSD_CREASE_CHAIKIN: return PxOsdOpenSubdivTokens->chaikin;
        case PXOSD_CREASE_UNIFORM:
        default: return PxOsdOpenSubdivTokens->uniform;
    }
}

static PxOsdCreaseMethod TokenToCreaseMethod(const TfToken& token) {
    if (token == PxOsdOpenSubdivTokens->chaikin) return PXOSD_CREASE_CHAIKIN;
    return PXOSD_CREASE_UNIFORM;
}

static TfToken TriSubdivToToken(PxOsdTriangleSubdivision rule) {
    switch (rule) {
        case PXOSD_TRI_SUBDIV_SMOOTH: return PxOsdOpenSubdivTokens->smooth;
        case PXOSD_TRI_SUBDIV_CATMULL_CLARK:
        default: return PxOsdOpenSubdivTokens->catmullClark;
    }
}

static PxOsdTriangleSubdivision TokenToTriSubdiv(const TfToken& token) {
    if (token == PxOsdOpenSubdivTokens->smooth) return PXOSD_TRI_SUBDIV_SMOOTH;
    return PXOSD_TRI_SUBDIV_CATMULL_CLARK;
}

static VtIntArray ToVtIntArray(const int* data, size_t count) {
    VtIntArray arr(count);
    for (size_t i = 0; i < count; ++i) {
        arr[i] = data[i];
    }
    return arr;
}

static VtFloatArray ToVtFloatArray(const float* data, size_t count) {
    VtFloatArray arr(count);
    for (size_t i = 0; i < count; ++i) {
        arr[i] = data[i];
    }
    return arr;
}

#endif // PXOSD_USE_USD

// ============================================================================
// MARK: - Token String Tables
// ============================================================================

static const char* const g_schemeStrings[] = {
    "catmullClark",
    "loop",
    "bilinear"
};

static const char* const g_orientationStrings[] = {
    "rightHanded",
    "leftHanded"
};

static const char* const g_vertexInterpStrings[] = {
    "none",
    "edgeOnly",
    "edgeAndCorner"
};

static const char* const g_fvarInterpStrings[] = {
    "none",
    "cornersOnly",
    "cornersPlus1",
    "cornersPlus2",
    "boundaries",
    "all"
};

static const char* const g_creaseMethodStrings[] = {
    "uniform",
    "chaikin"
};

static const char* const g_triSubdivStrings[] = {
    "catmullClark",
    "smooth"
};

extern "C" {

// ============================================================================
// MARK: - PxOsdSubdivTags Lifecycle
// ============================================================================

PxOsdSubdivTagsRef PxOsdSubdivTags_Create(void) {
    try {
        return new PxOsdSubdivTagsOpaque();
    } catch (...) {
        return nullptr;
    }
}

PxOsdSubdivTagsRef PxOsdSubdivTags_Copy(PxOsdSubdivTagsRef tags) {
    if (!tags) return nullptr;
    try {
        auto* copy = new PxOsdSubdivTagsOpaque();
#if PXOSD_USE_USD
        copy->tags = tags->tags;
#else
        copy->vertexInterpolationRule = tags->vertexInterpolationRule;
        copy->faceVaryingInterpolationRule = tags->faceVaryingInterpolationRule;
        copy->creaseMethod = tags->creaseMethod;
        copy->triangleSubdivision = tags->triangleSubdivision;
        copy->creaseLengths = tags->creaseLengths;
        copy->creaseIndices = tags->creaseIndices;
        copy->creaseWeights = tags->creaseWeights;
        copy->cornerIndices = tags->cornerIndices;
        copy->cornerWeights = tags->cornerWeights;
#endif
        return copy;
    } catch (...) {
        return nullptr;
    }
}

PxOsdSubdivTagsRef PxOsdSubdivTags_Retain(PxOsdSubdivTagsRef tags) {
    if (tags) {
        tags->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return tags;
}

void PxOsdSubdivTags_Release(PxOsdSubdivTagsRef tags) {
    if (tags) {
        if (tags->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete tags;
        }
    }
}

// ============================================================================
// MARK: - PxOsdSubdivTags Interpolation Rules
// ============================================================================

PxOsdVertexInterpolationRule PxOsdSubdivTags_GetVertexInterpolationRule(PxOsdSubdivTagsRef tags) {
    if (!tags) return PXOSD_VERTEX_INTERP_EDGE_AND_CORNER;
#if PXOSD_USE_USD
    return TokenToVertexInterp(tags->tags.GetVertexInterpolationRule());
#else
    return tags->vertexInterpolationRule;
#endif
}

void PxOsdSubdivTags_SetVertexInterpolationRule(PxOsdSubdivTagsRef tags, PxOsdVertexInterpolationRule rule) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetVertexInterpolationRule(VertexInterpToToken(rule));
#else
    tags->vertexInterpolationRule = rule;
#endif
}

PxOsdFaceVaryingInterpolationRule PxOsdSubdivTags_GetFaceVaryingInterpolationRule(PxOsdSubdivTagsRef tags) {
    if (!tags) return PXOSD_FVAR_INTERP_CORNERS_PLUS1;
#if PXOSD_USE_USD
    return TokenToFVarInterp(tags->tags.GetFaceVaryingInterpolationRule());
#else
    return tags->faceVaryingInterpolationRule;
#endif
}

void PxOsdSubdivTags_SetFaceVaryingInterpolationRule(PxOsdSubdivTagsRef tags, PxOsdFaceVaryingInterpolationRule rule) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetFaceVaryingInterpolationRule(FVarInterpToToken(rule));
#else
    tags->faceVaryingInterpolationRule = rule;
#endif
}

PxOsdCreaseMethod PxOsdSubdivTags_GetCreaseMethod(PxOsdSubdivTagsRef tags) {
    if (!tags) return PXOSD_CREASE_UNIFORM;
#if PXOSD_USE_USD
    return TokenToCreaseMethod(tags->tags.GetCreaseMethod());
#else
    return tags->creaseMethod;
#endif
}

void PxOsdSubdivTags_SetCreaseMethod(PxOsdSubdivTagsRef tags, PxOsdCreaseMethod method) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetCreaseMethod(CreaseMethodToToken(method));
#else
    tags->creaseMethod = method;
#endif
}

PxOsdTriangleSubdivision PxOsdSubdivTags_GetTriangleSubdivision(PxOsdSubdivTagsRef tags) {
    if (!tags) return PXOSD_TRI_SUBDIV_CATMULL_CLARK;
#if PXOSD_USE_USD
    return TokenToTriSubdiv(tags->tags.GetTriangleSubdivision());
#else
    return tags->triangleSubdivision;
#endif
}

void PxOsdSubdivTags_SetTriangleSubdivision(PxOsdSubdivTagsRef tags, PxOsdTriangleSubdivision rule) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetTriangleSubdivision(TriSubdivToToken(rule));
#else
    tags->triangleSubdivision = rule;
#endif
}

// ============================================================================
// MARK: - PxOsdSubdivTags Crease Data
// ============================================================================

size_t PxOsdSubdivTags_GetCreaseLengthCount(PxOsdSubdivTagsRef tags) {
    if (!tags) return 0;
#if PXOSD_USE_USD
    return tags->tags.GetCreaseLengths().size();
#else
    return tags->creaseLengths.size();
#endif
}

size_t PxOsdSubdivTags_GetCreaseLengths(PxOsdSubdivTagsRef tags, int* outLengths, size_t maxCount) {
    if (!tags || !outLengths || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtIntArray& lengths = tags->tags.GetCreaseLengths();
    size_t toCopy = (maxCount < lengths.size()) ? maxCount : lengths.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outLengths[i] = lengths[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < tags->creaseLengths.size()) ? maxCount : tags->creaseLengths.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outLengths[i] = tags->creaseLengths[i];
    }
    return toCopy;
#endif
}

void PxOsdSubdivTags_SetCreaseLengths(PxOsdSubdivTagsRef tags, const int* lengths, size_t count) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetCreaseLengths(ToVtIntArray(lengths, count));
#else
    tags->creaseLengths.assign(lengths, lengths + count);
#endif
}

size_t PxOsdSubdivTags_GetCreaseIndexCount(PxOsdSubdivTagsRef tags) {
    if (!tags) return 0;
#if PXOSD_USE_USD
    return tags->tags.GetCreaseIndices().size();
#else
    return tags->creaseIndices.size();
#endif
}

size_t PxOsdSubdivTags_GetCreaseIndices(PxOsdSubdivTagsRef tags, int* outIndices, size_t maxCount) {
    if (!tags || !outIndices || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtIntArray& indices = tags->tags.GetCreaseIndices();
    size_t toCopy = (maxCount < indices.size()) ? maxCount : indices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = indices[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < tags->creaseIndices.size()) ? maxCount : tags->creaseIndices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = tags->creaseIndices[i];
    }
    return toCopy;
#endif
}

void PxOsdSubdivTags_SetCreaseIndices(PxOsdSubdivTagsRef tags, const int* indices, size_t count) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetCreaseIndices(ToVtIntArray(indices, count));
#else
    tags->creaseIndices.assign(indices, indices + count);
#endif
}

size_t PxOsdSubdivTags_GetCreaseWeightCount(PxOsdSubdivTagsRef tags) {
    if (!tags) return 0;
#if PXOSD_USE_USD
    return tags->tags.GetCreaseWeights().size();
#else
    return tags->creaseWeights.size();
#endif
}

size_t PxOsdSubdivTags_GetCreaseWeights(PxOsdSubdivTagsRef tags, float* outWeights, size_t maxCount) {
    if (!tags || !outWeights || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtFloatArray& weights = tags->tags.GetCreaseWeights();
    size_t toCopy = (maxCount < weights.size()) ? maxCount : weights.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outWeights[i] = weights[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < tags->creaseWeights.size()) ? maxCount : tags->creaseWeights.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outWeights[i] = tags->creaseWeights[i];
    }
    return toCopy;
#endif
}

void PxOsdSubdivTags_SetCreaseWeights(PxOsdSubdivTagsRef tags, const float* weights, size_t count) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetCreaseWeights(ToVtFloatArray(weights, count));
#else
    tags->creaseWeights.assign(weights, weights + count);
#endif
}

// ============================================================================
// MARK: - PxOsdSubdivTags Corner Data
// ============================================================================

size_t PxOsdSubdivTags_GetCornerIndexCount(PxOsdSubdivTagsRef tags) {
    if (!tags) return 0;
#if PXOSD_USE_USD
    return tags->tags.GetCornerIndices().size();
#else
    return tags->cornerIndices.size();
#endif
}

size_t PxOsdSubdivTags_GetCornerIndices(PxOsdSubdivTagsRef tags, int* outIndices, size_t maxCount) {
    if (!tags || !outIndices || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtIntArray& indices = tags->tags.GetCornerIndices();
    size_t toCopy = (maxCount < indices.size()) ? maxCount : indices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = indices[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < tags->cornerIndices.size()) ? maxCount : tags->cornerIndices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = tags->cornerIndices[i];
    }
    return toCopy;
#endif
}

void PxOsdSubdivTags_SetCornerIndices(PxOsdSubdivTagsRef tags, const int* indices, size_t count) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetCornerIndices(ToVtIntArray(indices, count));
#else
    tags->cornerIndices.assign(indices, indices + count);
#endif
}

size_t PxOsdSubdivTags_GetCornerWeightCount(PxOsdSubdivTagsRef tags) {
    if (!tags) return 0;
#if PXOSD_USE_USD
    return tags->tags.GetCornerWeights().size();
#else
    return tags->cornerWeights.size();
#endif
}

size_t PxOsdSubdivTags_GetCornerWeights(PxOsdSubdivTagsRef tags, float* outWeights, size_t maxCount) {
    if (!tags || !outWeights || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtFloatArray& weights = tags->tags.GetCornerWeights();
    size_t toCopy = (maxCount < weights.size()) ? maxCount : weights.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outWeights[i] = weights[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < tags->cornerWeights.size()) ? maxCount : tags->cornerWeights.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outWeights[i] = tags->cornerWeights[i];
    }
    return toCopy;
#endif
}

void PxOsdSubdivTags_SetCornerWeights(PxOsdSubdivTagsRef tags, const float* weights, size_t count) {
    if (!tags) return;
#if PXOSD_USE_USD
    tags->tags.SetCornerWeights(ToVtFloatArray(weights, count));
#else
    tags->cornerWeights.assign(weights, weights + count);
#endif
}

// ============================================================================
// MARK: - PxOsdSubdivTags Comparison and Hashing
// ============================================================================

size_t PxOsdSubdivTags_ComputeHash(PxOsdSubdivTagsRef tags) {
    if (!tags) return 0;
#if PXOSD_USE_USD
    return tags->tags.ComputeHash();
#else
    // Simple hash combining for standalone mode
    size_t hash = 17;
    hash = hash * 31 + static_cast<size_t>(tags->vertexInterpolationRule);
    hash = hash * 31 + static_cast<size_t>(tags->faceVaryingInterpolationRule);
    hash = hash * 31 + static_cast<size_t>(tags->creaseMethod);
    hash = hash * 31 + static_cast<size_t>(tags->triangleSubdivision);
    hash = hash * 31 + tags->creaseLengths.size();
    hash = hash * 31 + tags->creaseIndices.size();
    hash = hash * 31 + tags->creaseWeights.size();
    hash = hash * 31 + tags->cornerIndices.size();
    hash = hash * 31 + tags->cornerWeights.size();
    return hash;
#endif
}

bool PxOsdSubdivTags_Equal(PxOsdSubdivTagsRef a, PxOsdSubdivTagsRef b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
#if PXOSD_USE_USD
    return a->tags == b->tags;
#else
    return a->vertexInterpolationRule == b->vertexInterpolationRule &&
           a->faceVaryingInterpolationRule == b->faceVaryingInterpolationRule &&
           a->creaseMethod == b->creaseMethod &&
           a->triangleSubdivision == b->triangleSubdivision &&
           a->creaseLengths == b->creaseLengths &&
           a->creaseIndices == b->creaseIndices &&
           a->creaseWeights == b->creaseWeights &&
           a->cornerIndices == b->cornerIndices &&
           a->cornerWeights == b->cornerWeights;
#endif
}

// ============================================================================
// MARK: - PxOsdMeshTopology Lifecycle
// ============================================================================

PxOsdMeshTopologyRef PxOsdMeshTopology_Create(void) {
    try {
        return new PxOsdMeshTopologyOpaque();
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_CreateWithTopology(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices)
{
    try {
        auto* topo = new PxOsdMeshTopologyOpaque();
#if PXOSD_USE_USD
        topo->topology = PxOsdMeshTopology(
            SchemeToToken(scheme),
            OrientationToToken(orientation),
            ToVtIntArray(faceVertexCounts, numFaces),
            ToVtIntArray(faceVertexIndices, numIndices)
        );
#else
        topo->scheme = scheme;
        topo->orientation = orientation;
        topo->faceVertexCounts.assign(faceVertexCounts, faceVertexCounts + numFaces);
        topo->faceVertexIndices.assign(faceVertexIndices, faceVertexIndices + numIndices);
#endif
        return topo;
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_CreateWithSubdivTags(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices,
    PxOsdSubdivTagsRef subdivTags)
{
    try {
        auto* topo = new PxOsdMeshTopologyOpaque();
#if PXOSD_USE_USD
        if (subdivTags) {
            topo->topology = PxOsdMeshTopology(
                SchemeToToken(scheme),
                OrientationToToken(orientation),
                ToVtIntArray(faceVertexCounts, numFaces),
                ToVtIntArray(faceVertexIndices, numIndices),
                subdivTags->tags
            );
        } else {
            topo->topology = PxOsdMeshTopology(
                SchemeToToken(scheme),
                OrientationToToken(orientation),
                ToVtIntArray(faceVertexCounts, numFaces),
                ToVtIntArray(faceVertexIndices, numIndices)
            );
        }
#else
        topo->scheme = scheme;
        topo->orientation = orientation;
        topo->faceVertexCounts.assign(faceVertexCounts, faceVertexCounts + numFaces);
        topo->faceVertexIndices.assign(faceVertexIndices, faceVertexIndices + numIndices);
        if (subdivTags) {
            topo->subdivTags = PxOsdSubdivTags_Copy(subdivTags);
        }
#endif
        return topo;
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_CreateWithHoles(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices,
    const int* holeIndices,
    size_t numHoles)
{
    try {
        auto* topo = new PxOsdMeshTopologyOpaque();
#if PXOSD_USE_USD
        topo->topology = PxOsdMeshTopology(
            SchemeToToken(scheme),
            OrientationToToken(orientation),
            ToVtIntArray(faceVertexCounts, numFaces),
            ToVtIntArray(faceVertexIndices, numIndices),
            ToVtIntArray(holeIndices, numHoles)
        );
#else
        topo->scheme = scheme;
        topo->orientation = orientation;
        topo->faceVertexCounts.assign(faceVertexCounts, faceVertexCounts + numFaces);
        topo->faceVertexIndices.assign(faceVertexIndices, faceVertexIndices + numIndices);
        topo->holeIndices.assign(holeIndices, holeIndices + numHoles);
#endif
        return topo;
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_CreateFull(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices,
    const int* holeIndices,
    size_t numHoles,
    PxOsdSubdivTagsRef subdivTags)
{
    try {
        auto* topo = new PxOsdMeshTopologyOpaque();
#if PXOSD_USE_USD
        if (subdivTags) {
            topo->topology = PxOsdMeshTopology(
                SchemeToToken(scheme),
                OrientationToToken(orientation),
                ToVtIntArray(faceVertexCounts, numFaces),
                ToVtIntArray(faceVertexIndices, numIndices),
                ToVtIntArray(holeIndices, numHoles),
                subdivTags->tags
            );
        } else {
            topo->topology = PxOsdMeshTopology(
                SchemeToToken(scheme),
                OrientationToToken(orientation),
                ToVtIntArray(faceVertexCounts, numFaces),
                ToVtIntArray(faceVertexIndices, numIndices),
                ToVtIntArray(holeIndices, numHoles)
            );
        }
#else
        topo->scheme = scheme;
        topo->orientation = orientation;
        topo->faceVertexCounts.assign(faceVertexCounts, faceVertexCounts + numFaces);
        topo->faceVertexIndices.assign(faceVertexIndices, faceVertexIndices + numIndices);
        topo->holeIndices.assign(holeIndices, holeIndices + numHoles);
        if (subdivTags) {
            topo->subdivTags = PxOsdSubdivTags_Copy(subdivTags);
        }
#endif
        return topo;
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_Copy(PxOsdMeshTopologyRef topology) {
    if (!topology) return nullptr;
    try {
        auto* copy = new PxOsdMeshTopologyOpaque();
#if PXOSD_USE_USD
        copy->topology = topology->topology;
#else
        copy->scheme = topology->scheme;
        copy->orientation = topology->orientation;
        copy->faceVertexCounts = topology->faceVertexCounts;
        copy->faceVertexIndices = topology->faceVertexIndices;
        copy->holeIndices = topology->holeIndices;
        if (topology->subdivTags) {
            copy->subdivTags = PxOsdSubdivTags_Copy(topology->subdivTags);
        }
#endif
        return copy;
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_Retain(PxOsdMeshTopologyRef topology) {
    if (topology) {
        topology->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return topology;
}

void PxOsdMeshTopology_Release(PxOsdMeshTopologyRef topology) {
    if (topology) {
        if (topology->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete topology;
        }
    }
}

// ============================================================================
// MARK: - PxOsdMeshTopology Properties
// ============================================================================

PxOsdSubdivScheme PxOsdMeshTopology_GetScheme(PxOsdMeshTopologyRef topology) {
    if (!topology) return PXOSD_SCHEME_BILINEAR;
#if PXOSD_USE_USD
    return TokenToScheme(topology->topology.GetScheme());
#else
    return topology->scheme;
#endif
}

PxOsdOrientation PxOsdMeshTopology_GetOrientation(PxOsdMeshTopologyRef topology) {
    if (!topology) return PXOSD_ORIENTATION_RIGHT_HANDED;
#if PXOSD_USE_USD
    return TokenToOrientation(topology->topology.GetOrientation());
#else
    return topology->orientation;
#endif
}

size_t PxOsdMeshTopology_GetFaceCount(PxOsdMeshTopologyRef topology) {
    if (!topology) return 0;
#if PXOSD_USE_USD
    return topology->topology.GetFaceVertexCounts().size();
#else
    return topology->faceVertexCounts.size();
#endif
}

size_t PxOsdMeshTopology_GetFaceVertexCounts(PxOsdMeshTopologyRef topology, int* outCounts, size_t maxCount) {
    if (!topology || !outCounts || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtIntArray& counts = topology->topology.GetFaceVertexCounts();
    size_t toCopy = (maxCount < counts.size()) ? maxCount : counts.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outCounts[i] = counts[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < topology->faceVertexCounts.size()) ? maxCount : topology->faceVertexCounts.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outCounts[i] = topology->faceVertexCounts[i];
    }
    return toCopy;
#endif
}

size_t PxOsdMeshTopology_GetFaceVertexIndexCount(PxOsdMeshTopologyRef topology) {
    if (!topology) return 0;
#if PXOSD_USE_USD
    return topology->topology.GetFaceVertexIndices().size();
#else
    return topology->faceVertexIndices.size();
#endif
}

size_t PxOsdMeshTopology_GetFaceVertexIndices(PxOsdMeshTopologyRef topology, int* outIndices, size_t maxCount) {
    if (!topology || !outIndices || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtIntArray& indices = topology->topology.GetFaceVertexIndices();
    size_t toCopy = (maxCount < indices.size()) ? maxCount : indices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = indices[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < topology->faceVertexIndices.size()) ? maxCount : topology->faceVertexIndices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = topology->faceVertexIndices[i];
    }
    return toCopy;
#endif
}

size_t PxOsdMeshTopology_GetHoleIndexCount(PxOsdMeshTopologyRef topology) {
    if (!topology) return 0;
#if PXOSD_USE_USD
    return topology->topology.GetHoleIndices().size();
#else
    return topology->holeIndices.size();
#endif
}

size_t PxOsdMeshTopology_GetHoleIndices(PxOsdMeshTopologyRef topology, int* outIndices, size_t maxCount) {
    if (!topology || !outIndices || maxCount == 0) return 0;
#if PXOSD_USE_USD
    const VtIntArray& indices = topology->topology.GetHoleIndices();
    size_t toCopy = (maxCount < indices.size()) ? maxCount : indices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = indices[i];
    }
    return toCopy;
#else
    size_t toCopy = (maxCount < topology->holeIndices.size()) ? maxCount : topology->holeIndices.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outIndices[i] = topology->holeIndices[i];
    }
    return toCopy;
#endif
}

PxOsdSubdivTagsRef PxOsdMeshTopology_GetSubdivTags(PxOsdMeshTopologyRef topology) {
    if (!topology) return nullptr;
    try {
#if PXOSD_USE_USD
        auto* tags = new PxOsdSubdivTagsOpaque();
        tags->tags = topology->topology.GetSubdivTags();
        return tags;
#else
        if (topology->subdivTags) {
            return PxOsdSubdivTags_Copy(topology->subdivTags);
        }
        return nullptr;
#endif
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - PxOsdMeshTopology Builder Methods
// ============================================================================

PxOsdMeshTopologyRef PxOsdMeshTopology_WithScheme(PxOsdMeshTopologyRef topology, PxOsdSubdivScheme scheme) {
    if (!topology) return nullptr;
    try {
#if PXOSD_USE_USD
        auto* copy = new PxOsdMeshTopologyOpaque();
        copy->topology = topology->topology.WithScheme(SchemeToToken(scheme));
        return copy;
#else
        auto* copy = PxOsdMeshTopology_Copy(topology);
        if (copy) {
            copy->scheme = scheme;
        }
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_WithOrientation(PxOsdMeshTopologyRef topology, PxOsdOrientation orientation) {
    if (!topology) return nullptr;
    try {
#if PXOSD_USE_USD
        auto* copy = new PxOsdMeshTopologyOpaque();
        copy->topology = topology->topology.WithOrientation(OrientationToToken(orientation));
        return copy;
#else
        auto* copy = PxOsdMeshTopology_Copy(topology);
        if (copy) {
            copy->orientation = orientation;
        }
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_WithSubdivTags(PxOsdMeshTopologyRef topology, PxOsdSubdivTagsRef subdivTags) {
    if (!topology) return nullptr;
    try {
#if PXOSD_USE_USD
        auto* copy = new PxOsdMeshTopologyOpaque();
        if (subdivTags) {
            copy->topology = topology->topology.WithSubdivTags(subdivTags->tags);
        } else {
            copy->topology = topology->topology.WithSubdivTags(PxOsdSubdivTags());
        }
        return copy;
#else
        auto* copy = PxOsdMeshTopology_Copy(topology);
        if (copy) {
            if (copy->subdivTags) {
                PxOsdSubdivTags_Release(copy->subdivTags);
                copy->subdivTags = nullptr;
            }
            if (subdivTags) {
                copy->subdivTags = PxOsdSubdivTags_Copy(subdivTags);
            }
        }
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

PxOsdMeshTopologyRef PxOsdMeshTopology_WithHoleIndices(PxOsdMeshTopologyRef topology, const int* holeIndices, size_t numHoles) {
    if (!topology) return nullptr;
    try {
#if PXOSD_USE_USD
        auto* copy = new PxOsdMeshTopologyOpaque();
        copy->topology = topology->topology.WithHoleIndices(ToVtIntArray(holeIndices, numHoles));
        return copy;
#else
        auto* copy = PxOsdMeshTopology_Copy(topology);
        if (copy) {
            copy->holeIndices.assign(holeIndices, holeIndices + numHoles);
        }
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - PxOsdMeshTopology Validation
// ============================================================================

PxOsdMeshTopologyValidationRef PxOsdMeshTopology_Validate(PxOsdMeshTopologyRef topology) {
    if (!topology) return nullptr;
    try {
        auto* validation = new PxOsdMeshTopologyValidationOpaque();
#if PXOSD_USE_USD
        PxOsdMeshTopologyValidation result = topology->topology.Validate();
        validation->isValid = static_cast<bool>(result);
        if (!validation->isValid) {
            for (const auto& invalidation : result) {
                PxOsdValidationError error;
                // Map the code
                error.code = static_cast<PxOsdValidationCode>(invalidation.code);
                std::strncpy(error.message, invalidation.message.c_str(), sizeof(error.message) - 1);
                error.message[sizeof(error.message) - 1] = '\0';
                validation->errors.push_back(error);
            }
        }
#else
        // Standalone validation - basic checks
        validation->isValid = true;

        // Check face vertex counts are positive
        for (int count : topology->faceVertexCounts) {
            if (count < 3) {
                validation->isValid = false;
                PxOsdValidationError error;
                error.code = PXOSD_VALIDATION_INVALID_FACE_VERTEX_COUNTS_ELEMENT;
                std::snprintf(error.message, sizeof(error.message),
                    "Face vertex count %d is less than 3", count);
                validation->errors.push_back(error);
            }
        }

        // Check total indices match
        size_t expectedIndices = 0;
        for (int count : topology->faceVertexCounts) {
            expectedIndices += static_cast<size_t>(count);
        }
        if (expectedIndices != topology->faceVertexIndices.size()) {
            validation->isValid = false;
            PxOsdValidationError error;
            error.code = PXOSD_VALIDATION_INVALID_FACE_VERTEX_INDICES_SIZE;
            std::snprintf(error.message, sizeof(error.message),
                "Face vertex indices size %zu doesn't match expected %zu",
                topology->faceVertexIndices.size(), expectedIndices);
            validation->errors.push_back(error);
        }

        // Check hole indices are valid face indices
        for (int holeIdx : topology->holeIndices) {
            if (holeIdx < 0 || static_cast<size_t>(holeIdx) >= topology->faceVertexCounts.size()) {
                validation->isValid = false;
                PxOsdValidationError error;
                error.code = PXOSD_VALIDATION_INVALID_HOLE_INDICES_ELEMENT;
                std::snprintf(error.message, sizeof(error.message),
                    "Hole index %d is out of range [0, %zu)",
                    holeIdx, topology->faceVertexCounts.size());
                validation->errors.push_back(error);
            }
        }
#endif
        return validation;
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - PxOsdMeshTopology Comparison and Hashing
// ============================================================================

size_t PxOsdMeshTopology_ComputeHash(PxOsdMeshTopologyRef topology) {
    if (!topology) return 0;
#if PXOSD_USE_USD
    return topology->topology.ComputeHash();
#else
    // Simple hash combining for standalone mode
    size_t hash = 17;
    hash = hash * 31 + static_cast<size_t>(topology->scheme);
    hash = hash * 31 + static_cast<size_t>(topology->orientation);
    hash = hash * 31 + topology->faceVertexCounts.size();
    hash = hash * 31 + topology->faceVertexIndices.size();
    hash = hash * 31 + topology->holeIndices.size();
    if (topology->subdivTags) {
        hash = hash * 31 + PxOsdSubdivTags_ComputeHash(topology->subdivTags);
    }
    return hash;
#endif
}

bool PxOsdMeshTopology_Equal(PxOsdMeshTopologyRef a, PxOsdMeshTopologyRef b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
#if PXOSD_USE_USD
    return a->topology == b->topology;
#else
    if (a->scheme != b->scheme) return false;
    if (a->orientation != b->orientation) return false;
    if (a->faceVertexCounts != b->faceVertexCounts) return false;
    if (a->faceVertexIndices != b->faceVertexIndices) return false;
    if (a->holeIndices != b->holeIndices) return false;
    // Compare subdiv tags
    if (!a->subdivTags && !b->subdivTags) return true;
    if (!a->subdivTags || !b->subdivTags) return false;
    return PxOsdSubdivTags_Equal(a->subdivTags, b->subdivTags);
#endif
}

// ============================================================================
// MARK: - PxOsdMeshTopologyValidation Lifecycle
// ============================================================================

PxOsdMeshTopologyValidationRef PxOsdMeshTopologyValidation_Retain(PxOsdMeshTopologyValidationRef validation) {
    if (validation) {
        validation->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return validation;
}

void PxOsdMeshTopologyValidation_Release(PxOsdMeshTopologyValidationRef validation) {
    if (validation) {
        if (validation->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete validation;
        }
    }
}

// ============================================================================
// MARK: - PxOsdMeshTopologyValidation Properties
// ============================================================================

bool PxOsdMeshTopologyValidation_IsValid(PxOsdMeshTopologyValidationRef validation) {
    if (!validation) return false;
    return validation->isValid;
}

size_t PxOsdMeshTopologyValidation_GetErrorCount(PxOsdMeshTopologyValidationRef validation) {
    if (!validation) return 0;
    return validation->errors.size();
}

size_t PxOsdMeshTopologyValidation_GetErrors(PxOsdMeshTopologyValidationRef validation, PxOsdValidationError* outErrors, size_t maxCount) {
    if (!validation || !outErrors || maxCount == 0) return 0;
    size_t toCopy = (maxCount < validation->errors.size()) ? maxCount : validation->errors.size();
    for (size_t i = 0; i < toCopy; ++i) {
        outErrors[i] = validation->errors[i];
    }
    return toCopy;
}

// ============================================================================
// MARK: - PxOsd Token Strings
// ============================================================================

const char* PxOsd_GetSchemeString(PxOsdSubdivScheme scheme) {
    if (scheme >= 0 && scheme <= 2) {
        return g_schemeStrings[scheme];
    }
    return "bilinear";
}

const char* PxOsd_GetOrientationString(PxOsdOrientation orientation) {
    if (orientation >= 0 && orientation <= 1) {
        return g_orientationStrings[orientation];
    }
    return "rightHanded";
}

const char* PxOsd_GetVertexInterpolationRuleString(PxOsdVertexInterpolationRule rule) {
    if (rule >= 0 && rule <= 2) {
        return g_vertexInterpStrings[rule];
    }
    return "edgeAndCorner";
}

const char* PxOsd_GetFaceVaryingInterpolationRuleString(PxOsdFaceVaryingInterpolationRule rule) {
    if (rule >= 0 && rule <= 5) {
        return g_fvarInterpStrings[rule];
    }
    return "cornersPlus1";
}

const char* PxOsd_GetCreaseMethodString(PxOsdCreaseMethod method) {
    if (method >= 0 && method <= 1) {
        return g_creaseMethodStrings[method];
    }
    return "uniform";
}

const char* PxOsd_GetTriangleSubdivisionString(PxOsdTriangleSubdivision rule) {
    if (rule >= 0 && rule <= 1) {
        return g_triSubdivStrings[rule];
    }
    return "catmullClark";
}

} // extern "C"
