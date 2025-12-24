// Sdf.swift - Sdf module exports
// Mirrors: pxr/usd/sdf/

// Re-export all Sdf types from this module file

// Path - A path value used to locate objects in layers or scenegraphs
// AssetPath - Contains an asset path and optional evaluated and resolved paths
// Layer - A scene description container

// Note: Individual types are defined in their own files:
// - Path.swift (SdfPath)
// - AssetPath.swift (SdfAssetPath)
// - Layer.swift (SdfLayer)

// MARK: - Type Aliases

/// Type alias for SdfPath
public typealias SdfPath = Path

/// Type alias for SdfAssetPath
public typealias SdfAssetPath = AssetPath

/// Type alias for SdfLayer
public typealias SdfLayer = Layer
