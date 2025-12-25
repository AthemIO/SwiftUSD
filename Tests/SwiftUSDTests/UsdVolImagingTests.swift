// UsdVolImagingTests.swift - Tests for UsdVolImaging module

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdVolImagingTests: XCTestCase {

    // MARK: - OpenVDBAssetAdapter Tests

    func testOpenVDBAssetAdapterCreation() {
        let adapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create()
        XCTAssertNotNil(adapter, "Should be able to create OpenVDB asset adapter")
        XCTAssertTrue(adapter?.IsValid() ?? false, "Adapter should be valid")
    }

    func testOpenVDBAssetAdapterPrimType() {
        guard let adapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create() else {
            XCTFail("Failed to create OpenVDB asset adapter")
            return
        }

        let primType = adapter.GetPrimTypeToken()
        XCTAssertEqual(String(cString: primType.GetText()), "OpenVDBAsset",
                       "Prim type should be 'OpenVDBAsset'")
    }

    func testOpenVDBAssetAdapterImagingToken() {
        guard let adapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create() else {
            XCTFail("Failed to create OpenVDB asset adapter")
            return
        }

        let token = adapter.GetImagingToken()
        XCTAssertEqual(String(cString: token.GetText()), "openvdbAsset",
                       "Imaging token should be 'openvdbAsset'")
    }

    func testOpenVDBAssetAdapterIsNotInstanceable() {
        guard let adapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create() else {
            XCTFail("Failed to create OpenVDB asset adapter")
            return
        }

        XCTAssertFalse(adapter.IsInstanceable(),
                       "Field adapters should not support instancing")
    }

    func testOpenVDBAssetAdapterSupportedExtensions() {
        guard let adapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create() else {
            XCTFail("Failed to create OpenVDB asset adapter")
            return
        }

        XCTAssertTrue(adapter.IsExtensionSupported("vdb"),
                      "Should support .vdb extension")
        XCTAssertTrue(adapter.IsExtensionSupported("VDB"),
                      "Should support .VDB extension (case insensitive)")
        XCTAssertFalse(adapter.IsExtensionSupported("f3d"),
                       "Should not support .f3d extension")
    }

    func testOpenVDBAssetAdapterExtensionList() {
        guard let adapter = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create() else {
            XCTFail("Failed to create OpenVDB asset adapter")
            return
        }

        let extensions = adapter.supportedExtensions
        XCTAssertEqual(extensions.count, 1, "Should have 1 supported extension")
        XCTAssertEqual(String(cString: extensions[0].GetText()), "vdb")
    }

    // MARK: - Field3DAssetAdapter Tests

    func testField3DAssetAdapterCreation() {
        let adapter = swiftusd.UsdVolImagingField3DAssetAdapterRef.create()
        XCTAssertNotNil(adapter, "Should be able to create Field3D asset adapter")
        XCTAssertTrue(adapter?.IsValid() ?? false, "Adapter should be valid")
    }

    func testField3DAssetAdapterPrimType() {
        guard let adapter = swiftusd.UsdVolImagingField3DAssetAdapterRef.create() else {
            XCTFail("Failed to create Field3D asset adapter")
            return
        }

        let primType = adapter.GetPrimTypeToken()
        XCTAssertEqual(String(cString: primType.GetText()), "Field3DAsset",
                       "Prim type should be 'Field3DAsset'")
    }

    func testField3DAssetAdapterImagingToken() {
        guard let adapter = swiftusd.UsdVolImagingField3DAssetAdapterRef.create() else {
            XCTFail("Failed to create Field3D asset adapter")
            return
        }

        let token = adapter.GetImagingToken()
        XCTAssertEqual(String(cString: token.GetText()), "field3dAsset",
                       "Imaging token should be 'field3dAsset'")
    }

    func testField3DAssetAdapterIsNotInstanceable() {
        guard let adapter = swiftusd.UsdVolImagingField3DAssetAdapterRef.create() else {
            XCTFail("Failed to create Field3D asset adapter")
            return
        }

        XCTAssertFalse(adapter.IsInstanceable(),
                       "Field adapters should not support instancing")
    }

    func testField3DAssetAdapterSupportedExtensions() {
        guard let adapter = swiftusd.UsdVolImagingField3DAssetAdapterRef.create() else {
            XCTFail("Failed to create Field3D asset adapter")
            return
        }

        XCTAssertTrue(adapter.IsExtensionSupported("f3d"),
                      "Should support .f3d extension")
        XCTAssertTrue(adapter.IsExtensionSupported("F3D"),
                      "Should support .F3D extension (case insensitive)")
        XCTAssertFalse(adapter.IsExtensionSupported("vdb"),
                       "Should not support .vdb extension")
    }

    func testField3DAssetAdapterExtensionList() {
        guard let adapter = swiftusd.UsdVolImagingField3DAssetAdapterRef.create() else {
            XCTFail("Failed to create Field3D asset adapter")
            return
        }

        let extensions = adapter.supportedExtensions
        XCTAssertEqual(extensions.count, 1, "Should have 1 supported extension")
        XCTAssertEqual(String(cString: extensions[0].GetText()), "f3d")
    }

    // MARK: - FieldParams Tests

    func testFieldParamsDefault() {
        let params = swiftusd.UsdVolImagingFieldParams.defaultParams()
        XCTAssertFalse(params.isValid, "Default params should not be valid")
        XCTAssertEqual(params.fieldIndex, 0, "Default field index should be 0")
    }

    func testFieldParamsEquality() {
        let params1 = swiftusd.UsdVolImagingFieldParams.defaultParams()
        let params2 = swiftusd.UsdVolImagingFieldParams.defaultParams()

        XCTAssertTrue(params1 == params2, "Default params should be equal")
    }

    // MARK: - Token Tests

    func testUsdVolImagingTokens() {
        XCTAssertEqual(String(cString: UsdVolImagingTokens.field3DAsset.GetText()), "field3dAsset")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.openVDBAsset.GetText()), "openvdbAsset")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.volume.GetText()), "Volume")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.field.GetText()), "field")
    }

    func testFieldAttributeTokens() {
        XCTAssertEqual(String(cString: UsdVolImagingTokens.filePath.GetText()), "filePath")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.fieldName.GetText()), "fieldName")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.fieldIndex.GetText()), "fieldIndex")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.fieldDataType.GetText()), "fieldDataType")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.vectorDataRoleHint.GetText()), "vectorDataRoleHint")
    }

    func testOpenVDBSpecificTokens() {
        XCTAssertEqual(String(cString: UsdVolImagingTokens.fieldClass.GetText()), "fieldClass")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.levelSet.GetText()), "levelSet")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.fogVolume.GetText()), "fogVolume")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.staggered.GetText()), "staggered")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.unknown.GetText()), "unknown")
    }

    func testField3DSpecificTokens() {
        XCTAssertEqual(String(cString: UsdVolImagingTokens.fieldPurpose.GetText()), "fieldPurpose")
    }

    func testDataTypeTokens() {
        XCTAssertEqual(String(cString: UsdVolImagingTokens.half.GetText()), "half")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.float.GetText()), "float")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.double.GetText()), "double")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.half3.GetText()), "half3")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.float3.GetText()), "float3")
        XCTAssertEqual(String(cString: UsdVolImagingTokens.double3.GetText()), "double3")
    }

    // MARK: - Utility Function Tests

    func testGetFieldTypeForExtension() {
        let vdbType = usdVolImagingGetFieldTypeForExtension("vdb")
        XCTAssertEqual(String(cString: vdbType.GetText()), "openvdbAsset",
                       "vdb should map to openvdbAsset")

        let f3dType = usdVolImagingGetFieldTypeForExtension("f3d")
        XCTAssertEqual(String(cString: f3dType.GetText()), "field3dAsset",
                       "f3d should map to field3dAsset")

        let unknownType = usdVolImagingGetFieldTypeForExtension("unknown")
        XCTAssertTrue(unknownType.IsEmpty(), "Unknown extension should return empty token")
    }

    func testIsVolumeExtension() {
        XCTAssertTrue(usdVolImagingIsVolumeExtension("vdb"),
                      "vdb should be a volume extension")
        XCTAssertTrue(usdVolImagingIsVolumeExtension("VDB"),
                      "VDB should be a volume extension")
        XCTAssertTrue(usdVolImagingIsVolumeExtension("f3d"),
                      "f3d should be a volume extension")
        XCTAssertTrue(usdVolImagingIsVolumeExtension("F3D"),
                      "F3D should be a volume extension")
        XCTAssertFalse(usdVolImagingIsVolumeExtension("jpg"),
                       "jpg should not be a volume extension")
        XCTAssertFalse(usdVolImagingIsVolumeExtension("usd"),
                       "usd should not be a volume extension")
    }

    func testGetVolumePrimType() {
        let volumeType = usdVolImagingGetVolumePrimType()
        XCTAssertEqual(String(cString: volumeType.GetText()), "Volume",
                       "Volume prim type should be 'Volume'")
    }

    func testIsFieldPrimType() {
        XCTAssertTrue(usdVolImagingIsFieldPrimType(swiftusd.Token("OpenVDBAsset")),
                      "OpenVDBAsset should be a field prim type")
        XCTAssertTrue(usdVolImagingIsFieldPrimType(swiftusd.Token("Field3DAsset")),
                      "Field3DAsset should be a field prim type")
        XCTAssertTrue(usdVolImagingIsFieldPrimType(swiftusd.Token("FieldAsset")),
                      "FieldAsset should be a field prim type")
        XCTAssertFalse(usdVolImagingIsFieldPrimType(swiftusd.Token("Mesh")),
                       "Mesh should not be a field prim type")
    }

    func testGetFieldRelationshipPrefix() {
        let prefix = usdVolImagingGetFieldRelationshipPrefix()
        XCTAssertEqual(String(cString: prefix.GetText()), "field",
                       "Field relationship prefix should be 'field'")
    }

    // MARK: - Reference Counting Tests

    func testOpenVDBAssetAdapterRetainRelease() {
        var adapter: swiftusd.UsdVolImagingOpenVDBAssetAdapterRef? = swiftusd.UsdVolImagingOpenVDBAssetAdapterRef.create()
        XCTAssertNotNil(adapter)

        // Retain
        adapter?.retain()

        // Release twice (once for retain, once for original)
        adapter?.release()
        adapter?.release()

        adapter = nil
    }

    func testField3DAssetAdapterRetainRelease() {
        var adapter: swiftusd.UsdVolImagingField3DAssetAdapterRef? = swiftusd.UsdVolImagingField3DAssetAdapterRef.create()
        XCTAssertNotNil(adapter)

        adapter?.retain()
        adapter?.release()
        adapter?.release()

        adapter = nil
    }
}
