// Light.swift - UsdLux light types
// Mirrors: pxr/usdLux/

import OpenUSDInterop

// MARK: - UsdLuxLightAPI

/// Base API for all lights.
/// Mirrors pxr::UsdLuxLightAPI from the USD C++ API.
public struct UsdLuxLightAPI: UsdAPISchemaBase, Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Returns the intensity attribute.
    public var intensityAttr: UsdAttribute? {
        return prim.attribute("inputs:intensity")
    }

    /// Gets the light intensity.
    public func getIntensity(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 1.0
    }

    /// Sets the light intensity.
    public func setIntensity(_ intensity: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Returns the color attribute.
    public var colorAttr: UsdAttribute? {
        return prim.attribute("inputs:color")
    }

    /// Gets the light color.
    public func getColor(at time: UsdTimeCode = .default) -> GfVec3f {
        // TODO: Implement via C interop
        return GfVec3f(1, 1, 1)
    }

    /// Sets the light color.
    public func setColor(_ color: GfVec3f, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Gets whether the light is enabled.
    public func getEnable(at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return true
    }

    /// Sets whether the light is enabled.
    public func setEnable(_ enable: Bool, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - UsdLuxDistantLight

/// A distant (directional) light.
/// Mirrors pxr::UsdLuxDistantLight from the USD C++ API.
public struct UsdLuxDistantLight: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Defines a new distant light at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdLuxDistantLight? {
        guard let prim = stage.definePrim(at: path, type: "DistantLight") else {
            return nil
        }
        return UsdLuxDistantLight(prim)
    }

    /// Gets the angular extent of the light in degrees.
    public func getAngle(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 0.53 // Default sun angle
    }

    /// Sets the angular extent of the light in degrees.
    public func setAngle(_ angle: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - UsdLuxSphereLight

/// A spherical area light.
/// Mirrors pxr::UsdLuxSphereLight from the USD C++ API.
public struct UsdLuxSphereLight: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Defines a new sphere light at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdLuxSphereLight? {
        guard let prim = stage.definePrim(at: path, type: "SphereLight") else {
            return nil
        }
        return UsdLuxSphereLight(prim)
    }

    /// Gets the radius of the light.
    public func getRadius(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 0.5
    }

    /// Sets the radius of the light.
    public func setRadius(_ radius: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - UsdLuxRectLight

/// A rectangular area light.
/// Mirrors pxr::UsdLuxRectLight from the USD C++ API.
public struct UsdLuxRectLight: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Defines a new rect light at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdLuxRectLight? {
        guard let prim = stage.definePrim(at: path, type: "RectLight") else {
            return nil
        }
        return UsdLuxRectLight(prim)
    }

    /// Gets the width of the light.
    public func getWidth(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 1.0
    }

    /// Sets the width of the light.
    public func setWidth(_ width: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Gets the height of the light.
    public func getHeight(at time: UsdTimeCode = .default) -> Float {
        // TODO: Implement via C interop
        return 1.0
    }

    /// Sets the height of the light.
    public func setHeight(_ height: Float, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - UsdLuxDomeLight

/// A dome light (environment/IBL light).
/// Mirrors pxr::UsdLuxDomeLight from the USD C++ API.
public struct UsdLuxDomeLight: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Defines a new dome light at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdLuxDomeLight? {
        guard let prim = stage.definePrim(at: path, type: "DomeLight") else {
            return nil
        }
        return UsdLuxDomeLight(prim)
    }

    /// Gets the texture file for the dome light.
    public func getTextureFile(at time: UsdTimeCode = .default) -> String? {
        // TODO: Implement via C interop
        return nil
    }

    /// Sets the texture file for the dome light.
    public func setTextureFile(_ path: String, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}
