// Vt.cpp - Value Types implementation
#include "../../include/base/Vt.hpp"

#include <pxr/base/vt/array.h>
#include <pxr/base/vt/value.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/gf/vec4f.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/tf/token.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace vt {

// ============================================================================
// VtIntArray implementation
// ============================================================================

static inline pxr::VtIntArray* asIntArray(void* impl) {
    return static_cast<pxr::VtIntArray*>(impl);
}

VtIntArray::VtIntArray() : impl_(new pxr::VtIntArray()) {}

VtIntArray::VtIntArray(size_t size) : impl_(new pxr::VtIntArray(size)) {}

VtIntArray::VtIntArray(const VtIntArray& other)
    : impl_(new pxr::VtIntArray(*asIntArray(other.impl_))) {}

VtIntArray::VtIntArray(VtIntArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtIntArray::~VtIntArray() {
    delete asIntArray(impl_);
}

VtIntArray& VtIntArray::operator=(const VtIntArray& other) {
    if (this != &other) {
        delete asIntArray(impl_);
        impl_ = new pxr::VtIntArray(*asIntArray(other.impl_));
    }
    return *this;
}

VtIntArray& VtIntArray::operator=(VtIntArray&& other) noexcept {
    if (this != &other) {
        delete asIntArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtIntArray::size() const {
    return impl_ ? asIntArray(impl_)->size() : 0;
}

bool VtIntArray::empty() const {
    return impl_ ? asIntArray(impl_)->empty() : true;
}

void VtIntArray::resize(size_t newSize) {
    if (impl_) asIntArray(impl_)->resize(newSize);
}

void VtIntArray::reserve(size_t capacity) {
    if (impl_) asIntArray(impl_)->reserve(capacity);
}

void VtIntArray::push_back(int value) {
    if (impl_) asIntArray(impl_)->push_back(value);
}

void VtIntArray::clear() {
    if (impl_) asIntArray(impl_)->clear();
}

int& VtIntArray::operator[](size_t index) {
    return (*asIntArray(impl_))[index];
}

const int& VtIntArray::operator[](size_t index) const {
    return (*asIntArray(impl_))[index];
}

int* VtIntArray::data() {
    return impl_ ? asIntArray(impl_)->data() : nullptr;
}

const int* VtIntArray::data() const {
    return impl_ ? asIntArray(impl_)->cdata() : nullptr;
}

VtIntArray VtIntArray::FromImpl(void* impl) {
    VtIntArray arr;
    delete asIntArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtFloatArray implementation
// ============================================================================

static inline pxr::VtFloatArray* asFloatArray(void* impl) {
    return static_cast<pxr::VtFloatArray*>(impl);
}

VtFloatArray::VtFloatArray() : impl_(new pxr::VtFloatArray()) {}

VtFloatArray::VtFloatArray(size_t size) : impl_(new pxr::VtFloatArray(size)) {}

VtFloatArray::VtFloatArray(const VtFloatArray& other)
    : impl_(new pxr::VtFloatArray(*asFloatArray(other.impl_))) {}

VtFloatArray::VtFloatArray(VtFloatArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtFloatArray::~VtFloatArray() {
    delete asFloatArray(impl_);
}

VtFloatArray& VtFloatArray::operator=(const VtFloatArray& other) {
    if (this != &other) {
        delete asFloatArray(impl_);
        impl_ = new pxr::VtFloatArray(*asFloatArray(other.impl_));
    }
    return *this;
}

VtFloatArray& VtFloatArray::operator=(VtFloatArray&& other) noexcept {
    if (this != &other) {
        delete asFloatArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtFloatArray::size() const {
    return impl_ ? asFloatArray(impl_)->size() : 0;
}

bool VtFloatArray::empty() const {
    return impl_ ? asFloatArray(impl_)->empty() : true;
}

void VtFloatArray::resize(size_t newSize) {
    if (impl_) asFloatArray(impl_)->resize(newSize);
}

void VtFloatArray::reserve(size_t capacity) {
    if (impl_) asFloatArray(impl_)->reserve(capacity);
}

void VtFloatArray::push_back(float value) {
    if (impl_) asFloatArray(impl_)->push_back(value);
}

void VtFloatArray::clear() {
    if (impl_) asFloatArray(impl_)->clear();
}

float& VtFloatArray::operator[](size_t index) {
    return (*asFloatArray(impl_))[index];
}

const float& VtFloatArray::operator[](size_t index) const {
    return (*asFloatArray(impl_))[index];
}

float* VtFloatArray::data() {
    return impl_ ? asFloatArray(impl_)->data() : nullptr;
}

const float* VtFloatArray::data() const {
    return impl_ ? asFloatArray(impl_)->cdata() : nullptr;
}

VtFloatArray VtFloatArray::FromImpl(void* impl) {
    VtFloatArray arr;
    delete asFloatArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtDoubleArray implementation
// ============================================================================

static inline pxr::VtDoubleArray* asDoubleArray(void* impl) {
    return static_cast<pxr::VtDoubleArray*>(impl);
}

VtDoubleArray::VtDoubleArray() : impl_(new pxr::VtDoubleArray()) {}

VtDoubleArray::VtDoubleArray(size_t size) : impl_(new pxr::VtDoubleArray(size)) {}

VtDoubleArray::VtDoubleArray(const VtDoubleArray& other)
    : impl_(new pxr::VtDoubleArray(*asDoubleArray(other.impl_))) {}

VtDoubleArray::VtDoubleArray(VtDoubleArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtDoubleArray::~VtDoubleArray() {
    delete asDoubleArray(impl_);
}

VtDoubleArray& VtDoubleArray::operator=(const VtDoubleArray& other) {
    if (this != &other) {
        delete asDoubleArray(impl_);
        impl_ = new pxr::VtDoubleArray(*asDoubleArray(other.impl_));
    }
    return *this;
}

VtDoubleArray& VtDoubleArray::operator=(VtDoubleArray&& other) noexcept {
    if (this != &other) {
        delete asDoubleArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtDoubleArray::size() const {
    return impl_ ? asDoubleArray(impl_)->size() : 0;
}

bool VtDoubleArray::empty() const {
    return impl_ ? asDoubleArray(impl_)->empty() : true;
}

void VtDoubleArray::resize(size_t newSize) {
    if (impl_) asDoubleArray(impl_)->resize(newSize);
}

void VtDoubleArray::reserve(size_t capacity) {
    if (impl_) asDoubleArray(impl_)->reserve(capacity);
}

void VtDoubleArray::push_back(double value) {
    if (impl_) asDoubleArray(impl_)->push_back(value);
}

void VtDoubleArray::clear() {
    if (impl_) asDoubleArray(impl_)->clear();
}

double& VtDoubleArray::operator[](size_t index) {
    return (*asDoubleArray(impl_))[index];
}

const double& VtDoubleArray::operator[](size_t index) const {
    return (*asDoubleArray(impl_))[index];
}

double* VtDoubleArray::data() {
    return impl_ ? asDoubleArray(impl_)->data() : nullptr;
}

const double* VtDoubleArray::data() const {
    return impl_ ? asDoubleArray(impl_)->cdata() : nullptr;
}

VtDoubleArray VtDoubleArray::FromImpl(void* impl) {
    VtDoubleArray arr;
    delete asDoubleArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtVec3fArray implementation
// ============================================================================

static inline pxr::VtVec3fArray* asVec3fArray(void* impl) {
    return static_cast<pxr::VtVec3fArray*>(impl);
}

VtVec3fArray::VtVec3fArray() : impl_(new pxr::VtVec3fArray()) {}

VtVec3fArray::VtVec3fArray(size_t size) : impl_(new pxr::VtVec3fArray(size)) {}

VtVec3fArray::VtVec3fArray(const VtVec3fArray& other)
    : impl_(new pxr::VtVec3fArray(*asVec3fArray(other.impl_))) {}

VtVec3fArray::VtVec3fArray(VtVec3fArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtVec3fArray::~VtVec3fArray() {
    delete asVec3fArray(impl_);
}

VtVec3fArray& VtVec3fArray::operator=(const VtVec3fArray& other) {
    if (this != &other) {
        delete asVec3fArray(impl_);
        impl_ = new pxr::VtVec3fArray(*asVec3fArray(other.impl_));
    }
    return *this;
}

VtVec3fArray& VtVec3fArray::operator=(VtVec3fArray&& other) noexcept {
    if (this != &other) {
        delete asVec3fArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtVec3fArray::size() const {
    return impl_ ? asVec3fArray(impl_)->size() : 0;
}

bool VtVec3fArray::empty() const {
    return impl_ ? asVec3fArray(impl_)->empty() : true;
}

void VtVec3fArray::resize(size_t newSize) {
    if (impl_) asVec3fArray(impl_)->resize(newSize);
}

void VtVec3fArray::reserve(size_t capacity) {
    if (impl_) asVec3fArray(impl_)->reserve(capacity);
}

void VtVec3fArray::push_back(const gf::Vec3f& value) {
    if (impl_) {
        asVec3fArray(impl_)->push_back(pxr::GfVec3f(value.x, value.y, value.z));
    }
}

void VtVec3fArray::clear() {
    if (impl_) asVec3fArray(impl_)->clear();
}

gf::Vec3f& VtVec3fArray::operator[](size_t index) {
    pxr::GfVec3f& pxrVec = (*asVec3fArray(impl_))[index];
    // Reinterpret cast is safe because gf::Vec3f has same layout as GfVec3f
    return reinterpret_cast<gf::Vec3f&>(pxrVec);
}

const gf::Vec3f& VtVec3fArray::operator[](size_t index) const {
    const pxr::GfVec3f& pxrVec = (*asVec3fArray(impl_))[index];
    return reinterpret_cast<const gf::Vec3f&>(pxrVec);
}

gf::Vec3f* VtVec3fArray::data() {
    if (!impl_) return nullptr;
    return reinterpret_cast<gf::Vec3f*>(asVec3fArray(impl_)->data());
}

const gf::Vec3f* VtVec3fArray::data() const {
    if (!impl_) return nullptr;
    return reinterpret_cast<const gf::Vec3f*>(asVec3fArray(impl_)->cdata());
}

VtVec3fArray VtVec3fArray::FromImpl(void* impl) {
    VtVec3fArray arr;
    delete asVec3fArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtVec2fArray implementation
// ============================================================================

static inline pxr::VtVec2fArray* asVec2fArray(void* impl) {
    return static_cast<pxr::VtVec2fArray*>(impl);
}

VtVec2fArray::VtVec2fArray() : impl_(new pxr::VtVec2fArray()) {}

VtVec2fArray::VtVec2fArray(size_t size) : impl_(new pxr::VtVec2fArray(size)) {}

VtVec2fArray::VtVec2fArray(const VtVec2fArray& other)
    : impl_(new pxr::VtVec2fArray(*asVec2fArray(other.impl_))) {}

VtVec2fArray::VtVec2fArray(VtVec2fArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtVec2fArray::~VtVec2fArray() {
    delete asVec2fArray(impl_);
}

VtVec2fArray& VtVec2fArray::operator=(const VtVec2fArray& other) {
    if (this != &other) {
        delete asVec2fArray(impl_);
        impl_ = new pxr::VtVec2fArray(*asVec2fArray(other.impl_));
    }
    return *this;
}

VtVec2fArray& VtVec2fArray::operator=(VtVec2fArray&& other) noexcept {
    if (this != &other) {
        delete asVec2fArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtVec2fArray::size() const {
    return impl_ ? asVec2fArray(impl_)->size() : 0;
}

bool VtVec2fArray::empty() const {
    return impl_ ? asVec2fArray(impl_)->empty() : true;
}

void VtVec2fArray::resize(size_t newSize) {
    if (impl_) asVec2fArray(impl_)->resize(newSize);
}

void VtVec2fArray::reserve(size_t capacity) {
    if (impl_) asVec2fArray(impl_)->reserve(capacity);
}

void VtVec2fArray::push_back(const gf::Vec2f& value) {
    if (impl_) {
        asVec2fArray(impl_)->push_back(pxr::GfVec2f(value.x, value.y));
    }
}

void VtVec2fArray::clear() {
    if (impl_) asVec2fArray(impl_)->clear();
}

gf::Vec2f& VtVec2fArray::operator[](size_t index) {
    pxr::GfVec2f& pxrVec = (*asVec2fArray(impl_))[index];
    return reinterpret_cast<gf::Vec2f&>(pxrVec);
}

const gf::Vec2f& VtVec2fArray::operator[](size_t index) const {
    const pxr::GfVec2f& pxrVec = (*asVec2fArray(impl_))[index];
    return reinterpret_cast<const gf::Vec2f&>(pxrVec);
}

gf::Vec2f* VtVec2fArray::data() {
    if (!impl_) return nullptr;
    return reinterpret_cast<gf::Vec2f*>(asVec2fArray(impl_)->data());
}

const gf::Vec2f* VtVec2fArray::data() const {
    if (!impl_) return nullptr;
    return reinterpret_cast<const gf::Vec2f*>(asVec2fArray(impl_)->cdata());
}

VtVec2fArray VtVec2fArray::FromImpl(void* impl) {
    VtVec2fArray arr;
    delete asVec2fArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtVec4fArray implementation
// ============================================================================

static inline pxr::VtVec4fArray* asVec4fArray(void* impl) {
    return static_cast<pxr::VtVec4fArray*>(impl);
}

VtVec4fArray::VtVec4fArray() : impl_(new pxr::VtVec4fArray()) {}

VtVec4fArray::VtVec4fArray(size_t size) : impl_(new pxr::VtVec4fArray(size)) {}

VtVec4fArray::VtVec4fArray(const VtVec4fArray& other)
    : impl_(new pxr::VtVec4fArray(*asVec4fArray(other.impl_))) {}

VtVec4fArray::VtVec4fArray(VtVec4fArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtVec4fArray::~VtVec4fArray() {
    delete asVec4fArray(impl_);
}

VtVec4fArray& VtVec4fArray::operator=(const VtVec4fArray& other) {
    if (this != &other) {
        delete asVec4fArray(impl_);
        impl_ = new pxr::VtVec4fArray(*asVec4fArray(other.impl_));
    }
    return *this;
}

VtVec4fArray& VtVec4fArray::operator=(VtVec4fArray&& other) noexcept {
    if (this != &other) {
        delete asVec4fArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtVec4fArray::size() const {
    return impl_ ? asVec4fArray(impl_)->size() : 0;
}

bool VtVec4fArray::empty() const {
    return impl_ ? asVec4fArray(impl_)->empty() : true;
}

void VtVec4fArray::resize(size_t newSize) {
    if (impl_) asVec4fArray(impl_)->resize(newSize);
}

void VtVec4fArray::reserve(size_t capacity) {
    if (impl_) asVec4fArray(impl_)->reserve(capacity);
}

void VtVec4fArray::push_back(const gf::Vec4f& value) {
    if (impl_) {
        asVec4fArray(impl_)->push_back(pxr::GfVec4f(value.x, value.y, value.z, value.w));
    }
}

void VtVec4fArray::clear() {
    if (impl_) asVec4fArray(impl_)->clear();
}

gf::Vec4f& VtVec4fArray::operator[](size_t index) {
    pxr::GfVec4f& pxrVec = (*asVec4fArray(impl_))[index];
    return reinterpret_cast<gf::Vec4f&>(pxrVec);
}

const gf::Vec4f& VtVec4fArray::operator[](size_t index) const {
    const pxr::GfVec4f& pxrVec = (*asVec4fArray(impl_))[index];
    return reinterpret_cast<const gf::Vec4f&>(pxrVec);
}

gf::Vec4f* VtVec4fArray::data() {
    if (!impl_) return nullptr;
    return reinterpret_cast<gf::Vec4f*>(asVec4fArray(impl_)->data());
}

const gf::Vec4f* VtVec4fArray::data() const {
    if (!impl_) return nullptr;
    return reinterpret_cast<const gf::Vec4f*>(asVec4fArray(impl_)->cdata());
}

VtVec4fArray VtVec4fArray::FromImpl(void* impl) {
    VtVec4fArray arr;
    delete asVec4fArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtMatrix4dArray implementation
// ============================================================================

static inline pxr::VtMatrix4dArray* asMatrix4dArray(void* impl) {
    return static_cast<pxr::VtMatrix4dArray*>(impl);
}

VtMatrix4dArray::VtMatrix4dArray() : impl_(new pxr::VtMatrix4dArray()) {}

VtMatrix4dArray::VtMatrix4dArray(size_t size) : impl_(new pxr::VtMatrix4dArray(size)) {}

VtMatrix4dArray::VtMatrix4dArray(const VtMatrix4dArray& other)
    : impl_(new pxr::VtMatrix4dArray(*asMatrix4dArray(other.impl_))) {}

VtMatrix4dArray::VtMatrix4dArray(VtMatrix4dArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtMatrix4dArray::~VtMatrix4dArray() {
    delete asMatrix4dArray(impl_);
}

VtMatrix4dArray& VtMatrix4dArray::operator=(const VtMatrix4dArray& other) {
    if (this != &other) {
        delete asMatrix4dArray(impl_);
        impl_ = new pxr::VtMatrix4dArray(*asMatrix4dArray(other.impl_));
    }
    return *this;
}

VtMatrix4dArray& VtMatrix4dArray::operator=(VtMatrix4dArray&& other) noexcept {
    if (this != &other) {
        delete asMatrix4dArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtMatrix4dArray::size() const {
    return impl_ ? asMatrix4dArray(impl_)->size() : 0;
}

bool VtMatrix4dArray::empty() const {
    return impl_ ? asMatrix4dArray(impl_)->empty() : true;
}

void VtMatrix4dArray::resize(size_t newSize) {
    if (impl_) asMatrix4dArray(impl_)->resize(newSize);
}

void VtMatrix4dArray::reserve(size_t capacity) {
    if (impl_) asMatrix4dArray(impl_)->reserve(capacity);
}

void VtMatrix4dArray::push_back(const gf::Matrix4d& value) {
    if (impl_) {
        pxr::GfMatrix4d pxrMat;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                pxrMat[i][j] = value.data[i][j];
            }
        }
        asMatrix4dArray(impl_)->push_back(pxrMat);
    }
}

void VtMatrix4dArray::clear() {
    if (impl_) asMatrix4dArray(impl_)->clear();
}

gf::Matrix4d& VtMatrix4dArray::operator[](size_t index) {
    pxr::GfMatrix4d& pxrMat = (*asMatrix4dArray(impl_))[index];
    // Safe reinterpret_cast: gf::Matrix4d has same memory layout (double[4][4])
    return reinterpret_cast<gf::Matrix4d&>(pxrMat);
}

const gf::Matrix4d& VtMatrix4dArray::operator[](size_t index) const {
    const pxr::GfMatrix4d& pxrMat = (*asMatrix4dArray(impl_))[index];
    return reinterpret_cast<const gf::Matrix4d&>(pxrMat);
}

gf::Matrix4d* VtMatrix4dArray::data() {
    if (!impl_) return nullptr;
    return reinterpret_cast<gf::Matrix4d*>(asMatrix4dArray(impl_)->data());
}

const gf::Matrix4d* VtMatrix4dArray::data() const {
    if (!impl_) return nullptr;
    return reinterpret_cast<const gf::Matrix4d*>(asMatrix4dArray(impl_)->cdata());
}

VtMatrix4dArray VtMatrix4dArray::FromImpl(void* impl) {
    VtMatrix4dArray arr;
    delete asMatrix4dArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtTokenArray implementation
// ============================================================================

static inline pxr::VtTokenArray* asTokenArray(void* impl) {
    return static_cast<pxr::VtTokenArray*>(impl);
}

VtTokenArray::VtTokenArray() : impl_(new pxr::VtTokenArray()) {}

VtTokenArray::VtTokenArray(size_t size) : impl_(new pxr::VtTokenArray(size)) {}

VtTokenArray::VtTokenArray(const VtTokenArray& other)
    : impl_(new pxr::VtTokenArray(*asTokenArray(other.impl_))) {}

VtTokenArray::VtTokenArray(VtTokenArray&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtTokenArray::~VtTokenArray() {
    delete asTokenArray(impl_);
}

VtTokenArray& VtTokenArray::operator=(const VtTokenArray& other) {
    if (this != &other) {
        delete asTokenArray(impl_);
        impl_ = new pxr::VtTokenArray(*asTokenArray(other.impl_));
    }
    return *this;
}

VtTokenArray& VtTokenArray::operator=(VtTokenArray&& other) noexcept {
    if (this != &other) {
        delete asTokenArray(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

size_t VtTokenArray::size() const {
    return impl_ ? asTokenArray(impl_)->size() : 0;
}

bool VtTokenArray::empty() const {
    return impl_ ? asTokenArray(impl_)->empty() : true;
}

void VtTokenArray::resize(size_t newSize) {
    if (impl_) asTokenArray(impl_)->resize(newSize);
}

void VtTokenArray::reserve(size_t capacity) {
    if (impl_) asTokenArray(impl_)->reserve(capacity);
}

void VtTokenArray::push_back(const tf::TfToken& value) {
    if (impl_) {
        pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(value.GetImpl());
        asTokenArray(impl_)->push_back(*pxrToken);
    }
}

void VtTokenArray::clear() {
    if (impl_) asTokenArray(impl_)->clear();
}

tf::TfToken VtTokenArray::Get(size_t index) const {
    if (!impl_) return tf::TfToken();
    const pxr::TfToken& pxrToken = (*asTokenArray(impl_))[index];
    return tf::TfToken(pxrToken.GetString());
}

void VtTokenArray::Set(size_t index, const tf::TfToken& value) {
    if (impl_) {
        pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(value.GetImpl());
        (*asTokenArray(impl_))[index] = *pxrToken;
    }
}

VtTokenArray VtTokenArray::FromImpl(void* impl) {
    VtTokenArray arr;
    delete asTokenArray(arr.impl_);
    arr.impl_ = impl;
    return arr;
}

// ============================================================================
// VtValue implementation
// ============================================================================

static inline pxr::VtValue* asValue(void* impl) {
    return static_cast<pxr::VtValue*>(impl);
}

VtValue::VtValue() : impl_(new pxr::VtValue()) {}

VtValue::VtValue(bool val) : impl_(new pxr::VtValue(val)) {}

VtValue::VtValue(int val) : impl_(new pxr::VtValue(val)) {}

VtValue::VtValue(float val) : impl_(new pxr::VtValue(val)) {}

VtValue::VtValue(double val) : impl_(new pxr::VtValue(val)) {}

VtValue::VtValue(const std::string& val) : impl_(new pxr::VtValue(val)) {}

VtValue::VtValue(const VtValue& other)
    : impl_(new pxr::VtValue(*asValue(other.impl_))) {}

VtValue::VtValue(VtValue&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

VtValue::~VtValue() {
    delete asValue(impl_);
}

VtValue& VtValue::operator=(const VtValue& other) {
    if (this != &other) {
        delete asValue(impl_);
        impl_ = new pxr::VtValue(*asValue(other.impl_));
    }
    return *this;
}

VtValue& VtValue::operator=(VtValue&& other) noexcept {
    if (this != &other) {
        delete asValue(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool VtValue::IsEmpty() const {
    return !impl_ || asValue(impl_)->IsEmpty();
}

bool VtValue::IsHolding(const char* typeName) const {
    if (!impl_) return false;
    return asValue(impl_)->GetTypeName() == typeName;
}

bool VtValue::GetBool() const {
    return asValue(impl_)->Get<bool>();
}

int VtValue::GetInt() const {
    return asValue(impl_)->Get<int>();
}

float VtValue::GetFloat() const {
    return asValue(impl_)->Get<float>();
}

double VtValue::GetDouble() const {
    return asValue(impl_)->Get<double>();
}

std::string VtValue::GetString() const {
    return asValue(impl_)->Get<std::string>();
}

VtValue VtValue::FromImpl(void* impl) {
    VtValue val;
    delete asValue(val.impl_);
    val.impl_ = impl;
    return val;
}

} // namespace vt
} // namespace cxxfacade
