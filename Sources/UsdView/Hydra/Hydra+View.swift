/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD
import SwiftCrossUI

#if canImport(Metal)
  import Metal
  import MetalKit
#endif // canImport(Metal)

public extension Hydra
{
  #if os(macOS) || os(iOS) || os(visionOS)
    typealias Viewport = Hydra.MTLView
  #elseif os(tvOS) || os(watchOS)
    typealias Viewport = UIViewRepresentable
  #else // os(Linux) || os(Windows)
    struct Viewport: View
    {
      public var body: some View
      {
        Text("Hello World.")
      }
    }
  #endif // os(macOS) || os(iOS) || os(visionOS)
}

public extension Hydra.Viewport
{
  init(engine: Hydra.RenderEngine)
  {
    #if canImport(Metal)
      let renderer = Hydra.MTLRenderer(hydra: engine)
      self.init(hydra: engine, renderer: renderer, cameraController: nil)
    #endif // canImport(Metal)
  }

  init(
    engine: Hydra.RenderEngine,
    cameraController: CameraController? = nil
    // selectionManager: SelectionManager?  // Disabled due to Swift compiler crash
  )
  {
    #if canImport(Metal)
      let renderer = Hydra.MTLRenderer(hydra: engine)
      self.init(hydra: engine, renderer: renderer, cameraController: cameraController)
    #endif // canImport(Metal)
  }
}
