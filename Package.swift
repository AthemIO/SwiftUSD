// swift-tools-version: 6.1

import PackageDescription

// MARK: - USD Build Configuration
//
// Set to true after running ./Scripts/build_usd.sh to link against real OpenUSD.
// When false, the interop layer uses standalone C++ fallback implementations.
//
let usePixarUSD = false

// MARK: - Platform Configuration

let platforms: [SupportedPlatform] = [
    .macOS(.v14),
    .iOS(.v17),
    .tvOS(.v17),
    .visionOS(.v1),
]

// MARK: - USD Build Settings

let usdCxxSettings: [CXXSetting] = [
    // C++17 required
    .unsafeFlags(["-std=c++17"]),

    // USD source paths
    .headerSearchPath("../../OpenUSD"),
    .headerSearchPath("../../OpenUSD/pxr"),

    // USD defines
    .define("PXR_PYTHON_SUPPORT_ENABLED", to: "0"),
    .define("PXR_PREFER_SAFETY_OVER_SPEED", to: "1"),
    .define("PXR_BUILD_MONOLITHIC", to: "1"),

    // Platform defines
    .define(
        "PXR_METAL_SUPPORT_ENABLED", to: "1", .when(platforms: [.macOS, .iOS, .tvOS, .visionOS])),

    // Suppress warnings (USD has many)
    .unsafeFlags(["-Wno-deprecated"]),
    .unsafeFlags(["-Wno-deprecated-declarations"]),
    .unsafeFlags(["-Wno-unused-parameter"]),
    .unsafeFlags(["-Wno-unused-variable"]),
    .unsafeFlags(["-Wno-missing-field-initializers"]),
    .unsafeFlags(["-Wno-sign-compare"]),
]

var interopCxxSettings: [CXXSetting] {
    var settings: [CXXSetting] = [
        .unsafeFlags(["-std=c++17"]),
        .headerSearchPath("../../OpenUSD"),
        .headerSearchPath("../../OpenUSD/pxr"),
        .define("PXR_PYTHON_SUPPORT_ENABLED", to: "0"),
    ]

    if usePixarUSD {
        // Include headers from built USD library
        #if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
        settings.append(.headerSearchPath("../../Vendor/USD/darwin/include"))
        #elseif os(Linux)
        settings.append(.headerSearchPath("../../Vendor/USD/linux/include"))
        #elseif os(Windows)
        settings.append(.headerSearchPath("../../Vendor/USD/windows/include"))
        #endif

        // Enable real USD integration
        settings.append(.define("USE_PIXAR_USD", to: "1"))
    }

    return settings
}

let swiftSettings: [SwiftSetting] = [
    .enableUpcomingFeature("StrictConcurrency")
]

// MARK: - Platform Linker Settings

var linkerSettings: [LinkerSetting] {
    var settings: [LinkerSetting] = []

    #if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
        settings += [
            .linkedFramework("Foundation"),
            .linkedFramework("CoreFoundation"),
            .linkedLibrary("z"),
            .linkedLibrary("c++"),
        ]

        if usePixarUSD {
            // Link against built USD library
            settings += [
                .linkedFramework("Metal"),
                .linkedFramework("MetalKit"),
                .linkedLibrary("usd_ms"),
                .unsafeFlags(["-L../../Vendor/USD/darwin/lib"]),
                .unsafeFlags(["-Wl,-rpath,@loader_path/../lib"]),
            ]
        }
    #endif

    #if os(Linux)
        settings += [
            .linkedLibrary("pthread"),
            .linkedLibrary("dl"),
            .linkedLibrary("z"),
            .linkedLibrary("stdc++"),
        ]

        if usePixarUSD {
            settings += [
                .linkedLibrary("usd_ms"),
                .unsafeFlags(["-L../../Vendor/USD/linux/lib"]),
                .unsafeFlags(["-Wl,-rpath,$ORIGIN/../lib"]),
            ]
        }
    #endif

    #if os(Windows)
        if usePixarUSD {
            settings += [
                .linkedLibrary("usd_ms"),
            ]
        }
    #endif

    return settings
}

// MARK: - Package Definition

let package = Package(
    name: "SwiftUSD",
    platforms: platforms,
    products: [
        .library(name: "SwiftUSD", targets: ["SwiftUSD"])
    ],
    targets: [
        // =====================================================================
        // MARK: - PixarUSD (C++ USD built from source)
        // =====================================================================
        // NOTE: PixarUSD target is disabled until OpenUSD is properly configured.
        // The OpenUSDInterop layer currently uses standalone implementations.
        // To enable full USD integration:
        // 1. Run cmake to configure OpenUSD and generate pxr/pxr.h
        // 2. Uncomment this target
        // 3. Add "PixarUSD" to OpenUSDInterop dependencies
        /*
        .target(
            name: "PixarUSD",
            path: "OpenUSD/pxr",
            sources: [
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

                // USD core
                "usd/ar",
                "usd/kind",
                "usd/sdf",
                "usd/pcp",
                "usd/usd",

                // USD schemas
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
            ],
            publicHeadersPath: ".",
            cxxSettings: usdCxxSettings,
            linkerSettings: linkerSettings
        ),
        */

        // =====================================================================
        // MARK: - OpenUSDInterop (C wrapper)
        // =====================================================================
        // When usePixarUSD = true, this links against the built libusd_ms library.
        // When usePixarUSD = false, it uses standalone C++ fallback implementations.
        .target(
            name: "OpenUSDInterop",
            dependencies: [],
            path: "Sources/OpenUSDInterop",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: interopCxxSettings,
            linkerSettings: linkerSettings
        ),

        // =====================================================================
        // MARK: - SwiftUSD (Swift API)
        // =====================================================================
        .target(
            name: "SwiftUSD",
            dependencies: ["OpenUSDInterop"],
            path: "Sources/SwiftUSD",
            swiftSettings: swiftSettings
        ),

        // =====================================================================
        // MARK: - Tests
        // =====================================================================
        .testTarget(
            name: "SwiftUSDTests",
            dependencies: ["SwiftUSD"],
            path: "Tests/SwiftUSDTests"
        ),
    ],
    cxxLanguageStandard: .cxx17
)
