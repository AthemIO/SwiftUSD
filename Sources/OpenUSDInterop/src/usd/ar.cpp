// ar.cpp - ArResolver, ArResolverContext, ArResolvedPath C wrapper implementation
// Mirrors: pxr/usd/ar/resolver.h, pxr/usd/ar/resolverContext.h, pxr/usd/ar/resolvedPath.h

#include "../../include/usd/ar.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define AR_USE_USD 1
    #include <pxr/usd/ar/resolver.h>
    #include <pxr/usd/ar/resolverContext.h>
    #include <pxr/usd/ar/resolverContextBinder.h>
    #include <pxr/usd/ar/resolverScopedCache.h>
    #include <pxr/usd/ar/resolvedPath.h>
    #include <pxr/usd/ar/timestamp.h>
    #include <pxr/usd/ar/asset.h>
    #include <pxr/usd/ar/writableAsset.h>
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

// ============================================================================
// MARK: - ArResolverContextBinder Wrapper Structure
// ============================================================================

struct ArResolverContextBinderOpaque {
#if AR_USE_USD
    std::unique_ptr<ArResolverContextBinder> binder;
    ArResolverContext boundContext;
#else
    ArResolverContextRef contextRef;
#endif

    ArResolverContextBinderOpaque()
#if !AR_USE_USD
        : contextRef(nullptr)
#endif
    {
    }

#if AR_USE_USD
    explicit ArResolverContextBinderOpaque(const ArResolverContext& ctx)
        : boundContext(ctx)
    {
        try {
            binder = std::make_unique<ArResolverContextBinder>(ctx);
        } catch (...) {
            // Leave binder null
        }
    }
#else
    explicit ArResolverContextBinderOpaque(ArResolverContextRef ctx)
        : contextRef(ctx)
    {
        if (ctx) {
            ArResolverContext_Retain(ctx);
        }
    }
#endif

    ~ArResolverContextBinderOpaque() {
#if !AR_USE_USD
        if (contextRef) {
            ArResolverContext_Release(contextRef);
        }
#endif
    }
};

// ============================================================================
// MARK: - ArResolverContextBinder Implementation
// ============================================================================

ArResolverContextBinderRef ArResolverContextBinder_Create(ArResolverContextRef context) {
    if (!context) return nullptr;
    try {
#if AR_USE_USD
        return new ArResolverContextBinderOpaque(context->context);
#else
        return new ArResolverContextBinderOpaque(context);
#endif
    } catch (...) {
        return nullptr;
    }
}

void ArResolverContextBinder_Release(ArResolverContextBinderRef binder) {
    if (binder) {
        delete binder;
    }
}

ArResolverContextRef ArResolverContextBinder_GetContext(ArResolverContextBinderRef binder) {
    if (!binder) return nullptr;
    try {
#if AR_USE_USD
        return new ArResolverContextOpaque(binder->boundContext);
#else
        if (binder->contextRef) {
            return ArResolverContext_Retain(binder->contextRef);
        }
        return nullptr;
#endif
    } catch (...) {
        return nullptr;
    }
}

// ============================================================================
// MARK: - ArResolverScopedCache Wrapper Structure
// ============================================================================

struct ArResolverScopedCacheOpaque {
#if AR_USE_USD
    std::unique_ptr<ArResolverScopedCache> cache;
#endif

    ArResolverScopedCacheOpaque() {
#if AR_USE_USD
        try {
            cache = std::make_unique<ArResolverScopedCache>();
        } catch (...) {
            // Leave cache null
        }
#endif
    }
};

// ============================================================================
// MARK: - ArResolverScopedCache Implementation
// ============================================================================

ArResolverScopedCacheRef ArResolverScopedCache_Create(void) {
    try {
        return new ArResolverScopedCacheOpaque();
    } catch (...) {
        return nullptr;
    }
}

void ArResolverScopedCache_Release(ArResolverScopedCacheRef cache) {
    if (cache) {
        delete cache;
    }
}

// ============================================================================
// MARK: - ArAsset Wrapper Structure
// ============================================================================

struct ArAssetOpaque {
#if AR_USE_USD
    std::shared_ptr<ArAsset> asset;
#else
    std::vector<char> buffer;
    std::string pathString;
#endif
    std::atomic<int> refCount;

    ArAssetOpaque()
        : refCount(1)
    {
    }

#if AR_USE_USD
    explicit ArAssetOpaque(std::shared_ptr<ArAsset> a)
        : asset(std::move(a))
        , refCount(1)
    {
    }
#else
    explicit ArAssetOpaque(const std::string& path)
        : pathString(path)
        , refCount(1)
    {
        // Fallback: try to read from filesystem
        FILE* f = fopen(path.c_str(), "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            fseek(f, 0, SEEK_SET);
            if (size > 0) {
                buffer.resize(size);
                size_t bytesRead = fread(buffer.data(), 1, size, f);
                buffer.resize(bytesRead);
            }
            fclose(f);
        }
    }
#endif

    size_t getSize() const {
#if AR_USE_USD
        if (asset) {
            return asset->GetSize();
        }
        return 0;
#else
        return buffer.size();
#endif
    }

    size_t read(void* buf, size_t count, size_t offset) const {
#if AR_USE_USD
        if (asset) {
            return asset->Read(buf, count, offset);
        }
        return 0;
#else
        if (offset >= buffer.size()) {
            return 0;
        }
        size_t available = buffer.size() - offset;
        size_t toRead = std::min(count, available);
        memcpy(buf, buffer.data() + offset, toRead);
        return toRead;
#endif
    }
};

// ============================================================================
// MARK: - ArAsset Implementation
// ============================================================================

ArAssetRef ArAsset_Open(ArResolvedPathRef resolvedPath) {
    if (!resolvedPath) return nullptr;
    try {
#if AR_USE_USD
        std::shared_ptr<ArAsset> asset = ArGetResolver().OpenAsset(resolvedPath->path);
        if (!asset) {
            return nullptr;
        }
        return new ArAssetOpaque(asset);
#else
        // Fallback: open from filesystem
        if (resolvedPath->pathString.empty()) {
            return nullptr;
        }
        auto* opaque = new ArAssetOpaque(resolvedPath->pathString);
        if (opaque->buffer.empty() && !resolvedPath->pathString.empty()) {
            // File couldn't be read
            delete opaque;
            return nullptr;
        }
        return opaque;
#endif
    } catch (...) {
        return nullptr;
    }
}

ArAssetRef ArAsset_Retain(ArAssetRef asset) {
    if (!asset) return nullptr;
    asset->refCount.fetch_add(1, std::memory_order_relaxed);
    return asset;
}

void ArAsset_Release(ArAssetRef asset) {
    if (!asset) return;
    if (asset->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete asset;
    }
}

size_t ArAsset_GetSize(ArAssetRef asset) {
    if (!asset) return 0;
    try {
        return asset->getSize();
    } catch (...) {
        return 0;
    }
}

size_t ArAsset_Read(
    ArAssetRef asset,
    void* buffer,
    size_t count,
    size_t offset
) {
    if (!asset || !buffer || count == 0) return 0;
    try {
        return asset->read(buffer, count, offset);
    } catch (...) {
        return 0;
    }
}

void* ArAsset_GetBuffer(ArAssetRef asset, size_t* outSize) {
    if (!asset) {
        if (outSize) *outSize = 0;
        return nullptr;
    }
    try {
        size_t size = asset->getSize();
        if (size == 0) {
            if (outSize) *outSize = 0;
            return nullptr;
        }

        void* buffer = malloc(size);
        if (!buffer) {
            if (outSize) *outSize = 0;
            return nullptr;
        }

        size_t bytesRead = asset->read(buffer, size, 0);
        if (outSize) *outSize = bytesRead;
        return buffer;
    } catch (...) {
        if (outSize) *outSize = 0;
        return nullptr;
    }
}

void ArAsset_FreeBuffer(void* buffer) {
    free(buffer);
}

// ============================================================================
// MARK: - ArWritableAsset Wrapper Structure
// ============================================================================

struct ArWritableAssetOpaque {
#if AR_USE_USD
    std::shared_ptr<ArWritableAsset> asset;
#else
    FILE* file;
    std::string pathString;
    ArWriteMode mode;
    bool closed;
#endif

    ArWritableAssetOpaque()
#if AR_USE_USD
    {
    }
#else
        : file(nullptr)
        , mode(AR_WRITE_MODE_UPDATE)
        , closed(false)
    {
    }
#endif

#if AR_USE_USD
    explicit ArWritableAssetOpaque(std::shared_ptr<ArWritableAsset> a)
        : asset(std::move(a))
    {
    }
#else
    explicit ArWritableAssetOpaque(const std::string& path, ArWriteMode m)
        : pathString(path)
        , mode(m)
        , closed(false)
    {
        // Fallback: open file for writing
        const char* openMode = (mode == AR_WRITE_MODE_UPDATE) ? "r+b" : "wb";
        file = fopen(path.c_str(), openMode);
        if (!file && mode == AR_WRITE_MODE_UPDATE) {
            // File doesn't exist, try creating it
            file = fopen(path.c_str(), "wb");
        }
    }
#endif

    ~ArWritableAssetOpaque() {
#if !AR_USE_USD
        if (file && !closed) {
            fclose(file);
        }
#endif
    }

    size_t write(const void* buf, size_t count, size_t offset) {
#if AR_USE_USD
        if (asset) {
            return asset->Write(buf, count, offset);
        }
        return 0;
#else
        if (!file || closed) return 0;
        if (fseek(file, static_cast<long>(offset), SEEK_SET) != 0) {
            return 0;
        }
        return fwrite(buf, 1, count, file);
#endif
    }

    bool close() {
#if AR_USE_USD
        if (asset) {
            return asset->Close();
        }
        return false;
#else
        if (!file || closed) return false;
        closed = true;
        int result = fclose(file);
        file = nullptr;
        return result == 0;
#endif
    }
};

// ============================================================================
// MARK: - ArWritableAsset Implementation
// ============================================================================

ArWritableAssetRef ArWritableAsset_Open(
    ArResolvedPathRef resolvedPath,
    ArWriteMode writeMode
) {
    if (!resolvedPath) return nullptr;
    try {
#if AR_USE_USD
        pxr::ArResolver::WriteMode mode = (writeMode == AR_WRITE_MODE_REPLACE)
            ? pxr::ArResolver::WriteMode::Replace
            : pxr::ArResolver::WriteMode::Update;
        std::shared_ptr<ArWritableAsset> asset = ArGetResolver().OpenAssetForWrite(resolvedPath->path, mode);
        if (!asset) {
            return nullptr;
        }
        return new ArWritableAssetOpaque(asset);
#else
        // Fallback: open file for writing
        if (resolvedPath->pathString.empty()) {
            return nullptr;
        }
        auto* opaque = new ArWritableAssetOpaque(resolvedPath->pathString, writeMode);
        if (!opaque->file) {
            delete opaque;
            return nullptr;
        }
        return opaque;
#endif
    } catch (...) {
        return nullptr;
    }
}

void ArWritableAsset_Release(ArWritableAssetRef asset) {
    if (asset) {
        delete asset;
    }
}

size_t ArWritableAsset_Write(
    ArWritableAssetRef asset,
    const void* buffer,
    size_t count,
    size_t offset
) {
    if (!asset || !buffer || count == 0) return 0;
    try {
        return asset->write(buffer, count, offset);
    } catch (...) {
        return 0;
    }
}

bool ArWritableAsset_Close(ArWritableAssetRef asset) {
    if (!asset) return false;
    try {
        return asset->close();
    } catch (...) {
        return false;
    }
}

} // extern "C"
