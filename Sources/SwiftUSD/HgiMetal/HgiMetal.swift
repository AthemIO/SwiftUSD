// HgiMetal.swift - Metal implementation of Hgi wrapper
// Mirrors: pxr/imaging/hgiMetal/*.h (Metal implementation of Hydra Graphics Interface)

import OpenUSDInterop

#if canImport(Metal)
import Metal
#endif

// MARK: - HgiMetal

/// Metal implementation of the Hydra Graphics Interface.
///
/// HgiMetal provides the Metal backend for Hydra's GPU abstraction layer.
/// It manages Metal devices, command queues, and command buffer lifecycle.
///
/// ## Availability
/// This class is only available on Apple platforms with Metal support.
///
/// ## Usage
/// ```swift
/// // Create the Metal Hgi instance
/// let hgiMetal = try HgiMetal()
///
/// // Access the Metal device
/// if let device = hgiMetal.primaryDevice {
///     print("Using Metal device: \(device.name)")
/// }
///
/// // Frame management
/// hgiMetal.startFrame()
/// // ... render operations ...
/// hgiMetal.endFrame()
/// ```
public final class HgiMetal: @unchecked Sendable {
    private let handle: HgiMetalRef

    /// Creates an HgiMetal instance using the default Metal device.
    /// - Throws: `HgiMetalError.creationFailed` if Metal is not available.
    public init() throws {
        guard let handle = HgiMetal_Create() else {
            throw HgiMetalError.creationFailed("Failed to create HgiMetal - Metal may not be available")
        }
        self.handle = handle
    }

    #if canImport(Metal)
    /// Creates an HgiMetal instance with a specific Metal device.
    /// - Parameter device: The Metal device to use, or nil for the default device.
    /// - Throws: `HgiMetalError.creationFailed` if creation fails.
    public init(device: MTLDevice?) throws {
        let devicePtr: UnsafeMutableRawPointer?
        if let device = device {
            devicePtr = Unmanaged.passUnretained(device as AnyObject).toOpaque()
        } else {
            devicePtr = nil
        }

        guard let handle = HgiMetal_CreateWithDevice(devicePtr) else {
            throw HgiMetalError.creationFailed("Failed to create HgiMetal with device")
        }
        self.handle = handle
    }
    #endif

    deinit {
        HgiMetal_Release(handle)
    }

    // MARK: - Properties

    /// The API name (always "Metal" for HgiMetal).
    public var apiName: String {
        var namePtr: UnsafeMutablePointer<CChar>?
        guard HgiMetal_GetAPIName(handle, &namePtr) == USD_RESULT_SUCCESS,
              let name = namePtr else {
            return "Metal"
        }
        defer { HgiMetal_FreeString(name) }
        return String(cString: name)
    }

    /// Returns true if the Metal backend is supported on current hardware.
    public var isBackendSupported: Bool {
        HgiMetal_IsBackendSupported(handle)
    }

    /// The Metal API version.
    public var apiVersion: APIVersion {
        APIVersion(rawValue: HgiMetal_GetAPIVersion(handle).rawValue) ?? .metal2_0
    }

    /// Metal-specific device capabilities.
    public var capabilities: Capabilities {
        Capabilities(HgiMetal_GetCapabilities(handle))
    }

    // MARK: - Frame Management

    /// Marks the start of a new rendering frame.
    public func startFrame() {
        HgiMetal_StartFrame(handle)
    }

    /// Marks the end of a rendering frame.
    public func endFrame() {
        HgiMetal_EndFrame(handle)
    }

    /// Performs garbage collection of destroyed resources.
    public func garbageCollect() {
        HgiMetal_GarbageCollect(handle)
    }

    // MARK: - Device Access

    #if canImport(Metal)
    /// The primary Metal device.
    public var primaryDevice: MTLDevice? {
        guard let ptr = HgiMetal_GetPrimaryDevice(handle) else {
            return nil
        }
        return Unmanaged<MTLDevice>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// The Metal command queue.
    public var commandQueue: MTLCommandQueue? {
        guard let ptr = HgiMetal_GetQueue(handle) else {
            return nil
        }
        return Unmanaged<MTLCommandQueue>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// Gets the primary command buffer.
    /// - Parameter flush: If true and there's pending work, flushes the current command buffer.
    /// - Returns: The primary command buffer, or nil if unavailable.
    public func primaryCommandBuffer(flush: Bool = true) -> MTLCommandBuffer? {
        guard let ptr = HgiMetal_GetPrimaryCommandBuffer(handle, flush) else {
            return nil
        }
        return Unmanaged<MTLCommandBuffer>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// Gets a secondary command buffer for concurrent encoding.
    /// - Returns: A secondary command buffer that must be released when done.
    public func secondaryCommandBuffer() -> MTLCommandBuffer? {
        guard let ptr = HgiMetal_GetSecondaryCommandBuffer(handle) else {
            return nil
        }
        return Unmanaged<MTLCommandBuffer>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// Releases a secondary command buffer.
    /// - Parameter commandBuffer: The command buffer to release.
    public func releaseSecondaryCommandBuffer(_ commandBuffer: MTLCommandBuffer) {
        let ptr = Unmanaged.passUnretained(commandBuffer as AnyObject).toOpaque()
        HgiMetal_ReleaseSecondaryCommandBuffer(handle, ptr)
    }

    /// Gets the buffer argument encoder.
    public var bufferArgumentEncoder: MTLArgumentEncoder? {
        guard let ptr = HgiMetal_GetBufferArgumentEncoder(handle) else {
            return nil
        }
        return Unmanaged<MTLArgumentEncoder>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// Gets the sampler argument encoder.
    public var samplerArgumentEncoder: MTLArgumentEncoder? {
        guard let ptr = HgiMetal_GetSamplerArgumentEncoder(handle) else {
            return nil
        }
        return Unmanaged<MTLArgumentEncoder>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// Gets the texture argument encoder.
    public var textureArgumentEncoder: MTLArgumentEncoder? {
        guard let ptr = HgiMetal_GetTextureArgumentEncoder(handle) else {
            return nil
        }
        return Unmanaged<MTLArgumentEncoder>.fromOpaque(ptr).takeUnretainedValue()
    }

    /// Gets an argument buffer for binding.
    public var argBuffer: MTLBuffer? {
        guard let ptr = HgiMetal_GetArgBuffer(handle) else {
            return nil
        }
        return Unmanaged<MTLBuffer>.fromOpaque(ptr).takeUnretainedValue()
    }
    #endif

    // MARK: - Work Tracking

    /// Indicates that there is work to be flushed from the primary command buffer.
    public func setHasWork() {
        HgiMetal_SetHasWork(handle)
    }

    // MARK: - Command Buffer Commits

    /// Commits the primary command buffer.
    /// - Parameters:
    ///   - waitType: How to wait for completion.
    ///   - forceNewBuffer: If true, forces creation of a new command buffer.
    public func commitPrimaryCommandBuffer(
        waitType: CommitWaitType = .noWait,
        forceNewBuffer: Bool = false
    ) {
        HgiMetal_CommitPrimaryCommandBuffer(
            handle,
            HgiMetalCommitWaitType(rawValue: waitType.rawValue),
            forceNewBuffer
        )
    }

    #if canImport(Metal)
    /// Commits a secondary command buffer.
    /// - Parameters:
    ///   - commandBuffer: The secondary command buffer to commit.
    ///   - waitType: How to wait for completion.
    public func commitSecondaryCommandBuffer(
        _ commandBuffer: MTLCommandBuffer,
        waitType: CommitWaitType = .noWait
    ) {
        let ptr = Unmanaged.passUnretained(commandBuffer as AnyObject).toOpaque()
        HgiMetal_CommitSecondaryCommandBuffer(
            handle,
            ptr,
            HgiMetalCommitWaitType(rawValue: waitType.rawValue)
        )
    }
    #endif
}

// MARK: - Nested Types

extension HgiMetal {
    /// Metal API version.
    public enum APIVersion: UInt32, Sendable {
        case metal1_0 = 0
        case metal2_0 = 1
        case metal3_0 = 2
    }

    /// Command buffer wait type for commits.
    public enum CommitWaitType: UInt32, Sendable {
        case noWait = 0
        case waitUntilScheduled = 1
        case waitUntilCompleted = 2
    }

    /// Metal-specific device capabilities.
    public struct Capabilities: @unchecked Sendable {
        private let handle: HgiMetalCapabilitiesRef?

        internal init(_ handle: HgiMetalCapabilitiesRef?) {
            self.handle = handle
        }

        /// The default storage mode for Metal resources.
        public var defaultStorageMode: UInt32 {
            guard let handle = handle else { return 0 }
            return HgiMetalCapabilities_GetDefaultStorageMode(handle)
        }

        /// Returns true if the device supports vertex memory barriers.
        public var hasVertexMemoryBarrier: Bool {
            guard let handle = handle else { return false }
            return HgiMetalCapabilities_HasVertexMemoryBarrier(handle)
        }

        /// Returns true if parallel command encoding should be used.
        public var useParallelEncoder: Bool {
            guard let handle = handle else { return false }
            return HgiMetalCapabilities_UseParallelEncoder(handle)
        }

        /// Returns true if the indirect draw fix is required.
        public var requiresIndirectDrawFix: Bool {
            guard let handle = handle else { return false }
            return HgiMetalCapabilities_RequiresIndirectDrawFix(handle)
        }

        /// Returns true if return after discard is required.
        public var requiresReturnAfterDiscard: Bool {
            guard let handle = handle else { return false }
            return HgiMetalCapabilities_RequiresReturnAfterDiscard(handle)
        }

        /// The API version.
        public var apiVersion: Int {
            guard let handle = handle else { return 0 }
            return Int(HgiMetalCapabilities_GetAPIVersion(handle))
        }

        /// The shader version.
        public var shaderVersion: Int {
            guard let handle = handle else { return 0 }
            return Int(HgiMetalCapabilities_GetShaderVersion(handle))
        }
    }
}

// MARK: - Error Type

/// Errors that can occur in HgiMetal operations.
public enum HgiMetalError: Error, CustomStringConvertible {
    case creationFailed(String)
    case deviceNotAvailable
    case commandBufferFailed(String)
    case notSupported

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "HgiMetal creation failed: \(message)"
        case .deviceNotAvailable:
            return "Metal device not available"
        case .commandBufferFailed(let message):
            return "Command buffer operation failed: \(message)"
        case .notSupported:
            return "HgiMetal is not supported on this platform"
        }
    }
}

// MARK: - Platform Availability

extension HgiMetal {
    /// Returns true if HgiMetal is available on this platform.
    public static var isAvailable: Bool {
        #if canImport(Metal)
        // On Apple platforms, check if Metal is available
        if let device = MTLCreateSystemDefaultDevice() {
            _ = device // Suppress unused variable warning
            return true
        }
        return false
        #else
        return false
        #endif
    }
}
