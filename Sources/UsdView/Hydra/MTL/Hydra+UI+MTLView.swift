/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

#if canImport(SwiftCrossUI)
  import SwiftCrossUI

  #if os(iOS) || os(visionOS)
    import Combine
    import Metal
    import MetalKit
    import UIKitBackend

    public extension Hydra
    {
      struct MTLView: UIViewRepresentable
      {
        public typealias UIViewType = MTKView
        public typealias SetNeedsDisplayTrigger = AnyPublisher<Void, Never>

        public enum DrawingMode
        {
          case timeUpdates(preferredFramesPerSecond: Int)
          case drawNotifications(setNeedsDisplayTrigger: SetNeedsDisplayTrigger?)
        }

        private let hydra: Hydra.RenderEngine!
        private let device: MTLDevice!
        private let renderer: MTLRenderer!

        public init(hydra: Hydra.RenderEngine, renderer: MTLRenderer)
        {
          self.hydra = hydra
          device = hydra.hydraDevice
          self.renderer = renderer
        }

        public func makeCoordinator() -> Coordinator
        {
          let mtkView = MTKView()
          mtkView.isPaused = false
          mtkView.framebufferOnly = true
          mtkView.preferredFramesPerSecond = 60
          // Don't set drawableSize here - let MTKView handle it automatically
          // or wait until the view has a valid frame size
          mtkView.autoResizeDrawable = true

          return Coordinator(mtkView: mtkView)
        }

        public func makeUIView(context: UIViewRepresentableContext<Coordinator>) -> MTKView
        {
          let metalView = context.coordinator.metalView

          metalView.device = device
          metalView.delegate = renderer
          metalView.colorPixelFormat = .bgra8Unorm
          metalView.sampleCount = 1
          metalView.layer.backgroundColor = UIColor.clear.cgColor
          metalView.layer.isOpaque = false

          metalView.becomeFirstResponder()
          return metalView
        }

        public func updateUIView(_ view: MTKView, context _: UIViewRepresentableContext<Coordinator>)
        {
          renderer.draw(in: view)
        }

        public class Coordinator
        {
          private var cancellable: AnyCancellable?
          public var metalView: MTKView

          public init(mtkView: MTKView)
          {
            cancellable = nil
            metalView = mtkView
          }
        }
      }
    }
  #endif // os(iOS)
#endif // canImport(SwiftUI)
