/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

// TODO: Create test for this
extension Sequence<String>
{
  var joinedList: String
  {
    var output = ""
    let array = Array(self)
    for (index, item) in array.enumerated()
    {
      if index == array.count - 1
      {
        output += "and "
      }
      output += String(describing: item)
      if index != array.count - 1
      {
        output += ", "
      }
    }
    return output
  }
}
