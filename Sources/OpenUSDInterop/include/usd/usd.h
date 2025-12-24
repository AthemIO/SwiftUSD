// usd.h - Core USD module C wrapper
// Mirrors: pxr/usd/usd/stage.h, pxr/usd/usd/prim.h, pxr/usd/usd/attribute.h, etc.

#ifndef OPENUSDINTEROP_USD_USD_H
#define OPENUSDINTEROP_USD_USD_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations for cross-module types
#include "../base/tf.h"
#include "../base/vt.h"
#include "sdf.h"

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
// MARK: - Forward Declarations
// ============================================================================

#ifndef USD_STAGE_REF_DEFINED
#define USD_STAGE_REF_DEFINED
typedef struct UsdStageOpaque* UsdStageRef;
#endif

#ifndef USD_PRIM_REF_DEFINED
#define USD_PRIM_REF_DEFINED
typedef struct UsdPrimOpaque* UsdPrimRef;
#endif

#ifndef USD_ATTRIBUTE_REF_DEFINED
#define USD_ATTRIBUTE_REF_DEFINED
typedef struct UsdAttributeOpaque* UsdAttributeRef;
#endif

#ifndef USD_RELATIONSHIP_REF_DEFINED
#define USD_RELATIONSHIP_REF_DEFINED
typedef struct UsdRelationshipOpaque* UsdRelationshipRef;
#endif

#ifndef USD_PROPERTY_REF_DEFINED
#define USD_PROPERTY_REF_DEFINED
typedef struct UsdPropertyOpaque* UsdPropertyRef;
#endif

// ============================================================================
// MARK: - Enums
// ============================================================================

/// Stage initial load policy
typedef enum {
    USD_LOAD_ALL = 0,    ///< Load all loadable prims
    USD_LOAD_NONE = 1    ///< Load no loadable prims
} UsdInitialLoadSet;

/// Prim specifier type
typedef enum {
    USD_SPECIFIER_DEF = 0,      ///< Defines a concrete prim
    USD_SPECIFIER_OVER = 1,     ///< Overrides an existing prim
    USD_SPECIFIER_CLASS = 2     ///< Defines an abstract class prim
} UsdSpecifier;

/// Attribute variability
typedef enum {
    USD_VARIABILITY_VARYING = 0,  ///< Can have time samples
    USD_VARIABILITY_UNIFORM = 1   ///< Single value only
} UsdVariability;

/// List edit position for composition arcs
typedef enum {
    USD_LIST_POSITION_FRONT_OF_PREPEND_LIST = 0,
    USD_LIST_POSITION_BACK_OF_PREPEND_LIST = 1,
    USD_LIST_POSITION_FRONT_OF_APPEND_LIST = 2,
    USD_LIST_POSITION_BACK_OF_APPEND_LIST = 3
} UsdListPosition;

// ============================================================================
// MARK: - UsdTimeCode
// ============================================================================

#ifndef USD_TIME_CODE_DEFINED
#define USD_TIME_CODE_DEFINED
/// Time code value for attribute queries
typedef struct {
    double time;
    bool isDefault;
} UsdTimeCode;
#endif

/// Creates a default time code
USD_INTEROP_API UsdTimeCode UsdTimeCode_Default(void);

/// Creates a time code for a specific time
USD_INTEROP_API UsdTimeCode UsdTimeCode_Time(double time);

/// Creates the earliest time code
USD_INTEROP_API UsdTimeCode UsdTimeCode_EarliestTime(void);

// ============================================================================
// MARK: - UsdStage
// ============================================================================

// MARK: - UsdStage Lifecycle

/// Creates a new stage at the given file path.
/// Returns NULL on failure. Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdStage_CreateNew(
    const char* identifier,
    UsdInitialLoadSet load
);

/// Creates an in-memory stage.
/// Returns NULL on failure. Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdStage_CreateInMemory(
    const char* identifier,
    UsdInitialLoadSet load
);

/// Opens an existing stage from a file path.
/// Returns NULL on failure. Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdStage_Open(
    const char* filePath,
    UsdInitialLoadSet load
);

/// Opens a stage from an existing root layer.
/// Returns NULL on failure. Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdStage_OpenLayer(
    SdfLayerRef rootLayer,
    UsdInitialLoadSet load
);

/// Increments the reference count of a stage.
USD_INTEROP_API UsdStageRef UsdStage_Retain(UsdStageRef stage);

/// Decrements the reference count and releases if count reaches zero.
USD_INTEROP_API void UsdStage_Release(UsdStageRef stage);

// MARK: - UsdStage Properties

/// Returns the stage's root layer.
/// Caller must release with SdfLayer_Release.
USD_INTEROP_API SdfLayerRef UsdStage_GetRootLayer(UsdStageRef stage);

/// Returns the stage's session layer.
/// Caller must release with SdfLayer_Release.
USD_INTEROP_API SdfLayerRef UsdStage_GetSessionLayer(UsdStageRef stage);

/// Gets the start time code.
USD_INTEROP_API double UsdStage_GetStartTimeCode(UsdStageRef stage);

/// Sets the start time code.
USD_INTEROP_API UsdResult UsdStage_SetStartTimeCode(UsdStageRef stage, double timeCode);

/// Gets the end time code.
USD_INTEROP_API double UsdStage_GetEndTimeCode(UsdStageRef stage);

/// Sets the end time code.
USD_INTEROP_API UsdResult UsdStage_SetEndTimeCode(UsdStageRef stage, double timeCode);

/// Gets the time codes per second.
USD_INTEROP_API double UsdStage_GetTimeCodesPerSecond(UsdStageRef stage);

/// Sets the time codes per second.
USD_INTEROP_API UsdResult UsdStage_SetTimeCodesPerSecond(UsdStageRef stage, double tcps);

/// Gets the frames per second.
USD_INTEROP_API double UsdStage_GetFramesPerSecond(UsdStageRef stage);

/// Sets the frames per second.
USD_INTEROP_API UsdResult UsdStage_SetFramesPerSecond(UsdStageRef stage, double fps);

// MARK: - UsdStage Prim Access

/// Gets the pseudo-root prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_GetPseudoRoot(UsdStageRef stage);

/// Gets the default prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_GetDefaultPrim(UsdStageRef stage);

/// Sets the default prim.
USD_INTEROP_API UsdResult UsdStage_SetDefaultPrim(UsdStageRef stage, UsdPrimRef prim);

/// Clears the default prim.
USD_INTEROP_API UsdResult UsdStage_ClearDefaultPrim(UsdStageRef stage);

/// Returns true if the stage has a default prim set.
USD_INTEROP_API bool UsdStage_HasDefaultPrim(UsdStageRef stage);

/// Gets a prim at the specified path.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_GetPrimAtPath(UsdStageRef stage, SdfPathRef path);

/// Gets a prim at the specified path string.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_GetPrimAtPathString(UsdStageRef stage, const char* pathString);

// MARK: - UsdStage Prim Creation

/// Defines a prim at the given path.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_DefinePrim(
    UsdStageRef stage,
    SdfPathRef path,
    TfTokenRef typeName
);

/// Defines a prim at the given path string.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_DefinePrimString(
    UsdStageRef stage,
    const char* pathString,
    const char* typeName
);

/// Creates an override prim at the given path.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_OverridePrim(UsdStageRef stage, SdfPathRef path);

/// Creates an override prim at the given path string.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_OverridePrimString(UsdStageRef stage, const char* pathString);

/// Creates a class prim at the given root path.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdStage_CreateClassPrim(UsdStageRef stage, SdfPathRef rootPrimPath);

/// Removes a prim at the given path from the current edit target.
USD_INTEROP_API UsdResult UsdStage_RemovePrim(UsdStageRef stage, SdfPathRef path);

// MARK: - UsdStage Traversal (Batch)

/// Gets the count of root prims.
USD_INTEROP_API size_t UsdStage_GetRootPrimCount(UsdStageRef stage);

/// Gets root prims.
/// outPrims must have space for maxCount prims.
/// Returns the number of prims written.
/// Caller must release each prim with UsdPrim_Release.
USD_INTEROP_API size_t UsdStage_GetRootPrims(
    UsdStageRef stage,
    UsdPrimRef* outPrims,
    size_t maxCount
);

/// Gets all prims by traversing the stage.
/// outPrims must have space for maxCount prims.
/// Returns the number of prims written.
/// Caller must release each prim with UsdPrim_Release.
USD_INTEROP_API size_t UsdStage_Traverse(
    UsdStageRef stage,
    UsdPrimRef* outPrims,
    size_t maxCount
);

/// Gets the count of all prims via traversal.
USD_INTEROP_API size_t UsdStage_GetTraversalCount(UsdStageRef stage);

// MARK: - UsdStage Serialization

/// Saves all dirty layers.
USD_INTEROP_API UsdResult UsdStage_Save(UsdStageRef stage);

/// Exports the flattened stage to a file.
USD_INTEROP_API UsdResult UsdStage_Export(
    UsdStageRef stage,
    const char* filename,
    bool addSourceFileComment
);

/// Exports the flattened stage to a string.
/// Caller must free the result with UsdStage_FreeString.
USD_INTEROP_API char* UsdStage_ExportToString(UsdStageRef stage);

/// Frees a string allocated by UsdStage functions.
USD_INTEROP_API void UsdStage_FreeString(char* str);

/// Reloads all non-session layers.
USD_INTEROP_API UsdResult UsdStage_Reload(UsdStageRef stage);

// MARK: - UsdStage Layer Stack

/// Gets the count of layers in the layer stack.
USD_INTEROP_API size_t UsdStage_GetLayerStackCount(UsdStageRef stage, bool includeSessionLayers);

/// Gets layers in the layer stack.
/// outLayers must have space for maxCount layers.
/// Caller must release each layer with SdfLayer_Release.
USD_INTEROP_API size_t UsdStage_GetLayerStack(
    UsdStageRef stage,
    SdfLayerRef* outLayers,
    size_t maxCount,
    bool includeSessionLayers
);

// ============================================================================
// MARK: - UsdPrim
// ============================================================================

// MARK: - UsdPrim Lifecycle

/// Creates a copy of a prim reference (increments ref count).
USD_INTEROP_API UsdPrimRef UsdPrim_Retain(UsdPrimRef prim);

/// Releases a prim reference.
USD_INTEROP_API void UsdPrim_Release(UsdPrimRef prim);

// MARK: - UsdPrim Properties

/// Returns true if the prim is valid.
USD_INTEROP_API bool UsdPrim_IsValid(UsdPrimRef prim);

/// Returns true if the prim is active.
USD_INTEROP_API bool UsdPrim_IsActive(UsdPrimRef prim);

/// Sets the prim's active state.
USD_INTEROP_API UsdResult UsdPrim_SetActive(UsdPrimRef prim, bool active);

/// Returns true if the prim is loaded.
USD_INTEROP_API bool UsdPrim_IsLoaded(UsdPrimRef prim);

/// Returns true if the prim is a model.
USD_INTEROP_API bool UsdPrim_IsModel(UsdPrimRef prim);

/// Returns true if the prim is a group.
USD_INTEROP_API bool UsdPrim_IsGroup(UsdPrimRef prim);

/// Returns true if the prim is a component.
USD_INTEROP_API bool UsdPrim_IsComponent(UsdPrimRef prim);

/// Returns true if the prim is abstract.
USD_INTEROP_API bool UsdPrim_IsAbstract(UsdPrimRef prim);

/// Returns true if the prim is defined.
USD_INTEROP_API bool UsdPrim_IsDefined(UsdPrimRef prim);

/// Returns true if the prim has a defining specifier.
USD_INTEROP_API bool UsdPrim_HasDefiningSpecifier(UsdPrimRef prim);

/// Gets the prim's path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdPrim_GetPath(UsdPrimRef prim);

/// Gets the prim's name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdPrim_GetName(UsdPrimRef prim);

/// Gets the prim's type name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdPrim_GetTypeName(UsdPrimRef prim);

/// Sets the prim's type name.
USD_INTEROP_API UsdResult UsdPrim_SetTypeName(UsdPrimRef prim, TfTokenRef typeName);

/// Clears the prim's type name.
USD_INTEROP_API UsdResult UsdPrim_ClearTypeName(UsdPrimRef prim);

/// Gets the prim's specifier.
USD_INTEROP_API UsdSpecifier UsdPrim_GetSpecifier(UsdPrimRef prim);

/// Sets the prim's specifier.
USD_INTEROP_API UsdResult UsdPrim_SetSpecifier(UsdPrimRef prim, UsdSpecifier specifier);

/// Gets the prim's kind.
/// Returns true and populates kind if successful.
/// Caller must release kind with TfToken_Release.
USD_INTEROP_API bool UsdPrim_GetKind(UsdPrimRef prim, TfTokenRef* kind);

/// Sets the prim's kind.
USD_INTEROP_API UsdResult UsdPrim_SetKind(UsdPrimRef prim, TfTokenRef kind);

// MARK: - UsdPrim Hierarchy Navigation

/// Gets the prim's parent.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdPrim_GetParent(UsdPrimRef prim);

/// Gets the prim's stage.
/// Caller must release with UsdStage_Release.
USD_INTEROP_API UsdStageRef UsdPrim_GetStage(UsdPrimRef prim);

/// Gets the count of children.
USD_INTEROP_API size_t UsdPrim_GetChildCount(UsdPrimRef prim);

/// Gets the prim's children.
/// outChildren must have space for maxCount prims.
/// Caller must release each prim with UsdPrim_Release.
USD_INTEROP_API size_t UsdPrim_GetChildren(
    UsdPrimRef prim,
    UsdPrimRef* outChildren,
    size_t maxCount
);

/// Gets all filtered children (active, loaded, defined).
/// outChildren must have space for maxCount prims.
/// Caller must release each prim with UsdPrim_Release.
USD_INTEROP_API size_t UsdPrim_GetFilteredChildren(
    UsdPrimRef prim,
    UsdPrimRef* outChildren,
    size_t maxCount
);

/// Gets all children including inactive ones.
/// outChildren must have space for maxCount prims.
/// Caller must release each prim with UsdPrim_Release.
USD_INTEROP_API size_t UsdPrim_GetAllChildren(
    UsdPrimRef prim,
    UsdPrimRef* outChildren,
    size_t maxCount
);

/// Gets a child prim by name.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdPrim_GetChild(UsdPrimRef prim, TfTokenRef name);

/// Gets a child prim by name string.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdPrim_GetChildString(UsdPrimRef prim, const char* name);

// MARK: - UsdPrim Subtree Traversal

/// Gets the count of prims in subtree.
USD_INTEROP_API size_t UsdPrim_GetDescendantCount(UsdPrimRef prim);

/// Gets all descendants.
/// outPrims must have space for maxCount prims.
/// Caller must release each prim with UsdPrim_Release.
USD_INTEROP_API size_t UsdPrim_GetDescendants(
    UsdPrimRef prim,
    UsdPrimRef* outPrims,
    size_t maxCount
);

// MARK: - UsdPrim Properties

/// Gets the count of properties.
USD_INTEROP_API size_t UsdPrim_GetPropertyCount(UsdPrimRef prim);

/// Gets the property names.
/// outNames must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdPrim_GetPropertyNames(
    UsdPrimRef prim,
    TfTokenRef* outNames,
    size_t maxCount
);

/// Gets a property by name.
/// Caller must release with UsdProperty_Release.
USD_INTEROP_API UsdPropertyRef UsdPrim_GetProperty(UsdPrimRef prim, TfTokenRef name);

/// Returns true if the prim has a property with the given name.
USD_INTEROP_API bool UsdPrim_HasProperty(UsdPrimRef prim, TfTokenRef name);

/// Removes a property.
USD_INTEROP_API UsdResult UsdPrim_RemoveProperty(UsdPrimRef prim, TfTokenRef name);

// MARK: - UsdPrim Attributes

/// Gets the count of attributes.
USD_INTEROP_API size_t UsdPrim_GetAttributeCount(UsdPrimRef prim);

/// Gets the attribute names.
/// outNames must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdPrim_GetAttributeNames(
    UsdPrimRef prim,
    TfTokenRef* outNames,
    size_t maxCount
);

/// Gets an attribute by name.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdPrim_GetAttribute(UsdPrimRef prim, TfTokenRef name);

/// Gets an attribute by name string.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdPrim_GetAttributeString(UsdPrimRef prim, const char* name);

/// Returns true if the prim has an attribute with the given name.
USD_INTEROP_API bool UsdPrim_HasAttribute(UsdPrimRef prim, TfTokenRef name);

/// Creates an attribute.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdPrim_CreateAttribute(
    UsdPrimRef prim,
    TfTokenRef name,
    TfTokenRef typeName,
    bool custom,
    UsdVariability variability
);

/// Creates an attribute by string names.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdPrim_CreateAttributeString(
    UsdPrimRef prim,
    const char* name,
    const char* typeName,
    bool custom,
    UsdVariability variability
);

// MARK: - UsdPrim Relationships

/// Gets the count of relationships.
USD_INTEROP_API size_t UsdPrim_GetRelationshipCount(UsdPrimRef prim);

/// Gets the relationship names.
/// outNames must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdPrim_GetRelationshipNames(
    UsdPrimRef prim,
    TfTokenRef* outNames,
    size_t maxCount
);

/// Gets a relationship by name.
/// Caller must release with UsdRelationship_Release.
USD_INTEROP_API UsdRelationshipRef UsdPrim_GetRelationship(UsdPrimRef prim, TfTokenRef name);

/// Returns true if the prim has a relationship with the given name.
USD_INTEROP_API bool UsdPrim_HasRelationship(UsdPrimRef prim, TfTokenRef name);

/// Creates a relationship.
/// Caller must release with UsdRelationship_Release.
USD_INTEROP_API UsdRelationshipRef UsdPrim_CreateRelationship(
    UsdPrimRef prim,
    TfTokenRef name,
    bool custom
);

// MARK: - UsdPrim References

/// Adds a reference to another layer/prim.
USD_INTEROP_API UsdResult UsdPrim_AddReference(
    UsdPrimRef prim,
    const char* identifier,
    SdfPathRef primPath,
    UsdListPosition position
);

/// Adds an internal reference to a prim in the same layer stack.
USD_INTEROP_API UsdResult UsdPrim_AddInternalReference(
    UsdPrimRef prim,
    SdfPathRef primPath,
    UsdListPosition position
);

/// Clears all references.
USD_INTEROP_API UsdResult UsdPrim_ClearReferences(UsdPrimRef prim);

// MARK: - UsdPrim Payloads

/// Adds a payload.
USD_INTEROP_API UsdResult UsdPrim_AddPayload(
    UsdPrimRef prim,
    const char* identifier,
    SdfPathRef primPath,
    UsdListPosition position
);

/// Clears all payloads.
USD_INTEROP_API UsdResult UsdPrim_ClearPayloads(UsdPrimRef prim);

/// Loads the prim's payload.
USD_INTEROP_API UsdResult UsdPrim_Load(UsdPrimRef prim);

/// Unloads the prim's payload.
USD_INTEROP_API UsdResult UsdPrim_Unload(UsdPrimRef prim);

// MARK: - UsdPrim Variants

/// Gets the count of variant sets.
USD_INTEROP_API size_t UsdPrim_GetVariantSetCount(UsdPrimRef prim);

/// Gets the variant set names.
/// outNames must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdPrim_GetVariantSetNames(
    UsdPrimRef prim,
    TfTokenRef* outNames,
    size_t maxCount
);

/// Returns true if the prim has a variant set with the given name.
USD_INTEROP_API bool UsdPrim_HasVariantSet(UsdPrimRef prim, const char* variantSetName);

/// Gets the variant selection for a variant set.
/// Returns NULL if no selection. Caller must free with UsdPrim_FreeString.
USD_INTEROP_API char* UsdPrim_GetVariantSelection(UsdPrimRef prim, const char* variantSetName);

/// Sets the variant selection for a variant set.
USD_INTEROP_API UsdResult UsdPrim_SetVariantSelection(
    UsdPrimRef prim,
    const char* variantSetName,
    const char* variantName
);

/// Frees a string allocated by UsdPrim functions.
USD_INTEROP_API void UsdPrim_FreeString(char* str);

/// Releases multiple prims.
USD_INTEROP_API void UsdPrim_ReleaseBatch(UsdPrimRef* prims, size_t count);

// ============================================================================
// MARK: - UsdProperty
// ============================================================================

/// Creates a copy of a property reference (increments ref count).
USD_INTEROP_API UsdPropertyRef UsdProperty_Retain(UsdPropertyRef prop);

/// Releases a property reference.
USD_INTEROP_API void UsdProperty_Release(UsdPropertyRef prop);

/// Returns true if the property is valid.
USD_INTEROP_API bool UsdProperty_IsValid(UsdPropertyRef prop);

/// Returns true if the property is an attribute.
USD_INTEROP_API bool UsdProperty_IsAttribute(UsdPropertyRef prop);

/// Returns true if the property is a relationship.
USD_INTEROP_API bool UsdProperty_IsRelationship(UsdPropertyRef prop);

/// Gets the property's name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdProperty_GetName(UsdPropertyRef prop);

/// Gets the property's base name (without namespace prefix).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdProperty_GetBaseName(UsdPropertyRef prop);

/// Gets the property's namespace prefix.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdProperty_GetNamespace(UsdPropertyRef prop);

/// Gets the property's path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdProperty_GetPath(UsdPropertyRef prop);

/// Gets the property's owning prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdProperty_GetPrim(UsdPropertyRef prop);

/// Returns true if the property is custom (user-defined).
USD_INTEROP_API bool UsdProperty_IsCustom(UsdPropertyRef prop);

/// Returns true if the property is defined.
USD_INTEROP_API bool UsdProperty_IsDefined(UsdPropertyRef prop);

/// Returns true if the property is authored.
USD_INTEROP_API bool UsdProperty_IsAuthored(UsdPropertyRef prop);

// ============================================================================
// MARK: - UsdAttribute
// ============================================================================

/// Creates a copy of an attribute reference (increments ref count).
USD_INTEROP_API UsdAttributeRef UsdAttribute_Retain(UsdAttributeRef attr);

/// Releases an attribute reference.
USD_INTEROP_API void UsdAttribute_Release(UsdAttributeRef attr);

/// Returns true if the attribute is valid.
USD_INTEROP_API bool UsdAttribute_IsValid(UsdAttributeRef attr);

/// Gets the attribute's name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdAttribute_GetName(UsdAttributeRef attr);

/// Gets the attribute's path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdAttribute_GetPath(UsdAttributeRef attr);

/// Gets the attribute's owning prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdAttribute_GetPrim(UsdAttributeRef attr);

/// Gets the attribute's type name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdAttribute_GetTypeName(UsdAttributeRef attr);

/// Gets the attribute's variability.
USD_INTEROP_API UsdVariability UsdAttribute_GetVariability(UsdAttributeRef attr);

/// Returns true if the attribute has a value.
USD_INTEROP_API bool UsdAttribute_HasValue(UsdAttributeRef attr);

/// Returns true if the attribute has an authored value.
USD_INTEROP_API bool UsdAttribute_HasAuthoredValue(UsdAttributeRef attr);

/// Returns true if the attribute has a fallback value.
USD_INTEROP_API bool UsdAttribute_HasFallbackValue(UsdAttributeRef attr);

/// Returns true if the attribute value might be time-varying.
USD_INTEROP_API bool UsdAttribute_ValueMightBeTimeVarying(UsdAttributeRef attr);

// MARK: - UsdAttribute Value Access (Type-specific)

/// Gets a bool value.
USD_INTEROP_API UsdResult UsdAttribute_GetBool(UsdAttributeRef attr, UsdTimeCode time, bool* value);

/// Sets a bool value.
USD_INTEROP_API UsdResult UsdAttribute_SetBool(UsdAttributeRef attr, UsdTimeCode time, bool value);

/// Gets an int value.
USD_INTEROP_API UsdResult UsdAttribute_GetInt(UsdAttributeRef attr, UsdTimeCode time, int* value);

/// Sets an int value.
USD_INTEROP_API UsdResult UsdAttribute_SetInt(UsdAttributeRef attr, UsdTimeCode time, int value);

/// Gets a float value.
USD_INTEROP_API UsdResult UsdAttribute_GetFloat(UsdAttributeRef attr, UsdTimeCode time, float* value);

/// Sets a float value.
USD_INTEROP_API UsdResult UsdAttribute_SetFloat(UsdAttributeRef attr, UsdTimeCode time, float value);

/// Gets a double value.
USD_INTEROP_API UsdResult UsdAttribute_GetDouble(UsdAttributeRef attr, UsdTimeCode time, double* value);

/// Sets a double value.
USD_INTEROP_API UsdResult UsdAttribute_SetDouble(UsdAttributeRef attr, UsdTimeCode time, double value);

/// Gets a string value.
/// Caller must free the result with UsdAttribute_FreeString.
USD_INTEROP_API UsdResult UsdAttribute_GetString(UsdAttributeRef attr, UsdTimeCode time, char** value);

/// Sets a string value.
USD_INTEROP_API UsdResult UsdAttribute_SetString(UsdAttributeRef attr, UsdTimeCode time, const char* value);

/// Gets a token value.
/// Caller must release with TfToken_Release.
USD_INTEROP_API UsdResult UsdAttribute_GetToken(UsdAttributeRef attr, UsdTimeCode time, TfTokenRef* value);

/// Sets a token value.
USD_INTEROP_API UsdResult UsdAttribute_SetToken(UsdAttributeRef attr, UsdTimeCode time, TfTokenRef value);

/// Gets a Vec3f value.
USD_INTEROP_API UsdResult UsdAttribute_GetVec3f(UsdAttributeRef attr, UsdTimeCode time, float* x, float* y, float* z);

/// Sets a Vec3f value.
USD_INTEROP_API UsdResult UsdAttribute_SetVec3f(UsdAttributeRef attr, UsdTimeCode time, float x, float y, float z);

/// Gets a Vec3d value.
USD_INTEROP_API UsdResult UsdAttribute_GetVec3d(UsdAttributeRef attr, UsdTimeCode time, double* x, double* y, double* z);

/// Sets a Vec3d value.
USD_INTEROP_API UsdResult UsdAttribute_SetVec3d(UsdAttributeRef attr, UsdTimeCode time, double x, double y, double z);

/// Gets a Matrix4d value.
USD_INTEROP_API UsdResult UsdAttribute_GetMatrix4d(UsdAttributeRef attr, UsdTimeCode time, double* values);

/// Sets a Matrix4d value (values is 16 doubles in row-major order).
USD_INTEROP_API UsdResult UsdAttribute_SetMatrix4d(UsdAttributeRef attr, UsdTimeCode time, const double* values);

/// Gets a VtValue (generic typed value).
/// Caller must release with VtValue_Release.
USD_INTEROP_API UsdResult UsdAttribute_GetValue(UsdAttributeRef attr, UsdTimeCode time, VtValueRef* value);

/// Sets a VtValue (generic typed value).
USD_INTEROP_API UsdResult UsdAttribute_SetValue(UsdAttributeRef attr, UsdTimeCode time, VtValueRef value);

/// Gets an asset path value.
/// Caller must release with SdfAssetPath_Release.
USD_INTEROP_API UsdResult UsdAttribute_GetAssetPath(UsdAttributeRef attr, UsdTimeCode time, SdfAssetPathRef* value);

/// Sets an asset path value.
USD_INTEROP_API UsdResult UsdAttribute_SetAssetPath(UsdAttributeRef attr, UsdTimeCode time, SdfAssetPathRef value);

// MARK: - UsdAttribute Time Samples

/// Gets the count of time samples.
USD_INTEROP_API size_t UsdAttribute_GetNumTimeSamples(UsdAttributeRef attr);

/// Gets the time sample times.
/// outTimes must have space for maxCount times.
/// Returns the number of times written.
USD_INTEROP_API size_t UsdAttribute_GetTimeSamples(
    UsdAttributeRef attr,
    double* outTimes,
    size_t maxCount
);

/// Gets bracketing time samples.
USD_INTEROP_API UsdResult UsdAttribute_GetBracketingTimeSamples(
    UsdAttributeRef attr,
    double desiredTime,
    double* lowerTime,
    double* upperTime,
    bool* hasTimeSamples
);

/// Clears all values (default and time samples).
USD_INTEROP_API UsdResult UsdAttribute_Clear(UsdAttributeRef attr);

/// Clears the value at a specific time.
USD_INTEROP_API UsdResult UsdAttribute_ClearAtTime(UsdAttributeRef attr, UsdTimeCode time);

/// Clears the default value.
USD_INTEROP_API UsdResult UsdAttribute_ClearDefault(UsdAttributeRef attr);

/// Blocks the attribute value.
USD_INTEROP_API UsdResult UsdAttribute_Block(UsdAttributeRef attr);

/// Frees a string allocated by UsdAttribute functions.
USD_INTEROP_API void UsdAttribute_FreeString(char* str);

// MARK: - UsdAttribute Connections

/// Gets the count of connections.
USD_INTEROP_API size_t UsdAttribute_GetConnectionCount(UsdAttributeRef attr);

/// Gets the connections.
/// outPaths must have space for maxCount paths.
/// Caller must release each path with SdfPath_Release.
USD_INTEROP_API size_t UsdAttribute_GetConnections(
    UsdAttributeRef attr,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Adds a connection.
USD_INTEROP_API UsdResult UsdAttribute_AddConnection(
    UsdAttributeRef attr,
    SdfPathRef source,
    UsdListPosition position
);

/// Removes a connection.
USD_INTEROP_API UsdResult UsdAttribute_RemoveConnection(UsdAttributeRef attr, SdfPathRef source);

/// Clears all connections.
USD_INTEROP_API UsdResult UsdAttribute_ClearConnections(UsdAttributeRef attr);

/// Sets connections (replaces all).
/// sources must have count paths.
USD_INTEROP_API UsdResult UsdAttribute_SetConnections(
    UsdAttributeRef attr,
    SdfPathRef* sources,
    size_t count
);

/// Returns true if the attribute has authored connections.
USD_INTEROP_API bool UsdAttribute_HasAuthoredConnections(UsdAttributeRef attr);

// ============================================================================
// MARK: - UsdRelationship
// ============================================================================

/// Creates a copy of a relationship reference (increments ref count).
USD_INTEROP_API UsdRelationshipRef UsdRelationship_Retain(UsdRelationshipRef rel);

/// Releases a relationship reference.
USD_INTEROP_API void UsdRelationship_Release(UsdRelationshipRef rel);

/// Returns true if the relationship is valid.
USD_INTEROP_API bool UsdRelationship_IsValid(UsdRelationshipRef rel);

/// Gets the relationship's name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdRelationship_GetName(UsdRelationshipRef rel);

/// Gets the relationship's path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdRelationship_GetPath(UsdRelationshipRef rel);

/// Gets the relationship's owning prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdRelationship_GetPrim(UsdRelationshipRef rel);

// MARK: - UsdRelationship Targets

/// Gets the count of targets.
USD_INTEROP_API size_t UsdRelationship_GetTargetCount(UsdRelationshipRef rel);

/// Gets the targets.
/// outPaths must have space for maxCount paths.
/// Caller must release each path with SdfPath_Release.
USD_INTEROP_API size_t UsdRelationship_GetTargets(
    UsdRelationshipRef rel,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Gets the forwarded targets (resolves forwarding relationships).
/// outPaths must have space for maxCount paths.
/// Caller must release each path with SdfPath_Release.
USD_INTEROP_API size_t UsdRelationship_GetForwardedTargets(
    UsdRelationshipRef rel,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Adds a target.
USD_INTEROP_API UsdResult UsdRelationship_AddTarget(
    UsdRelationshipRef rel,
    SdfPathRef target,
    UsdListPosition position
);

/// Removes a target.
USD_INTEROP_API UsdResult UsdRelationship_RemoveTarget(UsdRelationshipRef rel, SdfPathRef target);

/// Sets targets (replaces all).
/// targets must have count paths.
USD_INTEROP_API UsdResult UsdRelationship_SetTargets(
    UsdRelationshipRef rel,
    SdfPathRef* targets,
    size_t count
);

/// Clears all targets.
USD_INTEROP_API UsdResult UsdRelationship_ClearTargets(UsdRelationshipRef rel, bool removeSpec);

/// Returns true if the relationship has authored targets.
USD_INTEROP_API bool UsdRelationship_HasAuthoredTargets(UsdRelationshipRef rel);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_USD_H
