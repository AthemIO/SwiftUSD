// Prim.swift - UsdPrim wrapper
// Mirrors: pxr/usd/usd/prim.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - Prim

/// A UsdPrim is the fundamental addressable unit in a scene graph.
///
/// A `Prim` contains typed data (attributes) and relationships to other prims.
/// Prims are organized in a hierarchy and can have children.
///
/// Mirrors `pxr::UsdPrim` from the USD C++ API.
public final class Prim: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdPrimRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdPrimRef) {
        self.handle = handle
    }

    deinit {
        UsdPrim_Release(handle)
    }

    // MARK: - Validity

    /// Returns `true` if the prim is valid.
    public var isValid: Bool {
        UsdPrim_IsValid(handle)
    }

    /// Returns `true` if the prim is active.
    public var isActive: Bool {
        get { UsdPrim_IsActive(handle) }
    }

    /// Sets the prim's active state.
    @discardableResult
    public func setActive(_ active: Bool) -> Bool {
        UsdPrim_SetActive(handle, active) == USD_RESULT_SUCCESS
    }

    /// Returns `true` if the prim is loaded.
    public var isLoaded: Bool {
        UsdPrim_IsLoaded(handle)
    }

    /// Returns `true` if the prim is a model.
    public var isModel: Bool {
        UsdPrim_IsModel(handle)
    }

    /// Returns `true` if the prim is a group.
    public var isGroup: Bool {
        UsdPrim_IsGroup(handle)
    }

    /// Returns `true` if the prim is a component.
    public var isComponent: Bool {
        UsdPrim_IsComponent(handle)
    }

    /// Returns `true` if the prim is abstract.
    public var isAbstract: Bool {
        UsdPrim_IsAbstract(handle)
    }

    /// Returns `true` if the prim is defined (has a defining specifier).
    public var isDefined: Bool {
        UsdPrim_IsDefined(handle)
    }

    /// Returns `true` if the prim has a defining specifier.
    public var hasDefiningSpecifier: Bool {
        UsdPrim_HasDefiningSpecifier(handle)
    }

    // MARK: - Identity

    /// The prim's path in the scene graph.
    public var path: Path {
        let ref = UsdPrim_GetPath(handle)!
        return Path(handle: ref)
    }

    /// The prim's name.
    public var name: Token {
        let ref = UsdPrim_GetName(handle)!
        return Token(handle: ref)
    }

    /// The prim's type name.
    public var typeName: Token {
        get {
            let ref = UsdPrim_GetTypeName(handle)!
            return Token(handle: ref)
        }
    }

    /// Sets the prim's type name.
    @discardableResult
    public func setTypeName(_ typeName: Token) -> Bool {
        UsdPrim_SetTypeName(handle, typeName.handle) == USD_RESULT_SUCCESS
    }

    /// Clears the prim's type name.
    @discardableResult
    public func clearTypeName() -> Bool {
        UsdPrim_ClearTypeName(handle) == USD_RESULT_SUCCESS
    }

    /// The prim's specifier.
    public var specifier: Specifier {
        get { Specifier(cValue: UsdPrim_GetSpecifier(handle)) }
    }

    /// Sets the prim's specifier.
    @discardableResult
    public func setSpecifier(_ specifier: Specifier) -> Bool {
        UsdPrim_SetSpecifier(handle, specifier.cValue) == USD_RESULT_SUCCESS
    }

    /// The prim's kind.
    public var kind: Token? {
        get {
            var kindRef: TfTokenRef?
            guard UsdPrim_GetKind(handle, &kindRef), let ref = kindRef else {
                return nil
            }
            return Token(handle: ref)
        }
    }

    /// Sets the prim's kind.
    @discardableResult
    public func setKind(_ kind: Token) -> Bool {
        UsdPrim_SetKind(handle, kind.handle) == USD_RESULT_SUCCESS
    }

    // MARK: - Hierarchy

    /// The parent prim.
    public var parent: Prim? {
        guard let ref = UsdPrim_GetParent(handle) else {
            return nil
        }
        let prim = Prim(handle: ref)
        return prim.isValid ? prim : nil
    }

    /// The stage that contains this prim.
    public var stage: Stage? {
        guard let ref = UsdPrim_GetStage(handle) else {
            return nil
        }
        return Stage(handle: ref)
    }

    /// The number of children.
    public var childCount: Int {
        Int(UsdPrim_GetChildCount(handle))
    }

    /// The prim's children.
    public var children: [Prim] {
        let count = childCount
        guard count > 0 else { return [] }

        var handles = [UsdPrimRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetChildren(handle, buffer.baseAddress, count)
        }

        var result = [Prim]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Prim(handle: h))
            }
        }
        return result
    }

    /// The prim's filtered children (active, loaded, defined).
    public var filteredChildren: [Prim] {
        let count = childCount
        guard count > 0 else { return [] }

        var handles = [UsdPrimRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetFilteredChildren(handle, buffer.baseAddress, count)
        }

        var result = [Prim]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Prim(handle: h))
            }
        }
        return result
    }

    /// All children including inactive ones.
    public var allChildren: [Prim] {
        let count = childCount
        guard count > 0 else { return [] }

        let bufferSize = count * 2 // Extra space for inactive
        var handles = [UsdPrimRef?](repeating: nil, count: bufferSize)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetAllChildren(handle, buffer.baseAddress, bufferSize)
        }

        var result = [Prim]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Prim(handle: h))
            }
        }
        return result
    }

    /// Gets a child prim by name.
    ///
    /// - Parameter name: The name of the child.
    /// - Returns: The child prim, or `nil` if not found.
    public func child(named name: Token) -> Prim? {
        guard let ref = UsdPrim_GetChild(handle, name.handle) else {
            return nil
        }
        let prim = Prim(handle: ref)
        return prim.isValid ? prim : nil
    }

    /// Gets a child prim by name string.
    ///
    /// - Parameter name: The name string of the child.
    /// - Returns: The child prim, or `nil` if not found.
    public func child(named name: String) -> Prim? {
        guard let ref = UsdPrim_GetChildString(handle, name) else {
            return nil
        }
        let prim = Prim(handle: ref)
        return prim.isValid ? prim : nil
    }

    // MARK: - Subtree Traversal

    /// The number of descendants.
    public var descendantCount: Int {
        Int(UsdPrim_GetDescendantCount(handle))
    }

    /// All descendants of this prim.
    public var descendants: [Prim] {
        let count = descendantCount
        guard count > 0 else { return [] }

        var handles = [UsdPrimRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetDescendants(handle, buffer.baseAddress, count)
        }

        var result = [Prim]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Prim(handle: h))
            }
        }
        return result
    }

    // MARK: - Properties

    /// The number of properties.
    public var propertyCount: Int {
        Int(UsdPrim_GetPropertyCount(handle))
    }

    /// The names of all properties.
    public var propertyNames: [Token] {
        let count = propertyCount
        guard count > 0 else { return [] }

        var handles = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetPropertyNames(handle, buffer.baseAddress, count)
        }

        var result = [Token]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Token(handle: h))
            }
        }
        return result
    }

    /// Gets a property by name.
    ///
    /// - Parameter name: The name of the property.
    /// - Returns: The property, or `nil` if not found.
    public func property(named name: Token) -> Property? {
        guard let ref = UsdPrim_GetProperty(handle, name.handle) else {
            return nil
        }
        return Property(handle: ref)
    }

    /// Returns `true` if the prim has a property with the given name.
    public func hasProperty(named name: Token) -> Bool {
        UsdPrim_HasProperty(handle, name.handle)
    }

    /// Removes a property.
    ///
    /// - Parameter name: The name of the property to remove.
    /// - Returns: `true` on success.
    @discardableResult
    public func removeProperty(named name: Token) -> Bool {
        UsdPrim_RemoveProperty(handle, name.handle) == USD_RESULT_SUCCESS
    }

    // MARK: - Attributes

    /// The number of attributes.
    public var attributeCount: Int {
        Int(UsdPrim_GetAttributeCount(handle))
    }

    /// The names of all attributes.
    public var attributeNames: [Token] {
        let count = attributeCount
        guard count > 0 else { return [] }

        var handles = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetAttributeNames(handle, buffer.baseAddress, count)
        }

        var result = [Token]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Token(handle: h))
            }
        }
        return result
    }

    /// Gets an attribute by name.
    ///
    /// - Parameter name: The name of the attribute.
    /// - Returns: The attribute, or `nil` if not found.
    public func attribute(named name: Token) -> Attribute? {
        guard let ref = UsdPrim_GetAttribute(handle, name.handle) else {
            return nil
        }
        let attr = Attribute(handle: ref)
        return attr.isValid ? attr : nil
    }

    /// Gets an attribute by name string.
    ///
    /// - Parameter name: The name string of the attribute.
    /// - Returns: The attribute, or `nil` if not found.
    public func attribute(named name: String) -> Attribute? {
        guard let ref = UsdPrim_GetAttributeString(handle, name) else {
            return nil
        }
        let attr = Attribute(handle: ref)
        return attr.isValid ? attr : nil
    }

    /// Returns `true` if the prim has an attribute with the given name.
    public func hasAttribute(named name: Token) -> Bool {
        UsdPrim_HasAttribute(handle, name.handle)
    }

    /// Creates an attribute.
    ///
    /// - Parameters:
    ///   - name: The name of the attribute.
    ///   - typeName: The type name (e.g., "float", "color3f", "double3").
    ///   - custom: Whether this is a custom (user-defined) attribute.
    ///   - variability: Whether the attribute can vary over time.
    /// - Returns: The created attribute, or `nil` on failure.
    public func createAttribute(
        named name: Token,
        typeName: Token,
        custom: Bool = true,
        variability: Variability = .varying
    ) -> Attribute? {
        guard let ref = UsdPrim_CreateAttribute(
            handle, name.handle, typeName.handle, custom, variability.cValue
        ) else {
            return nil
        }
        return Attribute(handle: ref)
    }

    /// Creates an attribute with string names.
    ///
    /// - Parameters:
    ///   - name: The name of the attribute.
    ///   - typeName: The type name (e.g., "float", "color3f", "double3").
    ///   - custom: Whether this is a custom (user-defined) attribute.
    ///   - variability: Whether the attribute can vary over time.
    /// - Returns: The created attribute, or `nil` on failure.
    public func createAttribute(
        named name: String,
        typeName: String,
        custom: Bool = true,
        variability: Variability = .varying
    ) -> Attribute? {
        guard let ref = UsdPrim_CreateAttributeString(
            handle, name, typeName, custom, variability.cValue
        ) else {
            return nil
        }
        return Attribute(handle: ref)
    }

    // MARK: - Relationships

    /// The number of relationships.
    public var relationshipCount: Int {
        Int(UsdPrim_GetRelationshipCount(handle))
    }

    /// The names of all relationships.
    public var relationshipNames: [Token] {
        let count = relationshipCount
        guard count > 0 else { return [] }

        var handles = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetRelationshipNames(handle, buffer.baseAddress, count)
        }

        var result = [Token]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Token(handle: h))
            }
        }
        return result
    }

    /// Gets a relationship by name.
    ///
    /// - Parameter name: The name of the relationship.
    /// - Returns: The relationship, or `nil` if not found.
    public func relationship(named name: Token) -> Relationship? {
        guard let ref = UsdPrim_GetRelationship(handle, name.handle) else {
            return nil
        }
        let rel = Relationship(handle: ref)
        return rel.isValid ? rel : nil
    }

    /// Returns `true` if the prim has a relationship with the given name.
    public func hasRelationship(named name: Token) -> Bool {
        UsdPrim_HasRelationship(handle, name.handle)
    }

    /// Creates a relationship.
    ///
    /// - Parameters:
    ///   - name: The name of the relationship.
    ///   - custom: Whether this is a custom (user-defined) relationship.
    /// - Returns: The created relationship, or `nil` on failure.
    public func createRelationship(
        named name: Token,
        custom: Bool = true
    ) -> Relationship? {
        guard let ref = UsdPrim_CreateRelationship(handle, name.handle, custom) else {
            return nil
        }
        return Relationship(handle: ref)
    }

    // MARK: - References

    /// Adds a reference to another layer/prim.
    ///
    /// - Parameters:
    ///   - identifier: The layer identifier.
    ///   - primPath: The path to the referenced prim.
    ///   - position: Where to add the reference in the list.
    /// - Returns: `true` on success.
    @discardableResult
    public func addReference(
        identifier: String,
        primPath: Path,
        position: ListPosition = .backOfPrependList
    ) -> Bool {
        UsdPrim_AddReference(handle, identifier, primPath.handle, position.cValue) == USD_RESULT_SUCCESS
    }

    /// Adds an internal reference to a prim in the same layer stack.
    ///
    /// - Parameters:
    ///   - primPath: The path to the referenced prim.
    ///   - position: Where to add the reference in the list.
    /// - Returns: `true` on success.
    @discardableResult
    public func addInternalReference(
        to primPath: Path,
        position: ListPosition = .backOfPrependList
    ) -> Bool {
        UsdPrim_AddInternalReference(handle, primPath.handle, position.cValue) == USD_RESULT_SUCCESS
    }

    /// Clears all references.
    ///
    /// - Returns: `true` on success.
    @discardableResult
    public func clearReferences() -> Bool {
        UsdPrim_ClearReferences(handle) == USD_RESULT_SUCCESS
    }

    // MARK: - Payloads

    /// Adds a payload.
    ///
    /// - Parameters:
    ///   - identifier: The layer identifier.
    ///   - primPath: The path to the payload prim.
    ///   - position: Where to add the payload in the list.
    /// - Returns: `true` on success.
    @discardableResult
    public func addPayload(
        identifier: String,
        primPath: Path,
        position: ListPosition = .backOfPrependList
    ) -> Bool {
        UsdPrim_AddPayload(handle, identifier, primPath.handle, position.cValue) == USD_RESULT_SUCCESS
    }

    /// Clears all payloads.
    ///
    /// - Returns: `true` on success.
    @discardableResult
    public func clearPayloads() -> Bool {
        UsdPrim_ClearPayloads(handle) == USD_RESULT_SUCCESS
    }

    /// Loads the prim's payload.
    ///
    /// - Returns: `true` on success.
    @discardableResult
    public func load() -> Bool {
        UsdPrim_Load(handle) == USD_RESULT_SUCCESS
    }

    /// Unloads the prim's payload.
    ///
    /// - Returns: `true` on success.
    @discardableResult
    public func unload() -> Bool {
        UsdPrim_Unload(handle) == USD_RESULT_SUCCESS
    }

    // MARK: - Variants

    /// The number of variant sets.
    public var variantSetCount: Int {
        Int(UsdPrim_GetVariantSetCount(handle))
    }

    /// The names of all variant sets.
    public var variantSetNames: [Token] {
        let count = variantSetCount
        guard count > 0 else { return [] }

        var handles = [TfTokenRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdPrim_GetVariantSetNames(handle, buffer.baseAddress, count)
        }

        var result = [Token]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Token(handle: h))
            }
        }
        return result
    }

    /// Returns `true` if the prim has a variant set with the given name.
    public func hasVariantSet(named name: String) -> Bool {
        UsdPrim_HasVariantSet(handle, name)
    }

    /// Gets the current variant selection for a variant set.
    ///
    /// - Parameter variantSetName: The name of the variant set.
    /// - Returns: The selected variant name, or `nil` if none selected.
    public func variantSelection(for variantSetName: String) -> String? {
        guard let cstr = UsdPrim_GetVariantSelection(handle, variantSetName) else {
            return nil
        }
        defer { UsdPrim_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// Sets the variant selection for a variant set.
    ///
    /// - Parameters:
    ///   - variantSetName: The name of the variant set.
    ///   - variantName: The name of the variant to select.
    /// - Returns: `true` on success.
    @discardableResult
    public func setVariantSelection(
        for variantSetName: String,
        to variantName: String
    ) -> Bool {
        UsdPrim_SetVariantSelection(handle, variantSetName, variantName) == USD_RESULT_SUCCESS
    }
}

// MARK: - CustomStringConvertible

extension Prim: CustomStringConvertible {

    public var description: String {
        path.description
    }
}

// MARK: - CustomDebugStringConvertible

extension Prim: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Prim(\"\(path)\", type: \(typeName))"
    }
}

// MARK: - PrimError

/// Errors that can occur when working with prims.
public enum PrimError: Error, CustomStringConvertible {

    /// Failed to create attribute.
    case attributeCreationFailed(String)

    /// Failed to create relationship.
    case relationshipCreationFailed(String)

    /// A general operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .attributeCreationFailed(let message):
            return "Attribute creation failed: \(message)"
        case .relationshipCreationFailed(let message):
            return "Relationship creation failed: \(message)"
        case .operationFailed(let message):
            return "Prim operation failed: \(message)"
        }
    }
}
