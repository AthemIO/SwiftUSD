// tf.h - TfToken C wrapper
// Mirrors: pxr/base/tf/token.h

#ifndef OPENUSDINTEROP_BASE_TF_H
#define OPENUSDINTEROP_BASE_TF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations - these are defined in OpenUSDInterop.h
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

// MARK: - Opaque Handle Type

/// Opaque handle to a TfToken wrapper.
/// TfToken provides efficient string comparison, assignment, and hashing.
typedef struct TfTokenOpaque* TfTokenRef;

// MARK: - Lifecycle

/// Creates a TfToken from a C string.
/// Returns NULL on failure. Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef TfToken_Create(const char* text);

/// Creates a TfToken from a C string with explicit length.
/// Useful for strings that may contain null characters.
/// Returns NULL on failure. Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef TfToken_CreateWithLength(const char* text, size_t length);

/// Creates an empty TfToken (represents the empty string "").
/// Returns NULL on failure. Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef TfToken_CreateEmpty(void);

/// Creates a copy of an existing TfToken.
/// Returns NULL on failure. Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef TfToken_Copy(TfTokenRef token);

/// Increments the reference count of a TfToken.
/// Returns the same token for convenience, or NULL if token was NULL.
USD_INTEROP_API TfTokenRef TfToken_Retain(TfTokenRef token);

/// Decrements the reference count and frees the token wrapper if count reaches zero.
/// Safe to call with NULL.
USD_INTEROP_API void TfToken_Release(TfTokenRef token);

// MARK: - Token Properties

/// Returns the text of the token as a C string.
/// The returned string is valid as long as the token is valid.
/// Returns empty string "" for empty tokens, never NULL.
USD_INTEROP_API const char* TfToken_GetText(TfTokenRef token);

/// Returns the size (length) of the token string.
/// Returns 0 for NULL or empty tokens.
USD_INTEROP_API size_t TfToken_GetSize(TfTokenRef token);

/// Returns true if the token is empty (represents "").
USD_INTEROP_API bool TfToken_IsEmpty(TfTokenRef token);

/// Returns a hash value for the token.
/// Tokens with the same text have the same hash.
USD_INTEROP_API size_t TfToken_Hash(TfTokenRef token);

// MARK: - Comparison

/// Returns true if two tokens are equal (same text).
USD_INTEROP_API bool TfToken_Equal(TfTokenRef lhs, TfTokenRef rhs);

/// Returns true if the token equals the given C string.
USD_INTEROP_API bool TfToken_EqualString(TfTokenRef token, const char* str);

/// Compares two tokens lexicographically.
/// Returns < 0 if lhs < rhs, 0 if equal, > 0 if lhs > rhs.
USD_INTEROP_API int TfToken_Compare(TfTokenRef lhs, TfTokenRef rhs);

// MARK: - Token Lookup

/// Finds an existing token for the given string, or returns NULL if not found.
/// Does not create a new token. Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef TfToken_Find(const char* text);

// MARK: - Batch Operations

/// Creates multiple tokens from an array of C strings.
/// outTokens must have space for count tokens.
/// Returns the number of successfully created tokens.
/// Caller must release each created token with TfToken_Release.
USD_INTEROP_API size_t TfToken_CreateBatch(
    const char* const* texts,
    size_t count,
    TfTokenRef* outTokens
);

/// Gets the text of multiple tokens.
/// outTexts must have space for count pointers.
/// Returns the number of successfully retrieved texts.
/// The returned strings are valid as long as their tokens are valid.
USD_INTEROP_API size_t TfToken_GetTextBatch(
    const TfTokenRef* tokens,
    size_t count,
    const char** outTexts
);

/// Releases multiple tokens.
/// Safe to include NULL tokens in the array.
USD_INTEROP_API void TfToken_ReleaseBatch(TfTokenRef* tokens, size_t count);

// MARK: - String Conversion Utilities

/// Converts a vector of tokens to a C array of strings.
/// Caller must free the returned array and each string with free().
/// Returns NULL on failure. outCount receives the number of strings.
USD_INTEROP_API UsdResult TfToken_ToStringArray(
    const TfTokenRef* tokens,
    size_t tokenCount,
    char*** outStrings,
    size_t* outCount
);

/// Frees a string array allocated by TfToken_ToStringArray.
USD_INTEROP_API void TfToken_FreeStringArray(char** strings, size_t count);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_TF_H
