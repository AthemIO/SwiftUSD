/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

extension CaseIterable where Self: RawRepresentable, RawValue == String
{
  /// A string containing all possible values (for use in command-line option help messages).
  static var possibleValuesString: String
  {
    "(" + allCases.map(\.rawValue).joined(separator: "|") + ")"
  }
}
