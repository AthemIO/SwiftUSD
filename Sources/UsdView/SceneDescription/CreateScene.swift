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

extension UsdView
{
  // MARK: - Spline Animation Helpers

  /**
   * Create smooth ease-in/ease-out animation using cubic Bezier interpolation.
   * This generates a smooth curve that accelerates and decelerates naturally.
   *
   * - Parameters:
   *   - keyframes: Array of (time, value) tuples for key poses
   *   - startFrame: Start frame for sampling
   *   - endFrame: End frame for sampling
   * - Returns: Array of (time, value) tuples with smoothly interpolated values
   */
  static func createAutoEaseSpline(
    keyframes: [(time: Double, value: Double)],
    startFrame: Double,
    endFrame: Double,
    samplesPerFrame: Int = 1
  ) -> [(time: Double, value: Double)]
  {
    guard keyframes.count >= 2 else { return keyframes }

    var samples: [(time: Double, value: Double)] = []
    let frameStep = 1.0 / Double(samplesPerFrame)

    // For each pair of keyframes, interpolate with ease-in/ease-out
    for i in 0..<(keyframes.count - 1) {
      let kf1 = keyframes[i]
      let kf2 = keyframes[i + 1]

      let t1 = kf1.time
      let t2 = kf2.time
      let v1 = kf1.value
      let v2 = kf2.value

      var t = t1
      while t < t2 {
        // Normalize time to [0, 1]
        let u = (t - t1) / (t2 - t1)

        // Cubic ease-in-out function: 3u² - 2u³ (smoothstep)
        let ease = u * u * (3.0 - 2.0 * u)

        // Interpolate value
        let value = v1 + (v2 - v1) * ease

        if t >= startFrame && t <= endFrame {
          samples.append((time: t, value: value))
        }
        t += frameStep
      }
    }

    // Add the final keyframe
    if let last = keyframes.last, last.time <= endFrame {
      samples.append((time: last.time, value: last.value))
    }

    return samples
  }

  /**
   * Create a custom bounce animation that simulates a ball bouncing.
   * Uses parabolic motion for realistic physics-based bounce.
   *
   * - Parameters:
   *   - bounceHeight: Maximum height of first bounce
   *   - numBounces: Number of bounces
   *   - dampingFactor: How much energy is lost per bounce (0.0-1.0)
   *   - startFrame: Start frame
   *   - framesPerBounce: Frames for first bounce (subsequent bounces are shorter)
   * - Returns: Array of (time, value) tuples for the bounce animation
   */
  static func createBounceSpline(
    bounceHeight: Double,
    numBounces: Int,
    dampingFactor: Double,
    startFrame: Double,
    framesPerBounce: Double
  ) -> [(time: Double, value: Double)]
  {
    var samples: [(time: Double, value: Double)] = []
    var currentTime = startFrame
    var currentHeight = bounceHeight

    for bounce in 0..<numBounces {
      let bounceDuration = framesPerBounce * pow(dampingFactor, Double(bounce))
      let halfDuration = bounceDuration / 2.0

      // Sample the upward arc (rising)
      var t: Double = 0
      while t < halfDuration {
        let u = t / halfDuration
        // Parabolic motion: h = h_max * (1 - (1-u)²) for rising
        // Simplified: value increases with deceleration
        let value = currentHeight * (1.0 - (1.0 - u) * (1.0 - u))
        samples.append((time: currentTime + t, value: value))
        t += 1.0
      }

      // Peak
      samples.append((time: currentTime + halfDuration, value: currentHeight))

      // Sample the downward arc (falling)
      t = 0
      while t < halfDuration {
        let u = t / halfDuration
        // Parabolic motion: h = h_max * (1 - u²) for falling
        // Value decreases with acceleration (gravity)
        let value = currentHeight * (1.0 - u * u)
        samples.append((time: currentTime + halfDuration + t, value: value))
        t += 1.0
      }

      currentTime += bounceDuration
      currentHeight *= dampingFactor
    }

    // Add final ground position
    samples.append((time: currentTime, value: 0.0))

    return samples
  }

  // MARK: - Scene Creation

  /**
   * Create a basic usd scene with animation. */
  static func createScene() -> UsdStageRefPtr
  {
    /* Create stage with a dome light & sphere on a transform. */

    let stage = Usd.Stage.createNew("\(documentsDirPath())/HelloWorldExample", ext: .usd)

    // Set up animation time range (0 to 120 frames at 24fps = 5 seconds)
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
    // Rotate the distant light to shine from above-front-left
    distantLight.addRotateXYZOp().set(GfVec3f(-45.0, -30.0, 0.0))

    // Use convenience methods from @Xformable macro instead of addXformOp with type enum
    // (UsdGeomXformOp.Type enum doesn't export named values to Swift)
    let xform = UsdGeom.Xform.define(stage, path: "/Geometry")
    xform.addTranslateOp().set(GfVec3d(0.0, 0.0, 0.0))
    xform.addScaleOp().set(GfVec3f(1, 1, 1))

    // Create a subdivided cube mesh on the left (using OpenSubdiv Catmull-Clark)
    // This demonstrates OpenSubdiv - a cube becomes smooth/rounded when subdivided
    let subdividedMesh = UsdGeom.Mesh.define(stage, path: "/Geometry/SubdividedCube")
    subdividedMesh.addTranslateOp().set(GfVec3d(-1.5, 0.0, 0.0))

    // Add rotation animation using AutoEase spline for smooth acceleration/deceleration
    let subdivRotateOp = subdividedMesh.addRotateYOp()

    // Create AutoEase spline for smooth rotation with ease-in/ease-out
    let rotationKeyframes: [(time: Double, value: Double)] = [
      (0.0, 0.0),
      (30.0, 90.0),
      (60.0, 180.0),
      (90.0, 270.0),
      (120.0, 360.0)
    ]
    let smoothRotation = createAutoEaseSpline(
      keyframes: rotationKeyframes,
      startFrame: 0.0,
      endFrame: 120.0,
      samplesPerFrame: 1
    )

    // Apply the smooth rotation samples
    for sample in smoothRotation {
      subdivRotateOp.set(Float(sample.value), time: Usd.TimeCode(sample.time))
    }

    // Define cube vertices (8 corners of a unit cube centered at origin)
    var points = Vt.Vec3fArray()
    points.push_back(GfVec3f(-0.5, -0.5, -0.5))  // 0: back-bottom-left
    points.push_back(GfVec3f( 0.5, -0.5, -0.5))  // 1: back-bottom-right
    points.push_back(GfVec3f( 0.5,  0.5, -0.5))  // 2: back-top-right
    points.push_back(GfVec3f(-0.5,  0.5, -0.5))  // 3: back-top-left
    points.push_back(GfVec3f(-0.5, -0.5,  0.5))  // 4: front-bottom-left
    points.push_back(GfVec3f( 0.5, -0.5,  0.5))  // 5: front-bottom-right
    points.push_back(GfVec3f( 0.5,  0.5,  0.5))  // 6: front-top-right
    points.push_back(GfVec3f(-0.5,  0.5,  0.5))  // 7: front-top-left
    subdividedMesh.CreatePointsAttr(Vt.Value(points), false)

    // Define 6 quad faces (4 vertices each)
    var faceVertexCounts = Vt.IntArray()
    for _ in 0..<6 { faceVertexCounts.push_back(4) }
    subdividedMesh.CreateFaceVertexCountsAttr(Vt.Value(faceVertexCounts), false)

    // Define face vertex indices (counter-clockwise winding)
    var faceVertexIndices = Vt.IntArray()
    // Front face
    faceVertexIndices.push_back(4); faceVertexIndices.push_back(5)
    faceVertexIndices.push_back(6); faceVertexIndices.push_back(7)
    // Back face
    faceVertexIndices.push_back(1); faceVertexIndices.push_back(0)
    faceVertexIndices.push_back(3); faceVertexIndices.push_back(2)
    // Top face
    faceVertexIndices.push_back(3); faceVertexIndices.push_back(7)
    faceVertexIndices.push_back(6); faceVertexIndices.push_back(2)
    // Bottom face
    faceVertexIndices.push_back(0); faceVertexIndices.push_back(1)
    faceVertexIndices.push_back(5); faceVertexIndices.push_back(4)
    // Right face
    faceVertexIndices.push_back(1); faceVertexIndices.push_back(2)
    faceVertexIndices.push_back(6); faceVertexIndices.push_back(5)
    // Left face
    faceVertexIndices.push_back(0); faceVertexIndices.push_back(4)
    faceVertexIndices.push_back(7); faceVertexIndices.push_back(3)
    subdividedMesh.CreateFaceVertexIndicesAttr(Vt.Value(faceVertexIndices), false)

    // Apply Catmull-Clark subdivision (OpenSubdiv)
    subdividedMesh.CreateSubdivisionSchemeAttr(Vt.Value(Tf.Token("catmullClark")), false)

    // Bind orange metallic material
    UsdShade.MaterialBindingAPI.apply(subdividedMesh.GetPrim()).bind(matDefMtlxMetallic(stage, color: .orange, roughness: 0.15))

    // Create a blue cube with subsurface scattering on the right (using MaterialX)
    let cube = UsdGeom.Cube.define(stage, path: "/Geometry/Cube")
    cube.addTranslateOp().set(GfVec3d(1.5, 0.0, 0.0))

    // Add rotation animation using AutoEase for smooth spinning
    let cubeRotateOp = cube.addRotateXOp()
    let cubeRotationKeyframes: [(time: Double, value: Double)] = [
      (0.0, 0.0),
      (60.0, -180.0),
      (120.0, -360.0)
    ]
    let smoothCubeRotation = createAutoEaseSpline(
      keyframes: cubeRotationKeyframes,
      startFrame: 0.0,
      endFrame: 120.0,
      samplesPerFrame: 1
    )
    for sample in smoothCubeRotation {
      cubeRotateOp.set(Float(sample.value), time: Usd.TimeCode(sample.time))
    }

    // Add bouncing animation using AutoEase (smooth bounce up and down)
    let cubeTranslateYOp = cube.addTranslateOp(suffix: Tf.Token("bounce"))
    let bounceKeyframes: [(time: Double, value: Double)] = [
      (0.0, 0.0),
      (15.0, 0.5),
      (30.0, 0.0),
      (45.0, 0.5),
      (60.0, 0.0),
      (75.0, 0.5),
      (90.0, 0.0),
      (105.0, 0.5),
      (120.0, 0.0)
    ]
    let smoothBounce = createAutoEaseSpline(
      keyframes: bounceKeyframes,
      startFrame: 0.0,
      endFrame: 120.0,
      samplesPerFrame: 1
    )
    for sample in smoothBounce {
      cubeTranslateYOp.set(GfVec3d(0.0, sample.value, 0.0), time: Usd.TimeCode(sample.time))
    }

    UsdShade.MaterialBindingAPI.apply(cube).bind(matDefMtlxSubsurface(stage, color: .blue, subsurfaceScale: 0.5))

    // Create a bouncing sphere with custom physics-based bounce animation
    let sphere = UsdGeom.Sphere.define(stage, path: "/Geometry/BouncingSphere")
    sphere.CreateRadiusAttr(Vt.Value(Double(0.3)), false)

    // Position sphere in front of the other objects
    sphere.addTranslateOp().set(GfVec3d(0.0, 0.0, 1.5))

    // Add custom bounce animation with physics-like damping
    let sphereBounceOp = sphere.addTranslateOp(suffix: Tf.Token("bounce"))
    let bounceSamples = createBounceSpline(
      bounceHeight: 1.5,
      numBounces: 4,
      dampingFactor: 0.7,
      startFrame: 0.0,
      framesPerBounce: 40.0
    )
    for sample in bounceSamples {
      sphereBounceOp.set(GfVec3d(0.0, sample.value, 0.0), time: Usd.TimeCode(sample.time))
    }

    // Bind a shiny red material to the sphere
    UsdShade.MaterialBindingAPI.apply(sphere).bind(matDefMtlxMetallic(stage, color: .red, roughness: 0.05))

    /* Iterate the stage and print out the path to each prim. */

    for prim in stage.traverse()
    {
      let primType = !prim.typeName.isEmpty ? "(\(prim.typeName.string))" : ""
      Msg.logger.log(level: .info, "\(prim.name.string)\(primType) -> \(prim.path.string)")
    }

    /* Save the stage to disk. */

    stage.getPseudoRoot().set(doc: "SwiftUSD v\(Pixar.version)")
    stage.save()

    return stage
  }
}
