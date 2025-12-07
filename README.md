# SwiftUSD

[![OpenUSD](https://img.shields.io/badge/OpenUSD-25.11-blue)](https://openusd.org)
[![Swift](https://img.shields.io/badge/Swift-6.1-orange)](https://swift.org)

[![macOS](https://img.shields.io/github/actions/workflow/status/AthemIO/SwiftUSD/swift-macos.yml?label=macOS&logo=apple)](https://github.com/AthemIO/SwiftUSD/actions/workflows/swift-macos.yml)
[![Linux](https://img.shields.io/github/actions/workflow/status/AthemIO/SwiftUSD/swift-ubuntu.yml?label=Linux&logo=ubuntu)](https://github.com/AthemIO/SwiftUSD/actions/workflows/swift-ubuntu.yml)
[![Windows](https://img.shields.io/github/actions/workflow/status/AthemIO/SwiftUSD/swift-windows.yml?label=Windows&logo=windows)](https://github.com/AthemIO/SwiftUSD/actions/workflows/swift-windows.yml)

**OpenUSD for Swift** — Pixar's Universal Scene Description with native Swift bindings.

Built and maintained by [ATHEM](https://github.com/AthemIO).

---

## Overview

SwiftUSD brings [OpenUSD](https://openusd.org) to Swift through C++ interoperability, enabling you to author, read, and manipulate USD scene description directly in Swift applications.

## Installation

### macOS

Developed and tested with **Xcode 16.4**. Requires Swift C++ interop support.

### Package.swift

Add SwiftUSD to your `Package.swift`:

```swift
// swift-tools-version: 6.0
import PackageDescription

let package = Package(
  name: "MyApp",
  platforms: [
    .macOS(.v14),
    .visionOS(.v1),
    .iOS(.v17),
  ],
  dependencies: [
    .package(url: "https://github.com/AthemIO/SwiftUSD.git", from: "25.11.0"),
  ],
  targets: [
    .executableTarget(
      name: "MyApp",
      dependencies: [
        .product(name: "PixarUSD", package: "SwiftUSD"),
      ],
      cxxSettings: [
        .define("_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH", .when(platforms: [.windows])),
        .define("_ALLOW_KEYWORD_MACROS", to: "1", .when(platforms: [.windows])),
      ],
      swiftSettings: [
        .interoperabilityMode(.Cxx)
      ]
    ),
  ],
  cxxLanguageStandard: .cxx17
)
```

## Quick Start

```swift
import PixarUSD

@main
enum HelloUSD {
  static func main() {
    // Required: Initialize USD resources
    Pixar.Bundler.shared.setup(.resources)

    // Create a stage with geometry
    let stage = Usd.Stage.createNew("HelloWorld.usda")

    UsdGeom.Xform.define(stage, path: "/Root")
    UsdGeom.Sphere.define(stage, path: "/Root/Sphere")

    stage.save()
  }
}
```

## Extending with Plugins

SwiftUSD supports USD's plugin architecture. You can integrate third-party file format plugins like [Adobe's USD-Fileformat-plugins](https://github.com/adobe/USD-Fileformat-plugins) to add support for glTF, OBJ, STL, and other formats.

### Adding a File Format Plugin

1. Add the plugin source to your project:

```bash
git clone https://github.com/adobe/USD-Fileformat-plugins.git External/USD-Fileformat-plugins
```

2. Create targets in your `Package.swift`:

```swift
.target(
  name: "UsdGltf",
  dependencies: [
    .product(name: "PixarUSD", package: "SwiftUSD"),
  ],
  path: "External/USD-Fileformat-plugins/gltf",
  sources: ["src"],
  resources: [
    .copy("resources/plugInfo.json")
  ],
  cxxSettings: [
    .headerSearchPath("src"),
  ]
)
```

3. Register plugins at runtime:

```swift
import PixarUSD

// Initialize USD
Pixar.Bundler.shared.setup(.resources)

// Register additional plugins
var plugPaths = Pixar.PlugRegistry.PlugPathsVector()
plugPaths.push_back(std.string("/path/to/plugin/resources"))
Pixar.PlugRegistry.GetInstance().RegisterPlugins(plugPaths)

// Now open glTF files directly
let stage = Usd.Stage.open("model.glb")
```

## Building

SwiftUSD uses [swift-bundler](https://github.com/stackotter/swift-bundler) for building applications:

```bash
# Run the UsdView application
swift bundler run -c release UsdView

# Run examples
swift bundler run -c release Examples
```

## License

SwiftUSD is licensed under the [Apache License 2.0](https://openusd.org/license).

---

Originally created by [Wabiverse](https://github.com/wabiverse). Now maintained by [AthemIO](https://github.com/AthemIO).
