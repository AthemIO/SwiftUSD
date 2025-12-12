/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ef

/**
 * # ``Ef``
 *
 * **Execution Foundation**
 *
 * ## Overview
 *
 * **Ef** is the **Execution Foundation** library, built on top of Vdf.
 * It provides VdfNode types, executor interfaces, caching structures,
 * and various utility functions for the execution system.
 *
 * ## Key Types
 *
 * - `Ef.Time`: Represents a point in time for execution with spline evaluation flags
 */
public enum Ef {}

// MARK: - Type Aliases

public extension Ef
{
  /// Type alias for the C++ EfTime class.
  ///
  /// Represents a point in time for execution with optional spline evaluation flags.
  typealias CxxTime = Pixar.EfTime
}

// MARK: - Ef.Time

public extension Ef
{
  /**
   * # ``Ef/Time``
   *
   * A Swift-friendly wrapper for execution time.
   *
   * ## Overview
   *
   * `Ef.Time` represents a point in time for execution. It wraps `UsdTimeCode`
   * with additional spline evaluation flags that can affect how time-dependent
   * values are computed.
   *
   * ## Spline Evaluation Flags
   *
   * The `splineEvaluationFlags` property contains application-specific flags
   * that affect spline evaluation. These flags are consumed by the application's
   * spline evaluation logic.
   *
   * ## Example Usage
   *
   * ```swift
   * // Create time at frame 24
   * let time = Ef.Time(frame: 24.0)
   *
   * // Create time with spline flags
   * let timeWithFlags = Ef.Time(frame: 24.0, splineFlags: 0x01)
   *
   * // Create default time (not on timeline)
   * let defaultTime = Ef.Time.default
   *
   * // Use with ExecUsd.System
   * system.changeTime(time)
   * ```
   */
  struct Time
  {
    /// The frame value, or nil for default time.
    public let frame: Double?

    /// Application-specific spline evaluation flags.
    public let splineEvaluationFlags: UInt8

    /// Creates a time at the specified frame.
    ///
    /// - Parameters:
    ///   - frame: The frame number.
    ///   - splineFlags: Optional spline evaluation flags (default: 0).
    public init(frame: Double, splineFlags: UInt8 = 0)
    {
      self.frame = frame
      self.splineEvaluationFlags = splineFlags
    }

    /// Creates a default time (not on the timeline).
    ///
    /// A default time represents a point that is not on the timeline,
    /// useful for time-independent computations.
    ///
    /// - Parameter splineFlags: Optional spline evaluation flags (default: 0).
    public init(defaultTime splineFlags: UInt8 = 0)
    {
      frame = nil
      splineEvaluationFlags = splineFlags
    }

    /// The default time (not on the timeline).
    public static var `default`: Time
    {
      Time(defaultTime: 0)
    }

    /// Creates a time from a UsdTimeCode.
    ///
    /// - Parameters:
    ///   - timeCode: The USD time code.
    ///   - splineFlags: Optional spline evaluation flags (default: 0).
    public init(timeCode: Pixar.UsdTimeCode, splineFlags: UInt8 = 0)
    {
      if timeCode.IsDefault()
      {
        frame = nil
      }
      else
      {
        frame = timeCode.GetValue()
      }
      splineEvaluationFlags = splineFlags
    }

    /// Returns true if this is the default time (not on timeline).
    public var isDefault: Bool
    {
      frame == nil
    }

    /// Returns the frame value, or 0.0 if this is the default time.
    public var frameValue: Double
    {
      frame ?? 0.0
    }

    /// Converts to a UsdTimeCode.
    public var timeCode: Pixar.UsdTimeCode
    {
      if let frame
      {
        return Pixar.UsdTimeCode(frame)
      }
      else
      {
        return Pixar.UsdTimeCode.Default()
      }
    }
  }
}
