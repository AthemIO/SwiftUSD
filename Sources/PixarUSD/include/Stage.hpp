// Stage.hpp - UsdStage wrapper for Swift/C++ interop
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
#include <atomic>
#include <vector>

namespace pixarusd {

// Forward declarations
namespace detail { struct StageImpl; }
class Prim;
class Stage;

} // namespace pixarusd

// Retain/release functions at global scope for Swift interop
void retainStage(pixarusd::Stage* s);
void releaseStage(pixarusd::Stage* s);

namespace pixarusd {

// ============================================================================
// MARK: - Stage
// ============================================================================

/// Wrapper around UsdStage with Swift-compatible reference counting.
/// Factory methods return nullptr on failure (Swift sees as Optional).
class Stage {
    detail::StageImpl* impl_;
    mutable std::atomic<int> refCount_{1};

    explicit Stage(detail::StageImpl* impl);

public:
    ~Stage();

    // Non-copyable
    Stage(const Stage&) = delete;
    Stage& operator=(const Stage&) = delete;

    // ========================================================================
    // Factory Methods - return nullptr on failure
    // ========================================================================

    /// Open an existing USD file. Returns nullptr if file doesn't exist or is invalid.
    static Stage* Open(const std::string& filePath) SWIFT_RETURNS_RETAINED;

    /// Create a new USD file. Returns nullptr if path is invalid.
    static Stage* CreateNew(const std::string& filePath) SWIFT_RETURNS_RETAINED;

    /// Create an anonymous in-memory stage.
    static Stage* CreateInMemory() SWIFT_RETURNS_RETAINED;

    // ========================================================================
    // Stage Operations
    // ========================================================================

    /// Save the stage to its root layer.
    bool Save() const;

    /// Export the stage to a file path.
    bool Export(const std::string& filePath) const;

    /// Export the stage to a string (USDA format).
    std::string ExportToString() const;

    // ========================================================================
    // Prim Access
    // ========================================================================

    /// Get the pseudo-root prim (parent of all root prims).
    Prim* GetPseudoRoot() const SWIFT_RETURNS_RETAINED;

    /// Get a prim at a specific path. Returns nullptr if not found.
    Prim* GetPrimAtPath(const std::string& path) const SWIFT_RETURNS_RETAINED;

    /// Define a new prim at path with optional type. Creates ancestors as needed.
    Prim* DefinePrim(const std::string& path, const std::string& typeName = "") SWIFT_RETURNS_RETAINED;

    /// Traverse all prims in the stage (depth-first).
    std::vector<Prim*> Traverse() const;

    // ========================================================================
    // Time Configuration
    // ========================================================================

    double GetStartTimeCode() const;
    double GetEndTimeCode() const;
    void SetStartTimeCode(double time);
    void SetEndTimeCode(double time);

    double GetFramesPerSecond() const;
    void SetFramesPerSecond(double fps);

    // ========================================================================
    // Advanced / Interop
    // ========================================================================

    /// Get raw pointer to underlying UsdStage for advanced C++ interop.
    /// WARNING: Do not hold this pointer beyond the Stage's lifetime.
    void* GetRawStagePtr() const;

    // Reference counting (called by Swift's ARC)
    friend void ::retainStage(Stage* s);
    friend void ::releaseStage(Stage* s);
} SWIFT_SHARED_REFERENCE(retainStage, releaseStage);

} // namespace pixarusd
