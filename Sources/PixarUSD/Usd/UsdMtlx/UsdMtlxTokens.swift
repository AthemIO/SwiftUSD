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

@preconcurrency import UsdMtlx

public extension UsdMtlx
{
  /**
   * Private struct to hold the static
   * data for the UsdMtlx library. */
  private struct StaticData: @unchecked Sendable
  {
    static let shared = StaticData()
    private init()
    {}

    let tokens = Pixar.UsdMtlxTokensType()
  }
}

public extension UsdMtlx
{
  /**
   * # UsdMtlx.Tokens
   *
   * ## Overview
   *
   * Public, client facing api to access
   * the static UsdMtlx tokens. */
  enum Tokens: CaseIterable
  {
    /// "config:mtlx:version" - MaterialX library version attribute
    case configMtlxVersion
    /// "out" - Default output name for MaterialX nodes
    case defaultOutputName
    /// "MaterialXConfigAPI" - Schema identifier for MaterialXConfigAPI
    case materialXConfigAPI

    public func getToken() -> Tf.Token
    {
      switch self
      {
        case .configMtlxVersion: StaticData.shared.tokens.configMtlxVersion
        case .defaultOutputName: StaticData.shared.tokens.DefaultOutputName
        case .materialXConfigAPI: StaticData.shared.tokens.MaterialXConfigAPI
      }
    }
  }
}
