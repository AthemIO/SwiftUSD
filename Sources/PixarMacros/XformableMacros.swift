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
    let decl: DeclSyntax = """
      public func addXformOp(type: UsdGeomXformOp.`Type`,
                             precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(0),
                             suffix: Tf.Token = Tf.Token(),
                             invert: Bool = false) -> UsdGeomXformOp
      {
        AddXformOp(type, precision, suffix, invert)
      }

      public func addTranslateOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(0),
                                 suffix: Tf.Token = Tf.Token(),
                                 invert: Bool = false) -> UsdGeomXformOp
      {
        AddTranslateOp(precision, suffix, invert)
      }

      public func addScaleOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                             suffix: Tf.Token = Tf.Token(),
                             invert: Bool = false) -> UsdGeomXformOp
      {
        AddScaleOp(precision, suffix, invert)
      }

      public func addRotateXOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                               suffix: Tf.Token = Tf.Token(),
                               invert: Bool = false) -> UsdGeomXformOp
      {
        AddRotateXOp(precision, suffix, invert)
      }

      public func addRotateYOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                               suffix: Tf.Token = Tf.Token(),
                               invert: Bool = false) -> UsdGeomXformOp
      {
        AddRotateYOp(precision, suffix, invert)
      }

      public func addRotateZOp(precision: UsdGeomXformOp.Precision = UsdGeomXformOp.Precision(1),
                               suffix: Tf.Token = Tf.Token(),
                               invert: Bool = false) -> UsdGeomXformOp
      {
        AddRotateZOp(precision, suffix, invert)
      }
      """

    return [decl]
  }
}
