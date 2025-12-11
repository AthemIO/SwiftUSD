/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOCPUProcessorRcPtr = OpenColorIO_v2_4.CPUProcessorRcPtr
public typealias OCIOConstCPUProcessorRcPtr = OpenColorIO_v2_4.ConstCPUProcessorRcPtr

public extension OCIO
{
  typealias CPUProcessorRcPtr = OCIOCPUProcessorRcPtr
  typealias ConstCPUProcessorRcPtr = OCIOConstCPUProcessorRcPtr
}

// MARK: - CPUProcessor Bridge Functions

public extension OCIO
{
  /// CPUProcessor bridge functions.
  ///
  /// A CPUProcessor applies color transformations to image data on the CPU.
  /// Get a CPUProcessor from Processor.getDefaultCPUProcessor().
  ///
  /// ## Usage
  /// ```swift
  /// let config = OCIO.getCurrentConfig()
  /// let processor = OCIO.Config.getProcessor(config, from: "ACEScg", to: "sRGB")
  /// let cpu = OCIO.Processor.getDefaultCPUProcessor(processor)
  ///
  /// var pixel: [Float] = [1.0, 0.5, 0.2, 1.0]
  /// OCIO.CPUProcessor.applyRGBA(cpu, &pixel)
  /// ```
  enum CPUProcessor
  {
    /// Check if a CPU processor pointer is valid.
    @inlinable
    public static func isValid(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_isValid(cpu)
    }

    /// Check if this processor is a no-op (identity transform).
    @inlinable
    public static func isNoOp(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_isNoOp(cpu)
    }

    /// Check if this processor is exactly an identity transform.
    @inlinable
    public static func isIdentity(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_isIdentity(cpu)
    }

    /// Check if this processor has channel crosstalk.
    @inlinable
    public static func hasChannelCrosstalk(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_hasChannelCrosstalk(cpu)
    }

    /// Get a cache ID for this processor.
    @inlinable
    public static func getCacheID(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOCPUProcessor_getCacheID(cpu))
    }

    /// Get the input bit depth.
    @inlinable
    public static func getInputBitDepth(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> OCIO.BitDepth
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_getInputBitDepth(cpu)
    }

    /// Get the output bit depth.
    @inlinable
    public static func getOutputBitDepth(_ cpu: OCIO.ConstCPUProcessorRcPtr) -> OCIO.BitDepth
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_getOutputBitDepth(cpu)
    }

    /// Apply the transform to a single RGB pixel in-place.
    ///
    /// - Parameters:
    ///   - cpu: The CPU processor.
    ///   - pixel: A pointer to 3 float values (R, G, B).
    @inlinable
    public static func applyRGB(_ cpu: OCIO.ConstCPUProcessorRcPtr, _ pixel: UnsafeMutablePointer<Float>)
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_applyRGB(cpu, pixel)
    }

    /// Apply the transform to a single RGBA pixel in-place.
    ///
    /// - Parameters:
    ///   - cpu: The CPU processor.
    ///   - pixel: A pointer to 4 float values (R, G, B, A).
    @inlinable
    public static func applyRGBA(_ cpu: OCIO.ConstCPUProcessorRcPtr, _ pixel: UnsafeMutablePointer<Float>)
    {
      OpenColorIO_v2_4.OCIOCPUProcessor_applyRGBA(cpu, pixel)
    }

    /// Apply the transform to an RGB pixel array.
    ///
    /// - Parameters:
    ///   - cpu: The CPU processor.
    ///   - pixel: An array of at least 3 float values.
    @inlinable
    public static func applyRGB(_ cpu: OCIO.ConstCPUProcessorRcPtr, _ pixel: inout [Float])
    {
      precondition(pixel.count >= 3, "RGB pixel must have at least 3 components")
      pixel.withUnsafeMutableBufferPointer { buffer in
        OpenColorIO_v2_4.OCIOCPUProcessor_applyRGB(cpu, buffer.baseAddress!)
      }
    }

    /// Apply the transform to an RGBA pixel array.
    ///
    /// - Parameters:
    ///   - cpu: The CPU processor.
    ///   - pixel: An array of at least 4 float values.
    @inlinable
    public static func applyRGBA(_ cpu: OCIO.ConstCPUProcessorRcPtr, _ pixel: inout [Float])
    {
      precondition(pixel.count >= 4, "RGBA pixel must have at least 4 components")
      pixel.withUnsafeMutableBufferPointer { buffer in
        OpenColorIO_v2_4.OCIOCPUProcessor_applyRGBA(cpu, buffer.baseAddress!)
      }
    }
  }
}

// Note: PackedImageDesc and ImageDesc are not directly accessible from Swift
// due to C++ interop limitations. Image processing should use the
// applyRGB/applyRGBA functions for individual pixels, or integrate with
// OCIO via C++ code for full image processing.
