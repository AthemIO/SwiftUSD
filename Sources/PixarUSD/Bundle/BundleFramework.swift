/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

public enum BundleFramework: CaseIterable, Sendable
{
  case ar
  case sdf
  case usd
  case ndr
  case usdGeom
  case usdShade
  case usdShaders
  case usdLux
  case usdHydra
  case sdrOsl
  case sdrGlslfx
  case usdAbc
  case usdDraco
  case usdMedia
  case usdMtlx
  case usdPhysics
  case usdProc
  case usdRender
  case usdRi
  case usdSkel
  case usdUI
  case usdVol
  case hd
  case hgiMetal
  case hgiVulkan
  case hgiGL
  case hdSi
  case hdSt
  case hdStorm
  case hdx
  case hio
  case glf
  case usdImaging
  case usdImagingGL

  /// Core plugins that define SdfMetadata and must be loaded first.
  /// These plugins must be registered before others to ensure metadata
  /// like `apiSchemas` is available when other plugins are loaded.
  public static let corePlugins: [BundleFramework] = [.sdf, .ar, .usd]

  public var resourcePath: String?
  {
    let bundle: Bundle? = switch self
    {
      case .ar: Bundle.ar
      case .sdf: Bundle.sdf
      case .usd: Bundle.usd
      case .ndr: Bundle.ndr
      case .usdGeom: Bundle.usdGeom
      case .usdShade: Bundle.usdShade
      case .usdShaders: Bundle.usdShaders
      case .usdLux: Bundle.usdLux
      case .usdHydra: Bundle.usdHydra
      case .sdrOsl: Bundle.sdrOsl
      case .sdrGlslfx: Bundle.sdrGlslfx
      case .usdAbc: Bundle.usdAbc
      case .usdDraco: Bundle.usdDraco
      case .usdMedia: Bundle.usdMedia
      case .usdMtlx: Bundle.usdMtlx
      case .usdPhysics: Bundle.usdPhysics
      case .usdProc: Bundle.usdProc
      case .usdRender: Bundle.usdRender
      case .usdRi: Bundle.usdRi
      case .usdSkel: Bundle.usdSkel
      case .usdUI: Bundle.usdUI
      case .usdVol: Bundle.usdVol
      case .hd: Bundle.hd
      case .hgiMetal: Bundle.hgiMetal
      case .hgiVulkan: Bundle.hgiVulkan
      case .hgiGL: Bundle.hgiGL
      case .hdSi: Bundle.hdSi
      case .hdSt: Bundle.hdSt
      case .hdStorm: Bundle.hdStorm
      case .hdx: Bundle.hdx
      case .hio: Bundle.hio
      case .glf: Bundle.glf
      case .usdImaging: Bundle.usdImaging
      case .usdImagingGL: Bundle.usdImagingGL
    }

    guard let basePath = bundle?.resourcePath else { return nil }

    // When using .copy("Resources") in Package.swift, files are nested:
    // Bundle.resourcePath returns .../Contents/Resources
    // But plugInfo.json is at .../Contents/Resources/Resources/plugInfo.json
    // Check for this nested structure and return the correct path.
    let nestedPath = "\(basePath)/Resources"
    if FileManager.default.fileExists(atPath: "\(nestedPath)/plugInfo.json")
    {
      return nestedPath
    }

    // Standard case: plugInfo.json at basePath (when using .process())
    return basePath
  }
}
