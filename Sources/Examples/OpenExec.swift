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
import CxxStdlib

/**
 * OpenExec Examples
 *
 * Demonstrates using the OpenExec execution system to compute values
 * on USD scene data. OpenExec provides efficient, vectorized, multithreaded
 * evaluation of computational behaviors associated with USD schemas.
 */

func testExecUsdSystem()
{
  Msg.logger.info("Testing ExecUsd.System...")

  // Create a stage with geometry that has transforms
  let stage = Usd.Stage.createNew("\(documentsDirPath())/OpenExecTest", ext: .usda)

  // Create a hierarchy of transforms using the addTranslateOp helper
  let root = UsdGeom.Xform.define(stage, path: "/Root")
  root.addTranslateOp().set(GfVec3d(10.0, 0.0, 0.0))

  let child = UsdGeom.Xform.define(stage, path: "/Root/Child")
  child.addTranslateOp().set(GfVec3d(0.0, 5.0, 0.0))
  child.addRotateZOp().set(Float(45.0))

  let leaf = UsdGeom.Sphere.define(stage, path: "/Root/Child/Sphere")
  leaf.addScaleOp().set(GfVec3f(2.0, 2.0, 2.0))

  // Save the stage
  stage.save()
  Msg.logger.info("Created test stage at: \(documentsDirPath())/OpenExecTest.usda")

  // Create an execution system from the stage
  let system = ExecUsd.System(stage: stage)
  guard system.isValid else {
    Msg.logger.error("Failed to create ExecUsd.System!")
    return
  }
  Msg.logger.info("Created ExecUsd.System successfully")

  // Get the prims using the Swift wrapper
  let rootPrim = stage.getPrim(at: "/Root")
  let childPrim = stage.getPrim(at: "/Root/Child")
  let spherePrim = stage.getPrim(at: "/Root/Child/Sphere")

  guard rootPrim.IsValid(), childPrim.IsValid(), spherePrim.IsValid() else {
    Msg.logger.error("Failed to get prims from stage!")
    return
  }

  // Create value keys for computing local-to-world transforms
  let rootKey = ExecUsd.ValueKey(
    prim: rootPrim,
    computation: ExecGeom.Xformable.computeLocalToWorldTransform
  )
  let childKey = ExecUsd.ValueKey(
    prim: childPrim,
    computation: ExecGeom.Xformable.computeLocalToWorldTransform
  )
  let sphereKey = ExecUsd.ValueKey(
    prim: spherePrim,
    computation: ExecGeom.Xformable.computeLocalToWorldTransform
  )

  Msg.logger.info("Created value keys for transform computations")

  // Build a request for all three transforms at once
  var request = system.buildRequest(valueKeys: [rootKey, childKey, sphereKey])
  guard request.isValid else {
    Msg.logger.error("Failed to build request!")
    return
  }
  Msg.logger.info("Built request successfully")

  // Compute the transforms
  let cacheView = system.compute(request: &request)
  guard cacheView.isValid else {
    Msg.logger.error("Compute returned invalid cache view!")
    return
  }
  Msg.logger.info("Compute completed successfully")

  // Extract and display the results
  // Note: VtValue may not print correctly, so we check IsEmpty first
  let rootTransform = cacheView.get(index: 0)
  let childTransform = cacheView.get(index: 1)
  let sphereTransform = cacheView.get(index: 2)

  Msg.logger.info("Root transform empty: \(rootTransform.IsEmpty())")
  Msg.logger.info("Child transform empty: \(childTransform.IsEmpty())")
  Msg.logger.info("Sphere transform empty: \(sphereTransform.IsEmpty())")

  // If non-empty, extract as GfMatrix4d
  if !rootTransform.IsEmpty() {
    Msg.logger.info("Root transform type: \(rootTransform.GetTypeName())")
  }
  if !childTransform.IsEmpty() {
    Msg.logger.info("Child transform type: \(childTransform.GetTypeName())")
  }
  if !sphereTransform.IsEmpty() {
    Msg.logger.info("Sphere transform type: \(sphereTransform.GetTypeName())")
  }

  Msg.logger.info("ExecUsd.System test complete!")

  // CRITICAL: Ensure proper destruction order using withExtendedLifetime.
  // Swift ARC can destroy objects in any order after their last use, which
  // would crash if the system is destroyed before the request (the request
  // destructor accesses the system's request tracker).
  // Order: cacheView and valueKeys can be destroyed first, then request, then system.
  withExtendedLifetime((system, request)) {}
}

func testAnimatedScene()
{
  Msg.logger.info("Testing ExecUsd with animated scene...")
  Msg.logger.info("DEBUG: Creating stage...")

  // Create a stage with animated transforms
  let stage = Usd.Stage.createNew("\(documentsDirPath())/OpenExecAnimTest", ext: .usda)
  Msg.logger.info("DEBUG: Stage created")

  // Set up frame range using C++ API via pointee
  Msg.logger.info("DEBUG: Setting time codes...")
  stage.pointee.SetStartTimeCode(0.0)
  stage.pointee.SetEndTimeCode(48.0)
  Msg.logger.info("DEBUG: Time codes set")

  // Create an animated bouncing ball
  let ball = UsdGeom.Sphere.define(stage, path: "/BouncingBall")
  ball.GetRadiusAttr().Set(Pixar.VtValue(1.0))

  // Add animated translation - ball bounces up and down
  let translateOp = ball.addTranslateOp()

  // Keyframe 0: Ball at ground level
  translateOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(0.0))
  // Keyframe 12: Ball at peak height
  translateOp.set(GfVec3d(0.0, 10.0, 0.0), time: Usd.TimeCode(12.0))
  // Keyframe 24: Ball back at ground
  translateOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(24.0))
  // Keyframe 36: Ball at second peak
  translateOp.set(GfVec3d(0.0, 7.0, 0.0), time: Usd.TimeCode(36.0))
  // Keyframe 48: Ball at rest
  translateOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(48.0))

  // Also add horizontal movement
  let ball2 = UsdGeom.Sphere.define(stage, path: "/MovingBall")
  ball2.GetRadiusAttr().Set(Pixar.VtValue(0.5))

  let translate2 = ball2.addTranslateOp()
  translate2.set(GfVec3d(-10.0, 2.0, 0.0), time: Usd.TimeCode(0.0))
  translate2.set(GfVec3d(10.0, 2.0, 0.0), time: Usd.TimeCode(48.0))

  stage.save()
  Msg.logger.info("Created animated stage at: \(documentsDirPath())/OpenExecAnimTest.usda")

  // Create the execution system
  let system = ExecUsd.System(stage: stage)
  guard system.isValid else {
    Msg.logger.error("Failed to create ExecUsd.System!")
    return
  }

  // Get the ball prims
  let ballPrim = stage.getPrim(at: "/BouncingBall")
  let ball2Prim = stage.getPrim(at: "/MovingBall")

  guard ballPrim.IsValid(), ball2Prim.IsValid() else {
    Msg.logger.error("Failed to get ball prims!")
    return
  }

  // Create value keys
  let ballKey = ExecUsd.ValueKey(
    prim: ballPrim,
    computation: ExecGeom.Xformable.computeLocalToWorldTransform
  )
  let ball2Key = ExecUsd.ValueKey(
    prim: ball2Prim,
    computation: ExecGeom.Xformable.computeLocalToWorldTransform
  )

  Msg.logger.info("")
  Msg.logger.info("=== Computing transforms at different times ===")

  // Sample at various frames to show animation
  let sampleTimes: [Double] = [0.0, 6.0, 12.0, 18.0, 24.0, 36.0, 48.0]

  for time in sampleTimes {
    // Change the time in the system
    system.changeTime(time)

    // Build and execute a request at this time
    var request = system.buildRequest(valueKeys: [ballKey, ball2Key])
    let cacheView = system.compute(request: &request)

    if cacheView.isValid {
      let ballTransform = cacheView.get(index: 0)
      let ball2Transform = cacheView.get(index: 1)

      Msg.logger.info("")
      Msg.logger.info("Time \(time):")
      Msg.logger.info("  BouncingBall empty: \(ballTransform.IsEmpty())")
      Msg.logger.info("  MovingBall empty: \(ball2Transform.IsEmpty())")
      if !ballTransform.IsEmpty() {
        Msg.logger.info("  BouncingBall type: \(ballTransform.GetTypeName())")
      }
    } else {
      Msg.logger.warning("Cache view invalid at time \(time)")
    }
  }

  Msg.logger.info("")
  Msg.logger.info("Animated scene test complete!")

  // CRITICAL: Ensure proper destruction order using withExtendedLifetime.
  withExtendedLifetime(system) {}
}

public enum OpenExecExamples
{
  public static func run()
  {
    Msg.logger.info("Running OpenExec examples...")
    Msg.logger.info("")

    testExecUsdSystem()
    Msg.logger.info("")

    // Fixed: The System wrapper now tracks all requests and ensures proper
    // destruction order (requests before system), preventing use-after-free.
    testAnimatedScene()
    Msg.logger.info("")

    Msg.logger.info("OpenExec examples complete.")
  }
}
