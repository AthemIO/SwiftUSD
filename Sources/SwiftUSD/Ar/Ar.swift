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
/// - ``ResolverContextBinder``: Scoped context binding
/// - ``ResolverScopedCache``: Scoped resolver caching
/// - ``DefaultResolverContext``: Search path context for the default resolver
/// - ``ResolvedPath``: A resolved asset path
/// - ``Timestamp``: Asset modification timestamp
/// - ``Asset``: Read-only access to asset contents
/// - ``WritableAsset``: Write access to asset contents
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
///
/// // Bind the context for a scope
/// try ResolverContextBinder.withBoundContext(resolverContext) {
///     // Resolution uses the bound context here
///     let resolved = Resolver.resolve("model.usd")
/// }
/// ```
///
/// ## Reading and Writing Assets
///
/// Assets can be read and written using the Asset and WritableAsset types:
///
/// ```swift
/// // Read an asset
/// if let resolved = Resolver.resolve("data.txt"),
///    let asset = Asset.open(resolved) {
///     if let contents = asset.string {
///         print(contents)
///     }
/// }
///
/// // Write an asset
/// if let resolved = Resolver.resolveForNewAsset("output.txt"),
///    let asset = WritableAsset.open(resolved, mode: .replace) {
///     asset.write("Hello, World!")
///     asset.close()
/// }
/// ```
///
/// ## Thread Safety
///
/// Most resolver operations are thread-safe. However, some operations
/// like `Resolver.setDefaultSearchPath(_:)` are not thread-safe and
/// should not be called concurrently with other resolver operations.
///
/// Context binding via ``ResolverContextBinder`` is thread-local.
///
/// Mirrors `pxr/usd/ar/` from the USD C++ library.
public enum Ar {
    // This enum serves as a namespace for the Ar module.
    // All types are defined in their own files.
}
