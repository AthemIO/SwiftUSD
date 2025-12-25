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

// MARK: - USDCxx Build Settings

var usdCxxInteropSettings: [CXXSetting] {
    var settings: [CXXSetting] = [
        .unsafeFlags(["-std=c++17"]),
    ]

    if usePixarUSD {
        // Include headers from built USD library
        #if os(macOS) || os(iOS) || os(tvOS) || os(visionOS)
        settings.append(.headerSearchPath("../../Vendor/USD/darwin/include"))
        settings.append(.headerSearchPath("../../OpenUSD"))
        #elseif os(Linux)
        settings.append(.headerSearchPath("../../Vendor/USD/linux/include"))
        settings.append(.headerSearchPath("../../OpenUSD"))
        #elseif os(Windows)
        settings.append(.headerSearchPath("../../Vendor/USD/windows/include"))
        settings.append(.headerSearchPath("../../OpenUSD"))
        #endif

        // Enable real USD integration
        settings.append(.define("USE_PIXAR_USD", to: "1"))
    }

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
        // MARK: - USDCxx (C++ interop with Swift annotations)
        // =====================================================================
        // Modern C++ interop layer using SWIFT_* annotations for direct Swift/C++ interop.
        // When usePixarUSD = true, wraps real OpenUSD.
        // When usePixarUSD = false, uses standalone implementations.
        .target(
            name: "USDCxx",
            dependencies: [],
            path: "Sources/USDCxx",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: usdCxxInteropSettings,
            linkerSettings: linkerSettings
        ),

        // =====================================================================
        // MARK: - SwiftUSD (Swift API)
        // =====================================================================
        // Currently only includes Arch module during migration to USDCxx.
        // Other modules will be added as they are migrated from OpenUSDInterop.
        .target(
            name: "SwiftUSD",
            dependencies: ["USDCxx"],
            path: "Sources/SwiftUSD",
            sources: ["Arch"],
            swiftSettings: swiftSettings + [
                .interoperabilityMode(.Cxx)
            ]
        ),

        // =====================================================================
        // MARK: - Tests
        // =====================================================================
        .testTarget(
            name: "SwiftUSDTests",
            dependencies: ["SwiftUSD"],
            path: "Tests/SwiftUSDTests",
            sources: ["ArchTests.swift"],
            swiftSettings: [
                .interoperabilityMode(.Cxx)
            ]
        ),
    ],
    cxxLanguageStandard: .cxx17
)
