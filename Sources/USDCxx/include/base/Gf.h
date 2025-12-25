// Gf.h - Graphics Foundations for SwiftUSD
// Mirrors: pxr/base/gf/*.h (vec, matrix, quaternion, range, bbox, frustum)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"

#include <cmath>
#include <algorithm>
#include <cstddef>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/base/gf/vec2f.h"
#include "pxr/base/gf/vec2d.h"
#include "pxr/base/gf/vec2i.h"
#include "pxr/base/gf/vec3f.h"
#include "pxr/base/gf/vec3d.h"
#include "pxr/base/gf/vec3i.h"
#include "pxr/base/gf/vec4f.h"
#include "pxr/base/gf/vec4d.h"
#include "pxr/base/gf/matrix4d.h"
#include "pxr/base/gf/matrix4f.h"
#include "pxr/base/gf/quatd.h"
#include "pxr/base/gf/quatf.h"
#include "pxr/base/gf/range3d.h"
#include "pxr/base/gf/range3f.h"
#include "pxr/base/gf/bbox3d.h"
#include "pxr/base/gf/frustum.h"
#endif

namespace swiftusd {

// ============================================================================
// MARK: - Vec2f (2D float vector)
// ============================================================================

/// A 2D vector of single-precision floats.
struct Vec2f {
    float x, y;

    // Constructors
    Vec2f() SWIFTUSD_NOEXCEPT : x(0), y(0) {}
    Vec2f(float x_, float y_) SWIFTUSD_NOEXCEPT : x(x_), y(y_) {}
    explicit Vec2f(float scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar) {}

#if defined(USE_PIXAR_USD)
    Vec2f(const PXR_NS::GfVec2f& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]) {}
    PXR_NS::GfVec2f ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec2f(x, y); }
#endif

    // Static factories
    static Vec2f Zero() SWIFTUSD_NOEXCEPT { return Vec2f(0, 0); }
    static Vec2f XAxis() SWIFTUSD_NOEXCEPT { return Vec2f(1, 0); }
    static Vec2f YAxis() SWIFTUSD_NOEXCEPT { return Vec2f(0, 1); }

    // Element access
    float operator[](size_t i) const SWIFTUSD_NOEXCEPT { return i == 0 ? x : y; }
    float& operator[](size_t i) SWIFTUSD_NOEXCEPT { return i == 0 ? x : y; }

    // Length operations
    float GetLengthSq() const SWIFTUSD_NOEXCEPT { return x*x + y*y; }
    float GetLength() const SWIFTUSD_NOEXCEPT { return std::sqrt(GetLengthSq()); }

    Vec2f GetNormalized() const SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        return len > 0 ? Vec2f(x/len, y/len) : Vec2f();
    }

    float Normalize() SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        if (len > 0) { x /= len; y /= len; }
        return len;
    }

    // Dot product
    float Dot(const Vec2f& other) const SWIFTUSD_NOEXCEPT {
        return x * other.x + y * other.y;
    }

    // Arithmetic operators
    Vec2f operator+(const Vec2f& r) const SWIFTUSD_NOEXCEPT { return Vec2f(x+r.x, y+r.y); }
    Vec2f operator-(const Vec2f& r) const SWIFTUSD_NOEXCEPT { return Vec2f(x-r.x, y-r.y); }
    Vec2f operator*(float s) const SWIFTUSD_NOEXCEPT { return Vec2f(x*s, y*s); }
    Vec2f operator/(float s) const SWIFTUSD_NOEXCEPT { return Vec2f(x/s, y/s); }
    Vec2f operator-() const SWIFTUSD_NOEXCEPT { return Vec2f(-x, -y); }

    Vec2f& operator+=(const Vec2f& r) SWIFTUSD_NOEXCEPT { x+=r.x; y+=r.y; return *this; }
    Vec2f& operator-=(const Vec2f& r) SWIFTUSD_NOEXCEPT { x-=r.x; y-=r.y; return *this; }
    Vec2f& operator*=(float s) SWIFTUSD_NOEXCEPT { x*=s; y*=s; return *this; }
    Vec2f& operator/=(float s) SWIFTUSD_NOEXCEPT { x/=s; y/=s; return *this; }

    // Comparison
    bool operator==(const Vec2f& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y; }
    bool operator!=(const Vec2f& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Vec2f operator*(float s, const Vec2f& v) SWIFTUSD_NOEXCEPT { return v * s; }

// ============================================================================
// MARK: - Vec2d (2D double vector)
// ============================================================================

/// A 2D vector of double-precision floats.
struct Vec2d {
    double x, y;

    Vec2d() SWIFTUSD_NOEXCEPT : x(0), y(0) {}
    Vec2d(double x_, double y_) SWIFTUSD_NOEXCEPT : x(x_), y(y_) {}
    explicit Vec2d(double scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar) {}
    explicit Vec2d(const Vec2f& v) SWIFTUSD_NOEXCEPT : x(v.x), y(v.y) {}

#if defined(USE_PIXAR_USD)
    Vec2d(const PXR_NS::GfVec2d& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]) {}
    PXR_NS::GfVec2d ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec2d(x, y); }
#endif

    static Vec2d Zero() SWIFTUSD_NOEXCEPT { return Vec2d(0, 0); }
    static Vec2d XAxis() SWIFTUSD_NOEXCEPT { return Vec2d(1, 0); }
    static Vec2d YAxis() SWIFTUSD_NOEXCEPT { return Vec2d(0, 1); }

    double operator[](size_t i) const SWIFTUSD_NOEXCEPT { return i == 0 ? x : y; }
    double& operator[](size_t i) SWIFTUSD_NOEXCEPT { return i == 0 ? x : y; }

    double GetLengthSq() const SWIFTUSD_NOEXCEPT { return x*x + y*y; }
    double GetLength() const SWIFTUSD_NOEXCEPT { return std::sqrt(GetLengthSq()); }

    Vec2d GetNormalized() const SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        return len > 0 ? Vec2d(x/len, y/len) : Vec2d();
    }

    double Normalize() SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        if (len > 0) { x /= len; y /= len; }
        return len;
    }

    double Dot(const Vec2d& other) const SWIFTUSD_NOEXCEPT {
        return x * other.x + y * other.y;
    }

    Vec2d operator+(const Vec2d& r) const SWIFTUSD_NOEXCEPT { return Vec2d(x+r.x, y+r.y); }
    Vec2d operator-(const Vec2d& r) const SWIFTUSD_NOEXCEPT { return Vec2d(x-r.x, y-r.y); }
    Vec2d operator*(double s) const SWIFTUSD_NOEXCEPT { return Vec2d(x*s, y*s); }
    Vec2d operator/(double s) const SWIFTUSD_NOEXCEPT { return Vec2d(x/s, y/s); }
    Vec2d operator-() const SWIFTUSD_NOEXCEPT { return Vec2d(-x, -y); }

    Vec2d& operator+=(const Vec2d& r) SWIFTUSD_NOEXCEPT { x+=r.x; y+=r.y; return *this; }
    Vec2d& operator-=(const Vec2d& r) SWIFTUSD_NOEXCEPT { x-=r.x; y-=r.y; return *this; }
    Vec2d& operator*=(double s) SWIFTUSD_NOEXCEPT { x*=s; y*=s; return *this; }
    Vec2d& operator/=(double s) SWIFTUSD_NOEXCEPT { x/=s; y/=s; return *this; }

    bool operator==(const Vec2d& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y; }
    bool operator!=(const Vec2d& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Vec2d operator*(double s, const Vec2d& v) SWIFTUSD_NOEXCEPT { return v * s; }

// ============================================================================
// MARK: - Vec2i (2D integer vector)
// ============================================================================

/// A 2D vector of 32-bit integers.
struct Vec2i {
    int x, y;

    Vec2i() SWIFTUSD_NOEXCEPT : x(0), y(0) {}
    Vec2i(int x_, int y_) SWIFTUSD_NOEXCEPT : x(x_), y(y_) {}
    explicit Vec2i(int scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar) {}

#if defined(USE_PIXAR_USD)
    Vec2i(const PXR_NS::GfVec2i& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]) {}
    PXR_NS::GfVec2i ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec2i(x, y); }
#endif

    static Vec2i Zero() SWIFTUSD_NOEXCEPT { return Vec2i(0, 0); }

    int operator[](size_t i) const SWIFTUSD_NOEXCEPT { return i == 0 ? x : y; }
    int& operator[](size_t i) SWIFTUSD_NOEXCEPT { return i == 0 ? x : y; }

    Vec2i operator+(const Vec2i& r) const SWIFTUSD_NOEXCEPT { return Vec2i(x+r.x, y+r.y); }
    Vec2i operator-(const Vec2i& r) const SWIFTUSD_NOEXCEPT { return Vec2i(x-r.x, y-r.y); }
    Vec2i operator*(int s) const SWIFTUSD_NOEXCEPT { return Vec2i(x*s, y*s); }
    Vec2i operator-() const SWIFTUSD_NOEXCEPT { return Vec2i(-x, -y); }

    bool operator==(const Vec2i& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y; }
    bool operator!=(const Vec2i& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Vec3f (3D float vector)
// ============================================================================

/// A 3D vector of single-precision floats.
struct Vec3f {
    float x, y, z;

    Vec3f() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0) {}
    Vec3f(float x_, float y_, float z_) SWIFTUSD_NOEXCEPT : x(x_), y(y_), z(z_) {}
    explicit Vec3f(float scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar), z(scalar) {}

#if defined(USE_PIXAR_USD)
    Vec3f(const PXR_NS::GfVec3f& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]) {}
    PXR_NS::GfVec3f ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec3f(x, y, z); }
#endif

    static Vec3f Zero() SWIFTUSD_NOEXCEPT { return Vec3f(0, 0, 0); }
    static Vec3f One() SWIFTUSD_NOEXCEPT { return Vec3f(1, 1, 1); }
    static Vec3f XAxis() SWIFTUSD_NOEXCEPT { return Vec3f(1, 0, 0); }
    static Vec3f YAxis() SWIFTUSD_NOEXCEPT { return Vec3f(0, 1, 0); }
    static Vec3f ZAxis() SWIFTUSD_NOEXCEPT { return Vec3f(0, 0, 1); }

    float operator[](size_t i) const SWIFTUSD_NOEXCEPT {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    float& operator[](size_t i) SWIFTUSD_NOEXCEPT {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    float GetLengthSq() const SWIFTUSD_NOEXCEPT { return x*x + y*y + z*z; }
    float GetLength() const SWIFTUSD_NOEXCEPT { return std::sqrt(GetLengthSq()); }

    Vec3f GetNormalized() const SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        return len > 0 ? Vec3f(x/len, y/len, z/len) : Vec3f();
    }

    float Normalize() SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        if (len > 0) { x /= len; y /= len; z /= len; }
        return len;
    }

    float Dot(const Vec3f& other) const SWIFTUSD_NOEXCEPT {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3f Cross(const Vec3f& r) const SWIFTUSD_NOEXCEPT {
        return Vec3f(
            y * r.z - z * r.y,
            z * r.x - x * r.z,
            x * r.y - y * r.x
        );
    }

    // Projection of this vector onto another
    Vec3f GetProjection(const Vec3f& onto) const SWIFTUSD_NOEXCEPT {
        float d = onto.Dot(onto);
        return d > 0 ? onto * (Dot(onto) / d) : Vec3f();
    }

    // Orthogonal complement (perpendicular component)
    Vec3f GetComplement(const Vec3f& onto) const SWIFTUSD_NOEXCEPT {
        return *this - GetProjection(onto);
    }

    Vec3f operator+(const Vec3f& r) const SWIFTUSD_NOEXCEPT { return Vec3f(x+r.x, y+r.y, z+r.z); }
    Vec3f operator-(const Vec3f& r) const SWIFTUSD_NOEXCEPT { return Vec3f(x-r.x, y-r.y, z-r.z); }
    Vec3f operator*(float s) const SWIFTUSD_NOEXCEPT { return Vec3f(x*s, y*s, z*s); }
    Vec3f operator/(float s) const SWIFTUSD_NOEXCEPT { return Vec3f(x/s, y/s, z/s); }
    Vec3f operator-() const SWIFTUSD_NOEXCEPT { return Vec3f(-x, -y, -z); }

    Vec3f& operator+=(const Vec3f& r) SWIFTUSD_NOEXCEPT { x+=r.x; y+=r.y; z+=r.z; return *this; }
    Vec3f& operator-=(const Vec3f& r) SWIFTUSD_NOEXCEPT { x-=r.x; y-=r.y; z-=r.z; return *this; }
    Vec3f& operator*=(float s) SWIFTUSD_NOEXCEPT { x*=s; y*=s; z*=s; return *this; }
    Vec3f& operator/=(float s) SWIFTUSD_NOEXCEPT { x/=s; y/=s; z/=s; return *this; }

    bool operator==(const Vec3f& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y && z==r.z; }
    bool operator!=(const Vec3f& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Vec3f operator*(float s, const Vec3f& v) SWIFTUSD_NOEXCEPT { return v * s; }

// ============================================================================
// MARK: - Vec3d (3D double vector)
// ============================================================================

/// A 3D vector of double-precision floats.
struct Vec3d {
    double x, y, z;

    Vec3d() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0) {}
    Vec3d(double x_, double y_, double z_) SWIFTUSD_NOEXCEPT : x(x_), y(y_), z(z_) {}
    explicit Vec3d(double scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar), z(scalar) {}
    explicit Vec3d(const Vec3f& v) SWIFTUSD_NOEXCEPT : x(v.x), y(v.y), z(v.z) {}

#if defined(USE_PIXAR_USD)
    Vec3d(const PXR_NS::GfVec3d& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]) {}
    PXR_NS::GfVec3d ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec3d(x, y, z); }
#endif

    static Vec3d Zero() SWIFTUSD_NOEXCEPT { return Vec3d(0, 0, 0); }
    static Vec3d One() SWIFTUSD_NOEXCEPT { return Vec3d(1, 1, 1); }
    static Vec3d XAxis() SWIFTUSD_NOEXCEPT { return Vec3d(1, 0, 0); }
    static Vec3d YAxis() SWIFTUSD_NOEXCEPT { return Vec3d(0, 1, 0); }
    static Vec3d ZAxis() SWIFTUSD_NOEXCEPT { return Vec3d(0, 0, 1); }

    double operator[](size_t i) const SWIFTUSD_NOEXCEPT {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    double& operator[](size_t i) SWIFTUSD_NOEXCEPT {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    double GetLengthSq() const SWIFTUSD_NOEXCEPT { return x*x + y*y + z*z; }
    double GetLength() const SWIFTUSD_NOEXCEPT { return std::sqrt(GetLengthSq()); }

    Vec3d GetNormalized() const SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        return len > 0 ? Vec3d(x/len, y/len, z/len) : Vec3d();
    }

    double Normalize() SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        if (len > 0) { x /= len; y /= len; z /= len; }
        return len;
    }

    double Dot(const Vec3d& other) const SWIFTUSD_NOEXCEPT {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3d Cross(const Vec3d& r) const SWIFTUSD_NOEXCEPT {
        return Vec3d(
            y * r.z - z * r.y,
            z * r.x - x * r.z,
            x * r.y - y * r.x
        );
    }

    Vec3d GetProjection(const Vec3d& onto) const SWIFTUSD_NOEXCEPT {
        double d = onto.Dot(onto);
        return d > 0 ? onto * (Dot(onto) / d) : Vec3d();
    }

    Vec3d GetComplement(const Vec3d& onto) const SWIFTUSD_NOEXCEPT {
        return *this - GetProjection(onto);
    }

    Vec3d operator+(const Vec3d& r) const SWIFTUSD_NOEXCEPT { return Vec3d(x+r.x, y+r.y, z+r.z); }
    Vec3d operator-(const Vec3d& r) const SWIFTUSD_NOEXCEPT { return Vec3d(x-r.x, y-r.y, z-r.z); }
    Vec3d operator*(double s) const SWIFTUSD_NOEXCEPT { return Vec3d(x*s, y*s, z*s); }
    Vec3d operator/(double s) const SWIFTUSD_NOEXCEPT { return Vec3d(x/s, y/s, z/s); }
    Vec3d operator-() const SWIFTUSD_NOEXCEPT { return Vec3d(-x, -y, -z); }

    Vec3d& operator+=(const Vec3d& r) SWIFTUSD_NOEXCEPT { x+=r.x; y+=r.y; z+=r.z; return *this; }
    Vec3d& operator-=(const Vec3d& r) SWIFTUSD_NOEXCEPT { x-=r.x; y-=r.y; z-=r.z; return *this; }
    Vec3d& operator*=(double s) SWIFTUSD_NOEXCEPT { x*=s; y*=s; z*=s; return *this; }
    Vec3d& operator/=(double s) SWIFTUSD_NOEXCEPT { x/=s; y/=s; z/=s; return *this; }

    bool operator==(const Vec3d& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y && z==r.z; }
    bool operator!=(const Vec3d& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Vec3d operator*(double s, const Vec3d& v) SWIFTUSD_NOEXCEPT { return v * s; }

// ============================================================================
// MARK: - Vec3i (3D integer vector)
// ============================================================================

/// A 3D vector of 32-bit integers.
struct Vec3i {
    int x, y, z;

    Vec3i() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0) {}
    Vec3i(int x_, int y_, int z_) SWIFTUSD_NOEXCEPT : x(x_), y(y_), z(z_) {}
    explicit Vec3i(int scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar), z(scalar) {}

#if defined(USE_PIXAR_USD)
    Vec3i(const PXR_NS::GfVec3i& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]) {}
    PXR_NS::GfVec3i ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec3i(x, y, z); }
#endif

    static Vec3i Zero() SWIFTUSD_NOEXCEPT { return Vec3i(0, 0, 0); }

    int operator[](size_t i) const SWIFTUSD_NOEXCEPT {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    int& operator[](size_t i) SWIFTUSD_NOEXCEPT {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    Vec3i operator+(const Vec3i& r) const SWIFTUSD_NOEXCEPT { return Vec3i(x+r.x, y+r.y, z+r.z); }
    Vec3i operator-(const Vec3i& r) const SWIFTUSD_NOEXCEPT { return Vec3i(x-r.x, y-r.y, z-r.z); }
    Vec3i operator*(int s) const SWIFTUSD_NOEXCEPT { return Vec3i(x*s, y*s, z*s); }
    Vec3i operator-() const SWIFTUSD_NOEXCEPT { return Vec3i(-x, -y, -z); }

    bool operator==(const Vec3i& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y && z==r.z; }
    bool operator!=(const Vec3i& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Vec4f (4D float vector)
// ============================================================================

/// A 4D vector of single-precision floats.
struct Vec4f {
    float x, y, z, w;

    Vec4f() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0), w(0) {}
    Vec4f(float x_, float y_, float z_, float w_) SWIFTUSD_NOEXCEPT : x(x_), y(y_), z(z_), w(w_) {}
    explicit Vec4f(float scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vec4f(const Vec3f& v, float w_) SWIFTUSD_NOEXCEPT : x(v.x), y(v.y), z(v.z), w(w_) {}

#if defined(USE_PIXAR_USD)
    Vec4f(const PXR_NS::GfVec4f& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
    PXR_NS::GfVec4f ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec4f(x, y, z, w); }
#endif

    static Vec4f Zero() SWIFTUSD_NOEXCEPT { return Vec4f(0, 0, 0, 0); }
    static Vec4f XAxis() SWIFTUSD_NOEXCEPT { return Vec4f(1, 0, 0, 0); }
    static Vec4f YAxis() SWIFTUSD_NOEXCEPT { return Vec4f(0, 1, 0, 0); }
    static Vec4f ZAxis() SWIFTUSD_NOEXCEPT { return Vec4f(0, 0, 1, 0); }
    static Vec4f WAxis() SWIFTUSD_NOEXCEPT { return Vec4f(0, 0, 0, 1); }

    float operator[](size_t i) const SWIFTUSD_NOEXCEPT {
        switch(i) { case 0: return x; case 1: return y; case 2: return z; default: return w; }
    }
    float& operator[](size_t i) SWIFTUSD_NOEXCEPT {
        switch(i) { case 0: return x; case 1: return y; case 2: return z; default: return w; }
    }

    /// Extract XYZ components as a Vec3f
    Vec3f GetXYZ() const SWIFTUSD_NOEXCEPT { return Vec3f(x, y, z); }

    float GetLengthSq() const SWIFTUSD_NOEXCEPT { return x*x + y*y + z*z + w*w; }
    float GetLength() const SWIFTUSD_NOEXCEPT { return std::sqrt(GetLengthSq()); }

    Vec4f GetNormalized() const SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        return len > 0 ? Vec4f(x/len, y/len, z/len, w/len) : Vec4f();
    }

    float Normalize() SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        if (len > 0) { x /= len; y /= len; z /= len; w /= len; }
        return len;
    }

    float Dot(const Vec4f& other) const SWIFTUSD_NOEXCEPT {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Vec4f operator+(const Vec4f& r) const SWIFTUSD_NOEXCEPT { return Vec4f(x+r.x, y+r.y, z+r.z, w+r.w); }
    Vec4f operator-(const Vec4f& r) const SWIFTUSD_NOEXCEPT { return Vec4f(x-r.x, y-r.y, z-r.z, w-r.w); }
    Vec4f operator*(float s) const SWIFTUSD_NOEXCEPT { return Vec4f(x*s, y*s, z*s, w*s); }
    Vec4f operator/(float s) const SWIFTUSD_NOEXCEPT { return Vec4f(x/s, y/s, z/s, w/s); }
    Vec4f operator-() const SWIFTUSD_NOEXCEPT { return Vec4f(-x, -y, -z, -w); }

    Vec4f& operator+=(const Vec4f& r) SWIFTUSD_NOEXCEPT { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
    Vec4f& operator-=(const Vec4f& r) SWIFTUSD_NOEXCEPT { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
    Vec4f& operator*=(float s) SWIFTUSD_NOEXCEPT { x*=s; y*=s; z*=s; w*=s; return *this; }
    Vec4f& operator/=(float s) SWIFTUSD_NOEXCEPT { x/=s; y/=s; z/=s; w/=s; return *this; }

    bool operator==(const Vec4f& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y && z==r.z && w==r.w; }
    bool operator!=(const Vec4f& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Vec4f operator*(float s, const Vec4f& v) SWIFTUSD_NOEXCEPT { return v * s; }

// ============================================================================
// MARK: - Vec4d (4D double vector)
// ============================================================================

/// A 4D vector of double-precision floats.
struct Vec4d {
    double x, y, z, w;

    Vec4d() SWIFTUSD_NOEXCEPT : x(0), y(0), z(0), w(0) {}
    Vec4d(double x_, double y_, double z_, double w_) SWIFTUSD_NOEXCEPT : x(x_), y(y_), z(z_), w(w_) {}
    explicit Vec4d(double scalar) SWIFTUSD_NOEXCEPT : x(scalar), y(scalar), z(scalar), w(scalar) {}
    Vec4d(const Vec3d& v, double w_) SWIFTUSD_NOEXCEPT : x(v.x), y(v.y), z(v.z), w(w_) {}
    explicit Vec4d(const Vec4f& v) SWIFTUSD_NOEXCEPT : x(v.x), y(v.y), z(v.z), w(v.w) {}

#if defined(USE_PIXAR_USD)
    Vec4d(const PXR_NS::GfVec4d& v) SWIFTUSD_NOEXCEPT : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
    PXR_NS::GfVec4d ToGf() const SWIFTUSD_NOEXCEPT { return PXR_NS::GfVec4d(x, y, z, w); }
#endif

    static Vec4d Zero() SWIFTUSD_NOEXCEPT { return Vec4d(0, 0, 0, 0); }
    static Vec4d XAxis() SWIFTUSD_NOEXCEPT { return Vec4d(1, 0, 0, 0); }
    static Vec4d YAxis() SWIFTUSD_NOEXCEPT { return Vec4d(0, 1, 0, 0); }
    static Vec4d ZAxis() SWIFTUSD_NOEXCEPT { return Vec4d(0, 0, 1, 0); }
    static Vec4d WAxis() SWIFTUSD_NOEXCEPT { return Vec4d(0, 0, 0, 1); }

    double operator[](size_t i) const SWIFTUSD_NOEXCEPT {
        switch(i) { case 0: return x; case 1: return y; case 2: return z; default: return w; }
    }
    double& operator[](size_t i) SWIFTUSD_NOEXCEPT {
        switch(i) { case 0: return x; case 1: return y; case 2: return z; default: return w; }
    }

    Vec3d GetXYZ() const SWIFTUSD_NOEXCEPT { return Vec3d(x, y, z); }

    double GetLengthSq() const SWIFTUSD_NOEXCEPT { return x*x + y*y + z*z + w*w; }
    double GetLength() const SWIFTUSD_NOEXCEPT { return std::sqrt(GetLengthSq()); }

    Vec4d GetNormalized() const SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        return len > 0 ? Vec4d(x/len, y/len, z/len, w/len) : Vec4d();
    }

    double Normalize() SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        if (len > 0) { x /= len; y /= len; z /= len; w /= len; }
        return len;
    }

    double Dot(const Vec4d& other) const SWIFTUSD_NOEXCEPT {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Vec4d operator+(const Vec4d& r) const SWIFTUSD_NOEXCEPT { return Vec4d(x+r.x, y+r.y, z+r.z, w+r.w); }
    Vec4d operator-(const Vec4d& r) const SWIFTUSD_NOEXCEPT { return Vec4d(x-r.x, y-r.y, z-r.z, w-r.w); }
    Vec4d operator*(double s) const SWIFTUSD_NOEXCEPT { return Vec4d(x*s, y*s, z*s, w*s); }
    Vec4d operator/(double s) const SWIFTUSD_NOEXCEPT { return Vec4d(x/s, y/s, z/s, w/s); }
    Vec4d operator-() const SWIFTUSD_NOEXCEPT { return Vec4d(-x, -y, -z, -w); }

    Vec4d& operator+=(const Vec4d& r) SWIFTUSD_NOEXCEPT { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
    Vec4d& operator-=(const Vec4d& r) SWIFTUSD_NOEXCEPT { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
    Vec4d& operator*=(double s) SWIFTUSD_NOEXCEPT { x*=s; y*=s; z*=s; w*=s; return *this; }
    Vec4d& operator/=(double s) SWIFTUSD_NOEXCEPT { x/=s; y/=s; z/=s; w/=s; return *this; }

    bool operator==(const Vec4d& r) const SWIFTUSD_NOEXCEPT { return x==r.x && y==r.y && z==r.z && w==r.w; }
    bool operator!=(const Vec4d& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Vec4d operator*(double s, const Vec4d& v) SWIFTUSD_NOEXCEPT { return v * s; }

// ============================================================================
// MARK: - Matrix4d (4x4 double matrix)
// ============================================================================

/// A 4x4 matrix of double-precision floats in row-major order.
struct Matrix4d {
    double m[16]; // Row-major: m[row*4 + col]

    // Default constructor creates identity matrix
    Matrix4d() SWIFTUSD_NOEXCEPT {
        SetIdentity();
    }

    // Create from 16 values in row-major order
    Matrix4d(
        double m00, double m01, double m02, double m03,
        double m10, double m11, double m12, double m13,
        double m20, double m21, double m22, double m23,
        double m30, double m31, double m32, double m33
    ) SWIFTUSD_NOEXCEPT {
        m[0]=m00; m[1]=m01; m[2]=m02; m[3]=m03;
        m[4]=m10; m[5]=m11; m[6]=m12; m[7]=m13;
        m[8]=m20; m[9]=m21; m[10]=m22; m[11]=m23;
        m[12]=m30; m[13]=m31; m[14]=m32; m[15]=m33;
    }

    // Create diagonal matrix
    explicit Matrix4d(double diag) SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? diag : 0.0;
    }

#if defined(USE_PIXAR_USD)
    Matrix4d(const PXR_NS::GfMatrix4d& mat) SWIFTUSD_NOEXCEPT {
        const double* data = mat.GetArray();
        for (int i = 0; i < 16; ++i) m[i] = data[i];
    }
    PXR_NS::GfMatrix4d ToGf() const SWIFTUSD_NOEXCEPT {
        return PXR_NS::GfMatrix4d(reinterpret_cast<const double(*)[4]>(m));
    }
#endif

    // Static factories
    static Matrix4d Identity() SWIFTUSD_NOEXCEPT { return Matrix4d(); }

    static Matrix4d Zero() SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        for (int i = 0; i < 16; ++i) result.m[i] = 0.0;
        return result;
    }

    // Element access
    double Get(int row, int col) const SWIFTUSD_NOEXCEPT { return m[row * 4 + col]; }
    void Set(int row, int col, double val) SWIFTUSD_NOEXCEPT { m[row * 4 + col] = val; }

    // Row/column access
    Vec4d GetRow(int row) const SWIFTUSD_NOEXCEPT {
        int i = row * 4;
        return Vec4d(m[i], m[i+1], m[i+2], m[i+3]);
    }

    Vec4d GetColumn(int col) const SWIFTUSD_NOEXCEPT {
        return Vec4d(m[col], m[col+4], m[col+8], m[col+12]);
    }

    void SetRow(int row, const Vec4d& v) SWIFTUSD_NOEXCEPT {
        int i = row * 4;
        m[i] = v.x; m[i+1] = v.y; m[i+2] = v.z; m[i+3] = v.w;
    }

    void SetColumn(int col, const Vec4d& v) SWIFTUSD_NOEXCEPT {
        m[col] = v.x; m[col+4] = v.y; m[col+8] = v.z; m[col+12] = v.w;
    }

    // Set to identity
    void SetIdentity() SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }

    // Set to zero
    void SetZero() SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = 0.0;
    }

    // Transpose
    Matrix4d GetTranspose() const SWIFTUSD_NOEXCEPT {
        return Matrix4d(
            m[0], m[4], m[8], m[12],
            m[1], m[5], m[9], m[13],
            m[2], m[6], m[10], m[14],
            m[3], m[7], m[11], m[15]
        );
    }

    // Determinant (using cofactor expansion)
    double GetDeterminant() const SWIFTUSD_NOEXCEPT {
        double a0 = m[0]*m[5] - m[1]*m[4];
        double a1 = m[0]*m[6] - m[2]*m[4];
        double a2 = m[0]*m[7] - m[3]*m[4];
        double a3 = m[1]*m[6] - m[2]*m[5];
        double a4 = m[1]*m[7] - m[3]*m[5];
        double a5 = m[2]*m[7] - m[3]*m[6];
        double b0 = m[8]*m[13] - m[9]*m[12];
        double b1 = m[8]*m[14] - m[10]*m[12];
        double b2 = m[8]*m[15] - m[11]*m[12];
        double b3 = m[9]*m[14] - m[10]*m[13];
        double b4 = m[9]*m[15] - m[11]*m[13];
        double b5 = m[10]*m[15] - m[11]*m[14];
        return a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
    }

    // Inverse (returns identity if singular)
    Matrix4d GetInverse() const SWIFTUSD_NOEXCEPT {
        double det = GetDeterminant();
        if (std::abs(det) < 1e-14) return Matrix4d::Identity();

        double invDet = 1.0 / det;
        Matrix4d result;

        result.m[0] = invDet * (m[5]*(m[10]*m[15]-m[11]*m[14]) - m[6]*(m[9]*m[15]-m[11]*m[13]) + m[7]*(m[9]*m[14]-m[10]*m[13]));
        result.m[1] = invDet * -(m[1]*(m[10]*m[15]-m[11]*m[14]) - m[2]*(m[9]*m[15]-m[11]*m[13]) + m[3]*(m[9]*m[14]-m[10]*m[13]));
        result.m[2] = invDet * (m[1]*(m[6]*m[15]-m[7]*m[14]) - m[2]*(m[5]*m[15]-m[7]*m[13]) + m[3]*(m[5]*m[14]-m[6]*m[13]));
        result.m[3] = invDet * -(m[1]*(m[6]*m[11]-m[7]*m[10]) - m[2]*(m[5]*m[11]-m[7]*m[9]) + m[3]*(m[5]*m[10]-m[6]*m[9]));

        result.m[4] = invDet * -(m[4]*(m[10]*m[15]-m[11]*m[14]) - m[6]*(m[8]*m[15]-m[11]*m[12]) + m[7]*(m[8]*m[14]-m[10]*m[12]));
        result.m[5] = invDet * (m[0]*(m[10]*m[15]-m[11]*m[14]) - m[2]*(m[8]*m[15]-m[11]*m[12]) + m[3]*(m[8]*m[14]-m[10]*m[12]));
        result.m[6] = invDet * -(m[0]*(m[6]*m[15]-m[7]*m[14]) - m[2]*(m[4]*m[15]-m[7]*m[12]) + m[3]*(m[4]*m[14]-m[6]*m[12]));
        result.m[7] = invDet * (m[0]*(m[6]*m[11]-m[7]*m[10]) - m[2]*(m[4]*m[11]-m[7]*m[8]) + m[3]*(m[4]*m[10]-m[6]*m[8]));

        result.m[8] = invDet * (m[4]*(m[9]*m[15]-m[11]*m[13]) - m[5]*(m[8]*m[15]-m[11]*m[12]) + m[7]*(m[8]*m[13]-m[9]*m[12]));
        result.m[9] = invDet * -(m[0]*(m[9]*m[15]-m[11]*m[13]) - m[1]*(m[8]*m[15]-m[11]*m[12]) + m[3]*(m[8]*m[13]-m[9]*m[12]));
        result.m[10] = invDet * (m[0]*(m[5]*m[15]-m[7]*m[13]) - m[1]*(m[4]*m[15]-m[7]*m[12]) + m[3]*(m[4]*m[13]-m[5]*m[12]));
        result.m[11] = invDet * -(m[0]*(m[5]*m[11]-m[7]*m[9]) - m[1]*(m[4]*m[11]-m[7]*m[8]) + m[3]*(m[4]*m[9]-m[5]*m[8]));

        result.m[12] = invDet * -(m[4]*(m[9]*m[14]-m[10]*m[13]) - m[5]*(m[8]*m[14]-m[10]*m[12]) + m[6]*(m[8]*m[13]-m[9]*m[12]));
        result.m[13] = invDet * (m[0]*(m[9]*m[14]-m[10]*m[13]) - m[1]*(m[8]*m[14]-m[10]*m[12]) + m[2]*(m[8]*m[13]-m[9]*m[12]));
        result.m[14] = invDet * -(m[0]*(m[5]*m[14]-m[6]*m[13]) - m[1]*(m[4]*m[14]-m[6]*m[12]) + m[2]*(m[4]*m[13]-m[5]*m[12]));
        result.m[15] = invDet * (m[0]*(m[5]*m[10]-m[6]*m[9]) - m[1]*(m[4]*m[10]-m[6]*m[8]) + m[2]*(m[4]*m[9]-m[5]*m[8]));

        return result;
    }

    // Extract translation from matrix (row 3)
    Vec3d ExtractTranslation() const SWIFTUSD_NOEXCEPT {
        return Vec3d(m[12], m[13], m[14]);
    }

    // Transform a point (with translation)
    Vec3d TransformPoint(const Vec3d& p) const SWIFTUSD_NOEXCEPT {
        double w = m[3]*p.x + m[7]*p.y + m[11]*p.z + m[15];
        if (std::abs(w) < 1e-14) w = 1.0;
        return Vec3d(
            (m[0]*p.x + m[4]*p.y + m[8]*p.z + m[12]) / w,
            (m[1]*p.x + m[5]*p.y + m[9]*p.z + m[13]) / w,
            (m[2]*p.x + m[6]*p.y + m[10]*p.z + m[14]) / w
        );
    }

    // Transform a direction (without translation)
    Vec3d TransformDir(const Vec3d& d) const SWIFTUSD_NOEXCEPT {
        return Vec3d(
            m[0]*d.x + m[4]*d.y + m[8]*d.z,
            m[1]*d.x + m[5]*d.y + m[9]*d.z,
            m[2]*d.x + m[6]*d.y + m[10]*d.z
        );
    }

    // Affine transform (assumes w=1, no perspective division)
    Vec3d TransformAffine(const Vec3d& p) const SWIFTUSD_NOEXCEPT {
        return Vec3d(
            m[0]*p.x + m[4]*p.y + m[8]*p.z + m[12],
            m[1]*p.x + m[5]*p.y + m[9]*p.z + m[13],
            m[2]*p.x + m[6]*p.y + m[10]*p.z + m[14]
        );
    }

    // Create translation matrix
    static Matrix4d SetTranslate(const Vec3d& t) SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        result.m[12] = t.x; result.m[13] = t.y; result.m[14] = t.z;
        return result;
    }

    static Matrix4d SetTranslate(double x, double y, double z) SWIFTUSD_NOEXCEPT {
        return SetTranslate(Vec3d(x, y, z));
    }

    // Create uniform scale matrix
    static Matrix4d SetScale(double s) SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        result.m[0] = s; result.m[5] = s; result.m[10] = s;
        return result;
    }

    // Create non-uniform scale matrix
    static Matrix4d SetScale(const Vec3d& s) SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        result.m[0] = s.x; result.m[5] = s.y; result.m[10] = s.z;
        return result;
    }

    static Matrix4d SetScale(double x, double y, double z) SWIFTUSD_NOEXCEPT {
        return SetScale(Vec3d(x, y, z));
    }

    // Matrix multiplication
    Matrix4d operator*(const Matrix4d& r) const SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row*4+col] =
                    m[row*4+0]*r.m[0+col] +
                    m[row*4+1]*r.m[4+col] +
                    m[row*4+2]*r.m[8+col] +
                    m[row*4+3]*r.m[12+col];
            }
        }
        return result;
    }

    // Scalar multiplication
    Matrix4d operator*(double s) const SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        for (int i = 0; i < 16; ++i) result.m[i] = m[i] * s;
        return result;
    }

    // Matrix addition/subtraction
    Matrix4d operator+(const Matrix4d& r) const SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        for (int i = 0; i < 16; ++i) result.m[i] = m[i] + r.m[i];
        return result;
    }

    Matrix4d operator-(const Matrix4d& r) const SWIFTUSD_NOEXCEPT {
        Matrix4d result;
        for (int i = 0; i < 16; ++i) result.m[i] = m[i] - r.m[i];
        return result;
    }

    Matrix4d& operator*=(const Matrix4d& r) SWIFTUSD_NOEXCEPT {
        *this = *this * r;
        return *this;
    }

    Matrix4d& operator*=(double s) SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] *= s;
        return *this;
    }

    bool operator==(const Matrix4d& r) const SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) if (m[i] != r.m[i]) return false;
        return true;
    }

    bool operator!=(const Matrix4d& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Matrix4d operator*(double s, const Matrix4d& m) SWIFTUSD_NOEXCEPT { return m * s; }

// ============================================================================
// MARK: - Matrix4f (4x4 float matrix)
// ============================================================================

/// A 4x4 matrix of single-precision floats in row-major order.
struct Matrix4f {
    float m[16];

    Matrix4f() SWIFTUSD_NOEXCEPT {
        SetIdentity();
    }

    Matrix4f(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ) SWIFTUSD_NOEXCEPT {
        m[0]=m00; m[1]=m01; m[2]=m02; m[3]=m03;
        m[4]=m10; m[5]=m11; m[6]=m12; m[7]=m13;
        m[8]=m20; m[9]=m21; m[10]=m22; m[11]=m23;
        m[12]=m30; m[13]=m31; m[14]=m32; m[15]=m33;
    }

    explicit Matrix4f(float diag) SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? diag : 0.0f;
    }

    explicit Matrix4f(const Matrix4d& mat) SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = static_cast<float>(mat.m[i]);
    }

#if defined(USE_PIXAR_USD)
    Matrix4f(const PXR_NS::GfMatrix4f& mat) SWIFTUSD_NOEXCEPT {
        const float* data = mat.GetArray();
        for (int i = 0; i < 16; ++i) m[i] = data[i];
    }
    PXR_NS::GfMatrix4f ToGf() const SWIFTUSD_NOEXCEPT {
        return PXR_NS::GfMatrix4f(reinterpret_cast<const float(*)[4]>(m));
    }
#endif

    static Matrix4f Identity() SWIFTUSD_NOEXCEPT { return Matrix4f(); }
    static Matrix4f Zero() SWIFTUSD_NOEXCEPT {
        Matrix4f result;
        for (int i = 0; i < 16; ++i) result.m[i] = 0.0f;
        return result;
    }

    float Get(int row, int col) const SWIFTUSD_NOEXCEPT { return m[row * 4 + col]; }
    void Set(int row, int col, float val) SWIFTUSD_NOEXCEPT { m[row * 4 + col] = val; }

    void SetIdentity() SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    void SetZero() SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    }

    Matrix4f GetTranspose() const SWIFTUSD_NOEXCEPT {
        return Matrix4f(
            m[0], m[4], m[8], m[12],
            m[1], m[5], m[9], m[13],
            m[2], m[6], m[10], m[14],
            m[3], m[7], m[11], m[15]
        );
    }

    Vec3f ExtractTranslation() const SWIFTUSD_NOEXCEPT {
        return Vec3f(m[12], m[13], m[14]);
    }

    Matrix4f operator*(const Matrix4f& r) const SWIFTUSD_NOEXCEPT {
        Matrix4f result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row*4+col] =
                    m[row*4+0]*r.m[0+col] +
                    m[row*4+1]*r.m[4+col] +
                    m[row*4+2]*r.m[8+col] +
                    m[row*4+3]*r.m[12+col];
            }
        }
        return result;
    }

    bool operator==(const Matrix4f& r) const SWIFTUSD_NOEXCEPT {
        for (int i = 0; i < 16; ++i) if (m[i] != r.m[i]) return false;
        return true;
    }

    bool operator!=(const Matrix4f& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Quatd (double quaternion)
// ============================================================================

/// A quaternion with double-precision components (w, x, y, z).
/// Stored as (real, i, j, k) where the quaternion is real + i*i + j*j + k*k.
struct Quatd {
    double real; // w component (real/scalar part)
    double i, j, k; // xyz imaginary components

    // Default is identity quaternion (1, 0, 0, 0)
    Quatd() SWIFTUSD_NOEXCEPT : real(1), i(0), j(0), k(0) {}

    Quatd(double real_, double i_, double j_, double k_) SWIFTUSD_NOEXCEPT
        : real(real_), i(i_), j(j_), k(k_) {}

    // Create from real value only
    explicit Quatd(double real_) SWIFTUSD_NOEXCEPT : real(real_), i(0), j(0), k(0) {}

    // Create from real and imaginary Vec3d
    Quatd(double real_, const Vec3d& imag) SWIFTUSD_NOEXCEPT
        : real(real_), i(imag.x), j(imag.y), k(imag.z) {}

#if defined(USE_PIXAR_USD)
    Quatd(const PXR_NS::GfQuatd& q) SWIFTUSD_NOEXCEPT
        : real(q.GetReal()),
          i(q.GetImaginary()[0]),
          j(q.GetImaginary()[1]),
          k(q.GetImaginary()[2]) {}

    PXR_NS::GfQuatd ToGf() const SWIFTUSD_NOEXCEPT {
        return PXR_NS::GfQuatd(real, PXR_NS::GfVec3d(i, j, k));
    }
#endif

    static Quatd Identity() SWIFTUSD_NOEXCEPT { return Quatd(); }
    static Quatd Zero() SWIFTUSD_NOEXCEPT { return Quatd(0, 0, 0, 0); }

    // Get imaginary part as Vec3d
    Vec3d GetImaginary() const SWIFTUSD_NOEXCEPT { return Vec3d(i, j, k); }
    double GetReal() const SWIFTUSD_NOEXCEPT { return real; }

    void SetReal(double r) SWIFTUSD_NOEXCEPT { real = r; }
    void SetImaginary(const Vec3d& v) SWIFTUSD_NOEXCEPT { i = v.x; j = v.y; k = v.z; }
    void SetImaginary(double i_, double j_, double k_) SWIFTUSD_NOEXCEPT { i = i_; j = j_; k = k_; }

    double GetLength() const SWIFTUSD_NOEXCEPT {
        return std::sqrt(real*real + i*i + j*j + k*k);
    }

    Quatd GetNormalized() const SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        return len > 0 ? Quatd(real/len, i/len, j/len, k/len) : Quatd::Identity();
    }

    double Normalize() SWIFTUSD_NOEXCEPT {
        double len = GetLength();
        if (len > 0) { real /= len; i /= len; j /= len; k /= len; }
        return len;
    }

    // Conjugate: (w, -x, -y, -z)
    Quatd GetConjugate() const SWIFTUSD_NOEXCEPT {
        return Quatd(real, -i, -j, -k);
    }

    // Inverse: conjugate / length^2
    Quatd GetInverse() const SWIFTUSD_NOEXCEPT {
        double lenSq = real*real + i*i + j*j + k*k;
        if (lenSq < 1e-14) return Quatd::Identity();
        double invLen = 1.0 / lenSq;
        return Quatd(real * invLen, -i * invLen, -j * invLen, -k * invLen);
    }

    // Dot product
    double Dot(const Quatd& other) const SWIFTUSD_NOEXCEPT {
        return real * other.real + i * other.i + j * other.j + k * other.k;
    }

    // Quaternion multiplication (Hamilton product)
    Quatd operator*(const Quatd& r) const SWIFTUSD_NOEXCEPT {
        return Quatd(
            real*r.real - i*r.i - j*r.j - k*r.k,
            real*r.i + i*r.real + j*r.k - k*r.j,
            real*r.j - i*r.k + j*r.real + k*r.i,
            real*r.k + i*r.j - j*r.i + k*r.real
        );
    }

    Quatd operator*(double s) const SWIFTUSD_NOEXCEPT {
        return Quatd(real*s, i*s, j*s, k*s);
    }

    Quatd operator+(const Quatd& r) const SWIFTUSD_NOEXCEPT {
        return Quatd(real+r.real, i+r.i, j+r.j, k+r.k);
    }

    Quatd operator-(const Quatd& r) const SWIFTUSD_NOEXCEPT {
        return Quatd(real-r.real, i-r.i, j-r.j, k-r.k);
    }

    Quatd operator-() const SWIFTUSD_NOEXCEPT {
        return Quatd(-real, -i, -j, -k);
    }

    Quatd& operator*=(const Quatd& r) SWIFTUSD_NOEXCEPT { *this = *this * r; return *this; }
    Quatd& operator*=(double s) SWIFTUSD_NOEXCEPT { real*=s; i*=s; j*=s; k*=s; return *this; }
    Quatd& operator+=(const Quatd& r) SWIFTUSD_NOEXCEPT { real+=r.real; i+=r.i; j+=r.j; k+=r.k; return *this; }
    Quatd& operator-=(const Quatd& r) SWIFTUSD_NOEXCEPT { real-=r.real; i-=r.i; j-=r.j; k-=r.k; return *this; }

    // Rotate a vector by this quaternion
    Vec3d Transform(const Vec3d& v) const SWIFTUSD_NOEXCEPT {
        // q * v * q^-1, optimized
        Vec3d qv(i, j, k);
        Vec3d t = qv.Cross(v) * 2.0;
        return v + t * real + qv.Cross(t);
    }

    // Spherical linear interpolation
    static Quatd Slerp(const Quatd& q0, const Quatd& q1, double t) SWIFTUSD_NOEXCEPT {
        double dot = q0.Dot(q1);

        // If dot is negative, negate one quaternion to take shorter path
        Quatd q1adj = dot < 0 ? -q1 : q1;
        dot = std::abs(dot);

        // If quaternions are very close, use linear interpolation
        if (dot > 0.9995) {
            Quatd result = q0 + (q1adj - q0) * t;
            return result.GetNormalized();
        }

        // Standard slerp
        double theta = std::acos(dot);
        double sinTheta = std::sin(theta);
        double w0 = std::sin((1.0 - t) * theta) / sinTheta;
        double w1 = std::sin(t * theta) / sinTheta;

        return q0 * w0 + q1adj * w1;
    }

    // Create rotation quaternion from axis and angle (radians)
    static Quatd FromAxisAngle(const Vec3d& axis, double angleRadians) SWIFTUSD_NOEXCEPT {
        Vec3d normAxis = axis.GetNormalized();
        double halfAngle = angleRadians * 0.5;
        double s = std::sin(halfAngle);
        return Quatd(std::cos(halfAngle), normAxis.x * s, normAxis.y * s, normAxis.z * s);
    }

    bool operator==(const Quatd& r) const SWIFTUSD_NOEXCEPT {
        return real==r.real && i==r.i && j==r.j && k==r.k;
    }
    bool operator!=(const Quatd& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

inline Quatd operator*(double s, const Quatd& q) SWIFTUSD_NOEXCEPT { return q * s; }

// ============================================================================
// MARK: - Quatf (float quaternion)
// ============================================================================

/// A quaternion with single-precision components.
struct Quatf {
    float real;
    float i, j, k;

    Quatf() SWIFTUSD_NOEXCEPT : real(1), i(0), j(0), k(0) {}
    Quatf(float real_, float i_, float j_, float k_) SWIFTUSD_NOEXCEPT
        : real(real_), i(i_), j(j_), k(k_) {}
    explicit Quatf(float real_) SWIFTUSD_NOEXCEPT : real(real_), i(0), j(0), k(0) {}
    explicit Quatf(const Quatd& q) SWIFTUSD_NOEXCEPT
        : real(static_cast<float>(q.real)),
          i(static_cast<float>(q.i)),
          j(static_cast<float>(q.j)),
          k(static_cast<float>(q.k)) {}

#if defined(USE_PIXAR_USD)
    Quatf(const PXR_NS::GfQuatf& q) SWIFTUSD_NOEXCEPT
        : real(q.GetReal()),
          i(q.GetImaginary()[0]),
          j(q.GetImaginary()[1]),
          k(q.GetImaginary()[2]) {}

    PXR_NS::GfQuatf ToGf() const SWIFTUSD_NOEXCEPT {
        return PXR_NS::GfQuatf(real, PXR_NS::GfVec3f(i, j, k));
    }
#endif

    static Quatf Identity() SWIFTUSD_NOEXCEPT { return Quatf(); }
    static Quatf Zero() SWIFTUSD_NOEXCEPT { return Quatf(0, 0, 0, 0); }

    Vec3f GetImaginary() const SWIFTUSD_NOEXCEPT { return Vec3f(i, j, k); }
    float GetReal() const SWIFTUSD_NOEXCEPT { return real; }

    float GetLength() const SWIFTUSD_NOEXCEPT {
        return std::sqrt(real*real + i*i + j*j + k*k);
    }

    Quatf GetNormalized() const SWIFTUSD_NOEXCEPT {
        float len = GetLength();
        return len > 0 ? Quatf(real/len, i/len, j/len, k/len) : Quatf::Identity();
    }

    Quatf GetConjugate() const SWIFTUSD_NOEXCEPT {
        return Quatf(real, -i, -j, -k);
    }

    Quatf operator*(const Quatf& r) const SWIFTUSD_NOEXCEPT {
        return Quatf(
            real*r.real - i*r.i - j*r.j - k*r.k,
            real*r.i + i*r.real + j*r.k - k*r.j,
            real*r.j - i*r.k + j*r.real + k*r.i,
            real*r.k + i*r.j - j*r.i + k*r.real
        );
    }

    bool operator==(const Quatf& r) const SWIFTUSD_NOEXCEPT {
        return real==r.real && i==r.i && j==r.j && k==r.k;
    }
    bool operator!=(const Quatf& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Range3d (3D axis-aligned range)
// ============================================================================

/// A 3D axis-aligned range (interval) with double-precision endpoints.
struct Range3d {
    Vec3d min;
    Vec3d max;

    // Default creates empty range (max < min)
    Range3d() SWIFTUSD_NOEXCEPT
        : min(std::numeric_limits<double>::max(),
              std::numeric_limits<double>::max(),
              std::numeric_limits<double>::max()),
          max(std::numeric_limits<double>::lowest(),
              std::numeric_limits<double>::lowest(),
              std::numeric_limits<double>::lowest()) {}

    Range3d(const Vec3d& min_, const Vec3d& max_) SWIFTUSD_NOEXCEPT
        : min(min_), max(max_) {}

#if defined(USE_PIXAR_USD)
    Range3d(const PXR_NS::GfRange3d& r) SWIFTUSD_NOEXCEPT
        : min(r.GetMin()), max(r.GetMax()) {}

    PXR_NS::GfRange3d ToGf() const SWIFTUSD_NOEXCEPT {
        return PXR_NS::GfRange3d(min.ToGf(), max.ToGf());
    }
#endif

    static Range3d Empty() SWIFTUSD_NOEXCEPT { return Range3d(); }

    static Range3d UnitCube() SWIFTUSD_NOEXCEPT {
        return Range3d(Vec3d(0, 0, 0), Vec3d(1, 1, 1));
    }

    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
        return min.x > max.x || min.y > max.y || min.z > max.z;
    }

    void SetEmpty() SWIFTUSD_NOEXCEPT {
        min = Vec3d(std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::max());
        max = Vec3d(std::numeric_limits<double>::lowest(),
                    std::numeric_limits<double>::lowest(),
                    std::numeric_limits<double>::lowest());
    }

    Vec3d GetMin() const SWIFTUSD_NOEXCEPT { return min; }
    Vec3d GetMax() const SWIFTUSD_NOEXCEPT { return max; }

    void SetMin(const Vec3d& v) SWIFTUSD_NOEXCEPT { min = v; }
    void SetMax(const Vec3d& v) SWIFTUSD_NOEXCEPT { max = v; }

    Vec3d GetSize() const SWIFTUSD_NOEXCEPT { return max - min; }
    Vec3d GetMidpoint() const SWIFTUSD_NOEXCEPT { return (min + max) * 0.5; }

    // Check if point is inside range
    bool Contains(const Vec3d& p) const SWIFTUSD_NOEXCEPT {
        return p.x >= min.x && p.x <= max.x &&
               p.y >= min.y && p.y <= max.y &&
               p.z >= min.z && p.z <= max.z;
    }

    // Check if other range is entirely inside this range
    bool Contains(const Range3d& other) const SWIFTUSD_NOEXCEPT {
        return Contains(other.min) && Contains(other.max);
    }

    // Check if ranges do not overlap
    bool IsOutside(const Range3d& other) const SWIFTUSD_NOEXCEPT {
        return min.x > other.max.x || other.min.x > max.x ||
               min.y > other.max.y || other.min.y > max.y ||
               min.z > other.max.z || other.min.z > max.z;
    }

    // Extend range to include point
    void UnionWith(const Vec3d& p) SWIFTUSD_NOEXCEPT {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        min.z = std::min(min.z, p.z);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        max.z = std::max(max.z, p.z);
    }

    // Extend range to include other range
    void UnionWith(const Range3d& other) SWIFTUSD_NOEXCEPT {
        if (!other.IsEmpty()) {
            UnionWith(other.min);
            UnionWith(other.max);
        }
    }

    // Return union of two ranges
    static Range3d GetUnion(const Range3d& a, const Range3d& b) SWIFTUSD_NOEXCEPT {
        Range3d result = a;
        result.UnionWith(b);
        return result;
    }

    // Return intersection of two ranges
    static Range3d GetIntersection(const Range3d& a, const Range3d& b) SWIFTUSD_NOEXCEPT {
        if (a.IsOutside(b)) return Range3d::Empty();
        return Range3d(
            Vec3d(std::max(a.min.x, b.min.x),
                  std::max(a.min.y, b.min.y),
                  std::max(a.min.z, b.min.z)),
            Vec3d(std::min(a.max.x, b.max.x),
                  std::min(a.max.y, b.max.y),
                  std::min(a.max.z, b.max.z))
        );
    }

    // Squared distance from point to range (0 if inside)
    double GetDistanceSquared(const Vec3d& p) const SWIFTUSD_NOEXCEPT {
        double dx = std::max(0.0, std::max(min.x - p.x, p.x - max.x));
        double dy = std::max(0.0, std::max(min.y - p.y, p.y - max.y));
        double dz = std::max(0.0, std::max(min.z - p.z, p.z - max.z));
        return dx*dx + dy*dy + dz*dz;
    }

    // Get corner by index (0-7)
    Vec3d GetCorner(int i) const SWIFTUSD_NOEXCEPT {
        return Vec3d(
            (i & 1) ? max.x : min.x,
            (i & 2) ? max.y : min.y,
            (i & 4) ? max.z : min.z
        );
    }

    bool operator==(const Range3d& r) const SWIFTUSD_NOEXCEPT {
        return min == r.min && max == r.max;
    }
    bool operator!=(const Range3d& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Range3f (3D axis-aligned range, float)
// ============================================================================

/// A 3D axis-aligned range with single-precision endpoints.
struct Range3f {
    Vec3f min;
    Vec3f max;

    Range3f() SWIFTUSD_NOEXCEPT
        : min(std::numeric_limits<float>::max(),
              std::numeric_limits<float>::max(),
              std::numeric_limits<float>::max()),
          max(std::numeric_limits<float>::lowest(),
              std::numeric_limits<float>::lowest(),
              std::numeric_limits<float>::lowest()) {}

    Range3f(const Vec3f& min_, const Vec3f& max_) SWIFTUSD_NOEXCEPT
        : min(min_), max(max_) {}

    explicit Range3f(const Range3d& r) SWIFTUSD_NOEXCEPT
        : min(static_cast<float>(r.min.x), static_cast<float>(r.min.y), static_cast<float>(r.min.z)),
          max(static_cast<float>(r.max.x), static_cast<float>(r.max.y), static_cast<float>(r.max.z)) {}

#if defined(USE_PIXAR_USD)
    Range3f(const PXR_NS::GfRange3f& r) SWIFTUSD_NOEXCEPT
        : min(r.GetMin()), max(r.GetMax()) {}

    PXR_NS::GfRange3f ToGf() const SWIFTUSD_NOEXCEPT {
        return PXR_NS::GfRange3f(min.ToGf(), max.ToGf());
    }
#endif

    static Range3f Empty() SWIFTUSD_NOEXCEPT { return Range3f(); }
    static Range3f UnitCube() SWIFTUSD_NOEXCEPT {
        return Range3f(Vec3f(0, 0, 0), Vec3f(1, 1, 1));
    }

    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
        return min.x > max.x || min.y > max.y || min.z > max.z;
    }

    Vec3f GetSize() const SWIFTUSD_NOEXCEPT { return max - min; }
    Vec3f GetMidpoint() const SWIFTUSD_NOEXCEPT { return (min + max) * 0.5f; }

    bool Contains(const Vec3f& p) const SWIFTUSD_NOEXCEPT {
        return p.x >= min.x && p.x <= max.x &&
               p.y >= min.y && p.y <= max.y &&
               p.z >= min.z && p.z <= max.z;
    }

    void UnionWith(const Vec3f& p) SWIFTUSD_NOEXCEPT {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        min.z = std::min(min.z, p.z);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        max.z = std::max(max.z, p.z);
    }

    bool operator==(const Range3f& r) const SWIFTUSD_NOEXCEPT {
        return min == r.min && max == r.max;
    }
    bool operator!=(const Range3f& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - BBox3d (bounding box with transform)
// ============================================================================

/// A 3D bounding box with an associated transformation matrix.
struct BBox3d {
    Range3d range;
    Matrix4d matrix;
    bool hasZeroAreaPrimitives;

    BBox3d() SWIFTUSD_NOEXCEPT
        : range(), matrix(), hasZeroAreaPrimitives(false) {}

    explicit BBox3d(const Range3d& r) SWIFTUSD_NOEXCEPT
        : range(r), matrix(), hasZeroAreaPrimitives(false) {}

    BBox3d(const Range3d& r, const Matrix4d& m) SWIFTUSD_NOEXCEPT
        : range(r), matrix(m), hasZeroAreaPrimitives(false) {}

#if defined(USE_PIXAR_USD)
    BBox3d(const PXR_NS::GfBBox3d& bbox) SWIFTUSD_NOEXCEPT
        : range(bbox.GetRange()),
          matrix(bbox.GetMatrix()),
          hasZeroAreaPrimitives(bbox.HasZeroAreaPrimitives()) {}

    PXR_NS::GfBBox3d ToGf() const SWIFTUSD_NOEXCEPT {
        PXR_NS::GfBBox3d result(range.ToGf(), matrix.ToGf());
        result.SetHasZeroAreaPrimitives(hasZeroAreaPrimitives);
        return result;
    }
#endif

    Range3d GetRange() const SWIFTUSD_NOEXCEPT { return range; }
    Matrix4d GetMatrix() const SWIFTUSD_NOEXCEPT { return matrix; }
    Matrix4d GetInverseMatrix() const SWIFTUSD_NOEXCEPT { return matrix.GetInverse(); }
    bool HasZeroAreaPrimitives() const SWIFTUSD_NOEXCEPT { return hasZeroAreaPrimitives; }

    void SetRange(const Range3d& r) SWIFTUSD_NOEXCEPT { range = r; }
    void SetMatrix(const Matrix4d& m) SWIFTUSD_NOEXCEPT { matrix = m; }
    void SetHasZeroAreaPrimitives(bool v) SWIFTUSD_NOEXCEPT { hasZeroAreaPrimitives = v; }

    // Compute volume
    double GetVolume() const SWIFTUSD_NOEXCEPT {
        if (range.IsEmpty()) return 0.0;
        Vec3d size = range.GetSize();
        double det = std::abs(matrix.GetDeterminant());
        return size.x * size.y * size.z * det;
    }

    // Apply additional transform
    void Transform(const Matrix4d& m) SWIFTUSD_NOEXCEPT {
        matrix = matrix * m;
    }

    // Compute axis-aligned range by transforming corners
    Range3d ComputeAlignedRange() const SWIFTUSD_NOEXCEPT {
        if (range.IsEmpty()) return Range3d::Empty();

        Range3d result;
        for (int i = 0; i < 8; ++i) {
            Vec3d corner = range.GetCorner(i);
            Vec3d transformed = matrix.TransformAffine(corner);
            result.UnionWith(transformed);
        }
        return result;
    }

    // Compute centroid (transformed midpoint)
    Vec3d ComputeCentroid() const SWIFTUSD_NOEXCEPT {
        return matrix.TransformAffine(range.GetMidpoint());
    }

    // Combine two bounding boxes
    static BBox3d Combine(const BBox3d& b1, const BBox3d& b2) SWIFTUSD_NOEXCEPT {
        // Simple approach: compute aligned ranges and combine
        Range3d r1 = b1.ComputeAlignedRange();
        Range3d r2 = b2.ComputeAlignedRange();
        Range3d combined = Range3d::GetUnion(r1, r2);

        BBox3d result(combined);
        result.hasZeroAreaPrimitives = b1.hasZeroAreaPrimitives || b2.hasZeroAreaPrimitives;
        return result;
    }

    bool operator==(const BBox3d& r) const SWIFTUSD_NOEXCEPT {
        return range == r.range && matrix == r.matrix &&
               hasZeroAreaPrimitives == r.hasZeroAreaPrimitives;
    }
    bool operator!=(const BBox3d& r) const SWIFTUSD_NOEXCEPT { return !(*this == r); }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Linearly interpolate between two values
inline double Lerp(double a, double b, double t) SWIFTUSD_NOEXCEPT {
    return a + t * (b - a);
}

/// Clamp a value to a range
inline double Clamp(double value, double min, double max) SWIFTUSD_NOEXCEPT {
    return value < min ? min : (value > max ? max : value);
}

/// Convert degrees to radians
inline double DegreesToRadians(double degrees) SWIFTUSD_NOEXCEPT {
    return degrees * 0.017453292519943295; // pi / 180
}

/// Convert radians to degrees
inline double RadiansToDegrees(double radians) SWIFTUSD_NOEXCEPT {
    return radians * 57.29577951308232; // 180 / pi
}

/// Check if two doubles are approximately equal
inline bool IsClose(double a, double b, double tolerance = 1e-10) SWIFTUSD_NOEXCEPT {
    return std::abs(a - b) <= tolerance;
}

} // namespace swiftusd
