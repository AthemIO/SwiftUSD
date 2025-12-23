// Array.swift - VtArray wrapper
// Mirrors: pxr/base/vt/array.h

import OpenUSDInterop

/// A wrapper for USD's VtArray type, providing efficient array storage.
/// Mirrors pxr::VtArray from the USD C++ API.
public struct VtArray<Element>: Sendable where Element: Sendable {
    internal var handle: OpaquePointer?

    public init() {
        self.handle = nil
    }

    public var count: Int {
        // TODO: Implement via C interop
        return 0
    }

    public var isEmpty: Bool {
        return count == 0
    }
}

// MARK: - Type-specific VtArray aliases

/// An array of 3D float vectors.
public typealias VtVec3fArray = VtArray<GfVec3f>

/// An array of 3D double vectors.
public typealias VtVec3dArray = VtArray<GfVec3d>

/// An array of integers.
public typealias VtIntArray = VtArray<Int32>

/// An array of floats.
public typealias VtFloatArray = VtArray<Float>

/// An array of doubles.
public typealias VtDoubleArray = VtArray<Double>
