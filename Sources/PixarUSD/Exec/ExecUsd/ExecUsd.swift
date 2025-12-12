/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import ExecUsd

/**
 * # ``ExecUsd``
 *
 * **Execution System for USD**
 *
 * ## Overview
 *
 * **ExecUsd** is the **Execution System for USD** library, the primary entry point
 * for OpenExec. It provides APIs for:
 * - Registration of computational behaviors associated with USD schemas
 * - Ingesting a UsdStage to compile the data flow network
 * - Requesting values for efficient, vectorized, multithreaded evaluation
 *
 * ## Example Usage
 *
 * ```swift
 * // Create an execution system from a USD stage
 * let system = ExecUsd.System(stage: stage)
 *
 * // Create value keys for computations
 * let valueKey = ExecUsd.ValueKey(prim: prim, computation: ExecGeom.Xformable.computeLocalToWorldTransform)
 *
 * // Build and compute the request
 * var request = system.buildRequest(valueKeys: [valueKey])
 * let cacheView = system.compute(request: &request)
 *
 * // Extract the computed value
 * let transform = cacheView.get(index: 0)
 * ```
 */
public enum ExecUsd {}

// MARK: - Type Aliases

public extension ExecUsd
{
  /// Type alias for the request handle.
  /// This is exposed because it's a simple struct with void* and bool.
  typealias CxxRequestHandle = Pixar.ExecUsd_Swift_RequestHandle

  // Note: All other ExecUsd types (ExecUsdSystem, ExecUsdRequest, ExecUsdCacheView,
  // ExecUsdValueKey) are NOT exposed as type aliases because they crash Swift's
  // C++ interop compiler. They are accessed via opaque void* pointers through
  // bridge functions.

  // MARK: - Callback Types

  /// Information about the time interval during which values are invalid.
  ///
  /// This is a simplified representation of `EfTimeInterval` that can be
  /// efficiently passed across the Swift/C++ boundary.
  struct InvalidationTimeInterval
  {
    /// The minimum time of the invalid interval (may be -infinity).
    public let minTime: Double

    /// The maximum time of the invalid interval (may be +infinity).
    public let maxTime: Double

    /// Whether the default time (time-independent) is part of the invalid interval.
    public let includesDefaultTime: Bool

    /// Returns true if this represents an empty interval.
    public var isEmpty: Bool
    {
      !includesDefaultTime && minTime > maxTime
    }

    /// Returns true if this represents the full interval (all time).
    public var isFullInterval: Bool
    {
      includesDefaultTime && minTime == -.infinity && maxTime == .infinity
    }
  }

  /// Callback type for computed value invalidation.
  ///
  /// This callback is invoked when previously computed value keys become invalid
  /// as a result of authored value changes or structural invalidation of the scene.
  ///
  /// - Parameters:
  ///   - invalidIndices: The indices of value keys that became invalid.
  ///   - timeInterval: The time interval during which the values are invalid.
  ///
  /// - Important: Do not call into execution (including `compute()` or value extraction)
  ///   from within this callback.
  typealias ValueInvalidationCallback = (_ invalidIndices: [Int], _ timeInterval: InvalidationTimeInterval) -> Void

  /// Callback type for time change invalidation.
  ///
  /// This callback is invoked when previously computed value keys become invalid
  /// as a result of time changing. The invalid value keys are the set of time-dependent
  /// value keys, further filtered to only include the value keys where input dependencies
  /// are *actually* changing between the old time and new time.
  ///
  /// - Parameter invalidIndices: The indices of value keys that became invalid due to time change.
  ///
  /// - Important: Do not call into execution (including `compute()` or value extraction)
  ///   from within this callback.
  typealias TimeChangeCallback = (_ invalidIndices: [Int]) -> Void
}

// MARK: - Callback Trampolines

/// Internal class to box Swift closures for C callback context.
private final class CallbackBox
{
  var valueCallback: ExecUsd.ValueInvalidationCallback?
  var timeCallback: ExecUsd.TimeChangeCallback?

  init(valueCallback: @escaping ExecUsd.ValueInvalidationCallback)
  {
    self.valueCallback = valueCallback
    self.timeCallback = nil
  }

  init(timeCallback: @escaping ExecUsd.TimeChangeCallback)
  {
    self.valueCallback = nil
    self.timeCallback = timeCallback
  }
}

/// Trampoline function for value invalidation callbacks.
/// This is called from C++ and forwards to the Swift closure.
private func valueInvalidationTrampoline(
  context: UnsafeMutableRawPointer?,
  indices: UnsafePointer<Int32>?,
  indexCount: Int,
  timeIntervalMin: Double,
  timeIntervalMax: Double,
  includesDefaultTime: Bool
)
{
  guard let context = context, let indices = indices else { return }

  // Retrieve the boxed callback
  let box = Unmanaged<CallbackBox>.fromOpaque(context).takeUnretainedValue()
  guard let callback = box.valueCallback else { return }

  // Convert C array to Swift array
  var swiftIndices: [Int] = []
  swiftIndices.reserveCapacity(indexCount)
  for i in 0 ..< indexCount
  {
    swiftIndices.append(Int(indices[i]))
  }

  // Create time interval struct
  let timeInterval = ExecUsd.InvalidationTimeInterval(
    minTime: timeIntervalMin,
    maxTime: timeIntervalMax,
    includesDefaultTime: includesDefaultTime
  )

  // Call the Swift closure
  callback(swiftIndices, timeInterval)
}

/// Trampoline function for time change callbacks.
/// This is called from C++ and forwards to the Swift closure.
private func timeChangeTrampoline(
  context: UnsafeMutableRawPointer?,
  indices: UnsafePointer<Int32>?,
  indexCount: Int
)
{
  guard let context = context, let indices = indices else { return }

  // Retrieve the boxed callback
  let box = Unmanaged<CallbackBox>.fromOpaque(context).takeUnretainedValue()
  guard let callback = box.timeCallback else { return }

  // Convert C array to Swift array
  var swiftIndices: [Int] = []
  swiftIndices.reserveCapacity(indexCount)
  for i in 0 ..< indexCount
  {
    swiftIndices.append(Int(indices[i]))
  }

  // Call the Swift closure
  callback(swiftIndices)
}

// MARK: - ExecUsd.System

public extension ExecUsd
{
  /**
   * # ``ExecUsd/System``
   *
   * The implementation of a system to procedurally compute values based on USD
   * scene description and computation definitions.
   *
   * ## Overview
   *
   * `ExecUsd.System` specializes the base `ExecSystem` class and owns USD-specific
   * structures and logic necessary to compile, schedule and evaluate requested
   * computation values.
   *
   * The system extends the lifetime of the UsdStage it is constructed with,
   * although it is atypical for a system to outlive its stage in practice.
   * As a rule of thumb, the system lives right alongside the UsdStage in most use-cases.
   *
   * ## Memory Management
   *
   * This wrapper manages the lifetime of the underlying C++ ExecUsdSystem.
   * The system is automatically destroyed when this Swift object is deallocated.
   *
   * **IMPORTANT**: Requests and CacheViews created from this system must not outlive
   * the system. Ensure requests and cache views are local variables that go out of
   * scope before the system does.
   */
  final class System
  {
    /// The underlying C++ system pointer (stored as opaque raw pointer).
    /// This is actually an ExecUsdSystem* but Swift can't see that type
    /// because it contains std::unique_ptr members.
    private var _systemPtr: UnsafeMutableRawPointer?

    /// Creates a new execution system from a USD stage.
    ///
    /// - Parameter stage: The USD stage to create the system from.
    public init(stage: UsdStageRefPtr)
    {
      // Create the system using the bridge function that accepts UsdStageRefPtr
      _systemPtr = Pixar.ExecUsd_Swift_CreateSystemFromStage(stage)
    }

    deinit
    {
      if let ptr = _systemPtr
      {
        // The system is destroyed last. Any outstanding requests will have
        // already been destroyed by Swift ARC when they went out of scope,
        // as long as they are local variables that go out of scope before
        // the system.
        Pixar.ExecUsd_Swift_DestroySystem(ptr)
      }
    }

    /// Returns whether the system is valid.
    public var isValid: Bool
    {
      _systemPtr != nil
    }

    /// Changes the time at which values are computed.
    ///
    /// Calling this method re-resolves time-dependent inputs from the scene
    /// graph at the new time, and determines which of these inputs are
    /// *actually* changing between the old and new time. Computed values that
    /// are dependent on the changing inputs are then invalidated, and requests
    /// are notified of the time change.
    ///
    /// - Note: When computing multiple requests over multiple times, it is much more
    ///   efficient to compute all requests at the same time, before moving on to
    ///   the next time. Doing so allows time-dependent intermediate results to
    ///   remain cached and be re-used across the multiple calls to `compute()`.
    ///
    /// - Parameter time: The new time to compute at.
    public func changeTime(_ time: Double)
    {
      guard let ptr = _systemPtr else { return }
      Pixar.ExecUsd_Swift_ChangeTime(ptr, time)
    }

    /// Changes the time at which values are computed using a UsdTimeCode.
    ///
    /// - Parameter timeCode: The new time code to compute at.
    public func changeTime(_ timeCode: Pixar.UsdTimeCode)
    {
      guard let ptr = _systemPtr else { return }
      Pixar.ExecUsd_Swift_ChangeTimeCode(ptr, timeCode)
    }

    /// Changes the time at which values are computed using an Ef.Time.
    ///
    /// This method allows specifying spline evaluation flags along with the time.
    /// Note: Spline flags are reserved for future API expansion and are currently
    /// not used by the underlying C++ implementation.
    ///
    /// - Parameter time: The new time to compute at, including optional spline flags.
    public func changeTime(_ time: Ef.Time)
    {
      guard let ptr = _systemPtr else { return }
      Pixar.ExecUsd_Swift_ChangeTimeWithFlags(
        ptr,
        time.frameValue,
        time.isDefault,
        time.splineEvaluationFlags
      )
    }

    /// Builds a request for the given value keys.
    ///
    /// - Parameter valueKeys: The value keys to include in the request.
    /// - Returns: A request object for computing the specified values.
    ///
    /// - Important: The returned request must not outlive this system. Ensure the
    ///   request goes out of scope before the system does.
    public func buildRequest(valueKeys: [ExecUsd.ValueKey]) -> ExecUsd.Request
    {
      guard let ptr = _systemPtr
      else
      {
        return Request()
      }

      // Create a C++ vector and populate it
      guard let vec = Pixar.ExecUsd_Swift_CreateValueKeyVector()
      else
      {
        return Request()
      }
      defer { Pixar.ExecUsd_Swift_DestroyValueKeyVector(vec) }

      for key in valueKeys
      {
        if let keyPtr = key._valueKeyPtr
        {
          Pixar.ExecUsd_Swift_ValueKeyVectorPush(vec, keyPtr)
        }
      }

      // Build the request using the opaque system pointer
      let handle = Pixar.ExecUsd_Swift_BuildRequest(ptr, vec)
      return Request(handle: handle)
    }

    /// Builds a request for the given value keys with invalidation callbacks.
    ///
    /// The optionally provided `onValueInvalidation` callback will be invoked when
    /// previously computed value keys become invalid as a result of authored
    /// value changes or structural invalidation of the scene. If multiple
    /// value keys become invalid at the same time, they may be batched into a
    /// single invocation of the callback.
    ///
    /// The optionally provided `onTimeChange` callback will be invoked when
    /// previously computed value keys become invalid as a result of time
    /// changing. The invalid value keys are the set of time-dependent value
    /// keys in this request, further filtered to only include the value keys
    /// where input dependencies are *actually* changing between the old time
    /// and new time.
    ///
    /// - Note: The `onValueInvalidation` callback is only guaranteed to be invoked at least
    ///   once per invalid value key and invalid time interval combination, and only after
    ///   `compute()` has been called. If clients want to be notified of future invalidation,
    ///   they must call `compute()` again to renew their interest in the computed value keys.
    ///
    /// - Important: The client must not call into execution (including, but not limited to
    ///   `compute()` or value extraction) from within the callbacks.
    ///
    /// - Parameters:
    ///   - valueKeys: The value keys to include in the request.
    ///   - onValueInvalidation: Callback invoked when values become invalid.
    ///   - onTimeChange: Callback invoked when values become invalid due to time changes.
    /// - Returns: A request object for computing the specified values.
    ///
    /// - Important: The returned request must not outlive this system. Ensure the
    ///   request goes out of scope before the system does.
    public func buildRequest(
      valueKeys: [ExecUsd.ValueKey],
      onValueInvalidation: ExecUsd.ValueInvalidationCallback? = nil,
      onTimeChange: ExecUsd.TimeChangeCallback? = nil
    ) -> ExecUsd.Request
    {
      guard let ptr = _systemPtr
      else
      {
        return Request()
      }

      // Create a C++ vector and populate it
      guard let vec = Pixar.ExecUsd_Swift_CreateValueKeyVector()
      else
      {
        return Request()
      }
      defer { Pixar.ExecUsd_Swift_DestroyValueKeyVector(vec) }

      for key in valueKeys
      {
        if let keyPtr = key._valueKeyPtr
        {
          Pixar.ExecUsd_Swift_ValueKeyVectorPush(vec, keyPtr)
        }
      }

      // If no callbacks, use the simpler bridge function
      if onValueInvalidation == nil && onTimeChange == nil
      {
        let handle = Pixar.ExecUsd_Swift_BuildRequest(ptr, vec)
        return Request(handle: handle)
      }

      // Create context objects to hold the Swift closures
      // We use Unmanaged to prevent ARC from deallocating them while in use
      var valueCallbackContext: UnsafeMutableRawPointer? = nil
      var timeCallbackContext: UnsafeMutableRawPointer? = nil

      // Wrap value invalidation callback
      let valueCallbackPtr: Pixar.ExecUsd_Swift_ValueInvalidationCallback?
      if let callback = onValueInvalidation
      {
        // Box the callback and retain it
        let boxed = CallbackBox(valueCallback: callback)
        valueCallbackContext = Unmanaged.passRetained(boxed).toOpaque()
        valueCallbackPtr = valueInvalidationTrampoline
      }
      else
      {
        valueCallbackPtr = nil
      }

      // Wrap time change callback
      let timeCallbackPtr: Pixar.ExecUsd_Swift_TimeChangeCallback?
      if let callback = onTimeChange
      {
        // Box the callback and retain it
        let boxed = CallbackBox(timeCallback: callback)
        timeCallbackContext = Unmanaged.passRetained(boxed).toOpaque()
        timeCallbackPtr = timeChangeTrampoline
      }
      else
      {
        timeCallbackPtr = nil
      }

      // Build the request with callbacks
      let handle = Pixar.ExecUsd_Swift_BuildRequestWithCallbacks(
        ptr,
        vec,
        valueCallbackPtr,
        valueCallbackContext,
        timeCallbackPtr,
        timeCallbackContext
      )

      // Create request and attach context ownership
      let request = Request(handle: handle)
      request._valueCallbackContext = valueCallbackContext
      request._timeCallbackContext = timeCallbackContext

      return request
    }

    /// Prepares a given request for execution.
    ///
    /// This ensures the exec network is compiled and scheduled for the value
    /// keys in the request. `compute()` will implicitly prepare the request
    /// if needed, but calling `prepareRequest()` separately enables clients to
    /// front-load compilation and scheduling cost.
    ///
    /// - Parameter request: The request to prepare.
    public func prepareRequest(_ request: inout ExecUsd.Request)
    {
      guard let ptr = _systemPtr else { return }
      Pixar.ExecUsd_Swift_PrepareRequest(ptr, &request._handle)
    }

    /// Executes the given request and returns a cache view for extracting
    /// the computed values.
    ///
    /// This implicitly calls `prepareRequest()`, though clients may choose to
    /// call `prepareRequest()` ahead of time and front-load the associated
    /// compilation and scheduling cost.
    ///
    /// - Parameter request: The request to compute.
    /// - Returns: A cache view for extracting computed values.
    public func compute(request: inout ExecUsd.Request) -> ExecUsd.CacheView
    {
      guard let ptr = _systemPtr
      else
      {
        return CacheView()
      }
      let cacheViewPtr = Pixar.ExecUsd_Swift_Compute(ptr, &request._handle)
      return CacheView(cacheViewPtr: cacheViewPtr)
    }

    /// Creates a diagnostics object for this system.
    ///
    /// - Returns: A diagnostics object for debugging and profiling.
    ///
    /// - Important: The diagnostics object must not outlive this system.
    public func createDiagnostics() -> ExecUsd.Diagnostics
    {
      guard let ptr = _systemPtr
      else
      {
        return Diagnostics()
      }
      return Diagnostics(systemPtr: ptr)
    }
  }
}

// MARK: - ExecUsd.Diagnostics

public extension ExecUsd
{
  /**
   * # ``ExecUsd/Diagnostics``
   *
   * Utility class with diagnostic functions for the execution system.
   *
   * ## Overview
   *
   * `ExecUsd.Diagnostics` provides debugging and profiling capabilities:
   * - `invalidateAll()`: Reset the system to its initial state
   * - `graphNetwork(filename:)`: Generate a DOT graph visualization of the computation network
   *
   * ## Memory Management
   *
   * This wrapper manages the lifetime of the underlying C++ ExecSystem::Diagnostics.
   * The diagnostics object must not outlive the system it was created from.
   *
   * ## Example Usage
   *
   * ```swift
   * let system = ExecUsd.System(stage: stage)
   * let diagnostics = system.createDiagnostics()
   *
   * // Generate a DOT graph of the computation network
   * diagnostics.graphNetwork(filename: "/tmp/exec_network.dot")
   *
   * // Reset the system to its initial state
   * diagnostics.invalidateAll()
   * ```
   */
  final class Diagnostics
  {
    /// The underlying C++ diagnostics pointer (stored as opaque raw pointer).
    private var _diagnosticsPtr: UnsafeMutableRawPointer?

    /// Creates an invalid diagnostics object.
    init()
    {
      _diagnosticsPtr = nil
    }

    /// Creates a diagnostics object from a system pointer (internal use).
    init(systemPtr: UnsafeMutableRawPointer)
    {
      _diagnosticsPtr = Pixar.ExecUsd_Swift_CreateDiagnostics(systemPtr)
    }

    deinit
    {
      if let ptr = _diagnosticsPtr
      {
        Pixar.ExecUsd_Swift_DestroyDiagnostics(ptr)
      }
    }

    /// Returns whether the diagnostics object is valid.
    public var isValid: Bool
    {
      _diagnosticsPtr != nil
    }

    /// Invalidates all internal state of the exec system, resetting it
    /// to a state equivalent to when it was first constructed.
    ///
    /// This is useful for:
    /// - Forcing recompilation of the computation network
    /// - Clearing all cached values
    /// - Debugging issues with incremental updates
    public func invalidateAll()
    {
      guard let ptr = _diagnosticsPtr else { return }
      Pixar.ExecUsd_Swift_Diagnostics_InvalidateAll(ptr)
    }

    /// Produces a DOT graph of the currently compiled exec network
    /// and writes its contents to the specified file.
    ///
    /// The generated DOT file can be visualized using Graphviz tools:
    /// ```bash
    /// dot -Tpng exec_network.dot -o exec_network.png
    /// ```
    ///
    /// - Parameter filename: The path to write the DOT graph file.
    public func graphNetwork(filename: String)
    {
      guard let ptr = _diagnosticsPtr else { return }
      filename.withCString { cstr in
        Pixar.ExecUsd_Swift_Diagnostics_GraphNetwork(ptr, cstr)
      }
    }
  }
}

// MARK: - ExecUsd.ValueKey

public extension ExecUsd
{
  /**
   * # ``ExecUsd/ValueKey``
   *
   * Specifies a computed value.
   *
   * ## Overview
   *
   * Clients identify computations to evaluate using a UsdObject that provides
   * computations and the name of the computation.
   *
   * ## Memory Management
   *
   * This wrapper manages the lifetime of the underlying C++ ExecUsdValueKey.
   * The value key is automatically destroyed when this Swift object is deallocated.
   */
  final class ValueKey
  {
    /// The underlying C++ value key pointer (stored as opaque raw pointer).
    /// This is actually an ExecUsdValueKey* but Swift can't see that type
    /// because it uses std::variant which crashes Swift's C++ interop.
    var _valueKeyPtr: UnsafeMutableRawPointer?

    /// Creates a value key that computes the builtin computeValue attribute computation.
    ///
    /// - Parameter attribute: The attribute provider.
    public init(attribute: Pixar.UsdAttribute)
    {
      _valueKeyPtr = Pixar.ExecUsd_Swift_CreateValueKeyFromAttribute(attribute)
    }

    /// Creates a value key representing an attribute computation.
    ///
    /// - Parameters:
    ///   - attribute: The attribute provider.
    ///   - computation: The name of the computation.
    public init(attribute: Pixar.UsdAttribute, computation: Pixar.TfToken)
    {
      _valueKeyPtr = Pixar.ExecUsd_Swift_CreateValueKeyFromAttributeAndToken(attribute, computation)
    }

    /// Creates a value key representing a prim computation.
    ///
    /// - Parameters:
    ///   - prim: The prim provider.
    ///   - computation: The name of the computation.
    public init(prim: Pixar.UsdPrim, computation: Pixar.TfToken)
    {
      _valueKeyPtr = Pixar.ExecUsd_Swift_CreateValueKeyFromPrimAndToken(prim, computation)
    }

    deinit
    {
      if let ptr = _valueKeyPtr
      {
        Pixar.ExecUsd_Swift_DestroyValueKey(ptr)
      }
    }

    /// Returns whether the value key is valid.
    public var isValid: Bool
    {
      _valueKeyPtr != nil
    }
  }
}

// MARK: - ExecUsd.Request

public extension ExecUsd
{
  /**
   * # ``ExecUsd/Request``
   *
   * A batch of values to compute together.
   *
   * ## Overview
   *
   * `ExecUsd.Request` allows clients to specify multiple values to compute at the
   * same time. It is more efficient to perform compilation, scheduling and
   * evaluation for many attributes at the same time than to perform each of
   * these steps value-by-value.
   */
  final class Request
  {
    /// The underlying handle.
    var _handle: Pixar.ExecUsd_Swift_RequestHandle

    /// Context pointer for value invalidation callback (retained CallbackBox).
    var _valueCallbackContext: UnsafeMutableRawPointer?

    /// Context pointer for time change callback (retained CallbackBox).
    var _timeCallbackContext: UnsafeMutableRawPointer?

    /// Creates an invalid request.
    init()
    {
      _handle = Pixar.ExecUsd_Swift_RequestHandle()
      _handle.request = nil
      _handle.isValid = false
      _valueCallbackContext = nil
      _timeCallbackContext = nil
    }

    /// Creates a request from a handle.
    init(handle: Pixar.ExecUsd_Swift_RequestHandle)
    {
      _handle = handle
      _valueCallbackContext = nil
      _timeCallbackContext = nil
    }

    deinit
    {
      // Release any retained callback contexts
      if let context = _valueCallbackContext
      {
        Unmanaged<CallbackBox>.fromOpaque(context).release()
      }
      if let context = _timeCallbackContext
      {
        Unmanaged<CallbackBox>.fromOpaque(context).release()
      }

      // Destroy the C++ request
      Pixar.ExecUsd_Swift_DestroyRequest(&_handle)
    }

    /// Returns true if this request may be used to compute values.
    ///
    /// - Note: A return value of true does not mean that values are cached
    ///   or even that the network has been compiled. It only means that
    ///   calling `prepareRequest()` or `compute()` is allowed.
    public var isValid: Bool
    {
      Pixar.ExecUsd_Swift_RequestIsValid(&_handle)
    }
  }
}

// MARK: - ExecUsd.CacheView

public extension ExecUsd
{
  /**
   * # ``ExecUsd/CacheView``
   *
   * Provides a view of values computed by `ExecUsd.System.compute()`.
   *
   * ## Overview
   *
   * Cache views must not outlive the `ExecUsd.System` or `ExecUsd.Request` from
   * which they were built.
   *
   * ## Memory Management
   *
   * This wrapper manages the lifetime of the underlying C++ ExecUsdCacheView.
   * The cache view is automatically destroyed when this Swift object is deallocated.
   * Note: The underlying type is handled as an opaque pointer because it contains
   * Exec_CacheView which has Vdf dependencies that crash Swift's C++ interop.
   */
  final class CacheView
  {
    /// The underlying C++ cache view pointer (stored as opaque raw pointer).
    /// This is actually an ExecUsdCacheView* but Swift can't see that type
    /// because it contains Exec_CacheView with Vdf dependencies.
    private var _cacheViewPtr: UnsafeMutableRawPointer?

    /// Creates an invalid cache view.
    init()
    {
      _cacheViewPtr = nil
    }

    /// Creates a cache view from an opaque pointer (internal use).
    init(cacheViewPtr: UnsafeMutableRawPointer?)
    {
      _cacheViewPtr = cacheViewPtr
    }

    deinit
    {
      if let ptr = _cacheViewPtr
      {
        Pixar.ExecUsd_Swift_DestroyCacheView(ptr)
      }
    }

    /// Returns whether the cache view is valid.
    public var isValid: Bool
    {
      _cacheViewPtr != nil
    }

    /// Returns the computed value for the provided extraction index.
    ///
    /// Emits an error and returns an empty value if the index is not evaluated.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed value.
    public func get(index: Int) -> Pixar.VtValue
    {
      guard let ptr = _cacheViewPtr
      else
      {
        return Pixar.VtValue()
      }
      return Pixar.ExecUsd_Swift_CacheViewGet(ptr, Int32(index))
    }

    /// Subscript access to computed values.
    public subscript(index: Int) -> Pixar.VtValue
    {
      return get(index: index)
    }

    // MARK: - Typed Value Extraction

    /// Returns the computed value at the given index as a `GfMatrix4d`.
    ///
    /// This is the most efficient way to extract transform matrices from
    /// OpenExec computations like `computeLocalToWorldTransform`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed matrix, or identity if the value is not a matrix or index is invalid.
    public func getMatrix4d(index: Int) -> Pixar.GfMatrix4d
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfMatrix4d(1.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetMatrix4d(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfMatrix4f`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed matrix, or identity if the value is not a matrix or index is invalid.
    public func getMatrix4f(index: Int) -> Pixar.GfMatrix4f
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfMatrix4f(1.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetMatrix4f(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `Double`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed double, or 0.0 if the value is not a double or index is invalid.
    public func getDouble(index: Int) -> Double
    {
      guard let ptr = _cacheViewPtr else { return 0.0 }
      return Pixar.ExecUsd_Swift_CacheViewGetDouble(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `Float`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed float, or 0.0 if the value is not a float or index is invalid.
    public func getFloat(index: Int) -> Float
    {
      guard let ptr = _cacheViewPtr else { return 0.0 }
      return Pixar.ExecUsd_Swift_CacheViewGetFloat(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as an `Int`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed int, or 0 if the value is not an int or index is invalid.
    public func getInt(index: Int) -> Int
    {
      guard let ptr = _cacheViewPtr else { return 0 }
      return Int(Pixar.ExecUsd_Swift_CacheViewGetInt(ptr, Int32(index)))
    }

    /// Returns the computed value at the given index as a `Bool`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed bool, or false if the value is not a bool or index is invalid.
    public func getBool(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewGetBool(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfVec2f`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed vector, or zero vector if the value is not a vec2f or index is invalid.
    public func getVec2f(index: Int) -> Pixar.GfVec2f
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfVec2f(0.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetVec2f(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfVec2d`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed vector, or zero vector if the value is not a vec2d or index is invalid.
    public func getVec2d(index: Int) -> Pixar.GfVec2d
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfVec2d(0.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetVec2d(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfVec3f`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed vector, or zero vector if the value is not a vec3f or index is invalid.
    public func getVec3f(index: Int) -> Pixar.GfVec3f
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfVec3f(0.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetVec3f(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfVec3d`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed vector, or zero vector if the value is not a vec3d or index is invalid.
    public func getVec3d(index: Int) -> Pixar.GfVec3d
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfVec3d(0.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetVec3d(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfVec4f`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed vector, or zero vector if the value is not a vec4f or index is invalid.
    public func getVec4f(index: Int) -> Pixar.GfVec4f
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfVec4f(0.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetVec4f(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfVec4d`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed vector, or zero vector if the value is not a vec4d or index is invalid.
    public func getVec4d(index: Int) -> Pixar.GfVec4d
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfVec4d(0.0) }
      return Pixar.ExecUsd_Swift_CacheViewGetVec4d(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfQuatf`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed quaternion, or identity if the value is not a quatf or index is invalid.
    public func getQuatf(index: Int) -> Pixar.GfQuatf
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfQuatf.GetIdentity() }
      return Pixar.ExecUsd_Swift_CacheViewGetQuatf(ptr, Int32(index))
    }

    /// Returns the computed value at the given index as a `GfQuatd`.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The computed quaternion, or identity if the value is not a quatd or index is invalid.
    public func getQuatd(index: Int) -> Pixar.GfQuatd
    {
      guard let ptr = _cacheViewPtr else { return Pixar.GfQuatd.GetIdentity() }
      return Pixar.ExecUsd_Swift_CacheViewGetQuatd(ptr, Int32(index))
    }

    // MARK: - Type Checking

    /// Returns true if the value at the given index is a `GfMatrix4d`.
    public func isHoldingMatrix4d(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingMatrix4d(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is a `GfMatrix4f`.
    public func isHoldingMatrix4f(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingMatrix4f(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is a `Double`.
    public func isHoldingDouble(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingDouble(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is a `Float`.
    public func isHoldingFloat(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingFloat(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is an `Int`.
    public func isHoldingInt(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingInt(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is a `Bool`.
    public func isHoldingBool(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingBool(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is a `GfVec3f`.
    public func isHoldingVec3f(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingVec3f(ptr, Int32(index))
    }

    /// Returns true if the value at the given index is a `GfVec3d`.
    public func isHoldingVec3d(index: Int) -> Bool
    {
      guard let ptr = _cacheViewPtr else { return false }
      return Pixar.ExecUsd_Swift_CacheViewIsHoldingVec3d(ptr, Int32(index))
    }

    /// Returns the type name of the value at the given index.
    ///
    /// Useful for debugging or determining which typed getter to use.
    ///
    /// - Parameter index: The extraction index.
    /// - Returns: The type name, or empty string if index is invalid or value is empty.
    public func getTypeName(index: Int) -> String
    {
      guard let ptr = _cacheViewPtr else { return "" }
      return String(Pixar.ExecUsd_Swift_CacheViewGetTypeName(ptr, Int32(index)))
    }
  }
}
