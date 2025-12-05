/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Hgi

public enum Hgi
{}

public extension Hgi
{
  /// Helper function to return a ``__ObjC/Pixar/Hgi`` object for the current
  /// platform. For example on Linux this may return HgiGL while on macOS, it
  /// may return HgiMetal. Caller, usually the application, owns the lifetime
  /// of the Hgi object and the object is destroyed when the caller drops the
  /// unique ptr. Thread safety: Not thread safe.
  static func createPlatformDefaultHgi() -> Pixar.Hgi
  {
    /* Currently swift does not support std.unique_ptr,
     * use the deprecated GetPlatformDefaultHgi() call
     * instead of CreatePlatformDefaultHgi() for now. */
    Pixar.Hgi.GetPlatformDefaultHgi()
  }
}
