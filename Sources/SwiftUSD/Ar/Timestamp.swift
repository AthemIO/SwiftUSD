// Timestamp.swift - ArTimestamp wrapper
// Mirrors: pxr/usd/ar/timestamp.h

import Foundation
import OpenUSDInterop

// MARK: - Timestamp

/// Represents a timestamp for an asset.
///
/// Timestamps are represented by Unix time, the number of seconds
/// elapsed since 00:00:00 UTC 1/1/1970.
///
/// Mirrors `pxr::ArTimestamp` from the USD C++ API.
public struct Timestamp: Sendable {

    // MARK: - Properties

    /// The underlying C timestamp.
    internal let cTimestamp: ArTimestamp

    // MARK: - Initialization

    /// Creates an invalid timestamp.
    public init() {
        self.cTimestamp = ArTimestamp_CreateInvalid()
    }

    /// Creates a timestamp from Unix time.
    ///
    /// - Parameter time: The Unix time value (seconds since epoch).
    public init(time: Double) {
        self.cTimestamp = ArTimestamp_Create(time)
    }

    /// Internal initializer from a C timestamp.
    internal init(cTimestamp: ArTimestamp) {
        self.cTimestamp = cTimestamp
    }

    // MARK: - Static Factory Methods

    /// An invalid timestamp.
    public static var invalid: Timestamp {
        Timestamp()
    }

    /// Creates a timestamp representing the current time.
    public static var now: Timestamp {
        Timestamp(time: Date().timeIntervalSince1970)
    }

    // MARK: - Properties

    /// Returns `true` if this timestamp is valid.
    public var isValid: Bool {
        ArTimestamp_IsValid(cTimestamp)
    }

    /// The Unix time value (seconds since epoch).
    ///
    /// Returns `NaN` if the timestamp is invalid.
    public var time: Double {
        ArTimestamp_GetTime(cTimestamp)
    }

    /// The timestamp as a `Date`, or `nil` if invalid.
    public var date: Date? {
        guard isValid else { return nil }
        return Date(timeIntervalSince1970: time)
    }
}

// MARK: - Equatable

extension Timestamp: Equatable {

    public static func == (lhs: Timestamp, rhs: Timestamp) -> Bool {
        ArTimestamp_Equal(lhs.cTimestamp, rhs.cTimestamp)
    }
}

// MARK: - Comparable

extension Timestamp: Comparable {

    public static func < (lhs: Timestamp, rhs: Timestamp) -> Bool {
        ArTimestamp_Compare(lhs.cTimestamp, rhs.cTimestamp) < 0
    }
}

// MARK: - Hashable

extension Timestamp: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(time)
    }
}

// MARK: - CustomStringConvertible

extension Timestamp: CustomStringConvertible {

    public var description: String {
        if isValid {
            return "Timestamp(\(time))"
        } else {
            return "Timestamp(invalid)"
        }
    }
}

// MARK: - CustomDebugStringConvertible

extension Timestamp: CustomDebugStringConvertible {

    public var debugDescription: String {
        if isValid, let date = date {
            return "Timestamp(\(time), \(date))"
        } else {
            return "Timestamp(invalid)"
        }
    }
}

// MARK: - Codable

extension Timestamp: Codable {

    public init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        if let time = try? container.decode(Double.self) {
            self.init(time: time)
        } else {
            self.init()
        }
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        if isValid {
            try container.encode(time)
        } else {
            try container.encodeNil()
        }
    }
}
