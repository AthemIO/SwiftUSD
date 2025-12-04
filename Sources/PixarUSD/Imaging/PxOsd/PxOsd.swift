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

/**
 * # ``PxOsd``
 *
 * **OpenSubdiv Integration for USD/Hydra**
 *
 * PxOsd provides the interface between USD and OpenSubdiv for subdivision
 * surface processing. It defines mesh topology structures, subdivision tags,
 * and validation utilities used throughout the USD imaging pipeline.
 *
 * ## Overview
 *
 * OpenSubdiv is a library for efficient subdivision surface evaluation
 * developed by Pixar. PxOsd wraps OpenSubdiv concepts for use with USD:
 *
 * - **Subdivision Schemes**: Catmull-Clark, Loop, and Bilinear subdivision
 * - **Mesh Topology**: Immutable representation of mesh connectivity
 * - **Subdiv Tags**: Edge creases, corners, and interpolation rules
 * - **Validation**: Comprehensive topology validation
 *
 * ## Key Types
 *
 * - ``PxOsd/MeshTopology``: Immutable mesh topology representation
 * - ``PxOsd/SubdivTags``: Subdivision parameters (creases, corners)
 * - ``PxOsd/MeshTopologyValidation``: Topology validation results
 * - ``PxOsd/Tokens``: OpenSubdiv-related tokens
 *
 * ## Subdivision Schemes
 *
 * | Scheme | Description | Best For |
 * |--------|-------------|----------|
 * | Catmull-Clark | Smooth quad-dominant | Characters, organic shapes |
 * | Loop | Triangle-based | Triangle meshes |
 * | Bilinear | Linear interpolation | Low-poly, hard surfaces |
 *
 * ## Example Usage
 *
 * ### Creating Mesh Topology
 *
 * ```swift
 * import PixarUSD
 *
 * // Create a simple quad mesh topology
 * var faceVertexCounts = Vt.IntArray()
 * faceVertexCounts.push_back(4)  // One quad
 *
 * var faceVertexIndices = Vt.IntArray()
 * faceVertexIndices.push_back(0)
 * faceVertexIndices.push_back(1)
 * faceVertexIndices.push_back(2)
 * faceVertexIndices.push_back(3)
 *
 * let topology = PxOsd.MeshTopology.create(
 *     scheme: .catmullClark,
 *     orientation: .rightHanded,
 *     faceVertexCounts: faceVertexCounts,
 *     faceVertexIndices: faceVertexIndices
 * )
 *
 * // Validate the topology
 * if topology.isValid {
 *     print("Topology is valid!")
 * }
 * ```
 *
 * ### Working with Subdivision Tags
 *
 * ```swift
 * // Create subdiv tags with edge creases
 * var tags = PxOsd.SubdivTags.create(
 *     vertexInterpolation: .cornersPlus1,
 *     faceVaryingInterpolation: .cornersOnly,
 *     creaseMethod: .uniform
 * )
 *
 * // Add crease data
 * var creaseIndices = Vt.IntArray()
 * creaseIndices.push_back(0)
 * creaseIndices.push_back(1)
 *
 * var creaseLengths = Vt.IntArray()
 * creaseLengths.push_back(2)
 *
 * var creaseWeights = Vt.FloatArray()
 * creaseWeights.push_back(2.0)  // Sharp crease
 *
 * tags.setCreases(
 *     indices: creaseIndices,
 *     lengths: creaseLengths,
 *     weights: creaseWeights
 * )
 * ```
 *
 * ### Using with UsdGeom.Mesh
 *
 * ```swift
 * // UsdGeom.Mesh automatically uses OpenSubdiv when subdivision is enabled
 * let mesh = UsdGeom.Mesh.define(stage, path: "/Model/Mesh")
 * mesh.CreateSubdivisionSchemeAttr(Vt.Value(Tf.Token("catmullClark")), false)
 *
 * // Set crease data
 * mesh.CreateCreaseIndicesAttr(Vt.Value(creaseIndices), false)
 * mesh.CreateCreaseLengthsAttr(Vt.Value(creaseLengths), false)
 * mesh.CreateCreaseSharpnessesAttr(Vt.Value(creaseWeights), false)
 * ```
 *
 * ## Token Reference
 *
 * Access OpenSubdiv tokens via ``PxOsd/Tokens``:
 *
 * ```swift
 * // Subdivision schemes
 * let catmullClark = PxOsd.Tokens.catmullClark.getToken()
 * let loop = PxOsd.Tokens.loop.getToken()
 * let bilinear = PxOsd.Tokens.bilinear.getToken()
 *
 * // Orientations
 * let rightHanded = PxOsd.Tokens.rightHanded.getToken()
 * let leftHanded = PxOsd.Tokens.leftHanded.getToken()
 *
 * // Boundary interpolation
 * let cornersOnly = PxOsd.Tokens.cornersOnly.getToken()
 * let cornersPlus1 = PxOsd.Tokens.cornersPlus1.getToken()
 * ```
 */
public enum PxOsd
{}
