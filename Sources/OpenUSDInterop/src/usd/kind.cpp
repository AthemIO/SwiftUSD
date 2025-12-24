// kind.cpp - Kind module C wrapper implementation
// Mirrors: pxr/usd/kind/registry.h

#include "../../include/usd/kind.h"
#include "../../include/base/tf.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define KIND_USE_USD 1
    #include <pxr/usd/kind/registry.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define KIND_USE_USD 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_set>

// MARK: - Internal Helper Functions

#if KIND_USE_USD

// Helper to extract TfToken from TfTokenRef
// Assumes TfTokenOpaque has a 'token' member when USE_PIXAR_USD is defined
struct TfTokenOpaque;
extern const TfToken& TfTokenOpaque_GetToken(TfTokenRef ref);

// We need to access the internal TfToken from TfTokenRef
// This is defined in tf.cpp and we rely on the struct layout
// For now, we'll use a simpler approach: re-create the token from text

static TfToken GetTokenFromRef(TfTokenRef ref) {
    if (!ref) {
        return TfToken();
    }
    const char* text = TfToken_GetText(ref);
    return TfToken(text ? text : "");
}

#endif

// MARK: - Fallback Kind Data (when not using full USD)

#if !KIND_USE_USD

// Simple fallback implementation with built-in kinds
// The kind hierarchy is:
//   model
//     component
//     group
//       assembly
//   subcomponent

struct FallbackKindData {
    std::string name;
    std::string baseKind;
};

static const FallbackKindData g_fallbackKinds[] = {
    {"model", ""},
    {"component", "model"},
    {"group", "model"},
    {"assembly", "group"},
    {"subcomponent", ""}
};

static const size_t g_fallbackKindsCount = sizeof(g_fallbackKinds) / sizeof(g_fallbackKinds[0]);

static const FallbackKindData* FindFallbackKind(const char* name) {
    if (!name) return nullptr;
    for (size_t i = 0; i < g_fallbackKindsCount; i++) {
        if (g_fallbackKinds[i].name == name) {
            return &g_fallbackKinds[i];
        }
    }
    return nullptr;
}

static bool FallbackIsA(const char* derived, const char* base) {
    if (!derived || !base) return false;
    if (strcmp(derived, base) == 0) return true;

    // Walk up the hierarchy
    const FallbackKindData* current = FindFallbackKind(derived);
    while (current && !current->baseKind.empty()) {
        if (current->baseKind == base) {
            return true;
        }
        current = FindFallbackKind(current->baseKind.c_str());
    }
    return false;
}

#endif

// MARK: - Kind Queries

extern "C" {

bool KindRegistry_HasKind(TfTokenRef kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        return KindRegistry::HasKind(token);
#else
        const char* text = TfToken_GetText(kind);
        return FindFallbackKind(text) != nullptr;
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_HasKindString(const char* kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::HasKind(TfToken(kind));
#else
        return FindFallbackKind(kind) != nullptr;
#endif
    } catch (...) {
        return false;
    }
}

TfTokenRef KindRegistry_GetBaseKind(TfTokenRef kind) {
    if (!kind) return nullptr;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        TfToken baseKind = KindRegistry::GetBaseKind(token);
        if (baseKind.IsEmpty()) {
            return nullptr;
        }
        return TfToken_Create(baseKind.GetText());
#else
        const char* text = TfToken_GetText(kind);
        const FallbackKindData* data = FindFallbackKind(text);
        if (!data || data->baseKind.empty()) {
            return nullptr;
        }
        return TfToken_Create(data->baseKind.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef KindRegistry_GetBaseKindString(const char* kind) {
    if (!kind) return nullptr;
    try {
#if KIND_USE_USD
        TfToken baseKind = KindRegistry::GetBaseKind(TfToken(kind));
        if (baseKind.IsEmpty()) {
            return nullptr;
        }
        return TfToken_Create(baseKind.GetText());
#else
        const FallbackKindData* data = FindFallbackKind(kind);
        if (!data || data->baseKind.empty()) {
            return nullptr;
        }
        return TfToken_Create(data->baseKind.c_str());
#endif
    } catch (...) {
        return nullptr;
    }
}

bool KindRegistry_IsA(TfTokenRef derivedKind, TfTokenRef baseKind) {
    if (!derivedKind || !baseKind) return false;
    try {
#if KIND_USE_USD
        TfToken derived = GetTokenFromRef(derivedKind);
        TfToken base = GetTokenFromRef(baseKind);
        return KindRegistry::IsA(derived, base);
#else
        const char* derivedText = TfToken_GetText(derivedKind);
        const char* baseText = TfToken_GetText(baseKind);
        return FallbackIsA(derivedText, baseText);
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsAString(const char* derivedKind, const char* baseKind) {
    if (!derivedKind || !baseKind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::IsA(TfToken(derivedKind), TfToken(baseKind));
#else
        return FallbackIsA(derivedKind, baseKind);
#endif
    } catch (...) {
        return false;
    }
}

// MARK: - Built-in Kind Tests

bool KindRegistry_IsModel(TfTokenRef kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        return KindRegistry::IsA(token, KindTokens->model);
#else
        const char* text = TfToken_GetText(kind);
        return FallbackIsA(text, "model");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsModelString(const char* kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::IsA(TfToken(kind), KindTokens->model);
#else
        return FallbackIsA(kind, "model");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsGroup(TfTokenRef kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        return KindRegistry::IsA(token, KindTokens->group);
#else
        const char* text = TfToken_GetText(kind);
        return FallbackIsA(text, "group");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsGroupString(const char* kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::IsA(TfToken(kind), KindTokens->group);
#else
        return FallbackIsA(kind, "group");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsAssembly(TfTokenRef kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        return KindRegistry::IsA(token, KindTokens->assembly);
#else
        const char* text = TfToken_GetText(kind);
        return FallbackIsA(text, "assembly");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsAssemblyString(const char* kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::IsA(TfToken(kind), KindTokens->assembly);
#else
        return FallbackIsA(kind, "assembly");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsComponent(TfTokenRef kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        return KindRegistry::IsA(token, KindTokens->component);
#else
        const char* text = TfToken_GetText(kind);
        return FallbackIsA(text, "component");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsComponentString(const char* kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::IsA(TfToken(kind), KindTokens->component);
#else
        return FallbackIsA(kind, "component");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsSubComponent(TfTokenRef kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        TfToken token = GetTokenFromRef(kind);
        return KindRegistry::IsA(token, KindTokens->subcomponent);
#else
        const char* text = TfToken_GetText(kind);
        return FallbackIsA(text, "subcomponent");
#endif
    } catch (...) {
        return false;
    }
}

bool KindRegistry_IsSubComponentString(const char* kind) {
    if (!kind) return false;
    try {
#if KIND_USE_USD
        return KindRegistry::IsA(TfToken(kind), KindTokens->subcomponent);
#else
        return FallbackIsA(kind, "subcomponent");
#endif
    } catch (...) {
        return false;
    }
}

// MARK: - All Kinds Enumeration

size_t KindRegistry_GetAllKindsCount(void) {
    try {
#if KIND_USE_USD
        std::vector<TfToken> allKinds = KindRegistry::GetAllKinds();
        return allKinds.size();
#else
        return g_fallbackKindsCount;
#endif
    } catch (...) {
        return 0;
    }
}

size_t KindRegistry_GetAllKinds(TfTokenRef* outKinds, size_t maxCount) {
    if (!outKinds || maxCount == 0) return 0;
    try {
#if KIND_USE_USD
        std::vector<TfToken> allKinds = KindRegistry::GetAllKinds();
        size_t count = std::min(allKinds.size(), maxCount);
        for (size_t i = 0; i < count; i++) {
            outKinds[i] = TfToken_Create(allKinds[i].GetText());
        }
        return count;
#else
        size_t count = std::min(g_fallbackKindsCount, maxCount);
        for (size_t i = 0; i < count; i++) {
            outKinds[i] = TfToken_Create(g_fallbackKinds[i].name.c_str());
        }
        return count;
#endif
    } catch (...) {
        return 0;
    }
}

// MARK: - KindTokens (Built-in Kind Constants)

TfTokenRef KindTokens_Model(void) {
    try {
#if KIND_USE_USD
        return TfToken_Create(KindTokens->model.GetText());
#else
        return TfToken_Create("model");
#endif
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef KindTokens_Component(void) {
    try {
#if KIND_USE_USD
        return TfToken_Create(KindTokens->component.GetText());
#else
        return TfToken_Create("component");
#endif
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef KindTokens_Group(void) {
    try {
#if KIND_USE_USD
        return TfToken_Create(KindTokens->group.GetText());
#else
        return TfToken_Create("group");
#endif
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef KindTokens_Assembly(void) {
    try {
#if KIND_USE_USD
        return TfToken_Create(KindTokens->assembly.GetText());
#else
        return TfToken_Create("assembly");
#endif
    } catch (...) {
        return nullptr;
    }
}

TfTokenRef KindTokens_Subcomponent(void) {
    try {
#if KIND_USE_USD
        return TfToken_Create(KindTokens->subcomponent.GetText());
#else
        return TfToken_Create("subcomponent");
#endif
    } catch (...) {
        return nullptr;
    }
}

} // extern "C"
