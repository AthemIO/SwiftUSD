// vt.cpp - Vt (Value Types) C wrapper implementation
// Mirrors: pxr/base/vt/

#include "../../include/base/vt.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define VT_USE_USD 1
    #include <pxr/base/vt/value.h>
    #include <pxr/base/vt/array.h>
    #include <pxr/base/vt/dictionary.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/gf/vec3d.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define VT_USE_USD 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <variant>

// ============================================================================
// MARK: - VtValue Implementation
// ============================================================================

struct VtValueOpaque {
#if VT_USE_USD
    VtValue value;
#else
    // Fallback: use variant for common types
    std::variant<
        std::monostate,  // empty
        bool,
        int,
        int64_t,
        unsigned int,
        uint64_t,
        float,
        double,
        std::string
    > value;
#endif
    std::string typeName;  // Cached type name
    std::atomic<int> refCount;

    VtValueOpaque() : refCount(1) {
#if VT_USE_USD
        // value is default-constructed as empty
#endif
    }

#if VT_USE_USD
    explicit VtValueOpaque(const VtValue& v) : value(v), refCount(1) {
        typeName = value.GetTypeName();
    }
#endif

    template<typename T>
    explicit VtValueOpaque(T val) : refCount(1) {
#if VT_USE_USD
        value = VtValue(val);
        typeName = value.GetTypeName();
#else
        value = val;
        updateTypeName();
#endif
    }

#if !VT_USE_USD
    void updateTypeName() {
        std::visit([this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                typeName = "void";
            } else if constexpr (std::is_same_v<T, bool>) {
                typeName = "bool";
            } else if constexpr (std::is_same_v<T, int>) {
                typeName = "int";
            } else if constexpr (std::is_same_v<T, int64_t>) {
                typeName = "int64";
            } else if constexpr (std::is_same_v<T, unsigned int>) {
                typeName = "uint";
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                typeName = "uint64";
            } else if constexpr (std::is_same_v<T, float>) {
                typeName = "float";
            } else if constexpr (std::is_same_v<T, double>) {
                typeName = "double";
            } else if constexpr (std::is_same_v<T, std::string>) {
                typeName = "string";
            }
        }, value);
    }
#endif

    bool isEmpty() const {
#if VT_USE_USD
        return value.IsEmpty();
#else
        return std::holds_alternative<std::monostate>(value);
#endif
    }

    VtValueType getType() const {
#if VT_USE_USD
        if (value.IsEmpty()) return VT_TYPE_UNKNOWN;
        if (value.IsHolding<bool>()) return VT_TYPE_BOOL;
        if (value.IsHolding<int>()) return VT_TYPE_INT;
        if (value.IsHolding<int64_t>()) return VT_TYPE_INT64;
        if (value.IsHolding<unsigned int>()) return VT_TYPE_UINT;
        if (value.IsHolding<uint64_t>()) return VT_TYPE_UINT64;
        if (value.IsHolding<float>()) return VT_TYPE_FLOAT;
        if (value.IsHolding<double>()) return VT_TYPE_DOUBLE;
        if (value.IsHolding<std::string>()) return VT_TYPE_STRING;
        if (value.IsHolding<TfToken>()) return VT_TYPE_TOKEN;
        if (value.IsHolding<GfVec3f>()) return VT_TYPE_VEC3F;
        if (value.IsHolding<GfVec3d>()) return VT_TYPE_VEC3D;
        if (value.IsHolding<VtArray<int>>()) return VT_TYPE_ARRAY_INT;
        if (value.IsHolding<VtArray<float>>()) return VT_TYPE_ARRAY_FLOAT;
        if (value.IsHolding<VtArray<double>>()) return VT_TYPE_ARRAY_DOUBLE;
        if (value.IsHolding<VtArray<GfVec3f>>()) return VT_TYPE_ARRAY_VEC3F;
        if (value.IsHolding<VtArray<GfVec3d>>()) return VT_TYPE_ARRAY_VEC3D;
        if (value.IsHolding<VtDictionary>()) return VT_TYPE_DICTIONARY;
        return VT_TYPE_UNKNOWN;
#else
        return std::visit([](auto&& arg) -> VtValueType {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) return VT_TYPE_UNKNOWN;
            if constexpr (std::is_same_v<T, bool>) return VT_TYPE_BOOL;
            if constexpr (std::is_same_v<T, int>) return VT_TYPE_INT;
            if constexpr (std::is_same_v<T, int64_t>) return VT_TYPE_INT64;
            if constexpr (std::is_same_v<T, unsigned int>) return VT_TYPE_UINT;
            if constexpr (std::is_same_v<T, uint64_t>) return VT_TYPE_UINT64;
            if constexpr (std::is_same_v<T, float>) return VT_TYPE_FLOAT;
            if constexpr (std::is_same_v<T, double>) return VT_TYPE_DOUBLE;
            if constexpr (std::is_same_v<T, std::string>) return VT_TYPE_STRING;
            return VT_TYPE_UNKNOWN;
        }, value);
#endif
    }

    size_t hash() const {
#if VT_USE_USD
        if (value.CanHash()) {
            return value.GetHash();
        }
        return 0;
#else
        return std::visit([](auto&& arg) -> size_t {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return 0;
            } else {
                return std::hash<T>{}(arg);
            }
        }, value);
#endif
    }
};

extern "C" {

// MARK: VtValue Lifecycle

VtValueRef VtValue_Create(void) {
    try {
        return new VtValueOpaque();
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateBool(bool value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateInt(int value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateInt64(int64_t value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateUInt(unsigned int value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateUInt64(uint64_t value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateFloat(float value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateDouble(double value) {
    try {
        return new VtValueOpaque(value);
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_CreateString(const char* value) {
    try {
        return new VtValueOpaque(std::string(value ? value : ""));
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_Copy(VtValueRef value) {
    if (!value) return nullptr;
    try {
#if VT_USE_USD
        return new VtValueOpaque(value->value);
#else
        auto* copy = new VtValueOpaque();
        copy->value = value->value;
        copy->typeName = value->typeName;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

VtValueRef VtValue_Retain(VtValueRef value) {
    if (!value) return nullptr;
    value->refCount.fetch_add(1, std::memory_order_relaxed);
    return value;
}

void VtValue_Release(VtValueRef value) {
    if (!value) return;
    if (value->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete value;
    }
}

// MARK: VtValue Type Inspection

bool VtValue_IsEmpty(VtValueRef value) {
    if (!value) return true;
    return value->isEmpty();
}

VtValueType VtValue_GetType(VtValueRef value) {
    if (!value) return VT_TYPE_UNKNOWN;
    return value->getType();
}

const char* VtValue_GetTypeName(VtValueRef value) {
    if (!value) return "";
    try {
#if VT_USE_USD
        if (value->typeName.empty()) {
            value->typeName = value->value.GetTypeName();
        }
#endif
        return value->typeName.c_str();
    } catch (...) {
        return "";
    }
}

bool VtValue_IsArrayValued(VtValueRef value) {
    if (!value) return false;
#if VT_USE_USD
    return value->value.IsArrayValued();
#else
    return false;  // Fallback doesn't support arrays
#endif
}

size_t VtValue_GetArraySize(VtValueRef value) {
    if (!value) return 0;
#if VT_USE_USD
    if (value->value.IsArrayValued()) {
        return value->value.GetArraySize();
    }
#endif
    return 0;
}

bool VtValue_IsHolding(VtValueRef value, VtValueType type) {
    if (!value) return false;
    return value->getType() == type;
}

// MARK: VtValue Getters

bool VtValue_GetBool(VtValueRef value) {
    if (!value) return false;
#if VT_USE_USD
    if (value->value.IsHolding<bool>()) {
        return value->value.UncheckedGet<bool>();
    }
#else
    if (auto* p = std::get_if<bool>(&value->value)) {
        return *p;
    }
#endif
    return false;
}

int VtValue_GetInt(VtValueRef value) {
    if (!value) return 0;
#if VT_USE_USD
    if (value->value.IsHolding<int>()) {
        return value->value.UncheckedGet<int>();
    }
#else
    if (auto* p = std::get_if<int>(&value->value)) {
        return *p;
    }
#endif
    return 0;
}

int64_t VtValue_GetInt64(VtValueRef value) {
    if (!value) return 0;
#if VT_USE_USD
    if (value->value.IsHolding<int64_t>()) {
        return value->value.UncheckedGet<int64_t>();
    }
#else
    if (auto* p = std::get_if<int64_t>(&value->value)) {
        return *p;
    }
#endif
    return 0;
}

unsigned int VtValue_GetUInt(VtValueRef value) {
    if (!value) return 0;
#if VT_USE_USD
    if (value->value.IsHolding<unsigned int>()) {
        return value->value.UncheckedGet<unsigned int>();
    }
#else
    if (auto* p = std::get_if<unsigned int>(&value->value)) {
        return *p;
    }
#endif
    return 0;
}

uint64_t VtValue_GetUInt64(VtValueRef value) {
    if (!value) return 0;
#if VT_USE_USD
    if (value->value.IsHolding<uint64_t>()) {
        return value->value.UncheckedGet<uint64_t>();
    }
#else
    if (auto* p = std::get_if<uint64_t>(&value->value)) {
        return *p;
    }
#endif
    return 0;
}

float VtValue_GetFloat(VtValueRef value) {
    if (!value) return 0.0f;
#if VT_USE_USD
    if (value->value.IsHolding<float>()) {
        return value->value.UncheckedGet<float>();
    }
#else
    if (auto* p = std::get_if<float>(&value->value)) {
        return *p;
    }
#endif
    return 0.0f;
}

double VtValue_GetDouble(VtValueRef value) {
    if (!value) return 0.0;
#if VT_USE_USD
    if (value->value.IsHolding<double>()) {
        return value->value.UncheckedGet<double>();
    }
#else
    if (auto* p = std::get_if<double>(&value->value)) {
        return *p;
    }
#endif
    return 0.0;
}

char* VtValue_GetString(VtValueRef value) {
    if (!value) {
        char* empty = static_cast<char*>(malloc(1));
        if (empty) empty[0] = '\0';
        return empty;
    }

    std::string str;
#if VT_USE_USD
    if (value->value.IsHolding<std::string>()) {
        str = value->value.UncheckedGet<std::string>();
    } else if (value->value.IsHolding<TfToken>()) {
        str = value->value.UncheckedGet<TfToken>().GetString();
    }
#else
    if (auto* p = std::get_if<std::string>(&value->value)) {
        str = *p;
    }
#endif

    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

void VtValue_FreeString(char* str) {
    free(str);
}

// MARK: VtValue Setters

void VtValue_SetBool(VtValueRef value, bool v) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue(v);
    value->typeName = value->value.GetTypeName();
#else
    value->value = v;
    value->updateTypeName();
#endif
}

void VtValue_SetInt(VtValueRef value, int v) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue(v);
    value->typeName = value->value.GetTypeName();
#else
    value->value = v;
    value->updateTypeName();
#endif
}

void VtValue_SetInt64(VtValueRef value, int64_t v) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue(v);
    value->typeName = value->value.GetTypeName();
#else
    value->value = v;
    value->updateTypeName();
#endif
}

void VtValue_SetFloat(VtValueRef value, float v) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue(v);
    value->typeName = value->value.GetTypeName();
#else
    value->value = v;
    value->updateTypeName();
#endif
}

void VtValue_SetDouble(VtValueRef value, double v) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue(v);
    value->typeName = value->value.GetTypeName();
#else
    value->value = v;
    value->updateTypeName();
#endif
}

void VtValue_SetString(VtValueRef value, const char* str) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue(std::string(str ? str : ""));
    value->typeName = value->value.GetTypeName();
#else
    value->value = std::string(str ? str : "");
    value->updateTypeName();
#endif
}

void VtValue_Clear(VtValueRef value) {
    if (!value) return;
#if VT_USE_USD
    value->value = VtValue();
    value->typeName.clear();
#else
    value->value = std::monostate{};
    value->typeName = "void";
#endif
}

// MARK: VtValue Comparison

bool VtValue_Equal(VtValueRef v1, VtValueRef v2) {
    if (v1 == v2) return true;
    if (!v1 || !v2) return false;
#if VT_USE_USD
    return v1->value == v2->value;
#else
    return v1->value == v2->value;
#endif
}

size_t VtValue_Hash(VtValueRef value) {
    if (!value) return 0;
    return value->hash();
}

bool VtValue_CanHash(VtValueRef value) {
    if (!value) return false;
#if VT_USE_USD
    return value->value.CanHash();
#else
    return true;  // All fallback types are hashable
#endif
}

// MARK: VtValue Casting

bool VtValue_CanCast(VtValueRef value, VtValueType toType) {
    if (!value) return false;
    VtValueType fromType = value->getType();
    if (fromType == toType) return true;

    // Basic numeric casts
    bool fromNumeric = (fromType >= VT_TYPE_BOOL && fromType <= VT_TYPE_DOUBLE);
    bool toNumeric = (toType >= VT_TYPE_BOOL && toType <= VT_TYPE_DOUBLE);
    return fromNumeric && toNumeric;
}

UsdResult VtValue_Cast(VtValueRef value, VtValueType toType) {
    if (!value) return USD_RESULT_INVALID_ARGUMENT;
    if (!VtValue_CanCast(value, toType)) return USD_RESULT_ERROR;

    // If already the right type, nothing to do
    if (value->getType() == toType) return USD_RESULT_SUCCESS;

#if VT_USE_USD
    try {
        // For USD, we handle common casts
        switch (toType) {
            case VT_TYPE_BOOL:
                if (value->value.CanCast<bool>()) {
                    value->value.Cast<bool>();
                }
                break;
            case VT_TYPE_INT:
                if (value->value.CanCast<int>()) {
                    value->value.Cast<int>();
                }
                break;
            case VT_TYPE_FLOAT:
                if (value->value.CanCast<float>()) {
                    value->value.Cast<float>();
                }
                break;
            case VT_TYPE_DOUBLE:
                if (value->value.CanCast<double>()) {
                    value->value.Cast<double>();
                }
                break;
            default:
                return USD_RESULT_ERROR;
        }
        value->typeName = value->value.GetTypeName();
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Fallback casting
    try {
        double numericValue = 0.0;

        // Extract numeric value
        std::visit([&numericValue](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>) {
                numericValue = arg ? 1.0 : 0.0;
            } else if constexpr (std::is_arithmetic_v<T>) {
                numericValue = static_cast<double>(arg);
            }
        }, value->value);

        // Convert to target type
        switch (toType) {
            case VT_TYPE_BOOL:
                value->value = (numericValue != 0.0);
                break;
            case VT_TYPE_INT:
                value->value = static_cast<int>(numericValue);
                break;
            case VT_TYPE_INT64:
                value->value = static_cast<int64_t>(numericValue);
                break;
            case VT_TYPE_UINT:
                value->value = static_cast<unsigned int>(numericValue);
                break;
            case VT_TYPE_UINT64:
                value->value = static_cast<uint64_t>(numericValue);
                break;
            case VT_TYPE_FLOAT:
                value->value = static_cast<float>(numericValue);
                break;
            case VT_TYPE_DOUBLE:
                value->value = numericValue;
                break;
            default:
                return USD_RESULT_ERROR;
        }
        value->updateTypeName();
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#endif
}

} // extern "C"

// ============================================================================
// MARK: - VtArray<int> Implementation
// ============================================================================

struct VtArrayIntOpaque {
#if VT_USE_USD
    VtArray<int> array;
#else
    std::vector<int> array;
#endif
    std::atomic<int> refCount;

    VtArrayIntOpaque() : refCount(1) {}

    explicit VtArrayIntOpaque(size_t size) : refCount(1) {
        array.resize(size);
    }

#if VT_USE_USD
    explicit VtArrayIntOpaque(const VtArray<int>& arr) : array(arr), refCount(1) {}
#endif
};

extern "C" {

VtArrayIntRef VtArrayInt_Create(void) {
    try {
        return new VtArrayIntOpaque();
    } catch (...) {
        return nullptr;
    }
}

VtArrayIntRef VtArrayInt_CreateWithSize(size_t size) {
    try {
        return new VtArrayIntOpaque(size);
    } catch (...) {
        return nullptr;
    }
}

VtArrayIntRef VtArrayInt_CreateFromData(const int* data, size_t count) {
    if (!data && count > 0) return nullptr;
    try {
        auto* arr = new VtArrayIntOpaque();
        arr->array.resize(count);
        if (count > 0 && data) {
#if VT_USE_USD
            memcpy(arr->array.data(), data, count * sizeof(int));
#else
            memcpy(arr->array.data(), data, count * sizeof(int));
#endif
        }
        return arr;
    } catch (...) {
        return nullptr;
    }
}

VtArrayIntRef VtArrayInt_Copy(VtArrayIntRef array) {
    if (!array) return nullptr;
    try {
#if VT_USE_USD
        return new VtArrayIntOpaque(array->array);
#else
        auto* copy = new VtArrayIntOpaque();
        copy->array = array->array;
        return copy;
#endif
    } catch (...) {
        return nullptr;
    }
}

VtArrayIntRef VtArrayInt_Retain(VtArrayIntRef array) {
    if (!array) return nullptr;
    array->refCount.fetch_add(1, std::memory_order_relaxed);
    return array;
}

void VtArrayInt_Release(VtArrayIntRef array) {
    if (!array) return;
    if (array->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete array;
    }
}

size_t VtArrayInt_GetSize(VtArrayIntRef array) {
    if (!array) return 0;
    return array->array.size();
}

bool VtArrayInt_IsEmpty(VtArrayIntRef array) {
    if (!array) return true;
    return array->array.empty();
}

size_t VtArrayInt_GetCapacity(VtArrayIntRef array) {
    if (!array) return 0;
    return array->array.capacity();
}

size_t VtArrayInt_Hash(VtArrayIntRef array) {
    if (!array) return 0;
    size_t h = 0;
    for (size_t i = 0; i < array->array.size(); ++i) {
        h ^= std::hash<int>{}(array->array[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

int VtArrayInt_GetElement(VtArrayIntRef array, size_t index) {
    if (!array || index >= array->array.size()) return 0;
    return array->array[index];
}

void VtArrayInt_SetElement(VtArrayIntRef array, size_t index, int value) {
    if (!array || index >= array->array.size()) return;
    array->array[index] = value;
}

size_t VtArrayInt_GetElements(VtArrayIntRef array, int* buffer, size_t count) {
    if (!array || !buffer) return 0;
    size_t toCopy = std::min(count, array->array.size());
    if (toCopy > 0) {
        memcpy(buffer, array->array.data(), toCopy * sizeof(int));
    }
    return toCopy;
}

int* VtArrayInt_CopyElements(VtArrayIntRef array, size_t* outCount) {
    if (!array || !outCount) {
        if (outCount) *outCount = 0;
        return nullptr;
    }
    size_t size = array->array.size();
    if (size == 0) {
        *outCount = 0;
        return nullptr;
    }
    int* buffer = static_cast<int*>(malloc(size * sizeof(int)));
    if (!buffer) {
        *outCount = 0;
        return nullptr;
    }
    memcpy(buffer, array->array.data(), size * sizeof(int));
    *outCount = size;
    return buffer;
}

void VtArrayInt_Reserve(VtArrayIntRef array, size_t capacity) {
    if (!array) return;
    try {
        array->array.reserve(capacity);
    } catch (...) {}
}

void VtArrayInt_Resize(VtArrayIntRef array, size_t size) {
    if (!array) return;
    try {
        array->array.resize(size);
    } catch (...) {}
}

void VtArrayInt_PushBack(VtArrayIntRef array, int value) {
    if (!array) return;
    try {
        array->array.push_back(value);
    } catch (...) {}
}

void VtArrayInt_PopBack(VtArrayIntRef array) {
    if (!array || array->array.empty()) return;
    array->array.pop_back();
}

void VtArrayInt_Clear(VtArrayIntRef array) {
    if (!array) return;
    array->array.clear();
}

void VtArrayInt_Assign(VtArrayIntRef array, const int* data, size_t count) {
    if (!array) return;
    try {
        array->array.resize(count);
        if (count > 0 && data) {
            memcpy(array->array.data(), data, count * sizeof(int));
        }
    } catch (...) {}
}

bool VtArrayInt_Equal(VtArrayIntRef a1, VtArrayIntRef a2) {
    if (a1 == a2) return true;
    if (!a1 || !a2) return false;
    return a1->array == a2->array;
}

} // extern "C"

// ============================================================================
// MARK: - VtArray<float> Implementation
// ============================================================================

struct VtArrayFloatOpaque {
#if VT_USE_USD
    VtArray<float> array;
#else
    std::vector<float> array;
#endif
    std::atomic<int> refCount;

    VtArrayFloatOpaque() : refCount(1) {}
    explicit VtArrayFloatOpaque(size_t size) : refCount(1) { array.resize(size); }
#if VT_USE_USD
    explicit VtArrayFloatOpaque(const VtArray<float>& arr) : array(arr), refCount(1) {}
#endif
};

extern "C" {

VtArrayFloatRef VtArrayFloat_Create(void) {
    try { return new VtArrayFloatOpaque(); } catch (...) { return nullptr; }
}

VtArrayFloatRef VtArrayFloat_CreateWithSize(size_t size) {
    try { return new VtArrayFloatOpaque(size); } catch (...) { return nullptr; }
}

VtArrayFloatRef VtArrayFloat_CreateFromData(const float* data, size_t count) {
    if (!data && count > 0) return nullptr;
    try {
        auto* arr = new VtArrayFloatOpaque();
        arr->array.resize(count);
        if (count > 0 && data) {
            memcpy(arr->array.data(), data, count * sizeof(float));
        }
        return arr;
    } catch (...) { return nullptr; }
}

VtArrayFloatRef VtArrayFloat_Copy(VtArrayFloatRef array) {
    if (!array) return nullptr;
    try {
#if VT_USE_USD
        return new VtArrayFloatOpaque(array->array);
#else
        auto* copy = new VtArrayFloatOpaque();
        copy->array = array->array;
        return copy;
#endif
    } catch (...) { return nullptr; }
}

VtArrayFloatRef VtArrayFloat_Retain(VtArrayFloatRef array) {
    if (!array) return nullptr;
    array->refCount.fetch_add(1, std::memory_order_relaxed);
    return array;
}

void VtArrayFloat_Release(VtArrayFloatRef array) {
    if (!array) return;
    if (array->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete array;
    }
}

size_t VtArrayFloat_GetSize(VtArrayFloatRef array) {
    return array ? array->array.size() : 0;
}

bool VtArrayFloat_IsEmpty(VtArrayFloatRef array) {
    return !array || array->array.empty();
}

size_t VtArrayFloat_GetCapacity(VtArrayFloatRef array) {
    return array ? array->array.capacity() : 0;
}

size_t VtArrayFloat_Hash(VtArrayFloatRef array) {
    if (!array) return 0;
    size_t h = 0;
    for (size_t i = 0; i < array->array.size(); ++i) {
        h ^= std::hash<float>{}(array->array[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

float VtArrayFloat_GetElement(VtArrayFloatRef array, size_t index) {
    if (!array || index >= array->array.size()) return 0.0f;
    return array->array[index];
}

void VtArrayFloat_SetElement(VtArrayFloatRef array, size_t index, float value) {
    if (!array || index >= array->array.size()) return;
    array->array[index] = value;
}

size_t VtArrayFloat_GetElements(VtArrayFloatRef array, float* buffer, size_t count) {
    if (!array || !buffer) return 0;
    size_t toCopy = std::min(count, array->array.size());
    if (toCopy > 0) memcpy(buffer, array->array.data(), toCopy * sizeof(float));
    return toCopy;
}

float* VtArrayFloat_CopyElements(VtArrayFloatRef array, size_t* outCount) {
    if (!array || !outCount) { if (outCount) *outCount = 0; return nullptr; }
    size_t size = array->array.size();
    if (size == 0) { *outCount = 0; return nullptr; }
    float* buffer = static_cast<float*>(malloc(size * sizeof(float)));
    if (!buffer) { *outCount = 0; return nullptr; }
    memcpy(buffer, array->array.data(), size * sizeof(float));
    *outCount = size;
    return buffer;
}

void VtArrayFloat_Reserve(VtArrayFloatRef array, size_t capacity) {
    if (array) try { array->array.reserve(capacity); } catch (...) {}
}

void VtArrayFloat_Resize(VtArrayFloatRef array, size_t size) {
    if (array) try { array->array.resize(size); } catch (...) {}
}

void VtArrayFloat_PushBack(VtArrayFloatRef array, float value) {
    if (array) try { array->array.push_back(value); } catch (...) {}
}

void VtArrayFloat_PopBack(VtArrayFloatRef array) {
    if (array && !array->array.empty()) array->array.pop_back();
}

void VtArrayFloat_Clear(VtArrayFloatRef array) {
    if (array) array->array.clear();
}

void VtArrayFloat_Assign(VtArrayFloatRef array, const float* data, size_t count) {
    if (!array) return;
    try {
        array->array.resize(count);
        if (count > 0 && data) memcpy(array->array.data(), data, count * sizeof(float));
    } catch (...) {}
}

bool VtArrayFloat_Equal(VtArrayFloatRef a1, VtArrayFloatRef a2) {
    if (a1 == a2) return true;
    if (!a1 || !a2) return false;
    return a1->array == a2->array;
}

} // extern "C"

// ============================================================================
// MARK: - VtArray<double> Implementation
// ============================================================================

struct VtArrayDoubleOpaque {
#if VT_USE_USD
    VtArray<double> array;
#else
    std::vector<double> array;
#endif
    std::atomic<int> refCount;

    VtArrayDoubleOpaque() : refCount(1) {}
    explicit VtArrayDoubleOpaque(size_t size) : refCount(1) { array.resize(size); }
#if VT_USE_USD
    explicit VtArrayDoubleOpaque(const VtArray<double>& arr) : array(arr), refCount(1) {}
#endif
};

extern "C" {

VtArrayDoubleRef VtArrayDouble_Create(void) {
    try { return new VtArrayDoubleOpaque(); } catch (...) { return nullptr; }
}

VtArrayDoubleRef VtArrayDouble_CreateWithSize(size_t size) {
    try { return new VtArrayDoubleOpaque(size); } catch (...) { return nullptr; }
}

VtArrayDoubleRef VtArrayDouble_CreateFromData(const double* data, size_t count) {
    if (!data && count > 0) return nullptr;
    try {
        auto* arr = new VtArrayDoubleOpaque();
        arr->array.resize(count);
        if (count > 0 && data) memcpy(arr->array.data(), data, count * sizeof(double));
        return arr;
    } catch (...) { return nullptr; }
}

VtArrayDoubleRef VtArrayDouble_Copy(VtArrayDoubleRef array) {
    if (!array) return nullptr;
    try {
#if VT_USE_USD
        return new VtArrayDoubleOpaque(array->array);
#else
        auto* copy = new VtArrayDoubleOpaque();
        copy->array = array->array;
        return copy;
#endif
    } catch (...) { return nullptr; }
}

VtArrayDoubleRef VtArrayDouble_Retain(VtArrayDoubleRef array) {
    if (!array) return nullptr;
    array->refCount.fetch_add(1, std::memory_order_relaxed);
    return array;
}

void VtArrayDouble_Release(VtArrayDoubleRef array) {
    if (!array) return;
    if (array->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete array;
}

size_t VtArrayDouble_GetSize(VtArrayDoubleRef array) {
    return array ? array->array.size() : 0;
}

bool VtArrayDouble_IsEmpty(VtArrayDoubleRef array) {
    return !array || array->array.empty();
}

size_t VtArrayDouble_GetCapacity(VtArrayDoubleRef array) {
    return array ? array->array.capacity() : 0;
}

size_t VtArrayDouble_Hash(VtArrayDoubleRef array) {
    if (!array) return 0;
    size_t h = 0;
    for (size_t i = 0; i < array->array.size(); ++i) {
        h ^= std::hash<double>{}(array->array[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

double VtArrayDouble_GetElement(VtArrayDoubleRef array, size_t index) {
    if (!array || index >= array->array.size()) return 0.0;
    return array->array[index];
}

void VtArrayDouble_SetElement(VtArrayDoubleRef array, size_t index, double value) {
    if (!array || index >= array->array.size()) return;
    array->array[index] = value;
}

size_t VtArrayDouble_GetElements(VtArrayDoubleRef array, double* buffer, size_t count) {
    if (!array || !buffer) return 0;
    size_t toCopy = std::min(count, array->array.size());
    if (toCopy > 0) memcpy(buffer, array->array.data(), toCopy * sizeof(double));
    return toCopy;
}

double* VtArrayDouble_CopyElements(VtArrayDoubleRef array, size_t* outCount) {
    if (!array || !outCount) { if (outCount) *outCount = 0; return nullptr; }
    size_t size = array->array.size();
    if (size == 0) { *outCount = 0; return nullptr; }
    double* buffer = static_cast<double*>(malloc(size * sizeof(double)));
    if (!buffer) { *outCount = 0; return nullptr; }
    memcpy(buffer, array->array.data(), size * sizeof(double));
    *outCount = size;
    return buffer;
}

void VtArrayDouble_Reserve(VtArrayDoubleRef array, size_t capacity) {
    if (array) try { array->array.reserve(capacity); } catch (...) {}
}

void VtArrayDouble_Resize(VtArrayDoubleRef array, size_t size) {
    if (array) try { array->array.resize(size); } catch (...) {}
}

void VtArrayDouble_PushBack(VtArrayDoubleRef array, double value) {
    if (array) try { array->array.push_back(value); } catch (...) {}
}

void VtArrayDouble_PopBack(VtArrayDoubleRef array) {
    if (array && !array->array.empty()) array->array.pop_back();
}

void VtArrayDouble_Clear(VtArrayDoubleRef array) {
    if (array) array->array.clear();
}

void VtArrayDouble_Assign(VtArrayDoubleRef array, const double* data, size_t count) {
    if (!array) return;
    try {
        array->array.resize(count);
        if (count > 0 && data) memcpy(array->array.data(), data, count * sizeof(double));
    } catch (...) {}
}

bool VtArrayDouble_Equal(VtArrayDoubleRef a1, VtArrayDoubleRef a2) {
    if (a1 == a2) return true;
    if (!a1 || !a2) return false;
    return a1->array == a2->array;
}

} // extern "C"

// ============================================================================
// MARK: - VtArray<GfVec3f> Implementation
// ============================================================================

struct VtArrayVec3fOpaque {
#if VT_USE_USD
    VtArray<GfVec3f> array;
#else
    std::vector<float> array;  // Stored as flat [x,y,z,x,y,z,...]
#endif
    std::atomic<int> refCount;

    VtArrayVec3fOpaque() : refCount(1) {}
    explicit VtArrayVec3fOpaque(size_t size) : refCount(1) {
#if VT_USE_USD
        array.resize(size);
#else
        array.resize(size * 3);
#endif
    }
#if VT_USE_USD
    explicit VtArrayVec3fOpaque(const VtArray<GfVec3f>& arr) : array(arr), refCount(1) {}
#endif

    size_t getSize() const {
#if VT_USE_USD
        return array.size();
#else
        return array.size() / 3;
#endif
    }
};

extern "C" {

VtArrayVec3fRef VtArrayVec3f_Create(void) {
    try { return new VtArrayVec3fOpaque(); } catch (...) { return nullptr; }
}

VtArrayVec3fRef VtArrayVec3f_CreateWithSize(size_t size) {
    try { return new VtArrayVec3fOpaque(size); } catch (...) { return nullptr; }
}

VtArrayVec3fRef VtArrayVec3f_CreateFromData(const float* data, size_t count) {
    if (!data && count > 0) return nullptr;
    try {
        auto* arr = new VtArrayVec3fOpaque();
#if VT_USE_USD
        arr->array.resize(count);
        for (size_t i = 0; i < count; ++i) {
            arr->array[i] = GfVec3f(data[i*3], data[i*3+1], data[i*3+2]);
        }
#else
        arr->array.resize(count * 3);
        if (count > 0 && data) memcpy(arr->array.data(), data, count * 3 * sizeof(float));
#endif
        return arr;
    } catch (...) { return nullptr; }
}

VtArrayVec3fRef VtArrayVec3f_Copy(VtArrayVec3fRef array) {
    if (!array) return nullptr;
    try {
#if VT_USE_USD
        return new VtArrayVec3fOpaque(array->array);
#else
        auto* copy = new VtArrayVec3fOpaque();
        copy->array = array->array;
        return copy;
#endif
    } catch (...) { return nullptr; }
}

VtArrayVec3fRef VtArrayVec3f_Retain(VtArrayVec3fRef array) {
    if (!array) return nullptr;
    array->refCount.fetch_add(1, std::memory_order_relaxed);
    return array;
}

void VtArrayVec3f_Release(VtArrayVec3fRef array) {
    if (!array) return;
    if (array->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete array;
}

size_t VtArrayVec3f_GetSize(VtArrayVec3fRef array) {
    return array ? array->getSize() : 0;
}

bool VtArrayVec3f_IsEmpty(VtArrayVec3fRef array) {
    return !array || array->getSize() == 0;
}

size_t VtArrayVec3f_Hash(VtArrayVec3fRef array) {
    if (!array) return 0;
    size_t h = 0;
#if VT_USE_USD
    for (size_t i = 0; i < array->array.size(); ++i) {
        const auto& v = array->array[i];
        h ^= std::hash<float>{}(v[0]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<float>{}(v[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<float>{}(v[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
#else
    for (size_t i = 0; i < array->array.size(); ++i) {
        h ^= std::hash<float>{}(array->array[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
#endif
    return h;
}

void VtArrayVec3f_GetElement(VtArrayVec3fRef array, size_t index,
                              float* outX, float* outY, float* outZ) {
    if (!array || !outX || !outY || !outZ || index >= array->getSize()) {
        if (outX) *outX = 0; if (outY) *outY = 0; if (outZ) *outZ = 0;
        return;
    }
#if VT_USE_USD
    const auto& v = array->array[index];
    *outX = v[0]; *outY = v[1]; *outZ = v[2];
#else
    *outX = array->array[index*3];
    *outY = array->array[index*3+1];
    *outZ = array->array[index*3+2];
#endif
}

void VtArrayVec3f_SetElement(VtArrayVec3fRef array, size_t index, float x, float y, float z) {
    if (!array || index >= array->getSize()) return;
#if VT_USE_USD
    array->array[index] = GfVec3f(x, y, z);
#else
    array->array[index*3] = x;
    array->array[index*3+1] = y;
    array->array[index*3+2] = z;
#endif
}

size_t VtArrayVec3f_GetElements(VtArrayVec3fRef array, float* buffer, size_t maxCount) {
    if (!array || !buffer) return 0;
    size_t count = std::min(maxCount, array->getSize());
#if VT_USE_USD
    for (size_t i = 0; i < count; ++i) {
        const auto& v = array->array[i];
        buffer[i*3] = v[0];
        buffer[i*3+1] = v[1];
        buffer[i*3+2] = v[2];
    }
#else
    if (count > 0) memcpy(buffer, array->array.data(), count * 3 * sizeof(float));
#endif
    return count;
}

float* VtArrayVec3f_CopyElements(VtArrayVec3fRef array, size_t* outCount) {
    if (!array || !outCount) { if (outCount) *outCount = 0; return nullptr; }
    size_t size = array->getSize();
    if (size == 0) { *outCount = 0; return nullptr; }
    float* buffer = static_cast<float*>(malloc(size * 3 * sizeof(float)));
    if (!buffer) { *outCount = 0; return nullptr; }
    VtArrayVec3f_GetElements(array, buffer, size);
    *outCount = size;
    return buffer;
}

void VtArrayVec3f_Reserve(VtArrayVec3fRef array, size_t capacity) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.reserve(capacity);
#else
        array->array.reserve(capacity * 3);
#endif
    } catch (...) {}
}

void VtArrayVec3f_Resize(VtArrayVec3fRef array, size_t size) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.resize(size);
#else
        array->array.resize(size * 3);
#endif
    } catch (...) {}
}

void VtArrayVec3f_PushBack(VtArrayVec3fRef array, float x, float y, float z) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.push_back(GfVec3f(x, y, z));
#else
        array->array.push_back(x);
        array->array.push_back(y);
        array->array.push_back(z);
#endif
    } catch (...) {}
}

void VtArrayVec3f_Clear(VtArrayVec3fRef array) {
    if (array) array->array.clear();
}

void VtArrayVec3f_Assign(VtArrayVec3fRef array, const float* data, size_t count) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.resize(count);
        for (size_t i = 0; i < count; ++i) {
            array->array[i] = GfVec3f(data[i*3], data[i*3+1], data[i*3+2]);
        }
#else
        array->array.resize(count * 3);
        if (count > 0 && data) memcpy(array->array.data(), data, count * 3 * sizeof(float));
#endif
    } catch (...) {}
}

bool VtArrayVec3f_Equal(VtArrayVec3fRef a1, VtArrayVec3fRef a2) {
    if (a1 == a2) return true;
    if (!a1 || !a2) return false;
    return a1->array == a2->array;
}

} // extern "C"

// ============================================================================
// MARK: - VtArray<GfVec3d> Implementation
// ============================================================================

struct VtArrayVec3dOpaque {
#if VT_USE_USD
    VtArray<GfVec3d> array;
#else
    std::vector<double> array;
#endif
    std::atomic<int> refCount;

    VtArrayVec3dOpaque() : refCount(1) {}
    explicit VtArrayVec3dOpaque(size_t size) : refCount(1) {
#if VT_USE_USD
        array.resize(size);
#else
        array.resize(size * 3);
#endif
    }
#if VT_USE_USD
    explicit VtArrayVec3dOpaque(const VtArray<GfVec3d>& arr) : array(arr), refCount(1) {}
#endif

    size_t getSize() const {
#if VT_USE_USD
        return array.size();
#else
        return array.size() / 3;
#endif
    }
};

extern "C" {

VtArrayVec3dRef VtArrayVec3d_Create(void) {
    try { return new VtArrayVec3dOpaque(); } catch (...) { return nullptr; }
}

VtArrayVec3dRef VtArrayVec3d_CreateWithSize(size_t size) {
    try { return new VtArrayVec3dOpaque(size); } catch (...) { return nullptr; }
}

VtArrayVec3dRef VtArrayVec3d_CreateFromData(const double* data, size_t count) {
    if (!data && count > 0) return nullptr;
    try {
        auto* arr = new VtArrayVec3dOpaque();
#if VT_USE_USD
        arr->array.resize(count);
        for (size_t i = 0; i < count; ++i) {
            arr->array[i] = GfVec3d(data[i*3], data[i*3+1], data[i*3+2]);
        }
#else
        arr->array.resize(count * 3);
        if (count > 0 && data) memcpy(arr->array.data(), data, count * 3 * sizeof(double));
#endif
        return arr;
    } catch (...) { return nullptr; }
}

VtArrayVec3dRef VtArrayVec3d_Copy(VtArrayVec3dRef array) {
    if (!array) return nullptr;
    try {
#if VT_USE_USD
        return new VtArrayVec3dOpaque(array->array);
#else
        auto* copy = new VtArrayVec3dOpaque();
        copy->array = array->array;
        return copy;
#endif
    } catch (...) { return nullptr; }
}

VtArrayVec3dRef VtArrayVec3d_Retain(VtArrayVec3dRef array) {
    if (!array) return nullptr;
    array->refCount.fetch_add(1, std::memory_order_relaxed);
    return array;
}

void VtArrayVec3d_Release(VtArrayVec3dRef array) {
    if (!array) return;
    if (array->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete array;
}

size_t VtArrayVec3d_GetSize(VtArrayVec3dRef array) {
    return array ? array->getSize() : 0;
}

bool VtArrayVec3d_IsEmpty(VtArrayVec3dRef array) {
    return !array || array->getSize() == 0;
}

size_t VtArrayVec3d_Hash(VtArrayVec3dRef array) {
    if (!array) return 0;
    size_t h = 0;
#if VT_USE_USD
    for (size_t i = 0; i < array->array.size(); ++i) {
        const auto& v = array->array[i];
        h ^= std::hash<double>{}(v[0]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(v[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<double>{}(v[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
#else
    for (size_t i = 0; i < array->array.size(); ++i) {
        h ^= std::hash<double>{}(array->array[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
#endif
    return h;
}

void VtArrayVec3d_GetElement(VtArrayVec3dRef array, size_t index,
                              double* outX, double* outY, double* outZ) {
    if (!array || !outX || !outY || !outZ || index >= array->getSize()) {
        if (outX) *outX = 0; if (outY) *outY = 0; if (outZ) *outZ = 0;
        return;
    }
#if VT_USE_USD
    const auto& v = array->array[index];
    *outX = v[0]; *outY = v[1]; *outZ = v[2];
#else
    *outX = array->array[index*3];
    *outY = array->array[index*3+1];
    *outZ = array->array[index*3+2];
#endif
}

void VtArrayVec3d_SetElement(VtArrayVec3dRef array, size_t index, double x, double y, double z) {
    if (!array || index >= array->getSize()) return;
#if VT_USE_USD
    array->array[index] = GfVec3d(x, y, z);
#else
    array->array[index*3] = x;
    array->array[index*3+1] = y;
    array->array[index*3+2] = z;
#endif
}

size_t VtArrayVec3d_GetElements(VtArrayVec3dRef array, double* buffer, size_t maxCount) {
    if (!array || !buffer) return 0;
    size_t count = std::min(maxCount, array->getSize());
#if VT_USE_USD
    for (size_t i = 0; i < count; ++i) {
        const auto& v = array->array[i];
        buffer[i*3] = v[0];
        buffer[i*3+1] = v[1];
        buffer[i*3+2] = v[2];
    }
#else
    if (count > 0) memcpy(buffer, array->array.data(), count * 3 * sizeof(double));
#endif
    return count;
}

double* VtArrayVec3d_CopyElements(VtArrayVec3dRef array, size_t* outCount) {
    if (!array || !outCount) { if (outCount) *outCount = 0; return nullptr; }
    size_t size = array->getSize();
    if (size == 0) { *outCount = 0; return nullptr; }
    double* buffer = static_cast<double*>(malloc(size * 3 * sizeof(double)));
    if (!buffer) { *outCount = 0; return nullptr; }
    VtArrayVec3d_GetElements(array, buffer, size);
    *outCount = size;
    return buffer;
}

void VtArrayVec3d_Reserve(VtArrayVec3dRef array, size_t capacity) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.reserve(capacity);
#else
        array->array.reserve(capacity * 3);
#endif
    } catch (...) {}
}

void VtArrayVec3d_Resize(VtArrayVec3dRef array, size_t size) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.resize(size);
#else
        array->array.resize(size * 3);
#endif
    } catch (...) {}
}

void VtArrayVec3d_PushBack(VtArrayVec3dRef array, double x, double y, double z) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.push_back(GfVec3d(x, y, z));
#else
        array->array.push_back(x);
        array->array.push_back(y);
        array->array.push_back(z);
#endif
    } catch (...) {}
}

void VtArrayVec3d_Clear(VtArrayVec3dRef array) {
    if (array) array->array.clear();
}

void VtArrayVec3d_Assign(VtArrayVec3dRef array, const double* data, size_t count) {
    if (!array) return;
    try {
#if VT_USE_USD
        array->array.resize(count);
        for (size_t i = 0; i < count; ++i) {
            array->array[i] = GfVec3d(data[i*3], data[i*3+1], data[i*3+2]);
        }
#else
        array->array.resize(count * 3);
        if (count > 0 && data) memcpy(array->array.data(), data, count * 3 * sizeof(double));
#endif
    } catch (...) {}
}

bool VtArrayVec3d_Equal(VtArrayVec3dRef a1, VtArrayVec3dRef a2) {
    if (a1 == a2) return true;
    if (!a1 || !a2) return false;
    return a1->array == a2->array;
}

} // extern "C"

// ============================================================================
// MARK: - VtDictionary Implementation
// ============================================================================

struct VtDictionaryOpaque {
#if VT_USE_USD
    VtDictionary dict;
#else
    std::map<std::string, VtValueRef> dict;
#endif
    std::atomic<int> refCount;

    VtDictionaryOpaque() : refCount(1) {}

#if VT_USE_USD
    explicit VtDictionaryOpaque(const VtDictionary& d) : dict(d), refCount(1) {}
#endif

#if !VT_USE_USD
    ~VtDictionaryOpaque() {
        // Release all values in fallback mode
        for (auto& pair : dict) {
            VtValue_Release(pair.second);
        }
    }
#endif
};

extern "C" {

VtDictionaryRef VtDictionary_Create(void) {
    try { return new VtDictionaryOpaque(); } catch (...) { return nullptr; }
}

VtDictionaryRef VtDictionary_Copy(VtDictionaryRef dict) {
    if (!dict) return nullptr;
    try {
#if VT_USE_USD
        return new VtDictionaryOpaque(dict->dict);
#else
        auto* copy = new VtDictionaryOpaque();
        for (const auto& pair : dict->dict) {
            copy->dict[pair.first] = VtValue_Copy(pair.second);
        }
        return copy;
#endif
    } catch (...) { return nullptr; }
}

VtDictionaryRef VtDictionary_Retain(VtDictionaryRef dict) {
    if (!dict) return nullptr;
    dict->refCount.fetch_add(1, std::memory_order_relaxed);
    return dict;
}

void VtDictionary_Release(VtDictionaryRef dict) {
    if (!dict) return;
    if (dict->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete dict;
    }
}

size_t VtDictionary_GetSize(VtDictionaryRef dict) {
    return dict ? dict->dict.size() : 0;
}

bool VtDictionary_IsEmpty(VtDictionaryRef dict) {
    return !dict || dict->dict.empty();
}

size_t VtDictionary_Hash(VtDictionaryRef dict) {
    if (!dict) return 0;
#if VT_USE_USD
    return hash_value(dict->dict);
#else
    size_t h = 0;
    for (const auto& pair : dict->dict) {
        h ^= std::hash<std::string>{}(pair.first) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= VtValue_Hash(pair.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
#endif
}

bool VtDictionary_Contains(VtDictionaryRef dict, const char* key) {
    if (!dict || !key) return false;
#if VT_USE_USD
    return dict->dict.count(key) > 0;
#else
    return dict->dict.count(key) > 0;
#endif
}

VtValueRef VtDictionary_GetValue(VtDictionaryRef dict, const char* key) {
    if (!dict || !key) return nullptr;
    try {
#if VT_USE_USD
        auto it = dict->dict.find(key);
        if (it != dict->dict.end()) {
            return new VtValueOpaque(it->second);
        }
#else
        auto it = dict->dict.find(key);
        if (it != dict->dict.end()) {
            return VtValue_Copy(it->second);
        }
#endif
        return nullptr;
    } catch (...) { return nullptr; }
}

VtValueRef VtDictionary_GetValueAtPath(VtDictionaryRef dict, const char* path) {
    if (!dict || !path) return nullptr;
    try {
#if VT_USE_USD
        const VtValue* val = dict->dict.GetValueAtPath(path);
        if (val) {
            return new VtValueOpaque(*val);
        }
#else
        // Simple path parsing for fallback (colon-separated)
        std::string pathStr(path);
        size_t pos = pathStr.find(':');
        if (pos == std::string::npos) {
            return VtDictionary_GetValue(dict, path);
        }
        // Nested path not fully supported in fallback
#endif
        return nullptr;
    } catch (...) { return nullptr; }
}

void VtDictionary_SetValue(VtDictionaryRef dict, const char* key, VtValueRef value) {
    if (!dict || !key) return;
    try {
#if VT_USE_USD
        if (value) {
            dict->dict[key] = value->value;
        } else {
            dict->dict.erase(key);
        }
#else
        auto it = dict->dict.find(key);
        if (it != dict->dict.end()) {
            VtValue_Release(it->second);
        }
        if (value) {
            dict->dict[key] = VtValue_Copy(value);
        } else {
            dict->dict.erase(key);
        }
#endif
    } catch (...) {}
}

void VtDictionary_SetValueAtPath(VtDictionaryRef dict, const char* path, VtValueRef value) {
    if (!dict || !path || !value) return;
    try {
#if VT_USE_USD
        dict->dict.SetValueAtPath(path, value->value);
#else
        // Simple implementation for fallback
        VtDictionary_SetValue(dict, path, value);
#endif
    } catch (...) {}
}

bool VtDictionary_Erase(VtDictionaryRef dict, const char* key) {
    if (!dict || !key) return false;
    try {
#if VT_USE_USD
        return dict->dict.erase(key) > 0;
#else
        auto it = dict->dict.find(key);
        if (it != dict->dict.end()) {
            VtValue_Release(it->second);
            dict->dict.erase(it);
            return true;
        }
        return false;
#endif
    } catch (...) { return false; }
}

bool VtDictionary_EraseAtPath(VtDictionaryRef dict, const char* path) {
    if (!dict || !path) return false;
    try {
#if VT_USE_USD
        dict->dict.EraseValueAtPath(path);
        return true;  // VtDictionary doesn't return whether it was found
#else
        return VtDictionary_Erase(dict, path);
#endif
    } catch (...) { return false; }
}

void VtDictionary_Clear(VtDictionaryRef dict) {
    if (!dict) return;
#if VT_USE_USD
    dict->dict.clear();
#else
    for (auto& pair : dict->dict) {
        VtValue_Release(pair.second);
    }
    dict->dict.clear();
#endif
}

char** VtDictionary_GetKeys(VtDictionaryRef dict, size_t* outCount) {
    if (!dict || !outCount) {
        if (outCount) *outCount = 0;
        return nullptr;
    }

    try {
        size_t size = dict->dict.size();
        if (size == 0) {
            *outCount = 0;
            return nullptr;
        }

        char** keys = static_cast<char**>(malloc(size * sizeof(char*)));
        if (!keys) {
            *outCount = 0;
            return nullptr;
        }

        size_t i = 0;
#if VT_USE_USD
        for (const auto& pair : dict->dict) {
            const std::string& key = pair.first;
            keys[i] = static_cast<char*>(malloc(key.size() + 1));
            if (!keys[i]) {
                for (size_t j = 0; j < i; ++j) free(keys[j]);
                free(keys);
                *outCount = 0;
                return nullptr;
            }
            memcpy(keys[i], key.c_str(), key.size() + 1);
            ++i;
        }
#else
        for (const auto& pair : dict->dict) {
            keys[i] = static_cast<char*>(malloc(pair.first.size() + 1));
            if (!keys[i]) {
                for (size_t j = 0; j < i; ++j) free(keys[j]);
                free(keys);
                *outCount = 0;
                return nullptr;
            }
            memcpy(keys[i], pair.first.c_str(), pair.first.size() + 1);
            ++i;
        }
#endif

        *outCount = size;
        return keys;
    } catch (...) {
        *outCount = 0;
        return nullptr;
    }
}

void VtDictionary_FreeKeys(char** keys, size_t count) {
    if (!keys) return;
    for (size_t i = 0; i < count; ++i) {
        free(keys[i]);
    }
    free(keys);
}

bool VtDictionary_Equal(VtDictionaryRef d1, VtDictionaryRef d2) {
    if (d1 == d2) return true;
    if (!d1 || !d2) return false;
#if VT_USE_USD
    return d1->dict == d2->dict;
#else
    if (d1->dict.size() != d2->dict.size()) return false;
    for (const auto& pair : d1->dict) {
        auto it = d2->dict.find(pair.first);
        if (it == d2->dict.end()) return false;
        if (!VtValue_Equal(pair.second, it->second)) return false;
    }
    return true;
#endif
}

VtDictionaryRef VtDictionary_Over(VtDictionaryRef strong, VtDictionaryRef weak) {
    if (!strong) return VtDictionary_Copy(weak);
    if (!weak) return VtDictionary_Copy(strong);
    try {
#if VT_USE_USD
        return new VtDictionaryOpaque(VtDictionaryOver(strong->dict, weak->dict));
#else
        // Simple composition: start with weak, overlay strong
        auto* result = new VtDictionaryOpaque();
        for (const auto& pair : weak->dict) {
            result->dict[pair.first] = VtValue_Copy(pair.second);
        }
        for (const auto& pair : strong->dict) {
            auto it = result->dict.find(pair.first);
            if (it != result->dict.end()) {
                VtValue_Release(it->second);
            }
            result->dict[pair.first] = VtValue_Copy(pair.second);
        }
        return result;
#endif
    } catch (...) { return nullptr; }
}

} // extern "C"
