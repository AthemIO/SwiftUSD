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
 * # GfColor
 *
 * Represents a color in a specific color space.
 *
 * The GfColor class represents a color in a specific color space. It provides
 * various methods for constructing, manipulating, and retrieving color values.
 *
 * The color values are stored as an RGB tuple and are associated with a color
 * space. The color space determines the interpretation of the RGB values. The
 * values are colorimetric, but not photometric as there is no normalizing
 * constant (such as a luminance factor).
 */
public typealias GfColor = Pixar.GfColor

public extension Gf
{
  /**
   * # Gf.Color
   *
   * Represents a color in a specific color space.
   *
   * The GfColor class represents a color in a specific color space. It provides
   * various methods for constructing, manipulating, and retrieving color values.
   *
   * The color values are stored as an RGB tuple and are associated with a color
   * space. The color space determines the interpretation of the RGB values.
   */
  typealias Color = GfColor
}

// MARK: - Constructors

public extension Gf.Color
{
  /// Create a black color in the specified named color space.
  ///
  /// - Parameter colorSpace: The color space for this color.
  ///
  /// Example:
  /// ```swift
  /// let black = Gf.Color(in: .acescg)
  /// ```
  init(in colorSpace: Gf.ColorSpace.Name)
  {
    self.init(Gf.ColorSpace(colorSpace))
  }

  /// Create a color from RGB values in the specified named color space.
  ///
  /// - Parameters:
  ///   - rgb: The RGB values (red, green, blue).
  ///   - colorSpace: The color space for interpreting the RGB values.
  ///
  /// Example:
  /// ```swift
  /// let red = Gf.Color(rgb: Gf.Vec3f(1, 0, 0), in: .srgb)
  /// ```
  init(rgb: Gf.Vec3f, in colorSpace: Gf.ColorSpace.Name)
  {
    self.init(rgb, Gf.ColorSpace(colorSpace))
  }

  /// Create a color from RGB values in the specified color space.
  ///
  /// - Parameters:
  ///   - rgb: The RGB values (red, green, blue).
  ///   - colorSpace: The color space for interpreting the RGB values.
  init(rgb: Gf.Vec3f, in colorSpace: Gf.ColorSpace)
  {
    self.init(rgb, colorSpace)
  }

  /// Create a color from individual RGB components in the specified named color space.
  ///
  /// - Parameters:
  ///   - red: The red component.
  ///   - green: The green component.
  ///   - blue: The blue component.
  ///   - colorSpace: The color space for interpreting the RGB values.
  ///
  /// Example:
  /// ```swift
  /// let color = Gf.Color(red: 0.5, green: 0.8, blue: 0.2, in: .srgb)
  /// ```
  init(red: Float, green: Float, blue: Float, in colorSpace: Gf.ColorSpace.Name)
  {
    self.init(Gf.Vec3f(red, green, blue), Gf.ColorSpace(colorSpace))
  }

  /// Create a color from individual RGB components in the specified color space.
  ///
  /// - Parameters:
  ///   - red: The red component.
  ///   - green: The green component.
  ///   - blue: The blue component.
  ///   - colorSpace: The color space for interpreting the RGB values.
  init(red: Float, green: Float, blue: Float, in colorSpace: Gf.ColorSpace)
  {
    self.init(Gf.Vec3f(red, green, blue), colorSpace)
  }

  /// Create a color by converting from another color to a new named color space.
  ///
  /// - Parameters:
  ///   - color: The source color to convert.
  ///   - colorSpace: The target color space.
  ///
  /// Example:
  /// ```swift
  /// let srgbRed = Gf.Color(rgb: Gf.Vec3f(1, 0, 0), in: .srgb)
  /// let acescgRed = Gf.Color(converting: srgbRed, to: .acescg)
  /// ```
  init(converting color: Gf.Color, to colorSpace: Gf.ColorSpace.Name)
  {
    self.init(color, Gf.ColorSpace(colorSpace))
  }

  /// Create a color by converting from another color to a new color space.
  ///
  /// - Parameters:
  ///   - color: The source color to convert.
  ///   - colorSpace: The target color space.
  init(converting color: Gf.Color, to colorSpace: Gf.ColorSpace)
  {
    self.init(color, colorSpace)
  }
}

// MARK: - Instance Methods

public extension Gf.Color
{
  /// Set the color from the Planckian locus (blackbody radiation) temperature.
  ///
  /// Values are computed for temperatures between 1000K and 15000K.
  /// Note that temperatures below 1900K are out of gamut for Rec.709.
  ///
  /// - Parameters:
  ///   - kelvin: The temperature in Kelvin (1000-15000).
  ///   - luminance: The desired luminance.
  ///
  /// Example:
  /// ```swift
  /// var sunlight = Gf.Color(in: .linearRec709)
  /// sunlight.setFromPlanckianLocus(kelvin: 5778, luminance: 1.0)
  /// ```
  mutating func setFromPlanckianLocus(kelvin: Float, luminance: Float)
  {
    SetFromPlanckianLocus(kelvin, luminance)
  }

  /// Get the RGB values of this color.
  ///
  /// - Returns: The RGB values as a vector.
  func getRGB() -> Gf.Vec3f
  {
    GetRGB()
  }

  /// Get the color space of this color.
  ///
  /// - Returns: The color space.
  func getColorSpace() -> Gf.ColorSpace
  {
    GetColorSpace()
  }

  /// The red component of this color.
  var red: Float
  {
    GetRGB()[0]
  }

  /// The green component of this color.
  var green: Float
  {
    GetRGB()[1]
  }

  /// The blue component of this color.
  var blue: Float
  {
    GetRGB()[2]
  }

  /// Convert this color to a different named color space.
  ///
  /// - Parameter colorSpace: The target color space.
  /// - Returns: A new color in the target color space.
  ///
  /// Example:
  /// ```swift
  /// let srgbColor = Gf.Color(rgb: Gf.Vec3f(0.5, 0.5, 0.5), in: .srgb)
  /// let linearColor = srgbColor.converted(to: .linearRec709)
  /// ```
  func converted(to colorSpace: Gf.ColorSpace.Name) -> Gf.Color
  {
    Gf.Color(converting: self, to: colorSpace)
  }

  /// Convert this color to a different color space.
  ///
  /// - Parameter colorSpace: The target color space.
  /// - Returns: A new color in the target color space.
  func converted(to colorSpace: Gf.ColorSpace) -> Gf.Color
  {
    Gf.Color(converting: self, to: colorSpace)
  }
}

// MARK: - Equatable

extension Gf.Color: Equatable
{
  public static func == (lhs: Gf.Color, rhs: Gf.Color) -> Bool
  {
    lhs.GetRGB() == rhs.GetRGB() && lhs.GetColorSpace() == rhs.GetColorSpace()
  }
}

// MARK: - Approximate Equality

public extension Gf.Color
{
  /// Check if two colors are approximately equal within a tolerance.
  ///
  /// This comparison does not adapt the colors to the same color space before
  /// comparing, and is not a perceptual comparison. Both colors should be in
  /// the same color space for meaningful results.
  ///
  /// - Parameters:
  ///   - other: The color to compare against.
  ///   - tolerance: The maximum allowed difference.
  /// - Returns: `true` if the colors are within tolerance.
  func isClose(to other: Gf.Color, tolerance: Double) -> Bool
  {
    Pixar.GfIsClose(self, other, tolerance)
  }
}

// MARK: - Common Colors (Convenience)

public extension Gf.Color
{
  /// Black in Linear Rec.709 color space (the default).
  static var black: Gf.Color { Gf.Color() }

  /// White in sRGB color space.
  static var white: Gf.Color
  {
    Gf.Color(red: 1.0, green: 1.0, blue: 1.0, in: Gf.ColorSpace.Name.srgb)
  }

  /// Create a grayscale color in sRGB.
  ///
  /// - Parameter value: The gray value (0.0 = black, 1.0 = white).
  /// - Returns: A grayscale color.
  static func gray(_ value: Float) -> Gf.Color
  {
    Gf.Color(red: value, green: value, blue: value, in: Gf.ColorSpace.Name.srgb)
  }

  /// Create a color from a blackbody temperature.
  ///
  /// This computes the color of a perfect blackbody radiator at the
  /// given temperature. Useful for physically-based lighting.
  ///
  /// Common temperatures:
  /// - 1850K: Candle flame
  /// - 2700K: Incandescent bulb
  /// - 3200K: Studio tungsten
  /// - 5000K: Horizon daylight
  /// - 5778K: Sun surface
  /// - 6500K: Average daylight (D65)
  /// - 10000K: Blue sky
  ///
  /// - Parameters:
  ///   - kelvin: Temperature in Kelvin (1000-15000).
  ///   - luminance: The luminance value (default 1.0).
  ///   - colorSpace: The color space for the result (default linear Rec.709).
  /// - Returns: A color representing the blackbody radiation.
  static func fromPlanckianLocus(
    kelvin: Float,
    luminance: Float = 1.0,
    in colorSpace: Gf.ColorSpace.Name = .linearRec709
  ) -> Gf.Color
  {
    var color = Gf.Color(in: colorSpace)
    color.setFromPlanckianLocus(kelvin: kelvin, luminance: luminance)
    return color
  }
}

// MARK: - CustomStringConvertible

extension Gf.Color: CustomStringConvertible
{
  public var description: String
  {
    let rgb = getRGB()
    let colorSpaceName = getColorSpace().getName().string
    return "GfColor(r: \(rgb[0]), g: \(rgb[1]), b: \(rgb[2]), space: \(colorSpaceName))"
  }
}
