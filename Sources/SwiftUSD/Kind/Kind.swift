// Kind.swift - Model hierarchy kinds
// Mirrors: pxr/usd/kind/registry.h

import OpenUSDInterop

// MARK: - KindRegistry

/// Registry for model hierarchy kinds.
///
/// The KindRegistry manages the set of known kinds used in USD to classify
/// models and their components. Kinds form a hierarchy where:
/// - `model` is the root kind for models
/// - `component` and `group` are sub-kinds of `model`
/// - `assembly` is a sub-kind of `group`
/// - `subcomponent` is a standalone kind for parts below component level
///
/// The registry is a singleton and provides query-only access to the kind
/// hierarchy. Custom kinds can be registered through plugInfo.json files.
///
/// Mirrors `pxr::KindRegistry` from the USD C++ API.
public enum KindRegistry {

    // MARK: - Kind Queries

    /// Returns `true` if the given kind is known to the registry.
    ///
    /// Unknown kinds return `false` but do not produce errors.
    ///
    /// - Parameter kind: The kind token to check.
    /// - Returns: `true` if the kind is registered.
    public static func hasKind(_ kind: Token) -> Bool {
        KindRegistry_HasKind(kind.handle)
    }

    /// Returns `true` if the given kind string is known to the registry.
    ///
    /// - Parameter kind: The kind string to check.
    /// - Returns: `true` if the kind is registered.
    public static func hasKind(_ kind: String) -> Bool {
        KindRegistry_HasKindString(kind)
    }

    /// Gets the base (parent) kind of a derived kind.
    ///
    /// For example, `component` has base kind `model`, and `assembly` has
    /// base kind `group`.
    ///
    /// - Parameter kind: The kind to get the base kind for.
    /// - Returns: The base kind, or `nil` if the kind has no base or is unknown.
    public static func getBaseKind(_ kind: Token) -> Token? {
        guard let ref = KindRegistry_GetBaseKind(kind.handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Gets the base (parent) kind of a derived kind string.
    ///
    /// - Parameter kind: The kind string to get the base kind for.
    /// - Returns: The base kind, or `nil` if the kind has no base or is unknown.
    public static func getBaseKind(_ kind: String) -> Token? {
        guard let ref = KindRegistry_GetBaseKindString(kind) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Returns `true` if `derivedKind` is or derives from `baseKind`.
    ///
    /// This walks up the kind hierarchy checking if the derived kind
    /// eventually inherits from the base kind.
    ///
    /// - Parameters:
    ///   - derivedKind: The potentially derived kind.
    ///   - baseKind: The base kind to check against.
    /// - Returns: `true` if `derivedKind` equals or inherits from `baseKind`.
    public static func isA(_ derivedKind: Token, _ baseKind: Token) -> Bool {
        KindRegistry_IsA(derivedKind.handle, baseKind.handle)
    }

    /// Returns `true` if `derivedKind` string is or derives from `baseKind` string.
    ///
    /// - Parameters:
    ///   - derivedKind: The potentially derived kind string.
    ///   - baseKind: The base kind string to check against.
    /// - Returns: `true` if `derivedKind` equals or inherits from `baseKind`.
    public static func isA(_ derivedKind: String, _ baseKind: String) -> Bool {
        KindRegistry_IsAString(derivedKind, baseKind)
    }

    // MARK: - Built-in Kind Tests

    /// Returns `true` if the kind is a model or derives from model.
    ///
    /// This includes `model`, `component`, `group`, and `assembly`.
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: `true` if the kind is a model kind.
    public static func isModel(_ kind: Token) -> Bool {
        KindRegistry_IsModel(kind.handle)
    }

    /// Returns `true` if the kind string is a model or derives from model.
    ///
    /// - Parameter kind: The kind string to check.
    /// - Returns: `true` if the kind is a model kind.
    public static func isModel(_ kind: String) -> Bool {
        KindRegistry_IsModelString(kind)
    }

    /// Returns `true` if the kind is a group or derives from group.
    ///
    /// This includes `group` and `assembly`.
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: `true` if the kind is a group kind.
    public static func isGroup(_ kind: Token) -> Bool {
        KindRegistry_IsGroup(kind.handle)
    }

    /// Returns `true` if the kind string is a group or derives from group.
    ///
    /// - Parameter kind: The kind string to check.
    /// - Returns: `true` if the kind is a group kind.
    public static func isGroup(_ kind: String) -> Bool {
        KindRegistry_IsGroupString(kind)
    }

    /// Returns `true` if the kind is an assembly or derives from assembly.
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: `true` if the kind is an assembly kind.
    public static func isAssembly(_ kind: Token) -> Bool {
        KindRegistry_IsAssembly(kind.handle)
    }

    /// Returns `true` if the kind string is an assembly or derives from assembly.
    ///
    /// - Parameter kind: The kind string to check.
    /// - Returns: `true` if the kind is an assembly kind.
    public static func isAssembly(_ kind: String) -> Bool {
        KindRegistry_IsAssemblyString(kind)
    }

    /// Returns `true` if the kind is a component or derives from component.
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: `true` if the kind is a component kind.
    public static func isComponent(_ kind: Token) -> Bool {
        KindRegistry_IsComponent(kind.handle)
    }

    /// Returns `true` if the kind string is a component or derives from component.
    ///
    /// - Parameter kind: The kind string to check.
    /// - Returns: `true` if the kind is a component kind.
    public static func isComponent(_ kind: String) -> Bool {
        KindRegistry_IsComponentString(kind)
    }

    /// Returns `true` if the kind is a subcomponent or derives from subcomponent.
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: `true` if the kind is a subcomponent kind.
    public static func isSubComponent(_ kind: Token) -> Bool {
        KindRegistry_IsSubComponent(kind.handle)
    }

    /// Returns `true` if the kind string is a subcomponent or derives from subcomponent.
    ///
    /// - Parameter kind: The kind string to check.
    /// - Returns: `true` if the kind is a subcomponent kind.
    public static func isSubComponent(_ kind: String) -> Bool {
        KindRegistry_IsSubComponentString(kind)
    }

    // MARK: - All Kinds Enumeration

    /// Returns all registered kinds.
    ///
    /// This includes both built-in kinds and any custom kinds registered
    /// through plugInfo.json files.
    ///
    /// - Returns: An array of all registered kind tokens.
    public static func getAllKinds() -> [Token] {
        let count = KindRegistry_GetAllKindsCount()
        guard count > 0 else { return [] }

        var refs = [TfTokenRef?](repeating: nil, count: count)
        let written = refs.withUnsafeMutableBufferPointer { buffer in
            KindRegistry_GetAllKinds(buffer.baseAddress, count)
        }

        var tokens = [Token]()
        tokens.reserveCapacity(written)

        for i in 0..<written {
            if let ref = refs[i] {
                tokens.append(Token(handle: ref))
            }
        }

        return tokens
    }
}

// MARK: - KindTokens

/// Standard kind tokens for model hierarchy classification.
///
/// These are the built-in kinds defined by USD:
/// - `model`: Base kind for all model types
/// - `component`: A leaf model that doesn't contain other models
/// - `group`: A model that can contain other models
/// - `assembly`: A published group that can be referenced
/// - `subcomponent`: A part below the component level
///
/// Mirrors `pxr::KindTokens` from the USD C++ API.
public enum KindTokens {

    /// The "model" kind token.
    ///
    /// Models are prims that represent meaningful assets in a scene.
    /// This is the root kind from which `component` and `group` derive.
    public static var model: Token {
        guard let ref = KindTokens_Model() else {
            fatalError("Failed to create model kind token")
        }
        return Token(handle: ref)
    }

    /// The "component" kind token.
    ///
    /// Components are leaf models that don't contain other models.
    /// They represent individual assets like a chair or a lamp.
    public static var component: Token {
        guard let ref = KindTokens_Component() else {
            fatalError("Failed to create component kind token")
        }
        return Token(handle: ref)
    }

    /// The "group" kind token.
    ///
    /// Groups are models that can contain other models.
    /// They're used for organizing related models together.
    public static var group: Token {
        guard let ref = KindTokens_Group() else {
            fatalError("Failed to create group kind token")
        }
        return Token(handle: ref)
    }

    /// The "assembly" kind token.
    ///
    /// Assemblies are published groups that can be referenced.
    /// They represent complete, publishable assets like a room or a building.
    public static var assembly: Token {
        guard let ref = KindTokens_Assembly() else {
            fatalError("Failed to create assembly kind token")
        }
        return Token(handle: ref)
    }

    /// The "subcomponent" kind token.
    ///
    /// Subcomponents are parts below the component level.
    /// They represent pieces that don't stand alone as complete assets.
    public static var subcomponent: Token {
        guard let ref = KindTokens_Subcomponent() else {
            fatalError("Failed to create subcomponent kind token")
        }
        return Token(handle: ref)
    }
}
