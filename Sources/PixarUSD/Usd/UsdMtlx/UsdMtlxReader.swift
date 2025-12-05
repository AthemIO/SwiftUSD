/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import UsdMtlx

public extension UsdMtlx
{
  // MARK: - MaterialX to USD Conversion

  /// Convert MaterialX XML string to a USD stage.
  ///
  /// This function parses MaterialX XML content and converts it to USD
  /// scene description, creating materials, shading networks, and looks.
  ///
  /// - Parameters:
  ///   - buffer: MaterialX XML content as a string
  ///   - nodeGraphsOnly: If true, only node graphs are read;
  ///     otherwise everything else (materials, looks) is read
  /// - Returns: A USD stage containing the converted MaterialX content
  ///
  /// ## Example
  /// ```swift
  /// let mtlxXml = """
  /// <?xml version="1.0"?>
  /// <materialx version="1.38">
  ///     <standard_surface name="my_material" type="surfaceshader">
  ///         <input name="base_color" type="color3" value="0.8, 0.2, 0.1"/>
  ///     </standard_surface>
  /// </materialx>
  /// """
  /// let stage = UsdMtlx.testString(mtlxXml)
  /// ```
  @discardableResult
  static func testString(_ buffer: String, nodeGraphsOnly: Bool = false) -> Usd.StageRefPtr
  {
    Pixar.UsdMtlx_TestString(std.string(buffer), nodeGraphsOnly)
  }

  /// Convert MaterialX file to a USD stage.
  ///
  /// This function reads a `.mtlx` file and converts it to USD
  /// scene description, creating materials, shading networks, and looks.
  ///
  /// - Parameters:
  ///   - pathname: Path to the `.mtlx` file
  ///   - nodeGraphsOnly: If true, only node graphs are read;
  ///     otherwise everything else (materials, looks) is read
  /// - Returns: A USD stage containing the converted MaterialX content
  ///
  /// ## Example
  /// ```swift
  /// let stage = UsdMtlx.testFile("/path/to/material.mtlx")
  /// for prim in stage.traverse() {
  ///     print(prim.GetPath())
  /// }
  /// ```
  @discardableResult
  static func testFile(_ pathname: String, nodeGraphsOnly: Bool = false) -> Usd.StageRefPtr
  {
    Pixar.UsdMtlx_TestFile(std.string(pathname), nodeGraphsOnly)
  }
}
