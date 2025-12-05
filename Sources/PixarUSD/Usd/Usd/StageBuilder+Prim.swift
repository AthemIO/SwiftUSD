/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import Usd

public protocol Prim
{
  var path: Sdf.Path { get }

  var name: Tf.Token { get }

  var children: [any Prim] { get }
}

/**
 * A ``Usd/Prim`` for declaratively authoring scene description
 * on a ``USDStage``. */
public struct USDPrim
{
  public var path: Sdf.Path
  public var type: PrimType
  public var children: [USDPrim]

  public init(_ path: String, type: PrimType = .token(Tf.Token()), @StageBuilder children: () -> [USDPrim] = { [] })
  {
    self.path = Sdf.Path("/\(path)")
    self.type = type
    self.children = children()
  }
}

extension USDPrim: Equatable
{
  public static func == (lhs: USDPrim, rhs: USDPrim) -> Bool
  {
    lhs.path.string == rhs.path.string
  }
}

public extension USDPrim
{
  enum PrimType
  {
    case basisCurves
    case hermiteCurves
    case nurbsCurves
    case nurbsPatch
    case boundable
    case imageable
    case mesh
    case pointBased
    case pointInstancer
    case points
    case plane
    case camera
    case capsule
    case cone
    case cube
    case curves
    case cylinder
    case sphere
    case scope
    case geomSubset
    case gprim
    case distantLight
    case diskLight
    case rectLight
    case sphereLight
    case cylinderLight
    case geometryLight
    case domeLight
    case portalLight
    case xform
    case xformable
    case xformCommonAPI
    case lightAPI
    case meshLightAPI
    case volumeLightAPI
    case motionAPI
    case primvarsAPI
    case geomModelAPI
    case visibilityAPI
    case token(Tf.Token)
    case group([USDPrim])
  }
}
