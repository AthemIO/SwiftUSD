// Relationship.swift - UsdRelationship wrapper
// Mirrors: pxr/usd/usd/relationship.h

import OpenUSDInterop

// MARK: - Relationship

/// A relationship between prims in the scene graph.
///
/// Relationships define dependencies between prims by allowing a prim to
/// target other prims, attributes, or relationships. Unlike attributes,
/// relationships are always uniform (don't vary over time).
///
/// Mirrors `pxr::UsdRelationship` from the USD C++ API.
public final class Relationship: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdRelationshipRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdRelationshipRef) {
        self.handle = handle
    }

    deinit {
        UsdRelationship_Release(handle)
    }

    // MARK: - Validity

    /// Returns `true` if the relationship is valid.
    public var isValid: Bool {
        UsdRelationship_IsValid(handle)
    }

    // MARK: - Naming

    /// The relationship's name.
    public var name: Token {
        let ref = UsdRelationship_GetName(handle)!
        return Token(handle: ref)
    }

    /// The relationship's path.
    public var path: Path {
        let ref = UsdRelationship_GetPath(handle)!
        return Path(handle: ref)
    }

    // MARK: - Ownership

    /// The prim that owns this relationship.
    public var prim: Prim? {
        guard let ref = UsdRelationship_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Targets

    /// The number of targets.
    public var targetCount: Int {
        Int(UsdRelationship_GetTargetCount(handle))
    }

    /// Returns `true` if the relationship has authored targets.
    public var hasAuthoredTargets: Bool {
        UsdRelationship_HasAuthoredTargets(handle)
    }

    /// Gets the relationship's targets.
    ///
    /// - Returns: Array of target paths.
    public func targets() -> [Path] {
        let count = targetCount
        guard count > 0 else { return [] }

        var handles = [SdfPathRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdRelationship_GetTargets(handle, buffer.baseAddress, count)
        }

        var result = [Path]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Path(handle: h))
            }
        }
        return result
    }

    /// Gets the forwarded targets (resolves relationship forwarding).
    ///
    /// When a relationship targets another relationship, this method
    /// resolves the chain and returns the ultimate targets.
    ///
    /// - Returns: Array of resolved target paths.
    public func forwardedTargets() -> [Path] {
        // Use a larger buffer since forwarding can expand targets
        let initialCount = max(targetCount * 2, 16)

        var handles = [SdfPathRef?](repeating: nil, count: initialCount)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdRelationship_GetForwardedTargets(handle, buffer.baseAddress, initialCount)
        }

        var result = [Path]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Path(handle: h))
            }
        }
        return result
    }

    /// Adds a target to this relationship.
    ///
    /// - Parameters:
    ///   - target: The path to the target.
    ///   - position: Where to add the target in the list.
    /// - Returns: `true` on success.
    @discardableResult
    public func addTarget(
        _ target: Path,
        position: ListPosition = .backOfPrependList
    ) -> Bool {
        UsdRelationship_AddTarget(handle, target.handle, position.cValue) == USD_RESULT_SUCCESS
    }

    /// Removes a target from this relationship.
    ///
    /// - Parameter target: The path to the target to remove.
    /// - Returns: `true` on success.
    @discardableResult
    public func removeTarget(_ target: Path) -> Bool {
        UsdRelationship_RemoveTarget(handle, target.handle) == USD_RESULT_SUCCESS
    }

    /// Sets the targets (replaces all existing targets).
    ///
    /// - Parameter targets: Array of target paths.
    /// - Returns: `true` on success.
    @discardableResult
    public func setTargets(_ targets: [Path]) -> Bool {
        var handles = targets.map { $0.handle as SdfPathRef? }
        return handles.withUnsafeMutableBufferPointer { buffer in
            UsdRelationship_SetTargets(handle, buffer.baseAddress, targets.count) == USD_RESULT_SUCCESS
        }
    }

    /// Clears all targets.
    ///
    /// - Parameter removeSpec: If `true`, removes the relationship spec entirely.
    /// - Returns: `true` on success.
    @discardableResult
    public func clearTargets(removeSpec: Bool = false) -> Bool {
        UsdRelationship_ClearTargets(handle, removeSpec) == USD_RESULT_SUCCESS
    }
}

// MARK: - CustomStringConvertible

extension Relationship: CustomStringConvertible {

    public var description: String {
        path.description
    }
}

// MARK: - CustomDebugStringConvertible

extension Relationship: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Relationship(\"\(path)\")"
    }
}

// MARK: - RelationshipError

/// Errors that can occur when working with relationships.
public enum RelationshipError: Error, CustomStringConvertible {

    /// Failed to add target.
    case addTargetFailed(String)

    /// Failed to remove target.
    case removeTargetFailed(String)

    /// Failed to set targets.
    case setTargetsFailed(String)

    public var description: String {
        switch self {
        case .addTargetFailed(let message):
            return "Relationship add target failed: \(message)"
        case .removeTargetFailed(let message):
            return "Relationship remove target failed: \(message)"
        case .setTargetsFailed(let message):
            return "Relationship set targets failed: \(message)"
        }
    }
}
