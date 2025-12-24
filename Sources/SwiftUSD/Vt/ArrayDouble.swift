// ArrayDouble.swift - VtArray<double> wrapper
// Mirrors: pxr/base/vt/array.h

import OpenUSDInterop

// MARK: - ArrayDouble

/// A typed array of double-precision floats with copy-on-write semantics.
///
/// `ArrayDouble` provides efficient storage for double arrays used in USD attributes.
/// It uses copy-on-write semantics internally for efficient copying.
///
/// Mirrors `pxr::VtArray<double>` from the USD C++ API.
public final class ArrayDouble: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtArrayDoubleRef

    // MARK: - Initialization

    /// Creates an empty double array.
    ///
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init() throws {
        guard let ref = VtArrayDouble_Create() else {
            throw ArrayError.creationFailed("Failed to create empty ArrayDouble")
        }
        self.handle = ref
    }

    /// Creates a double array with the specified size.
    ///
    /// - Parameter size: The initial size of the array.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(size: Int) throws {
        guard let ref = VtArrayDouble_CreateWithSize(size) else {
            throw ArrayError.creationFailed("Failed to create ArrayDouble with size \(size)")
        }
        self.handle = ref
    }

    /// Creates a double array from a Swift array.
    ///
    /// - Parameter elements: The elements to initialize the array with.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(_ elements: [Double]) throws {
        guard let ref = elements.withUnsafeBufferPointer({ buffer in
            VtArrayDouble_CreateFromData(buffer.baseAddress, buffer.count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayDouble from elements")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    internal init(handle: VtArrayDoubleRef) {
        self.handle = handle
    }

    deinit {
        VtArrayDouble_Release(handle)
    }

    // MARK: - Properties

    /// The number of elements in the array.
    public var count: Int {
        Int(VtArrayDouble_GetSize(handle))
    }

    /// Returns `true` if the array is empty.
    public var isEmpty: Bool {
        VtArrayDouble_IsEmpty(handle)
    }

    /// The current capacity of the array.
    public var capacity: Int {
        Int(VtArrayDouble_GetCapacity(handle))
    }

    // MARK: - Element Access

    /// Accesses the element at the specified index.
    public subscript(index: Int) -> Double {
        get {
            VtArrayDouble_GetElement(handle, index)
        }
        set {
            VtArrayDouble_SetElement(handle, index, newValue)
        }
    }

    /// Returns all elements as a Swift array.
    public var elements: [Double] {
        let size = count
        if size == 0 { return [] }
        var buffer = [Double](repeating: 0, count: size)
        buffer.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayDouble_GetElements(handle, ptr.baseAddress, size)
        }
        return buffer
    }

    // MARK: - Modification

    /// Reserves capacity for at least the specified number of elements.
    public func reserveCapacity(_ minimumCapacity: Int) {
        VtArrayDouble_Reserve(handle, minimumCapacity)
    }

    /// Resizes the array to the specified size.
    public func resize(_ newSize: Int) {
        VtArrayDouble_Resize(handle, newSize)
    }

    /// Appends an element to the end of the array.
    public func append(_ element: Double) {
        VtArrayDouble_PushBack(handle, element)
    }

    /// Removes the last element from the array.
    public func removeLast() {
        VtArrayDouble_PopBack(handle)
    }

    /// Removes all elements from the array.
    public func removeAll() {
        VtArrayDouble_Clear(handle)
    }

    /// Replaces all elements with the contents of a Swift array.
    public func assign(_ elements: [Double]) {
        elements.withUnsafeBufferPointer { buffer in
            VtArrayDouble_Assign(handle, buffer.baseAddress, buffer.count)
        }
    }

    // MARK: - Copying

    /// Creates an independent copy of this array.
    public func copy() throws -> ArrayDouble {
        guard let ref = VtArrayDouble_Copy(handle) else {
            throw ArrayError.creationFailed("Failed to copy ArrayDouble")
        }
        return ArrayDouble(handle: ref)
    }
}

// MARK: - Equatable

extension ArrayDouble: Equatable {

    public static func == (lhs: ArrayDouble, rhs: ArrayDouble) -> Bool {
        VtArrayDouble_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Hashable

extension ArrayDouble: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(VtArrayDouble_Hash(handle))
    }
}

// MARK: - Collection

extension ArrayDouble: Collection {

    public var startIndex: Int { 0 }
    public var endIndex: Int { count }

    public func index(after i: Int) -> Int {
        i + 1
    }
}

// MARK: - CustomStringConvertible

extension ArrayDouble: CustomStringConvertible {

    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "ArrayDouble([])"
        }
        if elems.count <= 10 {
            return "ArrayDouble([\(elems.map { String($0) }.joined(separator: ", "))])"
        }
        let first5 = elems.prefix(5).map { String($0) }.joined(separator: ", ")
        return "ArrayDouble([\(first5), ... (\(elems.count) elements)])"
    }
}

// MARK: - ExpressibleByArrayLiteral

extension ArrayDouble: ExpressibleByArrayLiteral {

    public convenience init(arrayLiteral elements: Double...) {
        do {
            try self.init(elements)
        } catch {
            fatalError("Failed to create ArrayDouble from array literal: \(error)")
        }
    }
}
