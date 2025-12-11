/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Gf

/* note: the typealiases are documented the same way twice,
 * keep it like this so that sourcekit shows documentation
 * regardless of which typealias a user might use in their
 * code. */

/**
 * # GfColorSpace
 *
 * Basic type: ColorSpace
 *
 * This class represents a colorspace. Color spaces may be created by
 * name, parameterization, or by a 3x3 matrix and a gamma operator.
 *
 * The color spaces natively recognized by GfColorSpace are listed in
 * `Gf.ColorSpace.Name`.
 */
public typealias GfColorSpace = Pixar.GfColorSpace

public extension Gf
{
  /**
   * # Gf.ColorSpace
   *
   * Basic type: ColorSpace
   *
   * This class represents a colorspace. Color spaces may be created by
   * name, parameterization, or by a 3x3 matrix and a gamma operator.
   *
   * The color spaces natively recognized by GfColorSpace are listed in
   * `Gf.ColorSpace.Name`.
   */
  typealias ColorSpace = GfColorSpace
}

// MARK: - Named Color Spaces

public extension Gf.ColorSpace
{
  /// Named color spaces supported natively by GfColorSpace.
  ///
  /// These color spaces follow the Color Interop Forum naming convention:
  /// `<encoding>_<primaries>_<image_state>`
  ///
  /// Where:
  /// - Encoding: `lin` (linear), `srgb` (IEC 61966-2-1:1999), or `gNN` (gamma NN)
  /// - Primaries: Short name of CIF color space (e.g., `rec709`, `ap1`, `p3d65`)
  /// - Image state: `scene` (scene-referred) or display-referred
  enum Name: String, CaseIterable, Sendable
  {
    // MARK: Linear Color Spaces

    /// ACEScg - Linear AP1 primaries, scene-referred.
    /// The standard working space for VFX production.
    case acescg = "lin_ap1_scene"

    /// ACES2065-1 - Linear AP0 primaries, scene-referred.
    /// The archival and interchange format for ACES.
    case aces2065_1 = "lin_ap0_scene"

    /// Linear Rec.709 (sRGB primaries) - scene-referred.
    /// Linear version of sRGB, common for rendering.
    case linearRec709 = "lin_rec709_scene"

    /// Linear P3-D65 - scene-referred.
    /// Wide gamut display space used by Apple devices and digital cinema.
    case linearP3D65 = "lin_p3d65_scene"

    /// Linear Rec.2020 - scene-referred.
    /// Ultra-wide gamut for HDR and UHDTV content.
    case linearRec2020 = "lin_rec2020_scene"

    /// Linear AdobeRGB - scene-referred.
    /// Wide gamut space common in print workflows.
    case linearAdobeRGB = "lin_adobergb_scene"

    /// CIE XYZ with D65 white point - scene-referred.
    /// The fundamental colorimetric reference space.
    case linearCIEXYZD65 = "lin_ciexyzd65_scene"

    // MARK: sRGB Encoded Color Spaces

    /// sRGB - sRGB transfer function with Rec.709 primaries, scene-referred.
    /// The most common color space for web and display content.
    case srgb = "srgb_rec709_scene"

    /// sRGB transfer function with AP1 primaries, scene-referred.
    case srgbAP1 = "srgb_ap1_scene"

    /// sRGB transfer function with P3-D65 primaries, scene-referred.
    case srgbP3D65 = "srgb_p3d65_scene"

    // MARK: Gamma Encoded Color Spaces

    /// Gamma 2.2 with Rec.709 primaries, scene-referred.
    case gamma22Rec709 = "g22_rec709_scene"

    /// Gamma 1.8 with Rec.709 primaries, scene-referred.
    /// Traditional Mac gamma.
    case gamma18Rec709 = "g18_rec709_scene"

    /// Gamma 2.2 with AP1 primaries, scene-referred.
    case gamma22AP1 = "g22_ap1_scene"

    /// Gamma 2.2 with AdobeRGB primaries, scene-referred.
    case gamma22AdobeRGB = "g22_adobergb_scene"

    // MARK: Special Color Spaces

    /// Identity color space - no transformation.
    case identity = "identity"

    /// Data - non-color data (e.g., normal maps, displacement).
    case data = "data"

    /// Raw - unmanaged color data.
    case raw = "raw"

    /// Unknown color space.
    case unknown = "unknown"

    /// The TfToken representation of this color space name.
    public var token: Tf.Token
    {
      Tf.Token(rawValue)
    }
  }
}

// MARK: - Constructors

public extension Gf.ColorSpace
{
  /// Create a color space from a named color space.
  ///
  /// - Parameter name: The named color space to create.
  ///
  /// Example:
  /// ```swift
  /// let srgb = Gf.ColorSpace(.srgb)
  /// let acescg = Gf.ColorSpace(.acescg)
  /// ```
  init(_ name: Name)
  {
    self.init(name.token)
  }

  /// Create a custom color space from chromaticity coordinates.
  ///
  /// - Parameters:
  ///   - name: A unique name for this custom color space.
  ///   - redChroma: The red chromaticity coordinates (CIE xy).
  ///   - greenChroma: The green chromaticity coordinates (CIE xy).
  ///   - blueChroma: The blue chromaticity coordinates (CIE xy).
  ///   - whitePoint: The white point chromaticity coordinates (CIE xy).
  ///   - gamma: The gamma value for the transfer function (1.0 for linear).
  ///   - linearBias: The linear bias for the transfer function (0.0 for pure gamma).
  ///
  /// Example:
  /// ```swift
  /// // Define a custom color space with Rec.709 primaries and gamma 2.4
  /// let custom = Gf.ColorSpace(
  ///     name: "my_custom_space",
  ///     redChroma: Gf.Vec2f(0.64, 0.33),
  ///     greenChroma: Gf.Vec2f(0.30, 0.60),
  ///     blueChroma: Gf.Vec2f(0.15, 0.06),
  ///     whitePoint: Gf.Vec2f(0.3127, 0.3290),  // D65
  ///     gamma: 2.4,
  ///     linearBias: 0.0
  /// )
  /// ```
  init(name: String,
       redChroma: Gf.Vec2f,
       greenChroma: Gf.Vec2f,
       blueChroma: Gf.Vec2f,
       whitePoint: Gf.Vec2f,
       gamma: Float,
       linearBias: Float)
  {
    self.init(Tf.Token(name),
              redChroma,
              greenChroma,
              blueChroma,
              whitePoint,
              gamma,
              linearBias)
  }

  /// Create a custom color space from an RGB-to-XYZ matrix.
  ///
  /// - Parameters:
  ///   - name: A unique name for this custom color space.
  ///   - rgbToXYZ: The 3x3 matrix that transforms RGB to CIE XYZ.
  ///   - gamma: The gamma value for the transfer function (1.0 for linear).
  ///   - linearBias: The linear bias for the transfer function (0.0 for pure gamma).
  init(name: String,
       rgbToXYZ: Pixar.GfMatrix3f,
       gamma: Float,
       linearBias: Float)
  {
    self.init(Tf.Token(name),
              rgbToXYZ,
              gamma,
              linearBias)
  }
}

// MARK: - Static Methods

public extension Gf.ColorSpace
{
  /// Check if a color space name is valid for constructing a GfColorSpace.
  ///
  /// - Parameter name: The color space name to validate.
  /// - Returns: `true` if the name corresponds to a built-in color space.
  static func isValid(_ name: Name) -> Bool
  {
    Pixar.GfColorSpace.IsValid(name.token)
  }

  /// Check if a color space name token is valid for constructing a GfColorSpace.
  ///
  /// - Parameter token: The TfToken to validate.
  /// - Returns: `true` if the token corresponds to a built-in color space.
  static func isValid(_ token: Tf.Token) -> Bool
  {
    Pixar.GfColorSpace.IsValid(token)
  }
}

// MARK: - Instance Methods

public extension Gf.ColorSpace
{
  /// Get the name of this color space.
  ///
  /// - Returns: The name as a TfToken.
  func getName() -> Tf.Token
  {
    GetName()
  }

  /// Get the RGB to CIE XYZ conversion matrix.
  ///
  /// - Returns: The 3x3 transformation matrix.
  func getRGBToXYZ() -> Pixar.GfMatrix3f
  {
    GetRGBToXYZ()
  }

  /// Get the RGB to RGB conversion matrix from a source color space to this one.
  ///
  /// This matrix can be used to efficiently convert many colors at once
  /// by matrix multiplication (for linear color spaces).
  ///
  /// - Parameter source: The source color space.
  /// - Returns: The 3x3 transformation matrix.
  func getRGBToRGB(from source: Gf.ColorSpace) -> Pixar.GfMatrix3f
  {
    GetRGBToRGB(source)
  }

  /// Get the gamma value of this color space.
  ///
  /// - Returns: The gamma exponent (1.0 for linear spaces).
  func getGamma() -> Float
  {
    GetGamma()
  }

  /// Get the linear bias of this color space.
  ///
  /// The linear bias is used in transfer functions that have a linear
  /// segment near black (like sRGB).
  ///
  /// - Returns: The linear bias value (0.0 for pure gamma curves).
  func getLinearBias() -> Float
  {
    GetLinearBias()
  }

  /// Get the computed K0 and Phi values for use in the transfer function.
  ///
  /// These values define the transition point between the linear and
  /// gamma portions of the transfer function.
  ///
  /// - Returns: A tuple containing (k0, phi).
  func getTransferFunctionParams() -> (k0: Float, phi: Float)
  {
    let pair = GetTransferFunctionParams()
    return (pair.first, pair.second)
  }
}

// MARK: - Equatable

extension Gf.ColorSpace: Equatable
{
  public static func == (lhs: Gf.ColorSpace, rhs: Gf.ColorSpace) -> Bool
  {
    lhs.GetName() == rhs.GetName()
  }
}

// MARK: - Common Color Spaces (Convenience)

public extension Gf.ColorSpace
{
  /// sRGB color space - the most common color space for web and display content.
  static var srgb: Gf.ColorSpace { Gf.ColorSpace(.srgb) }

  /// ACEScg color space - the standard working space for VFX production.
  static var acescg: Gf.ColorSpace { Gf.ColorSpace(.acescg) }

  /// ACES2065-1 color space - the archival and interchange format for ACES.
  static var aces2065_1: Gf.ColorSpace { Gf.ColorSpace(.aces2065_1) }

  /// Linear Rec.709 - linear version of sRGB primaries.
  static var linearRec709: Gf.ColorSpace { Gf.ColorSpace(.linearRec709) }

  /// Linear P3-D65 - common for wide-gamut displays and Apple devices.
  static var linearP3D65: Gf.ColorSpace { Gf.ColorSpace(.linearP3D65) }

  /// Linear Rec.2020 - for HDR and ultra-wide gamut content.
  static var linearRec2020: Gf.ColorSpace { Gf.ColorSpace(.linearRec2020) }

  /// Raw/unmanaged color space.
  static var raw: Gf.ColorSpace { Gf.ColorSpace(.raw) }

  /// Data color space - for non-color data like normal maps.
  static var data: Gf.ColorSpace { Gf.ColorSpace(.data) }
}

// MARK: - CustomStringConvertible

extension Gf.ColorSpace: CustomStringConvertible
{
  public var description: String
  {
    "GfColorSpace(\(getName().string))"
  }
}
