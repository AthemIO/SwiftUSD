// HioCxx.swift - Swift convenience layer for Hio (Hydra Image I/O)
// Provides idiomatic Swift API for image loading and texture support

import USDCxx

// MARK: - Type Aliases

/// Image handle for reading and writing texture images.
public typealias HioImage = swiftusd.HioImageRef

/// Image storage specification describing memory layout.
public typealias HioStorageSpec = swiftusd.HioStorageSpec

// MARK: - HioFormat Convenience

/// Pixel format for image buffers.
public typealias HioFormat = swiftusd.HioFormat

extension swiftusd.HioFormat: CustomStringConvertible {
    public var description: String {
        switch self {
        case .Invalid: return "Invalid"
        case .UNorm8: return "UNorm8"
        case .UNorm8Vec2: return "UNorm8Vec2"
        case .UNorm8Vec3: return "UNorm8Vec3"
        case .UNorm8Vec4: return "UNorm8Vec4"
        case .SNorm8: return "SNorm8"
        case .SNorm8Vec2: return "SNorm8Vec2"
        case .SNorm8Vec3: return "SNorm8Vec3"
        case .SNorm8Vec4: return "SNorm8Vec4"
        case .Float16: return "Float16"
        case .Float16Vec2: return "Float16Vec2"
        case .Float16Vec3: return "Float16Vec3"
        case .Float16Vec4: return "Float16Vec4"
        case .Float32: return "Float32"
        case .Float32Vec2: return "Float32Vec2"
        case .Float32Vec3: return "Float32Vec3"
        case .Float32Vec4: return "Float32Vec4"
        case .Double64: return "Double64"
        case .Double64Vec2: return "Double64Vec2"
        case .Double64Vec3: return "Double64Vec3"
        case .Double64Vec4: return "Double64Vec4"
        case .UInt16: return "UInt16"
        case .UInt16Vec2: return "UInt16Vec2"
        case .UInt16Vec3: return "UInt16Vec3"
        case .UInt16Vec4: return "UInt16Vec4"
        case .Int16: return "Int16"
        case .Int16Vec2: return "Int16Vec2"
        case .Int16Vec3: return "Int16Vec3"
        case .Int16Vec4: return "Int16Vec4"
        case .UInt32: return "UInt32"
        case .UInt32Vec2: return "UInt32Vec2"
        case .UInt32Vec3: return "UInt32Vec3"
        case .UInt32Vec4: return "UInt32Vec4"
        case .Int32: return "Int32"
        case .Int32Vec2: return "Int32Vec2"
        case .Int32Vec3: return "Int32Vec3"
        case .Int32Vec4: return "Int32Vec4"
        case .UNorm8srgb: return "UNorm8srgb"
        case .UNorm8Vec2srgb: return "UNorm8Vec2srgb"
        case .UNorm8Vec3srgb: return "UNorm8Vec3srgb"
        case .UNorm8Vec4srgb: return "UNorm8Vec4srgb"
        case .BC6FloatVec3: return "BC6FloatVec3"
        case .BC6UFloatVec3: return "BC6UFloatVec3"
        case .BC7UNorm8Vec4: return "BC7UNorm8Vec4"
        case .BC7UNorm8Vec4srgb: return "BC7UNorm8Vec4srgb"
        case .BC1UNorm8Vec4: return "BC1UNorm8Vec4"
        case .BC3UNorm8Vec4: return "BC3UNorm8Vec4"
        case .Count: return "Count"
        @unknown default: return "Unknown(\(rawValue))"
        }
    }

    /// Number of components (channels) in this format.
    public var componentCount: Int {
        return Int(swiftusd.HioGetComponentCount(self))
    }

    /// Size in bytes per pixel for this format.
    public var bytesPerPixel: Int {
        return Int(swiftusd.HioGetDataSizeOfFormat(self))
    }

    /// Whether this format uses block compression.
    public var isCompressed: Bool {
        return swiftusd.HioIsCompressed(self)
    }
}

// MARK: - HioType Convenience

/// Component data type for image pixels.
public typealias HioType = swiftusd.HioType

extension swiftusd.HioType: CustomStringConvertible {
    public var description: String {
        switch self {
        case .UnsignedByte: return "UnsignedByte"
        case .UnsignedByteSRGB: return "UnsignedByteSRGB"
        case .SignedByte: return "SignedByte"
        case .UnsignedShort: return "UnsignedShort"
        case .SignedShort: return "SignedShort"
        case .UnsignedInt: return "UnsignedInt"
        case .Int: return "Int"
        case .HalfFloat: return "HalfFloat"
        case .Float: return "Float"
        case .Double: return "Double"
        case .Count: return "Count"
        @unknown default: return "Unknown(\(rawValue))"
        }
    }

    /// Size in bytes for a single component of this type.
    public var byteSize: Swift.Int {
        return Swift.Int(swiftusd.HioGetDataSizeOfType(self))
    }
}

// MARK: - HioAddressDimension

/// Texture sampling dimension.
public typealias HioAddressDimension = swiftusd.HioAddressDimension

extension swiftusd.HioAddressDimension: CustomStringConvertible {
    public var description: String {
        switch self {
        case .U: return "U"
        case .V: return "V"
        case .W: return "W"
        @unknown default: return "Unknown(\(rawValue))"
        }
    }
}

// MARK: - HioAddressMode

/// Texture address (wrap) mode.
public typealias HioAddressMode = swiftusd.HioAddressMode

extension swiftusd.HioAddressMode: CustomStringConvertible {
    public var description: String {
        switch self {
        case .ClampToEdge: return "ClampToEdge"
        case .MirrorClampToEdge: return "MirrorClampToEdge"
        case .Repeat: return "Repeat"
        case .MirrorRepeat: return "MirrorRepeat"
        case .ClampToBorderColor: return "ClampToBorderColor"
        @unknown default: return "Unknown(\(rawValue))"
        }
    }
}

// MARK: - HioImageOriginLocation

/// Image origin location (top-left or bottom-left).
public typealias HioImageOriginLocation = swiftusd.HioImageOriginLocation

extension swiftusd.HioImageOriginLocation: CustomStringConvertible {
    public var description: String {
        switch self {
        case .UpperLeft: return "UpperLeft"
        case .LowerLeft: return "LowerLeft"
        @unknown default: return "Unknown(\(rawValue))"
        }
    }
}

// MARK: - HioSourceColorSpace

/// Source color space for texture interpretation.
public typealias HioSourceColorSpace = swiftusd.HioSourceColorSpace

extension swiftusd.HioSourceColorSpace: CustomStringConvertible {
    public var description: String {
        switch self {
        case .Raw: return "Raw"
        case .SRGB: return "SRGB"
        case .Auto: return "Auto"
        @unknown default: return "Unknown(\(rawValue))"
        }
    }
}

// MARK: - HioImageRef Extensions

extension swiftusd.HioImageRef {
    /// Open an image file for reading with Swift string.
    ///
    /// - Parameters:
    ///   - filename: Path to the image file
    ///   - subimage: Subimage index for multi-part images (default: 0)
    ///   - mip: Mip level to read (default: 0)
    ///   - colorSpace: Color space hint (default: .Auto)
    ///   - suppressErrors: If true, don't log errors (default: false)
    /// - Returns: An HioImageRef if successful, nil otherwise
    public static func open(
        forReading filename: String,
        subimage: Int32 = 0,
        mip: Int32 = 0,
        colorSpace: HioSourceColorSpace = .Auto,
        suppressErrors: Bool = false
    ) -> swiftusd.HioImageRef? {
        return filename.withCString { cstr in
            return OpenForReading(cstr, subimage, mip, colorSpace, suppressErrors)
        }
    }

    /// Open an image file for writing with Swift string.
    ///
    /// - Parameter filename: Path to the output image file
    /// - Returns: An HioImageRef if successful, nil otherwise
    public static func open(forWriting filename: String) -> swiftusd.HioImageRef? {
        return filename.withCString { cstr in
            return OpenForWriting(cstr)
        }
    }

    /// Check if a filename is a supported image format.
    ///
    /// - Parameter filename: Path to check
    /// - Returns: true if the file extension indicates a supported format
    public static func isSupportedImageFile(_ filename: String) -> Bool {
        return filename.withCString { cstr in
            return IsSupportedImageFile(cstr)
        }
    }

    /// The filename of this image as a Swift String.
    public var filename: String {
        return String(cString: GetFilename())
    }

    /// The width of the image in pixels.
    public var width: Int {
        return Int(GetWidth())
    }

    /// The height of the image in pixels.
    public var height: Int {
        return Int(GetHeight())
    }

    /// The pixel format of the image.
    public var format: HioFormat {
        return GetFormat()
    }

    /// The number of bytes per pixel.
    public var bytesPerPixel: Int {
        return Int(GetBytesPerPixel())
    }

    /// The number of mip levels available.
    public var mipLevelCount: Int {
        return Int(GetNumMipLevels())
    }

    /// Whether the image is in sRGB color space.
    public var isSRGB: Bool {
        return IsColorSpaceSRGB()
    }

    /// The dimensions of the image as a tuple.
    public var dimensions: (width: Int, height: Int) {
        return (width, height)
    }

    /// Calculate the total byte size for image data.
    public var dataSize: Int {
        return Int(swiftusd.HioGetDataSize(format, Int32(width), Int32(height), 1))
    }
}

// MARK: - HioStorageSpec Extensions

extension swiftusd.HioStorageSpec {
    /// Create a storage spec for a 2D image.
    ///
    /// - Parameters:
    ///   - width: Image width in pixels
    ///   - height: Image height in pixels
    ///   - format: Pixel format
    ///   - flipped: Whether the image is vertically flipped
    public init(width: Int, height: Int, format: HioFormat, flipped: Bool = false) {
        self.init(Int32(width), Int32(height), format)
        self.flipped = flipped
    }

    /// Calculate the required buffer size for this storage spec.
    public var requiredBufferSize: Int {
        return Int(swiftusd.HioGetDataSize(format, width, height, depth))
    }
}

// MARK: - Utility Functions

/// Get the HioFormat for a given number of channels and type.
///
/// - Parameters:
///   - channels: Number of color channels (1-4)
///   - type: Component data type
///   - isSRGB: Whether to use sRGB encoding
/// - Returns: The corresponding HioFormat
public func hioFormat(channels: Int, type: HioType, isSRGB: Bool = false) -> HioFormat {
    return swiftusd.HioGetFormat(UInt32(channels), type, isSRGB)
}

/// Get the HioType for a given format.
///
/// - Parameter format: The pixel format
/// - Returns: The component type used by the format
public func hioType(for format: HioFormat) -> HioType {
    return swiftusd.HioGetType(format)
}

/// Calculate the byte size for image data.
///
/// - Parameters:
///   - format: Pixel format
///   - width: Image width
///   - height: Image height
///   - depth: Image depth (default: 1 for 2D images)
/// - Returns: Total byte size required
public func hioDataSize(format: HioFormat, width: Int, height: Int, depth: Int = 1) -> Int {
    return Int(swiftusd.HioGetDataSize(format, Int32(width), Int32(height), Int32(depth)))
}
