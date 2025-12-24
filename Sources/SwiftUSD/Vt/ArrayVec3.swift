// ArrayVec3.swift - VtArray<GfVec3f> and VtArray<GfVec3d> wrappers
// Mirrors: pxr/base/vt/array.h

import OpenUSDInterop

// MARK: - ArrayVec3f

/// A typed array of 3D float vectors with copy-on-write semantics.
///
/// `ArrayVec3f` provides efficient storage for Vec3f arrays used in USD attributes,
/// commonly used for positions, normals, and other 3D data.
///
/// Mirrors `pxr::VtArray<GfVec3f>` from the USD C++ API.
public final class ArrayVec3f: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtArrayVec3fRef

    // MARK: - Initialization

    /// Creates an empty Vec3f array.
    ///
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init() throws {
        guard let ref = VtArrayVec3f_Create() else {
            throw ArrayError.creationFailed("Failed to create empty ArrayVec3f")
        }
        self.handle = ref
    }

    /// Creates a Vec3f array with the specified size.
    ///
    /// - Parameter size: The initial size of the array.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(size: Int) throws {
        guard let ref = VtArrayVec3f_CreateWithSize(size) else {
            throw ArrayError.creationFailed("Failed to create ArrayVec3f with size \(size)")
        }
        self.handle = ref
    }

    /// Creates a Vec3f array from flat float data (x,y,z,x,y,z,...).
    ///
    /// - Parameter flatData: The flat float data (must have count * 3 elements).
    /// - Parameter count: The number of Vec3f elements.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(flatData: [Float], count: Int) throws {
        guard flatData.count >= count * 3 else {
            throw ArrayError.creationFailed("Insufficient data: need \(count * 3) floats, got \(flatData.count)")
        }
        guard let ref = flatData.withUnsafeBufferPointer({ buffer in
            VtArrayVec3f_CreateFromData(buffer.baseAddress, count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayVec3f from data")
        }
        self.handle = ref
    }

    /// Creates a Vec3f array from an array of tuples.
    ///
    /// - Parameter elements: Array of (x, y, z) tuples.
    /// - Throws: `ArrayError.creationFailed` if the array cannot be created.
    public init(_ elements: [(Float, Float, Float)]) throws {
        var flatData = [Float]()
        flatData.reserveCapacity(elements.count * 3)
        for (x, y, z) in elements {
            flatData.append(x)
            flatData.append(y)
            flatData.append(z)
        }
        guard let ref = flatData.withUnsafeBufferPointer({ buffer in
            VtArrayVec3f_CreateFromData(buffer.baseAddress, elements.count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayVec3f from elements")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    internal init(handle: VtArrayVec3fRef) {
        self.handle = handle
    }

    deinit {
        VtArrayVec3f_Release(handle)
    }

    // MARK: - Properties

    /// The number of Vec3f elements in the array.
    public var count: Int {
        Int(VtArrayVec3f_GetSize(handle))
    }

    /// Returns `true` if the array is empty.
    public var isEmpty: Bool {
        VtArrayVec3f_IsEmpty(handle)
    }

    // MARK: - Element Access

    /// Gets the element at the specified index.
    ///
    /// - Parameter index: The index of the element.
    /// - Returns: The (x, y, z) components of the vector.
    public func getElement(at index: Int) -> (x: Float, y: Float, z: Float) {
        var x: Float = 0, y: Float = 0, z: Float = 0
        VtArrayVec3f_GetElement(handle, index, &x, &y, &z)
        return (x, y, z)
    }

    /// Sets the element at the specified index.
    ///
    /// - Parameters:
    ///   - x: The x component.
    ///   - y: The y component.
    ///   - z: The z component.
    ///   - index: The index of the element to set.
    public func setElement(x: Float, y: Float, z: Float, at index: Int) {
        VtArrayVec3f_SetElement(handle, index, x, y, z)
    }

    /// Returns all elements as an array of tuples.
    public var elements: [(x: Float, y: Float, z: Float)] {
        let size = count
        if size == 0 { return [] }
        var flatData = [Float](repeating: 0, count: size * 3)
        flatData.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayVec3f_GetElements(handle, ptr.baseAddress, size)
        }
        var result = [(x: Float, y: Float, z: Float)]()
        result.reserveCapacity(size)
        for i in 0..<size {
            result.append((flatData[i*3], flatData[i*3+1], flatData[i*3+2]))
        }
        return result
    }

    /// Returns all elements as flat float data (x,y,z,x,y,z,...).
    public var flatData: [Float] {
        let size = count
        if size == 0 { return [] }
        var data = [Float](repeating: 0, count: size * 3)
        data.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayVec3f_GetElements(handle, ptr.baseAddress, size)
        }
        return data
    }

    // MARK: - Modification

    /// Reserves capacity for at least the specified number of elements.
    public func reserveCapacity(_ minimumCapacity: Int) {
        VtArrayVec3f_Reserve(handle, minimumCapacity)
    }

    /// Resizes the array to the specified size.
    public func resize(_ newSize: Int) {
        VtArrayVec3f_Resize(handle, newSize)
    }

    /// Appends an element to the end of the array.
    public func append(x: Float, y: Float, z: Float) {
        VtArrayVec3f_PushBack(handle, x, y, z)
    }

    /// Removes all elements from the array.
    public func removeAll() {
        VtArrayVec3f_Clear(handle)
    }

    /// Replaces all elements with the contents of flat float data.
    public func assign(flatData: [Float], count: Int) {
        flatData.withUnsafeBufferPointer { buffer in
            VtArrayVec3f_Assign(handle, buffer.baseAddress, count)
        }
    }

    // MARK: - Copying

    /// Creates an independent copy of this array.
    public func copy() throws -> ArrayVec3f {
        guard let ref = VtArrayVec3f_Copy(handle) else {
            throw ArrayError.creationFailed("Failed to copy ArrayVec3f")
        }
        return ArrayVec3f(handle: ref)
    }
}

// MARK: - ArrayVec3f Equatable

extension ArrayVec3f: Equatable {

    public static func == (lhs: ArrayVec3f, rhs: ArrayVec3f) -> Bool {
        VtArrayVec3f_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - ArrayVec3f Hashable

extension ArrayVec3f: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(VtArrayVec3f_Hash(handle))
    }
}

// MARK: - ArrayVec3f CustomStringConvertible

extension ArrayVec3f: CustomStringConvertible {

    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "ArrayVec3f([])"
        }
        if elems.count <= 5 {
            let strs = elems.map { "(\($0.x), \($0.y), \($0.z))" }
            return "ArrayVec3f([\(strs.joined(separator: ", "))])"
        }
        return "ArrayVec3f(\(elems.count) elements)"
    }
}

// MARK: - ArrayVec3d

/// A typed array of 3D double vectors with copy-on-write semantics.
///
/// `ArrayVec3d` provides efficient storage for Vec3d arrays used in USD attributes,
/// commonly used for positions, normals, and other 3D data requiring higher precision.
///
/// Mirrors `pxr::VtArray<GfVec3d>` from the USD C++ API.
public final class ArrayVec3d: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtArrayVec3dRef

    // MARK: - Initialization

    /// Creates an empty Vec3d array.
    public init() throws {
        guard let ref = VtArrayVec3d_Create() else {
            throw ArrayError.creationFailed("Failed to create empty ArrayVec3d")
        }
        self.handle = ref
    }

    /// Creates a Vec3d array with the specified size.
    public init(size: Int) throws {
        guard let ref = VtArrayVec3d_CreateWithSize(size) else {
            throw ArrayError.creationFailed("Failed to create ArrayVec3d with size \(size)")
        }
        self.handle = ref
    }

    /// Creates a Vec3d array from flat double data.
    public init(flatData: [Double], count: Int) throws {
        guard flatData.count >= count * 3 else {
            throw ArrayError.creationFailed("Insufficient data: need \(count * 3) doubles, got \(flatData.count)")
        }
        guard let ref = flatData.withUnsafeBufferPointer({ buffer in
            VtArrayVec3d_CreateFromData(buffer.baseAddress, count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayVec3d from data")
        }
        self.handle = ref
    }

    /// Creates a Vec3d array from an array of tuples.
    public init(_ elements: [(Double, Double, Double)]) throws {
        var flatData = [Double]()
        flatData.reserveCapacity(elements.count * 3)
        for (x, y, z) in elements {
            flatData.append(x)
            flatData.append(y)
            flatData.append(z)
        }
        guard let ref = flatData.withUnsafeBufferPointer({ buffer in
            VtArrayVec3d_CreateFromData(buffer.baseAddress, elements.count)
        }) else {
            throw ArrayError.creationFailed("Failed to create ArrayVec3d from elements")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    internal init(handle: VtArrayVec3dRef) {
        self.handle = handle
    }

    deinit {
        VtArrayVec3d_Release(handle)
    }

    // MARK: - Properties

    /// The number of Vec3d elements in the array.
    public var count: Int {
        Int(VtArrayVec3d_GetSize(handle))
    }

    /// Returns `true` if the array is empty.
    public var isEmpty: Bool {
        VtArrayVec3d_IsEmpty(handle)
    }

    // MARK: - Element Access

    /// Gets the element at the specified index.
    public func getElement(at index: Int) -> (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        VtArrayVec3d_GetElement(handle, index, &x, &y, &z)
        return (x, y, z)
    }

    /// Sets the element at the specified index.
    public func setElement(x: Double, y: Double, z: Double, at index: Int) {
        VtArrayVec3d_SetElement(handle, index, x, y, z)
    }

    /// Returns all elements as an array of tuples.
    public var elements: [(x: Double, y: Double, z: Double)] {
        let size = count
        if size == 0 { return [] }
        var flatData = [Double](repeating: 0, count: size * 3)
        flatData.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayVec3d_GetElements(handle, ptr.baseAddress, size)
        }
        var result = [(x: Double, y: Double, z: Double)]()
        result.reserveCapacity(size)
        for i in 0..<size {
            result.append((flatData[i*3], flatData[i*3+1], flatData[i*3+2]))
        }
        return result
    }

    /// Returns all elements as flat double data.
    public var flatData: [Double] {
        let size = count
        if size == 0 { return [] }
        var data = [Double](repeating: 0, count: size * 3)
        data.withUnsafeMutableBufferPointer { ptr in
            _ = VtArrayVec3d_GetElements(handle, ptr.baseAddress, size)
        }
        return data
    }

    // MARK: - Modification

    /// Reserves capacity for at least the specified number of elements.
    public func reserveCapacity(_ minimumCapacity: Int) {
        VtArrayVec3d_Reserve(handle, minimumCapacity)
    }

    /// Resizes the array to the specified size.
    public func resize(_ newSize: Int) {
        VtArrayVec3d_Resize(handle, newSize)
    }

    /// Appends an element to the end of the array.
    public func append(x: Double, y: Double, z: Double) {
        VtArrayVec3d_PushBack(handle, x, y, z)
    }

    /// Removes all elements from the array.
    public func removeAll() {
        VtArrayVec3d_Clear(handle)
    }

    /// Replaces all elements with the contents of flat double data.
    public func assign(flatData: [Double], count: Int) {
        flatData.withUnsafeBufferPointer { buffer in
            VtArrayVec3d_Assign(handle, buffer.baseAddress, count)
        }
    }

    // MARK: - Copying

    /// Creates an independent copy of this array.
    public func copy() throws -> ArrayVec3d {
        guard let ref = VtArrayVec3d_Copy(handle) else {
            throw ArrayError.creationFailed("Failed to copy ArrayVec3d")
        }
        return ArrayVec3d(handle: ref)
    }
}

// MARK: - ArrayVec3d Equatable

extension ArrayVec3d: Equatable {

    public static func == (lhs: ArrayVec3d, rhs: ArrayVec3d) -> Bool {
        VtArrayVec3d_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - ArrayVec3d Hashable

extension ArrayVec3d: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(VtArrayVec3d_Hash(handle))
    }
}

// MARK: - ArrayVec3d CustomStringConvertible

extension ArrayVec3d: CustomStringConvertible {

    public var description: String {
        let elems = elements
        if elems.isEmpty {
            return "ArrayVec3d([])"
        }
        if elems.count <= 5 {
            let strs = elems.map { "(\($0.x), \($0.y), \($0.z))" }
            return "ArrayVec3d([\(strs.joined(separator: ", "))])"
        }
        return "ArrayVec3d(\(elems.count) elements)"
    }
}
