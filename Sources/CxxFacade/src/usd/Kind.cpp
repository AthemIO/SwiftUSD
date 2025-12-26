// Kind.cpp - Model hierarchy kinds for scene organization
// Implementation with real USD calls
//
// This file contains the implementation of the Kind facade, which wraps
// the Pixar KindRegistry singleton to provide model hierarchy classification.

#include "../../include/usd/Kind.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/usd/kind/registry.h>

// Alias to avoid conflict with our namespace
namespace pxrKind = pxr;

namespace cxxfacade {
namespace kind {

// ============================================================================
// Helper function to convert between TfToken types
// ============================================================================

/// Convert from cxxfacade::tf::TfToken to pxr::TfToken
static inline pxr::TfToken toPxr(const tf::TfToken& token) {
    // Access the underlying pxr::TfToken via the opaque pointer
    void* impl = token.GetImpl();
    if (!impl) {
        return pxr::TfToken();
    }
    return *static_cast<const pxr::TfToken*>(impl);
}

/// Convert from pxr::TfToken to cxxfacade::tf::TfToken
static inline tf::TfToken fromPxr(const pxr::TfToken& pxrToken) {
    // Create a new TfToken from the pxr token's string
    return tf::TfToken(pxrToken.GetString());
}

// ============================================================================
// Tokens Implementation
// ============================================================================

tf::TfToken Tokens::Model() {
    static tf::TfToken token(pxrKind::KindTokens->model.GetString());
    return token;
}

tf::TfToken Tokens::Component() {
    static tf::TfToken token(pxrKind::KindTokens->component.GetString());
    return token;
}

tf::TfToken Tokens::Group() {
    static tf::TfToken token(pxrKind::KindTokens->group.GetString());
    return token;
}

tf::TfToken Tokens::Assembly() {
    static tf::TfToken token(pxrKind::KindTokens->assembly.GetString());
    return token;
}

tf::TfToken Tokens::SubComponent() {
    static tf::TfToken token(pxrKind::KindTokens->subcomponent.GetString());
    return token;
}

// ============================================================================
// Registry Free Functions Implementation
// ============================================================================

bool HasKind(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    return pxr::KindRegistry::HasKind(pxrKind);
}

tf::TfToken GetBaseKind(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    pxr::TfToken baseKind = pxr::KindRegistry::GetBaseKind(pxrKind);
    return fromPxr(baseKind);
}

bool IsA(const tf::TfToken& derivedKind, const tf::TfToken& baseKind) {
    pxr::TfToken pxrDerived = toPxr(derivedKind);
    pxr::TfToken pxrBase = toPxr(baseKind);
    return pxr::KindRegistry::IsA(pxrDerived, pxrBase);
}

std::vector<tf::TfToken> GetAllKinds() {
    std::vector<pxr::TfToken> pxrKinds = pxr::KindRegistry::GetAllKinds();
    std::vector<tf::TfToken> result;
    result.reserve(pxrKinds.size());
    for (const auto& pxrKind : pxrKinds) {
        result.push_back(fromPxr(pxrKind));
    }
    return result;
}

bool IsModel(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    return pxr::KindRegistry::IsModel(pxrKind);
}

bool IsGroup(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    return pxr::KindRegistry::IsGroup(pxrKind);
}

bool IsAssembly(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    return pxr::KindRegistry::IsAssembly(pxrKind);
}

bool IsComponent(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    return pxr::KindRegistry::IsComponent(pxrKind);
}

bool IsSubComponent(const tf::TfToken& kind) {
    pxr::TfToken pxrKind = toPxr(kind);
    return pxr::KindRegistry::IsSubComponent(pxrKind);
}

} // namespace kind
} // namespace cxxfacade
