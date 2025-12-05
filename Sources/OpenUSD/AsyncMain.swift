/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
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

      // Disable colored output if run from Xcode (the Xcode console does not support colors)
      // Note: Rainbow.enabled access is disabled due to Swift 6 concurrency restrictions
    #endif

    await OpenUSD.main()
  }
}
