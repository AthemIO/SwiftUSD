# SwiftUSD

[![OpenUSD](https://img.shields.io/badge/OpenUSD-25.11-blue)](https://openusd.org)
[![Swift](https://img.shields.io/badge/Swift-6.0-orange)](https://swift.org)

[![macOS](https://img.shields.io/badge/macOS-14%2B-black?logo=apple)](https://developer.apple.com/macos/)
[![visionOS](https://img.shields.io/badge/visionOS-1%2B-black?logo=apple)](https://developer.apple.com/visionos/)
[![iOS](https://img.shields.io/badge/iOS-17%2B-black?logo=apple)](https://developer.apple.com/ios/)
[![Linux](https://img.shields.io/badge/Linux-Ubuntu-orange?logo=ubuntu)](https://ubuntu.com)

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

## License

SwiftUSD is licensed under the [Apache License 2.0](https://openusd.org/license).

---

Originally created by [Wabiverse](https://github.com/wabiverse).
