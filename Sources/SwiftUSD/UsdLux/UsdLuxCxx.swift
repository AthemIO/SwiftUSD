// UsdLuxCxx.swift - Swift convenience wrappers for UsdLux C++ interop
// Provides Swift-idiomatic access to UsdLux lighting schemas

import USDCxx

// MARK: - Type Aliases for UsdLux types

/// UsdLuxLightAPI - API schema for light properties
public typealias LightAPI = swiftusd.LuxLightAPIRef

/// UsdLuxShadowAPI - API schema for shadow control
public typealias ShadowAPI = swiftusd.LuxShadowAPIRef

/// UsdLuxShapingAPI - API schema for spot/cone shaping
public typealias ShapingAPI = swiftusd.LuxShapingAPIRef

/// UsdLuxDistantLight - Directional light from infinity
public typealias DistantLight = swiftusd.LuxDistantLightRef

/// UsdLuxDomeLight - Environment/IBL light
public typealias DomeLight = swiftusd.LuxDomeLightRef

/// UsdLuxRectLight - Rectangular area light
public typealias RectLight = swiftusd.LuxRectLightRef

/// UsdLuxSphereLight - Spherical/point light
public typealias SphereLight = swiftusd.LuxSphereLightRef

/// UsdLuxCylinderLight - Cylindrical/tube light
public typealias CylinderLight = swiftusd.LuxCylinderLightRef

/// UsdLuxDiskLight - Circular disk light
public typealias DiskLight = swiftusd.LuxDiskLightRef

/// UsdLuxLightFilter - Modifies light effect
public typealias LightFilter = swiftusd.LuxLightFilterRef

// MARK: - LightAPI Extensions

extension swiftusd.LuxLightAPIRef {
    /// Get the light color as a Swift tuple
    public func getColor(at time: swiftusd.TimeCode = .Default()) -> (r: Float, g: Float, b: Float)? {
        var color = swiftusd.Vec3f()
        guard GetColor(time, &color) else { return nil }
        return (color.x, color.y, color.z)
    }

    /// Set the light color from a Swift tuple
    @discardableResult
    public func setColor(_ r: Float, _ g: Float, _ b: Float, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetColor(swiftusd.Vec3f(r, g, b), time)
    }
}

// MARK: - ShadowAPI Extensions

extension swiftusd.LuxShadowAPIRef {
    /// Get the shadow color as a Swift tuple
    public func getShadowColor(at time: swiftusd.TimeCode = .Default()) -> (r: Float, g: Float, b: Float)? {
        var color = swiftusd.Vec3f()
        guard GetShadowColor(time, &color) else { return nil }
        return (color.x, color.y, color.z)
    }

    /// Set the shadow color from a Swift tuple
    @discardableResult
    public func setShadowColor(_ r: Float, _ g: Float, _ b: Float, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetShadowColor(swiftusd.Vec3f(r, g, b), time)
    }
}

// MARK: - ShapingAPI Extensions

extension swiftusd.LuxShapingAPIRef {
    /// Get the focus tint color as a Swift tuple
    public func getFocusTint(at time: swiftusd.TimeCode = .Default()) -> (r: Float, g: Float, b: Float)? {
        var color = swiftusd.Vec3f()
        guard GetShapingFocusTint(time, &color) else { return nil }
        return (color.x, color.y, color.z)
    }

    /// Set the focus tint from a Swift tuple
    @discardableResult
    public func setFocusTint(_ r: Float, _ g: Float, _ b: Float, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetShapingFocusTint(swiftusd.Vec3f(r, g, b), time)
    }
}

// MARK: - DistantLight Extensions

extension swiftusd.LuxDistantLightRef {
    /// Create a distant light at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxDistantLightRef? {
        Define(stage, path)
    }

    /// The angular diameter of the light source in degrees (default 0.53 like the sun)
    public var angle: Float {
        get { GetAngle() }
        set { SetAngle(newValue) }
    }
}

// MARK: - DomeLight Extensions

extension swiftusd.LuxDomeLightRef {
    /// Create a dome light at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxDomeLightRef? {
        Define(stage, path)
    }

    /// The HDR texture file path
    public func getTextureFilePath() -> String {
        String(cString: self.GetTextureFile())
    }

    /// Set the HDR texture file path
    @discardableResult
    public func setTextureFilePath(_ path: String) -> Bool {
        self.SetTextureFile(path)
    }

    /// The texture format (automatic, latlong, mirroredBall, angular, cubeMapVerticalCross)
    public var textureFormat: swiftusd.Token {
        get { GetTextureFormat() }
        set { SetTextureFormat(newValue) }
    }

    /// The guide geometry radius for visualization
    public var guideRadius: Float {
        get { GetGuideRadius() }
        set { SetGuideRadius(newValue) }
    }
}

// MARK: - RectLight Extensions

extension swiftusd.LuxRectLightRef {
    /// Create a rect light at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxRectLightRef? {
        Define(stage, path)
    }

    /// The width of the rectangle
    public var width: Float {
        get { GetWidth() }
        set { SetWidth(newValue) }
    }

    /// The height of the rectangle
    public var height: Float {
        get { GetHeight() }
        set { SetHeight(newValue) }
    }

    /// The texture file path
    public func getTextureFilePath() -> String {
        String(cString: self.GetTextureFile())
    }

    /// Set the texture file path
    @discardableResult
    public func setTextureFilePath(_ path: String) -> Bool {
        self.SetTextureFile(path)
    }
}

// MARK: - SphereLight Extensions

extension swiftusd.LuxSphereLightRef {
    /// Create a sphere light at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxSphereLightRef? {
        Define(stage, path)
    }

    /// The radius of the sphere
    public var radius: Float {
        get { GetRadius() }
        set { SetRadius(newValue) }
    }

    /// Whether to treat as a point light
    public var treatAsPoint: Bool {
        get { GetTreatAsPoint() }
        set { SetTreatAsPoint(newValue) }
    }
}

// MARK: - CylinderLight Extensions

extension swiftusd.LuxCylinderLightRef {
    /// Create a cylinder light at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxCylinderLightRef? {
        Define(stage, path)
    }

    /// The length of the cylinder
    public var length: Float {
        get { GetLength() }
        set { SetLength(newValue) }
    }

    /// The radius of the cylinder
    public var radius: Float {
        get { GetRadius() }
        set { SetRadius(newValue) }
    }

    /// Whether to treat as a line light
    public var treatAsLine: Bool {
        get { GetTreatAsLine() }
        set { SetTreatAsLine(newValue) }
    }
}

// MARK: - DiskLight Extensions

extension swiftusd.LuxDiskLightRef {
    /// Create a disk light at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxDiskLightRef? {
        Define(stage, path)
    }

    /// The radius of the disk
    public var radius: Float {
        get { GetRadius() }
        set { SetRadius(newValue) }
    }
}

// MARK: - LightFilter Extensions

extension swiftusd.LuxLightFilterRef {
    /// Create a light filter at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.LuxLightFilterRef? {
        Define(stage, path)
    }

    /// The shader ID for the filter
    public var shaderId: swiftusd.Token {
        get { GetShaderId() }
        set { SetShaderId(newValue) }
    }
}
