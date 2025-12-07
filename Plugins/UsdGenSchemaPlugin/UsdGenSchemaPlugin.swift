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
struct UsdGenSchemaPlugin: CommandPlugin
{
  /// This entry point is called when operating on a Swift package.
  func performCommand(context: PluginContext, arguments: [String]) throws
  {
    let genSchema = try context.tool(named: "UsdGenSchema")

    try run(command: genSchema.url, with: arguments)
  }
}

#if canImport(XcodeProjectPlugin)
  import XcodeProjectPlugin

  extension UsdGenSchemaPlugin: XcodeCommandPlugin
  {
    /// This entry point is called when operating on an Xcode project.
    func performCommand(context: XcodePluginContext, arguments: [String]) throws
    {
      let genSchema = try context.tool(named: "UsdGenSchema")

      try run(command: genSchema.url, with: arguments)
    }
  }
#endif

extension UsdGenSchemaPlugin
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
      print("UsdGenSchemaPlugin succesfully completed.")
    }
    else
    {
      let problem = "\(process.terminationReason):\(process.terminationStatus)"
      Diagnostics.error("UsdGenSchemaPlugin failed: \(problem)")
    }
  }
}
