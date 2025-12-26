// Xform.hpp - UsdGeomXform convenience wrapper for Swift/C++ interop
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
class Xform;
} // namespace pixarusd

// Retain/release functions at global scope for Swift interop
void retainXform(pixarusd::Xform* x);
void releaseXform(pixarusd::Xform* x);

namespace pixarusd {

// ============================================================================
// MARK: - Xform
// ============================================================================

/// Convenience wrapper for UsdGeomXform prims.
/// Provides transform operations with a simple API.
class Xform {
    Prim* prim_;
    mutable std::atomic<int> refCount_{1};

public:
    /// Wrap an existing prim. Retains the prim.
    explicit Xform(Prim* prim);
    ~Xform();

    // Non-copyable
    Xform(const Xform&) = delete;
    Xform& operator=(const Xform&) = delete;

    // ========================================================================
    // Factory
    // ========================================================================

    /// Factory method - returns nullptr if prim isn't Xformable.
    static Xform* Get(Prim* prim) SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Validation
    // ========================================================================

    bool IsValid() const;

    /// Get the underlying prim.
    Prim* GetPrim() const SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Transform Access
    // ========================================================================

    /// Get the local transform matrix.
    Matrix4d GetLocalTransform(TimeCode time = TimeCode::Default()) const;

    /// Get the world transform matrix (includes all ancestor transforms).
    Matrix4d GetWorldTransform(TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Transform Components
    // ========================================================================

    /// Get translation component.
    Vec3d GetTranslation(TimeCode time = TimeCode::Default()) const;

    /// Get rotation as quaternion.
    Quatf GetRotation(TimeCode time = TimeCode::Default()) const;

    /// Get scale component.
    Vec3d GetScale(TimeCode time = TimeCode::Default()) const;

    // ========================================================================
    // Transform Setters
    // ========================================================================

    /// Set translation. Creates xformOp:translate if needed.
    bool SetTranslation(const Vec3d& translation, TimeCode time = TimeCode::Default());

    /// Set rotation from quaternion. Creates xformOp:orient if needed.
    bool SetRotation(const Quatf& rotation, TimeCode time = TimeCode::Default());

    /// Set scale. Creates xformOp:scale if needed.
    bool SetScale(const Vec3d& scale, TimeCode time = TimeCode::Default());

    /// Set the complete local transform matrix.
    bool SetLocalTransform(const Matrix4d& matrix, TimeCode time = TimeCode::Default());

    // ========================================================================
    // Transform Stack
    // ========================================================================

    /// Clear all transform operations.
    bool ClearXformOpOrder();

    /// Reset transform to identity (clears ops and sets identity matrix).
    bool ResetToIdentity();

    friend void ::retainXform(Xform* x);
    friend void ::releaseXform(Xform* x);
} SWIFT_SHARED_REFERENCE(retainXform, releaseXform);

} // namespace pixarusd
