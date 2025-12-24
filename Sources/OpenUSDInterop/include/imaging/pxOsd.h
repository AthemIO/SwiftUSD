// pxOsd.h - PxOsd C wrapper
// Mirrors: pxr/imaging/pxOsd/*.h (OpenSubdiv integration, subdivision surfaces)

#ifndef OPENUSDINTEROP_IMAGING_PXOSD_H
#define OPENUSDINTEROP_IMAGING_PXOSD_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

/// Opaque handle to PxOsdSubdivTags (subdivision surface parameters).
typedef struct PxOsdSubdivTagsOpaque* PxOsdSubdivTagsRef;

/// Opaque handle to PxOsdMeshTopology (mesh connectivity and subdivision data).
typedef struct PxOsdMeshTopologyOpaque* PxOsdMeshTopologyRef;

/// Opaque handle to PxOsdMeshTopologyValidation (validation result).
typedef struct PxOsdMeshTopologyValidationOpaque* PxOsdMeshTopologyValidationRef;

// ============================================================================
// MARK: - PxOsd Tokens (Subdivision Schemes and Options)
// ============================================================================

/// Subdivision scheme tokens.
/// Mirrors PxOsdOpenSubdivTokens for subdivision schemes.
typedef enum {
    PXOSD_SCHEME_CATMULL_CLARK = 0,
    PXOSD_SCHEME_LOOP = 1,
    PXOSD_SCHEME_BILINEAR = 2
} PxOsdSubdivScheme;

/// Vertex interpolation rule tokens.
/// Controls how boundaries are interpolated during subdivision.
typedef enum {
    PXOSD_VERTEX_INTERP_NONE = 0,
    PXOSD_VERTEX_INTERP_EDGE_ONLY = 1,
    PXOSD_VERTEX_INTERP_EDGE_AND_CORNER = 2
} PxOsdVertexInterpolationRule;

/// Face-varying interpolation rule tokens.
/// Controls how face-varying data (like UVs) is interpolated.
typedef enum {
    PXOSD_FVAR_INTERP_NONE = 0,
    PXOSD_FVAR_INTERP_CORNERS_ONLY = 1,
    PXOSD_FVAR_INTERP_CORNERS_PLUS1 = 2,
    PXOSD_FVAR_INTERP_CORNERS_PLUS2 = 3,
    PXOSD_FVAR_INTERP_BOUNDARIES = 4,
    PXOSD_FVAR_INTERP_ALL = 5
} PxOsdFaceVaryingInterpolationRule;

/// Crease method tokens.
/// Controls how creases are computed.
typedef enum {
    PXOSD_CREASE_UNIFORM = 0,
    PXOSD_CREASE_CHAIKIN = 1
} PxOsdCreaseMethod;

/// Triangle subdivision rule tokens.
typedef enum {
    PXOSD_TRI_SUBDIV_CATMULL_CLARK = 0,
    PXOSD_TRI_SUBDIV_SMOOTH = 1
} PxOsdTriangleSubdivision;

/// Mesh orientation tokens.
typedef enum {
    PXOSD_ORIENTATION_RIGHT_HANDED = 0,
    PXOSD_ORIENTATION_LEFT_HANDED = 1
} PxOsdOrientation;

// ============================================================================
// MARK: - Validation Error Codes
// ============================================================================

/// Validation error codes for mesh topology.
/// Mirrors PxOsdMeshTopologyValidation::Code.
typedef enum {
    PXOSD_VALIDATION_OK = 0,
    PXOSD_VALIDATION_INVALID_SCHEME,
    PXOSD_VALIDATION_INVALID_ORIENTATION,
    PXOSD_VALIDATION_INVALID_TRIANGLE_SUBDIVISION,
    PXOSD_VALIDATION_INVALID_VERTEX_INTERPOLATION_RULE,
    PXOSD_VALIDATION_INVALID_FVAR_INTERPOLATION_RULE,
    PXOSD_VALIDATION_INVALID_CREASE_METHOD,
    PXOSD_VALIDATION_INVALID_CREASE_LENGTH_ELEMENT,
    PXOSD_VALIDATION_INVALID_CREASE_INDICES_SIZE,
    PXOSD_VALIDATION_INVALID_CREASE_INDICES_ELEMENT,
    PXOSD_VALIDATION_INVALID_CREASE_WEIGHTS_SIZE,
    PXOSD_VALIDATION_NEGATIVE_CREASE_WEIGHTS,
    PXOSD_VALIDATION_INVALID_CORNER_INDICES_ELEMENT,
    PXOSD_VALIDATION_INVALID_CORNER_WEIGHTS_SIZE,
    PXOSD_VALIDATION_NEGATIVE_CORNER_WEIGHTS,
    PXOSD_VALIDATION_INVALID_HOLE_INDICES_ELEMENT,
    PXOSD_VALIDATION_INVALID_FACE_VERTEX_COUNTS_ELEMENT,
    PXOSD_VALIDATION_INVALID_FACE_VERTEX_INDICES_SIZE,
    PXOSD_VALIDATION_INVALID_FACE_VERTEX_INDICES_ELEMENT
} PxOsdValidationCode;

/// A single validation error with code and message.
typedef struct {
    PxOsdValidationCode code;
    char message[256];
} PxOsdValidationError;

// ============================================================================
// MARK: - PxOsdSubdivTags Lifecycle
// ============================================================================

/// Creates an empty PxOsdSubdivTags with default values.
USD_INTEROP_API PxOsdSubdivTagsRef PxOsdSubdivTags_Create(void);

/// Creates a copy of existing subdiv tags.
USD_INTEROP_API PxOsdSubdivTagsRef PxOsdSubdivTags_Copy(PxOsdSubdivTagsRef tags);

/// Increments the reference count.
USD_INTEROP_API PxOsdSubdivTagsRef PxOsdSubdivTags_Retain(PxOsdSubdivTagsRef tags);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void PxOsdSubdivTags_Release(PxOsdSubdivTagsRef tags);

// ============================================================================
// MARK: - PxOsdSubdivTags Interpolation Rules
// ============================================================================

/// Gets the vertex interpolation rule.
USD_INTEROP_API PxOsdVertexInterpolationRule PxOsdSubdivTags_GetVertexInterpolationRule(
    PxOsdSubdivTagsRef tags);

/// Sets the vertex interpolation rule.
USD_INTEROP_API void PxOsdSubdivTags_SetVertexInterpolationRule(
    PxOsdSubdivTagsRef tags,
    PxOsdVertexInterpolationRule rule);

/// Gets the face-varying interpolation rule.
USD_INTEROP_API PxOsdFaceVaryingInterpolationRule PxOsdSubdivTags_GetFaceVaryingInterpolationRule(
    PxOsdSubdivTagsRef tags);

/// Sets the face-varying interpolation rule.
USD_INTEROP_API void PxOsdSubdivTags_SetFaceVaryingInterpolationRule(
    PxOsdSubdivTagsRef tags,
    PxOsdFaceVaryingInterpolationRule rule);

/// Gets the crease method.
USD_INTEROP_API PxOsdCreaseMethod PxOsdSubdivTags_GetCreaseMethod(PxOsdSubdivTagsRef tags);

/// Sets the crease method.
USD_INTEROP_API void PxOsdSubdivTags_SetCreaseMethod(
    PxOsdSubdivTagsRef tags,
    PxOsdCreaseMethod method);

/// Gets the triangle subdivision rule.
USD_INTEROP_API PxOsdTriangleSubdivision PxOsdSubdivTags_GetTriangleSubdivision(
    PxOsdSubdivTagsRef tags);

/// Sets the triangle subdivision rule.
USD_INTEROP_API void PxOsdSubdivTags_SetTriangleSubdivision(
    PxOsdSubdivTagsRef tags,
    PxOsdTriangleSubdivision rule);

// ============================================================================
// MARK: - PxOsdSubdivTags Crease Data
// ============================================================================

/// Gets the number of crease chains.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCreaseLengthCount(PxOsdSubdivTagsRef tags);

/// Gets the crease lengths array.
/// - Parameters:
///   - tags: The subdiv tags.
///   - outLengths: Output buffer for crease lengths.
///   - maxCount: Maximum number of lengths to copy.
/// - Returns: Actual number of lengths copied.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCreaseLengths(
    PxOsdSubdivTagsRef tags,
    int* outLengths,
    size_t maxCount);

/// Sets the crease lengths array.
USD_INTEROP_API void PxOsdSubdivTags_SetCreaseLengths(
    PxOsdSubdivTagsRef tags,
    const int* lengths,
    size_t count);

/// Gets the number of crease indices.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCreaseIndexCount(PxOsdSubdivTagsRef tags);

/// Gets the crease indices array.
/// - Parameters:
///   - tags: The subdiv tags.
///   - outIndices: Output buffer for crease indices.
///   - maxCount: Maximum number of indices to copy.
/// - Returns: Actual number of indices copied.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCreaseIndices(
    PxOsdSubdivTagsRef tags,
    int* outIndices,
    size_t maxCount);

/// Sets the crease indices array.
USD_INTEROP_API void PxOsdSubdivTags_SetCreaseIndices(
    PxOsdSubdivTagsRef tags,
    const int* indices,
    size_t count);

/// Gets the number of crease weights.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCreaseWeightCount(PxOsdSubdivTagsRef tags);

/// Gets the crease weights array.
/// - Parameters:
///   - tags: The subdiv tags.
///   - outWeights: Output buffer for crease weights.
///   - maxCount: Maximum number of weights to copy.
/// - Returns: Actual number of weights copied.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCreaseWeights(
    PxOsdSubdivTagsRef tags,
    float* outWeights,
    size_t maxCount);

/// Sets the crease weights array.
USD_INTEROP_API void PxOsdSubdivTags_SetCreaseWeights(
    PxOsdSubdivTagsRef tags,
    const float* weights,
    size_t count);

// ============================================================================
// MARK: - PxOsdSubdivTags Corner Data
// ============================================================================

/// Gets the number of corner indices.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCornerIndexCount(PxOsdSubdivTagsRef tags);

/// Gets the corner indices array.
/// - Parameters:
///   - tags: The subdiv tags.
///   - outIndices: Output buffer for corner indices.
///   - maxCount: Maximum number of indices to copy.
/// - Returns: Actual number of indices copied.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCornerIndices(
    PxOsdSubdivTagsRef tags,
    int* outIndices,
    size_t maxCount);

/// Sets the corner indices array.
USD_INTEROP_API void PxOsdSubdivTags_SetCornerIndices(
    PxOsdSubdivTagsRef tags,
    const int* indices,
    size_t count);

/// Gets the number of corner weights.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCornerWeightCount(PxOsdSubdivTagsRef tags);

/// Gets the corner weights array.
/// - Parameters:
///   - tags: The subdiv tags.
///   - outWeights: Output buffer for corner weights.
///   - maxCount: Maximum number of weights to copy.
/// - Returns: Actual number of weights copied.
USD_INTEROP_API size_t PxOsdSubdivTags_GetCornerWeights(
    PxOsdSubdivTagsRef tags,
    float* outWeights,
    size_t maxCount);

/// Sets the corner weights array.
USD_INTEROP_API void PxOsdSubdivTags_SetCornerWeights(
    PxOsdSubdivTagsRef tags,
    const float* weights,
    size_t count);

// ============================================================================
// MARK: - PxOsdSubdivTags Comparison and Hashing
// ============================================================================

/// Computes a hash value for the subdiv tags (for caching/instancing).
USD_INTEROP_API size_t PxOsdSubdivTags_ComputeHash(PxOsdSubdivTagsRef tags);

/// Returns true if two subdiv tags are equal.
USD_INTEROP_API bool PxOsdSubdivTags_Equal(PxOsdSubdivTagsRef a, PxOsdSubdivTagsRef b);

// ============================================================================
// MARK: - PxOsdMeshTopology Lifecycle
// ============================================================================

/// Creates an empty mesh topology with default values (bilinear, right-handed).
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_Create(void);

/// Creates a mesh topology with the specified parameters.
/// - Parameters:
///   - scheme: Subdivision scheme (catmullClark, loop, bilinear).
///   - orientation: Face winding (rightHanded, leftHanded).
///   - faceVertexCounts: Number of vertices per face.
///   - numFaces: Number of faces.
///   - faceVertexIndices: Vertex indices for all faces (flattened).
///   - numIndices: Total number of face vertex indices.
/// - Returns: New mesh topology handle, or NULL on failure.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_CreateWithTopology(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices);

/// Creates a mesh topology with subdivision tags.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_CreateWithSubdivTags(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices,
    PxOsdSubdivTagsRef subdivTags);

/// Creates a mesh topology with hole indices.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_CreateWithHoles(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices,
    const int* holeIndices,
    size_t numHoles);

/// Creates a mesh topology with holes and subdivision tags.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_CreateFull(
    PxOsdSubdivScheme scheme,
    PxOsdOrientation orientation,
    const int* faceVertexCounts,
    size_t numFaces,
    const int* faceVertexIndices,
    size_t numIndices,
    const int* holeIndices,
    size_t numHoles,
    PxOsdSubdivTagsRef subdivTags);

/// Creates a copy of existing mesh topology.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_Copy(PxOsdMeshTopologyRef topology);

/// Increments the reference count.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_Retain(PxOsdMeshTopologyRef topology);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void PxOsdMeshTopology_Release(PxOsdMeshTopologyRef topology);

// ============================================================================
// MARK: - PxOsdMeshTopology Properties
// ============================================================================

/// Gets the subdivision scheme.
USD_INTEROP_API PxOsdSubdivScheme PxOsdMeshTopology_GetScheme(PxOsdMeshTopologyRef topology);

/// Gets the orientation (face winding).
USD_INTEROP_API PxOsdOrientation PxOsdMeshTopology_GetOrientation(PxOsdMeshTopologyRef topology);

/// Gets the number of faces.
USD_INTEROP_API size_t PxOsdMeshTopology_GetFaceCount(PxOsdMeshTopologyRef topology);

/// Gets the face vertex counts array.
/// - Parameters:
///   - topology: The mesh topology.
///   - outCounts: Output buffer for face vertex counts.
///   - maxCount: Maximum number of counts to copy.
/// - Returns: Actual number of counts copied.
USD_INTEROP_API size_t PxOsdMeshTopology_GetFaceVertexCounts(
    PxOsdMeshTopologyRef topology,
    int* outCounts,
    size_t maxCount);

/// Gets the total number of face vertex indices.
USD_INTEROP_API size_t PxOsdMeshTopology_GetFaceVertexIndexCount(PxOsdMeshTopologyRef topology);

/// Gets the face vertex indices array.
/// - Parameters:
///   - topology: The mesh topology.
///   - outIndices: Output buffer for face vertex indices.
///   - maxCount: Maximum number of indices to copy.
/// - Returns: Actual number of indices copied.
USD_INTEROP_API size_t PxOsdMeshTopology_GetFaceVertexIndices(
    PxOsdMeshTopologyRef topology,
    int* outIndices,
    size_t maxCount);

/// Gets the number of hole indices.
USD_INTEROP_API size_t PxOsdMeshTopology_GetHoleIndexCount(PxOsdMeshTopologyRef topology);

/// Gets the hole indices array.
/// - Parameters:
///   - topology: The mesh topology.
///   - outIndices: Output buffer for hole indices.
///   - maxCount: Maximum number of indices to copy.
/// - Returns: Actual number of indices copied.
USD_INTEROP_API size_t PxOsdMeshTopology_GetHoleIndices(
    PxOsdMeshTopologyRef topology,
    int* outIndices,
    size_t maxCount);

/// Gets the subdivision tags. Caller must release the returned handle.
/// Returns NULL if no subdiv tags are set.
USD_INTEROP_API PxOsdSubdivTagsRef PxOsdMeshTopology_GetSubdivTags(PxOsdMeshTopologyRef topology);

// ============================================================================
// MARK: - PxOsdMeshTopology Builder Methods (Immutable - Return Modified Copies)
// ============================================================================

/// Returns a copy with a different subdivision scheme.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_WithScheme(
    PxOsdMeshTopologyRef topology,
    PxOsdSubdivScheme scheme);

/// Returns a copy with a different orientation.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_WithOrientation(
    PxOsdMeshTopologyRef topology,
    PxOsdOrientation orientation);

/// Returns a copy with different subdivision tags.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_WithSubdivTags(
    PxOsdMeshTopologyRef topology,
    PxOsdSubdivTagsRef subdivTags);

/// Returns a copy with different hole indices.
USD_INTEROP_API PxOsdMeshTopologyRef PxOsdMeshTopology_WithHoleIndices(
    PxOsdMeshTopologyRef topology,
    const int* holeIndices,
    size_t numHoles);

// ============================================================================
// MARK: - PxOsdMeshTopology Validation
// ============================================================================

/// Validates the mesh topology and returns a validation result.
/// Caller must release the returned handle.
USD_INTEROP_API PxOsdMeshTopologyValidationRef PxOsdMeshTopology_Validate(
    PxOsdMeshTopologyRef topology);

// ============================================================================
// MARK: - PxOsdMeshTopology Comparison and Hashing
// ============================================================================

/// Computes a hash value for the mesh topology (for caching/instancing).
USD_INTEROP_API size_t PxOsdMeshTopology_ComputeHash(PxOsdMeshTopologyRef topology);

/// Returns true if two mesh topologies are equal.
USD_INTEROP_API bool PxOsdMeshTopology_Equal(PxOsdMeshTopologyRef a, PxOsdMeshTopologyRef b);

// ============================================================================
// MARK: - PxOsdMeshTopologyValidation Lifecycle
// ============================================================================

/// Increments the reference count.
USD_INTEROP_API PxOsdMeshTopologyValidationRef PxOsdMeshTopologyValidation_Retain(
    PxOsdMeshTopologyValidationRef validation);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void PxOsdMeshTopologyValidation_Release(
    PxOsdMeshTopologyValidationRef validation);

// ============================================================================
// MARK: - PxOsdMeshTopologyValidation Properties
// ============================================================================

/// Returns true if the validation passed (no errors).
USD_INTEROP_API bool PxOsdMeshTopologyValidation_IsValid(
    PxOsdMeshTopologyValidationRef validation);

/// Gets the number of validation errors.
USD_INTEROP_API size_t PxOsdMeshTopologyValidation_GetErrorCount(
    PxOsdMeshTopologyValidationRef validation);

/// Gets validation errors.
/// - Parameters:
///   - validation: The validation result.
///   - outErrors: Output buffer for validation errors.
///   - maxCount: Maximum number of errors to copy.
/// - Returns: Actual number of errors copied.
USD_INTEROP_API size_t PxOsdMeshTopologyValidation_GetErrors(
    PxOsdMeshTopologyValidationRef validation,
    PxOsdValidationError* outErrors,
    size_t maxCount);

// ============================================================================
// MARK: - PxOsd Tokens (String Accessors)
// ============================================================================

/// Gets the string name for a subdivision scheme.
/// The returned string is a static constant and should not be freed.
USD_INTEROP_API const char* PxOsd_GetSchemeString(PxOsdSubdivScheme scheme);

/// Gets the string name for an orientation.
USD_INTEROP_API const char* PxOsd_GetOrientationString(PxOsdOrientation orientation);

/// Gets the string name for a vertex interpolation rule.
USD_INTEROP_API const char* PxOsd_GetVertexInterpolationRuleString(
    PxOsdVertexInterpolationRule rule);

/// Gets the string name for a face-varying interpolation rule.
USD_INTEROP_API const char* PxOsd_GetFaceVaryingInterpolationRuleString(
    PxOsdFaceVaryingInterpolationRule rule);

/// Gets the string name for a crease method.
USD_INTEROP_API const char* PxOsd_GetCreaseMethodString(PxOsdCreaseMethod method);

/// Gets the string name for a triangle subdivision rule.
USD_INTEROP_API const char* PxOsd_GetTriangleSubdivisionString(PxOsdTriangleSubdivision rule);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_PXOSD_H
