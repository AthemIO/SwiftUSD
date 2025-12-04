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

import PxOsd

/// Swift typealias for PxOsdMeshTopologyValidation
public typealias PxOsdMeshTopologyValidation = Pixar.PxOsdMeshTopologyValidation

public extension PxOsd
{
  /// Utility to help validate an OpenSubdiv mesh topology.
  ///
  /// This class is created by `PxOsdMeshTopology.validate()` and contains
  /// a list of any validation errors found. The C++ type has an `explicit operator bool()`
  /// that returns true if the topology is valid.
  ///
  /// ## Example
  /// ```swift
  /// let topology = PxOsd.MeshTopology.create(...)
  /// let validation = topology.validate()
  /// // Use validation in conditional context directly
  /// ```
  typealias MeshTopologyValidation = PxOsdMeshTopologyValidation
}

// MARK: - Validation Code Swift Enum

public extension PxOsd
{
  /// Codes for various invalid states in mesh topology.
  enum ValidationCode: Int, CaseIterable, CustomStringConvertible
  {
    /// Invalid subdivision scheme token value
    case invalidScheme = 0
    /// Invalid orientation token value
    case invalidOrientation
    /// Invalid triangle subdivision token value
    case invalidTriangleSubdivision
    /// Invalid vertex interpolation rule token value
    case invalidVertexInterpolationRule
    /// Invalid face-varying interpolation rule token value
    case invalidFaceVaryingInterpolationRule
    /// Invalid crease method token value
    case invalidCreaseMethod
    /// Crease lengths element less than 2
    case invalidCreaseLengthElement
    /// Crease indices size not matching sum of lengths array
    case invalidCreaseIndicesSize
    /// Crease indices element not in face vertex indices vector
    case invalidCreaseIndicesElement
    /// Crease weights size doesn't match number of creases or edges
    case invalidCreaseWeightsSize
    /// Crease weights contain negative values
    case negativeCreaseWeights
    /// Corner indices element not in face vertex indices vector
    case invalidCornerIndicesElement
    /// Corner weights contain negative values
    case negativeCornerWeights
    /// Corner weights size doesn't match number of corner indices
    case invalidCornerWeightsSize
    /// Hole indices negative or greater than max face index
    case invalidHoleIndicesElement
    /// Face vertex count less than 3
    case invalidFaceVertexCountsElement
    /// Face vertex index is negative
    case invalidFaceVertexIndicesElement
    /// Indices size doesn't match sum of face vertex counts
    case invalidFaceVertexIndicesSize

    public var description: String
    {
      switch self
      {
        case .invalidScheme: "Invalid subdivision scheme"
        case .invalidOrientation: "Invalid orientation"
        case .invalidTriangleSubdivision: "Invalid triangle subdivision"
        case .invalidVertexInterpolationRule: "Invalid vertex interpolation rule"
        case .invalidFaceVaryingInterpolationRule: "Invalid face-varying interpolation rule"
        case .invalidCreaseMethod: "Invalid crease method"
        case .invalidCreaseLengthElement: "Crease length element less than 2"
        case .invalidCreaseIndicesSize: "Crease indices size mismatch"
        case .invalidCreaseIndicesElement: "Invalid crease indices element"
        case .invalidCreaseWeightsSize: "Crease weights size mismatch"
        case .negativeCreaseWeights: "Negative crease weights"
        case .invalidCornerIndicesElement: "Invalid corner indices element"
        case .negativeCornerWeights: "Negative corner weights"
        case .invalidCornerWeightsSize: "Corner weights size mismatch"
        case .invalidHoleIndicesElement: "Invalid hole indices element"
        case .invalidFaceVertexCountsElement: "Face vertex count less than 3"
        case .invalidFaceVertexIndicesElement: "Negative face vertex index"
        case .invalidFaceVertexIndicesSize: "Face vertex indices size mismatch"
      }
    }
  }
}

// MARK: - Swift-Friendly Validation Error

public extension PxOsd
{
  /// A validation error containing a code and descriptive message.
  struct ValidationError: CustomStringConvertible
  {
    /// The validation code indicating the type of error
    public let code: ValidationCode

    /// A human-readable message describing the error
    public let message: String

    public var description: String
    {
      "[\(code)]: \(message)"
    }
  }
}
