/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar
import CxxStdlib

public extension ArAssetInfo
{
  // MARK: - Swift-Friendly Accessors

  /// Version of the resolved asset, if any.
  var versionString: String
  {
    get { String(version) }
    set { version = std.string(newValue) }
  }

  /// The name of the asset represented by the resolved asset, if any.
  var name: String
  {
    get { String(assetName) }
    set { assetName = std.string(newValue) }
  }

  /// The repository path corresponding to the resolved asset (deprecated).
  @available(*, deprecated, message: "Use resolverInfo instead")
  var repositoryPath: String
  {
    get { String(repoPath) }
    set { repoPath = std.string(newValue) }
  }

  /// Check if the asset info has valid version information.
  var hasVersion: Bool
  {
    !version.empty()
  }

  /// Check if the asset info has a valid asset name.
  var hasName: Bool
  {
    !assetName.empty()
  }

  /// Check if the asset info has resolver-specific information.
  var hasResolverInfo: Bool
  {
    !resolverInfo.IsEmpty()
  }
}

// MARK: - CustomStringConvertible

extension ArAssetInfo: CustomStringConvertible
{
  public var description: String
  {
    var parts: [String] = []
    if hasVersion
    {
      parts.append("version: \(versionString)")
    }
    if hasName
    {
      parts.append("name: \(name)")
    }
    return "ArAssetInfo(\(parts.joined(separator: ", ")))"
  }
}
