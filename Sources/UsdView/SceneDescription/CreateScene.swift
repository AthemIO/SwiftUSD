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

    // Add rotation animation to the subdivided cube (rotates 360° over 120 frames)
    let subdivRotateOp = subdividedMesh.addRotateYOp()
    subdivRotateOp.set(Float(0.0), time: Usd.TimeCode(0.0))
    subdivRotateOp.set(Float(360.0), time: Usd.TimeCode(120.0))

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

    // Add rotation animation to the blue cube (rotates opposite direction)
    let cubeRotateOp = cube.addRotateXOp()
    cubeRotateOp.set(Float(0.0), time: Usd.TimeCode(0.0))
    cubeRotateOp.set(Float(-360.0), time: Usd.TimeCode(120.0))

    // Add bouncing animation (up and down)
    let cubeTranslateYOp = cube.addTranslateOp(suffix: Tf.Token("bounce"))
    cubeTranslateYOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(0.0))
    cubeTranslateYOp.set(GfVec3d(0.0, 0.5, 0.0), time: Usd.TimeCode(30.0))
    cubeTranslateYOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(60.0))
    cubeTranslateYOp.set(GfVec3d(0.0, 0.5, 0.0), time: Usd.TimeCode(90.0))
    cubeTranslateYOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(120.0))

    UsdShade.MaterialBindingAPI.apply(cube).bind(matDefMtlxSubsurface(stage, color: .blue, subsurfaceScale: 0.5))

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
