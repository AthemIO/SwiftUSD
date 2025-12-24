import XCTest
@testable import SwiftUSD

#if canImport(Metal)
import Metal
#endif

final class HgiMetalTests: XCTestCase {

    // MARK: - Availability Tests

    func testHgiMetalAvailability() {
        #if canImport(Metal)
        let available = HgiMetal.isAvailable
        print("HgiMetal is available: \(available)")
        // On macOS, Metal should generally be available
        #if os(macOS) || os(iOS) || os(tvOS)
        // Don't assert true because CI runners might not have Metal
        print("Running on Apple platform, Metal availability: \(available)")
        #endif
        #else
        // HgiMetal shouldn't be available on non-Apple platforms
        XCTAssertFalse(HgiMetal.isAvailable)
        #endif
    }

    // MARK: - HgiMetal Creation Tests

    #if canImport(Metal)
    func testCreateHgiMetal() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()
        XCTAssertEqual(hgiMetal.apiName, "Metal")
        print("Created HgiMetal with API: \(hgiMetal.apiName)")
    }

    func testHgiMetalWithDevice() throws {
        guard let device = MTLCreateSystemDefaultDevice() else {
            throw XCTSkip("No Metal device available")
        }

        let hgiMetal = try HgiMetal(device: device)
        XCTAssertEqual(hgiMetal.apiName, "Metal")
        print("Created HgiMetal with device: \(device.name)")
    }

    func testHgiMetalBackendSupported() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()
        XCTAssertTrue(hgiMetal.isBackendSupported)
        print("Metal backend supported: \(hgiMetal.isBackendSupported)")
    }
    #endif

    // MARK: - API Version Tests

    #if canImport(Metal)
    func testHgiMetalAPIVersion() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()
        let version = hgiMetal.apiVersion

        print("Metal API version: \(version)")

        // Metal 2.0 is the minimum for modern features
        switch version {
        case .metal1_0:
            print("Using Metal 1.0")
        case .metal2_0:
            print("Using Metal 2.0")
        case .metal3_0:
            print("Using Metal 3.0")
        }
    }
    #endif

    // MARK: - Capabilities Tests

    #if canImport(Metal)
    func testHgiMetalCapabilities() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()
        let caps = hgiMetal.capabilities

        print("\n=== HgiMetal Capabilities ===")
        print("Default Storage Mode: \(caps.defaultStorageMode)")
        print("Has Vertex Memory Barrier: \(caps.hasVertexMemoryBarrier)")
        print("Use Parallel Encoder: \(caps.useParallelEncoder)")
        print("Requires Indirect Draw Fix: \(caps.requiresIndirectDrawFix)")
        print("Requires Return After Discard: \(caps.requiresReturnAfterDiscard)")
        print("API Version: \(caps.apiVersion)")
        print("Shader Version: \(caps.shaderVersion)")
        print("=============================\n")

        // API version should be at least 1 (Metal 1.0)
        XCTAssertGreaterThanOrEqual(caps.apiVersion, 1)
    }
    #endif

    // MARK: - Device Access Tests

    #if canImport(Metal)
    func testPrimaryDevice() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()
        if let device = hgiMetal.primaryDevice {
            print("Primary Metal device: \(device.name)")
            print("Device supports GPU family: \(device.supportsFamily(.apple1))")
        } else {
            print("No primary device available (standalone mode)")
        }
    }

    func testCommandQueue() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()
        if let queue = hgiMetal.commandQueue {
            print("Command queue obtained successfully")
            _ = queue // Use to avoid warning
        } else {
            print("No command queue available (standalone mode)")
        }
    }

    func testCommandBuffers() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()

        // Test primary command buffer
        if let primaryCmdBuffer = hgiMetal.primaryCommandBuffer(flush: false) {
            print("Primary command buffer obtained")
            _ = primaryCmdBuffer
        } else {
            print("No primary command buffer available (standalone mode)")
        }

        // Test secondary command buffer
        if let secondaryCmdBuffer = hgiMetal.secondaryCommandBuffer() {
            print("Secondary command buffer obtained")
            hgiMetal.releaseSecondaryCommandBuffer(secondaryCmdBuffer)
            print("Secondary command buffer released")
        } else {
            print("No secondary command buffer available (standalone mode)")
        }
    }
    #endif

    // MARK: - Frame Management Tests

    #if canImport(Metal)
    func testFrameManagement() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()

        // These should not crash
        hgiMetal.startFrame()
        hgiMetal.setHasWork()
        hgiMetal.endFrame()
        hgiMetal.garbageCollect()

        print("Frame management calls completed successfully")
    }
    #endif

    // MARK: - Command Buffer Commit Tests

    #if canImport(Metal)
    func testCommitPrimaryCommandBuffer() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()

        hgiMetal.startFrame()

        // Commit with no wait
        hgiMetal.commitPrimaryCommandBuffer(waitType: .noWait, forceNewBuffer: true)

        // Commit with wait until scheduled
        hgiMetal.commitPrimaryCommandBuffer(waitType: .waitUntilScheduled, forceNewBuffer: true)

        // Commit with wait until completed
        hgiMetal.commitPrimaryCommandBuffer(waitType: .waitUntilCompleted, forceNewBuffer: true)

        hgiMetal.endFrame()

        print("Primary command buffer commit tests passed")
    }

    func testCommitSecondaryCommandBuffer() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()

        if let secondaryCmdBuffer = hgiMetal.secondaryCommandBuffer() {
            hgiMetal.commitSecondaryCommandBuffer(secondaryCmdBuffer, waitType: .noWait)
            print("Secondary command buffer committed successfully")
        } else {
            print("No secondary command buffer available (standalone mode)")
        }
    }
    #endif

    // MARK: - Argument Encoder Tests

    #if canImport(Metal)
    func testArgumentEncoders() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()

        // Test buffer argument encoder
        if let bufferEncoder = hgiMetal.bufferArgumentEncoder {
            print("Buffer argument encoder available")
            _ = bufferEncoder
        } else {
            print("No buffer argument encoder (standalone mode)")
        }

        // Test sampler argument encoder
        if let samplerEncoder = hgiMetal.samplerArgumentEncoder {
            print("Sampler argument encoder available")
            _ = samplerEncoder
        } else {
            print("No sampler argument encoder (standalone mode)")
        }

        // Test texture argument encoder
        if let textureEncoder = hgiMetal.textureArgumentEncoder {
            print("Texture argument encoder available")
            _ = textureEncoder
        } else {
            print("No texture argument encoder (standalone mode)")
        }

        // Test arg buffer
        if let argBuffer = hgiMetal.argBuffer {
            print("Arg buffer available")
            _ = argBuffer
        } else {
            print("No arg buffer (standalone mode)")
        }
    }
    #endif

    // MARK: - Error Tests

    func testHgiMetalError() {
        let error1 = HgiMetalError.creationFailed("Test")
        XCTAssertTrue(error1.description.contains("creation"))

        let error2 = HgiMetalError.deviceNotAvailable
        XCTAssertTrue(error2.description.contains("device"))

        let error3 = HgiMetalError.commandBufferFailed("Test")
        XCTAssertTrue(error3.description.contains("Command buffer"))

        let error4 = HgiMetalError.notSupported
        XCTAssertTrue(error4.description.contains("not supported"))
    }

    // MARK: - Integration Tests

    #if canImport(Metal)
    func testFullHgiMetalWorkflow() throws {
        guard HgiMetal.isAvailable else {
            throw XCTSkip("Metal is not available on this system")
        }

        let hgiMetal = try HgiMetal()

        print("\n=== HgiMetal Full Workflow Test ===")
        print("API: \(hgiMetal.apiName)")
        print("API Version: \(hgiMetal.apiVersion)")
        print("Backend Supported: \(hgiMetal.isBackendSupported)")

        // Get capabilities
        let caps = hgiMetal.capabilities
        print("Metal API Version (from caps): \(caps.apiVersion)")
        print("Shader Version: \(caps.shaderVersion)")

        // Device info
        if let device = hgiMetal.primaryDevice {
            print("Device Name: \(device.name)")
            print("Device is Low Power: \(device.isLowPower)")
            #if os(macOS)
            print("Device is Headless: \(device.isHeadless)")
            #endif
        }

        // Frame simulation
        hgiMetal.startFrame()

        // Simulate some work
        hgiMetal.setHasWork()
        hgiMetal.commitPrimaryCommandBuffer(waitType: .waitUntilCompleted, forceNewBuffer: false)

        hgiMetal.endFrame()
        hgiMetal.garbageCollect()

        print("=== Test Complete ===\n")
    }
    #endif

    // MARK: - Commit Wait Type Tests

    func testCommitWaitTypes() {
        XCTAssertEqual(HgiMetal.CommitWaitType.noWait.rawValue, 0)
        XCTAssertEqual(HgiMetal.CommitWaitType.waitUntilScheduled.rawValue, 1)
        XCTAssertEqual(HgiMetal.CommitWaitType.waitUntilCompleted.rawValue, 2)
    }

    func testAPIVersionTypes() {
        XCTAssertEqual(HgiMetal.APIVersion.metal1_0.rawValue, 0)
        XCTAssertEqual(HgiMetal.APIVersion.metal2_0.rawValue, 1)
        XCTAssertEqual(HgiMetal.APIVersion.metal3_0.rawValue, 2)
    }
}
