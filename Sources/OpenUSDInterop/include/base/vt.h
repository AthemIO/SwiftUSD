// vt.h - Vt (Value Types) C wrapper
// Mirrors: pxr/base/vt/

#ifndef OPENUSDINTEROP_BASE_VT_H
#define OPENUSDINTEROP_BASE_VT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations - these are defined in OpenUSDInterop.h
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
// MARK: - Type Definitions
// ============================================================================

/// Value type identifiers for VtValue.
/// Corresponds to commonly used types in USD.
typedef enum {
    VT_TYPE_UNKNOWN = 0,
    VT_TYPE_BOOL,
    VT_TYPE_INT,
    VT_TYPE_INT64,
    VT_TYPE_UINT,
    VT_TYPE_UINT64,
    VT_TYPE_FLOAT,
    VT_TYPE_DOUBLE,
    VT_TYPE_HALF,
    VT_TYPE_STRING,
    VT_TYPE_TOKEN,
    VT_TYPE_VEC2F,
    VT_TYPE_VEC2D,
    VT_TYPE_VEC2I,
    VT_TYPE_VEC3F,
    VT_TYPE_VEC3D,
    VT_TYPE_VEC3I,
    VT_TYPE_VEC4F,
    VT_TYPE_VEC4D,
    VT_TYPE_VEC4I,
    VT_TYPE_MATRIX2D,
    VT_TYPE_MATRIX3D,
    VT_TYPE_MATRIX4D,
    VT_TYPE_QUATD,
    VT_TYPE_QUATF,
    VT_TYPE_RANGE3D,
    VT_TYPE_ARRAY_BOOL,
    VT_TYPE_ARRAY_INT,
    VT_TYPE_ARRAY_INT64,
    VT_TYPE_ARRAY_UINT,
    VT_TYPE_ARRAY_UINT64,
    VT_TYPE_ARRAY_FLOAT,
    VT_TYPE_ARRAY_DOUBLE,
    VT_TYPE_ARRAY_HALF,
    VT_TYPE_ARRAY_STRING,
    VT_TYPE_ARRAY_TOKEN,
    VT_TYPE_ARRAY_VEC2F,
    VT_TYPE_ARRAY_VEC2D,
    VT_TYPE_ARRAY_VEC3F,
    VT_TYPE_ARRAY_VEC3D,
    VT_TYPE_ARRAY_VEC4F,
    VT_TYPE_ARRAY_VEC4D,
    VT_TYPE_ARRAY_MATRIX4D,
    VT_TYPE_DICTIONARY
} VtValueType;

// ============================================================================
// MARK: - Forward Declarations (Opaque Handles)
// ============================================================================

/// Opaque handle to a VtValue wrapper.
typedef struct VtValueOpaque* VtValueRef;

/// Opaque handle to a VtArray<int> wrapper.
typedef struct VtArrayIntOpaque* VtArrayIntRef;

/// Opaque handle to a VtArray<int64_t> wrapper.
typedef struct VtArrayInt64Opaque* VtArrayInt64Ref;

/// Opaque handle to a VtArray<float> wrapper.
typedef struct VtArrayFloatOpaque* VtArrayFloatRef;

/// Opaque handle to a VtArray<double> wrapper.
typedef struct VtArrayDoubleOpaque* VtArrayDoubleRef;

/// Opaque handle to a VtArray<GfVec3f> wrapper.
typedef struct VtArrayVec3fOpaque* VtArrayVec3fRef;

/// Opaque handle to a VtArray<GfVec3d> wrapper.
typedef struct VtArrayVec3dOpaque* VtArrayVec3dRef;

/// Opaque handle to a VtDictionary wrapper.
typedef struct VtDictionaryOpaque* VtDictionaryRef;

// ============================================================================
// MARK: - VtValue (Type-erased container)
// ============================================================================

// MARK: VtValue Lifecycle

/// Creates an empty VtValue.
USD_INTEROP_API VtValueRef VtValue_Create(void);

/// Creates a VtValue holding a boolean.
USD_INTEROP_API VtValueRef VtValue_CreateBool(bool value);

/// Creates a VtValue holding an int.
USD_INTEROP_API VtValueRef VtValue_CreateInt(int value);

/// Creates a VtValue holding an int64.
USD_INTEROP_API VtValueRef VtValue_CreateInt64(int64_t value);

/// Creates a VtValue holding an unsigned int.
USD_INTEROP_API VtValueRef VtValue_CreateUInt(unsigned int value);

/// Creates a VtValue holding a uint64.
USD_INTEROP_API VtValueRef VtValue_CreateUInt64(uint64_t value);

/// Creates a VtValue holding a float.
USD_INTEROP_API VtValueRef VtValue_CreateFloat(float value);

/// Creates a VtValue holding a double.
USD_INTEROP_API VtValueRef VtValue_CreateDouble(double value);

/// Creates a VtValue holding a string.
USD_INTEROP_API VtValueRef VtValue_CreateString(const char* value);

/// Creates a copy of an existing VtValue.
USD_INTEROP_API VtValueRef VtValue_Copy(VtValueRef value);

/// Increments the reference count.
USD_INTEROP_API VtValueRef VtValue_Retain(VtValueRef value);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtValue_Release(VtValueRef value);

// MARK: VtValue Type Inspection

/// Returns true if the VtValue is empty (holds no value).
USD_INTEROP_API bool VtValue_IsEmpty(VtValueRef value);

/// Returns the type identifier for the held value.
USD_INTEROP_API VtValueType VtValue_GetType(VtValueRef value);

/// Returns the type name as a string.
/// The returned string is valid as long as the value exists.
USD_INTEROP_API const char* VtValue_GetTypeName(VtValueRef value);

/// Returns true if the value is holding an array type.
USD_INTEROP_API bool VtValue_IsArrayValued(VtValueRef value);

/// Returns the array size if holding an array, 0 otherwise.
USD_INTEROP_API size_t VtValue_GetArraySize(VtValueRef value);

/// Returns true if the value holds the specified type.
USD_INTEROP_API bool VtValue_IsHolding(VtValueRef value, VtValueType type);

// MARK: VtValue Getters (scalar types)

/// Gets the boolean value. Returns false if not holding bool.
USD_INTEROP_API bool VtValue_GetBool(VtValueRef value);

/// Gets the int value. Returns 0 if not holding int.
USD_INTEROP_API int VtValue_GetInt(VtValueRef value);

/// Gets the int64 value. Returns 0 if not holding int64.
USD_INTEROP_API int64_t VtValue_GetInt64(VtValueRef value);

/// Gets the unsigned int value. Returns 0 if not holding uint.
USD_INTEROP_API unsigned int VtValue_GetUInt(VtValueRef value);

/// Gets the uint64 value. Returns 0 if not holding uint64.
USD_INTEROP_API uint64_t VtValue_GetUInt64(VtValueRef value);

/// Gets the float value. Returns 0 if not holding float.
USD_INTEROP_API float VtValue_GetFloat(VtValueRef value);

/// Gets the double value. Returns 0 if not holding double.
USD_INTEROP_API double VtValue_GetDouble(VtValueRef value);

/// Gets the string value. Returns empty string if not holding string.
/// Caller must free the returned string with VtValue_FreeString.
USD_INTEROP_API char* VtValue_GetString(VtValueRef value);

/// Frees a string allocated by VtValue_GetString.
USD_INTEROP_API void VtValue_FreeString(char* str);

// MARK: VtValue Setters

/// Sets the value to a boolean.
USD_INTEROP_API void VtValue_SetBool(VtValueRef value, bool v);

/// Sets the value to an int.
USD_INTEROP_API void VtValue_SetInt(VtValueRef value, int v);

/// Sets the value to an int64.
USD_INTEROP_API void VtValue_SetInt64(VtValueRef value, int64_t v);

/// Sets the value to a float.
USD_INTEROP_API void VtValue_SetFloat(VtValueRef value, float v);

/// Sets the value to a double.
USD_INTEROP_API void VtValue_SetDouble(VtValueRef value, double v);

/// Sets the value to a string.
USD_INTEROP_API void VtValue_SetString(VtValueRef value, const char* str);

/// Clears the value (makes it empty).
USD_INTEROP_API void VtValue_Clear(VtValueRef value);

// MARK: VtValue Comparison

/// Returns true if two values are equal.
USD_INTEROP_API bool VtValue_Equal(VtValueRef v1, VtValueRef v2);

/// Returns a hash value for the VtValue.
USD_INTEROP_API size_t VtValue_Hash(VtValueRef value);

/// Returns true if the value can be hashed.
USD_INTEROP_API bool VtValue_CanHash(VtValueRef value);

// MARK: VtValue Casting

/// Returns true if the value can be cast to the specified type.
USD_INTEROP_API bool VtValue_CanCast(VtValueRef value, VtValueType toType);

/// Casts the value to the specified type in place.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult VtValue_Cast(VtValueRef value, VtValueType toType);

// ============================================================================
// MARK: - VtArray<int> (Integer array)
// ============================================================================

// MARK: VtArrayInt Lifecycle

/// Creates an empty int array.
USD_INTEROP_API VtArrayIntRef VtArrayInt_Create(void);

/// Creates an int array with the specified size.
USD_INTEROP_API VtArrayIntRef VtArrayInt_CreateWithSize(size_t size);

/// Creates an int array from a C array.
USD_INTEROP_API VtArrayIntRef VtArrayInt_CreateFromData(const int* data, size_t count);

/// Creates a copy of an existing int array.
USD_INTEROP_API VtArrayIntRef VtArrayInt_Copy(VtArrayIntRef array);

/// Increments the reference count.
USD_INTEROP_API VtArrayIntRef VtArrayInt_Retain(VtArrayIntRef array);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtArrayInt_Release(VtArrayIntRef array);

// MARK: VtArrayInt Properties

/// Returns the number of elements in the array.
USD_INTEROP_API size_t VtArrayInt_GetSize(VtArrayIntRef array);

/// Returns true if the array is empty.
USD_INTEROP_API bool VtArrayInt_IsEmpty(VtArrayIntRef array);

/// Returns the current capacity of the array.
USD_INTEROP_API size_t VtArrayInt_GetCapacity(VtArrayIntRef array);

/// Returns a hash value for the array.
USD_INTEROP_API size_t VtArrayInt_Hash(VtArrayIntRef array);

// MARK: VtArrayInt Element Access

/// Gets an element at the specified index. Returns 0 if out of bounds.
USD_INTEROP_API int VtArrayInt_GetElement(VtArrayIntRef array, size_t index);

/// Sets an element at the specified index.
USD_INTEROP_API void VtArrayInt_SetElement(VtArrayIntRef array, size_t index, int value);

/// Gets multiple elements. Returns number of elements copied.
/// Buffer must have space for at least count elements.
USD_INTEROP_API size_t VtArrayInt_GetElements(VtArrayIntRef array, int* buffer, size_t count);

/// Copies all elements to a newly allocated buffer.
/// Caller must free the returned buffer with free().
/// Returns NULL on failure. outCount receives the number of elements.
USD_INTEROP_API int* VtArrayInt_CopyElements(VtArrayIntRef array, size_t* outCount);

// MARK: VtArrayInt Modification

/// Reserves capacity for at least the specified number of elements.
USD_INTEROP_API void VtArrayInt_Reserve(VtArrayIntRef array, size_t capacity);

/// Resizes the array to the specified size.
USD_INTEROP_API void VtArrayInt_Resize(VtArrayIntRef array, size_t size);

/// Appends an element to the end of the array.
USD_INTEROP_API void VtArrayInt_PushBack(VtArrayIntRef array, int value);

/// Removes the last element from the array.
USD_INTEROP_API void VtArrayInt_PopBack(VtArrayIntRef array);

/// Clears all elements from the array.
USD_INTEROP_API void VtArrayInt_Clear(VtArrayIntRef array);

/// Assigns new data to the array.
USD_INTEROP_API void VtArrayInt_Assign(VtArrayIntRef array, const int* data, size_t count);

// MARK: VtArrayInt Comparison

/// Returns true if two arrays are equal.
USD_INTEROP_API bool VtArrayInt_Equal(VtArrayIntRef a1, VtArrayIntRef a2);

// ============================================================================
// MARK: - VtArray<float> (Float array)
// ============================================================================

// MARK: VtArrayFloat Lifecycle

/// Creates an empty float array.
USD_INTEROP_API VtArrayFloatRef VtArrayFloat_Create(void);

/// Creates a float array with the specified size.
USD_INTEROP_API VtArrayFloatRef VtArrayFloat_CreateWithSize(size_t size);

/// Creates a float array from a C array.
USD_INTEROP_API VtArrayFloatRef VtArrayFloat_CreateFromData(const float* data, size_t count);

/// Creates a copy of an existing float array.
USD_INTEROP_API VtArrayFloatRef VtArrayFloat_Copy(VtArrayFloatRef array);

/// Increments the reference count.
USD_INTEROP_API VtArrayFloatRef VtArrayFloat_Retain(VtArrayFloatRef array);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtArrayFloat_Release(VtArrayFloatRef array);

// MARK: VtArrayFloat Properties

/// Returns the number of elements in the array.
USD_INTEROP_API size_t VtArrayFloat_GetSize(VtArrayFloatRef array);

/// Returns true if the array is empty.
USD_INTEROP_API bool VtArrayFloat_IsEmpty(VtArrayFloatRef array);

/// Returns the current capacity of the array.
USD_INTEROP_API size_t VtArrayFloat_GetCapacity(VtArrayFloatRef array);

/// Returns a hash value for the array.
USD_INTEROP_API size_t VtArrayFloat_Hash(VtArrayFloatRef array);

// MARK: VtArrayFloat Element Access

/// Gets an element at the specified index. Returns 0 if out of bounds.
USD_INTEROP_API float VtArrayFloat_GetElement(VtArrayFloatRef array, size_t index);

/// Sets an element at the specified index.
USD_INTEROP_API void VtArrayFloat_SetElement(VtArrayFloatRef array, size_t index, float value);

/// Gets multiple elements. Returns number of elements copied.
USD_INTEROP_API size_t VtArrayFloat_GetElements(VtArrayFloatRef array, float* buffer, size_t count);

/// Copies all elements to a newly allocated buffer.
/// Caller must free the returned buffer with free().
USD_INTEROP_API float* VtArrayFloat_CopyElements(VtArrayFloatRef array, size_t* outCount);

// MARK: VtArrayFloat Modification

/// Reserves capacity for at least the specified number of elements.
USD_INTEROP_API void VtArrayFloat_Reserve(VtArrayFloatRef array, size_t capacity);

/// Resizes the array to the specified size.
USD_INTEROP_API void VtArrayFloat_Resize(VtArrayFloatRef array, size_t size);

/// Appends an element to the end of the array.
USD_INTEROP_API void VtArrayFloat_PushBack(VtArrayFloatRef array, float value);

/// Removes the last element from the array.
USD_INTEROP_API void VtArrayFloat_PopBack(VtArrayFloatRef array);

/// Clears all elements from the array.
USD_INTEROP_API void VtArrayFloat_Clear(VtArrayFloatRef array);

/// Assigns new data to the array.
USD_INTEROP_API void VtArrayFloat_Assign(VtArrayFloatRef array, const float* data, size_t count);

// MARK: VtArrayFloat Comparison

/// Returns true if two arrays are equal.
USD_INTEROP_API bool VtArrayFloat_Equal(VtArrayFloatRef a1, VtArrayFloatRef a2);

// ============================================================================
// MARK: - VtArray<double> (Double array)
// ============================================================================

// MARK: VtArrayDouble Lifecycle

/// Creates an empty double array.
USD_INTEROP_API VtArrayDoubleRef VtArrayDouble_Create(void);

/// Creates a double array with the specified size.
USD_INTEROP_API VtArrayDoubleRef VtArrayDouble_CreateWithSize(size_t size);

/// Creates a double array from a C array.
USD_INTEROP_API VtArrayDoubleRef VtArrayDouble_CreateFromData(const double* data, size_t count);

/// Creates a copy of an existing double array.
USD_INTEROP_API VtArrayDoubleRef VtArrayDouble_Copy(VtArrayDoubleRef array);

/// Increments the reference count.
USD_INTEROP_API VtArrayDoubleRef VtArrayDouble_Retain(VtArrayDoubleRef array);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtArrayDouble_Release(VtArrayDoubleRef array);

// MARK: VtArrayDouble Properties

/// Returns the number of elements in the array.
USD_INTEROP_API size_t VtArrayDouble_GetSize(VtArrayDoubleRef array);

/// Returns true if the array is empty.
USD_INTEROP_API bool VtArrayDouble_IsEmpty(VtArrayDoubleRef array);

/// Returns the current capacity of the array.
USD_INTEROP_API size_t VtArrayDouble_GetCapacity(VtArrayDoubleRef array);

/// Returns a hash value for the array.
USD_INTEROP_API size_t VtArrayDouble_Hash(VtArrayDoubleRef array);

// MARK: VtArrayDouble Element Access

/// Gets an element at the specified index. Returns 0 if out of bounds.
USD_INTEROP_API double VtArrayDouble_GetElement(VtArrayDoubleRef array, size_t index);

/// Sets an element at the specified index.
USD_INTEROP_API void VtArrayDouble_SetElement(VtArrayDoubleRef array, size_t index, double value);

/// Gets multiple elements. Returns number of elements copied.
USD_INTEROP_API size_t VtArrayDouble_GetElements(VtArrayDoubleRef array, double* buffer, size_t count);

/// Copies all elements to a newly allocated buffer.
/// Caller must free the returned buffer with free().
USD_INTEROP_API double* VtArrayDouble_CopyElements(VtArrayDoubleRef array, size_t* outCount);

// MARK: VtArrayDouble Modification

/// Reserves capacity for at least the specified number of elements.
USD_INTEROP_API void VtArrayDouble_Reserve(VtArrayDoubleRef array, size_t capacity);

/// Resizes the array to the specified size.
USD_INTEROP_API void VtArrayDouble_Resize(VtArrayDoubleRef array, size_t size);

/// Appends an element to the end of the array.
USD_INTEROP_API void VtArrayDouble_PushBack(VtArrayDoubleRef array, double value);

/// Removes the last element from the array.
USD_INTEROP_API void VtArrayDouble_PopBack(VtArrayDoubleRef array);

/// Clears all elements from the array.
USD_INTEROP_API void VtArrayDouble_Clear(VtArrayDoubleRef array);

/// Assigns new data to the array.
USD_INTEROP_API void VtArrayDouble_Assign(VtArrayDoubleRef array, const double* data, size_t count);

// MARK: VtArrayDouble Comparison

/// Returns true if two arrays are equal.
USD_INTEROP_API bool VtArrayDouble_Equal(VtArrayDoubleRef a1, VtArrayDoubleRef a2);

// ============================================================================
// MARK: - VtArray<GfVec3f> (Vec3f array)
// ============================================================================

// MARK: VtArrayVec3f Lifecycle

/// Creates an empty Vec3f array.
USD_INTEROP_API VtArrayVec3fRef VtArrayVec3f_Create(void);

/// Creates a Vec3f array with the specified size.
USD_INTEROP_API VtArrayVec3fRef VtArrayVec3f_CreateWithSize(size_t size);

/// Creates a Vec3f array from flat float data (x,y,z,x,y,z,...).
/// count is the number of Vec3f elements (data must have count*3 floats).
USD_INTEROP_API VtArrayVec3fRef VtArrayVec3f_CreateFromData(const float* data, size_t count);

/// Creates a copy of an existing Vec3f array.
USD_INTEROP_API VtArrayVec3fRef VtArrayVec3f_Copy(VtArrayVec3fRef array);

/// Increments the reference count.
USD_INTEROP_API VtArrayVec3fRef VtArrayVec3f_Retain(VtArrayVec3fRef array);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtArrayVec3f_Release(VtArrayVec3fRef array);

// MARK: VtArrayVec3f Properties

/// Returns the number of elements in the array.
USD_INTEROP_API size_t VtArrayVec3f_GetSize(VtArrayVec3fRef array);

/// Returns true if the array is empty.
USD_INTEROP_API bool VtArrayVec3f_IsEmpty(VtArrayVec3fRef array);

/// Returns a hash value for the array.
USD_INTEROP_API size_t VtArrayVec3f_Hash(VtArrayVec3fRef array);

// MARK: VtArrayVec3f Element Access

/// Gets an element at the specified index.
USD_INTEROP_API void VtArrayVec3f_GetElement(VtArrayVec3fRef array, size_t index,
                                              float* outX, float* outY, float* outZ);

/// Sets an element at the specified index.
USD_INTEROP_API void VtArrayVec3f_SetElement(VtArrayVec3fRef array, size_t index,
                                              float x, float y, float z);

/// Gets all elements as flat float data (x,y,z,x,y,z,...).
/// buffer must have space for size * 3 floats.
/// Returns number of Vec3f elements copied.
USD_INTEROP_API size_t VtArrayVec3f_GetElements(VtArrayVec3fRef array, float* buffer, size_t maxCount);

/// Copies all elements to a newly allocated buffer.
/// Caller must free the returned buffer with free().
/// Returns NULL on failure. outCount receives the number of Vec3f elements.
USD_INTEROP_API float* VtArrayVec3f_CopyElements(VtArrayVec3fRef array, size_t* outCount);

// MARK: VtArrayVec3f Modification

/// Reserves capacity for at least the specified number of elements.
USD_INTEROP_API void VtArrayVec3f_Reserve(VtArrayVec3fRef array, size_t capacity);

/// Resizes the array to the specified size.
USD_INTEROP_API void VtArrayVec3f_Resize(VtArrayVec3fRef array, size_t size);

/// Appends an element to the end of the array.
USD_INTEROP_API void VtArrayVec3f_PushBack(VtArrayVec3fRef array, float x, float y, float z);

/// Clears all elements from the array.
USD_INTEROP_API void VtArrayVec3f_Clear(VtArrayVec3fRef array);

/// Assigns new data to the array from flat float data.
USD_INTEROP_API void VtArrayVec3f_Assign(VtArrayVec3fRef array, const float* data, size_t count);

// MARK: VtArrayVec3f Comparison

/// Returns true if two arrays are equal.
USD_INTEROP_API bool VtArrayVec3f_Equal(VtArrayVec3fRef a1, VtArrayVec3fRef a2);

// ============================================================================
// MARK: - VtArray<GfVec3d> (Vec3d array)
// ============================================================================

// MARK: VtArrayVec3d Lifecycle

/// Creates an empty Vec3d array.
USD_INTEROP_API VtArrayVec3dRef VtArrayVec3d_Create(void);

/// Creates a Vec3d array with the specified size.
USD_INTEROP_API VtArrayVec3dRef VtArrayVec3d_CreateWithSize(size_t size);

/// Creates a Vec3d array from flat double data (x,y,z,x,y,z,...).
USD_INTEROP_API VtArrayVec3dRef VtArrayVec3d_CreateFromData(const double* data, size_t count);

/// Creates a copy of an existing Vec3d array.
USD_INTEROP_API VtArrayVec3dRef VtArrayVec3d_Copy(VtArrayVec3dRef array);

/// Increments the reference count.
USD_INTEROP_API VtArrayVec3dRef VtArrayVec3d_Retain(VtArrayVec3dRef array);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtArrayVec3d_Release(VtArrayVec3dRef array);

// MARK: VtArrayVec3d Properties

/// Returns the number of elements in the array.
USD_INTEROP_API size_t VtArrayVec3d_GetSize(VtArrayVec3dRef array);

/// Returns true if the array is empty.
USD_INTEROP_API bool VtArrayVec3d_IsEmpty(VtArrayVec3dRef array);

/// Returns a hash value for the array.
USD_INTEROP_API size_t VtArrayVec3d_Hash(VtArrayVec3dRef array);

// MARK: VtArrayVec3d Element Access

/// Gets an element at the specified index.
USD_INTEROP_API void VtArrayVec3d_GetElement(VtArrayVec3dRef array, size_t index,
                                              double* outX, double* outY, double* outZ);

/// Sets an element at the specified index.
USD_INTEROP_API void VtArrayVec3d_SetElement(VtArrayVec3dRef array, size_t index,
                                              double x, double y, double z);

/// Gets all elements as flat double data.
USD_INTEROP_API size_t VtArrayVec3d_GetElements(VtArrayVec3dRef array, double* buffer, size_t maxCount);

/// Copies all elements to a newly allocated buffer.
USD_INTEROP_API double* VtArrayVec3d_CopyElements(VtArrayVec3dRef array, size_t* outCount);

// MARK: VtArrayVec3d Modification

/// Reserves capacity for at least the specified number of elements.
USD_INTEROP_API void VtArrayVec3d_Reserve(VtArrayVec3dRef array, size_t capacity);

/// Resizes the array to the specified size.
USD_INTEROP_API void VtArrayVec3d_Resize(VtArrayVec3dRef array, size_t size);

/// Appends an element to the end of the array.
USD_INTEROP_API void VtArrayVec3d_PushBack(VtArrayVec3dRef array, double x, double y, double z);

/// Clears all elements from the array.
USD_INTEROP_API void VtArrayVec3d_Clear(VtArrayVec3dRef array);

/// Assigns new data to the array from flat double data.
USD_INTEROP_API void VtArrayVec3d_Assign(VtArrayVec3dRef array, const double* data, size_t count);

// MARK: VtArrayVec3d Comparison

/// Returns true if two arrays are equal.
USD_INTEROP_API bool VtArrayVec3d_Equal(VtArrayVec3dRef a1, VtArrayVec3dRef a2);

// ============================================================================
// MARK: - VtDictionary (Key-value container)
// ============================================================================

// MARK: VtDictionary Lifecycle

/// Creates an empty dictionary.
USD_INTEROP_API VtDictionaryRef VtDictionary_Create(void);

/// Creates a copy of an existing dictionary.
USD_INTEROP_API VtDictionaryRef VtDictionary_Copy(VtDictionaryRef dict);

/// Increments the reference count.
USD_INTEROP_API VtDictionaryRef VtDictionary_Retain(VtDictionaryRef dict);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void VtDictionary_Release(VtDictionaryRef dict);

// MARK: VtDictionary Properties

/// Returns the number of key-value pairs in the dictionary.
USD_INTEROP_API size_t VtDictionary_GetSize(VtDictionaryRef dict);

/// Returns true if the dictionary is empty.
USD_INTEROP_API bool VtDictionary_IsEmpty(VtDictionaryRef dict);

/// Returns a hash value for the dictionary.
USD_INTEROP_API size_t VtDictionary_Hash(VtDictionaryRef dict);

// MARK: VtDictionary Access

/// Returns true if the dictionary contains the specified key.
USD_INTEROP_API bool VtDictionary_Contains(VtDictionaryRef dict, const char* key);

/// Gets the value for the specified key.
/// Returns NULL if key not found. Caller must release the returned VtValue.
USD_INTEROP_API VtValueRef VtDictionary_GetValue(VtDictionaryRef dict, const char* key);

/// Gets the value at a path (e.g., "key1:key2:key3").
/// Returns NULL if path not found. Caller must release the returned VtValue.
USD_INTEROP_API VtValueRef VtDictionary_GetValueAtPath(VtDictionaryRef dict, const char* path);

// MARK: VtDictionary Modification

/// Sets the value for the specified key.
USD_INTEROP_API void VtDictionary_SetValue(VtDictionaryRef dict, const char* key, VtValueRef value);

/// Sets the value at a path (creates intermediate dictionaries as needed).
USD_INTEROP_API void VtDictionary_SetValueAtPath(VtDictionaryRef dict, const char* path, VtValueRef value);

/// Erases the value for the specified key.
/// Returns true if the key was found and erased.
USD_INTEROP_API bool VtDictionary_Erase(VtDictionaryRef dict, const char* key);

/// Erases the value at a path.
USD_INTEROP_API bool VtDictionary_EraseAtPath(VtDictionaryRef dict, const char* path);

/// Clears all entries from the dictionary.
USD_INTEROP_API void VtDictionary_Clear(VtDictionaryRef dict);

// MARK: VtDictionary Iteration

/// Gets all keys in the dictionary.
/// Caller must free each string and the array with VtDictionary_FreeKeys.
USD_INTEROP_API char** VtDictionary_GetKeys(VtDictionaryRef dict, size_t* outCount);

/// Frees the keys array returned by VtDictionary_GetKeys.
USD_INTEROP_API void VtDictionary_FreeKeys(char** keys, size_t count);

// MARK: VtDictionary Comparison

/// Returns true if two dictionaries are equal.
USD_INTEROP_API bool VtDictionary_Equal(VtDictionaryRef d1, VtDictionaryRef d2);

// MARK: VtDictionary Composition

/// Returns a dictionary that is the composition of strong over weak.
/// Caller must release the returned dictionary.
USD_INTEROP_API VtDictionaryRef VtDictionary_Over(VtDictionaryRef strong, VtDictionaryRef weak);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_VT_H
