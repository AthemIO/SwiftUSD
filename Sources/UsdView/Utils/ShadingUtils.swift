/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
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
 * Create a production-ready material with MaterialX primary and UsdPreviewSurface fallback.
 *
 * This creates a material with dual outputs:
 * - `outputs:mtlx:surface` - MaterialX standard_surface (preferred by Storm)
 * - `outputs:surface` - UsdPreviewSurface (fallback for renderers without MaterialX)
 *
 * Storm will use MaterialX when available and fall back to UsdPreviewSurface
 * if MaterialX support is disabled.
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The base color to set on both shaders.
 * - Parameter metallic: Metalness value (0.0 = dielectric, 1.0 = metal).
 * - Parameter roughness: Surface roughness (0.0 = mirror, 1.0 = rough).
 * - Returns: The newly created dual-output material.
 */
public func matDefWithFallback(
  _ stage: UsdStageRefPtr,
  color: ShadeColor = ShadeColor.white,
  metallic: Float = 0.0,
  roughness: Float = 0.3
) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)Material"
  let matPath = "/Materials/\(matName)"

  let material = UsdShade.Material.define(stage, path: matPath)

  // ============================================================================
  // PRIMARY: MaterialX standard_surface shader (preferred by Storm)
  // ============================================================================
  var mtlxShader = UsdShade.Shader.define(stage, path: "\(matPath)/MtlxStandardSurface")
  mtlxShader.createIdAttr(Tf.Token("ND_standard_surface_surfaceshader"))

  // MaterialX inputs
  mtlxShader.createInput(for: "base_color", type: .color3f).set(color.vec3f)
  mtlxShader.createInput(for: "base", type: .float).set(Float(1.0))
  mtlxShader.createInput(for: "metalness", type: .float).set(metallic)
  mtlxShader.createInput(for: "specular_roughness", type: .float).set(roughness)
  mtlxShader.createInput(for: "specular", type: .float).set(Float(1.0))

  // Connect MaterialX shader to mtlx render context output
  material.createSurfaceOutput(renderContext: Tf.Token("mtlx"))
    .connectTo(source: mtlxShader.connectableAPI(), at: Tf.Token("out"))

  // ============================================================================
  // FALLBACK: UsdPreviewSurface shader (for renderers without MaterialX)
  // ============================================================================
  var pbrShader = UsdShade.Shader.define(stage, path: "\(matPath)/PBRShader")
  pbrShader.createIdAttr(.usdPreviewSurface)

  // UsdPreviewSurface inputs (matching the MaterialX parameters)
  pbrShader.createInput(for: .diffuseColor, type: .color3f).set(color.vec3f)
  pbrShader.createInput(for: .roughness, type: .float).set(roughness)
  pbrShader.createInput(for: .metallic, type: .float).set(metallic)

  // Connect UsdPreviewSurface to universal/default output (fallback)
  material.createSurfaceOutput().connectTo(source: pbrShader.connectableAPI(), at: .surface)

  return material
}

/**
 * Create a production-ready metallic material with MaterialX primary and UsdPreviewSurface fallback.
 *
 * This creates a metallic material with dual outputs:
 * - `outputs:mtlx:surface` - MaterialX standard_surface with metalness=1.0
 * - `outputs:surface` - UsdPreviewSurface metallic (fallback)
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The base color (reflectivity color for metals).
 * - Parameter roughness: Surface roughness (0.0 = mirror, 1.0 = rough).
 * - Returns: The newly created dual-output metallic material.
 */
public func matDefMetallicWithFallback(
  _ stage: UsdStageRefPtr,
  color: ShadeColor = ShadeColor.orange,
  roughness: Float = 0.2
) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)MetallicMaterial"
  let matPath = "/Materials/\(matName)"

  let material = UsdShade.Material.define(stage, path: matPath)

  // ============================================================================
  // PRIMARY: MaterialX standard_surface metallic shader
  // ============================================================================
  var mtlxShader = UsdShade.Shader.define(stage, path: "\(matPath)/MtlxStandardSurface")
  mtlxShader.createIdAttr(Tf.Token("ND_standard_surface_surfaceshader"))

  // MaterialX metallic inputs
  mtlxShader.createInput(for: "base_color", type: .color3f).set(color.vec3f)
  mtlxShader.createInput(for: "base", type: .float).set(Float(1.0))
  mtlxShader.createInput(for: "metalness", type: .float).set(Float(1.0))
  mtlxShader.createInput(for: "specular_roughness", type: .float).set(roughness)
  mtlxShader.createInput(for: "specular", type: .float).set(Float(1.0))

  // Connect MaterialX shader to mtlx render context output
  material.createSurfaceOutput(renderContext: Tf.Token("mtlx"))
    .connectTo(source: mtlxShader.connectableAPI(), at: Tf.Token("out"))

  // ============================================================================
  // FALLBACK: UsdPreviewSurface metallic shader
  // ============================================================================
  var pbrShader = UsdShade.Shader.define(stage, path: "\(matPath)/PBRShader")
  pbrShader.createIdAttr(.usdPreviewSurface)

  // UsdPreviewSurface metallic inputs
  pbrShader.createInput(for: .diffuseColor, type: .color3f).set(color.vec3f)
  pbrShader.createInput(for: .roughness, type: .float).set(roughness)
  pbrShader.createInput(for: .metallic, type: .float).set(Float(1.0))

  // Connect UsdPreviewSurface to universal/default output (fallback)
  material.createSurfaceOutput().connectTo(source: pbrShader.connectableAPI(), at: .surface)

  return material
}

/**
 * Create a production-ready subsurface scattering material with MaterialX primary and UsdPreviewSurface fallback.
 *
 * This creates a subsurface material with dual outputs:
 * - `outputs:mtlx:surface` - MaterialX standard_surface with subsurface scattering
 * - `outputs:surface` - UsdPreviewSurface approximation (diffuse only, SSS not supported)
 *
 * Note: UsdPreviewSurface does not support subsurface scattering, so the fallback
 * uses a diffuse material that approximates the overall color appearance.
 *
 * - Parameter stage: The stage to create the material on.
 * - Parameter color: The base color.
 * - Parameter subsurfaceScale: The subsurface scattering scale/radius.
 * - Returns: The newly created dual-output subsurface material.
 */
public func matDefSubsurfaceWithFallback(
  _ stage: UsdStageRefPtr,
  color: ShadeColor = ShadeColor.blue,
  subsurfaceScale: Float = 0.5
) -> UsdShade.Material
{
  let matName = "\(color.rawValue.capitalized)SubsurfaceMaterial"
  let matPath = "/Materials/\(matName)"

  let material = UsdShade.Material.define(stage, path: matPath)

  // ============================================================================
  // PRIMARY: MaterialX standard_surface with subsurface scattering
  // ============================================================================
  var mtlxShader = UsdShade.Shader.define(stage, path: "\(matPath)/MtlxStandardSurface")
  mtlxShader.createIdAttr(Tf.Token("ND_standard_surface_surfaceshader"))

  // Base color
  mtlxShader.createInput(for: "base_color", type: .color3f).set(color.vec3f)
  mtlxShader.createInput(for: "base", type: .float).set(Float(1.0))

  // Subsurface scattering settings
  mtlxShader.createInput(for: "subsurface", type: .float).set(Float(0.8))
  let sssColor = GfVec3f(color.vec3f[0] * 0.8, color.vec3f[1] * 0.9, color.vec3f[2] * 1.0)
  mtlxShader.createInput(for: "subsurface_color", type: .color3f).set(sssColor)
  let sssRadius = GfVec3f(subsurfaceScale, subsurfaceScale * 0.8, subsurfaceScale * 0.6)
  mtlxShader.createInput(for: "subsurface_radius", type: .color3f).set(sssRadius)
  mtlxShader.createInput(for: "subsurface_scale", type: .float).set(Float(1.0))

  // Specular settings for waxy appearance
  mtlxShader.createInput(for: "specular", type: .float).set(Float(0.5))
  mtlxShader.createInput(for: "specular_roughness", type: .float).set(Float(0.4))

  // Connect MaterialX shader to mtlx render context output
  material.createSurfaceOutput(renderContext: Tf.Token("mtlx"))
    .connectTo(source: mtlxShader.connectableAPI(), at: Tf.Token("out"))

  // ============================================================================
  // FALLBACK: UsdPreviewSurface (approximation - no SSS support)
  // ============================================================================
  var pbrShader = UsdShade.Shader.define(stage, path: "\(matPath)/PBRShader")
  pbrShader.createIdAttr(.usdPreviewSurface)

  // Approximate SSS with slightly brighter diffuse and low metallic
  // Blend base color with SSS color for a softer appearance
  let fallbackColor = GfVec3f(
    (color.vec3f[0] + sssColor[0]) * 0.5,
    (color.vec3f[1] + sssColor[1]) * 0.5,
    (color.vec3f[2] + sssColor[2]) * 0.5
  )
  pbrShader.createInput(for: .diffuseColor, type: .color3f).set(fallbackColor)
  pbrShader.createInput(for: .roughness, type: .float).set(Float(0.5))
  pbrShader.createInput(for: .metallic, type: .float).set(Float(0.0))

  // Connect UsdPreviewSurface to universal/default output (fallback)
  material.createSurfaceOutput().connectTo(source: pbrShader.connectableAPI(), at: .surface)

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
