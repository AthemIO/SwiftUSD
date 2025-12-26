// Prim.hpp - UsdPrim wrapper for Swift/C++ interop
// Clean header - NO USD includes, uses pimpl pattern

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

#include <string>
#include <vector>
#include <atomic>
#include "Types.hpp"

namespace pixarusd {

// Forward declarations
namespace detail { struct PrimImpl; }
class Stage;
class Prim;

} // namespace pixarusd

// Retain/release functions at global scope for Swift interop
void retainPrim(pixarusd::Prim* p);
void releasePrim(pixarusd::Prim* p);

namespace pixarusd {

// ============================================================================
// MARK: - Prim
// ============================================================================

/// Wrapper around UsdPrim with Swift-compatible reference counting.
/// Prims retain their owning Stage to prevent use-after-free.
class Prim {
    detail::PrimImpl* impl_;
    mutable std::atomic<int> refCount_{1};

public:
    explicit Prim(detail::PrimImpl* impl);
    ~Prim();

    // Non-copyable
    Prim(const Prim&) = delete;
    Prim& operator=(const Prim&) = delete;

    // ========================================================================
    // Identity
    // ========================================================================

    bool IsValid() const;
    std::string GetName() const;
    std::string GetPath() const;
    std::string GetTypeName() const;

    // ========================================================================
    // Type Checking
    // ========================================================================

    /// Check if prim is of a specific schema type (e.g., "Mesh", "Xform").
    bool IsA(const std::string& schemaType) const;

    /// Check if prim has a specific API schema applied.
    bool HasAPI(const std::string& apiSchemaType) const;

    // ========================================================================
    // Hierarchy
    // ========================================================================

    Prim* GetParent() const SWIFT_RETURNS_RETAINED;
    std::vector<Prim*> GetChildren() const;
    Prim* GetChild(const std::string& name) const SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Attributes - Generic
    // ========================================================================

    bool HasAttribute(const std::string& name) const;
    std::vector<std::string> GetAttributeNames() const;

    // ========================================================================
    // Typed Attribute Getters
    // ========================================================================

    bool GetBool(const std::string& name, TimeCode time = TimeCode::Default()) const;
    int32_t GetInt(const std::string& name, TimeCode time = TimeCode::Default()) const;
    float GetFloat(const std::string& name, TimeCode time = TimeCode::Default()) const;
    double GetDouble(const std::string& name, TimeCode time = TimeCode::Default()) const;
    std::string GetString(const std::string& name, TimeCode time = TimeCode::Default()) const;
    Vec3f GetVec3f(const std::string& name, TimeCode time = TimeCode::Default()) const;
    Vec3d GetVec3d(const std::string& name, TimeCode time = TimeCode::Default()) const;
    Matrix4d GetMatrix4d(const std::string& name, TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Array Attribute Getters
    // ========================================================================

    std::vector<Vec3f> GetVec3fArray(const std::string& name, TimeCode time = TimeCode::Default()) const;
    std::vector<Vec3d> GetVec3dArray(const std::string& name, TimeCode time = TimeCode::Default()) const;
    std::vector<int32_t> GetIntArray(const std::string& name, TimeCode time = TimeCode::Default()) const;
    std::vector<float> GetFloatArray(const std::string& name, TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Typed Attribute Setters
    // ========================================================================

    bool SetBool(const std::string& name, bool value, TimeCode time = TimeCode::Default());
    bool SetInt(const std::string& name, int32_t value, TimeCode time = TimeCode::Default());
    bool SetFloat(const std::string& name, float value, TimeCode time = TimeCode::Default());
    bool SetDouble(const std::string& name, double value, TimeCode time = TimeCode::Default());
    bool SetString(const std::string& name, const std::string& value, TimeCode time = TimeCode::Default());
    bool SetVec3f(const std::string& name, Vec3f value, TimeCode time = TimeCode::Default());
    bool SetVec3d(const std::string& name, Vec3d value, TimeCode time = TimeCode::Default());

    // ========================================================================
    // Array Attribute Setters
    // ========================================================================

    bool SetVec3fArray(const std::string& name, const std::vector<Vec3f>& values, TimeCode time = TimeCode::Default());
    bool SetIntArray(const std::string& name, const std::vector<int32_t>& values, TimeCode time = TimeCode::Default());
    bool SetFloatArray(const std::string& name, const std::vector<float>& values, TimeCode time = TimeCode::Default());

    // ========================================================================
    // Transform (for Xformable prims)
    // ========================================================================

    Matrix4d GetLocalTransform(TimeCode time = TimeCode::Default()) const;
    Matrix4d GetWorldTransform(TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Bounding Box
    // ========================================================================

    BBox3d ComputeWorldBound(TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Advanced / Interop
    // ========================================================================

    void* GetRawPrimPtr() const;

    friend void ::retainPrim(Prim* p);
    friend void ::releasePrim(Prim* p);
} SWIFT_SHARED_REFERENCE(retainPrim, releasePrim);

} // namespace pixarusd
