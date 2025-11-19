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
import Rainbow

@main
struct AsyncMain
{
  static func main() async
  {
    #if os(macOS)
      // Kill all running processes on exit
      for signal in Signal.allCases
      {
        trap(signal)
        { _ in
          for process in processes
          {
            process.terminate()
          }
          Foundation.exit(1)
        }
      }

      // Note: Rainbow colorization setup removed due to Swift 6 concurrency restrictions
      // Colors are controlled by Rainbow.enabled default value (respects NO_COLOR env var)
    #endif

    await OpenUSD.main()
  }
}
