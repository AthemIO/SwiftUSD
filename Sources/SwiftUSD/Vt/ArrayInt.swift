// ArrayInt.swift - VtArray<int> wrapper
// Mirrors: pxr/base/vt/array.h

import OpenUSDInterop

// MARK: - ArrayInt

/// A typed array of integers with copy-on-write semantics.
///
/// `ArrayInt` provides efficient storage for integer arrays used in USD attributes.
/// It uses copy-on-write semantics internally for efficient copying.
///
/// Mirrors `pxr::VtArray<int>` from the USD C++ API.
///
/// ## Example Usage
///
/// ```swift
/// // Create from Swift array
/// let array = try ArrayInt([1, 2, 3, 4, 5])
///
/// // Access elements
/// let first = array[0]  // 1
///
/// // Modify elements
/// array[0] = 10
///
/// // Append elements
/// array.append(6)
/// ```
public final class ArrayInt: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtArrayIntRef

    // MARK: - Initialization

    /// Creates an empty integer array.
    ///
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init() throws {
        guard let ref = VtArrayInt_Create() else {
            throw ArrayError.creationFailed("Failed to create empty ArrayInt")
        }
        self.handle = ref
    }

    /// Creates an integer array with the specified size.
    ///
    /// All elements are initialized to 0.
    ///
    /// - Parameter size: The initial size of the array.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(size: Int) throws {
        guard let ref = VtArrayInt_CreateWithSize(size) else {
            throw ArrayError.creationFailed("Failed to create ArrayInt with size \(size)")
        }
        self.handle = ref
    }

    /// Creates an integer array from a Swift array.
    ///
    /// - Parameter elements: The elements to initialize the array with.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(_ elements: [Int32]) throws {
        guard let ref = elements.withUnsafeBufferPointer({ buffer in
            VtArrayInt_CreateFromData(buffer.baseAddress, buffer.count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayInt from elements")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: VtArrayIntRef) {
        self.handle = handle
    }

    deinit {
        VtArrayInt_Release(handle)
    }

    // MARK: - Properties

    /// The number of elements in the array.
    public var count: Int {
        Int(VtArrayInt_GetSize(handle))
    }

    /// Returns `true` if the array is empty.
    public var isEmpty: Bool {
        VtArrayInt_IsEmpty(handle)
    }

    /// The current capacity of the array.
    public var capacity: Int {
        Int(VtArrayInt_GetCapacity(handle))
    }

    // MARK: - Element Access

    /// Accesses the element at the specified index.
    ///
    /// - Parameter index: The index of the element to access.
    /// - Returns: The element at the specified index, or 0 if out of bounds.
    public subscript(index: Int) -> Int32 {
        get {
            VtArrayInt_GetElement(handle, index)
        }
        set {
            VtArrayInt_SetElement(handle, index, newValue)
        }
    }

    /// Returns all elements as a Swift array.
    public var elements: [Int32] {
        let size = count
        if size == 0 { return [] }
        var buffer = [Int32](repeating: 0, count: size)
        buffer.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayInt_GetElements(handle, ptr.baseAddress, size)
        }
        return buffer
    }

    // MARK: - Modification

    /// Reserves capacity for at least the specified number of elements.
    ///
    /// - Parameter minimumCapacity: The minimum number of elements to reserve space for.
    public func reserveCapacity(_ minimumCapacity: Int) {
        VtArrayInt_Reserve(handle, minimumCapacity)
    }

    /// Resizes the array to the specified size.
    ///
    /// New elements are initialized to 0.
    ///
    /// - Parameter newSize: The new size of the array.
    public func resize(_ newSize: Int) {
        VtArrayInt_Resize(handle, newSize)
    }

    /// Appends an element to the end of the array.
    ///
    /// - Parameter element: The element to append.
    public func append(_ element: Int32) {
        VtArrayInt_PushBack(handle, element)
    }

    /// Removes the last element from the array.
    public func removeLast() {
        VtArrayInt_PopBack(handle)
    }

    /// Removes all elements from the array.
    public func removeAll() {
        VtArrayInt_Clear(handle)
    }

    /// Replaces all elements with the contents of a Swift array.
    ///
    /// - Parameter elements: The new elements.
    public func assign(_ elements: [Int32]) {
        elements.withUnsafeBufferPointer { buffer in
            VtArrayInt_Assign(handle, buffer.baseAddress, buffer.count)
        }
    }

    // MARK: - Copying

    /// Creates an independent copy of this array.
    ///
    /// - Returns: A new array with the same elements.
    /// - Throws: `ArrayError.creationFailed` if the copy cannot be created.
    public func copy() throws -> ArrayInt {
        guard let ref = VtArrayInt_Copy(handle) else {
            throw ArrayError.creationFailed("Failed to copy ArrayInt")
        }
        return ArrayInt(handle: ref)
    }
}

// MARK: - Equatable

extension ArrayInt: Equatable {

    public static func == (lhs: ArrayInt, rhs: ArrayInt) -> Bool {
        VtArrayInt_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Hashable

extension ArrayInt: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(VtArrayInt_Hash(handle))
    }
}

// MARK: - Collection

extension ArrayInt: Collection {

    public var startIndex: Int { 0 }
    public var endIndex: Int { count }

    public func index(after i: Int) -> Int {
        i + 1
    }
}

// MARK: - CustomStringConvertible

extension ArrayInt: CustomStringConvertible {

    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "ArrayInt([])"
        }
        if elems.count <= 10 {
            return "ArrayInt([\(elems.map { String($0) }.joined(separator: ", "))])"
        }
        let first5 = elems.prefix(5).map { String($0) }.joined(separator: ", ")
        return "ArrayInt([\(first5), ... (\(elems.count) elements)])"
    }
}

// MARK: - CustomDebugStringConvertible

extension ArrayInt: CustomDebugStringConvertible {

    public var debugDescription: String {
        "ArrayInt(count: \(count), capacity: \(capacity))"
    }
}

// MARK: - ExpressibleByArrayLiteral

extension ArrayInt: ExpressibleByArrayLiteral {

    public convenience init(arrayLiteral elements: Int32...) {
        do {
            try self.init(elements)
        } catch {
            fatalError("Failed to create ArrayInt from array literal: \(error)")
        }
    }
}
