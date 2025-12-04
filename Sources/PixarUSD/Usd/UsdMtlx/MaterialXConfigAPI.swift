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

import CxxStdlib
import UsdMtlx

// MARK: - Typealias

public typealias UsdMtlxMaterialXConfigAPI = Pixar.UsdMtlxMaterialXConfigAPI

public extension UsdMtlx
{
  typealias MaterialXConfigAPI = UsdMtlxMaterialXConfigAPI
}

// MARK: - MaterialXConfigAPI Extensions

public extension UsdMtlx.MaterialXConfigAPI
{
  /// Return a UsdMtlxMaterialXConfigAPI holding the prim adhering to this
  /// schema at `path` on `stage`.
  ///
  /// If no prim exists at `path` on `stage`, or if the prim at that path
  /// does not adhere to this schema, return an invalid schema object.
  ///
  /// - Parameters:
  ///   - stage: The USD stage
  ///   - path: The prim path
  /// - Returns: A MaterialXConfigAPI schema object
  static func get(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdMtlx.MaterialXConfigAPI
  {
    Pixar.UsdMtlxMaterialXConfigAPI.Get(stage.pointee.getPtr(), path)
  }

  /// Return a UsdMtlxMaterialXConfigAPI holding the prim adhering to this
  /// schema at `path` on `stage`.
  ///
  /// Convenience overload accepting a String path.
  ///
  /// - Parameters:
  ///   - stage: The USD stage
  ///   - path: The prim path as a string
  /// - Returns: A MaterialXConfigAPI schema object
  static func get(_ stage: Usd.StageRefPtr, path: String) -> UsdMtlx.MaterialXConfigAPI
  {
    get(stage, path: Sdf.Path(path))
  }

  /// Returns true if this single-apply API schema can be applied to
  /// the given prim.
  ///
  /// - Parameter prim: The prim to check
  /// - Returns: true if the schema can be applied
  static func canApply(_ prim: Usd.Prim) -> Bool
  {
    Pixar.UsdMtlxMaterialXConfigAPI.CanApply(prim, nil)
  }

  /// Applies this single-apply API schema to the given prim.
  ///
  /// This information is stored by adding "MaterialXConfigAPI" to the
  /// token-valued, listOp metadata `apiSchemas` on the prim.
  ///
  /// - Parameter prim: The prim to apply the schema to
  /// - Returns: A valid MaterialXConfigAPI object upon success,
  ///   an invalid object upon failure
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdMtlx.MaterialXConfigAPI
  {
    Pixar.UsdMtlxMaterialXConfigAPI.Apply(prim)
  }

  /// Get the `config:mtlx:version` attribute.
  ///
  /// MaterialX library version that the data has been authored against.
  /// Defaults to "1.38" to allow correct versioning of old files.
  ///
  /// - Returns: The version attribute
  func getConfigMtlxVersionAttr() -> Usd.Attribute
  {
    GetConfigMtlxVersionAttr()
  }

  /// Create the `config:mtlx:version` attribute.
  ///
  /// - Parameters:
  ///   - defaultValue: The default value to author
  ///   - writeSparsely: If true, write sparsely when it makes sense
  /// - Returns: The created attribute
  @discardableResult
  func createConfigMtlxVersionAttr(
    defaultValue: Vt.Value = Vt.Value(),
    writeSparsely: Bool = false
  ) -> Usd.Attribute
  {
    CreateConfigMtlxVersionAttr(defaultValue, writeSparsely)
  }
}
