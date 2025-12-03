/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD
import SwiftCrossUI
import ExecUsd
import ExecGeom

/// ``UsdView``
///
/// ## Overview
///
/// A ``UsdView`` application written in Swift for
/// the purposes of demonstrating the usage of USD,
/// from the Swift programming language.
@main
struct UsdView: App {
    typealias Backend = PlatformBackend

    /// the active usd stage.
    let stage: UsdStageRefPtr
    /// the hydra rendering engine.
    let engine: Hydra.RenderEngine
    /// camera controller for interactive manipulation
    @State var cameraController: CameraController
    /// selection manager for focus-on-selection functionality
    // @State var selectionManager: SelectionManager

    public init() {
        // register all usd plugins & resources.
        Pixar.Bundler.shared.setup(.resources)

        // create a new usd stage.
        stage = UsdView.createScene()

        // setup hydra to render the usd stage.
        engine = Hydra.RenderEngine(stage: stage)

        // create camera controller with Z-up based on stage
        let isZUp = Hydra.RenderEngine.isZUp(for: stage)
        cameraController = CameraController(isZUp: isZUp)

        print(
            "Camera initialized - isZUp: \(isZUp), eye: \(cameraController.eye), at: \(cameraController.at)"
        )

        // create selection manager
        // selectionManager = SelectionManager(stage: stage)

        // connect camera controller to render engine
        engine.setCameraController(cameraController)

        Msg.logger.log(level: .info, "UsdView launched | USD v\(Pixar.version).")

        // Test OpenExec integration
        testOpenExec(stage: stage)
    }

    /// Test OpenExec integration by computing transforms using the ExecUsd system.
    private func testOpenExec(stage: UsdStageRefPtr) {
        print("=== OpenExec Test ===")

        // Create an ExecUsdSystem from the stage
        var execSystem = Pixar.ExecUsdSystem(stage)

        // Get the root prim to test with
        let rootPath = Sdf.Path("/World")
        let rootPrim = stage.pointee.GetPrimAtPath(rootPath)

        guard rootPrim.IsValid() else {
            print("OpenExec: No valid prim at /World, skipping test")
            return
        }

        // Create a value key for the computeLocalToWorldTransform computation
        let computationToken = Pixar.ExecGeomXformableTokens.computeLocalToWorldTransform
        let valueKey = Pixar.ExecUsdValueKey(rootPrim, computationToken)

        // Build a request with the value key
        var valueKeys = std.vector<Pixar.ExecUsdValueKey>()
        valueKeys.push_back(valueKey)

        var request = execSystem.BuildRequest(std.move(valueKeys))

        if request.IsValid() {
            // Compute the request
            let cacheView = execSystem.Compute(request)
            let result = cacheView.Get(0)
            print("OpenExec: Computed transform for /World: \(result)")
        } else {
            print("OpenExec: Request is not valid")
        }

        print("=== OpenExec Test Complete ===")
    }

    var body: some Scene {
        WindowGroup("UsdView") {
            Hydra.Viewport(
                engine: engine,
                cameraController: cameraController
            )
        }
    }
}
