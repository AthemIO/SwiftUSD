/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Base Transform Type Aliases

public typealias OCIOTransformRcPtr = OpenColorIO_v2_4.TransformRcPtr
public typealias OCIOConstTransformRcPtr = OpenColorIO_v2_4.ConstTransformRcPtr

public extension OCIO
{
  typealias TransformRcPtr = OCIOTransformRcPtr
  typealias ConstTransformRcPtr = OCIOConstTransformRcPtr
}

// MARK: - Transform Creation Functions

public extension OCIO
{
  /// Transform creation functions.
  ///
  /// Use these to create OCIO transforms for color processing.
  enum Transforms
  {
    /// Create a new ColorSpaceTransform.
    @inlinable
    public static func createColorSpaceTransform() -> OCIO.ColorSpaceTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOColorSpaceTransform_Create()
    }

    /// Create a new DisplayViewTransform.
    @inlinable
    public static func createDisplayViewTransform() -> OCIO.DisplayViewTransformRcPtr
    {
      OpenColorIO_v2_4.OCIODisplayViewTransform_Create()
    }

    /// Create a new MatrixTransform.
    @inlinable
    public static func createMatrixTransform() -> OCIO.MatrixTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOMatrixTransform_Create()
    }

    /// Create a new ExponentTransform.
    @inlinable
    public static func createExponentTransform() -> OCIO.ExponentTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOExponentTransform_Create()
    }

    /// Create a new LogTransform.
    @inlinable
    public static func createLogTransform() -> OCIO.LogTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOLogTransform_Create()
    }

    /// Create a new LookTransform.
    @inlinable
    public static func createLookTransform() -> OCIO.LookTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOLookTransform_Create()
    }

    /// Create a new GroupTransform.
    @inlinable
    public static func createGroupTransform() -> OCIO.GroupTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOGroupTransform_Create()
    }

    /// Create a new CDLTransform.
    @inlinable
    public static func createCDLTransform() -> OCIO.CDLTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOCDLTransform_Create()
    }

    /// Create a new FileTransform.
    @inlinable
    public static func createFileTransform() -> OCIO.FileTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOFileTransform_Create()
    }

    /// Create a new RangeTransform.
    @inlinable
    public static func createRangeTransform() -> OCIO.RangeTransformRcPtr
    {
      OpenColorIO_v2_4.OCIORangeTransform_Create()
    }
  }
}

// MARK: - Transform Type Aliases

public typealias OCIOColorSpaceTransformRcPtr = OpenColorIO_v2_4.ColorSpaceTransformRcPtr
public typealias OCIOConstColorSpaceTransformRcPtr = OpenColorIO_v2_4.ConstColorSpaceTransformRcPtr

public typealias OCIODisplayViewTransformRcPtr = OpenColorIO_v2_4.DisplayViewTransformRcPtr
public typealias OCIOConstDisplayViewTransformRcPtr = OpenColorIO_v2_4.ConstDisplayViewTransformRcPtr

public typealias OCIOMatrixTransformRcPtr = OpenColorIO_v2_4.MatrixTransformRcPtr
public typealias OCIOConstMatrixTransformRcPtr = OpenColorIO_v2_4.ConstMatrixTransformRcPtr

public typealias OCIOExponentTransformRcPtr = OpenColorIO_v2_4.ExponentTransformRcPtr
public typealias OCIOConstExponentTransformRcPtr = OpenColorIO_v2_4.ConstExponentTransformRcPtr

public typealias OCIOLogTransformRcPtr = OpenColorIO_v2_4.LogTransformRcPtr
public typealias OCIOConstLogTransformRcPtr = OpenColorIO_v2_4.ConstLogTransformRcPtr

public typealias OCIOLookTransformRcPtr = OpenColorIO_v2_4.LookTransformRcPtr
public typealias OCIOConstLookTransformRcPtr = OpenColorIO_v2_4.ConstLookTransformRcPtr

public typealias OCIOGroupTransformRcPtr = OpenColorIO_v2_4.GroupTransformRcPtr
public typealias OCIOConstGroupTransformRcPtr = OpenColorIO_v2_4.ConstGroupTransformRcPtr

public typealias OCIOCDLTransformRcPtr = OpenColorIO_v2_4.CDLTransformRcPtr
public typealias OCIOConstCDLTransformRcPtr = OpenColorIO_v2_4.ConstCDLTransformRcPtr

public typealias OCIOFileTransformRcPtr = OpenColorIO_v2_4.FileTransformRcPtr
public typealias OCIOConstFileTransformRcPtr = OpenColorIO_v2_4.ConstFileTransformRcPtr

public typealias OCIORangeTransformRcPtr = OpenColorIO_v2_4.RangeTransformRcPtr
public typealias OCIOConstRangeTransformRcPtr = OpenColorIO_v2_4.ConstRangeTransformRcPtr

public extension OCIO
{
  typealias ColorSpaceTransformRcPtr = OCIOColorSpaceTransformRcPtr
  typealias ConstColorSpaceTransformRcPtr = OCIOConstColorSpaceTransformRcPtr

  typealias DisplayViewTransformRcPtr = OCIODisplayViewTransformRcPtr
  typealias ConstDisplayViewTransformRcPtr = OCIOConstDisplayViewTransformRcPtr

  typealias MatrixTransformRcPtr = OCIOMatrixTransformRcPtr
  typealias ConstMatrixTransformRcPtr = OCIOConstMatrixTransformRcPtr

  typealias ExponentTransformRcPtr = OCIOExponentTransformRcPtr
  typealias ConstExponentTransformRcPtr = OCIOConstExponentTransformRcPtr

  typealias LogTransformRcPtr = OCIOLogTransformRcPtr
  typealias ConstLogTransformRcPtr = OCIOConstLogTransformRcPtr

  typealias LookTransformRcPtr = OCIOLookTransformRcPtr
  typealias ConstLookTransformRcPtr = OCIOConstLookTransformRcPtr

  typealias GroupTransformRcPtr = OCIOGroupTransformRcPtr
  typealias ConstGroupTransformRcPtr = OCIOConstGroupTransformRcPtr

  typealias CDLTransformRcPtr = OCIOCDLTransformRcPtr
  typealias ConstCDLTransformRcPtr = OCIOConstCDLTransformRcPtr

  typealias FileTransformRcPtr = OCIOFileTransformRcPtr
  typealias ConstFileTransformRcPtr = OCIOConstFileTransformRcPtr

  typealias RangeTransformRcPtr = OCIORangeTransformRcPtr
  typealias ConstRangeTransformRcPtr = OCIOConstRangeTransformRcPtr
}
