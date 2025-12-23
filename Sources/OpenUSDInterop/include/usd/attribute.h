// attribute.h - UsdAttribute C wrapper
// Mirrors: pxr/usd/usd/attribute.h

#ifndef OPENUSDINTEROP_USD_ATTRIBUTE_H
#define OPENUSDINTEROP_USD_ATTRIBUTE_H

#include "../OpenUSDInterop.h"
#include "../base/gf.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Attribute Lifecycle

/// Destroys an attribute handle.
USD_INTEROP_API void UsdAttribute_Destroy(UsdAttributeRef attr);

/// Returns true if the attribute is valid.
USD_INTEROP_API bool UsdAttribute_IsValid(UsdAttributeRef attr);

// MARK: - Attribute Properties

/// Returns the name of the attribute.
USD_INTEROP_API UsdResult UsdAttribute_GetName(UsdAttributeRef attr, UsdTfTokenRef* outName);

/// Returns the path of the attribute.
USD_INTEROP_API UsdResult UsdAttribute_GetPath(UsdAttributeRef attr, UsdSdfPathRef* outPath);

/// Returns the type name of the attribute.
USD_INTEROP_API UsdResult UsdAttribute_GetTypeName(UsdAttributeRef attr, UsdTfTokenRef* outTypeName);

/// Returns true if the attribute has a value at the given time.
USD_INTEROP_API bool UsdAttribute_HasValue(UsdAttributeRef attr);

/// Returns true if the attribute has time samples.
USD_INTEROP_API bool UsdAttribute_HasTimeSamples(UsdAttributeRef attr);

/// Returns the number of time samples.
USD_INTEROP_API size_t UsdAttribute_GetTimeSampleCount(UsdAttributeRef attr);

// MARK: - Value Access (Scalar Types)

/// Gets a bool value.
USD_INTEROP_API UsdResult UsdAttribute_GetBool(UsdAttributeRef attr, UsdTimeCode time, bool* outValue);

/// Sets a bool value.
USD_INTEROP_API UsdResult UsdAttribute_SetBool(UsdAttributeRef attr, UsdTimeCode time, bool value);

/// Gets an int value.
USD_INTEROP_API UsdResult UsdAttribute_GetInt(UsdAttributeRef attr, UsdTimeCode time, int32_t* outValue);

/// Sets an int value.
USD_INTEROP_API UsdResult UsdAttribute_SetInt(UsdAttributeRef attr, UsdTimeCode time, int32_t value);

/// Gets a float value.
USD_INTEROP_API UsdResult UsdAttribute_GetFloat(UsdAttributeRef attr, UsdTimeCode time, float* outValue);

/// Sets a float value.
USD_INTEROP_API UsdResult UsdAttribute_SetFloat(UsdAttributeRef attr, UsdTimeCode time, float value);

/// Gets a double value.
USD_INTEROP_API UsdResult UsdAttribute_GetDouble(UsdAttributeRef attr, UsdTimeCode time, double* outValue);

/// Sets a double value.
USD_INTEROP_API UsdResult UsdAttribute_SetDouble(UsdAttributeRef attr, UsdTimeCode time, double value);

/// Gets a string value.
USD_INTEROP_API UsdResult UsdAttribute_GetString(UsdAttributeRef attr, UsdTimeCode time, char* buffer, size_t bufferSize);

/// Sets a string value.
USD_INTEROP_API UsdResult UsdAttribute_SetString(UsdAttributeRef attr, UsdTimeCode time, const char* value);

// MARK: - Value Access (Vector Types)

/// Gets a Vec3f value.
USD_INTEROP_API UsdResult UsdAttribute_GetVec3f(UsdAttributeRef attr, UsdTimeCode time, UsdGfVec3f* outValue);

/// Sets a Vec3f value.
USD_INTEROP_API UsdResult UsdAttribute_SetVec3f(UsdAttributeRef attr, UsdTimeCode time, UsdGfVec3f value);

/// Gets a Vec3d value.
USD_INTEROP_API UsdResult UsdAttribute_GetVec3d(UsdAttributeRef attr, UsdTimeCode time, UsdGfVec3d* outValue);

/// Sets a Vec3d value.
USD_INTEROP_API UsdResult UsdAttribute_SetVec3d(UsdAttributeRef attr, UsdTimeCode time, UsdGfVec3d value);

/// Gets a Matrix4d value.
USD_INTEROP_API UsdResult UsdAttribute_GetMatrix4d(UsdAttributeRef attr, UsdTimeCode time, UsdGfMatrix4d* outValue);

/// Sets a Matrix4d value.
USD_INTEROP_API UsdResult UsdAttribute_SetMatrix4d(UsdAttributeRef attr, UsdTimeCode time, UsdGfMatrix4d value);

// MARK: - Value Access (Array Types)

/// Gets a VtArray value. Caller owns the returned array.
USD_INTEROP_API UsdResult UsdAttribute_GetVtArray(UsdAttributeRef attr, UsdTimeCode time, UsdVtArrayRef* outArray);

/// Sets a VtArray value.
USD_INTEROP_API UsdResult UsdAttribute_SetVtArray(UsdAttributeRef attr, UsdTimeCode time, UsdVtArrayRef array);

// MARK: - Value Clearing

/// Clears the value at the default time.
USD_INTEROP_API UsdResult UsdAttribute_Clear(UsdAttributeRef attr);

/// Clears the value at a specific time.
USD_INTEROP_API UsdResult UsdAttribute_ClearAtTime(UsdAttributeRef attr, UsdTimeCode time);

/// Clears all time samples.
USD_INTEROP_API UsdResult UsdAttribute_ClearAllTimeSamples(UsdAttributeRef attr);

// MARK: - Connections

/// Returns true if the attribute has any connections.
USD_INTEROP_API bool UsdAttribute_HasConnections(UsdAttributeRef attr);

/// Adds a connection to another attribute.
USD_INTEROP_API UsdResult UsdAttribute_AddConnection(UsdAttributeRef attr, UsdSdfPathRef targetPath);

/// Removes a connection.
USD_INTEROP_API UsdResult UsdAttribute_RemoveConnection(UsdAttributeRef attr, UsdSdfPathRef targetPath);

/// Clears all connections.
USD_INTEROP_API UsdResult UsdAttribute_ClearConnections(UsdAttributeRef attr);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_ATTRIBUTE_H
