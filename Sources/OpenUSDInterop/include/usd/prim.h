// prim.h - UsdPrim C wrapper
// Mirrors: pxr/usd/usd/prim.h

#ifndef OPENUSDINTEROP_USD_PRIM_H
#define OPENUSDINTEROP_USD_PRIM_H

#include "../OpenUSDInterop.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Prim Lifecycle

/// Destroys a prim handle (does not delete the prim from the stage).
USD_INTEROP_API void UsdPrim_Destroy(UsdPrimRef prim);

/// Returns true if the prim is valid.
USD_INTEROP_API bool UsdPrim_IsValid(UsdPrimRef prim);

// MARK: - Prim Properties

/// Returns the path of the prim.
USD_INTEROP_API UsdResult UsdPrim_GetPath(UsdPrimRef prim, UsdSdfPathRef* outPath);

/// Returns the name of the prim.
USD_INTEROP_API UsdResult UsdPrim_GetName(UsdPrimRef prim, UsdTfTokenRef* outName);

/// Returns the type name of the prim.
USD_INTEROP_API UsdResult UsdPrim_GetTypeName(UsdPrimRef prim, UsdTfTokenRef* outTypeName);

/// Returns true if the prim is active.
USD_INTEROP_API bool UsdPrim_IsActive(UsdPrimRef prim);

/// Sets whether the prim is active.
USD_INTEROP_API UsdResult UsdPrim_SetActive(UsdPrimRef prim, bool active);

/// Returns true if the prim is defined.
USD_INTEROP_API bool UsdPrim_IsDefined(UsdPrimRef prim);

/// Returns true if the prim is abstract (no type).
USD_INTEROP_API bool UsdPrim_IsAbstract(UsdPrimRef prim);

// MARK: - Hierarchy

/// Returns the parent prim.
USD_INTEROP_API UsdResult UsdPrim_GetParent(UsdPrimRef prim, UsdPrimRef* outParent);

/// Returns the number of children.
USD_INTEROP_API size_t UsdPrim_GetChildCount(UsdPrimRef prim);

/// Returns the child at the given index.
USD_INTEROP_API UsdResult UsdPrim_GetChildAtIndex(UsdPrimRef prim, size_t index, UsdPrimRef* outChild);

/// Returns a child by name.
USD_INTEROP_API UsdResult UsdPrim_GetChildByName(UsdPrimRef prim, UsdTfTokenRef name, UsdPrimRef* outChild);

// MARK: - Attributes

/// Returns the number of attributes.
USD_INTEROP_API size_t UsdPrim_GetAttributeCount(UsdPrimRef prim);

/// Returns an attribute by name.
USD_INTEROP_API UsdResult UsdPrim_GetAttribute(UsdPrimRef prim, UsdTfTokenRef name, UsdAttributeRef* outAttr);

/// Creates a new attribute.
USD_INTEROP_API UsdResult UsdPrim_CreateAttribute(
    UsdPrimRef prim,
    UsdTfTokenRef name,
    UsdTfTokenRef typeName,
    bool custom,
    UsdAttributeRef* outAttr
);

/// Returns true if the prim has an attribute with the given name.
USD_INTEROP_API bool UsdPrim_HasAttribute(UsdPrimRef prim, UsdTfTokenRef name);

// MARK: - Metadata

/// Returns true if the prim has the given metadata key.
USD_INTEROP_API bool UsdPrim_HasMetadata(UsdPrimRef prim, UsdTfTokenRef key);

/// Gets a string metadata value.
USD_INTEROP_API UsdResult UsdPrim_GetMetadataString(UsdPrimRef prim, UsdTfTokenRef key, char* buffer, size_t bufferSize);

/// Sets a string metadata value.
USD_INTEROP_API UsdResult UsdPrim_SetMetadataString(UsdPrimRef prim, UsdTfTokenRef key, const char* value);

// MARK: - Schema

/// Returns true if the prim has the given applied API schema.
USD_INTEROP_API bool UsdPrim_HasAPI(UsdPrimRef prim, UsdTfTokenRef schemaName);

/// Applies an API schema to the prim.
USD_INTEROP_API UsdResult UsdPrim_ApplyAPI(UsdPrimRef prim, UsdTfTokenRef schemaName);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_PRIM_H
