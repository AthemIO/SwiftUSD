// VtCxx.swift - Swift extensions for Vt array types via C++ interop
// Provides Swift-friendly API for swiftusd::IntArray, FloatArray, Vec3fArray, Matrix4dArray

import USDCxx

// MARK: - Type Aliases

/// Type aliases that map to the C++ types in swiftusd namespace
public typealias VtIntArray = swiftusd.IntArray
public typealias VtFloatArray = swiftusd.FloatArray
public typealias VtDoubleArray = swiftusd.DoubleArray
public typealias VtVec3fArray = swiftusd.Vec3fArray
public typealias VtVec3dArray = swiftusd.Vec3dArray
public typealias VtMatrix4dArray = swiftusd.Matrix4dArray

// MARK: - IntArray Extensions

extension swiftusd.IntArray: @unchecked Sendable {}

extension swiftusd.IntArray {
    /// Creates an IntArray from a Swift array of Int32.
    public static func from(_ elements: [Int32]) -> swiftusd.IntArray {
        let result = swiftusd.IntArray()
        result.reserve(elements.count)
        for element in elements {
            result.append(element)
        }
        return result
    }

    /// Returns all elements as a Swift array.
    public var elements: [Int32] {
        let c = Int(size())
        guard c > 0 else { return [] }
        var result = [Int32]()
        result.reserveCapacity(c)
        for i in 0..<c {
            result.append(GetAt(i))
        }
        return result
    }

    /// The number of elements.
    public var count: Int { Int(size()) }

    /// Whether the array is empty.
    public var isEmpty: Bool { empty() }

    /// Subscript access to elements.
    public subscript(index: Int) -> Int32 {
        get { GetAt(index) }
        set { SetAt(index, newValue) }
    }
}

extension swiftusd.IntArray: Equatable {
    public static func == (lhs: swiftusd.IntArray, rhs: swiftusd.IntArray) -> Bool {
        lhs.elements == rhs.elements
    }
}

extension swiftusd.IntArray: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(hash())
    }
}

extension swiftusd.IntArray: CustomStringConvertible {
    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "VtIntArray([])"
        }
        if elems.count <= 10 {
            return "VtIntArray([\(elems.map { String($0) }.joined(separator: ", "))])"
        }
        let first5 = elems.prefix(5).map { String($0) }.joined(separator: ", ")
        return "VtIntArray([\(first5), ... (\(elems.count) elements)])"
    }
}

// MARK: - FloatArray Extensions

extension swiftusd.FloatArray: @unchecked Sendable {}

extension swiftusd.FloatArray {
    /// Creates a FloatArray from a Swift array of Float.
    public static func from(_ elements: [Float]) -> swiftusd.FloatArray {
        let result = swiftusd.FloatArray()
        result.reserve(elements.count)
        for element in elements {
            result.append(element)
        }
        return result
    }

    /// Returns all elements as a Swift array.
    public var elements: [Float] {
        let c = Int(size())
        guard c > 0 else { return [] }
        var result = [Float]()
        result.reserveCapacity(c)
        for i in 0..<c {
            result.append(GetAt(i))
        }
        return result
    }

    /// The number of elements.
    public var count: Int { Int(size()) }

    /// Whether the array is empty.
    public var isEmpty: Bool { empty() }

    /// Subscript access to elements.
    public subscript(index: Int) -> Float {
        get { GetAt(index) }
        set { SetAt(index, newValue) }
    }
}

extension swiftusd.FloatArray: Equatable {
    public static func == (lhs: swiftusd.FloatArray, rhs: swiftusd.FloatArray) -> Bool {
        lhs.elements == rhs.elements
    }
}

extension swiftusd.FloatArray: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(hash())
    }
}

extension swiftusd.FloatArray: CustomStringConvertible {
    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "VtFloatArray([])"
        }
        if elems.count <= 10 {
            return "VtFloatArray([\(elems.map { String($0) }.joined(separator: ", "))])"
        }
        let first5 = elems.prefix(5).map { String($0) }.joined(separator: ", ")
        return "VtFloatArray([\(first5), ... (\(elems.count) elements)])"
    }
}

// MARK: - DoubleArray Extensions

extension swiftusd.DoubleArray: @unchecked Sendable {}

extension swiftusd.DoubleArray {
    /// Creates a DoubleArray from a Swift array of Double.
    public static func from(_ elements: [Double]) -> swiftusd.DoubleArray {
        let result = swiftusd.DoubleArray()
        result.reserve(elements.count)
        for element in elements {
            result.append(element)
        }
        return result
    }

    /// Returns all elements as a Swift array.
    public var elements: [Double] {
        let c = Int(size())
        guard c > 0 else { return [] }
        var result = [Double]()
        result.reserveCapacity(c)
        for i in 0..<c {
            result.append(GetAt(i))
        }
        return result
    }

    /// The number of elements.
    public var count: Int { Int(size()) }

    /// Whether the array is empty.
    public var isEmpty: Bool { empty() }

    /// Subscript access to elements.
    public subscript(index: Int) -> Double {
        get { GetAt(index) }
        set { SetAt(index, newValue) }
    }
}

extension swiftusd.DoubleArray: Equatable {
    public static func == (lhs: swiftusd.DoubleArray, rhs: swiftusd.DoubleArray) -> Bool {
        lhs.elements == rhs.elements
    }
}

extension swiftusd.DoubleArray: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(hash())
    }
}

extension swiftusd.DoubleArray: CustomStringConvertible {
    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "VtDoubleArray([])"
        }
        if elems.count <= 10 {
            return "VtDoubleArray([\(elems.map { String($0) }.joined(separator: ", "))])"
        }
        let first5 = elems.prefix(5).map { String($0) }.joined(separator: ", ")
        return "VtDoubleArray([\(first5), ... (\(elems.count) elements)])"
    }
}

// MARK: - Vec3fArray Extensions

extension swiftusd.Vec3fArray: @unchecked Sendable {}

extension swiftusd.Vec3fArray {
    /// Creates a Vec3fArray from a Swift array of Vec3f.
    public static func from(_ elements: [swiftusd.Vec3f]) -> swiftusd.Vec3fArray {
        let result = swiftusd.Vec3fArray()
        result.reserve(elements.count)
        for element in elements {
            result.append(element)
        }
        return result
    }

    /// Returns all elements as a Swift array.
    public var elements: [swiftusd.Vec3f] {
        let c = Int(size())
        guard c > 0 else { return [] }
        var result = [swiftusd.Vec3f]()
        result.reserveCapacity(c)
        for i in 0..<c {
            result.append(GetAt(i))
        }
        return result
    }

    /// The number of elements.
    public var count: Int { Int(size()) }

    /// Whether the array is empty.
    public var isEmpty: Bool { empty() }

    /// Subscript access to elements.
    public subscript(index: Int) -> swiftusd.Vec3f {
        get { GetAt(index) }
        set { SetAt(index, newValue) }
    }
}

extension swiftusd.Vec3fArray: Equatable {
    public static func == (lhs: swiftusd.Vec3fArray, rhs: swiftusd.Vec3fArray) -> Bool {
        guard lhs.count == rhs.count else { return false }
        for i in 0..<lhs.count {
            if lhs.GetAt(i) != rhs.GetAt(i) { return false }
        }
        return true
    }
}

extension swiftusd.Vec3fArray: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(hash())
    }
}

extension swiftusd.Vec3fArray: CustomStringConvertible {
    public var description: String {
        let c = count
        if c == 0 {
            return "VtVec3fArray([])"
        }
        if c <= 5 {
            let items = elements.map { "(\($0.x), \($0.y), \($0.z))" }.joined(separator: ", ")
            return "VtVec3fArray([\(items)])"
        }
        return "VtVec3fArray(\(c) elements)"
    }
}

// MARK: - Vec3dArray Extensions

extension swiftusd.Vec3dArray: @unchecked Sendable {}

extension swiftusd.Vec3dArray {
    /// Creates a Vec3dArray from a Swift array of Vec3d.
    public static func from(_ elements: [swiftusd.Vec3d]) -> swiftusd.Vec3dArray {
        let result = swiftusd.Vec3dArray()
        result.reserve(elements.count)
        for element in elements {
            result.append(element)
        }
        return result
    }

    /// Returns all elements as a Swift array.
    public var elements: [swiftusd.Vec3d] {
        let c = Int(size())
        guard c > 0 else { return [] }
        var result = [swiftusd.Vec3d]()
        result.reserveCapacity(c)
        for i in 0..<c {
            result.append(GetAt(i))
        }
        return result
    }

    /// The number of elements.
    public var count: Int { Int(size()) }

    /// Whether the array is empty.
    public var isEmpty: Bool { empty() }

    /// Subscript access to elements.
    public subscript(index: Int) -> swiftusd.Vec3d {
        get { GetAt(index) }
        set { SetAt(index, newValue) }
    }
}

extension swiftusd.Vec3dArray: Equatable {
    public static func == (lhs: swiftusd.Vec3dArray, rhs: swiftusd.Vec3dArray) -> Bool {
        guard lhs.count == rhs.count else { return false }
        for i in 0..<lhs.count {
            if lhs.GetAt(i) != rhs.GetAt(i) { return false }
        }
        return true
    }
}

extension swiftusd.Vec3dArray: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(hash())
    }
}

extension swiftusd.Vec3dArray: CustomStringConvertible {
    public var description: String {
        let c = count
        if c == 0 {
            return "VtVec3dArray([])"
        }
        if c <= 5 {
            let items = elements.map { "(\($0.x), \($0.y), \($0.z))" }.joined(separator: ", ")
            return "VtVec3dArray([\(items)])"
        }
        return "VtVec3dArray(\(c) elements)"
    }
}

// MARK: - Matrix4dArray Extensions

extension swiftusd.Matrix4dArray: @unchecked Sendable {}

extension swiftusd.Matrix4dArray {
    /// Creates a Matrix4dArray from a Swift array of Matrix4d.
    public static func from(_ elements: [swiftusd.Matrix4d]) -> swiftusd.Matrix4dArray {
        let result = swiftusd.Matrix4dArray()
        result.reserve(elements.count)
        for element in elements {
            result.append(element)
        }
        return result
    }

    /// Returns all elements as a Swift array.
    public var elements: [swiftusd.Matrix4d] {
        let c = Int(size())
        guard c > 0 else { return [] }
        var result = [swiftusd.Matrix4d]()
        result.reserveCapacity(c)
        for i in 0..<c {
            result.append(GetAt(i))
        }
        return result
    }

    /// The number of elements.
    public var count: Int { Int(size()) }

    /// Whether the array is empty.
    public var isEmpty: Bool { empty() }

    /// Subscript access to elements.
    public subscript(index: Int) -> swiftusd.Matrix4d {
        get { GetAt(index) }
        set { SetAt(index, newValue) }
    }
}

extension swiftusd.Matrix4dArray: Equatable {
    public static func == (lhs: swiftusd.Matrix4dArray, rhs: swiftusd.Matrix4dArray) -> Bool {
        guard lhs.count == rhs.count else { return false }
        for i in 0..<lhs.count {
            if lhs.GetAt(i) != rhs.GetAt(i) { return false }
        }
        return true
    }
}

extension swiftusd.Matrix4dArray: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(hash())
    }
}

extension swiftusd.Matrix4dArray: CustomStringConvertible {
    public var description: String {
        let c = count
        if c == 0 {
            return "VtMatrix4dArray([])"
        }
        return "VtMatrix4dArray(\(c) elements)"
    }
}
