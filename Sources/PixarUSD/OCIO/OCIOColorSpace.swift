/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOColorSpaceRcPtr = OpenColorIO_v2_4.ColorSpaceRcPtr
public typealias OCIOConstColorSpaceRcPtr = OpenColorIO_v2_4.ConstColorSpaceRcPtr

public extension OCIO
{
  typealias ColorSpaceRcPtr = OCIOColorSpaceRcPtr
  typealias ConstColorSpaceRcPtr = OCIOConstColorSpaceRcPtr
}

// MARK: - ColorSpace Bridge Functions

public extension OCIO
{
  /// ColorSpace bridge functions.
  ///
  /// Due to Swift C++ interop limitations with `std::shared_ptr`,
  /// ColorSpace methods are exposed as static functions that take
  /// the shared_ptr as the first argument.
  enum ColorSpace
  {
    /// Check if a color space pointer is valid.
    @inlinable
    public static func isValid(_ cs: OCIO.ConstColorSpaceRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOColorSpace_isValid(cs)
    }

    /// Get the name of the color space.
    @inlinable
    public static func getName(_ cs: OCIO.ConstColorSpaceRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOColorSpace_getName(cs))
    }

    /// Get the family of the color space.
    @inlinable
    public static func getFamily(_ cs: OCIO.ConstColorSpaceRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOColorSpace_getFamily(cs))
    }

    /// Get the description of the color space.
    @inlinable
    public static func getDescription(_ cs: OCIO.ConstColorSpaceRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOColorSpace_getDescription(cs))
    }

    /// Get the encoding of the color space (e.g., "scene-linear", "sdr-video").
    @inlinable
    public static func getEncoding(_ cs: OCIO.ConstColorSpaceRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOColorSpace_getEncoding(cs))
    }

    /// Get the bit depth of the color space.
    @inlinable
    public static func getBitDepth(_ cs: OCIO.ConstColorSpaceRcPtr) -> OCIO.BitDepth
    {
      OpenColorIO_v2_4.OCIOColorSpace_getBitDepth(cs)
    }

    /// Check if this is a data (non-color) color space.
    @inlinable
    public static func isData(_ cs: OCIO.ConstColorSpaceRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOColorSpace_isData(cs)
    }

    /// Get the reference space type (scene or display).
    @inlinable
    public static func getReferenceSpaceType(_ cs: OCIO.ConstColorSpaceRcPtr) -> OCIO.ReferenceSpaceType
    {
      OpenColorIO_v2_4.OCIOColorSpace_getReferenceSpaceType(cs)
    }

    /// Get the transform for a given direction.
    @inlinable
    public static func getTransform(
      _ cs: OCIO.ConstColorSpaceRcPtr,
      direction: OCIO.ColorSpaceDirection
    ) -> OCIO.ConstTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOColorSpace_getTransform(cs, direction)
    }
  }
}
