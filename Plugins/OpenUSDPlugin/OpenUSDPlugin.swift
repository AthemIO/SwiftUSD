/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PackagePlugin

@main
struct OpenUSDPlugin: CommandPlugin
{
  func performCommand(context: PluginContext, arguments: [String]) throws
  {
    let openusd = try context.tool(named: "OpenUSD")

    try run(command: openusd.url, with: arguments)
  }
}

extension OpenUSDPlugin
{
  /// Run a command with the given arguments.
  func run(command: URL, with arguments: [String]) throws
  {
    let process = try Process.run(command, arguments: arguments)
    process.waitUntilExit()

    // Check whether the subprocess invocation was successful.
    if process.terminationReason == .exit,
       process.terminationStatus == 0
    {
      print("openusd success: completed.")
    }
    else
    {
      let problem = "\(process.terminationReason):\(process.terminationStatus)"
      Diagnostics.error("openusd failed: \(problem)")
    }
  }
}
