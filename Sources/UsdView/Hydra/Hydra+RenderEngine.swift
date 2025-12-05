/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import PixarUSD

#if canImport(Metal)
  import Metal
  import MetalKit
#else // !canImport(Metal)
  import HgiGL
#endif // canImport(Metal)

public enum Hydra
{
  public class RenderEngine
  {
    public var stage: UsdStageRefPtr

    #if canImport(Metal)
      private let hgi: Pixar.HgiMetalPtr
    #else // !canImport(Metal)
      private let hgi: Pixar.HgiGLPtr
    #endif // canImport(Metal)

    private let engine: UsdImagingGL.EngineSharedPtr

    /// External camera controller (set after initialization)
    public weak var cameraController: CameraController?

    private var worldCenter: Pixar.GfVec3d = .init(0.0, 0.0, 0.0)
    private var worldSize: Double = 1.0

    private var material = Pixar.GlfSimpleMaterial()
    private var sceneAmbient = Pixar.GfVec4f(0.01, 0.01, 0.01, 1.0)

    public required init(stage: UsdStageRefPtr)
    {
      self.stage = stage

      #if canImport(Metal)
        hgi = HgiMetal.createHgi()
        let driver = HdDriver(name: Hgi.Tokens.renderDriver.token, driver: hgi.value)
      #else // !canImport(Metal)
        hgi = HgiGL.createHgi()
        let driver = HdDriver(name: Hgi.Tokens.renderDriver.token, driver: hgi.value)
      #endif // canImport(Metal)

      engine = UsdImagingGL.Engine.createEngine(
        rootPath: stage.getPseudoRoot().getPath(),
        excludedPaths: Sdf.PathVector(),
        invisedPaths: Sdf.PathVector(),
        sceneDelegateId: Sdf.Path.absoluteRootPath(),
        driver: driver
      )

      engine.setEnablePresentation(false)
      engine.setRendererAov(.color)

      setupMaterial()
    }

    /// Set the camera controller
    public func setCameraController(_ controller: CameraController)
    {
      cameraController = controller
    }

    public func render(at timeCode: Double, viewSize: CGSize) -> Pixar.HgiTextureHandle
    {
      // draws the scene using hydra.
      guard let camera = cameraController
      else
      {
        Msg.logger.log(level: .error, "RenderEngine: No camera controller set")
        return Pixar.HgiTextureHandle()
      }

      // Get view matrix from camera (eye/at/up approach)
      let viewMatrix = camera.getViewMatrix()

      // Compute projection matrix
      let fov = 60.0 // Default FOV
      let nearPlane = 1.0
      let farPlane = 100_000.0
      var frustum = Pixar.GfFrustum()
      let aspectRatio = Double(viewSize.width) / Double(viewSize.height)
      frustum.SetPerspective(fov, true, aspectRatio, nearPlane, farPlane)
      let projMatrix = frustum.ComputeProjectionMatrix()

      engine.setCameraState(modelViewMatrix: viewMatrix, projectionMatrix: projMatrix)

      // viewport setup.
      let viewport = Gf.Vec4d(0, 0, viewSize.width, viewSize.height)
      engine.setRenderViewport(viewport)
      engine.setWindowPolicy(.matchHorizontally)

      // light and material setup.
      // var lights = computeLights(cameraTransform: cameraTransform)
      // engine.setLightingState(lights: .init(), material: material, sceneAmbient: sceneAmbient)

      var params = UsdImagingGL.RenderParams()
      params.frame = Usd.TimeCode(timeCode)
      params.clearColor = .init(0.0, 0.0, 0.0, 0.0)
      params.colorCorrectionMode = .sRGB
      // Enable OpenSubdiv refinement: complexity 1.0 = refineLevel 0, 2.0 = refineLevel 8
      params.complexity = 1.2 // Lower subdivision for spheres
      // TODO: enableIdRender was removed in OpenUSD 25.11
      // params.enableIdRender = false
      params.showGuides = true
      params.showRender = true
      params.showProxy = true

      // render the frame.
      engine.render(rootPrim: stage.getPseudoRoot(), params: params)

      // return the color output.
      return engine.getAovTexture(.color)
    }

    // creates a light source located at the camera position.
    // func computeCameraLight(cameraTransform: Gf.Matrix4d) -> Pixar.GlfSimpleLight
    // {
    //   let cameraPosition = Pixar.GfVec3f(cameraTransform.ExtractTranslation())

    //   let light = Pixar.GlfSimpleLightCollector.createLight(Pixar.GfVec4f(cameraPosition[0], cameraPosition[1], cameraPosition[2], 1))

    //   return light
    // }

    // func computeLights(cameraTransform: Gf.Matrix4d) -> Pixar.GlfSimpleLightCollector
    // {
    //   var lights = Pixar.GlfSimpleLightCollector()
    //   lights.addLight(computeCameraLight(cameraTransform: cameraTransform))

    //   return lights
    // }

    func setupMaterial()
    {
      let kA = Float(0.2)
      let kS = Float(0.1)

      material.SetAmbient(Pixar.GfVec4f(kA, kA, kA, 1.0))
      material.SetSpecular(Pixar.GfVec4f(kS, kS, kS, 1.0))
      material.SetShininess(Double(32.0))

      sceneAmbient = Pixar.GfVec4f(Float(0.01), Float(0.01), Float(0.01), Float(1.0))
    }

    public func calculateOriginAndSize()
    {
      var bboxCache = computeBBoxCache()

      var bbox = bboxCache.ComputeWorldBound(stage.getPseudoRoot())

      if bbox.GetRange().pointee.IsEmpty() || isInfiniteBBox(bbox)
      {
        bbox = Pixar.GfBBox3d(.init(.init(-10, -10, -10), .init(10, 10, 10)))
      }

      let world = bbox.ComputeAlignedRange()

      worldCenter = (world.GetMin().pointee + world.GetMax().pointee) / 2.0
      worldSize = world.GetSize().GetLength()
    }

    func isInfiniteBBox(_ bbox: Pixar.GfBBox3d) -> Bool
    {
      Double(bbox.GetRange().pointee.GetMin().pointee.GetLength()).isInfinite ||
        Double(bbox.GetRange().pointee.GetMax().pointee.GetLength()).isInfinite
    }

    func computeBBoxCache() -> Pixar.UsdGeomBBoxCache
    {
      var purposes = Pixar.TfTokenVector()
      purposes.push_back(UsdGeom.Tokens.default_.token)
      purposes.push_back(UsdGeom.Tokens.proxy.token)

      let useExtentHints = true
      var timeCode = UsdTimeCode.Default()
      if stage.pointee.HasAuthoredTimeCodeRange()
      {
        timeCode = UsdTimeCode(stage.pointee.GetStartTimeCode())
      }

      let bboxCache = Pixar.UsdGeomBBoxCache(timeCode, purposes, useExtentHints, false)
      return bboxCache
    }

    #if canImport(Metal)
      public var hydraDevice: MTLDevice
      {
        hgi.device
      }

      public func getHgi() -> Pixar.HgiMetalPtr
      {
        hgi
      }
    #else // !canImport(Metal)
      public func getHgi() -> Pixar.HgiGLPtr
      {
        hgi
      }
    #endif // canImport(Metal)

    public func getEngine() -> UsdImagingGL.EngineSharedPtr
    {
      engine
    }

    static func isZUp(for stage: UsdStageRefPtr) -> Bool
    {
      Pixar.UsdGeomGetStageUpAxis(stage.pointee.getPtr()) == .z
    }
  }
}
