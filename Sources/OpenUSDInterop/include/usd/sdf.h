// sdf.h - SdfPath, SdfLayer, SdfAssetPath C wrapper
// Mirrors: pxr/usd/sdf/path.h, pxr/usd/sdf/layer.h, pxr/usd/sdf/assetPath.h

#ifndef OPENUSDINTEROP_USD_SDF_H
#define OPENUSDINTEROP_USD_SDF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations for cross-module types
#include "../base/tf.h"

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
// MARK: - SdfPath
// ============================================================================

/// Opaque handle to an SdfPath wrapper.
/// SdfPath is used to locate objects in layers or scenegraphs.
typedef struct SdfPathOpaque* SdfPathRef;

// MARK: - SdfPath Lifecycle

/// Creates an SdfPath from a path string.
/// Returns NULL on failure. Caller must release with SdfPath_Release.
/// Examples: "/World", "/World/Mesh", "/World/Mesh.points"
USD_INTEROP_API SdfPathRef SdfPath_Create(const char* pathString);

/// Creates an empty SdfPath.
/// Returns NULL on failure. Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_CreateEmpty(void);

/// Creates a copy of an existing SdfPath.
/// Returns NULL on failure. Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_Copy(SdfPathRef path);

/// Increments the reference count of an SdfPath.
/// Returns the same path for convenience, or NULL if path was NULL.
USD_INTEROP_API SdfPathRef SdfPath_Retain(SdfPathRef path);

/// Decrements the reference count and frees the path wrapper if count reaches zero.
/// Safe to call with NULL.
USD_INTEROP_API void SdfPath_Release(SdfPathRef path);

// MARK: - SdfPath Static Accessors

/// Returns the absolute root path "/".
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AbsoluteRootPath(void);

/// Returns the reflexive relative path ".".
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_ReflexiveRelativePath(void);

// MARK: - SdfPath Properties

/// Returns the string representation of the path.
/// The returned string is valid as long as the path is valid.
/// Returns empty string "" for empty paths, never NULL.
USD_INTEROP_API const char* SdfPath_GetText(SdfPathRef path);

/// Returns the string representation as an allocated string.
/// Caller must free the result with SdfPath_FreeString.
USD_INTEROP_API char* SdfPath_GetString(SdfPathRef path);

/// Frees a string allocated by SdfPath_GetString.
USD_INTEROP_API void SdfPath_FreeString(char* str);

/// Returns the name of the prim, property, or relational attribute.
/// The returned string is valid as long as the path is valid.
USD_INTEROP_API const char* SdfPath_GetName(SdfPathRef path);

/// Returns the name as a TfToken.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef SdfPath_GetNameToken(SdfPathRef path);

/// Returns the number of path elements.
USD_INTEROP_API size_t SdfPath_GetElementCount(SdfPathRef path);

/// Returns true if the path is empty.
USD_INTEROP_API bool SdfPath_IsEmpty(SdfPathRef path);

/// Returns true if the path is absolute (starts with "/").
USD_INTEROP_API bool SdfPath_IsAbsolutePath(SdfPathRef path);

/// Returns true if the path is the absolute root path "/".
USD_INTEROP_API bool SdfPath_IsAbsoluteRootPath(SdfPathRef path);

/// Returns true if the path identifies a prim.
USD_INTEROP_API bool SdfPath_IsPrimPath(SdfPathRef path);

/// Returns true if the path identifies a prim or the absolute root.
USD_INTEROP_API bool SdfPath_IsAbsoluteRootOrPrimPath(SdfPathRef path);

/// Returns true if the path identifies a root prim (e.g., "/Foo").
USD_INTEROP_API bool SdfPath_IsRootPrimPath(SdfPathRef path);

/// Returns true if the path identifies a property.
USD_INTEROP_API bool SdfPath_IsPropertyPath(SdfPathRef path);

/// Returns true if the path identifies a prim's property.
USD_INTEROP_API bool SdfPath_IsPrimPropertyPath(SdfPathRef path);

/// Returns true if the path contains a variant selection.
USD_INTEROP_API bool SdfPath_ContainsPrimVariantSelection(SdfPathRef path);

/// Returns a hash value for the path.
USD_INTEROP_API size_t SdfPath_Hash(SdfPathRef path);

// MARK: - SdfPath Navigation

/// Returns the parent path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_GetParentPath(SdfPathRef path);

/// Returns the prim path (strips property components).
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_GetPrimPath(SdfPathRef path);

/// Returns the absolute root or prim path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_GetAbsoluteRootOrPrimPath(SdfPathRef path);

// MARK: - SdfPath Construction

/// Appends a child element to the path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AppendChild(SdfPathRef path, TfTokenRef childName);

/// Appends a child element by string name.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AppendChildString(SdfPathRef path, const char* childName);

/// Appends a property element to the path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AppendProperty(SdfPathRef path, TfTokenRef propName);

/// Appends a property element by string name.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AppendPropertyString(SdfPathRef path, const char* propName);

/// Appends a variant selection to the path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AppendVariantSelection(
    SdfPathRef path,
    const char* variantSet,
    const char* variant
);

/// Appends a relative path to this path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_AppendPath(SdfPathRef path, SdfPathRef suffix);

/// Replaces a prefix in the path with a new prefix.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_ReplacePrefix(
    SdfPathRef path,
    SdfPathRef oldPrefix,
    SdfPathRef newPrefix
);

/// Returns the common prefix of two paths.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_GetCommonPrefix(SdfPathRef path1, SdfPathRef path2);

/// Makes the path absolute using the given anchor path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_MakeAbsolutePath(SdfPathRef path, SdfPathRef anchor);

/// Makes the path relative to the given anchor path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef SdfPath_MakeRelativePath(SdfPathRef path, SdfPathRef anchor);

// MARK: - SdfPath Comparison

/// Returns true if two paths are equal.
USD_INTEROP_API bool SdfPath_Equal(SdfPathRef lhs, SdfPathRef rhs);

/// Compares two paths lexicographically.
/// Returns < 0 if lhs < rhs, 0 if equal, > 0 if lhs > rhs.
USD_INTEROP_API int SdfPath_Compare(SdfPathRef lhs, SdfPathRef rhs);

/// Returns true if path has the given prefix.
USD_INTEROP_API bool SdfPath_HasPrefix(SdfPathRef path, SdfPathRef prefix);

// MARK: - SdfPath Validation

/// Returns true if the string is a valid path string.
USD_INTEROP_API bool SdfPath_IsValidPathString(const char* pathString);

/// Returns true if the name is a valid identifier.
USD_INTEROP_API bool SdfPath_IsValidIdentifier(const char* name);

// MARK: - SdfPath Batch Operations

/// Gets the prefixes of a path (ancestors from root to path).
/// outPaths must have space for maxCount paths.
/// Returns the number of paths written.
/// Caller must release each path with SdfPath_Release.
USD_INTEROP_API size_t SdfPath_GetPrefixes(
    SdfPathRef path,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Gets the count of prefixes for a path.
USD_INTEROP_API size_t SdfPath_GetPrefixCount(SdfPathRef path);

/// Releases multiple paths.
USD_INTEROP_API void SdfPath_ReleaseBatch(SdfPathRef* paths, size_t count);

// ============================================================================
// MARK: - SdfAssetPath
// ============================================================================

/// Opaque handle to an SdfAssetPath wrapper.
/// SdfAssetPath contains an asset path and optional resolved path.
typedef struct SdfAssetPathOpaque* SdfAssetPathRef;

// MARK: - SdfAssetPath Lifecycle

/// Creates an SdfAssetPath from an authored path string.
/// Returns NULL on failure. Caller must release with SdfAssetPath_Release.
USD_INTEROP_API SdfAssetPathRef SdfAssetPath_Create(const char* authoredPath);

/// Creates an SdfAssetPath with both authored and resolved paths.
/// Returns NULL on failure. Caller must release with SdfAssetPath_Release.
USD_INTEROP_API SdfAssetPathRef SdfAssetPath_CreateWithResolvedPath(
    const char* authoredPath,
    const char* resolvedPath
);

/// Creates an empty SdfAssetPath.
/// Returns NULL on failure. Caller must release with SdfAssetPath_Release.
USD_INTEROP_API SdfAssetPathRef SdfAssetPath_CreateEmpty(void);

/// Creates a copy of an existing SdfAssetPath.
/// Returns NULL on failure. Caller must release with SdfAssetPath_Release.
USD_INTEROP_API SdfAssetPathRef SdfAssetPath_Copy(SdfAssetPathRef assetPath);

/// Increments the reference count of an SdfAssetPath.
USD_INTEROP_API SdfAssetPathRef SdfAssetPath_Retain(SdfAssetPathRef assetPath);

/// Decrements the reference count and frees if count reaches zero.
USD_INTEROP_API void SdfAssetPath_Release(SdfAssetPathRef assetPath);

// MARK: - SdfAssetPath Properties

/// Returns the authored asset path.
/// The returned string is valid as long as the asset path is valid.
USD_INTEROP_API const char* SdfAssetPath_GetAuthoredPath(SdfAssetPathRef assetPath);

/// Returns the asset path (evaluated path if present, otherwise authored).
/// The returned string is valid as long as the asset path is valid.
USD_INTEROP_API const char* SdfAssetPath_GetAssetPath(SdfAssetPathRef assetPath);

/// Returns the resolved path.
/// The returned string is valid as long as the asset path is valid.
USD_INTEROP_API const char* SdfAssetPath_GetResolvedPath(SdfAssetPathRef assetPath);

/// Returns a hash value for the asset path.
USD_INTEROP_API size_t SdfAssetPath_Hash(SdfAssetPathRef assetPath);

// MARK: - SdfAssetPath Comparison

/// Returns true if two asset paths are equal.
USD_INTEROP_API bool SdfAssetPath_Equal(SdfAssetPathRef lhs, SdfAssetPathRef rhs);

/// Compares two asset paths.
USD_INTEROP_API int SdfAssetPath_Compare(SdfAssetPathRef lhs, SdfAssetPathRef rhs);

// ============================================================================
// MARK: - SdfLayer
// ============================================================================

/// Opaque handle to an SdfLayer wrapper.
/// SdfLayer is a scene description container.
typedef struct SdfLayerOpaque* SdfLayerRef;

// MARK: - SdfLayer Lifecycle

/// Creates a new empty layer at the given identifier.
/// Returns NULL on failure. Caller must release with SdfLayer_Release.
USD_INTEROP_API SdfLayerRef SdfLayer_CreateNew(const char* identifier);

/// Creates a new anonymous layer with an optional tag.
/// Returns NULL on failure. Caller must release with SdfLayer_Release.
USD_INTEROP_API SdfLayerRef SdfLayer_CreateAnonymous(const char* tag);

/// Opens an existing layer or creates it if it doesn't exist.
/// Returns NULL on failure. Caller must release with SdfLayer_Release.
USD_INTEROP_API SdfLayerRef SdfLayer_FindOrOpen(const char* identifier);

/// Finds an existing layer by identifier.
/// Returns NULL if not found. Caller must release with SdfLayer_Release.
USD_INTEROP_API SdfLayerRef SdfLayer_Find(const char* identifier);

/// Increments the reference count of a layer.
USD_INTEROP_API SdfLayerRef SdfLayer_Retain(SdfLayerRef layer);

/// Decrements the reference count and releases if count reaches zero.
USD_INTEROP_API void SdfLayer_Release(SdfLayerRef layer);

// MARK: - SdfLayer Properties

/// Returns the layer's identifier.
/// The returned string is valid as long as the layer is valid.
USD_INTEROP_API const char* SdfLayer_GetIdentifier(SdfLayerRef layer);

/// Returns the layer's display name.
/// Caller must free the result with SdfLayer_FreeString.
USD_INTEROP_API char* SdfLayer_GetDisplayName(SdfLayerRef layer);

/// Returns the layer's real (resolved) path.
/// The returned string is valid as long as the layer is valid.
USD_INTEROP_API const char* SdfLayer_GetRealPath(SdfLayerRef layer);

/// Returns the layer's file extension.
/// Caller must free the result with SdfLayer_FreeString.
USD_INTEROP_API char* SdfLayer_GetFileExtension(SdfLayerRef layer);

/// Frees a string allocated by SdfLayer functions.
USD_INTEROP_API void SdfLayer_FreeString(char* str);

/// Returns true if the layer is anonymous.
USD_INTEROP_API bool SdfLayer_IsAnonymous(SdfLayerRef layer);

/// Returns true if the layer is dirty (has unsaved changes).
USD_INTEROP_API bool SdfLayer_IsDirty(SdfLayerRef layer);

/// Returns true if the layer is empty.
USD_INTEROP_API bool SdfLayer_IsEmpty(SdfLayerRef layer);

/// Returns true if the layer is muted.
USD_INTEROP_API bool SdfLayer_IsMuted(SdfLayerRef layer);

// MARK: - SdfLayer Content

/// Clears all content from the layer.
USD_INTEROP_API UsdResult SdfLayer_Clear(SdfLayerRef layer);

/// Saves the layer to its current location.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult SdfLayer_Save(SdfLayerRef layer, bool force);

/// Exports the layer to a new file.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult SdfLayer_Export(
    SdfLayerRef layer,
    const char* filename,
    const char* comment
);

/// Exports the layer content to a string.
/// Caller must free the result with SdfLayer_FreeString.
USD_INTEROP_API char* SdfLayer_ExportToString(SdfLayerRef layer);

/// Imports content from a string.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult SdfLayer_ImportFromString(SdfLayerRef layer, const char* content);

/// Reloads the layer from disk.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult SdfLayer_Reload(SdfLayerRef layer, bool force);

// MARK: - SdfLayer Metadata

/// Gets the default prim name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef SdfLayer_GetDefaultPrim(SdfLayerRef layer);

/// Sets the default prim name.
USD_INTEROP_API UsdResult SdfLayer_SetDefaultPrim(SdfLayerRef layer, TfTokenRef name);

/// Gets the documentation string.
/// Caller must free the result with SdfLayer_FreeString.
USD_INTEROP_API char* SdfLayer_GetDocumentation(SdfLayerRef layer);

/// Sets the documentation string.
USD_INTEROP_API UsdResult SdfLayer_SetDocumentation(SdfLayerRef layer, const char* doc);

/// Gets the comment string.
/// Caller must free the result with SdfLayer_FreeString.
USD_INTEROP_API char* SdfLayer_GetComment(SdfLayerRef layer);

/// Sets the comment string.
USD_INTEROP_API UsdResult SdfLayer_SetComment(SdfLayerRef layer, const char* comment);

/// Gets the start time code.
USD_INTEROP_API double SdfLayer_GetStartTimeCode(SdfLayerRef layer);

/// Sets the start time code.
USD_INTEROP_API UsdResult SdfLayer_SetStartTimeCode(SdfLayerRef layer, double timeCode);

/// Returns true if start time code is set.
USD_INTEROP_API bool SdfLayer_HasStartTimeCode(SdfLayerRef layer);

/// Gets the end time code.
USD_INTEROP_API double SdfLayer_GetEndTimeCode(SdfLayerRef layer);

/// Sets the end time code.
USD_INTEROP_API UsdResult SdfLayer_SetEndTimeCode(SdfLayerRef layer, double timeCode);

/// Returns true if end time code is set.
USD_INTEROP_API bool SdfLayer_HasEndTimeCode(SdfLayerRef layer);

/// Gets the time codes per second.
USD_INTEROP_API double SdfLayer_GetTimeCodesPerSecond(SdfLayerRef layer);

/// Sets the time codes per second.
USD_INTEROP_API UsdResult SdfLayer_SetTimeCodesPerSecond(SdfLayerRef layer, double tcps);

/// Returns true if time codes per second is set.
USD_INTEROP_API bool SdfLayer_HasTimeCodesPerSecond(SdfLayerRef layer);

/// Gets the frames per second.
USD_INTEROP_API double SdfLayer_GetFramesPerSecond(SdfLayerRef layer);

/// Sets the frames per second.
USD_INTEROP_API UsdResult SdfLayer_SetFramesPerSecond(SdfLayerRef layer, double fps);

/// Returns true if frames per second is set.
USD_INTEROP_API bool SdfLayer_HasFramesPerSecond(SdfLayerRef layer);

// MARK: - SdfLayer Sublayers

/// Gets the number of sublayers.
USD_INTEROP_API size_t SdfLayer_GetNumSubLayerPaths(SdfLayerRef layer);

/// Gets sublayer paths.
/// outPaths must have space for maxCount strings.
/// Returns the number of paths written.
/// Caller must free each string with free().
USD_INTEROP_API size_t SdfLayer_GetSubLayerPaths(
    SdfLayerRef layer,
    char** outPaths,
    size_t maxCount
);

/// Inserts a sublayer path at the given index.
/// Use index -1 to append at the end.
USD_INTEROP_API UsdResult SdfLayer_InsertSubLayerPath(
    SdfLayerRef layer,
    const char* path,
    int index
);

/// Removes a sublayer path at the given index.
USD_INTEROP_API UsdResult SdfLayer_RemoveSubLayerPath(SdfLayerRef layer, int index);

// MARK: - SdfLayer Path Operations

/// Returns true if a spec exists at the given path.
USD_INTEROP_API bool SdfLayer_HasSpec(SdfLayerRef layer, SdfPathRef path);

/// Traverses the layer hierarchy, calling the callback for each path.
/// The callback receives the path string. Return false to stop traversal.
typedef bool (*SdfLayerTraverseCallback)(const char* pathStr, void* userData);
USD_INTEROP_API UsdResult SdfLayer_Traverse(
    SdfLayerRef layer,
    SdfPathRef rootPath,
    SdfLayerTraverseCallback callback,
    void* userData
);

// MARK: - SdfLayer Root Prims

/// Gets the count of root prims.
USD_INTEROP_API size_t SdfLayer_GetRootPrimCount(SdfLayerRef layer);

/// Gets the names of root prims.
/// outNames must have space for maxCount tokens.
/// Returns the number of names written.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t SdfLayer_GetRootPrimNames(
    SdfLayerRef layer,
    TfTokenRef* outNames,
    size_t maxCount
);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_SDF_H
