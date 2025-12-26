// UsdLux.swift - Lighting Schemas - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's UsdLux types,
// including lights, light filters, and lighting-related API schemas.
//
// The UsdLux module is the primary API for working with lighting in USD,
// providing schemas for various light types, shadows, shaping, and more.

import CxxFacade

// MARK: - Type Aliases

// --- Enumerations ---

/// UsdLuxListAPIComputeMode - Light list computation modes.
public typealias UsdLuxListAPIComputeMode = cxxfacade.usdLux.UsdLuxListAPIComputeMode

// --- API Schema Types ---

/// UsdLuxLightAPI - API schema for imparting the quality of being a light.
///
/// A light is any prim that has this schema applied to it. This is true
/// regardless of whether LightAPI is included as a built-in API of the prim
/// type (e.g. RectLight or DistantLight) or is applied directly to a Gprim.
///
/// ## Usage
/// ```swift
/// let lightAPI = UsdLuxLightAPI.apply(prim)
/// let intensityAttr = lightAPI.GetIntensityAttr()
/// let colorAttr = lightAPI.GetColorAttr()
/// ```
public typealias UsdLuxLightAPI = cxxfacade.usdLux.UsdLuxLightAPI

/// UsdLuxMeshLightAPI - API schema for treating a mesh as a light source.
public typealias UsdLuxMeshLightAPI = cxxfacade.usdLux.UsdLuxMeshLightAPI

/// UsdLuxVolumeLightAPI - API schema for treating a volume as a light source.
public typealias UsdLuxVolumeLightAPI = cxxfacade.usdLux.UsdLuxVolumeLightAPI

/// UsdLuxListAPI - API schema for listing lights in a scene.
public typealias UsdLuxListAPI = cxxfacade.usdLux.UsdLuxListAPI

/// UsdLuxShadowAPI - Controls to refine a light's shadow behavior.
///
/// These are non-physical controls that are valuable for visual lighting work.
public typealias UsdLuxShadowAPI = cxxfacade.usdLux.UsdLuxShadowAPI

/// UsdLuxShapingAPI - Controls for shaping a light's emission.
///
/// Provides controls for focus, cone angle, and IES profiles.
public typealias UsdLuxShapingAPI = cxxfacade.usdLux.UsdLuxShapingAPI

// --- Light Filter Types ---

/// UsdLuxLightFilter - A light filter modifies the effect of a light.
public typealias UsdLuxLightFilter = cxxfacade.usdLux.UsdLuxLightFilter

// --- Light Types ---

/// UsdLuxDistantLight - Light emitted from a distant source along the -Z axis.
///
/// Also known as a directional light. This is commonly used to represent
/// sunlight or other infinitely distant light sources.
public typealias UsdLuxDistantLight = cxxfacade.usdLux.UsdLuxDistantLight

/// UsdLuxDomeLight - Light emitted inward from a distant external environment.
///
/// Used for image-based lighting (IBL). The dome's default orientation
/// is such that its top pole is aligned with the world's +Y axis.
///
/// ## Usage
/// ```swift
/// var dome = UsdLuxDomeLight.define(&stage, path: "/lights/dome")
/// dome.CreateTextureFileAttr().Set("sky.hdr")
/// dome.OrientToStageUpAxis()
/// ```
public typealias UsdLuxDomeLight = cxxfacade.usdLux.UsdLuxDomeLight

/// UsdLuxRectLight - Light emitted from one side of a rectangle.
///
/// Rectangular area lights are commonly used for soft, diffuse lighting.
public typealias UsdLuxRectLight = cxxfacade.usdLux.UsdLuxRectLight

/// UsdLuxSphereLight - Light emitted outward from a sphere.
///
/// A sphere light with radius 0 is equivalent to a point light.
public typealias UsdLuxSphereLight = cxxfacade.usdLux.UsdLuxSphereLight

/// UsdLuxCylinderLight - Light emitted outward from a cylinder.
///
/// Useful for tube lights, neon signs, etc.
public typealias UsdLuxCylinderLight = cxxfacade.usdLux.UsdLuxCylinderLight

/// UsdLuxDiskLight - Light emitted from one side of a circular disk.
public typealias UsdLuxDiskLight = cxxfacade.usdLux.UsdLuxDiskLight

// --- Tokens ---

/// UsdLuxTokens - Common tokens used in UsdLux.
public typealias UsdLuxTokens = cxxfacade.usdLux.UsdLuxTokens

// MARK: - Utility Functions

/// Convert a color temperature in Kelvin to an RGB color.
///
/// This uses the Planckian locus approximation to convert blackbody
/// temperature to RGB color values.
///
/// - Parameter kelvin: The color temperature in Kelvin (typically 1000-40000K)
/// - Returns: An RGB color value
public func blackbodyTemperatureAsRgb(_ kelvin: Float) -> Vec3f {
    return cxxfacade.usdLux.BlackbodyTemperatureAsRgb(kelvin)
}

// MARK: - UsdLuxLightAPI Extensions

extension UsdLuxLightAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdLuxLightAPI {
        return UsdLuxLightAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdLuxLightAPI.CanApply(prim, nil)
    }

    /// Get the ConnectableAPI for this light.
    public func connectableAPI() -> UsdShadeConnectableAPI {
        return self.ConnectableAPI()
    }

    /// The intensity of the light.
    public var intensity: UsdAttribute {
        return self.GetIntensityAttr()
    }

    /// The exposure of the light.
    public var exposure: UsdAttribute {
        return self.GetExposureAttr()
    }

    /// The diffuse multiplier.
    public var diffuse: UsdAttribute {
        return self.GetDiffuseAttr()
    }

    /// The specular multiplier.
    public var specular: UsdAttribute {
        return self.GetSpecularAttr()
    }

    /// Whether to normalize the light output.
    public var normalize: UsdAttribute {
        return self.GetNormalizeAttr()
    }

    /// The color of the light.
    public var color: UsdAttribute {
        return self.GetColorAttr()
    }

    /// Whether color temperature is enabled.
    public var enableColorTemperature: UsdAttribute {
        return self.GetEnableColorTemperatureAttr()
    }

    /// The color temperature in Kelvin.
    public var colorTemperature: UsdAttribute {
        return self.GetColorTemperatureAttr()
    }

    /// The material sync mode.
    public var materialSyncMode: UsdAttribute {
        return self.GetMaterialSyncModeAttr()
    }

    /// The filters relationship.
    public var filters: UsdRelationship {
        return self.GetFiltersRel()
    }
}

// MARK: - UsdLuxShadowAPI Extensions

extension UsdLuxShadowAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdLuxShadowAPI {
        return UsdLuxShadowAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdLuxShadowAPI.CanApply(prim, nil)
    }

    /// Whether shadows are enabled.
    public var shadowEnable: UsdAttribute {
        return self.GetShadowEnableAttr()
    }

    /// The shadow color.
    public var shadowColor: UsdAttribute {
        return self.GetShadowColorAttr()
    }

    /// The shadow distance.
    public var shadowDistance: UsdAttribute {
        return self.GetShadowDistanceAttr()
    }

    /// The shadow falloff.
    public var shadowFalloff: UsdAttribute {
        return self.GetShadowFalloffAttr()
    }

    /// The shadow falloff gamma.
    public var shadowFalloffGamma: UsdAttribute {
        return self.GetShadowFalloffGammaAttr()
    }
}

// MARK: - UsdLuxShapingAPI Extensions

extension UsdLuxShapingAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdLuxShapingAPI {
        return UsdLuxShapingAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdLuxShapingAPI.CanApply(prim, nil)
    }

    /// The shaping focus.
    public var focus: UsdAttribute {
        return self.GetShapingFocusAttr()
    }

    /// The focus tint color.
    public var focusTint: UsdAttribute {
        return self.GetShapingFocusTintAttr()
    }

    /// The cone angle in degrees.
    public var coneAngle: UsdAttribute {
        return self.GetShapingConeAngleAttr()
    }

    /// The cone softness.
    public var coneSoftness: UsdAttribute {
        return self.GetShapingConeSoftnessAttr()
    }

    /// The IES profile file.
    public var iesFile: UsdAttribute {
        return self.GetShapingIesFileAttr()
    }

    /// The IES angle scale.
    public var iesAngleScale: UsdAttribute {
        return self.GetShapingIesAngleScaleAttr()
    }

    /// Whether to normalize the IES profile.
    public var iesNormalize: UsdAttribute {
        return self.GetShapingIesNormalizeAttr()
    }
}

// MARK: - UsdLuxListAPI Extensions

extension UsdLuxListAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdLuxListAPI {
        return UsdLuxListAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdLuxListAPI.CanApply(prim, nil)
    }

    /// The light list relationship.
    public var lightList: UsdRelationship {
        return self.GetLightListRel()
    }

    /// The light list cache behavior attribute.
    public var lightListCacheBehavior: UsdAttribute {
        return self.GetLightListCacheBehaviorAttr()
    }

    /// Compute the list of lights beneath this prim.
    public func computeLightList(mode: UsdLuxListAPIComputeMode = .ComputeModeConsultModelHierarchyCache) -> [SdfPath] {
        return Array(self.ComputeLightList(mode))
    }

    /// Invalidate the light list cache.
    public func invalidateLightListCache() {
        self.InvalidateLightListCache()
    }
}

// MARK: - UsdLuxMeshLightAPI Extensions

extension UsdLuxMeshLightAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdLuxMeshLightAPI {
        return UsdLuxMeshLightAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdLuxMeshLightAPI.CanApply(prim, nil)
    }
}

// MARK: - UsdLuxVolumeLightAPI Extensions

extension UsdLuxVolumeLightAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdLuxVolumeLightAPI {
        return UsdLuxVolumeLightAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdLuxVolumeLightAPI.CanApply(prim, nil)
    }
}

// MARK: - UsdLuxDistantLight Extensions

extension UsdLuxDistantLight {
    /// Define a UsdLuxDistantLight at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxDistantLight {
        return UsdLuxDistantLight.Define(&stage, path)
    }

    /// Define a UsdLuxDistantLight at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxDistantLight {
        return UsdLuxDistantLight.Define(&stage, SdfPath(path))
    }

    /// Get the LightAPI for this light.
    public func lightAPI() -> UsdLuxLightAPI {
        return self.LightAPI()
    }

    /// The angular diameter of the light in degrees.
    public var angle: UsdAttribute {
        return self.GetAngleAttr()
    }
}

// MARK: - UsdLuxDomeLight Extensions

extension UsdLuxDomeLight {
    /// Define a UsdLuxDomeLight at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxDomeLight {
        return UsdLuxDomeLight.Define(&stage, path)
    }

    /// Define a UsdLuxDomeLight at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxDomeLight {
        return UsdLuxDomeLight.Define(&stage, SdfPath(path))
    }

    /// Get the LightAPI for this light.
    public func lightAPI() -> UsdLuxLightAPI {
        return self.LightAPI()
    }

    /// The texture file (e.g., HDR image for IBL).
    public var textureFile: UsdAttribute {
        return self.GetTextureFileAttr()
    }

    /// The texture format (latlong, mirroredBall, etc.).
    public var textureFormat: UsdAttribute {
        return self.GetTextureFormatAttr()
    }

    /// The guide radius for visualization.
    public var guideRadius: UsdAttribute {
        return self.GetGuideRadiusAttr()
    }

    /// The portals relationship.
    public var portals: UsdRelationship {
        return self.GetPortalsRel()
    }

    /// Orient the dome to align with the stage's up axis.
    public func orientToStageUpAxis() {
        self.OrientToStageUpAxis()
    }
}

// MARK: - UsdLuxRectLight Extensions

extension UsdLuxRectLight {
    /// Define a UsdLuxRectLight at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxRectLight {
        return UsdLuxRectLight.Define(&stage, path)
    }

    /// Define a UsdLuxRectLight at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxRectLight {
        return UsdLuxRectLight.Define(&stage, SdfPath(path))
    }

    /// Get the LightAPI for this light.
    public func lightAPI() -> UsdLuxLightAPI {
        return self.LightAPI()
    }

    /// The width of the rect light.
    public var width: UsdAttribute {
        return self.GetWidthAttr()
    }

    /// The height of the rect light.
    public var height: UsdAttribute {
        return self.GetHeightAttr()
    }

    /// The texture file for the rect light.
    public var textureFile: UsdAttribute {
        return self.GetTextureFileAttr()
    }
}

// MARK: - UsdLuxSphereLight Extensions

extension UsdLuxSphereLight {
    /// Define a UsdLuxSphereLight at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxSphereLight {
        return UsdLuxSphereLight.Define(&stage, path)
    }

    /// Define a UsdLuxSphereLight at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxSphereLight {
        return UsdLuxSphereLight.Define(&stage, SdfPath(path))
    }

    /// Get the LightAPI for this light.
    public func lightAPI() -> UsdLuxLightAPI {
        return self.LightAPI()
    }

    /// The radius of the sphere light.
    public var radius: UsdAttribute {
        return self.GetRadiusAttr()
    }

    /// Whether to treat this as a point light.
    public var treatAsPoint: UsdAttribute {
        return self.GetTreatAsPointAttr()
    }
}

// MARK: - UsdLuxCylinderLight Extensions

extension UsdLuxCylinderLight {
    /// Define a UsdLuxCylinderLight at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxCylinderLight {
        return UsdLuxCylinderLight.Define(&stage, path)
    }

    /// Define a UsdLuxCylinderLight at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxCylinderLight {
        return UsdLuxCylinderLight.Define(&stage, SdfPath(path))
    }

    /// Get the LightAPI for this light.
    public func lightAPI() -> UsdLuxLightAPI {
        return self.LightAPI()
    }

    /// The length of the cylinder light.
    public var length: UsdAttribute {
        return self.GetLengthAttr()
    }

    /// The radius of the cylinder light.
    public var radius: UsdAttribute {
        return self.GetRadiusAttr()
    }

    /// Whether to treat this as a line light.
    public var treatAsLine: UsdAttribute {
        return self.GetTreatAsLineAttr()
    }
}

// MARK: - UsdLuxDiskLight Extensions

extension UsdLuxDiskLight {
    /// Define a UsdLuxDiskLight at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxDiskLight {
        return UsdLuxDiskLight.Define(&stage, path)
    }

    /// Define a UsdLuxDiskLight at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxDiskLight {
        return UsdLuxDiskLight.Define(&stage, SdfPath(path))
    }

    /// Get the LightAPI for this light.
    public func lightAPI() -> UsdLuxLightAPI {
        return self.LightAPI()
    }

    /// The radius of the disk light.
    public var radius: UsdAttribute {
        return self.GetRadiusAttr()
    }
}

// MARK: - UsdLuxLightFilter Extensions

extension UsdLuxLightFilter {
    /// Define a UsdLuxLightFilter at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdLuxLightFilter {
        return UsdLuxLightFilter.Define(&stage, path)
    }

    /// Define a UsdLuxLightFilter at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdLuxLightFilter {
        return UsdLuxLightFilter.Define(&stage, SdfPath(path))
    }

    /// Get the ConnectableAPI for this light filter.
    public func connectableAPI() -> UsdShadeConnectableAPI {
        return self.ConnectableAPI()
    }

    /// The shader ID attribute.
    public var shaderId: UsdAttribute {
        return self.GetShaderIdAttr()
    }
}
