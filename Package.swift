import Foundation
// swift-tools-version: 6.1
import PackageDescription

// MARK: - Platform Detection & Configuration

let isDarwin =
    ProcessInfo.processInfo.environment["SDKROOT"]?.contains("MacOSX") ?? false
    || ProcessInfo.processInfo.environment["DEVELOPER_DIR"] != nil

#if os(Windows)
    let platform: Platform = .windows
#elseif os(Linux)
    let platform: Platform = .linux
#else
    let platform: Platform = .darwin
#endif

enum Platform {
    case darwin, linux, windows

    var usdLibPath: String {
        switch self {
        case .darwin: return "Vendor/OpenUSD/lib"
        case .linux: return "Vendor/OpenUSD/lib"
        case .windows: return "Vendor/OpenUSD/lib"
        }
    }

    var usdIncludePath: String {
        return "Vendor/OpenUSD/include"
    }
}

// MARK: - Build Settings

let commonCxxSettings: [CXXSetting] = [
    .define("PXR_PYTHON_ENABLED", to: "0"),  // Disable Python for minimal builds
    .define("BOOST_ALL_NO_LIB"),  // Prevent auto-linking on Windows
    .headerSearchPath("include"),
    .headerSearchPath("../../../Vendor/OpenUSD/include"),
    .unsafeFlags(["-std=c++17"]),
]

let darwinCxxSettings: [CXXSetting] =
    commonCxxSettings + [
        .define("PXR_BUILD_LOCATION", to: "\"pxr\""),
        .define("PXR_PLUGINPATH_NAME", to: "\"PXR_PLUGINPATH_NAME\""),
    ]

let linuxCxxSettings: [CXXSetting] =
    commonCxxSettings + [
        .define("PXR_BUILD_LOCATION", to: "\"pxr\""),
        .define("LINUX"),
    ]

let windowsCxxSettings: [CXXSetting] =
    commonCxxSettings + [
        .define("NOMINMAX"),
        .define("WIN32_LEAN_AND_MEAN"),
        .define("_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH"),
        .define("_ALLOW_KEYWORD_MACROS", to: "1"),
    ]

let interopCxxSettings: [CXXSetting] = {
    #if os(Windows)
        return windowsCxxSettings
    #elseif os(Linux)
        return linuxCxxSettings
    #else
        return darwinCxxSettings
    #endif
}()

// MARK: - Linker Settings

let darwinLinkerSettings: [LinkerSetting] = [
    .linkedLibrary("usd_ms"),  // Monolithic USD library
    .unsafeFlags(["-L", "Vendor/OpenUSD/lib"]),
    .unsafeFlags(["-Wl,-rpath,@loader_path/../lib"]),
]

let linuxLinkerSettings: [LinkerSetting] = [
    .linkedLibrary("usd_ms"),
    .linkedLibrary("pthread"),
    .linkedLibrary("dl"),
    .unsafeFlags(["-L", "Vendor/OpenUSD/lib"]),
    .unsafeFlags(["-Wl,-rpath,$ORIGIN/../lib"]),
]

let windowsLinkerSettings: [LinkerSetting] = [
    .linkedLibrary("usd_ms"),
    .unsafeFlags(["-L", "Vendor/OpenUSD/lib"]),
]

let interopLinkerSettings: [LinkerSetting] = {
    #if os(Windows)
        return windowsLinkerSettings
    #elseif os(Linux)
        return linuxLinkerSettings
    #else
        return darwinLinkerSettings
    #endif
}()

// MARK: - Package Definition

let package = Package(
    name: "SwiftUSD",
    platforms: [
        .macOS(.v13),
        .iOS(.v16),
        .visionOS(.v1),
        .tvOS(.v16),
        .watchOS(.v9),
    ],
    products: [
        // Main library product
        .library(
            name: "SwiftUSD",
            targets: ["SwiftUSD"]
        ),
        // Low-level interop for advanced users
        .library(
            name: "OpenUSDInterop",
            targets: ["OpenUSDInterop"]
        ),
    ],
    dependencies: [
        // Optional: for async utilities
        // .package(url: "https://github.com/apple/swift-async-algorithms", from: "1.0.0"),
    ],
    targets: [
        // MARK: - Main Swift API
        .target(
            name: "SwiftUSD",
            dependencies: ["OpenUSDInterop"],
            path: "Sources/SwiftUSD",
            swiftSettings: [
                .enableExperimentalFeature("StrictConcurrency"),
                .define("DEBUG", .when(configuration: .debug)),
            ]
        ),

        // MARK: - C/C++ Interop Layer
        .target(
            name: "OpenUSDInterop",
            path: "Sources/OpenUSDInterop",
            sources: ["src"],
            publicHeadersPath: "include",
            cxxSettings: interopCxxSettings,
            linkerSettings: interopLinkerSettings
        ),

        // MARK: - Tests
        .testTarget(
            name: "SwiftUSDTests",
            dependencies: ["SwiftUSD"],
            path: "Tests/SwiftUSDTests"
        ),
    ],
    cxxLanguageStandard: .cxx17
)
