/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD
#if canImport(HgiGL)
  import HgiGL

  public extension Hydra
  {
    /**
     * ``GLRenderer``
     *
     * ## Overview
     *
     * The Hydra Engine (``Hd``) OpenGL renderer for the ``UsdView``
     * application. */
    class GLRenderer
    {
      let hgi: Pixar.HgiGLPtr
      // TODO: HgiGLDevice returns raw pointer which has Swift C++ interop issues
      // var device: Pixar.HgiGLDevice!

      public var stage: UsdStageRefPtr

      #if canImport(UsdImagingGL)
        /// UsdImagingGL is not available on iOS.
        var engine: UsdImagingGL.EngineSharedPtr
      #endif // canImport(UsdImagingGL)

      public required init(stage: UsdStageRefPtr)
      {
        hgi = HgiGL.createHgi()
        // TODO: device and value properties unavailable due to Swift C++ interop issues
        // device = hgi.device
        self.stage = stage

        // Note: Passing empty driver - UsdImagingGLEngine creates its own internal Hgi
        // when _hgiDriver.driver.IsEmpty() is true (see engine.cpp:1502-1506)
        let driver = HdDriver(name: Tf.Token(), driver: VtValue())

        #if canImport(UsdImagingGL)
          engine = UsdImagingGL.Engine.createEngine(
            rootPath: stage.getPseudoRoot().getPath(),
            excludedPaths: Sdf.PathVector(),
            invisedPaths: Sdf.PathVector(),
            sceneDelegateId: Sdf.Path.absoluteRootPath(),
            driver: driver
          )

          engine.setEnablePresentation(false)
          engine.setRendererAov(.color)
        #endif // canImport(UsdImagingGL)
      }

      public func info()
      {
        Msg.logger.log(level: .info, "Created HGI -> OpenGL.")
      }
    }
  }
#endif // canImport(HgiGL)
