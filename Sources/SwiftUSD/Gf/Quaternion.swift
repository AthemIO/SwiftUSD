// Quaternion.swift - GfQuaternion types
// Mirrors: pxr/base/gf/quatd.h, quatf.h, quath.h

import OpenUSDInterop

/// A quaternion of double-precision floats.
/// Mirrors pxr::GfQuatd from the USD C++ API.
public struct GfQuatd: Hashable, Sendable {
    public var real: Double
    public var imaginary: GfVec3d

    public init() {
        self.real = 1
        self.imaginary = .zero
    }

    public init(real: Double, imaginary: GfVec3d) {
        self.real = real
        self.imaginary = imaginary
    }

    public static let identity = GfQuatd()
}

/// A quaternion of single-precision floats.
/// Mirrors pxr::GfQuatf from the USD C++ API.
public struct GfQuatf: Hashable, Sendable {
    public var real: Float
    public var imaginary: GfVec3f

    public init() {
        self.real = 1
        self.imaginary = .zero
    }

    public init(real: Float, imaginary: GfVec3f) {
        self.real = real
        self.imaginary = imaginary
    }

    public static let identity = GfQuatf()
}
