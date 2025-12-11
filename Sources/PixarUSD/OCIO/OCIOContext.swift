/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import OpenColorIO

// MARK: - Type Aliases

public typealias OCIOContextRcPtr = OpenColorIO_v2_4.ContextRcPtr
public typealias OCIOConstContextRcPtr = OpenColorIO_v2_4.ConstContextRcPtr

public extension OCIO
{
  typealias ContextRcPtr = OCIOContextRcPtr
  typealias ConstContextRcPtr = OCIOConstContextRcPtr
}

// MARK: - Context Bridge Functions

public extension OCIO
{
  /// Context bridge functions.
  ///
  /// A Context holds environment variables and settings that affect
  /// how color transforms are resolved.
  enum Context
  {
    /// Check if a context pointer is valid.
    @inlinable
    public static func isValid(_ ctx: OCIO.ConstContextRcPtr) -> Bool
    {
      OpenColorIO_v2_4.OCIOContext_isValid(ctx)
    }

    /// Get a cache ID for this context.
    @inlinable
    public static func getCacheID(_ ctx: OCIO.ConstContextRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOContext_getCacheID(ctx))
    }

    /// Get the number of string variables.
    @inlinable
    public static func getNumStringVars(_ ctx: OCIO.ConstContextRcPtr) -> Int32
    {
      OpenColorIO_v2_4.OCIOContext_getNumStringVars(ctx)
    }

    /// Get a string variable name by index.
    @inlinable
    public static func getStringVarNameByIndex(_ ctx: OCIO.ConstContextRcPtr, index: Int32) -> String?
    {
      guard let cStr = OpenColorIO_v2_4.OCIOContext_getStringVarNameByIndex(ctx, index) else { return nil }
      return String(cString: cStr)
    }

    /// Get a string variable by name.
    @inlinable
    public static func getStringVar(_ ctx: OCIO.ConstContextRcPtr, name: String) -> String
    {
      name.withCString { cStr in
        String(cString: OpenColorIO_v2_4.OCIOContext_getStringVar(ctx, cStr))
      }
    }

    /// Get the working directory.
    @inlinable
    public static func getWorkingDir(_ ctx: OCIO.ConstContextRcPtr) -> String
    {
      String(cString: OpenColorIO_v2_4.OCIOContext_getWorkingDir(ctx))
    }

    /// Resolve a string with context variables.
    @inlinable
    public static func resolveStringVar(_ ctx: OCIO.ConstContextRcPtr, value: String) -> String
    {
      value.withCString { cStr in
        String(cString: OpenColorIO_v2_4.OCIOContext_resolveStringVar(ctx, cStr))
      }
    }
  }
}
