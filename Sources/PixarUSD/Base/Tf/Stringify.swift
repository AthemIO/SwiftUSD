/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

/**
 * A macro that produces a string containing
 * the source code that generated the value.
 *
 * For example:
 *
 *   #stringify(x + y)
 *
 * produces a string `"x + y"`. */
@freestanding(expression)
public macro stringify<T>(_ value: T) -> String = #externalMacro(module: "PixarMacros", type: "StringifyMacro")
