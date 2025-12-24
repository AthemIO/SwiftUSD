// Dictionary.swift - VtDictionary wrapper
// Mirrors: pxr/base/vt/dictionary.h

import OpenUSDInterop

// MARK: - VtDict

/// A dictionary with string keys and type-erased values.
///
/// `VtDict` provides key-value storage for USD metadata and custom properties.
/// Values can be any type supported by `Value`.
///
/// Mirrors `pxr::VtDictionary` from the USD C++ API.
///
/// ## Example Usage
///
/// ```swift
/// let dict = try VtDict()
/// try dict.setValue(Value(42), forKey: "count")
/// try dict.setValue(Value("hello"), forKey: "message")
///
/// if let value = dict.getValue(forKey: "count") {
///     print("count: \(value.getInt())")
/// }
/// ```
public final class VtDict: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtDictionaryRef

    // MARK: - Initialization

    /// Creates an empty dictionary.
    ///
    /// - Throws: `DictionaryError.creationFailed` if the dictionary cannot be created.
    public init() throws {
        guard let ref = VtDictionary_Create() else {
            throw DictionaryError.creationFailed("Failed to create empty VtDict")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: VtDictionaryRef) {
        self.handle = handle
    }

    deinit {
        VtDictionary_Release(handle)
    }

    // MARK: - Properties

    /// The number of key-value pairs in the dictionary.
    public var count: Int {
        Int(VtDictionary_GetSize(handle))
    }

    /// Returns `true` if the dictionary is empty.
    public var isEmpty: Bool {
        VtDictionary_IsEmpty(handle)
    }

    /// All keys in the dictionary.
    public var keys: [String] {
        var count: Int = 0
        guard let keysPtr = VtDictionary_GetKeys(handle, &count) else {
            return []
        }
        defer { VtDictionary_FreeKeys(keysPtr, count) }

        var result = [String]()
        result.reserveCapacity(count)
        for i in 0..<count {
            if let keyPtr = keysPtr[i] {
                result.append(String(cString: keyPtr))
            }
        }
        return result
    }

    // MARK: - Access

    /// Checks if the dictionary contains the specified key.
    ///
    /// - Parameter key: The key to check.
    /// - Returns: `true` if the key exists in the dictionary.
    public func contains(key: String) -> Bool {
        VtDictionary_Contains(handle, key)
    }

    /// Gets the value for the specified key.
    ///
    /// - Parameter key: The key to look up.
    /// - Returns: The value associated with the key, or `nil` if not found.
    public func getValue(forKey key: String) -> Value? {
        guard let ref = VtDictionary_GetValue(handle, key) else {
            return nil
        }
        return Value(handle: ref)
    }

    /// Gets the value at a path (e.g., "key1:key2:key3").
    ///
    /// - Parameter path: The colon-separated path to the value.
    /// - Returns: The value at the path, or `nil` if not found.
    public func getValue(atPath path: String) -> Value? {
        guard let ref = VtDictionary_GetValueAtPath(handle, path) else {
            return nil
        }
        return Value(handle: ref)
    }

    /// Subscript access to values by key.
    ///
    /// - Parameter key: The key to look up.
    /// - Returns: The value associated with the key, or `nil` if not found.
    public subscript(key: String) -> Value? {
        get {
            getValue(forKey: key)
        }
        set {
            if let value = newValue {
                VtDictionary_SetValue(handle, key, value.handle)
            } else {
                _ = VtDictionary_Erase(handle, key)
            }
        }
    }

    // MARK: - Modification

    /// Sets the value for the specified key.
    ///
    /// - Parameters:
    ///   - value: The value to set.
    ///   - key: The key to associate with the value.
    public func setValue(_ value: Value, forKey key: String) {
        VtDictionary_SetValue(handle, key, value.handle)
    }

    /// Sets the value at a path (creates intermediate dictionaries as needed).
    ///
    /// - Parameters:
    ///   - value: The value to set.
    ///   - path: The colon-separated path.
    public func setValue(_ value: Value, atPath path: String) {
        VtDictionary_SetValueAtPath(handle, path, value.handle)
    }

    /// Removes the value for the specified key.
    ///
    /// - Parameter key: The key to remove.
    /// - Returns: `true` if the key was found and removed.
    @discardableResult
    public func removeValue(forKey key: String) -> Bool {
        VtDictionary_Erase(handle, key)
    }

    /// Removes the value at a path.
    ///
    /// - Parameter path: The colon-separated path.
    /// - Returns: `true` if the value was found and removed.
    @discardableResult
    public func removeValue(atPath path: String) -> Bool {
        VtDictionary_EraseAtPath(handle, path)
    }

    /// Removes all entries from the dictionary.
    public func removeAll() {
        VtDictionary_Clear(handle)
    }

    // MARK: - Composition

    /// Returns a new dictionary that is the composition of this dictionary over another.
    ///
    /// Values from this dictionary take precedence over values from `weak`.
    ///
    /// - Parameter weak: The weaker dictionary to compose with.
    /// - Returns: A new dictionary with composed values.
    /// - Throws: `DictionaryError.creationFailed` if the operation fails.
    public func over(_ weak: VtDict) throws -> VtDict {
        guard let ref = VtDictionary_Over(handle, weak.handle) else {
            throw DictionaryError.creationFailed("Failed to compose dictionaries")
        }
        return VtDict(handle: ref)
    }

    // MARK: - Copying

    /// Creates an independent copy of this dictionary.
    ///
    /// - Returns: A new dictionary with the same contents.
    /// - Throws: `DictionaryError.creationFailed` if the copy cannot be created.
    public func copy() throws -> VtDict {
        guard let ref = VtDictionary_Copy(handle) else {
            throw DictionaryError.creationFailed("Failed to copy VtDict")
        }
        return VtDict(handle: ref)
    }
}

// MARK: - Equatable

extension VtDict: Equatable {

    public static func == (lhs: VtDict, rhs: VtDict) -> Bool {
        VtDictionary_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Hashable

extension VtDict: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(VtDictionary_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension VtDict: CustomStringConvertible {

    public var description: String {
        let k = keys
        if k.isEmpty {
            return "VtDict({})"
        }
        if k.count <= 5 {
            let pairs = k.map { key -> String in
                if let value = getValue(forKey: key) {
                    return "\"\(key)\": \(value)"
                }
                return "\"\(key)\": nil"
            }
            return "VtDict({\(pairs.joined(separator: ", "))})"
        }
        return "VtDict(\(k.count) entries)"
    }
}

// MARK: - CustomDebugStringConvertible

extension VtDict: CustomDebugStringConvertible {

    public var debugDescription: String {
        "VtDict(count: \(count), keys: \(keys))"
    }
}

// MARK: - DictionaryError

/// Errors that can occur when working with VtDict.
public enum DictionaryError: Error, CustomStringConvertible {

    /// Dictionary creation failed.
    case creationFailed(String)

    /// Key not found.
    case keyNotFound(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Dictionary creation failed: \(message)"
        case .keyNotFound(let key):
            return "Key not found: \(key)"
        }
    }
}
