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

public typealias SdfLayerOffset = Pixar.SdfLayerOffset

public extension Sdf
{
  typealias LayerOffset = SdfLayerOffset
}

public extension Sdf.LayerOffset
{
  init(offset: Double = 0.0, scale: Double = 1.0)
  {
    self = Sdf.LayerOffset(offset, scale)
  }
}
