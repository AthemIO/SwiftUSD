// KindCxx.swift - Swift extensions for Kind types via C++ interop
// Provides Swift-friendly API for swiftusd::KindRegistry and KindTokens

import USDCxx

// MARK: - Type Aliases

/// Type alias for KindRegistry from USDCxx
public typealias KindRegistryCxx = swiftusd.KindRegistry

/// Type alias for KindTokens from USDCxx
public typealias KindTokensCxx = swiftusd.KindTokens

// MARK: - Token Type Alias (for convenience)

/// Token type from USDCxx for use with Kind APIs
public typealias TfTokenCxx = swiftusd.Token

// MARK: - KindRegistry Extensions

extension swiftusd.KindRegistry: @unchecked Sendable {}

extension swiftusd.KindRegistry {
    /// Get all registered kinds as an array of Tokens.
    ///
    /// This includes built-in kinds and any custom kinds registered
    /// through plugInfo.json files.
    public static func getAllKindsArray() -> [swiftusd.Token] {
        let kinds = GetAllKinds()
        var result: [swiftusd.Token] = []
        // Convert std::vector to Swift array
        for i in 0..<kinds.size() {
            result.append(kinds[i])
        }
        return result
    }

    /// Get the base kind as an optional Token.
    ///
    /// Returns nil if the kind has no base or is unknown.
    public static func getBaseKindOptional(_ kind: swiftusd.Token) -> swiftusd.Token? {
        let base = GetBaseKind(kind)
        return base.IsEmpty() ? nil : base
    }

    /// Get the base kind from a string as an optional Token.
    ///
    /// Returns nil if the kind has no base or is unknown.
    public static func getBaseKindOptional(_ kind: String) -> swiftusd.Token? {
        let base = GetBaseKind(kind)
        return base.IsEmpty() ? nil : base
    }
}

// MARK: - KindTokens Extensions

extension swiftusd.KindTokens: @unchecked Sendable {}

extension swiftusd.KindTokens {
    /// The "model" kind token.
    public static var model: swiftusd.Token { Model() }

    /// The "component" kind token.
    public static var component: swiftusd.Token { Component() }

    /// The "group" kind token.
    public static var group: swiftusd.Token { Group() }

    /// The "assembly" kind token.
    public static var assembly: swiftusd.Token { Assembly() }

    /// The "subcomponent" kind token.
    public static var subcomponent: swiftusd.Token { Subcomponent() }
}

// MARK: - Token Extensions for Kind

extension swiftusd.Token {
    /// Check if this token represents a known kind.
    public var isKnownKind: Bool {
        swiftusd.KindRegistry.HasKind(self)
    }

    /// Get the base kind of this kind token.
    ///
    /// Returns nil if this kind has no base or is unknown.
    public var baseKind: swiftusd.Token? {
        swiftusd.KindRegistry.getBaseKindOptional(self)
    }

    /// Check if this kind is a model or derives from model.
    public var isModelKind: Bool {
        swiftusd.KindRegistry.IsModel(self)
    }

    /// Check if this kind is a group or derives from group.
    public var isGroupKind: Bool {
        swiftusd.KindRegistry.IsGroup(self)
    }

    /// Check if this kind is an assembly or derives from assembly.
    public var isAssemblyKind: Bool {
        swiftusd.KindRegistry.IsAssembly(self)
    }

    /// Check if this kind is a component or derives from component.
    public var isComponentKind: Bool {
        swiftusd.KindRegistry.IsComponent(self)
    }

    /// Check if this kind is a subcomponent or derives from subcomponent.
    public var isSubcomponentKind: Bool {
        swiftusd.KindRegistry.IsSubcomponent(self)
    }

    /// Check if this kind is the same as or derives from another kind.
    public func isA(_ baseKind: swiftusd.Token) -> Bool {
        swiftusd.KindRegistry.IsA(self, baseKind)
    }
}
