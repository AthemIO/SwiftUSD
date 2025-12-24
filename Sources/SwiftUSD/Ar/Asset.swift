// Asset.swift - ArAsset wrapper
// Mirrors: pxr/usd/ar/asset.h

import Foundation
import OpenUSDInterop

// MARK: - Asset

/// A read-only asset that provides access to asset contents.
///
/// Assets represent resolved file data that can be read. They are
/// typically obtained by opening a resolved path using ``open(_:)``.
///
/// Example usage:
/// ```swift
/// if let resolvedPath = Resolver.resolve("model.usd"),
///    let asset = try? Asset.open(resolvedPath) {
///     let size = asset.size
///     print("Asset size: \(size) bytes")
///
///     // Read entire contents
///     if let data = asset.data {
///         // Process data...
///     }
///
///     // Or read specific range
///     var buffer = [UInt8](repeating: 0, count: 1024)
///     let bytesRead = asset.read(into: &buffer, count: 1024, offset: 0)
/// }
/// ```
///
/// Mirrors `pxr::ArAsset` from the USD C++ API.
public final class Asset: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    private let handle: ArAssetRef

    // MARK: - Initialization

    /// Opens an asset at the given resolved path.
    ///
    /// - Parameter resolvedPath: The resolved path to open.
    /// - Returns: The opened asset, or `nil` if the asset cannot be opened.
    public static func open(_ resolvedPath: ResolvedPath) -> Asset? {
        guard let ref = ArAsset_Open(resolvedPath.handle) else {
            return nil
        }
        return Asset(handle: ref)
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    private init(handle: ArAssetRef) {
        self.handle = handle
    }

    deinit {
        ArAsset_Release(handle)
    }

    // MARK: - Properties

    /// The size of the asset in bytes.
    public var size: Int {
        ArAsset_GetSize(handle)
    }

    /// Returns `true` if the asset is empty (zero bytes).
    public var isEmpty: Bool {
        size == 0
    }

    /// The entire asset contents as `Data`.
    ///
    /// This property reads the entire asset into memory. For large assets,
    /// consider using ``read(into:count:offset:)`` to read in chunks.
    ///
    /// - Returns: The asset data, or `nil` if reading fails.
    public var data: Data? {
        var outSize: Int = 0
        guard let buffer = ArAsset_GetBuffer(handle, &outSize), outSize > 0 else {
            return nil
        }
        defer { ArAsset_FreeBuffer(buffer) }
        return Data(bytes: buffer, count: outSize)
    }

    /// The entire asset contents as a String (assuming UTF-8 encoding).
    ///
    /// - Returns: The asset contents as a string, or `nil` if reading fails
    ///   or the content is not valid UTF-8.
    public var string: String? {
        guard let data = data else { return nil }
        return String(data: data, encoding: .utf8)
    }

    // MARK: - Reading

    /// Reads bytes from the asset into a buffer.
    ///
    /// - Parameters:
    ///   - buffer: The buffer to read into.
    ///   - count: The number of bytes to read.
    ///   - offset: The byte offset to start reading from.
    /// - Returns: The number of bytes actually read.
    public func read(into buffer: UnsafeMutableRawPointer, count: Int, offset: Int = 0) -> Int {
        ArAsset_Read(handle, buffer, count, offset)
    }

    /// Reads bytes from the asset into a byte array.
    ///
    /// - Parameters:
    ///   - buffer: The byte array to read into.
    ///   - count: The maximum number of bytes to read.
    ///   - offset: The byte offset to start reading from.
    /// - Returns: The number of bytes actually read.
    public func read(into buffer: inout [UInt8], count: Int, offset: Int = 0) -> Int {
        let bufferCount = buffer.count
        return buffer.withUnsafeMutableBytes { bufferPtr in
            guard let baseAddress = bufferPtr.baseAddress else { return 0 }
            let bytesToRead = min(count, bufferCount)
            return ArAsset_Read(handle, baseAddress, bytesToRead, offset)
        }
    }

    /// Reads a range of bytes from the asset as Data.
    ///
    /// - Parameters:
    ///   - range: The byte range to read.
    /// - Returns: The data read, or `nil` if reading fails.
    public func read(range: Range<Int>) -> Data? {
        let count = range.count
        guard count > 0 else { return Data() }

        var buffer = [UInt8](repeating: 0, count: count)
        let bytesRead = read(into: &buffer, count: count, offset: range.lowerBound)
        guard bytesRead > 0 else { return nil }

        return Data(buffer.prefix(bytesRead))
    }
}

// MARK: - CustomStringConvertible

extension Asset: CustomStringConvertible {

    public var description: String {
        "Asset(size: \(size))"
    }
}

// MARK: - CustomDebugStringConvertible

extension Asset: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Asset(size: \(size) bytes)"
    }
}
