// Mesh.hpp - UsdGeomMesh convenience wrapper for Swift/C++ interop
// Clean header - NO USD includes

#pragma once

// Include swift/bridging only when Swift is importing (not pure C++)
// __swift__ is defined by Swift's ClangImporter
#ifdef __swift__
#include <swift/bridging>
#else
// Fallback for pure C++ compilation - empty macros
#define SWIFT_SHARED_REFERENCE(retain, release)
#define SWIFT_RETURNS_RETAINED
#endif

#include <atomic>
#include "Prim.hpp"
#include "Types.hpp"

namespace pixarusd {
class Mesh;
} // namespace pixarusd

// Retain/release functions at global scope for Swift interop
void retainMesh(pixarusd::Mesh* m);
void releaseMesh(pixarusd::Mesh* m);

namespace pixarusd {

// ============================================================================
// MARK: - Mesh
// ============================================================================

/// Convenience wrapper for UsdGeomMesh prims.
/// Provides batch operations for efficient geometry access.
class Mesh {
    Prim* prim_;
    mutable std::atomic<int> refCount_{1};

public:
    /// Wrap an existing prim. Retains the prim.
    explicit Mesh(Prim* prim);
    ~Mesh();

    // Non-copyable
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // ========================================================================
    // Factory
    // ========================================================================

    /// Factory method - returns nullptr if prim isn't a Mesh.
    static Mesh* Get(Prim* prim) SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Validation
    // ========================================================================

    bool IsValid() const;

    /// Get the underlying prim.
    Prim* GetPrim() const SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Batch Geometry Access (Recommended)
    // ========================================================================

    /// Get all geometry data in a single call (efficient).
    MeshData GetGeometry(TimeCode time = TimeCode::Default()) const;

    /// Set all geometry data in a single call.
    bool SetGeometry(const MeshData& data, TimeCode time = TimeCode::Default());

    // ========================================================================
    // Individual Accessors
    // ========================================================================

    std::vector<Vec3f> GetPoints(TimeCode time = TimeCode::Default()) const;
    std::vector<Vec3f> GetNormals(TimeCode time = TimeCode::Default()) const;
    std::vector<int32_t> GetFaceVertexCounts() const;
    std::vector<int32_t> GetFaceVertexIndices() const;
    std::vector<Vec2f> GetUVs(TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Individual Setters
    // ========================================================================

    bool SetPoints(const std::vector<Vec3f>& points, TimeCode time = TimeCode::Default());
    bool SetNormals(const std::vector<Vec3f>& normals, TimeCode time = TimeCode::Default());
    bool SetFaceVertexCounts(const std::vector<int32_t>& counts);
    bool SetFaceVertexIndices(const std::vector<int32_t>& indices);
    bool SetUVs(const std::vector<Vec2f>& uvs, TimeCode time = TimeCode::Default());

    // ========================================================================
    // Computed Properties
    // ========================================================================

    /// Compute the bounding box of this mesh.
    BBox3d ComputeBoundingBox(TimeCode time = TimeCode::Default()) const;

    /// Get total vertex count.
    size_t GetPointCount(TimeCode time = TimeCode::Default()) const;

    /// Get total face count.
    size_t GetFaceCount() const;

    friend void ::retainMesh(Mesh* m);
    friend void ::releaseMesh(Mesh* m);
} SWIFT_SHARED_REFERENCE(retainMesh, releaseMesh);

} // namespace pixarusd
