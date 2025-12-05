/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation
import Usd

public protocol StageConvertible
{
  func asPrims() -> [USDPrim]
}

extension USDPrim: StageConvertible
{
  public func asPrims() -> [USDPrim] { [self] }

  public struct Empty: StageConvertible
  {
    public func asPrims() -> [USDPrim] { [] }
  }
}

extension USDStage: StageConvertible
{
  public func asPrims() -> [USDPrim]
  {
    [USDPrim(stage.getPseudoRoot().path.string, type: .group(prims))]
  }
}

/**
 * Here we extend Array to make it
 * conform to our StageConvertible
 * protocol. */
extension [USDPrim]: StageConvertible
{
  public func asPrims() -> [USDPrim] { self }
}

@resultBuilder
public enum StageBuilder
{
  public static func buildBlock(_ prims: StageConvertible...) -> [USDPrim]
  {
    prims.flatMap { $0.asPrims() }
  }

  public static func buildIf(_ path: StageConvertible?) -> StageConvertible
  {
    path ?? []
  }

  public static func buildEither(first: StageConvertible) -> StageConvertible
  {
    first
  }

  public static func buildEither(second: StageConvertible) -> StageConvertible
  {
    second
  }
}
