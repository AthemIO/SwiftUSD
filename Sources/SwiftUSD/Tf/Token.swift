// Token.swift - TfToken wrapper
// Mirrors: pxr/base/tf/token.h

import OpenUSDInterop

// MARK: - TfToken

/// Token for efficient comparison, assignment, and hashing of known strings.
///
/// A `Token` is a handle for a registered string, and can be compared,
/// assigned, and hashed in constant time. It is useful when a bounded number
/// of strings are used as fixed symbols (but never modified).
///
/// Mirrors `pxr::TfToken` from the USD C++ API.
///
/// - Note: Token creation involves a string hash and lookup in a global table,
///   so it should not be done more often than necessary. When possible, create
///   a token once and reuse it many times.
public final class Token: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: TfTokenRef

    // MARK: - Initialization

    /// Creates a token from the given string.
    ///
    /// - Parameter text: The string value for the token.
    /// - Throws: `TokenError.creationFailed` if the token cannot be created.
    public init(_ text: String) throws {
        guard let ref = TfToken_Create(text) else {
            throw TokenError.creationFailed("Failed to create token from '\(text)'")
        }
        self.handle = ref
    }

    /// Creates a token from a C string pointer with explicit length.
    ///
    /// Useful for strings that may contain null characters.
    ///
    /// - Parameters:
    ///   - buffer: Pointer to the string buffer.
    ///   - length: Number of bytes in the string.
    /// - Throws: `TokenError.creationFailed` if the token cannot be created.
    public init(buffer: UnsafePointer<CChar>, length: Int) throws {
        guard length >= 0 else {
            throw TokenError.invalidArgument("Length cannot be negative")
        }
        guard let ref = TfToken_CreateWithLength(buffer, length) else {
            throw TokenError.creationFailed("Failed to create token from buffer")
        }
        self.handle = ref
    }

    /// Creates an empty token (represents the empty string "").
    ///
    /// - Throws: `TokenError.creationFailed` if the token cannot be created.
    public init() throws {
        guard let ref = TfToken_CreateEmpty() else {
            throw TokenError.creationFailed("Failed to create empty token")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: TfTokenRef) {
        self.handle = handle
    }

    deinit {
        TfToken_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Creates an empty token.
    public static var empty: Token {
        get throws {
            try Token()
        }
    }

    /// Finds an existing token for the given string.
    ///
    /// If a token has previously been created for the given string, this returns it.
    /// Otherwise, returns `nil`.
    ///
    /// - Parameter text: The string to search for.
    /// - Returns: The existing token, or `nil` if not found.
    public static func find(_ text: String) -> Token? {
        guard let ref = TfToken_Find(text) else {
            return nil
        }
        return Token(handle: ref)
    }

    // MARK: - Properties

    /// The text that this token represents.
    ///
    /// - Note: For empty tokens, returns an empty string "".
    public var text: String {
        String(cString: TfToken_GetText(handle))
    }

    /// The number of characters in the token's string.
    public var count: Int {
        Int(TfToken_GetSize(handle))
    }

    /// Returns `true` if this token represents the empty string "".
    public var isEmpty: Bool {
        TfToken_IsEmpty(handle)
    }

    /// Returns a hash value for this token.
    ///
    /// The hash is based on the token's storage identity; this is immutable
    /// as long as the token is in use anywhere in the process.
    public var hashValue: Int {
        Int(truncatingIfNeeded: TfToken_Hash(handle))
    }

    // MARK: - Copying

    /// Creates an independent copy of this token.
    ///
    /// - Returns: A new token with the same text.
    /// - Throws: `TokenError.creationFailed` if the copy cannot be created.
    public func copy() throws -> Token {
        guard let ref = TfToken_Copy(handle) else {
            throw TokenError.creationFailed("Failed to copy token")
        }
        return Token(handle: ref)
    }
}

// MARK: - Equatable

extension Token: Equatable {

    public static func == (lhs: Token, rhs: Token) -> Bool {
        TfToken_Equal(lhs.handle, rhs.handle)
    }

    /// Checks if this token equals the given string.
    ///
    /// - Parameter string: The string to compare against.
    /// - Returns: `true` if the token's text equals the string.
    public func equals(_ string: String) -> Bool {
        TfToken_EqualString(handle, string)
    }
}

// MARK: - Comparable

extension Token: Comparable {

    public static func < (lhs: Token, rhs: Token) -> Bool {
        TfToken_Compare(lhs.handle, rhs.handle) < 0
    }
}

// MARK: - Hashable

extension Token: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(TfToken_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension Token: CustomStringConvertible {

    public var description: String {
        text
    }
}

// MARK: - CustomDebugStringConvertible

extension Token: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Token(\"\(text)\")"
    }
}

// MARK: - ExpressibleByStringLiteral

extension Token: ExpressibleByStringLiteral {

    public convenience init(stringLiteral value: String) {
        // For ExpressibleByStringLiteral, we must not throw.
        // If creation fails, we create an empty token as fallback.
        if let ref = TfToken_Create(value) {
            self.init(handle: ref)
        } else if let emptyRef = TfToken_CreateEmpty() {
            self.init(handle: emptyRef)
        } else {
            // This should never happen, but we need a valid handle
            fatalError("Failed to create token for string literal '\(value)'")
        }
    }
}

// MARK: - Collection Support

extension Token {

    /// Creates tokens from an array of strings.
    ///
    /// - Parameter strings: The strings to tokenize.
    /// - Returns: An array of tokens corresponding to the input strings.
    /// - Throws: `TokenError.creationFailed` if any token cannot be created.
    public static func tokens(from strings: [String]) throws -> [Token] {
        guard !strings.isEmpty else { return [] }

        // Simple implementation: create tokens one by one
        // This could be optimized with batch operations in the future
        var tokens = [Token]()
        tokens.reserveCapacity(strings.count)

        for string in strings {
            let token = try Token(string)
            tokens.append(token)
        }

        return tokens
    }

    /// Converts an array of tokens to an array of strings.
    ///
    /// - Parameter tokens: The tokens to convert.
    /// - Returns: An array of strings corresponding to the input tokens.
    public static func strings(from tokens: [Token]) -> [String] {
        tokens.map { $0.text }
    }
}

// MARK: - TokenError

/// Errors that can occur when working with tokens.
public enum TokenError: Error, CustomStringConvertible {

    /// Token creation failed.
    case creationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Token creation failed: \(message)"
        case .invalidArgument(let message):
            return "Invalid argument: \(message)"
        }
    }
}

// MARK: - Codable

extension Token: Codable {

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        let text = try container.decode(String.self)
        try self.init(text)
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encode(text)
    }
}
