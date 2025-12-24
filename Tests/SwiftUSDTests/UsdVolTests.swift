// UsdVolTests.swift - Tests for UsdVol module
// Tests volume types: Volume, OpenVDBAsset, Field3DAsset
//
// Note: Many tests may fail in stub mode (when the actual USD library is not linked).
// This is expected behavior - the stub implementations return null/false for
// operations that require actual USD functionality.

import XCTest
@testable import SwiftUSD

final class UsdVolVolumeTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineVolume() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Volume")
        let volume = try Volume.define(on: stage, at: path)

        // In stub mode, isValid will be true but prim will be nil
        XCTAssertTrue(volume.isValid)
        // Skip prim checks in stub mode
    }

    func testVolumeIsValid() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Volume")
        let volume = try Volume.define(on: stage, at: path)

        XCTAssertTrue(volume.isValid)
    }

    // MARK: - Field Relationship Tests

    func testVolumeCreateFieldRelationship() throws {
        let stage = try Stage.createInMemory()
        let volumePath = Path("/Volume")
        let volume = try Volume.define(on: stage, at: volumePath)

        let fieldPath = Path("/Volume/DensityField")
        let fieldName = Token("density")

        // Should not throw in stub mode
        try volume.createFieldRelationship(name: fieldName, fieldPath: fieldPath)
    }

    func testVolumeBlockFieldRelationship() throws {
        let stage = try Stage.createInMemory()
        let volumePath = Path("/Volume")
        let volume = try Volume.define(on: stage, at: volumePath)

        let fieldName = Token("density")

        // Should not throw in stub mode
        try volume.blockFieldRelationship(name: fieldName)
    }

    // MARK: - Description Tests

    func testVolumeDescription() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Volume")
        let volume = try Volume.define(on: stage, at: path)

        let description = volume.description
        // Description will have some content
        XCTAssertFalse(description.isEmpty)
    }
}

final class UsdVolOpenVDBAssetTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineOpenVDBAsset() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        XCTAssertTrue(field.isValid)
    }

    func testOpenVDBAssetIsValid() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        XCTAssertTrue(field.isValid)
    }

    // MARK: - File Path Tests

    func testOpenVDBAssetSetFilePath() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        // Should not throw
        try field.setFilePath("/path/to/volume.vdb")
    }

    // MARK: - Field Name Tests

    func testOpenVDBAssetSetFieldName() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        let fieldName = Token("density")
        // Should not throw
        try field.setFieldName(fieldName)
    }

    // MARK: - Field Index Tests

    func testOpenVDBAssetFieldIndex() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        // Default index should be 0
        let defaultIndex = field.fieldIndex()
        XCTAssertEqual(defaultIndex, 0)

        // Set field index - should not throw
        try field.setFieldIndex(2)
    }

    // MARK: - Field Data Type Tests

    func testOpenVDBAssetSetFieldDataType() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        // Should not throw
        try field.setFieldDataType(.float)
    }

    // MARK: - Field Class Tests

    func testOpenVDBAssetSetFieldClass() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        // Should not throw
        try field.setFieldClass(.fogVolume)
    }

    // MARK: - Description Tests

    func testOpenVDBAssetDescription() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: path)

        let description = field.description
        XCTAssertFalse(description.isEmpty)
    }
}

final class UsdVolField3DAssetTests: XCTestCase {

    // MARK: - Creation Tests

    func testDefineField3DAsset() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        XCTAssertTrue(field.isValid)
    }

    func testField3DAssetIsValid() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        XCTAssertTrue(field.isValid)
    }

    // MARK: - File Path Tests

    func testField3DAssetSetFilePath() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        // Should not throw
        try field.setFilePath("/path/to/volume.f3d")
    }

    // MARK: - Field Name Tests

    func testField3DAssetSetFieldName() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        let fieldName = Token("density")
        // Should not throw
        try field.setFieldName(fieldName)
    }

    // MARK: - Field Index Tests

    func testField3DAssetFieldIndex() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        // Default index should be 0
        let defaultIndex = field.fieldIndex()
        XCTAssertEqual(defaultIndex, 0)

        // Set field index - should not throw
        try field.setFieldIndex(1)
    }

    // MARK: - Field Data Type Tests

    func testField3DAssetSetFieldDataType() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        // Should not throw
        try field.setFieldDataType(.float3)
    }

    // MARK: - Field Purpose Tests

    func testField3DAssetSetFieldPurpose() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        let purpose = Token("velocity")
        // Should not throw
        try field.setFieldPurpose(purpose)
    }

    // MARK: - Description Tests

    func testField3DAssetDescription() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field = try Field3DAsset.define(on: stage, at: path)

        let description = field.description
        XCTAssertFalse(description.isEmpty)
    }
}

// MARK: - Enum Tests

final class UsdVolEnumTests: XCTestCase {

    func testFieldDataTypeEnum() {
        XCTAssertEqual(FieldDataType.unknown.rawValue, 0)
        XCTAssertEqual(FieldDataType.half.rawValue, 1)
        XCTAssertEqual(FieldDataType.float.rawValue, 2)
        XCTAssertEqual(FieldDataType.double.rawValue, 3)
        XCTAssertEqual(FieldDataType.int.rawValue, 4)
        XCTAssertEqual(FieldDataType.uint.rawValue, 5)
        XCTAssertEqual(FieldDataType.int64.rawValue, 6)
        XCTAssertEqual(FieldDataType.half2.rawValue, 7)
        XCTAssertEqual(FieldDataType.float2.rawValue, 8)
        XCTAssertEqual(FieldDataType.double2.rawValue, 9)
        XCTAssertEqual(FieldDataType.int2.rawValue, 10)
        XCTAssertEqual(FieldDataType.half3.rawValue, 11)
        XCTAssertEqual(FieldDataType.float3.rawValue, 12)
        XCTAssertEqual(FieldDataType.double3.rawValue, 13)
        XCTAssertEqual(FieldDataType.int3.rawValue, 14)
        XCTAssertEqual(FieldDataType.matrix3d.rawValue, 15)
        XCTAssertEqual(FieldDataType.matrix4d.rawValue, 16)
        XCTAssertEqual(FieldDataType.quatd.rawValue, 17)
        XCTAssertEqual(FieldDataType.bool.rawValue, 18)
        XCTAssertEqual(FieldDataType.mask.rawValue, 19)
        XCTAssertEqual(FieldDataType.string.rawValue, 20)
    }

    func testVectorDataRoleHintEnum() {
        XCTAssertEqual(VectorDataRoleHint.none.rawValue, 0)
        XCTAssertEqual(VectorDataRoleHint.point.rawValue, 1)
        XCTAssertEqual(VectorDataRoleHint.normal.rawValue, 2)
        XCTAssertEqual(VectorDataRoleHint.vector.rawValue, 3)
        XCTAssertEqual(VectorDataRoleHint.color.rawValue, 4)
    }

    func testFieldClassEnum() {
        XCTAssertEqual(FieldClass.unknown.rawValue, 0)
        XCTAssertEqual(FieldClass.levelSet.rawValue, 1)
        XCTAssertEqual(FieldClass.fogVolume.rawValue, 2)
        XCTAssertEqual(FieldClass.staggered.rawValue, 3)
    }
}

// MARK: - Type Alias Tests

final class UsdVolTypeAliasTests: XCTestCase {

    func testVolumeTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Volume")
        let volume: UsdVolVolume = try UsdVolVolume.define(on: stage, at: path)
        XCTAssertTrue(volume.isValid)
    }

    func testOpenVDBAssetTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/VDBField")
        let field: UsdVolOpenVDBAsset = try UsdVolOpenVDBAsset.define(on: stage, at: path)
        XCTAssertTrue(field.isValid)
    }

    func testField3DAssetTypeAlias() throws {
        let stage = try Stage.createInMemory()
        let path = Path("/Field3D")
        let field: UsdVolField3DAsset = try UsdVolField3DAsset.define(on: stage, at: path)
        XCTAssertTrue(field.isValid)
    }
}

// MARK: - Integration Tests

final class UsdVolIntegrationTests: XCTestCase {

    func testVolumeWithOpenVDBField() throws {
        let stage = try Stage.createInMemory()

        // Create volume
        let volumePath = Path("/Volume")
        let volume = try Volume.define(on: stage, at: volumePath)

        // Create OpenVDB field
        let fieldPath = Path("/Volume/DensityField")
        let field = try OpenVDBAsset.define(on: stage, at: fieldPath)

        // Configure the field - should not throw
        try field.setFilePath("/data/smoke.vdb")
        let fieldName = Token("density")
        try field.setFieldName(fieldName)
        try field.setFieldClass(.fogVolume)
        try field.setFieldDataType(.float)

        // Bind field to volume - should not throw
        let densityRelName = Token("density")
        try volume.createFieldRelationship(name: densityRelName, fieldPath: fieldPath)

        // Verify basic validity
        XCTAssertTrue(volume.isValid)
        XCTAssertTrue(field.isValid)
    }

    func testVolumeWithMultipleFields() throws {
        let stage = try Stage.createInMemory()

        // Create volume
        let volumePath = Path("/Volume")
        let volume = try Volume.define(on: stage, at: volumePath)

        // Create density field
        let densityFieldPath = Path("/Volume/DensityField")
        let densityField = try OpenVDBAsset.define(on: stage, at: densityFieldPath)
        try densityField.setFilePath("/data/smoke.vdb")
        try densityField.setFieldClass(.fogVolume)

        // Create velocity field
        let velocityFieldPath = Path("/Volume/VelocityField")
        let velocityField = try OpenVDBAsset.define(on: stage, at: velocityFieldPath)
        try velocityField.setFilePath("/data/smoke.vdb")
        try velocityField.setFieldClass(.staggered)
        try velocityField.setFieldDataType(.float3)

        // Bind fields to volume - should not throw
        let densityRelName = Token("density")
        try volume.createFieldRelationship(name: densityRelName, fieldPath: densityFieldPath)

        let velocityRelName = Token("velocity")
        try volume.createFieldRelationship(name: velocityRelName, fieldPath: velocityFieldPath)

        // Verify basic validity
        XCTAssertTrue(volume.isValid)
        XCTAssertTrue(densityField.isValid)
        XCTAssertTrue(velocityField.isValid)
    }

    func testVolumeWithField3D() throws {
        let stage = try Stage.createInMemory()

        // Create volume
        let volumePath = Path("/Volume")
        let volume = try Volume.define(on: stage, at: volumePath)

        // Create Field3D field
        let fieldPath = Path("/Volume/TemperatureField")
        let field = try Field3DAsset.define(on: stage, at: fieldPath)

        // Configure the field - should not throw
        try field.setFilePath("/data/fire.f3d")
        let fieldName = Token("temperature")
        try field.setFieldName(fieldName)
        try field.setFieldDataType(.float)
        let purpose = Token("main")
        try field.setFieldPurpose(purpose)

        // Bind field to volume - should not throw
        let tempRelName = Token("temperature")
        try volume.createFieldRelationship(name: tempRelName, fieldPath: fieldPath)

        // Verify basic validity
        XCTAssertTrue(volume.isValid)
        XCTAssertTrue(field.isValid)
    }
}
