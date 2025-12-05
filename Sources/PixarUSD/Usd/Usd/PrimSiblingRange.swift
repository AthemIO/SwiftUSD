/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Usd

public typealias UsdPrimSiblingRange = Pixar.UsdPrimSiblingRange

public extension Usd
{
  typealias PrimSiblingRange = UsdPrimSiblingRange
}

extension Usd.PrimSiblingRange: IteratorProtocol
{
  public typealias Element = Usd.Prim

  @discardableResult
  private mutating func advance_beginCopy() -> Usd.PrimSiblingRange
  {
    __advance_beginUnsafe(1).pointee
  }

  public mutating func next() -> Element?
  {
    guard empty() == false
    else { return nil }

    let prim = front()

    advance_beginCopy()

    return prim
  }
}
