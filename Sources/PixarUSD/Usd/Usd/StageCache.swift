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
import Usd

public typealias UsdStageCache = Pixar.UsdStageCache

// TODO: UsdStageCacheContext uses TF_DEFINE_STACKED macro which creates
// complex template metaprogramming that Swift C++ interop cannot import.
// public typealias UsdStageCacheContext = Pixar.UsdStageCacheContext

public extension UsdStageCache
{
  func contains(_ stage: UsdStageRefPtr) -> Bool
  {
    Contains(stage)
  }
}

// TODO: UsdStageCacheContext extension commented out due to Swift interop issues
// public extension UsdStageCacheContext
// {
//   @discardableResult
//   static func bind(cache: inout UsdStageCache) -> UsdStageCacheContext
//   {
//     UsdStageCacheContext.CreateCache(&cache)
//   }
// }
