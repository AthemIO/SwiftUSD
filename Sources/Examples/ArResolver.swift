/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

final class CustomResolver: ArDefaultResolver {
    func _resolve(_ path: std.string) -> Pixar.ArResolvedPath {
        Msg.logger.info("resolving path: \(path)")

        // let asset = _Resolve(path)
        // if !asset.empty()
        // {
        //   return asset
        // }

        return .init()
    }
}

public enum ArResolverExamples {
    public static func run() {
        Msg.logger.info("running ar resolver examples...")

        Msg.logger.info("creating new ar custom resolver.")
        let resolver = CustomResolver()
        let resolvedPath = resolver._resolve(std.string("HelloPixarUSD.usda"))
        Msg.logger.info("resolved path: \(resolvedPath.empty() ? "empty" : resolvedPath.path)")

        Msg.logger.info("ar resolver examples succesfully completed.")
    }
}
