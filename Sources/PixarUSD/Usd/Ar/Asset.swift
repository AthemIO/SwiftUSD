/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

import Ar
import CxxStdlib
import Foundation

// MARK: - ArAsset Extensions

public extension ArAsset
{
  /// Returns the size of the asset.
  var size: Int
  {
    Int(GetSize())
  }

  /// Read bytes from the asset.
  ///
  /// - Parameters:
  ///   - count: Number of bytes to read
  ///   - offset: Offset from the beginning of the asset
  /// - Returns: Data read from the asset, or nil on error
  func read(count: Int, offset: Int = 0) -> Data?
  {
    var buffer = [UInt8](repeating: 0, count: count)
    let bytesRead = buffer.withUnsafeMutableBufferPointer { ptr in
      Read(ptr.baseAddress, count, offset)
    }
    guard bytesRead > 0 else { return nil }
    return Data(buffer.prefix(bytesRead))
  }

  /// Read all bytes from the asset.
  ///
  /// - Returns: All data from the asset, or nil on error
  func readAll() -> Data?
  {
    read(count: size)
  }
}

// MARK: - ArWritableAsset Extensions

public extension ArWritableAsset
{
  /// Write data to the asset.
  ///
  /// - Parameters:
  ///   - data: The data to write
  ///   - offset: Offset from the beginning of the asset
  /// - Returns: Number of bytes written
  @discardableResult
  func write(_ data: Data, at offset: Int = 0) -> Int
  {
    data.withUnsafeBytes { ptr in
      Int(Write(ptr.baseAddress, data.count, offset))
    }
  }

  /// Close the asset, committing any written data.
  ///
  /// - Returns: True if close succeeded
  @discardableResult
  func close() -> Bool
  {
    Close()
  }
}

// MARK: - ArFilesystemAsset Extensions

public extension ArFilesystemAsset
{
  /// Get the modification timestamp for a file.
  ///
  /// - Parameter resolvedPath: The resolved path to the file
  /// - Returns: The modification timestamp
  static func getModificationTimestamp(_ resolvedPath: ArResolvedPath) -> ArTimestamp
  {
    ArFilesystemAsset.GetModificationTimestamp(resolvedPath)
  }
}

// Note: Factory methods like Open(), FromAsset(), and Create() return std::shared_ptr
// which Swift handles automatically. Use the C++ static methods directly:
//   - ArFilesystemAsset.Open(resolvedPath)
//   - ArInMemoryAsset.FromAsset(srcAsset)
//   - ArFilesystemWritableAsset.Create(resolvedPath, writeMode)
