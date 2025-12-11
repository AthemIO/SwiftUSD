/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Namespace

/**
 * # OCIO
 *
 * OpenColorIO (OCIO) is the industry-standard color management library
 * used throughout VFX and animation production pipelines.
 *
 * ## Overview
 *
 * OpenColorIO provides a consistent color management solution across
 * applications and pipelines. It's built around the concept of a
 * configuration file that defines all the colorspaces, looks, and
 * display transforms available in a production.
 *
 * ## Basic Usage
 *
 * ```swift
 * // Get the current config (from OCIO environment variable)
 * let config = OCIO.getCurrentConfig()
 *
 * // Get a processor to convert between color spaces
 * let processor = try config.getProcessor(
 *     from: "ACEScg",
 *     to: "sRGB - Display"
 * )
 *
 * // Get a CPU processor for image processing
 * let cpuProcessor = processor.getDefaultCPUProcessor()
 *
 * // Apply to image data
 * cpuProcessor.applyRGBA(&pixelData)
 * ```
 *
 * ## Configuration
 *
 * OCIO configurations can be loaded from:
 * - The `OCIO` environment variable (default)
 * - A file path
 * - Built-in configs (ACES, CG config)
 *
 * ```swift
 * // Load from environment
 * let config = OCIO.Config.createFromEnv()
 *
 * // Load from file
 * let config = OCIO.Config.createFromFile("/path/to/config.ocio")
 *
 * // Use built-in ACES config
 * let config = OCIO.Config.createFromBuiltinConfig(
 *     "studio-config-v2.1.0_aces-v1.3_ocio-v2.3"
 * )
 * ```
 */
public enum OCIO {}

// MARK: - Global Functions

public extension OCIO
{
  /// Get the current global OpenColorIO configuration.
  ///
  /// This returns the config that was set via `setCurrentConfig()`,
  /// or auto-initializes from the `OCIO` environment variable on first use.
  ///
  /// - Returns: The current configuration.
  @inlinable
  static func getCurrentConfig() -> OCIO.ConstConfigRcPtr
  {
    OpenColorIO_v2_4.GetCurrentConfig()
  }

  /// Set the current global OpenColorIO configuration.
  ///
  /// This configuration will be used by default when applications
  /// request the "current" config.
  ///
  /// - Parameter config: The configuration to set as current.
  @inlinable
  static func setCurrentConfig(_ config: OCIO.ConstConfigRcPtr)
  {
    OpenColorIO_v2_4.SetCurrentConfig(config)
  }

  /// Clear all global OCIO caches.
  ///
  /// This flushes cached LUT contents, intermediate results, and
  /// other global information. Useful when designing OCIO profiles
  /// and wanting to re-read LUTs without restarting.
  ///
  /// - Note: This does not clear instance-specific caches like
  ///   the Processor cache in a Config instance.
  @inlinable
  static func clearAllCaches()
  {
    OpenColorIO_v2_4.ClearAllCaches()
  }

  /// Get the OCIO library version as a string (e.g., "2.4.2").
  ///
  /// - Returns: The version string.
  @inlinable
  static func getVersion() -> String
  {
    String(cString: OpenColorIO_v2_4.GetVersion())
  }

  /// Get the OCIO library version as a hex number.
  ///
  /// The format is `0xMMmmpp00` where MM=major, mm=minor, pp=patch.
  /// For example, version 2.4.2 would be `0x02040200`.
  ///
  /// - Returns: The version as a hex integer.
  @inlinable
  static func getVersionHex() -> Int32
  {
    OpenColorIO_v2_4.GetVersionHex()
  }

  /// Get the current logging level.
  ///
  /// - Returns: The current logging level.
  @inlinable
  static func getLoggingLevel() -> OCIO.LoggingLevel
  {
    OpenColorIO_v2_4.GetLoggingLevel()
  }

  /// Set the global logging level.
  ///
  /// - Parameter level: The logging level to set.
  @inlinable
  static func setLoggingLevel(_ level: OCIO.LoggingLevel)
  {
    OpenColorIO_v2_4.SetLoggingLevel(level)
  }

  /// Reset the logging function to the default (stderr).
  @inlinable
  static func resetToDefaultLoggingFunction()
  {
    OpenColorIO_v2_4.ResetToDefaultLoggingFunction()
  }

  /// Log a message using OCIO's logging system.
  ///
  /// - Parameters:
  ///   - level: The logging level for the message.
  ///   - message: The message to log.
  @inlinable
  static func logMessage(_ level: OCIO.LoggingLevel, _ message: String)
  {
    message.withCString { cStr in
      OpenColorIO_v2_4.LogMessage(level, cStr)
    }
  }

  /// Get an environment variable value.
  ///
  /// - Parameter name: The environment variable name.
  /// - Returns: The value, or nil if not set.
  /// - Warning: This method is not thread safe.
  @inlinable
  static func getEnvVariable(_ name: String) -> String?
  {
    let result = name.withCString { cStr in
      OpenColorIO_v2_4.GetEnvVariable(cStr)
    }
    guard let result else { return nil }
    let str = String(cString: result)
    return str.isEmpty ? nil : str
  }

  /// Set an environment variable value.
  ///
  /// - Parameters:
  ///   - name: The environment variable name.
  ///   - value: The value to set.
  /// - Warning: This method is not thread safe.
  @inlinable
  static func setEnvVariable(_ name: String, _ value: String)
  {
    name.withCString { nameCStr in
      value.withCString { valueCStr in
        OpenColorIO_v2_4.SetEnvVariable(nameCStr, valueCStr)
      }
    }
  }

  /// Unset an environment variable.
  ///
  /// - Parameter name: The environment variable name.
  /// - Warning: This method is not thread safe.
  @inlinable
  static func unsetEnvVariable(_ name: String)
  {
    name.withCString { cStr in
      OpenColorIO_v2_4.UnsetEnvVariable(cStr)
    }
  }
}

// MARK: - Role Constants

public extension OCIO
{
  /// Standard OCIO role names.
  ///
  /// Roles are semantic names for colorspaces that allow applications
  /// to request colorspaces by purpose rather than specific name.
  enum Role
  {
    /// The default colorspace for 8-bit images.
    public static let `default` = "default"

    /// The reference colorspace (usually linear).
    public static let reference = "reference"

    /// The colorspace for data (non-color) images like normal maps.
    public static let data = "data"

    /// The default colorspace for color picking UI.
    public static let colorPicking = "color_picking"

    /// The default colorspace for scene-linear data.
    public static let sceneLinear = "scene_linear"

    /// The default colorspace for compositing log data.
    public static let compositingLog = "compositing_log"

    /// The default colorspace for color timing/grading.
    public static let colorTiming = "color_timing"

    /// The default colorspace for texture painting.
    public static let texturePaint = "texture_paint"

    /// The default colorspace for matte painting.
    public static let mattePaint = "matte_paint"

    /// The interchange scene-referred colorspace (usually ACES2065-1).
    public static let interchangeScene = "aces_interchange"

    /// The interchange display-referred colorspace.
    public static let interchangeDisplay = "aces_interchange_display"

    /// The rendering colorspace (working space for rendering).
    public static let rendering = "rendering"
  }
}

// MARK: - Built-in Config Names

public extension OCIO
{
  /// Built-in OCIO configuration names.
  ///
  /// These configurations are bundled with OCIO and can be loaded
  /// without an external file.
  enum BuiltinConfig
  {
    /// ACES Studio Config v1.0.0 (ACES v1.3, OCIO v2.1)
    public static let studioV100 = "studio-config-v1.0.0_aces-v1.3_ocio-v2.1"

    /// ACES Studio Config v2.1.0 (ACES v1.3, OCIO v2.3)
    public static let studioV210 = "studio-config-v2.1.0_aces-v1.3_ocio-v2.3"

    /// ACES Studio Config v2.2.0 (ACES v1.3, OCIO v2.4) - Latest
    public static let studioV220 = "studio-config-v2.2.0_aces-v1.3_ocio-v2.4"

    /// ACES CG Config v1.0.0 (ACES v1.3, OCIO v2.1)
    public static let cgV100 = "cg-config-v1.0.0_aces-v1.3_ocio-v2.1"

    /// ACES CG Config v2.1.0 (ACES v1.3, OCIO v2.3)
    public static let cgV210 = "cg-config-v2.1.0_aces-v1.3_ocio-v2.3"

    /// ACES CG Config v2.2.0 (ACES v1.3, OCIO v2.4) - Latest
    public static let cgV220 = "cg-config-v2.2.0_aces-v1.3_ocio-v2.4"

    /// The latest recommended Studio config.
    public static let latestStudio = studioV220

    /// The latest recommended CG config.
    public static let latestCG = cgV220
  }
}
