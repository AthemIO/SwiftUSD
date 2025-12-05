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

/**
 * OpenExec Visual Benchmark Scene
 *
 * Creates a visually rich scene demonstrating OpenExec capabilities:
 * - Deep transform hierarchies with animated rotations
 * - Many interconnected moving parts (like a solar system or clock)
 * - Showcases computeLocalToWorldTransform computation
 *
 * This scene is designed to be both visually interesting and
 * computationally intensive for OpenExec benchmarking.
 */

extension UsdView
{
  // MARK: - OpenExec Visual Benchmark Scene

  /**
   * Create a "solar system" style scene that demonstrates
   * deep transform hierarchies with nested rotations.
   *
   * Each planet orbits the sun, moons orbit planets, and
   * rings rotate around larger bodies - all animated.
   */
  public static func createOpenExecBenchmarkScene() -> UsdStageRefPtr
  {
    let stage = Usd.Stage.createNew("\(documentsDirPath())/OpenExecBenchmark", ext: .usd)

    // Set up animation (5 seconds at 24fps)
    stage.pointee.SetStartTimeCode(0.0)
    stage.pointee.SetEndTimeCode(120.0)
    stage.pointee.SetFramesPerSecond(24.0)

    // Add dome light with HDRI for environment lighting
    let domeLight = UsdLux.DomeLight.define(stage, path: "/World/DefaultDomeLight")
    if let hdxResources = Bundle.hdx?.resourcePath {
      let tex = "\(hdxResources)/textures/StinsonBeach.hdr"
      if FileManager.default.fileExists(atPath: tex) {
        let hdrAsset = Sdf.AssetPath(tex)
        domeLight.createTextureFileAttr().set(hdrAsset)
      }
    }

    // Add a distant light for direct illumination
    let distantLight = UsdLux.DistantLight.define(stage, path: "/World/DistantLight")
    distantLight.addRotateXYZOp().set(GfVec3f(-45.0, -30.0, 0.0))

    // Create root transform
    let root = UsdGeom.Xform.define(stage, path: "/SolarSystem")
    root.addTranslateOp().set(GfVec3d(0.0, 0.0, 0.0))

    // Create the Sun (center)
    let sun = UsdGeom.Sphere.define(stage, path: "/SolarSystem/Sun")
    sun.CreateRadiusAttr(Vt.Value(1.5), false)

    // Sun slowly rotates
    let sunRotate = sun.addRotateYOp()
    animateRotationOp(sunRotate, duration: 120, rotations: 0.5)

    // Bind glowing yellow-orange material to sun (emissive-like with high brightness)
    UsdShade.MaterialBindingAPI.apply(sun.GetPrim()).bind(
      matDefMetallicWithFallback(stage, color: .orange, roughness: 0.1)
    )

    // Create planetary orbits with different speeds and distances
    let planets: [(name: String, distance: Double, size: Double, orbitSpeed: Double, color: ShadeColor, hasMoons: Int)] = [
      ("Mercury", 2.5, 0.15, 4.0, .white, 0),
      ("Venus", 3.5, 0.25, 3.0, .yellow, 0),
      ("Earth", 5.0, 0.3, 2.0, .blue, 1),
      ("Mars", 6.5, 0.22, 1.5, .red, 2),
      ("Jupiter", 9.0, 0.8, 0.8, .orange, 4),
      ("Saturn", 12.0, 0.7, 0.6, .yellow, 3),
    ]

    for (index, planet) in planets.enumerated() {
      createPlanetSystemForBenchmark(
        stage: stage,
        parentPath: "/SolarSystem",
        name: planet.name,
        distance: planet.distance,
        size: planet.size,
        orbitSpeed: planet.orbitSpeed,
        color: planet.color,
        moonCount: planet.hasMoons,
        planetIndex: index
      )
    }

    // Create asteroid belt between Mars and Jupiter (spread out wider)
    // With PointInstancer, we can now handle thousands of asteroids with GPU instancing!
    createAsteroidBeltForBenchmark(stage: stage, parentPath: "/SolarSystem", innerRadius: 6.5, outerRadius: 10.0, count: 2000)

    // Save and return
    stage.getPseudoRoot().set(doc: "OpenExec Benchmark - SwiftUSD v\(Pixar.version)")
    stage.save()

    // Print prim hierarchy
    Msg.logger.info("")
    Msg.logger.info("=== OpenExec Benchmark Scene Created ===")
    var primCount = 0
    for prim in stage.traverse() {
      primCount += 1
    }
    Msg.logger.info("Total prims: \(primCount)")
    Msg.logger.info("Scene saved to: \(documentsDirPath())/OpenExecBenchmark.usd")
    Msg.logger.info("")

    return stage
  }

  // MARK: - Planet System Creation

  private static func createPlanetSystemForBenchmark(
    stage: UsdStageRefPtr,
    parentPath: String,
    name: String,
    distance: Double,
    size: Double,
    orbitSpeed: Double,
    color: ShadeColor,
    moonCount: Int,
    planetIndex: Int
  ) {
    // Orbit pivot - rotates around the parent (sun)
    let orbitPath = "\(parentPath)/\(name)_Orbit"
    let orbit = UsdGeom.Xform.define(stage, path: orbitPath)

    // Add orbit rotation animation
    let orbitRotate = orbit.addRotateYOp()
    animateRotationOp(orbitRotate, duration: 120, rotations: orbitSpeed)

    // Planet offset from sun
    let planetPath = "\(orbitPath)/\(name)"
    let planet = UsdGeom.Sphere.define(stage, path: planetPath)
    planet.CreateRadiusAttr(Vt.Value(size), false)

    // Position planet at distance from center
    planet.addTranslateOp().set(GfVec3d(distance, 0.0, 0.0))

    // Planet self-rotation (day/night cycle)
    let planetRotate = planet.addRotateYOp()
    animateRotationOp(planetRotate, duration: 120, rotations: orbitSpeed * 5.0)

    // Bind material
    UsdShade.MaterialBindingAPI.apply(planet.GetPrim()).bind(
      matDefMetallicWithFallback(stage, color: color, roughness: 0.4)
    )

    // Create moons
    for moonIndex in 0..<moonCount {
      createMoonForBenchmark(
        stage: stage,
        parentPath: planetPath,
        planetSize: size,
        moonIndex: moonIndex
      )
    }

    // Add rings to Saturn-like planets
    if name == "Saturn" {
      createRingsForBenchmark(stage: stage, planetPath: planetPath, innerRadius: size * 1.5, outerRadius: size * 2.5)
    }
  }

  private static func createMoonForBenchmark(
    stage: UsdStageRefPtr,
    parentPath: String,
    planetSize: Double,
    moonIndex: Int
  ) {
    // Moon orbit pivot
    let moonOrbitPath = "\(parentPath)/Moon\(moonIndex)_Orbit"
    let moonOrbit = UsdGeom.Xform.define(stage, path: moonOrbitPath)

    // Different orbit phase for each moon
    let phaseOffset = Float(moonIndex) * 90.0
    moonOrbit.addRotateYOp().set(phaseOffset)

    // Moon orbit rotation
    let moonOrbitRotate = moonOrbit.addRotateYOp(suffix: Tf.Token("anim"))
    let moonSpeed = 3.0 + Double(moonIndex) * 0.5
    animateRotationOp(moonOrbitRotate, duration: 120, rotations: moonSpeed)

    // Moon sphere
    let moonPath = "\(moonOrbitPath)/Moon\(moonIndex)"
    let moon = UsdGeom.Sphere.define(stage, path: moonPath)

    let moonSize = planetSize * 0.2
    moon.CreateRadiusAttr(Vt.Value(moonSize), false)

    // Position moon relative to planet
    let moonDistance = planetSize * (1.8 + Double(moonIndex) * 0.4)
    moon.addTranslateOp().set(GfVec3d(moonDistance, 0.0, 0.0))

    // Moon material
    UsdShade.MaterialBindingAPI.apply(moon.GetPrim()).bind(
      matDefMetallicWithFallback(stage, color: .white, roughness: 0.6)
    )
  }

  private static func createRingsForBenchmark(
    stage: UsdStageRefPtr,
    planetPath: String,
    innerRadius: Double,
    outerRadius: Double
  ) {
    // Create ring as a torus-like structure using multiple thin cylinders
    let ringsPath = "\(planetPath)/Rings"
    let rings = UsdGeom.Xform.define(stage, path: ringsPath)

    // Tilt rings slightly
    rings.addRotateXOp().set(Float(20.0))

    // Create ring segments
    let segmentCount = 12
    for i in 0..<segmentCount {
      let angle = Double(i) * (360.0 / Double(segmentCount))
      let radius = (innerRadius + outerRadius) / 2.0

      let segPath = "\(ringsPath)/Segment\(i)"
      let segment = UsdGeom.Cube.define(stage, path: segPath)

      // Position and rotate segment
      let radians = angle * .pi / 180.0
      let x = cos(radians) * radius
      let z = sin(radians) * radius

      segment.addTranslateOp().set(GfVec3d(x, 0.0, z))
      segment.addRotateYOp().set(Float(-angle))
      segment.addScaleOp().set(GfVec3f(Float((outerRadius - innerRadius) * 0.8), 0.02, 0.3))

      // Ring material
      UsdShade.MaterialBindingAPI.apply(segment.GetPrim()).bind(
        matDefMetallicWithFallback(stage, color: .yellow, roughness: 0.3)
      )
    }
  }

  private static func createAsteroidBeltForBenchmark(
    stage: UsdStageRefPtr,
    parentPath: String,
    innerRadius: Double,
    outerRadius: Double,
    count: Int
  ) {
    // Use PointInstancer for GPU-accelerated rendering
    // This allows thousands of asteroids with a single draw call!
    let beltPath = "\(parentPath)/AsteroidBelt"
    let instancer = UsdGeom.PointInstancer.define(stage, path: beltPath)

    // Slowly rotate the entire belt (the instancer itself)
    let beltRotate = instancer.addRotateYOp()
    animateRotationOp(beltRotate, duration: 120, rotations: 0.2)

    // Create prototype geometry (only one cube for all asteroids)
    let prototypesPath = "\(beltPath)/Prototypes"
    UsdGeom.Xform.define(stage, path: prototypesPath)
    let asteroidPrototype = UsdGeom.Cube.define(stage, path: "\(prototypesPath)/Asteroid")

    // Apply gray rocky material to prototype
    UsdShade.MaterialBindingAPI.apply(asteroidPrototype.GetPrim()).bind(
      matDefMetallicWithFallback(stage, color: .white, roughness: 0.8)
    )

    // Set up prototypes relationship
    instancer.getPrototypesRel().AddTarget(Sdf.Path("\(prototypesPath)/Asteroid"))

    // Build per-instance arrays
    var protoIndices = Pixar.VtIntArray()
    var positions = Pixar.VtVec3fArray()
    var orientations = Pixar.VtQuathArray()
    var scales = Pixar.VtVec3fArray()

    for i in 0..<count {
      let angle = Double(i) * (360.0 / Double(count))
      let radius = innerRadius + Double.random(in: 0...(outerRadius - innerRadius))

      // All instances use prototype 0 (the only one)
      protoIndices.push_back(0)

      // Position
      let radians = angle * .pi / 180.0
      let x = Float(cos(radians) * radius)
      let z = Float(sin(radians) * radius)
      let y = Float.random(in: -0.2...0.2)
      positions.push_back(GfVec3f(x, y, z))

      // Use identity orientation (1, 0, 0, 0) - the random non-uniform scale provides visual variety
      // Note: GfQuath identity quaternion has w=1, xyz=0
      orientations.push_back(Pixar.GfQuath.GetIdentity())

      // Random scale
      let size = Float.random(in: 0.05...0.15)
      scales.push_back(GfVec3f(size, size * 0.7, size * 0.8))
    }

    // Set the per-instance data
    instancer.createProtoIndicesAttr().set(Vt.Value(protoIndices))
    instancer.createPositionsAttr().set(Vt.Value(positions))
    instancer.createOrientationsAttr().set(Vt.Value(orientations))
    instancer.createScalesAttr().set(Vt.Value(scales))
  }

  // MARK: - Animation Helpers

  private static func animateRotationOp(
    _ rotateOp: Pixar.UsdGeomXformOp,
    duration: Int,
    rotations: Double
  ) {
    for frame in stride(from: 0, through: duration, by: 2) {
      let progress = Double(frame) / Double(duration)
      let angle = Float(progress * rotations * 360.0)
      rotateOp.set(angle, time: Usd.TimeCode(Double(frame)))
    }
  }
}
