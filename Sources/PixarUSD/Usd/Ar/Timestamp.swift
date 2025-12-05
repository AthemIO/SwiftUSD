/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar
import Foundation

public extension ArTimestamp
{
  // MARK: - Factory Methods

  /// Create an invalid timestamp.
  static func invalid() -> ArTimestamp
  {
    ArTimestamp()
  }

  /// Create a timestamp at the given Unix time.
  ///
  /// - Parameter time: Unix time value (seconds since epoch)
  static func at(time: Double) -> ArTimestamp
  {
    ArTimestamp(time)
  }

  /// Create a timestamp for the current time.
  static func now() -> ArTimestamp
  {
    ArTimestamp(Date().timeIntervalSince1970)
  }

  // MARK: - Properties

  /// Returns true if this timestamp is valid.
  var isValid: Bool
  {
    IsValid()
  }

  /// Return the time represented by this timestamp as a Double.
  ///
  /// - Warning: If this timestamp is invalid, a coding error will be issued.
  var time: Double
  {
    GetTime()
  }

  /// Safely get the time, returning nil if invalid.
  var timeIfValid: Double?
  {
    isValid ? GetTime() : nil
  }

  // MARK: - Foundation Integration

  /// Convert to a Foundation Date, or nil if invalid.
  var date: Date?
  {
    guard isValid else { return nil }
    return Date(timeIntervalSince1970: GetTime())
  }

  /// Create from a Foundation Date.
  init(date: Date)
  {
    self.init(date.timeIntervalSince1970)
  }
}

// MARK: - CustomStringConvertible

extension ArTimestamp: CustomStringConvertible
{
  public var description: String
  {
    if isValid
    {
      "ArTimestamp(\(time))"
    }
    else
    {
      "ArTimestamp(invalid)"
    }
  }
}
