/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD
import QuartzCore

/* 
 * OpenExec Benchmark Suite
 *
 * Comprehensive benchmark for testing OpenExec execution system performance.
 * Creates scenes with various complexity levels to stress test:
 *
 * - Deep transform hierarchies (computeLocalToWorldTransform)
 * - Many sibling prims (parallel computation)
 * - Animated transforms (time-varying computation)
 * - Batch request processing (multiple value keys)
 */

// MARK: - Benchmark Configuration

public struct OpenExecBenchmarkConfig
{
  /// Depth of transform hierarchy (parent->child chain)
  public var hierarchyDepth: Int = 10

  /// Number of sibling prims at each level
  public var siblingsPerLevel: Int = 4

  /// Number of animation frames
  public var animationFrames: Int = 120

  /// Number of time samples to compute
  public var timeSamples: Int = 24

  /// Number of benchmark iterations for averaging
  public var iterations: Int = 5

  public init() {}

  /// Total number of prims created
  public var totalPrims: Int
  {
    // Each level has siblingsPerLevel prims, and we have hierarchyDepth levels
    // Plus one root prim
    var total = 1 // Root
    var currentLevelPrims = 1
    for _ in 0 ..< hierarchyDepth
    {
      currentLevelPrims *= siblingsPerLevel
      total += currentLevelPrims
    }
    return total
  }
}

// MARK: - Benchmark Results

public struct OpenExecBenchmarkResults
{
  public var config: OpenExecBenchmarkConfig
  public var sceneCreationTime: Double = 0
  public var systemCreationTime: Double = 0
  public var singleComputeTime: Double = 0
  public var batchComputeTime: Double = 0
  public var animationComputeTime: Double = 0
  public var totalPrimsComputed: Int = 0
  public var computationsPerSecond: Double = 0

  public func printReport()
  {
    Msg.logger.info("")
    Msg.logger.info("╔══════════════════════════════════════════════════════════════╗")
    Msg.logger.info("║           OPENEXEC BENCHMARK RESULTS                         ║")
    Msg.logger.info("╠══════════════════════════════════════════════════════════════╣")
    Msg.logger.info("║ Configuration:                                               ║")
    Msg.logger.info("║   Hierarchy Depth:     \(String(format: "%4d", config.hierarchyDepth)) levels                          ║")
    Msg.logger.info("║   Siblings Per Level:  \(String(format: "%4d", config.siblingsPerLevel)) prims                           ║")
    Msg.logger.info("║   Total Prims:         \(String(format: "%4d", config.totalPrims))                                 ║")
    Msg.logger.info("║   Animation Frames:    \(String(format: "%4d", config.animationFrames))                                 ║")
    Msg.logger.info("║   Time Samples:        \(String(format: "%4d", config.timeSamples))                                   ║")
    Msg.logger.info("╠══════════════════════════════════════════════════════════════╣")
    Msg.logger.info("║ Timing Results:                                              ║")
    Msg.logger.info("║   Scene Creation:      \(String(format: "%8.3f", sceneCreationTime * 1000)) ms                       ║")
    Msg.logger.info("║   System Creation:     \(String(format: "%8.3f", systemCreationTime * 1000)) ms                       ║")
    Msg.logger.info("║   Single Compute:      \(String(format: "%8.3f", singleComputeTime * 1000)) ms                       ║")
    Msg.logger.info("║   Batch Compute:       \(String(format: "%8.3f", batchComputeTime * 1000)) ms                       ║")
    Msg.logger.info("║   Animation Compute:   \(String(format: "%8.3f", animationComputeTime * 1000)) ms                       ║")
    Msg.logger.info("╠══════════════════════════════════════════════════════════════╣")
    Msg.logger.info("║ Performance:                                                 ║")
    Msg.logger.info("║   Total Prims Computed: \(String(format: "%6d", totalPrimsComputed))                            ║")
    Msg.logger.info("║   Computations/Second:  \(String(format: "%8.1f", computationsPerSecond))                        ║")
    Msg.logger.info("╚══════════════════════════════════════════════════════════════╝")
    Msg.logger.info("")
  }
}

// MARK: - Benchmark Scene Creation

public enum OpenExecBenchmark
{
  /// Create a benchmark scene with deep transform hierarchy
  public static func createBenchmarkScene(config: OpenExecBenchmarkConfig) -> UsdStageRefPtr
  {
    let stage = Usd.Stage.createNew("\(documentsDirPath())/OpenExecBenchmark", ext: .usda)

    // Set up animation time range
    stage.pointee.SetStartTimeCode(0.0)
    stage.pointee.SetEndTimeCode(Double(config.animationFrames))
    stage.pointee.SetFramesPerSecond(24.0)

    // Create root transform
    let root = UsdGeom.Xform.define(stage, path: "/BenchmarkRoot")
    root.addTranslateOp().set(GfVec3d(0.0, 0.0, 0.0))

    // Add slow rotation to root (animates entire hierarchy)
    let rootRotate = root.addRotateYOp()
    for frame in stride(from: 0, through: config.animationFrames, by: 1)
    {
      let angle = Float(frame) * (360.0 / Float(config.animationFrames))
      rootRotate.set(angle, time: Usd.TimeCode(Double(frame)))
    }

    // Recursively create hierarchy
    createHierarchyLevel(
      stage: stage,
      parentPath: "/BenchmarkRoot",
      currentDepth: 0,
      maxDepth: config.hierarchyDepth,
      siblingsPerLevel: config.siblingsPerLevel,
      animationFrames: config.animationFrames
    )

    stage.save()
    return stage
  }

  /// Recursively create transform hierarchy
  private static func createHierarchyLevel(
    stage: UsdStageRefPtr,
    parentPath: String,
    currentDepth: Int,
    maxDepth: Int,
    siblingsPerLevel: Int,
    animationFrames: Int
  )
  {
    guard currentDepth < maxDepth else { return }

    let spacing = 2.0 / Double(siblingsPerLevel)

    for i in 0 ..< siblingsPerLevel
    {
      let name = "Level\(currentDepth)_Child\(i)"
      let path = "\(parentPath)/\(name)"

      // Create either an xform (interior) or sphere (leaf)
      let isLeaf = currentDepth == maxDepth - 1

      if isLeaf
      {
        let sphere = UsdGeom.Sphere.define(stage, path: path)
        sphere.CreateRadiusAttr(Vt.Value(0.1), false)

        // Position in a grid pattern
        let offset = Double(i) * spacing - 1.0
        sphere.addTranslateOp().set(GfVec3d(offset, 0.0, 0.0))

        // Add scale animation for visual feedback
        let scaleOp = sphere.addScaleOp()
        for frame in stride(from: 0, through: animationFrames, by: 4)
        {
          let phase = Double(frame + i * 10) / Double(animationFrames) * .pi * 2
          let scale = Float(1.0 + 0.3 * sin(phase))
          scaleOp.set(GfVec3f(scale, scale, scale), time: Usd.TimeCode(Double(frame)))
        }
      }
      else
      {
        let xform = UsdGeom.Xform.define(stage, path: path)

        // Position child with offset
        let offset = Double(i) * spacing - 1.0
        xform.addTranslateOp().set(GfVec3d(offset, 1.0, 0.0))

        // Add rotation animation at different speeds per level
        let rotateOp = xform.addRotateZOp()
        let rotationSpeed = Float(currentDepth + 1) * 45.0 // Faster at deeper levels
        for frame in stride(from: 0, through: animationFrames, by: 2)
        {
          let angle = Float(frame) * rotationSpeed / Float(animationFrames)
          rotateOp.set(angle, time: Usd.TimeCode(Double(frame)))
        }

        // Recurse to create children
        createHierarchyLevel(
          stage: stage,
          parentPath: path,
          currentDepth: currentDepth + 1,
          maxDepth: maxDepth,
          siblingsPerLevel: siblingsPerLevel,
          animationFrames: animationFrames
        )
      }
    }
  }

  /// Collect all Xformable prims from the stage
  public static func collectXformablePrims(stage: UsdStageRefPtr) -> [Pixar.UsdPrim]
  {
    var prims: [Pixar.UsdPrim] = []
    for prim in stage.traverse()
    {
      // Check if prim has xform ops (is transformable)
      let xformable = Pixar.UsdGeomXformable(prim)
      if xformable.GetXformOpOrderAttr().IsValid()
      {
        prims.append(prim)
      }
    }
    return prims
  }

  // MARK: - Benchmark Execution

  /// Run the complete benchmark suite
  public static func runBenchmark(config: OpenExecBenchmarkConfig = OpenExecBenchmarkConfig()) -> OpenExecBenchmarkResults
  {
    var results = OpenExecBenchmarkResults(config: config)

    Msg.logger.info("Starting OpenExec Benchmark...")
    Msg.logger.info("Config: depth=\(config.hierarchyDepth), siblings=\(config.siblingsPerLevel), expected prims=\(config.totalPrims)")

    // 1. Benchmark scene creation
    let sceneStart = CACurrentMediaTime()
    let stage = createBenchmarkScene(config: config)
    results.sceneCreationTime = CACurrentMediaTime() - sceneStart

    Msg.logger.info("Scene created in \(String(format: "%.3f", results.sceneCreationTime * 1000)) ms")

    // 2. Benchmark system creation
    let systemStart = CACurrentMediaTime()
    let system = ExecUsd.System(stage: stage)
    results.systemCreationTime = CACurrentMediaTime() - systemStart

    guard system.isValid
    else
    {
      Msg.logger.error("Failed to create ExecUsd.System!")
      return results
    }
    Msg.logger.info("System created in \(String(format: "%.3f", results.systemCreationTime * 1000)) ms")

    // Collect all xformable prims
    let prims = collectXformablePrims(stage: stage)
    Msg.logger.info("Found \(prims.count) xformable prims")

    // 3. Benchmark single prim computation
    if let firstPrim = prims.first
    {
      let singleKey = ExecUsd.ValueKey(
        prim: firstPrim,
        computation: ExecGeom.Xformable.computeLocalToWorldTransform
      )

      var totalTime: Double = 0
      for _ in 0 ..< config.iterations
      {
        let start = CACurrentMediaTime()
        var request = system.buildRequest(valueKeys: [singleKey])
        let _ = system.compute(request: &request)
        totalTime += CACurrentMediaTime() - start
      }
      results.singleComputeTime = totalTime / Double(config.iterations)
      Msg.logger.info("Single compute: \(String(format: "%.3f", results.singleComputeTime * 1000)) ms (avg of \(config.iterations) iterations)")
    }

    // 4. Benchmark batch computation (all prims at once)
    let valueKeys = prims.map
    { prim in
      ExecUsd.ValueKey(
        prim: prim,
        computation: ExecGeom.Xformable.computeLocalToWorldTransform
      )
    }

    var batchTotalTime: Double = 0
    for _ in 0 ..< config.iterations
    {
      let start = CACurrentMediaTime()
      var request = system.buildRequest(valueKeys: valueKeys)
      let cacheView = system.compute(request: &request)

      // Verify results
      var validResults = 0
      for i in 0 ..< prims.count
      {
        let value = cacheView.get(index: i)
        if !value.IsEmpty()
        {
          validResults += 1
        }
      }

      batchTotalTime += CACurrentMediaTime() - start
    }
    results.batchComputeTime = batchTotalTime / Double(config.iterations)
    Msg.logger.info("Batch compute (\(prims.count) prims): \(String(format: "%.3f", results.batchComputeTime * 1000)) ms (avg of \(config.iterations) iterations)")

    // 5. Benchmark animation (compute at multiple times)
    let timeStep = Double(config.animationFrames) / Double(config.timeSamples)
    var animTotalTime: Double = 0

    for iteration in 0 ..< config.iterations
    {
      let start = CACurrentMediaTime()

      for i in 0 ..< config.timeSamples
      {
        let time = Double(i) * timeStep
        system.changeTime(time)

        var request = system.buildRequest(valueKeys: valueKeys)
        let _ = system.compute(request: &request)
      }

      animTotalTime += CACurrentMediaTime() - start

      if iteration == 0
      {
        Msg.logger.info("Animation pass \(iteration + 1): \(config.timeSamples) time samples computed")
      }
    }
    results.animationComputeTime = animTotalTime / Double(config.iterations)

    // Calculate metrics
    results.totalPrimsComputed = prims.count * config.timeSamples * config.iterations
    let totalComputeTime = results.animationComputeTime * Double(config.iterations)
    results.computationsPerSecond = Double(prims.count * config.timeSamples) / results.animationComputeTime

    Msg.logger.info("Animation compute (\(config.timeSamples) frames): \(String(format: "%.3f", results.animationComputeTime * 1000)) ms (avg of \(config.iterations) iterations)")

    // Cleanup
    withExtendedLifetime(system) {}

    return results
  }

  // MARK: - Public API

  /// Run benchmark with default settings and print results
  public static func run()
  {
    Msg.logger.info("")
    Msg.logger.info("═══════════════════════════════════════════════════════════════")
    Msg.logger.info("              OPENEXEC BENCHMARK SUITE                         ")
    Msg.logger.info("═══════════════════════════════════════════════════════════════")

    // Run small benchmark first
    Msg.logger.info("")
    Msg.logger.info("--- Small Benchmark (quick test) ---")
    var smallConfig = OpenExecBenchmarkConfig()
    smallConfig.hierarchyDepth = 4
    smallConfig.siblingsPerLevel = 3
    smallConfig.animationFrames = 48
    smallConfig.timeSamples = 12
    smallConfig.iterations = 3

    let smallResults = runBenchmark(config: smallConfig)
    smallResults.printReport()

    // Run medium benchmark
    Msg.logger.info("")
    Msg.logger.info("--- Medium Benchmark (stress test) ---")
    var mediumConfig = OpenExecBenchmarkConfig()
    mediumConfig.hierarchyDepth = 6
    mediumConfig.siblingsPerLevel = 4
    mediumConfig.animationFrames = 120
    mediumConfig.timeSamples = 24
    mediumConfig.iterations = 3

    let mediumResults = runBenchmark(config: mediumConfig)
    mediumResults.printReport()

    Msg.logger.info("═══════════════════════════════════════════════════════════════")
    Msg.logger.info("              BENCHMARK COMPLETE                               ")
    Msg.logger.info("═══════════════════════════════════════════════════════════════")
  }

  /// Run a quick validation test
  public static func runQuickTest()
  {
    Msg.logger.info("Running quick OpenExec validation test...")

    var config = OpenExecBenchmarkConfig()
    config.hierarchyDepth = 3
    config.siblingsPerLevel = 2
    config.animationFrames = 24
    config.timeSamples = 4
    config.iterations = 1

    let results = runBenchmark(config: config)

    if results.computationsPerSecond > 0
    {
      Msg.logger.info("Quick test PASSED - OpenExec is functional")
    }
    else
    {
      Msg.logger.error("Quick test FAILED - OpenExec may have issues")
    }
  }
}
