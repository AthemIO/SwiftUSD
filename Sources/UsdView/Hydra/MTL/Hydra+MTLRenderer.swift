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
#if canImport(Metal)
  import Metal
  import MetalKit

  public extension Hydra
  {
    class MTLRenderer: NSObject, MTKViewDelegate
    {
      private let device: MTLDevice
      private var hydra: Hydra.RenderEngine?
      private var pipelineState: MTLRenderPipelineState?

      private var inFlightSemaphore = DispatchSemaphore(value: 1)

      // Animation playback state
      private var startTimeCode: Double = 0.0
      private var endTimeCode: Double = 0.0
      private var framesPerSecond: Double = 24.0
      private var currentTimeCode: Double = 0.0
      private var lastFrameTime: CFTimeInterval = 0.0
      private var isAnimating: Bool = false

      convenience init(hydra: Hydra.RenderEngine)
      {
        self.init(device: hydra.hydraDevice)!
        self.hydra = hydra

        // Read animation parameters from the stage
        if hydra.stage.pointee.HasAuthoredTimeCodeRange() {
          startTimeCode = hydra.stage.pointee.GetStartTimeCode()
          endTimeCode = hydra.stage.pointee.GetEndTimeCode()
          framesPerSecond = hydra.stage.pointee.GetFramesPerSecond()
          currentTimeCode = startTimeCode
          isAnimating = (endTimeCode > startTimeCode)
          lastFrameTime = CACurrentMediaTime()
        }
      }

      init?(device: MTLDevice)
      {
        self.device = device

        super.init()

        setupPipeline()
      }

      private func setupPipeline()
      {
        do
        {
          // Try multiple methods to load the Metal library
          let defaultLibrary: MTLLibrary

          // Method 1: Try loading from bundle
          if let bundleLibrary = try? device.makeDefaultLibrary(bundle: .usdview) {
            defaultLibrary = bundleLibrary
          }
          // Method 2: Try loading from main bundle (for bundled apps)
          else if let mainLibrary = try? device.makeDefaultLibrary(bundle: .main) {
            defaultLibrary = mainLibrary
          }
          // Method 3: Try the device's default library (looks in app bundle)
          else if let deviceLibrary = device.makeDefaultLibrary() {
            defaultLibrary = deviceLibrary
          }
          // Method 4: Try loading from explicit path
          else if let bundlePath = Bundle.usdview.resourcePath,
                  let pathLibrary = try? device.makeLibrary(filepath: "\(bundlePath)/default.metallib") {
            defaultLibrary = pathLibrary
          }
          else {
            Msg.logger.error("HYDRA: Failed to load Metal library from any source")
            return
          }

          guard let vertexFunction = defaultLibrary.makeFunction(name: "vtxBlit")
          else { Msg.logger.error("HYDRA: Failed to create vertex function."); return }

          guard let fragmentFunction = defaultLibrary.makeFunction(name: "fragBlitLinear")
          else { Msg.logger.error("HYDRA: Failed to create fragment function."); return }

          // set up the pipeline state descriptor.
          let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
          pipelineStateDescriptor.rasterSampleCount = 1
          pipelineStateDescriptor.vertexFunction = vertexFunction
          pipelineStateDescriptor.fragmentFunction = fragmentFunction
          pipelineStateDescriptor.depthAttachmentPixelFormat = .invalid

          // configure the color attachment for blending.
          if let colorAttachment = pipelineStateDescriptor.colorAttachments[0]
          {
            colorAttachment.pixelFormat = .bgra8Unorm
            colorAttachment.isBlendingEnabled = true
            colorAttachment.rgbBlendOperation = .add
            colorAttachment.alphaBlendOperation = .add
            colorAttachment.sourceRGBBlendFactor = .one
            colorAttachment.sourceAlphaBlendFactor = .one
            colorAttachment.destinationRGBBlendFactor = .oneMinusSourceAlpha
            colorAttachment.destinationAlphaBlendFactor = .zero
          }

          // create the pipeline state object.
          pipelineState = try device.makeRenderPipelineState(descriptor: pipelineStateDescriptor)
        }
        catch
        {
          Msg.logger.error("HYDRA: Failed to create pipeline state: \(error.localizedDescription)")
        }
      }

      public func mtkView(_: MTKView, drawableSizeWillChange _: CGSize)
      {}

      public func draw(in view: MTKView)
      {
        view.drawableSize = CGSize(
          width: (view.frame.size.width > 0) ? view.frame.size.width : 400,
          height: (view.frame.size.height > 0) ? view.frame.size.height : 300
        )

        // Advance animation time if the stage has animation
        if isAnimating {
          let currentTime = CACurrentMediaTime()
          let elapsedSeconds = currentTime - lastFrameTime
          lastFrameTime = currentTime

          // Advance time based on elapsed real time and frames per second
          currentTimeCode += elapsedSeconds * framesPerSecond

          // Loop the animation
          if currentTimeCode > endTimeCode {
            currentTimeCode = startTimeCode + (currentTimeCode - endTimeCode).truncatingRemainder(dividingBy: (endTimeCode - startTimeCode))
          }
        }

        _ = drawFrame(in: view, timeCode: currentTimeCode)
      }

      /// draw the scene, and blit the result to the view.
      @MainActor
      func drawFrame(in view: MTKView, timeCode: Double) -> Bool
      {
        guard let hgi = hydra?.getHgi()
        else { Msg.logger.error("HYDRA: Failed to retrieve hgi."); return false }

        // start the next frame.
        _ = inFlightSemaphore.wait(timeout: .distantFuture)
        defer { inFlightSemaphore.signal() }

        /*
         * ---------------------------------------------------------------- */

        hgi.pointee.StartFrame()

        // draw the scene using hydra, and recast the result to a MTLTexture.
        let viewSize = view.drawableSize
        guard
          let hgiTexture = hydra?.render(at: timeCode, viewSize: viewSize),
          let metalTexture = getMetalTexture(from: hgiTexture),
          let commandBuffer = hgi.pointee.GetPrimaryCommandBuffer()
        else { Msg.logger.error("HYDRA: Failed to draw the scene."); return false }

        // copy the rendered texture to the view.
        blitToView(view, commandBuffer: commandBuffer, texture: metalTexture)

        // tell hydra to commit the command buffer and complete the work.
        hgi.pointee.CommitPrimaryCommandBuffer()

        hgi.pointee.EndFrame()

        /*
         * ---------------------------------------------------------------- */

        return true
      }

      /// copies the texture to the view with a shader.
      @MainActor
      public func blitToView(_ view: MTKView, commandBuffer: MTLCommandBuffer, texture: MTLTexture)
      {
        // Ensure the view has a valid drawable before attempting to get the render pass descriptor
        guard let drawable = view.currentDrawable else {
          // This can happen if the view size is invalid or no drawable is available
          // Just skip this frame - it's normal during initialization or window resizing
          return
        }

        guard let renderPassDescriptor = view.currentRenderPassDescriptor
        else {
          Msg.logger.error("HYDRA: Failed to blit because there is no render pass descriptor for the current view.")
          return
        }

        // create a render command encoder to encode the copy command.
        guard let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)
        else { Msg.logger.error("HYDRA: Failed to create a render command encoder to blit the texture to the view."); return }

        // blit the texture to the view.
        renderEncoder.pushDebugGroup("FinalBlit")
        renderEncoder.setFragmentTexture(texture, index: 0)
        if let pipelineState
        {
          renderEncoder.setRenderPipelineState(pipelineState)
        }
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 3)
        renderEncoder.popDebugGroup()

        // finish encoding the copy command.
        renderEncoder.endEncoding()

        // Present the drawable (we already checked it exists above)
        commandBuffer.present(drawable)
      }

      public func getMetalTexture(from hgiTexture: Pixar.HgiTextureHandle) -> MTLTexture?
      {
        // get the hgi texture handle.
        guard let hgiTex = hgiTexture.Get()
        else { Msg.logger.error("HYDRA: Failed to retrieve the hgi texture."); return nil }

        // get the raw pointer from the hgi handle.
        let rawPtr = UnsafeRawPointer(hgiTex)

        // get the hgi texture from the raw pointer.
        let texPtr: Pixar.HgiMetalTexture = Unmanaged.fromOpaque(rawPtr).takeUnretainedValue()

        // get the metal texture from the hgi texture.
        let metalTexture = texPtr.GetTextureId()

        return metalTexture
      }
    }
  }
#endif // canImport(Metal)
