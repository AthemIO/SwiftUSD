// Types.hpp - Value types for Swift/C++ interop
// These are SWIFT_SELF_CONTAINED - passed by value, no reference counting needed

#pragma once

// Include swift/bridging only when Swift is importing (not pure C++)
// __swift__ is defined by Swift's ClangImporter
#ifdef __swift__
#include <swift/bridging>
#else
// Fallback for pure C++ compilation - empty macros
#define SWIFT_SELF_CONTAINED
#endif

#include <vector>
#include <cstdint>
#include <limits>

namespace pixarusd {

// ============================================================================
// MARK: - Vector Types
// ============================================================================

struct Vec2f {
    float x{0}, y{0};

    Vec2f() = default;
    Vec2f(float x_, float y_) : x(x_), y(y_) {}
} SWIFT_SELF_CONTAINED;

struct Vec3f {
    float x{0}, y{0}, z{0};

    Vec3f() = default;
    Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
} SWIFT_SELF_CONTAINED;

struct Vec3d {
    double x{0}, y{0}, z{0};

    Vec3d() = default;
    Vec3d(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
} SWIFT_SELF_CONTAINED;

struct Vec4f {
    float x{0}, y{0}, z{0}, w{0};

    Vec4f() = default;
    Vec4f(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Quaternion
// ============================================================================

struct Quatf {
    float w{1}, x{0}, y{0}, z{0};

    Quatf() = default;
    Quatf(float w_, float x_, float y_, float z_) : w(w_), x(x_), y(y_), z(z_) {}

    static Quatf Identity() { return Quatf{1, 0, 0, 0}; }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Matrix Types
// ============================================================================

struct Matrix4d {
    double m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    Matrix4d() = default;

    static Matrix4d Identity() { return Matrix4d{}; }

    // Access elements
    double& operator()(int row, int col) { return m[row][col]; }
    double operator()(int row, int col) const { return m[row][col]; }
} SWIFT_SELF_CONTAINED;

struct Matrix4f {
    float m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    Matrix4f() = default;

    static Matrix4f Identity() { return Matrix4f{}; }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Time
// ============================================================================

struct TimeCode {
    double value{0};

    TimeCode() = default;
    explicit TimeCode(double v) : value(v) {}

    static TimeCode Default() { return TimeCode{0}; }
    static TimeCode EarliestTime() { return TimeCode{-std::numeric_limits<double>::max()}; }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Geometry Data
// ============================================================================

struct MeshData {
    std::vector<Vec3f> points;
    std::vector<Vec3f> normals;
    std::vector<int32_t> faceVertexCounts;
    std::vector<int32_t> faceVertexIndices;
    std::vector<Vec2f> uvs;

    MeshData() = default;

    bool IsEmpty() const { return points.empty(); }
    size_t GetPointCount() const { return points.size(); }
    size_t GetFaceCount() const { return faceVertexCounts.size(); }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Bounding Box
// ============================================================================

struct BBox3d {
    Vec3d min{0, 0, 0};
    Vec3d max{0, 0, 0};

    BBox3d() = default;
    BBox3d(const Vec3d& min_, const Vec3d& max_) : min(min_), max(max_) {}

    Vec3d GetCenter() const {
        return Vec3d{
            (min.x + max.x) * 0.5,
            (min.y + max.y) * 0.5,
            (min.z + max.z) * 0.5
        };
    }

    Vec3d GetSize() const {
        return Vec3d{
            max.x - min.x,
            max.y - min.y,
            max.z - min.z
        };
    }
} SWIFT_SELF_CONTAINED;

} // namespace pixarusd
