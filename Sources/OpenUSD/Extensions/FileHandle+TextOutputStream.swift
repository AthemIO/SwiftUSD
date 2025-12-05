/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

extension FileHandle: TextOutputStream
{
  public func write(_ string: String)
  {
    let data = Data(string.utf8)
    write(data)
  }
}
