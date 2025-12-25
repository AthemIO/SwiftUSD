// HgiMetalCxxTests.swift - Tests for HgiMetal C++ interop module
//
// Platform: Apple only (macOS, iOS, visionOS)

#if os(macOS) || os(iOS) || os(visionOS)

import XCTest
import USDCxx
@testable import SwiftUSD

final class HgiMetalCxxTests: XCTestCase {

    // MARK: - Factory Method Tests

    func testHgiMetalCreate() {
        let hgi = swiftusd.HgiMetalRef.Create()
        XCTAssertNotNil(hgi, "Should be able to create HgiMetal instance")

        if let hgi = hgi {
            XCTAssertTrue(hgi.IsBackendSupported(), "Metal backend should be supported")
        }
    }

    func testHgiMetalCreateDefault() {
        let hgi = swiftusd.HgiMetalRef.createDefault()
        XCTAssertNotNil(hgi, "Should be able to create default HgiMetal instance")
    }

    func testHgiMetalIsSupported() {
        // On Apple platforms, Metal should always be supported
        XCTAssertTrue(swiftusd.HgiMetalRef.IsSupported())
    }

    // MARK: - API Information Tests

    func testHgiMetalGetAPIName() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        let apiName = String(cString: hgi.GetAPIName().GetText())
        XCTAssertEqual(apiName, "Metal", "API name should be 'Metal'")
    }

    func testHgiMetalApiNameExtension() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        XCTAssertEqual(hgi.apiName, "Metal")
    }

    func testHgiMetalGetAPIVersion() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        let version = hgi.GetAPIVersion()
        XCTAssertGreaterThanOrEqual(version, 0, "API version should be non-negative")
    }

    // MARK: - Capabilities Tests

    func testHgiMetalCapabilitiesCreate() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        let caps = hgi.GetMetalCapabilities()
        XCTAssertNotNil(caps, "Should be able to get Metal capabilities")
    }

    func testHgiMetalCapabilitiesValues() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        guard let caps = hgi.GetMetalCapabilities() else {
            XCTFail("Failed to get capabilities")
            return
        }

        // API version should be valid
        XCTAssertGreaterThanOrEqual(caps.GetAPIVersion(), 0)

        // Shader version should be positive
        XCTAssertGreaterThan(caps.GetShaderVersion(), 0)

        // Size limits should be positive
        XCTAssertGreaterThan(caps.GetMaxUniformBlockSize(), 0)
        XCTAssertGreaterThan(caps.GetMaxShaderStorageBlockSize(), 0)
        XCTAssertGreaterThan(caps.GetUniformBufferOffsetAlignment(), 0)
    }

    func testHgiMetalCapabilitiesExtensions() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        guard let caps = hgi.GetMetalCapabilities() else {
            XCTFail("Failed to get capabilities")
            return
        }

        XCTAssertGreaterThanOrEqual(caps.apiVersion, 0)
        XCTAssertGreaterThan(caps.shaderVersion, 0)
        XCTAssertGreaterThan(caps.maxUniformBlockSize, 0)
        XCTAssertGreaterThan(caps.maxShaderStorageBlockSize, 0)
        XCTAssertGreaterThan(caps.uniformBufferOffsetAlignment, 0)
    }

    func testHgiMetalCapabilitiesFeatures() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        guard let caps = hgi.GetMetalCapabilities() else {
            XCTFail("Failed to get capabilities")
            return
        }

        // These should return boolean values (test that they don't crash)
        _ = caps.hasVertexMemoryBarrier
        _ = caps.useParallelEncoder
    }

    func testHgiMetalCapabilitiesVersion() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        guard let caps = hgi.GetMetalCapabilities() else {
            XCTFail("Failed to get capabilities")
            return
        }

        let metalVersion = caps.GetMetalAPIVersion()
        // Should be a valid Metal version
        XCTAssertTrue(
            metalVersion == .Metal1_0 ||
            metalVersion == .Metal2_0 ||
            metalVersion == .Metal3_0
        )
    }

    // MARK: - Frame Management Tests

    func testHgiMetalFrameManagement() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        // Test frame start/end cycle
        hgi.StartFrame()
        hgi.EndFrame()

        // Test garbage collection
        hgi.GarbageCollect()

        // Test multiple frames
        for _ in 0..<3 {
            hgi.StartFrame()
            hgi.EndFrame()
        }
    }

    // MARK: - Buffer Management Tests

    func testHgiMetalCreateBuffer() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        var desc = swiftusd.HgiBufferDesc()
        desc.byteSize = 4096
        desc.usage = HgiBufferUsageFlags.vertex

        let buffer = hgi.CreateBuffer(desc)
        XCTAssertNotNil(buffer)

        if let buffer = buffer {
            XCTAssertTrue(buffer.IsValid())
            XCTAssertEqual(buffer.GetByteSize(), 4096)
            XCTAssertEqual(buffer.GetUsage(), HgiBufferUsageFlags.vertex)

            // Clean up
            hgi.DestroyBuffer(buffer)
        }
    }

    func testHgiMetalCreateBufferConvenience() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        let buffer = hgi.createBuffer(
            name: "TestBuffer",
            byteSize: 2048,
            usage: HgiBufferUsageFlags.uniform
        )
        XCTAssertNotNil(buffer)

        if let buffer = buffer {
            XCTAssertTrue(buffer.isValid)
            XCTAssertEqual(buffer.byteSize, 2048)
        }
    }

    // MARK: - Texture Management Tests

    func testHgiMetalCreateTexture() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        var desc = swiftusd.HgiTextureDesc()
        desc.format = .UNorm8Vec4
        desc.type = .Texture2D
        desc.width = 256
        desc.height = 256
        desc.depth = 1
        desc.mipLevels = 1
        desc.layerCount = 1
        desc.sampleCount = 1
        desc.usage = HgiTextureUsageFlags.shaderRead

        let texture = hgi.CreateTexture(desc)
        XCTAssertNotNil(texture)

        if let texture = texture {
            XCTAssertTrue(texture.IsValid())
            XCTAssertEqual(texture.GetWidth(), 256)
            XCTAssertEqual(texture.GetHeight(), 256)
            XCTAssertEqual(texture.GetDepth(), 1)
            XCTAssertEqual(texture.GetFormat(), .UNorm8Vec4)
            XCTAssertEqual(texture.GetType(), .Texture2D)

            // Clean up
            hgi.DestroyTexture(texture)
        }
    }

    func testHgiMetalCreateTexture2DConvenience() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        let texture = hgi.createTexture2D(
            name: "TestTexture",
            width: 512,
            height: 512,
            format: .Float32Vec4
        )
        XCTAssertNotNil(texture)

        if let texture = texture {
            XCTAssertTrue(texture.isValid)
            XCTAssertEqual(texture.width, 512)
            XCTAssertEqual(texture.height, 512)
            XCTAssertEqual(texture.depth, 1)
            XCTAssertEqual(texture.format, .Float32Vec4)
        }
    }

    // MARK: - Command Buffer Tests

    func testHgiMetalSetHasWork() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        // This should not crash
        hgi.SetHasWork()
    }

    func testHgiMetalCommitPrimaryCommandBuffer() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        hgi.StartFrame()
        hgi.SetHasWork()
        hgi.CommitPrimaryCommandBuffer(.NoWait, false)
        hgi.EndFrame()
    }

    func testHgiMetalCommitCommandsConvenience() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        hgi.StartFrame()
        hgi.SetHasWork()
        hgi.commitCommands(wait: .NoWait, forceNewBuffer: false)
        hgi.EndFrame()
    }

    // MARK: - Metal Version Enum Tests

    func testHgiMetalAPIVersionRawValues() {
        XCTAssertEqual(swiftusd.HgiMetalAPIVersion.Metal1_0.rawValue, 0)
        XCTAssertEqual(swiftusd.HgiMetalAPIVersion.Metal2_0.rawValue, 1)
        XCTAssertEqual(swiftusd.HgiMetalAPIVersion.Metal3_0.rawValue, 2)
    }

    func testHgiMetalAPIVersionDescription() {
        XCTAssertEqual(swiftusd.HgiMetalAPIVersion.Metal1_0.description, "Metal 1.0")
        XCTAssertEqual(swiftusd.HgiMetalAPIVersion.Metal2_0.description, "Metal 2.0")
        XCTAssertEqual(swiftusd.HgiMetalAPIVersion.Metal3_0.description, "Metal 3.0")
    }

    func testHgiMetalAPIVersionFeatures() {
        XCTAssertFalse(swiftusd.HgiMetalAPIVersion.Metal1_0.supportsArgumentBuffers)
        XCTAssertTrue(swiftusd.HgiMetalAPIVersion.Metal2_0.supportsArgumentBuffers)
        XCTAssertTrue(swiftusd.HgiMetalAPIVersion.Metal3_0.supportsArgumentBuffers)

        XCTAssertFalse(swiftusd.HgiMetalAPIVersion.Metal1_0.supportsMeshShaders)
        XCTAssertFalse(swiftusd.HgiMetalAPIVersion.Metal2_0.supportsMeshShaders)
        XCTAssertTrue(swiftusd.HgiMetalAPIVersion.Metal3_0.supportsMeshShaders)
    }

    // MARK: - Commit Wait Type Tests

    func testHgiMetalCommitWaitTypeRawValues() {
        XCTAssertEqual(swiftusd.HgiMetalCommitWaitType.NoWait.rawValue, 0)
        XCTAssertEqual(swiftusd.HgiMetalCommitWaitType.WaitUntilScheduled.rawValue, 1)
        XCTAssertEqual(swiftusd.HgiMetalCommitWaitType.WaitUntilCompleted.rawValue, 2)
    }

    func testHgiMetalCommitWaitTypeDescription() {
        XCTAssertEqual(swiftusd.HgiMetalCommitWaitType.NoWait.description, "No Wait")
        XCTAssertEqual(swiftusd.HgiMetalCommitWaitType.WaitUntilScheduled.description, "Wait Until Scheduled")
        XCTAssertEqual(swiftusd.HgiMetalCommitWaitType.WaitUntilCompleted.description, "Wait Until Completed")
    }

    // MARK: - Metal Version Extension Tests

    func testHgiMetalVersionFromInstance() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        // metalVersion extension should work
        let version = hgi.metalVersion
        XCTAssertTrue(
            version == .Metal1_0 ||
            version == .Metal2_0 ||
            version == .Metal3_0
        )
    }

    // MARK: - Type Alias Tests

    func testTypeAliases() {
        // These type aliases should compile correctly
        let _: HgiMetal.Type = swiftusd.HgiMetalRef.self
        let _: HgiMetalCapabilities.Type = swiftusd.HgiMetalCapabilitiesRef.self
        let _: HgiMetalVersion.Type = swiftusd.HgiMetalAPIVersion.self
        let _: HgiMetalWaitType.Type = swiftusd.HgiMetalCommitWaitType.self
    }

    // MARK: - Capability Flag Tests

    func testMetalCapabilityFlags() {
        guard let hgi = swiftusd.HgiMetalRef.Create() else {
            XCTFail("Failed to create HgiMetal")
            return
        }

        guard let caps = hgi.GetMetalCapabilities() else {
            XCTFail("Failed to get capabilities")
            return
        }

        // Test hasCapability extension
        _ = caps.hasCapability(HgiDeviceCapabilityFlags.presentation)
        _ = caps.hasCapability(HgiDeviceCapabilityFlags.unifiedMemory)
        _ = caps.hasCapability(HgiDeviceCapabilityFlags.metalTessellation)
    }
}

#endif // os(macOS) || os(iOS) || os(visionOS)
