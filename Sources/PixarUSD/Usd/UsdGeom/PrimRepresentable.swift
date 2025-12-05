/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Usd

@attached(member, names: arbitrary, conformances: PrimRepresentable)
public macro Prim() = #externalMacro(module: "PixarMacros", type: "PixarPrimMacro")

public protocol PrimRepresentable
{
  func getAttribute(_ name: Tf.Token) -> Usd.Attribute
}
