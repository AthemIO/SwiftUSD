// Kind.swift - Model Hierarchy Kinds - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Kind types,
// which are used to classify prims in a scene hierarchy. Kinds are
// organized in a hierarchy with "model" at the root.
//
// Kind Hierarchy:
//   model
//   ├── component (leaf models with geometry)
//   └── group (container models)
//       └── assembly (publishable group)
//   subcomponent (below component level)

import CxxFacade

// MARK: - Kind Token Constants

/// Standard kind tokens used in USD for model hierarchy classification.
///
/// The kind hierarchy is:
/// - `model`: Base kind for all models
/// - `component`: Leaf models containing geometry (derived from model)
/// - `group`: Container models (derived from model)
/// - `assembly`: Publishable group models (derived from group)
/// - `subcomponent`: Parts below the component level (not a model)
public enum KindTokens {
    /// The "model" kind - base kind for all models.
    ///
    /// A model is a meaningful grouping of scene description that
    /// can be referenced, overridden, and organized in a hierarchy.
    public static var model: TfToken {
        return cxxfacade.kind.Tokens.Model()
    }

    /// The "component" kind - leaf model containing geometry.
    ///
    /// Components are the smallest publishable models. They typically
    /// contain the actual geometric data and cannot be further
    /// subdivided into meaningful models.
    public static var component: TfToken {
        return cxxfacade.kind.Tokens.Component()
    }

    /// The "group" kind - container model.
    ///
    /// Groups are models that organize other models. They don't
    /// directly contain geometry but serve as organizational units
    /// in the scene hierarchy.
    public static var group: TfToken {
        return cxxfacade.kind.Tokens.Group()
    }

    /// The "assembly" kind - publishable group.
    ///
    /// Assemblies are groups that represent a complete, publishable
    /// collection of models. They're often used as the top-level
    /// container for a complete asset or set.
    public static var assembly: TfToken {
        return cxxfacade.kind.Tokens.Assembly()
    }

    /// The "subcomponent" kind - below component level.
    ///
    /// Subcomponents are organizational units within a component.
    /// They are not considered models themselves and are below
    /// the component level in the hierarchy.
    public static var subcomponent: TfToken {
        return cxxfacade.kind.Tokens.SubComponent()
    }
}

// MARK: - KindRegistry

/// KindRegistry provides access to the singleton that holds known kinds
/// and information about their relationships.
///
/// The registry is initialized at startup with the core kinds (model,
/// component, group, assembly, subcomponent) and can be extended via
/// plugInfo.json files.
///
/// ## Usage
/// ```swift
/// // Check if a kind is registered
/// if KindRegistry.hasKind(myKind) {
///     // ...
/// }
///
/// // Check kind relationships
/// if KindRegistry.isA(derivedKind, baseKind: baseKind) {
///     // derivedKind is same as or derived from baseKind
/// }
///
/// // Check for specific kind types
/// if KindRegistry.isModel(primKind) {
///     // prim is some kind of model
/// }
/// ```
public enum KindRegistry {
    /// Test whether a kind is known to the registry.
    ///
    /// - Parameter kind: The kind token to check.
    /// - Returns: True if the kind is registered.
    public static func hasKind(_ kind: TfToken) -> Bool {
        return cxxfacade.kind.HasKind(kind)
    }

    /// Return the base kind of the given kind.
    ///
    /// If there is no base (i.e., the kind is a root of the hierarchy),
    /// returns nil.
    ///
    /// - Parameter kind: The kind to query.
    /// - Returns: The base kind token, or nil if none.
    public static func baseKind(of kind: TfToken) -> TfToken? {
        let base = cxxfacade.kind.GetBaseKind(kind)
        return base.IsEmpty() ? nil : base
    }

    /// Test whether derivedKind is the same as or derived from baseKind.
    ///
    /// This checks if derivedKind has baseKind in its ancestor chain
    /// (either directly or indirectly through parent kinds).
    ///
    /// It is not required that either kind be registered: if they are
    /// unknown but equal, returns true; otherwise if either is unknown,
    /// returns false.
    ///
    /// - Parameters:
    ///   - derivedKind: The potential derived kind.
    ///   - baseKind: The potential base kind.
    /// - Returns: True if derivedKind is same as or derived from baseKind.
    public static func isA(_ derivedKind: TfToken, baseKind: TfToken) -> Bool {
        return cxxfacade.kind.IsA(derivedKind, baseKind)
    }

    /// Return all kinds known to the registry.
    ///
    /// - Returns: Array of all registered kind tokens.
    public static func allKinds() -> [TfToken] {
        let kinds = cxxfacade.kind.GetAllKinds()
        var result: [TfToken] = []
        for i in 0..<kinds.size() {
            result.append(kinds[i])
        }
        return result
    }

    /// Returns true if kind is a model (i.e., IsA(kind, "model")).
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: True if kind is any type of model.
    public static func isModel(_ kind: TfToken) -> Bool {
        return cxxfacade.kind.IsModel(kind)
    }

    /// Returns true if kind is a group (i.e., IsA(kind, "group")).
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: True if kind is a group or derived from group.
    public static func isGroup(_ kind: TfToken) -> Bool {
        return cxxfacade.kind.IsGroup(kind)
    }

    /// Returns true if kind is an assembly (i.e., IsA(kind, "assembly")).
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: True if kind is an assembly or derived from assembly.
    public static func isAssembly(_ kind: TfToken) -> Bool {
        return cxxfacade.kind.IsAssembly(kind)
    }

    /// Returns true if kind is a component (i.e., IsA(kind, "component")).
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: True if kind is a component or derived from component.
    public static func isComponent(_ kind: TfToken) -> Bool {
        return cxxfacade.kind.IsComponent(kind)
    }

    /// Returns true if kind is a subcomponent (i.e., IsA(kind, "subcomponent")).
    ///
    /// - Parameter kind: The kind to check.
    /// - Returns: True if kind is a subcomponent or derived from subcomponent.
    public static func isSubComponent(_ kind: TfToken) -> Bool {
        return cxxfacade.kind.IsSubComponent(kind)
    }
}

// MARK: - TfToken Kind Extensions

extension TfToken {
    /// Check if this token represents a model kind.
    ///
    /// Returns true if this kind is "model" or derived from it.
    public var isModelKind: Bool {
        return KindRegistry.isModel(self)
    }

    /// Check if this token represents a group kind.
    ///
    /// Returns true if this kind is "group" or derived from it.
    public var isGroupKind: Bool {
        return KindRegistry.isGroup(self)
    }

    /// Check if this token represents an assembly kind.
    ///
    /// Returns true if this kind is "assembly" or derived from it.
    public var isAssemblyKind: Bool {
        return KindRegistry.isAssembly(self)
    }

    /// Check if this token represents a component kind.
    ///
    /// Returns true if this kind is "component" or derived from it.
    public var isComponentKind: Bool {
        return KindRegistry.isComponent(self)
    }

    /// Check if this token represents a subcomponent kind.
    ///
    /// Returns true if this kind is "subcomponent" or derived from it.
    public var isSubComponentKind: Bool {
        return KindRegistry.isSubComponent(self)
    }

    /// Get the base kind of this kind, if any.
    ///
    /// Returns nil if this kind is a root or not registered.
    public var baseKind: TfToken? {
        return KindRegistry.baseKind(of: self)
    }

    /// Check if this kind is the same as or derived from another kind.
    ///
    /// - Parameter baseKind: The potential base kind.
    /// - Returns: True if this kind is same as or derived from baseKind.
    public func isA(baseKind: TfToken) -> Bool {
        return KindRegistry.isA(self, baseKind: baseKind)
    }
}
