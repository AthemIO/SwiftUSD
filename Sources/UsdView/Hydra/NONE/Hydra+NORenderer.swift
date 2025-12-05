/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

public extension Hydra
{
  /**
   * ``NORenderer``
   *
   * ## Overview
   *
   * The Hydra Engine (``Hd``) no-op renderer for the ``UsdView``
   * application. Note: This renders nothing. */
  class NORenderer
  {
    public var stage: UsdStageRefPtr

    public required init(stage: UsdStageRefPtr)
    {
      self.stage = stage
    }

    public func info()
    {
      Msg.logger.log(level: .info, "Created HGI -> None.")
    }
  }
}
