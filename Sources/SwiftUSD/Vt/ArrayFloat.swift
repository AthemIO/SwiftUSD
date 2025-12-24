// ArrayFloat.swift - VtArray<float> wrapper
// Mirrors: pxr/base/vt/array.h

import OpenUSDInterop

// MARK: - ArrayFloat

/// A typed array of single-precision floats with copy-on-write semantics.
///
/// `ArrayFloat` provides efficient storage for float arrays used in USD attributes.
/// It uses copy-on-write semantics internally for efficient copying.
///
/// Mirrors `pxr::VtArray<float>` from the USD C++ API.
public final class ArrayFloat: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtArrayFloatRef

    // MARK: - Initialization

    /// Creates an empty float array.
    ///
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init() throws {
        guard let ref = VtArrayFloat_Create() else {
            throw ArrayError.creationFailed("Failed to create empty ArrayFloat")
        }
        self.handle = ref
    }

    /// Creates a float array with the specified size.
    ///
    /// - Parameter size: The initial size of the array.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(size: Int) throws {
        guard let ref = VtArrayFloat_CreateWithSize(size) else {
            throw ArrayError.creationFailed("Failed to create ArrayFloat with size \(size)")
        }
        self.handle = ref
    }

    /// Creates a float array from a Swift array.
    ///
    /// - Parameter elements: The elements to initialize the array with.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(_ elements: [Float]) throws {
        guard let ref = elements.withUnsafeBufferPointer({ buffer in
            VtArrayFloat_CreateFromData(buffer.baseAddress, buffer.count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayFloat from elements")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    internal init(handle: VtArrayFloatRef) {
        self.handle = handle
    }

    deinit {
        VtArrayFloat_Release(handle)
    }

    // MARK: - Properties

    /// The number of elements in the array.
    public var count: Int {
        Int(VtArrayFloat_GetSize(handle))
    }

    /// Returns `true` if the array is empty.
    public var isEmpty: Bool {
        VtArrayFloat_IsEmpty(handle)
    }

    /// The current capacity of the array.
    public var capacity: Int {
        Int(VtArrayFloat_GetCapacity(handle))
    }

    // MARK: - Element Access

    /// Accesses the element at the specified index.
    public subscript(index: Int) -> Float {
        get {
            VtArrayFloat_GetElement(handle, index)
        }
        set {
            VtArrayFloat_SetElement(handle, index, newValue)
        }
    }

    /// Returns all elements as a Swift array.
    public var elements: [Float] {
        let size = count
        if size == 0 { return [] }
        var buffer = [Float](repeating: 0, count: size)
        buffer.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayFloat_GetElements(handle, ptr.baseAddress, size)
        }
        return buffer
    }

    // MARK: - Modification

    /// Reserves capacity for at least the specified number of elements.
    public func reserveCapacity(_ minimumCapacity: Int) {
        VtArrayFloat_Reserve(handle, minimumCapacity)
    }

    /// Resizes the array to the specified size.
    public func resize(_ newSize: Int) {
        VtArrayFloat_Resize(handle, newSize)
    }

    /// Appends an element to the end of the array.
    public func append(_ element: Float) {
        VtArrayFloat_PushBack(handle, element)
    }

    /// Removes the last element from the array.
    public func removeLast() {
        VtArrayFloat_PopBack(handle)
    }

    /// Removes all elements from the array.
    public func removeAll() {
        VtArrayFloat_Clear(handle)
    }

    /// Replaces all elements with the contents of a Swift array.
    public func assign(_ elements: [Float]) {
        elements.withUnsafeBufferPointer { buffer in
            VtArrayFloat_Assign(handle, buffer.baseAddress, buffer.count)
        }
    }

    // MARK: - Copying

    /// Creates an independent copy of this array.
    public func copy() throws -> ArrayFloat {
        guard let ref = VtArrayFloat_Copy(handle) else {
            throw ArrayError.creationFailed("Failed to copy ArrayFloat")
        }
        return ArrayFloat(handle: ref)
    }
}

// MARK: - Equatable

extension ArrayFloat: Equatable {

    public static func == (lhs: ArrayFloat, rhs: ArrayFloat) -> Bool {
        VtArrayFloat_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Hashable

extension ArrayFloat: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(VtArrayFloat_Hash(handle))
    }
}

// MARK: - Collection

extension ArrayFloat: Collection {

    public var startIndex: Int { 0 }
    public var endIndex: Int { count }

    public func index(after i: Int) -> Int {
        i + 1
    }
}

// MARK: - CustomStringConvertible

extension ArrayFloat: CustomStringConvertible {

    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "ArrayFloat([])"
        }
        if elems.count <= 10 {
            return "ArrayFloat([\(elems.map { String($0) }.joined(separator: ", "))])"
        }
        let first5 = elems.prefix(5).map { String($0) }.joined(separator: ", ")
        return "ArrayFloat([\(first5), ... (\(elems.count) elements)])"
    }
}

// MARK: - ExpressibleByArrayLiteral

extension ArrayFloat: ExpressibleByArrayLiteral {

    public convenience init(arrayLiteral elements: Float...) {
        do {
            try self.init(elements)
        } catch {
            fatalError("Failed to create ArrayFloat from array literal: \(error)")
        }
    }
}
