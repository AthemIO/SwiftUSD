// Gf.cpp - Graphics Foundation implementation
//
// This file implements the math types for the Gf module.
// Since these are pure math types, we implement them directly without
// wrapping pxr types - the math is the same either way.

#include "../../include/base/Gf.hpp"

#include <cmath>
#include <limits>
#include <algorithm>

namespace cxxfacade {
namespace gf {

// ============================================================================
// Vec2f implementations
// ============================================================================

float Vec2f::GetLength() const {
    return std::sqrt(x*x + y*y);
}

Vec2f Vec2f::GetNormalized() const {
    float len = GetLength();
    if (len < 1e-6f) return Vec2f(0, 0);
    return *this / len;
}

float Vec2f::Normalize() {
    float len = GetLength();
    if (len < 1e-6f) {
        x = y = 0;
        return 0;
    }
    x /= len;
    y /= len;
    return len;
}

// ============================================================================
// Vec2d implementations
// ============================================================================

double Vec2d::GetLength() const {
    return std::sqrt(x*x + y*y);
}

Vec2d Vec2d::GetNormalized() const {
    double len = GetLength();
    if (len < 1e-12) return Vec2d(0, 0);
    return *this / len;
}

double Vec2d::Normalize() {
    double len = GetLength();
    if (len < 1e-12) {
        x = y = 0;
        return 0;
    }
    x /= len;
    y /= len;
    return len;
}

// ============================================================================
// Vec3f implementations
// ============================================================================

float Vec3f::GetLength() const {
    return std::sqrt(x*x + y*y + z*z);
}

Vec3f Vec3f::GetNormalized() const {
    float len = GetLength();
    if (len < 1e-6f) return Vec3f(0, 0, 0);
    return *this / len;
}

float Vec3f::Normalize() {
    float len = GetLength();
    if (len < 1e-6f) {
        x = y = z = 0;
        return 0;
    }
    x /= len;
    y /= len;
    z /= len;
    return len;
}

// ============================================================================
// Vec3d implementations
// ============================================================================

double Vec3d::GetLength() const {
    return std::sqrt(x*x + y*y + z*z);
}

Vec3d Vec3d::GetNormalized() const {
    double len = GetLength();
    if (len < 1e-12) return Vec3d(0, 0, 0);
    return *this / len;
}

double Vec3d::Normalize() {
    double len = GetLength();
    if (len < 1e-12) {
        x = y = z = 0;
        return 0;
    }
    x /= len;
    y /= len;
    z /= len;
    return len;
}

// ============================================================================
// Vec4f implementations
// ============================================================================

float Vec4f::GetLength() const {
    return std::sqrt(x*x + y*y + z*z + w*w);
}

Vec4f Vec4f::GetNormalized() const {
    float len = GetLength();
    if (len < 1e-6f) return Vec4f(0, 0, 0, 0);
    return *this / len;
}

float Vec4f::Normalize() {
    float len = GetLength();
    if (len < 1e-6f) {
        x = y = z = w = 0;
        return 0;
    }
    x /= len;
    y /= len;
    z /= len;
    w /= len;
    return len;
}

// ============================================================================
// Vec4d implementations
// ============================================================================

double Vec4d::GetLength() const {
    return std::sqrt(x*x + y*y + z*z + w*w);
}

Vec4d Vec4d::GetNormalized() const {
    double len = GetLength();
    if (len < 1e-12) return Vec4d(0, 0, 0, 0);
    return *this / len;
}

double Vec4d::Normalize() {
    double len = GetLength();
    if (len < 1e-12) {
        x = y = z = w = 0;
        return 0;
    }
    x /= len;
    y /= len;
    z /= len;
    w /= len;
    return len;
}

// ============================================================================
// Matrix4f implementations
// ============================================================================

Matrix4f::Matrix4f() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            data[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Matrix4f::Matrix4f(float m00, float m01, float m02, float m03,
                   float m10, float m11, float m12, float m13,
                   float m20, float m21, float m22, float m23,
                   float m30, float m31, float m32, float m33) {
    data[0][0] = m00; data[0][1] = m01; data[0][2] = m02; data[0][3] = m03;
    data[1][0] = m10; data[1][1] = m11; data[1][2] = m12; data[1][3] = m13;
    data[2][0] = m20; data[2][1] = m21; data[2][2] = m22; data[2][3] = m23;
    data[3][0] = m30; data[3][1] = m31; data[3][2] = m32; data[3][3] = m33;
}

Matrix4f Matrix4f::Identity() {
    return Matrix4f();
}

Matrix4f Matrix4f::FromTranslation(const Vec3f& translation) {
    Matrix4f m;
    m[3][0] = translation.x;
    m[3][1] = translation.y;
    m[3][2] = translation.z;
    return m;
}

Matrix4f Matrix4f::FromScale(const Vec3f& scale) {
    Matrix4f m;
    m[0][0] = scale.x;
    m[1][1] = scale.y;
    m[2][2] = scale.z;
    return m;
}

Matrix4f Matrix4f::FromRotationX(float angleRadians) {
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    Matrix4f m;
    m[1][1] = c;  m[1][2] = s;
    m[2][1] = -s; m[2][2] = c;
    return m;
}

Matrix4f Matrix4f::FromRotationY(float angleRadians) {
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    Matrix4f m;
    m[0][0] = c;  m[0][2] = -s;
    m[2][0] = s;  m[2][2] = c;
    return m;
}

Matrix4f Matrix4f::FromRotationZ(float angleRadians) {
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    Matrix4f m;
    m[0][0] = c;  m[0][1] = s;
    m[1][0] = -s; m[1][1] = c;
    return m;
}

Matrix4f Matrix4f::operator*(const Matrix4f& other) const {
    Matrix4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix4f Matrix4f::operator+(const Matrix4f& other) const {
    Matrix4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix4f Matrix4f::operator-(const Matrix4f& other) const {
    Matrix4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix4f Matrix4f::operator*(float scalar) const {
    Matrix4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

Vec3f Matrix4f::TransformPoint(const Vec3f& point) const {
    // Row-major: row vector * matrix, with homogeneous divide
    // p' = p * M (for row vectors in row-major layout)
    float w = point.x * data[0][3] + point.y * data[1][3] + point.z * data[2][3] + data[3][3];
    if (std::abs(w) < 1e-6f) w = 1.0f;
    return Vec3f(
        (point.x * data[0][0] + point.y * data[1][0] + point.z * data[2][0] + data[3][0]) / w,
        (point.x * data[0][1] + point.y * data[1][1] + point.z * data[2][1] + data[3][1]) / w,
        (point.x * data[0][2] + point.y * data[1][2] + point.z * data[2][2] + data[3][2]) / w
    );
}

Vec3f Matrix4f::TransformDirection(const Vec3f& dir) const {
    // Row-major: row vector * matrix (3x3 upper left only)
    return Vec3f(
        dir.x * data[0][0] + dir.y * data[1][0] + dir.z * data[2][0],
        dir.x * data[0][1] + dir.y * data[1][1] + dir.z * data[2][1],
        dir.x * data[0][2] + dir.y * data[1][2] + dir.z * data[2][2]
    );
}

Vec4f Matrix4f::Transform(const Vec4f& v) const {
    // Row-major: row vector * matrix
    return Vec4f(
        v.x * data[0][0] + v.y * data[1][0] + v.z * data[2][0] + v.w * data[3][0],
        v.x * data[0][1] + v.y * data[1][1] + v.z * data[2][1] + v.w * data[3][1],
        v.x * data[0][2] + v.y * data[1][2] + v.z * data[2][2] + v.w * data[3][2],
        v.x * data[0][3] + v.y * data[1][3] + v.z * data[2][3] + v.w * data[3][3]
    );
}

Matrix4f Matrix4f::GetTranspose() const {
    Matrix4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[j][i];
        }
    }
    return result;
}

float Matrix4f::GetDeterminant() const {
    float s0 = data[0][0] * data[1][1] - data[1][0] * data[0][1];
    float s1 = data[0][0] * data[1][2] - data[1][0] * data[0][2];
    float s2 = data[0][0] * data[1][3] - data[1][0] * data[0][3];
    float s3 = data[0][1] * data[1][2] - data[1][1] * data[0][2];
    float s4 = data[0][1] * data[1][3] - data[1][1] * data[0][3];
    float s5 = data[0][2] * data[1][3] - data[1][2] * data[0][3];

    float c5 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
    float c4 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    float c3 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    float c2 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    float c1 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    float c0 = data[2][0] * data[3][1] - data[3][0] * data[2][1];

    return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
}

Matrix4f Matrix4f::GetInverse() const {
    Matrix4f inv;
    float det;

    float s0 = data[0][0] * data[1][1] - data[1][0] * data[0][1];
    float s1 = data[0][0] * data[1][2] - data[1][0] * data[0][2];
    float s2 = data[0][0] * data[1][3] - data[1][0] * data[0][3];
    float s3 = data[0][1] * data[1][2] - data[1][1] * data[0][2];
    float s4 = data[0][1] * data[1][3] - data[1][1] * data[0][3];
    float s5 = data[0][2] * data[1][3] - data[1][2] * data[0][3];

    float c5 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
    float c4 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    float c3 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    float c2 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    float c1 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    float c0 = data[2][0] * data[3][1] - data[3][0] * data[2][1];

    det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (std::abs(det) < 1e-6f) {
        return Matrix4f::Identity();
    }

    float invdet = 1.0f / det;

    inv[0][0] = ( data[1][1] * c5 - data[1][2] * c4 + data[1][3] * c3) * invdet;
    inv[0][1] = (-data[0][1] * c5 + data[0][2] * c4 - data[0][3] * c3) * invdet;
    inv[0][2] = ( data[3][1] * s5 - data[3][2] * s4 + data[3][3] * s3) * invdet;
    inv[0][3] = (-data[2][1] * s5 + data[2][2] * s4 - data[2][3] * s3) * invdet;

    inv[1][0] = (-data[1][0] * c5 + data[1][2] * c2 - data[1][3] * c1) * invdet;
    inv[1][1] = ( data[0][0] * c5 - data[0][2] * c2 + data[0][3] * c1) * invdet;
    inv[1][2] = (-data[3][0] * s5 + data[3][2] * s2 - data[3][3] * s1) * invdet;
    inv[1][3] = ( data[2][0] * s5 - data[2][2] * s2 + data[2][3] * s1) * invdet;

    inv[2][0] = ( data[1][0] * c4 - data[1][1] * c2 + data[1][3] * c0) * invdet;
    inv[2][1] = (-data[0][0] * c4 + data[0][1] * c2 - data[0][3] * c0) * invdet;
    inv[2][2] = ( data[3][0] * s4 - data[3][1] * s2 + data[3][3] * s0) * invdet;
    inv[2][3] = (-data[2][0] * s4 + data[2][1] * s2 - data[2][3] * s0) * invdet;

    inv[3][0] = (-data[1][0] * c3 + data[1][1] * c1 - data[1][2] * c0) * invdet;
    inv[3][1] = ( data[0][0] * c3 - data[0][1] * c1 + data[0][2] * c0) * invdet;
    inv[3][2] = (-data[3][0] * s3 + data[3][1] * s1 - data[3][2] * s0) * invdet;
    inv[3][3] = ( data[2][0] * s3 - data[2][1] * s1 + data[2][2] * s0) * invdet;

    return inv;
}

void Matrix4f::SetRow(size_t row, const Vec4f& v) {
    data[row][0] = v.x;
    data[row][1] = v.y;
    data[row][2] = v.z;
    data[row][3] = v.w;
}

Vec4f Matrix4f::GetRow(size_t row) const {
    return Vec4f(data[row][0], data[row][1], data[row][2], data[row][3]);
}

void Matrix4f::SetColumn(size_t col, const Vec4f& v) {
    data[0][col] = v.x;
    data[1][col] = v.y;
    data[2][col] = v.z;
    data[3][col] = v.w;
}

Vec4f Matrix4f::GetColumn(size_t col) const {
    return Vec4f(data[0][col], data[1][col], data[2][col], data[3][col]);
}

bool Matrix4f::operator==(const Matrix4f& other) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (data[i][j] != other.data[i][j]) return false;
        }
    }
    return true;
}

// ============================================================================
// Matrix4d implementations
// ============================================================================

Matrix4d::Matrix4d() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

Matrix4d::Matrix4d(double m00, double m01, double m02, double m03,
                   double m10, double m11, double m12, double m13,
                   double m20, double m21, double m22, double m23,
                   double m30, double m31, double m32, double m33) {
    data[0][0] = m00; data[0][1] = m01; data[0][2] = m02; data[0][3] = m03;
    data[1][0] = m10; data[1][1] = m11; data[1][2] = m12; data[1][3] = m13;
    data[2][0] = m20; data[2][1] = m21; data[2][2] = m22; data[2][3] = m23;
    data[3][0] = m30; data[3][1] = m31; data[3][2] = m32; data[3][3] = m33;
}

Matrix4d::Matrix4d(const Matrix4f& m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            data[i][j] = static_cast<double>(m.data[i][j]);
        }
    }
}

Matrix4d Matrix4d::Identity() {
    return Matrix4d();
}

Matrix4d Matrix4d::FromTranslation(const Vec3d& translation) {
    Matrix4d m;
    m[3][0] = translation.x;
    m[3][1] = translation.y;
    m[3][2] = translation.z;
    return m;
}

Matrix4d Matrix4d::FromScale(const Vec3d& scale) {
    Matrix4d m;
    m[0][0] = scale.x;
    m[1][1] = scale.y;
    m[2][2] = scale.z;
    return m;
}

Matrix4d Matrix4d::FromRotationX(double angleRadians) {
    double c = std::cos(angleRadians);
    double s = std::sin(angleRadians);
    Matrix4d m;
    m[1][1] = c;  m[1][2] = s;
    m[2][1] = -s; m[2][2] = c;
    return m;
}

Matrix4d Matrix4d::FromRotationY(double angleRadians) {
    double c = std::cos(angleRadians);
    double s = std::sin(angleRadians);
    Matrix4d m;
    m[0][0] = c;  m[0][2] = -s;
    m[2][0] = s;  m[2][2] = c;
    return m;
}

Matrix4d Matrix4d::FromRotationZ(double angleRadians) {
    double c = std::cos(angleRadians);
    double s = std::sin(angleRadians);
    Matrix4d m;
    m[0][0] = c;  m[0][1] = s;
    m[1][0] = -s; m[1][1] = c;
    return m;
}

Matrix4d Matrix4d::operator*(const Matrix4d& other) const {
    Matrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix4d Matrix4d::operator+(const Matrix4d& other) const {
    Matrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix4d Matrix4d::operator-(const Matrix4d& other) const {
    Matrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix4d Matrix4d::operator*(double scalar) const {
    Matrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

Vec3d Matrix4d::TransformPoint(const Vec3d& point) const {
    // Row-major: row vector * matrix, with homogeneous divide
    double w = point.x * data[0][3] + point.y * data[1][3] + point.z * data[2][3] + data[3][3];
    if (std::abs(w) < 1e-12) w = 1.0;
    return Vec3d(
        (point.x * data[0][0] + point.y * data[1][0] + point.z * data[2][0] + data[3][0]) / w,
        (point.x * data[0][1] + point.y * data[1][1] + point.z * data[2][1] + data[3][1]) / w,
        (point.x * data[0][2] + point.y * data[1][2] + point.z * data[2][2] + data[3][2]) / w
    );
}

Vec3d Matrix4d::TransformDirection(const Vec3d& dir) const {
    // Row-major: row vector * matrix (3x3 upper left only)
    return Vec3d(
        dir.x * data[0][0] + dir.y * data[1][0] + dir.z * data[2][0],
        dir.x * data[0][1] + dir.y * data[1][1] + dir.z * data[2][1],
        dir.x * data[0][2] + dir.y * data[1][2] + dir.z * data[2][2]
    );
}

Vec4d Matrix4d::Transform(const Vec4d& v) const {
    // Row-major: row vector * matrix
    return Vec4d(
        v.x * data[0][0] + v.y * data[1][0] + v.z * data[2][0] + v.w * data[3][0],
        v.x * data[0][1] + v.y * data[1][1] + v.z * data[2][1] + v.w * data[3][1],
        v.x * data[0][2] + v.y * data[1][2] + v.z * data[2][2] + v.w * data[3][2],
        v.x * data[0][3] + v.y * data[1][3] + v.z * data[2][3] + v.w * data[3][3]
    );
}

Matrix4d Matrix4d::GetTranspose() const {
    Matrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = data[j][i];
        }
    }
    return result;
}

double Matrix4d::GetDeterminant() const {
    double s0 = data[0][0] * data[1][1] - data[1][0] * data[0][1];
    double s1 = data[0][0] * data[1][2] - data[1][0] * data[0][2];
    double s2 = data[0][0] * data[1][3] - data[1][0] * data[0][3];
    double s3 = data[0][1] * data[1][2] - data[1][1] * data[0][2];
    double s4 = data[0][1] * data[1][3] - data[1][1] * data[0][3];
    double s5 = data[0][2] * data[1][3] - data[1][2] * data[0][3];

    double c5 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
    double c4 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    double c3 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    double c2 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    double c1 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    double c0 = data[2][0] * data[3][1] - data[3][0] * data[2][1];

    return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
}

Matrix4d Matrix4d::GetInverse() const {
    Matrix4d inv;
    double det;

    double s0 = data[0][0] * data[1][1] - data[1][0] * data[0][1];
    double s1 = data[0][0] * data[1][2] - data[1][0] * data[0][2];
    double s2 = data[0][0] * data[1][3] - data[1][0] * data[0][3];
    double s3 = data[0][1] * data[1][2] - data[1][1] * data[0][2];
    double s4 = data[0][1] * data[1][3] - data[1][1] * data[0][3];
    double s5 = data[0][2] * data[1][3] - data[1][2] * data[0][3];

    double c5 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
    double c4 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
    double c3 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
    double c2 = data[2][0] * data[3][3] - data[3][0] * data[2][3];
    double c1 = data[2][0] * data[3][2] - data[3][0] * data[2][2];
    double c0 = data[2][0] * data[3][1] - data[3][0] * data[2][1];

    det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (std::abs(det) < 1e-12) {
        return Matrix4d::Identity();
    }

    double invdet = 1.0 / det;

    inv[0][0] = ( data[1][1] * c5 - data[1][2] * c4 + data[1][3] * c3) * invdet;
    inv[0][1] = (-data[0][1] * c5 + data[0][2] * c4 - data[0][3] * c3) * invdet;
    inv[0][2] = ( data[3][1] * s5 - data[3][2] * s4 + data[3][3] * s3) * invdet;
    inv[0][3] = (-data[2][1] * s5 + data[2][2] * s4 - data[2][3] * s3) * invdet;

    inv[1][0] = (-data[1][0] * c5 + data[1][2] * c2 - data[1][3] * c1) * invdet;
    inv[1][1] = ( data[0][0] * c5 - data[0][2] * c2 + data[0][3] * c1) * invdet;
    inv[1][2] = (-data[3][0] * s5 + data[3][2] * s2 - data[3][3] * s1) * invdet;
    inv[1][3] = ( data[2][0] * s5 - data[2][2] * s2 + data[2][3] * s1) * invdet;

    inv[2][0] = ( data[1][0] * c4 - data[1][1] * c2 + data[1][3] * c0) * invdet;
    inv[2][1] = (-data[0][0] * c4 + data[0][1] * c2 - data[0][3] * c0) * invdet;
    inv[2][2] = ( data[3][0] * s4 - data[3][1] * s2 + data[3][3] * s0) * invdet;
    inv[2][3] = (-data[2][0] * s4 + data[2][1] * s2 - data[2][3] * s0) * invdet;

    inv[3][0] = (-data[1][0] * c3 + data[1][1] * c1 - data[1][2] * c0) * invdet;
    inv[3][1] = ( data[0][0] * c3 - data[0][1] * c1 + data[0][2] * c0) * invdet;
    inv[3][2] = (-data[3][0] * s3 + data[3][1] * s1 - data[3][2] * s0) * invdet;
    inv[3][3] = ( data[2][0] * s3 - data[2][1] * s1 + data[2][2] * s0) * invdet;

    return inv;
}

void Matrix4d::SetRow(size_t row, const Vec4d& v) {
    data[row][0] = v.x;
    data[row][1] = v.y;
    data[row][2] = v.z;
    data[row][3] = v.w;
}

Vec4d Matrix4d::GetRow(size_t row) const {
    return Vec4d(data[row][0], data[row][1], data[row][2], data[row][3]);
}

void Matrix4d::SetColumn(size_t col, const Vec4d& v) {
    data[0][col] = v.x;
    data[1][col] = v.y;
    data[2][col] = v.z;
    data[3][col] = v.w;
}

Vec4d Matrix4d::GetColumn(size_t col) const {
    return Vec4d(data[0][col], data[1][col], data[2][col], data[3][col]);
}

Matrix4f Matrix4d::ToFloat() const {
    Matrix4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = static_cast<float>(data[i][j]);
        }
    }
    return result;
}

bool Matrix4d::operator==(const Matrix4d& other) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (data[i][j] != other.data[i][j]) return false;
        }
    }
    return true;
}

// ============================================================================
// Quatf implementations
// ============================================================================

Quatf Quatf::FromAxisAngle(const Vec3f& axis, float angleRadians) {
    float halfAngle = angleRadians * 0.5f;
    float s = std::sin(halfAngle);
    Vec3f normalizedAxis = axis.GetNormalized();
    return Quatf(std::cos(halfAngle), normalizedAxis.x * s, normalizedAxis.y * s, normalizedAxis.z * s);
}

Quatf Quatf::FromRotationMatrix(const Matrix4f& m) {
    // Algorithm from Ken Shoemake's article in 1987 SIGGRAPH course notes
    float trace = m.data[0][0] + m.data[1][1] + m.data[2][2];
    Quatf q;

    if (trace > 0.0f) {
        float s = std::sqrt(trace + 1.0f) * 2.0f;
        q.w = 0.25f * s;
        q.x = (m.data[2][1] - m.data[1][2]) / s;
        q.y = (m.data[0][2] - m.data[2][0]) / s;
        q.z = (m.data[1][0] - m.data[0][1]) / s;
    } else if (m.data[0][0] > m.data[1][1] && m.data[0][0] > m.data[2][2]) {
        float s = std::sqrt(1.0f + m.data[0][0] - m.data[1][1] - m.data[2][2]) * 2.0f;
        q.w = (m.data[2][1] - m.data[1][2]) / s;
        q.x = 0.25f * s;
        q.y = (m.data[0][1] + m.data[1][0]) / s;
        q.z = (m.data[0][2] + m.data[2][0]) / s;
    } else if (m.data[1][1] > m.data[2][2]) {
        float s = std::sqrt(1.0f + m.data[1][1] - m.data[0][0] - m.data[2][2]) * 2.0f;
        q.w = (m.data[0][2] - m.data[2][0]) / s;
        q.x = (m.data[0][1] + m.data[1][0]) / s;
        q.y = 0.25f * s;
        q.z = (m.data[1][2] + m.data[2][1]) / s;
    } else {
        float s = std::sqrt(1.0f + m.data[2][2] - m.data[0][0] - m.data[1][1]) * 2.0f;
        q.w = (m.data[1][0] - m.data[0][1]) / s;
        q.x = (m.data[0][2] + m.data[2][0]) / s;
        q.y = (m.data[1][2] + m.data[2][1]) / s;
        q.z = 0.25f * s;
    }

    return q.GetNormalized();
}

Quatf Quatf::operator*(const Quatf& other) const {
    return Quatf(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    );
}

float Quatf::GetLength() const {
    return std::sqrt(w*w + x*x + y*y + z*z);
}

Quatf Quatf::GetNormalized() const {
    float len = GetLength();
    if (len < 1e-6f) return Quatf::Identity();
    return Quatf(w/len, x/len, y/len, z/len);
}

float Quatf::Normalize() {
    float len = GetLength();
    if (len < 1e-6f) {
        *this = Quatf::Identity();
        return 0;
    }
    w /= len;
    x /= len;
    y /= len;
    z /= len;
    return len;
}

Quatf Quatf::GetInverse() const {
    float lenSq = w*w + x*x + y*y + z*z;
    if (lenSq < 1e-6f) return Quatf::Identity();
    return Quatf(w/lenSq, -x/lenSq, -y/lenSq, -z/lenSq);
}

Vec3f Quatf::RotateVector(const Vec3f& v) const {
    // q * v * q^-1 (optimized)
    Vec3f qv(x, y, z);
    Vec3f uv = qv.Cross(v);
    Vec3f uuv = qv.Cross(uv);
    return v + (uv * w + uuv) * 2.0f;
}

Matrix4f Quatf::ToMatrix() const {
    Quatf q = GetNormalized();
    float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

    // Row-major layout: rows represent basis vectors after transform
    // For row-vector multiplication: v' = v * M
    // This is the transpose of the column-major rotation matrix
    return Matrix4f(
        1 - 2*(yy + zz),     2*(xy + wz),     2*(xz - wy), 0,
            2*(xy - wz), 1 - 2*(xx + zz),     2*(yz + wx), 0,
            2*(xz + wy),     2*(yz - wx), 1 - 2*(xx + yy), 0,
                      0,               0,               0, 1
    );
}

Quatf Quatf::Slerp(const Quatf& q0, const Quatf& q1, float t) {
    float dot = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;

    Quatf q1Adjusted = q1;
    if (dot < 0.0f) {
        q1Adjusted = -q1;
        dot = -dot;
    }

    if (dot > 0.9995f) {
        // Linear interpolation for very close quaternions
        Quatf result = q0 + (q1Adjusted - q0) * t;
        return result.GetNormalized();
    }

    float theta0 = std::acos(dot);
    float theta = theta0 * t;
    float sinTheta = std::sin(theta);
    float sinTheta0 = std::sin(theta0);

    float s0 = std::cos(theta) - dot * sinTheta / sinTheta0;
    float s1 = sinTheta / sinTheta0;

    return Quatf(
        s0 * q0.w + s1 * q1Adjusted.w,
        s0 * q0.x + s1 * q1Adjusted.x,
        s0 * q0.y + s1 * q1Adjusted.y,
        s0 * q0.z + s1 * q1Adjusted.z
    );
}

// ============================================================================
// Quatd implementations
// ============================================================================

Quatd Quatd::FromAxisAngle(const Vec3d& axis, double angleRadians) {
    double halfAngle = angleRadians * 0.5;
    double s = std::sin(halfAngle);
    Vec3d normalizedAxis = axis.GetNormalized();
    return Quatd(std::cos(halfAngle), normalizedAxis.x * s, normalizedAxis.y * s, normalizedAxis.z * s);
}

Quatd Quatd::FromRotationMatrix(const Matrix4d& m) {
    double trace = m.data[0][0] + m.data[1][1] + m.data[2][2];
    Quatd q;

    if (trace > 0.0) {
        double s = std::sqrt(trace + 1.0) * 2.0;
        q.w = 0.25 * s;
        q.x = (m.data[2][1] - m.data[1][2]) / s;
        q.y = (m.data[0][2] - m.data[2][0]) / s;
        q.z = (m.data[1][0] - m.data[0][1]) / s;
    } else if (m.data[0][0] > m.data[1][1] && m.data[0][0] > m.data[2][2]) {
        double s = std::sqrt(1.0 + m.data[0][0] - m.data[1][1] - m.data[2][2]) * 2.0;
        q.w = (m.data[2][1] - m.data[1][2]) / s;
        q.x = 0.25 * s;
        q.y = (m.data[0][1] + m.data[1][0]) / s;
        q.z = (m.data[0][2] + m.data[2][0]) / s;
    } else if (m.data[1][1] > m.data[2][2]) {
        double s = std::sqrt(1.0 + m.data[1][1] - m.data[0][0] - m.data[2][2]) * 2.0;
        q.w = (m.data[0][2] - m.data[2][0]) / s;
        q.x = (m.data[0][1] + m.data[1][0]) / s;
        q.y = 0.25 * s;
        q.z = (m.data[1][2] + m.data[2][1]) / s;
    } else {
        double s = std::sqrt(1.0 + m.data[2][2] - m.data[0][0] - m.data[1][1]) * 2.0;
        q.w = (m.data[1][0] - m.data[0][1]) / s;
        q.x = (m.data[0][2] + m.data[2][0]) / s;
        q.y = (m.data[1][2] + m.data[2][1]) / s;
        q.z = 0.25 * s;
    }

    return q.GetNormalized();
}

Quatd Quatd::operator*(const Quatd& other) const {
    return Quatd(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    );
}

double Quatd::GetLength() const {
    return std::sqrt(w*w + x*x + y*y + z*z);
}

Quatd Quatd::GetNormalized() const {
    double len = GetLength();
    if (len < 1e-12) return Quatd::Identity();
    return Quatd(w/len, x/len, y/len, z/len);
}

double Quatd::Normalize() {
    double len = GetLength();
    if (len < 1e-12) {
        *this = Quatd::Identity();
        return 0;
    }
    w /= len;
    x /= len;
    y /= len;
    z /= len;
    return len;
}

Quatd Quatd::GetInverse() const {
    double lenSq = w*w + x*x + y*y + z*z;
    if (lenSq < 1e-12) return Quatd::Identity();
    return Quatd(w/lenSq, -x/lenSq, -y/lenSq, -z/lenSq);
}

Vec3d Quatd::RotateVector(const Vec3d& v) const {
    Vec3d qv(x, y, z);
    Vec3d uv = qv.Cross(v);
    Vec3d uuv = qv.Cross(uv);
    return v + (uv * w + uuv) * 2.0;
}

Matrix4d Quatd::ToMatrix() const {
    Quatd q = GetNormalized();
    double xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    double xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    double wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

    // Row-major layout: rows represent basis vectors after transform
    // For row-vector multiplication: v' = v * M
    // This is the transpose of the column-major rotation matrix
    return Matrix4d(
        1 - 2*(yy + zz),     2*(xy + wz),     2*(xz - wy), 0,
            2*(xy - wz), 1 - 2*(xx + zz),     2*(yz + wx), 0,
            2*(xz + wy),     2*(yz - wx), 1 - 2*(xx + yy), 0,
                      0,               0,               0, 1
    );
}

Quatd Quatd::Slerp(const Quatd& q0, const Quatd& q1, double t) {
    double dot = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;

    Quatd q1Adjusted = q1;
    if (dot < 0.0) {
        q1Adjusted = -q1;
        dot = -dot;
    }

    if (dot > 0.9995) {
        Quatd result = q0 + (q1Adjusted - q0) * t;
        return result.GetNormalized();
    }

    double theta0 = std::acos(dot);
    double theta = theta0 * t;
    double sinTheta = std::sin(theta);
    double sinTheta0 = std::sin(theta0);

    double s0 = std::cos(theta) - dot * sinTheta / sinTheta0;
    double s1 = sinTheta / sinTheta0;

    return Quatd(
        s0 * q0.w + s1 * q1Adjusted.w,
        s0 * q0.x + s1 * q1Adjusted.x,
        s0 * q0.y + s1 * q1Adjusted.y,
        s0 * q0.z + s1 * q1Adjusted.z
    );
}

// ============================================================================
// Range3d implementations
// ============================================================================

Range3d::Range3d() {
    // Initialize to "empty" range (min > max)
    min = Vec3d(std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max());
    max = Vec3d(std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::lowest());
}

Range3d::Range3d(const Vec3d& min_, const Vec3d& max_) : min(min_), max(max_) {}

bool Range3d::IsEmpty() const {
    return min.x > max.x || min.y > max.y || min.z > max.z;
}

Vec3d Range3d::GetSize() const {
    if (IsEmpty()) return Vec3d(0, 0, 0);
    return max - min;
}

Vec3d Range3d::GetMidpoint() const {
    return (min + max) * 0.5;
}

Vec3d Range3d::GetCorner(size_t i) const {
    return Vec3d(
        (i & 1) ? max.x : min.x,
        (i & 2) ? max.y : min.y,
        (i & 4) ? max.z : min.z
    );
}

void Range3d::UnionWith(const Vec3d& point) {
    if (IsEmpty()) {
        min = max = point;
    } else {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }
}

void Range3d::UnionWith(const Range3d& other) {
    if (other.IsEmpty()) return;
    if (IsEmpty()) {
        *this = other;
    } else {
        min.x = std::min(min.x, other.min.x);
        min.y = std::min(min.y, other.min.y);
        min.z = std::min(min.z, other.min.z);
        max.x = std::max(max.x, other.max.x);
        max.y = std::max(max.y, other.max.y);
        max.z = std::max(max.z, other.max.z);
    }
}

void Range3d::ExtendBy(double amount) {
    if (IsEmpty()) return;
    Vec3d delta(amount, amount, amount);
    min = min - delta;
    max = max + delta;
}

bool Range3d::Contains(const Vec3d& point) const {
    return !IsEmpty() &&
           point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool Range3d::Contains(const Range3d& other) const {
    if (IsEmpty() || other.IsEmpty()) return false;
    return other.min.x >= min.x && other.max.x <= max.x &&
           other.min.y >= min.y && other.max.y <= max.y &&
           other.min.z >= min.z && other.max.z <= max.z;
}

bool Range3d::Intersects(const Range3d& other) const {
    if (IsEmpty() || other.IsEmpty()) return false;
    return min.x <= other.max.x && max.x >= other.min.x &&
           min.y <= other.max.y && max.y >= other.min.y &&
           min.z <= other.max.z && max.z >= other.min.z;
}

Range3d Range3d::GetIntersection(const Range3d& other) const {
    if (!Intersects(other)) return Range3d();
    return Range3d(
        Vec3d(std::max(min.x, other.min.x),
              std::max(min.y, other.min.y),
              std::max(min.z, other.min.z)),
        Vec3d(std::min(max.x, other.max.x),
              std::min(max.y, other.max.y),
              std::min(max.z, other.max.z))
    );
}

Range3d Range3d::GetUnion(const Range3d& other) const {
    Range3d result = *this;
    result.UnionWith(other);
    return result;
}

Vec3d Range3d::GetClosestPoint(const Vec3d& point) const {
    if (IsEmpty()) return point;
    return Vec3d(
        std::max(min.x, std::min(max.x, point.x)),
        std::max(min.y, std::min(max.y, point.y)),
        std::max(min.z, std::min(max.z, point.z))
    );
}

double Range3d::GetDistanceToPoint(const Vec3d& point) const {
    Vec3d closest = GetClosestPoint(point);
    return (point - closest).GetLength();
}

Range3d Range3d::Empty() {
    return Range3d();
}

// ============================================================================
// BBox3d implementations
// ============================================================================

BBox3d::BBox3d() : range_(), matrix_(Matrix4d::Identity()), hasNonIdentityMatrix_(false) {}

BBox3d::BBox3d(const Range3d& range) : range_(range), matrix_(Matrix4d::Identity()), hasNonIdentityMatrix_(false) {}

BBox3d::BBox3d(const Range3d& range, const Matrix4d& matrix)
    : range_(range), matrix_(matrix), hasNonIdentityMatrix_(true) {}

void BBox3d::SetMatrix(const Matrix4d& matrix) {
    matrix_ = matrix;
    hasNonIdentityMatrix_ = true;
}

double BBox3d::GetVolume() const {
    if (range_.IsEmpty()) return 0.0;
    Vec3d size = range_.GetSize();
    return size.x * size.y * size.z;
}

Vec3d BBox3d::GetCentroid() const {
    Vec3d localCenter = range_.GetMidpoint();
    if (!hasNonIdentityMatrix_) {
        return localCenter;
    }
    return matrix_.TransformPoint(localCenter);
}

void BBox3d::GetCorners(Vec3d corners[8]) const {
    for (size_t i = 0; i < 8; i++) {
        Vec3d localCorner = range_.GetCorner(i);
        if (hasNonIdentityMatrix_) {
            corners[i] = matrix_.TransformPoint(localCorner);
        } else {
            corners[i] = localCorner;
        }
    }
}

Range3d BBox3d::ComputeAlignedRange() const {
    if (range_.IsEmpty()) return Range3d();

    if (!hasNonIdentityMatrix_) {
        return range_;
    }

    Range3d result;
    Vec3d corners[8];
    GetCorners(corners);
    for (size_t i = 0; i < 8; i++) {
        result.UnionWith(corners[i]);
    }
    return result;
}

BBox3d BBox3d::Combine(const BBox3d& b1, const BBox3d& b2) {
    // For simplicity, compute aligned ranges and combine
    Range3d r1 = b1.ComputeAlignedRange();
    Range3d r2 = b2.ComputeAlignedRange();
    return BBox3d(r1.GetUnion(r2));
}

BBox3d BBox3d::Transform(const Matrix4d& matrix) const {
    if (hasNonIdentityMatrix_) {
        return BBox3d(range_, matrix * matrix_);
    }
    return BBox3d(range_, matrix);
}

} // namespace gf
} // namespace cxxfacade
