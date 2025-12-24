// Hio.swift - Image I/O wrapper
// Mirrors: pxr/imaging/hio/*.h (Image loading, OpenEXR, texture formats)

import Foundation
import OpenUSDInterop

// MARK: - Hio Namespace

/// Image I/O utilities for texture loading and shader effects.
/// Mirrors pxr::Hio from the USD imaging library.
///
/// The Hio module provides:
/// - Image file reading and writing (PNG, JPEG, EXR, TIFF, etc.)
/// - GLSL shader effects parsing (glslfx)
/// - Volumetric field texture data (OpenVDB, Field3D)
/// - Texture format utilities
///
/// ## Usage
/// ```swift
/// // Check if an image format is supported
/// if Hio.isSupportedImageFile("texture.exr") {
///     // Open and read image
///     if let image = try? HioImage(filename: "texture.exr") {
///         print("Size: \(image.width)x\(image.height)")
///     }
/// }
///
/// // Get supported formats
/// let formats = Hio.supportedImageFormats
/// ```
public enum Hio {

    // MARK: - Image Format Support

    /// Returns true if the filename has a supported image extension.
    public static func isSupportedImageFile(_ filename: String) -> Bool {
        Hio_IsSupportedImageFile(filename)
    }

    /// Returns the number of supported image format extensions.
    public static var supportedImageFormatCount: Int {
        Int(Hio_GetSupportedImageFormatCount())
    }

    /// Returns all supported image format extensions.
    public static var supportedImageFormats: [String] {
        let count = supportedImageFormatCount
        guard count > 0 else { return [] }

        var pointers = [UnsafeMutablePointer<CChar>?](repeating: nil, count: count)
        let actualCount = Hio_GetSupportedImageFormats(&pointers, count)

        var result = [String]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            if let ptr = pointers[i] {
                result.append(String(cString: ptr))
                Hio_FreeString(ptr)
            }
        }
        return result
    }
}

// MARK: - HioFormat

extension Hio {

    /// Pixel format enumeration matching pxr::HioFormat.
    public enum Format: UInt32, Sendable, CustomStringConvertible {
        case invalid = 0

        // UNorm8 formats (8-bit normalized unsigned)
        case unorm8 = 1
        case unorm8Vec2 = 2
        case unorm8Vec3 = 3
        case unorm8Vec4 = 4

        // SNorm8 formats (8-bit normalized signed)
        case snorm8 = 5
        case snorm8Vec2 = 6
        case snorm8Vec3 = 7
        case snorm8Vec4 = 8

        // Float16 formats (half-precision)
        case float16 = 9
        case float16Vec2 = 10
        case float16Vec3 = 11
        case float16Vec4 = 12

        // Float32 formats (single-precision)
        case float32 = 13
        case float32Vec2 = 14
        case float32Vec3 = 15
        case float32Vec4 = 16

        // Double64 formats (double-precision)
        case double64 = 17
        case double64Vec2 = 18
        case double64Vec3 = 19
        case double64Vec4 = 20

        // UInt16 formats
        case uint16 = 21
        case uint16Vec2 = 22
        case uint16Vec3 = 23
        case uint16Vec4 = 24

        // Int16 formats
        case int16 = 25
        case int16Vec2 = 26
        case int16Vec3 = 27
        case int16Vec4 = 28

        // UInt32 formats
        case uint32 = 29
        case uint32Vec2 = 30
        case uint32Vec3 = 31
        case uint32Vec4 = 32

        // Int32 formats
        case int32 = 33
        case int32Vec2 = 34
        case int32Vec3 = 35
        case int32Vec4 = 36

        // sRGB formats
        case unorm8SRGB = 37
        case unorm8Vec2SRGB = 38
        case unorm8Vec3SRGB = 39
        case unorm8Vec4SRGB = 40

        // Compressed formats (BC/DXT)
        case bc6FloatVec3 = 41
        case bc6UFloatVec3 = 42
        case bc7Unorm8Vec4 = 43
        case bc7Unorm8Vec4SRGB = 44
        case bc1Unorm8Vec4 = 45
        case bc3Unorm8Vec4 = 46

        public var description: String {
            switch self {
            case .invalid: return "Invalid"
            case .unorm8: return "UNorm8"
            case .unorm8Vec2: return "UNorm8Vec2"
            case .unorm8Vec3: return "UNorm8Vec3"
            case .unorm8Vec4: return "UNorm8Vec4"
            case .snorm8: return "SNorm8"
            case .snorm8Vec2: return "SNorm8Vec2"
            case .snorm8Vec3: return "SNorm8Vec3"
            case .snorm8Vec4: return "SNorm8Vec4"
            case .float16: return "Float16"
            case .float16Vec2: return "Float16Vec2"
            case .float16Vec3: return "Float16Vec3"
            case .float16Vec4: return "Float16Vec4"
            case .float32: return "Float32"
            case .float32Vec2: return "Float32Vec2"
            case .float32Vec3: return "Float32Vec3"
            case .float32Vec4: return "Float32Vec4"
            case .double64: return "Double64"
            case .double64Vec2: return "Double64Vec2"
            case .double64Vec3: return "Double64Vec3"
            case .double64Vec4: return "Double64Vec4"
            case .uint16: return "UInt16"
            case .uint16Vec2: return "UInt16Vec2"
            case .uint16Vec3: return "UInt16Vec3"
            case .uint16Vec4: return "UInt16Vec4"
            case .int16: return "Int16"
            case .int16Vec2: return "Int16Vec2"
            case .int16Vec3: return "Int16Vec3"
            case .int16Vec4: return "Int16Vec4"
            case .uint32: return "UInt32"
            case .uint32Vec2: return "UInt32Vec2"
            case .uint32Vec3: return "UInt32Vec3"
            case .uint32Vec4: return "UInt32Vec4"
            case .int32: return "Int32"
            case .int32Vec2: return "Int32Vec2"
            case .int32Vec3: return "Int32Vec3"
            case .int32Vec4: return "Int32Vec4"
            case .unorm8SRGB: return "UNorm8sRGB"
            case .unorm8Vec2SRGB: return "UNorm8Vec2sRGB"
            case .unorm8Vec3SRGB: return "UNorm8Vec3sRGB"
            case .unorm8Vec4SRGB: return "UNorm8Vec4sRGB"
            case .bc6FloatVec3: return "BC6FloatVec3"
            case .bc6UFloatVec3: return "BC6UFloatVec3"
            case .bc7Unorm8Vec4: return "BC7UNorm8Vec4"
            case .bc7Unorm8Vec4SRGB: return "BC7UNorm8Vec4sRGB"
            case .bc1Unorm8Vec4: return "BC1UNorm8Vec4"
            case .bc3Unorm8Vec4: return "BC3UNorm8Vec4"
            }
        }

        /// Returns the number of components in this format.
        public var componentCount: Int {
            Int(HioGetComponentCount(HioFormat(rawValue: rawValue)))
        }

        /// Returns the size in bytes of a single pixel.
        public var bytesPerPixel: Int {
            Int(HioGetDataSizeOfFormat(HioFormat(rawValue: rawValue)))
        }

        /// Returns true if this is a compressed format.
        public var isCompressed: Bool {
            HioIsCompressed(HioFormat(rawValue: rawValue))
        }

        /// Returns the data type for this format.
        public var dataType: DataType {
            DataType(rawValue: HioGetType(HioFormat(rawValue: rawValue)).rawValue) ?? .unsignedByte
        }

        /// Calculates the total data size for given dimensions.
        public func dataSize(width: Int, height: Int, depth: Int = 1) -> Int {
            Int(HioGetDataSize(HioFormat(rawValue: rawValue), Int32(width), Int32(height), Int32(depth)))
        }
    }

    /// Component data type enumeration.
    public enum DataType: UInt32, Sendable {
        case unsignedByte = 0
        case unsignedByteSRGB = 1
        case signedByte = 2
        case unsignedShort = 3
        case signedShort = 4
        case unsignedInt = 5
        case int = 6
        case halfFloat = 7
        case float = 8
        case double = 9
    }

    /// Image origin location.
    public enum ImageOrigin: UInt32, Sendable {
        case upperLeft = 0
        case lowerLeft = 1
    }

    /// Source color space interpretation.
    public enum SourceColorSpace: UInt32, Sendable {
        case raw = 0
        case sRGB = 1
        case auto = 2
    }

    /// Texture address mode.
    public enum AddressMode: UInt32, Sendable {
        case clampToEdge = 0
        case mirrorClampToEdge = 1
        case `repeat` = 2
        case mirrorRepeat = 3
        case clampToBorderColor = 4
    }

    /// Gets a format from component count and data type.
    public static func getFormat(channels: Int, type: DataType, isSRGB: Bool = false) -> Format {
        let hioFormat = HioGetFormat(UInt32(channels), HioType(rawValue: UInt32(type.rawValue)), isSRGB)
        return Format(rawValue: hioFormat.rawValue) ?? .invalid
    }
}

// MARK: - HioImage

/// Represents an image file for reading or writing.
public final class HioImage: @unchecked Sendable {
    private let handle: HioImageRef

    private init(_ handle: HioImageRef) {
        self.handle = handle
    }

    deinit {
        HioImage_Release(handle)
    }

    /// Opens an image file for reading.
    /// - Parameters:
    ///   - filename: Path to the image file
    ///   - subimage: Subimage index (for multi-image files like EXR)
    ///   - mip: Mip level to read
    ///   - colorSpace: Source color space interpretation
    ///   - premultiplyAlpha: Whether to premultiply alpha
    /// - Throws: HioError if the file cannot be opened
    public convenience init(
        filename: String,
        subimage: Int = 0,
        mip: Int = 0,
        colorSpace: Hio.SourceColorSpace = .auto,
        premultiplyAlpha: Bool = false
    ) throws {
        guard let handle = HioImage_OpenForReading(
            filename,
            Int32(subimage),
            Int32(mip),
            HioSourceColorSpace(rawValue: UInt32(colorSpace.rawValue)),
            premultiplyAlpha
        ) else {
            throw HioError.failedToOpenFile(filename)
        }
        self.init(handle)
    }

    /// Opens an image file for writing.
    /// - Parameter filename: Path to write the image
    /// - Throws: HioError if the file cannot be created
    public static func openForWriting(_ filename: String) throws -> HioImage {
        guard let handle = HioImage_OpenForWriting(filename) else {
            throw HioError.failedToCreateFile(filename)
        }
        return HioImage(handle)
    }

    /// Returns true if the filename has a supported image extension.
    public static func isSupportedImageFile(_ filename: String) -> Bool {
        HioImage_IsSupportedImageFile(filename)
    }

    /// The image filename.
    public var filename: String {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard HioImage_GetFilename(handle, &strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return ""
        }
        defer { Hio_FreeString(str) }
        return String(cString: str)
    }

    /// The image width in pixels.
    public var width: Int {
        Int(HioImage_GetWidth(handle))
    }

    /// The image height in pixels.
    public var height: Int {
        Int(HioImage_GetHeight(handle))
    }

    /// The image depth (for 3D textures).
    public var depth: Int {
        Int(HioImage_GetDepth(handle))
    }

    /// The pixel format.
    public var format: Hio.Format {
        Hio.Format(rawValue: HioImage_GetFormat(handle).rawValue) ?? .invalid
    }

    /// The number of bytes per pixel.
    public var bytesPerPixel: Int {
        Int(HioImage_GetBytesPerPixel(handle))
    }

    /// The number of mip levels.
    public var numMipLevels: Int {
        Int(HioImage_GetNumMipLevels(handle))
    }

    /// Returns true if the image is in sRGB color space.
    public var isColorSpaceSRGB: Bool {
        HioImage_IsColorSpaceSRGB(handle)
    }

    /// Reads the image data into a buffer.
    /// - Returns: The image pixel data
    /// - Throws: HioError if reading fails
    public func read() throws -> Data {
        let size = format.dataSize(width: width, height: height, depth: depth)
        var data = Data(count: size)

        try data.withUnsafeMutableBytes { buffer in
            var spec = HioImageStorageSpec(
                width: Int32(width),
                height: Int32(height),
                depth: Int32(depth),
                format: HioFormat(rawValue: format.rawValue),
                flipped: false,
                data: buffer.baseAddress
            )

            let result = HioImage_Read(handle, &spec)
            guard result == USD_RESULT_SUCCESS else {
                throw HioError.readFailed
            }
        }

        return data
    }

    /// Reads a cropped region of the image.
    /// - Parameters:
    ///   - top: Pixels to crop from top
    ///   - bottom: Pixels to crop from bottom
    ///   - left: Pixels to crop from left
    ///   - right: Pixels to crop from right
    /// - Returns: The cropped image pixel data
    /// - Throws: HioError if reading fails
    public func readCropped(top: Int, bottom: Int, left: Int, right: Int) throws -> Data {
        let croppedWidth = width - left - right
        let croppedHeight = height - top - bottom

        guard croppedWidth > 0 && croppedHeight > 0 else {
            throw HioError.invalidCropRegion
        }

        let size = format.dataSize(width: croppedWidth, height: croppedHeight, depth: depth)
        var data = Data(count: size)

        try data.withUnsafeMutableBytes { buffer in
            var spec = HioImageStorageSpec(
                width: Int32(croppedWidth),
                height: Int32(croppedHeight),
                depth: Int32(depth),
                format: HioFormat(rawValue: format.rawValue),
                flipped: false,
                data: buffer.baseAddress
            )

            let result = HioImage_ReadCropped(
                handle,
                Int32(top), Int32(bottom), Int32(left), Int32(right),
                &spec
            )
            guard result == USD_RESULT_SUCCESS else {
                throw HioError.readFailed
            }
        }

        return data
    }

    /// Writes image data to the file.
    /// - Parameters:
    ///   - data: The pixel data to write
    ///   - width: Image width
    ///   - height: Image height
    ///   - depth: Image depth (for 3D textures)
    ///   - format: Pixel format
    ///   - flipped: Whether to flip the image vertically
    /// - Throws: HioError if writing fails
    public func write(
        data: Data,
        width: Int,
        height: Int,
        depth: Int = 1,
        format: Hio.Format,
        flipped: Bool = false
    ) throws {
        try data.withUnsafeBytes { buffer in
            var spec = HioImageStorageSpec(
                width: Int32(width),
                height: Int32(height),
                depth: Int32(depth),
                format: HioFormat(rawValue: format.rawValue),
                flipped: flipped,
                data: UnsafeMutableRawPointer(mutating: buffer.baseAddress)
            )

            let result = HioImage_Write(handle, &spec)
            guard result == USD_RESULT_SUCCESS else {
                throw HioError.writeFailed
            }
        }
    }
}

// MARK: - HioGlslfx

/// GLSL shader effects file parser.
/// Parses .glslfx files containing shader configuration and source code.
public final class HioGlslfx: @unchecked Sendable {
    private let handle: HioGlslfxRef

    private init(_ handle: HioGlslfxRef) {
        self.handle = handle
    }

    deinit {
        HioGlslfx_Release(handle)
    }

    /// Creates a Glslfx object from a file path.
    /// - Parameter filePath: Path to the .glslfx file
    /// - Throws: HioError if the file cannot be parsed
    public convenience init(filePath: String) throws {
        guard let handle = HioGlslfx_CreateFromFile(filePath) else {
            throw HioError.failedToParseGlslfx(filePath)
        }
        self.init(handle)
    }

    /// Creates a Glslfx object from a string.
    /// - Parameter source: The glslfx source code
    /// - Throws: HioError if the source cannot be parsed
    public convenience init(source: String) throws {
        guard let handle = HioGlslfx_CreateFromString(source) else {
            throw HioError.failedToParseGlslfxSource
        }
        self.init(handle)
    }

    /// Returns whether the Glslfx is valid.
    public var isValid: Bool {
        HioGlslfx_IsValid(handle)
    }

    /// The surface shader source code.
    public var surfaceSource: String {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard HioGlslfx_GetSurfaceSource(handle, &strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return ""
        }
        defer { Hio_FreeString(str) }
        return String(cString: str)
    }

    /// The displacement shader source code.
    public var displacementSource: String {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard HioGlslfx_GetDisplacementSource(handle, &strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return ""
        }
        defer { Hio_FreeString(str) }
        return String(cString: str)
    }

    /// The volume shader source code.
    public var volumeSource: String {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard HioGlslfx_GetVolumeSource(handle, &strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return ""
        }
        defer { Hio_FreeString(str) }
        return String(cString: str)
    }

    /// Gets shader source for a specific shader stage.
    /// - Parameter shaderStageKey: The shader stage key (e.g., "fragmentShader", "vertexShader")
    /// - Returns: The shader source code
    public func getSource(for shaderStageKey: String) -> String {
        var strPtr: UnsafeMutablePointer<CChar>?
        guard HioGlslfx_GetSource(handle, shaderStageKey, &strPtr) == USD_RESULT_SUCCESS,
              let str = strPtr else {
            return ""
        }
        defer { Hio_FreeString(str) }
        return String(cString: str)
    }

    /// The shader parameter names.
    public var parameterNames: [String] {
        let count = HioGlslfx_GetParameterCount(handle)
        guard count > 0 else { return [] }

        var pointers = [UnsafeMutablePointer<CChar>?](repeating: nil, count: Int(count))
        let actualCount = HioGlslfx_GetParameterNames(handle, &pointers, count)

        var result = [String]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            if let ptr = pointers[i] {
                result.append(String(cString: ptr))
                Hio_FreeString(ptr)
            }
        }
        return result
    }

    /// The shader texture names.
    public var textureNames: [String] {
        let count = HioGlslfx_GetTextureCount(handle)
        guard count > 0 else { return [] }

        var pointers = [UnsafeMutablePointer<CChar>?](repeating: nil, count: Int(count))
        let actualCount = HioGlslfx_GetTextureNames(handle, &pointers, count)

        var result = [String]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            if let ptr = pointers[i] {
                result.append(String(cString: ptr))
                Hio_FreeString(ptr)
            }
        }
        return result
    }

    /// The shader attribute names.
    public var attributeNames: [String] {
        let count = HioGlslfx_GetAttributeCount(handle)
        guard count > 0 else { return [] }

        var pointers = [UnsafeMutablePointer<CChar>?](repeating: nil, count: Int(count))
        let actualCount = HioGlslfx_GetAttributeNames(handle, &pointers, count)

        var result = [String]()
        result.reserveCapacity(Int(actualCount))
        for i in 0..<Int(actualCount) {
            if let ptr = pointers[i] {
                result.append(String(cString: ptr))
                Hio_FreeString(ptr)
            }
        }
        return result
    }

    /// A hash of the Glslfx contents.
    public var hash: Int {
        Int(HioGlslfx_GetHash(handle))
    }
}

// MARK: - HioFieldTextureData

/// Volumetric field texture data for OpenVDB and Field3D files.
public final class HioFieldTextureData: @unchecked Sendable {
    private let handle: HioFieldTextureDataRef

    private init(_ handle: HioFieldTextureDataRef) {
        self.handle = handle
    }

    deinit {
        HioFieldTextureData_Release(handle)
    }

    /// Creates field texture data from a file.
    /// - Parameters:
    ///   - filePath: Path to the volume file (OpenVDB or Field3D)
    ///   - fieldName: Name of the grid/layer
    ///   - fieldIndex: Partition index
    ///   - fieldPurpose: Partition name/grouping
    ///   - targetMemory: Target memory size for resizing (0 for no limit)
    /// - Throws: HioError if the file cannot be loaded
    public convenience init(
        filePath: String,
        fieldName: String = "",
        fieldIndex: Int = 0,
        fieldPurpose: String = "",
        targetMemory: Int = 0
    ) throws {
        guard let handle = HioFieldTextureData_Create(
            filePath,
            fieldName,
            Int32(fieldIndex),
            fieldPurpose,
            targetMemory
        ) else {
            throw HioError.failedToLoadFieldData(filePath)
        }
        self.init(handle)
    }

    /// The pixel format of the field data.
    public var format: Hio.Format {
        Hio.Format(rawValue: HioFieldTextureData_GetFormat(handle).rawValue) ?? .invalid
    }

    /// The resized width.
    public var width: Int {
        Int(HioFieldTextureData_GetResizedWidth(handle))
    }

    /// The resized height.
    public var height: Int {
        Int(HioFieldTextureData_GetResizedHeight(handle))
    }

    /// The resized depth.
    public var depth: Int {
        Int(HioFieldTextureData_GetResizedDepth(handle))
    }

    /// The bounding box minimum point.
    public var boundingBoxMin: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        HioFieldTextureData_GetBoundingBoxMin(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// The bounding box maximum point.
    public var boundingBoxMax: (x: Double, y: Double, z: Double) {
        var x: Double = 0, y: Double = 0, z: Double = 0
        HioFieldTextureData_GetBoundingBoxMax(handle, &x, &y, &z)
        return (x, y, z)
    }

    /// Returns true if a raw buffer is available.
    public var hasRawBuffer: Bool {
        HioFieldTextureData_HasRawBuffer(handle)
    }

    /// Reads the field texture data.
    /// - Throws: HioError if reading fails
    public func read() throws {
        let result = HioFieldTextureData_Read(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw HioError.readFailed
        }
    }

    /// Gets the raw buffer data.
    /// - Returns: The raw buffer data, or nil if not available
    public func getRawBuffer() -> Data? {
        var buffer: UnsafeRawPointer?
        var size: Int = 0
        guard HioFieldTextureData_GetRawBuffer(handle, &buffer, &size),
              let bufferPtr = buffer else {
            return nil
        }
        return Data(bytes: bufferPtr, count: size)
    }
}

// MARK: - Errors

/// Errors that can occur in Hio operations.
public enum HioError: Error, CustomStringConvertible {
    case failedToOpenFile(String)
    case failedToCreateFile(String)
    case failedToParseGlslfx(String)
    case failedToParseGlslfxSource
    case failedToLoadFieldData(String)
    case readFailed
    case writeFailed
    case invalidCropRegion

    public var description: String {
        switch self {
        case .failedToOpenFile(let path):
            return "Failed to open image file: \(path)"
        case .failedToCreateFile(let path):
            return "Failed to create image file: \(path)"
        case .failedToParseGlslfx(let path):
            return "Failed to parse glslfx file: \(path)"
        case .failedToParseGlslfxSource:
            return "Failed to parse glslfx source"
        case .failedToLoadFieldData(let path):
            return "Failed to load field texture data: \(path)"
        case .readFailed:
            return "Failed to read image data"
        case .writeFailed:
            return "Failed to write image data"
        case .invalidCropRegion:
            return "Invalid crop region"
        }
    }
}
