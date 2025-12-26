// Tf.cpp - Token/Type Foundation implementation
#include "../../include/base/Tf.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/base/tf/token.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace tf {

// ============================================================================
// Helper functions to access the underlying pxr::TfToken
// ============================================================================

static inline pxr::TfToken* asToken(void* impl) {
    return static_cast<pxr::TfToken*>(impl);
}

static inline const pxr::TfToken* asToken(const void* impl) {
    return static_cast<const pxr::TfToken*>(impl);
}

// ============================================================================
// TfToken implementation
// ============================================================================

TfToken::TfToken() : impl_(new pxr::TfToken()) {}

TfToken::TfToken(const std::string& text) : impl_(new pxr::TfToken(text)) {}

TfToken::TfToken(const char* text) : impl_(new pxr::TfToken(text)) {}

TfToken::TfToken(const TfToken& other)
    : impl_(new pxr::TfToken(*asToken(other.impl_))) {}

TfToken::TfToken(TfToken&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

TfToken::~TfToken() {
    delete asToken(impl_);
}

TfToken& TfToken::operator=(const TfToken& other) {
    if (this != &other) {
        delete asToken(impl_);
        impl_ = new pxr::TfToken(*asToken(other.impl_));
    }
    return *this;
}

TfToken& TfToken::operator=(TfToken&& other) noexcept {
    if (this != &other) {
        delete asToken(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

const char* TfToken::GetText() const {
    if (!impl_) return "";
    return asToken(impl_)->GetText();
}

std::string TfToken::GetString() const {
    if (!impl_) return "";
    return asToken(impl_)->GetString();
}

size_t TfToken::size() const {
    if (!impl_) return 0;
    return asToken(impl_)->size();
}

bool TfToken::IsEmpty() const {
    if (!impl_) return true;
    return asToken(impl_)->IsEmpty();
}

size_t TfToken::Hash() const {
    if (!impl_) return 0;
    return asToken(impl_)->Hash();
}

bool TfToken::operator==(const TfToken& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *asToken(impl_) == *asToken(other.impl_);
}

bool TfToken::operator!=(const TfToken& other) const {
    return !(*this == other);
}

bool TfToken::operator==(const std::string& other) const {
    if (!impl_) return other.empty();
    return *asToken(impl_) == other;
}

bool TfToken::operator!=(const std::string& other) const {
    return !(*this == other);
}

bool TfToken::operator==(const char* other) const {
    if (!impl_) return other == nullptr || other[0] == '\0';
    return *asToken(impl_) == other;
}

bool TfToken::operator!=(const char* other) const {
    return !(*this == other);
}

bool TfToken::operator<(const TfToken& other) const {
    if (!impl_) return other.impl_ != nullptr;
    if (!other.impl_) return false;
    return *asToken(impl_) < *asToken(other.impl_);
}

bool TfToken::operator>(const TfToken& other) const {
    return other < *this;
}

bool TfToken::operator<=(const TfToken& other) const {
    return !(other < *this);
}

bool TfToken::operator>=(const TfToken& other) const {
    return !(*this < other);
}

void TfToken::Swap(TfToken& other) {
    std::swap(impl_, other.impl_);
}

TfToken TfToken::Find(const std::string& s) {
    pxr::TfToken found = pxr::TfToken::Find(s);
    if (found.IsEmpty()) {
        return TfToken();
    }
    // Use FromImpl to wrap the found token
    return TfToken::FromImpl(new pxr::TfToken(found));
}

TfToken TfToken::FromImpl(void* impl) {
    TfToken token;
    delete asToken(token.impl_);
    token.impl_ = impl;
    return token;
}

} // namespace tf
} // namespace cxxfacade
