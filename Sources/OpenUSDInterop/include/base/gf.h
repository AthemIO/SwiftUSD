// gf.h - Gf (Graphics Foundations) C wrapper
// Mirrors: pxr/base/gf/

#ifndef OPENUSDINTEROP_BASE_GF_H
#define OPENUSDINTEROP_BASE_GF_H

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
// MARK: - GfVec3f (3D float vector)
// ============================================================================

/// Opaque handle to a GfVec3f wrapper.
typedef struct GfVec3fOpaque* GfVec3fRef;

// MARK: Vec3f Lifecycle

/// Creates a GfVec3f with the specified components.
USD_INTEROP_API GfVec3fRef GfVec3f_Create(float x, float y, float z);

/// Creates a GfVec3f with all components set to the same value.
USD_INTEROP_API GfVec3fRef GfVec3f_CreateScalar(float value);

/// Creates a zero vector.
USD_INTEROP_API GfVec3fRef GfVec3f_CreateZero(void);

/// Creates a copy of an existing GfVec3f.
USD_INTEROP_API GfVec3fRef GfVec3f_Copy(GfVec3fRef vec);

/// Increments the reference count.
USD_INTEROP_API GfVec3fRef GfVec3f_Retain(GfVec3fRef vec);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GfVec3f_Release(GfVec3fRef vec);

// MARK: Vec3f Properties

/// Gets the X component.
USD_INTEROP_API float GfVec3f_GetX(GfVec3fRef vec);

/// Gets the Y component.
USD_INTEROP_API float GfVec3f_GetY(GfVec3fRef vec);

/// Gets the Z component.
USD_INTEROP_API float GfVec3f_GetZ(GfVec3fRef vec);

/// Gets all components as an array [x, y, z].
USD_INTEROP_API void GfVec3f_GetComponents(GfVec3fRef vec, float* outX, float* outY, float* outZ);

/// Sets the components.
USD_INTEROP_API void GfVec3f_Set(GfVec3fRef vec, float x, float y, float z);

/// Gets the length (magnitude) of the vector.
USD_INTEROP_API float GfVec3f_GetLength(GfVec3fRef vec);

/// Gets the squared length of the vector (faster than GetLength).
USD_INTEROP_API float GfVec3f_GetLengthSq(GfVec3fRef vec);

/// Returns a hash value for the vector.
USD_INTEROP_API size_t GfVec3f_Hash(GfVec3fRef vec);

// MARK: Vec3f Operations

/// Returns a normalized copy of the vector.
USD_INTEROP_API GfVec3fRef GfVec3f_GetNormalized(GfVec3fRef vec);

/// Normalizes the vector in place, returning the original length.
USD_INTEROP_API float GfVec3f_Normalize(GfVec3fRef vec);

/// Returns the dot product of two vectors.
USD_INTEROP_API float GfVec3f_Dot(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the cross product of two vectors.
USD_INTEROP_API GfVec3fRef GfVec3f_Cross(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the sum of two vectors.
USD_INTEROP_API GfVec3fRef GfVec3f_Add(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the difference of two vectors (v1 - v2).
USD_INTEROP_API GfVec3fRef GfVec3f_Subtract(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the vector multiplied by a scalar.
USD_INTEROP_API GfVec3fRef GfVec3f_Multiply(GfVec3fRef vec, float scalar);

/// Returns the vector divided by a scalar.
USD_INTEROP_API GfVec3fRef GfVec3f_Divide(GfVec3fRef vec, float scalar);

/// Returns the negation of the vector.
USD_INTEROP_API GfVec3fRef GfVec3f_Negate(GfVec3fRef vec);

/// Returns the component-wise multiplication of two vectors.
USD_INTEROP_API GfVec3fRef GfVec3f_CompMult(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the component-wise division of two vectors.
USD_INTEROP_API GfVec3fRef GfVec3f_CompDiv(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the projection of v1 onto v2.
USD_INTEROP_API GfVec3fRef GfVec3f_GetProjection(GfVec3fRef v1, GfVec3fRef v2);

/// Returns the orthogonal complement of v1.GetProjection(v2).
USD_INTEROP_API GfVec3fRef GfVec3f_GetComplement(GfVec3fRef v1, GfVec3fRef v2);

// MARK: Vec3f Comparison

/// Returns true if two vectors are equal.
USD_INTEROP_API bool GfVec3f_Equal(GfVec3fRef v1, GfVec3fRef v2);

/// Returns true if two vectors are close within a tolerance.
USD_INTEROP_API bool GfVec3f_IsClose(GfVec3fRef v1, GfVec3fRef v2, float tolerance);

// MARK: Vec3f Static Factories

/// Returns the X axis unit vector (1, 0, 0).
USD_INTEROP_API GfVec3fRef GfVec3f_XAxis(void);

/// Returns the Y axis unit vector (0, 1, 0).
USD_INTEROP_API GfVec3fRef GfVec3f_YAxis(void);

/// Returns the Z axis unit vector (0, 0, 1).
USD_INTEROP_API GfVec3fRef GfVec3f_ZAxis(void);

// ============================================================================
// MARK: - GfVec3d (3D double vector)
// ============================================================================

/// Opaque handle to a GfVec3d wrapper.
typedef struct GfVec3dOpaque* GfVec3dRef;

// MARK: Vec3d Lifecycle

/// Creates a GfVec3d with the specified components.
USD_INTEROP_API GfVec3dRef GfVec3d_Create(double x, double y, double z);

/// Creates a GfVec3d with all components set to the same value.
USD_INTEROP_API GfVec3dRef GfVec3d_CreateScalar(double value);

/// Creates a zero vector.
USD_INTEROP_API GfVec3dRef GfVec3d_CreateZero(void);

/// Creates a copy of an existing GfVec3d.
USD_INTEROP_API GfVec3dRef GfVec3d_Copy(GfVec3dRef vec);

/// Increments the reference count.
USD_INTEROP_API GfVec3dRef GfVec3d_Retain(GfVec3dRef vec);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GfVec3d_Release(GfVec3dRef vec);

// MARK: Vec3d Properties

/// Gets the X component.
USD_INTEROP_API double GfVec3d_GetX(GfVec3dRef vec);

/// Gets the Y component.
USD_INTEROP_API double GfVec3d_GetY(GfVec3dRef vec);

/// Gets the Z component.
USD_INTEROP_API double GfVec3d_GetZ(GfVec3dRef vec);

/// Gets all components.
USD_INTEROP_API void GfVec3d_GetComponents(GfVec3dRef vec, double* outX, double* outY, double* outZ);

/// Sets the components.
USD_INTEROP_API void GfVec3d_Set(GfVec3dRef vec, double x, double y, double z);

/// Gets the length (magnitude) of the vector.
USD_INTEROP_API double GfVec3d_GetLength(GfVec3dRef vec);

/// Gets the squared length of the vector.
USD_INTEROP_API double GfVec3d_GetLengthSq(GfVec3dRef vec);

/// Returns a hash value for the vector.
USD_INTEROP_API size_t GfVec3d_Hash(GfVec3dRef vec);

// MARK: Vec3d Operations

/// Returns a normalized copy of the vector.
USD_INTEROP_API GfVec3dRef GfVec3d_GetNormalized(GfVec3dRef vec);

/// Normalizes the vector in place, returning the original length.
USD_INTEROP_API double GfVec3d_Normalize(GfVec3dRef vec);

/// Returns the dot product of two vectors.
USD_INTEROP_API double GfVec3d_Dot(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the cross product of two vectors.
USD_INTEROP_API GfVec3dRef GfVec3d_Cross(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the sum of two vectors.
USD_INTEROP_API GfVec3dRef GfVec3d_Add(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the difference of two vectors (v1 - v2).
USD_INTEROP_API GfVec3dRef GfVec3d_Subtract(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the vector multiplied by a scalar.
USD_INTEROP_API GfVec3dRef GfVec3d_Multiply(GfVec3dRef vec, double scalar);

/// Returns the vector divided by a scalar.
USD_INTEROP_API GfVec3dRef GfVec3d_Divide(GfVec3dRef vec, double scalar);

/// Returns the negation of the vector.
USD_INTEROP_API GfVec3dRef GfVec3d_Negate(GfVec3dRef vec);

/// Returns the component-wise multiplication of two vectors.
USD_INTEROP_API GfVec3dRef GfVec3d_CompMult(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the component-wise division of two vectors.
USD_INTEROP_API GfVec3dRef GfVec3d_CompDiv(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the projection of v1 onto v2.
USD_INTEROP_API GfVec3dRef GfVec3d_GetProjection(GfVec3dRef v1, GfVec3dRef v2);

/// Returns the orthogonal complement of v1.GetProjection(v2).
USD_INTEROP_API GfVec3dRef GfVec3d_GetComplement(GfVec3dRef v1, GfVec3dRef v2);

// MARK: Vec3d Comparison

/// Returns true if two vectors are equal.
USD_INTEROP_API bool GfVec3d_Equal(GfVec3dRef v1, GfVec3dRef v2);

/// Returns true if two vectors are close within a tolerance.
USD_INTEROP_API bool GfVec3d_IsClose(GfVec3dRef v1, GfVec3dRef v2, double tolerance);

// MARK: Vec3d Static Factories

/// Returns the X axis unit vector (1, 0, 0).
USD_INTEROP_API GfVec3dRef GfVec3d_XAxis(void);

/// Returns the Y axis unit vector (0, 1, 0).
USD_INTEROP_API GfVec3dRef GfVec3d_YAxis(void);

/// Returns the Z axis unit vector (0, 0, 1).
USD_INTEROP_API GfVec3dRef GfVec3d_ZAxis(void);

// ============================================================================
// MARK: - GfMatrix4d (4x4 double matrix)
// ============================================================================

/// Opaque handle to a GfMatrix4d wrapper.
typedef struct GfMatrix4dOpaque* GfMatrix4dRef;

// MARK: Matrix4d Lifecycle

/// Creates a GfMatrix4d from 16 values in row-major order.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_Create(
    double m00, double m01, double m02, double m03,
    double m10, double m11, double m12, double m13,
    double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33);

/// Creates an identity matrix.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_CreateIdentity(void);

/// Creates a zero matrix.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_CreateZero(void);

/// Creates a diagonal matrix with the given scale factor.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_CreateDiagonal(double scale);

/// Creates a copy of an existing GfMatrix4d.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_Copy(GfMatrix4dRef mat);

/// Increments the reference count.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_Retain(GfMatrix4dRef mat);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GfMatrix4d_Release(GfMatrix4dRef mat);

// MARK: Matrix4d Properties

/// Gets a single element at (row, col).
USD_INTEROP_API double GfMatrix4d_GetElement(GfMatrix4dRef mat, int row, int col);

/// Sets a single element at (row, col).
USD_INTEROP_API void GfMatrix4d_SetElement(GfMatrix4dRef mat, int row, int col, double value);

/// Gets all 16 elements in row-major order.
USD_INTEROP_API void GfMatrix4d_GetElements(GfMatrix4dRef mat, double* outElements);

/// Sets all 16 elements from an array in row-major order.
USD_INTEROP_API void GfMatrix4d_SetElements(GfMatrix4dRef mat, const double* elements);

/// Gets a row as a Vec4 (x, y, z, w).
USD_INTEROP_API void GfMatrix4d_GetRow(GfMatrix4dRef mat, int row, double* outX, double* outY, double* outZ, double* outW);

/// Gets a column as a Vec4 (x, y, z, w).
USD_INTEROP_API void GfMatrix4d_GetColumn(GfMatrix4dRef mat, int col, double* outX, double* outY, double* outZ, double* outW);

/// Sets the matrix to identity.
USD_INTEROP_API void GfMatrix4d_SetIdentity(GfMatrix4dRef mat);

/// Sets the matrix to zero.
USD_INTEROP_API void GfMatrix4d_SetZero(GfMatrix4dRef mat);

/// Returns a hash value for the matrix.
USD_INTEROP_API size_t GfMatrix4d_Hash(GfMatrix4dRef mat);

// MARK: Matrix4d Operations

/// Returns the transpose of the matrix.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_GetTranspose(GfMatrix4dRef mat);

/// Returns the inverse of the matrix. Returns identity if singular.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_GetInverse(GfMatrix4dRef mat);

/// Returns the determinant of the matrix.
USD_INTEROP_API double GfMatrix4d_GetDeterminant(GfMatrix4dRef mat);

/// Returns the product of two matrices.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_Multiply(GfMatrix4dRef m1, GfMatrix4dRef m2);

/// Returns the matrix multiplied by a scalar.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_MultiplyScalar(GfMatrix4dRef mat, double scalar);

/// Returns the sum of two matrices.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_Add(GfMatrix4dRef m1, GfMatrix4dRef m2);

/// Returns the difference of two matrices.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_Subtract(GfMatrix4dRef m1, GfMatrix4dRef m2);

// MARK: Matrix4d Transform Operations

/// Creates a translation matrix.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_SetTranslate(double x, double y, double z);

/// Creates a uniform scale matrix.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_SetScale(double scale);

/// Creates a non-uniform scale matrix.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_SetScaleVec(double sx, double sy, double sz);

/// Creates a rotation matrix from a quaternion.
USD_INTEROP_API GfMatrix4dRef GfMatrix4d_SetRotateQuat(GfQuatdRef quat);

/// Extracts the translation component from the matrix.
USD_INTEROP_API void GfMatrix4d_ExtractTranslation(GfMatrix4dRef mat, double* outX, double* outY, double* outZ);

/// Transforms a point (with translation).
USD_INTEROP_API void GfMatrix4d_Transform(GfMatrix4dRef mat, double x, double y, double z,
                                          double* outX, double* outY, double* outZ);

/// Transforms a direction (without translation).
USD_INTEROP_API void GfMatrix4d_TransformDir(GfMatrix4dRef mat, double x, double y, double z,
                                             double* outX, double* outY, double* outZ);

/// Transforms a point using affine transformation (assumes w=1).
USD_INTEROP_API void GfMatrix4d_TransformAffine(GfMatrix4dRef mat, double x, double y, double z,
                                                double* outX, double* outY, double* outZ);

// MARK: Matrix4d Comparison

/// Returns true if two matrices are equal.
USD_INTEROP_API bool GfMatrix4d_Equal(GfMatrix4dRef m1, GfMatrix4dRef m2);

/// Returns true if two matrices are close within a tolerance.
USD_INTEROP_API bool GfMatrix4d_IsClose(GfMatrix4dRef m1, GfMatrix4dRef m2, double tolerance);

// ============================================================================
// MARK: - GfQuatd (double quaternion)
// ============================================================================

/// Opaque handle to a GfQuatd wrapper.
typedef struct GfQuatdOpaque* GfQuatdRef;

// MARK: Quatd Lifecycle

/// Creates a GfQuatd with real and imaginary components.
USD_INTEROP_API GfQuatdRef GfQuatd_Create(double real, double i, double j, double k);

/// Creates a GfQuatd with only the real component set.
USD_INTEROP_API GfQuatdRef GfQuatd_CreateReal(double real);

/// Creates the identity quaternion (1, 0, 0, 0).
USD_INTEROP_API GfQuatdRef GfQuatd_CreateIdentity(void);

/// Creates the zero quaternion.
USD_INTEROP_API GfQuatdRef GfQuatd_CreateZero(void);

/// Creates a copy of an existing GfQuatd.
USD_INTEROP_API GfQuatdRef GfQuatd_Copy(GfQuatdRef quat);

/// Increments the reference count.
USD_INTEROP_API GfQuatdRef GfQuatd_Retain(GfQuatdRef quat);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GfQuatd_Release(GfQuatdRef quat);

// MARK: Quatd Properties

/// Gets the real (scalar) component.
USD_INTEROP_API double GfQuatd_GetReal(GfQuatdRef quat);

/// Gets the imaginary (vector) components.
USD_INTEROP_API void GfQuatd_GetImaginary(GfQuatdRef quat, double* outI, double* outJ, double* outK);

/// Sets the real component.
USD_INTEROP_API void GfQuatd_SetReal(GfQuatdRef quat, double real);

/// Sets the imaginary components.
USD_INTEROP_API void GfQuatd_SetImaginary(GfQuatdRef quat, double i, double j, double k);

/// Gets the length of the quaternion.
USD_INTEROP_API double GfQuatd_GetLength(GfQuatdRef quat);

/// Returns a hash value for the quaternion.
USD_INTEROP_API size_t GfQuatd_Hash(GfQuatdRef quat);

// MARK: Quatd Operations

/// Returns a normalized copy of the quaternion.
USD_INTEROP_API GfQuatdRef GfQuatd_GetNormalized(GfQuatdRef quat);

/// Normalizes the quaternion in place, returning the original length.
USD_INTEROP_API double GfQuatd_Normalize(GfQuatdRef quat);

/// Returns the conjugate of the quaternion.
USD_INTEROP_API GfQuatdRef GfQuatd_GetConjugate(GfQuatdRef quat);

/// Returns the inverse of the quaternion.
USD_INTEROP_API GfQuatdRef GfQuatd_GetInverse(GfQuatdRef quat);

/// Returns the dot product of two quaternions.
USD_INTEROP_API double GfQuatd_Dot(GfQuatdRef q1, GfQuatdRef q2);

/// Returns the product of two quaternions.
USD_INTEROP_API GfQuatdRef GfQuatd_Multiply(GfQuatdRef q1, GfQuatdRef q2);

/// Returns the quaternion multiplied by a scalar.
USD_INTEROP_API GfQuatdRef GfQuatd_MultiplyScalar(GfQuatdRef quat, double scalar);

/// Returns the sum of two quaternions.
USD_INTEROP_API GfQuatdRef GfQuatd_Add(GfQuatdRef q1, GfQuatdRef q2);

/// Returns the difference of two quaternions.
USD_INTEROP_API GfQuatdRef GfQuatd_Subtract(GfQuatdRef q1, GfQuatdRef q2);

/// Returns the negation of the quaternion.
USD_INTEROP_API GfQuatdRef GfQuatd_Negate(GfQuatdRef quat);

/// Transforms a point by the quaternion rotation.
USD_INTEROP_API void GfQuatd_Transform(GfQuatdRef quat, double x, double y, double z,
                                       double* outX, double* outY, double* outZ);

/// Performs spherical linear interpolation between two quaternions.
USD_INTEROP_API GfQuatdRef GfQuatd_Slerp(GfQuatdRef q0, GfQuatdRef q1, double alpha);

// MARK: Quatd Comparison

/// Returns true if two quaternions are equal.
USD_INTEROP_API bool GfQuatd_Equal(GfQuatdRef q1, GfQuatdRef q2);

// ============================================================================
// MARK: - GfRange3d (3D double range/interval)
// ============================================================================

/// Opaque handle to a GfRange3d wrapper.
typedef struct GfRange3dOpaque* GfRange3dRef;

// MARK: Range3d Lifecycle

/// Creates a GfRange3d with min and max points.
USD_INTEROP_API GfRange3dRef GfRange3d_Create(
    double minX, double minY, double minZ,
    double maxX, double maxY, double maxZ);

/// Creates an empty range.
USD_INTEROP_API GfRange3dRef GfRange3d_CreateEmpty(void);

/// Creates the unit cube [0,1] in each dimension.
USD_INTEROP_API GfRange3dRef GfRange3d_CreateUnitCube(void);

/// Creates a copy of an existing GfRange3d.
USD_INTEROP_API GfRange3dRef GfRange3d_Copy(GfRange3dRef range);

/// Increments the reference count.
USD_INTEROP_API GfRange3dRef GfRange3d_Retain(GfRange3dRef range);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GfRange3d_Release(GfRange3dRef range);

// MARK: Range3d Properties

/// Gets the minimum point.
USD_INTEROP_API void GfRange3d_GetMin(GfRange3dRef range, double* outX, double* outY, double* outZ);

/// Gets the maximum point.
USD_INTEROP_API void GfRange3d_GetMax(GfRange3dRef range, double* outX, double* outY, double* outZ);

/// Sets the minimum point.
USD_INTEROP_API void GfRange3d_SetMin(GfRange3dRef range, double x, double y, double z);

/// Sets the maximum point.
USD_INTEROP_API void GfRange3d_SetMax(GfRange3dRef range, double x, double y, double z);

/// Gets the size of the range.
USD_INTEROP_API void GfRange3d_GetSize(GfRange3dRef range, double* outX, double* outY, double* outZ);

/// Gets the midpoint of the range.
USD_INTEROP_API void GfRange3d_GetMidpoint(GfRange3dRef range, double* outX, double* outY, double* outZ);

/// Returns true if the range is empty.
USD_INTEROP_API bool GfRange3d_IsEmpty(GfRange3dRef range);

/// Sets the range to empty.
USD_INTEROP_API void GfRange3d_SetEmpty(GfRange3dRef range);

/// Returns a hash value for the range.
USD_INTEROP_API size_t GfRange3d_Hash(GfRange3dRef range);

// MARK: Range3d Operations

/// Returns true if the range contains the given point.
USD_INTEROP_API bool GfRange3d_ContainsPoint(GfRange3dRef range, double x, double y, double z);

/// Returns true if the range contains another range.
USD_INTEROP_API bool GfRange3d_ContainsRange(GfRange3dRef range, GfRange3dRef other);

/// Returns true if the range is outside another range.
USD_INTEROP_API bool GfRange3d_IsOutside(GfRange3dRef range, GfRange3dRef other);

/// Extends the range to include the given point.
USD_INTEROP_API void GfRange3d_UnionWithPoint(GfRange3dRef range, double x, double y, double z);

/// Extends the range to include another range.
USD_INTEROP_API void GfRange3d_UnionWithRange(GfRange3dRef range, GfRange3dRef other);

/// Returns the union of two ranges.
USD_INTEROP_API GfRange3dRef GfRange3d_GetUnion(GfRange3dRef r1, GfRange3dRef r2);

/// Returns the intersection of two ranges.
USD_INTEROP_API GfRange3dRef GfRange3d_GetIntersection(GfRange3dRef r1, GfRange3dRef r2);

/// Returns the squared distance from a point to the range.
USD_INTEROP_API double GfRange3d_GetDistanceSquared(GfRange3dRef range, double x, double y, double z);

/// Returns the ith corner of the range (0-7).
USD_INTEROP_API void GfRange3d_GetCorner(GfRange3dRef range, size_t i, double* outX, double* outY, double* outZ);

// MARK: Range3d Comparison

/// Returns true if two ranges are equal.
USD_INTEROP_API bool GfRange3d_Equal(GfRange3dRef r1, GfRange3dRef r2);

// ============================================================================
// MARK: - GfBBox3d (3D bounding box with transform)
// ============================================================================

/// Opaque handle to a GfBBox3d wrapper.
typedef struct GfBBox3dOpaque* GfBBox3dRef;

// MARK: BBox3d Lifecycle

/// Creates an empty GfBBox3d with identity matrix.
USD_INTEROP_API GfBBox3dRef GfBBox3d_Create(void);

/// Creates a GfBBox3d from a range with identity matrix.
USD_INTEROP_API GfBBox3dRef GfBBox3d_CreateFromRange(GfRange3dRef range);

/// Creates a GfBBox3d from a range and matrix.
USD_INTEROP_API GfBBox3dRef GfBBox3d_CreateFromRangeAndMatrix(GfRange3dRef range, GfMatrix4dRef matrix);

/// Creates a copy of an existing GfBBox3d.
USD_INTEROP_API GfBBox3dRef GfBBox3d_Copy(GfBBox3dRef bbox);

/// Increments the reference count.
USD_INTEROP_API GfBBox3dRef GfBBox3d_Retain(GfBBox3dRef bbox);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GfBBox3d_Release(GfBBox3dRef bbox);

// MARK: BBox3d Properties

/// Gets the axis-aligned range.
USD_INTEROP_API GfRange3dRef GfBBox3d_GetRange(GfBBox3dRef bbox);

/// Gets the transformation matrix.
USD_INTEROP_API GfMatrix4dRef GfBBox3d_GetMatrix(GfBBox3dRef bbox);

/// Gets the inverse transformation matrix.
USD_INTEROP_API GfMatrix4dRef GfBBox3d_GetInverseMatrix(GfBBox3dRef bbox);

/// Sets the range.
USD_INTEROP_API void GfBBox3d_SetRange(GfBBox3dRef bbox, GfRange3dRef range);

/// Sets the transformation matrix.
USD_INTEROP_API void GfBBox3d_SetMatrix(GfBBox3dRef bbox, GfMatrix4dRef matrix);

/// Gets the zero-area primitives flag.
USD_INTEROP_API bool GfBBox3d_HasZeroAreaPrimitives(GfBBox3dRef bbox);

/// Sets the zero-area primitives flag.
USD_INTEROP_API void GfBBox3d_SetHasZeroAreaPrimitives(GfBBox3dRef bbox, bool hasZeroArea);

/// Returns a hash value for the bbox.
USD_INTEROP_API size_t GfBBox3d_Hash(GfBBox3dRef bbox);

// MARK: BBox3d Operations

/// Returns the volume of the bounding box.
USD_INTEROP_API double GfBBox3d_GetVolume(GfBBox3dRef bbox);

/// Transforms the bounding box by a matrix.
USD_INTEROP_API void GfBBox3d_Transform(GfBBox3dRef bbox, GfMatrix4dRef matrix);

/// Returns the aligned range (axis-aligned bounding box).
USD_INTEROP_API GfRange3dRef GfBBox3d_ComputeAlignedRange(GfBBox3dRef bbox);

/// Computes the centroid of the bounding box.
USD_INTEROP_API void GfBBox3d_ComputeCentroid(GfBBox3dRef bbox, double* outX, double* outY, double* outZ);

/// Combines two bounding boxes.
USD_INTEROP_API GfBBox3dRef GfBBox3d_Combine(GfBBox3dRef b1, GfBBox3dRef b2);

// MARK: BBox3d Comparison

/// Returns true if two bounding boxes are equal.
USD_INTEROP_API bool GfBBox3d_Equal(GfBBox3dRef b1, GfBBox3dRef b2);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_GF_H
