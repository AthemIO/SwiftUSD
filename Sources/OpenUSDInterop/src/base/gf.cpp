// gf.cpp - Gf (Graphics Foundations) C wrapper implementation
// Mirrors: pxr/base/gf/

#include "../../include/base/gf.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define GF_USE_USD 1
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/gf/vec3d.h>
    #include <pxr/base/gf/matrix4d.h>
    #include <pxr/base/gf/quatd.h>
    #include <pxr/base/gf/range3d.h>
    #include <pxr/base/gf/bbox3d.h>
    #include <pxr/base/tf/hash.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define GF_USE_USD 0
#endif

#include <atomic>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cfloat>

// ============================================================================
// MARK: - GfVec3f Implementation
// ============================================================================

struct GfVec3fOpaque {
#if GF_USE_USD
    GfVec3f vec;
#else
    float data[3];
#endif
    std::atomic<int> refCount;

    GfVec3fOpaque() : refCount(1) {
#if !GF_USE_USD
        data[0] = data[1] = data[2] = 0.0f;
#endif
    }

    GfVec3fOpaque(float x, float y, float z) : refCount(1) {
#if GF_USE_USD
        vec = GfVec3f(x, y, z);
#else
        data[0] = x; data[1] = y; data[2] = z;
#endif
    }

#if GF_USE_USD
    GfVec3fOpaque(const GfVec3f& v) : vec(v), refCount(1) {}
#endif

    float getX() const {
#if GF_USE_USD
        return vec[0];
#else
        return data[0];
#endif
    }

    float getY() const {
#if GF_USE_USD
        return vec[1];
#else
        return data[1];
#endif
    }

    float getZ() const {
#if GF_USE_USD
        return vec[2];
#else
        return data[2];
#endif
    }

    void set(float x, float y, float z) {
#if GF_USE_USD
        vec.Set(x, y, z);
#else
        data[0] = x; data[1] = y; data[2] = z;
#endif
    }

    float getLength() const {
#if GF_USE_USD
        return vec.GetLength();
#else
        return std::sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
#endif
    }

    float getLengthSq() const {
#if GF_USE_USD
        return vec.GetLengthSq();
#else
        return data[0]*data[0] + data[1]*data[1] + data[2]*data[2];
#endif
    }

    float normalize() {
#if GF_USE_USD
        return vec.Normalize();
#else
        float length = getLength();
        if (length > 1e-10f) {
            data[0] /= length;
            data[1] /= length;
            data[2] /= length;
        }
        return length;
#endif
    }

    size_t hash() const {
#if GF_USE_USD
        return hash_value(vec);
#else
        size_t h = 0;
        h ^= std::hash<float>{}(data[0]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<float>{}(data[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<float>{}(data[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
#endif
    }
};

extern "C" {

GfVec3fRef GfVec3f_Create(float x, float y, float z) {
    try {
        return new GfVec3fOpaque(x, y, z);
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_CreateScalar(float value) {
    return GfVec3f_Create(value, value, value);
}

GfVec3fRef GfVec3f_CreateZero(void) {
    return GfVec3f_Create(0.0f, 0.0f, 0.0f);
}

GfVec3fRef GfVec3f_Copy(GfVec3fRef vec) {
    if (!vec) return nullptr;
    try {
        return new GfVec3fOpaque(vec->getX(), vec->getY(), vec->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_Retain(GfVec3fRef vec) {
    if (!vec) return nullptr;
    vec->refCount.fetch_add(1, std::memory_order_relaxed);
    return vec;
}

void GfVec3f_Release(GfVec3fRef vec) {
    if (!vec) return;
    if (vec->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete vec;
    }
}

float GfVec3f_GetX(GfVec3fRef vec) {
    if (!vec) return 0.0f;
    return vec->getX();
}

float GfVec3f_GetY(GfVec3fRef vec) {
    if (!vec) return 0.0f;
    return vec->getY();
}

float GfVec3f_GetZ(GfVec3fRef vec) {
    if (!vec) return 0.0f;
    return vec->getZ();
}

void GfVec3f_GetComponents(GfVec3fRef vec, float* outX, float* outY, float* outZ) {
    if (!vec || !outX || !outY || !outZ) return;
    *outX = vec->getX();
    *outY = vec->getY();
    *outZ = vec->getZ();
}

void GfVec3f_Set(GfVec3fRef vec, float x, float y, float z) {
    if (!vec) return;
    vec->set(x, y, z);
}

float GfVec3f_GetLength(GfVec3fRef vec) {
    if (!vec) return 0.0f;
    return vec->getLength();
}

float GfVec3f_GetLengthSq(GfVec3fRef vec) {
    if (!vec) return 0.0f;
    return vec->getLengthSq();
}

size_t GfVec3f_Hash(GfVec3fRef vec) {
    if (!vec) return 0;
    return vec->hash();
}

GfVec3fRef GfVec3f_GetNormalized(GfVec3fRef vec) {
    if (!vec) return nullptr;
    try {
        auto* result = new GfVec3fOpaque(vec->getX(), vec->getY(), vec->getZ());
        result->normalize();
        return result;
    } catch (...) {
        return nullptr;
    }
}

float GfVec3f_Normalize(GfVec3fRef vec) {
    if (!vec) return 0.0f;
    return vec->normalize();
}

float GfVec3f_Dot(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return 0.0f;
#if GF_USE_USD
    return GfDot(v1->vec, v2->vec);
#else
    return v1->data[0]*v2->data[0] + v1->data[1]*v2->data[1] + v1->data[2]*v2->data[2];
#endif
}

GfVec3fRef GfVec3f_Cross(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
#if GF_USE_USD
        return new GfVec3fOpaque(GfCross(v1->vec, v2->vec));
#else
        float x = v1->data[1]*v2->data[2] - v1->data[2]*v2->data[1];
        float y = v1->data[2]*v2->data[0] - v1->data[0]*v2->data[2];
        float z = v1->data[0]*v2->data[1] - v1->data[1]*v2->data[0];
        return new GfVec3fOpaque(x, y, z);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_Add(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
        return new GfVec3fOpaque(
            v1->getX() + v2->getX(),
            v1->getY() + v2->getY(),
            v1->getZ() + v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_Subtract(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
        return new GfVec3fOpaque(
            v1->getX() - v2->getX(),
            v1->getY() - v2->getY(),
            v1->getZ() - v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_Multiply(GfVec3fRef vec, float scalar) {
    if (!vec) return nullptr;
    try {
        return new GfVec3fOpaque(
            vec->getX() * scalar,
            vec->getY() * scalar,
            vec->getZ() * scalar);
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_Divide(GfVec3fRef vec, float scalar) {
    if (!vec || scalar == 0.0f) return nullptr;
    return GfVec3f_Multiply(vec, 1.0f / scalar);
}

GfVec3fRef GfVec3f_Negate(GfVec3fRef vec) {
    if (!vec) return nullptr;
    try {
        return new GfVec3fOpaque(-vec->getX(), -vec->getY(), -vec->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_CompMult(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
        return new GfVec3fOpaque(
            v1->getX() * v2->getX(),
            v1->getY() * v2->getY(),
            v1->getZ() * v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_CompDiv(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    if (v2->getX() == 0.0f || v2->getY() == 0.0f || v2->getZ() == 0.0f) return nullptr;
    try {
        return new GfVec3fOpaque(
            v1->getX() / v2->getX(),
            v1->getY() / v2->getY(),
            v1->getZ() / v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_GetProjection(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
#if GF_USE_USD
        return new GfVec3fOpaque(v1->vec.GetProjection(v2->vec));
#else
        float dot = GfVec3f_Dot(v1, v2);
        return new GfVec3fOpaque(
            v2->data[0] * dot,
            v2->data[1] * dot,
            v2->data[2] * dot);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfVec3fRef GfVec3f_GetComplement(GfVec3fRef v1, GfVec3fRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
#if GF_USE_USD
        return new GfVec3fOpaque(v1->vec.GetComplement(v2->vec));
#else
        GfVec3fRef proj = GfVec3f_GetProjection(v1, v2);
        if (!proj) return nullptr;
        auto* result = new GfVec3fOpaque(
            v1->data[0] - proj->data[0],
            v1->data[1] - proj->data[1],
            v1->data[2] - proj->data[2]);
        GfVec3f_Release(proj);
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

bool GfVec3f_Equal(GfVec3fRef v1, GfVec3fRef v2) {
    if (v1 == v2) return true;
    if (!v1 || !v2) return false;
#if GF_USE_USD
    return v1->vec == v2->vec;
#else
    return v1->data[0] == v2->data[0] &&
           v1->data[1] == v2->data[1] &&
           v1->data[2] == v2->data[2];
#endif
}

bool GfVec3f_IsClose(GfVec3fRef v1, GfVec3fRef v2, float tolerance) {
    if (v1 == v2) return true;
    if (!v1 || !v2) return false;
#if GF_USE_USD
    return GfIsClose(v1->vec, v2->vec, tolerance);
#else
    float dx = v1->data[0] - v2->data[0];
    float dy = v1->data[1] - v2->data[1];
    float dz = v1->data[2] - v2->data[2];
    return (dx*dx + dy*dy + dz*dz) <= tolerance * tolerance;
#endif
}

GfVec3fRef GfVec3f_XAxis(void) {
    return GfVec3f_Create(1.0f, 0.0f, 0.0f);
}

GfVec3fRef GfVec3f_YAxis(void) {
    return GfVec3f_Create(0.0f, 1.0f, 0.0f);
}

GfVec3fRef GfVec3f_ZAxis(void) {
    return GfVec3f_Create(0.0f, 0.0f, 1.0f);
}

} // extern "C"

// ============================================================================
// MARK: - GfVec3d Implementation
// ============================================================================

struct GfVec3dOpaque {
#if GF_USE_USD
    GfVec3d vec;
#else
    double data[3];
#endif
    std::atomic<int> refCount;

    GfVec3dOpaque() : refCount(1) {
#if !GF_USE_USD
        data[0] = data[1] = data[2] = 0.0;
#endif
    }

    GfVec3dOpaque(double x, double y, double z) : refCount(1) {
#if GF_USE_USD
        vec = GfVec3d(x, y, z);
#else
        data[0] = x; data[1] = y; data[2] = z;
#endif
    }

#if GF_USE_USD
    GfVec3dOpaque(const GfVec3d& v) : vec(v), refCount(1) {}
#endif

    double getX() const {
#if GF_USE_USD
        return vec[0];
#else
        return data[0];
#endif
    }

    double getY() const {
#if GF_USE_USD
        return vec[1];
#else
        return data[1];
#endif
    }

    double getZ() const {
#if GF_USE_USD
        return vec[2];
#else
        return data[2];
#endif
    }

    void set(double x, double y, double z) {
#if GF_USE_USD
        vec.Set(x, y, z);
#else
        data[0] = x; data[1] = y; data[2] = z;
#endif
    }

    double getLength() const {
#if GF_USE_USD
        return vec.GetLength();
#else
        return std::sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
#endif
    }

    double getLengthSq() const {
#if GF_USE_USD
        return vec.GetLengthSq();
#else
        return data[0]*data[0] + data[1]*data[1] + data[2]*data[2];
#endif
    }

    double normalize() {
#if GF_USE_USD
        return vec.Normalize();
#else
        double length = getLength();
        if (length > 1e-10) {
            data[0] /= length;
            data[1] /= length;
            data[2] /= length;
        }
        return length;
#endif
    }

    size_t hash() const {
#if GF_USE_USD
        return hash_value(vec);
#else
        size_t h = 0;
        h ^= std::hash<double>{}(data[0]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(data[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(data[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
#endif
    }
};

extern "C" {

GfVec3dRef GfVec3d_Create(double x, double y, double z) {
    try {
        return new GfVec3dOpaque(x, y, z);
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_CreateScalar(double value) {
    return GfVec3d_Create(value, value, value);
}

GfVec3dRef GfVec3d_CreateZero(void) {
    return GfVec3d_Create(0.0, 0.0, 0.0);
}

GfVec3dRef GfVec3d_Copy(GfVec3dRef vec) {
    if (!vec) return nullptr;
    try {
        return new GfVec3dOpaque(vec->getX(), vec->getY(), vec->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_Retain(GfVec3dRef vec) {
    if (!vec) return nullptr;
    vec->refCount.fetch_add(1, std::memory_order_relaxed);
    return vec;
}

void GfVec3d_Release(GfVec3dRef vec) {
    if (!vec) return;
    if (vec->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete vec;
    }
}

double GfVec3d_GetX(GfVec3dRef vec) {
    if (!vec) return 0.0;
    return vec->getX();
}

double GfVec3d_GetY(GfVec3dRef vec) {
    if (!vec) return 0.0;
    return vec->getY();
}

double GfVec3d_GetZ(GfVec3dRef vec) {
    if (!vec) return 0.0;
    return vec->getZ();
}

void GfVec3d_GetComponents(GfVec3dRef vec, double* outX, double* outY, double* outZ) {
    if (!vec || !outX || !outY || !outZ) return;
    *outX = vec->getX();
    *outY = vec->getY();
    *outZ = vec->getZ();
}

void GfVec3d_Set(GfVec3dRef vec, double x, double y, double z) {
    if (!vec) return;
    vec->set(x, y, z);
}

double GfVec3d_GetLength(GfVec3dRef vec) {
    if (!vec) return 0.0;
    return vec->getLength();
}

double GfVec3d_GetLengthSq(GfVec3dRef vec) {
    if (!vec) return 0.0;
    return vec->getLengthSq();
}

size_t GfVec3d_Hash(GfVec3dRef vec) {
    if (!vec) return 0;
    return vec->hash();
}

GfVec3dRef GfVec3d_GetNormalized(GfVec3dRef vec) {
    if (!vec) return nullptr;
    try {
        auto* result = new GfVec3dOpaque(vec->getX(), vec->getY(), vec->getZ());
        result->normalize();
        return result;
    } catch (...) {
        return nullptr;
    }
}

double GfVec3d_Normalize(GfVec3dRef vec) {
    if (!vec) return 0.0;
    return vec->normalize();
}

double GfVec3d_Dot(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return 0.0;
#if GF_USE_USD
    return GfDot(v1->vec, v2->vec);
#else
    return v1->data[0]*v2->data[0] + v1->data[1]*v2->data[1] + v1->data[2]*v2->data[2];
#endif
}

GfVec3dRef GfVec3d_Cross(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
#if GF_USE_USD
        return new GfVec3dOpaque(GfCross(v1->vec, v2->vec));
#else
        double x = v1->data[1]*v2->data[2] - v1->data[2]*v2->data[1];
        double y = v1->data[2]*v2->data[0] - v1->data[0]*v2->data[2];
        double z = v1->data[0]*v2->data[1] - v1->data[1]*v2->data[0];
        return new GfVec3dOpaque(x, y, z);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_Add(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
        return new GfVec3dOpaque(
            v1->getX() + v2->getX(),
            v1->getY() + v2->getY(),
            v1->getZ() + v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_Subtract(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
        return new GfVec3dOpaque(
            v1->getX() - v2->getX(),
            v1->getY() - v2->getY(),
            v1->getZ() - v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_Multiply(GfVec3dRef vec, double scalar) {
    if (!vec) return nullptr;
    try {
        return new GfVec3dOpaque(
            vec->getX() * scalar,
            vec->getY() * scalar,
            vec->getZ() * scalar);
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_Divide(GfVec3dRef vec, double scalar) {
    if (!vec || scalar == 0.0) return nullptr;
    return GfVec3d_Multiply(vec, 1.0 / scalar);
}

GfVec3dRef GfVec3d_Negate(GfVec3dRef vec) {
    if (!vec) return nullptr;
    try {
        return new GfVec3dOpaque(-vec->getX(), -vec->getY(), -vec->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_CompMult(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
        return new GfVec3dOpaque(
            v1->getX() * v2->getX(),
            v1->getY() * v2->getY(),
            v1->getZ() * v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_CompDiv(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    if (v2->getX() == 0.0 || v2->getY() == 0.0 || v2->getZ() == 0.0) return nullptr;
    try {
        return new GfVec3dOpaque(
            v1->getX() / v2->getX(),
            v1->getY() / v2->getY(),
            v1->getZ() / v2->getZ());
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_GetProjection(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
#if GF_USE_USD
        return new GfVec3dOpaque(v1->vec.GetProjection(v2->vec));
#else
        double dot = GfVec3d_Dot(v1, v2);
        return new GfVec3dOpaque(
            v2->data[0] * dot,
            v2->data[1] * dot,
            v2->data[2] * dot);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfVec3dRef GfVec3d_GetComplement(GfVec3dRef v1, GfVec3dRef v2) {
    if (!v1 || !v2) return nullptr;
    try {
#if GF_USE_USD
        return new GfVec3dOpaque(v1->vec.GetComplement(v2->vec));
#else
        GfVec3dRef proj = GfVec3d_GetProjection(v1, v2);
        if (!proj) return nullptr;
        auto* result = new GfVec3dOpaque(
            v1->data[0] - proj->data[0],
            v1->data[1] - proj->data[1],
            v1->data[2] - proj->data[2]);
        GfVec3d_Release(proj);
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

bool GfVec3d_Equal(GfVec3dRef v1, GfVec3dRef v2) {
    if (v1 == v2) return true;
    if (!v1 || !v2) return false;
#if GF_USE_USD
    return v1->vec == v2->vec;
#else
    return v1->data[0] == v2->data[0] &&
           v1->data[1] == v2->data[1] &&
           v1->data[2] == v2->data[2];
#endif
}

bool GfVec3d_IsClose(GfVec3dRef v1, GfVec3dRef v2, double tolerance) {
    if (v1 == v2) return true;
    if (!v1 || !v2) return false;
#if GF_USE_USD
    return GfIsClose(v1->vec, v2->vec, tolerance);
#else
    double dx = v1->data[0] - v2->data[0];
    double dy = v1->data[1] - v2->data[1];
    double dz = v1->data[2] - v2->data[2];
    return (dx*dx + dy*dy + dz*dz) <= tolerance * tolerance;
#endif
}

GfVec3dRef GfVec3d_XAxis(void) {
    return GfVec3d_Create(1.0, 0.0, 0.0);
}

GfVec3dRef GfVec3d_YAxis(void) {
    return GfVec3d_Create(0.0, 1.0, 0.0);
}

GfVec3dRef GfVec3d_ZAxis(void) {
    return GfVec3d_Create(0.0, 0.0, 1.0);
}

} // extern "C"

// ============================================================================
// MARK: - GfMatrix4d Implementation
// ============================================================================

struct GfMatrix4dOpaque {
#if GF_USE_USD
    GfMatrix4d mat;
#else
    double data[16]; // row-major
#endif
    std::atomic<int> refCount;

    GfMatrix4dOpaque() : refCount(1) {
#if !GF_USE_USD
        memset(data, 0, sizeof(data));
#endif
    }

#if GF_USE_USD
    GfMatrix4dOpaque(const GfMatrix4d& m) : mat(m), refCount(1) {}
#endif

    void setIdentity() {
#if GF_USE_USD
        mat.SetIdentity();
#else
        memset(data, 0, sizeof(data));
        data[0] = data[5] = data[10] = data[15] = 1.0;
#endif
    }

    void setZero() {
#if GF_USE_USD
        mat.SetZero();
#else
        memset(data, 0, sizeof(data));
#endif
    }

    double getElement(int row, int col) const {
#if GF_USE_USD
        return mat[row][col];
#else
        return data[row * 4 + col];
#endif
    }

    void setElement(int row, int col, double value) {
#if GF_USE_USD
        mat[row][col] = value;
#else
        data[row * 4 + col] = value;
#endif
    }

    size_t hash() const {
#if GF_USE_USD
        return hash_value(mat);
#else
        size_t h = 0;
        for (int i = 0; i < 16; i++) {
            h ^= std::hash<double>{}(data[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
#endif
    }
};

extern "C" {

GfMatrix4dRef GfMatrix4d_Create(
    double m00, double m01, double m02, double m03,
    double m10, double m11, double m12, double m13,
    double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(GfMatrix4d(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33));
#else
        auto* result = new GfMatrix4dOpaque();
        result->data[0] = m00; result->data[1] = m01; result->data[2] = m02; result->data[3] = m03;
        result->data[4] = m10; result->data[5] = m11; result->data[6] = m12; result->data[7] = m13;
        result->data[8] = m20; result->data[9] = m21; result->data[10] = m22; result->data[11] = m23;
        result->data[12] = m30; result->data[13] = m31; result->data[14] = m32; result->data[15] = m33;
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_CreateIdentity(void) {
    try {
        auto* result = new GfMatrix4dOpaque();
        result->setIdentity();
        return result;
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_CreateZero(void) {
    try {
        auto* result = new GfMatrix4dOpaque();
        result->setZero();
        return result;
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_CreateDiagonal(double scale) {
    return GfMatrix4d_Create(
        scale, 0, 0, 0,
        0, scale, 0, 0,
        0, 0, scale, 0,
        0, 0, 0, scale);
}

GfMatrix4dRef GfMatrix4d_Copy(GfMatrix4dRef mat) {
    if (!mat) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(mat->mat);
#else
        auto* result = new GfMatrix4dOpaque();
        memcpy(result->data, mat->data, sizeof(mat->data));
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_Retain(GfMatrix4dRef mat) {
    if (!mat) return nullptr;
    mat->refCount.fetch_add(1, std::memory_order_relaxed);
    return mat;
}

void GfMatrix4d_Release(GfMatrix4dRef mat) {
    if (!mat) return;
    if (mat->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete mat;
    }
}

double GfMatrix4d_GetElement(GfMatrix4dRef mat, int row, int col) {
    if (!mat || row < 0 || row > 3 || col < 0 || col > 3) return 0.0;
    return mat->getElement(row, col);
}

void GfMatrix4d_SetElement(GfMatrix4dRef mat, int row, int col, double value) {
    if (!mat || row < 0 || row > 3 || col < 0 || col > 3) return;
    mat->setElement(row, col, value);
}

void GfMatrix4d_GetElements(GfMatrix4dRef mat, double* outElements) {
    if (!mat || !outElements) return;
#if GF_USE_USD
    const double* data = mat->mat.GetArray();
    memcpy(outElements, data, 16 * sizeof(double));
#else
    memcpy(outElements, mat->data, 16 * sizeof(double));
#endif
}

void GfMatrix4d_SetElements(GfMatrix4dRef mat, const double* elements) {
    if (!mat || !elements) return;
#if GF_USE_USD
    double arr[4][4];
    memcpy(arr, elements, 16 * sizeof(double));
    mat->mat.Set(arr);
#else
    memcpy(mat->data, elements, 16 * sizeof(double));
#endif
}

void GfMatrix4d_GetRow(GfMatrix4dRef mat, int row, double* outX, double* outY, double* outZ, double* outW) {
    if (!mat || row < 0 || row > 3 || !outX || !outY || !outZ || !outW) return;
    *outX = mat->getElement(row, 0);
    *outY = mat->getElement(row, 1);
    *outZ = mat->getElement(row, 2);
    *outW = mat->getElement(row, 3);
}

void GfMatrix4d_GetColumn(GfMatrix4dRef mat, int col, double* outX, double* outY, double* outZ, double* outW) {
    if (!mat || col < 0 || col > 3 || !outX || !outY || !outZ || !outW) return;
    *outX = mat->getElement(0, col);
    *outY = mat->getElement(1, col);
    *outZ = mat->getElement(2, col);
    *outW = mat->getElement(3, col);
}

void GfMatrix4d_SetIdentity(GfMatrix4dRef mat) {
    if (!mat) return;
    mat->setIdentity();
}

void GfMatrix4d_SetZero(GfMatrix4dRef mat) {
    if (!mat) return;
    mat->setZero();
}

size_t GfMatrix4d_Hash(GfMatrix4dRef mat) {
    if (!mat) return 0;
    return mat->hash();
}

GfMatrix4dRef GfMatrix4d_GetTranspose(GfMatrix4dRef mat) {
    if (!mat) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(mat->mat.GetTranspose());
#else
        auto* result = new GfMatrix4dOpaque();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result->data[i * 4 + j] = mat->data[j * 4 + i];
            }
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_GetInverse(GfMatrix4dRef mat) {
    if (!mat) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(mat->mat.GetInverse());
#else
        // Simple fallback - returns identity for non-USD mode
        // Full matrix inversion is complex; recommend using USD mode
        auto* result = new GfMatrix4dOpaque();
        result->setIdentity();
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

double GfMatrix4d_GetDeterminant(GfMatrix4dRef mat) {
    if (!mat) return 0.0;
#if GF_USE_USD
    return mat->mat.GetDeterminant();
#else
    // Simplified determinant calculation
    const double* m = mat->data;
    return m[0] * (m[5]*(m[10]*m[15] - m[11]*m[14]) - m[6]*(m[9]*m[15] - m[11]*m[13]) + m[7]*(m[9]*m[14] - m[10]*m[13]))
         - m[1] * (m[4]*(m[10]*m[15] - m[11]*m[14]) - m[6]*(m[8]*m[15] - m[11]*m[12]) + m[7]*(m[8]*m[14] - m[10]*m[12]))
         + m[2] * (m[4]*(m[9]*m[15] - m[11]*m[13]) - m[5]*(m[8]*m[15] - m[11]*m[12]) + m[7]*(m[8]*m[13] - m[9]*m[12]))
         - m[3] * (m[4]*(m[9]*m[14] - m[10]*m[13]) - m[5]*(m[8]*m[14] - m[10]*m[12]) + m[6]*(m[8]*m[13] - m[9]*m[12]));
#endif
}

GfMatrix4dRef GfMatrix4d_Multiply(GfMatrix4dRef m1, GfMatrix4dRef m2) {
    if (!m1 || !m2) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(m1->mat * m2->mat);
#else
        auto* result = new GfMatrix4dOpaque();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                double sum = 0.0;
                for (int k = 0; k < 4; k++) {
                    sum += m1->data[i * 4 + k] * m2->data[k * 4 + j];
                }
                result->data[i * 4 + j] = sum;
            }
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_MultiplyScalar(GfMatrix4dRef mat, double scalar) {
    if (!mat) return nullptr;
    try {
#if GF_USE_USD
        GfMatrix4d result = mat->mat;
        result *= scalar;
        return new GfMatrix4dOpaque(result);
#else
        auto* result = new GfMatrix4dOpaque();
        for (int i = 0; i < 16; i++) {
            result->data[i] = mat->data[i] * scalar;
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_Add(GfMatrix4dRef m1, GfMatrix4dRef m2) {
    if (!m1 || !m2) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(m1->mat + m2->mat);
#else
        auto* result = new GfMatrix4dOpaque();
        for (int i = 0; i < 16; i++) {
            result->data[i] = m1->data[i] + m2->data[i];
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_Subtract(GfMatrix4dRef m1, GfMatrix4dRef m2) {
    if (!m1 || !m2) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(m1->mat - m2->mat);
#else
        auto* result = new GfMatrix4dOpaque();
        for (int i = 0; i < 16; i++) {
            result->data[i] = m1->data[i] - m2->data[i];
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_SetTranslate(double x, double y, double z) {
    try {
#if GF_USE_USD
        GfMatrix4d mat;
        mat.SetTranslate(GfVec3d(x, y, z));
        return new GfMatrix4dOpaque(mat);
#else
        auto* result = new GfMatrix4dOpaque();
        result->setIdentity();
        result->data[12] = x;
        result->data[13] = y;
        result->data[14] = z;
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_SetScale(double scale) {
    return GfMatrix4d_SetScaleVec(scale, scale, scale);
}

GfMatrix4dRef GfMatrix4d_SetScaleVec(double sx, double sy, double sz) {
    try {
#if GF_USE_USD
        GfMatrix4d mat;
        mat.SetScale(GfVec3d(sx, sy, sz));
        return new GfMatrix4dOpaque(mat);
#else
        return GfMatrix4d_Create(
            sx, 0, 0, 0,
            0, sy, 0, 0,
            0, 0, sz, 0,
            0, 0, 0, 1);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfMatrix4d_SetRotateQuat(GfQuatdRef quat) {
    if (!quat) return nullptr;
    try {
#if GF_USE_USD
        GfMatrix4d mat;
        mat.SetRotate(quat->quat);
        return new GfMatrix4dOpaque(mat);
#else
        // Fallback quaternion to matrix conversion
        auto* result = new GfMatrix4dOpaque();
        result->setIdentity();
        // Simplified - full implementation would convert quaternion to rotation matrix
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

void GfMatrix4d_ExtractTranslation(GfMatrix4dRef mat, double* outX, double* outY, double* outZ) {
    if (!mat || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d t = mat->mat.ExtractTranslation();
    *outX = t[0];
    *outY = t[1];
    *outZ = t[2];
#else
    *outX = mat->data[12];
    *outY = mat->data[13];
    *outZ = mat->data[14];
#endif
}

void GfMatrix4d_Transform(GfMatrix4dRef mat, double x, double y, double z,
                          double* outX, double* outY, double* outZ) {
    if (!mat || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d result = mat->mat.Transform(GfVec3d(x, y, z));
    *outX = result[0];
    *outY = result[1];
    *outZ = result[2];
#else
    // Transform with homogeneous divide
    double w = mat->data[3]*x + mat->data[7]*y + mat->data[11]*z + mat->data[15];
    if (std::abs(w) > 1e-10) {
        *outX = (mat->data[0]*x + mat->data[4]*y + mat->data[8]*z + mat->data[12]) / w;
        *outY = (mat->data[1]*x + mat->data[5]*y + mat->data[9]*z + mat->data[13]) / w;
        *outZ = (mat->data[2]*x + mat->data[6]*y + mat->data[10]*z + mat->data[14]) / w;
    }
#endif
}

void GfMatrix4d_TransformDir(GfMatrix4dRef mat, double x, double y, double z,
                             double* outX, double* outY, double* outZ) {
    if (!mat || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d result = mat->mat.TransformDir(GfVec3d(x, y, z));
    *outX = result[0];
    *outY = result[1];
    *outZ = result[2];
#else
    // Transform direction (no translation)
    *outX = mat->data[0]*x + mat->data[4]*y + mat->data[8]*z;
    *outY = mat->data[1]*x + mat->data[5]*y + mat->data[9]*z;
    *outZ = mat->data[2]*x + mat->data[6]*y + mat->data[10]*z;
#endif
}

void GfMatrix4d_TransformAffine(GfMatrix4dRef mat, double x, double y, double z,
                                double* outX, double* outY, double* outZ) {
    if (!mat || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d result = mat->mat.TransformAffine(GfVec3d(x, y, z));
    *outX = result[0];
    *outY = result[1];
    *outZ = result[2];
#else
    // Affine transform (no perspective divide)
    *outX = mat->data[0]*x + mat->data[4]*y + mat->data[8]*z + mat->data[12];
    *outY = mat->data[1]*x + mat->data[5]*y + mat->data[9]*z + mat->data[13];
    *outZ = mat->data[2]*x + mat->data[6]*y + mat->data[10]*z + mat->data[14];
#endif
}

bool GfMatrix4d_Equal(GfMatrix4dRef m1, GfMatrix4dRef m2) {
    if (m1 == m2) return true;
    if (!m1 || !m2) return false;
#if GF_USE_USD
    return m1->mat == m2->mat;
#else
    return memcmp(m1->data, m2->data, sizeof(m1->data)) == 0;
#endif
}

bool GfMatrix4d_IsClose(GfMatrix4dRef m1, GfMatrix4dRef m2, double tolerance) {
    if (m1 == m2) return true;
    if (!m1 || !m2) return false;
#if GF_USE_USD
    return GfIsClose(m1->mat, m2->mat, tolerance);
#else
    for (int i = 0; i < 16; i++) {
        if (std::abs(m1->data[i] - m2->data[i]) > tolerance) {
            return false;
        }
    }
    return true;
#endif
}

} // extern "C"

// ============================================================================
// MARK: - GfQuatd Implementation
// ============================================================================

struct GfQuatdOpaque {
#if GF_USE_USD
    GfQuatd quat;
#else
    double real;
    double imag[3];
#endif
    std::atomic<int> refCount;

    GfQuatdOpaque() : refCount(1) {
#if !GF_USE_USD
        real = 1.0;
        imag[0] = imag[1] = imag[2] = 0.0;
#endif
    }

    GfQuatdOpaque(double r, double i, double j, double k) : refCount(1) {
#if GF_USE_USD
        quat = GfQuatd(r, i, j, k);
#else
        real = r;
        imag[0] = i; imag[1] = j; imag[2] = k;
#endif
    }

#if GF_USE_USD
    GfQuatdOpaque(const GfQuatd& q) : quat(q), refCount(1) {}
#endif

    double getReal() const {
#if GF_USE_USD
        return quat.GetReal();
#else
        return real;
#endif
    }

    void getImaginary(double& i, double& j, double& k) const {
#if GF_USE_USD
        const GfVec3d& im = quat.GetImaginary();
        i = im[0]; j = im[1]; k = im[2];
#else
        i = imag[0]; j = imag[1]; k = imag[2];
#endif
    }

    double getLength() const {
#if GF_USE_USD
        return quat.GetLength();
#else
        return std::sqrt(real*real + imag[0]*imag[0] + imag[1]*imag[1] + imag[2]*imag[2]);
#endif
    }

    double normalize() {
#if GF_USE_USD
        return quat.Normalize();
#else
        double len = getLength();
        if (len > 1e-10) {
            real /= len;
            imag[0] /= len;
            imag[1] /= len;
            imag[2] /= len;
        } else {
            real = 1.0;
            imag[0] = imag[1] = imag[2] = 0.0;
        }
        return len;
#endif
    }

    size_t hash() const {
#if GF_USE_USD
        return hash_value(quat);
#else
        size_t h = 0;
        h ^= std::hash<double>{}(real) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(imag[0]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(imag[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(imag[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
#endif
    }
};

extern "C" {

GfQuatdRef GfQuatd_Create(double real, double i, double j, double k) {
    try {
        return new GfQuatdOpaque(real, i, j, k);
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_CreateReal(double real) {
    return GfQuatd_Create(real, 0.0, 0.0, 0.0);
}

GfQuatdRef GfQuatd_CreateIdentity(void) {
    return GfQuatd_Create(1.0, 0.0, 0.0, 0.0);
}

GfQuatdRef GfQuatd_CreateZero(void) {
    return GfQuatd_Create(0.0, 0.0, 0.0, 0.0);
}

GfQuatdRef GfQuatd_Copy(GfQuatdRef quat) {
    if (!quat) return nullptr;
    try {
        double i, j, k;
        quat->getImaginary(i, j, k);
        return new GfQuatdOpaque(quat->getReal(), i, j, k);
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_Retain(GfQuatdRef quat) {
    if (!quat) return nullptr;
    quat->refCount.fetch_add(1, std::memory_order_relaxed);
    return quat;
}

void GfQuatd_Release(GfQuatdRef quat) {
    if (!quat) return;
    if (quat->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete quat;
    }
}

double GfQuatd_GetReal(GfQuatdRef quat) {
    if (!quat) return 0.0;
    return quat->getReal();
}

void GfQuatd_GetImaginary(GfQuatdRef quat, double* outI, double* outJ, double* outK) {
    if (!quat || !outI || !outJ || !outK) return;
    quat->getImaginary(*outI, *outJ, *outK);
}

void GfQuatd_SetReal(GfQuatdRef quat, double real) {
    if (!quat) return;
#if GF_USE_USD
    quat->quat.SetReal(real);
#else
    quat->real = real;
#endif
}

void GfQuatd_SetImaginary(GfQuatdRef quat, double i, double j, double k) {
    if (!quat) return;
#if GF_USE_USD
    quat->quat.SetImaginary(i, j, k);
#else
    quat->imag[0] = i;
    quat->imag[1] = j;
    quat->imag[2] = k;
#endif
}

double GfQuatd_GetLength(GfQuatdRef quat) {
    if (!quat) return 0.0;
    return quat->getLength();
}

size_t GfQuatd_Hash(GfQuatdRef quat) {
    if (!quat) return 0;
    return quat->hash();
}

GfQuatdRef GfQuatd_GetNormalized(GfQuatdRef quat) {
    if (!quat) return nullptr;
    try {
        GfQuatdRef result = GfQuatd_Copy(quat);
        if (result) result->normalize();
        return result;
    } catch (...) {
        return nullptr;
    }
}

double GfQuatd_Normalize(GfQuatdRef quat) {
    if (!quat) return 0.0;
    return quat->normalize();
}

GfQuatdRef GfQuatd_GetConjugate(GfQuatdRef quat) {
    if (!quat) return nullptr;
    try {
#if GF_USE_USD
        return new GfQuatdOpaque(quat->quat.GetConjugate());
#else
        return new GfQuatdOpaque(quat->real, -quat->imag[0], -quat->imag[1], -quat->imag[2]);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_GetInverse(GfQuatdRef quat) {
    if (!quat) return nullptr;
    try {
#if GF_USE_USD
        return new GfQuatdOpaque(quat->quat.GetInverse());
#else
        double lenSq = quat->real*quat->real +
                       quat->imag[0]*quat->imag[0] +
                       quat->imag[1]*quat->imag[1] +
                       quat->imag[2]*quat->imag[2];
        if (lenSq < 1e-20) return GfQuatd_CreateIdentity();
        return new GfQuatdOpaque(
            quat->real / lenSq,
            -quat->imag[0] / lenSq,
            -quat->imag[1] / lenSq,
            -quat->imag[2] / lenSq);
#endif
    } catch (...) {
        return nullptr;
    }
}

double GfQuatd_Dot(GfQuatdRef q1, GfQuatdRef q2) {
    if (!q1 || !q2) return 0.0;
#if GF_USE_USD
    return GfDot(q1->quat, q2->quat);
#else
    double i1, j1, k1, i2, j2, k2;
    q1->getImaginary(i1, j1, k1);
    q2->getImaginary(i2, j2, k2);
    return q1->getReal()*q2->getReal() + i1*i2 + j1*j2 + k1*k2;
#endif
}

GfQuatdRef GfQuatd_Multiply(GfQuatdRef q1, GfQuatdRef q2) {
    if (!q1 || !q2) return nullptr;
    try {
#if GF_USE_USD
        return new GfQuatdOpaque(q1->quat * q2->quat);
#else
        double r1 = q1->real, r2 = q2->real;
        double i1, j1, k1, i2, j2, k2;
        q1->getImaginary(i1, j1, k1);
        q2->getImaginary(i2, j2, k2);

        return new GfQuatdOpaque(
            r1*r2 - i1*i2 - j1*j2 - k1*k2,
            r1*i2 + i1*r2 + j1*k2 - k1*j2,
            r1*j2 - i1*k2 + j1*r2 + k1*i2,
            r1*k2 + i1*j2 - j1*i2 + k1*r2);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_MultiplyScalar(GfQuatdRef quat, double scalar) {
    if (!quat) return nullptr;
    try {
        double i, j, k;
        quat->getImaginary(i, j, k);
        return new GfQuatdOpaque(
            quat->getReal() * scalar,
            i * scalar,
            j * scalar,
            k * scalar);
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_Add(GfQuatdRef q1, GfQuatdRef q2) {
    if (!q1 || !q2) return nullptr;
    try {
        double i1, j1, k1, i2, j2, k2;
        q1->getImaginary(i1, j1, k1);
        q2->getImaginary(i2, j2, k2);
        return new GfQuatdOpaque(
            q1->getReal() + q2->getReal(),
            i1 + i2,
            j1 + j2,
            k1 + k2);
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_Subtract(GfQuatdRef q1, GfQuatdRef q2) {
    if (!q1 || !q2) return nullptr;
    try {
        double i1, j1, k1, i2, j2, k2;
        q1->getImaginary(i1, j1, k1);
        q2->getImaginary(i2, j2, k2);
        return new GfQuatdOpaque(
            q1->getReal() - q2->getReal(),
            i1 - i2,
            j1 - j2,
            k1 - k2);
    } catch (...) {
        return nullptr;
    }
}

GfQuatdRef GfQuatd_Negate(GfQuatdRef quat) {
    if (!quat) return nullptr;
    try {
        double i, j, k;
        quat->getImaginary(i, j, k);
        return new GfQuatdOpaque(-quat->getReal(), -i, -j, -k);
    } catch (...) {
        return nullptr;
    }
}

void GfQuatd_Transform(GfQuatdRef quat, double x, double y, double z,
                       double* outX, double* outY, double* outZ) {
    if (!quat || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d result = quat->quat.Transform(GfVec3d(x, y, z));
    *outX = result[0];
    *outY = result[1];
    *outZ = result[2];
#else
    // q * v * q^-1 for quaternion rotation
    // Simplified implementation
    double r = quat->real;
    double qi, qj, qk;
    quat->getImaginary(qi, qj, qk);

    // v' = v + 2r(q x v) + 2(q x (q x v))
    double cx1 = qj*z - qk*y;
    double cy1 = qk*x - qi*z;
    double cz1 = qi*y - qj*x;

    double cx2 = qj*cz1 - qk*cy1;
    double cy2 = qk*cx1 - qi*cz1;
    double cz2 = qi*cy1 - qj*cx1;

    *outX = x + 2.0*(r*cx1 + cx2);
    *outY = y + 2.0*(r*cy1 + cy2);
    *outZ = z + 2.0*(r*cz1 + cz2);
#endif
}

GfQuatdRef GfQuatd_Slerp(GfQuatdRef q0, GfQuatdRef q1, double alpha) {
    if (!q0 || !q1) return nullptr;
    try {
#if GF_USE_USD
        return new GfQuatdOpaque(GfSlerp(alpha, q0->quat, q1->quat));
#else
        // Simplified slerp implementation
        double dot = GfQuatd_Dot(q0, q1);

        // If dot is negative, negate one quaternion to take shorter path
        GfQuatdRef q1_adj = q1;
        bool negated = false;
        if (dot < 0.0) {
            q1_adj = GfQuatd_Negate(q1);
            dot = -dot;
            negated = true;
        }

        double r0 = q0->getReal(), r1 = q1_adj->getReal();
        double i0, j0, k0, i1, j1, k1;
        q0->getImaginary(i0, j0, k0);
        q1_adj->getImaginary(i1, j1, k1);

        double scale0, scale1;
        if (dot > 0.9995) {
            // Linear interpolation for nearly identical quaternions
            scale0 = 1.0 - alpha;
            scale1 = alpha;
        } else {
            double theta = std::acos(dot);
            double sinTheta = std::sin(theta);
            scale0 = std::sin((1.0 - alpha) * theta) / sinTheta;
            scale1 = std::sin(alpha * theta) / sinTheta;
        }

        auto* result = new GfQuatdOpaque(
            scale0 * r0 + scale1 * r1,
            scale0 * i0 + scale1 * i1,
            scale0 * j0 + scale1 * j1,
            scale0 * k0 + scale1 * k1);

        if (negated) GfQuatd_Release(q1_adj);
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

bool GfQuatd_Equal(GfQuatdRef q1, GfQuatdRef q2) {
    if (q1 == q2) return true;
    if (!q1 || !q2) return false;
#if GF_USE_USD
    return q1->quat == q2->quat;
#else
    double i1, j1, k1, i2, j2, k2;
    q1->getImaginary(i1, j1, k1);
    q2->getImaginary(i2, j2, k2);
    return q1->getReal() == q2->getReal() &&
           i1 == i2 && j1 == j2 && k1 == k2;
#endif
}

} // extern "C"

// ============================================================================
// MARK: - GfRange3d Implementation
// ============================================================================

struct GfRange3dOpaque {
#if GF_USE_USD
    GfRange3d range;
#else
    double min[3];
    double max[3];
#endif
    std::atomic<int> refCount;

    GfRange3dOpaque() : refCount(1) {
#if GF_USE_USD
        // Default constructor creates empty range
#else
        min[0] = min[1] = min[2] = FLT_MAX;
        max[0] = max[1] = max[2] = -FLT_MAX;
#endif
    }

    GfRange3dOpaque(double minX, double minY, double minZ,
                    double maxX, double maxY, double maxZ) : refCount(1) {
#if GF_USE_USD
        range = GfRange3d(GfVec3d(minX, minY, minZ), GfVec3d(maxX, maxY, maxZ));
#else
        min[0] = minX; min[1] = minY; min[2] = minZ;
        max[0] = maxX; max[1] = maxY; max[2] = maxZ;
#endif
    }

#if GF_USE_USD
    GfRange3dOpaque(const GfRange3d& r) : range(r), refCount(1) {}
#endif

    bool isEmpty() const {
#if GF_USE_USD
        return range.IsEmpty();
#else
        return min[0] > max[0] || min[1] > max[1] || min[2] > max[2];
#endif
    }

    void setEmpty() {
#if GF_USE_USD
        range.SetEmpty();
#else
        min[0] = min[1] = min[2] = FLT_MAX;
        max[0] = max[1] = max[2] = -FLT_MAX;
#endif
    }

    size_t hash() const {
#if GF_USE_USD
        return hash_value(range);
#else
        size_t h = 0;
        for (int i = 0; i < 3; i++) {
            h ^= std::hash<double>{}(min[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<double>{}(max[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
#endif
    }
};

extern "C" {

GfRange3dRef GfRange3d_Create(
    double minX, double minY, double minZ,
    double maxX, double maxY, double maxZ) {
    try {
        return new GfRange3dOpaque(minX, minY, minZ, maxX, maxY, maxZ);
    } catch (...) {
        return nullptr;
    }
}

GfRange3dRef GfRange3d_CreateEmpty(void) {
    try {
        return new GfRange3dOpaque();
    } catch (...) {
        return nullptr;
    }
}

GfRange3dRef GfRange3d_CreateUnitCube(void) {
    return GfRange3d_Create(0, 0, 0, 1, 1, 1);
}

GfRange3dRef GfRange3d_Copy(GfRange3dRef range) {
    if (!range) return nullptr;
    try {
#if GF_USE_USD
        return new GfRange3dOpaque(range->range);
#else
        return new GfRange3dOpaque(
            range->min[0], range->min[1], range->min[2],
            range->max[0], range->max[1], range->max[2]);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfRange3dRef GfRange3d_Retain(GfRange3dRef range) {
    if (!range) return nullptr;
    range->refCount.fetch_add(1, std::memory_order_relaxed);
    return range;
}

void GfRange3d_Release(GfRange3dRef range) {
    if (!range) return;
    if (range->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete range;
    }
}

void GfRange3d_GetMin(GfRange3dRef range, double* outX, double* outY, double* outZ) {
    if (!range || !outX || !outY || !outZ) return;
#if GF_USE_USD
    const GfVec3d& m = range->range.GetMin();
    *outX = m[0]; *outY = m[1]; *outZ = m[2];
#else
    *outX = range->min[0]; *outY = range->min[1]; *outZ = range->min[2];
#endif
}

void GfRange3d_GetMax(GfRange3dRef range, double* outX, double* outY, double* outZ) {
    if (!range || !outX || !outY || !outZ) return;
#if GF_USE_USD
    const GfVec3d& m = range->range.GetMax();
    *outX = m[0]; *outY = m[1]; *outZ = m[2];
#else
    *outX = range->max[0]; *outY = range->max[1]; *outZ = range->max[2];
#endif
}

void GfRange3d_SetMin(GfRange3dRef range, double x, double y, double z) {
    if (!range) return;
#if GF_USE_USD
    range->range.SetMin(GfVec3d(x, y, z));
#else
    range->min[0] = x; range->min[1] = y; range->min[2] = z;
#endif
}

void GfRange3d_SetMax(GfRange3dRef range, double x, double y, double z) {
    if (!range) return;
#if GF_USE_USD
    range->range.SetMax(GfVec3d(x, y, z));
#else
    range->max[0] = x; range->max[1] = y; range->max[2] = z;
#endif
}

void GfRange3d_GetSize(GfRange3dRef range, double* outX, double* outY, double* outZ) {
    if (!range || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d size = range->range.GetSize();
    *outX = size[0]; *outY = size[1]; *outZ = size[2];
#else
    *outX = range->max[0] - range->min[0];
    *outY = range->max[1] - range->min[1];
    *outZ = range->max[2] - range->min[2];
#endif
}

void GfRange3d_GetMidpoint(GfRange3dRef range, double* outX, double* outY, double* outZ) {
    if (!range || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d mid = range->range.GetMidpoint();
    *outX = mid[0]; *outY = mid[1]; *outZ = mid[2];
#else
    *outX = (range->min[0] + range->max[0]) * 0.5;
    *outY = (range->min[1] + range->max[1]) * 0.5;
    *outZ = (range->min[2] + range->max[2]) * 0.5;
#endif
}

bool GfRange3d_IsEmpty(GfRange3dRef range) {
    if (!range) return true;
    return range->isEmpty();
}

void GfRange3d_SetEmpty(GfRange3dRef range) {
    if (!range) return;
    range->setEmpty();
}

size_t GfRange3d_Hash(GfRange3dRef range) {
    if (!range) return 0;
    return range->hash();
}

bool GfRange3d_ContainsPoint(GfRange3dRef range, double x, double y, double z) {
    if (!range) return false;
#if GF_USE_USD
    return range->range.Contains(GfVec3d(x, y, z));
#else
    return x >= range->min[0] && x <= range->max[0] &&
           y >= range->min[1] && y <= range->max[1] &&
           z >= range->min[2] && z <= range->max[2];
#endif
}

bool GfRange3d_ContainsRange(GfRange3dRef range, GfRange3dRef other) {
    if (!range || !other) return false;
#if GF_USE_USD
    return range->range.Contains(other->range);
#else
    return range->min[0] <= other->min[0] && range->max[0] >= other->max[0] &&
           range->min[1] <= other->min[1] && range->max[1] >= other->max[1] &&
           range->min[2] <= other->min[2] && range->max[2] >= other->max[2];
#endif
}

bool GfRange3d_IsOutside(GfRange3dRef range, GfRange3dRef other) {
    if (!range || !other) return true;
#if GF_USE_USD
    return range->range.IsOutside(other->range);
#else
    return other->max[0] < range->min[0] || other->min[0] > range->max[0] ||
           other->max[1] < range->min[1] || other->min[1] > range->max[1] ||
           other->max[2] < range->min[2] || other->min[2] > range->max[2];
#endif
}

void GfRange3d_UnionWithPoint(GfRange3dRef range, double x, double y, double z) {
    if (!range) return;
#if GF_USE_USD
    range->range.UnionWith(GfVec3d(x, y, z));
#else
    range->min[0] = std::min(range->min[0], x);
    range->min[1] = std::min(range->min[1], y);
    range->min[2] = std::min(range->min[2], z);
    range->max[0] = std::max(range->max[0], x);
    range->max[1] = std::max(range->max[1], y);
    range->max[2] = std::max(range->max[2], z);
#endif
}

void GfRange3d_UnionWithRange(GfRange3dRef range, GfRange3dRef other) {
    if (!range || !other) return;
#if GF_USE_USD
    range->range.UnionWith(other->range);
#else
    range->min[0] = std::min(range->min[0], other->min[0]);
    range->min[1] = std::min(range->min[1], other->min[1]);
    range->min[2] = std::min(range->min[2], other->min[2]);
    range->max[0] = std::max(range->max[0], other->max[0]);
    range->max[1] = std::max(range->max[1], other->max[1]);
    range->max[2] = std::max(range->max[2], other->max[2]);
#endif
}

GfRange3dRef GfRange3d_GetUnion(GfRange3dRef r1, GfRange3dRef r2) {
    if (!r1 || !r2) return nullptr;
    try {
#if GF_USE_USD
        return new GfRange3dOpaque(GfRange3d::GetUnion(r1->range, r2->range));
#else
        return new GfRange3dOpaque(
            std::min(r1->min[0], r2->min[0]),
            std::min(r1->min[1], r2->min[1]),
            std::min(r1->min[2], r2->min[2]),
            std::max(r1->max[0], r2->max[0]),
            std::max(r1->max[1], r2->max[1]),
            std::max(r1->max[2], r2->max[2]));
#endif
    } catch (...) {
        return nullptr;
    }
}

GfRange3dRef GfRange3d_GetIntersection(GfRange3dRef r1, GfRange3dRef r2) {
    if (!r1 || !r2) return nullptr;
    try {
#if GF_USE_USD
        return new GfRange3dOpaque(GfRange3d::GetIntersection(r1->range, r2->range));
#else
        return new GfRange3dOpaque(
            std::max(r1->min[0], r2->min[0]),
            std::max(r1->min[1], r2->min[1]),
            std::max(r1->min[2], r2->min[2]),
            std::min(r1->max[0], r2->max[0]),
            std::min(r1->max[1], r2->max[1]),
            std::min(r1->max[2], r2->max[2]));
#endif
    } catch (...) {
        return nullptr;
    }
}

double GfRange3d_GetDistanceSquared(GfRange3dRef range, double x, double y, double z) {
    if (!range) return 0.0;
#if GF_USE_USD
    return range->range.GetDistanceSquared(GfVec3d(x, y, z));
#else
    double dx = 0, dy = 0, dz = 0;
    if (x < range->min[0]) dx = range->min[0] - x;
    else if (x > range->max[0]) dx = x - range->max[0];
    if (y < range->min[1]) dy = range->min[1] - y;
    else if (y > range->max[1]) dy = y - range->max[1];
    if (z < range->min[2]) dz = range->min[2] - z;
    else if (z > range->max[2]) dz = z - range->max[2];
    return dx*dx + dy*dy + dz*dz;
#endif
}

void GfRange3d_GetCorner(GfRange3dRef range, size_t i, double* outX, double* outY, double* outZ) {
    if (!range || !outX || !outY || !outZ || i > 7) return;
#if GF_USE_USD
    GfVec3d corner = range->range.GetCorner(i);
    *outX = corner[0]; *outY = corner[1]; *outZ = corner[2];
#else
    *outX = (i & 1) ? range->max[0] : range->min[0];
    *outY = (i & 2) ? range->max[1] : range->min[1];
    *outZ = (i & 4) ? range->max[2] : range->min[2];
#endif
}

bool GfRange3d_Equal(GfRange3dRef r1, GfRange3dRef r2) {
    if (r1 == r2) return true;
    if (!r1 || !r2) return false;
#if GF_USE_USD
    return r1->range == r2->range;
#else
    return r1->min[0] == r2->min[0] && r1->min[1] == r2->min[1] && r1->min[2] == r2->min[2] &&
           r1->max[0] == r2->max[0] && r1->max[1] == r2->max[1] && r1->max[2] == r2->max[2];
#endif
}

} // extern "C"

// ============================================================================
// MARK: - GfBBox3d Implementation
// ============================================================================

struct GfBBox3dOpaque {
#if GF_USE_USD
    GfBBox3d bbox;
#else
    // Store raw data instead of embedded opaques (to avoid atomic copy issues)
    double rangeMin[3];
    double rangeMax[3];
    double matrix[16];
    double inverse[16];
    bool hasZeroAreaPrimitives;
#endif
    std::atomic<int> refCount;

    GfBBox3dOpaque() : refCount(1) {
#if !GF_USE_USD
        // Empty range
        rangeMin[0] = rangeMin[1] = rangeMin[2] = FLT_MAX;
        rangeMax[0] = rangeMax[1] = rangeMax[2] = -FLT_MAX;
        // Identity matrices
        memset(matrix, 0, sizeof(matrix));
        matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
        memset(inverse, 0, sizeof(inverse));
        inverse[0] = inverse[5] = inverse[10] = inverse[15] = 1.0;
        hasZeroAreaPrimitives = false;
#endif
    }

#if GF_USE_USD
    GfBBox3dOpaque(const GfBBox3d& b) : bbox(b), refCount(1) {}
#endif

    size_t hash() const {
#if GF_USE_USD
        return hash_value(bbox);
#else
        size_t h = 0;
        for (int i = 0; i < 3; i++) {
            h ^= std::hash<double>{}(rangeMin[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<double>{}(rangeMax[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        for (int i = 0; i < 16; i++) {
            h ^= std::hash<double>{}(matrix[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
#endif
    }
};

extern "C" {

GfBBox3dRef GfBBox3d_Create(void) {
    try {
        return new GfBBox3dOpaque();
    } catch (...) {
        return nullptr;
    }
}

GfBBox3dRef GfBBox3d_CreateFromRange(GfRange3dRef range) {
    if (!range) return GfBBox3d_Create();
    try {
#if GF_USE_USD
        return new GfBBox3dOpaque(GfBBox3d(range->range));
#else
        auto* result = new GfBBox3dOpaque();
        memcpy(result->rangeMin, range->min, sizeof(result->rangeMin));
        memcpy(result->rangeMax, range->max, sizeof(result->rangeMax));
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfBBox3dRef GfBBox3d_CreateFromRangeAndMatrix(GfRange3dRef range, GfMatrix4dRef matrix) {
    if (!range || !matrix) return GfBBox3d_Create();
    try {
#if GF_USE_USD
        return new GfBBox3dOpaque(GfBBox3d(range->range, matrix->mat));
#else
        auto* result = new GfBBox3dOpaque();
        memcpy(result->rangeMin, range->min, sizeof(result->rangeMin));
        memcpy(result->rangeMax, range->max, sizeof(result->rangeMax));
        memcpy(result->matrix, matrix->data, sizeof(result->matrix));
        // Identity inverse for simplification
        memset(result->inverse, 0, sizeof(result->inverse));
        result->inverse[0] = result->inverse[5] = result->inverse[10] = result->inverse[15] = 1.0;
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfBBox3dRef GfBBox3d_Copy(GfBBox3dRef bbox) {
    if (!bbox) return nullptr;
    try {
#if GF_USE_USD
        return new GfBBox3dOpaque(bbox->bbox);
#else
        auto* result = new GfBBox3dOpaque();
        memcpy(result->rangeMin, bbox->rangeMin, sizeof(result->rangeMin));
        memcpy(result->rangeMax, bbox->rangeMax, sizeof(result->rangeMax));
        memcpy(result->matrix, bbox->matrix, sizeof(result->matrix));
        memcpy(result->inverse, bbox->inverse, sizeof(result->inverse));
        result->hasZeroAreaPrimitives = bbox->hasZeroAreaPrimitives;
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfBBox3dRef GfBBox3d_Retain(GfBBox3dRef bbox) {
    if (!bbox) return nullptr;
    bbox->refCount.fetch_add(1, std::memory_order_relaxed);
    return bbox;
}

void GfBBox3d_Release(GfBBox3dRef bbox) {
    if (!bbox) return;
    if (bbox->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete bbox;
    }
}

GfRange3dRef GfBBox3d_GetRange(GfBBox3dRef bbox) {
    if (!bbox) return nullptr;
    try {
#if GF_USE_USD
        return new GfRange3dOpaque(bbox->bbox.GetRange());
#else
        return GfRange3d_Create(
            bbox->rangeMin[0], bbox->rangeMin[1], bbox->rangeMin[2],
            bbox->rangeMax[0], bbox->rangeMax[1], bbox->rangeMax[2]);
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfBBox3d_GetMatrix(GfBBox3dRef bbox) {
    if (!bbox) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(bbox->bbox.GetMatrix());
#else
        auto* result = new GfMatrix4dOpaque();
        memcpy(result->data, bbox->matrix, sizeof(result->data));
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

GfMatrix4dRef GfBBox3d_GetInverseMatrix(GfBBox3dRef bbox) {
    if (!bbox) return nullptr;
    try {
#if GF_USE_USD
        return new GfMatrix4dOpaque(bbox->bbox.GetInverseMatrix());
#else
        auto* result = new GfMatrix4dOpaque();
        memcpy(result->data, bbox->inverse, sizeof(result->data));
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

void GfBBox3d_SetRange(GfBBox3dRef bbox, GfRange3dRef range) {
    if (!bbox || !range) return;
#if GF_USE_USD
    bbox->bbox.SetRange(range->range);
#else
    memcpy(bbox->rangeMin, range->min, sizeof(bbox->rangeMin));
    memcpy(bbox->rangeMax, range->max, sizeof(bbox->rangeMax));
#endif
}

void GfBBox3d_SetMatrix(GfBBox3dRef bbox, GfMatrix4dRef matrix) {
    if (!bbox || !matrix) return;
#if GF_USE_USD
    bbox->bbox.SetMatrix(matrix->mat);
#else
    memcpy(bbox->matrix, matrix->data, sizeof(bbox->matrix));
    // Would need to compute inverse properly
#endif
}

bool GfBBox3d_HasZeroAreaPrimitives(GfBBox3dRef bbox) {
    if (!bbox) return false;
#if GF_USE_USD
    return bbox->bbox.HasZeroAreaPrimitives();
#else
    return bbox->hasZeroAreaPrimitives;
#endif
}

void GfBBox3d_SetHasZeroAreaPrimitives(GfBBox3dRef bbox, bool hasZeroArea) {
    if (!bbox) return;
#if GF_USE_USD
    bbox->bbox.SetHasZeroAreaPrimitives(hasZeroArea);
#else
    bbox->hasZeroAreaPrimitives = hasZeroArea;
#endif
}

size_t GfBBox3d_Hash(GfBBox3dRef bbox) {
    if (!bbox) return 0;
    return bbox->hash();
}

double GfBBox3d_GetVolume(GfBBox3dRef bbox) {
    if (!bbox) return 0.0;
#if GF_USE_USD
    return bbox->bbox.GetVolume();
#else
    // Check if empty (max < min in any dimension)
    for (int i = 0; i < 3; ++i) {
        if (bbox->rangeMax[i] < bbox->rangeMin[i]) return 0.0;
    }
    double sx = bbox->rangeMax[0] - bbox->rangeMin[0];
    double sy = bbox->rangeMax[1] - bbox->rangeMin[1];
    double sz = bbox->rangeMax[2] - bbox->rangeMin[2];
    return sx * sy * sz;
#endif
}

void GfBBox3d_Transform(GfBBox3dRef bbox, GfMatrix4dRef matrix) {
    if (!bbox || !matrix) return;
#if GF_USE_USD
    bbox->bbox.Transform(matrix->mat);
#else
    // Would need proper matrix multiplication
#endif
}

GfRange3dRef GfBBox3d_ComputeAlignedRange(GfBBox3dRef bbox) {
    if (!bbox) return nullptr;
    try {
#if GF_USE_USD
        return new GfRange3dOpaque(bbox->bbox.ComputeAlignedRange());
#else
        // Simplified - return the untransformed range
        return GfRange3d_Create(
            bbox->rangeMin[0], bbox->rangeMin[1], bbox->rangeMin[2],
            bbox->rangeMax[0], bbox->rangeMax[1], bbox->rangeMax[2]);
#endif
    } catch (...) {
        return nullptr;
    }
}

void GfBBox3d_ComputeCentroid(GfBBox3dRef bbox, double* outX, double* outY, double* outZ) {
    if (!bbox || !outX || !outY || !outZ) return;
#if GF_USE_USD
    GfVec3d centroid = bbox->bbox.ComputeCentroid();
    *outX = centroid[0];
    *outY = centroid[1];
    *outZ = centroid[2];
#else
    // Compute midpoint of range
    *outX = (bbox->rangeMin[0] + bbox->rangeMax[0]) * 0.5;
    *outY = (bbox->rangeMin[1] + bbox->rangeMax[1]) * 0.5;
    *outZ = (bbox->rangeMin[2] + bbox->rangeMax[2]) * 0.5;
#endif
}

GfBBox3dRef GfBBox3d_Combine(GfBBox3dRef b1, GfBBox3dRef b2) {
    if (!b1 || !b2) return nullptr;
    try {
#if GF_USE_USD
        return new GfBBox3dOpaque(GfBBox3d::Combine(b1->bbox, b2->bbox));
#else
        // Simplified - union of ranges with identity matrix
        auto* result = new GfBBox3dOpaque();
        // Compute union of the two ranges
        for (int i = 0; i < 3; ++i) {
            result->rangeMin[i] = std::min(b1->rangeMin[i], b2->rangeMin[i]);
            result->rangeMax[i] = std::max(b1->rangeMax[i], b2->rangeMax[i]);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

bool GfBBox3d_Equal(GfBBox3dRef b1, GfBBox3dRef b2) {
    if (b1 == b2) return true;
    if (!b1 || !b2) return false;
#if GF_USE_USD
    return b1->bbox == b2->bbox;
#else
    // Compare ranges
    for (int i = 0; i < 3; ++i) {
        if (b1->rangeMin[i] != b2->rangeMin[i]) return false;
        if (b1->rangeMax[i] != b2->rangeMax[i]) return false;
    }
    // Compare matrices
    for (int i = 0; i < 16; ++i) {
        if (b1->matrix[i] != b2->matrix[i]) return false;
    }
    return true;
#endif
}

} // extern "C"
