// Tf.swift - Tf module namespace
// Mirrors: pxr/base/tf (tokens, types, debugging, error handling)

import OpenUSDInterop

// MARK: - Tf Namespace

/// Type foundation utilities for tokens, types, and debugging.
/// Mirrors pxr::Tf from the USD C++ API.
///
/// The Tf module provides:
/// - `Token`: Efficient interned strings for constant-time comparison
///
/// ## Usage
/// ```swift
/// // Create tokens for efficient string comparison
/// let token = try Token("primName")
/// print(token.text)  // "primName"
/// ```
public enum Tf {
    // This namespace enum groups Tf-related utilities.
    // The primary type is Token, which is defined in Token.swift.
}
