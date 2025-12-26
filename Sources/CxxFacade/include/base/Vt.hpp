// Vt.hpp - Value Types facade
// CLEAN HEADER - NO pxr includes!
//
// This module provides value type arrays with copy-on-write semantics.
// Arrays are used extensively in USD for mesh data, primvars, etc.
#pragma once

#include "../CxxFacade.h"
#include "Gf.hpp"
#include "Tf.hpp"

namespace cxxfacade {
namespace vt {

/// VtIntArray - Array of integers with copy-on-write semantics
class VtIntArray {
public:
    VtIntArray();
    explicit VtIntArray(size_t size);
    VtIntArray(const VtIntArray& other);
    VtIntArray(VtIntArray&& other) noexcept;
    ~VtIntArray();

    VtIntArray& operator=(const VtIntArray& other);
    VtIntArray& operator=(VtIntArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(int value);
    void clear();

    int& operator[](size_t index);
    const int& operator[](size_t index) const;

    int* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const int* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtIntArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtFloatArray - Array of floats with copy-on-write semantics
class VtFloatArray {
public:
    VtFloatArray();
    explicit VtFloatArray(size_t size);
    VtFloatArray(const VtFloatArray& other);
    VtFloatArray(VtFloatArray&& other) noexcept;
    ~VtFloatArray();

    VtFloatArray& operator=(const VtFloatArray& other);
    VtFloatArray& operator=(VtFloatArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(float value);
    void clear();

    float& operator[](size_t index);
    const float& operator[](size_t index) const;

    float* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const float* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtFloatArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtDoubleArray - Array of doubles with copy-on-write semantics
class VtDoubleArray {
public:
    VtDoubleArray();
    explicit VtDoubleArray(size_t size);
    VtDoubleArray(const VtDoubleArray& other);
    VtDoubleArray(VtDoubleArray&& other) noexcept;
    ~VtDoubleArray();

    VtDoubleArray& operator=(const VtDoubleArray& other);
    VtDoubleArray& operator=(VtDoubleArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(double value);
    void clear();

    double& operator[](size_t index);
    const double& operator[](size_t index) const;

    double* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const double* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtDoubleArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtVec3fArray - Array of Vec3f with copy-on-write semantics
class VtVec3fArray {
public:
    VtVec3fArray();
    explicit VtVec3fArray(size_t size);
    VtVec3fArray(const VtVec3fArray& other);
    VtVec3fArray(VtVec3fArray&& other) noexcept;
    ~VtVec3fArray();

    VtVec3fArray& operator=(const VtVec3fArray& other);
    VtVec3fArray& operator=(VtVec3fArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(const gf::Vec3f& value);
    void clear();

    gf::Vec3f& operator[](size_t index);
    const gf::Vec3f& operator[](size_t index) const;

    gf::Vec3f* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const gf::Vec3f* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtVec3fArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtVec2fArray - Array of Vec2f with copy-on-write semantics
/// Used for UV coordinates and other 2D data
class VtVec2fArray {
public:
    VtVec2fArray();
    explicit VtVec2fArray(size_t size);
    VtVec2fArray(const VtVec2fArray& other);
    VtVec2fArray(VtVec2fArray&& other) noexcept;
    ~VtVec2fArray();

    VtVec2fArray& operator=(const VtVec2fArray& other);
    VtVec2fArray& operator=(VtVec2fArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(const gf::Vec2f& value);
    void clear();

    gf::Vec2f& operator[](size_t index);
    const gf::Vec2f& operator[](size_t index) const;

    gf::Vec2f* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const gf::Vec2f* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtVec2fArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtVec4fArray - Array of Vec4f with copy-on-write semantics
/// Used for RGBA colors, quaternions, etc.
class VtVec4fArray {
public:
    VtVec4fArray();
    explicit VtVec4fArray(size_t size);
    VtVec4fArray(const VtVec4fArray& other);
    VtVec4fArray(VtVec4fArray&& other) noexcept;
    ~VtVec4fArray();

    VtVec4fArray& operator=(const VtVec4fArray& other);
    VtVec4fArray& operator=(VtVec4fArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(const gf::Vec4f& value);
    void clear();

    gf::Vec4f& operator[](size_t index);
    const gf::Vec4f& operator[](size_t index) const;

    gf::Vec4f* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const gf::Vec4f* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtVec4fArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtMatrix4dArray - Array of Matrix4d with copy-on-write semantics
/// Used for skeletal animation and instancing transforms
class VtMatrix4dArray {
public:
    VtMatrix4dArray();
    explicit VtMatrix4dArray(size_t size);
    VtMatrix4dArray(const VtMatrix4dArray& other);
    VtMatrix4dArray(VtMatrix4dArray&& other) noexcept;
    ~VtMatrix4dArray();

    VtMatrix4dArray& operator=(const VtMatrix4dArray& other);
    VtMatrix4dArray& operator=(VtMatrix4dArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(const gf::Matrix4d& value);
    void clear();

    gf::Matrix4d& operator[](size_t index);
    const gf::Matrix4d& operator[](size_t index) const;

    gf::Matrix4d* data() SWIFT_RETURNS_INDEPENDENT_VALUE;
    const gf::Matrix4d* data() const SWIFT_RETURNS_INDEPENDENT_VALUE;

    void* GetImpl() const { return impl_; }
    static VtMatrix4dArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtTokenArray - Array of TfTokens with copy-on-write semantics
/// Used for string-based enumerations and primvar interpolation
class VtTokenArray {
public:
    VtTokenArray();
    explicit VtTokenArray(size_t size);
    VtTokenArray(const VtTokenArray& other);
    VtTokenArray(VtTokenArray&& other) noexcept;
    ~VtTokenArray();

    VtTokenArray& operator=(const VtTokenArray& other);
    VtTokenArray& operator=(VtTokenArray&& other) noexcept;

    size_t size() const;
    bool empty() const;
    void resize(size_t newSize);
    void reserve(size_t capacity);
    void push_back(const tf::TfToken& value);
    void clear();

    /// Get element by index (copies token since TfToken is value-type wrapper)
    tf::TfToken Get(size_t index) const;

    /// Set element at index
    void Set(size_t index, const tf::TfToken& value);

    void* GetImpl() const { return impl_; }
    static VtTokenArray FromImpl(void* impl);

private:
    void* impl_;
};

/// VtValue - Type-erased value container
class VtValue {
public:
    VtValue();
    explicit VtValue(bool val);
    explicit VtValue(int val);
    explicit VtValue(float val);
    explicit VtValue(double val);
    explicit VtValue(const std::string& val);
    VtValue(const VtValue& other);
    VtValue(VtValue&& other) noexcept;
    ~VtValue();

    VtValue& operator=(const VtValue& other);
    VtValue& operator=(VtValue&& other) noexcept;

    bool IsEmpty() const;
    bool IsHolding(const char* typeName) const;

    // Type-specific getters (throw if wrong type)
    bool GetBool() const;
    int GetInt() const;
    float GetFloat() const;
    double GetDouble() const;
    std::string GetString() const;

    // Try to cast to another type
    template<typename T>
    bool CanCast() const;

    void* GetImpl() const { return impl_; }
    static VtValue FromImpl(void* impl);

private:
    void* impl_;
};

} // namespace vt
} // namespace cxxfacade
