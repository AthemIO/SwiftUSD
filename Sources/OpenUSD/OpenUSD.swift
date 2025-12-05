/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import ArgumentParser
import Foundation
import Version

/// The root command of OpenUSD.
struct OpenUSD: AsyncParsableCommand
{
  static let version = Version(1, 0, 0)

  static let configuration = CommandConfiguration(
    commandName: "openusd",
    abstract: "A tool for updating the version of USD in the current package.",
    version: "v" + version.description,
    shouldDisplay: true,
    subcommands: [
      UpdateCommand.self
    ]
  )

  @Flag(
    name: .shortAndLong,
    help: "Print verbose error messages."
  )
  var verbose = false

  func validate() throws
  {
    if verbose
    {
      log.logLevel = .debug
    }
  }
}
