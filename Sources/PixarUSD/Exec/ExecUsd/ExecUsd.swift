/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
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

public extension ExecUsd {
    /// Type alias for the request handle.
    /// This is exposed because it's a simple struct with void* and bool.
    typealias CxxRequestHandle = Pixar.ExecUsd_Swift_RequestHandle

    // Note: All other ExecUsd types (ExecUsdSystem, ExecUsdRequest, ExecUsdCacheView,
    // ExecUsdValueKey) are NOT exposed as type aliases because they crash Swift's
    // C++ interop compiler. They are accessed via opaque void* pointers through
    // bridge functions.
}

// MARK: - ExecUsd.System

public extension ExecUsd {
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
    final class System {
        /// The underlying C++ system pointer (stored as opaque raw pointer).
        /// This is actually an ExecUsdSystem* but Swift can't see that type
        /// because it contains std::unique_ptr members.
        private var _systemPtr: UnsafeMutableRawPointer?

        /// Creates a new execution system from a USD stage.
        ///
        /// - Parameter stage: The USD stage to create the system from.
        public init(stage: UsdStageRefPtr) {
            // Create the system using the bridge function that accepts UsdStageRefPtr
            _systemPtr = Pixar.ExecUsd_Swift_CreateSystemFromStage(stage)
        }

        deinit {
            if let ptr = _systemPtr {
                // The system is destroyed last. Any outstanding requests will have
                // already been destroyed by Swift ARC when they went out of scope,
                // as long as they are local variables that go out of scope before
                // the system.
                Pixar.ExecUsd_Swift_DestroySystem(ptr)
            }
        }

        /// Returns whether the system is valid.
        public var isValid: Bool {
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
        public func changeTime(_ time: Double) {
            guard let ptr = _systemPtr else { return }
            Pixar.ExecUsd_Swift_ChangeTime(ptr, time)
        }

        /// Changes the time at which values are computed using a UsdTimeCode.
        ///
        /// - Parameter timeCode: The new time code to compute at.
        public func changeTime(_ timeCode: Pixar.UsdTimeCode) {
            guard let ptr = _systemPtr else { return }
            Pixar.ExecUsd_Swift_ChangeTimeCode(ptr, timeCode)
        }

        /// Builds a request for the given value keys.
        ///
        /// - Parameter valueKeys: The value keys to include in the request.
        /// - Returns: A request object for computing the specified values.
        ///
        /// - Important: The returned request must not outlive this system. Ensure the
        ///   request goes out of scope before the system does.
        public func buildRequest(valueKeys: [ExecUsd.ValueKey]) -> ExecUsd.Request {
            guard let ptr = _systemPtr else {
                return Request()
            }

            // Create a C++ vector and populate it
            guard let vec = Pixar.ExecUsd_Swift_CreateValueKeyVector() else {
                return Request()
            }
            defer { Pixar.ExecUsd_Swift_DestroyValueKeyVector(vec) }

            for key in valueKeys {
                if let keyPtr = key._valueKeyPtr {
                    Pixar.ExecUsd_Swift_ValueKeyVectorPush(vec, keyPtr)
                }
            }

            // Build the request using the opaque system pointer
            var handle = Pixar.ExecUsd_Swift_BuildRequest(ptr, vec)
            return Request(handle: handle)
        }

        /// Prepares a given request for execution.
        ///
        /// This ensures the exec network is compiled and scheduled for the value
        /// keys in the request. `compute()` will implicitly prepare the request
        /// if needed, but calling `prepareRequest()` separately enables clients to
        /// front-load compilation and scheduling cost.
        ///
        /// - Parameter request: The request to prepare.
        public func prepareRequest(_ request: inout ExecUsd.Request) {
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
        public func compute(request: inout ExecUsd.Request) -> ExecUsd.CacheView {
            guard let ptr = _systemPtr else {
                return CacheView()
            }
            let cacheViewPtr = Pixar.ExecUsd_Swift_Compute(ptr, &request._handle)
            return CacheView(cacheViewPtr: cacheViewPtr)
        }
    }
}

// MARK: - ExecUsd.ValueKey

public extension ExecUsd {
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
    final class ValueKey {
        /// The underlying C++ value key pointer (stored as opaque raw pointer).
        /// This is actually an ExecUsdValueKey* but Swift can't see that type
        /// because it uses std::variant which crashes Swift's C++ interop.
        internal var _valueKeyPtr: UnsafeMutableRawPointer?

        /// Creates a value key that computes the builtin computeValue attribute computation.
        ///
        /// - Parameter attribute: The attribute provider.
        public init(attribute: Pixar.UsdAttribute) {
            _valueKeyPtr = Pixar.ExecUsd_Swift_CreateValueKeyFromAttribute(attribute)
        }

        /// Creates a value key representing an attribute computation.
        ///
        /// - Parameters:
        ///   - attribute: The attribute provider.
        ///   - computation: The name of the computation.
        public init(attribute: Pixar.UsdAttribute, computation: Pixar.TfToken) {
            _valueKeyPtr = Pixar.ExecUsd_Swift_CreateValueKeyFromAttributeAndToken(attribute, computation)
        }

        /// Creates a value key representing a prim computation.
        ///
        /// - Parameters:
        ///   - prim: The prim provider.
        ///   - computation: The name of the computation.
        public init(prim: Pixar.UsdPrim, computation: Pixar.TfToken) {
            _valueKeyPtr = Pixar.ExecUsd_Swift_CreateValueKeyFromPrimAndToken(prim, computation)
        }

        deinit {
            if let ptr = _valueKeyPtr {
                Pixar.ExecUsd_Swift_DestroyValueKey(ptr)
            }
        }

        /// Returns whether the value key is valid.
        public var isValid: Bool {
            _valueKeyPtr != nil
        }
    }
}

// MARK: - ExecUsd.Request

public extension ExecUsd {
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
    final class Request {
        /// The underlying handle.
        internal var _handle: Pixar.ExecUsd_Swift_RequestHandle

        /// Creates an invalid request.
        internal init() {
            _handle = Pixar.ExecUsd_Swift_RequestHandle()
            _handle.request = nil
            _handle.isValid = false
        }

        /// Creates a request from a handle.
        internal init(handle: Pixar.ExecUsd_Swift_RequestHandle) {
            _handle = handle
        }

        deinit {
            Pixar.ExecUsd_Swift_DestroyRequest(&_handle)
        }

        /// Returns true if this request may be used to compute values.
        ///
        /// - Note: A return value of true does not mean that values are cached
        ///   or even that the network has been compiled. It only means that
        ///   calling `prepareRequest()` or `compute()` is allowed.
        public var isValid: Bool {
            Pixar.ExecUsd_Swift_RequestIsValid(&_handle)
        }
    }
}

// MARK: - ExecUsd.CacheView

public extension ExecUsd {
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
    final class CacheView {
        /// The underlying C++ cache view pointer (stored as opaque raw pointer).
        /// This is actually an ExecUsdCacheView* but Swift can't see that type
        /// because it contains Exec_CacheView with Vdf dependencies.
        private var _cacheViewPtr: UnsafeMutableRawPointer?

        /// Creates an invalid cache view.
        internal init() {
            _cacheViewPtr = nil
        }

        /// Creates a cache view from an opaque pointer (internal use).
        internal init(cacheViewPtr: UnsafeMutableRawPointer?) {
            _cacheViewPtr = cacheViewPtr
        }

        deinit {
            if let ptr = _cacheViewPtr {
                Pixar.ExecUsd_Swift_DestroyCacheView(ptr)
            }
        }

        /// Returns whether the cache view is valid.
        public var isValid: Bool {
            _cacheViewPtr != nil
        }

        /// Returns the computed value for the provided extraction index.
        ///
        /// Emits an error and returns an empty value if the index is not evaluated.
        ///
        /// - Parameter index: The extraction index.
        /// - Returns: The computed value.
        public func get(index: Int) -> Pixar.VtValue {
            guard let ptr = _cacheViewPtr else {
                return Pixar.VtValue()
            }
            return Pixar.ExecUsd_Swift_CacheViewGet(ptr, Int32(index))
        }

        /// Subscript access to computed values.
        public subscript(index: Int) -> Pixar.VtValue {
            return get(index: index)
        }
    }
}
