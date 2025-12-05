/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import Foundation
import Sdf

public typealias SdfPath = Pixar.SdfPath

public extension Sdf
{
  typealias Path = SdfPath
}

public extension Sdf.Path
{
  init(_ path: String)
  {
    self.init(std.string(path))
  }

  static func emptyPath() -> Sdf.Path
  {
    Sdf.Path.EmptyPath().pointee
  }

  static func absoluteRootPath() -> Sdf.Path
  {
    Sdf.Path.AbsoluteRootPath().pointee
  }

  static func reflexiveRelativePath() -> Sdf.Path
  {
    Sdf.Path.ReflexiveRelativePath().pointee
  }

  private borrowing func GetNameCopy() -> std.string
  {
    __GetNameUnsafe().pointee
  }

  func getAsString() -> String
  {
    String(GetAsString())
  }

  func append(path: Sdf.Path) -> Sdf.Path
  {
    AppendPath(path)
  }

  var string: String
  {
    String(GetAsString())
  }

  var name: String
  {
    String(GetNameCopy())
  }
}
