// UsdLux.hpp - Lighting Schemas facade
// CLEAN HEADER - NO pxr includes!
//
// This module provides wrappers for UsdLux schema classes including lights,
// light filters, and lighting-related API schemas.
#pragma once

#include "../CxxFacade.h"
#include "../usd/Usd.hpp"
#include "../usd/Sdf.hpp"
#include "../base/Gf.hpp"
#include "../base/Tf.hpp"
#include "../base/Vt.hpp"
#include "../usdGeom/UsdGeom.hpp"
#include "../usdShade/UsdShade.hpp"

namespace cxxfacade {
namespace usdLux {

// ============================================================================
// Forward Declarations
// ============================================================================

class UsdLuxLightAPI;
class UsdLuxMeshLightAPI;
class UsdLuxVolumeLightAPI;
class UsdLuxListAPI;
class UsdLuxShadowAPI;
class UsdLuxShapingAPI;
class UsdLuxLightFilter;
class UsdLuxDistantLight;
class UsdLuxDomeLight;
class UsdLuxRectLight;
class UsdLuxSphereLight;
class UsdLuxCylinderLight;
class UsdLuxDiskLight;

// ============================================================================
// UsdLuxLightAPI - Base API for all lights
// ============================================================================

/// UsdLuxLightAPI - API schema that imparts the quality of being a light onto a prim.
///
/// A light is any prim that has this schema applied to it. This is true
/// regardless of whether LightAPI is included as a built-in API of the prim
/// type (e.g. RectLight or DistantLight) or is applied directly to a Gprim.
class UsdLuxLightAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdLuxLightAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxLightAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxLightAPI();

    /// Get this API on the prim at path on stage
    static UsdLuxLightAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdLuxLightAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

    // ----- Shader ID -----

    /// Get the shader ID attribute
    usd::UsdAttribute GetShaderIdAttr() const;

    /// Create the shader ID attribute
    usd::UsdAttribute CreateShaderIdAttr();

    // ----- Intensity -----

    /// Get the intensity attribute
    usd::UsdAttribute GetIntensityAttr() const;

    /// Create the intensity attribute
    usd::UsdAttribute CreateIntensityAttr();

    // ----- Exposure -----

    /// Get the exposure attribute
    usd::UsdAttribute GetExposureAttr() const;

    /// Create the exposure attribute
    usd::UsdAttribute CreateExposureAttr();

    // ----- Diffuse -----

    /// Get the diffuse multiplier attribute
    usd::UsdAttribute GetDiffuseAttr() const;

    /// Create the diffuse multiplier attribute
    usd::UsdAttribute CreateDiffuseAttr();

    // ----- Specular -----

    /// Get the specular multiplier attribute
    usd::UsdAttribute GetSpecularAttr() const;

    /// Create the specular multiplier attribute
    usd::UsdAttribute CreateSpecularAttr();

    // ----- Normalize -----

    /// Get the normalize attribute
    usd::UsdAttribute GetNormalizeAttr() const;

    /// Create the normalize attribute
    usd::UsdAttribute CreateNormalizeAttr();

    // ----- Color -----

    /// Get the color attribute
    usd::UsdAttribute GetColorAttr() const;

    /// Create the color attribute
    usd::UsdAttribute CreateColorAttr();

    // ----- Color Temperature -----

    /// Get the enableColorTemperature attribute
    usd::UsdAttribute GetEnableColorTemperatureAttr() const;

    /// Create the enableColorTemperature attribute
    usd::UsdAttribute CreateEnableColorTemperatureAttr();

    /// Get the colorTemperature attribute
    usd::UsdAttribute GetColorTemperatureAttr() const;

    /// Create the colorTemperature attribute
    usd::UsdAttribute CreateColorTemperatureAttr();

    // ----- Material Sync Mode -----

    /// Get the materialSyncMode attribute
    usd::UsdAttribute GetMaterialSyncModeAttr() const;

    /// Create the materialSyncMode attribute
    usd::UsdAttribute CreateMaterialSyncModeAttr();

    // ----- Filters Relationship -----

    /// Get the filters relationship
    usd::UsdRelationship GetFiltersRel() const;

    /// Create the filters relationship
    usd::UsdRelationship CreateFiltersRel();

    // ----- ConnectableAPI -----

    /// Get the ConnectableAPI for this light
    usdShade::UsdShadeConnectableAPI ConnectableAPI() const;

    // ----- Inputs/Outputs (Shade interface) -----

    /// Create an input
    usdShade::UsdShadeInput CreateInput(const tf::TfToken& name, const std::string& typeName);

    /// Get an input by name
    usdShade::UsdShadeInput GetInput(const tf::TfToken& name) const;

    /// Get all inputs
    std::vector<usdShade::UsdShadeInput> GetInputs(bool onlyAuthored = true) const;

    /// Create an output
    usdShade::UsdShadeOutput CreateOutput(const tf::TfToken& name, const std::string& typeName);

    /// Get an output by name
    usdShade::UsdShadeOutput GetOutput(const tf::TfToken& name) const;

    /// Get all outputs
    std::vector<usdShade::UsdShadeOutput> GetOutputs(bool onlyAuthored = true) const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxMeshLightAPI - API for mesh lights
// ============================================================================

/// UsdLuxMeshLightAPI - API schema for treating a mesh as a light source.
class UsdLuxMeshLightAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdLuxMeshLightAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxMeshLightAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxMeshLightAPI();

    /// Get this API on the prim at path on stage
    static UsdLuxMeshLightAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdLuxMeshLightAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxVolumeLightAPI - API for volume lights
// ============================================================================

/// UsdLuxVolumeLightAPI - API schema for treating a volume as a light source.
class UsdLuxVolumeLightAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdLuxVolumeLightAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxVolumeLightAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxVolumeLightAPI();

    /// Get this API on the prim at path on stage
    static UsdLuxVolumeLightAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdLuxVolumeLightAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxListAPI - API for light lists
// ============================================================================

/// Light list computation modes
enum class UsdLuxListAPIComputeMode {
    ComputeModeConsultModelHierarchyCache,
    ComputeModeIgnoreCache
};

/// UsdLuxListAPI - API schema for listing lights in a scene.
class UsdLuxListAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdLuxListAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxListAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxListAPI();

    /// Get this API on the prim at path on stage
    static UsdLuxListAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdLuxListAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

    /// Get the lightList relationship
    usd::UsdRelationship GetLightListRel() const;

    /// Create the lightList relationship
    usd::UsdRelationship CreateLightListRel();

    /// Get the lightList:cacheBehavior attribute
    usd::UsdAttribute GetLightListCacheBehaviorAttr() const;

    /// Create the lightList:cacheBehavior attribute
    usd::UsdAttribute CreateLightListCacheBehaviorAttr();

    /// Compute the list of lights beneath this prim
    std::vector<sdf::SdfPath> ComputeLightList(UsdLuxListAPIComputeMode mode) const;

    /// Store the light list
    void StoreLightList(const std::vector<sdf::SdfPath>& paths) const;

    /// Invalidate the light list cache
    void InvalidateLightListCache() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxShadowAPI - Shadow control API
// ============================================================================

/// UsdLuxShadowAPI - Controls to refine a light's shadow behavior.
class UsdLuxShadowAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdLuxShadowAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxShadowAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxShadowAPI();

    /// Get this API on the prim at path on stage
    static UsdLuxShadowAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdLuxShadowAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

    // ----- Shadow Enable -----

    /// Get the shadow:enable attribute
    usd::UsdAttribute GetShadowEnableAttr() const;

    /// Create the shadow:enable attribute
    usd::UsdAttribute CreateShadowEnableAttr();

    // ----- Shadow Color -----

    /// Get the shadow:color attribute
    usd::UsdAttribute GetShadowColorAttr() const;

    /// Create the shadow:color attribute
    usd::UsdAttribute CreateShadowColorAttr();

    // ----- Shadow Distance -----

    /// Get the shadow:distance attribute
    usd::UsdAttribute GetShadowDistanceAttr() const;

    /// Create the shadow:distance attribute
    usd::UsdAttribute CreateShadowDistanceAttr();

    // ----- Shadow Falloff -----

    /// Get the shadow:falloff attribute
    usd::UsdAttribute GetShadowFalloffAttr() const;

    /// Create the shadow:falloff attribute
    usd::UsdAttribute CreateShadowFalloffAttr();

    // ----- Shadow Falloff Gamma -----

    /// Get the shadow:falloffGamma attribute
    usd::UsdAttribute GetShadowFalloffGammaAttr() const;

    /// Create the shadow:falloffGamma attribute
    usd::UsdAttribute CreateShadowFalloffGammaAttr();

    // ----- ConnectableAPI -----

    /// Get the ConnectableAPI for this API
    usdShade::UsdShadeConnectableAPI ConnectableAPI() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxShapingAPI - Shaping control API
// ============================================================================

/// UsdLuxShapingAPI - Controls for shaping a light's emission.
class UsdLuxShapingAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdLuxShapingAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxShapingAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxShapingAPI();

    /// Get this API on the prim at path on stage
    static UsdLuxShapingAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdLuxShapingAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

    // ----- Focus -----

    /// Get the shaping:focus attribute
    usd::UsdAttribute GetShapingFocusAttr() const;

    /// Create the shaping:focus attribute
    usd::UsdAttribute CreateShapingFocusAttr();

    // ----- Focus Tint -----

    /// Get the shaping:focusTint attribute
    usd::UsdAttribute GetShapingFocusTintAttr() const;

    /// Create the shaping:focusTint attribute
    usd::UsdAttribute CreateShapingFocusTintAttr();

    // ----- Cone Angle -----

    /// Get the shaping:cone:angle attribute
    usd::UsdAttribute GetShapingConeAngleAttr() const;

    /// Create the shaping:cone:angle attribute
    usd::UsdAttribute CreateShapingConeAngleAttr();

    // ----- Cone Softness -----

    /// Get the shaping:cone:softness attribute
    usd::UsdAttribute GetShapingConeSoftnessAttr() const;

    /// Create the shaping:cone:softness attribute
    usd::UsdAttribute CreateShapingConeSoftnessAttr();

    // ----- IES Profile -----

    /// Get the shaping:ies:file attribute
    usd::UsdAttribute GetShapingIesFileAttr() const;

    /// Create the shaping:ies:file attribute
    usd::UsdAttribute CreateShapingIesFileAttr();

    /// Get the shaping:ies:angleScale attribute
    usd::UsdAttribute GetShapingIesAngleScaleAttr() const;

    /// Create the shaping:ies:angleScale attribute
    usd::UsdAttribute CreateShapingIesAngleScaleAttr();

    /// Get the shaping:ies:normalize attribute
    usd::UsdAttribute GetShapingIesNormalizeAttr() const;

    /// Create the shaping:ies:normalize attribute
    usd::UsdAttribute CreateShapingIesNormalizeAttr();

    // ----- ConnectableAPI -----

    /// Get the ConnectableAPI for this API
    usdShade::UsdShadeConnectableAPI ConnectableAPI() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxLightFilter - Light filter base
// ============================================================================

/// UsdLuxLightFilter - A light filter modifies the effect of a light.
class UsdLuxLightFilter : public usd::UsdTyped {
public:
    explicit UsdLuxLightFilter(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxLightFilter(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxLightFilter();

    /// Get this prim at path on stage
    static UsdLuxLightFilter Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a light filter at the given path
    static UsdLuxLightFilter Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the shader ID attribute
    usd::UsdAttribute GetShaderIdAttr() const;

    /// Create the shader ID attribute
    usd::UsdAttribute CreateShaderIdAttr();

    /// Get the ConnectableAPI for this light filter
    usdShade::UsdShadeConnectableAPI ConnectableAPI() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxDistantLight - Directional light from infinity
// ============================================================================

/// UsdLuxDistantLight - Light emitted from a distant source along the -Z axis.
/// Also known as a directional light.
class UsdLuxDistantLight : public usdGeom::UsdGeomXformable {
public:
    explicit UsdLuxDistantLight(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxDistantLight(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxDistantLight();

    /// Get this prim at path on stage
    static UsdLuxDistantLight Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a distant light at the given path
    static UsdLuxDistantLight Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the LightAPI for this light
    UsdLuxLightAPI LightAPI() const;

    // ----- Angle -----

    /// Get the angle attribute (angular diameter in degrees)
    usd::UsdAttribute GetAngleAttr() const;

    /// Create the angle attribute
    usd::UsdAttribute CreateAngleAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxDomeLight - Environment/IBL light
// ============================================================================

/// UsdLuxDomeLight - Light emitted inward from a distant external environment.
/// Used for image-based lighting (IBL).
class UsdLuxDomeLight : public usdGeom::UsdGeomXformable {
public:
    explicit UsdLuxDomeLight(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxDomeLight(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxDomeLight();

    /// Get this prim at path on stage
    static UsdLuxDomeLight Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a dome light at the given path
    static UsdLuxDomeLight Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the LightAPI for this light
    UsdLuxLightAPI LightAPI() const;

    // ----- Texture File -----

    /// Get the texture:file attribute
    usd::UsdAttribute GetTextureFileAttr() const;

    /// Create the texture:file attribute
    usd::UsdAttribute CreateTextureFileAttr();

    // ----- Texture Format -----

    /// Get the texture:format attribute
    usd::UsdAttribute GetTextureFormatAttr() const;

    /// Create the texture:format attribute
    usd::UsdAttribute CreateTextureFormatAttr();

    // ----- Guide Radius -----

    /// Get the guideRadius attribute
    usd::UsdAttribute GetGuideRadiusAttr() const;

    /// Create the guideRadius attribute
    usd::UsdAttribute CreateGuideRadiusAttr();

    // ----- Portals -----

    /// Get the portals relationship
    usd::UsdRelationship GetPortalsRel() const;

    /// Create the portals relationship
    usd::UsdRelationship CreatePortalsRel();

    /// Orient the dome to align with the stage's up axis
    void OrientToStageUpAxis() const;

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxRectLight - Rectangular area light
// ============================================================================

/// UsdLuxRectLight - Light emitted from one side of a rectangle.
class UsdLuxRectLight : public usdGeom::UsdGeomBoundable {
public:
    explicit UsdLuxRectLight(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxRectLight(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxRectLight();

    /// Get this prim at path on stage
    static UsdLuxRectLight Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a rect light at the given path
    static UsdLuxRectLight Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the LightAPI for this light
    UsdLuxLightAPI LightAPI() const;

    // ----- Width -----

    /// Get the width attribute
    usd::UsdAttribute GetWidthAttr() const;

    /// Create the width attribute
    usd::UsdAttribute CreateWidthAttr();

    // ----- Height -----

    /// Get the height attribute
    usd::UsdAttribute GetHeightAttr() const;

    /// Create the height attribute
    usd::UsdAttribute CreateHeightAttr();

    // ----- Texture File -----

    /// Get the texture:file attribute
    usd::UsdAttribute GetTextureFileAttr() const;

    /// Create the texture:file attribute
    usd::UsdAttribute CreateTextureFileAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxSphereLight - Sphere/point light
// ============================================================================

/// UsdLuxSphereLight - Light emitted outward from a sphere.
/// A sphere light with radius 0 is equivalent to a point light.
class UsdLuxSphereLight : public usdGeom::UsdGeomBoundable {
public:
    explicit UsdLuxSphereLight(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxSphereLight(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxSphereLight();

    /// Get this prim at path on stage
    static UsdLuxSphereLight Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a sphere light at the given path
    static UsdLuxSphereLight Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the LightAPI for this light
    UsdLuxLightAPI LightAPI() const;

    // ----- Radius -----

    /// Get the radius attribute
    usd::UsdAttribute GetRadiusAttr() const;

    /// Create the radius attribute
    usd::UsdAttribute CreateRadiusAttr();

    // ----- Treat As Point -----

    /// Get the treatAsPoint attribute
    usd::UsdAttribute GetTreatAsPointAttr() const;

    /// Create the treatAsPoint attribute
    usd::UsdAttribute CreateTreatAsPointAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxCylinderLight - Cylindrical light
// ============================================================================

/// UsdLuxCylinderLight - Light emitted outward from a cylinder.
class UsdLuxCylinderLight : public usdGeom::UsdGeomBoundable {
public:
    explicit UsdLuxCylinderLight(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxCylinderLight(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxCylinderLight();

    /// Get this prim at path on stage
    static UsdLuxCylinderLight Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a cylinder light at the given path
    static UsdLuxCylinderLight Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the LightAPI for this light
    UsdLuxLightAPI LightAPI() const;

    // ----- Length -----

    /// Get the length attribute
    usd::UsdAttribute GetLengthAttr() const;

    /// Create the length attribute
    usd::UsdAttribute CreateLengthAttr();

    // ----- Radius -----

    /// Get the radius attribute
    usd::UsdAttribute GetRadiusAttr() const;

    /// Create the radius attribute
    usd::UsdAttribute CreateRadiusAttr();

    // ----- Treat As Line -----

    /// Get the treatAsLine attribute
    usd::UsdAttribute GetTreatAsLineAttr() const;

    /// Create the treatAsLine attribute
    usd::UsdAttribute CreateTreatAsLineAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdLuxDiskLight - Disk-shaped area light
// ============================================================================

/// UsdLuxDiskLight - Light emitted from one side of a circular disk.
class UsdLuxDiskLight : public usdGeom::UsdGeomBoundable {
public:
    explicit UsdLuxDiskLight(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdLuxDiskLight(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdLuxDiskLight();

    /// Get this prim at path on stage
    static UsdLuxDiskLight Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a disk light at the given path
    static UsdLuxDiskLight Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Get the LightAPI for this light
    UsdLuxLightAPI LightAPI() const;

    // ----- Radius -----

    /// Get the radius attribute
    usd::UsdAttribute GetRadiusAttr() const;

    /// Create the radius attribute
    usd::UsdAttribute CreateRadiusAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// Token Constants
// ============================================================================

/// Common tokens used in UsdLux
namespace UsdLuxTokens {
    // Light types
    tf::TfToken distantLight();
    tf::TfToken domeLight();
    tf::TfToken rectLight();
    tf::TfToken sphereLight();
    tf::TfToken cylinderLight();
    tf::TfToken diskLight();
    tf::TfToken lightFilter();

    // Texture formats
    tf::TfToken automatic();
    tf::TfToken latlong();
    tf::TfToken mirroredBall();
    tf::TfToken angular();
    tf::TfToken cubeMapVerticalCross();

    // Material sync modes
    tf::TfToken materialGlowTintsLight();
    tf::TfToken independent();
    tf::TfToken noMaterialResponse();

    // Light list cache behavior
    tf::TfToken consumeAndHalt();
    tf::TfToken consumeAndContinue();
    tf::TfToken ignoreCache();
}

// ============================================================================
// Utility Functions
// ============================================================================

/// Convert a color temperature in Kelvin to an RGB color
gf::Vec3f BlackbodyTemperatureAsRgb(float kelvin);

} // namespace usdLux
} // namespace cxxfacade
