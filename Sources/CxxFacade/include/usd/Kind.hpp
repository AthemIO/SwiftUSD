// Kind.hpp - Model hierarchy kinds for scene organization
// CLEAN HEADER - NO pxr includes!
//
// The Kind module provides a registry of "kinds" which are used to classify
// prims in a scene hierarchy. Kinds are organized into a hierarchy with
// "model" at the root, and specific types like "component", "group", and
// "assembly" as derived kinds.
//
// Kind Hierarchy:
//   model
//   ├── component (leaf models, geometry)
//   └── group (container models)
//       └── assembly (publishable group)
//   subcomponent (below component level)
//
#pragma once

#include "../CxxFacade.h"
#include "../base/Tf.hpp"

#include <vector>

namespace cxxfacade {
namespace kind {

// ============================================================================
// KindTokens - Built-in kind token constants
// ============================================================================
/// Static tokens for the built-in kinds.
///
/// These correspond to the standard USD kind hierarchy:
/// - model: Base kind for all models
/// - component: Leaf models containing geometry
/// - group: Container models
/// - assembly: Publishable group models
/// - subcomponent: Parts below the component level
namespace Tokens {
    /// Get the "model" kind token - base kind for all models
    tf::TfToken Model();

    /// Get the "component" kind token - leaf model with geometry
    tf::TfToken Component();

    /// Get the "group" kind token - container model
    tf::TfToken Group();

    /// Get the "assembly" kind token - publishable group
    tf::TfToken Assembly();

    /// Get the "subcomponent" kind token - below component level
    tf::TfToken SubComponent();
}

// ============================================================================
// KindRegistry - Free functions for querying the kind registry
// ============================================================================
/// The KindRegistry functions provide access to the singleton that holds known
/// kinds and information about their relationships.
///
/// The registry is initialized at startup with the core kinds (model,
/// component, group, assembly, subcomponent) and can be extended via
/// plugInfo.json files.
///
/// All functions are thread-safe for reading. The registry cannot be
/// modified at runtime.
///
/// ## Usage
/// ```cpp
/// // Check if a kind is registered
/// if (kind::HasKind(tf::TfToken("myKind"))) {
///     // ...
/// }
///
/// // Check kind relationships
/// if (kind::IsA(derivedKind, baseKind)) {
///     // derivedKind is same as or derived from baseKind
/// }
///
/// // Check for specific kind types
/// if (kind::IsModel(primKind)) {
///     // prim is some kind of model
/// }
/// ```

/// Test whether a kind is known to the registry.
///
/// @param kind The kind token to check.
/// @return True if the kind is registered.
bool HasKind(const tf::TfToken& kind);

/// Return the base kind of the given kind.
///
/// If there is no base (i.e., the kind is a root of the hierarchy),
/// returns an empty token.
///
/// @param kind The kind to query.
/// @return The base kind token, or empty if none.
tf::TfToken GetBaseKind(const tf::TfToken& kind);

/// Test whether derivedKind is the same as or derived from baseKind.
///
/// This checks if derivedKind has baseKind in its ancestor chain
/// (either directly or indirectly through parent kinds).
///
/// It is not required that either kind be registered: if they are
/// unknown but equal, returns true; otherwise if either is unknown,
/// returns false.
///
/// @param derivedKind The potential derived kind.
/// @param baseKind The potential base kind.
/// @return True if derivedKind is same as or derived from baseKind.
bool IsA(const tf::TfToken& derivedKind, const tf::TfToken& baseKind);

/// Return all kinds known to the registry.
///
/// The returned vector is unordered.
///
/// @return Vector of all registered kind tokens.
std::vector<tf::TfToken> GetAllKinds();

/// Returns true if kind is a model (i.e., IsA(kind, "model")).
///
/// @param kind The kind to check.
/// @return True if kind is any type of model.
bool IsModel(const tf::TfToken& kind);

/// Returns true if kind is a group (i.e., IsA(kind, "group")).
///
/// @param kind The kind to check.
/// @return True if kind is a group or derived from group.
bool IsGroup(const tf::TfToken& kind);

/// Returns true if kind is an assembly (i.e., IsA(kind, "assembly")).
///
/// @param kind The kind to check.
/// @return True if kind is an assembly or derived from assembly.
bool IsAssembly(const tf::TfToken& kind);

/// Returns true if kind is a component (i.e., IsA(kind, "component")).
///
/// @param kind The kind to check.
/// @return True if kind is a component or derived from component.
bool IsComponent(const tf::TfToken& kind);

/// Returns true if kind is a subcomponent (i.e., IsA(kind, "subcomponent")).
///
/// @param kind The kind to check.
/// @return True if kind is a subcomponent or derived from subcomponent.
bool IsSubComponent(const tf::TfToken& kind);

} // namespace kind
} // namespace cxxfacade
