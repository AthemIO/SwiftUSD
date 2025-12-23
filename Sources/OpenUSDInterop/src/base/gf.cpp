// gf.cpp - Gf math types C wrapper implementation
// Mirrors: pxr/base/gf/

#include "../../include/base/gf.h"
#include <cmath>

extern "C" {

// MARK: - GfVec3f

UsdGfVec3f UsdGfVec3f_Zero(void) {
    return {0.0f, 0.0f, 0.0f};
}

UsdGfVec3f UsdGfVec3f_Create(float x, float y, float z) {
    return {x, y, z};
}

float UsdGfVec3f_Length(UsdGfVec3f v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

UsdGfVec3f UsdGfVec3f_Normalize(UsdGfVec3f v) {
    float len = UsdGfVec3f_Length(v);
    if (len > 0.0f) {
        return {v.x / len, v.y / len, v.z / len};
    }
    return v;
}

float UsdGfVec3f_Dot(UsdGfVec3f a, UsdGfVec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

UsdGfVec3f UsdGfVec3f_Cross(UsdGfVec3f a, UsdGfVec3f b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// MARK: - GfVec3d

UsdGfVec3d UsdGfVec3d_Zero(void) {
    return {0.0, 0.0, 0.0};
}

UsdGfVec3d UsdGfVec3d_Create(double x, double y, double z) {
    return {x, y, z};
}

double UsdGfVec3d_Length(UsdGfVec3d v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

UsdGfVec3d UsdGfVec3d_Normalize(UsdGfVec3d v) {
    double len = UsdGfVec3d_Length(v);
    if (len > 0.0) {
        return {v.x / len, v.y / len, v.z / len};
    }
    return v;
}

// MARK: - GfMatrix4d

UsdGfMatrix4d UsdGfMatrix4d_Identity(void) {
    UsdGfMatrix4d m = {};
    m.data[0] = 1.0;
    m.data[5] = 1.0;
    m.data[10] = 1.0;
    m.data[15] = 1.0;
    return m;
}

UsdGfMatrix4d UsdGfMatrix4d_SetTranslate(UsdGfVec3d translation) {
    UsdGfMatrix4d m = UsdGfMatrix4d_Identity();
    m.data[12] = translation.x;
    m.data[13] = translation.y;
    m.data[14] = translation.z;
    return m;
}

UsdGfMatrix4d UsdGfMatrix4d_SetScale(UsdGfVec3d scale) {
    UsdGfMatrix4d m = {};
    m.data[0] = scale.x;
    m.data[5] = scale.y;
    m.data[10] = scale.z;
    m.data[15] = 1.0;
    return m;
}

UsdGfMatrix4d UsdGfMatrix4d_Multiply(UsdGfMatrix4d a, UsdGfMatrix4d b) {
    UsdGfMatrix4d result = {};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            double sum = 0.0;
            for (int k = 0; k < 4; ++k) {
                sum += a.data[row * 4 + k] * b.data[k * 4 + col];
            }
            result.data[row * 4 + col] = sum;
        }
    }
    return result;
}

UsdGfMatrix4d UsdGfMatrix4d_GetInverse(UsdGfMatrix4d m) {
    // TODO: Implement proper matrix inverse
    return UsdGfMatrix4d_Identity();
}

UsdGfMatrix4d UsdGfMatrix4d_GetTranspose(UsdGfMatrix4d m) {
    UsdGfMatrix4d result = {};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.data[col * 4 + row] = m.data[row * 4 + col];
        }
    }
    return result;
}

UsdGfVec3d UsdGfMatrix4d_ExtractTranslation(UsdGfMatrix4d m) {
    return {m.data[12], m.data[13], m.data[14]};
}

// MARK: - GfQuatd

UsdGfQuatd UsdGfQuatd_Identity(void) {
    return {1.0, {0.0, 0.0, 0.0}};
}

UsdGfQuatd UsdGfQuatd_FromAxisAngle(UsdGfVec3d axis, double angle) {
    double halfAngle = angle * 0.5;
    double s = std::sin(halfAngle);
    UsdGfVec3d normAxis = UsdGfVec3d_Normalize(axis);
    return {
        std::cos(halfAngle),
        {normAxis.x * s, normAxis.y * s, normAxis.z * s}
    };
}

UsdGfQuatd UsdGfQuatd_Normalize(UsdGfQuatd q) {
    double len = std::sqrt(
        q.real * q.real +
        q.imaginary[0] * q.imaginary[0] +
        q.imaginary[1] * q.imaginary[1] +
        q.imaginary[2] * q.imaginary[2]
    );
    if (len > 0.0) {
        return {
            q.real / len,
            {q.imaginary[0] / len, q.imaginary[1] / len, q.imaginary[2] / len}
        };
    }
    return q;
}

UsdGfMatrix4d UsdGfQuatd_ToMatrix(UsdGfQuatd q) {
    // TODO: Implement quaternion to matrix conversion
    return UsdGfMatrix4d_Identity();
}

} // extern "C"
