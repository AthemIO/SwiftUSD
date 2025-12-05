/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Hd

/**
 * ``HdDriver``
 *
 * ## Overview
 *
 * Represents a device object, commonly a render
 * device, that is owned by the application and passed to
 * HdRenderIndex. The RenderIndex passes it to the render
 * delegate and rendering tasks. The application manages
 * the lifetime (destruction) of HdDriver and must ensure
 * it remains valid while Hydra is running. */
public typealias HdDriver = Pixar.HdDriver

public extension Hd
{
  /**
   * ``Driver``
   *
   * ## Overview
   *
   * Represents a device object, commonly a render
   * device, that is owned by the application and passed to
   * HdRenderIndex. The RenderIndex passes it to the render
   * delegate and rendering tasks. The application manages
   * the lifetime (destruction) of HdDriver and must ensure
   * it remains valid while Hydra is running. */
  typealias Driver = HdDriver
}

public extension Hd.Driver
{
  init(name: Hgi.Tokens, driver: Vt.Value)
  {
    self.init(name: name.token, driver: driver)
  }
}
