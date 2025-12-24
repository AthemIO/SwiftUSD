// ar.cpp - ArResolver, ArResolverContext, ArResolvedPath C wrapper implementation
// Mirrors: pxr/usd/ar/resolver.h, pxr/usd/ar/resolverContext.h, pxr/usd/ar/resolvedPath.h

#include "../../include/usd/ar.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define AR_USE_USD 1
    #include <pxr/usd/ar/resolver.h>
    #include <pxr/usd/ar/resolverContext.h>
    #include <pxr/usd/ar/resolvedPath.h>
    #include <pxr/usd/ar/timestamp.h>
    #include <pxr/usd/ar/defaultResolver.h>
    #include <pxr/usd/ar/defaultResolverContext.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define AR_USE_USD 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

// ============================================================================
// MARK: - ArResolvedPath Wrapper Structure
// ============================================================================

struct ArResolvedPathOpaque {
#if AR_USE_USD
    ArResolvedPath path;
#else
    std::string pathString;
#endif
    std::atomic<int> refCount;

    ArResolvedPathOpaque()
        : refCount(1)
    {
    }

    explicit ArResolvedPathOpaque(const char* str)
        : refCount(1)
    {
#if AR_USE_USD
        try {
            if (str && str[0] != '\0') {
                path = ArResolvedPath(str);
            }
        } catch (...) {
            // Leave as empty
        }
#else
        if (str) {
            pathString = str;
        }
#endif
    }

#if AR_USE_USD
    explicit ArResolvedPathOpaque(const ArResolvedPath& p)
        : path(p)
        , refCount(1)
    {
    }
#endif

    const char* getPathString() const {
#if AR_USE_USD
        return path.GetPathString().c_str();
#else
        return pathString.c_str();
#endif
    }

    bool isEmpty() const {
#if AR_USE_USD
        return path.IsEmpty();
#else
        return pathString.empty();
#endif
    }

    size_t getHash() const {
#if AR_USE_USD
        return path.GetHash();
#else
        return std::hash<std::string>{}(pathString);
#endif
    }
};

// ============================================================================
// MARK: - ArResolverContext Wrapper Structure
// ============================================================================

struct ArResolverContextOpaque {
#if AR_USE_USD
    ArResolverContext context;
#else
    std::string debugString;
#endif
    std::atomic<int> refCount;

    ArResolverContextOpaque()
        : refCount(1)
    {
    }

#if AR_USE_USD
    explicit ArResolverContextOpaque(const ArResolverContext& ctx)
        : context(ctx)
        , refCount(1)
    {
    }
#endif

    bool isEmpty() const {
#if AR_USE_USD
        return context.IsEmpty();
#else
        return debugString.empty();
#endif
    }

    std::string getDebugString() const {
#if AR_USE_USD
        return context.GetDebugString();
#else
        return debugString;
#endif
    }

    size_t getHash() const {
#if AR_USE_USD
        return hash_value(context);
#else
        return std::hash<std::string>{}(debugString);
#endif
    }
};

// ============================================================================
// MARK: - ArDefaultResolverContext Wrapper Structure
// ============================================================================

struct ArDefaultResolverContextOpaque {
#if AR_USE_USD
    ArDefaultResolverContext context;
#else
    std::vector<std::string> searchPaths;
#endif
    std::atomic<int> refCount;

    ArDefaultResolverContextOpaque()
        : refCount(1)
    {
    }

#if AR_USE_USD
    explicit ArDefaultResolverContextOpaque(const ArDefaultResolverContext& ctx)
        : context(ctx)
        , refCount(1)
    {
    }

    explicit ArDefaultResolverContextOpaque(const std::vector<std::string>& paths)
        : context(paths)
        , refCount(1)
    {
    }
#else
    explicit ArDefaultResolverContextOpaque(const std::vector<std::string>& paths)
        : searchPaths(paths)
        , refCount(1)
    {
    }
#endif

    const std::vector<std::string>& getSearchPath() const {
#if AR_USE_USD
        return context.GetSearchPath();
#else
        return searchPaths;
#endif
    }

    std::string getAsString() const {
#if AR_USE_USD
        return context.GetAsString();
#else
        std::string result;
        for (size_t i = 0; i < searchPaths.size(); i++) {
            if (i > 0) result += ":";
            result += searchPaths[i];
        }
        return result;
#endif
    }

    size_t getHash() const {
#if AR_USE_USD
        return hash_value(context);
#else
        size_t h = 0;
        for (const auto& p : searchPaths) {
            h ^= std::hash<std::string>{}(p) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
#endif
    }
};

// ============================================================================
// MARK: - ArResolvedPath Implementation
// ============================================================================

extern "C" {

ArResolvedPathRef ArResolvedPath_Create(const char* resolvedPath) {
    try {
        return new ArResolvedPathOpaque(resolvedPath);
    } catch (...) {
        return nullptr;
    }
}

ArResolvedPathRef ArResolvedPath_CreateEmpty(void) {
    try {
        return new ArResolvedPathOpaque();
    } catch (...) {
        return nullptr;
    }
}

ArResolvedPathRef ArResolvedPath_Copy(ArResolvedPathRef path) {
    if (!path) return nullptr;
    try {
#if AR_USE_USD
        return new ArResolvedPathOpaque(path->path);
#else
        auto* copy = new ArResolvedPathOpaque();
        copy->pathString = path->pathString;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolvedPathRef ArResolvedPath_Retain(ArResolvedPathRef path) {
    if (!path) return nullptr;
    path->refCount.fetch_add(1, std::memory_order_relaxed);
    return path;
}

void ArResolvedPath_Release(ArResolvedPathRef path) {
    if (!path) return;
    if (path->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete path;
    }
}

const char* ArResolvedPath_GetPathString(ArResolvedPathRef path) {
    if (!path) return "";
    try {
        return path->getPathString();
    } catch (...) {
        return "";
    }
}

bool ArResolvedPath_IsEmpty(ArResolvedPathRef path) {
    if (!path) return true;
    try {
        return path->isEmpty();
    } catch (...) {
        return true;
    }
}

size_t ArResolvedPath_Hash(ArResolvedPathRef path) {
    if (!path) return 0;
    try {
        return path->getHash();
    } catch (...) {
        return 0;
    }
}

bool ArResolvedPath_Equal(ArResolvedPathRef lhs, ArResolvedPathRef rhs) {
    if (lhs == rhs) return true;
    if (!lhs || !rhs) return false;
    try {
#if AR_USE_USD
        return lhs->path == rhs->path;
#else
        return lhs->pathString == rhs->pathString;
#endif
    } catch (...) {
        return false;
    }
}

int ArResolvedPath_Compare(ArResolvedPathRef lhs, ArResolvedPathRef rhs) {
    if (lhs == rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    try {
#if AR_USE_USD
        if (lhs->path < rhs->path) return -1;
        if (rhs->path < lhs->path) return 1;
        return 0;
#else
        return lhs->pathString.compare(rhs->pathString);
#endif
    } catch (...) {
        return 0;
    }
}

// ============================================================================
// MARK: - ArTimestamp Implementation
// ============================================================================

ArTimestamp ArTimestamp_CreateInvalid(void) {
    ArTimestamp ts;
    ts.time = std::nan("");
    return ts;
}

ArTimestamp ArTimestamp_Create(double time) {
    ArTimestamp ts;
    ts.time = time;
    return ts;
}

bool ArTimestamp_IsValid(ArTimestamp timestamp) {
    return !std::isnan(timestamp.time);
}

double ArTimestamp_GetTime(ArTimestamp timestamp) {
    return timestamp.time;
}

int ArTimestamp_Compare(ArTimestamp lhs, ArTimestamp rhs) {
    bool lhsValid = !std::isnan(lhs.time);
    bool rhsValid = !std::isnan(rhs.time);

    if (!lhsValid && !rhsValid) return 0;
    if (!lhsValid) return -1;  // Invalid is less than valid
    if (!rhsValid) return 1;

    if (lhs.time < rhs.time) return -1;
    if (lhs.time > rhs.time) return 1;
    return 0;
}

bool ArTimestamp_Equal(ArTimestamp lhs, ArTimestamp rhs) {
    bool lhsValid = !std::isnan(lhs.time);
    bool rhsValid = !std::isnan(rhs.time);

    if (!lhsValid && !rhsValid) return true;
    if (lhsValid != rhsValid) return false;
    return lhs.time == rhs.time;
}

// ============================================================================
// MARK: - ArResolverContext Implementation
// ============================================================================

ArResolverContextRef ArResolverContext_CreateEmpty(void) {
    try {
        return new ArResolverContextOpaque();
    } catch (...) {
        return nullptr;
    }
}

ArResolverContextRef ArResolverContext_Copy(ArResolverContextRef context) {
    if (!context) return nullptr;
    try {
#if AR_USE_USD
        return new ArResolverContextOpaque(context->context);
#else
        auto* copy = new ArResolverContextOpaque();
        copy->debugString = context->debugString;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolverContextRef ArResolverContext_Retain(ArResolverContextRef context) {
    if (!context) return nullptr;
    context->refCount.fetch_add(1, std::memory_order_relaxed);
    return context;
}

void ArResolverContext_Release(ArResolverContextRef context) {
    if (!context) return;
    if (context->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete context;
    }
}

bool ArResolverContext_IsEmpty(ArResolverContextRef context) {
    if (!context) return true;
    try {
        return context->isEmpty();
    } catch (...) {
        return true;
    }
}

char* ArResolverContext_GetDebugString(ArResolverContextRef context) {
    if (!context) return nullptr;
    try {
        std::string str = context->getDebugString();
        char* result = static_cast<char*>(malloc(str.length() + 1));
        if (result) {
            memcpy(result, str.c_str(), str.length() + 1);
        }
        return result;
    } catch (...) {
        return nullptr;
    }
}

void ArResolverContext_FreeString(char* str) {
    free(str);
}

size_t ArResolverContext_Hash(ArResolverContextRef context) {
    if (!context) return 0;
    try {
        return context->getHash();
    } catch (...) {
        return 0;
    }
}

bool ArResolverContext_Equal(ArResolverContextRef lhs, ArResolverContextRef rhs) {
    if (lhs == rhs) return true;
    if (!lhs || !rhs) return false;
    try {
#if AR_USE_USD
        return lhs->context == rhs->context;
#else
        return lhs->debugString == rhs->debugString;
#endif
    } catch (...) {
        return false;
    }
}

int ArResolverContext_Compare(ArResolverContextRef lhs, ArResolverContextRef rhs) {
    if (lhs == rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    try {
#if AR_USE_USD
        if (lhs->context < rhs->context) return -1;
        if (rhs->context < lhs->context) return 1;
        return 0;
#else
        return lhs->debugString.compare(rhs->debugString);
#endif
    } catch (...) {
        return 0;
    }
}

// ============================================================================
// MARK: - ArDefaultResolverContext Implementation
// ============================================================================

ArDefaultResolverContextRef ArDefaultResolverContext_Create(void) {
    try {
        return new ArDefaultResolverContextOpaque();
    } catch (...) {
        return nullptr;
    }
}

ArDefaultResolverContextRef ArDefaultResolverContext_CreateWithSearchPaths(
    const char* const* searchPaths,
    size_t count
) {
    try {
        std::vector<std::string> paths;
        paths.reserve(count);
        for (size_t i = 0; i < count; i++) {
            if (searchPaths[i]) {
                paths.push_back(searchPaths[i]);
            }
        }
        return new ArDefaultResolverContextOpaque(paths);
    } catch (...) {
        return nullptr;
    }
}

ArDefaultResolverContextRef ArDefaultResolverContext_Copy(ArDefaultResolverContextRef context) {
    if (!context) return nullptr;
    try {
#if AR_USE_USD
        return new ArDefaultResolverContextOpaque(context->context);
#else
        auto* copy = new ArDefaultResolverContextOpaque();
        copy->searchPaths = context->searchPaths;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArDefaultResolverContextRef ArDefaultResolverContext_Retain(ArDefaultResolverContextRef context) {
    if (!context) return nullptr;
    context->refCount.fetch_add(1, std::memory_order_relaxed);
    return context;
}

void ArDefaultResolverContext_Release(ArDefaultResolverContextRef context) {
    if (!context) return;
    if (context->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete context;
    }
}

size_t ArDefaultResolverContext_GetSearchPathCount(ArDefaultResolverContextRef context) {
    if (!context) return 0;
    try {
        return context->getSearchPath().size();
    } catch (...) {
        return 0;
    }
}

size_t ArDefaultResolverContext_GetSearchPaths(
    ArDefaultResolverContextRef context,
    char** outPaths,
    size_t maxCount
) {
    if (!context || !outPaths || maxCount == 0) return 0;
    try {
        const auto& paths = context->getSearchPath();
        size_t count = std::min(paths.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            const std::string& path = paths[i];
            outPaths[i] = static_cast<char*>(malloc(path.length() + 1));
            if (outPaths[i]) {
                memcpy(outPaths[i], path.c_str(), path.length() + 1);
            }
        }
        return count;
    } catch (...) {
        return 0;
    }
}

char* ArDefaultResolverContext_GetAsString(ArDefaultResolverContextRef context) {
    if (!context) return nullptr;
    try {
        std::string str = context->getAsString();
        char* result = static_cast<char*>(malloc(str.length() + 1));
        if (result) {
            memcpy(result, str.c_str(), str.length() + 1);
        }
        return result;
    } catch (...) {
        return nullptr;
    }
}

void ArDefaultResolverContext_FreeString(char* str) {
    free(str);
}

size_t ArDefaultResolverContext_Hash(ArDefaultResolverContextRef context) {
    if (!context) return 0;
    try {
        return context->getHash();
    } catch (...) {
        return 0;
    }
}

bool ArDefaultResolverContext_Equal(
    ArDefaultResolverContextRef lhs,
    ArDefaultResolverContextRef rhs
) {
    if (lhs == rhs) return true;
    if (!lhs || !rhs) return false;
    try {
#if AR_USE_USD
        return lhs->context == rhs->context;
#else
        return lhs->searchPaths == rhs->searchPaths;
#endif
    } catch (...) {
        return false;
    }
}

int ArDefaultResolverContext_Compare(
    ArDefaultResolverContextRef lhs,
    ArDefaultResolverContextRef rhs
) {
    if (lhs == rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    try {
#if AR_USE_USD
        if (lhs->context < rhs->context) return -1;
        if (rhs->context < lhs->context) return 1;
        return 0;
#else
        if (lhs->searchPaths < rhs->searchPaths) return -1;
        if (lhs->searchPaths > rhs->searchPaths) return 1;
        return 0;
#endif
    } catch (...) {
        return 0;
    }
}

ArResolverContextRef ArDefaultResolverContext_ToResolverContext(
    ArDefaultResolverContextRef context
) {
    if (!context) return nullptr;
    try {
#if AR_USE_USD
        return new ArResolverContextOpaque(ArResolverContext(context->context));
#else
        auto* result = new ArResolverContextOpaque();
        result->debugString = context->getAsString();
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - ArResolver (Global) Implementation
// ============================================================================

char* Ar_CreateIdentifier(
    const char* assetPath,
    ArResolvedPathRef anchorAssetPath
) {
    if (!assetPath) return nullptr;
    try {
#if AR_USE_USD
        ArResolvedPath anchor;
        if (anchorAssetPath) {
            anchor = anchorAssetPath->path;
        }
        std::string identifier = ArGetResolver().CreateIdentifier(assetPath, anchor);
        char* result = static_cast<char*>(malloc(identifier.length() + 1));
        if (result) {
            memcpy(result, identifier.c_str(), identifier.length() + 1);
        }
        return result;
#else
        // Fallback: just return a copy of the asset path
        size_t len = strlen(assetPath);
        char* result = static_cast<char*>(malloc(len + 1));
        if (result) {
            memcpy(result, assetPath, len + 1);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

char* Ar_CreateIdentifierForNewAsset(
    const char* assetPath,
    ArResolvedPathRef anchorAssetPath
) {
    if (!assetPath) return nullptr;
    try {
#if AR_USE_USD
        ArResolvedPath anchor;
        if (anchorAssetPath) {
            anchor = anchorAssetPath->path;
        }
        std::string identifier = ArGetResolver().CreateIdentifierForNewAsset(assetPath, anchor);
        char* result = static_cast<char*>(malloc(identifier.length() + 1));
        if (result) {
            memcpy(result, identifier.c_str(), identifier.length() + 1);
        }
        return result;
#else
        size_t len = strlen(assetPath);
        char* result = static_cast<char*>(malloc(len + 1));
        if (result) {
            memcpy(result, assetPath, len + 1);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolvedPathRef Ar_Resolve(const char* assetPath) {
    if (!assetPath) return nullptr;
    try {
#if AR_USE_USD
        ArResolvedPath resolved = ArGetResolver().Resolve(assetPath);
        if (resolved.IsEmpty()) {
            return nullptr;
        }
        return new ArResolvedPathOpaque(resolved);
#else
        // Fallback: return the path as-is (no actual resolution)
        return new ArResolvedPathOpaque(assetPath);
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolvedPathRef Ar_ResolveForNewAsset(const char* assetPath) {
    if (!assetPath) return nullptr;
    try {
#if AR_USE_USD
        ArResolvedPath resolved = ArGetResolver().ResolveForNewAsset(assetPath);
        if (resolved.IsEmpty()) {
            return nullptr;
        }
        return new ArResolvedPathOpaque(resolved);
#else
        return new ArResolvedPathOpaque(assetPath);
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolverContextRef Ar_CreateDefaultContext(void) {
    try {
#if AR_USE_USD
        ArResolverContext ctx = ArGetResolver().CreateDefaultContext();
        return new ArResolverContextOpaque(ctx);
#else
        return new ArResolverContextOpaque();
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolverContextRef Ar_CreateDefaultContextForAsset(const char* assetPath) {
    if (!assetPath) return nullptr;
    try {
#if AR_USE_USD
        ArResolverContext ctx = ArGetResolver().CreateDefaultContextForAsset(assetPath);
        return new ArResolverContextOpaque(ctx);
#else
        return new ArResolverContextOpaque();
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolverContextRef Ar_CreateContextFromString(const char* contextStr) {
    if (!contextStr) return nullptr;
    try {
#if AR_USE_USD
        ArResolverContext ctx = ArGetResolver().CreateContextFromString(contextStr);
        return new ArResolverContextOpaque(ctx);
#else
        auto* result = new ArResolverContextOpaque();
        result->debugString = contextStr;
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArResolverContextRef Ar_CreateContextFromStringForScheme(
    const char* uriScheme,
    const char* contextStr
) {
    if (!contextStr) return nullptr;
    try {
#if AR_USE_USD
        std::string scheme = uriScheme ? uriScheme : "";
        ArResolverContext ctx = ArGetResolver().CreateContextFromString(scheme, contextStr);
        return new ArResolverContextOpaque(ctx);
#else
        auto* result = new ArResolverContextOpaque();
        result->debugString = contextStr;
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

UsdResult Ar_RefreshContext(ArResolverContextRef context) {
    if (!context) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if AR_USE_USD
        ArGetResolver().RefreshContext(context->context);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

ArResolverContextRef Ar_GetCurrentContext(void) {
    try {
#if AR_USE_USD
        ArResolverContext ctx = ArGetResolver().GetCurrentContext();
        return new ArResolverContextOpaque(ctx);
#else
        return new ArResolverContextOpaque();
#endif
    } catch (...) {
        return nullptr;
    }
}

bool Ar_IsContextDependentPath(const char* assetPath) {
    if (!assetPath) return false;
    try {
#if AR_USE_USD
        return ArGetResolver().IsContextDependentPath(assetPath);
#else
        return false;
#endif
    } catch (...) {
        return false;
    }
}

char* Ar_GetExtension(const char* assetPath) {
    if (!assetPath) return nullptr;
    try {
#if AR_USE_USD
        std::string ext = ArGetResolver().GetExtension(assetPath);
        char* result = static_cast<char*>(malloc(ext.length() + 1));
        if (result) {
            memcpy(result, ext.c_str(), ext.length() + 1);
        }
        return result;
#else
        // Fallback: extract extension from path
        std::string path(assetPath);
        size_t dot = path.rfind('.');
        size_t slash = path.rfind('/');
        if (dot != std::string::npos && (slash == std::string::npos || dot > slash)) {
            std::string ext = path.substr(dot + 1);
            char* result = static_cast<char*>(malloc(ext.length() + 1));
            if (result) {
                memcpy(result, ext.c_str(), ext.length() + 1);
            }
            return result;
        }
        // No extension
        char* result = static_cast<char*>(malloc(1));
        if (result) {
            result[0] = '\0';
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArTimestamp Ar_GetModificationTimestamp(
    const char* assetPath,
    ArResolvedPathRef resolvedPath
) {
    ArTimestamp invalid;
    invalid.time = std::nan("");

    if (!assetPath || !resolvedPath) return invalid;
    try {
#if AR_USE_USD
        pxr::ArTimestamp ts = ArGetResolver().GetModificationTimestamp(assetPath, resolvedPath->path);
        if (!ts.IsValid()) {
            return invalid;
        }
        ArTimestamp result;
        result.time = ts.GetTime();
        return result;
#else
        return invalid;
#endif
    } catch (...) {
        return invalid;
    }
}

bool Ar_CanWriteAssetToPath(
    ArResolvedPathRef resolvedPath,
    char** whyNot
) {
    if (!resolvedPath) {
        if (whyNot) {
            const char* msg = "Invalid resolved path";
            *whyNot = static_cast<char*>(malloc(strlen(msg) + 1));
            if (*whyNot) {
                strcpy(*whyNot, msg);
            }
        }
        return false;
    }
    try {
#if AR_USE_USD
        std::string reason;
        bool canWrite = ArGetResolver().CanWriteAssetToPath(resolvedPath->path, whyNot ? &reason : nullptr);
        if (!canWrite && whyNot && !reason.empty()) {
            *whyNot = static_cast<char*>(malloc(reason.length() + 1));
            if (*whyNot) {
                memcpy(*whyNot, reason.c_str(), reason.length() + 1);
            }
        }
        return canWrite;
#else
        // Fallback: assume we can write
        return true;
#endif
    } catch (...) {
        if (whyNot) {
            const char* msg = "Exception occurred";
            *whyNot = static_cast<char*>(malloc(strlen(msg) + 1));
            if (*whyNot) {
                strcpy(*whyNot, msg);
            }
        }
        return false;
    }
}

size_t Ar_GetRegisteredURISchemeCount(void) {
    try {
#if AR_USE_USD
        return ArGetRegisteredURISchemes().size();
#else
        return 0;
#endif
    } catch (...) {
        return 0;
    }
}

size_t Ar_GetRegisteredURISchemes(
    char** outSchemes,
    size_t maxCount
) {
    if (!outSchemes || maxCount == 0) return 0;
    try {
#if AR_USE_USD
        const auto& schemes = ArGetRegisteredURISchemes();
        size_t count = std::min(schemes.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            const std::string& scheme = schemes[i];
            outSchemes[i] = static_cast<char*>(malloc(scheme.length() + 1));
            if (outSchemes[i]) {
                memcpy(outSchemes[i], scheme.c_str(), scheme.length() + 1);
            }
        }
        return count;
#else
        return 0;
#endif
    } catch (...) {
        return 0;
    }
}

UsdResult Ar_SetDefaultSearchPath(
    const char* const* searchPaths,
    size_t count
) {
    try {
#if AR_USE_USD
        std::vector<std::string> paths;
        paths.reserve(count);
        for (size_t i = 0; i < count; i++) {
            if (searchPaths[i]) {
                paths.push_back(searchPaths[i]);
            }
        }
        ArDefaultResolver::SetDefaultSearchPath(paths);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

void Ar_FreeString(char* str) {
    free(str);
}

void Ar_FreeStringArray(char** strings, size_t count) {
    if (!strings) return;
    for (size_t i = 0; i < count; i++) {
        free(strings[i]);
    }
}

} // extern "C"
