// tf.h - TfToken C wrapper
// Mirrors: pxr/base/tf/token.h

#ifndef OPENUSDINTEROP_BASE_TF_H
#define OPENUSDINTEROP_BASE_TF_H

#include "../OpenUSDInterop.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - TfToken

/// Creates an empty token.
USD_INTEROP_API UsdResult UsdTfToken_Create(UsdTfTokenRef* outToken);

/// Creates a token from a string.
USD_INTEROP_API UsdResult UsdTfToken_CreateFromString(const char* text, UsdTfTokenRef* outToken);

/// Destroys a token.
USD_INTEROP_API void UsdTfToken_Destroy(UsdTfTokenRef token);

/// Returns the text of a token. The returned string is owned by the token.
USD_INTEROP_API const char* UsdTfToken_GetText(UsdTfTokenRef token);

/// Returns true if the token is empty.
USD_INTEROP_API bool UsdTfToken_IsEmpty(UsdTfTokenRef token);

/// Returns true if two tokens are equal.
USD_INTEROP_API bool UsdTfToken_Equal(UsdTfTokenRef a, UsdTfTokenRef b);

/// Returns the hash of a token.
USD_INTEROP_API uint64_t UsdTfToken_Hash(UsdTfTokenRef token);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_TF_H
