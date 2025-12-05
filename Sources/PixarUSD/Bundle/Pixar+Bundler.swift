/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Arch
import CxxStdlib
import Foundation
import Plug
import Rainbow

public extension Pixar
{
  final class Bundler: Sendable
  {
    public static let shared = Bundler()

    private init()
    {}

    public func setup(_ kind: BundleKind)
    {
      switch kind
      {
        case .resources:
          resourcesInit()
      }
    }

    private func resourcesInit()
    {
      /* 1. find all resource paths (ex. Usd/Contents/Resources) */
      let resources = BundleFramework.allCases.compactMap(\.resourcePath)

      /* 2. fill a std.vector of std.string plugin paths. */
      var plugPaths = Pixar.PlugRegistry.PlugPathsVector()
      _ = resources.map
      { path in
        #if DEBUG_PIXAR_BUNDLE
          Msg.logger.log(level: .info, "Adding usd resource -> \(path)")
        #endif /* DEBUG_PIXAR_BUNDLE */

        plugPaths.push_back(std.string(path))
      }

      /* 3. registers all plugins discovered in any plugPaths. */
      Pixar.PlugRegistry.GetInstance().RegisterPlugins(plugPaths)

      /* 4. Set up MaterialX standard library search path.
       *    UsdMtlx and HdMtlx look for MaterialX libraries via the
       *    PXR_MTLX_STDLIB_SEARCH_PATHS environment variable. */
      if let materialXBundle = Bundle.materialX,
         let materialXLibraries = materialXBundle.path(forResource: "libraries", ofType: nil)
      {
        #if DEBUG_PIXAR_BUNDLE
          Msg.logger.log(level: .info, "Setting MaterialX stdlib path -> \(materialXLibraries)")
        #endif /* DEBUG_PIXAR_BUNDLE */
        setenv("PXR_MTLX_STDLIB_SEARCH_PATHS", materialXLibraries, 1)
      }
    }
  }
}
