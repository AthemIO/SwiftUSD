/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import SwiftCompilerPlugin
import SwiftSyntax
import SwiftSyntaxBuilder
import SwiftSyntaxMacros

public struct PixarXformableMacro: MemberMacro
{
  public static func expansion(of _: SwiftSyntax.AttributeSyntax,
                               providingMembersOf _: some SwiftSyntax.DeclGroupSyntax,
                               in _: some SwiftSyntaxMacros.MacroExpansionContext) throws -> [SwiftSyntax.DeclSyntax]
  {
    // UsdGeomXformOp.Precision enum values (C++ enum doesn't export named values):
    // - PrecisionDouble = 0: Double precision
    // - PrecisionFloat = 1: Floating-point precision
    // - PrecisionHalf = 2: Half-float precision
    //
    // Note: xformablePrim() must be implemented by each conforming type to work
    // around Swift C++ interop's ambiguous method resolution for GetPrim().
    // The bridge functions in xformableBridge.h take UsdPrim directly.
    let decl: DeclSyntax = """
      public func addXformOp(type: UsdGeomXformOp.`Type`,
                             precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(0),
                             suffix: Tf.Token = Tf.Token(),
                             invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let opType: UsdGeomXformOp.`Type` = type
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddXformOp(prim, opType, prec, sfx, inv)
      }

      public func addTranslateOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(0),
                                 suffix: Tf.Token = Tf.Token(),
                                 invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddTranslateOp(prim, prec, sfx, inv)
      }

      public func addScaleOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                             suffix: Tf.Token = Tf.Token(),
                             invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddScaleOp(prim, prec, sfx, inv)
      }

      public func addRotateXOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                               suffix: Tf.Token = Tf.Token(),
                               invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddRotateXOp(prim, prec, sfx, inv)
      }

      public func addRotateYOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                               suffix: Tf.Token = Tf.Token(),
                               invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddRotateYOp(prim, prec, sfx, inv)
      }

      public func addRotateZOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                               suffix: Tf.Token = Tf.Token(),
                               invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddRotateZOp(prim, prec, sfx, inv)
      }

      public func addRotateXYZOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                                 suffix: Tf.Token = Tf.Token(),
                                 invert: Bool = false) -> UsdGeomXformOp
      {
        let prim = xformablePrim()
        let prec: UsdGeomXformOp.Precision = precision
        let sfx: TfToken = suffix
        let inv: Bool = invert
        return Pixar.UsdGeomXformable_AddRotateXYZOp(prim, prec, sfx, inv)
      }
      """

    return [decl]
  }
}
