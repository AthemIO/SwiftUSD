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

// ============================================================================
// MARK: - ArResolverContextBinder
// ============================================================================

/// Opaque handle to an ArResolverContextBinder wrapper.
/// A context binder binds a resolver context in a scoped manner.
typedef struct ArResolverContextBinderOpaque* ArResolverContextBinderRef;

// MARK: - ArResolverContextBinder Lifecycle

/// Creates and binds a resolver context binder.
/// The context will remain bound until the binder is released.
/// Returns NULL on failure. Caller must release with ArResolverContextBinder_Release.
USD_INTEROP_API ArResolverContextBinderRef ArResolverContextBinder_Create(ArResolverContextRef context);

/// Releases the context binder and unbinds the context.
USD_INTEROP_API void ArResolverContextBinder_Release(ArResolverContextBinderRef binder);

/// Returns the context bound by this binder.
USD_INTEROP_API ArResolverContextRef ArResolverContextBinder_GetContext(ArResolverContextBinderRef binder);

// ============================================================================
// MARK: - ArResolverScopedCache
// ============================================================================

/// Opaque handle to an ArResolverScopedCache wrapper.
/// A scoped cache enables caching of resolver queries within a scope.
typedef struct ArResolverScopedCacheOpaque* ArResolverScopedCacheRef;

// MARK: - ArResolverScopedCache Lifecycle

/// Creates a scoped cache and begins caching.
/// Returns NULL on failure. Caller must release with ArResolverScopedCache_Release.
USD_INTEROP_API ArResolverScopedCacheRef ArResolverScopedCache_Create(void);

/// Releases the scoped cache and ends caching for this scope.
USD_INTEROP_API void ArResolverScopedCache_Release(ArResolverScopedCacheRef cache);

// ============================================================================
// MARK: - ArAsset
// ============================================================================

/// Opaque handle to an ArAsset wrapper.
/// Provides read-only access to asset contents.
typedef struct ArAssetOpaque* ArAssetRef;

// MARK: - ArAsset Lifecycle

/// Opens an asset at the given resolved path.
/// Returns NULL if the asset cannot be opened.
/// Caller must release with ArAsset_Release.
USD_INTEROP_API ArAssetRef ArAsset_Open(ArResolvedPathRef resolvedPath);

/// Increments the reference count of an asset.
USD_INTEROP_API ArAssetRef ArAsset_Retain(ArAssetRef asset);

/// Decrements the reference count and frees if count reaches zero.
USD_INTEROP_API void ArAsset_Release(ArAssetRef asset);

// MARK: - ArAsset Properties

/// Returns the size of the asset in bytes.
/// Returns 0 if the asset is invalid.
USD_INTEROP_API size_t ArAsset_GetSize(ArAssetRef asset);

/// Reads bytes from the asset.
/// Returns the number of bytes actually read.
/// - Parameters:
///   - asset: The asset to read from.
///   - buffer: Buffer to read into.
///   - count: Number of bytes to read.
///   - offset: Byte offset to start reading from.
USD_INTEROP_API size_t ArAsset_Read(
    ArAssetRef asset,
    void* buffer,
    size_t count,
    size_t offset
);

/// Returns the entire asset contents as a buffer.
/// Returns NULL on failure.
/// Caller must free the result with ArAsset_FreeBuffer.
/// outSize will be set to the buffer size.
USD_INTEROP_API void* ArAsset_GetBuffer(ArAssetRef asset, size_t* outSize);

/// Frees a buffer allocated by ArAsset_GetBuffer.
USD_INTEROP_API void ArAsset_FreeBuffer(void* buffer);

// ============================================================================
// MARK: - ArWritableAsset
// ============================================================================

/// Write mode for writable assets.
typedef enum {
    /// Open for in-place updates to an existing file.
    AR_WRITE_MODE_UPDATE = 0,
    /// Open for replacement (writes to temp file, then renames on close).
    AR_WRITE_MODE_REPLACE = 1
} ArWriteMode;

/// Opaque handle to an ArWritableAsset wrapper.
/// Provides write access to asset contents.
typedef struct ArWritableAssetOpaque* ArWritableAssetRef;

// MARK: - ArWritableAsset Lifecycle

/// Opens a writable asset at the given resolved path.
/// Returns NULL if the asset cannot be opened for writing.
/// Caller must release with ArWritableAsset_Release.
USD_INTEROP_API ArWritableAssetRef ArWritableAsset_Open(
    ArResolvedPathRef resolvedPath,
    ArWriteMode writeMode
);

/// Releases the writable asset. Does NOT close the asset.
/// Call ArWritableAsset_Close first to properly finalize writes.
USD_INTEROP_API void ArWritableAsset_Release(ArWritableAssetRef asset);

// MARK: - ArWritableAsset Operations

/// Writes bytes to the asset.
/// Returns the number of bytes actually written.
/// - Parameters:
///   - asset: The asset to write to.
///   - buffer: Buffer containing data to write.
///   - count: Number of bytes to write.
///   - offset: Byte offset to start writing at.
USD_INTEROP_API size_t ArWritableAsset_Write(
    ArWritableAssetRef asset,
    const void* buffer,
    size_t count,
    size_t offset
);

/// Closes the writable asset and finalizes writes.
/// For AR_WRITE_MODE_REPLACE, this performs the atomic rename.
/// Returns true on success, false on failure.
USD_INTEROP_API bool ArWritableAsset_Close(ArWritableAssetRef asset);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USD_AR_H
