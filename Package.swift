// swift-tools-version: 6.1

import PackageDescription

// MARK: - USD Source Directories
//
// These directories from OpenUSD/pxr/ are compiled directly by Swift Package Manager.
// No separate build step required - just `swift build`.

let usdSourceDirs: [String] = [
    // Base libraries
    "base/arch",
    "base/tf",
    "base/gf",
    "base/vt",
    "base/work",
    "base/plug",
    "base/trace",
    "base/js",
    "base/ts",

    // USD Core
    "usd/ar",
    "usd/kind",
    "usd/sdf",
    "usd/pcp",
    "usd/usd",

    // USD Schemas
    "usd/usdGeom",
    "usd/usdShade",
    "usd/usdLux",
    "usd/usdSkel",
    "usd/usdVol",
    "usd/usdMedia",
    "usd/usdPhysics",
    "usd/usdProc",
    "usd/usdRender",
    "usd/usdUI",
    "usd/usdUtils",

    // Imaging Foundation
    "imaging/garch",
    "imaging/hio",
    "imaging/hgi",
    "imaging/cameraUtil",
    "imaging/geomUtil",
    "imaging/pxOsd",

    // Hydra Core
    "imaging/hf",
    "imaging/hd",
    "imaging/hdSt",
    "imaging/hdx",

    // Note: USD Imaging modules excluded for now due to complex header dependencies
    // "usdImaging/usdImaging",
    // "usdImaging/usdSkelImaging",
    // "usdImaging/usdVolImaging",
    // "usdImaging/usdLuxImaging",
    // "usdImaging/usdAppUtils",
]

// Platform-specific GPU backend
#if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
    let usdPlatformDirs: [String] = ["imaging/hgiMetal"]
#elseif os(Linux) || os(Windows)
    let usdPlatformDirs: [String] = ["imaging/hgiVulkan"]
#else
    let usdPlatformDirs: [String] = []
#endif

// MARK: - Platform Configuration

let platforms: [SupportedPlatform] = [
    .macOS(.v14),
    .iOS(.v17),
    .tvOS(.v17),
    .visionOS(.v1),
]

// MARK: - PixarUSD Build Settings

// Platform-specific GPU defines
#if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
    let pixarUsdPlatformDefines: [CXXSetting] = [
        .define("PXR_METAL_SUPPORT_ENABLED", to: "1"),
        .define("PXR_VULKAN_SUPPORT_ENABLED", to: "0"),
    ]
#elseif os(Linux) || os(Windows)
    let pixarUsdPlatformDefines: [CXXSetting] = [
        .define("PXR_METAL_SUPPORT_ENABLED", to: "0"),
        .define("PXR_VULKAN_SUPPORT_ENABLED", to: "1"),
    ]
#else
    let pixarUsdPlatformDefines: [CXXSetting] = []
#endif

let pixarUsdCxxSettings: [CXXSetting] =
    [
        // Header search paths
        .headerSearchPath("../.."),
        .headerSearchPath("."),

        // USD configuration
        .define("PXR_USE_NAMESPACES", to: "0"),
        .define("PXR_PYTHON_SUPPORT_ENABLED", to: "0"),
        .define("PXR_PREFER_SAFETY_OVER_SPEED", to: "1"),

        // Suppress warnings (USD source has many)
        .unsafeFlags([
            "-Wno-deprecated",
            "-Wno-deprecated-declarations",
            "-Wno-unused-parameter",
            "-Wno-unused-variable",
            "-Wno-missing-field-initializers",
            "-Wno-sign-compare",
            "-Wno-shorten-64-to-32",
        ]),
    ] + pixarUsdPlatformDefines

// MARK: - USDCxx Wrapper Settings

let usdCxxWrapperSettings: [CXXSetting] = [
    .headerSearchPath("../../OpenUSD"),
    .headerSearchPath("../../OpenUSD/pxr"),
    .define("PXR_USE_NAMESPACES", to: "0"),
]

// MARK: - Swift Settings

let swiftSettings: [SwiftSetting] = [
    .interoperabilityMode(.Cxx),
    .enableUpcomingFeature("StrictConcurrency"),
    // Note: USE_PIXAR_USD cannot be enabled via Swift because USD headers
    // have #include statements inside namespace blocks which break Clang modules.
    // The system header prelude in Swift.h helps with some issues, but USD headers
    // like pxr/base/arch/fileSystem.h include <limits.h> inside PXR_NAMESPACE_OPEN_SCOPE
    // which causes "redundant #include of module within namespace" errors.
    // Real USD rendering requires a C++ rendering backend with a simpler Swift API.
]

// MARK: - Platform Linker Settings

#if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
    let pixarUsdLinkerSettings: [LinkerSetting] = [
        .linkedFramework("Foundation"),
        .linkedFramework("CoreFoundation"),
        .linkedFramework("Metal"),
        .linkedFramework("MetalKit"),
        .linkedFramework("CoreGraphics"),
        .linkedLibrary("z"),
        .linkedLibrary("c++"),
    ]
#elseif os(Linux)
    let pixarUsdLinkerSettings: [LinkerSetting] = [
        .linkedLibrary("pthread"),
        .linkedLibrary("dl"),
        .linkedLibrary("z"),
        .linkedLibrary("stdc++"),
        .linkedLibrary("vulkan"),
    ]
#elseif os(Windows)
    let pixarUsdLinkerSettings: [LinkerSetting] = [
        .linkedLibrary("z"),
        .linkedLibrary("vulkan-1"),
    ]
#else
    let pixarUsdLinkerSettings: [LinkerSetting] = []
#endif

// MARK: - Package Definition

let package = Package(
    name: "SwiftUSD",
    platforms: platforms,
    products: [
        .library(name: "SwiftUSD", targets: ["SwiftUSD"]),
        .library(name: "USDCxx", targets: ["USDCxx"]),
        .library(name: "PixarUSD", targets: ["PixarUSD"]),
    ],
    targets: [
        // =====================================================================
        // MARK: - PixarUSD (C++ USD built from source) - DISABLED
        // =====================================================================
        // Note: PixarUSD target is currently disabled because SPM cannot
        // properly handle the OpenUSD source tree (non-source files like .md,
        // .png, .usda get picked up and cause build failures). Enable this
        // target once proper source filtering is in place.
        //
        // .target(
        //     name: "PixarUSD",
        //     path: "OpenUSD/pxr",
        //     sources: usdSourceDirs + usdPlatformDirs,
        //     publicHeadersPath: ".",
        //     cxxSettings: pixarUsdCxxSettings,
        //     linkerSettings: pixarUsdLinkerSettings
        // ),

        // =====================================================================
        // MARK: - USDCxx (C++ wrappers with SWIFT_* annotations)
        // =====================================================================
        // Thin C++ wrapper layer using SWIFT_SHARED_REFERENCE and
        // SWIFT_SELF_CONTAINED annotations for direct Swift/C++ interop.
        // Namespace: swiftusd::
        //
        // USE_PIXAR_USD mode: To enable real USD, uncomment the settings below.
        // Note: Some ToGf() methods need updates for real USD type compatibility.
        //
        // OpenUSD libraries are available at: Vendor/USD/darwin/
        .target(
            name: "USDCxx",
            path: "Sources/USDCxx",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: [
                .unsafeFlags(["-std=c++17"]),
                .define("USE_PIXAR_USD", to: "1"),
                .headerSearchPath("../../Vendor/USD/darwin/include"),
            ],
            linkerSettings: [
                .unsafeFlags(["-L/Users/jonathanpeterson/dev/SwiftUSD/Vendor/USD/darwin/lib"]),
                .unsafeFlags(["-Xlinker", "-rpath", "-Xlinker", "/Users/jonathanpeterson/dev/SwiftUSD/Vendor/USD/darwin/lib"]),
                .linkedLibrary("usd_tf"),
                .linkedLibrary("usd_gf"),
                .linkedLibrary("usd_vt"),
                .linkedLibrary("usd_sdf"),
                .linkedLibrary("usd_usd"),
                .linkedLibrary("usd_usdGeom"),
                .linkedLibrary("usd_usdShade"),
                .linkedLibrary("usd_hd"),
                .linkedLibrary("usd_hdSt"),
                .linkedLibrary("usd_usdImaging"),
                .linkedLibrary("tbb"),
            ]
        ),

        // =====================================================================
        // MARK: - PixarUSD (Clean C++ Facade for Swift)
        // =====================================================================
        // Clean C++ wrapper that hides USD headers from Swift.
        // Public headers (include/) have NO USD includes - Swift-visible.
        // Implementation files (src/) include real USD - NOT parsed by Swift.
        // Uses pimpl pattern with SWIFT_SHARED_REFERENCE for memory management.
        // Namespace: pixarusd::
        .target(
            name: "PixarUSD",
            path: "Sources/PixarUSD",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: [
                .unsafeFlags(["-std=c++17"]),
                .headerSearchPath("../../Vendor/USD/darwin/include"),
                // Suppress warnings from USD headers in implementation files
                .unsafeFlags([
                    "-Wno-deprecated",
                    "-Wno-deprecated-declarations",
                    "-Wno-unused-parameter",
                    "-Wno-unused-variable",
                ]),
            ],
            linkerSettings: [
                .unsafeFlags(["-L/Users/jonathanpeterson/dev/SwiftUSD/Vendor/USD/darwin/lib"]),
                .unsafeFlags(["-Xlinker", "-rpath", "-Xlinker", "/Users/jonathanpeterson/dev/SwiftUSD/Vendor/USD/darwin/lib"]),
                .linkedLibrary("usd_arch"),
                .linkedLibrary("usd_work"),
                .linkedLibrary("usd_tf"),
                .linkedLibrary("usd_gf"),
                .linkedLibrary("usd_vt"),
                .linkedLibrary("usd_sdf"),
                .linkedLibrary("usd_usd"),
                .linkedLibrary("usd_usdGeom"),
                .linkedLibrary("usd_usdShade"),
                .linkedLibrary("usd_hd"),
                .linkedLibrary("usd_hdSt"),
                .linkedLibrary("usd_hdx"),
                .linkedLibrary("usd_hgi"),
                .linkedLibrary("usd_hgiMetal"),
                .linkedLibrary("usd_glf"),
                .linkedLibrary("usd_usdImaging"),
                .linkedLibrary("usd_usdImagingGL"),
                .linkedLibrary("tbb"),
                .linkedFramework("Metal"),
                .linkedFramework("OpenGL"),
            ]
        ),

        // =====================================================================
        // MARK: - SwiftUSD (Swift API)
        // =====================================================================
        // Idiomatic Swift interface that imports USDCxx.
        // Uses Swift/C++ interop to call C++ types directly.
        // Note: Excludes files that still use the old OpenUSDInterop module
        .target(
            name: "SwiftUSD",
            dependencies: ["USDCxx"],
            path: "Sources/SwiftUSD",
            exclude: [
                // These files use OpenUSDInterop which is not available
                "Ar/Asset.swift",
                "Ar/WritableAsset.swift",
                "Ar/ResolverScopedCache.swift",
                "Ar/ResolverContextBinder.swift",
                "Ar/Resolver.swift",
                "Ar/DefaultResolverContext.swift",
                "Ar/Timestamp.swift",
                "Ar/ResolverContext.swift",
                "Ar/ResolvedPath.swift",
                "Ar/Ar.swift",
                "CameraUtil/CameraUtil.swift",
                "Garch/Garch.swift",
                "GeomUtil/GeomUtil.swift",
                "Gf/BBox3.swift",
                "Gf/Gf.swift",
                "Gf/Matrix4.swift",
                "Gf/Quaternion.swift",
                "Gf/Range3.swift",
                "Gf/Vec3.swift",
                "Hgi/Hgi.swift",
                "HgiMetal/HgiMetal.swift",
                "Hio/Hio.swift",
                "Kind/Kind.swift",
                "PxOsd/PxOsd.swift",
                "Sdf/AssetPath.swift",
                "Sdf/Layer.swift",
                "Sdf/Path.swift",
                "Sdf/Sdf.swift",
                "Tf/Tf.swift",
                "Tf/Token.swift",
                "Usd/Attribute.swift",
                "Usd/Prim.swift",
                "Usd/Property.swift",
                "Usd/Relationship.swift",
                "Usd/Specifier.swift",
                "Usd/Stage.swift",
                "Usd/TimeCode.swift",
                "Usd/Usd.swift",
                "UsdGeom/UsdGeom.swift",
                "UsdLux/UsdLux.swift",
                "UsdProc/UsdProc.swift",
                "UsdShade/ConnectableAPI.swift",
                "UsdShade/Input.swift",
                "UsdShade/Material.swift",
                "UsdShade/MaterialBindingAPI.swift",
                "UsdShade/NodeGraph.swift",
                "UsdShade/Output.swift",
                "UsdShade/Shader.swift",
                "UsdShade/UsdShade.swift",
                "UsdSkel/UsdSkel.swift",
                "UsdUtils/UsdUtils.swift",
                "UsdVol/UsdVol.swift",
                "Vt/ArrayDouble.swift",
                "Vt/ArrayFloat.swift",
                "Vt/ArrayInt.swift",
                "Vt/ArrayVec3.swift",
                "Vt/Dictionary.swift",
                "Vt/Value.swift",
                "Vt/Vt.swift",
            ],
            swiftSettings: swiftSettings
        ),

        // =====================================================================
        // MARK: - Tests
        // =====================================================================
        .testTarget(
            name: "SwiftUSDTests",
            dependencies: ["SwiftUSD"],
            path: "Tests/SwiftUSDTests",
            swiftSettings: swiftSettings
        ),
    ],
    cxxLanguageStandard: .cxx17
)
