# SwiftUSD

[![OpenUSD](https://img.shields.io/badge/OpenUSD-25.11-blue)](https://openusd.org)

**OpenUSD for Swift** — Pixar's Universal Scene Description with native Swift bindings.

Built and maintained by [ATHEM](https://github.com/AthemIO).

---

## Overview

SwiftUSD brings [OpenUSD](https://openusd.org) to Swift through C++ interoperability, enabling you to author, read, and manipulate USD scene description directly in Swift applications.

## Installation

Add to your `Package.swift`:

```swift
dependencies: [
  .package(url: "https://github.com/AthemIO/SwiftUSD.git", from: "25.11.0"),
]
```

Then add `PixarUSD` as a dependency to your target:

```swift
.target(
  name: "MyApp",
  dependencies: [
    .product(name: "PixarUSD", package: "SwiftUSD"),
  ],
  swiftSettings: [
    .interoperabilityMode(.Cxx)
  ]
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

## Building

SwiftUSD uses [swift-bundler](https://github.com/stackotter/swift-bundler) for building applications:

```bash
# Run the UsdView application
swift bundler run -c release UsdView

# Run examples
swift bundler run -c release Examples
```

## Platforms

- macOS 14+
- visionOS 1+
- iOS 17+
- Linux (Ubuntu)

## License

SwiftUSD is licensed under the [Apache License 2.0](https://openusd.org/license).

---

Originally created by [Wabiverse](https://github.com/wabiverse).
