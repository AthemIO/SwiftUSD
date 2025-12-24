// kind.h - Kind module C wrapper
// Mirrors: pxr/usd/kind/registry.h

#ifndef OPENUSDINTEROP_USD_KIND_H
#define OPENUSDINTEROP_USD_KIND_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations for cross-module types
#include "../base/tf.h"

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - KindRegistry
// ============================================================================

// Note: KindRegistry is a singleton in USD, so we don't need opaque handles.
// All functions operate on the global registry instance.

// MARK: - Kind Queries

/// Returns true if the given kind is known to the registry.
/// Unknown kinds return false but do not produce errors.
USD_INTEROP_API bool KindRegistry_HasKind(TfTokenRef kind);

/// Returns true if the given kind string is known to the registry.
USD_INTEROP_API bool KindRegistry_HasKindString(const char* kind);

/// Gets the base (parent) kind of a derived kind.
/// Returns NULL if the kind has no base kind or is unknown.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindRegistry_GetBaseKind(TfTokenRef kind);

/// Gets the base kind of a derived kind by string.
/// Returns NULL if the kind has no base kind or is unknown.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindRegistry_GetBaseKindString(const char* kind);

/// Returns true if derivedKind is or derives from baseKind.
/// Returns true if both kinds are equal, even if unknown.
/// Returns false if derivedKind doesn't derive from baseKind.
USD_INTEROP_API bool KindRegistry_IsA(TfTokenRef derivedKind, TfTokenRef baseKind);

/// Returns true if derivedKind string is or derives from baseKind string.
USD_INTEROP_API bool KindRegistry_IsAString(const char* derivedKind, const char* baseKind);

// MARK: - Built-in Kind Tests

/// Returns true if kind is a model or derives from model.
USD_INTEROP_API bool KindRegistry_IsModel(TfTokenRef kind);

/// Returns true if kind string is a model or derives from model.
USD_INTEROP_API bool KindRegistry_IsModelString(const char* kind);

/// Returns true if kind is a group or derives from group.
USD_INTEROP_API bool KindRegistry_IsGroup(TfTokenRef kind);

/// Returns true if kind string is a group or derives from group.
USD_INTEROP_API bool KindRegistry_IsGroupString(const char* kind);

/// Returns true if kind is an assembly or derives from assembly.
USD_INTEROP_API bool KindRegistry_IsAssembly(TfTokenRef kind);

/// Returns true if kind string is an assembly or derives from assembly.
USD_INTEROP_API bool KindRegistry_IsAssemblyString(const char* kind);

/// Returns true if kind is a component or derives from component.
USD_INTEROP_API bool KindRegistry_IsComponent(TfTokenRef kind);

/// Returns true if kind string is a component or derives from component.
USD_INTEROP_API bool KindRegistry_IsComponentString(const char* kind);

/// Returns true if kind is a subcomponent or derives from subcomponent.
USD_INTEROP_API bool KindRegistry_IsSubComponent(TfTokenRef kind);

/// Returns true if kind string is a subcomponent or derives from subcomponent.
USD_INTEROP_API bool KindRegistry_IsSubComponentString(const char* kind);

// MARK: - All Kinds Enumeration

/// Gets the count of all registered kinds.
USD_INTEROP_API size_t KindRegistry_GetAllKindsCount(void);

/// Gets all registered kinds.
/// outKinds must have space for maxCount tokens.
/// Returns the number of kinds written.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t KindRegistry_GetAllKinds(
    TfTokenRef* outKinds,
    size_t maxCount
);

// ============================================================================
// MARK: - KindTokens (Built-in Kind Constants)
// ============================================================================

/// Returns the "model" kind token.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindTokens_Model(void);

/// Returns the "component" kind token.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindTokens_Component(void);

/// Returns the "group" kind token.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindTokens_Group(void);

/// Returns the "assembly" kind token.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindTokens_Assembly(void);

/// Returns the "subcomponent" kind token.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef KindTokens_Subcomponent(void);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_KIND_H
