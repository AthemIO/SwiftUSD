/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOProcessorRcPtr = OpenColorIO_v2_4.ProcessorRcPtr
public typealias OCIOConstProcessorRcPtr = OpenColorIO_v2_4.ConstProcessorRcPtr

public extension OCIO
{
  typealias ProcessorRcPtr = OCIOProcessorRcPtr
  typealias ConstProcessorRcPtr = OCIOConstProcessorRcPtr
}

// MARK: - Processor Bridge Functions

public extension OCIO
{
  /// Processor bridge functions.
  ///
  /// A Processor represents a compiled color transformation pipeline.
  /// Get a Processor from Config.getProcessor(), then use these methods
  /// to access CPU/GPU processors for actual color processing.
  enum Processor
  {
    /// Check if a processor pointer is valid.
    @inlinable
    public static func isValid(_ proc: OCIO.ConstProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOProcessor_isValid(proc)
    }

    /// Check if this processor is a no-op (identity transform).
    @inlinable
    public static func isNoOp(_ proc: OCIO.ConstProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOProcessor_isNoOp(proc)
    }

    /// Check if this processor has channel crosstalk.
    @inlinable
    public static func hasChannelCrosstalk(_ proc: OCIO.ConstProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOProcessor_hasChannelCrosstalk(proc)
    }

    /// Get a cache ID for this processor.
    @inlinable
    public static func getCacheID(_ proc: OCIO.ConstProcessorRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOProcessor_getCacheID(proc))
    }

    /// Get the number of transforms in this processor.
    @inlinable
    public static func getNumTransforms(_ proc: OCIO.ConstProcessorRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOProcessor_getNumTransforms(proc)
    }

    /// Get the default CPU processor.
    ///
    /// This returns a CPU processor optimized with default settings.
    @inlinable
    public static func getDefaultCPUProcessor(_ proc: OCIO.ConstProcessorRcPtr) -> OCIO.ConstCPUProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOProcessor_getDefaultCPUProcessor(proc)
    }

    /// Get an optimized CPU processor.
    @inlinable
    public static func getOptimizedCPUProcessor(
      _ proc: OCIO.ConstProcessorRcPtr,
      flags: OCIO.OptimizationFlags
    ) -> OCIO.ConstCPUProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOProcessor_getOptimizedCPUProcessor(proc, flags)
    }

    /// Get an optimized CPU processor with bit depth control.
    @inlinable
    public static func getOptimizedCPUProcessor(
      _ proc: OCIO.ConstProcessorRcPtr,
      inputBitDepth: OCIO.BitDepth,
      outputBitDepth: OCIO.BitDepth,
      flags: OCIO.OptimizationFlags
    ) -> OCIO.ConstCPUProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOProcessor_getOptimizedCPUProcessorWithBitDepth(proc, inputBitDepth, outputBitDepth, flags)
    }

    /// Get the default GPU processor.
    ///
    /// This returns a GPU processor optimized with default settings.
    @inlinable
    public static func getDefaultGPUProcessor(_ proc: OCIO.ConstProcessorRcPtr) -> OCIO.ConstGPUProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOProcessor_getDefaultGPUProcessor(proc)
    }

    /// Get an optimized GPU processor.
    @inlinable
    public static func getOptimizedGPUProcessor(
      _ proc: OCIO.ConstProcessorRcPtr,
      flags: OCIO.OptimizationFlags
    ) -> OCIO.ConstGPUProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOProcessor_getOptimizedGPUProcessor(proc, flags)
    }
  }
}
