/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

#if canImport(SwiftCrossUI)
  import SwiftCrossUI

  #if os(macOS)
    import Combine
    import Metal
    import MetalKit
    import AppKitBackend

    public extension Hydra
    {
      /// Interactive MTKView subclass that handles camera manipulation input
      class InteractiveMTKView: MTKView
      {
        weak var cameraController: CameraController?
        // weak var selectionManager: SelectionManager?  // Disabled due to Swift compiler crash

        private var lastMouseLocation: NSPoint?
        private var currentInteractionMode: InteractionMode = .none

        override public var acceptsFirstResponder: Bool { true }

        override public var canBecomeKeyView: Bool { true }

        override public func viewDidMoveToWindow() {
          super.viewDidMoveToWindow()
          // Ensure we become first responder when added to window
          window?.makeFirstResponder(self)
        }

        override public func mouseDown(with event: NSEvent)
        {
          super.mouseDown(with: event)
          lastMouseLocation = event.locationInWindow
          currentInteractionMode = determineInteractionMode(from: event)
        }

        override public func mouseDragged(with event: NSEvent)
        {
          super.mouseDragged(with: event)

          guard let lastLocation = lastMouseLocation else { return }

          let currentLocation = event.locationInWindow
          let delta = CGPoint(
            x: currentLocation.x - lastLocation.x,
            y: currentLocation.y - lastLocation.y
          )
          lastMouseLocation = currentLocation

          // Apply camera manipulation based on interaction mode
          guard let camera = cameraController else { return }

          switch currentInteractionMode
          {
            case .orbit:
              camera.orbit(delta: delta)
            case .pan:
              camera.pan(delta: delta)
            case .zoom:
              camera.zoom(delta: delta.x)
            default:
              break
          }

          // Trigger redraw
          setNeedsDisplay(bounds)
        }

        override public func mouseUp(with event: NSEvent)
        {
          super.mouseUp(with: event)
          lastMouseLocation = nil
          currentInteractionMode = .none
        }

        override public func scrollWheel(with event: NSEvent)
        {
          super.scrollWheel(with: event)

          // Always use scroll for zooming (both trackpad and mouse wheel)
          let scrollDelta = event.scrollingDeltaY
          cameraController?.zoomScroll(delta: scrollDelta)

          // Trigger redraw
          setNeedsDisplay(bounds)
        }

        override public func keyDown(with event: NSEvent)
        {
          super.keyDown(with: event)

          guard let characters = event.charactersIgnoringModifiers?.lowercased()
          else { return }

          switch characters
          {
            case "f":
              // Focus on nearest object
              focusOnNearestObject()
              setNeedsDisplay(bounds)

            case "r":
              // Reset camera
              cameraController?.reset()
              setNeedsDisplay(bounds)

            default:
              break
          }
        }

        private func focusOnNearestObject()
        {
          guard let camera = cameraController else { return }

          // Define known object positions from the scene
          let spherePosition = Pixar.GfVec3d(-1.5, 0.0, 0.0)
          let cubePosition = Pixar.GfVec3d(1.5, 0.0, 0.0)

          let cameraPosition = camera.eye

          // Calculate distances
          let sphereDistance = (spherePosition - cameraPosition).GetLength()
          let cubeDistance = (cubePosition - cameraPosition).GetLength()

          // Focus on the nearest object
          if sphereDistance < cubeDistance {
            camera.focus(on: spherePosition, distance: 5.0)
          } else {
            camera.focus(on: cubePosition, distance: 5.0)
          }
        }

        private func determineInteractionMode(from event: NSEvent) -> InteractionMode
        {
          let modifiers = event.modifierFlags

          // Left Mouse (no modifiers) = Orbit
          if event.type == .leftMouseDown && !modifiers.contains(.shift) && !modifiers.contains(.option)
          {
            return .orbit
          }

          // Shift + Left Mouse = Pan
          if modifiers.contains(.shift), event.type == .leftMouseDown
          {
            return .pan
          }

          // Alt (Option) + Left Mouse = Zoom (alternative to scroll wheel)
          if modifiers.contains(.option), event.type == .leftMouseDown
          {
            return .zoom
          }

          // Right Mouse = Pan (alternative)
          if event.type == .rightMouseDown
          {
            return .pan
          }

          return .none
        }
      }

      struct MTLView: NSViewRepresentable
      {
        public typealias NSViewType = InteractiveMTKView
        public typealias SetNeedsDisplayTrigger = AnyPublisher<Void, Never>

        public enum DrawingMode
        {
          case timeUpdates(preferredFramesPerSecond: Int)
          case drawNotifications(setNeedsDisplayTrigger: SetNeedsDisplayTrigger?)
        }

        private let hydra: Hydra.RenderEngine!
        private let device: MTLDevice!
        private let renderer: MTLRenderer!
        private let cameraController: CameraController?
        // private let selectionManager: SelectionManager?  // Disabled due to Swift compiler crash

        public init(
          hydra: Hydra.RenderEngine,
          renderer: MTLRenderer,
          cameraController: CameraController? = nil
          // selectionManager: SelectionManager? = nil  // Disabled due to Swift compiler crash
        )
        {
          self.hydra = hydra
          device = hydra.hydraDevice
          self.renderer = renderer
          self.cameraController = cameraController
          // self.selectionManager = selectionManager
        }

        public func makeCoordinator() -> Coordinator
        {
          let mtkView = InteractiveMTKView()
          mtkView.isPaused = false
          mtkView.framebufferOnly = true
          if let mode = CGDisplayCopyDisplayMode(CGMainDisplayID())
          {
            mtkView.preferredFramesPerSecond = Int(mode.refreshRate)
          }
          else
          {
            mtkView.preferredFramesPerSecond = 60
          }
          // Don't set drawableSize here - let MTKView handle it automatically
          // or wait until the view has a valid frame size
          mtkView.autoResizeDrawable = true

          // Set camera controller reference
          mtkView.cameraController = cameraController
          // mtkView.selectionManager = selectionManager  // Disabled due to Swift compiler crash

          return Coordinator(mtkView: mtkView)
        }

        public func makeNSView(context: NSViewRepresentableContext<Coordinator>) -> InteractiveMTKView
        {
          let metalView = context.coordinator.metalView

          metalView.device = device
          metalView.delegate = renderer
          metalView.colorPixelFormat = .bgra8Unorm
          metalView.sampleCount = 1
          metalView.layer?.backgroundColor = NSColor.clear.cgColor
          metalView.layer?.isOpaque = false

          // First responder will be set in viewDidMoveToWindow
          return metalView
        }

        public func updateNSView(_ view: InteractiveMTKView, context _: NSViewRepresentableContext<Coordinator>)
        {
          // Update camera controller reference
          view.cameraController = cameraController
          // view.selectionManager = selectionManager  // Disabled due to Swift compiler crash

          renderer.draw(in: view)
        }

        public class Coordinator
        {
          private var cancellable: AnyCancellable?
          public var metalView: InteractiveMTKView

          public init(mtkView: InteractiveMTKView)
          {
            cancellable = nil
            metalView = mtkView
          }
        }
      }
    }
  #endif // os(macOS)
#endif // canImport(SwiftUI)
