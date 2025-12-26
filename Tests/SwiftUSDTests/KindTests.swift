// KindTests.swift - Tests for Kind module (Model Hierarchy Kinds)

import XCTest
@testable import SwiftUSD
@testable import CxxFacade

final class KindTests: XCTestCase {

    // MARK: - Kind Tokens Tests (using CxxFacade directly)

    func testKindTokenModel() {
        let model = cxxfacade.kind.Tokens.Model()
        XCTAssertFalse(model.IsEmpty())
        XCTAssertEqual(String(cString: model.GetText()), "model")
    }

    func testKindTokenComponent() {
        let component = cxxfacade.kind.Tokens.Component()
        XCTAssertFalse(component.IsEmpty())
        XCTAssertEqual(String(cString: component.GetText()), "component")
    }

    func testKindTokenGroup() {
        let group = cxxfacade.kind.Tokens.Group()
        XCTAssertFalse(group.IsEmpty())
        XCTAssertEqual(String(cString: group.GetText()), "group")
    }

    func testKindTokenAssembly() {
        let assembly = cxxfacade.kind.Tokens.Assembly()
        XCTAssertFalse(assembly.IsEmpty())
        XCTAssertEqual(String(cString: assembly.GetText()), "assembly")
    }

    func testKindTokenSubComponent() {
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()
        XCTAssertFalse(subcomponent.IsEmpty())
        XCTAssertEqual(String(cString: subcomponent.GetText()), "subcomponent")
    }

    // MARK: - Registry HasKind Tests

    func testHasKindModel() {
        let model = cxxfacade.kind.Tokens.Model()
        XCTAssertTrue(cxxfacade.kind.HasKind(model))
    }

    func testHasKindComponent() {
        let component = cxxfacade.kind.Tokens.Component()
        XCTAssertTrue(cxxfacade.kind.HasKind(component))
    }

    func testHasKindGroup() {
        let group = cxxfacade.kind.Tokens.Group()
        XCTAssertTrue(cxxfacade.kind.HasKind(group))
    }

    func testHasKindAssembly() {
        let assembly = cxxfacade.kind.Tokens.Assembly()
        XCTAssertTrue(cxxfacade.kind.HasKind(assembly))
    }

    func testHasKindSubComponent() {
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()
        XCTAssertTrue(cxxfacade.kind.HasKind(subcomponent))
    }

    func testHasKindUnknown() {
        let unknown = cxxfacade.tf.TfToken("unknownKind12345")
        XCTAssertFalse(cxxfacade.kind.HasKind(unknown))
    }

    // MARK: - Registry GetBaseKind Tests

    func testGetBaseKindOfComponent() {
        let component = cxxfacade.kind.Tokens.Component()
        let base = cxxfacade.kind.GetBaseKind(component)
        XCTAssertFalse(base.IsEmpty())
        XCTAssertEqual(String(cString: base.GetText()), "model")
    }

    func testGetBaseKindOfGroup() {
        let group = cxxfacade.kind.Tokens.Group()
        let base = cxxfacade.kind.GetBaseKind(group)
        XCTAssertFalse(base.IsEmpty())
        XCTAssertEqual(String(cString: base.GetText()), "model")
    }

    func testGetBaseKindOfAssembly() {
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let base = cxxfacade.kind.GetBaseKind(assembly)
        XCTAssertFalse(base.IsEmpty())
        XCTAssertEqual(String(cString: base.GetText()), "group")
    }

    func testGetBaseKindOfModel() {
        // Model is at the root of the hierarchy, so it has no base
        let model = cxxfacade.kind.Tokens.Model()
        let base = cxxfacade.kind.GetBaseKind(model)
        XCTAssertTrue(base.IsEmpty())
    }

    // MARK: - Registry IsA Tests

    func testIsAModelModel() {
        let model = cxxfacade.kind.Tokens.Model()
        XCTAssertTrue(cxxfacade.kind.IsA(model, model))
    }

    func testIsAComponentModel() {
        let component = cxxfacade.kind.Tokens.Component()
        let model = cxxfacade.kind.Tokens.Model()
        XCTAssertTrue(cxxfacade.kind.IsA(component, model))
    }

    func testIsAGroupModel() {
        let group = cxxfacade.kind.Tokens.Group()
        let model = cxxfacade.kind.Tokens.Model()
        XCTAssertTrue(cxxfacade.kind.IsA(group, model))
    }

    func testIsAAssemblyModel() {
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let model = cxxfacade.kind.Tokens.Model()
        XCTAssertTrue(cxxfacade.kind.IsA(assembly, model))
    }

    func testIsAAssemblyGroup() {
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let group = cxxfacade.kind.Tokens.Group()
        XCTAssertTrue(cxxfacade.kind.IsA(assembly, group))
    }

    func testIsAModelComponent() {
        // Model is NOT a component (component derives from model, not the other way)
        let model = cxxfacade.kind.Tokens.Model()
        let component = cxxfacade.kind.Tokens.Component()
        XCTAssertFalse(cxxfacade.kind.IsA(model, component))
    }

    func testIsAComponentGroup() {
        // Component is NOT a group (they're siblings under model)
        let component = cxxfacade.kind.Tokens.Component()
        let group = cxxfacade.kind.Tokens.Group()
        XCTAssertFalse(cxxfacade.kind.IsA(component, group))
    }

    func testIsAUnknownKinds() {
        // Unknown but equal kinds should return true
        let unknown1 = cxxfacade.tf.TfToken("customKind")
        let unknown2 = cxxfacade.tf.TfToken("customKind")
        XCTAssertTrue(cxxfacade.kind.IsA(unknown1, unknown2))
    }

    func testIsADifferentUnknownKinds() {
        // Different unknown kinds should return false
        let unknown1 = cxxfacade.tf.TfToken("customKindA")
        let unknown2 = cxxfacade.tf.TfToken("customKindB")
        XCTAssertFalse(cxxfacade.kind.IsA(unknown1, unknown2))
    }

    // MARK: - Registry Convenience Methods Tests

    func testIsModel() {
        let model = cxxfacade.kind.Tokens.Model()
        let component = cxxfacade.kind.Tokens.Component()
        let group = cxxfacade.kind.Tokens.Group()
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()
        let unknown = cxxfacade.tf.TfToken("unknown")

        XCTAssertTrue(cxxfacade.kind.IsModel(model))
        XCTAssertTrue(cxxfacade.kind.IsModel(component))
        XCTAssertTrue(cxxfacade.kind.IsModel(group))
        XCTAssertTrue(cxxfacade.kind.IsModel(assembly))
        XCTAssertFalse(cxxfacade.kind.IsModel(subcomponent))
        XCTAssertFalse(cxxfacade.kind.IsModel(unknown))
    }

    func testIsGroup() {
        let model = cxxfacade.kind.Tokens.Model()
        let component = cxxfacade.kind.Tokens.Component()
        let group = cxxfacade.kind.Tokens.Group()
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()

        XCTAssertFalse(cxxfacade.kind.IsGroup(model))
        XCTAssertFalse(cxxfacade.kind.IsGroup(component))
        XCTAssertTrue(cxxfacade.kind.IsGroup(group))
        XCTAssertTrue(cxxfacade.kind.IsGroup(assembly))
        XCTAssertFalse(cxxfacade.kind.IsGroup(subcomponent))
    }

    func testIsAssembly() {
        let model = cxxfacade.kind.Tokens.Model()
        let component = cxxfacade.kind.Tokens.Component()
        let group = cxxfacade.kind.Tokens.Group()
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()

        XCTAssertFalse(cxxfacade.kind.IsAssembly(model))
        XCTAssertFalse(cxxfacade.kind.IsAssembly(component))
        XCTAssertFalse(cxxfacade.kind.IsAssembly(group))
        XCTAssertTrue(cxxfacade.kind.IsAssembly(assembly))
        XCTAssertFalse(cxxfacade.kind.IsAssembly(subcomponent))
    }

    func testIsComponent() {
        let model = cxxfacade.kind.Tokens.Model()
        let component = cxxfacade.kind.Tokens.Component()
        let group = cxxfacade.kind.Tokens.Group()
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()

        XCTAssertFalse(cxxfacade.kind.IsComponent(model))
        XCTAssertTrue(cxxfacade.kind.IsComponent(component))
        XCTAssertFalse(cxxfacade.kind.IsComponent(group))
        XCTAssertFalse(cxxfacade.kind.IsComponent(assembly))
        XCTAssertFalse(cxxfacade.kind.IsComponent(subcomponent))
    }

    func testIsSubComponent() {
        let model = cxxfacade.kind.Tokens.Model()
        let component = cxxfacade.kind.Tokens.Component()
        let group = cxxfacade.kind.Tokens.Group()
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let subcomponent = cxxfacade.kind.Tokens.SubComponent()

        XCTAssertFalse(cxxfacade.kind.IsSubComponent(model))
        XCTAssertFalse(cxxfacade.kind.IsSubComponent(component))
        XCTAssertFalse(cxxfacade.kind.IsSubComponent(group))
        XCTAssertFalse(cxxfacade.kind.IsSubComponent(assembly))
        XCTAssertTrue(cxxfacade.kind.IsSubComponent(subcomponent))
    }

    // MARK: - GetAllKinds Tests

    func testGetAllKinds() {
        let allKinds = cxxfacade.kind.GetAllKinds()

        // Should have at least the 5 built-in kinds
        XCTAssertGreaterThanOrEqual(allKinds.size(), 5)

        // Convert to strings for easier checking
        var kindStrings: [String] = []
        for i in 0..<allKinds.size() {
            kindStrings.append(String(cString: allKinds[i].GetText()))
        }

        XCTAssertTrue(kindStrings.contains("model"))
        XCTAssertTrue(kindStrings.contains("component"))
        XCTAssertTrue(kindStrings.contains("group"))
        XCTAssertTrue(kindStrings.contains("assembly"))
        XCTAssertTrue(kindStrings.contains("subcomponent"))
    }

    // MARK: - Edge Cases

    func testEmptyTokenKindChecks() {
        let empty = cxxfacade.tf.TfToken()

        XCTAssertFalse(cxxfacade.kind.HasKind(empty))
        XCTAssertFalse(cxxfacade.kind.IsModel(empty))
        XCTAssertFalse(cxxfacade.kind.IsGroup(empty))
        XCTAssertFalse(cxxfacade.kind.IsAssembly(empty))
        XCTAssertFalse(cxxfacade.kind.IsComponent(empty))
        XCTAssertFalse(cxxfacade.kind.IsSubComponent(empty))
    }

    func testKindHierarchyConsistency() {
        // Verify the kind hierarchy is consistent
        // assembly -> group -> model
        let assembly = cxxfacade.kind.Tokens.Assembly()
        let group = cxxfacade.kind.Tokens.Group()
        let model = cxxfacade.kind.Tokens.Model()

        // Assembly's base is group
        let assemblyBase = cxxfacade.kind.GetBaseKind(assembly)
        XCTAssertEqual(String(cString: assemblyBase.GetText()), "group")

        // Group's base is model
        let groupBase = cxxfacade.kind.GetBaseKind(group)
        XCTAssertEqual(String(cString: groupBase.GetText()), "model")

        // Model has no base
        let modelBase = cxxfacade.kind.GetBaseKind(model)
        XCTAssertTrue(modelBase.IsEmpty())

        // Assembly is-a group is-a model
        XCTAssertTrue(cxxfacade.kind.IsA(assembly, group))
        XCTAssertTrue(cxxfacade.kind.IsA(group, model))
        XCTAssertTrue(cxxfacade.kind.IsA(assembly, model))
    }

    func testComponentHierarchy() {
        // Verify component hierarchy
        // component -> model
        let component = cxxfacade.kind.Tokens.Component()
        let model = cxxfacade.kind.Tokens.Model()

        let componentBase = cxxfacade.kind.GetBaseKind(component)
        XCTAssertEqual(String(cString: componentBase.GetText()), "model")
        XCTAssertTrue(cxxfacade.kind.IsA(component, model))
    }
}
