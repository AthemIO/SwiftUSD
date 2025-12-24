// usdLux.cpp - UsdLux module C wrapper implementation
// Mirrors: pxr/usd/usdLux/*.h

#include "../../include/usdLux/usdLux.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdLux/distantLight.h>
    #include <pxr/usd/usdLux/domeLight.h>
    #include <pxr/usd/usdLux/rectLight.h>
    #include <pxr/usd/usdLux/sphereLight.h>
    #include <pxr/usd/usdLux/cylinderLight.h>
    #include <pxr/usd/usdLux/diskLight.h>
    #include <pxr/usd/usdLux/lightFilter.h>
    #include <pxr/usd/usdLux/shadowAPI.h>
    #include <pxr/usd/usdLux/lightAPI.h>
    #include <pxr/usd/usdLux/tokens.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/usd/sdf/assetPath.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>

// ============================================================================
// MARK: - Wrapper Structures
// ============================================================================

struct UsdLuxDistantLightOpaque {
#if USD_USE_FULL
    UsdLuxDistantLight light;
#endif
    std::atomic<int> refCount;
    bool valid;
    float angle;
    float intensity;
    float color[3];
    float exposure;
    float diffuse;
    float specular;
    bool normalize;

    UsdLuxDistantLightOpaque() : refCount(1), valid(false), angle(0.53f), intensity(1.0f),
        exposure(0.0f), diffuse(1.0f), specular(1.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxDistantLightOpaque(const UsdLuxDistantLight& l)
        : light(l), refCount(1), valid(true), angle(0.53f), intensity(1.0f),
          exposure(0.0f), diffuse(1.0f), specular(1.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }
#endif
};

struct UsdLuxDomeLightOpaque {
#if USD_USE_FULL
    UsdLuxDomeLight light;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::string textureFile;
    std::string textureFormat;
    float guideRadius;
    float intensity;
    float color[3];
    float exposure;

    UsdLuxDomeLightOpaque() : refCount(1), valid(false), textureFormat("automatic"),
        guideRadius(100000.0f), intensity(1.0f), exposure(0.0f) {
        color[0] = color[1] = color[2] = 1.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxDomeLightOpaque(const UsdLuxDomeLight& l)
        : light(l), refCount(1), valid(true), textureFormat("automatic"),
          guideRadius(100000.0f), intensity(1.0f), exposure(0.0f) {
        color[0] = color[1] = color[2] = 1.0f;
    }
#endif
};

struct UsdLuxRectLightOpaque {
#if USD_USE_FULL
    UsdLuxRectLight light;
#endif
    std::atomic<int> refCount;
    bool valid;
    float width;
    float height;
    std::string textureFile;
    float intensity;
    float color[3];
    float exposure;
    bool normalize;

    UsdLuxRectLightOpaque() : refCount(1), valid(false), width(1.0f), height(1.0f),
        intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxRectLightOpaque(const UsdLuxRectLight& l)
        : light(l), refCount(1), valid(true), width(1.0f), height(1.0f),
          intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }
#endif
};

struct UsdLuxSphereLightOpaque {
#if USD_USE_FULL
    UsdLuxSphereLight light;
#endif
    std::atomic<int> refCount;
    bool valid;
    float radius;
    bool treatAsPoint;
    float intensity;
    float color[3];
    float exposure;
    bool normalize;

    UsdLuxSphereLightOpaque() : refCount(1), valid(false), radius(0.5f), treatAsPoint(false),
        intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxSphereLightOpaque(const UsdLuxSphereLight& l)
        : light(l), refCount(1), valid(true), radius(0.5f), treatAsPoint(false),
          intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }
#endif
};

struct UsdLuxCylinderLightOpaque {
#if USD_USE_FULL
    UsdLuxCylinderLight light;
#endif
    std::atomic<int> refCount;
    bool valid;
    float length;
    float radius;
    bool treatAsLine;
    float intensity;
    float color[3];
    float exposure;
    bool normalize;

    UsdLuxCylinderLightOpaque() : refCount(1), valid(false), length(1.0f), radius(0.5f),
        treatAsLine(false), intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxCylinderLightOpaque(const UsdLuxCylinderLight& l)
        : light(l), refCount(1), valid(true), length(1.0f), radius(0.5f),
          treatAsLine(false), intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }
#endif
};

struct UsdLuxDiskLightOpaque {
#if USD_USE_FULL
    UsdLuxDiskLight light;
#endif
    std::atomic<int> refCount;
    bool valid;
    float radius;
    float intensity;
    float color[3];
    float exposure;
    bool normalize;

    UsdLuxDiskLightOpaque() : refCount(1), valid(false), radius(0.5f),
        intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxDiskLightOpaque(const UsdLuxDiskLight& l)
        : light(l), refCount(1), valid(true), radius(0.5f),
          intensity(1.0f), exposure(0.0f), normalize(false) {
        color[0] = color[1] = color[2] = 1.0f;
    }
#endif
};

struct UsdLuxLightFilterOpaque {
#if USD_USE_FULL
    UsdLuxLightFilter filter;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::string shaderId;

    UsdLuxLightFilterOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdLuxLightFilterOpaque(const UsdLuxLightFilter& f)
        : filter(f), refCount(1), valid(true) {}
#endif
};

struct UsdLuxShadowAPIOpaque {
#if USD_USE_FULL
    UsdLuxShadowAPI shadowAPI;
#endif
    std::atomic<int> refCount;
    bool valid;
    bool shadowEnable;
    float shadowColor[3];
    float shadowDistance;
    float shadowFalloff;
    float shadowFalloffGamma;

    UsdLuxShadowAPIOpaque() : refCount(1), valid(false), shadowEnable(true),
        shadowDistance(-1.0f), shadowFalloff(-1.0f), shadowFalloffGamma(1.0f) {
        shadowColor[0] = shadowColor[1] = shadowColor[2] = 0.0f;
    }

#if USD_USE_FULL
    explicit UsdLuxShadowAPIOpaque(const UsdLuxShadowAPI& s)
        : shadowAPI(s), refCount(1), valid(true), shadowEnable(true),
          shadowDistance(-1.0f), shadowFalloff(-1.0f), shadowFalloffGamma(1.0f) {
        shadowColor[0] = shadowColor[1] = shadowColor[2] = 0.0f;
    }
#endif
};

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - UsdLuxDistantLight Implementation
// ============================================================================

extern "C" {

UsdLuxDistantLightRef UsdLuxDistantLight_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxDistantLight light = UsdLuxDistantLight::Define(stage->stage, path->path);
        if (light) {
            return new UsdLuxDistantLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxDistantLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxDistantLightRef UsdLuxDistantLight_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxDistantLight light(prim->prim);
        if (light) {
            return new UsdLuxDistantLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxDistantLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxDistantLightRef UsdLuxDistantLight_Retain(UsdLuxDistantLightRef light) {
    if (!light) return nullptr;
    light->refCount.fetch_add(1, std::memory_order_relaxed);
    return light;
}

void UsdLuxDistantLight_Release(UsdLuxDistantLightRef light) {
    if (!light) return;
    if (light->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete light;
    }
}

bool UsdLuxDistantLight_IsValid(UsdLuxDistantLightRef light) {
    if (!light) return false;
#if USD_USE_FULL
    return light->light;
#else
    return light->valid;
#endif
}

UsdPrimRef UsdLuxDistantLight_GetPrim(UsdLuxDistantLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = light->light.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

float UsdLuxDistantLight_GetAngle(UsdLuxDistantLightRef light, UsdTimeCode time) {
    if (!light) return 0.53f;
#if USD_USE_FULL
    float angle = 0.53f;
    light->light.GetAngleAttr().Get(&angle,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return angle;
#else
    return light->angle;
#endif
}

UsdResult UsdLuxDistantLight_SetAngle(UsdLuxDistantLightRef light, UsdTimeCode time, float angle) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetAngleAttr().Set(angle,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->angle = angle;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDistantLight_GetIntensity(UsdLuxDistantLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float intensity = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetIntensityAttr().Get(&intensity,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return intensity;
#else
    return light->intensity;
#endif
}

UsdResult UsdLuxDistantLight_SetIntensity(UsdLuxDistantLightRef light, UsdTimeCode time, float intensity) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetIntensityAttr().Set(intensity,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->intensity = intensity;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxDistantLight_GetColor(UsdLuxDistantLightRef light, UsdTimeCode time, float* outColor) {
    if (!light || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(1.0f, 1.0f, 1.0f);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = light->color[0];
    outColor[1] = light->color[1];
    outColor[2] = light->color[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxDistantLight_SetColor(UsdLuxDistantLightRef light, UsdTimeCode time, const float* color) {
    if (!light || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->color[0] = color[0];
    light->color[1] = color[1];
    light->color[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDistantLight_GetExposure(UsdLuxDistantLightRef light, UsdTimeCode time) {
    if (!light) return 0.0f;
#if USD_USE_FULL
    float exposure = 0.0f;
    UsdLuxLightAPI api(light->light);
    api.GetExposureAttr().Get(&exposure,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return exposure;
#else
    return light->exposure;
#endif
}

UsdResult UsdLuxDistantLight_SetExposure(UsdLuxDistantLightRef light, UsdTimeCode time, float exposure) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetExposureAttr().Set(exposure,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->exposure = exposure;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDistantLight_GetDiffuse(UsdLuxDistantLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float diffuse = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetDiffuseAttr().Get(&diffuse,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return diffuse;
#else
    return light->diffuse;
#endif
}

UsdResult UsdLuxDistantLight_SetDiffuse(UsdLuxDistantLightRef light, UsdTimeCode time, float diffuse) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetDiffuseAttr().Set(diffuse,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->diffuse = diffuse;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDistantLight_GetSpecular(UsdLuxDistantLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float specular = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetSpecularAttr().Get(&specular,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return specular;
#else
    return light->specular;
#endif
}

UsdResult UsdLuxDistantLight_SetSpecular(UsdLuxDistantLightRef light, UsdTimeCode time, float specular) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetSpecularAttr().Set(specular,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->specular = specular;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxDistantLight_GetNormalize(UsdLuxDistantLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool normalize = false;
    UsdLuxLightAPI api(light->light);
    api.GetNormalizeAttr().Get(&normalize,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return normalize;
#else
    return light->normalize;
#endif
}

UsdResult UsdLuxDistantLight_SetNormalize(UsdLuxDistantLightRef light, UsdTimeCode time, bool normalize) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetNormalizeAttr().Set(normalize,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->normalize = normalize;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxDomeLight Implementation
// ============================================================================

UsdLuxDomeLightRef UsdLuxDomeLight_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxDomeLight light = UsdLuxDomeLight::Define(stage->stage, path->path);
        if (light) {
            return new UsdLuxDomeLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxDomeLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxDomeLightRef UsdLuxDomeLight_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxDomeLight light(prim->prim);
        if (light) {
            return new UsdLuxDomeLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxDomeLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxDomeLightRef UsdLuxDomeLight_Retain(UsdLuxDomeLightRef light) {
    if (!light) return nullptr;
    light->refCount.fetch_add(1, std::memory_order_relaxed);
    return light;
}

void UsdLuxDomeLight_Release(UsdLuxDomeLightRef light) {
    if (!light) return;
    if (light->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete light;
    }
}

bool UsdLuxDomeLight_IsValid(UsdLuxDomeLightRef light) {
    if (!light) return false;
#if USD_USE_FULL
    return light->light;
#else
    return light->valid;
#endif
}

UsdPrimRef UsdLuxDomeLight_GetPrim(UsdLuxDomeLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = light->light.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

char* UsdLuxDomeLight_GetTextureFile(UsdLuxDomeLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        SdfAssetPath path;
        light->light.GetTextureFileAttr().Get(&path);
        std::string pathStr = path.GetAssetPath();
        char* result = static_cast<char*>(malloc(pathStr.size() + 1));
        if (result) {
            strcpy(result, pathStr.c_str());
        }
        return result;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(light->textureFile.size() + 1));
    if (result) {
        strcpy(result, light->textureFile.c_str());
    }
    return result;
#endif
}

UsdResult UsdLuxDomeLight_SetTextureFile(UsdLuxDomeLightRef light, const char* path) {
    if (!light || !path) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetTextureFileAttr().Set(SdfAssetPath(path));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->textureFile = path;
    return USD_RESULT_SUCCESS;
#endif
}

char* UsdLuxDomeLight_GetTextureFormat(UsdLuxDomeLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        TfToken format;
        light->light.GetTextureFormatAttr().Get(&format);
        std::string formatStr = format.GetString();
        char* result = static_cast<char*>(malloc(formatStr.size() + 1));
        if (result) {
            strcpy(result, formatStr.c_str());
        }
        return result;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(light->textureFormat.size() + 1));
    if (result) {
        strcpy(result, light->textureFormat.c_str());
    }
    return result;
#endif
}

UsdResult UsdLuxDomeLight_SetTextureFormat(UsdLuxDomeLightRef light, const char* format) {
    if (!light || !format) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetTextureFormatAttr().Set(TfToken(format));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->textureFormat = format;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDomeLight_GetGuideRadius(UsdLuxDomeLightRef light, UsdTimeCode time) {
    if (!light) return 100000.0f;
#if USD_USE_FULL
    float radius = 100000.0f;
    light->light.GetGuideRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return light->guideRadius;
#endif
}

UsdResult UsdLuxDomeLight_SetGuideRadius(UsdLuxDomeLightRef light, UsdTimeCode time, float radius) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetGuideRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->guideRadius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDomeLight_GetIntensity(UsdLuxDomeLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float intensity = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetIntensityAttr().Get(&intensity,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return intensity;
#else
    return light->intensity;
#endif
}

UsdResult UsdLuxDomeLight_SetIntensity(UsdLuxDomeLightRef light, UsdTimeCode time, float intensity) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetIntensityAttr().Set(intensity,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->intensity = intensity;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxDomeLight_GetColor(UsdLuxDomeLightRef light, UsdTimeCode time, float* outColor) {
    if (!light || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(1.0f, 1.0f, 1.0f);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = light->color[0];
    outColor[1] = light->color[1];
    outColor[2] = light->color[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxDomeLight_SetColor(UsdLuxDomeLightRef light, UsdTimeCode time, const float* color) {
    if (!light || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->color[0] = color[0];
    light->color[1] = color[1];
    light->color[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDomeLight_GetExposure(UsdLuxDomeLightRef light, UsdTimeCode time) {
    if (!light) return 0.0f;
#if USD_USE_FULL
    float exposure = 0.0f;
    UsdLuxLightAPI api(light->light);
    api.GetExposureAttr().Get(&exposure,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return exposure;
#else
    return light->exposure;
#endif
}

UsdResult UsdLuxDomeLight_SetExposure(UsdLuxDomeLightRef light, UsdTimeCode time, float exposure) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetExposureAttr().Set(exposure,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->exposure = exposure;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxRectLight Implementation
// ============================================================================

UsdLuxRectLightRef UsdLuxRectLight_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxRectLight light = UsdLuxRectLight::Define(stage->stage, path->path);
        if (light) {
            return new UsdLuxRectLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxRectLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxRectLightRef UsdLuxRectLight_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxRectLight light(prim->prim);
        if (light) {
            return new UsdLuxRectLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxRectLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxRectLightRef UsdLuxRectLight_Retain(UsdLuxRectLightRef light) {
    if (!light) return nullptr;
    light->refCount.fetch_add(1, std::memory_order_relaxed);
    return light;
}

void UsdLuxRectLight_Release(UsdLuxRectLightRef light) {
    if (!light) return;
    if (light->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete light;
    }
}

bool UsdLuxRectLight_IsValid(UsdLuxRectLightRef light) {
    if (!light) return false;
#if USD_USE_FULL
    return light->light;
#else
    return light->valid;
#endif
}

UsdPrimRef UsdLuxRectLight_GetPrim(UsdLuxRectLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = light->light.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

float UsdLuxRectLight_GetWidth(UsdLuxRectLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float width = 1.0f;
    light->light.GetWidthAttr().Get(&width,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return width;
#else
    return light->width;
#endif
}

UsdResult UsdLuxRectLight_SetWidth(UsdLuxRectLightRef light, UsdTimeCode time, float width) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetWidthAttr().Set(width,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->width = width;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxRectLight_GetHeight(UsdLuxRectLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float height = 1.0f;
    light->light.GetHeightAttr().Get(&height,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return height;
#else
    return light->height;
#endif
}

UsdResult UsdLuxRectLight_SetHeight(UsdLuxRectLightRef light, UsdTimeCode time, float height) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetHeightAttr().Set(height,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->height = height;
    return USD_RESULT_SUCCESS;
#endif
}

char* UsdLuxRectLight_GetTextureFile(UsdLuxRectLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        SdfAssetPath path;
        light->light.GetTextureFileAttr().Get(&path);
        std::string pathStr = path.GetAssetPath();
        char* result = static_cast<char*>(malloc(pathStr.size() + 1));
        if (result) {
            strcpy(result, pathStr.c_str());
        }
        return result;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(light->textureFile.size() + 1));
    if (result) {
        strcpy(result, light->textureFile.c_str());
    }
    return result;
#endif
}

UsdResult UsdLuxRectLight_SetTextureFile(UsdLuxRectLightRef light, const char* path) {
    if (!light || !path) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetTextureFileAttr().Set(SdfAssetPath(path));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->textureFile = path;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxRectLight_GetIntensity(UsdLuxRectLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float intensity = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetIntensityAttr().Get(&intensity,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return intensity;
#else
    return light->intensity;
#endif
}

UsdResult UsdLuxRectLight_SetIntensity(UsdLuxRectLightRef light, UsdTimeCode time, float intensity) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetIntensityAttr().Set(intensity,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->intensity = intensity;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxRectLight_GetColor(UsdLuxRectLightRef light, UsdTimeCode time, float* outColor) {
    if (!light || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(1.0f, 1.0f, 1.0f);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = light->color[0];
    outColor[1] = light->color[1];
    outColor[2] = light->color[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxRectLight_SetColor(UsdLuxRectLightRef light, UsdTimeCode time, const float* color) {
    if (!light || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->color[0] = color[0];
    light->color[1] = color[1];
    light->color[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxRectLight_GetExposure(UsdLuxRectLightRef light, UsdTimeCode time) {
    if (!light) return 0.0f;
#if USD_USE_FULL
    float exposure = 0.0f;
    UsdLuxLightAPI api(light->light);
    api.GetExposureAttr().Get(&exposure,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return exposure;
#else
    return light->exposure;
#endif
}

UsdResult UsdLuxRectLight_SetExposure(UsdLuxRectLightRef light, UsdTimeCode time, float exposure) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetExposureAttr().Set(exposure,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->exposure = exposure;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxRectLight_GetNormalize(UsdLuxRectLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool normalize = false;
    UsdLuxLightAPI api(light->light);
    api.GetNormalizeAttr().Get(&normalize,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return normalize;
#else
    return light->normalize;
#endif
}

UsdResult UsdLuxRectLight_SetNormalize(UsdLuxRectLightRef light, UsdTimeCode time, bool normalize) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetNormalizeAttr().Set(normalize,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->normalize = normalize;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxSphereLight Implementation
// ============================================================================

UsdLuxSphereLightRef UsdLuxSphereLight_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxSphereLight light = UsdLuxSphereLight::Define(stage->stage, path->path);
        if (light) {
            return new UsdLuxSphereLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxSphereLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxSphereLightRef UsdLuxSphereLight_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxSphereLight light(prim->prim);
        if (light) {
            return new UsdLuxSphereLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxSphereLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxSphereLightRef UsdLuxSphereLight_Retain(UsdLuxSphereLightRef light) {
    if (!light) return nullptr;
    light->refCount.fetch_add(1, std::memory_order_relaxed);
    return light;
}

void UsdLuxSphereLight_Release(UsdLuxSphereLightRef light) {
    if (!light) return;
    if (light->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete light;
    }
}

bool UsdLuxSphereLight_IsValid(UsdLuxSphereLightRef light) {
    if (!light) return false;
#if USD_USE_FULL
    return light->light;
#else
    return light->valid;
#endif
}

UsdPrimRef UsdLuxSphereLight_GetPrim(UsdLuxSphereLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = light->light.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

float UsdLuxSphereLight_GetRadius(UsdLuxSphereLightRef light, UsdTimeCode time) {
    if (!light) return 0.5f;
#if USD_USE_FULL
    float radius = 0.5f;
    light->light.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return light->radius;
#endif
}

UsdResult UsdLuxSphereLight_SetRadius(UsdLuxSphereLightRef light, UsdTimeCode time, float radius) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxSphereLight_GetTreatAsPoint(UsdLuxSphereLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool treatAsPoint = false;
    light->light.GetTreatAsPointAttr().Get(&treatAsPoint,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return treatAsPoint;
#else
    return light->treatAsPoint;
#endif
}

UsdResult UsdLuxSphereLight_SetTreatAsPoint(UsdLuxSphereLightRef light, UsdTimeCode time, bool treatAsPoint) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetTreatAsPointAttr().Set(treatAsPoint,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->treatAsPoint = treatAsPoint;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxSphereLight_GetIntensity(UsdLuxSphereLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float intensity = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetIntensityAttr().Get(&intensity,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return intensity;
#else
    return light->intensity;
#endif
}

UsdResult UsdLuxSphereLight_SetIntensity(UsdLuxSphereLightRef light, UsdTimeCode time, float intensity) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetIntensityAttr().Set(intensity,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->intensity = intensity;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxSphereLight_GetColor(UsdLuxSphereLightRef light, UsdTimeCode time, float* outColor) {
    if (!light || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(1.0f, 1.0f, 1.0f);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = light->color[0];
    outColor[1] = light->color[1];
    outColor[2] = light->color[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxSphereLight_SetColor(UsdLuxSphereLightRef light, UsdTimeCode time, const float* color) {
    if (!light || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->color[0] = color[0];
    light->color[1] = color[1];
    light->color[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxSphereLight_GetExposure(UsdLuxSphereLightRef light, UsdTimeCode time) {
    if (!light) return 0.0f;
#if USD_USE_FULL
    float exposure = 0.0f;
    UsdLuxLightAPI api(light->light);
    api.GetExposureAttr().Get(&exposure,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return exposure;
#else
    return light->exposure;
#endif
}

UsdResult UsdLuxSphereLight_SetExposure(UsdLuxSphereLightRef light, UsdTimeCode time, float exposure) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetExposureAttr().Set(exposure,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->exposure = exposure;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxSphereLight_GetNormalize(UsdLuxSphereLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool normalize = false;
    UsdLuxLightAPI api(light->light);
    api.GetNormalizeAttr().Get(&normalize,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return normalize;
#else
    return light->normalize;
#endif
}

UsdResult UsdLuxSphereLight_SetNormalize(UsdLuxSphereLightRef light, UsdTimeCode time, bool normalize) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetNormalizeAttr().Set(normalize,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->normalize = normalize;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxCylinderLight Implementation
// ============================================================================

UsdLuxCylinderLightRef UsdLuxCylinderLight_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxCylinderLight light = UsdLuxCylinderLight::Define(stage->stage, path->path);
        if (light) {
            return new UsdLuxCylinderLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxCylinderLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxCylinderLightRef UsdLuxCylinderLight_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxCylinderLight light(prim->prim);
        if (light) {
            return new UsdLuxCylinderLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxCylinderLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxCylinderLightRef UsdLuxCylinderLight_Retain(UsdLuxCylinderLightRef light) {
    if (!light) return nullptr;
    light->refCount.fetch_add(1, std::memory_order_relaxed);
    return light;
}

void UsdLuxCylinderLight_Release(UsdLuxCylinderLightRef light) {
    if (!light) return;
    if (light->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete light;
    }
}

bool UsdLuxCylinderLight_IsValid(UsdLuxCylinderLightRef light) {
    if (!light) return false;
#if USD_USE_FULL
    return light->light;
#else
    return light->valid;
#endif
}

UsdPrimRef UsdLuxCylinderLight_GetPrim(UsdLuxCylinderLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = light->light.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

float UsdLuxCylinderLight_GetLength(UsdLuxCylinderLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float length = 1.0f;
    light->light.GetLengthAttr().Get(&length,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return length;
#else
    return light->length;
#endif
}

UsdResult UsdLuxCylinderLight_SetLength(UsdLuxCylinderLightRef light, UsdTimeCode time, float length) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetLengthAttr().Set(length,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->length = length;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxCylinderLight_GetRadius(UsdLuxCylinderLightRef light, UsdTimeCode time) {
    if (!light) return 0.5f;
#if USD_USE_FULL
    float radius = 0.5f;
    light->light.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return light->radius;
#endif
}

UsdResult UsdLuxCylinderLight_SetRadius(UsdLuxCylinderLightRef light, UsdTimeCode time, float radius) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxCylinderLight_GetTreatAsLine(UsdLuxCylinderLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool treatAsLine = false;
    light->light.GetTreatAsLineAttr().Get(&treatAsLine,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return treatAsLine;
#else
    return light->treatAsLine;
#endif
}

UsdResult UsdLuxCylinderLight_SetTreatAsLine(UsdLuxCylinderLightRef light, UsdTimeCode time, bool treatAsLine) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetTreatAsLineAttr().Set(treatAsLine,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->treatAsLine = treatAsLine;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxCylinderLight_GetIntensity(UsdLuxCylinderLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float intensity = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetIntensityAttr().Get(&intensity,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return intensity;
#else
    return light->intensity;
#endif
}

UsdResult UsdLuxCylinderLight_SetIntensity(UsdLuxCylinderLightRef light, UsdTimeCode time, float intensity) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetIntensityAttr().Set(intensity,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->intensity = intensity;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxCylinderLight_GetColor(UsdLuxCylinderLightRef light, UsdTimeCode time, float* outColor) {
    if (!light || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(1.0f, 1.0f, 1.0f);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = light->color[0];
    outColor[1] = light->color[1];
    outColor[2] = light->color[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxCylinderLight_SetColor(UsdLuxCylinderLightRef light, UsdTimeCode time, const float* color) {
    if (!light || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->color[0] = color[0];
    light->color[1] = color[1];
    light->color[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxCylinderLight_GetExposure(UsdLuxCylinderLightRef light, UsdTimeCode time) {
    if (!light) return 0.0f;
#if USD_USE_FULL
    float exposure = 0.0f;
    UsdLuxLightAPI api(light->light);
    api.GetExposureAttr().Get(&exposure,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return exposure;
#else
    return light->exposure;
#endif
}

UsdResult UsdLuxCylinderLight_SetExposure(UsdLuxCylinderLightRef light, UsdTimeCode time, float exposure) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetExposureAttr().Set(exposure,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->exposure = exposure;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxCylinderLight_GetNormalize(UsdLuxCylinderLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool normalize = false;
    UsdLuxLightAPI api(light->light);
    api.GetNormalizeAttr().Get(&normalize,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return normalize;
#else
    return light->normalize;
#endif
}

UsdResult UsdLuxCylinderLight_SetNormalize(UsdLuxCylinderLightRef light, UsdTimeCode time, bool normalize) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetNormalizeAttr().Set(normalize,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->normalize = normalize;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxDiskLight Implementation
// ============================================================================

UsdLuxDiskLightRef UsdLuxDiskLight_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxDiskLight light = UsdLuxDiskLight::Define(stage->stage, path->path);
        if (light) {
            return new UsdLuxDiskLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxDiskLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxDiskLightRef UsdLuxDiskLight_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxDiskLight light(prim->prim);
        if (light) {
            return new UsdLuxDiskLightOpaque(light);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxDiskLightOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxDiskLightRef UsdLuxDiskLight_Retain(UsdLuxDiskLightRef light) {
    if (!light) return nullptr;
    light->refCount.fetch_add(1, std::memory_order_relaxed);
    return light;
}

void UsdLuxDiskLight_Release(UsdLuxDiskLightRef light) {
    if (!light) return;
    if (light->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete light;
    }
}

bool UsdLuxDiskLight_IsValid(UsdLuxDiskLightRef light) {
    if (!light) return false;
#if USD_USE_FULL
    return light->light;
#else
    return light->valid;
#endif
}

UsdPrimRef UsdLuxDiskLight_GetPrim(UsdLuxDiskLightRef light) {
    if (!light) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = light->light.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

float UsdLuxDiskLight_GetRadius(UsdLuxDiskLightRef light, UsdTimeCode time) {
    if (!light) return 0.5f;
#if USD_USE_FULL
    float radius = 0.5f;
    light->light.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return light->radius;
#endif
}

UsdResult UsdLuxDiskLight_SetRadius(UsdLuxDiskLightRef light, UsdTimeCode time, float radius) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        light->light.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDiskLight_GetIntensity(UsdLuxDiskLightRef light, UsdTimeCode time) {
    if (!light) return 1.0f;
#if USD_USE_FULL
    float intensity = 1.0f;
    UsdLuxLightAPI api(light->light);
    api.GetIntensityAttr().Get(&intensity,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return intensity;
#else
    return light->intensity;
#endif
}

UsdResult UsdLuxDiskLight_SetIntensity(UsdLuxDiskLightRef light, UsdTimeCode time, float intensity) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetIntensityAttr().Set(intensity,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->intensity = intensity;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxDiskLight_GetColor(UsdLuxDiskLightRef light, UsdTimeCode time, float* outColor) {
    if (!light || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(1.0f, 1.0f, 1.0f);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = light->color[0];
    outColor[1] = light->color[1];
    outColor[2] = light->color[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxDiskLight_SetColor(UsdLuxDiskLightRef light, UsdTimeCode time, const float* color) {
    if (!light || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        UsdLuxLightAPI api(light->light);
        api.GetColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->color[0] = color[0];
    light->color[1] = color[1];
    light->color[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxDiskLight_GetExposure(UsdLuxDiskLightRef light, UsdTimeCode time) {
    if (!light) return 0.0f;
#if USD_USE_FULL
    float exposure = 0.0f;
    UsdLuxLightAPI api(light->light);
    api.GetExposureAttr().Get(&exposure,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return exposure;
#else
    return light->exposure;
#endif
}

UsdResult UsdLuxDiskLight_SetExposure(UsdLuxDiskLightRef light, UsdTimeCode time, float exposure) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetExposureAttr().Set(exposure,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->exposure = exposure;
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdLuxDiskLight_GetNormalize(UsdLuxDiskLightRef light, UsdTimeCode time) {
    if (!light) return false;
#if USD_USE_FULL
    bool normalize = false;
    UsdLuxLightAPI api(light->light);
    api.GetNormalizeAttr().Get(&normalize,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return normalize;
#else
    return light->normalize;
#endif
}

UsdResult UsdLuxDiskLight_SetNormalize(UsdLuxDiskLightRef light, UsdTimeCode time, bool normalize) {
    if (!light) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdLuxLightAPI api(light->light);
        api.GetNormalizeAttr().Set(normalize,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    light->normalize = normalize;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxLightFilter Implementation
// ============================================================================

UsdLuxLightFilterRef UsdLuxLightFilter_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxLightFilter filter = UsdLuxLightFilter::Define(stage->stage, path->path);
        if (filter) {
            return new UsdLuxLightFilterOpaque(filter);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxLightFilterOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxLightFilterRef UsdLuxLightFilter_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxLightFilter filter(prim->prim);
        if (filter) {
            return new UsdLuxLightFilterOpaque(filter);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxLightFilterOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxLightFilterRef UsdLuxLightFilter_Retain(UsdLuxLightFilterRef filter) {
    if (!filter) return nullptr;
    filter->refCount.fetch_add(1, std::memory_order_relaxed);
    return filter;
}

void UsdLuxLightFilter_Release(UsdLuxLightFilterRef filter) {
    if (!filter) return;
    if (filter->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete filter;
    }
}

bool UsdLuxLightFilter_IsValid(UsdLuxLightFilterRef filter) {
    if (!filter) return false;
#if USD_USE_FULL
    return filter->filter;
#else
    return filter->valid;
#endif
}

UsdPrimRef UsdLuxLightFilter_GetPrim(UsdLuxLightFilterRef filter) {
    if (!filter) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = filter->filter.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

char* UsdLuxLightFilter_GetShaderId(UsdLuxLightFilterRef filter) {
    if (!filter) return nullptr;
#if USD_USE_FULL
    try {
        TfToken shaderId;
        filter->filter.GetShaderIdAttr().Get(&shaderId);
        std::string idStr = shaderId.GetString();
        char* result = static_cast<char*>(malloc(idStr.size() + 1));
        if (result) {
            strcpy(result, idStr.c_str());
        }
        return result;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(filter->shaderId.size() + 1));
    if (result) {
        strcpy(result, filter->shaderId.c_str());
    }
    return result;
#endif
}

UsdResult UsdLuxLightFilter_SetShaderId(UsdLuxLightFilterRef filter, const char* shaderId) {
    if (!filter || !shaderId) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        filter->filter.GetShaderIdAttr().Set(TfToken(shaderId));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    filter->shaderId = shaderId;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdLuxShadowAPI Implementation
// ============================================================================

UsdLuxShadowAPIRef UsdLuxShadowAPI_Apply(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxShadowAPI shadowAPI = UsdLuxShadowAPI::Apply(prim->prim);
        if (shadowAPI) {
            return new UsdLuxShadowAPIOpaque(shadowAPI);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxShadowAPIOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxShadowAPIRef UsdLuxShadowAPI_Get(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdLuxShadowAPI shadowAPI(prim->prim);
        if (shadowAPI) {
            return new UsdLuxShadowAPIOpaque(shadowAPI);
        }
        return nullptr;
#else
        auto* wrapper = new UsdLuxShadowAPIOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdLuxShadowAPIRef UsdLuxShadowAPI_Retain(UsdLuxShadowAPIRef shadow) {
    if (!shadow) return nullptr;
    shadow->refCount.fetch_add(1, std::memory_order_relaxed);
    return shadow;
}

void UsdLuxShadowAPI_Release(UsdLuxShadowAPIRef shadow) {
    if (!shadow) return;
    if (shadow->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete shadow;
    }
}

bool UsdLuxShadowAPI_IsValid(UsdLuxShadowAPIRef shadow) {
    if (!shadow) return false;
#if USD_USE_FULL
    return shadow->shadowAPI;
#else
    return shadow->valid;
#endif
}

UsdPrimRef UsdLuxShadowAPI_GetPrim(UsdLuxShadowAPIRef shadow) {
    if (!shadow) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = shadow->shadowAPI.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

bool UsdLuxShadowAPI_GetShadowEnable(UsdLuxShadowAPIRef shadow, UsdTimeCode time) {
    if (!shadow) return true;
#if USD_USE_FULL
    bool enable = true;
    shadow->shadowAPI.GetShadowEnableAttr().Get(&enable,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return enable;
#else
    return shadow->shadowEnable;
#endif
}

UsdResult UsdLuxShadowAPI_SetShadowEnable(UsdLuxShadowAPIRef shadow, UsdTimeCode time, bool enable) {
    if (!shadow) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        shadow->shadowAPI.GetShadowEnableAttr().Set(enable,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    shadow->shadowEnable = enable;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxShadowAPI_GetShadowColor(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float* outColor) {
    if (!shadow || !outColor) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f color(0.0f, 0.0f, 0.0f);
        shadow->shadowAPI.GetShadowColorAttr().Get(&color,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        outColor[0] = color[0];
        outColor[1] = color[1];
        outColor[2] = color[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outColor[0] = shadow->shadowColor[0];
    outColor[1] = shadow->shadowColor[1];
    outColor[2] = shadow->shadowColor[2];
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdLuxShadowAPI_SetShadowColor(UsdLuxShadowAPIRef shadow, UsdTimeCode time, const float* color) {
    if (!shadow || !color) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec3f c(color[0], color[1], color[2]);
        shadow->shadowAPI.GetShadowColorAttr().Set(c,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    shadow->shadowColor[0] = color[0];
    shadow->shadowColor[1] = color[1];
    shadow->shadowColor[2] = color[2];
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxShadowAPI_GetShadowDistance(UsdLuxShadowAPIRef shadow, UsdTimeCode time) {
    if (!shadow) return -1.0f;
#if USD_USE_FULL
    float distance = -1.0f;
    shadow->shadowAPI.GetShadowDistanceAttr().Get(&distance,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return distance;
#else
    return shadow->shadowDistance;
#endif
}

UsdResult UsdLuxShadowAPI_SetShadowDistance(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float distance) {
    if (!shadow) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        shadow->shadowAPI.GetShadowDistanceAttr().Set(distance,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    shadow->shadowDistance = distance;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxShadowAPI_GetShadowFalloff(UsdLuxShadowAPIRef shadow, UsdTimeCode time) {
    if (!shadow) return -1.0f;
#if USD_USE_FULL
    float falloff = -1.0f;
    shadow->shadowAPI.GetShadowFalloffAttr().Get(&falloff,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return falloff;
#else
    return shadow->shadowFalloff;
#endif
}

UsdResult UsdLuxShadowAPI_SetShadowFalloff(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float falloff) {
    if (!shadow) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        shadow->shadowAPI.GetShadowFalloffAttr().Set(falloff,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    shadow->shadowFalloff = falloff;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdLuxShadowAPI_GetShadowFalloffGamma(UsdLuxShadowAPIRef shadow, UsdTimeCode time) {
    if (!shadow) return 1.0f;
#if USD_USE_FULL
    float gamma = 1.0f;
    shadow->shadowAPI.GetShadowFalloffGammaAttr().Get(&gamma,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return gamma;
#else
    return shadow->shadowFalloffGamma;
#endif
}

UsdResult UsdLuxShadowAPI_SetShadowFalloffGamma(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float gamma) {
    if (!shadow) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        shadow->shadowAPI.GetShadowFalloffGammaAttr().Set(gamma,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    shadow->shadowFalloffGamma = gamma;
    return USD_RESULT_SUCCESS;
#endif
}

} // extern "C"
