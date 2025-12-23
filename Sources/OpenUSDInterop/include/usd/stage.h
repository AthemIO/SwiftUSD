// stage.h - UsdStage C wrapper
// Mirrors: pxr/usd/usd/stage.h

#ifndef OPENUSDINTEROP_USD_STAGE_H
#define OPENUSDINTEROP_USD_STAGE_H

#include "../OpenUSDInterop.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Stage Creation

/// Creates a new stage with an anonymous root layer.
USD_INTEROP_API UsdResult UsdStage_CreateNew(UsdStageRef* outStage);

/// Creates a new stage backed by a root layer at the given path.
USD_INTEROP_API UsdResult UsdStage_CreateNewAtPath(const char* path, UsdStageRef* outStage);

/// Opens an existing stage from a USD file.
USD_INTEROP_API UsdResult UsdStage_Open(const char* path, UsdStageRef* outStage);

/// Destroys a stage (decrements reference count).
USD_INTEROP_API void UsdStage_Destroy(UsdStageRef stage);

// MARK: - Stage Properties

/// Returns the root layer of the stage.
USD_INTEROP_API UsdResult UsdStage_GetRootLayer(UsdStageRef stage, UsdSdfLayerRef* outLayer);

/// Returns the session layer of the stage.
USD_INTEROP_API UsdResult UsdStage_GetSessionLayer(UsdStageRef stage, UsdSdfLayerRef* outLayer);

// MARK: - Prim Access

/// Returns the pseudo-root prim.
USD_INTEROP_API UsdResult UsdStage_GetPseudoRoot(UsdStageRef stage, UsdPrimRef* outPrim);

/// Returns the default prim, if set.
USD_INTEROP_API UsdResult UsdStage_GetDefaultPrim(UsdStageRef stage, UsdPrimRef* outPrim);

/// Sets the default prim.
USD_INTEROP_API UsdResult UsdStage_SetDefaultPrim(UsdStageRef stage, UsdPrimRef prim);

/// Returns the prim at the given path.
USD_INTEROP_API UsdResult UsdStage_GetPrimAtPath(UsdStageRef stage, UsdSdfPathRef path, UsdPrimRef* outPrim);

/// Defines a new prim at the given path with the given type.
USD_INTEROP_API UsdResult UsdStage_DefinePrim(UsdStageRef stage, UsdSdfPathRef path, UsdTfTokenRef typeName, UsdPrimRef* outPrim);

/// Overrides a prim at the given path (creates if not exists).
USD_INTEROP_API UsdResult UsdStage_OverridePrim(UsdStageRef stage, UsdSdfPathRef path, UsdPrimRef* outPrim);

/// Removes a prim at the given path.
USD_INTEROP_API UsdResult UsdStage_RemovePrim(UsdStageRef stage, UsdSdfPathRef path);

// MARK: - Traversal

/// Returns the count of prims when traversing.
USD_INTEROP_API size_t UsdStage_GetPrimCount(UsdStageRef stage);

/// Traverses prims and calls the callback for each.
typedef void (*UsdStageTraverseCallback)(UsdPrimRef prim, void* userData);
USD_INTEROP_API void UsdStage_Traverse(UsdStageRef stage, UsdStageTraverseCallback callback, void* userData);

// MARK: - Save/Export

/// Saves all dirty layers in the stage.
USD_INTEROP_API UsdResult UsdStage_Save(UsdStageRef stage);

/// Exports the flattened stage to a new file.
USD_INTEROP_API UsdResult UsdStage_Export(UsdStageRef stage, const char* path);

// MARK: - Time

/// Returns the start time code of the stage.
USD_INTEROP_API double UsdStage_GetStartTimeCode(UsdStageRef stage);

/// Returns the end time code of the stage.
USD_INTEROP_API double UsdStage_GetEndTimeCode(UsdStageRef stage);

/// Sets the start time code of the stage.
USD_INTEROP_API void UsdStage_SetStartTimeCode(UsdStageRef stage, double timeCode);

/// Sets the end time code of the stage.
USD_INTEROP_API void UsdStage_SetEndTimeCode(UsdStageRef stage, double timeCode);

/// Returns the time codes per second.
USD_INTEROP_API double UsdStage_GetTimeCodesPerSecond(UsdStageRef stage);

/// Returns the frames per second.
USD_INTEROP_API double UsdStage_GetFramesPerSecond(UsdStageRef stage);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_STAGE_H
