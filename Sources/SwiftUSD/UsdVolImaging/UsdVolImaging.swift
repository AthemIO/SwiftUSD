// UsdVolImaging.swift - Swift wrapper for UsdVolImaging module
// Provides volume rendering adapter support for Hydra

import USDCxx

// MARK: - Type Aliases

/// OpenVDB asset imaging adapter for rendering OpenVDB volume fields
public typealias UsdVolImagingOpenVDBAssetAdapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef

/// Field3D asset imaging adapter for rendering Field3D volume fields
public typealias UsdVolImagingField3DAssetAdapter = swiftusd.UsdVolImagingField3DAssetAdapterRef

/// Volume field imaging parameters
public typealias UsdVolImagingFieldParams = swiftusd.UsdVolImagingFieldParams

// MARK: - UsdVolImagingOpenVDBAssetAdapter Extensions

extension swiftusd.UsdVolImagingOpenVDBAssetAdapterRef {
    /// Create a new OpenVDB asset adapter
    public static func create() -> swiftusd.UsdVolImagingOpenVDBAssetAdapterRef? {
        return swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.Create()
    }

    /// The prim type this adapter handles
    public var primType: swiftusd.Token {
        return GetPrimTypeToken()
    }

    /// The imaging token for this field type
    public var imagingToken: swiftusd.Token {
        return GetImagingToken()
    }

    /// Check if a file extension is supported
    public func supportsExtension(_ ext: String) -> Bool {
        return IsExtensionSupported(ext)
    }

    /// Get supported file extensions
    public var supportedExtensions: [swiftusd.Token] {
        var tokens = [swiftusd.Token](repeating: swiftusd.Token(), count: 4)
        let count = tokens.withUnsafeMutableBufferPointer { buffer in
            GetSupportedExtensions(buffer.baseAddress, buffer.count)
        }
        return Array(tokens.prefix(Int(count)))
    }
}

// MARK: - UsdVolImagingField3DAssetAdapter Extensions

extension swiftusd.UsdVolImagingField3DAssetAdapterRef {
    /// Create a new Field3D asset adapter
    public static func create() -> swiftusd.UsdVolImagingField3DAssetAdapterRef? {
        return swiftusd.UsdVolImagingField3DAssetAdapterRef.Create()
    }

    /// The prim type this adapter handles
    public var primType: swiftusd.Token {
        return GetPrimTypeToken()
    }

    /// The imaging token for this field type
    public var imagingToken: swiftusd.Token {
        return GetImagingToken()
    }

    /// Check if a file extension is supported
    public func supportsExtension(_ ext: String) -> Bool {
        return IsExtensionSupported(ext)
    }

    /// Get supported file extensions
    public var supportedExtensions: [swiftusd.Token] {
        var tokens = [swiftusd.Token](repeating: swiftusd.Token(), count: 4)
        let count = tokens.withUnsafeMutableBufferPointer { buffer in
            GetSupportedExtensions(buffer.baseAddress, buffer.count)
        }
        return Array(tokens.prefix(Int(count)))
    }
}

// MARK: - UsdVolImagingFieldParams Extensions

extension swiftusd.UsdVolImagingFieldParams {
    /// Check if the field parameters are valid
    public var isValid: Bool {
        return IsValid()
    }

    /// The file path as a Swift String
    public var filePathString: String {
        return withUnsafePointer(to: filePath) { ptr in
            ptr.withMemoryRebound(to: CChar.self, capacity: 512) { charPtr in
                String(cString: charPtr)
            }
        }
    }

    /// The field name as a Swift String
    public var fieldNameString: String {
        return withUnsafePointer(to: fieldName) { ptr in
            ptr.withMemoryRebound(to: CChar.self, capacity: 128) { charPtr in
                String(cString: charPtr)
            }
        }
    }

    /// Create field params with default values
    public static func defaultParams() -> swiftusd.UsdVolImagingFieldParams {
        return swiftusd.UsdVolImagingFieldParams()
    }
}

// MARK: - Token Helpers

/// UsdVolImaging tokens namespace
public enum UsdVolImagingTokens {
    /// Field asset type tokens
    public static var field3DAsset: swiftusd.Token { swiftusd.UsdVolImagingTokens.Field3DAsset() }
    public static var openVDBAsset: swiftusd.Token { swiftusd.UsdVolImagingTokens.OpenVDBAsset() }

    /// Volume tokens
    public static var volume: swiftusd.Token { swiftusd.UsdVolImagingTokens.Volume() }
    public static var field: swiftusd.Token { swiftusd.UsdVolImagingTokens.Field() }

    /// Field attribute tokens
    public static var filePath: swiftusd.Token { swiftusd.UsdVolImagingTokens.FilePath() }
    public static var fieldName: swiftusd.Token { swiftusd.UsdVolImagingTokens.FieldName() }
    public static var fieldIndex: swiftusd.Token { swiftusd.UsdVolImagingTokens.FieldIndex() }
    public static var fieldDataType: swiftusd.Token { swiftusd.UsdVolImagingTokens.FieldDataType() }
    public static var vectorDataRoleHint: swiftusd.Token { swiftusd.UsdVolImagingTokens.VectorDataRoleHint() }

    /// OpenVDB-specific tokens
    public static var fieldClass: swiftusd.Token { swiftusd.UsdVolImagingTokens.FieldClass() }
    public static var levelSet: swiftusd.Token { swiftusd.UsdVolImagingTokens.LevelSet() }
    public static var fogVolume: swiftusd.Token { swiftusd.UsdVolImagingTokens.FogVolume() }
    public static var staggered: swiftusd.Token { swiftusd.UsdVolImagingTokens.Staggered() }
    public static var unknown: swiftusd.Token { swiftusd.UsdVolImagingTokens.Unknown() }

    /// Field3D-specific tokens
    public static var fieldPurpose: swiftusd.Token { swiftusd.UsdVolImagingTokens.FieldPurpose() }

    /// Data type tokens
    public static var half: swiftusd.Token { swiftusd.UsdVolImagingTokens.Half() }
    public static var float: swiftusd.Token { swiftusd.UsdVolImagingTokens.Float() }
    public static var double: swiftusd.Token { swiftusd.UsdVolImagingTokens.Double() }
    public static var half3: swiftusd.Token { swiftusd.UsdVolImagingTokens.Half3() }
    public static var float3: swiftusd.Token { swiftusd.UsdVolImagingTokens.Float3() }
    public static var double3: swiftusd.Token { swiftusd.UsdVolImagingTokens.Double3() }
}

// MARK: - Utility Functions

/// Get the field asset type token for a given file extension
public func usdVolImagingGetFieldTypeForExtension(_ extension: String) -> swiftusd.Token {
    return swiftusd.UsdVolImagingGetFieldTypeForExtension(`extension`)
}

/// Check if a file extension is a supported volume format
public func usdVolImagingIsVolumeExtension(_ extension: String) -> Bool {
    return swiftusd.UsdVolImagingIsVolumeExtension(`extension`)
}

/// Get the volume prim type token
public func usdVolImagingGetVolumePrimType() -> swiftusd.Token {
    return swiftusd.UsdVolImagingGetVolumePrimType()
}

/// Check if a token represents a volume field prim type
public func usdVolImagingIsFieldPrimType(_ primType: swiftusd.Token) -> Bool {
    return swiftusd.UsdVolImagingIsFieldPrimType(primType)
}

/// Get the field relationship prefix used in Volume prims
public func usdVolImagingGetFieldRelationshipPrefix() -> swiftusd.Token {
    return swiftusd.UsdVolImagingGetFieldRelationshipPrefix()
}
