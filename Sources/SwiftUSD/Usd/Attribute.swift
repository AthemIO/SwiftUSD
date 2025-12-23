// Attribute.swift - UsdAttribute wrapper
// Mirrors: pxr/usd/usd/attribute.h

import OpenUSDInterop

/// An attribute holds typed data on a prim.
/// Mirrors pxr::UsdAttribute from the USD C++ API.
public struct UsdAttribute: Sendable {
    internal var handle: OpaquePointer?

    internal init(handle: OpaquePointer?) {
        self.handle = handle
    }

    /// Returns true if this attribute is valid.
    public var isValid: Bool {
        return handle != nil
    }

    /// Returns the name of this attribute.
    public var name: TfToken {
        // TODO: Implement via C interop
        return TfToken()
    }

    /// Returns the path of this attribute.
    public var path: SdfPath {
        // TODO: Implement via C interop
        return SdfPath()
    }

    /// Returns the type name of this attribute.
    public var typeName: SdfValueTypeName {
        // TODO: Implement via C interop
        return SdfValueTypeName()
    }

    // MARK: - Value Access

    /// Gets the value at the default time.
    public func get<T>() -> T? {
        // TODO: Implement via C interop
        return nil
    }

    /// Gets the value at a specific time.
    public func get<T>(at time: UsdTimeCode) -> T? {
        // TODO: Implement via C interop
        return nil
    }

    /// Sets the value at the default time.
    public func set<T>(_ value: T) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Sets the value at a specific time.
    public func set<T>(_ value: T, at time: UsdTimeCode) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Time Samples

    /// Returns true if this attribute has any time samples.
    public var hasTimeSamples: Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Returns the number of time samples.
    public var timeSampleCount: Int {
        // TODO: Implement via C interop
        return 0
    }

    /// Clears the value at the default time.
    public func clear() -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Clears the value at a specific time.
    public func clear(at time: UsdTimeCode) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - Time Code

/// Represents a time value in USD.
/// Mirrors pxr::UsdTimeCode from the USD C++ API.
public struct UsdTimeCode: Hashable, Sendable {
    public let value: Double

    public init(_ value: Double) {
        self.value = value
    }

    /// The default time code.
    public static let `default` = UsdTimeCode(.nan)

    /// The earliest time code.
    public static let earliestTime = UsdTimeCode(-.infinity)

    /// Returns true if this is the default time code.
    public var isDefault: Bool {
        return value.isNaN
    }
}

extension UsdTimeCode: ExpressibleByFloatLiteral {
    public init(floatLiteral value: Double) {
        self.init(value)
    }
}
