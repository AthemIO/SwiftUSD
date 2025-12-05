/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import ArgumentParser
import Foundation

struct UpdateArguments: ParsableArguments
{
  /// The version of OpenUSD to migrate to.
  @Argument(
    help: "The version of OpenUSD to migrate to.")
  var usdVersion: String?

  /// The directory containing the package to update.
  @Option(
    name: [.customShort("d"), .customLong("directory")],
    help: "The directory containing the package to update.",
    transform: URL.init(fileURLWithPath:)
  )
  var packageDirectory: URL?
}
