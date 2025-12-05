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

public typealias SdfReference = Pixar.SdfReference

public extension Sdf
{
  typealias Reference = SdfReference
}

public extension Sdf.Reference
{
  init(assetPath: String = "",
       primPath: Sdf.Path = Sdf.Path(),
       layerOffset: Sdf.LayerOffset = Sdf.LayerOffset(),
       customData: Vt.Dictionary = Vt.Dictionary())
  {
    self = Sdf.Reference(
      std.string(assetPath),
      primPath,
      layerOffset,
      customData
    )
  }
}
