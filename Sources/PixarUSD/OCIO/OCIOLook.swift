/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOLookRcPtr = OpenColorIO_v2_4.LookRcPtr
public typealias OCIOConstLookRcPtr = OpenColorIO_v2_4.ConstLookRcPtr

public extension OCIO
{
  typealias LookRcPtr = OCIOLookRcPtr
  typealias ConstLookRcPtr = OCIOConstLookRcPtr
}

// MARK: - Look Bridge Functions

public extension OCIO
{
  /// Look bridge functions.
  ///
  /// A Look represents a creative "look" or grade that can be applied to images.
  enum Look
  {
    /// Check if a look pointer is valid.
    @inlinable
    public static func isValid(_ look: OCIO.ConstLookRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOLook_isValid(look)
    }

    /// Get the look name.
    @inlinable
    public static func getName(_ look: OCIO.ConstLookRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOLook_getName(look))
    }

    /// Get the process space.
    ///
    /// The process space is the color space where this look's transform is applied.
    @inlinable
    public static func getProcessSpace(_ look: OCIO.ConstLookRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOLook_getProcessSpace(look))
    }

    /// Get the description.
    @inlinable
    public static func getDescription(_ look: OCIO.ConstLookRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOLook_getDescription(look))
    }

    /// Get the forward transform.
    @inlinable
    public static func getTransform(_ look: OCIO.ConstLookRcPtr) -> OCIO.ConstTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOLook_getTransform(look)
    }

    /// Get the inverse transform.
    @inlinable
    public static func getInverseTransform(_ look: OCIO.ConstLookRcPtr) -> OCIO.ConstTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOLook_getInverseTransform(look)
    }
  }
}
