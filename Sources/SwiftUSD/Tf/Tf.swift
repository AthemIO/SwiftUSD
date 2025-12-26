// Tf.swift - Token/Type Foundation - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Tf types,
// particularly TfToken - interned strings used as identifiers throughout USD.

import CxxFacade

// MARK: - TfToken Swift Type Alias

/// TfToken - Interned string for efficient comparison and hashing.
///
/// A TfToken is a handle for a registered string, and can be compared,
/// assigned, and hashed in constant time. It is useful when a bounded number
/// of strings are used as fixed symbols (but never modified).
///
/// For example, the set of property names in a USD scene is large but bounded,
/// and once discovered, they are never manipulated. If these names were passed
/// around as strings, every comparison and hash would be linear in the number
/// of characters. Using TfToken, these operations are O(1).
///
/// ## Usage
/// ```swift
/// // Create tokens from strings
/// let token = TfToken("myProperty")
///
/// // Empty token
/// let empty = TfToken()
///
/// // Efficient comparison
/// if token1 == token2 {
///     // O(1) pointer comparison
/// }
/// ```
///
/// ## Performance Notes
/// - Token creation involves a hash and global table lookup - do this once and reuse
/// - Token comparison is O(1) - feel free to compare as often as needed
/// - Token hashing is O(1) - suitable for use in dictionaries and sets
public typealias TfToken = cxxfacade.tf.TfToken

// MARK: - TfToken Extensions

extension TfToken: CustomStringConvertible {
    /// A textual representation of the token.
    public var description: String {
        return String(cString: self.GetText())
    }
}

extension TfToken: CustomDebugStringConvertible {
    /// A debug textual representation of the token.
    public var debugDescription: String {
        if self.IsEmpty() {
            return "TfToken(\"\")"
        }
        return "TfToken(\"\(String(cString: self.GetText()))\")"
    }
}

extension TfToken: Hashable {
    /// Hashes the essential components of this value.
    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.Hash())
    }
}

extension TfToken: ExpressibleByStringLiteral {
    /// Creates a token from a string literal.
    ///
    /// This allows you to write:
    /// ```swift
    /// let token: TfToken = "myProperty"
    /// ```
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension TfToken {
    /// Creates a token from a Swift String.
    ///
    /// - Parameter string: The string to create a token from.
    public init(_ string: String) {
        self.init(std.string(string))
    }

    /// The string value of this token as a Swift String.
    public var text: String {
        return String(cString: self.GetText())
    }

    /// The length of the token's string representation.
    public var count: Int {
        return Int(self.size())
    }

    /// Whether this token represents an empty string.
    public var isEmpty: Bool {
        return self.IsEmpty()
    }

    /// Creates an empty token.
    public static var empty: TfToken {
        return TfToken()
    }

    /// Finds an existing token for the given string.
    ///
    /// If a token has previously been created for the given string, this
    /// returns it. Otherwise, returns nil.
    ///
    /// - Parameter string: The string to look up.
    /// - Returns: The existing token, or nil if not found.
    public static func find(_ string: String) -> TfToken? {
        let result = TfToken.Find(std.string(string))
        return result.IsEmpty() ? nil : result
    }
}

// MARK: - Common Token Constants

extension TfToken {
    /// Common token values used throughout USD.
    public enum Common {
        /// The empty token.
        nonisolated(unsafe) public static let empty = TfToken()
    }
}
