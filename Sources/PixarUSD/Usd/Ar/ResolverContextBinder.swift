/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar

// Note: ArResolverContextBinder is a C++ RAII class.
// Swift will call its destructor when the object goes out of scope,
// which will unbind the context automatically.

public extension Ar
{
  /// Swift-friendly helper to bind a context for a specific scope.
  ///
  /// Example usage:
  /// ```swift
  /// let context = resolver.createDefaultContext()
  /// Ar.withBoundContext(context) {
  ///     // Context is bound in this scope
  ///     let resolved = Ar.getResolver().resolve("model.usd")
  /// }
  /// // Context is automatically unbound when closure returns
  /// ```
  ///
  /// - Parameters:
  ///   - context: The context to bind
  ///   - body: The closure to execute with the context bound
  /// - Returns: The result of the closure
  /// - Throws: Rethrows any error from the closure
  @discardableResult
  static func withBoundContext<T>(
    _ context: ArResolverContext,
    body: () throws -> T
  ) rethrows -> T
  {
    // Create binder - context is bound on construction
    let binder = ArResolverContextBinder(context)
    // Keep binder alive during body execution
    defer { _ = binder }
    // Execute body with context bound
    return try body()
  }
}
