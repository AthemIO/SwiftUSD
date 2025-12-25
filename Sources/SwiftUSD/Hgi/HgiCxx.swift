// HgiCxx.swift - Swift convenience wrapper for Hgi C++ interop
//
// Provides Swift-idiomatic API for the Hydra Graphics Interface.
// This module wraps the C++ Hgi types for use in Swift applications.

import USDCxx

// MARK: - Type Aliases

/// Platform-agnostic GPU interface for Hydra rendering.
public typealias Hgi = swiftusd.HgiRef

/// GPU buffer resource handle.
public typealias HgiBuffer = swiftusd.HgiBufferRef

/// GPU texture resource handle.
public typealias HgiTexture = swiftusd.HgiTextureRef

/// Device capabilities query.
public typealias HgiCapabilities = swiftusd.HgiCapabilitiesRef

/// Buffer creation descriptor.
public typealias HgiBufferDescriptor = swiftusd.HgiBufferDesc

/// Texture creation descriptor.
public typealias HgiTextureDescriptor = swiftusd.HgiTextureDesc

// MARK: - Format Type Alias

/// Pixel/buffer format enumeration.
public typealias HgiPixelFormat = swiftusd.HgiFormat

// MARK: - HgiRef Extensions

extension swiftusd.HgiRef {
    /// Create a platform-default GPU interface.
    ///
    /// On Apple platforms, this creates a Metal-backed Hgi.
    /// On other platforms, this creates Vulkan or OpenGL-backed Hgi.
    ///
    /// - Returns: A new Hgi instance, or nil if creation failed.
    @inlinable
    public static func createDefault() -> swiftusd.HgiRef? {
        return swiftusd.HgiRef.CreatePlatformDefault()
    }

    /// Get the graphics API name as a Swift String.
    @inlinable
    public var apiName: String {
        return String(cString: GetAPIName().GetText())
    }

    /// Create a buffer with the specified parameters.
    ///
    /// - Parameters:
    ///   - name: Debug name for the buffer.
    ///   - byteSize: Size of the buffer in bytes.
    ///   - usage: Usage flags for the buffer.
    /// - Returns: A new buffer handle, or nil if creation failed.
    @inlinable
    public func createBuffer(
        name: String,
        byteSize: Int,
        usage: swiftusd.HgiBufferUsage
    ) -> swiftusd.HgiBufferRef? {
        var desc = swiftusd.HgiBufferDesc()
        desc.byteSize = byteSize
        desc.usage = usage
        return CreateBuffer(desc)
    }

    /// Create a 2D texture with the specified parameters.
    ///
    /// - Parameters:
    ///   - name: Debug name for the texture.
    ///   - width: Width of the texture.
    ///   - height: Height of the texture.
    ///   - format: Pixel format.
    ///   - usage: Usage flags for the texture.
    /// - Returns: A new texture handle, or nil if creation failed.
    @inlinable
    public func createTexture2D(
        name: String,
        width: Int,
        height: Int,
        format: swiftusd.HgiFormat = .UNorm8Vec4,
        usage: UInt32 = 1 << 3 // HgiTextureUsageShaderRead
    ) -> swiftusd.HgiTextureRef? {
        var desc = swiftusd.HgiTextureDesc()
        desc.type = .Texture2D
        desc.width = Int32(width)
        desc.height = Int32(height)
        desc.depth = 1
        desc.format = format
        desc.usage = usage
        desc.mipLevels = 1
        desc.layerCount = 1
        desc.sampleCount = 1
        return CreateTexture(desc)
    }
}

// MARK: - Usage Constants
// These constants mirror the C++ constexpr values in Hgi.h

/// Buffer usage flags
public enum HgiBufferUsageFlags {
    public static let none: UInt32 = 0
    public static let uniform: UInt32 = 1 << 0
    public static let index32: UInt32 = 1 << 1
    public static let vertex: UInt32 = 1 << 2
    public static let storage: UInt32 = 1 << 3
    public static let indirect: UInt32 = 1 << 4
}

/// Texture usage flags
public enum HgiTextureUsageFlags {
    public static let none: UInt32 = 0
    public static let colorTarget: UInt32 = 1 << 0
    public static let depthTarget: UInt32 = 1 << 1
    public static let stencilTarget: UInt32 = 1 << 2
    public static let shaderRead: UInt32 = 1 << 3
    public static let shaderWrite: UInt32 = 1 << 4
}

/// Device capability flags
public enum HgiDeviceCapabilityFlags {
    public static let none: UInt32 = 0
    public static let presentation: UInt32 = 1 << 0
    public static let bindlessBuffers: UInt32 = 1 << 1
    public static let concurrentDispatch: UInt32 = 1 << 2
    public static let unifiedMemory: UInt32 = 1 << 3
    public static let builtinBarycentrics: UInt32 = 1 << 4
    public static let shaderDoublePrecision: UInt32 = 1 << 5
    public static let depthRangeMinusOneToOne: UInt32 = 1 << 6
    public static let metalTessellation: UInt32 = 1 << 7
    public static let multiDrawIndirect: UInt32 = 1 << 8
}

// MARK: - HgiBufferRef Extensions

extension swiftusd.HgiBufferRef {
    /// The size of the buffer in bytes.
    @inlinable
    public var byteSize: Int {
        return Int(GetByteSize())
    }

    /// The debug name of the buffer as a Swift String.
    @inlinable
    public var name: String {
        return String(cString: GetDebugName())
    }

    /// Whether this buffer handle is valid.
    @inlinable
    public var isValid: Bool {
        return IsValid()
    }
}

// MARK: - HgiTextureRef Extensions

extension swiftusd.HgiTextureRef {
    /// The width of the texture.
    @inlinable
    public var width: Int {
        return Int(GetWidth())
    }

    /// The height of the texture.
    @inlinable
    public var height: Int {
        return Int(GetHeight())
    }

    /// The depth of the texture.
    @inlinable
    public var depth: Int {
        return Int(GetDepth())
    }

    /// The pixel format of the texture.
    @inlinable
    public var format: swiftusd.HgiFormat {
        return GetFormat()
    }

    /// The debug name of the texture as a Swift String.
    @inlinable
    public var name: String {
        return String(cString: GetDebugName())
    }

    /// Whether this texture handle is valid.
    @inlinable
    public var isValid: Bool {
        return IsValid()
    }
}

// MARK: - HgiCapabilitiesRef Extensions

extension swiftusd.HgiCapabilitiesRef {
    /// The graphics API version.
    @inlinable
    public var apiVersion: Int {
        return Int(GetAPIVersion())
    }

    /// The shader language version.
    @inlinable
    public var shaderVersion: Int {
        return Int(GetShaderVersion())
    }

    /// Maximum size of a uniform block.
    @inlinable
    public var maxUniformBlockSize: Int {
        return Int(GetMaxUniformBlockSize())
    }

    /// Maximum size of a shader storage block.
    @inlinable
    public var maxShaderStorageBlockSize: Int {
        return Int(GetMaxShaderStorageBlockSize())
    }

    /// Check if a device capability is supported.
    ///
    /// - Parameter capability: The capability flags to check.
    /// - Returns: True if the capability is supported.
    @inlinable
    public func hasCapability(_ capability: swiftusd.HgiDeviceCapabilities) -> Bool {
        return IsSet(capability)
    }
}

// MARK: - Format Utilities

extension swiftusd.HgiFormat {
    /// The number of components in this format.
    @inlinable
    public var componentCount: Int {
        return Int(swiftusd.HgiGetComponentCount(self))
    }

    /// The byte size of a single element of this format.
    @inlinable
    public var bytesPerElement: Int {
        return Int(swiftusd.HgiGetDataSizeOfFormat(self))
    }

    /// Whether this format uses block compression.
    @inlinable
    public var isCompressed: Bool {
        return swiftusd.HgiIsCompressed(self)
    }
}
