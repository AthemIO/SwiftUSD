/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import ArgumentParser
import Foundation

extension Array: ExpressibleByArgument where Element: ExpressibleByArgument
{
  public var defaultValueDescription: String
  {
    "[" + map(\.defaultValueDescription).joined(separator: ", ") + "]"
  }

  public init?(argument _: String)
  {
    nil
  }
}
