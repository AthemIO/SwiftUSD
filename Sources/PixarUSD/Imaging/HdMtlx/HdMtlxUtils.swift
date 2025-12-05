/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import HdMtlx

public extension HdMtlx
{
  // MARK: - Value Conversion

  /// Convert a Hydra parameter value to a MaterialX string representation.
  ///
  /// This function handles conversion of various USD/Hydra value types
  /// to their MaterialX string equivalents:
  /// - `Bool` → "true" or "false"
  /// - Numeric types → decimal string
  /// - Color types → "R G B" format
  /// - Vector types → space-separated components
  ///
  /// - Parameter hdParameterValue: The Hydra parameter value to convert
  /// - Returns: MaterialX-compatible string representation
  static func convertToString(_ hdParameterValue: Vt.Value) -> String
  {
    String(Pixar.HdMtlxConvertToString(hdParameterValue))
  }

  // MARK: - Name/Path Conversion

  /// Create a valid MaterialX name from an SdfPath.
  ///
  /// MaterialX has specific naming requirements that differ from USD paths.
  /// This function converts USD paths to MaterialX-compatible names,
  /// handling special characters and platform differences.
  ///
  /// - Parameter path: The USD SdfPath to convert
  /// - Returns: A MaterialX-compatible name string
  static func createNameFromPath(_ path: Sdf.Path) -> String
  {
    String(Pixar.HdMtlxCreateNameFromPath(path))
  }

  // MARK: - NodeDef Utilities

  /// Get the NodeDef name appropriate for the current MaterialX version.
  ///
  /// NodeDef names may change between MaterialX versions. This function
  /// returns the correct NodeDef name for the version of MaterialX
  /// currently being used.
  ///
  /// - Parameter prevMxNodeDefName: NodeDef name from a previous MaterialX version
  /// - Returns: The current version's NodeDef name
  static func getNodeDefName(_ prevMxNodeDefName: String) -> String
  {
    String(Pixar.HdMtlxGetNodeDefName(std.string(prevMxNodeDefName)))
  }
}
