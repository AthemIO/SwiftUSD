// UsdUtils.swift - USD Utilities
// Mirrors: pxr/usd/usdUtils/stageCache.h, flattenLayerStack.h, stitch.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - StageCache

/// A global stage cache for sharing stages across an application.
///
/// `StageCache` provides access to the singleton USD stage cache, which allows
/// different parts of an application to share and reuse `Stage` instances.
/// This is particularly useful for avoiding redundant loading of the same
/// USD files.
///
/// Mirrors `pxr::UsdUtilsStageCache` from the USD C++ API.
public enum StageCache {

    /// A unique identifier for a stage in the cache.
    public typealias Id = Int64

    // MARK: - Properties

    /// The number of stages currently in the cache.
    public static var count: Int {
        Int(UsdUtilsStageCache_GetCount())
    }

    /// Whether the cache is empty.
    public static var isEmpty: Bool {
        count == 0
    }

    // MARK: - Methods

    /// Finds a stage in the cache by its ID.
    ///
    /// - Parameter id: The cache ID of the stage.
    /// - Returns: The stage if found, or `nil` if not in the cache.
    public static func find(id: Id) -> Stage? {
        guard let ref = UsdUtilsStageCache_Find(id) else {
            return nil
        }
        return Stage(handle: ref)
    }

    /// Finds a stage in the cache by its root layer path.
    ///
    /// - Parameter layerPath: The file path of the stage's root layer.
    /// - Returns: The stage if found, or `nil` if not in the cache.
    public static func find(byLayerPath layerPath: String) -> Stage? {
        guard let ref = UsdUtilsStageCache_FindByLayerPath(layerPath) else {
            return nil
        }
        return Stage(handle: ref)
    }

    /// Inserts a stage into the cache.
    ///
    /// The cache takes a reference to the stage, so the stage will remain
    /// valid as long as it is in the cache.
    ///
    /// - Parameter stage: The stage to insert.
    /// - Returns: The cache ID for the inserted stage, or `nil` on error.
    @discardableResult
    public static func insert(_ stage: Stage) -> Id? {
        let id = UsdUtilsStageCache_Insert(stage.handle)
        return id >= 0 ? id : nil
    }

    /// Removes a stage from the cache by its ID.
    ///
    /// - Parameter id: The cache ID of the stage to remove.
    /// - Returns: `true` if the stage was removed, `false` if not found.
    @discardableResult
    public static func erase(id: Id) -> Bool {
        UsdUtilsStageCache_Erase(id)
    }

    /// Removes a stage from the cache.
    ///
    /// - Parameter stage: The stage to remove.
    /// - Returns: `true` if the stage was removed, `false` if not found.
    @discardableResult
    public static func erase(_ stage: Stage) -> Bool {
        UsdUtilsStageCache_EraseStage(stage.handle)
    }

    /// Clears all stages from the cache.
    public static func clear() {
        UsdUtilsStageCache_Clear()
    }

    /// Gets all stages currently in the cache.
    ///
    /// - Returns: An array of all cached stages.
    public static var allStages: [Stage] {
        let count = UsdUtilsStageCache_GetCount()
        guard count > 0 else { return [] }

        var handles = [UsdStageRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdUtilsStageCache_GetAllStages(buffer.baseAddress, count)
        }

        var result = [Stage]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Stage(handle: h))
            }
        }
        return result
    }

    /// Gets all stage IDs currently in the cache.
    ///
    /// - Returns: An array of all cached stage IDs.
    public static var allIds: [Id] {
        let count = UsdUtilsStageCache_GetCount()
        guard count > 0 else { return [] }

        var ids = [Id](repeating: 0, count: count)
        let actualCount = ids.withUnsafeMutableBufferPointer { buffer in
            UsdUtilsStageCache_GetAllIds(buffer.baseAddress, count)
        }

        return Array(ids.prefix(actualCount))
    }
}

// MARK: - Layer Stack Flattening

/// Utilities for flattening layer stacks.
public enum LayerStackUtils {

    /// Flatten the root layer stack of the given stage into a single layer.
    ///
    /// The result layer can be substituted for the original layer stack
    /// while producing the same composed `Stage`.
    ///
    /// Unlike `Stage.export()`, this function does not flatten composition arcs
    /// such as references, payloads, inherits, specializes, or variants.
    ///
    /// Sublayer time offsets will be applied to remap any time-keyed scene
    /// description, such as time samples and clips.
    ///
    /// Asset paths will be resolved to absolute form to ensure they continue
    /// to identify the same asset from the output layer.
    ///
    /// - Parameters:
    ///   - stage: The stage whose layer stack should be flattened.
    ///   - tag: An optional tag for the new layer (e.g., "flattened").
    /// - Returns: A new layer containing the flattened layer stack.
    /// - Throws: `UsdUtilsError.flattenFailed` if flattening fails.
    public static func flattenLayerStack(of stage: Stage, tag: String? = nil) throws -> Layer {
        guard let ref = UsdUtilsFlattenLayerStack(stage.handle, tag) else {
            throw UsdUtilsError.flattenFailed("Failed to flatten layer stack")
        }
        return Layer(handle: ref)
    }

    /// Resolves an asset path during flatten layer stack operation.
    ///
    /// For layer-relative paths, returns an absolute path.
    /// For search paths and absolute paths, returns the original path.
    ///
    /// - Parameters:
    ///   - sourceLayer: The layer containing the asset path.
    ///   - assetPath: The asset path to resolve.
    /// - Returns: The resolved asset path.
    public static func resolveAssetPath(
        sourceLayer: Layer,
        assetPath: String
    ) -> String {
        guard let result = UsdUtilsFlattenLayerStackResolveAssetPath(
            sourceLayer.handle,
            assetPath
        ) else {
            return assetPath
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }
}

// MARK: - Layer Stitching

/// Utilities for stitching (merging) layers.
public enum StitchUtils {

    /// Merge all scene description in the weak layer into the strong layer.
    ///
    /// Prims and properties in `weakLayer` that do not exist in `strongLayer`
    /// will be copied into `strongLayer`. Prims and properties that do exist
    /// in `strongLayer` will be merged with the existing scene description.
    ///
    /// Merging prims and properties is done on a field-by-field basis.
    /// In general, if a field has a value in `strongLayer`, the value from
    /// `weakLayer` will be ignored. However, certain fields have special
    /// rules for merging values together:
    ///
    /// - For map and dictionary-valued fields (including time samples),
    ///   a dictionary merge is performed; values in the weaker dictionary
    ///   are copied into the stronger dictionary only if the key does not
    ///   already exist.
    ///
    /// - For listOp-valued fields, the listOps will be combined into a
    ///   single listOp.
    ///
    /// - The minimum startTimeCode and maximum endTimeCode will be used.
    ///
    /// - Parameters:
    ///   - strongLayer: The layer that takes precedence in conflicts (modified in place).
    ///   - weakLayer: The layer to merge from.
    /// - Throws: `UsdUtilsError.stitchFailed` if stitching fails.
    public static func stitchLayers(
        strong strongLayer: Layer,
        weak weakLayer: Layer
    ) throws {
        let result = UsdUtilsStitchLayers(strongLayer.handle, weakLayer.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw UsdUtilsError.stitchFailed("Failed to stitch layers")
        }
    }
}

// MARK: - Dependencies Utilities

/// The result of extracting external references from a USD file.
public struct ExternalReferences {
    /// Paths to sublayers.
    public let subLayers: [String]

    /// Paths to references (prim references, value clip references, asset path attributes).
    public let references: [String]

    /// Paths to payloads.
    public let payloads: [String]
}

/// The result of computing all dependencies for a USD asset.
public struct AssetDependencies {
    /// Paths to dependent layers.
    public let layers: [String]

    /// Paths to dependent non-layer assets.
    public let assets: [String]

    /// Paths that could not be resolved.
    public let unresolvedPaths: [String]
}

/// Utilities for analyzing USD asset dependencies.
public enum DependencyUtils {

    /// Extracts external references from a USD file.
    ///
    /// Parses the file at the given path, identifying external references
    /// and sorting them into separate type-based buckets.
    ///
    /// - Parameter filePath: Path to the USD file to analyze.
    /// - Returns: The extracted references grouped by type.
    /// - Throws: `UsdUtilsError.dependencyAnalysisFailed` on error.
    public static func extractExternalReferences(from filePath: String) throws -> ExternalReferences {
        // First get the counts
        var subLayerCount: Int = 0
        var referenceCount: Int = 0
        var payloadCount: Int = 0

        let countResult = UsdUtilsGetExternalReferenceCounts(
            filePath,
            &subLayerCount,
            &referenceCount,
            &payloadCount
        )

        guard countResult == USD_RESULT_SUCCESS else {
            throw UsdUtilsError.dependencyAnalysisFailed("Failed to get reference counts for \(filePath)")
        }

        // Allocate buffers
        var subLayers = [UnsafeMutablePointer<CChar>?](repeating: nil, count: subLayerCount)
        var references = [UnsafeMutablePointer<CChar>?](repeating: nil, count: referenceCount)
        var payloads = [UnsafeMutablePointer<CChar>?](repeating: nil, count: payloadCount)

        var actualSubLayerCount: Int = 0
        var actualReferenceCount: Int = 0
        var actualPayloadCount: Int = 0

        let result = subLayers.withUnsafeMutableBufferPointer { subLayerBuffer in
            references.withUnsafeMutableBufferPointer { referenceBuffer in
                payloads.withUnsafeMutableBufferPointer { payloadBuffer in
                    UsdUtilsExtractExternalReferences(
                        filePath,
                        subLayerBuffer.baseAddress,
                        subLayerCount,
                        referenceBuffer.baseAddress,
                        referenceCount,
                        payloadBuffer.baseAddress,
                        payloadCount,
                        &actualSubLayerCount,
                        &actualReferenceCount,
                        &actualPayloadCount
                    )
                }
            }
        }

        guard result == USD_RESULT_SUCCESS else {
            // Clean up any allocated strings
            UsdUtils_FreeStringArray(&subLayers, actualSubLayerCount)
            UsdUtils_FreeStringArray(&references, actualReferenceCount)
            UsdUtils_FreeStringArray(&payloads, actualPayloadCount)
            throw UsdUtilsError.dependencyAnalysisFailed("Failed to extract references from \(filePath)")
        }

        // Convert to Swift strings
        let subLayerStrings = (0..<actualSubLayerCount).compactMap { i -> String? in
            guard let ptr = subLayers[i] else { return nil }
            defer { UsdUtils_FreeString(ptr) }
            return String(cString: ptr)
        }

        let referenceStrings = (0..<actualReferenceCount).compactMap { i -> String? in
            guard let ptr = references[i] else { return nil }
            defer { UsdUtils_FreeString(ptr) }
            return String(cString: ptr)
        }

        let payloadStrings = (0..<actualPayloadCount).compactMap { i -> String? in
            guard let ptr = payloads[i] else { return nil }
            defer { UsdUtils_FreeString(ptr) }
            return String(cString: ptr)
        }

        return ExternalReferences(
            subLayers: subLayerStrings,
            references: referenceStrings,
            payloads: payloadStrings
        )
    }

    /// Recursively computes all dependencies of the given asset.
    ///
    /// - Parameter assetPath: Path to the USD asset to analyze.
    /// - Returns: The computed dependencies.
    /// - Throws: `UsdUtilsError.dependencyAnalysisFailed` if the asset cannot be resolved.
    public static func computeAllDependencies(of assetPath: String) throws -> AssetDependencies {
        // First get the counts
        var layerCount: Int = 0
        var assetCount: Int = 0
        var unresolvedCount: Int = 0

        let countSuccess = UsdUtilsGetAllDependencyCounts(
            assetPath,
            &layerCount,
            &assetCount,
            &unresolvedCount
        )

        guard countSuccess else {
            throw UsdUtilsError.dependencyAnalysisFailed("Failed to get dependency counts for \(assetPath)")
        }

        // Allocate buffers
        var layers = [UnsafeMutablePointer<CChar>?](repeating: nil, count: layerCount)
        var assets = [UnsafeMutablePointer<CChar>?](repeating: nil, count: assetCount)
        var unresolved = [UnsafeMutablePointer<CChar>?](repeating: nil, count: unresolvedCount)

        var actualLayerCount: Int = 0
        var actualAssetCount: Int = 0
        var actualUnresolvedCount: Int = 0

        let success = layers.withUnsafeMutableBufferPointer { layerBuffer in
            assets.withUnsafeMutableBufferPointer { assetBuffer in
                unresolved.withUnsafeMutableBufferPointer { unresolvedBuffer in
                    UsdUtilsComputeAllDependencies(
                        assetPath,
                        layerBuffer.baseAddress,
                        layerCount,
                        assetBuffer.baseAddress,
                        assetCount,
                        unresolvedBuffer.baseAddress,
                        unresolvedCount,
                        &actualLayerCount,
                        &actualAssetCount,
                        &actualUnresolvedCount
                    )
                }
            }
        }

        guard success else {
            // Clean up any allocated strings
            UsdUtils_FreeStringArray(&layers, actualLayerCount)
            UsdUtils_FreeStringArray(&assets, actualAssetCount)
            UsdUtils_FreeStringArray(&unresolved, actualUnresolvedCount)
            throw UsdUtilsError.dependencyAnalysisFailed("Failed to compute dependencies for \(assetPath)")
        }

        // Convert to Swift strings
        let layerStrings = (0..<actualLayerCount).compactMap { i -> String? in
            guard let ptr = layers[i] else { return nil }
            defer { UsdUtils_FreeString(ptr) }
            return String(cString: ptr)
        }

        let assetStrings = (0..<actualAssetCount).compactMap { i -> String? in
            guard let ptr = assets[i] else { return nil }
            defer { UsdUtils_FreeString(ptr) }
            return String(cString: ptr)
        }

        let unresolvedStrings = (0..<actualUnresolvedCount).compactMap { i -> String? in
            guard let ptr = unresolved[i] else { return nil }
            defer { UsdUtils_FreeString(ptr) }
            return String(cString: ptr)
        }

        return AssetDependencies(
            layers: layerStrings,
            assets: assetStrings,
            unresolvedPaths: unresolvedStrings
        )
    }
}

// MARK: - Pipeline Utilities

/// Utilities for USD pipeline conventions.
///
/// These utilities provide access to pipeline configuration values that help
/// establish consistent naming conventions across a USD workflow.
public enum PipelineUtils {

    /// Gets the name of the USD prim under which materials are expected to be authored.
    ///
    /// By default, this is "Looks". Can be configured via plugInfo.json.
    ///
    /// - Parameter forceDefault: If true, ignores any plugInfo.json configuration.
    /// - Returns: The materials scope name.
    public static func materialsScopeName(forceDefault: Bool = false) -> String {
        guard let result = UsdUtilsGetMaterialsScopeName(forceDefault) else {
            return "Looks"
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }

    /// Gets the name of the primary UV set used on meshes and nurbs.
    ///
    /// By default, this is "st".
    ///
    /// - Returns: The primary UV set name.
    public static var primaryUVSetName: String {
        guard let result = UsdUtilsGetPrimaryUVSetName() else {
            return "st"
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }

    /// Gets the name of the reference position used on meshes and nurbs.
    ///
    /// By default, this is "pref".
    ///
    /// - Returns: The pref name.
    public static var prefName: String {
        guard let result = UsdUtilsGetPrefName() else {
            return "pref"
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }

    /// Gets the name of the primary camera.
    ///
    /// By default, this is "main_cam". Can be configured via plugInfo.json.
    ///
    /// - Parameter forceDefault: If true, ignores any plugInfo.json configuration.
    /// - Returns: The primary camera name.
    public static func primaryCameraName(forceDefault: Bool = false) -> String {
        guard let result = UsdUtilsGetPrimaryCameraName(forceDefault) else {
            return "main_cam"
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }

    /// Gets the alpha attribute name for a given color attribute.
    ///
    /// Follows the shading pipeline's convention for naming a companion
    /// alpha/opacity attribute given a color-valued attribute name.
    ///
    /// - Parameter colorAttrName: The name of the color attribute.
    /// - Returns: The alpha attribute name.
    public static func alphaAttributeName(for colorAttrName: String) -> String {
        guard let result = UsdUtilsGetAlphaAttributeNameForColor(colorAttrName) else {
            return "\(colorAttrName)_opacity"
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }

    /// Gets the model name associated with a given root layer.
    ///
    /// In order, it looks for: defaultPrim metadata, a prim matching the filename,
    /// and then the first concrete root prim.
    ///
    /// - Parameter rootLayer: The root layer.
    /// - Returns: The model name, or `nil` if none can be determined.
    public static func modelName(from rootLayer: Layer) -> String? {
        guard let result = UsdUtilsGetModelNameFromRootLayer(rootLayer.handle) else {
            return nil
        }
        defer { UsdUtils_FreeString(result) }
        return String(cString: result)
    }
}

// MARK: - UsdUtilsError

/// Errors that can occur when using USD utilities.
public enum UsdUtilsError: Error, CustomStringConvertible {

    /// Layer stack flattening failed.
    case flattenFailed(String)

    /// Layer stitching failed.
    case stitchFailed(String)

    /// Dependency analysis failed.
    case dependencyAnalysisFailed(String)

    public var description: String {
        switch self {
        case .flattenFailed(let message):
            return "Layer stack flatten failed: \(message)"
        case .stitchFailed(let message):
            return "Layer stitch failed: \(message)"
        case .dependencyAnalysisFailed(let message):
            return "Dependency analysis failed: \(message)"
        }
    }
}
