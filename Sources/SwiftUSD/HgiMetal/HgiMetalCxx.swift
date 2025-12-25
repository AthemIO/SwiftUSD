// HgiMetalCxx.swift - Swift convenience wrapper for HgiMetal C++ interop
//
// Provides Swift-idiomatic API for the Metal implementation of Hgi.
// This module wraps the C++ HgiMetal types for use in Swift applications.
//
// Platform: Apple only (macOS, iOS, visionOS)

#if os(macOS) || os(iOS) || os(visionOS)

import USDCxx

// MARK: - Type Aliases

/// Metal implementation of the Hydra Graphics Interface.
public typealias HgiMetal = swiftusd.HgiMetalRef

/// Metal-specific device capabilities.
public typealias HgiMetalCapabilities = swiftusd.HgiMetalCapabilitiesRef

/// Metal API version enum.
public typealias HgiMetalVersion = swiftusd.HgiMetalAPIVersion

/// Metal command buffer wait type.
public typealias HgiMetalWaitType = swiftusd.HgiMetalCommitWaitType

// MARK: - HgiMetalRef Extensions

extension swiftusd.HgiMetalRef {
    /// Create a Metal GPU interface using the system default device.
    ///
    /// - Returns: A new HgiMetal instance, or nil if creation failed.
    @inlinable
    public static func createDefault() -> swiftusd.HgiMetalRef? {
        return swiftusd.HgiMetalRef.Create()
    }

    /// Get the graphics API name as a Swift String (always "Metal").
    @inlinable
    public var apiName: String {
        return String(cString: GetAPIName().GetText())
    }

    /// Get the Metal API version level.
    @inlinable
    public var metalVersion: swiftusd.HgiMetalAPIVersion {
        let caps = GetMetalCapabilities()
        return caps?.GetMetalAPIVersion() ?? .Metal1_0
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

    /// Commit the primary command buffer with a Swift-friendly API.
    ///
    /// - Parameters:
    ///   - wait: Whether to wait for the command buffer to complete.
    ///   - forceNewBuffer: Force creation of a new buffer after commit.
    @inlinable
    public func commitCommands(
        wait: swiftusd.HgiMetalCommitWaitType = .NoWait,
        forceNewBuffer: Bool = false
    ) {
        CommitPrimaryCommandBuffer(wait, forceNewBuffer)
    }
}

// MARK: - HgiMetalCapabilitiesRef Extensions

extension swiftusd.HgiMetalCapabilitiesRef {
    /// The Metal API version level.
    @inlinable
    public var metalVersion: swiftusd.HgiMetalAPIVersion {
        return GetMetalAPIVersion()
    }

    /// The API version as an integer.
    @inlinable
    public var apiVersion: Int {
        return Int(GetAPIVersion())
    }

    /// The Metal Shading Language (MSL) version.
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

    /// Uniform buffer offset alignment requirement.
    @inlinable
    public var uniformBufferOffsetAlignment: Int {
        return Int(GetUniformBufferOffsetAlignment())
    }

    /// Whether vertex memory barriers are supported.
    @inlinable
    public var hasVertexMemoryBarrier: Bool {
        return HasVertexMemoryBarrier()
    }

    /// Whether parallel encoder is supported/enabled.
    @inlinable
    public var useParallelEncoder: Bool {
        return UseParallelEncoder()
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

// MARK: - HgiMetalAPIVersion Extensions

extension swiftusd.HgiMetalAPIVersion {
    /// A human-readable description of the Metal version.
    public var description: String {
        switch self {
        case .Metal1_0:
            return "Metal 1.0"
        case .Metal2_0:
            return "Metal 2.0"
        case .Metal3_0:
            return "Metal 3.0"
        @unknown default:
            return "Metal (Unknown)"
        }
    }

    /// Whether this version supports argument buffers.
    @inlinable
    public var supportsArgumentBuffers: Bool {
        return self.rawValue >= swiftusd.HgiMetalAPIVersion.Metal2_0.rawValue
    }

    /// Whether this version supports mesh shaders.
    @inlinable
    public var supportsMeshShaders: Bool {
        return self.rawValue >= swiftusd.HgiMetalAPIVersion.Metal3_0.rawValue
    }
}

// MARK: - HgiMetalCommitWaitType Extensions

extension swiftusd.HgiMetalCommitWaitType {
    /// A human-readable description of the wait type.
    public var description: String {
        switch self {
        case .NoWait:
            return "No Wait"
        case .WaitUntilScheduled:
            return "Wait Until Scheduled"
        case .WaitUntilCompleted:
            return "Wait Until Completed"
        @unknown default:
            return "Unknown"
        }
    }
}

#endif // os(macOS) || os(iOS) || os(visionOS)
