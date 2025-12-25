// UsdVolCxxTests.swift - Tests for UsdVol C++ interop

import XCTest
import USDCxx
@testable import SwiftUSD

final class UsdVolCxxTests: XCTestCase {

    // MARK: - Volume Tests

    func testVolumeCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Cloud")
        guard let volume = swiftusd.VolVolumeRef.define(on: stage, at: path) else {
            XCTFail("Failed to create Volume")
            return
        }
        defer { volume.release() }

        XCTAssertTrue(volume.IsValid())
    }

    func testVolumeFieldRelationships() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        // Create a volume
        let volumePath = swiftusd.Path("/World/Cloud")
        guard let volume = swiftusd.VolVolumeRef.define(on: stage, at: volumePath) else {
            XCTFail("Failed to create Volume")
            return
        }
        defer { volume.release() }

        // Create a field
        let densityFieldPath = swiftusd.Path("/World/Cloud/DensityField")
        guard let densityField = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: densityFieldPath) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { densityField.release() }

        // Create field relationship
        XCTAssertTrue(volume.createField(name: "density", fieldPath: densityFieldPath))
        XCTAssertTrue(volume.hasField(name: "density"))

        // Get the field path back
        let retrievedPath = volume.fieldPath(name: "density")
        XCTAssertFalse(retrievedPath.IsEmpty())

        // Block the field relationship
        XCTAssertTrue(volume.blockField(name: "density"))
        XCTAssertFalse(volume.hasField(name: "density"))
    }

    func testVolumeVisibility() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/Cloud")
        guard let volume = swiftusd.VolVolumeRef.define(on: stage, at: path) else {
            XCTFail("Failed to create Volume")
            return
        }
        defer { volume.release() }

        // Test visibility property
        volume.visibility = swiftusd.Token("invisible")
        XCTAssertEqual(String(cString: volume.visibility.GetText()), "invisible")
    }

    // MARK: - OpenVDBAsset Tests

    func testOpenVDBAssetCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DensityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        XCTAssertTrue(field.IsValid())
    }

    func testOpenVDBAssetFilePath() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DensityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        // Test file path
        XCTAssertTrue(field.setFilePath("/path/to/cloud.vdb"))
        XCTAssertEqual(field.getFilePath(), "/path/to/cloud.vdb")
    }

    func testOpenVDBAssetFieldName() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DensityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        // Test field name property
        field.fieldName = swiftusd.Token("density")
        XCTAssertEqual(String(cString: field.fieldName.GetText()), "density")
    }

    func testOpenVDBAssetFieldIndex() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DensityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        // Test field index property
        field.fieldIndex = 2
        XCTAssertEqual(field.fieldIndex, 2)
    }

    func testOpenVDBAssetFieldDataType() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DensityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        // Test field data type property
        field.fieldDataType = swiftusd.Token("float")
        XCTAssertEqual(String(cString: field.fieldDataType.GetText()), "float")
    }

    func testOpenVDBAssetFieldClass() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/DensityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        // Test field class property
        field.fieldClass = .fogVolume
        XCTAssertEqual(field.fieldClass.value.rawValue, 2) // FogVolume = 2

        field.fieldClass = .levelSet
        XCTAssertEqual(field.fieldClass.value.rawValue, 1) // LevelSet = 1
    }

    func testOpenVDBAssetVectorDataRoleHint() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/VelocityField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { field.release() }

        // Test vector data role hint property
        field.vectorDataRoleHint = .vector
        XCTAssertEqual(field.vectorDataRoleHint.value.rawValue, 3) // Vector = 3

        field.vectorDataRoleHint = .color
        XCTAssertEqual(field.vectorDataRoleHint.value.rawValue, 4) // Color = 4
    }

    // MARK: - Field3DAsset Tests

    func testField3DAssetCreate() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TemperatureField")
        guard let field = swiftusd.VolField3DAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create Field3DAsset")
            return
        }
        defer { field.release() }

        XCTAssertTrue(field.IsValid())
    }

    func testField3DAssetFilePath() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TemperatureField")
        guard let field = swiftusd.VolField3DAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create Field3DAsset")
            return
        }
        defer { field.release() }

        // Test file path
        XCTAssertTrue(field.setFilePath("/path/to/temperature.f3d"))
        XCTAssertEqual(field.getFilePath(), "/path/to/temperature.f3d")
    }

    func testField3DAssetFieldPurpose() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TemperatureField")
        guard let field = swiftusd.VolField3DAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create Field3DAsset")
            return
        }
        defer { field.release() }

        // Test field purpose property (Field3D-specific)
        field.fieldPurpose = swiftusd.Token("simulation")
        XCTAssertEqual(String(cString: field.fieldPurpose.GetText()), "simulation")
    }

    func testField3DAssetFieldDataType() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TemperatureField")
        guard let field = swiftusd.VolField3DAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create Field3DAsset")
            return
        }
        defer { field.release() }

        // Test field data type - Field3D supports: half, float, double, half3, float3, double3
        field.fieldDataType = swiftusd.Token("float3")
        XCTAssertEqual(String(cString: field.fieldDataType.GetText()), "float3")
    }

    // MARK: - Reference Counting Tests

    func testReferenceCountingVolume() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TestVolume")
        guard let volume = swiftusd.VolVolumeRef.define(on: stage, at: path) else {
            XCTFail("Failed to create volume")
            return
        }

        // Manually retain and release
        volume.retain()
        volume.release()

        // Volume should still be valid
        XCTAssertTrue(volume.IsValid())

        // Final release
        volume.release()
    }

    func testReferenceCountingOpenVDBAsset() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        let path = swiftusd.Path("/World/TestField")
        guard let field = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: path) else {
            XCTFail("Failed to create field")
            return
        }

        // Manually retain and release
        field.retain()
        field.release()

        // Field should still be valid
        XCTAssertTrue(field.IsValid())

        // Final release
        field.release()
    }

    // MARK: - Integration Tests

    func testCompleteVolumeSetup() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        // Create a volume
        let volumePath = swiftusd.Path("/World/Cloud")
        guard let volume = swiftusd.VolVolumeRef.define(on: stage, at: volumePath) else {
            XCTFail("Failed to create Volume")
            return
        }
        defer { volume.release() }

        // Create density field
        let densityPath = swiftusd.Path("/World/Cloud/Density")
        guard let densityField = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: densityPath) else {
            XCTFail("Failed to create density field")
            return
        }
        defer { densityField.release() }

        densityField.setFilePath("/data/cloud.vdb")
        densityField.fieldName = swiftusd.Token("density")
        densityField.fieldDataType = swiftusd.Token("float")
        densityField.fieldClass = .fogVolume

        // Create velocity field
        let velocityPath = swiftusd.Path("/World/Cloud/Velocity")
        guard let velocityField = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: velocityPath) else {
            XCTFail("Failed to create velocity field")
            return
        }
        defer { velocityField.release() }

        velocityField.setFilePath("/data/cloud.vdb")
        velocityField.fieldName = swiftusd.Token("velocity")
        velocityField.fieldDataType = swiftusd.Token("float3")
        velocityField.vectorDataRoleHint = .vector

        // Bind fields to volume
        XCTAssertTrue(volume.createField(name: "density", fieldPath: densityPath))
        XCTAssertTrue(volume.createField(name: "velocity", fieldPath: velocityPath))

        // Verify all are valid
        XCTAssertTrue(volume.IsValid())
        XCTAssertTrue(densityField.IsValid())
        XCTAssertTrue(velocityField.IsValid())

        // Verify relationships
        XCTAssertTrue(volume.hasField(name: "density"))
        XCTAssertTrue(volume.hasField(name: "velocity"))
    }

    func testMultipleFieldTypes() throws {
        guard let stage = swiftusd.StageRef.createInMemory() else {
            XCTFail("Failed to create stage")
            return
        }
        defer { stage.release() }

        // Create an OpenVDB field
        let vdbPath = swiftusd.Path("/World/VDBField")
        guard let vdbField = swiftusd.VolOpenVDBAssetRef.define(on: stage, at: vdbPath) else {
            XCTFail("Failed to create OpenVDBAsset")
            return
        }
        defer { vdbField.release() }

        vdbField.fieldClass = .levelSet
        XCTAssertEqual(vdbField.fieldClass.value.rawValue, 1) // LevelSet = 1

        // Create a Field3D field
        let f3dPath = swiftusd.Path("/World/Field3DField")
        guard let f3dField = swiftusd.VolField3DAssetRef.define(on: stage, at: f3dPath) else {
            XCTFail("Failed to create Field3DAsset")
            return
        }
        defer { f3dField.release() }

        f3dField.fieldPurpose = swiftusd.Token("emission")
        XCTAssertEqual(String(cString: f3dField.fieldPurpose.GetText()), "emission")

        // Both should be valid
        XCTAssertTrue(vdbField.IsValid())
        XCTAssertTrue(f3dField.IsValid())
    }

    // MARK: - Enum Type Tests

    func testVectorDataRoleHintEnums() throws {
        // Test all VectorDataRoleHint values using raw value comparison
        let none = swiftusd.VectorDataRoleHint.none
        XCTAssertEqual(none.value.rawValue, 0) // None = 0

        let point = swiftusd.VectorDataRoleHint.point
        XCTAssertEqual(point.value.rawValue, 1) // Point = 1

        let normal = swiftusd.VectorDataRoleHint.normal
        XCTAssertEqual(normal.value.rawValue, 2) // Normal = 2

        let vector = swiftusd.VectorDataRoleHint.vector
        XCTAssertEqual(vector.value.rawValue, 3) // Vector = 3

        let color = swiftusd.VectorDataRoleHint.color
        XCTAssertEqual(color.value.rawValue, 4) // Color = 4
    }

    func testOpenVDBFieldClassEnums() throws {
        // Test all OpenVDBFieldClass values using raw value comparison
        let unknown = swiftusd.OpenVDBFieldClass.unknown
        XCTAssertEqual(unknown.value.rawValue, 0) // Unknown = 0

        let levelSet = swiftusd.OpenVDBFieldClass.levelSet
        XCTAssertEqual(levelSet.value.rawValue, 1) // LevelSet = 1

        let fogVolume = swiftusd.OpenVDBFieldClass.fogVolume
        XCTAssertEqual(fogVolume.value.rawValue, 2) // FogVolume = 2

        let staggered = swiftusd.OpenVDBFieldClass.staggered
        XCTAssertEqual(staggered.value.rawValue, 3) // Staggered = 3
    }
}
