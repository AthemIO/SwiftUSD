import XCTest
@testable import SwiftUSD

final class KindRegistryTests: XCTestCase {

    // MARK: - HasKind Tests

    func testHasKindBuiltIn() throws {
        // Built-in kinds should be recognized
        let model = try Token("model")
        let component = try Token("component")
        let group = try Token("group")
        let assembly = try Token("assembly")
        let subcomponent = try Token("subcomponent")

        XCTAssertTrue(KindRegistry.hasKind(model))
        XCTAssertTrue(KindRegistry.hasKind(component))
        XCTAssertTrue(KindRegistry.hasKind(group))
        XCTAssertTrue(KindRegistry.hasKind(assembly))
        XCTAssertTrue(KindRegistry.hasKind(subcomponent))
    }

    func testHasKindBuiltInString() {
        XCTAssertTrue(KindRegistry.hasKind("model"))
        XCTAssertTrue(KindRegistry.hasKind("component"))
        XCTAssertTrue(KindRegistry.hasKind("group"))
        XCTAssertTrue(KindRegistry.hasKind("assembly"))
        XCTAssertTrue(KindRegistry.hasKind("subcomponent"))
    }

    func testHasKindUnknown() throws {
        let unknown = try Token("unknown_kind_12345")
        XCTAssertFalse(KindRegistry.hasKind(unknown))
    }

    func testHasKindUnknownString() {
        XCTAssertFalse(KindRegistry.hasKind("unknown_kind_12345"))
    }

    // MARK: - GetBaseKind Tests

    func testGetBaseKindComponent() throws {
        // component's base is model
        let baseKind = KindRegistry.getBaseKind("component")
        XCTAssertNotNil(baseKind)
        XCTAssertEqual(baseKind?.text, "model")
    }

    func testGetBaseKindGroup() throws {
        // group's base is model
        let baseKind = KindRegistry.getBaseKind("group")
        XCTAssertNotNil(baseKind)
        XCTAssertEqual(baseKind?.text, "model")
    }

    func testGetBaseKindAssembly() throws {
        // assembly's base is group
        let baseKind = KindRegistry.getBaseKind("assembly")
        XCTAssertNotNil(baseKind)
        XCTAssertEqual(baseKind?.text, "group")
    }

    func testGetBaseKindModel() {
        // model has no base kind
        let baseKind = KindRegistry.getBaseKind("model")
        XCTAssertNil(baseKind)
    }

    func testGetBaseKindSubcomponent() {
        // subcomponent has no base kind
        let baseKind = KindRegistry.getBaseKind("subcomponent")
        XCTAssertNil(baseKind)
    }

    func testGetBaseKindUnknown() {
        let baseKind = KindRegistry.getBaseKind("unknown_kind")
        XCTAssertNil(baseKind)
    }

    func testGetBaseKindWithToken() throws {
        let component = try Token("component")
        let baseKind = KindRegistry.getBaseKind(component)
        XCTAssertNotNil(baseKind)
        XCTAssertEqual(baseKind?.text, "model")
    }

    // MARK: - IsA Tests

    func testIsADirect() {
        // A kind is itself
        XCTAssertTrue(KindRegistry.isA("model", "model"))
        XCTAssertTrue(KindRegistry.isA("component", "component"))
        XCTAssertTrue(KindRegistry.isA("group", "group"))
    }

    func testIsADerived() {
        // component derives from model
        XCTAssertTrue(KindRegistry.isA("component", "model"))

        // group derives from model
        XCTAssertTrue(KindRegistry.isA("group", "model"))

        // assembly derives from group
        XCTAssertTrue(KindRegistry.isA("assembly", "group"))

        // assembly also derives from model (transitively)
        XCTAssertTrue(KindRegistry.isA("assembly", "model"))
    }

    func testIsANotDerived() {
        // model doesn't derive from component
        XCTAssertFalse(KindRegistry.isA("model", "component"))

        // group doesn't derive from component
        XCTAssertFalse(KindRegistry.isA("group", "component"))

        // subcomponent doesn't derive from model
        XCTAssertFalse(KindRegistry.isA("subcomponent", "model"))
    }

    func testIsAWithTokens() throws {
        let component = try Token("component")
        let model = try Token("model")

        XCTAssertTrue(KindRegistry.isA(component, model))
    }

    // MARK: - IsModel Tests

    func testIsModel() {
        XCTAssertTrue(KindRegistry.isModel("model"))
        XCTAssertTrue(KindRegistry.isModel("component"))
        XCTAssertTrue(KindRegistry.isModel("group"))
        XCTAssertTrue(KindRegistry.isModel("assembly"))
        XCTAssertFalse(KindRegistry.isModel("subcomponent"))
        XCTAssertFalse(KindRegistry.isModel("unknown"))
    }

    func testIsModelWithToken() throws {
        let component = try Token("component")
        XCTAssertTrue(KindRegistry.isModel(component))

        let subcomponent = try Token("subcomponent")
        XCTAssertFalse(KindRegistry.isModel(subcomponent))
    }

    // MARK: - IsGroup Tests

    func testIsGroup() {
        XCTAssertTrue(KindRegistry.isGroup("group"))
        XCTAssertTrue(KindRegistry.isGroup("assembly"))
        XCTAssertFalse(KindRegistry.isGroup("model"))
        XCTAssertFalse(KindRegistry.isGroup("component"))
        XCTAssertFalse(KindRegistry.isGroup("subcomponent"))
    }

    func testIsGroupWithToken() throws {
        let assembly = try Token("assembly")
        XCTAssertTrue(KindRegistry.isGroup(assembly))

        let component = try Token("component")
        XCTAssertFalse(KindRegistry.isGroup(component))
    }

    // MARK: - IsAssembly Tests

    func testIsAssembly() {
        XCTAssertTrue(KindRegistry.isAssembly("assembly"))
        XCTAssertFalse(KindRegistry.isAssembly("group"))
        XCTAssertFalse(KindRegistry.isAssembly("model"))
        XCTAssertFalse(KindRegistry.isAssembly("component"))
    }

    func testIsAssemblyWithToken() throws {
        let assembly = try Token("assembly")
        XCTAssertTrue(KindRegistry.isAssembly(assembly))

        let group = try Token("group")
        XCTAssertFalse(KindRegistry.isAssembly(group))
    }

    // MARK: - IsComponent Tests

    func testIsComponent() {
        XCTAssertTrue(KindRegistry.isComponent("component"))
        XCTAssertFalse(KindRegistry.isComponent("group"))
        XCTAssertFalse(KindRegistry.isComponent("model"))
        XCTAssertFalse(KindRegistry.isComponent("assembly"))
    }

    func testIsComponentWithToken() throws {
        let component = try Token("component")
        XCTAssertTrue(KindRegistry.isComponent(component))

        let model = try Token("model")
        XCTAssertFalse(KindRegistry.isComponent(model))
    }

    // MARK: - IsSubComponent Tests

    func testIsSubComponent() {
        XCTAssertTrue(KindRegistry.isSubComponent("subcomponent"))
        XCTAssertFalse(KindRegistry.isSubComponent("component"))
        XCTAssertFalse(KindRegistry.isSubComponent("model"))
        XCTAssertFalse(KindRegistry.isSubComponent("group"))
    }

    func testIsSubComponentWithToken() throws {
        let subcomponent = try Token("subcomponent")
        XCTAssertTrue(KindRegistry.isSubComponent(subcomponent))

        let component = try Token("component")
        XCTAssertFalse(KindRegistry.isSubComponent(component))
    }

    // MARK: - GetAllKinds Tests

    func testGetAllKinds() {
        let allKinds = KindRegistry.getAllKinds()

        // Should have at least the 5 built-in kinds
        XCTAssertGreaterThanOrEqual(allKinds.count, 5)

        // Check that built-in kinds are present
        let kindTexts = Set(allKinds.map { $0.text })
        XCTAssertTrue(kindTexts.contains("model"))
        XCTAssertTrue(kindTexts.contains("component"))
        XCTAssertTrue(kindTexts.contains("group"))
        XCTAssertTrue(kindTexts.contains("assembly"))
        XCTAssertTrue(kindTexts.contains("subcomponent"))
    }
}

// MARK: - KindTokens Tests

final class KindTokensTests: XCTestCase {

    func testModelToken() {
        let model = KindTokens.model
        XCTAssertEqual(model.text, "model")
    }

    func testComponentToken() {
        let component = KindTokens.component
        XCTAssertEqual(component.text, "component")
    }

    func testGroupToken() {
        let group = KindTokens.group
        XCTAssertEqual(group.text, "group")
    }

    func testAssemblyToken() {
        let assembly = KindTokens.assembly
        XCTAssertEqual(assembly.text, "assembly")
    }

    func testSubcomponentToken() {
        let subcomponent = KindTokens.subcomponent
        XCTAssertEqual(subcomponent.text, "subcomponent")
    }

    func testTokensAreUsableWithRegistry() {
        // Verify that KindTokens work correctly with KindRegistry
        XCTAssertTrue(KindRegistry.hasKind(KindTokens.model))
        XCTAssertTrue(KindRegistry.hasKind(KindTokens.component))
        XCTAssertTrue(KindRegistry.hasKind(KindTokens.group))
        XCTAssertTrue(KindRegistry.hasKind(KindTokens.assembly))
        XCTAssertTrue(KindRegistry.hasKind(KindTokens.subcomponent))
    }

    func testTokenIsARelationships() {
        // component is a model
        XCTAssertTrue(KindRegistry.isA(KindTokens.component, KindTokens.model))

        // group is a model
        XCTAssertTrue(KindRegistry.isA(KindTokens.group, KindTokens.model))

        // assembly is a group
        XCTAssertTrue(KindRegistry.isA(KindTokens.assembly, KindTokens.group))

        // assembly is also a model (transitively)
        XCTAssertTrue(KindRegistry.isA(KindTokens.assembly, KindTokens.model))
    }

    func testTokenEquality() throws {
        // Tokens created via KindTokens should equal tokens created from strings
        let modelFromTokens = KindTokens.model
        let modelFromString = try Token("model")

        XCTAssertEqual(modelFromTokens.text, modelFromString.text)
    }
}

// MARK: - Kind Hierarchy Tests

final class KindHierarchyTests: XCTestCase {

    func testCompleteHierarchy() {
        // Test the full kind hierarchy:
        //   model
        //     component
        //     group
        //       assembly
        //   subcomponent (standalone)

        // model is the root
        XCTAssertNil(KindRegistry.getBaseKind("model"))

        // component and group derive from model
        XCTAssertEqual(KindRegistry.getBaseKind("component")?.text, "model")
        XCTAssertEqual(KindRegistry.getBaseKind("group")?.text, "model")

        // assembly derives from group
        XCTAssertEqual(KindRegistry.getBaseKind("assembly")?.text, "group")

        // subcomponent is standalone
        XCTAssertNil(KindRegistry.getBaseKind("subcomponent"))
    }

    func testTransitiveInheritance() {
        // assembly -> group -> model
        XCTAssertTrue(KindRegistry.isA("assembly", "group"))
        XCTAssertTrue(KindRegistry.isA("assembly", "model"))

        // component -> model (direct only)
        XCTAssertTrue(KindRegistry.isA("component", "model"))
        XCTAssertFalse(KindRegistry.isA("component", "group"))
    }

    func testNoInheritanceBetweenBranches() {
        // component and group are siblings, not related
        XCTAssertFalse(KindRegistry.isA("component", "group"))
        XCTAssertFalse(KindRegistry.isA("group", "component"))

        // assembly and component are cousins, not related
        XCTAssertFalse(KindRegistry.isA("assembly", "component"))
        XCTAssertFalse(KindRegistry.isA("component", "assembly"))
    }

    func testSubcomponentIsIsolated() {
        // subcomponent doesn't inherit from anything
        XCTAssertFalse(KindRegistry.isA("subcomponent", "model"))
        XCTAssertFalse(KindRegistry.isA("subcomponent", "component"))
        XCTAssertFalse(KindRegistry.isA("subcomponent", "group"))
        XCTAssertFalse(KindRegistry.isA("subcomponent", "assembly"))

        // nothing inherits from subcomponent
        XCTAssertFalse(KindRegistry.isA("model", "subcomponent"))
        XCTAssertFalse(KindRegistry.isA("component", "subcomponent"))
        XCTAssertFalse(KindRegistry.isA("group", "subcomponent"))
        XCTAssertFalse(KindRegistry.isA("assembly", "subcomponent"))
    }
}
