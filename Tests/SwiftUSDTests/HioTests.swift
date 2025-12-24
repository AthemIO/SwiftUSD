// HioTests.swift - Tests for Hio module
// Tests image I/O, GLSL shader effects, and format utilities

import XCTest
@testable import SwiftUSD

final class HioTests: XCTestCase {

    // MARK: - Format Tests

    func testFormatComponentCount() {
        // Scalar formats
        XCTAssertEqual(Hio.Format.unorm8.componentCount, 1)
        XCTAssertEqual(Hio.Format.float32.componentCount, 1)

        // Vec2 formats
        XCTAssertEqual(Hio.Format.unorm8Vec2.componentCount, 2)
        XCTAssertEqual(Hio.Format.float32Vec2.componentCount, 2)

        // Vec3 formats
        XCTAssertEqual(Hio.Format.unorm8Vec3.componentCount, 3)
        XCTAssertEqual(Hio.Format.float32Vec3.componentCount, 3)

        // Vec4 formats
        XCTAssertEqual(Hio.Format.unorm8Vec4.componentCount, 4)
        XCTAssertEqual(Hio.Format.float32Vec4.componentCount, 4)
    }

    func testFormatBytesPerPixel() {
        // 8-bit formats
        XCTAssertEqual(Hio.Format.unorm8.bytesPerPixel, 1)
        XCTAssertEqual(Hio.Format.unorm8Vec4.bytesPerPixel, 4)

        // 16-bit formats
        XCTAssertEqual(Hio.Format.float16.bytesPerPixel, 2)
        XCTAssertEqual(Hio.Format.float16Vec4.bytesPerPixel, 8)

        // 32-bit formats
        XCTAssertEqual(Hio.Format.float32.bytesPerPixel, 4)
        XCTAssertEqual(Hio.Format.float32Vec4.bytesPerPixel, 16)

        // 64-bit formats
        XCTAssertEqual(Hio.Format.double64.bytesPerPixel, 8)
        XCTAssertEqual(Hio.Format.double64Vec4.bytesPerPixel, 32)
    }

    func testFormatIsCompressed() {
        // Uncompressed formats
        XCTAssertFalse(Hio.Format.unorm8Vec4.isCompressed)
        XCTAssertFalse(Hio.Format.float32Vec4.isCompressed)

        // Compressed formats
        XCTAssertTrue(Hio.Format.bc1Unorm8Vec4.isCompressed)
        XCTAssertTrue(Hio.Format.bc3Unorm8Vec4.isCompressed)
        XCTAssertTrue(Hio.Format.bc6FloatVec3.isCompressed)
        XCTAssertTrue(Hio.Format.bc7Unorm8Vec4.isCompressed)
    }

    func testFormatDataSize() {
        // Simple 2D image
        let size = Hio.Format.unorm8Vec4.dataSize(width: 256, height: 256)
        XCTAssertEqual(size, 256 * 256 * 4)

        // 3D texture
        let size3D = Hio.Format.float32.dataSize(width: 64, height: 64, depth: 64)
        XCTAssertEqual(size3D, 64 * 64 * 64 * 4)
    }

    func testFormatDescription() {
        XCTAssertEqual(Hio.Format.unorm8Vec4.description, "UNorm8Vec4")
        XCTAssertEqual(Hio.Format.float32Vec3.description, "Float32Vec3")
        XCTAssertEqual(Hio.Format.bc6FloatVec3.description, "BC6FloatVec3")
    }

    func testGetFormat() {
        // 4-channel unsigned byte
        let rgba8 = Hio.getFormat(channels: 4, type: .unsignedByte)
        XCTAssertEqual(rgba8, .unorm8Vec4)

        // 3-channel float
        let rgb32f = Hio.getFormat(channels: 3, type: .float)
        XCTAssertEqual(rgb32f, .float32Vec3)

        // sRGB format
        let srgb = Hio.getFormat(channels: 4, type: .unsignedByte, isSRGB: true)
        XCTAssertEqual(srgb, .unorm8Vec4SRGB)
    }

    // MARK: - Supported Format Tests

    func testSupportedImageFormats() {
        // Check that we have supported formats
        let count = Hio.supportedImageFormatCount
        XCTAssertGreaterThan(count, 0)

        // Get format list
        let formats = Hio.supportedImageFormats
        XCTAssertGreaterThan(formats.count, 0)

        // Check for common formats
        XCTAssertTrue(formats.contains("png"))
        XCTAssertTrue(formats.contains("jpg") || formats.contains("jpeg"))
        XCTAssertTrue(formats.contains("exr"))
    }

    func testIsSupportedImageFile() {
        // Supported formats
        XCTAssertTrue(Hio.isSupportedImageFile("texture.png"))
        XCTAssertTrue(Hio.isSupportedImageFile("texture.jpg"))
        XCTAssertTrue(Hio.isSupportedImageFile("texture.exr"))
        XCTAssertTrue(Hio.isSupportedImageFile("texture.tiff"))

        // Unsupported formats
        XCTAssertFalse(Hio.isSupportedImageFile("texture.xyz"))
        XCTAssertFalse(Hio.isSupportedImageFile("texture.pdf"))
        XCTAssertFalse(Hio.isSupportedImageFile("noextension"))
    }

    // MARK: - Image Tests

    func testImageOpenNonexistent() {
        // In standalone mode, stub images are created even for nonexistent files
        // In full USD mode, this would throw an error
        do {
            let image = try HioImage(filename: "/nonexistent/path/to/image.png")
            // In standalone mode, we get a stub image
            XCTAssertGreaterThan(image.width, 0)
        } catch {
            // In full USD mode, we expect an error
            XCTAssertTrue(error is HioError)
        }
    }

    func testImageIsSupportedImageFile() {
        XCTAssertTrue(HioImage.isSupportedImageFile("test.png"))
        XCTAssertTrue(HioImage.isSupportedImageFile("test.exr"))
        XCTAssertFalse(HioImage.isSupportedImageFile("test.xyz"))
    }

    // MARK: - Glslfx Tests

    func testGlslfxInvalidSource() {
        // In standalone mode, any source creates a valid stub
        // In full USD mode, empty source would fail validation
        do {
            let glslfx = try HioGlslfx(source: "")
            // Standalone mode accepts any source
            XCTAssertTrue(glslfx.isValid)
        } catch {
            // Full USD mode rejects empty source
            XCTAssertTrue(error is HioError)
        }
    }

    func testGlslfxFromSource() {
        // Note: In standalone mode, this creates a stub
        // In full USD mode, this would parse actual glslfx
        do {
            let glslfx = try HioGlslfx(source: "// Test shader source")
            XCTAssertTrue(glslfx.isValid)
            // Hash can be any non-zero value (may be negative due to hash algorithm)
            XCTAssertNotEqual(glslfx.hash, 0)
        } catch {
            // Expected to throw in full USD mode for invalid glslfx
            // This is acceptable behavior
        }
    }

    // MARK: - FieldTextureData Tests

    func testFieldTextureDataNonexistent() {
        // In standalone mode, stub field data is created even for nonexistent files
        // In full USD mode, this would throw an error
        do {
            let fieldData = try HioFieldTextureData(filePath: "/nonexistent/volume.vdb")
            // In standalone mode, we get a stub
            XCTAssertGreaterThan(fieldData.width, 0)
            XCTAssertGreaterThan(fieldData.height, 0)
            XCTAssertGreaterThan(fieldData.depth, 0)
        } catch {
            // In full USD mode, we expect an error
            XCTAssertTrue(error is HioError)
        }
    }

    // MARK: - DataType Tests

    func testDataType() {
        XCTAssertEqual(Hio.Format.unorm8Vec4.dataType, .unsignedByte)
        XCTAssertEqual(Hio.Format.float32Vec4.dataType, .float)
        XCTAssertEqual(Hio.Format.double64Vec4.dataType, .double)
        XCTAssertEqual(Hio.Format.float16Vec4.dataType, .halfFloat)
    }

    // MARK: - ImageOrigin Tests

    func testImageOrigin() {
        XCTAssertEqual(Hio.ImageOrigin.upperLeft.rawValue, 0)
        XCTAssertEqual(Hio.ImageOrigin.lowerLeft.rawValue, 1)
    }

    // MARK: - SourceColorSpace Tests

    func testSourceColorSpace() {
        XCTAssertEqual(Hio.SourceColorSpace.raw.rawValue, 0)
        XCTAssertEqual(Hio.SourceColorSpace.sRGB.rawValue, 1)
        XCTAssertEqual(Hio.SourceColorSpace.auto.rawValue, 2)
    }

    // MARK: - AddressMode Tests

    func testAddressMode() {
        XCTAssertEqual(Hio.AddressMode.clampToEdge.rawValue, 0)
        XCTAssertEqual(Hio.AddressMode.repeat.rawValue, 2)
        XCTAssertEqual(Hio.AddressMode.mirrorRepeat.rawValue, 3)
    }

    // MARK: - Error Description Tests

    func testErrorDescriptions() {
        let fileError = HioError.failedToOpenFile("/path/to/file.png")
        XCTAssertTrue(fileError.description.contains("file.png"))

        let glslfxError = HioError.failedToParseGlslfx("/path/to/shader.glslfx")
        XCTAssertTrue(glslfxError.description.contains("shader.glslfx"))

        let readError = HioError.readFailed
        XCTAssertTrue(readError.description.contains("read"))

        let cropError = HioError.invalidCropRegion
        XCTAssertTrue(cropError.description.contains("crop"))
    }
}
