// Sdf.cpp - Scene Description Foundation implementation
#include "../../include/usd/Sdf.hpp"

#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/layer.h>
#include <pxr/usd/sdf/assetPath.h>
#include <pxr/usd/sdf/layerOffset.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace sdf {

// ============================================================================
// SdfLayerOffset implementation
// ============================================================================

static inline pxr::SdfLayerOffset* asLayerOffset(void* impl) {
    return static_cast<pxr::SdfLayerOffset*>(impl);
}

SdfLayerOffset::SdfLayerOffset(double offset, double scale)
    : impl_(new pxr::SdfLayerOffset(offset, scale)) {}

SdfLayerOffset::SdfLayerOffset(const SdfLayerOffset& other)
    : impl_(new pxr::SdfLayerOffset(*asLayerOffset(other.impl_))) {}

SdfLayerOffset::SdfLayerOffset(SdfLayerOffset&& other) noexcept
    : impl_(other.impl_) {
    other.impl_ = nullptr;
}

SdfLayerOffset::~SdfLayerOffset() {
    delete asLayerOffset(impl_);
}

SdfLayerOffset& SdfLayerOffset::operator=(const SdfLayerOffset& other) {
    if (this != &other) {
        delete asLayerOffset(impl_);
        impl_ = new pxr::SdfLayerOffset(*asLayerOffset(other.impl_));
    }
    return *this;
}

SdfLayerOffset& SdfLayerOffset::operator=(SdfLayerOffset&& other) noexcept {
    if (this != &other) {
        delete asLayerOffset(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

double SdfLayerOffset::GetOffset() const {
    if (!impl_) return 0.0;
    return asLayerOffset(impl_)->GetOffset();
}

double SdfLayerOffset::GetScale() const {
    if (!impl_) return 1.0;
    return asLayerOffset(impl_)->GetScale();
}

void SdfLayerOffset::SetOffset(double newOffset) {
    if (impl_) {
        asLayerOffset(impl_)->SetOffset(newOffset);
    }
}

void SdfLayerOffset::SetScale(double newScale) {
    if (impl_) {
        asLayerOffset(impl_)->SetScale(newScale);
    }
}

bool SdfLayerOffset::IsIdentity() const {
    if (!impl_) return true;
    return asLayerOffset(impl_)->IsIdentity();
}

bool SdfLayerOffset::IsValid() const {
    if (!impl_) return false;
    return asLayerOffset(impl_)->IsValid();
}

SdfLayerOffset SdfLayerOffset::GetInverse() const {
    if (!impl_) return SdfLayerOffset();
    return SdfLayerOffset::FromImpl(
        new pxr::SdfLayerOffset(asLayerOffset(impl_)->GetInverse()));
}

size_t SdfLayerOffset::GetHash() const {
    if (!impl_) return 0;
    return asLayerOffset(impl_)->GetHash();
}

bool SdfLayerOffset::operator==(const SdfLayerOffset& rhs) const {
    if (!impl_ && !rhs.impl_) return true;
    if (!impl_ || !rhs.impl_) return false;
    return *asLayerOffset(impl_) == *asLayerOffset(rhs.impl_);
}

bool SdfLayerOffset::operator!=(const SdfLayerOffset& rhs) const {
    return !(*this == rhs);
}

bool SdfLayerOffset::operator<(const SdfLayerOffset& rhs) const {
    if (!impl_) return rhs.impl_ != nullptr;
    if (!rhs.impl_) return false;
    return *asLayerOffset(impl_) < *asLayerOffset(rhs.impl_);
}

bool SdfLayerOffset::operator>(const SdfLayerOffset& rhs) const {
    return rhs < *this;
}

bool SdfLayerOffset::operator<=(const SdfLayerOffset& rhs) const {
    return !(rhs < *this);
}

bool SdfLayerOffset::operator>=(const SdfLayerOffset& rhs) const {
    return !(*this < rhs);
}

SdfLayerOffset SdfLayerOffset::operator*(const SdfLayerOffset& rhs) const {
    if (!impl_ || !rhs.impl_) return SdfLayerOffset();
    return SdfLayerOffset::FromImpl(
        new pxr::SdfLayerOffset(*asLayerOffset(impl_) * *asLayerOffset(rhs.impl_)));
}

double SdfLayerOffset::operator*(double rhs) const {
    if (!impl_) return rhs;
    return *asLayerOffset(impl_) * rhs;
}

SdfLayerOffset SdfLayerOffset::FromImpl(void* impl) {
    SdfLayerOffset offset;
    delete asLayerOffset(offset.impl_);
    offset.impl_ = impl;
    return offset;
}

// ============================================================================
// SdfPath implementation
// ============================================================================

static inline pxr::SdfPath* asPath(void* impl) {
    return static_cast<pxr::SdfPath*>(impl);
}

SdfPath::SdfPath() : impl_(new pxr::SdfPath()) {}

SdfPath::SdfPath(const std::string& path) : impl_(new pxr::SdfPath(path)) {}

SdfPath::SdfPath(const char* path) : impl_(new pxr::SdfPath(path)) {}

SdfPath::SdfPath(const SdfPath& other)
    : impl_(new pxr::SdfPath(*asPath(other.impl_))) {}

SdfPath::SdfPath(SdfPath&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

SdfPath::~SdfPath() {
    delete asPath(impl_);
}

SdfPath& SdfPath::operator=(const SdfPath& other) {
    if (this != &other) {
        delete asPath(impl_);
        impl_ = new pxr::SdfPath(*asPath(other.impl_));
    }
    return *this;
}

SdfPath& SdfPath::operator=(SdfPath&& other) noexcept {
    if (this != &other) {
        delete asPath(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

std::string SdfPath::GetString() const {
    if (!impl_) return "";
    return asPath(impl_)->GetString();
}

const char* SdfPath::GetText() const {
    if (!impl_) return "";
    return asPath(impl_)->GetText();
}

bool SdfPath::IsEmpty() const {
    return !impl_ || asPath(impl_)->IsEmpty();
}

bool SdfPath::IsAbsolutePath() const {
    return impl_ && asPath(impl_)->IsAbsolutePath();
}

bool SdfPath::IsPrimPath() const {
    return impl_ && asPath(impl_)->IsPrimPath();
}

bool SdfPath::IsPropertyPath() const {
    return impl_ && asPath(impl_)->IsPropertyPath();
}

SdfPath SdfPath::GetParentPath() const {
    if (!impl_) return SdfPath();
    return SdfPath::FromImpl(new pxr::SdfPath(asPath(impl_)->GetParentPath()));
}

tf::TfToken SdfPath::GetName() const {
    if (!impl_) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asPath(impl_)->GetName()));
}

SdfPath SdfPath::AppendChild(const tf::TfToken& childName) const {
    if (!impl_) return SdfPath();
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(childName.GetImpl());
    return SdfPath::FromImpl(new pxr::SdfPath(asPath(impl_)->AppendChild(*pxrToken)));
}

SdfPath SdfPath::AppendProperty(const tf::TfToken& propName) const {
    if (!impl_) return SdfPath();
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(propName.GetImpl());
    return SdfPath::FromImpl(new pxr::SdfPath(asPath(impl_)->AppendProperty(*pxrToken)));
}

SdfPath SdfPath::AbsoluteRootPath() {
    return SdfPath::FromImpl(new pxr::SdfPath(pxr::SdfPath::AbsoluteRootPath()));
}

SdfPath SdfPath::EmptyPath() {
    return SdfPath();
}

bool SdfPath::operator==(const SdfPath& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *asPath(impl_) == *asPath(other.impl_);
}

bool SdfPath::operator!=(const SdfPath& other) const {
    return !(*this == other);
}

bool SdfPath::operator<(const SdfPath& other) const {
    if (!impl_) return other.impl_ != nullptr;
    if (!other.impl_) return false;
    return *asPath(impl_) < *asPath(other.impl_);
}

SdfPath SdfPath::FromImpl(void* impl) {
    SdfPath path;
    delete asPath(path.impl_);
    path.impl_ = impl;
    return path;
}

// ============================================================================
// SdfAssetPath implementation
// ============================================================================

static inline pxr::SdfAssetPath* asAssetPath(void* impl) {
    return static_cast<pxr::SdfAssetPath*>(impl);
}

SdfAssetPath::SdfAssetPath() : impl_(new pxr::SdfAssetPath()) {}

SdfAssetPath::SdfAssetPath(const std::string& path)
    : impl_(new pxr::SdfAssetPath(path)) {}

SdfAssetPath::SdfAssetPath(const std::string& path, const std::string& resolvedPath)
    : impl_(new pxr::SdfAssetPath(path, resolvedPath)) {}

SdfAssetPath::SdfAssetPath(const SdfAssetPath& other)
    : impl_(new pxr::SdfAssetPath(*asAssetPath(other.impl_))) {}

SdfAssetPath::SdfAssetPath(SdfAssetPath&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

SdfAssetPath::~SdfAssetPath() {
    delete asAssetPath(impl_);
}

SdfAssetPath& SdfAssetPath::operator=(const SdfAssetPath& other) {
    if (this != &other) {
        delete asAssetPath(impl_);
        impl_ = new pxr::SdfAssetPath(*asAssetPath(other.impl_));
    }
    return *this;
}

SdfAssetPath& SdfAssetPath::operator=(SdfAssetPath&& other) noexcept {
    if (this != &other) {
        delete asAssetPath(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

std::string SdfAssetPath::GetAssetPath() const {
    if (!impl_) return "";
    return asAssetPath(impl_)->GetAssetPath();
}

std::string SdfAssetPath::GetResolvedPath() const {
    if (!impl_) return "";
    return asAssetPath(impl_)->GetResolvedPath();
}

bool SdfAssetPath::operator==(const SdfAssetPath& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *asAssetPath(impl_) == *asAssetPath(other.impl_);
}

bool SdfAssetPath::operator!=(const SdfAssetPath& other) const {
    return !(*this == other);
}

SdfAssetPath SdfAssetPath::FromImpl(void* impl) {
    SdfAssetPath path;
    delete asAssetPath(path.impl_);
    path.impl_ = impl;
    return path;
}

// ============================================================================
// SdfLayer implementation
// ============================================================================

// We store a shared_ptr internally using a wrapper struct
struct SdfLayerHandle {
    pxr::SdfLayerRefPtr ptr;
    SdfLayerHandle() = default;
    explicit SdfLayerHandle(const pxr::SdfLayerRefPtr& p) : ptr(p) {}
};

static inline SdfLayerHandle* asLayerHandle(void* impl) {
    return static_cast<SdfLayerHandle*>(impl);
}

SdfLayer::SdfLayer() : impl_(new SdfLayerHandle()) {}

SdfLayer::SdfLayer(const SdfLayer& other)
    : impl_(new SdfLayerHandle(asLayerHandle(other.impl_)->ptr)) {}

SdfLayer::SdfLayer(SdfLayer&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

SdfLayer::~SdfLayer() {
    delete asLayerHandle(impl_);
}

SdfLayer& SdfLayer::operator=(const SdfLayer& other) {
    if (this != &other) {
        delete asLayerHandle(impl_);
        impl_ = new SdfLayerHandle(asLayerHandle(other.impl_)->ptr);
    }
    return *this;
}

SdfLayer& SdfLayer::operator=(SdfLayer&& other) noexcept {
    if (this != &other) {
        delete asLayerHandle(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool SdfLayer::IsValid() const {
    return impl_ && asLayerHandle(impl_)->ptr;
}

std::string SdfLayer::GetIdentifier() const {
    if (!IsValid()) return "";
    return asLayerHandle(impl_)->ptr->GetIdentifier();
}

std::string SdfLayer::GetDisplayName() const {
    if (!IsValid()) return "";
    return asLayerHandle(impl_)->ptr->GetDisplayName();
}

bool SdfLayer::IsAnonymous() const {
    return IsValid() && asLayerHandle(impl_)->ptr->IsAnonymous();
}

bool SdfLayer::IsDirty() const {
    return IsValid() && asLayerHandle(impl_)->ptr->IsDirty();
}

bool SdfLayer::Save() const {
    if (!IsValid()) return false;
    return asLayerHandle(impl_)->ptr->Save();
}

bool SdfLayer::Export(const std::string& filename) const {
    if (!IsValid()) return false;
    return asLayerHandle(impl_)->ptr->Export(filename);
}

void SdfLayer::Clear() {
    if (IsValid()) {
        asLayerHandle(impl_)->ptr->Clear();
    }
}

SdfLayer SdfLayer::CreateAnonymous() {
    SdfLayer layer;
    asLayerHandle(layer.impl_)->ptr = pxr::SdfLayer::CreateAnonymous();
    return layer;
}

SdfLayer SdfLayer::CreateNew(const std::string& identifier) {
    SdfLayer layer;
    asLayerHandle(layer.impl_)->ptr = pxr::SdfLayer::CreateNew(identifier);
    return layer;
}

SdfLayer SdfLayer::FindOrOpen(const std::string& identifier) {
    SdfLayer layer;
    asLayerHandle(layer.impl_)->ptr = pxr::SdfLayer::FindOrOpen(identifier);
    return layer;
}

SdfLayer SdfLayer::FromImpl(void* impl) {
    SdfLayer layer;
    delete asLayerHandle(layer.impl_);
    layer.impl_ = impl;
    return layer;
}

} // namespace sdf
} // namespace cxxfacade
