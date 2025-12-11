/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import OpenColorIO

// MARK: - Type Aliases for OCIO Enums

public extension OCIO
{
  /// Logging level for OCIO messages.
  typealias LoggingLevel = OpenColorIO_v2_4.LoggingLevel

  /// Bit depth for pixel data.
  typealias BitDepth = OpenColorIO_v2_4.BitDepth

  /// Transform direction (forward or inverse).
  typealias TransformDirection = OpenColorIO_v2_4.TransformDirection

  /// Reference space type (scene or display).
  typealias ReferenceSpaceType = OpenColorIO_v2_4.ReferenceSpaceType

  /// Search reference space type for queries.
  typealias SearchReferenceSpaceType = OpenColorIO_v2_4.SearchReferenceSpaceType

  /// Visibility filter for color spaces.
  typealias ColorSpaceVisibility = OpenColorIO_v2_4.ColorSpaceVisibility

  /// View transform direction.
  typealias ViewTransformDirection = OpenColorIO_v2_4.ViewTransformDirection

  /// Interpolation method for LUTs.
  typealias Interpolation = OpenColorIO_v2_4.Interpolation

  /// GPU shader language.
  typealias GpuLanguage = OpenColorIO_v2_4.GpuLanguage

  /// Environment mode for context variables.
  typealias EnvironmentMode = OpenColorIO_v2_4.EnvironmentMode

  /// Optimization flags for processors.
  typealias OptimizationFlags = OpenColorIO_v2_4.OptimizationFlags

  /// Transform type identifier.
  typealias TransformType = OpenColorIO_v2_4.TransformType

  /// Color space direction.
  typealias ColorSpaceDirection = OpenColorIO_v2_4.ColorSpaceDirection

  /// Fixed function style.
  typealias FixedFunctionStyle = OpenColorIO_v2_4.FixedFunctionStyle

  /// Exposure contrast style.
  typealias ExposureContrastStyle = OpenColorIO_v2_4.ExposureContrastStyle

  /// CDL style (ASC vs non-clamping).
  typealias CDLStyle = OpenColorIO_v2_4.CDLStyle

  /// Negative handling style.
  typealias NegativeStyle = OpenColorIO_v2_4.NegativeStyle

  /// Grading style.
  typealias GradingStyle = OpenColorIO_v2_4.GradingStyle

  /// Dynamic property type.
  typealias DynamicPropertyType = OpenColorIO_v2_4.DynamicPropertyType

  /// Uniform data type for GPU shaders.
  typealias UniformDataType = OpenColorIO_v2_4.UniformDataType

  /// GPU allocation mode.
  typealias GpuAllocation = OpenColorIO_v2_4.Allocation
}

// MARK: - Swift-Friendly Enum Extensions

public extension OCIO.LoggingLevel
{
  /// No logging.
  static let none = OpenColorIO_v2_4.LOGGING_LEVEL_NONE

  /// Warning messages only.
  static let warning = OpenColorIO_v2_4.LOGGING_LEVEL_WARNING

  /// Info and warning messages.
  static let info = OpenColorIO_v2_4.LOGGING_LEVEL_INFO

  /// Debug, info, and warning messages.
  static let debug = OpenColorIO_v2_4.LOGGING_LEVEL_DEBUG

  /// Unknown logging level.
  static let unknown = OpenColorIO_v2_4.LOGGING_LEVEL_UNKNOWN
}

public extension OCIO.BitDepth
{
  /// Unknown bit depth.
  static let unknown = OpenColorIO_v2_4.BIT_DEPTH_UNKNOWN

  /// 8-bit unsigned integer.
  static let uint8 = OpenColorIO_v2_4.BIT_DEPTH_UINT8

  /// 10-bit unsigned integer.
  static let uint10 = OpenColorIO_v2_4.BIT_DEPTH_UINT10

  /// 12-bit unsigned integer.
  static let uint12 = OpenColorIO_v2_4.BIT_DEPTH_UINT12

  /// 14-bit unsigned integer.
  static let uint14 = OpenColorIO_v2_4.BIT_DEPTH_UINT14

  /// 16-bit unsigned integer.
  static let uint16 = OpenColorIO_v2_4.BIT_DEPTH_UINT16

  /// 32-bit unsigned integer.
  static let uint32 = OpenColorIO_v2_4.BIT_DEPTH_UINT32

  /// 16-bit half float.
  static let float16 = OpenColorIO_v2_4.BIT_DEPTH_F16

  /// 32-bit float.
  static let float32 = OpenColorIO_v2_4.BIT_DEPTH_F32
}

public extension OCIO.TransformDirection
{
  /// Forward transform direction.
  static let forward = OpenColorIO_v2_4.TRANSFORM_DIR_FORWARD

  /// Inverse transform direction.
  static let inverse = OpenColorIO_v2_4.TRANSFORM_DIR_INVERSE
}

public extension OCIO.ReferenceSpaceType
{
  /// Scene-referred reference space.
  static let scene = OpenColorIO_v2_4.REFERENCE_SPACE_SCENE

  /// Display-referred reference space.
  static let display = OpenColorIO_v2_4.REFERENCE_SPACE_DISPLAY
}

public extension OCIO.SearchReferenceSpaceType
{
  /// Search in scene-referred space.
  static let scene = OpenColorIO_v2_4.SEARCH_REFERENCE_SPACE_SCENE

  /// Search in display-referred space.
  static let display = OpenColorIO_v2_4.SEARCH_REFERENCE_SPACE_DISPLAY

  /// Search in all reference spaces.
  static let all = OpenColorIO_v2_4.SEARCH_REFERENCE_SPACE_ALL
}

public extension OCIO.ColorSpaceVisibility
{
  /// Active color spaces only.
  static let active = OpenColorIO_v2_4.COLORSPACE_ACTIVE

  /// Inactive color spaces only.
  static let inactive = OpenColorIO_v2_4.COLORSPACE_INACTIVE

  /// All color spaces.
  static let all = OpenColorIO_v2_4.COLORSPACE_ALL
}

public extension OCIO.ViewTransformDirection
{
  /// Transform to reference space.
  static let toReference = OpenColorIO_v2_4.VIEWTRANSFORM_DIR_TO_REFERENCE

  /// Transform from reference space.
  static let fromReference = OpenColorIO_v2_4.VIEWTRANSFORM_DIR_FROM_REFERENCE
}

public extension OCIO.Interpolation
{
  /// Unknown interpolation.
  static let unknown = OpenColorIO_v2_4.INTERP_UNKNOWN

  /// Nearest neighbor interpolation.
  static let nearest = OpenColorIO_v2_4.INTERP_NEAREST

  /// Linear interpolation.
  static let linear = OpenColorIO_v2_4.INTERP_LINEAR

  /// Tetrahedral interpolation (best for 3D LUTs).
  static let tetrahedral = OpenColorIO_v2_4.INTERP_TETRAHEDRAL

  /// Cubic interpolation.
  static let cubic = OpenColorIO_v2_4.INTERP_CUBIC

  /// Default interpolation (best for the context).
  static let `default` = OpenColorIO_v2_4.INTERP_DEFAULT

  /// Best interpolation available.
  static let best = OpenColorIO_v2_4.INTERP_BEST
}

public extension OCIO.GpuLanguage
{
  /// GLSL 1.2 (OpenGL 2.1 / ES 2.0).
  static let glsl_1_2 = OpenColorIO_v2_4.GPU_LANGUAGE_GLSL_1_2

  /// GLSL 1.3 (OpenGL 3.0).
  static let glsl_1_3 = OpenColorIO_v2_4.GPU_LANGUAGE_GLSL_1_3

  /// GLSL 4.0 (OpenGL 4.0+).
  static let glsl_4_0 = OpenColorIO_v2_4.GPU_LANGUAGE_GLSL_4_0

  /// GLSL ES 1.0 (OpenGL ES 2.0).
  static let glslES_1_0 = OpenColorIO_v2_4.GPU_LANGUAGE_GLSL_ES_1_0

  /// GLSL ES 3.0 (OpenGL ES 3.0+).
  static let glslES_3_0 = OpenColorIO_v2_4.GPU_LANGUAGE_GLSL_ES_3_0

  /// HLSL Shader Model 5.0 (DirectX 11+).
  static let hlsl = OpenColorIO_v2_4.GPU_LANGUAGE_HLSL_DX11

  /// Metal Shading Language 2.0.
  static let msl_2_0 = OpenColorIO_v2_4.GPU_LANGUAGE_MSL_2_0
}

public extension OCIO.EnvironmentMode
{
  /// Load all environment variables.
  static let loadAll = OpenColorIO_v2_4.ENV_ENVIRONMENT_LOAD_ALL

  /// Load only predefined environment variables.
  static let loadPredefined = OpenColorIO_v2_4.ENV_ENVIRONMENT_LOAD_PREDEFINED

  /// Unknown environment mode.
  static let unknown = OpenColorIO_v2_4.ENV_ENVIRONMENT_UNKNOWN
}

public extension OCIO.OptimizationFlags
{
  /// No optimization.
  static let none = OpenColorIO_v2_4.OPTIMIZATION_NONE

  /// Lossless optimizations only.
  static let lossless = OpenColorIO_v2_4.OPTIMIZATION_LOSSLESS

  /// Very good quality optimizations.
  static let veryGood = OpenColorIO_v2_4.OPTIMIZATION_VERY_GOOD

  /// Good quality optimizations.
  static let good = OpenColorIO_v2_4.OPTIMIZATION_GOOD

  /// Draft quality optimizations (fastest).
  static let draft = OpenColorIO_v2_4.OPTIMIZATION_DRAFT

  /// Default optimizations.
  static let `default` = OpenColorIO_v2_4.OPTIMIZATION_DEFAULT

  /// All optimizations enabled.
  static let all = OpenColorIO_v2_4.OPTIMIZATION_ALL
}

public extension OCIO.CDLStyle
{
  /// No clamping (values can exceed 0-1 range).
  static let noClamping = OpenColorIO_v2_4.CDL_NO_CLAMP

  /// ASC standard clamping.
  static let asc = OpenColorIO_v2_4.CDL_ASC
}

public extension OCIO.GpuAllocation
{
  /// Unknown allocation.
  static let unknown = OpenColorIO_v2_4.ALLOCATION_UNKNOWN

  /// Uniform allocation.
  static let uniform = OpenColorIO_v2_4.ALLOCATION_UNIFORM

  /// Logarithmic allocation (better for wide dynamic range).
  static let log2 = OpenColorIO_v2_4.ALLOCATION_LG2
}
