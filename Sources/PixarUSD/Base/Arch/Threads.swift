/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Arch

/**
 * # Multithreading
 *
 * Functions having to do with multithreading. */
public extension Arch
{
  /// Return true if the calling thread is the main thread, false otherwise.
  static func isMainThread() -> Bool
  {
    Pixar.ArchIsMainThread()
  }
}
