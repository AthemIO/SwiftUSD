// KindCxxTests.swift - Tests for Kind C++ interop types
// Tests: KindRegistry, KindTokens via USDCxx module

import XCTest
@testable import SwiftUSD
import USDCxx

// Create local type aliases to swiftusd types for testing
typealias KindRegistry = swiftusd.KindRegistry
typealias KindTokens = swiftusd.KindTokens
typealias Token = swiftusd.Token

final class KindCxxTests: XCTestCase {

    // MARK: - KindTokens Tests

    func testKindTokensModel() {
        let model = KindTokens.Model()
        XCTAssertFalse(model.IsEmpty())
        XCTAssertEqual(String(cString: model.GetText()), "model")
    }

    func testKindTokensComponent() {
        let component = KindTokens.Component()
        XCTAssertFalse(component.IsEmpty())
        XCTAssertEqual(String(cString: component.GetText()), "component")
    }

    func testKindTokensGroup() {
        let group = KindTokens.Group()
        XCTAssertFalse(group.IsEmpty())
        XCTAssertEqual(String(cString: group.GetText()), "group")
    }

    func testKindTokensAssembly() {
        let assembly = KindTokens.Assembly()
        XCTAssertFalse(assembly.IsEmpty())
        XCTAssertEqual(String(cString: assembly.GetText()), "assembly")
    }

    func testKindTokensSubcomponent() {
        let subcomponent = KindTokens.Subcomponent()
        XCTAssertFalse(subcomponent.IsEmpty())
        XCTAssertEqual(String(cString: subcomponent.GetText()), "subcomponent")
    }

    func testKindTokensStaticProperties() {
        // Test the Swift extension properties
        XCTAssertEqual(String(cString: KindTokensCxx.model.GetText()), "model")
        XCTAssertEqual(String(cString: KindTokensCxx.component.GetText()), "component")
        XCTAssertEqual(String(cString: KindTokensCxx.group.GetText()), "group")
        XCTAssertEqual(String(cString: KindTokensCxx.assembly.GetText()), "assembly")
        XCTAssertEqual(String(cString: KindTokensCxx.subcomponent.GetText()), "subcomponent")
    }

    // MARK: - KindRegistry.HasKind Tests

    func testHasKindBuiltIn() {
        XCTAssertTrue(KindRegistry.HasKind(KindTokens.Model()))
        XCTAssertTrue(KindRegistry.HasKind(KindTokens.Component()))
        XCTAssertTrue(KindRegistry.HasKind(KindTokens.Group()))
        XCTAssertTrue(KindRegistry.HasKind(KindTokens.Assembly()))
        XCTAssertTrue(KindRegistry.HasKind(KindTokens.Subcomponent()))
    }

    func testHasKindString() {
        XCTAssertTrue(KindRegistry.HasKind("model"))
        XCTAssertTrue(KindRegistry.HasKind("component"))
        XCTAssertTrue(KindRegistry.HasKind("group"))
        XCTAssertTrue(KindRegistry.HasKind("assembly"))
        XCTAssertTrue(KindRegistry.HasKind("subcomponent"))
    }

    func testHasKindUnknown() {
        let unknown = Token("unknown_kind_xyz")
        XCTAssertFalse(KindRegistry.HasKind(unknown))
        XCTAssertFalse(KindRegistry.HasKind("unknown_kind_xyz"))
    }

    // MARK: - KindRegistry.GetBaseKind Tests

    func testGetBaseKindComponent() {
        let base = KindRegistry.GetBaseKind(KindTokens.Component())
        XCTAssertFalse(base.IsEmpty())
        XCTAssertEqual(String(cString: base.GetText()), "model")
    }

    func testGetBaseKindGroup() {
        let base = KindRegistry.GetBaseKind(KindTokens.Group())
        XCTAssertFalse(base.IsEmpty())
        XCTAssertEqual(String(cString: base.GetText()), "model")
    }

    func testGetBaseKindAssembly() {
        let base = KindRegistry.GetBaseKind(KindTokens.Assembly())
        XCTAssertFalse(base.IsEmpty())
        XCTAssertEqual(String(cString: base.GetText()), "group")
    }

    func testGetBaseKindModel() {
        // model has no base kind
        let base = KindRegistry.GetBaseKind(KindTokens.Model())
        XCTAssertTrue(base.IsEmpty())
    }

    func testGetBaseKindSubcomponent() {
        // subcomponent has no base kind
        let base = KindRegistry.GetBaseKind(KindTokens.Subcomponent())
        XCTAssertTrue(base.IsEmpty())
    }

    func testGetBaseKindString() {
        let base = KindRegistry.GetBaseKind("component")
        XCTAssertEqual(String(cString: base.GetText()), "model")
    }

    func testGetBaseKindOptional() {
        // Using Swift extension
        XCTAssertNotNil(KindRegistryCxx.getBaseKindOptional(KindTokens.Component()))
        XCTAssertNil(KindRegistryCxx.getBaseKindOptional(KindTokens.Model()))
        XCTAssertNil(KindRegistryCxx.getBaseKindOptional(KindTokens.Subcomponent()))
    }

    // MARK: - KindRegistry.IsA Tests

    func testIsASameKind() {
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Model(), KindTokens.Model()))
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Component(), KindTokens.Component()))
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Group(), KindTokens.Group()))
    }

    func testIsADerived() {
        // component derives from model
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Component(), KindTokens.Model()))

        // group derives from model
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Group(), KindTokens.Model()))

        // assembly derives from group
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Assembly(), KindTokens.Group()))

        // assembly also derives from model (transitively)
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Assembly(), KindTokens.Model()))
    }

    func testIsANotDerived() {
        // model doesn't derive from component
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Model(), KindTokens.Component()))

        // group doesn't derive from component
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Group(), KindTokens.Component()))

        // subcomponent doesn't derive from model
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Subcomponent(), KindTokens.Model()))
    }

    func testIsAString() {
        XCTAssertTrue(KindRegistry.IsA("component", "model"))
        XCTAssertTrue(KindRegistry.IsA("assembly", "group"))
        XCTAssertTrue(KindRegistry.IsA("assembly", "model"))
        XCTAssertFalse(KindRegistry.IsA("model", "component"))
    }

    // MARK: - KindRegistry.IsModel Tests

    func testIsModel() {
        XCTAssertTrue(KindRegistry.IsModel(KindTokens.Model()))
        XCTAssertTrue(KindRegistry.IsModel(KindTokens.Component()))
        XCTAssertTrue(KindRegistry.IsModel(KindTokens.Group()))
        XCTAssertTrue(KindRegistry.IsModel(KindTokens.Assembly()))
        XCTAssertFalse(KindRegistry.IsModel(KindTokens.Subcomponent()))
    }

    func testIsModelString() {
        XCTAssertTrue(KindRegistry.IsModel("model"))
        XCTAssertTrue(KindRegistry.IsModel("component"))
        XCTAssertFalse(KindRegistry.IsModel("subcomponent"))
        XCTAssertFalse(KindRegistry.IsModel("unknown"))
    }

    // MARK: - KindRegistry.IsGroup Tests

    func testIsGroup() {
        XCTAssertTrue(KindRegistry.IsGroup(KindTokens.Group()))
        XCTAssertTrue(KindRegistry.IsGroup(KindTokens.Assembly()))
        XCTAssertFalse(KindRegistry.IsGroup(KindTokens.Model()))
        XCTAssertFalse(KindRegistry.IsGroup(KindTokens.Component()))
        XCTAssertFalse(KindRegistry.IsGroup(KindTokens.Subcomponent()))
    }

    func testIsGroupString() {
        XCTAssertTrue(KindRegistry.IsGroup("group"))
        XCTAssertTrue(KindRegistry.IsGroup("assembly"))
        XCTAssertFalse(KindRegistry.IsGroup("model"))
        XCTAssertFalse(KindRegistry.IsGroup("component"))
    }

    // MARK: - KindRegistry.IsAssembly Tests

    func testIsAssembly() {
        XCTAssertTrue(KindRegistry.IsAssembly(KindTokens.Assembly()))
        XCTAssertFalse(KindRegistry.IsAssembly(KindTokens.Group()))
        XCTAssertFalse(KindRegistry.IsAssembly(KindTokens.Model()))
        XCTAssertFalse(KindRegistry.IsAssembly(KindTokens.Component()))
    }

    func testIsAssemblyString() {
        XCTAssertTrue(KindRegistry.IsAssembly("assembly"))
        XCTAssertFalse(KindRegistry.IsAssembly("group"))
    }

    // MARK: - KindRegistry.IsComponent Tests

    func testIsComponent() {
        XCTAssertTrue(KindRegistry.IsComponent(KindTokens.Component()))
        XCTAssertFalse(KindRegistry.IsComponent(KindTokens.Group()))
        XCTAssertFalse(KindRegistry.IsComponent(KindTokens.Model()))
        XCTAssertFalse(KindRegistry.IsComponent(KindTokens.Assembly()))
    }

    func testIsComponentString() {
        XCTAssertTrue(KindRegistry.IsComponent("component"))
        XCTAssertFalse(KindRegistry.IsComponent("group"))
    }

    // MARK: - KindRegistry.IsSubcomponent Tests

    func testIsSubcomponent() {
        XCTAssertTrue(KindRegistry.IsSubcomponent(KindTokens.Subcomponent()))
        XCTAssertFalse(KindRegistry.IsSubcomponent(KindTokens.Component()))
        XCTAssertFalse(KindRegistry.IsSubcomponent(KindTokens.Model()))
        XCTAssertFalse(KindRegistry.IsSubcomponent(KindTokens.Group()))
    }

    func testIsSubcomponentString() {
        XCTAssertTrue(KindRegistry.IsSubcomponent("subcomponent"))
        XCTAssertFalse(KindRegistry.IsSubcomponent("component"))
    }

    // MARK: - KindRegistry.GetAllKinds Tests

    func testGetAllKindsCount() {
        let count = KindRegistry.GetAllKindsCount()
        // Should have at least the 5 built-in kinds
        XCTAssertGreaterThanOrEqual(count, 5)
    }

    func testGetAllKindsArray() {
        let allKinds = KindRegistryCxx.getAllKindsArray()

        // Should have at least the 5 built-in kinds
        XCTAssertGreaterThanOrEqual(allKinds.count, 5)

        // Check that built-in kinds are present
        let kindTexts = Set(allKinds.map { String(cString: $0.GetText()) })
        XCTAssertTrue(kindTexts.contains("model"))
        XCTAssertTrue(kindTexts.contains("component"))
        XCTAssertTrue(kindTexts.contains("group"))
        XCTAssertTrue(kindTexts.contains("assembly"))
        XCTAssertTrue(kindTexts.contains("subcomponent"))
    }

    // MARK: - Token Kind Extension Tests

    func testTokenIsKnownKind() {
        let model = KindTokens.Model()
        XCTAssertTrue(model.isKnownKind)

        let unknown = Token("unknown_xyz")
        XCTAssertFalse(unknown.isKnownKind)
    }

    func testTokenBaseKind() {
        let component = KindTokens.Component()
        let base = component.baseKind
        XCTAssertNotNil(base)
        XCTAssertEqual(String(cString: base!.GetText()), "model")

        let model = KindTokens.Model()
        XCTAssertNil(model.baseKind)
    }

    func testTokenKindPredicates() {
        let component = KindTokens.Component()
        XCTAssertTrue(component.isModelKind)
        XCTAssertTrue(component.isComponentKind)
        XCTAssertFalse(component.isGroupKind)

        let assembly = KindTokens.Assembly()
        XCTAssertTrue(assembly.isModelKind)
        XCTAssertTrue(assembly.isGroupKind)
        XCTAssertTrue(assembly.isAssemblyKind)
        XCTAssertFalse(assembly.isComponentKind)

        let subcomponent = KindTokens.Subcomponent()
        XCTAssertFalse(subcomponent.isModelKind)
        XCTAssertTrue(subcomponent.isSubcomponentKind)
    }

    func testTokenIsA() {
        let assembly = KindTokens.Assembly()
        XCTAssertTrue(assembly.isA(KindTokens.Group()))
        XCTAssertTrue(assembly.isA(KindTokens.Model()))
        XCTAssertFalse(assembly.isA(KindTokens.Component()))
    }

    // MARK: - Kind Hierarchy Tests

    func testCompleteHierarchy() {
        // Test the full kind hierarchy:
        //   model
        //     component
        //     group
        //       assembly
        //   subcomponent (standalone)

        // model is the root
        XCTAssertTrue(KindRegistry.GetBaseKind(KindTokens.Model()).IsEmpty())

        // component and group derive from model
        XCTAssertEqual(
            String(cString: KindRegistry.GetBaseKind(KindTokens.Component()).GetText()),
            "model"
        )
        XCTAssertEqual(
            String(cString: KindRegistry.GetBaseKind(KindTokens.Group()).GetText()),
            "model"
        )

        // assembly derives from group
        XCTAssertEqual(
            String(cString: KindRegistry.GetBaseKind(KindTokens.Assembly()).GetText()),
            "group"
        )

        // subcomponent is standalone
        XCTAssertTrue(KindRegistry.GetBaseKind(KindTokens.Subcomponent()).IsEmpty())
    }

    func testTransitiveInheritance() {
        // assembly -> group -> model
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Assembly(), KindTokens.Group()))
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Assembly(), KindTokens.Model()))

        // component -> model (direct only)
        XCTAssertTrue(KindRegistry.IsA(KindTokens.Component(), KindTokens.Model()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Component(), KindTokens.Group()))
    }

    func testNoInheritanceBetweenBranches() {
        // component and group are siblings, not related
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Component(), KindTokens.Group()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Group(), KindTokens.Component()))

        // assembly and component are cousins, not related
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Assembly(), KindTokens.Component()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Component(), KindTokens.Assembly()))
    }

    func testSubcomponentIsIsolated() {
        // subcomponent doesn't inherit from anything
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Subcomponent(), KindTokens.Model()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Subcomponent(), KindTokens.Component()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Subcomponent(), KindTokens.Group()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Subcomponent(), KindTokens.Assembly()))

        // nothing inherits from subcomponent
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Model(), KindTokens.Subcomponent()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Component(), KindTokens.Subcomponent()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Group(), KindTokens.Subcomponent()))
        XCTAssertFalse(KindRegistry.IsA(KindTokens.Assembly(), KindTokens.Subcomponent()))
    }
}
