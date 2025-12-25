// SPDX-License-Identifier: Apache-2.0
// Copyright Contributors to the SwiftUSD Project

/// SwiftMath - SIMD <-> Gf type conversions
///
/// This module provides bidirectional conversions between Swift's native SIMD types
/// and OpenUSD's Gf (Graphics Foundations) math types, enabling seamless interoperability
/// for 3D graphics applications.

import simd
import USDCxx

// Note: Type aliases (GfVec3f, GfMatrix4d, etc.) are already defined in Gf/GfCxx.swift

// MARK: - Vec2 <-> SIMD2 Conversions

extension swiftusd.Vec2f {
    /// Creates a Vec2f from a SIMD2<Float>.
    @inlinable
    public init(_ simd: SIMD2<Float>) {
        self.init(simd.x, simd.y)
    }

    /// Converts to SIMD2<Float>.
    @inlinable
    public var simd: SIMD2<Float> {
        SIMD2<Float>(x, y)
    }
}

extension swiftusd.Vec2d {
    /// Creates a Vec2d from a SIMD2<Double>.
    @inlinable
    public init(_ simd: SIMD2<Double>) {
        self.init(simd.x, simd.y)
    }

    /// Converts to SIMD2<Double>.
    @inlinable
    public var simd: SIMD2<Double> {
        SIMD2<Double>(x, y)
    }
}

extension swiftusd.Vec2i {
    /// Creates a Vec2i from a SIMD2<Int32>.
    @inlinable
    public init(_ simd: SIMD2<Int32>) {
        self.init(simd.x, simd.y)
    }

    /// Converts to SIMD2<Int32>.
    @inlinable
    public var simd: SIMD2<Int32> {
        SIMD2<Int32>(x, y)
    }
}

extension SIMD2 where Scalar == Float {
    /// Creates a SIMD2<Float> from a GfVec2f.
    @inlinable
    public init(_ vec: swiftusd.Vec2f) {
        self.init(vec.x, vec.y)
    }
}

extension SIMD2 where Scalar == Double {
    /// Creates a SIMD2<Double> from a GfVec2d.
    @inlinable
    public init(_ vec: swiftusd.Vec2d) {
        self.init(vec.x, vec.y)
    }
}

extension SIMD2 where Scalar == Int32 {
    /// Creates a SIMD2<Int32> from a GfVec2i.
    @inlinable
    public init(_ vec: swiftusd.Vec2i) {
        self.init(vec.x, vec.y)
    }
}

// MARK: - Vec3 <-> SIMD3 Conversions

extension swiftusd.Vec3f {
    /// Creates a Vec3f from a SIMD3<Float>.
    @inlinable
    public init(_ simd: SIMD3<Float>) {
        self.init(simd.x, simd.y, simd.z)
    }

    /// Converts to SIMD3<Float>.
    @inlinable
    public var simd: SIMD3<Float> {
        SIMD3<Float>(x, y, z)
    }
}

extension swiftusd.Vec3d {
    /// Creates a Vec3d from a SIMD3<Double>.
    @inlinable
    public init(_ simd: SIMD3<Double>) {
        self.init(simd.x, simd.y, simd.z)
    }

    /// Converts to SIMD3<Double>.
    @inlinable
    public var simd: SIMD3<Double> {
        SIMD3<Double>(x, y, z)
    }
}

extension swiftusd.Vec3i {
    /// Creates a Vec3i from a SIMD3<Int32>.
    @inlinable
    public init(_ simd: SIMD3<Int32>) {
        self.init(simd.x, simd.y, simd.z)
    }

    /// Converts to SIMD3<Int32>.
    @inlinable
    public var simd: SIMD3<Int32> {
        SIMD3<Int32>(x, y, z)
    }
}

extension SIMD3 where Scalar == Float {
    /// Creates a SIMD3<Float> from a GfVec3f.
    @inlinable
    public init(_ vec: swiftusd.Vec3f) {
        self.init(vec.x, vec.y, vec.z)
    }
}

extension SIMD3 where Scalar == Double {
    /// Creates a SIMD3<Double> from a GfVec3d.
    @inlinable
    public init(_ vec: swiftusd.Vec3d) {
        self.init(vec.x, vec.y, vec.z)
    }
}

extension SIMD3 where Scalar == Int32 {
    /// Creates a SIMD3<Int32> from a GfVec3i.
    @inlinable
    public init(_ vec: swiftusd.Vec3i) {
        self.init(vec.x, vec.y, vec.z)
    }
}

// MARK: - Vec4 <-> SIMD4 Conversions

extension swiftusd.Vec4f {
    /// Creates a Vec4f from a SIMD4<Float>.
    @inlinable
    public init(_ simd: SIMD4<Float>) {
        self.init(simd.x, simd.y, simd.z, simd.w)
    }

    /// Converts to SIMD4<Float>.
    @inlinable
    public var simd: SIMD4<Float> {
        SIMD4<Float>(x, y, z, w)
    }
}

extension swiftusd.Vec4d {
    /// Creates a Vec4d from a SIMD4<Double>.
    @inlinable
    public init(_ simd: SIMD4<Double>) {
        self.init(simd.x, simd.y, simd.z, simd.w)
    }

    /// Converts to SIMD4<Double>.
    @inlinable
    public var simd: SIMD4<Double> {
        SIMD4<Double>(x, y, z, w)
    }
}

extension SIMD4 where Scalar == Float {
    /// Creates a SIMD4<Float> from a GfVec4f.
    @inlinable
    public init(_ vec: swiftusd.Vec4f) {
        self.init(vec.x, vec.y, vec.z, vec.w)
    }
}

extension SIMD4 where Scalar == Double {
    /// Creates a SIMD4<Double> from a GfVec4d.
    @inlinable
    public init(_ vec: swiftusd.Vec4d) {
        self.init(vec.x, vec.y, vec.z, vec.w)
    }
}

// MARK: - Matrix4 <-> simd_float4x4/simd_double4x4 Conversions

extension swiftusd.Matrix4f {
    /// Creates a Matrix4f from a simd_float4x4.
    ///
    /// Note: simd_float4x4 is column-major, while Matrix4f uses row-major storage.
    /// This constructor handles the transpose automatically.
    @inlinable
    public init(_ simd: simd_float4x4) {
        // simd_float4x4 is column-major, Matrix4f stores row-major
        // Transpose during conversion
        self.init(
            simd.columns.0.x, simd.columns.1.x, simd.columns.2.x, simd.columns.3.x,
            simd.columns.0.y, simd.columns.1.y, simd.columns.2.y, simd.columns.3.y,
            simd.columns.0.z, simd.columns.1.z, simd.columns.2.z, simd.columns.3.z,
            simd.columns.0.w, simd.columns.1.w, simd.columns.2.w, simd.columns.3.w
        )
    }

    /// Converts to simd_float4x4.
    ///
    /// Note: simd_float4x4 is column-major, while Matrix4f uses row-major storage.
    /// This property handles the transpose automatically.
    @inlinable
    public var simd: simd_float4x4 {
        // Matrix4f is row-major, simd_float4x4 is column-major
        // Transpose during conversion
        simd_float4x4(
            SIMD4<Float>(m.0, m.4, m.8, m.12),
            SIMD4<Float>(m.1, m.5, m.9, m.13),
            SIMD4<Float>(m.2, m.6, m.10, m.14),
            SIMD4<Float>(m.3, m.7, m.11, m.15)
        )
    }
}

extension swiftusd.Matrix4d {
    /// Creates a Matrix4d from a simd_double4x4.
    ///
    /// Note: simd_double4x4 is column-major, while Matrix4d uses row-major storage.
    /// This constructor handles the transpose automatically.
    @inlinable
    public init(_ simd: simd_double4x4) {
        // simd_double4x4 is column-major, Matrix4d stores row-major
        // Transpose during conversion
        self.init(
            simd.columns.0.x, simd.columns.1.x, simd.columns.2.x, simd.columns.3.x,
            simd.columns.0.y, simd.columns.1.y, simd.columns.2.y, simd.columns.3.y,
            simd.columns.0.z, simd.columns.1.z, simd.columns.2.z, simd.columns.3.z,
            simd.columns.0.w, simd.columns.1.w, simd.columns.2.w, simd.columns.3.w
        )
    }

    /// Converts to simd_double4x4.
    ///
    /// Note: simd_double4x4 is column-major, while Matrix4d uses row-major storage.
    /// This property handles the transpose automatically.
    @inlinable
    public var simd: simd_double4x4 {
        // Matrix4d is row-major, simd_double4x4 is column-major
        // Transpose during conversion
        simd_double4x4(
            SIMD4<Double>(m.0, m.4, m.8, m.12),
            SIMD4<Double>(m.1, m.5, m.9, m.13),
            SIMD4<Double>(m.2, m.6, m.10, m.14),
            SIMD4<Double>(m.3, m.7, m.11, m.15)
        )
    }
}

extension simd_float4x4 {
    /// Creates a simd_float4x4 from a GfMatrix4f.
    @inlinable
    public init(_ matrix: swiftusd.Matrix4f) {
        self = matrix.simd
    }
}

extension simd_double4x4 {
    /// Creates a simd_double4x4 from a GfMatrix4d.
    @inlinable
    public init(_ matrix: swiftusd.Matrix4d) {
        self = matrix.simd
    }
}

// MARK: - Quaternion <-> simd_quatf/simd_quatd Conversions

extension swiftusd.Quatf {
    /// Creates a Quatf from a simd_quatf.
    ///
    /// Note: simd_quatf stores as (x, y, z, w) but Quatf stores as (real, i, j, k).
    @inlinable
    public init(_ simd: simd_quatf) {
        let v = simd.vector
        self.init(v.w, v.x, v.y, v.z)  // real, i, j, k
    }

    /// Converts to simd_quatf.
    @inlinable
    public var simd: simd_quatf {
        simd_quatf(ix: i, iy: j, iz: k, r: real)
    }
}

extension swiftusd.Quatd {
    /// Creates a Quatd from a simd_quatd.
    ///
    /// Note: simd_quatd stores as (x, y, z, w) but Quatd stores as (real, i, j, k).
    @inlinable
    public init(_ simd: simd_quatd) {
        let v = simd.vector
        self.init(v.w, v.x, v.y, v.z)  // real, i, j, k
    }

    /// Converts to simd_quatd.
    @inlinable
    public var simd: simd_quatd {
        simd_quatd(ix: i, iy: j, iz: k, r: real)
    }
}

extension simd_quatf {
    /// Creates a simd_quatf from a GfQuatf.
    @inlinable
    public init(_ quat: swiftusd.Quatf) {
        self = quat.simd
    }
}

extension simd_quatd {
    /// Creates a simd_quatd from a GfQuatd.
    @inlinable
    public init(_ quat: swiftusd.Quatd) {
        self = quat.simd
    }
}

// MARK: - Array Conversions

extension Array where Element == swiftusd.Vec3f {
    /// Converts an array of Vec3f to an array of SIMD3<Float>.
    @inlinable
    public var simd: [SIMD3<Float>] {
        map { $0.simd }
    }
}

extension Array where Element == SIMD3<Float> {
    /// Converts an array of SIMD3<Float> to an array of Vec3f.
    @inlinable
    public var gf: [swiftusd.Vec3f] {
        map { swiftusd.Vec3f($0) }
    }
}

extension Array where Element == swiftusd.Vec3d {
    /// Converts an array of Vec3d to an array of SIMD3<Double>.
    @inlinable
    public var simd: [SIMD3<Double>] {
        map { $0.simd }
    }
}

extension Array where Element == SIMD3<Double> {
    /// Converts an array of SIMD3<Double> to an array of Vec3d.
    @inlinable
    public var gf: [swiftusd.Vec3d] {
        map { swiftusd.Vec3d($0) }
    }
}

// MARK: - Precision Conversion Utilities

extension swiftusd.Vec3f {
    /// Converts to double precision Vec3d.
    @inlinable
    public var asDouble: swiftusd.Vec3d {
        swiftusd.Vec3d(Double(x), Double(y), Double(z))
    }
}

extension swiftusd.Vec3d {
    /// Converts to single precision Vec3f.
    @inlinable
    public var asFloat: swiftusd.Vec3f {
        swiftusd.Vec3f(Float(x), Float(y), Float(z))
    }
}

extension swiftusd.Vec4f {
    /// Converts to double precision Vec4d.
    @inlinable
    public var asDouble: swiftusd.Vec4d {
        swiftusd.Vec4d(Double(x), Double(y), Double(z), Double(w))
    }
}

extension swiftusd.Vec4d {
    /// Converts to single precision Vec4f.
    @inlinable
    public var asFloat: swiftusd.Vec4f {
        swiftusd.Vec4f(Float(x), Float(y), Float(z), Float(w))
    }
}

extension swiftusd.Quatf {
    /// Converts to double precision Quatd.
    @inlinable
    public var asDouble: swiftusd.Quatd {
        swiftusd.Quatd(Double(real), Double(i), Double(j), Double(k))
    }
}

extension swiftusd.Quatd {
    /// Converts to single precision Quatf.
    @inlinable
    public var asFloat: swiftusd.Quatf {
        swiftusd.Quatf(Float(real), Float(i), Float(j), Float(k))
    }
}
