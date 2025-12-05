/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD
import SwiftCrossUI

/// ``UsdView``
///
/// ## Overview
///
/// A ``UsdView`` application written in Swift for
/// the purposes of demonstrating the usage of USD,
/// from the Swift programming language.
@main
struct UsdView: App
{
  typealias Backend = PlatformBackend

  /// the active usd stage.
  let stage: UsdStageRefPtr
  /// the hydra rendering engine.
  let engine: Hydra.RenderEngine
  /// camera controller for interactive manipulation
  @State var cameraController: CameraController
  // selection manager for focus-on-selection functionality
  // @State var selectionManager: SelectionManager

  init()
  {
    // register all usd plugins & resources.
    Pixar.Bundler.shared.setup(.resources)

    // create a new usd stage.
    stage = UsdView.createOpenExecBenchmarkScene()

    // setup hydra to render the usd stage.
    engine = Hydra.RenderEngine(stage: stage)

    // create camera controller with Z-up based on stage
    let isZUp = Hydra.RenderEngine.isZUp(for: stage)
    cameraController = CameraController(isZUp: isZUp)

    // create selection manager
    // selectionManager = SelectionManager(stage: stage)

    // connect camera controller to render engine
    engine.setCameraController(cameraController)

    Msg.logger.log(level: .info, "UsdView launched | USD v\(Pixar.version).")
  }

  var body: some Scene
  {
    WindowGroup("UsdView")
    {
      Hydra.Viewport(
        engine: engine,
        cameraController: cameraController
      )
    }
  }
}
