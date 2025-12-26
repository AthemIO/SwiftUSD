// Tf.hpp - Token/Type Foundation facade
// CLEAN HEADER - NO pxr includes!
#pragma once

#include "../CxxFacade.h"

namespace cxxfacade {
namespace tf {

// ============================================================================
// TfToken - Interned string for efficient comparison and hashing
// ============================================================================
/// TfToken is a handle for a registered string, and can be compared,
/// assigned, and hashed in constant time. It is useful when a bounded number
/// of strings are used as fixed symbols (but never modified).
///
/// This is a pimpl wrapper around pxr::TfToken that hides USD headers from Swift.
/// The underlying implementation stores the token in a global table, making
/// string comparisons O(1) pointer comparisons.
class TfToken {
public:
    /// Create an empty token (containing the empty string "")
    TfToken();

    /// Create a token from a string
    ///
    /// This constructor involves a string hash and a lookup in the global
    /// table, so should not be done more often than necessary. When possible,
    /// create a token once and reuse it many times.
    explicit TfToken(const std::string& text);

    /// Create a token from a C string
    ///
    /// This constructor involves a string hash and a lookup in the global
    /// table, so should not be done more often than necessary.
    explicit TfToken(const char* text);

    /// Copy constructor
    TfToken(const TfToken& other);

    /// Move constructor
    TfToken(TfToken&& other) noexcept;

    /// Destructor
    ~TfToken();

    /// Copy assignment
    TfToken& operator=(const TfToken& other);

    /// Move assignment
    TfToken& operator=(TfToken&& other) noexcept;

    /// Get the string representation as a C string
    ///
    /// The returned pointer value is not valid after this TfToken
    /// object has been destroyed.
    const char* GetText() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    /// Get the string representation as std::string
    std::string GetString() const;

    /// Return the size of the string that this token represents
    size_t size() const;

    /// Check if the token is empty (contains empty string "")
    bool IsEmpty() const;

    /// Get the hash value for use in containers
    ///
    /// The hash is based on the token's storage identity; this is immutable
    /// as long as the token is in use anywhere in the process.
    size_t Hash() const;

    /// Equality comparison (O(1) pointer comparison)
    bool operator==(const TfToken& other) const;
    bool operator!=(const TfToken& other) const;

    /// Equality comparison with std::string (O(n) string comparison)
    bool operator==(const std::string& other) const;
    bool operator!=(const std::string& other) const;

    /// Equality comparison with C string (O(n) string comparison)
    bool operator==(const char* other) const;
    bool operator!=(const char* other) const;

    /// Less-than comparison for sorting (lexicographical)
    bool operator<(const TfToken& other) const;

    /// Greater-than comparison
    bool operator>(const TfToken& other) const;

    /// Less-than-or-equal comparison
    bool operator<=(const TfToken& other) const;

    /// Greater-than-or-equal comparison
    bool operator>=(const TfToken& other) const;

    /// Swap this token with another
    void Swap(TfToken& other);

    /// Find a token for the given string, if one exists
    ///
    /// If a token has previously been created for the given string, this
    /// will return it. Otherwise, an empty token will be returned.
    static TfToken Find(const std::string& s);

    /// Access the internal implementation (for CxxFacade internal use only)
    void* GetImpl() const { return impl_; }

    /// Create from existing impl (takes ownership)
    static TfToken FromImpl(void* impl);

private:
    void* impl_;  // Opaque pointer to pxr::TfToken
};

// Swap function for ADL
inline void swap(TfToken& lhs, TfToken& rhs) {
    lhs.Swap(rhs);
}

} // namespace tf
} // namespace cxxfacade
