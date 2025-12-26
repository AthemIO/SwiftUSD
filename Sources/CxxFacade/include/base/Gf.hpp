// Gf.hpp - Graphics Foundation facade
// CLEAN HEADER - NO pxr includes!
//
// This module provides math types mirroring Pixar's GfVec, GfMatrix, GfQuat, etc.
// All types are value types without pimpl since they're just math primitives.
#pragma once

#include "../CxxFacade.h"

namespace cxxfacade {
namespace gf {

// ============================================================================
// Forward Declarations
// ============================================================================

class Vec2f;
class Vec2d;
class Vec3f;
class Vec3d;
class Vec4f;
class Vec4d;
class Matrix4f;
class Matrix4d;
class Quatf;
class Quatd;
class Range3d;
class BBox3d;

// ============================================================================
// Vec2f - 2D single-precision vector
// ============================================================================

/// Vec2f - 2D single-precision vector for texture coordinates, etc.
class Vec2f {
public:
    float x, y;

    Vec2f() : x(0), y(0) {}
    Vec2f(float x_, float y_) : x(x_), y(y_) {}
    explicit Vec2f(float scalar) : x(scalar), y(scalar) {}

    Vec2f operator+(const Vec2f& other) const {
        return Vec2f(x + other.x, y + other.y);
    }
    Vec2f operator-(const Vec2f& other) const {
        return Vec2f(x - other.x, y - other.y);
    }
    Vec2f operator*(float scalar) const {
        return Vec2f(x * scalar, y * scalar);
    }
    Vec2f operator/(float scalar) const {
        return Vec2f(x / scalar, y / scalar);
    }
    Vec2f operator-() const {
        return Vec2f(-x, -y);
    }

    Vec2f& operator+=(const Vec2f& other) {
        x += other.x; y += other.y;
        return *this;
    }
    Vec2f& operator-=(const Vec2f& other) {
        x -= other.x; y -= other.y;
        return *this;
    }
    Vec2f& operator*=(float scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }
    Vec2f& operator/=(float scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }

    float Dot(const Vec2f& other) const {
        return x * other.x + y * other.y;
    }
    float GetLength() const;
    float GetLengthSq() const { return x*x + y*y; }
    Vec2f GetNormalized() const;
    float Normalize();  // Returns old length

    bool operator==(const Vec2f& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vec2f& other) const { return !(*this == other); }

    const float& operator[](size_t i) const { return (&x)[i]; }
    float& operator[](size_t i) { return (&x)[i]; }

    static Vec2f Zero() { return Vec2f(0, 0); }
    static Vec2f Axis(size_t i) {
        Vec2f v(0, 0);
        v[i] = 1.0f;
        return v;
    }
};

inline Vec2f operator*(float scalar, const Vec2f& v) {
    return v * scalar;
}

// ============================================================================
// Vec2d - 2D double-precision vector
// ============================================================================

/// Vec2d - 2D double-precision vector
class Vec2d {
public:
    double x, y;

    Vec2d() : x(0), y(0) {}
    Vec2d(double x_, double y_) : x(x_), y(y_) {}
    explicit Vec2d(double scalar) : x(scalar), y(scalar) {}
    explicit Vec2d(const Vec2f& v) : x(v.x), y(v.y) {}

    Vec2d operator+(const Vec2d& other) const {
        return Vec2d(x + other.x, y + other.y);
    }
    Vec2d operator-(const Vec2d& other) const {
        return Vec2d(x - other.x, y - other.y);
    }
    Vec2d operator*(double scalar) const {
        return Vec2d(x * scalar, y * scalar);
    }
    Vec2d operator/(double scalar) const {
        return Vec2d(x / scalar, y / scalar);
    }
    Vec2d operator-() const {
        return Vec2d(-x, -y);
    }

    Vec2d& operator+=(const Vec2d& other) {
        x += other.x; y += other.y;
        return *this;
    }
    Vec2d& operator-=(const Vec2d& other) {
        x -= other.x; y -= other.y;
        return *this;
    }
    Vec2d& operator*=(double scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }
    Vec2d& operator/=(double scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }

    double Dot(const Vec2d& other) const {
        return x * other.x + y * other.y;
    }
    double GetLength() const;
    double GetLengthSq() const { return x*x + y*y; }
    Vec2d GetNormalized() const;
    double Normalize();  // Returns old length

    bool operator==(const Vec2d& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vec2d& other) const { return !(*this == other); }

    const double& operator[](size_t i) const { return (&x)[i]; }
    double& operator[](size_t i) { return (&x)[i]; }

    static Vec2d Zero() { return Vec2d(0, 0); }
    static Vec2d Axis(size_t i) {
        Vec2d v(0, 0);
        v[i] = 1.0;
        return v;
    }
};

inline Vec2d operator*(double scalar, const Vec2d& v) {
    return v * scalar;
}

// ============================================================================
// Vec3f - 3D single-precision vector
// ============================================================================

/// Vec3f - 3D single-precision vector for positions, normals, colors, etc.
class Vec3f {
public:
    float x, y, z;

    Vec3f() : x(0), y(0), z(0) {}
    Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    explicit Vec3f(float scalar) : x(scalar), y(scalar), z(scalar) {}

    Vec3f operator+(const Vec3f& other) const {
        return Vec3f(x + other.x, y + other.y, z + other.z);
    }
    Vec3f operator-(const Vec3f& other) const {
        return Vec3f(x - other.x, y - other.y, z - other.z);
    }
    Vec3f operator*(float scalar) const {
        return Vec3f(x * scalar, y * scalar, z * scalar);
    }
    Vec3f operator/(float scalar) const {
        return Vec3f(x / scalar, y / scalar, z / scalar);
    }
    Vec3f operator-() const {
        return Vec3f(-x, -y, -z);
    }

    Vec3f& operator+=(const Vec3f& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    Vec3f& operator-=(const Vec3f& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    Vec3f& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }
    Vec3f& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    float Dot(const Vec3f& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    Vec3f Cross(const Vec3f& other) const {
        return Vec3f(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    float GetLength() const;
    float GetLengthSq() const { return x*x + y*y + z*z; }
    Vec3f GetNormalized() const;
    float Normalize();  // Returns old length

    bool operator==(const Vec3f& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vec3f& other) const { return !(*this == other); }

    const float& operator[](size_t i) const { return (&x)[i]; }
    float& operator[](size_t i) { return (&x)[i]; }

    static Vec3f Zero() { return Vec3f(0, 0, 0); }
    static Vec3f XAxis() { return Vec3f(1, 0, 0); }
    static Vec3f YAxis() { return Vec3f(0, 1, 0); }
    static Vec3f ZAxis() { return Vec3f(0, 0, 1); }
    static Vec3f Axis(size_t i) {
        Vec3f v(0, 0, 0);
        v[i] = 1.0f;
        return v;
    }
};

inline Vec3f operator*(float scalar, const Vec3f& v) {
    return v * scalar;
}

// ============================================================================
// Vec3d - 3D double-precision vector
// ============================================================================

/// Vec3d - 3D double-precision vector
class Vec3d {
public:
    double x, y, z;

    Vec3d() : x(0), y(0), z(0) {}
    Vec3d(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    explicit Vec3d(double scalar) : x(scalar), y(scalar), z(scalar) {}
    explicit Vec3d(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}

    Vec3d operator+(const Vec3d& other) const {
        return Vec3d(x + other.x, y + other.y, z + other.z);
    }
    Vec3d operator-(const Vec3d& other) const {
        return Vec3d(x - other.x, y - other.y, z - other.z);
    }
    Vec3d operator*(double scalar) const {
        return Vec3d(x * scalar, y * scalar, z * scalar);
    }
    Vec3d operator/(double scalar) const {
        return Vec3d(x / scalar, y / scalar, z / scalar);
    }
    Vec3d operator-() const {
        return Vec3d(-x, -y, -z);
    }

    Vec3d& operator+=(const Vec3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    Vec3d& operator-=(const Vec3d& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    Vec3d& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }
    Vec3d& operator/=(double scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    double Dot(const Vec3d& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    Vec3d Cross(const Vec3d& other) const {
        return Vec3d(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    double GetLength() const;
    double GetLengthSq() const { return x*x + y*y + z*z; }
    Vec3d GetNormalized() const;
    double Normalize();  // Returns old length

    bool operator==(const Vec3d& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vec3d& other) const { return !(*this == other); }

    const double& operator[](size_t i) const { return (&x)[i]; }
    double& operator[](size_t i) { return (&x)[i]; }

    static Vec3d Zero() { return Vec3d(0, 0, 0); }
    static Vec3d XAxis() { return Vec3d(1, 0, 0); }
    static Vec3d YAxis() { return Vec3d(0, 1, 0); }
    static Vec3d ZAxis() { return Vec3d(0, 0, 1); }
    static Vec3d Axis(size_t i) {
        Vec3d v(0, 0, 0);
        v[i] = 1.0;
        return v;
    }
};

inline Vec3d operator*(double scalar, const Vec3d& v) {
    return v * scalar;
}

// ============================================================================
// Vec4f - 4D single-precision vector
// ============================================================================

/// Vec4f - 4D single-precision vector for RGBA colors, homogeneous coordinates, etc.
class Vec4f {
public:
    float x, y, z, w;

    Vec4f() : x(0), y(0), z(0), w(0) {}
    Vec4f(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    explicit Vec4f(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vec4f(const Vec3f& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}

    Vec4f operator+(const Vec4f& other) const {
        return Vec4f(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    Vec4f operator-(const Vec4f& other) const {
        return Vec4f(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    Vec4f operator*(float scalar) const {
        return Vec4f(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    Vec4f operator/(float scalar) const {
        return Vec4f(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    Vec4f operator-() const {
        return Vec4f(-x, -y, -z, -w);
    }

    Vec4f& operator+=(const Vec4f& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }
    Vec4f& operator-=(const Vec4f& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }
    Vec4f& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }
    Vec4f& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar; w /= scalar;
        return *this;
    }

    float Dot(const Vec4f& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    float GetLength() const;
    float GetLengthSq() const { return x*x + y*y + z*z + w*w; }
    Vec4f GetNormalized() const;
    float Normalize();  // Returns old length

    /// Get the first 3 components as Vec3f
    Vec3f GetVec3() const { return Vec3f(x, y, z); }

    bool operator==(const Vec4f& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    bool operator!=(const Vec4f& other) const { return !(*this == other); }

    const float& operator[](size_t i) const { return (&x)[i]; }
    float& operator[](size_t i) { return (&x)[i]; }

    static Vec4f Zero() { return Vec4f(0, 0, 0, 0); }
    static Vec4f Axis(size_t i) {
        Vec4f v(0, 0, 0, 0);
        v[i] = 1.0f;
        return v;
    }
};

inline Vec4f operator*(float scalar, const Vec4f& v) {
    return v * scalar;
}

// ============================================================================
// Vec4d - 4D double-precision vector
// ============================================================================

/// Vec4d - 4D double-precision vector
class Vec4d {
public:
    double x, y, z, w;

    Vec4d() : x(0), y(0), z(0), w(0) {}
    Vec4d(double x_, double y_, double z_, double w_) : x(x_), y(y_), z(z_), w(w_) {}
    explicit Vec4d(double scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vec4d(const Vec3d& v, double w_) : x(v.x), y(v.y), z(v.z), w(w_) {}
    explicit Vec4d(const Vec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    Vec4d operator+(const Vec4d& other) const {
        return Vec4d(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    Vec4d operator-(const Vec4d& other) const {
        return Vec4d(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    Vec4d operator*(double scalar) const {
        return Vec4d(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    Vec4d operator/(double scalar) const {
        return Vec4d(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    Vec4d operator-() const {
        return Vec4d(-x, -y, -z, -w);
    }

    Vec4d& operator+=(const Vec4d& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }
    Vec4d& operator-=(const Vec4d& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }
    Vec4d& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }
    Vec4d& operator/=(double scalar) {
        x /= scalar; y /= scalar; z /= scalar; w /= scalar;
        return *this;
    }

    double Dot(const Vec4d& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    double GetLength() const;
    double GetLengthSq() const { return x*x + y*y + z*z + w*w; }
    Vec4d GetNormalized() const;
    double Normalize();  // Returns old length

    /// Get the first 3 components as Vec3d
    Vec3d GetVec3() const { return Vec3d(x, y, z); }

    bool operator==(const Vec4d& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    bool operator!=(const Vec4d& other) const { return !(*this == other); }

    const double& operator[](size_t i) const { return (&x)[i]; }
    double& operator[](size_t i) { return (&x)[i]; }

    static Vec4d Zero() { return Vec4d(0, 0, 0, 0); }
    static Vec4d Axis(size_t i) {
        Vec4d v(0, 0, 0, 0);
        v[i] = 1.0;
        return v;
    }
};

inline Vec4d operator*(double scalar, const Vec4d& v) {
    return v * scalar;
}

// ============================================================================
// Matrix4f - 4x4 single-precision transformation matrix
// ============================================================================

/// Matrix4f - 4x4 single-precision transformation matrix (row-major)
class Matrix4f {
public:
    float data[4][4];

    Matrix4f();  // Identity
    Matrix4f(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33);

    static Matrix4f Identity();
    static Matrix4f FromTranslation(const Vec3f& translation);
    static Matrix4f FromScale(const Vec3f& scale);
    static Matrix4f FromRotationX(float angleRadians);
    static Matrix4f FromRotationY(float angleRadians);
    static Matrix4f FromRotationZ(float angleRadians);

    Matrix4f operator*(const Matrix4f& other) const;
    Matrix4f operator+(const Matrix4f& other) const;
    Matrix4f operator-(const Matrix4f& other) const;
    Matrix4f operator*(float scalar) const;

    Vec3f TransformPoint(const Vec3f& point) const;
    Vec3f TransformDirection(const Vec3f& dir) const;
    Vec4f Transform(const Vec4f& v) const;

    Matrix4f GetInverse() const;
    Matrix4f GetTranspose() const;
    float GetDeterminant() const;

    /// Set a row of the matrix
    void SetRow(size_t row, const Vec4f& v);
    /// Get a row of the matrix
    Vec4f GetRow(size_t row) const;
    /// Set a column of the matrix
    void SetColumn(size_t col, const Vec4f& v);
    /// Get a column of the matrix
    Vec4f GetColumn(size_t col) const;

    float* operator[](size_t row) { return data[row]; }
    const float* operator[](size_t row) const { return data[row]; }

    bool operator==(const Matrix4f& other) const;
    bool operator!=(const Matrix4f& other) const { return !(*this == other); }
};

inline Matrix4f operator*(float scalar, const Matrix4f& m) {
    return m * scalar;
}

// ============================================================================
// Matrix4d - 4x4 double-precision transformation matrix
// ============================================================================

/// Matrix4d - 4x4 double-precision transformation matrix (row-major)
class Matrix4d {
public:
    double data[4][4];

    Matrix4d();  // Identity
    Matrix4d(double m00, double m01, double m02, double m03,
             double m10, double m11, double m12, double m13,
             double m20, double m21, double m22, double m23,
             double m30, double m31, double m32, double m33);
    explicit Matrix4d(const Matrix4f& m);

    static Matrix4d Identity();
    static Matrix4d FromTranslation(const Vec3d& translation);
    static Matrix4d FromScale(const Vec3d& scale);
    static Matrix4d FromRotationX(double angleRadians);
    static Matrix4d FromRotationY(double angleRadians);
    static Matrix4d FromRotationZ(double angleRadians);

    Matrix4d operator*(const Matrix4d& other) const;
    Matrix4d operator+(const Matrix4d& other) const;
    Matrix4d operator-(const Matrix4d& other) const;
    Matrix4d operator*(double scalar) const;

    Vec3d TransformPoint(const Vec3d& point) const;
    Vec3d TransformDirection(const Vec3d& dir) const;
    Vec4d Transform(const Vec4d& v) const;

    Matrix4d GetInverse() const;
    Matrix4d GetTranspose() const;
    double GetDeterminant() const;

    /// Set a row of the matrix
    void SetRow(size_t row, const Vec4d& v);
    /// Get a row of the matrix
    Vec4d GetRow(size_t row) const;
    /// Set a column of the matrix
    void SetColumn(size_t col, const Vec4d& v);
    /// Get a column of the matrix
    Vec4d GetColumn(size_t col) const;

    /// Convert to single-precision
    Matrix4f ToFloat() const;

    double* operator[](size_t row) { return data[row]; }
    const double* operator[](size_t row) const { return data[row]; }

    bool operator==(const Matrix4d& other) const;
    bool operator!=(const Matrix4d& other) const { return !(*this == other); }
};

inline Matrix4d operator*(double scalar, const Matrix4d& m) {
    return m * scalar;
}

// ============================================================================
// Quatf - Single-precision quaternion
// ============================================================================

/// Quatf - Single-precision quaternion for 3D rotations
class Quatf {
public:
    float w, x, y, z;  // w is real part, (x,y,z) is imaginary

    Quatf() : w(1), x(0), y(0), z(0) {}  // Identity
    Quatf(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_) {}
    Quatf(float real, const Vec3f& imaginary) : w(real), x(imaginary.x), y(imaginary.y), z(imaginary.z) {}

    static Quatf Identity() { return Quatf(1, 0, 0, 0); }
    static Quatf FromAxisAngle(const Vec3f& axis, float angleRadians);
    static Quatf FromRotationMatrix(const Matrix4f& m);

    Quatf operator*(const Quatf& other) const;
    Quatf operator+(const Quatf& other) const {
        return Quatf(w + other.w, x + other.x, y + other.y, z + other.z);
    }
    Quatf operator-(const Quatf& other) const {
        return Quatf(w - other.w, x - other.x, y - other.y, z - other.z);
    }
    Quatf operator*(float scalar) const {
        return Quatf(w * scalar, x * scalar, y * scalar, z * scalar);
    }
    Quatf operator-() const {
        return Quatf(-w, -x, -y, -z);
    }

    Quatf GetConjugate() const { return Quatf(w, -x, -y, -z); }
    Quatf GetInverse() const;
    Quatf GetNormalized() const;
    float GetLength() const;
    float GetLengthSq() const { return w*w + x*x + y*y + z*z; }
    float Normalize();  // Returns old length

    /// Get the real (scalar) part
    float GetReal() const { return w; }
    /// Get the imaginary (vector) part
    Vec3f GetImaginary() const { return Vec3f(x, y, z); }

    Vec3f RotateVector(const Vec3f& v) const;
    Matrix4f ToMatrix() const;

    /// Spherical linear interpolation
    static Quatf Slerp(const Quatf& q0, const Quatf& q1, float t);

    bool operator==(const Quatf& other) const {
        return w == other.w && x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Quatf& other) const { return !(*this == other); }
};

inline Quatf operator*(float scalar, const Quatf& q) {
    return q * scalar;
}

// ============================================================================
// Quatd - Double-precision quaternion
// ============================================================================

/// Quatd - Double-precision quaternion for 3D rotations
class Quatd {
public:
    double w, x, y, z;  // w is real part, (x,y,z) is imaginary

    Quatd() : w(1), x(0), y(0), z(0) {}  // Identity
    Quatd(double w_, double x_, double y_, double z_) : w(w_), x(x_), y(y_), z(z_) {}
    Quatd(double real, const Vec3d& imaginary) : w(real), x(imaginary.x), y(imaginary.y), z(imaginary.z) {}
    explicit Quatd(const Quatf& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}

    static Quatd Identity() { return Quatd(1, 0, 0, 0); }
    static Quatd FromAxisAngle(const Vec3d& axis, double angleRadians);
    static Quatd FromRotationMatrix(const Matrix4d& m);

    Quatd operator*(const Quatd& other) const;
    Quatd operator+(const Quatd& other) const {
        return Quatd(w + other.w, x + other.x, y + other.y, z + other.z);
    }
    Quatd operator-(const Quatd& other) const {
        return Quatd(w - other.w, x - other.x, y - other.y, z - other.z);
    }
    Quatd operator*(double scalar) const {
        return Quatd(w * scalar, x * scalar, y * scalar, z * scalar);
    }
    Quatd operator-() const {
        return Quatd(-w, -x, -y, -z);
    }

    Quatd GetConjugate() const { return Quatd(w, -x, -y, -z); }
    Quatd GetInverse() const;
    Quatd GetNormalized() const;
    double GetLength() const;
    double GetLengthSq() const { return w*w + x*x + y*y + z*z; }
    double Normalize();  // Returns old length

    /// Get the real (scalar) part
    double GetReal() const { return w; }
    /// Get the imaginary (vector) part
    Vec3d GetImaginary() const { return Vec3d(x, y, z); }

    Vec3d RotateVector(const Vec3d& v) const;
    Matrix4d ToMatrix() const;

    /// Convert to single-precision
    Quatf ToFloat() const { return Quatf(static_cast<float>(w), static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }

    /// Spherical linear interpolation
    static Quatd Slerp(const Quatd& q0, const Quatd& q1, double t);

    bool operator==(const Quatd& other) const {
        return w == other.w && x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Quatd& other) const { return !(*this == other); }
};

inline Quatd operator*(double scalar, const Quatd& q) {
    return q * scalar;
}

// ============================================================================
// Range3d - Axis-aligned 3D range (bounding box without transform)
// ============================================================================

/// Range3d - Axis-aligned 3D range defined by min and max points
class Range3d {
public:
    Vec3d min;
    Vec3d max;

    /// Create an empty (invalid) range
    Range3d();

    /// Create a range from min and max points
    Range3d(const Vec3d& min_, const Vec3d& max_);

    /// Check if the range is empty (invalid)
    bool IsEmpty() const;

    /// Get the size of the range along each axis
    Vec3d GetSize() const;

    /// Get the center point of the range
    Vec3d GetMidpoint() const;

    /// Get the corner points of the range
    Vec3d GetCorner(size_t i) const;

    /// Union this range with a point
    void UnionWith(const Vec3d& point);

    /// Union this range with another range
    void UnionWith(const Range3d& other);

    /// Extend the range by a distance in all directions
    void ExtendBy(double amount);

    /// Check if the range contains a point
    bool Contains(const Vec3d& point) const;

    /// Check if this range contains another range
    bool Contains(const Range3d& other) const;

    /// Check if this range intersects another range
    bool Intersects(const Range3d& other) const;

    /// Get the intersection of this range with another
    Range3d GetIntersection(const Range3d& other) const;

    /// Get the union of this range with another
    Range3d GetUnion(const Range3d& other) const;

    /// Get the closest point in the range to the given point
    Vec3d GetClosestPoint(const Vec3d& point) const;

    /// Get the distance from a point to the range (0 if inside)
    double GetDistanceToPoint(const Vec3d& point) const;

    bool operator==(const Range3d& other) const {
        return min == other.min && max == other.max;
    }
    bool operator!=(const Range3d& other) const { return !(*this == other); }

    /// Create an empty range
    static Range3d Empty();
};

// ============================================================================
// BBox3d - Axis-aligned bounding box with optional transform
// ============================================================================

/// BBox3d - 3D bounding box with optional transformation matrix
///
/// Unlike Range3d, BBox3d can represent oriented bounding boxes through
/// its transformation matrix. The box represents the transformed Range3d.
class BBox3d {
public:
    /// Create an empty bounding box
    BBox3d();

    /// Create a bounding box from a range
    explicit BBox3d(const Range3d& range);

    /// Create a bounding box from a range and transformation
    BBox3d(const Range3d& range, const Matrix4d& matrix);

    /// Get the underlying range (in local space)
    const Range3d& GetRange() const { return range_; }

    /// Set the range
    void SetRange(const Range3d& range) { range_ = range; }

    /// Get the transformation matrix
    const Matrix4d& GetMatrix() const { return matrix_; }

    /// Set the transformation matrix
    void SetMatrix(const Matrix4d& matrix);

    /// Check if the box is empty
    bool IsEmpty() const { return range_.IsEmpty(); }

    /// Get the volume of the box
    double GetVolume() const;

    /// Get the center of the box (in world space)
    Vec3d GetCentroid() const;

    /// Get the 8 corners of the box (in world space)
    void GetCorners(Vec3d corners[8]) const;

    /// Compute the axis-aligned bounding box in world space
    Range3d ComputeAlignedRange() const;

    /// Combine this box with another (union)
    static BBox3d Combine(const BBox3d& b1, const BBox3d& b2);

    /// Transform this box by a matrix
    BBox3d Transform(const Matrix4d& matrix) const;

    bool operator==(const BBox3d& other) const {
        return range_ == other.range_ && matrix_ == other.matrix_;
    }
    bool operator!=(const BBox3d& other) const { return !(*this == other); }

private:
    Range3d range_;
    Matrix4d matrix_;
    bool hasNonIdentityMatrix_;
};

} // namespace gf
} // namespace cxxfacade
