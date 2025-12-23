// Vec3.swift - GfVec3 types
// Mirrors: pxr/base/gf/vec3f.h, vec3d.h, vec3h.h, vec3i.h

import OpenUSDInterop

/// A 3-component vector of single-precision floats.
/// Mirrors pxr::GfVec3f from the USD C++ API.
public struct GfVec3f: Hashable, Sendable {
    public var x: Float
    public var y: Float
    public var z: Float

    public init() {
        self.x = 0
        self.y = 0
        self.z = 0
    }

    public init(_ x: Float, _ y: Float, _ z: Float) {
        self.x = x
        self.y = y
        self.z = z
    }

    public init(repeating value: Float) {
        self.x = value
        self.y = value
        self.z = value
    }

    public static let zero = GfVec3f(0, 0, 0)
    public static let one = GfVec3f(1, 1, 1)

    public subscript(index: Int) -> Float {
        get {
            switch index {
            case 0: return x
            case 1: return y
            case 2: return z
            default: fatalError("Index out of range")
            }
        }
        set {
            switch index {
            case 0: x = newValue
            case 1: y = newValue
            case 2: z = newValue
            default: fatalError("Index out of range")
            }
        }
    }
}

/// A 3-component vector of double-precision floats.
/// Mirrors pxr::GfVec3d from the USD C++ API.
public struct GfVec3d: Hashable, Sendable {
    public var x: Double
    public var y: Double
    public var z: Double

    public init() {
        self.x = 0
        self.y = 0
        self.z = 0
    }

    public init(_ x: Double, _ y: Double, _ z: Double) {
        self.x = x
        self.y = y
        self.z = z
    }

    public init(repeating value: Double) {
        self.x = value
        self.y = value
        self.z = value
    }

    public static let zero = GfVec3d(0, 0, 0)
    public static let one = GfVec3d(1, 1, 1)

    public subscript(index: Int) -> Double {
        get {
            switch index {
            case 0: return x
            case 1: return y
            case 2: return z
            default: fatalError("Index out of range")
            }
        }
        set {
            switch index {
            case 0: x = newValue
            case 1: y = newValue
            case 2: z = newValue
            default: fatalError("Index out of range")
            }
        }
    }
}

/// A 3-component vector of integers.
/// Mirrors pxr::GfVec3i from the USD C++ API.
public struct GfVec3i: Hashable, Sendable {
    public var x: Int32
    public var y: Int32
    public var z: Int32

    public init() {
        self.x = 0
        self.y = 0
        self.z = 0
    }

    public init(_ x: Int32, _ y: Int32, _ z: Int32) {
        self.x = x
        self.y = y
        self.z = z
    }

    public static let zero = GfVec3i(0, 0, 0)
    public static let one = GfVec3i(1, 1, 1)
}
