// usdLux.h - UsdLux module C wrapper
// Mirrors: pxr/usd/usdLux/*.h

#ifndef OPENUSDINTEROP_USDLUX_USDLUX_H
#define OPENUSDINTEROP_USDLUX_USDLUX_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Cross-module dependencies
#include "../base/tf.h"
#include "../base/gf.h"
#include "../usd/sdf.h"
#include "../usd/usd.h"

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

typedef struct UsdLuxDistantLightOpaque* UsdLuxDistantLightRef;
typedef struct UsdLuxDomeLightOpaque* UsdLuxDomeLightRef;
typedef struct UsdLuxRectLightOpaque* UsdLuxRectLightRef;
typedef struct UsdLuxSphereLightOpaque* UsdLuxSphereLightRef;
typedef struct UsdLuxCylinderLightOpaque* UsdLuxCylinderLightRef;
typedef struct UsdLuxDiskLightOpaque* UsdLuxDiskLightRef;
typedef struct UsdLuxLightFilterOpaque* UsdLuxLightFilterRef;
typedef struct UsdLuxShadowAPIOpaque* UsdLuxShadowAPIRef;

// ============================================================================
// MARK: - UsdLuxDistantLight (Directional Light)
// ============================================================================

/// Defines a new DistantLight prim at the given path.
/// Caller must release with UsdLuxDistantLight_Release.
USD_INTEROP_API UsdLuxDistantLightRef UsdLuxDistantLight_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxDistantLight_Release.
USD_INTEROP_API UsdLuxDistantLightRef UsdLuxDistantLight_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxDistantLightRef UsdLuxDistantLight_Retain(UsdLuxDistantLightRef light);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxDistantLight_Release(UsdLuxDistantLightRef light);

/// Returns true if the light is valid.
USD_INTEROP_API bool UsdLuxDistantLight_IsValid(UsdLuxDistantLightRef light);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxDistantLight_GetPrim(UsdLuxDistantLightRef light);

/// Gets the angular diameter in degrees (default 0.53, like the sun).
USD_INTEROP_API float UsdLuxDistantLight_GetAngle(UsdLuxDistantLightRef light, UsdTimeCode time);

/// Sets the angular diameter in degrees.
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetAngle(UsdLuxDistantLightRef light, UsdTimeCode time, float angle);

/// Gets the intensity.
USD_INTEROP_API float UsdLuxDistantLight_GetIntensity(UsdLuxDistantLightRef light, UsdTimeCode time);

/// Sets the intensity.
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetIntensity(UsdLuxDistantLightRef light, UsdTimeCode time, float intensity);

/// Gets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxDistantLight_GetColor(UsdLuxDistantLightRef light, UsdTimeCode time, float* outColor);

/// Sets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetColor(UsdLuxDistantLightRef light, UsdTimeCode time, const float* color);

/// Gets the exposure value.
USD_INTEROP_API float UsdLuxDistantLight_GetExposure(UsdLuxDistantLightRef light, UsdTimeCode time);

/// Sets the exposure value.
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetExposure(UsdLuxDistantLightRef light, UsdTimeCode time, float exposure);

/// Gets the diffuse multiplier.
USD_INTEROP_API float UsdLuxDistantLight_GetDiffuse(UsdLuxDistantLightRef light, UsdTimeCode time);

/// Sets the diffuse multiplier.
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetDiffuse(UsdLuxDistantLightRef light, UsdTimeCode time, float diffuse);

/// Gets the specular multiplier.
USD_INTEROP_API float UsdLuxDistantLight_GetSpecular(UsdLuxDistantLightRef light, UsdTimeCode time);

/// Sets the specular multiplier.
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetSpecular(UsdLuxDistantLightRef light, UsdTimeCode time, float specular);

/// Gets the normalize flag.
USD_INTEROP_API bool UsdLuxDistantLight_GetNormalize(UsdLuxDistantLightRef light, UsdTimeCode time);

/// Sets the normalize flag.
USD_INTEROP_API UsdResult UsdLuxDistantLight_SetNormalize(UsdLuxDistantLightRef light, UsdTimeCode time, bool normalize);

// ============================================================================
// MARK: - UsdLuxDomeLight (Environment/IBL Light)
// ============================================================================

/// Defines a new DomeLight prim at the given path.
/// Caller must release with UsdLuxDomeLight_Release.
USD_INTEROP_API UsdLuxDomeLightRef UsdLuxDomeLight_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxDomeLight_Release.
USD_INTEROP_API UsdLuxDomeLightRef UsdLuxDomeLight_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxDomeLightRef UsdLuxDomeLight_Retain(UsdLuxDomeLightRef light);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxDomeLight_Release(UsdLuxDomeLightRef light);

/// Returns true if the light is valid.
USD_INTEROP_API bool UsdLuxDomeLight_IsValid(UsdLuxDomeLightRef light);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxDomeLight_GetPrim(UsdLuxDomeLightRef light);

/// Gets the texture file path.
/// Returns a newly allocated string that must be freed with free().
USD_INTEROP_API char* UsdLuxDomeLight_GetTextureFile(UsdLuxDomeLightRef light);

/// Sets the texture file path.
USD_INTEROP_API UsdResult UsdLuxDomeLight_SetTextureFile(UsdLuxDomeLightRef light, const char* path);

/// Gets the texture format as a string token.
/// Returns a newly allocated string that must be freed with free().
USD_INTEROP_API char* UsdLuxDomeLight_GetTextureFormat(UsdLuxDomeLightRef light);

/// Sets the texture format (automatic, latlong, mirroredBall, angular, cubeMapVerticalCross).
USD_INTEROP_API UsdResult UsdLuxDomeLight_SetTextureFormat(UsdLuxDomeLightRef light, const char* format);

/// Gets the guide radius for visualization.
USD_INTEROP_API float UsdLuxDomeLight_GetGuideRadius(UsdLuxDomeLightRef light, UsdTimeCode time);

/// Sets the guide radius for visualization.
USD_INTEROP_API UsdResult UsdLuxDomeLight_SetGuideRadius(UsdLuxDomeLightRef light, UsdTimeCode time, float radius);

/// Gets the intensity.
USD_INTEROP_API float UsdLuxDomeLight_GetIntensity(UsdLuxDomeLightRef light, UsdTimeCode time);

/// Sets the intensity.
USD_INTEROP_API UsdResult UsdLuxDomeLight_SetIntensity(UsdLuxDomeLightRef light, UsdTimeCode time, float intensity);

/// Gets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxDomeLight_GetColor(UsdLuxDomeLightRef light, UsdTimeCode time, float* outColor);

/// Sets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxDomeLight_SetColor(UsdLuxDomeLightRef light, UsdTimeCode time, const float* color);

/// Gets the exposure value.
USD_INTEROP_API float UsdLuxDomeLight_GetExposure(UsdLuxDomeLightRef light, UsdTimeCode time);

/// Sets the exposure value.
USD_INTEROP_API UsdResult UsdLuxDomeLight_SetExposure(UsdLuxDomeLightRef light, UsdTimeCode time, float exposure);

// ============================================================================
// MARK: - UsdLuxRectLight (Area Light)
// ============================================================================

/// Defines a new RectLight prim at the given path.
/// Caller must release with UsdLuxRectLight_Release.
USD_INTEROP_API UsdLuxRectLightRef UsdLuxRectLight_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxRectLight_Release.
USD_INTEROP_API UsdLuxRectLightRef UsdLuxRectLight_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxRectLightRef UsdLuxRectLight_Retain(UsdLuxRectLightRef light);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxRectLight_Release(UsdLuxRectLightRef light);

/// Returns true if the light is valid.
USD_INTEROP_API bool UsdLuxRectLight_IsValid(UsdLuxRectLightRef light);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxRectLight_GetPrim(UsdLuxRectLightRef light);

/// Gets the width.
USD_INTEROP_API float UsdLuxRectLight_GetWidth(UsdLuxRectLightRef light, UsdTimeCode time);

/// Sets the width.
USD_INTEROP_API UsdResult UsdLuxRectLight_SetWidth(UsdLuxRectLightRef light, UsdTimeCode time, float width);

/// Gets the height.
USD_INTEROP_API float UsdLuxRectLight_GetHeight(UsdLuxRectLightRef light, UsdTimeCode time);

/// Sets the height.
USD_INTEROP_API UsdResult UsdLuxRectLight_SetHeight(UsdLuxRectLightRef light, UsdTimeCode time, float height);

/// Gets the texture file path.
/// Returns a newly allocated string that must be freed with free().
USD_INTEROP_API char* UsdLuxRectLight_GetTextureFile(UsdLuxRectLightRef light);

/// Sets the texture file path.
USD_INTEROP_API UsdResult UsdLuxRectLight_SetTextureFile(UsdLuxRectLightRef light, const char* path);

/// Gets the intensity.
USD_INTEROP_API float UsdLuxRectLight_GetIntensity(UsdLuxRectLightRef light, UsdTimeCode time);

/// Sets the intensity.
USD_INTEROP_API UsdResult UsdLuxRectLight_SetIntensity(UsdLuxRectLightRef light, UsdTimeCode time, float intensity);

/// Gets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxRectLight_GetColor(UsdLuxRectLightRef light, UsdTimeCode time, float* outColor);

/// Sets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxRectLight_SetColor(UsdLuxRectLightRef light, UsdTimeCode time, const float* color);

/// Gets the exposure value.
USD_INTEROP_API float UsdLuxRectLight_GetExposure(UsdLuxRectLightRef light, UsdTimeCode time);

/// Sets the exposure value.
USD_INTEROP_API UsdResult UsdLuxRectLight_SetExposure(UsdLuxRectLightRef light, UsdTimeCode time, float exposure);

/// Gets the normalize flag.
USD_INTEROP_API bool UsdLuxRectLight_GetNormalize(UsdLuxRectLightRef light, UsdTimeCode time);

/// Sets the normalize flag.
USD_INTEROP_API UsdResult UsdLuxRectLight_SetNormalize(UsdLuxRectLightRef light, UsdTimeCode time, bool normalize);

// ============================================================================
// MARK: - UsdLuxSphereLight (Point Light)
// ============================================================================

/// Defines a new SphereLight prim at the given path.
/// Caller must release with UsdLuxSphereLight_Release.
USD_INTEROP_API UsdLuxSphereLightRef UsdLuxSphereLight_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxSphereLight_Release.
USD_INTEROP_API UsdLuxSphereLightRef UsdLuxSphereLight_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxSphereLightRef UsdLuxSphereLight_Retain(UsdLuxSphereLightRef light);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxSphereLight_Release(UsdLuxSphereLightRef light);

/// Returns true if the light is valid.
USD_INTEROP_API bool UsdLuxSphereLight_IsValid(UsdLuxSphereLightRef light);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxSphereLight_GetPrim(UsdLuxSphereLightRef light);

/// Gets the radius.
USD_INTEROP_API float UsdLuxSphereLight_GetRadius(UsdLuxSphereLightRef light, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdLuxSphereLight_SetRadius(UsdLuxSphereLightRef light, UsdTimeCode time, float radius);

/// Gets the treatAsPoint hint.
USD_INTEROP_API bool UsdLuxSphereLight_GetTreatAsPoint(UsdLuxSphereLightRef light, UsdTimeCode time);

/// Sets the treatAsPoint hint.
USD_INTEROP_API UsdResult UsdLuxSphereLight_SetTreatAsPoint(UsdLuxSphereLightRef light, UsdTimeCode time, bool treatAsPoint);

/// Gets the intensity.
USD_INTEROP_API float UsdLuxSphereLight_GetIntensity(UsdLuxSphereLightRef light, UsdTimeCode time);

/// Sets the intensity.
USD_INTEROP_API UsdResult UsdLuxSphereLight_SetIntensity(UsdLuxSphereLightRef light, UsdTimeCode time, float intensity);

/// Gets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxSphereLight_GetColor(UsdLuxSphereLightRef light, UsdTimeCode time, float* outColor);

/// Sets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxSphereLight_SetColor(UsdLuxSphereLightRef light, UsdTimeCode time, const float* color);

/// Gets the exposure value.
USD_INTEROP_API float UsdLuxSphereLight_GetExposure(UsdLuxSphereLightRef light, UsdTimeCode time);

/// Sets the exposure value.
USD_INTEROP_API UsdResult UsdLuxSphereLight_SetExposure(UsdLuxSphereLightRef light, UsdTimeCode time, float exposure);

/// Gets the normalize flag.
USD_INTEROP_API bool UsdLuxSphereLight_GetNormalize(UsdLuxSphereLightRef light, UsdTimeCode time);

/// Sets the normalize flag.
USD_INTEROP_API UsdResult UsdLuxSphereLight_SetNormalize(UsdLuxSphereLightRef light, UsdTimeCode time, bool normalize);

// ============================================================================
// MARK: - UsdLuxCylinderLight (Tube Light)
// ============================================================================

/// Defines a new CylinderLight prim at the given path.
/// Caller must release with UsdLuxCylinderLight_Release.
USD_INTEROP_API UsdLuxCylinderLightRef UsdLuxCylinderLight_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxCylinderLight_Release.
USD_INTEROP_API UsdLuxCylinderLightRef UsdLuxCylinderLight_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxCylinderLightRef UsdLuxCylinderLight_Retain(UsdLuxCylinderLightRef light);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxCylinderLight_Release(UsdLuxCylinderLightRef light);

/// Returns true if the light is valid.
USD_INTEROP_API bool UsdLuxCylinderLight_IsValid(UsdLuxCylinderLightRef light);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxCylinderLight_GetPrim(UsdLuxCylinderLightRef light);

/// Gets the length (in the X axis).
USD_INTEROP_API float UsdLuxCylinderLight_GetLength(UsdLuxCylinderLightRef light, UsdTimeCode time);

/// Sets the length (in the X axis).
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetLength(UsdLuxCylinderLightRef light, UsdTimeCode time, float length);

/// Gets the radius.
USD_INTEROP_API float UsdLuxCylinderLight_GetRadius(UsdLuxCylinderLightRef light, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetRadius(UsdLuxCylinderLightRef light, UsdTimeCode time, float radius);

/// Gets the treatAsLine hint.
USD_INTEROP_API bool UsdLuxCylinderLight_GetTreatAsLine(UsdLuxCylinderLightRef light, UsdTimeCode time);

/// Sets the treatAsLine hint.
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetTreatAsLine(UsdLuxCylinderLightRef light, UsdTimeCode time, bool treatAsLine);

/// Gets the intensity.
USD_INTEROP_API float UsdLuxCylinderLight_GetIntensity(UsdLuxCylinderLightRef light, UsdTimeCode time);

/// Sets the intensity.
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetIntensity(UsdLuxCylinderLightRef light, UsdTimeCode time, float intensity);

/// Gets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxCylinderLight_GetColor(UsdLuxCylinderLightRef light, UsdTimeCode time, float* outColor);

/// Sets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetColor(UsdLuxCylinderLightRef light, UsdTimeCode time, const float* color);

/// Gets the exposure value.
USD_INTEROP_API float UsdLuxCylinderLight_GetExposure(UsdLuxCylinderLightRef light, UsdTimeCode time);

/// Sets the exposure value.
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetExposure(UsdLuxCylinderLightRef light, UsdTimeCode time, float exposure);

/// Gets the normalize flag.
USD_INTEROP_API bool UsdLuxCylinderLight_GetNormalize(UsdLuxCylinderLightRef light, UsdTimeCode time);

/// Sets the normalize flag.
USD_INTEROP_API UsdResult UsdLuxCylinderLight_SetNormalize(UsdLuxCylinderLightRef light, UsdTimeCode time, bool normalize);

// ============================================================================
// MARK: - UsdLuxDiskLight
// ============================================================================

/// Defines a new DiskLight prim at the given path.
/// Caller must release with UsdLuxDiskLight_Release.
USD_INTEROP_API UsdLuxDiskLightRef UsdLuxDiskLight_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxDiskLight_Release.
USD_INTEROP_API UsdLuxDiskLightRef UsdLuxDiskLight_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxDiskLightRef UsdLuxDiskLight_Retain(UsdLuxDiskLightRef light);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxDiskLight_Release(UsdLuxDiskLightRef light);

/// Returns true if the light is valid.
USD_INTEROP_API bool UsdLuxDiskLight_IsValid(UsdLuxDiskLightRef light);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxDiskLight_GetPrim(UsdLuxDiskLightRef light);

/// Gets the radius.
USD_INTEROP_API float UsdLuxDiskLight_GetRadius(UsdLuxDiskLightRef light, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdLuxDiskLight_SetRadius(UsdLuxDiskLightRef light, UsdTimeCode time, float radius);

/// Gets the intensity.
USD_INTEROP_API float UsdLuxDiskLight_GetIntensity(UsdLuxDiskLightRef light, UsdTimeCode time);

/// Sets the intensity.
USD_INTEROP_API UsdResult UsdLuxDiskLight_SetIntensity(UsdLuxDiskLightRef light, UsdTimeCode time, float intensity);

/// Gets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxDiskLight_GetColor(UsdLuxDiskLightRef light, UsdTimeCode time, float* outColor);

/// Sets the color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxDiskLight_SetColor(UsdLuxDiskLightRef light, UsdTimeCode time, const float* color);

/// Gets the exposure value.
USD_INTEROP_API float UsdLuxDiskLight_GetExposure(UsdLuxDiskLightRef light, UsdTimeCode time);

/// Sets the exposure value.
USD_INTEROP_API UsdResult UsdLuxDiskLight_SetExposure(UsdLuxDiskLightRef light, UsdTimeCode time, float exposure);

/// Gets the normalize flag.
USD_INTEROP_API bool UsdLuxDiskLight_GetNormalize(UsdLuxDiskLightRef light, UsdTimeCode time);

/// Sets the normalize flag.
USD_INTEROP_API UsdResult UsdLuxDiskLight_SetNormalize(UsdLuxDiskLightRef light, UsdTimeCode time, bool normalize);

// ============================================================================
// MARK: - UsdLuxLightFilter (Light Filter)
// ============================================================================

/// Defines a new LightFilter prim at the given path.
/// Caller must release with UsdLuxLightFilter_Release.
USD_INTEROP_API UsdLuxLightFilterRef UsdLuxLightFilter_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdLuxLightFilter_Release.
USD_INTEROP_API UsdLuxLightFilterRef UsdLuxLightFilter_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxLightFilterRef UsdLuxLightFilter_Retain(UsdLuxLightFilterRef filter);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxLightFilter_Release(UsdLuxLightFilterRef filter);

/// Returns true if the light filter is valid.
USD_INTEROP_API bool UsdLuxLightFilter_IsValid(UsdLuxLightFilterRef filter);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxLightFilter_GetPrim(UsdLuxLightFilterRef filter);

/// Gets the shader ID.
/// Returns a newly allocated string that must be freed with free().
USD_INTEROP_API char* UsdLuxLightFilter_GetShaderId(UsdLuxLightFilterRef filter);

/// Sets the shader ID.
USD_INTEROP_API UsdResult UsdLuxLightFilter_SetShaderId(UsdLuxLightFilterRef filter, const char* shaderId);

// ============================================================================
// MARK: - UsdLuxShadowAPI (Shadow Controls)
// ============================================================================

/// Applies ShadowAPI to the given prim.
/// Caller must release with UsdLuxShadowAPI_Release.
USD_INTEROP_API UsdLuxShadowAPIRef UsdLuxShadowAPI_Apply(UsdPrimRef prim);

/// Creates ShadowAPI wrapper from a prim (if already applied).
/// Caller must release with UsdLuxShadowAPI_Release.
USD_INTEROP_API UsdLuxShadowAPIRef UsdLuxShadowAPI_Get(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdLuxShadowAPIRef UsdLuxShadowAPI_Retain(UsdLuxShadowAPIRef shadow);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdLuxShadowAPI_Release(UsdLuxShadowAPIRef shadow);

/// Returns true if the shadow API is valid.
USD_INTEROP_API bool UsdLuxShadowAPI_IsValid(UsdLuxShadowAPIRef shadow);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdLuxShadowAPI_GetPrim(UsdLuxShadowAPIRef shadow);

/// Gets whether shadows are enabled.
USD_INTEROP_API bool UsdLuxShadowAPI_GetShadowEnable(UsdLuxShadowAPIRef shadow, UsdTimeCode time);

/// Sets whether shadows are enabled.
USD_INTEROP_API UsdResult UsdLuxShadowAPI_SetShadowEnable(UsdLuxShadowAPIRef shadow, UsdTimeCode time, bool enable);

/// Gets the shadow color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxShadowAPI_GetShadowColor(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float* outColor);

/// Sets the shadow color (3 floats: r, g, b).
USD_INTEROP_API UsdResult UsdLuxShadowAPI_SetShadowColor(UsdLuxShadowAPIRef shadow, UsdTimeCode time, const float* color);

/// Gets the maximum shadow distance (-1 for no limit).
USD_INTEROP_API float UsdLuxShadowAPI_GetShadowDistance(UsdLuxShadowAPIRef shadow, UsdTimeCode time);

/// Sets the maximum shadow distance (-1 for no limit).
USD_INTEROP_API UsdResult UsdLuxShadowAPI_SetShadowDistance(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float distance);

/// Gets the shadow falloff (-1 for no falloff).
USD_INTEROP_API float UsdLuxShadowAPI_GetShadowFalloff(UsdLuxShadowAPIRef shadow, UsdTimeCode time);

/// Sets the shadow falloff (-1 for no falloff).
USD_INTEROP_API UsdResult UsdLuxShadowAPI_SetShadowFalloff(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float falloff);

/// Gets the shadow falloff gamma.
USD_INTEROP_API float UsdLuxShadowAPI_GetShadowFalloffGamma(UsdLuxShadowAPIRef shadow, UsdTimeCode time);

/// Sets the shadow falloff gamma.
USD_INTEROP_API UsdResult UsdLuxShadowAPI_SetShadowFalloffGamma(UsdLuxShadowAPIRef shadow, UsdTimeCode time, float gamma);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDLUX_USDLUX_H
