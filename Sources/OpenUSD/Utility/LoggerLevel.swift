/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Logging

extension Logger.Level
{
  /// The log level as a colored string.
  func coloring(_ string: String) -> String
  {
    switch self
    {
      case .critical:
        string.red.bold
      case .error:
        string.red.bold
      case .warning:
        string.yellow.bold
      case .notice:
        string.cyan
      case .info:
        string.cyan
      case .debug:
        string.lightWhite
      case .trace:
        string.lightWhite
    }
  }
}
