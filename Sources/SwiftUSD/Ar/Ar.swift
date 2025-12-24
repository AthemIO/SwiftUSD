// Ar.swift - Asset Resolution module
// Mirrors: pxr/usd/ar/

/// The Ar (Asset Resolution) module provides the interface for resolving
/// asset paths to their physical locations.
///
/// Asset resolution is a critical part of USD's architecture, allowing
/// asset paths in USD files to be resolved to actual file locations based
/// on configurable resolution rules.
///
/// ## Overview
///
/// The main types in this module are:
///
/// - ``Resolver``: The global asset resolver interface
/// - ``ResolverContext``: Context data that influences resolution
/// - ``DefaultResolverContext``: Search path context for the default resolver
/// - ``ResolvedPath``: A resolved asset path
/// - ``Timestamp``: Asset modification timestamp
///
/// ## Basic Usage
///
/// ```swift
/// // Resolve an asset path
/// if let resolved = Resolver.resolve("models/cube.usd") {
///     print("Resolved to: \(resolved.pathString)")
/// }
///
/// // Create a default resolver context with search paths
/// let context = try DefaultResolverContext(searchPaths: [
///     "/assets/models",
///     "/assets/textures"
/// ])
///
/// // Get the file extension
/// let ext = Resolver.getExtension(for: "model.usda")
/// print("Extension: \(ext)")  // "usda"
/// ```
///
/// ## Resolver Contexts
///
/// Resolver contexts allow you to customize how asset paths are resolved.
/// The default resolver uses search paths to locate assets:
///
/// ```swift
/// let context = try DefaultResolverContext(searchPaths: [
///     "/project/assets",
///     "/shared/assets"
/// ])
///
/// // Convert to a generic context for binding
/// let resolverContext = try context.toResolverContext()
/// ```
///
/// ## Thread Safety
///
/// Most resolver operations are thread-safe. However, some operations
/// like `Resolver.setDefaultSearchPath(_:)` are not thread-safe and
/// should not be called concurrently with other resolver operations.
///
/// Mirrors `pxr/usd/ar/` from the USD C++ library.
public enum Ar {
    // This enum serves as a namespace for the Ar module.
    // All types are defined in their own files.
}
