/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

extension [String]: OutputComponent
{
  var body: String
  {
    // StringOutput is used to avoid an infinite loop caused by result builders
    StringOutput(joined(separator: "\n"))
  }
}
