// UsdLux.swift - UsdLux module Swift wrappers
// Mirrors: pxr/usd/usdLux/*.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - LuxError

/// Errors that can occur when working with UsdLux types.
public enum LuxError: Error, CustomStringConvertible {

    /// Failed to define a light prim.
    case definitionFailed(String)

    /// A light operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .definitionFailed(let message):
            return "Light definition failed: \(message)"
        case .operationFailed(let message):
            return "Light operation failed: \(message)"
        }
    }
}

// MARK: - LuxDistantLight

/// A UsdLuxDistantLight is a directional light source.
///
/// Distant lights emit parallel rays of light, as if the light source
/// is infinitely far away. They are useful for simulating sunlight or
/// other distant light sources.
///
/// Mirrors `pxr::UsdLuxDistantLight` from the USD C++ API.
public final class LuxDistantLight: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdLuxDistantLightRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdLuxDistantLightRef) {
        self.handle = handle
    }

    deinit {
        UsdLuxDistantLight_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new DistantLight prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new DistantLight prim.
    /// - Returns: The newly defined DistantLight.
    /// - Throws: `LuxError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> LuxDistantLight {
        guard let ref = UsdLuxDistantLight_Define(stage.handle, path.handle) else {
            throw LuxError.definitionFailed("Failed to define DistantLight at '\(path)'")
        }
        return LuxDistantLight(handle: ref)
    }

    /// Creates a LuxDistantLight schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A LuxDistantLight wrapper, or `nil` if the prim is not a valid DistantLight.
    public static func from(prim: Prim) -> LuxDistantLight? {
        guard let ref = UsdLuxDistantLight_FromPrim(prim.handle) else {
            return nil
        }
        let light = LuxDistantLight(handle: ref)
        return light.isValid ? light : nil
    }

    // MARK: - Validity

    /// Returns `true` if the light is valid.
    public var isValid: Bool {
        UsdLuxDistantLight_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdLuxDistantLight_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Light Properties

    /// Gets the angular diameter in degrees at the given time.
    /// Default is 0.53 degrees (like the sun).
    public func angle(at time: TimeCode = .default) -> Float {
        UsdLuxDistantLight_GetAngle(handle, time.cTimeCode)
    }

    /// Sets the angular diameter in degrees at the given time.
    ///
    /// - Parameters:
    ///   - angle: The angular diameter in degrees.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setAngle(_ angle: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDistantLight_SetAngle(handle, time.cTimeCode, angle)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light angle")
        }
    }

    /// Gets the intensity at the given time.
    public func intensity(at time: TimeCode = .default) -> Float {
        UsdLuxDistantLight_GetIntensity(handle, time.cTimeCode)
    }

    /// Sets the intensity at the given time.
    ///
    /// - Parameters:
    ///   - intensity: The intensity value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setIntensity(_ intensity: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDistantLight_SetIntensity(handle, time.cTimeCode, intensity)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light intensity")
        }
    }

    /// Gets the color at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (r, g, b) color values, or `nil` on failure.
    public func color(at time: TimeCode = .default) -> (r: Float, g: Float, b: Float)? {
        var color: [Float] = [0, 0, 0]
        let result = color.withUnsafeMutableBufferPointer { buffer in
            UsdLuxDistantLight_GetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (color[0], color[1], color[2])
    }

    /// Sets the color at the given time.
    ///
    /// - Parameters:
    ///   - r: Red component (0-1).
    ///   - g: Green component (0-1).
    ///   - b: Blue component (0-1).
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setColor(r: Float, g: Float, b: Float, at time: TimeCode = .default) throws {
        let color: [Float] = [r, g, b]
        let result = color.withUnsafeBufferPointer { buffer in
            UsdLuxDistantLight_SetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light color")
        }
    }

    /// Gets the exposure value at the given time.
    public func exposure(at time: TimeCode = .default) -> Float {
        UsdLuxDistantLight_GetExposure(handle, time.cTimeCode)
    }

    /// Sets the exposure value at the given time.
    ///
    /// - Parameters:
    ///   - exposure: The exposure value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setExposure(_ exposure: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDistantLight_SetExposure(handle, time.cTimeCode, exposure)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light exposure")
        }
    }

    /// Gets the diffuse multiplier at the given time.
    public func diffuse(at time: TimeCode = .default) -> Float {
        UsdLuxDistantLight_GetDiffuse(handle, time.cTimeCode)
    }

    /// Sets the diffuse multiplier at the given time.
    ///
    /// - Parameters:
    ///   - diffuse: The diffuse multiplier.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setDiffuse(_ diffuse: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDistantLight_SetDiffuse(handle, time.cTimeCode, diffuse)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light diffuse")
        }
    }

    /// Gets the specular multiplier at the given time.
    public func specular(at time: TimeCode = .default) -> Float {
        UsdLuxDistantLight_GetSpecular(handle, time.cTimeCode)
    }

    /// Sets the specular multiplier at the given time.
    ///
    /// - Parameters:
    ///   - specular: The specular multiplier.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setSpecular(_ specular: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDistantLight_SetSpecular(handle, time.cTimeCode, specular)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light specular")
        }
    }

    /// Gets whether intensity should be normalized by solid angle.
    public func normalize(at time: TimeCode = .default) -> Bool {
        UsdLuxDistantLight_GetNormalize(handle, time.cTimeCode)
    }

    /// Sets whether intensity should be normalized by solid angle.
    ///
    /// - Parameters:
    ///   - normalize: Whether to normalize.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setNormalize(_ normalize: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxDistantLight_SetNormalize(handle, time.cTimeCode, normalize)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set distant light normalize")
        }
    }
}

// MARK: - CustomStringConvertible

extension LuxDistantLight: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid distant light>"
    }
}

// MARK: - LuxDomeLight

/// A UsdLuxDomeLight is an environment/IBL light source.
///
/// Dome lights provide image-based lighting from an environment map.
/// They are useful for realistic lighting based on HDR images.
///
/// Mirrors `pxr::UsdLuxDomeLight` from the USD C++ API.
public final class LuxDomeLight: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdLuxDomeLightRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdLuxDomeLightRef) {
        self.handle = handle
    }

    deinit {
        UsdLuxDomeLight_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new DomeLight prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new DomeLight prim.
    /// - Returns: The newly defined DomeLight.
    /// - Throws: `LuxError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> LuxDomeLight {
        guard let ref = UsdLuxDomeLight_Define(stage.handle, path.handle) else {
            throw LuxError.definitionFailed("Failed to define DomeLight at '\(path)'")
        }
        return LuxDomeLight(handle: ref)
    }

    /// Creates a LuxDomeLight schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A LuxDomeLight wrapper, or `nil` if the prim is not a valid DomeLight.
    public static func from(prim: Prim) -> LuxDomeLight? {
        guard let ref = UsdLuxDomeLight_FromPrim(prim.handle) else {
            return nil
        }
        let light = LuxDomeLight(handle: ref)
        return light.isValid ? light : nil
    }

    // MARK: - Validity

    /// Returns `true` if the light is valid.
    public var isValid: Bool {
        UsdLuxDomeLight_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdLuxDomeLight_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Light Properties

    /// Gets the texture file path.
    public var textureFile: String? {
        guard let cStr = UsdLuxDomeLight_GetTextureFile(handle) else {
            return nil
        }
        let result = String(cString: cStr)
        free(cStr)
        return result.isEmpty ? nil : result
    }

    /// Sets the texture file path.
    ///
    /// - Parameter path: The path to the texture file.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setTextureFile(_ path: String) throws {
        let result = UsdLuxDomeLight_SetTextureFile(handle, path)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set dome light texture file")
        }
    }

    /// Gets the texture format.
    public var textureFormat: String? {
        guard let cStr = UsdLuxDomeLight_GetTextureFormat(handle) else {
            return nil
        }
        let result = String(cString: cStr)
        free(cStr)
        return result.isEmpty ? nil : result
    }

    /// Sets the texture format.
    ///
    /// - Parameter format: The format (automatic, latlong, mirroredBall, angular, cubeMapVerticalCross).
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setTextureFormat(_ format: String) throws {
        let result = UsdLuxDomeLight_SetTextureFormat(handle, format)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set dome light texture format")
        }
    }

    /// Gets the guide radius at the given time.
    public func guideRadius(at time: TimeCode = .default) -> Float {
        UsdLuxDomeLight_GetGuideRadius(handle, time.cTimeCode)
    }

    /// Sets the guide radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The guide radius for visualization.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setGuideRadius(_ radius: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDomeLight_SetGuideRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set dome light guide radius")
        }
    }

    /// Gets the intensity at the given time.
    public func intensity(at time: TimeCode = .default) -> Float {
        UsdLuxDomeLight_GetIntensity(handle, time.cTimeCode)
    }

    /// Sets the intensity at the given time.
    ///
    /// - Parameters:
    ///   - intensity: The intensity value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setIntensity(_ intensity: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDomeLight_SetIntensity(handle, time.cTimeCode, intensity)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set dome light intensity")
        }
    }

    /// Gets the color at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (r, g, b) color values, or `nil` on failure.
    public func color(at time: TimeCode = .default) -> (r: Float, g: Float, b: Float)? {
        var color: [Float] = [0, 0, 0]
        let result = color.withUnsafeMutableBufferPointer { buffer in
            UsdLuxDomeLight_GetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (color[0], color[1], color[2])
    }

    /// Sets the color at the given time.
    ///
    /// - Parameters:
    ///   - r: Red component (0-1).
    ///   - g: Green component (0-1).
    ///   - b: Blue component (0-1).
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setColor(r: Float, g: Float, b: Float, at time: TimeCode = .default) throws {
        let color: [Float] = [r, g, b]
        let result = color.withUnsafeBufferPointer { buffer in
            UsdLuxDomeLight_SetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set dome light color")
        }
    }

    /// Gets the exposure value at the given time.
    public func exposure(at time: TimeCode = .default) -> Float {
        UsdLuxDomeLight_GetExposure(handle, time.cTimeCode)
    }

    /// Sets the exposure value at the given time.
    ///
    /// - Parameters:
    ///   - exposure: The exposure value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setExposure(_ exposure: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDomeLight_SetExposure(handle, time.cTimeCode, exposure)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set dome light exposure")
        }
    }
}

// MARK: - CustomStringConvertible

extension LuxDomeLight: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid dome light>"
    }
}

// MARK: - LuxRectLight

/// A UsdLuxRectLight is a rectangular area light source.
///
/// Rect lights emit light from a rectangular surface. They are
/// useful for soft, diffuse lighting like window light or softboxes.
///
/// Mirrors `pxr::UsdLuxRectLight` from the USD C++ API.
public final class LuxRectLight: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdLuxRectLightRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdLuxRectLightRef) {
        self.handle = handle
    }

    deinit {
        UsdLuxRectLight_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new RectLight prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new RectLight prim.
    /// - Returns: The newly defined RectLight.
    /// - Throws: `LuxError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> LuxRectLight {
        guard let ref = UsdLuxRectLight_Define(stage.handle, path.handle) else {
            throw LuxError.definitionFailed("Failed to define RectLight at '\(path)'")
        }
        return LuxRectLight(handle: ref)
    }

    /// Creates a LuxRectLight schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A LuxRectLight wrapper, or `nil` if the prim is not a valid RectLight.
    public static func from(prim: Prim) -> LuxRectLight? {
        guard let ref = UsdLuxRectLight_FromPrim(prim.handle) else {
            return nil
        }
        let light = LuxRectLight(handle: ref)
        return light.isValid ? light : nil
    }

    // MARK: - Validity

    /// Returns `true` if the light is valid.
    public var isValid: Bool {
        UsdLuxRectLight_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdLuxRectLight_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Light Properties

    /// Gets the width at the given time.
    public func width(at time: TimeCode = .default) -> Float {
        UsdLuxRectLight_GetWidth(handle, time.cTimeCode)
    }

    /// Sets the width at the given time.
    ///
    /// - Parameters:
    ///   - width: The width value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setWidth(_ width: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxRectLight_SetWidth(handle, time.cTimeCode, width)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light width")
        }
    }

    /// Gets the height at the given time.
    public func height(at time: TimeCode = .default) -> Float {
        UsdLuxRectLight_GetHeight(handle, time.cTimeCode)
    }

    /// Sets the height at the given time.
    ///
    /// - Parameters:
    ///   - height: The height value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setHeight(_ height: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxRectLight_SetHeight(handle, time.cTimeCode, height)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light height")
        }
    }

    /// Gets the texture file path.
    public var textureFile: String? {
        guard let cStr = UsdLuxRectLight_GetTextureFile(handle) else {
            return nil
        }
        let result = String(cString: cStr)
        free(cStr)
        return result.isEmpty ? nil : result
    }

    /// Sets the texture file path.
    ///
    /// - Parameter path: The path to the texture file.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setTextureFile(_ path: String) throws {
        let result = UsdLuxRectLight_SetTextureFile(handle, path)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light texture file")
        }
    }

    /// Gets the intensity at the given time.
    public func intensity(at time: TimeCode = .default) -> Float {
        UsdLuxRectLight_GetIntensity(handle, time.cTimeCode)
    }

    /// Sets the intensity at the given time.
    ///
    /// - Parameters:
    ///   - intensity: The intensity value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setIntensity(_ intensity: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxRectLight_SetIntensity(handle, time.cTimeCode, intensity)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light intensity")
        }
    }

    /// Gets the color at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (r, g, b) color values, or `nil` on failure.
    public func color(at time: TimeCode = .default) -> (r: Float, g: Float, b: Float)? {
        var color: [Float] = [0, 0, 0]
        let result = color.withUnsafeMutableBufferPointer { buffer in
            UsdLuxRectLight_GetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (color[0], color[1], color[2])
    }

    /// Sets the color at the given time.
    ///
    /// - Parameters:
    ///   - r: Red component (0-1).
    ///   - g: Green component (0-1).
    ///   - b: Blue component (0-1).
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setColor(r: Float, g: Float, b: Float, at time: TimeCode = .default) throws {
        let color: [Float] = [r, g, b]
        let result = color.withUnsafeBufferPointer { buffer in
            UsdLuxRectLight_SetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light color")
        }
    }

    /// Gets the exposure value at the given time.
    public func exposure(at time: TimeCode = .default) -> Float {
        UsdLuxRectLight_GetExposure(handle, time.cTimeCode)
    }

    /// Sets the exposure value at the given time.
    ///
    /// - Parameters:
    ///   - exposure: The exposure value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setExposure(_ exposure: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxRectLight_SetExposure(handle, time.cTimeCode, exposure)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light exposure")
        }
    }

    /// Gets whether intensity should be normalized by area.
    public func normalize(at time: TimeCode = .default) -> Bool {
        UsdLuxRectLight_GetNormalize(handle, time.cTimeCode)
    }

    /// Sets whether intensity should be normalized by area.
    ///
    /// - Parameters:
    ///   - normalize: Whether to normalize.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setNormalize(_ normalize: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxRectLight_SetNormalize(handle, time.cTimeCode, normalize)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set rect light normalize")
        }
    }
}

// MARK: - CustomStringConvertible

extension LuxRectLight: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid rect light>"
    }
}

// MARK: - LuxSphereLight

/// A UsdLuxSphereLight is a point/spherical light source.
///
/// Sphere lights emit light from a spherical surface. With zero radius,
/// they act as point lights.
///
/// Mirrors `pxr::UsdLuxSphereLight` from the USD C++ API.
public final class LuxSphereLight: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdLuxSphereLightRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdLuxSphereLightRef) {
        self.handle = handle
    }

    deinit {
        UsdLuxSphereLight_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new SphereLight prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new SphereLight prim.
    /// - Returns: The newly defined SphereLight.
    /// - Throws: `LuxError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> LuxSphereLight {
        guard let ref = UsdLuxSphereLight_Define(stage.handle, path.handle) else {
            throw LuxError.definitionFailed("Failed to define SphereLight at '\(path)'")
        }
        return LuxSphereLight(handle: ref)
    }

    /// Creates a LuxSphereLight schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A LuxSphereLight wrapper, or `nil` if the prim is not a valid SphereLight.
    public static func from(prim: Prim) -> LuxSphereLight? {
        guard let ref = UsdLuxSphereLight_FromPrim(prim.handle) else {
            return nil
        }
        let light = LuxSphereLight(handle: ref)
        return light.isValid ? light : nil
    }

    // MARK: - Validity

    /// Returns `true` if the light is valid.
    public var isValid: Bool {
        UsdLuxSphereLight_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdLuxSphereLight_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Light Properties

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Float {
        UsdLuxSphereLight_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxSphereLight_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set sphere light radius")
        }
    }

    /// Gets whether to treat this as a point light.
    public func treatAsPoint(at time: TimeCode = .default) -> Bool {
        UsdLuxSphereLight_GetTreatAsPoint(handle, time.cTimeCode)
    }

    /// Sets whether to treat this as a point light.
    ///
    /// - Parameters:
    ///   - treatAsPoint: Whether to treat as point light.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setTreatAsPoint(_ treatAsPoint: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxSphereLight_SetTreatAsPoint(handle, time.cTimeCode, treatAsPoint)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set sphere light treat as point")
        }
    }

    /// Gets the intensity at the given time.
    public func intensity(at time: TimeCode = .default) -> Float {
        UsdLuxSphereLight_GetIntensity(handle, time.cTimeCode)
    }

    /// Sets the intensity at the given time.
    ///
    /// - Parameters:
    ///   - intensity: The intensity value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setIntensity(_ intensity: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxSphereLight_SetIntensity(handle, time.cTimeCode, intensity)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set sphere light intensity")
        }
    }

    /// Gets the color at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (r, g, b) color values, or `nil` on failure.
    public func color(at time: TimeCode = .default) -> (r: Float, g: Float, b: Float)? {
        var color: [Float] = [0, 0, 0]
        let result = color.withUnsafeMutableBufferPointer { buffer in
            UsdLuxSphereLight_GetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (color[0], color[1], color[2])
    }

    /// Sets the color at the given time.
    ///
    /// - Parameters:
    ///   - r: Red component (0-1).
    ///   - g: Green component (0-1).
    ///   - b: Blue component (0-1).
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setColor(r: Float, g: Float, b: Float, at time: TimeCode = .default) throws {
        let color: [Float] = [r, g, b]
        let result = color.withUnsafeBufferPointer { buffer in
            UsdLuxSphereLight_SetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set sphere light color")
        }
    }

    /// Gets the exposure value at the given time.
    public func exposure(at time: TimeCode = .default) -> Float {
        UsdLuxSphereLight_GetExposure(handle, time.cTimeCode)
    }

    /// Sets the exposure value at the given time.
    ///
    /// - Parameters:
    ///   - exposure: The exposure value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setExposure(_ exposure: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxSphereLight_SetExposure(handle, time.cTimeCode, exposure)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set sphere light exposure")
        }
    }

    /// Gets whether intensity should be normalized by area.
    public func normalize(at time: TimeCode = .default) -> Bool {
        UsdLuxSphereLight_GetNormalize(handle, time.cTimeCode)
    }

    /// Sets whether intensity should be normalized by area.
    ///
    /// - Parameters:
    ///   - normalize: Whether to normalize.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setNormalize(_ normalize: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxSphereLight_SetNormalize(handle, time.cTimeCode, normalize)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set sphere light normalize")
        }
    }
}

// MARK: - CustomStringConvertible

extension LuxSphereLight: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid sphere light>"
    }
}

// MARK: - LuxCylinderLight

/// A UsdLuxCylinderLight is a tube/cylinder light source.
///
/// Cylinder lights emit light from a cylindrical surface. They are
/// useful for simulating fluorescent tubes or neon lights.
///
/// Mirrors `pxr::UsdLuxCylinderLight` from the USD C++ API.
public final class LuxCylinderLight: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdLuxCylinderLightRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdLuxCylinderLightRef) {
        self.handle = handle
    }

    deinit {
        UsdLuxCylinderLight_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new CylinderLight prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new CylinderLight prim.
    /// - Returns: The newly defined CylinderLight.
    /// - Throws: `LuxError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> LuxCylinderLight {
        guard let ref = UsdLuxCylinderLight_Define(stage.handle, path.handle) else {
            throw LuxError.definitionFailed("Failed to define CylinderLight at '\(path)'")
        }
        return LuxCylinderLight(handle: ref)
    }

    /// Creates a LuxCylinderLight schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A LuxCylinderLight wrapper, or `nil` if the prim is not a valid CylinderLight.
    public static func from(prim: Prim) -> LuxCylinderLight? {
        guard let ref = UsdLuxCylinderLight_FromPrim(prim.handle) else {
            return nil
        }
        let light = LuxCylinderLight(handle: ref)
        return light.isValid ? light : nil
    }

    // MARK: - Validity

    /// Returns `true` if the light is valid.
    public var isValid: Bool {
        UsdLuxCylinderLight_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdLuxCylinderLight_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Light Properties

    /// Gets the length (in the X axis) at the given time.
    public func length(at time: TimeCode = .default) -> Float {
        UsdLuxCylinderLight_GetLength(handle, time.cTimeCode)
    }

    /// Sets the length (in the X axis) at the given time.
    ///
    /// - Parameters:
    ///   - length: The length value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setLength(_ length: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxCylinderLight_SetLength(handle, time.cTimeCode, length)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light length")
        }
    }

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Float {
        UsdLuxCylinderLight_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxCylinderLight_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light radius")
        }
    }

    /// Gets whether to treat this as a line light.
    public func treatAsLine(at time: TimeCode = .default) -> Bool {
        UsdLuxCylinderLight_GetTreatAsLine(handle, time.cTimeCode)
    }

    /// Sets whether to treat this as a line light.
    ///
    /// - Parameters:
    ///   - treatAsLine: Whether to treat as line light.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setTreatAsLine(_ treatAsLine: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxCylinderLight_SetTreatAsLine(handle, time.cTimeCode, treatAsLine)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light treat as line")
        }
    }

    /// Gets the intensity at the given time.
    public func intensity(at time: TimeCode = .default) -> Float {
        UsdLuxCylinderLight_GetIntensity(handle, time.cTimeCode)
    }

    /// Sets the intensity at the given time.
    ///
    /// - Parameters:
    ///   - intensity: The intensity value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setIntensity(_ intensity: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxCylinderLight_SetIntensity(handle, time.cTimeCode, intensity)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light intensity")
        }
    }

    /// Gets the color at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (r, g, b) color values, or `nil` on failure.
    public func color(at time: TimeCode = .default) -> (r: Float, g: Float, b: Float)? {
        var color: [Float] = [0, 0, 0]
        let result = color.withUnsafeMutableBufferPointer { buffer in
            UsdLuxCylinderLight_GetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (color[0], color[1], color[2])
    }

    /// Sets the color at the given time.
    ///
    /// - Parameters:
    ///   - r: Red component (0-1).
    ///   - g: Green component (0-1).
    ///   - b: Blue component (0-1).
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setColor(r: Float, g: Float, b: Float, at time: TimeCode = .default) throws {
        let color: [Float] = [r, g, b]
        let result = color.withUnsafeBufferPointer { buffer in
            UsdLuxCylinderLight_SetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light color")
        }
    }

    /// Gets the exposure value at the given time.
    public func exposure(at time: TimeCode = .default) -> Float {
        UsdLuxCylinderLight_GetExposure(handle, time.cTimeCode)
    }

    /// Sets the exposure value at the given time.
    ///
    /// - Parameters:
    ///   - exposure: The exposure value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setExposure(_ exposure: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxCylinderLight_SetExposure(handle, time.cTimeCode, exposure)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light exposure")
        }
    }

    /// Gets whether intensity should be normalized by area.
    public func normalize(at time: TimeCode = .default) -> Bool {
        UsdLuxCylinderLight_GetNormalize(handle, time.cTimeCode)
    }

    /// Sets whether intensity should be normalized by area.
    ///
    /// - Parameters:
    ///   - normalize: Whether to normalize.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setNormalize(_ normalize: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxCylinderLight_SetNormalize(handle, time.cTimeCode, normalize)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set cylinder light normalize")
        }
    }
}

// MARK: - CustomStringConvertible

extension LuxCylinderLight: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid cylinder light>"
    }
}

// MARK: - LuxDiskLight

/// A UsdLuxDiskLight is a circular disk area light source.
///
/// Disk lights emit light from a circular disk surface. They are
/// useful for simulating spotlights or soft area lights.
///
/// Mirrors `pxr::UsdLuxDiskLight` from the USD C++ API.
public final class LuxDiskLight: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdLuxDiskLightRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdLuxDiskLightRef) {
        self.handle = handle
    }

    deinit {
        UsdLuxDiskLight_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new DiskLight prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new DiskLight prim.
    /// - Returns: The newly defined DiskLight.
    /// - Throws: `LuxError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> LuxDiskLight {
        guard let ref = UsdLuxDiskLight_Define(stage.handle, path.handle) else {
            throw LuxError.definitionFailed("Failed to define DiskLight at '\(path)'")
        }
        return LuxDiskLight(handle: ref)
    }

    /// Creates a LuxDiskLight schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A LuxDiskLight wrapper, or `nil` if the prim is not a valid DiskLight.
    public static func from(prim: Prim) -> LuxDiskLight? {
        guard let ref = UsdLuxDiskLight_FromPrim(prim.handle) else {
            return nil
        }
        let light = LuxDiskLight(handle: ref)
        return light.isValid ? light : nil
    }

    // MARK: - Validity

    /// Returns `true` if the light is valid.
    public var isValid: Bool {
        UsdLuxDiskLight_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdLuxDiskLight_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Light Properties

    /// Gets the radius at the given time.
    public func radius(at time: TimeCode = .default) -> Float {
        UsdLuxDiskLight_GetRadius(handle, time.cTimeCode)
    }

    /// Sets the radius at the given time.
    ///
    /// - Parameters:
    ///   - radius: The radius value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setRadius(_ radius: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDiskLight_SetRadius(handle, time.cTimeCode, radius)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set disk light radius")
        }
    }

    /// Gets the intensity at the given time.
    public func intensity(at time: TimeCode = .default) -> Float {
        UsdLuxDiskLight_GetIntensity(handle, time.cTimeCode)
    }

    /// Sets the intensity at the given time.
    ///
    /// - Parameters:
    ///   - intensity: The intensity value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setIntensity(_ intensity: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDiskLight_SetIntensity(handle, time.cTimeCode, intensity)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set disk light intensity")
        }
    }

    /// Gets the color at the given time.
    ///
    /// - Parameter time: The time code at which to evaluate.
    /// - Returns: A tuple of (r, g, b) color values, or `nil` on failure.
    public func color(at time: TimeCode = .default) -> (r: Float, g: Float, b: Float)? {
        var color: [Float] = [0, 0, 0]
        let result = color.withUnsafeMutableBufferPointer { buffer in
            UsdLuxDiskLight_GetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            return nil
        }
        return (color[0], color[1], color[2])
    }

    /// Sets the color at the given time.
    ///
    /// - Parameters:
    ///   - r: Red component (0-1).
    ///   - g: Green component (0-1).
    ///   - b: Blue component (0-1).
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setColor(r: Float, g: Float, b: Float, at time: TimeCode = .default) throws {
        let color: [Float] = [r, g, b]
        let result = color.withUnsafeBufferPointer { buffer in
            UsdLuxDiskLight_SetColor(handle, time.cTimeCode, buffer.baseAddress)
        }
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set disk light color")
        }
    }

    /// Gets the exposure value at the given time.
    public func exposure(at time: TimeCode = .default) -> Float {
        UsdLuxDiskLight_GetExposure(handle, time.cTimeCode)
    }

    /// Sets the exposure value at the given time.
    ///
    /// - Parameters:
    ///   - exposure: The exposure value.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setExposure(_ exposure: Float, at time: TimeCode = .default) throws {
        let result = UsdLuxDiskLight_SetExposure(handle, time.cTimeCode, exposure)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set disk light exposure")
        }
    }

    /// Gets whether intensity should be normalized by area.
    public func normalize(at time: TimeCode = .default) -> Bool {
        UsdLuxDiskLight_GetNormalize(handle, time.cTimeCode)
    }

    /// Sets whether intensity should be normalized by area.
    ///
    /// - Parameters:
    ///   - normalize: Whether to normalize.
    ///   - time: The time code at which to set the value.
    /// - Throws: `LuxError.operationFailed` if the operation fails.
    public func setNormalize(_ normalize: Bool, at time: TimeCode = .default) throws {
        let result = UsdLuxDiskLight_SetNormalize(handle, time.cTimeCode, normalize)
        guard result == USD_RESULT_SUCCESS else {
            throw LuxError.operationFailed("Failed to set disk light normalize")
        }
    }
}

// MARK: - CustomStringConvertible

extension LuxDiskLight: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid disk light>"
    }
}

// MARK: - Type Aliases

/// Type alias for UsdLuxDistantLight
public typealias UsdLuxDistantLight = LuxDistantLight

/// Type alias for UsdLuxDomeLight
public typealias UsdLuxDomeLight = LuxDomeLight

/// Type alias for UsdLuxRectLight
public typealias UsdLuxRectLight = LuxRectLight

/// Type alias for UsdLuxSphereLight
public typealias UsdLuxSphereLight = LuxSphereLight

/// Type alias for UsdLuxCylinderLight
public typealias UsdLuxCylinderLight = LuxCylinderLight

/// Type alias for UsdLuxDiskLight
public typealias UsdLuxDiskLight = LuxDiskLight
