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

/// Swift typealias for PxOsdMeshTopology
public typealias PxOsdMeshTopology = Pixar.PxOsdMeshTopology

public extension PxOsd
{
  /// Topology data for meshes (OpenSubdiv).
  ///
  /// Once constructed, this class is immutable (except when assigned or moved).
  /// It stores the mesh topology including face vertex counts, indices,
  /// subdivision scheme, and orientation.
  ///
  /// ## Usage
  /// ```swift
  /// // Create a mesh topology using Swift factory methods
  /// let topology = PxOsd.MeshTopology.create(
  ///     scheme: .catmullClark,
  ///     orientation: .rightHanded,
  ///     faceVertexCounts: faceVertexCounts,
  ///     faceVertexIndices: faceVertexIndices
  /// )
  ///
  /// // Access topology data using bridge functions
  /// let counts = Pixar.PxOsdMeshTopology_GetFaceVertexCounts(topology)
  /// let indices = Pixar.PxOsdMeshTopology_GetFaceVertexIndices(topology)
  /// ```
  typealias MeshTopology = PxOsdMeshTopology
}

// MARK: - MeshTopology Swift Factory Methods

public extension PxOsd.MeshTopology
{
  /// Create a mesh topology with basic parameters.
  ///
  /// - Parameters:
  ///   - scheme: Subdivision scheme (.catmullClark, .loop, .bilinear, .none)
  ///   - orientation: Winding orientation (.leftHanded, .rightHanded)
  ///   - faceVertexCounts: Number of vertices per face
  ///   - faceVertexIndices: Vertex indices for all faces
  /// - Returns: A new mesh topology
  static func create(
    scheme: PxOsd.SubdivisionScheme,
    orientation: PxOsd.Orientation,
    faceVertexCounts: Vt.IntArray,
    faceVertexIndices: Vt.IntArray
  ) -> PxOsd.MeshTopology
  {
    Pixar.PxOsdMeshTopology_Create(
      scheme.token,
      orientation.token,
      faceVertexCounts,
      faceVertexIndices
    )
  }

  /// Create a mesh topology with holes.
  static func create(
    scheme: PxOsd.SubdivisionScheme,
    orientation: PxOsd.Orientation,
    faceVertexCounts: Vt.IntArray,
    faceVertexIndices: Vt.IntArray,
    holeIndices: Vt.IntArray
  ) -> PxOsd.MeshTopology
  {
    Pixar.PxOsdMeshTopology_CreateWithHoles(
      scheme.token,
      orientation.token,
      faceVertexCounts,
      faceVertexIndices,
      holeIndices
    )
  }

  /// Create a mesh topology with subdivision tags.
  static func create(
    scheme: PxOsd.SubdivisionScheme,
    orientation: PxOsd.Orientation,
    faceVertexCounts: Vt.IntArray,
    faceVertexIndices: Vt.IntArray,
    subdivTags: PxOsd.SubdivTags
  ) -> PxOsd.MeshTopology
  {
    Pixar.PxOsdMeshTopology_CreateWithSubdivTags(
      scheme.token,
      orientation.token,
      faceVertexCounts,
      faceVertexIndices,
      subdivTags
    )
  }

  /// Create a mesh topology with holes and subdivision tags.
  static func create(
    scheme: PxOsd.SubdivisionScheme,
    orientation: PxOsd.Orientation,
    faceVertexCounts: Vt.IntArray,
    faceVertexIndices: Vt.IntArray,
    holeIndices: Vt.IntArray,
    subdivTags: PxOsd.SubdivTags
  ) -> PxOsd.MeshTopology
  {
    Pixar.PxOsdMeshTopology_CreateFull(
      scheme.token,
      orientation.token,
      faceVertexCounts,
      faceVertexIndices,
      holeIndices,
      subdivTags
    )
  }
}

// MARK: - Swift-Friendly Accessor Functions

/// Swift-friendly accessors for PxOsdMeshTopology.
/// Use these free functions to access topology data safely from Swift.
public extension PxOsd
{
  /// Returns face vertex counts from a mesh topology.
  static func getFaceVertexCounts(_ topology: MeshTopology) -> Vt.IntArray
  {
    Pixar.PxOsdMeshTopology_GetFaceVertexCounts(topology)
  }

  /// Returns face vertex indices from a mesh topology.
  static func getFaceVertexIndices(_ topology: MeshTopology) -> Vt.IntArray
  {
    Pixar.PxOsdMeshTopology_GetFaceVertexIndices(topology)
  }

  /// Returns orientation token from a mesh topology.
  static func getOrientation(_ topology: MeshTopology) -> Tf.Token
  {
    Pixar.PxOsdMeshTopology_GetOrientation(topology)
  }

  /// Returns hole indices from a mesh topology.
  static func getHoleIndices(_ topology: MeshTopology) -> Vt.IntArray
  {
    Pixar.PxOsdMeshTopology_GetHoleIndices(topology)
  }

  /// Returns subdivision tags from a mesh topology.
  static func getSubdivTags(_ topology: MeshTopology) -> SubdivTags
  {
    Pixar.PxOsdMeshTopology_GetSubdivTags(topology)
  }

  /// Returns whether a mesh topology is valid.
  static func isValid(_ topology: MeshTopology) -> Bool
  {
    Pixar.PxOsdMeshTopology_IsValid(topology)
  }
}
