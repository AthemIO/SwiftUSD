// HgiCxxTests.swift - Tests for Hgi C++ interop module

import XCTest
import USDCxx
@testable import SwiftUSD

final class HgiCxxTests: XCTestCase {

    // MARK: - HgiFormat Tests

    func testHgiFormatComponentCount() {
        // Single component formats
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.UNorm8), 1)
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.Float32), 1)
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.Int32), 1)

        // Two component formats
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.UNorm8Vec2), 2)
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.Float32Vec2), 2)

        // Three component formats
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.Float32Vec3), 3)
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.Float16Vec3), 3)

        // Four component formats
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.UNorm8Vec4), 4)
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.Float32Vec4), 4)
        XCTAssertEqual(swiftusd.HgiGetComponentCount(.UNorm8Vec4srgb), 4)
    }

    func testHgiFormatDataSize() {
        // 1-byte formats
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.UNorm8), 1)
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.SNorm8), 1)

        // 2-byte formats
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.UNorm8Vec2), 2)
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.Float16), 2)

        // 4-byte formats
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.Float32), 4)
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.UNorm8Vec4), 4)

        // 8-byte formats
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.Float32Vec2), 8)

        // 12-byte formats
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.Float32Vec3), 12)

        // 16-byte formats
        XCTAssertEqual(swiftusd.HgiGetDataSizeOfFormat(.Float32Vec4), 16)
    }

    func testHgiFormatCompression() {
        // Uncompressed formats
        XCTAssertFalse(swiftusd.HgiIsCompressed(.UNorm8Vec4))
        XCTAssertFalse(swiftusd.HgiIsCompressed(.Float32Vec4))
        XCTAssertFalse(swiftusd.HgiIsCompressed(.UNorm8Vec4srgb))

        // Compressed formats
        XCTAssertTrue(swiftusd.HgiIsCompressed(.BC6FloatVec3))
        XCTAssertTrue(swiftusd.HgiIsCompressed(.BC7UNorm8Vec4))
        XCTAssertTrue(swiftusd.HgiIsCompressed(.BC1UNorm8Vec4))
        XCTAssertTrue(swiftusd.HgiIsCompressed(.BC3UNorm8Vec4))
    }

    func testHgiFormatExtensions() {
        let format = swiftusd.HgiFormat.Float32Vec4
        XCTAssertEqual(format.componentCount, 4)
        XCTAssertEqual(format.bytesPerElement, 16)
        XCTAssertFalse(format.isCompressed)

        let compressedFormat = swiftusd.HgiFormat.BC7UNorm8Vec4
        XCTAssertTrue(compressedFormat.isCompressed)
    }

    // MARK: - HgiBufferDesc Tests

    func testHgiBufferDescDefault() {
        let desc = swiftusd.HgiBufferDesc()
        XCTAssertEqual(desc.byteSize, 0)
        XCTAssertEqual(desc.usage, HgiBufferUsageFlags.none)
    }

    func testHgiBufferDescInit() {
        var desc = swiftusd.HgiBufferDesc()
        desc.byteSize = 1024
        desc.usage = HgiBufferUsageFlags.vertex | HgiBufferUsageFlags.storage

        XCTAssertEqual(desc.byteSize, 1024)
        XCTAssertTrue((desc.usage & HgiBufferUsageFlags.vertex) != 0)
        XCTAssertTrue((desc.usage & HgiBufferUsageFlags.storage) != 0)
    }

    // MARK: - HgiTextureDesc Tests

    func testHgiTextureDescDefault() {
        let desc = swiftusd.HgiTextureDesc()
        XCTAssertEqual(desc.format, .UNorm8Vec4)
        XCTAssertEqual(desc.type, .Texture2D)
        XCTAssertEqual(desc.width, 1)
        XCTAssertEqual(desc.height, 1)
        XCTAssertEqual(desc.depth, 1)
        XCTAssertEqual(desc.mipLevels, 1)
        XCTAssertEqual(desc.layerCount, 1)
        XCTAssertEqual(desc.sampleCount, 1)
    }

    func testHgiTextureDescCustom() {
        var desc = swiftusd.HgiTextureDesc()
        desc.format = .Float32Vec4
        desc.type = .Texture3D
        desc.width = 256
        desc.height = 128
        desc.depth = 64
        desc.mipLevels = 4
        desc.usage = HgiTextureUsageFlags.colorTarget | HgiTextureUsageFlags.shaderRead

        XCTAssertEqual(desc.format, .Float32Vec4)
        XCTAssertEqual(desc.type, .Texture3D)
        XCTAssertEqual(desc.width, 256)
        XCTAssertEqual(desc.height, 128)
        XCTAssertEqual(desc.depth, 64)
        XCTAssertEqual(desc.mipLevels, 4)
        XCTAssertTrue((desc.usage & HgiTextureUsageFlags.colorTarget) != 0)
        XCTAssertTrue((desc.usage & HgiTextureUsageFlags.shaderRead) != 0)
    }

    // MARK: - HgiRef Tests

    func testHgiRefCreatePlatformDefault() {
        // This creates a standalone mock Hgi when USE_PIXAR_USD is not defined
        let hgi = swiftusd.HgiRef.CreatePlatformDefault()
        XCTAssertNotNil(hgi)

        if let hgi = hgi {
            XCTAssertTrue(hgi.IsBackendSupported())
            let apiName = String(cString: hgi.GetAPIName().GetText())
            XCTAssertFalse(apiName.isEmpty)
        }
    }

    func testHgiRefFrameManagement() {
        guard let hgi = swiftusd.HgiRef.CreatePlatformDefault() else {
            XCTFail("Failed to create Hgi")
            return
        }

        // Test frame start/end cycle
        hgi.StartFrame()
        hgi.EndFrame()

        // Test garbage collection
        hgi.GarbageCollect()
    }

    func testHgiRefCreateBuffer() {
        guard let hgi = swiftusd.HgiRef.CreatePlatformDefault() else {
            XCTFail("Failed to create Hgi")
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

    func testHgiRefCreateTexture() {
        guard let hgi = swiftusd.HgiRef.CreatePlatformDefault() else {
            XCTFail("Failed to create Hgi")
            return
        }

        var desc = swiftusd.HgiTextureDesc()
        desc.format = .UNorm8Vec4
        desc.type = .Texture2D
        desc.width = 512
        desc.height = 512
        desc.depth = 1
        desc.mipLevels = 1
        desc.layerCount = 1
        desc.sampleCount = 1
        desc.usage = HgiTextureUsageFlags.shaderRead

        let texture = hgi.CreateTexture(desc)
        XCTAssertNotNil(texture)

        if let texture = texture {
            XCTAssertTrue(texture.IsValid())
            XCTAssertEqual(texture.GetWidth(), 512)
            XCTAssertEqual(texture.GetHeight(), 512)
            XCTAssertEqual(texture.GetDepth(), 1)
            XCTAssertEqual(texture.GetFormat(), .UNorm8Vec4)
            XCTAssertEqual(texture.GetType(), .Texture2D)

            // Clean up
            hgi.DestroyTexture(texture)
        }
    }

    func testHgiRefConvenienceExtensions() {
        guard let hgi = swiftusd.HgiRef.createDefault() else {
            XCTFail("Failed to create Hgi")
            return
        }

        XCTAssertFalse(hgi.apiName.isEmpty)

        // Test convenience buffer creation
        let buffer = hgi.createBuffer(name: "TestBuffer", byteSize: 2048, usage: HgiBufferUsageFlags.uniform)
        XCTAssertNotNil(buffer)
        if let buffer = buffer {
            XCTAssertEqual(buffer.byteSize, 2048)
            XCTAssertTrue(buffer.isValid)
        }

        // Test convenience texture creation
        let texture = hgi.createTexture2D(name: "TestTexture", width: 256, height: 256)
        XCTAssertNotNil(texture)
        if let texture = texture {
            XCTAssertEqual(texture.width, 256)
            XCTAssertEqual(texture.height, 256)
            XCTAssertEqual(texture.depth, 1)
            XCTAssertTrue(texture.isValid)
        }
    }

    // MARK: - HgiCapabilities Tests

    func testHgiCapabilities() {
        guard let hgi = swiftusd.HgiRef.CreatePlatformDefault() else {
            XCTFail("Failed to create Hgi")
            return
        }

        let caps = hgi.GetCapabilities()
        XCTAssertNotNil(caps)

        if let caps = caps {
            // API version should be non-negative
            XCTAssertGreaterThanOrEqual(caps.GetAPIVersion(), 0)
            XCTAssertGreaterThanOrEqual(caps.GetShaderVersion(), 0)

            // Size limits should be positive
            XCTAssertGreaterThan(caps.GetMaxUniformBlockSize(), 0)
            XCTAssertGreaterThan(caps.GetMaxShaderStorageBlockSize(), 0)
            XCTAssertGreaterThan(caps.GetUniformBufferOffsetAlignment(), 0)
        }
    }

    func testHgiCapabilitiesExtensions() {
        guard let hgi = swiftusd.HgiRef.CreatePlatformDefault() else {
            XCTFail("Failed to create Hgi")
            return
        }

        guard let caps = hgi.GetCapabilities() else {
            XCTFail("Failed to get capabilities")
            return
        }

        XCTAssertGreaterThanOrEqual(caps.apiVersion, 0)
        XCTAssertGreaterThanOrEqual(caps.shaderVersion, 0)
        XCTAssertGreaterThan(caps.maxUniformBlockSize, 0)
        XCTAssertGreaterThan(caps.maxShaderStorageBlockSize, 0)

        // Test capability flag checking
        _ = caps.hasCapability(HgiDeviceCapabilityFlags.presentation)
    }

    // MARK: - Usage Flags Tests

    func testBufferUsageFlags() {
        XCTAssertEqual(HgiBufferUsageFlags.none, 0)
        XCTAssertEqual(HgiBufferUsageFlags.uniform, 1 << 0)
        XCTAssertEqual(HgiBufferUsageFlags.index32, 1 << 1)
        XCTAssertEqual(HgiBufferUsageFlags.vertex, 1 << 2)
        XCTAssertEqual(HgiBufferUsageFlags.storage, 1 << 3)
        XCTAssertEqual(HgiBufferUsageFlags.indirect, 1 << 4)

        // Test combining flags
        let combined = HgiBufferUsageFlags.vertex | HgiBufferUsageFlags.index32
        XCTAssertTrue((combined & HgiBufferUsageFlags.vertex) != 0)
        XCTAssertTrue((combined & HgiBufferUsageFlags.index32) != 0)
        XCTAssertFalse((combined & HgiBufferUsageFlags.uniform) != 0)
    }

    func testTextureUsageFlags() {
        XCTAssertEqual(HgiTextureUsageFlags.none, 0)
        XCTAssertEqual(HgiTextureUsageFlags.colorTarget, 1 << 0)
        XCTAssertEqual(HgiTextureUsageFlags.depthTarget, 1 << 1)
        XCTAssertEqual(HgiTextureUsageFlags.stencilTarget, 1 << 2)
        XCTAssertEqual(HgiTextureUsageFlags.shaderRead, 1 << 3)
        XCTAssertEqual(HgiTextureUsageFlags.shaderWrite, 1 << 4)

        // Test combining flags
        let combined = HgiTextureUsageFlags.colorTarget | HgiTextureUsageFlags.shaderRead
        XCTAssertTrue((combined & HgiTextureUsageFlags.colorTarget) != 0)
        XCTAssertTrue((combined & HgiTextureUsageFlags.shaderRead) != 0)
    }

    func testDeviceCapabilityFlags() {
        XCTAssertEqual(HgiDeviceCapabilityFlags.none, 0)
        XCTAssertEqual(HgiDeviceCapabilityFlags.presentation, 1 << 0)
        XCTAssertEqual(HgiDeviceCapabilityFlags.bindlessBuffers, 1 << 1)
        XCTAssertEqual(HgiDeviceCapabilityFlags.concurrentDispatch, 1 << 2)
        XCTAssertEqual(HgiDeviceCapabilityFlags.unifiedMemory, 1 << 3)
    }

    // MARK: - HgiTextureType Tests

    func testHgiTextureTypes() {
        XCTAssertEqual(swiftusd.HgiTextureType.Texture1D.rawValue, 0)
        XCTAssertEqual(swiftusd.HgiTextureType.Texture2D.rawValue, 1)
        XCTAssertEqual(swiftusd.HgiTextureType.Texture3D.rawValue, 2)
        XCTAssertEqual(swiftusd.HgiTextureType.Texturecube.rawValue, 3)
        XCTAssertEqual(swiftusd.HgiTextureType.Texture1DArray.rawValue, 4)
        XCTAssertEqual(swiftusd.HgiTextureType.Texture2DArray.rawValue, 5)
    }

    // MARK: - Static Support Check

    func testIsSupported() {
        // Check that some backend is supported (should always be true in standalone mode)
        XCTAssertTrue(swiftusd.HgiRef.IsSupported(swiftusd.Token()))
    }
}
