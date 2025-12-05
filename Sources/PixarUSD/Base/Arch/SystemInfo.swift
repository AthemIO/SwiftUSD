/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Arch

/**
 * # System Functions
 *
 * Functions that encapsulate differing low-level system calls. */
public extension Arch
{
  /// Return current working directory as a string.
  static func getCwd() -> String
  {
    String(Pixar.ArchGetCwd())
  }

  /// Return the path to the program's executable.
  static func getExecutablePath() -> String
  {
    String(Pixar.ArchGetExecutablePath())
  }

  /// Return the system's memory page size. Safe to assume power-of-two.
  static func getPageSize() -> Int
  {
    Int(Pixar.ArchGetPageSize())
  }
}
