// Tf.h - Type Foundations for SwiftUSD
// Mirrors: pxr/base/tf/token.h
// C++ header with Swift interop annotations for TfToken

#pragma once

#include "../Swift.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/base/tf/token.h"
#endif

namespace swiftusd {

// ============================================================================
// MARK: - Token (TfToken equivalent)
// ============================================================================

/// An interned string token for efficient comparison and hashing.
///
/// Token is a lightweight handle to an interned string. Comparison and
/// hashing are O(1) operations. This mirrors TfToken from Pixar's USD.
///
/// Usage:
/// ```swift
/// let token = Token("myPrimName")
/// let isEmpty = token.IsEmpty()
/// let text = token.GetText()
/// ```
struct Token {
    // Storage for the string - in standalone mode we store a copy
    // In USE_PIXAR_USD mode, we wrap TfToken which handles interning
#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken _token;
#else
    std::string _text;
#endif

    // Constructors
    Token() SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _token()
#else
        : _text()
#endif
    {}

    Token(const char* text) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _token(text ? text : "")
#else
        : _text(text ? text : "")
#endif
    {}

    Token(const std::string& text) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _token(text)
#else
        : _text(text)
#endif
    {}

#if defined(USE_PIXAR_USD)
    Token(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT : _token(token) {}

    PXR_NS::TfToken GetTfToken() const SWIFTUSD_NOEXCEPT { return _token; }
#endif

    // Static factories
    static Token Empty() SWIFTUSD_NOEXCEPT { return Token(); }

    // Query methods
    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _token.IsEmpty();
#else
        return _text.empty();
#endif
    }

    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetText() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _token.GetText();
#else
        return _text.c_str();
#endif
    }

    size_t Hash() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _token.Hash();
#else
        return std::hash<std::string>{}(_text);
#endif
    }

    size_t size() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _token.size();
#else
        return _text.size();
#endif
    }

    // Comparison operators
    bool operator==(const Token& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _token == other._token;
#else
        return _text == other._text;
#endif
    }

    bool operator!=(const Token& other) const SWIFTUSD_NOEXCEPT {
        return !(*this == other);
    }

    bool operator<(const Token& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _token < other._token;
#else
        return _text < other._text;
#endif
    }

    bool operator<=(const Token& other) const SWIFTUSD_NOEXCEPT {
        return !(other < *this);
    }

    bool operator>(const Token& other) const SWIFTUSD_NOEXCEPT {
        return other < *this;
    }

    bool operator>=(const Token& other) const SWIFTUSD_NOEXCEPT {
        return !(*this < other);
    }

    // String comparison
    bool operator==(const char* text) const SWIFTUSD_NOEXCEPT {
        if (!text) return IsEmpty();
#if defined(USE_PIXAR_USD)
        return _token == text;
#else
        return _text == text;
#endif
    }

    bool operator!=(const char* text) const SWIFTUSD_NOEXCEPT {
        return !(*this == text);
    }

    // Implicit conversion to bool (true if non-empty)
    explicit operator bool() const SWIFTUSD_NOEXCEPT {
        return !IsEmpty();
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Check if a string is a valid identifier (follows C identifier rules)
inline bool IsValidIdentifier(const char* name) SWIFTUSD_NOEXCEPT {
    if (!name || !*name) return false;

    // First character must be letter or underscore
    char c = *name;
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')) {
        return false;
    }

    // Rest can be letters, digits, or underscores
    for (const char* p = name + 1; *p; ++p) {
        c = *p;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == '_')) {
            return false;
        }
    }

    return true;
}

} // namespace swiftusd
