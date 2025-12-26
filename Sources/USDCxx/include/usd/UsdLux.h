// UsdLux.h - Lighting Schemas for SwiftUSD
// Mirrors: pxr/usd/usdLux/*.h (DistantLight, DomeLight, RectLight, SphereLight, CylinderLight, DiskLight, LightFilter, LightAPI, ShadowAPI)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Gf.h"
#include "../base/Tf.h"
#include "../base/Vt.h"
#include "Sdf.h"
#include "Usd.h"
#include "UsdGeom.h"
#include "UsdShade.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/usdLux/distantLight.h"
#include "pxr/usd/usdLux/domeLight.h"
#include "pxr/usd/usdLux/rectLight.h"
#include "pxr/usd/usdLux/sphereLight.h"
#include "pxr/usd/usdLux/cylinderLight.h"
#include "pxr/usd/usdLux/diskLight.h"
#include "pxr/usd/usdLux/lightFilter.h"
#include "pxr/usd/usdLux/lightAPI.h"
#include "pxr/usd/usdLux/shadowAPI.h"
#include "pxr/usd/usdLux/shapingAPI.h"
#include "pxr/usd/usdLux/tokens.h"
#endif

namespace swiftusd {

// Forward declarations
class LuxDistantLightRef;
class LuxDomeLightRef;
class LuxRectLightRef;
class LuxSphereLightRef;
class LuxCylinderLightRef;
class LuxDiskLightRef;
class LuxLightFilterRef;
class LuxLightAPIRef;
class LuxShadowAPIRef;
class LuxShapingAPIRef;

// ============================================================================
// MARK: - LuxLightAPIRef (UsdLuxLightAPI reference type)
// ============================================================================

/// API schema that imparts the quality of being a light onto a prim.
///
/// This is the base API for all light types, providing common attributes
/// like intensity, color, exposure, diffuse/specular multipliers, etc.
class LuxLightAPIRef {
public:
    // Factory methods
    static LuxLightAPIRef* Apply(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdLuxLightAPI::Apply(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new LuxLightAPIRef(api, prim->GetStage());
#else
        return new LuxLightAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    static LuxLightAPIRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdLuxLightAPI(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new LuxLightAPIRef(api, prim->GetStage());
#else
        return new LuxLightAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_api);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return nullptr;
        return new PrimRef(_api.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Intensity - scales brightness linearly (default 1.0)
    float GetIntensity(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 1.0f;
        float intensity = 1.0f;
        _api.GetIntensityAttr().Get(&intensity, time.ToUsd());
        return intensity;
#else
        return _intensity;
#endif
    }

    bool SetIntensity(float intensity, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetIntensityAttr().Set(intensity, time.ToUsd());
#else
        _intensity = intensity;
        return true;
#endif
    }

    // Exposure - scales brightness as power of 2 (default 0.0)
    float GetExposure(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 0.0f;
        float exposure = 0.0f;
        _api.GetExposureAttr().Get(&exposure, time.ToUsd());
        return exposure;
#else
        return _exposure;
#endif
    }

    bool SetExposure(float exposure, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetExposureAttr().Set(exposure, time.ToUsd());
#else
        _exposure = exposure;
        return true;
#endif
    }

    // Color - RGB color of emitted light (default white)
    bool GetColor(TimeCode time, Vec3f* outColor) const SWIFTUSD_NOEXCEPT {
        if (!outColor) return false;
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        PXR_NS::GfVec3f color(1.0f, 1.0f, 1.0f);
        if (!_api.GetColorAttr().Get(&color, time.ToUsd())) return false;
        *outColor = Vec3f(color[0], color[1], color[2]);
        return true;
#else
        *outColor = _color;
        return true;
#endif
    }

    bool SetColor(Vec3f color, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetColorAttr().Set(PXR_NS::GfVec3f(color.x, color.y, color.z), time.ToUsd());
#else
        _color = color;
        return true;
#endif
    }

    // Enable color temperature
    bool GetEnableColorTemperature(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        bool enable = false;
        _api.GetEnableColorTemperatureAttr().Get(&enable, time.ToUsd());
        return enable;
#else
        return _enableColorTemperature;
#endif
    }

    bool SetEnableColorTemperature(bool enable, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetEnableColorTemperatureAttr().Set(enable, time.ToUsd());
#else
        _enableColorTemperature = enable;
        return true;
#endif
    }

    // Color temperature in Kelvin (default 6500K)
    float GetColorTemperature(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 6500.0f;
        float temp = 6500.0f;
        _api.GetColorTemperatureAttr().Get(&temp, time.ToUsd());
        return temp;
#else
        return _colorTemperature;
#endif
    }

    bool SetColorTemperature(float temp, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetColorTemperatureAttr().Set(temp, time.ToUsd());
#else
        _colorTemperature = temp;
        return true;
#endif
    }

    // Diffuse multiplier (default 1.0)
    float GetDiffuse(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 1.0f;
        float diffuse = 1.0f;
        _api.GetDiffuseAttr().Get(&diffuse, time.ToUsd());
        return diffuse;
#else
        return _diffuse;
#endif
    }

    bool SetDiffuse(float diffuse, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetDiffuseAttr().Set(diffuse, time.ToUsd());
#else
        _diffuse = diffuse;
        return true;
#endif
    }

    // Specular multiplier (default 1.0)
    float GetSpecular(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 1.0f;
        float specular = 1.0f;
        _api.GetSpecularAttr().Get(&specular, time.ToUsd());
        return specular;
#else
        return _specular;
#endif
    }

    bool SetSpecular(float specular, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetSpecularAttr().Set(specular, time.ToUsd());
#else
        _specular = specular;
        return true;
#endif
    }

    // Normalize - normalizes emission so power is constant regardless of size
    bool GetNormalize(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        bool normalize = false;
        _api.GetNormalizeAttr().Get(&normalize, time.ToUsd());
        return normalize;
#else
        return _normalize;
#endif
    }

    bool SetNormalize(bool normalize, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetNormalizeAttr().Set(normalize, time.ToUsd());
#else
        _normalize = normalize;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxLightAPIRef(const PXR_NS::UsdLuxLightAPI& api, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _api(api), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxLightAPIRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxLightAPI _api;
#else
    LuxLightAPIRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _intensity(1.0f), _exposure(0.0f), _color(1.0f, 1.0f, 1.0f),
          _enableColorTemperature(false), _colorTemperature(6500.0f),
          _diffuse(1.0f), _specular(1.0f), _normalize(false),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxLightAPIRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _intensity;
    float _exposure;
    Vec3f _color;
    bool _enableColorTemperature;
    float _colorTemperature;
    float _diffuse;
    float _specular;
    bool _normalize;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxShadowAPIRef (UsdLuxShadowAPI reference type)
// ============================================================================

/// Controls to refine a light's shadow behavior.
class LuxShadowAPIRef {
public:
    // Factory methods
    static LuxShadowAPIRef* Apply(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdLuxShadowAPI::Apply(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new LuxShadowAPIRef(api, prim->GetStage());
#else
        return new LuxShadowAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    static LuxShadowAPIRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdLuxShadowAPI(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new LuxShadowAPIRef(api, prim->GetStage());
#else
        return new LuxShadowAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_api);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return nullptr;
        return new PrimRef(_api.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Shadow enable (default true)
    bool GetShadowEnable(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return true;
        bool enable = true;
        _api.GetShadowEnableAttr().Get(&enable, time.ToUsd());
        return enable;
#else
        return _shadowEnable;
#endif
    }

    bool SetShadowEnable(bool enable, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShadowEnableAttr().Set(enable, time.ToUsd());
#else
        _shadowEnable = enable;
        return true;
#endif
    }

    // Shadow color (default black)
    bool GetShadowColor(TimeCode time, Vec3f* outColor) const SWIFTUSD_NOEXCEPT {
        if (!outColor) return false;
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        PXR_NS::GfVec3f color(0.0f, 0.0f, 0.0f);
        if (!_api.GetShadowColorAttr().Get(&color, time.ToUsd())) return false;
        *outColor = Vec3f(color[0], color[1], color[2]);
        return true;
#else
        *outColor = _shadowColor;
        return true;
#endif
    }

    bool SetShadowColor(Vec3f color, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShadowColorAttr().Set(PXR_NS::GfVec3f(color.x, color.y, color.z), time.ToUsd());
#else
        _shadowColor = color;
        return true;
#endif
    }

    // Shadow distance (default -1 = no limit)
    float GetShadowDistance(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return -1.0f;
        float distance = -1.0f;
        _api.GetShadowDistanceAttr().Get(&distance, time.ToUsd());
        return distance;
#else
        return _shadowDistance;
#endif
    }

    bool SetShadowDistance(float distance, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShadowDistanceAttr().Set(distance, time.ToUsd());
#else
        _shadowDistance = distance;
        return true;
#endif
    }

    // Shadow falloff (default -1 = no falloff)
    float GetShadowFalloff(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return -1.0f;
        float falloff = -1.0f;
        _api.GetShadowFalloffAttr().Get(&falloff, time.ToUsd());
        return falloff;
#else
        return _shadowFalloff;
#endif
    }

    bool SetShadowFalloff(float falloff, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShadowFalloffAttr().Set(falloff, time.ToUsd());
#else
        _shadowFalloff = falloff;
        return true;
#endif
    }

    // Shadow falloff gamma (default 1.0)
    float GetShadowFalloffGamma(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 1.0f;
        float gamma = 1.0f;
        _api.GetShadowFalloffGammaAttr().Get(&gamma, time.ToUsd());
        return gamma;
#else
        return _shadowFalloffGamma;
#endif
    }

    bool SetShadowFalloffGamma(float gamma, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShadowFalloffGammaAttr().Set(gamma, time.ToUsd());
#else
        _shadowFalloffGamma = gamma;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxShadowAPIRef(const PXR_NS::UsdLuxShadowAPI& api, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _api(api), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxShadowAPIRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxShadowAPI _api;
#else
    LuxShadowAPIRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _shadowEnable(true), _shadowColor(0.0f, 0.0f, 0.0f),
          _shadowDistance(-1.0f), _shadowFalloff(-1.0f), _shadowFalloffGamma(1.0f),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxShadowAPIRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    bool _shadowEnable;
    Vec3f _shadowColor;
    float _shadowDistance;
    float _shadowFalloff;
    float _shadowFalloffGamma;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxShapingAPIRef (UsdLuxShapingAPI reference type)
// ============================================================================

/// Controls for shaping a light's emission cone (spot lights).
class LuxShapingAPIRef {
public:
    // Factory methods
    static LuxShapingAPIRef* Apply(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdLuxShapingAPI::Apply(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new LuxShapingAPIRef(api, prim->GetStage());
#else
        return new LuxShapingAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    static LuxShapingAPIRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdLuxShapingAPI(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new LuxShapingAPIRef(api, prim->GetStage());
#else
        return new LuxShapingAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_api);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return nullptr;
        return new PrimRef(_api.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Focus - beam concentration (default 0.0)
    float GetShapingFocus(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 0.0f;
        float focus = 0.0f;
        _api.GetShapingFocusAttr().Get(&focus, time.ToUsd());
        return focus;
#else
        return _focus;
#endif
    }

    bool SetShapingFocus(float focus, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShapingFocusAttr().Set(focus, time.ToUsd());
#else
        _focus = focus;
        return true;
#endif
    }

    // Focus tint - tint color at edges
    bool GetShapingFocusTint(TimeCode time, Vec3f* outColor) const SWIFTUSD_NOEXCEPT {
        if (!outColor) return false;
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        PXR_NS::GfVec3f color(0.0f, 0.0f, 0.0f);
        if (!_api.GetShapingFocusTintAttr().Get(&color, time.ToUsd())) return false;
        *outColor = Vec3f(color[0], color[1], color[2]);
        return true;
#else
        *outColor = _focusTint;
        return true;
#endif
    }

    bool SetShapingFocusTint(Vec3f color, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShapingFocusTintAttr().Set(PXR_NS::GfVec3f(color.x, color.y, color.z), time.ToUsd());
#else
        _focusTint = color;
        return true;
#endif
    }

    // Cone angle - angular extent of the cone (default 90.0)
    float GetShapingConeAngle(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 90.0f;
        float angle = 90.0f;
        _api.GetShapingConeAngleAttr().Get(&angle, time.ToUsd());
        return angle;
#else
        return _coneAngle;
#endif
    }

    bool SetShapingConeAngle(float angle, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShapingConeAngleAttr().Set(angle, time.ToUsd());
#else
        _coneAngle = angle;
        return true;
#endif
    }

    // Cone softness - edge softness (default 0.0)
    float GetShapingConeSoftness(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return 0.0f;
        float softness = 0.0f;
        _api.GetShapingConeSoftnessAttr().Get(&softness, time.ToUsd());
        return softness;
#else
        return _coneSoftness;
#endif
    }

    bool SetShapingConeSoftness(float softness, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_api) return false;
        return _api.GetShapingConeSoftnessAttr().Set(softness, time.ToUsd());
#else
        _coneSoftness = softness;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxShapingAPIRef(const PXR_NS::UsdLuxShapingAPI& api, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _api(api), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxShapingAPIRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxShapingAPI _api;
#else
    LuxShapingAPIRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _focus(0.0f), _focusTint(0.0f, 0.0f, 0.0f),
          _coneAngle(90.0f), _coneSoftness(0.0f),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxShapingAPIRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _focus;
    Vec3f _focusTint;
    float _coneAngle;
    float _coneSoftness;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxDistantLightRef (UsdLuxDistantLight reference type)
// ============================================================================

/// Light emitted from a distant source along the -Z axis.
/// Also known as a directional light.
class LuxDistantLightRef {
public:
    // Factory methods
    static LuxDistantLightRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxDistantLight::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!light) return nullptr;
        return new LuxDistantLightRef(light, stage);
#else
        // Define the prim in the stage first
        auto* prim = stage->DefinePrim(path, Token("DistantLight"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxDistantLightRef(path, stage);
#endif
    }

    static LuxDistantLightRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxDistantLight(prim->GetUsdPrim());
        if (!light) return nullptr;
        return new LuxDistantLightRef(light, prim->GetStage());
#else
        return new LuxDistantLightRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_light);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return nullptr;
        return new PrimRef(_light.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Angle - angular diameter in degrees (default 0.53 like the sun)
    float GetAngle(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 0.53f;
        float angle = 0.53f;
        _light.GetAngleAttr().Get(&angle, time.ToUsd());
        return angle;
#else
        return _angle;
#endif
    }

    bool SetAngle(float angle, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetAngleAttr().Set(angle, time.ToUsd());
#else
        _angle = angle;
        return true;
#endif
    }

    // LightAPI access
    LuxLightAPIRef* GetLightAPI() SWIFTUSD_NOEXCEPT {
        auto* prim = GetPrim();
        if (!prim) return nullptr;
        auto* api = LuxLightAPIRef::FromPrim(prim);
        prim->release();
        return api;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxDistantLightRef(const PXR_NS::UsdLuxDistantLight& light, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _light(light), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxDistantLightRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxDistantLight _light;
#else
    LuxDistantLightRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _angle(0.53f), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxDistantLightRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _angle;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxDomeLightRef (UsdLuxDomeLight reference type)
// ============================================================================

/// Light emitted inward from a distant external environment (IBL).
class LuxDomeLightRef {
public:
    // Factory methods
    static LuxDomeLightRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxDomeLight::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!light) return nullptr;
        return new LuxDomeLightRef(light, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("DomeLight"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxDomeLightRef(path, stage);
#endif
    }

    static LuxDomeLightRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxDomeLight(prim->GetUsdPrim());
        if (!light) return nullptr;
        return new LuxDomeLightRef(light, prim->GetStage());
#else
        return new LuxDomeLightRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_light);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return nullptr;
        return new PrimRef(_light.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Texture file - HDR environment map
    const char* GetTextureFile() const SWIFTUSD_NOEXCEPT SWIFT_RETURNS_INDEPENDENT_VALUE {
#if defined(USE_PIXAR_USD)
        if (!_light) return "";
        PXR_NS::SdfAssetPath path;
        _light.GetTextureFileAttr().Get(&path);
        _cachedTextureFile = path.GetAssetPath();
        return _cachedTextureFile.c_str();
#else
        return _textureFile.c_str();
#endif
    }

    bool SetTextureFile(const char* file, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        if (!file) return false;
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetTextureFileAttr().Set(PXR_NS::SdfAssetPath(file), time.ToUsd());
#else
        _textureFile = file;
        return true;
#endif
    }

    // Texture format - how the texture is parameterized
    Token GetTextureFormat(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return Token("automatic");
        PXR_NS::TfToken format;
        _light.GetTextureFormatAttr().Get(&format, time.ToUsd());
        return Token(format);
#else
        return _textureFormat;
#endif
    }

    bool SetTextureFormat(const Token& format, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetTextureFormatAttr().Set(format.GetTfToken(), time.ToUsd());
#else
        _textureFormat = format;
        return true;
#endif
    }

    // Guide radius for visualization
    float GetGuideRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 100000.0f;
        float radius = 100000.0f;
        _light.GetGuideRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _guideRadius;
#endif
    }

    bool SetGuideRadius(float radius, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetGuideRadiusAttr().Set(radius, time.ToUsd());
#else
        _guideRadius = radius;
        return true;
#endif
    }

    // LightAPI access
    LuxLightAPIRef* GetLightAPI() SWIFTUSD_NOEXCEPT {
        auto* prim = GetPrim();
        if (!prim) return nullptr;
        auto* api = LuxLightAPIRef::FromPrim(prim);
        prim->release();
        return api;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxDomeLightRef(const PXR_NS::UsdLuxDomeLight& light, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _light(light), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxDomeLightRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxDomeLight _light;
    mutable std::string _cachedTextureFile;
#else
    LuxDomeLightRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _textureFormat("automatic"), _guideRadius(100000.0f), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxDomeLightRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::string _textureFile;
    Token _textureFormat;
    float _guideRadius;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxRectLightRef (UsdLuxRectLight reference type)
// ============================================================================

/// Light emitted from one side of a rectangle.
class LuxRectLightRef {
public:
    // Factory methods
    static LuxRectLightRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxRectLight::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!light) return nullptr;
        return new LuxRectLightRef(light, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("RectLight"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxRectLightRef(path, stage);
#endif
    }

    static LuxRectLightRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxRectLight(prim->GetUsdPrim());
        if (!light) return nullptr;
        return new LuxRectLightRef(light, prim->GetStage());
#else
        return new LuxRectLightRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_light);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return nullptr;
        return new PrimRef(_light.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Width
    float GetWidth(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 1.0f;
        float width = 1.0f;
        _light.GetWidthAttr().Get(&width, time.ToUsd());
        return width;
#else
        return _width;
#endif
    }

    bool SetWidth(float width, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetWidthAttr().Set(width, time.ToUsd());
#else
        _width = width;
        return true;
#endif
    }

    // Height
    float GetHeight(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 1.0f;
        float height = 1.0f;
        _light.GetHeightAttr().Get(&height, time.ToUsd());
        return height;
#else
        return _height;
#endif
    }

    bool SetHeight(float height, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetHeightAttr().Set(height, time.ToUsd());
#else
        _height = height;
        return true;
#endif
    }

    // Texture file
    const char* GetTextureFile() const SWIFTUSD_NOEXCEPT SWIFT_RETURNS_INDEPENDENT_VALUE {
#if defined(USE_PIXAR_USD)
        if (!_light) return "";
        PXR_NS::SdfAssetPath path;
        _light.GetTextureFileAttr().Get(&path);
        _cachedTextureFile = path.GetAssetPath();
        return _cachedTextureFile.c_str();
#else
        return _textureFile.c_str();
#endif
    }

    bool SetTextureFile(const char* file, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        if (!file) return false;
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetTextureFileAttr().Set(PXR_NS::SdfAssetPath(file), time.ToUsd());
#else
        _textureFile = file;
        return true;
#endif
    }

    // LightAPI access
    LuxLightAPIRef* GetLightAPI() SWIFTUSD_NOEXCEPT {
        auto* prim = GetPrim();
        if (!prim) return nullptr;
        auto* api = LuxLightAPIRef::FromPrim(prim);
        prim->release();
        return api;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxRectLightRef(const PXR_NS::UsdLuxRectLight& light, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _light(light), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxRectLightRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxRectLight _light;
    mutable std::string _cachedTextureFile;
#else
    LuxRectLightRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _width(1.0f), _height(1.0f), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxRectLightRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _width;
    float _height;
    std::string _textureFile;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxSphereLightRef (UsdLuxSphereLight reference type)
// ============================================================================

/// Light emitted outward from a sphere.
class LuxSphereLightRef {
public:
    // Factory methods
    static LuxSphereLightRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxSphereLight::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!light) return nullptr;
        return new LuxSphereLightRef(light, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("SphereLight"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxSphereLightRef(path, stage);
#endif
    }

    static LuxSphereLightRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxSphereLight(prim->GetUsdPrim());
        if (!light) return nullptr;
        return new LuxSphereLightRef(light, prim->GetStage());
#else
        return new LuxSphereLightRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_light);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return nullptr;
        return new PrimRef(_light.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Radius
    float GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 0.5f;
        float radius = 0.5f;
        _light.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(float radius, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    // Treat as point
    bool GetTreatAsPoint(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        bool treatAsPoint = false;
        _light.GetTreatAsPointAttr().Get(&treatAsPoint, time.ToUsd());
        return treatAsPoint;
#else
        return _treatAsPoint;
#endif
    }

    bool SetTreatAsPoint(bool treatAsPoint, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetTreatAsPointAttr().Set(treatAsPoint, time.ToUsd());
#else
        _treatAsPoint = treatAsPoint;
        return true;
#endif
    }

    // LightAPI access
    LuxLightAPIRef* GetLightAPI() SWIFTUSD_NOEXCEPT {
        auto* prim = GetPrim();
        if (!prim) return nullptr;
        auto* api = LuxLightAPIRef::FromPrim(prim);
        prim->release();
        return api;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxSphereLightRef(const PXR_NS::UsdLuxSphereLight& light, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _light(light), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxSphereLightRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxSphereLight _light;
#else
    LuxSphereLightRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _radius(0.5f), _treatAsPoint(false), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxSphereLightRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _radius;
    bool _treatAsPoint;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxCylinderLightRef (UsdLuxCylinderLight reference type)
// ============================================================================

/// Light emitted outward from a cylinder.
class LuxCylinderLightRef {
public:
    // Factory methods
    static LuxCylinderLightRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxCylinderLight::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!light) return nullptr;
        return new LuxCylinderLightRef(light, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("CylinderLight"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxCylinderLightRef(path, stage);
#endif
    }

    static LuxCylinderLightRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxCylinderLight(prim->GetUsdPrim());
        if (!light) return nullptr;
        return new LuxCylinderLightRef(light, prim->GetStage());
#else
        return new LuxCylinderLightRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_light);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return nullptr;
        return new PrimRef(_light.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Length
    float GetLength(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 1.0f;
        float length = 1.0f;
        _light.GetLengthAttr().Get(&length, time.ToUsd());
        return length;
#else
        return _length;
#endif
    }

    bool SetLength(float length, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetLengthAttr().Set(length, time.ToUsd());
#else
        _length = length;
        return true;
#endif
    }

    // Radius
    float GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 0.5f;
        float radius = 0.5f;
        _light.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(float radius, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    // Treat as line
    bool GetTreatAsLine(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        bool treatAsLine = false;
        _light.GetTreatAsLineAttr().Get(&treatAsLine, time.ToUsd());
        return treatAsLine;
#else
        return _treatAsLine;
#endif
    }

    bool SetTreatAsLine(bool treatAsLine, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetTreatAsLineAttr().Set(treatAsLine, time.ToUsd());
#else
        _treatAsLine = treatAsLine;
        return true;
#endif
    }

    // LightAPI access
    LuxLightAPIRef* GetLightAPI() SWIFTUSD_NOEXCEPT {
        auto* prim = GetPrim();
        if (!prim) return nullptr;
        auto* api = LuxLightAPIRef::FromPrim(prim);
        prim->release();
        return api;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxCylinderLightRef(const PXR_NS::UsdLuxCylinderLight& light, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _light(light), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxCylinderLightRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxCylinderLight _light;
#else
    LuxCylinderLightRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _length(1.0f), _radius(0.5f), _treatAsLine(false), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxCylinderLightRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _length;
    float _radius;
    bool _treatAsLine;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxDiskLightRef (UsdLuxDiskLight reference type)
// ============================================================================

/// Light emitted from one side of a disk.
class LuxDiskLightRef {
public:
    // Factory methods
    static LuxDiskLightRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxDiskLight::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!light) return nullptr;
        return new LuxDiskLightRef(light, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("DiskLight"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxDiskLightRef(path, stage);
#endif
    }

    static LuxDiskLightRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto light = PXR_NS::UsdLuxDiskLight(prim->GetUsdPrim());
        if (!light) return nullptr;
        return new LuxDiskLightRef(light, prim->GetStage());
#else
        return new LuxDiskLightRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_light);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return nullptr;
        return new PrimRef(_light.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Radius
    float GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return 0.5f;
        float radius = 0.5f;
        _light.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(float radius, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_light) return false;
        return _light.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    // LightAPI access
    LuxLightAPIRef* GetLightAPI() SWIFTUSD_NOEXCEPT {
        auto* prim = GetPrim();
        if (!prim) return nullptr;
        auto* api = LuxLightAPIRef::FromPrim(prim);
        prim->release();
        return api;
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxDiskLightRef(const PXR_NS::UsdLuxDiskLight& light, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _light(light), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxDiskLightRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxDiskLight _light;
#else
    LuxDiskLightRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _radius(0.5f), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxDiskLightRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    float _radius;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - LuxLightFilterRef (UsdLuxLightFilter reference type)
// ============================================================================

/// A light filter modifies the effect of a light.
class LuxLightFilterRef {
public:
    // Factory methods
    static LuxLightFilterRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto filter = PXR_NS::UsdLuxLightFilter::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!filter) return nullptr;
        return new LuxLightFilterRef(filter, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("LightFilter"));
        if (!prim) return nullptr;
        prim->release();
        return new LuxLightFilterRef(path, stage);
#endif
    }

    static LuxLightFilterRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto filter = PXR_NS::UsdLuxLightFilter(prim->GetUsdPrim());
        if (!filter) return nullptr;
        return new LuxLightFilterRef(filter, prim->GetStage());
#else
        return new LuxLightFilterRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_filter);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_filter) return nullptr;
        return new PrimRef(_filter.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Shader ID
    Token GetShaderId() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_filter) return Token();
        PXR_NS::TfToken shaderId;
        _filter.GetShaderIdAttr().Get(&shaderId);
        return Token(shaderId);
#else
        return _shaderId;
#endif
    }

    bool SetShaderId(const Token& shaderId) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_filter) return false;
        return _filter.GetShaderIdAttr().Set(shaderId.GetTfToken());
#else
        _shaderId = shaderId;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    LuxLightFilterRef(const PXR_NS::UsdLuxLightFilter& filter, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _filter(filter), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxLightFilterRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdLuxLightFilter _filter;
#else
    LuxLightFilterRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~LuxLightFilterRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    Token _shaderId;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
