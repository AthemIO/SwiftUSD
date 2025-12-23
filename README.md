# SwiftOpenUSD

A high-performance Swift interface to Pixar's [OpenUSD](https://openusd.org), designed for cross-platform 3D content workflows.

```swift
import OpenUSD

let stage = try Usd.Stage.open(at: "scene.usda")

for prim in stage.traverse() {
    if prim.isA(.mesh) {
        let mesh = try UsdGeom.Mesh(prim)
        let points = mesh.points()  // Batch read - single C++ call
        print("\(prim.name): \(points.count) vertices")
    }
}
```

## Features

- **Pure Swift API** — No C++ in your Swift code
- **High Performance** — Batch operations minimize Swift↔C++ boundary crossings
- **Cross-Platform** — macOS, Linux, Windows
- **pxr/ Structure** — Mirrors OpenUSD's module organization
- **Type Safe** — Swift enums, optionals, and error handling
- **Memory Safe** — Automatic cleanup via Swift ARC

## Requirements

- Swift 6.1+
- OpenUSD 25.11+ (prebuilt or from source)
- macOS 14+ / Ubuntu 22.04+ / Windows 10+

## Installation

### Swift Package Manager

```swift
dependencies: [
    .package(url: "https://github.com/AthemIO/SwiftOpenUSD.git", from: "1.0.0")
]
```

### Building OpenUSD

SwiftOpenUSD requires OpenUSD libraries. Build them with the included script:

```bash
# macOS
./Scripts/build_usd.py --platform darwin --version 25.11

# Linux
./Scripts/build_usd.py --platform linux --version 25.11

# Windows
python Scripts\build_usd.py --platform windows --version 25.11
```

This installs USD to `./OpenUSD/`.

---

## Architecture

SwiftOpenUSD uses a three-layer architecture that provides performance and safety:

```
┌─────────────────────────────────────────────────────────────────────┐
│                         Your Application                             │
│                      import SwiftOpenUSD                                  │
└───────────────────────────────┬─────────────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────────────┐
│                    Sources/SwiftUSD (Swift)                           │
│                                                                      │
│   Pure Swift layer providing idiomatic APIs.                        │
│   Mirrors pxr/ structure: Arch/, Tf/, Gf/, Usd/, UsdGeom/...        │
└───────────────────────────────┬─────────────────────────────────────┘
                                │ C function calls
┌───────────────────────────────▼─────────────────────────────────────┐
│                 Sources/OpenUSDInterop (C/C++)                       │
│                                                                      │
│   Thin C wrapper with extern "C" functions.                         │
│   Opaque handles, batch APIs, exception handling.                   │
└───────────────────────────────┬─────────────────────────────────────┘
                                │ C++ calls
┌───────────────────────────────▼─────────────────────────────────────┐
│                     ./OpenUSD (Pixar)                           │
│                                                                      │
│   Prebuilt OpenUSD library (libusd_ms).                             │
└─────────────────────────────────────────────────────────────────────┘
```

### Why Not Direct C++ Interop?

| Direct C++ Interop | Our Approach |
|--------------------|--------------|
| Swift parses ~500K lines of USD headers | Swift parses ~2K lines of C headers |
| 3-5 minute clean builds | 10-20 second clean builds |
| Potential deep copies on iteration | Controlled batch copies |
| C++ exceptions crash Swift | Error codes → Swift errors |

---

## Project Structure

Mirrors Pixar's `pxr/` organization:

```
Sources/
├── OpenUSDInterop/                 # C/C++ Layer
│   ├── include/
│   │   ├── base/
│   │   │   ├── arch.h              # → pxr/base/arch
│   │   │   ├── tf.h                # → pxr/base/tf
│   │   │   ├── gf.h                # → pxr/base/gf
│   │   │   └── vt.h                # → pxr/base/vt
│   │   ├── usd/
│   │   │   ├── sdf.h               # → pxr/usd/sdf
│   │   │   ├── usd.h               # → pxr/usd/usd
│   │   │   └── stage.h
│   │   ├── usdGeom/
│   │   │   ├── mesh.h              # → pxr/usdGeom
│   │   │   └── xform.h
│   │   └── usdShade/
│   │       └── material.h          # → pxr/usdShade
│   └── src/
│       ├── base/
│       ├── usd/
│       ├── usdGeom/
│       └── usdShade/
│
└── SwiftUSD/                        # Swift Layer
    ├── Arch/                       # Platform utilities
    │   └── Arch.swift
    ├── Tf/                         # Tokens, types
    │   └── Token.swift
    ├── Gf/                         # Math types
    │   ├── Vec3.swift
    │   ├── Matrix4.swift
    │   └── Quaternion.swift
    ├── Vt/                         # Value types
    │   └── Array.swift
    ├── Sdf/                        # Scene description
    │   ├── Path.swift
    │   └── Layer.swift
    ├── Usd/                        # Core USD
    │   ├── Stage.swift
    │   ├── Prim.swift
    │   └── Attribute.swift
    ├── UsdGeom/                    # Geometry schemas
    │   ├── Mesh.swift
    │   ├── Xform.swift
    │   ├── Camera.swift
    │   └── Points.swift
    ├── UsdShade/                   # Shading schemas
    │   ├── Material.swift
    │   └── Shader.swift
    └── UsdLux/                     # Lighting schemas
        └── Light.swift
```

---

## Usage

### Opening a Stage

```swift
import OpenUSD

// Open from file
let stage = try Usd.Stage.open(at: "scene.usda")

// Create new
let stage = try Usd.Stage.create(at: "new_scene.usda")

// In-memory
let stage = try Usd.Stage.createInMemory()
```

### Working with Prims

```swift
// Define prims
let world = try stage.definePrim("/World", type: .xform)
let cube = try stage.definePrim("/World/Cube", type: .mesh)

// Navigate hierarchy
let parent = cube.parent
let children = world.children

// Check type
if cube.isA(.mesh) {
    print("It's a mesh!")
}

// Subscript access
if let prim = stage["/World/Cube"] {
    print(prim.typeName)
}
```

### Geometry Operations

```swift
// Create a mesh
let mesh = try UsdGeom.Mesh.define(at: "/World/Mesh", in: stage)

// Set geometry (batch operation)
try mesh.setGeometry(
    points: [
        SIMD3(-1, -1, 0), SIMD3(1, -1, 0),
        SIMD3(1, 1, 0), SIMD3(-1, 1, 0)
    ],
    faceVertexCounts: [4],
    faceVertexIndices: [0, 1, 2, 3]
)

// Read geometry (batch operation)
let points = mesh.points()      // All points in one call
let normals = mesh.normals()
let uvs = mesh.uvs()
```

### Animation

```swift
stage.startTimeCode = 1.0
stage.endTimeCode = 100.0
stage.framesPerSecond = 24.0

// Set values at specific times
for frame in 1...100 {
    let time = Usd.TimeCode.frame(Double(frame))
    let position = SIMD3<Float>(Float(frame) * 0.1, 0, 0)
    try xform.setTranslation(position, at: time)
}
```

### Traversal

```swift
// Efficient traversal (iteration in C++)
stage.traverse { prim, path, depth in
    let indent = String(repeating: "  ", count: depth)
    print("\(indent)\(prim.typeName): \(prim.name)")
    return true  // Continue traversal
}

// Batch prim collection
let allMeshes = stage.prims(ofType: .mesh)
let allLights = stage.prims(ofType: .distantLight)
```

### Materials

```swift
let material = try UsdShade.Material.define(at: "/Materials/Wood", in: stage)
let shader = try material.createShader(at: "PBRShader")

try shader.setInput("diffuseColor", value: SIMD3<Float>(0.5, 0.3, 0.1))
try shader.setInput("roughness", value: Float(0.8))

try mesh.bind(material: material)
```

### System Utilities

```swift
// System info (from Arch)
print("Cores: \(Arch.physicalCoreCount)")
print("RAM: \(Arch.totalRAM / 1_000_000_000) GB")
print("OS: \(Arch.osName)")

// High-resolution timing
let timer = try Arch.Timer()
timer.start()
// ... expensive operation ...
print("Elapsed: \(timer.elapsedSeconds)s")

// Or use measure
let (result, time) = Arch.Timer.measure {
    expensiveOperation()
}

// File utilities
let normalized = Arch.FileSystem.normalizePath("/foo/bar/../baz")
let expanded = Arch.FileSystem.expandTilde("~/Documents")

// Environment variables
if let path = Arch.Environment["PATH"] {
    print("PATH: \(path)")
}
```

---

## Performance

### Batch Operations

The key to performance is minimizing Swift↔C++ boundary crossings:

```swift
// ❌ Slow: N boundary crossings
for i in 0..<mesh.pointCount {
    let point = mesh.point(at: i)  // Crosses boundary each time!
}

// ✅ Fast: 1 boundary crossing
let points = mesh.points()  // Single batch call
for point in points {
    // Process in Swift
}
```

## Implementation Process

When implementing a new USD module, follow this process:

### 1. Study pxr/ Original

```bash
# Look at the USD source
ls pxr/base/arch/
# Identify APIs needed for Swift
```

### 2. Implement C Header

```c
// Sources/OpenUSDInterop/include/base/arch.h

#ifdef __cplusplus
extern "C" {
#endif

USD_INTEROP_API int ArchGetNumPhysicalCores(void);
USD_INTEROP_API uint64_t ArchGetTotalRAM(void);

#ifdef __cplusplus
}
#endif
```

### 3. Implement C++ Source

```cpp
// Sources/OpenUSDInterop/src/base/arch.cpp

#include <pxr/base/arch/systemInfo.h>

extern "C" int ArchGetNumPhysicalCores(void) {
    return ArchGetPhysicalConcurrency();
}
```

### 4. Implement Swift Wrapper

```swift
// Sources/OpenUSD/Arch/Arch.swift

import OpenUSDInterop

public enum Arch {
    public static var physicalCoreCount: Int {
        Int(ArchGetNumPhysicalCores())
    }
}
```

### 5. Write Tests

```swift
// Tests/OpenUSDTests/ArchTests.swift

func testPhysicalCoreCount() {
    XCTAssertGreaterThan(Arch.physicalCoreCount, 0)
}
```

See [IMPLEMENTATION_GUIDE.md](IMPLEMENTATION_GUIDE.md) for detailed guidelines.

## Comparison with Other Approaches

### vs. [SwiftUSD](https://github.com/AthemIO/SwiftUSD) (Direct C++ Interop)

| Aspect | SwiftUSD | SwiftOpenUSD |
|--------|----------|--------------|
| API Style | C++ style | Swift idiomatic |
| Build Time | 3-5 min | 10-20 sec |
| Iteration Safety | Risk of deep copies | Controlled batches |
| Exception Safety | C++ exceptions possible | All caught |
| Maintenance | Tracks USD API 1:1 | Curated Swift API |

## Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Quick Start

```bash
# Clone
git clone https://github.com/anthropics/SwiftOpenUSD.git
cd SwiftOpenUSD

# Build USD (first time only)
./Scripts/build_usd.py --platform darwin --version 24.08

# Build and test
swift build
swift test
```

### Adding a New Module

1. Create C header in `Sources/OpenUSDInterop/include/<module>/`
2. Create C++ implementation in `Sources/OpenUSDInterop/src/<module>/`
3. Create Swift wrapper in `Sources/OpenUSD/<Module>/`
4. Add tests in `Tests/OpenUSDTests/`
5. Update module.modulemap
6. Submit PR

---

## Resources

- [OpenUSD Documentation](https://openusd.org/release/index.html)
- [OpenUSD API Reference](https://openusd.org/release/api/index.html)
- [Implementation Guide](IMPLEMENTATION_GUIDE.md)
- [Updating USD Version](UPDATING_USD.md)

---

## License

SwiftOpenUSD is available under the Apache 2.0 license. See [LICENSE](LICENSE) for details.

OpenUSD is copyright Pixar and available under the [Apache 2.0 license](https://github.com/PixarAnimationStudios/OpenUSD/blob/release/LICENSE.txt).
