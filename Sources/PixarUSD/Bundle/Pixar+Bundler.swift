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
      /* 1. Register core plugins first (sdf, ar, usd) to ensure SdfMetadata
       *    like `apiSchemas` is available before other plugins are loaded.
       *    This is critical because PlugRegistry processes plugins in parallel,
       *    and without this ordering, metadata may not be registered in time. */
      var corePlugPaths = Pixar.PlugRegistry.PlugPathsVector()
      for framework in BundleFramework.corePlugins
      {
        if let path = framework.resourcePath
        {
          #if DEBUG_PIXAR_BUNDLE
            Msg.logger.log(level: .info, "Adding core usd resource -> \(path)")
          #endif /* DEBUG_PIXAR_BUNDLE */
          corePlugPaths.push_back(std.string(path))
        }
      }
      Pixar.PlugRegistry.GetInstance().RegisterPlugins(corePlugPaths)

      /* 2. Register all other plugins after core plugins are loaded. */
      let otherFrameworks = BundleFramework.allCases.filter
      {
        !BundleFramework.corePlugins.contains($0)
      }
      var otherPlugPaths = Pixar.PlugRegistry.PlugPathsVector()
      for framework in otherFrameworks
      {
        if let path = framework.resourcePath
        {
          #if DEBUG_PIXAR_BUNDLE
            Msg.logger.log(level: .info, "Adding usd resource -> \(path)")
          #endif /* DEBUG_PIXAR_BUNDLE */
          otherPlugPaths.push_back(std.string(path))
        }
      }
      Pixar.PlugRegistry.GetInstance().RegisterPlugins(otherPlugPaths)

      /* 3. Set up MaterialX standard library search paths.
       *    UsdMtlx and HdMtlx look for MaterialX libraries via the
       *    PXR_MTLX_STDLIB_SEARCH_PATHS environment variable.
       *
       *    IMPORTANT: For shader source code includes (like lib/$fileTransformUv),
       *    MaterialX needs the PARENT of the 'libraries' directory because:
       *    - GenOptions.libraryPrefix prepends "libraries" to include paths
       *    - So the full path becomes: {searchPath}/libraries/stdlib/genglsl/lib/mx_transform_uv.glsl
       *    - If searchPath already points to 'libraries', resolution fails */
      if let materialXBundle = Bundle.materialX,
         let materialXLibraries = materialXBundle.path(forResource: "libraries", ofType: nil)
      {
        // Get the parent directory of 'libraries' for source code resolution
        let materialXRoot = (materialXLibraries as NSString).deletingLastPathComponent

        #if DEBUG_PIXAR_BUNDLE
          Msg.logger.log(level: .info, "Setting MaterialX stdlib path -> \(materialXLibraries)")
          Msg.logger.log(level: .info, "Setting MaterialX root path -> \(materialXRoot)")
        #endif /* DEBUG_PIXAR_BUNDLE */

        // For MTLX nodedefs and library loading - points to 'libraries' directory
        setenv("PXR_MTLX_STDLIB_SEARCH_PATHS", materialXLibraries, 1)

        // For shader source code includes - points to parent of 'libraries'
        // HdMtlx and HdSt use this for resolving shader includes like lib/mx_*.glsl
        setenv("PXR_MTLX_PLUGIN_SEARCH_PATHS", materialXRoot, 1)
      }
    }
  }
}
