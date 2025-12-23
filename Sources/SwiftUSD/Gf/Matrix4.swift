// Matrix4.swift - GfMatrix4 types
// Mirrors: pxr/base/gf/matrix4d.h, matrix4f.h

import OpenUSDInterop

/// A 4x4 matrix of double-precision floats.
/// Mirrors pxr::GfMatrix4d from the USD C++ API.
public struct GfMatrix4d: Hashable, Sendable {
    public var data: (
        Double, Double, Double, Double,
        Double, Double, Double, Double,
        Double, Double, Double, Double,
        Double, Double, Double, Double
    )

    public init() {
        self.data = (
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        )
    }

    public static let identity = GfMatrix4d()

    public subscript(row: Int, col: Int) -> Double {
        get {
            let index = row * 4 + col
            return withUnsafeBytes(of: data) { buffer in
                buffer.load(fromByteOffset: index * MemoryLayout<Double>.size, as: Double.self)
            }
        }
        set {
            let index = row * 4 + col
            withUnsafeMutableBytes(of: &data) { buffer in
                buffer.storeBytes(of: newValue, toByteOffset: index * MemoryLayout<Double>.size, as: Double.self)
            }
        }
    }
}

/// A 4x4 matrix of single-precision floats.
/// Mirrors pxr::GfMatrix4f from the USD C++ API.
public struct GfMatrix4f: Hashable, Sendable {
    public var data: (
        Float, Float, Float, Float,
        Float, Float, Float, Float,
        Float, Float, Float, Float,
        Float, Float, Float, Float
    )

    public init() {
        self.data = (
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        )
    }

    public static let identity = GfMatrix4f()
}
