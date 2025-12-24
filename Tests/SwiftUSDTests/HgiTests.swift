import XCTest
@testable import SwiftUSD

final class HgiTests: XCTestCase {

    // MARK: - Hgi Creation Tests

    func testCreatePlatformDefaultHgi() throws {
        let hgi = try Hgi()
        XCTAssertFalse(hgi.apiName.isEmpty)
        print("Created Hgi with API: \(hgi.apiName)")
    }

    func testHgiBackendSupported() throws {
        let hgi = try Hgi()
        // In standalone mode, this should return true
        let supported = hgi.isBackendSupported
        print("Backend supported: \(supported)")
        XCTAssertTrue(supported)
    }

    func testHgiIsSupported() {
        // Test platform default support
        let defaultSupported = Hgi.isSupported()
        print("Platform default supported: \(defaultSupported)")
        XCTAssertTrue(defaultSupported)
    }

    func testHgiApiName() throws {
        let hgi = try Hgi()
        let name = hgi.apiName
        XCTAssertFalse(name.isEmpty)
        print("API Name: \(name)")
    }

    // MARK: - Capabilities Tests

    func testCapabilities() throws {
        let hgi = try Hgi()
        let caps = hgi.capabilities

        print("API Version: \(caps.apiVersion)")
        print("Shader Version: \(caps.shaderVersion)")
        print("Max Uniform Block Size: \(caps.maxUniformBlockSize)")
        print("Max Shader Storage Block Size: \(caps.maxShaderStorageBlockSize)")
        print("Uniform Buffer Offset Alignment: \(caps.uniformBufferOffsetAlignment)")
        print("Max Clip Distances: \(caps.maxClipDistances)")

        // These should have reasonable default values
        XCTAssertGreaterThan(caps.maxUniformBlockSize, 0)
    }

    func testCapabilityFlags() throws {
        let hgi = try Hgi()
        let caps = hgi.capabilities

        // Just verify these don't crash
        _ = caps.isSet(.presentation)
        _ = caps.isSet(.unifiedMemory)
        _ = caps.isSet(.concurrentDispatch)
    }

    // MARK: - Buffer Tests

    func testCreateBuffer() throws {
        let hgi = try Hgi()

        let buffer = try hgi.createBuffer(
            byteSize: 1024,
            usage: .uniform,
            debugName: "TestBuffer"
        )

        XCTAssertEqual(buffer.byteSize, 1024)
        XCTAssertEqual(buffer.usage, .uniform)
        XCTAssertGreaterThanOrEqual(buffer.byteSizeOfResource, 1024)
        print("Created buffer with size: \(buffer.byteSizeOfResource)")
    }

    func testCreateVertexBuffer() throws {
        let hgi = try Hgi()

        let buffer = try hgi.createBuffer(
            byteSize: 4096,
            usage: .vertex,
            vertexStride: 32,
            debugName: "VertexBuffer"
        )

        XCTAssertEqual(buffer.byteSize, 4096)
        XCTAssertEqual(buffer.usage, .vertex)
    }

    func testCreateBufferWithInitialData() throws {
        let hgi = try Hgi()

        let data: [Float] = [1.0, 2.0, 3.0, 4.0]
        let buffer = try data.withUnsafeBytes { ptr in
            try hgi.createBuffer(
                byteSize: data.count * MemoryLayout<Float>.stride,
                usage: .uniform,
                debugName: "DataBuffer",
                initialData: ptr.baseAddress
            )
        }

        XCTAssertEqual(buffer.byteSize, data.count * MemoryLayout<Float>.stride)
    }

    func testBufferCPUStagingAddress() throws {
        let hgi = try Hgi()

        let buffer = try hgi.createBuffer(
            byteSize: 256,
            usage: .uniform
        )

        // In standalone mode, staging should be available
        let staging = buffer.cpuStagingAddress
        print("CPU Staging address available: \(staging != nil)")
    }

    // MARK: - Texture Tests

    func testCreateTexture2D() throws {
        let hgi = try Hgi()

        let texture = try hgi.createTexture(
            width: 256,
            height: 256,
            format: .float32Vec4,
            type: .texture2D,
            usage: .shaderRead,
            debugName: "TestTexture"
        )

        XCTAssertEqual(texture.format, .float32Vec4)
        XCTAssertEqual(texture.type, .texture2D)
        let dims = texture.dimensions
        XCTAssertEqual(dims.width, 256)
        XCTAssertEqual(dims.height, 256)
        XCTAssertEqual(dims.depth, 1)
        print("Created 2D texture: \(dims.width)x\(dims.height)")
    }

    func testCreateTexture3D() throws {
        let hgi = try Hgi()

        let texture = try hgi.createTexture(
            width: 64,
            height: 64,
            depth: 64,
            format: .unorm8Vec4,
            type: .texture3D,
            usage: [.shaderRead, .shaderWrite],
            debugName: "VolumeTexture"
        )

        XCTAssertEqual(texture.type, .texture3D)
        let dims = texture.dimensions
        XCTAssertEqual(dims.depth, 64)
    }

    func testCreateTextureWithMips() throws {
        let hgi = try Hgi()

        let texture = try hgi.createTexture(
            width: 512,
            height: 512,
            format: .unorm8Vec4,
            mipLevels: 10,
            debugName: "MippedTexture"
        )

        XCTAssertEqual(texture.mipLevels, 10)
    }

    func testCreateMultisampleTexture() throws {
        let hgi = try Hgi()

        let texture = try hgi.createTexture(
            width: 1024,
            height: 1024,
            format: .float32Vec4,
            usage: .colorTarget,
            sampleCount: .count4,
            debugName: "MSAATexture"
        )

        XCTAssertEqual(texture.sampleCount, .count4)
    }

    // MARK: - Sampler Tests

    func testCreateSampler() throws {
        let hgi = try Hgi()

        let sampler = try hgi.createSampler(
            magFilter: .linear,
            minFilter: .linear,
            mipFilter: .linear,
            addressModeU: .repeat,
            addressModeV: .repeat,
            addressModeW: .repeat,
            debugName: "TestSampler"
        )

        // Sampler created successfully
        print("Created sampler with raw resource: \(sampler.rawResource)")
    }

    func testCreateSamplerWithCompare() throws {
        let hgi = try Hgi()

        let sampler = try hgi.createSampler(
            magFilter: .nearest,
            minFilter: .nearest,
            mipFilter: .notMipmapped,
            addressModeU: .clampToEdge,
            enableCompare: true,
            compareFunction: .less,
            debugName: "ShadowSampler"
        )

        // Sampler created successfully
        _ = sampler
    }

    // MARK: - Shader Tests

    func testCreateShaderFunction() throws {
        let hgi = try Hgi()

        let vertexShader = """
        #version 450
        layout(location = 0) in vec3 position;
        void main() {
            gl_Position = vec4(position, 1.0);
        }
        """

        let shaderFunc = try hgi.createShaderFunction(
            stage: .vertex,
            code: vertexShader,
            debugName: "VertexShader"
        )

        // In standalone mode, shaders are always "valid"
        XCTAssertTrue(shaderFunc.isValid)
        print("Shader function valid: \(shaderFunc.isValid)")
        print("Shader compile errors: '\(shaderFunc.compileErrors)'")
    }

    func testCreateFragmentShader() throws {
        let hgi = try Hgi()

        let fragmentShader = """
        #version 450
        layout(location = 0) out vec4 fragColor;
        void main() {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
        """

        let shaderFunc = try hgi.createShaderFunction(
            stage: .fragment,
            code: fragmentShader,
            debugName: "FragmentShader"
        )

        XCTAssertTrue(shaderFunc.isValid)
    }

    func testCreateShaderProgram() throws {
        let hgi = try Hgi()

        let vertexShader = try hgi.createShaderFunction(
            stage: .vertex,
            code: "void main() { gl_Position = vec4(0); }",
            debugName: "VS"
        )

        let fragmentShader = try hgi.createShaderFunction(
            stage: .fragment,
            code: "void main() { }",
            debugName: "FS"
        )

        let program = try hgi.createShaderProgram(
            shaderFunctions: [vertexShader, fragmentShader],
            debugName: "TestProgram"
        )

        XCTAssertTrue(program.isValid)
        print("Shader program valid: \(program.isValid)")
    }

    // MARK: - BlitCmds Tests

    func testCreateBlitCmds() throws {
        let hgi = try Hgi()
        let blitCmds = try hgi.createBlitCmds()
        XCTAssertFalse(blitCmds.isSubmitted)
    }

    func testBlitCmdsBufferCopy() throws {
        let hgi = try Hgi()

        // Create source data
        let sourceData: [Float] = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0]
        let byteSize = sourceData.count * MemoryLayout<Float>.stride

        // Create buffer
        let buffer = try hgi.createBuffer(
            byteSize: byteSize,
            usage: .uniform,
            debugName: "CopyTestBuffer"
        )

        // Create blit commands and copy
        let blitCmds = try hgi.createBlitCmds()
        sourceData.withUnsafeBytes { ptr in
            blitCmds.copyBufferCpuToGpu(
                destination: buffer,
                data: ptr.baseAddress!,
                byteSize: byteSize
            )
        }

        // Submit commands
        try hgi.submit(blitCmds)
        XCTAssertTrue(blitCmds.isSubmitted)
    }

    func testBlitCmdsBufferFill() throws {
        let hgi = try Hgi()

        let buffer = try hgi.createBuffer(
            byteSize: 1024,
            usage: .storage,
            debugName: "FillTestBuffer"
        )

        let blitCmds = try hgi.createBlitCmds()
        blitCmds.fillBuffer(buffer, value: 0xFF)

        try hgi.submit(blitCmds)
    }

    func testBlitCmdsBufferToBufferCopy() throws {
        let hgi = try Hgi()

        let srcBuffer = try hgi.createBuffer(
            byteSize: 512,
            usage: .storage,
            debugName: "SrcBuffer"
        )

        let dstBuffer = try hgi.createBuffer(
            byteSize: 512,
            usage: .storage,
            debugName: "DstBuffer"
        )

        let blitCmds = try hgi.createBlitCmds()
        blitCmds.copyBufferGpuToGpu(
            source: srcBuffer,
            destination: dstBuffer,
            byteSize: 512
        )

        try hgi.submit(blitCmds)
    }

    // MARK: - Format Tests

    func testFormatProperties() {
        // Test float32vec4
        let float4 = Hgi.Format.float32Vec4
        XCTAssertEqual(float4.componentCount, 4)
        XCTAssertEqual(float4.dataSize, 16)
        XCTAssertFalse(float4.isCompressed)
        XCTAssertTrue(float4.isFloat)

        // Test unorm8vec4
        let unorm8 = Hgi.Format.unorm8Vec4
        XCTAssertEqual(unorm8.componentCount, 4)
        XCTAssertEqual(unorm8.dataSize, 4)
        XCTAssertFalse(unorm8.isCompressed)
        XCTAssertFalse(unorm8.isFloat)

        // Test compressed format
        let bc7 = Hgi.Format.bc7Unorm8Vec4
        XCTAssertEqual(bc7.componentCount, 4)
        XCTAssertTrue(bc7.isCompressed)
    }

    func testAllFormats() {
        let formats: [Hgi.Format] = [
            .unorm8, .unorm8Vec2, .unorm8Vec4,
            .snorm8, .snorm8Vec2, .snorm8Vec4,
            .float16, .float16Vec2, .float16Vec3, .float16Vec4,
            .float32, .float32Vec2, .float32Vec3, .float32Vec4,
            .int16, .int16Vec2, .int16Vec3, .int16Vec4,
            .int32, .int32Vec2, .int32Vec3, .int32Vec4
        ]

        for format in formats {
            XCTAssertGreaterThan(format.componentCount, 0)
            XCTAssertGreaterThan(format.dataSize, 0)
        }
    }

    // MARK: - Frame Management Tests

    func testFrameManagement() throws {
        let hgi = try Hgi()

        // These should not crash
        hgi.startFrame()
        hgi.endFrame()
        hgi.garbageCollect()
    }

    // MARK: - Error Tests

    func testHgiError() {
        let error1 = HgiError.creationFailed("Test")
        XCTAssertTrue(error1.description.contains("creation"))

        let error2 = HgiError.submissionFailed("Test")
        XCTAssertTrue(error2.description.contains("submission"))

        let error3 = HgiError.compilationFailed("Test")
        XCTAssertTrue(error3.description.contains("compilation"))

        let error4 = HgiError.invalidOperation("Test")
        XCTAssertTrue(error4.description.contains("invalid"))
    }

    // MARK: - Integration Tests

    func testFullPipelineWorkflow() throws {
        let hgi = try Hgi()

        print("\n=== Hgi Full Pipeline Test ===")
        print("API: \(hgi.apiName)")
        print("Backend Supported: \(hgi.isBackendSupported)")

        // Create resources
        let vertexData: [Float] = [
            -1.0, -1.0, 0.0,
             1.0, -1.0, 0.0,
             0.0,  1.0, 0.0
        ]

        let vertexBuffer = try vertexData.withUnsafeBytes { ptr in
            try hgi.createBuffer(
                byteSize: vertexData.count * MemoryLayout<Float>.stride,
                usage: .vertex,
                vertexStride: UInt32(MemoryLayout<Float>.stride * 3),
                debugName: "TriangleVertices",
                initialData: ptr.baseAddress
            )
        }

        let texture = try hgi.createTexture(
            width: 512,
            height: 512,
            format: .float32Vec4,
            usage: .colorTarget,
            debugName: "RenderTarget"
        )

        let sampler = try hgi.createSampler(debugName: "LinearSampler")

        let vertexShader = try hgi.createShaderFunction(
            stage: .vertex,
            code: "void main() { gl_Position = vec4(0); }",
            debugName: "TriangleVS"
        )

        let fragmentShader = try hgi.createShaderFunction(
            stage: .fragment,
            code: "void main() { }",
            debugName: "TriangleFS"
        )

        let program = try hgi.createShaderProgram(
            shaderFunctions: [vertexShader, fragmentShader],
            debugName: "TriangleProgram"
        )

        print("Created vertex buffer: \(vertexBuffer.byteSize) bytes")
        print("Created texture: \(texture.dimensions.width)x\(texture.dimensions.height)")
        print("Created sampler: \(sampler.rawResource)")
        print("Shader program valid: \(program.isValid)")

        // Frame simulation
        hgi.startFrame()

        let blitCmds = try hgi.createBlitCmds()
        blitCmds.fillBuffer(vertexBuffer, value: 0)
        try hgi.submit(blitCmds, wait: .waitUntilCompleted)

        hgi.endFrame()
        hgi.garbageCollect()

        print("=== Test Complete ===\n")
    }

    func testCapabilitiesSummary() throws {
        let hgi = try Hgi()
        let caps = hgi.capabilities

        print("\n=== Hgi Capabilities Summary ===")
        print("API Version: \(caps.apiVersion)")
        print("Shader Version: \(caps.shaderVersion)")
        print("Max Uniform Block Size: \(caps.maxUniformBlockSize)")
        print("Max Shader Storage Block Size: \(caps.maxShaderStorageBlockSize)")
        print("Uniform Buffer Offset Alignment: \(caps.uniformBufferOffsetAlignment)")
        print("Max Clip Distances: \(caps.maxClipDistances)")
        print("================================\n")
    }
}
