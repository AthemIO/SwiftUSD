// ResolverContextBinder.swift - ArResolverContextBinder wrapper
// Mirrors: pxr/usd/ar/resolverContextBinder.h

import OpenUSDInterop

// MARK: - ResolverContextBinder

/// A scoped context binder that binds a resolver context for the duration
/// of its lifetime.
///
/// This class binds a resolver context when created and automatically
/// unbinds it when released. This is useful for ensuring that asset
/// resolution uses a specific context within a given scope.
///
/// Example usage:
/// ```swift
/// let context = try DefaultResolverContext(searchPaths: ["/assets"])
/// let resolverContext = try context.toResolverContext()
///
/// // The context is bound for the lifetime of the binder
/// let binder = try ResolverContextBinder(context: resolverContext)
///
/// // All resolution operations here use the bound context
/// let resolved = Resolver.resolve("model.usd")
///
/// // Context is automatically unbound when binder goes out of scope
/// ```
///
/// - Note: The context remains bound until the binder is deallocated.
///   For predictable unbinding, use the ``withBoundContext(_:perform:)``
///   static method instead.
///
/// Mirrors `pxr::ArResolverContextBinder` from the USD C++ API.
public final class ResolverContextBinder: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    private let handle: ArResolverContextBinderRef

    // MARK: - Initialization

    /// Creates a context binder that binds the given resolver context.
    ///
    /// The context will remain bound until this binder is deallocated.
    ///
    /// - Parameter context: The resolver context to bind.
    /// - Throws: `ResolverContextBinderError.bindingFailed` if binding fails.
    public init(context: ResolverContext) throws {
        guard let ref = ArResolverContextBinder_Create(context.handle) else {
            throw ResolverContextBinderError.bindingFailed("Failed to bind resolver context")
        }
        self.handle = ref
    }

    deinit {
        ArResolverContextBinder_Release(handle)
    }

    // MARK: - Properties

    /// The context that is bound by this binder.
    public var boundContext: ResolverContext? {
        guard let ref = ArResolverContextBinder_GetContext(handle) else {
            return nil
        }
        return ResolverContext(handle: ref)
    }

    // MARK: - Convenience Methods

    /// Executes a closure with a bound resolver context.
    ///
    /// This method provides a convenient way to bind a context for a
    /// specific scope and ensures the context is unbound when the
    /// closure completes, even if an error is thrown.
    ///
    /// Example usage:
    /// ```swift
    /// let context = try DefaultResolverContext(searchPaths: ["/assets"])
    /// let resolverContext = try context.toResolverContext()
    ///
    /// let result = try ResolverContextBinder.withBoundContext(resolverContext) {
    ///     // Resolution uses the bound context
    ///     return Resolver.resolve("model.usd")
    /// }
    /// ```
    ///
    /// - Parameters:
    ///   - context: The resolver context to bind.
    ///   - body: The closure to execute with the bound context.
    /// - Returns: The result of the closure.
    /// - Throws: Rethrows any error from the closure, or
    ///   `ResolverContextBinderError.bindingFailed` if binding fails.
    public static func withBoundContext<T>(
        _ context: ResolverContext,
        perform body: () throws -> T
    ) throws -> T {
        let binder = try ResolverContextBinder(context: context)
        _ = binder // Keep binder alive for the scope
        return try body()
    }

    /// Executes an async closure with a bound resolver context.
    ///
    /// - Parameters:
    ///   - context: The resolver context to bind.
    ///   - body: The async closure to execute with the bound context.
    /// - Returns: The result of the closure.
    /// - Throws: Rethrows any error from the closure, or
    ///   `ResolverContextBinderError.bindingFailed` if binding fails.
    @available(macOS 10.15, iOS 13.0, watchOS 6.0, tvOS 13.0, *)
    public static func withBoundContext<T>(
        _ context: ResolverContext,
        perform body: () async throws -> T
    ) async throws -> T {
        let binder = try ResolverContextBinder(context: context)
        _ = binder // Keep binder alive for the scope
        return try await body()
    }
}

// MARK: - ResolverContextBinderError

/// Errors that can occur when working with resolver context binders.
public enum ResolverContextBinderError: Error, CustomStringConvertible {

    /// Context binding failed.
    case bindingFailed(String)

    public var description: String {
        switch self {
        case .bindingFailed(let message):
            return "Resolver context binding failed: \(message)"
        }
    }
}
