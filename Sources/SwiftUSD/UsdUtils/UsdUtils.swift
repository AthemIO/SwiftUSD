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

// MARK: - UsdUtilsError

/// Errors that can occur when using USD utilities.
public enum UsdUtilsError: Error, CustomStringConvertible {

    /// Layer stack flattening failed.
    case flattenFailed(String)

    /// Layer stitching failed.
    case stitchFailed(String)

    public var description: String {
        switch self {
        case .flattenFailed(let message):
            return "Layer stack flatten failed: \(message)"
        case .stitchFailed(let message):
            return "Layer stitch failed: \(message)"
        }
    }
}
