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

public enum ShadeColor: String, CaseIterable
{
  case red
  case orange
  case yellow
  case green
  case blue
  case purple
  case white
  case black

  public var vec3f: GfVec3f
  {
    switch self
    {
      case .red: GfVec3f(0.992, 0.207, 0.061)
      case .orange: GfVec3f(0.922, 0.501, 0.0)
      case .yellow: GfVec3f(0.950, 0.800, 0.0)
      case .green: GfVec3f(0.0, 0.766, 0.014)
      case .blue: GfVec3f(0.132, 0.218, 0.932)
      case .purple: GfVec3f(0.531, 0.122, 0.922)
      case .white: GfVec3f(0.8, 0.8, 0.8)
      case .black: GfVec3f(0.2, 0.2, 0.2)
    }
  }
}

/**
 * Create a material with a surface shader.
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The diffuse color to set on the shader.
 * - Returns: The newly created material.
 */
public func matDef(_ stage: UsdStageRefPtr, color: ShadeColor = ShadeColor.white) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)Material"

  let material = UsdShade.Material.define(stage, path: "/Materials/\(matName)")

  var pbrShader = UsdShade.Shader.define(stage, path: "/Materials/\(matName)/PBRShader")
  pbrShader.createIdAttr(.usdPreviewSurface)
  pbrShader.createInput(for: .diffuseColor, type: .color3f).set(color.vec3f)
  pbrShader.createInput(for: .roughness, type: .float).set(Float(0.4))
  pbrShader.createInput(for: .metallic, type: .float).set(Float(0.0))
  material.createSurfaceOutput().connectTo(source: pbrShader.connectableAPI(), at: .surface)

  return material
}

/**
 * Create a MaterialX standard surface material.
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The base color to set on the shader.
 * - Returns: The newly created MaterialX material.
 */
public func matDefMtlx(_ stage: UsdStageRefPtr, color: ShadeColor = ShadeColor.white) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)MtlxMaterial"
  let matPath = "/Materials/\(matName)"

  let material = UsdShade.Material.define(stage, path: matPath)

  // Create MaterialX standard surface shader
  var mtlxShader = UsdShade.Shader.define(stage, path: "\(matPath)/MtlxStandardSurface")

  // Set shader ID to MaterialX standard_surface node definition
  mtlxShader.createIdAttr(Tf.Token("ND_standard_surface_surfaceshader"))

  // Set base color input
  mtlxShader.createInput(for: "base_color", type: .color3f).set(color.vec3f)

  // Set base weight (default 1.0 for full color)
  mtlxShader.createInput(for: "base", type: .float).set(Float(1.0))

  // Set specular roughness
  mtlxShader.createInput(for: "specular_roughness", type: .float).set(Float(0.3))

  // Set specular weight
  mtlxShader.createInput(for: "specular", type: .float).set(Float(1.0))

  // Connect shader to material's surface output (MaterialX uses mtlx render context)
  material.createSurfaceOutput(renderContext: Tf.Token("mtlx"))
    .connectTo(source: mtlxShader.connectableAPI(), at: Tf.Token("out"))

  return material
}

/**
 * Create a MaterialX metallic standard surface material.
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The base color to set on the shader.
 * - Parameter roughness: The specular roughness (0.0 = mirror, 1.0 = rough).
 * - Returns: The newly created MaterialX metallic material.
 */
public func matDefMtlxMetallic(_ stage: UsdStageRefPtr, color: ShadeColor = ShadeColor.orange, roughness: Float = 0.2) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)MetallicMtlxMaterial"
  let matPath = "/Materials/\(matName)"

  let material = UsdShade.Material.define(stage, path: matPath)

  // Create MaterialX standard surface shader
  var mtlxShader = UsdShade.Shader.define(stage, path: "\(matPath)/MtlxStandardSurface")

  // Set shader ID to MaterialX standard_surface node definition
  mtlxShader.createIdAttr(Tf.Token("ND_standard_surface_surfaceshader"))

  // Set base color input (for metals, this is the reflectivity color)
  mtlxShader.createInput(for: "base_color", type: .color3f).set(color.vec3f)

  // Set base weight (default 1.0 for full color)
  mtlxShader.createInput(for: "base", type: .float).set(Float(1.0))

  // Set metalness to 1.0 for full metallic look
  mtlxShader.createInput(for: "metalness", type: .float).set(Float(1.0))

  // Set specular roughness (lower = more reflective)
  mtlxShader.createInput(for: "specular_roughness", type: .float).set(roughness)

  // Set specular weight
  mtlxShader.createInput(for: "specular", type: .float).set(Float(1.0))

  // Connect shader to material's surface output (MaterialX uses mtlx render context)
  material.createSurfaceOutput(renderContext: Tf.Token("mtlx"))
    .connectTo(source: mtlxShader.connectableAPI(), at: Tf.Token("out"))

  return material
}

/**
 * Create a MaterialX subsurface scattering standard surface material.
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The base color to set on the shader.
 * - Parameter subsurfaceColor: The subsurface scattering color.
 * - Parameter subsurfaceScale: The subsurface scattering scale/radius.
 * - Returns: The newly created MaterialX subsurface material.
 */
public func matDefMtlxSubsurface(_ stage: UsdStageRefPtr, color: ShadeColor = ShadeColor.blue, subsurfaceScale: Float = 0.5) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)SubsurfaceMtlxMaterial"
  let matPath = "/Materials/\(matName)"

  let material = UsdShade.Material.define(stage, path: matPath)

  // Create MaterialX standard surface shader
  var mtlxShader = UsdShade.Shader.define(stage, path: "\(matPath)/MtlxStandardSurface")

  // Set shader ID to MaterialX standard_surface node definition
  mtlxShader.createIdAttr(Tf.Token("ND_standard_surface_surfaceshader"))

  // Set base color input
  mtlxShader.createInput(for: "base_color", type: .color3f).set(color.vec3f)

  // Set base weight
  mtlxShader.createInput(for: "base", type: .float).set(Float(1.0))

  // Enable subsurface scattering (weight 0-1)
  mtlxShader.createInput(for: "subsurface", type: .float).set(Float(0.8))

  // Set subsurface color (light scatters through material with this color)
  let sssColor = GfVec3f(color.vec3f[0] * 0.8, color.vec3f[1] * 0.9, color.vec3f[2] * 1.0)
  mtlxShader.createInput(for: "subsurface_color", type: .color3f).set(sssColor)

  // Set subsurface radius (controls how far light scatters)
  let sssRadius = GfVec3f(subsurfaceScale, subsurfaceScale * 0.8, subsurfaceScale * 0.6)
  mtlxShader.createInput(for: "subsurface_radius", type: .color3f).set(sssRadius)

  // Set subsurface scale multiplier
  mtlxShader.createInput(for: "subsurface_scale", type: .float).set(Float(1.0))

  // Lower specular for more diffuse/waxy look
  mtlxShader.createInput(for: "specular", type: .float).set(Float(0.5))

  // Set specular roughness (slightly rough for waxy appearance)
  mtlxShader.createInput(for: "specular_roughness", type: .float).set(Float(0.4))

  // Connect shader to material's surface output (MaterialX uses mtlx render context)
  material.createSurfaceOutput(renderContext: Tf.Token("mtlx"))
    .connectTo(source: mtlxShader.connectableAPI(), at: Tf.Token("out"))

  return material
}
