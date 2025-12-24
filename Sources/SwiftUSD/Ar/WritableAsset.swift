// WritableAsset.swift - ArWritableAsset wrapper
// Mirrors: pxr/usd/ar/writableAsset.h

import Foundation
import OpenUSDInterop

// MARK: - WriteMode

/// The mode for opening a writable asset.
public enum WriteMode: Sendable {
    /// Open for in-place updates to an existing file.
    ///
    /// The file is modified directly. If the write fails, the file
    /// may be left in a partially modified state.
    case update

    /// Open for replacement (atomic write).
    ///
    /// Writes go to a temporary file which is then atomically renamed
    /// to the target path when closed. This ensures the original file
    /// is not corrupted if the write fails.
    case replace

    /// Converts to the C enum value.
    internal var cValue: ArWriteMode {
        switch self {
        case .update: return AR_WRITE_MODE_UPDATE
        case .replace: return AR_WRITE_MODE_REPLACE
        }
    }
}

// MARK: - WritableAsset

/// A writable asset that provides write access to asset contents.
///
/// Writable assets allow you to write data to a resolved path. They
/// support two modes: update (in-place modification) and replace
/// (atomic replacement via temporary file).
///
/// Example usage:
/// ```swift
/// if let resolvedPath = Resolver.resolveForNewAsset("output.txt"),
///    let asset = try? WritableAsset.open(resolvedPath, mode: .replace) {
///     let data = "Hello, World!".data(using: .utf8)!
///     let bytesWritten = asset.write(data, offset: 0)
///     let success = asset.close()
/// }
/// ```
///
/// - Important: You must call ``close()`` to finalize the write.
///   For replace mode, this performs the atomic rename. If you don't
///   call close, the temporary file may be left behind.
///
/// Mirrors `pxr::ArWritableAsset` from the USD C++ API.
public final class WritableAsset: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    private let handle: ArWritableAssetRef

    /// Whether the asset has been closed.
    private var isClosed = false

    // MARK: - Initialization

    /// Opens a writable asset at the given resolved path.
    ///
    /// - Parameters:
    ///   - resolvedPath: The resolved path to open for writing.
    ///   - mode: The write mode (update or replace).
    /// - Returns: The opened writable asset, or `nil` if opening fails.
    public static func open(_ resolvedPath: ResolvedPath, mode: WriteMode = .update) -> WritableAsset? {
        guard let ref = ArWritableAsset_Open(resolvedPath.handle, mode.cValue) else {
            return nil
        }
        return WritableAsset(handle: ref)
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    private init(handle: ArWritableAssetRef) {
        self.handle = handle
    }

    deinit {
        ArWritableAsset_Release(handle)
    }

    // MARK: - Writing

    /// Writes bytes to the asset.
    ///
    /// - Parameters:
    ///   - buffer: The buffer containing data to write.
    ///   - count: The number of bytes to write.
    ///   - offset: The byte offset to start writing at.
    /// - Returns: The number of bytes actually written.
    public func write(from buffer: UnsafeRawPointer, count: Int, offset: Int = 0) -> Int {
        guard !isClosed else { return 0 }
        return ArWritableAsset_Write(handle, buffer, count, offset)
    }

    /// Writes bytes from a byte array to the asset.
    ///
    /// - Parameters:
    ///   - buffer: The byte array containing data to write.
    ///   - offset: The byte offset to start writing at.
    /// - Returns: The number of bytes actually written.
    public func write(_ buffer: [UInt8], offset: Int = 0) -> Int {
        guard !isClosed else { return 0 }
        return buffer.withUnsafeBytes { bufferPtr in
            guard let baseAddress = bufferPtr.baseAddress else { return 0 }
            return ArWritableAsset_Write(handle, baseAddress, buffer.count, offset)
        }
    }

    /// Writes Data to the asset.
    ///
    /// - Parameters:
    ///   - data: The data to write.
    ///   - offset: The byte offset to start writing at.
    /// - Returns: The number of bytes actually written.
    public func write(_ data: Data, offset: Int = 0) -> Int {
        guard !isClosed else { return 0 }
        return data.withUnsafeBytes { bufferPtr in
            guard let baseAddress = bufferPtr.baseAddress else { return 0 }
            return ArWritableAsset_Write(handle, baseAddress, data.count, offset)
        }
    }

    /// Writes a string to the asset (as UTF-8).
    ///
    /// - Parameters:
    ///   - string: The string to write.
    ///   - offset: The byte offset to start writing at.
    /// - Returns: The number of bytes actually written.
    public func write(_ string: String, offset: Int = 0) -> Int {
        guard !isClosed else { return 0 }
        guard let data = string.data(using: .utf8) else { return 0 }
        return write(data, offset: offset)
    }

    // MARK: - Closing

    /// Closes the writable asset and finalizes the write.
    ///
    /// For ``WriteMode/replace`` mode, this performs the atomic rename
    /// from the temporary file to the target path.
    ///
    /// - Returns: `true` if the close succeeded, `false` otherwise.
    @discardableResult
    public func close() -> Bool {
        guard !isClosed else { return false }
        isClosed = true
        return ArWritableAsset_Close(handle)
    }
}

// MARK: - CustomStringConvertible

extension WritableAsset: CustomStringConvertible {

    public var description: String {
        "WritableAsset(closed: \(isClosed))"
    }
}

// MARK: - CustomDebugStringConvertible

extension WritableAsset: CustomDebugStringConvertible {

    public var debugDescription: String {
        "WritableAsset(closed: \(isClosed))"
    }
}
