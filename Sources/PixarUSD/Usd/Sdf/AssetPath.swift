/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import Foundation
import Sdf

public typealias SdfAssetPath = Pixar.SdfAssetPath

public extension Sdf
{
  typealias AssetPath = SdfAssetPath
}

public extension Sdf.AssetPath
{
  init(_ path: String)
  {
    self.init(std.string(path))
  }
}
