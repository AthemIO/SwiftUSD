// usdUtils.h - UsdUtils module C wrapper
// Mirrors: pxr/usd/usdUtils/stageCache.h, flattenLayerStack.h, stitch.h

#ifndef OPENUSDINTEROP_USDUTILS_USDUTILS_H
#define OPENUSDINTEROP_USDUTILS_USDUTILS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations for cross-module types
#include "../usd/sdf.h"
#include "../usd/usd.h"

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
// MARK: - UsdUtilsStageCache
// ============================================================================

/// Gets the singleton stage cache.
/// This is a global cache that can be used to share stages across an application.
/// Note: The cache itself is not returned - instead, access methods are provided.

/// Gets the count of stages in the singleton cache.
USD_INTEROP_API size_t UsdUtilsStageCache_GetCount(void);

/// Gets a stage from the cache by its ID.
/// Returns NULL if the ID is not found. Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdUtilsStageCache_Find(int64_t id);

/// Gets a stage from the cache by its root layer path.
/// Returns NULL if not found. Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdUtilsStageCache_FindByLayerPath(const char* layerPath);

/// Inserts a stage into the singleton cache.
/// Returns the cache ID for the stage, or -1 on error.
/// Note: The cache takes a reference to the stage.
USD_INTEROP_API int64_t UsdUtilsStageCache_Insert(UsdStageRef stage);

/// Erases a stage from the singleton cache by ID.
/// Returns true if the stage was erased, false if not found.
USD_INTEROP_API bool UsdUtilsStageCache_Erase(int64_t id);

/// Erases a stage from the singleton cache by stage reference.
/// Returns true if the stage was erased, false if not found.
USD_INTEROP_API bool UsdUtilsStageCache_EraseStage(UsdStageRef stage);

/// Clears all stages from the singleton cache.
USD_INTEROP_API void UsdUtilsStageCache_Clear(void);

/// Gets all stage IDs in the cache.
/// outIds must have space for maxCount IDs.
/// Returns the number of IDs written.
USD_INTEROP_API size_t UsdUtilsStageCache_GetAllIds(
    int64_t* outIds,
    size_t maxCount
);

/// Gets all stages from the cache.
/// outStages must have space for maxCount stages.
/// Returns the number of stages written.
/// Caller must release each stage with UsdStage_Release.
USD_INTEROP_API size_t UsdUtilsStageCache_GetAllStages(
    UsdStageRef* outStages,
    size_t maxCount
);

// ============================================================================
// MARK: - UsdUtilsFlattenLayerStack
// ============================================================================

/// Flatten the root layer stack of the given stage into a single layer.
///
/// The result layer can be substituted for the original layer stack
/// while producing the same composed UsdStage.
///
/// Unlike UsdStage::Export(), this function does not flatten
/// composition arcs, such as references, payloads, inherits,
/// specializes, or variants.
///
/// - Parameters:
///   - stage: The stage whose layer stack should be flattened.
///   - tag: Optional tag for the new layer (can be NULL).
/// - Returns: A new layer containing the flattened layer stack.
///           Caller must release with SdfLayer_Release.
///           Returns NULL on error.
USD_INTEROP_API SdfLayerRef UsdUtilsFlattenLayerStack(
    UsdStageRef stage,
    const char* tag
);

/// Resolves an asset path during flatten layer stack operation.
/// For layer-relative paths, returns an absolute path.
/// For search paths and absolute paths, returns the original path.
///
/// - Parameters:
///   - sourceLayer: The layer containing the asset path.
///   - assetPath: The asset path to resolve.
/// - Returns: The resolved asset path. Caller must free with UsdUtils_FreeString.
USD_INTEROP_API char* UsdUtilsFlattenLayerStackResolveAssetPath(
    SdfLayerRef sourceLayer,
    const char* assetPath
);

// ============================================================================
// MARK: - UsdUtilsStitchLayers
// ============================================================================

/// Merge all scene description in weakLayer into strongLayer.
///
/// Prims and properties in weakLayer that do not exist in strongLayer
/// will be copied into strongLayer. Prims and properties that do
/// exist in strongLayer will be merged with the existing scene
/// description.
///
/// - Parameters:
///   - strongLayer: The layer that takes precedence in conflicts.
///   - weakLayer: The layer to merge from.
/// - Returns: USD_RESULT_SUCCESS on success, error code otherwise.
USD_INTEROP_API UsdResult UsdUtilsStitchLayers(
    SdfLayerRef strongLayer,
    SdfLayerRef weakLayer
);

// ============================================================================
// MARK: - UsdUtilsDependencies
// ============================================================================

/// Extract external references from a USD file.
/// Parses the file at filePath, identifying external references, and
/// sorting them into separate type-based buckets.
///
/// - Parameters:
///   - filePath: Path to the USD file to analyze.
///   - outSubLayers: Buffer to receive sublayer paths.
///   - subLayersMaxCount: Size of the outSubLayers buffer.
///   - outReferences: Buffer to receive reference paths.
///   - referencesMaxCount: Size of the outReferences buffer.
///   - outPayloads: Buffer to receive payload paths.
///   - payloadsMaxCount: Size of the outPayloads buffer.
///   - outSubLayersCount: Receives the number of sublayers found.
///   - outReferencesCount: Receives the number of references found.
///   - outPayloadsCount: Receives the number of payloads found.
/// - Returns: USD_RESULT_SUCCESS on success.
/// Note: Each string in the output buffers must be freed with UsdUtils_FreeString.
USD_INTEROP_API UsdResult UsdUtilsExtractExternalReferences(
    const char* filePath,
    char** outSubLayers,
    size_t subLayersMaxCount,
    char** outReferences,
    size_t referencesMaxCount,
    char** outPayloads,
    size_t payloadsMaxCount,
    size_t* outSubLayersCount,
    size_t* outReferencesCount,
    size_t* outPayloadsCount
);

/// Gets the count of external references from a USD file.
/// Use this to determine buffer sizes before calling UsdUtilsExtractExternalReferences.
///
/// - Parameters:
///   - filePath: Path to the USD file to analyze.
///   - outSubLayersCount: Receives the number of sublayers.
///   - outReferencesCount: Receives the number of references.
///   - outPayloadsCount: Receives the number of payloads.
/// - Returns: USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult UsdUtilsGetExternalReferenceCounts(
    const char* filePath,
    size_t* outSubLayersCount,
    size_t* outReferencesCount,
    size_t* outPayloadsCount
);

/// Recursively computes all dependencies of the given asset.
///
/// - Parameters:
///   - assetPath: Path to the USD asset.
///   - outLayerPaths: Buffer to receive dependent layer paths.
///   - layerPathsMaxCount: Size of the outLayerPaths buffer.
///   - outAssetPaths: Buffer to receive dependent asset paths (non-layer).
///   - assetPathsMaxCount: Size of the outAssetPaths buffer.
///   - outUnresolvedPaths: Buffer to receive unresolved paths.
///   - unresolvedMaxCount: Size of the outUnresolvedPaths buffer.
///   - outLayerCount: Receives the number of dependent layers found.
///   - outAssetCount: Receives the number of dependent assets found.
///   - outUnresolvedCount: Receives the number of unresolved paths.
/// - Returns: true if the asset was resolved correctly.
/// Note: Each string in the output buffers must be freed with UsdUtils_FreeString.
USD_INTEROP_API bool UsdUtilsComputeAllDependencies(
    const char* assetPath,
    char** outLayerPaths,
    size_t layerPathsMaxCount,
    char** outAssetPaths,
    size_t assetPathsMaxCount,
    char** outUnresolvedPaths,
    size_t unresolvedMaxCount,
    size_t* outLayerCount,
    size_t* outAssetCount,
    size_t* outUnresolvedCount
);

/// Gets the count of all dependencies for an asset.
/// Use this to determine buffer sizes before calling UsdUtilsComputeAllDependencies.
///
/// - Parameters:
///   - assetPath: Path to the USD asset.
///   - outLayerCount: Receives the number of dependent layers.
///   - outAssetCount: Receives the number of dependent assets.
///   - outUnresolvedCount: Receives the number of unresolved paths.
/// - Returns: true if the asset was resolved correctly.
USD_INTEROP_API bool UsdUtilsGetAllDependencyCounts(
    const char* assetPath,
    size_t* outLayerCount,
    size_t* outAssetCount,
    size_t* outUnresolvedCount
);

// ============================================================================
// MARK: - UsdUtilsPipeline
// ============================================================================

/// Gets the name of the USD prim under which materials are expected to be authored.
/// By default, this is "Looks". Can be configured via plugInfo.json.
///
/// - Parameter forceDefault: If true, ignores any plugInfo.json configuration.
/// - Returns: The materials scope name. Caller must free with UsdUtils_FreeString.
USD_INTEROP_API char* UsdUtilsGetMaterialsScopeName(bool forceDefault);

/// Gets the name of the primary UV set used on meshes and nurbs.
/// By default, this is "st".
///
/// - Returns: The primary UV set name. Caller must free with UsdUtils_FreeString.
USD_INTEROP_API char* UsdUtilsGetPrimaryUVSetName(void);

/// Gets the name of the reference position used on meshes and nurbs.
/// By default, this is "pref".
///
/// - Returns: The pref name. Caller must free with UsdUtils_FreeString.
USD_INTEROP_API char* UsdUtilsGetPrefName(void);

/// Gets the name of the primary camera.
/// By default, this is "main_cam". Can be configured via plugInfo.json.
///
/// - Parameter forceDefault: If true, ignores any plugInfo.json configuration.
/// - Returns: The primary camera name. Caller must free with UsdUtils_FreeString.
USD_INTEROP_API char* UsdUtilsGetPrimaryCameraName(bool forceDefault);

/// Gets the alpha attribute name for a given color attribute.
/// Follows the shading pipeline's convention for naming a companion
/// alpha/opacity attribute given a color-valued attribute name.
///
/// - Parameter colorAttrName: The name of the color attribute.
/// - Returns: The alpha attribute name. Caller must free with UsdUtils_FreeString.
USD_INTEROP_API char* UsdUtilsGetAlphaAttributeNameForColor(const char* colorAttrName);

/// Gets the model name associated with a given root layer.
/// In order, it looks for: defaultPrim metadata, a prim matching the filename,
/// and then the first concrete root prim.
///
/// - Parameter rootLayer: The root layer.
/// - Returns: The model name. Caller must free with UsdUtils_FreeString.
///            Returns NULL if no model name can be determined.
USD_INTEROP_API char* UsdUtilsGetModelNameFromRootLayer(SdfLayerRef rootLayer);

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Frees a string allocated by UsdUtils functions.
USD_INTEROP_API void UsdUtils_FreeString(char* str);

/// Frees an array of strings allocated by UsdUtils functions.
USD_INTEROP_API void UsdUtils_FreeStringArray(char** strings, size_t count);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDUTILS_USDUTILS_H
