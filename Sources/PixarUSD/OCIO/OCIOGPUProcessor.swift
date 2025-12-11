/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOGPUProcessorRcPtr = OpenColorIO_v2_4.GPUProcessorRcPtr
public typealias OCIOConstGPUProcessorRcPtr = OpenColorIO_v2_4.ConstGPUProcessorRcPtr

public extension OCIO
{
  typealias GPUProcessorRcPtr = OCIOGPUProcessorRcPtr
  typealias ConstGPUProcessorRcPtr = OCIOConstGPUProcessorRcPtr
}

// MARK: - GPUProcessor Bridge Functions

public extension OCIO
{
  /// GPUProcessor bridge functions.
  ///
  /// A GPUProcessor generates shader code and 3D LUT textures for
  /// GPU-accelerated color processing.
  enum GPUProcessor
  {
    /// Check if a GPU processor pointer is valid.
    @inlinable
    public static func isValid(_ gpu: OCIO.ConstGPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOGPUProcessor_isValid(gpu)
    }

    /// Check if this processor is a no-op (identity transform).
    @inlinable
    public static func isNoOp(_ gpu: OCIO.ConstGPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOGPUProcessor_isNoOp(gpu)
    }

    /// Check if this processor has channel crosstalk.
    @inlinable
    public static func hasChannelCrosstalk(_ gpu: OCIO.ConstGPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOGPUProcessor_hasChannelCrosstalk(gpu)
    }

    /// Get a cache ID for this processor.
    @inlinable
    public static func getCacheID(_ gpu: OCIO.ConstGPUProcessorRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOGPUProcessor_getCacheID(gpu))
    }
  }
}

// MARK: - GpuShaderDesc

public typealias OCIOGpuShaderDescRcPtr = OpenColorIO_v2_4.GpuShaderDescRcPtr

public extension OCIO
{
  typealias GpuShaderDescRcPtr = OCIOGpuShaderDescRcPtr
}

// Note: GpuShaderDesc creation requires C++ bridge functions that are not
// yet implemented. For GPU shader generation, use the C++ API directly
// or add bridge functions as needed.

// MARK: - GpuShaderCreator

public typealias OCIOGpuShaderCreatorRcPtr = OpenColorIO_v2_4.GpuShaderCreatorRcPtr

public extension OCIO
{
  typealias GpuShaderCreatorRcPtr = OCIOGpuShaderCreatorRcPtr
}
