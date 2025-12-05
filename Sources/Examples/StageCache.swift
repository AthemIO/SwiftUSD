/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

// DISABLED: UsdStageCacheContext binding API needs Swift wrapper updates
// func stageCacheBind()
// {
//   /* Create new stage for this example. */
//
//   let newStage: UsdStageRefPtr = Usd.Stage.createNew("\(documentsDirPath())/UsdStageCacheExample", ext: .usd)
//   newStage.save()
//
//   Msg.logger.info("created a new stage.")
//
//   /* Create the cache and bind the cache context. */
//
//   var stageCache = UsdStageCache()
//   UsdStageCacheContext.bind(cache: &stageCache)
//
//   Msg.logger.info("created a new usd stage cache, and bound it to a cache context.")
//
//   Msg.logger.info("inserting stage into usd stage cache...")
//   let stage = Usd.Stage.open("\(documentsDirPath())/UsdStageCacheExample", ext: .usd)
//   Msg.logger.info("checking if usd stage cache contains stage: \(stageCache.contains(stage))")
//
//   Msg.logger.info("attempting to retrieve stage from the cache.")
//   let stage2 = Usd.Stage.open("\(documentsDirPath())/UsdStageCacheExample", ext: .usd)
//
//   let id1 = stageCache.GetId(stage).ToLongInt()
//   let id2 = stageCache.GetId(stage2).ToLongInt()
//   Msg.logger.info("stage successfully retrieved from the cache: \(id1 == id2)")
// }

public enum StageCacheExamples
{
  static func run()
  {
    // DISABLED: UsdStageCacheContext binding API needs Swift wrapper updates
    Msg.logger.info("scene cache examples disabled - needs API updates")
  }
}
