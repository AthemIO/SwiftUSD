// gf.h - Gf math types C wrapper
// Mirrors: pxr/base/gf/vec3f.h, matrix4d.h, quatd.h, etc.

#ifndef OPENUSDINTEROP_BASE_GF_H
#define OPENUSDINTEROP_BASE_GF_H

#include "../OpenUSDInterop.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - GfVec3f

/// A 3-component vector of floats.
typedef struct {
    float x;
    float y;
    float z;
} UsdGfVec3f;

/// Creates a zero vector.
USD_INTEROP_API UsdGfVec3f UsdGfVec3f_Zero(void);

/// Creates a vector from components.
USD_INTEROP_API UsdGfVec3f UsdGfVec3f_Create(float x, float y, float z);

/// Returns the length of a vector.
USD_INTEROP_API float UsdGfVec3f_Length(UsdGfVec3f v);

/// Returns the normalized vector.
USD_INTEROP_API UsdGfVec3f UsdGfVec3f_Normalize(UsdGfVec3f v);

/// Returns the dot product of two vectors.
USD_INTEROP_API float UsdGfVec3f_Dot(UsdGfVec3f a, UsdGfVec3f b);

/// Returns the cross product of two vectors.
USD_INTEROP_API UsdGfVec3f UsdGfVec3f_Cross(UsdGfVec3f a, UsdGfVec3f b);

// MARK: - GfVec3d

/// A 3-component vector of doubles.
typedef struct {
    double x;
    double y;
    double z;
} UsdGfVec3d;

/// Creates a zero vector.
USD_INTEROP_API UsdGfVec3d UsdGfVec3d_Zero(void);

/// Creates a vector from components.
USD_INTEROP_API UsdGfVec3d UsdGfVec3d_Create(double x, double y, double z);

/// Returns the length of a vector.
USD_INTEROP_API double UsdGfVec3d_Length(UsdGfVec3d v);

/// Returns the normalized vector.
USD_INTEROP_API UsdGfVec3d UsdGfVec3d_Normalize(UsdGfVec3d v);

// MARK: - GfMatrix4d

/// A 4x4 matrix of doubles (row-major).
typedef struct {
    double data[16];
} UsdGfMatrix4d;

/// Creates an identity matrix.
USD_INTEROP_API UsdGfMatrix4d UsdGfMatrix4d_Identity(void);

/// Creates a translation matrix.
USD_INTEROP_API UsdGfMatrix4d UsdGfMatrix4d_SetTranslate(UsdGfVec3d translation);

/// Creates a scale matrix.
USD_INTEROP_API UsdGfMatrix4d UsdGfMatrix4d_SetScale(UsdGfVec3d scale);

/// Multiplies two matrices.
USD_INTEROP_API UsdGfMatrix4d UsdGfMatrix4d_Multiply(UsdGfMatrix4d a, UsdGfMatrix4d b);

/// Returns the inverse of a matrix.
USD_INTEROP_API UsdGfMatrix4d UsdGfMatrix4d_GetInverse(UsdGfMatrix4d m);

/// Returns the transpose of a matrix.
USD_INTEROP_API UsdGfMatrix4d UsdGfMatrix4d_GetTranspose(UsdGfMatrix4d m);

/// Extracts the translation component.
USD_INTEROP_API UsdGfVec3d UsdGfMatrix4d_ExtractTranslation(UsdGfMatrix4d m);

// MARK: - GfQuatd

/// A quaternion of doubles.
typedef struct {
    double real;
    double imaginary[3];
} UsdGfQuatd;

/// Creates an identity quaternion.
USD_INTEROP_API UsdGfQuatd UsdGfQuatd_Identity(void);

/// Creates a quaternion from axis-angle.
USD_INTEROP_API UsdGfQuatd UsdGfQuatd_FromAxisAngle(UsdGfVec3d axis, double angle);

/// Returns the normalized quaternion.
USD_INTEROP_API UsdGfQuatd UsdGfQuatd_Normalize(UsdGfQuatd q);

/// Converts a quaternion to a rotation matrix.
USD_INTEROP_API UsdGfMatrix4d UsdGfQuatd_ToMatrix(UsdGfQuatd q);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_GF_H
