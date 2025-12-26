// Kind.h - Model hierarchy kinds for SwiftUSD
// Mirrors: pxr/usd/kind/registry.h
// C++ header with Swift interop annotations for KindRegistry

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/kind/registry.h"
#endif

namespace swiftusd {

// ============================================================================
// MARK: - KindTokens (Built-in kind tokens)
// ============================================================================

/// Static tokens for the built-in USD kinds.
///
/// The kind hierarchy is:
/// - model (root for all model types)
///   - component (leaf models that don't contain other models)
///   - group (models that can contain other models)
///     - assembly (published groups that can be referenced)
/// - subcomponent (standalone, parts below component level)
struct KindTokens {
    /// The "model" kind - base for all model types.
    static Token Model() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(PXR_NS::KindTokens->model);
#else
        return Token("model");
#endif
    }

    /// The "component" kind - leaf models.
    static Token Component() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(PXR_NS::KindTokens->component);
#else
        return Token("component");
#endif
    }

    /// The "group" kind - models containing other models.
    static Token Group() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(PXR_NS::KindTokens->group);
#else
        return Token("group");
#endif
    }

    /// The "assembly" kind - published groups.
    static Token Assembly() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(PXR_NS::KindTokens->assembly);
#else
        return Token("assembly");
#endif
    }

    /// The "subcomponent" kind - parts below component level.
    static Token Subcomponent() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(PXR_NS::KindTokens->subcomponent);
#else
        return Token("subcomponent");
#endif
    }
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - KindRegistry (Singleton for kind queries)
// ============================================================================

/// Registry for querying model hierarchy kinds.
///
/// KindRegistry is a singleton that holds known kinds and information about
/// their inheritance relationships. The registry is read-only; all extensions
/// must be accomplished via plugInfo.json files.
///
/// The built-in kind hierarchy is:
/// ```
/// model
///   component
///   group
///     assembly
/// subcomponent (standalone)
/// ```
struct KindRegistry {

    /// Test whether a kind is known to the registry.
    ///
    /// Unknown kinds return false but do not produce errors.
    static bool HasKind(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::HasKind(kind.GetTfToken());
#else
        return _GetStandaloneRegistry().count(std::string(kind.GetText())) > 0;
#endif
    }

    /// Test whether a kind string is known to the registry.
    static bool HasKind(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return false;
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::HasKind(PXR_NS::TfToken(kind));
#else
        return _GetStandaloneRegistry().count(kind) > 0;
#endif
    }

    /// Return the base kind of the given kind.
    ///
    /// If there is no base, returns an empty token.
    /// Returns empty token if kind is unknown.
    static Token GetBaseKind(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!HasKind(kind)) return Token();
        return Token(PXR_NS::KindRegistry::GetBaseKind(kind.GetTfToken()));
#else
        auto& registry = _GetStandaloneRegistry();
        auto it = registry.find(std::string(kind.GetText()));
        if (it == registry.end()) return Token();
        return Token(it->second.c_str());
#endif
    }

    /// Return the base kind of the given kind string.
    static Token GetBaseKind(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return Token();
        return GetBaseKind(Token(kind));
    }

    /// Test whether derivedKind is the same as baseKind or has it as a base.
    ///
    /// This walks up the kind hierarchy checking if the derived kind
    /// eventually inherits from the base kind.
    ///
    /// It is not required that either kind be known to the registry:
    /// if they are unknown but equal, returns true; otherwise if either
    /// is unknown, returns false.
    static bool IsA(const Token& derivedKind, const Token& baseKind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::IsA(derivedKind.GetTfToken(), baseKind.GetTfToken());
#else
        // Same kind
        if (derivedKind == baseKind) return true;

        // Walk up the hierarchy
        auto& registry = _GetStandaloneRegistry();
        std::string current = derivedKind.GetText();

        while (!current.empty()) {
            auto it = registry.find(current);
            if (it == registry.end()) return false;

            if (it->second.empty()) return false;

            if (it->second == baseKind.GetText()) return true;
            current = it->second;
        }

        return false;
#endif
    }

    /// Test whether derivedKind string is the same as baseKind string or has it as a base.
    static bool IsA(const char* derivedKind, const char* baseKind) SWIFTUSD_NOEXCEPT {
        if (!derivedKind || !baseKind) return false;
        return IsA(Token(derivedKind), Token(baseKind));
    }

    /// Returns true if kind IsA model kind.
    static bool IsModel(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::IsModel(kind.GetTfToken());
#else
        return IsA(kind, KindTokens::Model());
#endif
    }

    /// Returns true if kind string IsA model kind.
    static bool IsModel(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return false;
        return IsModel(Token(kind));
    }

    /// Returns true if kind IsA group kind.
    static bool IsGroup(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::IsGroup(kind.GetTfToken());
#else
        return IsA(kind, KindTokens::Group());
#endif
    }

    /// Returns true if kind string IsA group kind.
    static bool IsGroup(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return false;
        return IsGroup(Token(kind));
    }

    /// Returns true if kind IsA assembly kind.
    static bool IsAssembly(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::IsAssembly(kind.GetTfToken());
#else
        return IsA(kind, KindTokens::Assembly());
#endif
    }

    /// Returns true if kind string IsA assembly kind.
    static bool IsAssembly(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return false;
        return IsAssembly(Token(kind));
    }

    /// Returns true if kind IsA component kind.
    static bool IsComponent(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::IsComponent(kind.GetTfToken());
#else
        return IsA(kind, KindTokens::Component());
#endif
    }

    /// Returns true if kind string IsA component kind.
    static bool IsComponent(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return false;
        return IsComponent(Token(kind));
    }

    /// Returns true if kind IsA subcomponent kind.
    static bool IsSubcomponent(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::IsSubComponent(kind.GetTfToken());
#else
        return IsA(kind, KindTokens::Subcomponent());
#endif
    }

    /// Returns true if kind string IsA subcomponent kind.
    static bool IsSubcomponent(const char* kind) SWIFTUSD_NOEXCEPT {
        if (!kind) return false;
        return IsSubcomponent(Token(kind));
    }

    /// Return the count of all registered kinds.
    static size_t GetAllKindsCount() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PXR_NS::KindRegistry::GetAllKinds().size();
#else
        return _GetStandaloneRegistry().size();
#endif
    }

    /// Return all registered kinds as a vector of tokens.
    static std::vector<Token> GetAllKinds() SWIFTUSD_NOEXCEPT {
        std::vector<Token> result;
#if defined(USE_PIXAR_USD)
        auto allKinds = PXR_NS::KindRegistry::GetAllKinds();
        result.reserve(allKinds.size());
        for (const auto& k : allKinds) {
            result.push_back(Token(k));
        }
#else
        auto& registry = _GetStandaloneRegistry();
        result.reserve(registry.size());
        for (const auto& entry : registry) {
            result.push_back(Token(entry.first));
        }
#endif
        return result;
    }

private:
#if !defined(USE_PIXAR_USD)
    // Standalone registry mapping kind -> baseKind
    // Empty string means no base kind
    using RegistryMap = std::unordered_map<std::string, std::string>;

    static RegistryMap& _GetStandaloneRegistry() SWIFTUSD_NOEXCEPT {
        static RegistryMap registry = {
            {"model", ""},             // model has no base
            {"component", "model"},    // component derives from model
            {"group", "model"},        // group derives from model
            {"assembly", "group"},     // assembly derives from group
            {"subcomponent", ""}       // subcomponent has no base (standalone)
        };
        return registry;
    }
#endif

} SWIFT_SELF_CONTAINED;

} // namespace swiftusd
