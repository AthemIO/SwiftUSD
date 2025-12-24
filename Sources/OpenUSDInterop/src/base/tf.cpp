// tf.cpp - TfToken C wrapper implementation
// Mirrors: pxr/base/tf/token.h

#include "../../include/base/tf.h"

// Check if we should use full USD library
// Define USE_PIXAR_USD=1 when linking against the full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define TF_USE_USD 1
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define TF_USE_USD 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

// MARK: - Token Wrapper Structure

/// Wrapper structure that holds either a real TfToken or a fallback string.
/// Uses atomic reference counting for thread safety.
struct TfTokenOpaque {
#if TF_USE_USD
    TfToken token;
#else
    std::string text;
    size_t hash;
#endif
    std::atomic<int> refCount;

    // Default constructor creates empty token
    TfTokenOpaque()
        : refCount(1)
    {
#if !TF_USE_USD
        hash = std::hash<std::string>{}(text);
#endif
    }

    // Constructor from C string
    explicit TfTokenOpaque(const char* str)
        : refCount(1)
    {
#if TF_USE_USD
        try {
            if (str) {
                token = TfToken(str);
            }
        } catch (...) {
            // Leave as empty token
        }
#else
        if (str) {
            text = str;
        }
        hash = std::hash<std::string>{}(text);
#endif
    }

    // Constructor from C string with length
    TfTokenOpaque(const char* str, size_t length)
        : refCount(1)
    {
#if TF_USE_USD
        try {
            if (str && length > 0) {
                token = TfToken(std::string(str, length));
            }
        } catch (...) {
            // Leave as empty token
        }
#else
        if (str && length > 0) {
            text = std::string(str, length);
        }
        hash = std::hash<std::string>{}(text);
#endif
    }

#if TF_USE_USD
    // Constructor from existing TfToken (for copy operations)
    explicit TfTokenOpaque(const TfToken& t)
        : token(t)
        , refCount(1)
    {
    }
#endif

    // Get the text of the token
    const char* getText() const {
#if TF_USE_USD
        return token.GetText();
#else
        return text.c_str();
#endif
    }

    // Get the size of the token string
    size_t getSize() const {
#if TF_USE_USD
        return token.size();
#else
        return text.size();
#endif
    }

    // Check if token is empty
    bool isEmpty() const {
#if TF_USE_USD
        return token.IsEmpty();
#else
        return text.empty();
#endif
    }

    // Get hash value
    size_t getHash() const {
#if TF_USE_USD
        return token.Hash();
#else
        return hash;
#endif
    }
};

// MARK: - Lifecycle Functions

extern "C" {

TfTokenRef TfToken_Create(const char* text) {
    try {
        return new TfTokenOpaque(text);
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef TfToken_CreateWithLength(const char* text, size_t length) {
    try {
        return new TfTokenOpaque(text, length);
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef TfToken_CreateEmpty(void) {
    try {
        return new TfTokenOpaque();
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef TfToken_Copy(TfTokenRef token) {
    if (!token) return nullptr;
    try {
#if TF_USE_USD
        return new TfTokenOpaque(token->token);
#else
        auto* copy = new TfTokenOpaque();
        copy->text = token->text;
        copy->hash = token->hash;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef TfToken_Retain(TfTokenRef token) {
    if (!token) return nullptr;
    token->refCount.fetch_add(1, std::memory_order_relaxed);
    return token;
}

void TfToken_Release(TfTokenRef token) {
    if (!token) return;
    if (token->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete token;
    }
}

// MARK: - Token Properties

const char* TfToken_GetText(TfTokenRef token) {
    if (!token) return "";
    try {
        return token->getText();
    } catch (...) {
        return "";
    }
}

size_t TfToken_GetSize(TfTokenRef token) {
    if (!token) return 0;
    try {
        return token->getSize();
    } catch (...) {
        return 0;
    }
}

bool TfToken_IsEmpty(TfTokenRef token) {
    if (!token) return true;
    try {
        return token->isEmpty();
    } catch (...) {
        return true;
    }
}

size_t TfToken_Hash(TfTokenRef token) {
    if (!token) return 0;
    try {
        return token->getHash();
    } catch (...) {
        return 0;
    }
}

// MARK: - Comparison Functions

bool TfToken_Equal(TfTokenRef lhs, TfTokenRef rhs) {
    if (lhs == rhs) return true;  // Same pointer or both null
    if (!lhs || !rhs) return false;
    try {
#if TF_USE_USD
        return lhs->token == rhs->token;
#else
        return lhs->text == rhs->text;
#endif
    } catch (...) {
        return false;
    }
}

bool TfToken_EqualString(TfTokenRef token, const char* str) {
    if (!token) {
        return str == nullptr || str[0] == '\0';
    }
    if (!str) {
        return token->isEmpty();
    }
    try {
#if TF_USE_USD
        return token->token == str;
#else
        return token->text == str;
#endif
    } catch (...) {
        return false;
    }
}

int TfToken_Compare(TfTokenRef lhs, TfTokenRef rhs) {
    if (lhs == rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    try {
#if TF_USE_USD
        const std::string& lhsStr = lhs->token.GetString();
        const std::string& rhsStr = rhs->token.GetString();
        return lhsStr.compare(rhsStr);
#else
        return lhs->text.compare(rhs->text);
#endif
    } catch (...) {
        return 0;
    }
}

// MARK: - Token Lookup

TfTokenRef TfToken_Find(const char* text) {
    if (!text) return nullptr;
    try {
#if TF_USE_USD
        TfToken found = TfToken::Find(std::string(text));
        if (found.IsEmpty() && text[0] != '\0') {
            // Token not found
            return nullptr;
        }
        return new TfTokenOpaque(found);
#else
        // Fallback mode: we don't have a global registry, so we cannot "find"
        // existing tokens. Return nullptr to indicate not found.
        // Note: In the real USD, TfToken::Find returns an empty token if not found,
        // but we differentiate by returning nullptr here.
        return nullptr;
#endif
    } catch (...) {
        return nullptr;
    }
}

// MARK: - Batch Operations

size_t TfToken_CreateBatch(
    const char* const* texts,
    size_t count,
    TfTokenRef* outTokens
) {
    if (!texts || !outTokens || count == 0) return 0;

    size_t created = 0;
    for (size_t i = 0; i < count; i++) {
        try {
            outTokens[i] = new TfTokenOpaque(texts[i]);
            if (outTokens[i]) {
                created++;
            }
        } catch (...) {
            outTokens[i] = nullptr;
        }
    }
    return created;
}

size_t TfToken_GetTextBatch(
    const TfTokenRef* tokens,
    size_t count,
    const char** outTexts
) {
    if (!tokens || !outTexts || count == 0) return 0;

    size_t retrieved = 0;
    for (size_t i = 0; i < count; i++) {
        if (tokens[i]) {
            try {
                outTexts[i] = tokens[i]->getText();
                retrieved++;
            } catch (...) {
                outTexts[i] = "";
            }
        } else {
            outTexts[i] = "";
        }
    }
    return retrieved;
}

void TfToken_ReleaseBatch(TfTokenRef* tokens, size_t count) {
    if (!tokens || count == 0) return;

    for (size_t i = 0; i < count; i++) {
        TfToken_Release(tokens[i]);
        tokens[i] = nullptr;
    }
}

// MARK: - String Conversion Utilities

UsdResult TfToken_ToStringArray(
    const TfTokenRef* tokens,
    size_t tokenCount,
    char*** outStrings,
    size_t* outCount
) {
    if (!outStrings || !outCount) {
        return USD_RESULT_INVALID_ARGUMENT;
    }

    *outStrings = nullptr;
    *outCount = 0;

    if (!tokens || tokenCount == 0) {
        return USD_RESULT_SUCCESS;
    }

    try {
        // Allocate array of string pointers
        char** strings = static_cast<char**>(malloc(tokenCount * sizeof(char*)));
        if (!strings) {
            return USD_RESULT_OUT_OF_MEMORY;
        }

        // Copy each token's text
        for (size_t i = 0; i < tokenCount; i++) {
            const char* text = tokens[i] ? tokens[i]->getText() : "";
            size_t len = strlen(text);
            strings[i] = static_cast<char*>(malloc(len + 1));
            if (!strings[i]) {
                // Free already allocated strings
                for (size_t j = 0; j < i; j++) {
                    free(strings[j]);
                }
                free(strings);
                return USD_RESULT_OUT_OF_MEMORY;
            }
            memcpy(strings[i], text, len + 1);
        }

        *outStrings = strings;
        *outCount = tokenCount;
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

void TfToken_FreeStringArray(char** strings, size_t count) {
    if (!strings) return;

    for (size_t i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
}

} // extern "C"
