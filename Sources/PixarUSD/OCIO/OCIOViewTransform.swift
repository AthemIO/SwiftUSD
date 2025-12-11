/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOViewTransformRcPtr = OpenColorIO_v2_4.ViewTransformRcPtr
public typealias OCIOConstViewTransformRcPtr = OpenColorIO_v2_4.ConstViewTransformRcPtr

public extension OCIO
{
  typealias ViewTransformRcPtr = OCIOViewTransformRcPtr
  typealias ConstViewTransformRcPtr = OCIOConstViewTransformRcPtr
}

// MARK: - ViewTransform Bridge Functions

public extension OCIO
{
  /// ViewTransform bridge functions.
  ///
  /// A ViewTransform converts between scene-referred and display-referred color spaces.
  enum ViewTransform
  {
    /// Check if a view transform pointer is valid.
    @inlinable
    public static func isValid(_ vt: OCIO.ConstViewTransformRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOViewTransform_isValid(vt)
    }

    /// Get the view transform name.
    @inlinable
    public static func getName(_ vt: OCIO.ConstViewTransformRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOViewTransform_getName(vt))
    }

    /// Get the family.
    @inlinable
    public static func getFamily(_ vt: OCIO.ConstViewTransformRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOViewTransform_getFamily(vt))
    }

    /// Get the description.
    @inlinable
    public static func getDescription(_ vt: OCIO.ConstViewTransformRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOViewTransform_getDescription(vt))
    }

    /// Get the reference space type.
    @inlinable
    public static func getReferenceSpaceType(_ vt: OCIO.ConstViewTransformRcPtr) -> OCIO.ReferenceSpaceType
    {
      OpenColorIO_v2_4.OCIOViewTransform_getReferenceSpaceType(vt)
    }

    /// Get the transform for a given direction.
    @inlinable
    public static func getTransform(
      _ vt: OCIO.ConstViewTransformRcPtr,
      direction: OCIO.ViewTransformDirection
    ) -> OCIO.ConstTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOViewTransform_getTransform(vt, direction)
    }
  }
}
