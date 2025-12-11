/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

/**
 * # OCIO.Config
 *
 * The Config is the central object in OpenColorIO, containing all
 * color space definitions, looks, displays, and views.
 *
 * ## Loading a Config
 *
 * ```swift
 * // From environment variable
 * let config = OCIO.Config.createFromEnv()
 *
 * // From file
 * let config = OCIO.Config.createFromFile("/path/to/config.ocio")
 *
 * // Built-in ACES config
 * let config = OCIO.Config.createFromBuiltinConfig(OCIO.BuiltinConfig.latestStudio)
 * ```
 *
 * ## Processing Colors
 *
 * ```swift
 * let processor = OCIO.Config.getProcessor(config, from: "ACEScg", to: "sRGB")
 * let cpu = OCIO.Processor.getDefaultCPUProcessor(processor)
 * OCIO.CPUProcessor.applyRGBA(cpu, &pixels)
 * ```
 */
public typealias OCIOConfigRcPtr = OpenColorIO_v2_4.ConfigRcPtr
public typealias OCIOConstConfigRcPtr = OpenColorIO_v2_4.ConstConfigRcPtr

public extension OCIO
{
  typealias ConfigRcPtr = OCIOConfigRcPtr
  typealias ConstConfigRcPtr = OCIOConstConfigRcPtr
}

// MARK: - Config Namespace for Static Methods

public extension OCIO
{
  /// Config factory methods and bridge functions.
  enum Config
  {
    // MARK: - Factory Methods

    /// Create an empty configuration.
    ///
    /// This creates a minimal config without any color spaces defined.
    /// Useful as a starting point for building custom configs.
    ///
    /// - Returns: A new empty configuration.
    @inlinable
    public static func create() -> OCIO.ConfigRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_Create()
    }

    /// Create a fallback configuration.
    ///
    /// This creates a simple "raw" config that can be used when no
    /// OCIO config file is available. It provides minimal color management.
    ///
    /// - Returns: A fallback configuration.
    @inlinable
    public static func createRaw() -> OCIO.ConstConfigRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_CreateRaw()
    }

    /// Create a configuration from the OCIO environment variable.
    ///
    /// Reads the `OCIO` environment variable to find the config file path.
    /// If the environment variable is not set, returns a default config.
    ///
    /// - Returns: The configuration from the environment.
    @inlinable
    public static func createFromEnv() -> OCIO.ConstConfigRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_CreateFromEnv()
    }

    /// Create a configuration from a file.
    ///
    /// Loads and parses an OCIO configuration file (`.ocio` or `.ocioz`).
    ///
    /// - Parameter filePath: Path to the configuration file.
    /// - Returns: The loaded configuration.
    @inlinable
    public static func createFromFile(_ filePath: String) -> OCIO.ConstConfigRcPtr
    {
      filePath.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_CreateFromFile(cStr)
      }
    }

    /// Create a configuration from a built-in config.
    ///
    /// OCIO includes several built-in configurations like ACES Studio
    /// and ACES CG configs that can be loaded without external files.
    ///
    /// - Parameter configName: The name of the built-in config.
    /// - Returns: The built-in configuration.
    ///
    /// ## Example
    /// ```swift
    /// // Load the latest ACES Studio config
    /// let config = OCIO.Config.createFromBuiltinConfig(OCIO.BuiltinConfig.latestStudio)
    /// ```
    @inlinable
    public static func createFromBuiltinConfig(_ configName: String) -> OCIO.ConstConfigRcPtr
    {
      configName.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_CreateFromBuiltinConfig(cStr)
      }
    }

    // MARK: - Bridge Functions for Config Instance Methods

    /// Check if a config pointer is valid.
    @inlinable
    public static func isValid(_ config: OCIO.ConstConfigRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOConfig_isValid(config)
    }

    /// Validate the configuration.
    ///
    /// Performs thorough validation of the configuration, checking for
    /// internal consistency, missing files, and other issues.
    @inlinable
    public static func validate(_ config: OCIO.ConstConfigRcPtr)
    {
      OpenColorIO_v2_4.OCIOConfig_validate(config)
    }

    /// Get a cache ID for this configuration.
    @inlinable
    public static func getCacheID(_ config: OCIO.ConstConfigRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOConfig_getCacheID(config))
    }

    /// Get the major version of this config file format.
    @inlinable
    public static func getMajorVersion(_ config: OCIO.ConstConfigRcPtr) -> UInt32
    {
      OpenColorIO_v2_4.OCIOConfig_getMajorVersion(config)
    }

    /// Get the minor version of this config file format.
    @inlinable
    public static func getMinorVersion(_ config: OCIO.ConstConfigRcPtr) -> UInt32
    {
      OpenColorIO_v2_4.OCIOConfig_getMinorVersion(config)
    }

    /// Get the config name.
    @inlinable
    public static func getName(_ config: OCIO.ConstConfigRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOConfig_getName(config))
    }

    /// Get the config description.
    @inlinable
    public static func getDescription(_ config: OCIO.ConstConfigRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOConfig_getDescription(config))
    }

    // MARK: - Color Spaces

    /// Get the number of color spaces in this config.
    @inlinable
    public static func getNumColorSpaces(_ config: OCIO.ConstConfigRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOConfig_getNumColorSpaces(config)
    }

    /// Get the name of a color space by index.
    @inlinable
    public static func getColorSpaceNameByIndex(_ config: OCIO.ConstConfigRcPtr, index: Int32) -> String?
    {
      guard let cStr = OpenColorIO_v2_4.OCIOConfig_getColorSpaceNameByIndex(config, index) else { return nil }
      return String(cString: cStr)
    }

    /// Get all color space names.
    @inlinable
    public static func getColorSpaceNames(_ config: OCIO.ConstConfigRcPtr) -> [String]
    {
      (0 ..< getNumColorSpaces(config)).compactMap { getColorSpaceNameByIndex(config, index: $0) }
    }

    /// Get a color space by name.
    @inlinable
    public static func getColorSpace(_ config: OCIO.ConstConfigRcPtr, name: String) -> OCIO.ConstColorSpaceRcPtr
    {
      name.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getColorSpace(config, cStr)
      }
    }

    /// Get the index of a color space by name.
    @inlinable
    public static func getColorSpaceIndex(_ config: OCIO.ConstConfigRcPtr, name: String) -> Int32
    {
      name.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getIndexForColorSpace(config, cStr)
      }
    }

    // MARK: - Roles

    /// Get the number of defined roles.
    @inlinable
    public static func getNumRoles(_ config: OCIO.ConstConfigRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOConfig_getNumRoles(config)
    }

    /// Check if a role is defined.
    @inlinable
    public static func hasRole(_ config: OCIO.ConstConfigRcPtr, role: String) -> Bool
    {
      role.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_hasRole(config, cStr)
      }
    }

    /// Get the role name by index.
    @inlinable
    public static func getRoleName(_ config: OCIO.ConstConfigRcPtr, index: Int32) -> String?
    {
      guard let cStr = OpenColorIO_v2_4.OCIOConfig_getRoleName(config, index) else { return nil }
      return String(cString: cStr)
    }

    /// Get the color space name assigned to a role.
    @inlinable
    public static func getRoleColorSpace(_ config: OCIO.ConstConfigRcPtr, role: String) -> String?
    {
      let result = role.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getColorSpaceNameByRole(config, cStr)
      }
      guard let result else { return nil }
      return String(cString: result)
    }

    // MARK: - Displays and Views

    /// Get the default display name.
    @inlinable
    public static func getDefaultDisplay(_ config: OCIO.ConstConfigRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOConfig_getDefaultDisplay(config))
    }

    /// Get the number of displays.
    @inlinable
    public static func getNumDisplays(_ config: OCIO.ConstConfigRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOConfig_getNumDisplays(config)
    }

    /// Get a display name by index.
    @inlinable
    public static func getDisplay(_ config: OCIO.ConstConfigRcPtr, index: Int32) -> String?
    {
      guard let cStr = OpenColorIO_v2_4.OCIOConfig_getDisplay(config, index) else { return nil }
      return String(cString: cStr)
    }

    /// Get all display names.
    @inlinable
    public static func getDisplayNames(_ config: OCIO.ConstConfigRcPtr) -> [String]
    {
      (0 ..< getNumDisplays(config)).compactMap { getDisplay(config, index: $0) }
    }

    /// Get the default view for a display.
    @inlinable
    public static func getDefaultView(_ config: OCIO.ConstConfigRcPtr, display: String) -> String
    {
      display.withCString { cStr in
        String(cString: OpenColorIO_v2_4.OCIOConfig_getDefaultView(config, cStr))
      }
    }

    /// Get the number of views for a display.
    @inlinable
    public static func getNumViews(_ config: OCIO.ConstConfigRcPtr, display: String) -> Int32
    {
      display.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getNumViews(config, cStr)
      }
    }

    /// Get a view name by index for a display.
    @inlinable
    public static func getView(_ config: OCIO.ConstConfigRcPtr, display: String, index: Int32) -> String?
    {
      let result = display.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getView(config, cStr, index)
      }
      guard let result else { return nil }
      return String(cString: result)
    }

    /// Get all view names for a display.
    @inlinable
    public static func getViews(_ config: OCIO.ConstConfigRcPtr, display: String) -> [String]
    {
      (0 ..< getNumViews(config, display: display)).compactMap { getView(config, display: display, index: $0) }
    }

    /// Get the color space name for a display/view pair.
    @inlinable
    public static func getDisplayViewColorSpaceName(
      _ config: OCIO.ConstConfigRcPtr,
      display: String,
      view: String
    ) -> String
    {
      display.withCString { displayCStr in
        view.withCString { viewCStr in
          String(cString: OpenColorIO_v2_4.OCIOConfig_getDisplayViewColorSpaceName(config, displayCStr, viewCStr))
        }
      }
    }

    // MARK: - Looks

    /// Get the number of looks.
    @inlinable
    public static func getNumLooks(_ config: OCIO.ConstConfigRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOConfig_getNumLooks(config)
    }

    /// Get a look name by index.
    @inlinable
    public static func getLookNameByIndex(_ config: OCIO.ConstConfigRcPtr, index: Int32) -> String?
    {
      guard let cStr = OpenColorIO_v2_4.OCIOConfig_getLookNameByIndex(config, index) else { return nil }
      return String(cString: cStr)
    }

    /// Get a look by name.
    @inlinable
    public static func getLook(_ config: OCIO.ConstConfigRcPtr, name: String) -> OCIO.ConstLookRcPtr
    {
      name.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getLook(config, cStr)
      }
    }

    /// Get all look names.
    @inlinable
    public static func getLookNames(_ config: OCIO.ConstConfigRcPtr) -> [String]
    {
      (0 ..< getNumLooks(config)).compactMap { getLookNameByIndex(config, index: $0) }
    }

    // MARK: - View Transforms

    /// Get the number of view transforms.
    @inlinable
    public static func getNumViewTransforms(_ config: OCIO.ConstConfigRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOConfig_getNumViewTransforms(config)
    }

    /// Get a view transform by index.
    @inlinable
    public static func getViewTransformByIndex(_ config: OCIO.ConstConfigRcPtr, index: Int32) -> OCIO.ConstViewTransformRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_getViewTransformByIndex(config, index)
    }

    /// Get a view transform by name.
    @inlinable
    public static func getViewTransform(_ config: OCIO.ConstConfigRcPtr, name: String) -> OCIO.ConstViewTransformRcPtr
    {
      name.withCString { cStr in
        OpenColorIO_v2_4.OCIOConfig_getViewTransform(config, cStr)
      }
    }

    // MARK: - Processors

    /// Get a processor to convert between two color spaces.
    ///
    /// This is the most common way to get a processor for color conversion.
    ///
    /// - Parameters:
    ///   - config: The OCIO config.
    ///   - srcColorSpace: The source color space name.
    ///   - dstColorSpace: The destination color space name.
    /// - Returns: A processor for the conversion.
    ///
    /// ## Example
    /// ```swift
    /// let processor = OCIO.Config.getProcessor(config, from: "ACEScg", to: "sRGB - Display")
    /// ```
    @inlinable
    public static func getProcessor(
      _ config: OCIO.ConstConfigRcPtr,
      from srcColorSpace: String,
      to dstColorSpace: String
    ) -> OCIO.ConstProcessorRcPtr
    {
      srcColorSpace.withCString { srcCStr in
        dstColorSpace.withCString { dstCStr in
          OpenColorIO_v2_4.OCIOConfig_getProcessor(config, srcCStr, dstCStr)
        }
      }
    }

    /// Get a processor to convert between two color spaces with context.
    @inlinable
    public static func getProcessor(
      _ config: OCIO.ConstConfigRcPtr,
      context: OCIO.ConstContextRcPtr,
      from srcColorSpace: String,
      to dstColorSpace: String
    ) -> OCIO.ConstProcessorRcPtr
    {
      srcColorSpace.withCString { srcCStr in
        dstColorSpace.withCString { dstCStr in
          OpenColorIO_v2_4.OCIOConfig_getProcessorWithContext(config, context, srcCStr, dstCStr)
        }
      }
    }

    /// Get a processor from a transform.
    @inlinable
    public static func getProcessor(
      _ config: OCIO.ConstConfigRcPtr,
      transform: OCIO.ConstTransformRcPtr
    ) -> OCIO.ConstProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_getProcessorFromTransform(config, transform)
    }

    /// Get a processor from a transform with direction.
    @inlinable
    public static func getProcessor(
      _ config: OCIO.ConstConfigRcPtr,
      transform: OCIO.ConstTransformRcPtr,
      direction: OCIO.TransformDirection
    ) -> OCIO.ConstProcessorRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_getProcessorFromTransformWithDirection(config, transform, direction)
    }

    // MARK: - Search Paths

    /// Get the number of search paths.
    @inlinable
    public static func getNumSearchPaths(_ config: OCIO.ConstConfigRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOConfig_getNumSearchPaths(config)
    }

    /// Get a search path by index.
    @inlinable
    public static func getSearchPath(_ config: OCIO.ConstConfigRcPtr, index: Int32) -> String?
    {
      guard let cStr = OpenColorIO_v2_4.OCIOConfig_getSearchPath(config, index) else { return nil }
      return String(cString: cStr)
    }

    /// Get all search paths.
    @inlinable
    public static func getSearchPaths(_ config: OCIO.ConstConfigRcPtr) -> [String]
    {
      (0 ..< getNumSearchPaths(config)).compactMap { getSearchPath(config, index: $0) }
    }

    /// Get the working directory.
    @inlinable
    public static func getWorkingDir(_ config: OCIO.ConstConfigRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOConfig_getWorkingDir(config))
    }

    // MARK: - Context

    /// Get the current context.
    @inlinable
    public static func getCurrentContext(_ config: OCIO.ConstConfigRcPtr) -> OCIO.ConstContextRcPtr
    {
      OpenColorIO_v2_4.OCIOConfig_getCurrentContext(config)
    }
  }
}
