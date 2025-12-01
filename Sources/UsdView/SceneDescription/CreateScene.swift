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
   * Create a basic usd scene. */
  static func createScene() -> UsdStageRefPtr
  {
    /* Create stage with a dome light & sphere on a transform. */

    let stage = Usd.Stage.createNew("\(documentsDirPath())/HelloWorldExample", ext: .usd)

    let domeLight = UsdLux.DomeLight.define(stage, path: "/World/DefaultDomeLight")

    if let hdxResources = Bundle.hdx?.resourcePath {
      let tex = "\(hdxResources)/textures/StinsonBeach.hdr"

      if FileManager.default.fileExists(atPath: tex) {
        let hdrAsset = Sdf.AssetPath(tex)
        domeLight.createTextureFileAttr().set(hdrAsset)
      }
    }

    // Use convenience methods from @Xformable macro instead of addXformOp with type enum
    // (UsdGeomXformOp.Type enum doesn't export named values to Swift)
    let xform = UsdGeom.Xform.define(stage, path: "/Geometry")
    xform.addTranslateOp().set(GfVec3d(0.0, 0.0, 0.0))
    xform.addScaleOp().set(GfVec3f(1, 1, 1))

    // Create a red sphere on the left
    let sphere = UsdGeom.Sphere.define(stage, path: "/Geometry/Sphere")
    sphere.addTranslateOp().set(GfVec3d(-1.5, 0.0, 0.0))
    UsdShade.MaterialBindingAPI.apply(sphere).bind(matDef(stage, color: .red))

    // Create a blue cube on the right
    let cube = UsdGeom.Cube.define(stage, path: "/Geometry/Cube")
    cube.addTranslateOp().set(GfVec3d(1.5, 0.0, 0.0))
    UsdShade.MaterialBindingAPI.apply(cube).bind(matDef(stage, color: .blue))

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
