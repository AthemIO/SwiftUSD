// TimeCode.swift - UsdTimeCode wrapper
// Mirrors: pxr/usd/usd/timeCode.h

import OpenUSDInterop

// MARK: - TimeCode

/// A time code for querying attribute values at specific times.
///
/// `TimeCode` represents a time value for queries on a USD stage.
/// It can be a specific numeric time, a default time (for non-time-varying data),
/// or the earliest time in the stage.
///
/// Mirrors `pxr::UsdTimeCode` from the USD C++ API.
public struct TimeCode: Sendable, Hashable {

    // MARK: - Properties

    /// The underlying time value.
    public let time: Double

    /// Whether this represents the default time code.
    public let isDefault: Bool

    // MARK: - Initialization

    /// Creates a time code for a specific time value.
    ///
    /// - Parameter time: The numeric time value.
    public init(_ time: Double) {
        self.time = time
        self.isDefault = false
    }

    /// Creates a default time code.
    private init(isDefault: Bool) {
        self.time = 0.0
        self.isDefault = true
    }

    // MARK: - Static Factory Methods

    /// The default time code (for non-time-varying queries).
    public static var `default`: TimeCode {
        TimeCode(isDefault: true)
    }

    /// The earliest time in the stage.
    public static var earliestTime: TimeCode {
        let c = UsdTimeCode_EarliestTime()
        return TimeCode(c.time)
    }

    // MARK: - Internal

    /// Converts to the C representation.
    internal var cTimeCode: UsdTimeCode {
        if isDefault {
            return UsdTimeCode_Default()
        } else {
            return UsdTimeCode_Time(time)
        }
    }
}

// MARK: - CustomStringConvertible

extension TimeCode: CustomStringConvertible {

    public var description: String {
        if isDefault {
            return "TimeCode.default"
        } else {
            return "TimeCode(\(time))"
        }
    }
}

// MARK: - ExpressibleByFloatLiteral

extension TimeCode: ExpressibleByFloatLiteral {

    public init(floatLiteral value: Double) {
        self.init(value)
    }
}

// MARK: - ExpressibleByIntegerLiteral

extension TimeCode: ExpressibleByIntegerLiteral {

    public init(integerLiteral value: Int) {
        self.init(Double(value))
    }
}
