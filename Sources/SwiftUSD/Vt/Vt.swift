// Vt.swift - Value Types - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Vt types,
// including arrays with copy-on-write semantics for efficient data handling.
// These arrays are used extensively in USD for mesh data, primvars, etc.

import CxxFacade

// MARK: - Type Aliases

/// VtIntArray - Array of integers with copy-on-write semantics
public typealias VtIntArray = cxxfacade.vt.VtIntArray

/// VtFloatArray - Array of floats with copy-on-write semantics
public typealias VtFloatArray = cxxfacade.vt.VtFloatArray

/// VtDoubleArray - Array of doubles with copy-on-write semantics
public typealias VtDoubleArray = cxxfacade.vt.VtDoubleArray

/// VtVec2fArray - Array of Vec2f with copy-on-write semantics (UV coordinates)
public typealias VtVec2fArray = cxxfacade.vt.VtVec2fArray

/// VtVec3fArray - Array of Vec3f with copy-on-write semantics (positions, normals, colors)
public typealias VtVec3fArray = cxxfacade.vt.VtVec3fArray

/// VtVec4fArray - Array of Vec4f with copy-on-write semantics (RGBA colors)
public typealias VtVec4fArray = cxxfacade.vt.VtVec4fArray

/// VtMatrix4dArray - Array of Matrix4d with copy-on-write semantics (skeletal transforms)
public typealias VtMatrix4dArray = cxxfacade.vt.VtMatrix4dArray

/// VtTokenArray - Array of TfToken with copy-on-write semantics (string enumerations)
public typealias VtTokenArray = cxxfacade.vt.VtTokenArray

/// VtValue - Type-erased value container
public typealias VtValue = cxxfacade.vt.VtValue

// MARK: - VtIntArray Extensions

extension VtIntArray: CustomStringConvertible {
    public var description: String {
        return "VtIntArray(count: \(size()))"
    }
}

extension VtIntArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Int32]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Int32] {
        var result = [Int32]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Int32) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtFloatArray Extensions

extension VtFloatArray: CustomStringConvertible {
    public var description: String {
        return "VtFloatArray(count: \(size()))"
    }
}

extension VtFloatArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Float]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Float] {
        var result = [Float]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Float) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtDoubleArray Extensions

extension VtDoubleArray: CustomStringConvertible {
    public var description: String {
        return "VtDoubleArray(count: \(size()))"
    }
}

extension VtDoubleArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Double]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Double] {
        var result = [Double]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Double) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtVec2fArray Extensions

extension VtVec2fArray: CustomStringConvertible {
    public var description: String {
        return "VtVec2fArray(count: \(size()))"
    }
}

extension VtVec2fArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Vec2f]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Vec2f] {
        var result = [Vec2f]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Vec2f) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtVec3fArray Extensions

extension VtVec3fArray: CustomStringConvertible {
    public var description: String {
        return "VtVec3fArray(count: \(size()))"
    }
}

extension VtVec3fArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Vec3f]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Vec3f] {
        var result = [Vec3f]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Vec3f) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtVec4fArray Extensions

extension VtVec4fArray: CustomStringConvertible {
    public var description: String {
        return "VtVec4fArray(count: \(size()))"
    }
}

extension VtVec4fArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Vec4f]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Vec4f] {
        var result = [Vec4f]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Vec4f) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtMatrix4dArray Extensions

extension VtMatrix4dArray: CustomStringConvertible {
    public var description: String {
        return "VtMatrix4dArray(count: \(size()))"
    }
}

extension VtMatrix4dArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array
    public init(_ elements: [Matrix4d]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            self[i] = value
        }
    }

    /// Convert to Swift array
    public func toArray() -> [Matrix4d] {
        var result = [Matrix4d]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(self[i])
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: Matrix4d) {
        push_back(value)
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }
}

// MARK: - VtTokenArray Extensions

extension VtTokenArray: CustomStringConvertible {
    public var description: String {
        return "VtTokenArray(count: \(size()))"
    }
}

extension VtTokenArray {
    /// The number of elements in the array
    public var count: Int { Int(size()) }

    /// Whether the array is empty
    public var isEmpty: Bool { empty() }

    /// Create an array with elements from a Swift array of TfTokens
    public init(_ elements: [TfToken]) {
        self.init(elements.count)
        for (i, value) in elements.enumerated() {
            Set(i, value)
        }
    }

    /// Create an array with elements from a Swift array of strings
    public init(strings: [String]) {
        self.init(strings.count)
        for (i, value) in strings.enumerated() {
            Set(i, TfToken(std.string(value)))
        }
    }

    /// Convert to Swift array
    public func toArray() -> [TfToken] {
        var result = [TfToken]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(Get(i))
        }
        return result
    }

    /// Convert to Swift array of strings
    public func toStringArray() -> [String] {
        var result = [String]()
        result.reserveCapacity(count)
        for i in 0..<count {
            result.append(String(Get(i).GetString()))
        }
        return result
    }

    /// Append an element
    public mutating func append(_ value: TfToken) {
        push_back(value)
    }

    /// Append a string as token
    public mutating func append(_ string: String) {
        push_back(TfToken(std.string(string)))
    }

    /// Remove all elements
    public mutating func removeAll() {
        clear()
    }

    /// Get element at index
    public subscript(index: Int) -> TfToken {
        get { Get(index) }
        set { Set(index, newValue) }
    }
}

// MARK: - VtValue Extensions

extension VtValue: CustomStringConvertible {
    public var description: String {
        if IsEmpty() {
            return "VtValue(empty)"
        }
        return "VtValue(holding value)"
    }
}

extension VtValue {
    /// Whether the value is empty
    public var isEmpty: Bool { IsEmpty() }

    /// Get the value as Bool (crashes if wrong type)
    public var boolValue: Bool { GetBool() }

    /// Get the value as Int (crashes if wrong type)
    public var intValue: Int32 { GetInt() }

    /// Get the value as Float (crashes if wrong type)
    public var floatValue: Float { GetFloat() }

    /// Get the value as Double (crashes if wrong type)
    public var doubleValue: Double { GetDouble() }

    /// Get the value as String (crashes if wrong type)
    public var stringValue: String { String(GetString()) }

    /// Create a VtValue holding a string
    public init(string: String) {
        self.init(std.string(string))
    }
}
