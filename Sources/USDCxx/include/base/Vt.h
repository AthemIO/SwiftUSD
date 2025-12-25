// Vt.h - Value Types for SwiftUSD
// Mirrors: pxr/base/vt/array.h
// C++ header with Swift interop annotations for VtArray types

#pragma once

#include "../Swift.h"
#include "Gf.h"

#include <atomic>
#include <cstring>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/base/vt/array.h"
#endif

namespace swiftusd {

// Forward declarations for retain/release functions
class IntArray;
class FloatArray;
class DoubleArray;
class Vec3fArray;
class Vec3dArray;
class Matrix4dArray;

// ============================================================================
// MARK: - IntArray
// ============================================================================

/// A dynamic array of 32-bit integers.
/// Provides a Swift-friendly wrapper around VtArray<int>.
struct IntArray {
    std::vector<int32_t> _data;

    // Constructors
    IntArray() SWIFTUSD_NOEXCEPT : _data() {}

    explicit IntArray(size_t count) SWIFTUSD_NOEXCEPT : _data(count, 0) {}

    IntArray(const int32_t* data, size_t count) SWIFTUSD_NOEXCEPT
        : _data(data, data + count) {}

#if defined(USE_PIXAR_USD)
    IntArray(const PXR_NS::VtArray<int>& array) SWIFTUSD_NOEXCEPT
        : _data(array.cdata(), array.cdata() + array.size()) {}

    PXR_NS::VtArray<int> ToVt() const SWIFTUSD_NOEXCEPT {
        PXR_NS::VtArray<int> result(_data.size());
        std::memcpy(result.data(), _data.data(), _data.size() * sizeof(int));
        return result;
    }
#endif

    // Factory methods
    static IntArray* Create() SWIFTUSD_NOEXCEPT {
        return new IntArray();
    }

    static IntArray* Create(size_t count) SWIFTUSD_NOEXCEPT {
        return new IntArray(count);
    }

    static IntArray* Create(const int32_t* data, size_t count) SWIFTUSD_NOEXCEPT {
        return new IntArray(data, count);
    }

    // Size/capacity
    size_t size() const SWIFTUSD_NOEXCEPT { return _data.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _data.empty(); }
    size_t capacity() const SWIFTUSD_NOEXCEPT { return _data.capacity(); }

    void reserve(size_t newCapacity) SWIFTUSD_NOEXCEPT { _data.reserve(newCapacity); }
    void resize(size_t newSize) SWIFTUSD_NOEXCEPT { _data.resize(newSize); }
    void clear() SWIFTUSD_NOEXCEPT { _data.clear(); }

    // Element access
    int32_t GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return index < _data.size() ? _data[index] : 0;
    }

    void SetAt(size_t index, int32_t value) SWIFTUSD_NOEXCEPT {
        if (index < _data.size()) {
            _data[index] = value;
        }
    }

    // Zero-copy data access
    const int32_t* data() const SWIFTUSD_NOEXCEPT { return _data.data(); }
    int32_t* mutableData() SWIFTUSD_NOEXCEPT { return _data.data(); }

    // Modification
    void append(int32_t value) SWIFTUSD_NOEXCEPT { _data.push_back(value); }
    void popBack() SWIFTUSD_NOEXCEPT { if (!_data.empty()) _data.pop_back(); }

    void assign(const int32_t* data, size_t count) SWIFTUSD_NOEXCEPT {
        _data.assign(data, data + count);
    }

    // Copy
    IntArray* copy() const SWIFTUSD_NOEXCEPT {
        IntArray* result = new IntArray();
        result->_data = _data;
        return result;
    }

    // Comparison
    bool equals(const IntArray* other) const SWIFTUSD_NOEXCEPT {
        return other && _data == other->_data;
    }

    size_t hash() const SWIFTUSD_NOEXCEPT {
        size_t h = 0;
        for (const auto& v : _data) {
            h ^= std::hash<int32_t>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - FloatArray
// ============================================================================

/// A dynamic array of 32-bit floats.
/// Provides a Swift-friendly wrapper around VtArray<float>.
struct FloatArray {
    std::vector<float> _data;

    // Constructors
    FloatArray() SWIFTUSD_NOEXCEPT : _data() {}

    explicit FloatArray(size_t count) SWIFTUSD_NOEXCEPT : _data(count, 0.0f) {}

    FloatArray(const float* data, size_t count) SWIFTUSD_NOEXCEPT
        : _data(data, data + count) {}

#if defined(USE_PIXAR_USD)
    FloatArray(const PXR_NS::VtArray<float>& array) SWIFTUSD_NOEXCEPT
        : _data(array.cdata(), array.cdata() + array.size()) {}

    PXR_NS::VtArray<float> ToVt() const SWIFTUSD_NOEXCEPT {
        PXR_NS::VtArray<float> result(_data.size());
        std::memcpy(result.data(), _data.data(), _data.size() * sizeof(float));
        return result;
    }
#endif

    // Factory methods
    static FloatArray* Create() SWIFTUSD_NOEXCEPT {
        return new FloatArray();
    }

    static FloatArray* Create(size_t count) SWIFTUSD_NOEXCEPT {
        return new FloatArray(count);
    }

    static FloatArray* Create(const float* data, size_t count) SWIFTUSD_NOEXCEPT {
        return new FloatArray(data, count);
    }

    // Size/capacity
    size_t size() const SWIFTUSD_NOEXCEPT { return _data.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _data.empty(); }
    size_t capacity() const SWIFTUSD_NOEXCEPT { return _data.capacity(); }

    void reserve(size_t newCapacity) SWIFTUSD_NOEXCEPT { _data.reserve(newCapacity); }
    void resize(size_t newSize) SWIFTUSD_NOEXCEPT { _data.resize(newSize); }
    void clear() SWIFTUSD_NOEXCEPT { _data.clear(); }

    // Element access
    float GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return index < _data.size() ? _data[index] : 0.0f;
    }

    void SetAt(size_t index, float value) SWIFTUSD_NOEXCEPT {
        if (index < _data.size()) {
            _data[index] = value;
        }
    }

    // Zero-copy data access
    const float* data() const SWIFTUSD_NOEXCEPT { return _data.data(); }
    float* mutableData() SWIFTUSD_NOEXCEPT { return _data.data(); }

    // Modification
    void append(float value) SWIFTUSD_NOEXCEPT { _data.push_back(value); }
    void popBack() SWIFTUSD_NOEXCEPT { if (!_data.empty()) _data.pop_back(); }

    void assign(const float* data, size_t count) SWIFTUSD_NOEXCEPT {
        _data.assign(data, data + count);
    }

    // Copy
    FloatArray* copy() const SWIFTUSD_NOEXCEPT {
        FloatArray* result = new FloatArray();
        result->_data = _data;
        return result;
    }

    // Comparison
    bool equals(const FloatArray* other) const SWIFTUSD_NOEXCEPT {
        return other && _data == other->_data;
    }

    size_t hash() const SWIFTUSD_NOEXCEPT {
        size_t h = 0;
        for (const auto& v : _data) {
            h ^= std::hash<float>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - DoubleArray
// ============================================================================

/// A dynamic array of 64-bit doubles.
/// Provides a Swift-friendly wrapper around VtArray<double>.
struct DoubleArray {
    std::vector<double> _data;

    // Constructors
    DoubleArray() SWIFTUSD_NOEXCEPT : _data() {}

    explicit DoubleArray(size_t count) SWIFTUSD_NOEXCEPT : _data(count, 0.0) {}

    DoubleArray(const double* data, size_t count) SWIFTUSD_NOEXCEPT
        : _data(data, data + count) {}

#if defined(USE_PIXAR_USD)
    DoubleArray(const PXR_NS::VtArray<double>& array) SWIFTUSD_NOEXCEPT
        : _data(array.cdata(), array.cdata() + array.size()) {}

    PXR_NS::VtArray<double> ToVt() const SWIFTUSD_NOEXCEPT {
        PXR_NS::VtArray<double> result(_data.size());
        std::memcpy(result.data(), _data.data(), _data.size() * sizeof(double));
        return result;
    }
#endif

    // Factory methods
    static DoubleArray* Create() SWIFTUSD_NOEXCEPT {
        return new DoubleArray();
    }

    static DoubleArray* Create(size_t count) SWIFTUSD_NOEXCEPT {
        return new DoubleArray(count);
    }

    static DoubleArray* Create(const double* data, size_t count) SWIFTUSD_NOEXCEPT {
        return new DoubleArray(data, count);
    }

    // Size/capacity
    size_t size() const SWIFTUSD_NOEXCEPT { return _data.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _data.empty(); }
    size_t capacity() const SWIFTUSD_NOEXCEPT { return _data.capacity(); }

    void reserve(size_t newCapacity) SWIFTUSD_NOEXCEPT { _data.reserve(newCapacity); }
    void resize(size_t newSize) SWIFTUSD_NOEXCEPT { _data.resize(newSize); }
    void clear() SWIFTUSD_NOEXCEPT { _data.clear(); }

    // Element access
    double GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return index < _data.size() ? _data[index] : 0.0;
    }

    void SetAt(size_t index, double value) SWIFTUSD_NOEXCEPT {
        if (index < _data.size()) {
            _data[index] = value;
        }
    }

    // Zero-copy data access
    const double* data() const SWIFTUSD_NOEXCEPT { return _data.data(); }
    double* mutableData() SWIFTUSD_NOEXCEPT { return _data.data(); }

    // Modification
    void append(double value) SWIFTUSD_NOEXCEPT { _data.push_back(value); }
    void popBack() SWIFTUSD_NOEXCEPT { if (!_data.empty()) _data.pop_back(); }

    void assign(const double* data, size_t count) SWIFTUSD_NOEXCEPT {
        _data.assign(data, data + count);
    }

    // Copy
    DoubleArray* copy() const SWIFTUSD_NOEXCEPT {
        DoubleArray* result = new DoubleArray();
        result->_data = _data;
        return result;
    }

    // Comparison
    bool equals(const DoubleArray* other) const SWIFTUSD_NOEXCEPT {
        return other && _data == other->_data;
    }

    size_t hash() const SWIFTUSD_NOEXCEPT {
        size_t h = 0;
        for (const auto& v : _data) {
            h ^= std::hash<double>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Vec3fArray
// ============================================================================

/// A dynamic array of Vec3f.
/// Provides a Swift-friendly wrapper around VtArray<GfVec3f>.
struct Vec3fArray {
    std::vector<Vec3f> _data;

    // Constructors
    Vec3fArray() SWIFTUSD_NOEXCEPT : _data() {}

    explicit Vec3fArray(size_t count) SWIFTUSD_NOEXCEPT : _data(count) {}

    Vec3fArray(const Vec3f* data, size_t count) SWIFTUSD_NOEXCEPT
        : _data(data, data + count) {}

#if defined(USE_PIXAR_USD)
    Vec3fArray(const PXR_NS::VtArray<PXR_NS::GfVec3f>& array) SWIFTUSD_NOEXCEPT
        : _data(array.size()) {
        for (size_t i = 0; i < array.size(); ++i) {
            _data[i] = Vec3f(array[i]);
        }
    }

    PXR_NS::VtArray<PXR_NS::GfVec3f> ToVt() const SWIFTUSD_NOEXCEPT {
        PXR_NS::VtArray<PXR_NS::GfVec3f> result(_data.size());
        for (size_t i = 0; i < _data.size(); ++i) {
            result[i] = _data[i].ToGf();
        }
        return result;
    }
#endif

    // Factory methods
    static Vec3fArray* Create() SWIFTUSD_NOEXCEPT {
        return new Vec3fArray();
    }

    static Vec3fArray* Create(size_t count) SWIFTUSD_NOEXCEPT {
        return new Vec3fArray(count);
    }

    static Vec3fArray* Create(const Vec3f* data, size_t count) SWIFTUSD_NOEXCEPT {
        return new Vec3fArray(data, count);
    }

    // Size/capacity
    size_t size() const SWIFTUSD_NOEXCEPT { return _data.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _data.empty(); }
    size_t capacity() const SWIFTUSD_NOEXCEPT { return _data.capacity(); }

    void reserve(size_t newCapacity) SWIFTUSD_NOEXCEPT { _data.reserve(newCapacity); }
    void resize(size_t newSize) SWIFTUSD_NOEXCEPT { _data.resize(newSize); }
    void clear() SWIFTUSD_NOEXCEPT { _data.clear(); }

    // Element access
    Vec3f GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return index < _data.size() ? _data[index] : Vec3f();
    }

    void SetAt(size_t index, Vec3f value) SWIFTUSD_NOEXCEPT {
        if (index < _data.size()) {
            _data[index] = value;
        }
    }

    // Zero-copy data access
    const Vec3f* data() const SWIFTUSD_NOEXCEPT { return _data.data(); }
    Vec3f* mutableData() SWIFTUSD_NOEXCEPT { return _data.data(); }

    // Modification
    void append(Vec3f value) SWIFTUSD_NOEXCEPT { _data.push_back(value); }
    void popBack() SWIFTUSD_NOEXCEPT { if (!_data.empty()) _data.pop_back(); }

    void assign(const Vec3f* data, size_t count) SWIFTUSD_NOEXCEPT {
        _data.assign(data, data + count);
    }

    // Copy
    Vec3fArray* copy() const SWIFTUSD_NOEXCEPT {
        Vec3fArray* result = new Vec3fArray();
        result->_data = _data;
        return result;
    }

    // Comparison
    bool equals(const Vec3fArray* other) const SWIFTUSD_NOEXCEPT {
        if (!other || _data.size() != other->_data.size()) return false;
        for (size_t i = 0; i < _data.size(); ++i) {
            if (_data[i] != other->_data[i]) return false;
        }
        return true;
    }

    size_t hash() const SWIFTUSD_NOEXCEPT {
        size_t h = 0;
        for (const auto& v : _data) {
            h ^= std::hash<float>{}(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<float>{}(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<float>{}(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Vec3dArray
// ============================================================================

/// A dynamic array of Vec3d.
/// Provides a Swift-friendly wrapper around VtArray<GfVec3d>.
struct Vec3dArray {
    std::vector<Vec3d> _data;

    // Constructors
    Vec3dArray() SWIFTUSD_NOEXCEPT : _data() {}

    explicit Vec3dArray(size_t count) SWIFTUSD_NOEXCEPT : _data(count) {}

    Vec3dArray(const Vec3d* data, size_t count) SWIFTUSD_NOEXCEPT
        : _data(data, data + count) {}

#if defined(USE_PIXAR_USD)
    Vec3dArray(const PXR_NS::VtArray<PXR_NS::GfVec3d>& array) SWIFTUSD_NOEXCEPT
        : _data(array.size()) {
        for (size_t i = 0; i < array.size(); ++i) {
            _data[i] = Vec3d(array[i]);
        }
    }

    PXR_NS::VtArray<PXR_NS::GfVec3d> ToVt() const SWIFTUSD_NOEXCEPT {
        PXR_NS::VtArray<PXR_NS::GfVec3d> result(_data.size());
        for (size_t i = 0; i < _data.size(); ++i) {
            result[i] = _data[i].ToGf();
        }
        return result;
    }
#endif

    // Factory methods
    static Vec3dArray* Create() SWIFTUSD_NOEXCEPT {
        return new Vec3dArray();
    }

    static Vec3dArray* Create(size_t count) SWIFTUSD_NOEXCEPT {
        return new Vec3dArray(count);
    }

    static Vec3dArray* Create(const Vec3d* data, size_t count) SWIFTUSD_NOEXCEPT {
        return new Vec3dArray(data, count);
    }

    // Size/capacity
    size_t size() const SWIFTUSD_NOEXCEPT { return _data.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _data.empty(); }
    size_t capacity() const SWIFTUSD_NOEXCEPT { return _data.capacity(); }

    void reserve(size_t newCapacity) SWIFTUSD_NOEXCEPT { _data.reserve(newCapacity); }
    void resize(size_t newSize) SWIFTUSD_NOEXCEPT { _data.resize(newSize); }
    void clear() SWIFTUSD_NOEXCEPT { _data.clear(); }

    // Element access
    Vec3d GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return index < _data.size() ? _data[index] : Vec3d();
    }

    void SetAt(size_t index, Vec3d value) SWIFTUSD_NOEXCEPT {
        if (index < _data.size()) {
            _data[index] = value;
        }
    }

    // Zero-copy data access
    const Vec3d* data() const SWIFTUSD_NOEXCEPT { return _data.data(); }
    Vec3d* mutableData() SWIFTUSD_NOEXCEPT { return _data.data(); }

    // Modification
    void append(Vec3d value) SWIFTUSD_NOEXCEPT { _data.push_back(value); }
    void popBack() SWIFTUSD_NOEXCEPT { if (!_data.empty()) _data.pop_back(); }

    void assign(const Vec3d* data, size_t count) SWIFTUSD_NOEXCEPT {
        _data.assign(data, data + count);
    }

    // Copy
    Vec3dArray* copy() const SWIFTUSD_NOEXCEPT {
        Vec3dArray* result = new Vec3dArray();
        result->_data = _data;
        return result;
    }

    // Comparison
    bool equals(const Vec3dArray* other) const SWIFTUSD_NOEXCEPT {
        if (!other || _data.size() != other->_data.size()) return false;
        for (size_t i = 0; i < _data.size(); ++i) {
            if (_data[i] != other->_data[i]) return false;
        }
        return true;
    }

    size_t hash() const SWIFTUSD_NOEXCEPT {
        size_t h = 0;
        for (const auto& v : _data) {
            h ^= std::hash<double>{}(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<double>{}(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<double>{}(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Matrix4dArray
// ============================================================================

/// A dynamic array of Matrix4d.
/// Provides a Swift-friendly wrapper around VtArray<GfMatrix4d>.
struct Matrix4dArray {
    std::vector<Matrix4d> _data;

    // Constructors
    Matrix4dArray() SWIFTUSD_NOEXCEPT : _data() {}

    explicit Matrix4dArray(size_t count) SWIFTUSD_NOEXCEPT : _data(count) {}

    Matrix4dArray(const Matrix4d* data, size_t count) SWIFTUSD_NOEXCEPT
        : _data(data, data + count) {}

#if defined(USE_PIXAR_USD)
    Matrix4dArray(const PXR_NS::VtArray<PXR_NS::GfMatrix4d>& array) SWIFTUSD_NOEXCEPT
        : _data(array.size()) {
        for (size_t i = 0; i < array.size(); ++i) {
            _data[i] = Matrix4d(array[i]);
        }
    }

    PXR_NS::VtArray<PXR_NS::GfMatrix4d> ToVt() const SWIFTUSD_NOEXCEPT {
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> result(_data.size());
        for (size_t i = 0; i < _data.size(); ++i) {
            result[i] = _data[i].ToGf();
        }
        return result;
    }
#endif

    // Factory methods
    static Matrix4dArray* Create() SWIFTUSD_NOEXCEPT {
        return new Matrix4dArray();
    }

    static Matrix4dArray* Create(size_t count) SWIFTUSD_NOEXCEPT {
        return new Matrix4dArray(count);
    }

    static Matrix4dArray* Create(const Matrix4d* data, size_t count) SWIFTUSD_NOEXCEPT {
        return new Matrix4dArray(data, count);
    }

    // Size/capacity
    size_t size() const SWIFTUSD_NOEXCEPT { return _data.size(); }
    bool empty() const SWIFTUSD_NOEXCEPT { return _data.empty(); }
    size_t capacity() const SWIFTUSD_NOEXCEPT { return _data.capacity(); }

    void reserve(size_t newCapacity) SWIFTUSD_NOEXCEPT { _data.reserve(newCapacity); }
    void resize(size_t newSize) SWIFTUSD_NOEXCEPT { _data.resize(newSize); }
    void clear() SWIFTUSD_NOEXCEPT { _data.clear(); }

    // Element access
    Matrix4d GetAt(size_t index) const SWIFTUSD_NOEXCEPT {
        return index < _data.size() ? _data[index] : Matrix4d();
    }

    void SetAt(size_t index, Matrix4d value) SWIFTUSD_NOEXCEPT {
        if (index < _data.size()) {
            _data[index] = value;
        }
    }

    // Zero-copy data access
    const Matrix4d* data() const SWIFTUSD_NOEXCEPT { return _data.data(); }
    Matrix4d* mutableData() SWIFTUSD_NOEXCEPT { return _data.data(); }

    // Modification
    void append(Matrix4d value) SWIFTUSD_NOEXCEPT { _data.push_back(value); }
    void popBack() SWIFTUSD_NOEXCEPT { if (!_data.empty()) _data.pop_back(); }

    void assign(const Matrix4d* data, size_t count) SWIFTUSD_NOEXCEPT {
        _data.assign(data, data + count);
    }

    // Copy
    Matrix4dArray* copy() const SWIFTUSD_NOEXCEPT {
        Matrix4dArray* result = new Matrix4dArray();
        result->_data = _data;
        return result;
    }

    // Comparison
    bool equals(const Matrix4dArray* other) const SWIFTUSD_NOEXCEPT {
        if (!other || _data.size() != other->_data.size()) return false;
        for (size_t i = 0; i < _data.size(); ++i) {
            if (_data[i] != other->_data[i]) return false;
        }
        return true;
    }

    size_t hash() const SWIFTUSD_NOEXCEPT {
        size_t h = 0;
        for (const auto& mat : _data) {
            for (int i = 0; i < 16; ++i) {
                h ^= std::hash<double>{}(mat.m[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
        }
        return h;
    }

    // Static factory to create translation matrix
    static Matrix4d translation(double x, double y, double z) SWIFTUSD_NOEXCEPT {
        return Matrix4d::SetTranslate(x, y, z);
    }
} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
