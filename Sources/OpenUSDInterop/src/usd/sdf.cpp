// sdf.cpp - SdfPath, SdfLayer, SdfAssetPath C wrapper implementation
// Mirrors: pxr/usd/sdf/path.h, pxr/usd/sdf/layer.h, pxr/usd/sdf/assetPath.h

#include "../../include/usd/sdf.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define SDF_USE_USD 1
    #include <pxr/usd/sdf/path.h>
    #include <pxr/usd/sdf/layer.h>
    #include <pxr/usd/sdf/assetPath.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define SDF_USE_USD 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <mutex>
#include <functional>

// ============================================================================
// MARK: - SdfPath Wrapper Structure
// ============================================================================

struct SdfPathOpaque {
#if SDF_USE_USD
    SdfPath path;
#else
    std::string pathString;
    std::string name;
    size_t elementCount;
    bool isAbsolute;
    bool isEmpty_;
#endif
    std::atomic<int> refCount;

    // Default constructor creates empty path
    SdfPathOpaque()
        : refCount(1)
    {
#if !SDF_USE_USD
        elementCount = 0;
        isAbsolute = false;
        isEmpty_ = true;
#endif
    }

    // Constructor from path string
    explicit SdfPathOpaque(const char* str)
        : refCount(1)
    {
#if SDF_USE_USD
        try {
            if (str && str[0] != '\0') {
                path = SdfPath(str);
            }
        } catch (...) {
            // Leave as empty path
        }
#else
        if (str && str[0] != '\0') {
            pathString = str;
            isAbsolute = (pathString[0] == '/');
            isEmpty_ = false;
            // Simple element count: count '/' characters for absolute paths
            elementCount = 0;
            for (char c : pathString) {
                if (c == '/') elementCount++;
            }
            // Extract name (last component)
            size_t lastSlash = pathString.rfind('/');
            size_t lastDot = pathString.rfind('.');
            if (lastDot != std::string::npos && lastDot > lastSlash) {
                name = pathString.substr(lastDot + 1);
            } else if (lastSlash != std::string::npos) {
                name = pathString.substr(lastSlash + 1);
            } else {
                name = pathString;
            }
        } else {
            elementCount = 0;
            isAbsolute = false;
            isEmpty_ = true;
        }
#endif
    }

#if SDF_USE_USD
    // Constructor from existing SdfPath
    explicit SdfPathOpaque(const SdfPath& p)
        : path(p)
        , refCount(1)
    {
    }
#endif

    const char* getText() const {
#if SDF_USE_USD
        return path.GetText();
#else
        return pathString.c_str();
#endif
    }

    std::string getString() const {
#if SDF_USE_USD
        return path.GetString();
#else
        return pathString;
#endif
    }

    const char* getName() const {
#if SDF_USE_USD
        return path.GetName().c_str();
#else
        return name.c_str();
#endif
    }

    bool isEmpty() const {
#if SDF_USE_USD
        return path.IsEmpty();
#else
        return isEmpty_;
#endif
    }

    size_t getElementCount() const {
#if SDF_USE_USD
        return path.GetPathElementCount();
#else
        return elementCount;
#endif
    }

    size_t getHash() const {
#if SDF_USE_USD
        return path.GetHash();
#else
        return std::hash<std::string>{}(pathString);
#endif
    }
};

// ============================================================================
// MARK: - SdfAssetPath Wrapper Structure
// ============================================================================

struct SdfAssetPathOpaque {
#if SDF_USE_USD
    SdfAssetPath assetPath;
#else
    std::string authoredPath;
    std::string resolvedPath;
#endif
    std::atomic<int> refCount;

    SdfAssetPathOpaque()
        : refCount(1)
    {
    }

    explicit SdfAssetPathOpaque(const char* authored)
        : refCount(1)
    {
#if SDF_USE_USD
        try {
            if (authored) {
                assetPath = SdfAssetPath(authored);
            }
        } catch (...) {
            // Leave as empty
        }
#else
        if (authored) {
            authoredPath = authored;
        }
#endif
    }

    SdfAssetPathOpaque(const char* authored, const char* resolved)
        : refCount(1)
    {
#if SDF_USE_USD
        try {
            if (authored && resolved) {
                assetPath = SdfAssetPath(authored, resolved);
            } else if (authored) {
                assetPath = SdfAssetPath(authored);
            }
        } catch (...) {
            // Leave as empty
        }
#else
        if (authored) authoredPath = authored;
        if (resolved) resolvedPath = resolved;
#endif
    }

#if SDF_USE_USD
    explicit SdfAssetPathOpaque(const SdfAssetPath& ap)
        : assetPath(ap)
        , refCount(1)
    {
    }
#endif

    const char* getAuthoredPath() const {
#if SDF_USE_USD
        return assetPath.GetAuthoredPath().c_str();
#else
        return authoredPath.c_str();
#endif
    }

    const char* getAssetPath() const {
#if SDF_USE_USD
        return assetPath.GetAssetPath().c_str();
#else
        return authoredPath.c_str();
#endif
    }

    const char* getResolvedPath() const {
#if SDF_USE_USD
        return assetPath.GetResolvedPath().c_str();
#else
        return resolvedPath.c_str();
#endif
    }

    size_t getHash() const {
#if SDF_USE_USD
        return assetPath.GetHash();
#else
        return std::hash<std::string>{}(authoredPath);
#endif
    }
};

// ============================================================================
// MARK: - SdfLayer Wrapper Structure
// ============================================================================

struct SdfLayerOpaque {
#if SDF_USE_USD
    SdfLayerRefPtr layer;
#else
    std::string identifier;
    std::string displayName;
    std::string documentation;
    std::string comment;
    std::string content;
    std::vector<std::string> subLayerPaths;
    std::vector<std::string> rootPrimNames;
    double startTimeCode;
    double endTimeCode;
    double timeCodesPerSecond;
    double framesPerSecond;
    bool hasStartTimeCode;
    bool hasEndTimeCode;
    bool hasTimeCodesPerSecond;
    bool hasFramesPerSecond;
    bool isAnonymous;
    bool isDirty;
#endif
    std::atomic<int> refCount;

    SdfLayerOpaque()
        : refCount(1)
    {
#if !SDF_USE_USD
        startTimeCode = 0.0;
        endTimeCode = 0.0;
        timeCodesPerSecond = 24.0;
        framesPerSecond = 24.0;
        hasStartTimeCode = false;
        hasEndTimeCode = false;
        hasTimeCodesPerSecond = false;
        hasFramesPerSecond = false;
        isAnonymous = false;
        isDirty = false;
#endif
    }

#if SDF_USE_USD
    explicit SdfLayerOpaque(const SdfLayerRefPtr& l)
        : layer(l)
        , refCount(1)
    {
    }
#endif

    const char* getIdentifier() const {
#if SDF_USE_USD
        if (layer) {
            return layer->GetIdentifier().c_str();
        }
        return "";
#else
        return identifier.c_str();
#endif
    }

    bool isLayerAnonymous() const {
#if SDF_USE_USD
        return layer && layer->IsAnonymous();
#else
        return isAnonymous;
#endif
    }

    bool isLayerDirty() const {
#if SDF_USE_USD
        return layer && layer->IsDirty();
#else
        return isDirty;
#endif
    }

    bool isLayerEmpty() const {
#if SDF_USE_USD
        return !layer || layer->IsEmpty();
#else
        return content.empty() && rootPrimNames.empty();
#endif
    }
};

// ============================================================================
// MARK: - SdfPath Implementation
// ============================================================================

extern "C" {

SdfPathRef SdfPath_Create(const char* pathString) {
    try {
        return new SdfPathOpaque(pathString);
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_CreateEmpty(void) {
    try {
        return new SdfPathOpaque();
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_Copy(SdfPathRef path) {
    if (!path) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path);
#else
        auto* copy = new SdfPathOpaque();
        copy->pathString = path->pathString;
        copy->name = path->name;
        copy->elementCount = path->elementCount;
        copy->isAbsolute = path->isAbsolute;
        copy->isEmpty_ = path->isEmpty_;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_Retain(SdfPathRef path) {
    if (!path) return nullptr;
    path->refCount.fetch_add(1, std::memory_order_relaxed);
    return path;
}

void SdfPath_Release(SdfPathRef path) {
    if (!path) return;
    if (path->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete path;
    }
}

SdfPathRef SdfPath_AbsoluteRootPath(void) {
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(SdfPath::AbsoluteRootPath());
#else
        return new SdfPathOpaque("/");
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_ReflexiveRelativePath(void) {
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(SdfPath::ReflexiveRelativePath());
#else
        return new SdfPathOpaque(".");
#endif
    } catch (...) {
        return nullptr;
    }
}

const char* SdfPath_GetText(SdfPathRef path) {
    if (!path) return "";
    try {
        return path->getText();
    } catch (...) {
        return "";
    }
}

char* SdfPath_GetString(SdfPathRef path) {
    if (!path) return nullptr;
    try {
        std::string str = path->getString();
        char* result = static_cast<char*>(malloc(str.length() + 1));
        if (result) {
            memcpy(result, str.c_str(), str.length() + 1);
        }
        return result;
    } catch (...) {
        return nullptr;
    }
}

void SdfPath_FreeString(char* str) {
    free(str);
}

const char* SdfPath_GetName(SdfPathRef path) {
    if (!path) return "";
    try {
        return path->getName();
    } catch (...) {
        return "";
    }
}

TfTokenRef SdfPath_GetNameToken(SdfPathRef path) {
    if (!path) return nullptr;
    try {
        return TfToken_Create(path->getName());
    } catch (...) {
        return nullptr;
    }
}

size_t SdfPath_GetElementCount(SdfPathRef path) {
    if (!path) return 0;
    try {
        return path->getElementCount();
    } catch (...) {
        return 0;
    }
}

bool SdfPath_IsEmpty(SdfPathRef path) {
    if (!path) return true;
    try {
        return path->isEmpty();
    } catch (...) {
        return true;
    }
}

bool SdfPath_IsAbsolutePath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsAbsolutePath();
#else
        return path->isAbsolute;
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsAbsoluteRootPath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsAbsoluteRootPath();
#else
        return path->pathString == "/";
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsPrimPath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsPrimPath();
#else
        // Fallback: check if path doesn't contain property indicator
        return !path->isEmpty_ && path->pathString.find('.') == std::string::npos;
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsAbsoluteRootOrPrimPath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsAbsoluteRootOrPrimPath();
#else
        return SdfPath_IsAbsoluteRootPath(path) || SdfPath_IsPrimPath(path);
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsRootPrimPath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsRootPrimPath();
#else
        // Fallback: absolute path with exactly one element
        return path->isAbsolute && path->elementCount == 1 &&
               path->pathString.find('.') == std::string::npos;
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsPropertyPath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsPropertyPath();
#else
        return path->pathString.find('.') != std::string::npos;
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsPrimPropertyPath(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.IsPrimPropertyPath();
#else
        return SdfPath_IsPropertyPath(path);
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_ContainsPrimVariantSelection(SdfPathRef path) {
    if (!path) return false;
    try {
#if SDF_USE_USD
        return path->path.ContainsPrimVariantSelection();
#else
        return path->pathString.find('{') != std::string::npos;
#endif
    } catch (...) {
        return false;
    }
}

size_t SdfPath_Hash(SdfPathRef path) {
    if (!path) return 0;
    try {
        return path->getHash();
    } catch (...) {
        return 0;
    }
}

SdfPathRef SdfPath_GetParentPath(SdfPathRef path) {
    if (!path) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.GetParentPath());
#else
        std::string parent = path->pathString;
        // Find last separator
        size_t lastSlash = parent.rfind('/');
        size_t lastDot = parent.rfind('.');
        if (lastDot != std::string::npos && lastDot > lastSlash) {
            parent = parent.substr(0, lastDot);
        } else if (lastSlash != std::string::npos && lastSlash > 0) {
            parent = parent.substr(0, lastSlash);
        } else if (lastSlash == 0) {
            parent = "/";
        } else {
            parent = "";
        }
        return new SdfPathOpaque(parent.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_GetPrimPath(SdfPathRef path) {
    if (!path) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.GetPrimPath());
#else
        std::string prim = path->pathString;
        size_t dot = prim.find('.');
        if (dot != std::string::npos) {
            prim = prim.substr(0, dot);
        }
        return new SdfPathOpaque(prim.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_GetAbsoluteRootOrPrimPath(SdfPathRef path) {
    if (!path) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.GetAbsoluteRootOrPrimPath());
#else
        return SdfPath_GetPrimPath(path);
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_AppendChild(SdfPathRef path, TfTokenRef childName) {
    if (!path || !childName) return nullptr;
    try {
        const char* name = TfToken_GetText(childName);
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.AppendChild(TfToken(name)));
#else
        std::string newPath = path->pathString;
        if (!newPath.empty() && newPath.back() != '/') {
            newPath += '/';
        }
        newPath += name;
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_AppendChildString(SdfPathRef path, const char* childName) {
    if (!path || !childName) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.AppendChild(TfToken(childName)));
#else
        std::string newPath = path->pathString;
        if (!newPath.empty() && newPath.back() != '/') {
            newPath += '/';
        }
        newPath += childName;
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_AppendProperty(SdfPathRef path, TfTokenRef propName) {
    if (!path || !propName) return nullptr;
    try {
        const char* name = TfToken_GetText(propName);
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.AppendProperty(TfToken(name)));
#else
        std::string newPath = path->pathString;
        newPath += '.';
        newPath += name;
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_AppendPropertyString(SdfPathRef path, const char* propName) {
    if (!path || !propName) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.AppendProperty(TfToken(propName)));
#else
        std::string newPath = path->pathString;
        newPath += '.';
        newPath += propName;
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_AppendVariantSelection(
    SdfPathRef path,
    const char* variantSet,
    const char* variant
) {
    if (!path || !variantSet || !variant) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.AppendVariantSelection(variantSet, variant));
#else
        std::string newPath = path->pathString;
        newPath += '{';
        newPath += variantSet;
        newPath += '=';
        newPath += variant;
        newPath += '}';
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_AppendPath(SdfPathRef path, SdfPathRef suffix) {
    if (!path || !suffix) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.AppendPath(suffix->path));
#else
        std::string newPath = path->pathString;
        if (!newPath.empty() && newPath.back() != '/') {
            newPath += '/';
        }
        newPath += suffix->pathString;
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_ReplacePrefix(
    SdfPathRef path,
    SdfPathRef oldPrefix,
    SdfPathRef newPrefix
) {
    if (!path || !oldPrefix || !newPrefix) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.ReplacePrefix(oldPrefix->path, newPrefix->path));
#else
        std::string pathStr = path->pathString;
        std::string oldStr = oldPrefix->pathString;
        std::string newStr = newPrefix->pathString;
        if (pathStr.find(oldStr) == 0) {
            pathStr.replace(0, oldStr.length(), newStr);
        }
        return new SdfPathOpaque(pathStr.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_GetCommonPrefix(SdfPathRef path1, SdfPathRef path2) {
    if (!path1 || !path2) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path1->path.GetCommonPrefix(path2->path));
#else
        // Fallback: find common prefix string
        const std::string& s1 = path1->pathString;
        const std::string& s2 = path2->pathString;
        size_t minLen = std::min(s1.length(), s2.length());
        size_t commonLen = 0;
        size_t lastSlash = 0;
        for (size_t i = 0; i < minLen; i++) {
            if (s1[i] != s2[i]) break;
            commonLen = i + 1;
            if (s1[i] == '/') lastSlash = i + 1;
        }
        // Return up to last complete path component
        if (commonLen < s1.length() && commonLen < s2.length()) {
            commonLen = lastSlash;
        }
        return new SdfPathOpaque(s1.substr(0, commonLen).c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_MakeAbsolutePath(SdfPathRef path, SdfPathRef anchor) {
    if (!path || !anchor) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.MakeAbsolutePath(anchor->path));
#else
        if (path->isAbsolute) {
            return SdfPath_Copy(path);
        }
        // Simple concatenation for fallback
        std::string newPath = anchor->pathString;
        if (!newPath.empty() && newPath.back() != '/') {
            newPath += '/';
        }
        newPath += path->pathString;
        return new SdfPathOpaque(newPath.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfPathRef SdfPath_MakeRelativePath(SdfPathRef path, SdfPathRef anchor) {
    if (!path || !anchor) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfPathOpaque(path->path.MakeRelativePath(anchor->path));
#else
        // Fallback: just return a copy
        return SdfPath_Copy(path);
#endif
    } catch (...) {
        return nullptr;
    }
}

bool SdfPath_Equal(SdfPathRef lhs, SdfPathRef rhs) {
    if (lhs == rhs) return true;
    if (!lhs || !rhs) return false;
    try {
#if SDF_USE_USD
        return lhs->path == rhs->path;
#else
        return lhs->pathString == rhs->pathString;
#endif
    } catch (...) {
        return false;
    }
}

int SdfPath_Compare(SdfPathRef lhs, SdfPathRef rhs) {
    if (lhs == rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    try {
#if SDF_USE_USD
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

bool SdfPath_HasPrefix(SdfPathRef path, SdfPathRef prefix) {
    if (!path || !prefix) return false;
    try {
#if SDF_USE_USD
        return path->path.HasPrefix(prefix->path);
#else
        return path->pathString.find(prefix->pathString) == 0;
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsValidPathString(const char* pathString) {
    if (!pathString) return false;
    try {
#if SDF_USE_USD
        std::string errMsg;
        return SdfPath::IsValidPathString(pathString, &errMsg);
#else
        // Fallback: basic validation
        return pathString[0] == '/' || pathString[0] == '.' ||
               (pathString[0] >= 'A' && pathString[0] <= 'z');
#endif
    } catch (...) {
        return false;
    }
}

bool SdfPath_IsValidIdentifier(const char* name) {
    if (!name || name[0] == '\0') return false;
    try {
#if SDF_USE_USD
        return SdfPath::IsValidIdentifier(name);
#else
        // Fallback: basic identifier validation
        if (!((name[0] >= 'A' && name[0] <= 'Z') ||
              (name[0] >= 'a' && name[0] <= 'z') ||
              name[0] == '_')) {
            return false;
        }
        for (int i = 1; name[i] != '\0'; i++) {
            if (!((name[i] >= 'A' && name[i] <= 'Z') ||
                  (name[i] >= 'a' && name[i] <= 'z') ||
                  (name[i] >= '0' && name[i] <= '9') ||
                  name[i] == '_')) {
                return false;
            }
        }
        return true;
#endif
    } catch (...) {
        return false;
    }
}

size_t SdfPath_GetPrefixes(
    SdfPathRef path,
    SdfPathRef* outPaths,
    size_t maxCount
) {
    if (!path || !outPaths || maxCount == 0) return 0;
    try {
#if SDF_USE_USD
        SdfPathVector prefixes;
        path->path.GetPrefixes(&prefixes);
        size_t count = std::min(prefixes.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            outPaths[i] = new SdfPathOpaque(prefixes[i]);
        }
        return count;
#else
        // Fallback: build prefixes from path string
        std::vector<std::string> prefixes;
        std::string current;
        for (char c : path->pathString) {
            current += c;
            if (c == '/') {
                prefixes.push_back(current);
            }
        }
        if (!current.empty() && current.back() != '/') {
            prefixes.push_back(current);
        }
        size_t count = std::min(prefixes.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            outPaths[i] = new SdfPathOpaque(prefixes[i].c_str());
        }
        return count;
#endif
    } catch (...) {
        return 0;
    }
}

size_t SdfPath_GetPrefixCount(SdfPathRef path) {
    if (!path) return 0;
    try {
#if SDF_USE_USD
        return path->path.GetPathElementCount() + 1; // +1 for root
#else
        return path->elementCount + 1;
#endif
    } catch (...) {
        return 0;
    }
}

void SdfPath_ReleaseBatch(SdfPathRef* paths, size_t count) {
    if (!paths) return;
    for (size_t i = 0; i < count; i++) {
        SdfPath_Release(paths[i]);
        paths[i] = nullptr;
    }
}

// ============================================================================
// MARK: - SdfAssetPath Implementation
// ============================================================================

SdfAssetPathRef SdfAssetPath_Create(const char* authoredPath) {
    try {
        return new SdfAssetPathOpaque(authoredPath);
    } catch (...) {
        return nullptr;
    }
}

SdfAssetPathRef SdfAssetPath_CreateWithResolvedPath(
    const char* authoredPath,
    const char* resolvedPath
) {
    try {
        return new SdfAssetPathOpaque(authoredPath, resolvedPath);
    } catch (...) {
        return nullptr;
    }
}

SdfAssetPathRef SdfAssetPath_CreateEmpty(void) {
    try {
        return new SdfAssetPathOpaque();
    } catch (...) {
        return nullptr;
    }
}

SdfAssetPathRef SdfAssetPath_Copy(SdfAssetPathRef assetPath) {
    if (!assetPath) return nullptr;
    try {
#if SDF_USE_USD
        return new SdfAssetPathOpaque(assetPath->assetPath);
#else
        auto* copy = new SdfAssetPathOpaque();
        copy->authoredPath = assetPath->authoredPath;
        copy->resolvedPath = assetPath->resolvedPath;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfAssetPathRef SdfAssetPath_Retain(SdfAssetPathRef assetPath) {
    if (!assetPath) return nullptr;
    assetPath->refCount.fetch_add(1, std::memory_order_relaxed);
    return assetPath;
}

void SdfAssetPath_Release(SdfAssetPathRef assetPath) {
    if (!assetPath) return;
    if (assetPath->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete assetPath;
    }
}

const char* SdfAssetPath_GetAuthoredPath(SdfAssetPathRef assetPath) {
    if (!assetPath) return "";
    try {
        return assetPath->getAuthoredPath();
    } catch (...) {
        return "";
    }
}

const char* SdfAssetPath_GetAssetPath(SdfAssetPathRef assetPath) {
    if (!assetPath) return "";
    try {
        return assetPath->getAssetPath();
    } catch (...) {
        return "";
    }
}

const char* SdfAssetPath_GetResolvedPath(SdfAssetPathRef assetPath) {
    if (!assetPath) return "";
    try {
        return assetPath->getResolvedPath();
    } catch (...) {
        return "";
    }
}

size_t SdfAssetPath_Hash(SdfAssetPathRef assetPath) {
    if (!assetPath) return 0;
    try {
        return assetPath->getHash();
    } catch (...) {
        return 0;
    }
}

bool SdfAssetPath_Equal(SdfAssetPathRef lhs, SdfAssetPathRef rhs) {
    if (lhs == rhs) return true;
    if (!lhs || !rhs) return false;
    try {
#if SDF_USE_USD
        return lhs->assetPath == rhs->assetPath;
#else
        return lhs->authoredPath == rhs->authoredPath &&
               lhs->resolvedPath == rhs->resolvedPath;
#endif
    } catch (...) {
        return false;
    }
}

int SdfAssetPath_Compare(SdfAssetPathRef lhs, SdfAssetPathRef rhs) {
    if (lhs == rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    try {
#if SDF_USE_USD
        if (lhs->assetPath < rhs->assetPath) return -1;
        if (rhs->assetPath < lhs->assetPath) return 1;
        return 0;
#else
        int cmp = lhs->authoredPath.compare(rhs->authoredPath);
        if (cmp != 0) return cmp;
        return lhs->resolvedPath.compare(rhs->resolvedPath);
#endif
    } catch (...) {
        return 0;
    }
}

// ============================================================================
// MARK: - SdfLayer Implementation
// ============================================================================

SdfLayerRef SdfLayer_CreateNew(const char* identifier) {
    if (!identifier) return nullptr;
    try {
#if SDF_USE_USD
        SdfLayerRefPtr layer = SdfLayer::CreateNew(identifier);
        if (!layer) return nullptr;
        return new SdfLayerOpaque(layer);
#else
        auto* wrapper = new SdfLayerOpaque();
        wrapper->identifier = identifier;
        wrapper->isDirty = true;
        return wrapper;
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfLayerRef SdfLayer_CreateAnonymous(const char* tag) {
    try {
#if SDF_USE_USD
        SdfLayerRefPtr layer = SdfLayer::CreateAnonymous(tag ? tag : "");
        if (!layer) return nullptr;
        return new SdfLayerOpaque(layer);
#else
        auto* wrapper = new SdfLayerOpaque();
        wrapper->identifier = std::string("anon:") + (tag ? tag : "");
        wrapper->isAnonymous = true;
        return wrapper;
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfLayerRef SdfLayer_FindOrOpen(const char* identifier) {
    if (!identifier) return nullptr;
    try {
#if SDF_USE_USD
        SdfLayerRefPtr layer = SdfLayer::FindOrOpen(identifier);
        if (!layer) return nullptr;
        return new SdfLayerOpaque(layer);
#else
        auto* wrapper = new SdfLayerOpaque();
        wrapper->identifier = identifier;
        return wrapper;
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfLayerRef SdfLayer_Find(const char* identifier) {
    if (!identifier) return nullptr;
    try {
#if SDF_USE_USD
        SdfLayerHandle layer = SdfLayer::Find(identifier);
        if (!layer) return nullptr;
        // Convert handle to refptr for our wrapper
        SdfLayerRefPtr refPtr = TfCreateRefPtrFromProtectedWeakPtr(layer);
        if (!refPtr) return nullptr;
        return new SdfLayerOpaque(refPtr);
#else
        return nullptr; // Fallback: cannot find layers
#endif
    } catch (...) {
        return nullptr;
    }
}

SdfLayerRef SdfLayer_Retain(SdfLayerRef layer) {
    if (!layer) return nullptr;
    layer->refCount.fetch_add(1, std::memory_order_relaxed);
    return layer;
}

void SdfLayer_Release(SdfLayerRef layer) {
    if (!layer) return;
    if (layer->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete layer;
    }
}

const char* SdfLayer_GetIdentifier(SdfLayerRef layer) {
    if (!layer) return "";
    try {
        return layer->getIdentifier();
    } catch (...) {
        return "";
    }
}

char* SdfLayer_GetDisplayName(SdfLayerRef layer) {
    if (!layer) return nullptr;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            std::string name = layer->layer->GetDisplayName();
            char* result = static_cast<char*>(malloc(name.length() + 1));
            if (result) {
                memcpy(result, name.c_str(), name.length() + 1);
            }
            return result;
        }
        return nullptr;
#else
        // Extract filename from identifier
        std::string name = layer->identifier;
        size_t lastSlash = name.rfind('/');
        if (lastSlash != std::string::npos) {
            name = name.substr(lastSlash + 1);
        }
        char* result = static_cast<char*>(malloc(name.length() + 1));
        if (result) {
            memcpy(result, name.c_str(), name.length() + 1);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

const char* SdfLayer_GetRealPath(SdfLayerRef layer) {
    if (!layer) return "";
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetRealPath().c_str();
        }
        return "";
#else
        return layer->identifier.c_str();
#endif
    } catch (...) {
        return "";
    }
}

char* SdfLayer_GetFileExtension(SdfLayerRef layer) {
    if (!layer) return nullptr;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            std::string ext = layer->layer->GetFileExtension();
            char* result = static_cast<char*>(malloc(ext.length() + 1));
            if (result) {
                memcpy(result, ext.c_str(), ext.length() + 1);
            }
            return result;
        }
        return nullptr;
#else
        std::string ext;
        size_t dot = layer->identifier.rfind('.');
        if (dot != std::string::npos) {
            ext = layer->identifier.substr(dot + 1);
        }
        char* result = static_cast<char*>(malloc(ext.length() + 1));
        if (result) {
            memcpy(result, ext.c_str(), ext.length() + 1);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

void SdfLayer_FreeString(char* str) {
    free(str);
}

bool SdfLayer_IsAnonymous(SdfLayerRef layer) {
    if (!layer) return false;
    try {
        return layer->isLayerAnonymous();
    } catch (...) {
        return false;
    }
}

bool SdfLayer_IsDirty(SdfLayerRef layer) {
    if (!layer) return false;
    try {
        return layer->isLayerDirty();
    } catch (...) {
        return false;
    }
}

bool SdfLayer_IsEmpty(SdfLayerRef layer) {
    if (!layer) return true;
    try {
        return layer->isLayerEmpty();
    } catch (...) {
        return true;
    }
}

bool SdfLayer_IsMuted(SdfLayerRef layer) {
    if (!layer) return false;
    try {
#if SDF_USE_USD
        return layer->layer && layer->layer->IsMuted();
#else
        return false;
#endif
    } catch (...) {
        return false;
    }
}

UsdResult SdfLayer_Clear(SdfLayerRef layer) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->Clear();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->content.clear();
        layer->rootPrimNames.clear();
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

UsdResult SdfLayer_Save(SdfLayerRef layer, bool force) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            bool success = layer->layer->Save(force);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_IO_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        layer->isDirty = false;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

UsdResult SdfLayer_Export(
    SdfLayerRef layer,
    const char* filename,
    const char* comment
) {
    if (!layer || !filename) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            bool success = layer->layer->Export(filename, comment ? comment : "");
            return success ? USD_RESULT_SUCCESS : USD_RESULT_IO_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

char* SdfLayer_ExportToString(SdfLayerRef layer) {
    if (!layer) return nullptr;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            std::string result;
            if (layer->layer->ExportToString(&result)) {
                char* str = static_cast<char*>(malloc(result.length() + 1));
                if (str) {
                    memcpy(str, result.c_str(), result.length() + 1);
                }
                return str;
            }
        }
        return nullptr;
#else
        char* str = static_cast<char*>(malloc(layer->content.length() + 1));
        if (str) {
            memcpy(str, layer->content.c_str(), layer->content.length() + 1);
        }
        return str;
#endif
    } catch (...) {
        return nullptr;
    }
}

UsdResult SdfLayer_ImportFromString(SdfLayerRef layer, const char* content) {
    if (!layer || !content) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            bool success = layer->layer->ImportFromString(content);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        layer->content = content;
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

UsdResult SdfLayer_Reload(SdfLayerRef layer, bool force) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            bool success = layer->layer->Reload(force);
            return success ? USD_RESULT_SUCCESS : USD_RESULT_IO_ERROR;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

TfTokenRef SdfLayer_GetDefaultPrim(SdfLayerRef layer) {
    if (!layer) return nullptr;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            TfToken token = layer->layer->GetDefaultPrim();
            if (!token.IsEmpty()) {
                return TfToken_Create(token.GetText());
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } catch (...) {
        return nullptr;
    }
}

UsdResult SdfLayer_SetDefaultPrim(SdfLayerRef layer, TfTokenRef name) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            if (name) {
                layer->layer->SetDefaultPrim(TfToken(TfToken_GetText(name)));
            } else {
                layer->layer->ClearDefaultPrim();
            }
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

char* SdfLayer_GetDocumentation(SdfLayerRef layer) {
    if (!layer) return nullptr;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            std::string doc = layer->layer->GetDocumentation();
            char* result = static_cast<char*>(malloc(doc.length() + 1));
            if (result) {
                memcpy(result, doc.c_str(), doc.length() + 1);
            }
            return result;
        }
        return nullptr;
#else
        char* result = static_cast<char*>(malloc(layer->documentation.length() + 1));
        if (result) {
            memcpy(result, layer->documentation.c_str(), layer->documentation.length() + 1);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

UsdResult SdfLayer_SetDocumentation(SdfLayerRef layer, const char* doc) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->SetDocumentation(doc ? doc : "");
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->documentation = doc ? doc : "";
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

char* SdfLayer_GetComment(SdfLayerRef layer) {
    if (!layer) return nullptr;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            std::string comment = layer->layer->GetComment();
            char* result = static_cast<char*>(malloc(comment.length() + 1));
            if (result) {
                memcpy(result, comment.c_str(), comment.length() + 1);
            }
            return result;
        }
        return nullptr;
#else
        char* result = static_cast<char*>(malloc(layer->comment.length() + 1));
        if (result) {
            memcpy(result, layer->comment.c_str(), layer->comment.length() + 1);
        }
        return result;
#endif
    } catch (...) {
        return nullptr;
    }
}

UsdResult SdfLayer_SetComment(SdfLayerRef layer, const char* comment) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->SetComment(comment ? comment : "");
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->comment = comment ? comment : "";
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

double SdfLayer_GetStartTimeCode(SdfLayerRef layer) {
    if (!layer) return 0.0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetStartTimeCode();
        }
        return 0.0;
#else
        return layer->startTimeCode;
#endif
    } catch (...) {
        return 0.0;
    }
}

UsdResult SdfLayer_SetStartTimeCode(SdfLayerRef layer, double timeCode) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->SetStartTimeCode(timeCode);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->startTimeCode = timeCode;
        layer->hasStartTimeCode = true;
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

bool SdfLayer_HasStartTimeCode(SdfLayerRef layer) {
    if (!layer) return false;
    try {
#if SDF_USE_USD
        return layer->layer && layer->layer->HasStartTimeCode();
#else
        return layer->hasStartTimeCode;
#endif
    } catch (...) {
        return false;
    }
}

double SdfLayer_GetEndTimeCode(SdfLayerRef layer) {
    if (!layer) return 0.0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetEndTimeCode();
        }
        return 0.0;
#else
        return layer->endTimeCode;
#endif
    } catch (...) {
        return 0.0;
    }
}

UsdResult SdfLayer_SetEndTimeCode(SdfLayerRef layer, double timeCode) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->SetEndTimeCode(timeCode);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->endTimeCode = timeCode;
        layer->hasEndTimeCode = true;
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

bool SdfLayer_HasEndTimeCode(SdfLayerRef layer) {
    if (!layer) return false;
    try {
#if SDF_USE_USD
        return layer->layer && layer->layer->HasEndTimeCode();
#else
        return layer->hasEndTimeCode;
#endif
    } catch (...) {
        return false;
    }
}

double SdfLayer_GetTimeCodesPerSecond(SdfLayerRef layer) {
    if (!layer) return 24.0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetTimeCodesPerSecond();
        }
        return 24.0;
#else
        return layer->timeCodesPerSecond;
#endif
    } catch (...) {
        return 24.0;
    }
}

UsdResult SdfLayer_SetTimeCodesPerSecond(SdfLayerRef layer, double tcps) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->SetTimeCodesPerSecond(tcps);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->timeCodesPerSecond = tcps;
        layer->hasTimeCodesPerSecond = true;
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

bool SdfLayer_HasTimeCodesPerSecond(SdfLayerRef layer) {
    if (!layer) return false;
    try {
#if SDF_USE_USD
        return layer->layer && layer->layer->HasTimeCodesPerSecond();
#else
        return layer->hasTimeCodesPerSecond;
#endif
    } catch (...) {
        return false;
    }
}

double SdfLayer_GetFramesPerSecond(SdfLayerRef layer) {
    if (!layer) return 24.0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetFramesPerSecond();
        }
        return 24.0;
#else
        return layer->framesPerSecond;
#endif
    } catch (...) {
        return 24.0;
    }
}

UsdResult SdfLayer_SetFramesPerSecond(SdfLayerRef layer, double fps) {
    if (!layer) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->SetFramesPerSecond(fps);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        layer->framesPerSecond = fps;
        layer->hasFramesPerSecond = true;
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

bool SdfLayer_HasFramesPerSecond(SdfLayerRef layer) {
    if (!layer) return false;
    try {
#if SDF_USE_USD
        return layer->layer && layer->layer->HasFramesPerSecond();
#else
        return layer->hasFramesPerSecond;
#endif
    } catch (...) {
        return false;
    }
}

size_t SdfLayer_GetNumSubLayerPaths(SdfLayerRef layer) {
    if (!layer) return 0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetNumSubLayerPaths();
        }
        return 0;
#else
        return layer->subLayerPaths.size();
#endif
    } catch (...) {
        return 0;
    }
}

size_t SdfLayer_GetSubLayerPaths(
    SdfLayerRef layer,
    char** outPaths,
    size_t maxCount
) {
    if (!layer || !outPaths || maxCount == 0) return 0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            auto proxy = layer->layer->GetSubLayerPaths();
            size_t count = std::min(static_cast<size_t>(proxy.size()), maxCount);
            for (size_t i = 0; i < count; i++) {
                const std::string& path = proxy[i];
                outPaths[i] = static_cast<char*>(malloc(path.length() + 1));
                if (outPaths[i]) {
                    memcpy(outPaths[i], path.c_str(), path.length() + 1);
                }
            }
            return count;
        }
        return 0;
#else
        size_t count = std::min(layer->subLayerPaths.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            const std::string& path = layer->subLayerPaths[i];
            outPaths[i] = static_cast<char*>(malloc(path.length() + 1));
            if (outPaths[i]) {
                memcpy(outPaths[i], path.c_str(), path.length() + 1);
            }
        }
        return count;
#endif
    } catch (...) {
        return 0;
    }
}

UsdResult SdfLayer_InsertSubLayerPath(
    SdfLayerRef layer,
    const char* path,
    int index
) {
    if (!layer || !path) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->InsertSubLayerPath(path, index);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        if (index < 0 || static_cast<size_t>(index) >= layer->subLayerPaths.size()) {
            layer->subLayerPaths.push_back(path);
        } else {
            layer->subLayerPaths.insert(
                layer->subLayerPaths.begin() + index,
                path
            );
        }
        layer->isDirty = true;
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

UsdResult SdfLayer_RemoveSubLayerPath(SdfLayerRef layer, int index) {
    if (!layer || index < 0) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            layer->layer->RemoveSubLayerPath(index);
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        if (static_cast<size_t>(index) < layer->subLayerPaths.size()) {
            layer->subLayerPaths.erase(layer->subLayerPaths.begin() + index);
            layer->isDirty = true;
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_INVALID_ARGUMENT;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

bool SdfLayer_HasSpec(SdfLayerRef layer, SdfPathRef path) {
    if (!layer || !path) return false;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->HasSpec(path->path);
        }
        return false;
#else
        return false;
#endif
    } catch (...) {
        return false;
    }
}

UsdResult SdfLayer_Traverse(
    SdfLayerRef layer,
    SdfPathRef rootPath,
    SdfLayerTraverseCallback callback,
    void* userData
) {
    if (!layer || !rootPath || !callback) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            // Use a lambda that calls the C callback
            layer->layer->Traverse(
                rootPath->path,
                [callback, userData](const SdfPath& p) {
                    callback(p.GetText(), userData);
                }
            );
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        // Fallback: just call callback with root path
        callback(rootPath->getText(), userData);
        return USD_RESULT_SUCCESS;
#endif
    } catch (...) {
        return USD_RESULT_ERROR;
    }
}

size_t SdfLayer_GetRootPrimCount(SdfLayerRef layer) {
    if (!layer) return 0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            return layer->layer->GetRootPrims().size();
        }
        return 0;
#else
        return layer->rootPrimNames.size();
#endif
    } catch (...) {
        return 0;
    }
}

size_t SdfLayer_GetRootPrimNames(
    SdfLayerRef layer,
    TfTokenRef* outNames,
    size_t maxCount
) {
    if (!layer || !outNames || maxCount == 0) return 0;
    try {
#if SDF_USE_USD
        if (layer->layer) {
            auto rootPrims = layer->layer->GetRootPrims();
            size_t count = std::min(rootPrims.size(), maxCount);
            size_t i = 0;
            for (const auto& prim : rootPrims) {
                if (i >= count) break;
                outNames[i] = TfToken_Create(prim->GetName().c_str());
                i++;
            }
            return i;
        }
        return 0;
#else
        size_t count = std::min(layer->rootPrimNames.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            outNames[i] = TfToken_Create(layer->rootPrimNames[i].c_str());
        }
        return count;
#endif
    } catch (...) {
        return 0;
    }
}

} // extern "C"
