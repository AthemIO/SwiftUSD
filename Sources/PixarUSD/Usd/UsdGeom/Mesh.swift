/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomMesh = Pixar.UsdGeomMesh

public extension UsdGeom
{
  typealias Mesh = UsdGeomMesh
}

@Xformable
extension UsdGeom.Mesh: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Mesh
  {
    UsdGeom.Mesh.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Mesh
  {
    UsdGeom.Mesh.define(stage, path: .init(path))
  }
}
