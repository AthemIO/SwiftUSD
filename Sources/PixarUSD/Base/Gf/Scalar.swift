/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

public protocol Scalar: Sequence, Dimensional
{
  /// Scalar element type.
  associatedtype ScalarType: Numeric

  /// Default constructor does no initialization.
  init()

  /// Initialize all elements to a single value.
  init(_ value: ScalarType)

  /// Indexing.
  subscript(_: Int) -> ScalarType { get set }

  /// Direct data access.
  func getArray() -> [ScalarType]

  /// Returns an iterator over the elements in this scalar.
  func makeIterator() -> ScalarIterator<Self>
}

/// Create an iterator for all scalar types,
/// so that they can act as their own iterator.
public struct ScalarIterator<T>: IteratorProtocol where T: Scalar
{
  let scalar: T
  var index = 0

  public init(_ scalar: T)
  {
    self.scalar = scalar
  }

  public mutating func next() -> T.ScalarType?
  {
    let nextIndex = index + 1
    guard nextIndex < (T.AxisCount.dimension + 1)
    else { return nil }

    let valueIndex = scalar.getArray().index(before: nextIndex)

    index += 1
    return scalar.getArray()[valueIndex]
  }
}
