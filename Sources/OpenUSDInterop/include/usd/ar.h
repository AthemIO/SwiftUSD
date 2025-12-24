// ar.h - ArResolver, ArResolverContext, ArResolvedPath, ArTimestamp C wrapper
// Mirrors: pxr/usd/ar/resolver.h, pxr/usd/ar/resolverContext.h, pxr/usd/ar/resolvedPath.h

#ifndef OPENUSDINTEROP_USD_AR_H
#define OPENUSDINTEROP_USD_AR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
// MARK: - ArResolvedPath
// ============================================================================

/// Opaque handle to an ArResolvedPath wrapper.
/// Represents a resolved asset path.
typedef struct ArResolvedPathOpaque* ArResolvedPathRef;

// MARK: - ArResolvedPath Lifecycle

/// Creates an ArResolvedPath from a resolved path string.
/// Returns NULL on failure. Caller must release with ArResolvedPath_Release.
USD_INTEROP_API ArResolvedPathRef ArResolvedPath_Create(const char* resolvedPath);

/// Creates an empty ArResolvedPath.
/// Returns NULL on failure. Caller must release with ArResolvedPath_Release.
USD_INTEROP_API ArResolvedPathRef ArResolvedPath_CreateEmpty(void);

/// Creates a copy of an existing ArResolvedPath.
/// Returns NULL on failure. Caller must release with ArResolvedPath_Release.
USD_INTEROP_API ArResolvedPathRef ArResolvedPath_Copy(ArResolvedPathRef path);

/// Increments the reference count of an ArResolvedPath.
USD_INTEROP_API ArResolvedPathRef ArResolvedPath_Retain(ArResolvedPathRef path);

/// Decrements the reference count and frees if count reaches zero.
USD_INTEROP_API void ArResolvedPath_Release(ArResolvedPathRef path);

// MARK: - ArResolvedPath Properties

/// Returns the path string.
/// The returned string is valid as long as the path is valid.
USD_INTEROP_API const char* ArResolvedPath_GetPathString(ArResolvedPathRef path);

/// Returns true if the path is empty.
USD_INTEROP_API bool ArResolvedPath_IsEmpty(ArResolvedPathRef path);

/// Returns a hash value for the path.
USD_INTEROP_API size_t ArResolvedPath_Hash(ArResolvedPathRef path);

// MARK: - ArResolvedPath Comparison

/// Returns true if two resolved paths are equal.
USD_INTEROP_API bool ArResolvedPath_Equal(ArResolvedPathRef lhs, ArResolvedPathRef rhs);

/// Compares two resolved paths lexicographically.
USD_INTEROP_API int ArResolvedPath_Compare(ArResolvedPathRef lhs, ArResolvedPathRef rhs);

// ============================================================================
// MARK: - ArTimestamp
// ============================================================================

/// Represents a timestamp for an asset (Unix time in seconds).
/// An invalid timestamp is represented by NaN.
typedef struct {
    double time;
} ArTimestamp;

/// Creates an invalid timestamp.
USD_INTEROP_API ArTimestamp ArTimestamp_CreateInvalid(void);

/// Creates a timestamp from Unix time (seconds since epoch).
USD_INTEROP_API ArTimestamp ArTimestamp_Create(double time);

/// Returns true if the timestamp is valid.
USD_INTEROP_API bool ArTimestamp_IsValid(ArTimestamp timestamp);

/// Returns the time value. Returns NaN if invalid.
USD_INTEROP_API double ArTimestamp_GetTime(ArTimestamp timestamp);

/// Compares two timestamps. Returns < 0 if lhs < rhs, 0 if equal, > 0 if lhs > rhs.
/// Invalid timestamps are considered less than valid ones.
USD_INTEROP_API int ArTimestamp_Compare(ArTimestamp lhs, ArTimestamp rhs);

/// Returns true if two timestamps are equal.
USD_INTEROP_API bool ArTimestamp_Equal(ArTimestamp lhs, ArTimestamp rhs);

// ============================================================================
// MARK: - ArResolverContext
// ============================================================================

/// Opaque handle to an ArResolverContext wrapper.
/// An asset resolver context allows clients to provide additional data
/// to the resolver for use during resolution.
typedef struct ArResolverContextOpaque* ArResolverContextRef;

// MARK: - ArResolverContext Lifecycle

/// Creates an empty ArResolverContext.
/// Returns NULL on failure. Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef ArResolverContext_CreateEmpty(void);

/// Creates a copy of an existing ArResolverContext.
/// Returns NULL on failure. Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef ArResolverContext_Copy(ArResolverContextRef context);

/// Increments the reference count of an ArResolverContext.
USD_INTEROP_API ArResolverContextRef ArResolverContext_Retain(ArResolverContextRef context);

/// Decrements the reference count and frees if count reaches zero.
USD_INTEROP_API void ArResolverContext_Release(ArResolverContextRef context);

// MARK: - ArResolverContext Properties

/// Returns true if the context is empty.
USD_INTEROP_API bool ArResolverContext_IsEmpty(ArResolverContextRef context);

/// Returns a debug string representation of the context.
/// Caller must free the result with ArResolverContext_FreeString.
USD_INTEROP_API char* ArResolverContext_GetDebugString(ArResolverContextRef context);

/// Frees a string allocated by ArResolverContext functions.
USD_INTEROP_API void ArResolverContext_FreeString(char* str);

/// Returns a hash value for the context.
USD_INTEROP_API size_t ArResolverContext_Hash(ArResolverContextRef context);

// MARK: - ArResolverContext Comparison

/// Returns true if two contexts are equal.
USD_INTEROP_API bool ArResolverContext_Equal(ArResolverContextRef lhs, ArResolverContextRef rhs);

/// Compares two contexts. Returns < 0 if lhs < rhs, 0 if equal, > 0 if lhs > rhs.
USD_INTEROP_API int ArResolverContext_Compare(ArResolverContextRef lhs, ArResolverContextRef rhs);

// ============================================================================
// MARK: - ArDefaultResolverContext
// ============================================================================

/// Opaque handle to an ArDefaultResolverContext wrapper.
/// Resolver context that specifies search paths for the default resolver.
typedef struct ArDefaultResolverContextOpaque* ArDefaultResolverContextRef;

// MARK: - ArDefaultResolverContext Lifecycle

/// Creates an ArDefaultResolverContext with no search paths.
/// Returns NULL on failure. Caller must release with ArDefaultResolverContext_Release.
USD_INTEROP_API ArDefaultResolverContextRef ArDefaultResolverContext_Create(void);

/// Creates an ArDefaultResolverContext with the given search paths.
/// Returns NULL on failure. Caller must release with ArDefaultResolverContext_Release.
USD_INTEROP_API ArDefaultResolverContextRef ArDefaultResolverContext_CreateWithSearchPaths(
    const char* const* searchPaths,
    size_t count
);

/// Creates a copy of an existing ArDefaultResolverContext.
/// Returns NULL on failure. Caller must release with ArDefaultResolverContext_Release.
USD_INTEROP_API ArDefaultResolverContextRef ArDefaultResolverContext_Copy(ArDefaultResolverContextRef context);

/// Increments the reference count of an ArDefaultResolverContext.
USD_INTEROP_API ArDefaultResolverContextRef ArDefaultResolverContext_Retain(ArDefaultResolverContextRef context);

/// Decrements the reference count and frees if count reaches zero.
USD_INTEROP_API void ArDefaultResolverContext_Release(ArDefaultResolverContextRef context);

// MARK: - ArDefaultResolverContext Properties

/// Returns the number of search paths.
USD_INTEROP_API size_t ArDefaultResolverContext_GetSearchPathCount(ArDefaultResolverContextRef context);

/// Gets the search paths.
/// outPaths must have space for maxCount strings.
/// Returns the number of paths written.
/// Caller must free each string with free().
USD_INTEROP_API size_t ArDefaultResolverContext_GetSearchPaths(
    ArDefaultResolverContextRef context,
    char** outPaths,
    size_t maxCount
);

/// Returns a string representation for debugging.
/// Caller must free the result with ArDefaultResolverContext_FreeString.
USD_INTEROP_API char* ArDefaultResolverContext_GetAsString(ArDefaultResolverContextRef context);

/// Frees a string allocated by ArDefaultResolverContext functions.
USD_INTEROP_API void ArDefaultResolverContext_FreeString(char* str);

/// Returns a hash value for the context.
USD_INTEROP_API size_t ArDefaultResolverContext_Hash(ArDefaultResolverContextRef context);

// MARK: - ArDefaultResolverContext Comparison

/// Returns true if two contexts are equal.
USD_INTEROP_API bool ArDefaultResolverContext_Equal(
    ArDefaultResolverContextRef lhs,
    ArDefaultResolverContextRef rhs
);

/// Compares two contexts.
USD_INTEROP_API int ArDefaultResolverContext_Compare(
    ArDefaultResolverContextRef lhs,
    ArDefaultResolverContextRef rhs
);

/// Converts an ArDefaultResolverContext to an ArResolverContext.
/// Returns NULL on failure. Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef ArDefaultResolverContext_ToResolverContext(
    ArDefaultResolverContextRef context
);

// ============================================================================
// MARK: - ArResolver (Global Asset Resolver)
// ============================================================================

/// Note: ArResolver is accessed via ArGetResolver() and is a singleton.
/// These functions operate on the global resolver instance.

// MARK: - Identifier Operations

/// Returns an identifier for the given asset path.
/// If anchorAssetPath is not NULL, it is used as the anchor for relative paths.
/// Caller must free the result with Ar_FreeString.
USD_INTEROP_API char* Ar_CreateIdentifier(
    const char* assetPath,
    ArResolvedPathRef anchorAssetPath
);

/// Returns an identifier for a new asset at the given path.
/// Caller must free the result with Ar_FreeString.
USD_INTEROP_API char* Ar_CreateIdentifierForNewAsset(
    const char* assetPath,
    ArResolvedPathRef anchorAssetPath
);

// MARK: - Resolution Operations

/// Resolves the given asset path to a resolved path.
/// Returns NULL if the asset does not exist.
/// Caller must release with ArResolvedPath_Release.
USD_INTEROP_API ArResolvedPathRef Ar_Resolve(const char* assetPath);

/// Resolves the given asset path for creating a new asset.
/// Returns NULL if a resolved path cannot be computed.
/// Caller must release with ArResolvedPath_Release.
USD_INTEROP_API ArResolvedPathRef Ar_ResolveForNewAsset(const char* assetPath);

// MARK: - Context Operations

/// Creates a default resolver context.
/// Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef Ar_CreateDefaultContext(void);

/// Creates a default resolver context for the given asset.
/// Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef Ar_CreateDefaultContextForAsset(const char* assetPath);

/// Creates a resolver context from a context string.
/// Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef Ar_CreateContextFromString(const char* contextStr);

/// Creates a resolver context from a context string for a specific URI scheme.
/// If uriScheme is NULL or empty, uses the primary resolver.
/// Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef Ar_CreateContextFromStringForScheme(
    const char* uriScheme,
    const char* contextStr
);

/// Refreshes caches associated with the given context.
/// May send notifications if cached data changes.
USD_INTEROP_API UsdResult Ar_RefreshContext(ArResolverContextRef context);

/// Returns the currently bound resolver context.
/// Caller must release with ArResolverContext_Release.
USD_INTEROP_API ArResolverContextRef Ar_GetCurrentContext(void);

/// Returns true if the given asset path is context-dependent.
USD_INTEROP_API bool Ar_IsContextDependentPath(const char* assetPath);

// MARK: - File/Asset Operations

/// Returns the file extension for the given asset path.
/// Caller must free the result with Ar_FreeString.
USD_INTEROP_API char* Ar_GetExtension(const char* assetPath);

/// Returns the modification timestamp for the given asset.
USD_INTEROP_API ArTimestamp Ar_GetModificationTimestamp(
    const char* assetPath,
    ArResolvedPathRef resolvedPath
);

/// Returns true if an asset can be written to the resolved path.
/// If whyNot is not NULL and the function returns false, it will be
/// set to an explanation string that the caller must free.
USD_INTEROP_API bool Ar_CanWriteAssetToPath(
    ArResolvedPathRef resolvedPath,
    char** whyNot
);

// MARK: - URI Scheme Operations

/// Returns the count of registered URI schemes.
USD_INTEROP_API size_t Ar_GetRegisteredURISchemeCount(void);

/// Gets the registered URI schemes.
/// outSchemes must have space for maxCount strings.
/// Returns the number of schemes written.
/// Caller must free each string with free().
USD_INTEROP_API size_t Ar_GetRegisteredURISchemes(
    char** outSchemes,
    size_t maxCount
);

// MARK: - Default Resolver Configuration

/// Sets the default search path for the default resolver.
/// WARNING: This function is not thread-safe.
USD_INTEROP_API UsdResult Ar_SetDefaultSearchPath(
    const char* const* searchPaths,
    size_t count
);

// MARK: - Utility Functions

/// Frees a string allocated by Ar functions.
USD_INTEROP_API void Ar_FreeString(char* str);

/// Frees a string array allocated by Ar functions.
USD_INTEROP_API void Ar_FreeStringArray(char** strings, size_t count);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_AR_H
