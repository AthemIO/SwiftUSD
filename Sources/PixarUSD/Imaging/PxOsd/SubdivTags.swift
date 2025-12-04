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

/// Swift typealias for PxOsdSubdivTags
public typealias PxOsdSubdivTags = Pixar.PxOsdSubdivTags

public extension PxOsd
{
  /// Subdivision tags for non-hierarchical subdiv surfaces.
  ///
  /// PxOsdSubdivTags contains information about vertex and face-varying
  /// interpolation rules, crease data, and corner weights for subdivision
  /// surfaces.
  ///
  /// ## Usage
  /// ```swift
  /// // Create subdivision tags with interpolation rules
  /// let tags = PxOsd.SubdivTags.create(
  ///     vertexInterpolation: .cornersPlus1,
  ///     faceVaryingInterpolation: .cornersOnly,
  ///     creaseMethod: .uniform
  /// )
  ///
  /// // Access crease data using bridge functions
  /// let creaseIndices = PxOsd.getCreaseIndices(tags)
  /// let creaseWeights = PxOsd.getCreaseWeights(tags)
  /// ```
  typealias SubdivTags = PxOsdSubdivTags
}

// MARK: - SubdivTags Swift Factory Method

public extension PxOsd.SubdivTags
{
  /// Create subdivision tags with all parameters.
  ///
  /// - Parameters:
  ///   - vertexInterpolation: Vertex boundary interpolation rule
  ///   - faceVaryingInterpolation: Face-varying boundary interpolation rule
  ///   - creaseMethod: Crease interpolation method
  ///   - triangleSubdivision: Triangle subdivision method
  ///   - creaseIndices: Edge crease vertex indices
  ///   - creaseLengths: Number of vertices per crease chain
  ///   - creaseWeights: Sharpness weights for creases
  ///   - cornerIndices: Corner vertex indices
  ///   - cornerWeights: Sharpness weights for corners
  /// - Returns: A new SubdivTags instance
  static func create(
    vertexInterpolation: PxOsd.BoundaryInterpolation = .cornersPlus1,
    faceVaryingInterpolation: PxOsd.BoundaryInterpolation = .cornersOnly,
    creaseMethod: PxOsd.CreaseMethod = .uniform,
    triangleSubdivision: Tf.Token = Tf.Token(),
    creaseIndices: Vt.IntArray = Vt.IntArray(),
    creaseLengths: Vt.IntArray = Vt.IntArray(),
    creaseWeights: Vt.FloatArray = Vt.FloatArray(),
    cornerIndices: Vt.IntArray = Vt.IntArray(),
    cornerWeights: Vt.FloatArray = Vt.FloatArray()
  ) -> PxOsd.SubdivTags
  {
    Pixar.PxOsdSubdivTags_Create(
      vertexInterpolation.token,
      faceVaryingInterpolation.token,
      creaseMethod.token,
      triangleSubdivision,
      creaseIndices,
      creaseLengths,
      creaseWeights,
      cornerIndices,
      cornerWeights
    )
  }
}

// MARK: - Swift-Friendly Accessor Functions

/// Swift-friendly accessors for PxOsdSubdivTags.
/// Use these free functions to access subdiv tag data safely from Swift.
public extension PxOsd
{
  /// Returns the edge crease indices.
  static func getCreaseIndices(_ tags: SubdivTags) -> Vt.IntArray
  {
    Pixar.PxOsdSubdivTags_GetCreaseIndices(tags)
  }

  /// Returns the edge crease loop lengths.
  static func getCreaseLengths(_ tags: SubdivTags) -> Vt.IntArray
  {
    Pixar.PxOsdSubdivTags_GetCreaseLengths(tags)
  }

  /// Returns the edge crease weights.
  static func getCreaseWeights(_ tags: SubdivTags) -> Vt.FloatArray
  {
    Pixar.PxOsdSubdivTags_GetCreaseWeights(tags)
  }

  /// Returns the corner indices.
  static func getCornerIndices(_ tags: SubdivTags) -> Vt.IntArray
  {
    Pixar.PxOsdSubdivTags_GetCornerIndices(tags)
  }

  /// Returns the corner weights.
  static func getCornerWeights(_ tags: SubdivTags) -> Vt.FloatArray
  {
    Pixar.PxOsdSubdivTags_GetCornerWeights(tags)
  }
}
