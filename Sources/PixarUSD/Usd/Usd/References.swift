/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import Foundation
import Usd

public typealias UsdReferences = Pixar.UsdReferences

public extension Usd
{
  typealias References = UsdReferences
}

public extension Usd.References
{
  /// UsdListPosition enum values for Swift (C++ enum doesn't export named values)
  /// - UsdListPositionFrontOfPrependList = 0
  /// - UsdListPositionBackOfPrependList = 1
  /// - UsdListPositionFrontOfAppendList = 2
  /// - UsdListPositionBackOfAppendList = 3
  static var listPositionBackOfPrependList: Pixar.UsdListPosition { Pixar.UsdListPosition(1) }

  @discardableResult
  mutating func addReference(_ ref: Sdf.Reference, position: Pixar.UsdListPosition = Pixar.UsdListPosition(1)) -> Bool
  {
    AddReference(ref, position)
  }

  @discardableResult
  mutating func addReference(assetPath: String,
                             primPath: Sdf.Path,
                             layerOffset: Sdf.LayerOffset = Sdf.LayerOffset(),
                             position: Pixar.UsdListPosition = Pixar.UsdListPosition(1)) -> Bool
  {
    AddReference(std.string(assetPath), primPath, layerOffset, position)
  }

  @discardableResult
  mutating func addReference(assetPath: String,
                             layerOffset: Sdf.LayerOffset = Sdf.LayerOffset(),
                             position: Pixar.UsdListPosition = Pixar.UsdListPosition(1)) -> Bool
  {
    AddReference(std.string(assetPath), layerOffset, position)
  }
}
