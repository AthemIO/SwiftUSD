/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import SwiftCompilerPlugin
import SwiftSyntaxMacros

@main
struct PixarMacrosPlugin: CompilerPlugin
{
  let providingMacros: [Macro.Type] = [
    PixarXformableMacro.self,
    PixarPrimMacro.self
  ]
}
