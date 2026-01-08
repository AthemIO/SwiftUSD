// swift-tools-version: 6.1

import PackageDescription

// MARK: - Platform Configuration

let platforms: [SupportedPlatform] = [
    .macOS(.v14),
    .iOS(.v17),
    .tvOS(.v17),
    .visionOS(.v1),
]

// MARK: - Swift Settings

let swiftSettings: [SwiftSetting] = [
    .interoperabilityMode(.Cxx),
    .enableUpcomingFeature("StrictConcurrency"),
]

// MARK: - Package Definition

let package = Package(
    name: "SwiftUSD",
    platforms: platforms,
    products: [
        .library(name: "SwiftUSD", targets: ["SwiftUSD"]),
        .library(name: "PixarUSD", targets: ["SwiftUSD"]),
        .library(name: "CxxFacade", targets: ["CxxFacade"]),
    ],
    targets: [
        // =====================================================================
        // MARK: - CxxFacade (Pimpl-based C++ wrapper)
        // =====================================================================
        // Clean C++ facade using pimpl pattern to hide USD headers from Swift.
        // Headers in include/ have NO pxr includes - Swift can parse these.
        // Implementations in src/ include real pxr headers - Swift never sees them.
        // This solves the Clang modules incompatibility with Pixar USD headers.
        // Namespace: cxxfacade::
        .target(
            name: "CxxFacade",
            path: "Sources/CxxFacade",
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
                .linkedLibrary("usd_plug"),
                .linkedLibrary("usd_ar"),
                .linkedLibrary("usd_work"),
                .linkedLibrary("usd_tf"),
                .linkedLibrary("usd_gf"),
                .linkedLibrary("usd_vt"),
                .linkedLibrary("usd_sdf"),
                .linkedLibrary("usd_kind"),
                .linkedLibrary("usd_usd"),
                .linkedLibrary("usd_usdGeom"),
                .linkedLibrary("usd_usdShade"),
                .linkedLibrary("usd_usdLux"),
                .linkedLibrary("usd_usdSkel"),
                .linkedLibrary("usd_hd"),
                .linkedLibrary("usd_hdSt"),
                .linkedLibrary("usd_hdx"),
                .linkedLibrary("usd_cameraUtil"),
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
        // Idiomatic Swift interface that imports CxxFacade.
        // Uses Swift/C++ interop to call C++ types directly.
        .target(
            name: "SwiftUSD",
            dependencies: ["CxxFacade"],
            path: "Sources/SwiftUSD",
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
