# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SwiftUSD is a Swift Package Manager wrapper around Pixar's Universal Scene Description (USD) - an efficient, scalable system for authoring, reading, and streaming time-sampled scene description for graphics applications interchange. The project enables Swift/C++ interoperability with USD.

**Current OpenUSD Version**: 24.08 (see `Sources/pxr/include/pxr/pxrns.h`)

## Build Commands

### macOS (Primary Development)
```bash
# Build and run UsdView application (requires swift-bundler)
swift bundler run -c release UsdView

# Run the Examples target
swift bundler run -c release Examples

# Run tests
swift bundler test
```

**IMPORTANT**: You CANNOT use `swift build` to build the project. Use `swift bundler` instead. The UsdView application requires bundled resources that only swift-bundler can properly package.

### Platform Support

| Platform | Version | Status |
|----------|---------|--------|
| macOS | 14+ | Full support (primary development) |
| visionOS | 1.0+ | Full support |
| iOS | 17+ | Full support |
| tvOS | 17+ | Full support |
| watchOS | 10+ | Full support |
| Linux | - | Partial (needs testing) |
| Windows | - | Partial (needs testing) |

---

## Architecture

### Complete Module List

The codebase contains **73 modules** in `Sources/`, organized as follows:

**Base Libraries** (foundation types and utilities):
| Module | Description |
|--------|-------------|
| `pxr` | Root namespace header |
| `Arch` | Architecture-dependent code, platform abstraction |
| `Tf` | Type foundations, debugging, error handling, tokens |
| `Gf` | Graphics foundations (vectors, matrices, math types) |
| `Vt` | Value types and typed arrays |
| `Work` | Multi-threading (TBB-based work queues) |
| `Plug` | Plugin framework and registry |
| `Trace` | Performance tracing |
| `Js` | JSON support |
| `Ts` | Time sampling/splines (source exists, not in Package.swift) |
| `Pegtl` | Parsing Expression Grammar Template Library |

**USD Core** (scene description and composition):
| Module | Description |
|--------|-------------|
| `Ar` | Asset resolution |
| `Sdf` | Scene description foundations (layers, specs, paths) |
| `Pcp` | Prim cache population (composition engine) |
| `Usd` | Core USD API (stages, prims, attributes) |
| `Kind` | Model hierarchy classification |
| `Ndr` | Node definition registry |
| `Sdr` | Shader definition registry |
| `SdrOsl` | OSL shader definitions |

**USD Schema Libraries** (domain-specific APIs):
| Module | Description |
|--------|-------------|
| `UsdGeom` | Geometry schemas (meshes, curves, points, xforms) |
| `UsdShade` | Shading/material schemas |
| `UsdLux` | Lighting schemas |
| `UsdSkel` | Skeletal animation schemas |
| `UsdPhysics` | Physics simulation schemas |
| `UsdVol` | Volumetric data schemas |
| `UsdMedia` | Media asset schemas |
| `UsdUI` | UI hint schemas |
| `UsdRender` | Render settings schemas |
| `UsdRi` | RenderMan-specific schemas |
| `UsdProc` | Procedural schemas |
| `UsdHydra` | Hydra-specific schemas |
| `UsdUtils` | USD utilities |

**USD Format Plugins** (file format support):
| Module | Description |
|--------|-------------|
| `UsdAbc` | Alembic file format support |
| `UsdDraco` | Draco compression support |
| `UsdMtlx` | MaterialX integration |

**Imaging/Hydra** (rendering framework):
| Module | Description |
|--------|-------------|
| `Garch` | Graphics architecture utilities |
| `Hf` | Hydra foundations |
| `Hd` | Hydra core (render delegate interface) |
| `HdAr` | Hydra asset resolution |
| `HdGp` | Hydra geometry processing |
| `HdMtlx` | Hydra MaterialX integration |
| `HdSi` | Hydra scene index |
| `HdSt` | Storm renderer (OpenGL/Metal) |
| `HdStorm` | Storm renderer plugin |
| `Hdx` | Hydra extensions (selection, picking) |
| `Hgi` | Hardware Graphics Interface (abstraction layer) |
| `HgiGL` | Hgi OpenGL backend |
| `HgiMetal` | Hgi Metal backend (Apple platforms) |
| `HgiVulkan` | Hgi Vulkan backend (disabled - source exists) |
| `HgiInterop` | Hgi interoperability utilities |
| `Hio` | Hydra I/O (image/texture loading) |
| `HioOpenVDB` | OpenVDB texture support |
| `Glf` | GL foundations |
| `PxOsd` | OpenSubdiv integration |
| `CameraUtil` | Camera utilities |
| `GeomUtil` | Geometry utilities |

**USD Imaging** (USD to Hydra bridge):
| Module | Description |
|--------|-------------|
| `UsdImaging` | Core USD imaging adapters |
| `UsdImagingGL` | OpenGL-specific USD imaging |
| `UsdShaders` | Built-in USD shaders |
| `UsdAppUtils` | Application utilities |
| `UsdProcImaging` | Procedural imaging |
| `UsdRiPxrImaging` | RenderMan imaging |
| `UsdSkelImaging` | Skeletal imaging |
| `UsdVolImaging` | Volume imaging |
| `UsdViewQ` | USD viewer Qt utilities |

**Swift Layer**:
| Module | Description |
|--------|-------------|
| `PixarUSD` | Monolithic Swift wrapper library |
| `PixarMacros` | Swift macros for USD interop |

**Applications and Tools**:
| Module | Description |
|--------|-------------|
| `UsdView` | USD scene viewer application |
| `Examples` | Example Swift code |
| `OpenUSD` | Upgrade tool (disabled) |
| `Bin` | Command-line tools |
| `Extras` | Additional utilities |
| `Plugin` | Plugin definitions |
| `ThirdParty` | Third-party dependencies |

---

## MetaverseKit Dependency

SwiftUSD depends on **MetaverseKit** for VFX platform libraries. All MetaverseKit dependencies are imported through the `Arch` target.

**Local Development Path**: `~/dev/Athem/MetaverseKit`

When upgrading OpenUSD, you may need to make changes to MetaverseKit as well (e.g., updating VFX library versions). The MetaverseKit repository is available locally for coordinated changes.

### Dependency Chain
```
SwiftUSD
└── MetaverseKit (LynrAI/MetaverseKit)
    ├── VFX Platform Libraries
    │   ├── MaterialX 1.38.8
    │   ├── OpenSubdiv 3.6.0
    │   ├── OpenImageIO 2.5.4.0
    │   ├── OpenColorIO 2.3.0
    │   ├── OpenEXR 3.2.1
    │   ├── Imath 3.1.9
    │   ├── OpenVDB 10.1.0
    │   ├── Alembic 1.8.5
    │   ├── Ptex 2.4.2
    │   └── Draco 1.5.6
    ├── Threading
    │   ├── OneTBB 2021.10.0
    │   └── MetaTBB (Swift wrapper)
    ├── Math
    │   └── Eigen 3.4.0
    └── Platform-Specific
        ├── Apple (Metal-cpp)
        └── MetaverseVulkanFramework (MoltenVK)
```

### Feature Flags

The following features are enabled via defines in the `Arch` target:

```cpp
PXR_OCIO_PLUGIN_ENABLED=1     // OpenColorIO support
PXR_OIIO_PLUGIN_ENABLED=1     // OpenImageIO support
PXR_PTEX_SUPPORT_ENABLED=1    // Ptex texture support
PXR_OPENVDB_SUPPORT_ENABLED=1 // OpenVDB support
PXR_MATERIALX_SUPPORT_ENABLED=1 // MaterialX support
PXR_HDF5_SUPPORT_ENABLED=1    // HDF5 support
PXR_METAL_SUPPORT_ENABLED=1   // Metal (Apple only)
PXR_OSL_SUPPORT_ENABLED=0     // OSL (disabled)
PXR_VULKAN_SUPPORT_ENABLED=0  // Vulkan (disabled)
PXR_PYTHON_SUPPORT_ENABLED=0  // Python (disabled)
```

---

## Namespace Configuration

The project uses a Swift-friendly namespace configuration in `Sources/pxr/include/pxr/pxrns.h`:

```cpp
#define PXR_NS pxr                    // C++ namespace
#define PXR_INTERNAL_NS Pixar         // Swift-visible namespace

namespace Pixar {}
namespace PXR_NS { using namespace PXR_INTERNAL_NS; }
```

**Why `Pixar` instead of `pxr`?** Swift cannot create typealiases to C++ namespaces. The internal namespace was renamed to `Pixar` for developer convenience, allowing Swift code to use `Pixar.TfToken`, `Pixar.UsdStage`, etc.

### Version Tracking

The project tracks both USD version and package evolution in `Sources/pxr/include/pxr/pxrns.h`:

```cpp
#define PXR_MAJOR_VERSION 0
#define PXR_MINOR_VERSION 24
#define PXR_PATCH_VERSION 8
#define PXR_VERSION 2408
#define SWIFTUSD_EVOLUTION 14  // Increment for SwiftUSD-specific changes
```

The Swift-visible version string is: `"24.08.14"` (see `Sources/PixarUSD/Pixar.swift`)

---

## Swift/C++ Interoperability Patterns

### Package Configuration

All targets using C++ interop require:
```swift
swiftSettings: [
    .interoperabilityMode(.Cxx)
]
```

Common C++ settings for cross-platform compatibility:
```swift
cxxSettings: [
    .define("_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH", .when(platforms: [.windows])),
    .define("_ALLOW_KEYWORD_MACROS", to: "1", .when(platforms: [.windows])),
    .define("static_assert(_conditional, ...)", to: "", .when(platforms: [.windows])),
]
```

### Critical Limitation: std::unique_ptr

**IMPORTANT**: Swift does NOT support `std::unique_ptr` in C++ interop:

```swift
.define("SWIFT_HAS_UNIQUE_PTR", to: "0"),
```

This means:
- C++ APIs returning `std::unique_ptr` cannot be called directly from Swift
- Workarounds may be needed for such APIs
- This affects some USD internal types

### Type Aliasing Pattern

Expose C++ types to Swift with public typealiases:
```swift
import CxxStdlib
import pxr

// Create Swift-friendly type aliases
public typealias UsdStageRefPtr = Pixar.UsdStageRefPtr
public typealias UsdStage = Pixar.UsdStage
public typealias SdfLayer = Pixar.SdfLayer
public typealias SdfLayerHandle = Pixar.SdfLayerHandle
public typealias TfToken = Pixar.TfToken
```

### Dual Extension Pattern (Class + Smart Pointer)

**CRITICAL PATTERN**: SwiftUSD provides extensions for BOTH the C++ class AND its smart pointer wrapper:

```swift
// Extension on the class itself (static methods)
public extension Usd.Stage {
    static func createNew(_ identifier: String) -> UsdStageRefPtr {
        Pixar.UsdStage.CreateNew(std.string(identifier))
    }
}

// Extension on the RefPtr (instance methods via pointee)
public extension Usd.StageRefPtr {
    func save() {
        pointee.Save()
    }

    func getRootLayer() -> SdfLayerHandle {
        pointee.GetRootLayer()
    }
}
```

**When to use which**:
- Use class extensions for static factory methods
- Use RefPtr/Handle extensions for instance methods
- Always delegate to the underlying type via `pointee`

### Pointer Access: pointee vs getPtr()

For C++ smart pointers (RefPtr, Handle):

```swift
// Use pointee for member access
extension UsdStageRefPtr {
    func save() {
        pointee.Save()  // Calls method on underlying UsdStage
    }
}

// Use getPtr() when passing to C++ functions expecting raw pointers
func traverse() -> [Usd.Prim] {
    let it = Usd.PrimRange.Stage(getPtr(), .init())  // getPtr() returns raw pointer
    return IteratorSequence(it).map { $0 }
}
```

### String Conversion Pattern

Always convert Swift strings to C++ std::string:
```swift
let cxxString = std.string(swiftString)  // Swift -> C++
let swiftString = String(cxxStdString)   // C++ -> Swift
```

### Namespace Enum Pattern

Use Swift enums as namespace containers:
```swift
public enum Tf {}      // Tools Foundation
public enum Gf {}      // Graphics Foundation
public enum Vt {}      // Value Types
public enum Sdf {}     // Scene Description
public enum Usd {}     // Universal Scene Description
public enum UsdGeom {} // USD Geometry
```

Then add typealiases and extensions within:
```swift
public extension Usd {
    typealias Stage = Pixar.UsdStage
    typealias StageRefPtr = Pixar.UsdStageRefPtr
}
```

### Swift-Friendly Enum Wrapper Pattern

Wrap C++ enums with Swift enums for a better API:

```swift
public extension Usd.Stage {
    enum InitialLoadingSet {
        case all
        case none

        public var rawValue: Pixar.UsdStage.InitialLoadSet {
            switch self {
            case .all: return .LoadAll
            case .none: return .LoadNone
            }
        }
    }

    static func open(_ filePath: String, load: InitialLoadingSet = .all) -> UsdStageRefPtr {
        Pixar.UsdStage.Open(std.string(filePath), load.rawValue)
    }
}
```

### Iterator Adaptation Pattern

Convert C++ iterators to Swift collections:

```swift
func traverse() -> [Usd.Prim] {
    let it = Usd.PrimRange.Stage(getPtr(), .init())
    return IteratorSequence(it).map { $0 }
}
```

### C++ Container Conversion Pattern

Working with C++ std::vector:

```swift
// Create and populate a C++ vector
var plugPaths = Pixar.PlugRegistry.PlugPathsVector()
plugPaths.push_back(std.string(path))

// Pass to C++ API
Pixar.PlugRegistry.GetInstance().RegisterPlugins(plugPaths)
```

---

## Resource Initialization (CRITICAL)

**You MUST call `Pixar.Bundler.shared.setup(.resources)` before using ANY USD APIs:**

```swift
import PixarUSD

@main
struct MyApp {
    static func main() {
        // REQUIRED: Initialize USD plugins and resources
        Pixar.Bundler.shared.setup(.resources)

        // Now safe to use USD APIs
        let stage = Usd.Stage.createNew("scene.usda")
    }
}
```

This initialization:
1. Discovers all USD resource paths across modules
2. Registers USD plugins with the Pixar plugin registry
3. Loads schema definitions

**Failure to initialize will result in**:
- Missing schema definitions
- Plugin load failures
- Runtime errors when accessing USD prims/attributes

### Debug Flags

Enable debug logging for troubleshooting:

```swift
// In Package.swift swiftSettings:
.define("DEBUG_PIXAR_BUNDLE"),        // Resource loading debug
.define("DEBUG_MEMORY_MANAGEMENT"),   // Memory management debug
```

---

## Umbrella Headers

Each C++ module has an umbrella header at `Sources/<Module>/include/<Module>/<Module>.h`. The goal is to include **all headers** for complete API exposure to Swift. Only exclude headers when source-level fixes are not possible.

### Swift Wrappers (PixarUSD)

Swift extensions live in `Sources/PixarUSD/` organized by USD domain:
```
Sources/PixarUSD/
├── Base/       # Wrappers for Tf, Gf, Vt, Arch, etc.
├── Usd/        # Wrappers for Sdf, Usd, UsdGeom, etc.
├── Imaging/    # Wrappers for Hd, Hgi, etc.
├── UsdImaging/ # Wrappers for UsdImaging
└── Bundle/     # Resource and plugin management (Pixar.Bundler)
```

---

## Source-Level Fixes for Swift Compatibility

When OpenUSD code doesn't compile in Swift module context, **fix the source** rather than excluding headers.

### Fix 1: TF_FATAL_ERROR Format Strings

**Problem**: TF_FATAL_ERROR uses iostream-style `<<` but Swift module builds require printf-style.

**Fix**: Convert to printf format:
```cpp
// Before (fails in Swift module build)
TF_FATAL_ERROR("Key " << key << " not found");

// After (works in Swift module build)
TF_FATAL_ERROR("Key '%s' not found", key.c_str());
```

### Fix 2: TF_DEFINE_STACKED Macro

**Problem**: Creates thread-local types that fail in Swift module context.

**Fix**: Modify the macro expansion or refactor to avoid thread-local storage in headers. For `Ts/raii.h`, this is the one known case where exclusion may still be necessary until upstream fixes.

### Fix 3: Forward Declaration Issues

**Problem**: Forward declarations don't work across Swift module boundaries.

**Fix**: Ensure the full type definition is visible by including the defining header earlier in the include chain, or restructuring to avoid forward declarations in public APIs.

### Fix 4: Template Metaprogramming

**Problem**: Complex template metaprogramming not exported in module builds.

**Fix**: Simplify or provide explicit instantiations for commonly used types.

### Fix 5: Bool Type Conflicts

**Problem**: Some USD types define `Bool` which conflicts with Swift's `Bool`.

**Fix**: Rename or namespace-qualify the conflicting type:
```cpp
namespace Sdf_CrateTypes {
    using CrateBool = bool;  // Renamed from Bool
}
```

### Fix 6: Platform-Specific Headers

**Fix**: Use preprocessor guards:
```cpp
#if defined(__APPLE__)
#include <Module/darwinSpecific.h>
#elif defined(_WIN32)
#include <Module/windowsSpecific.h>
#elif defined(__linux__)
#include <Module/linuxSpecific.h>
#endif
```

---

## Creating USD Content (Swift)

```swift
import PixarUSD

// Initialize (REQUIRED)
Pixar.Bundler.shared.setup(.resources)

// Create a new stage
let stage = Usd.Stage.createNew("HelloWorld.usda")

// Define geometry
UsdGeom.Xform.define(stage, path: "/Root")
UsdGeom.Sphere.define(stage, path: "/Root/Sphere")

// Set attributes
if let sphere = UsdGeom.Sphere(stage.getPrimAtPath("/Root/Sphere")) {
    sphere.getRadiusAttr().set(2.0)
}

// Traverse the stage
for prim in stage.traverse() {
    print(prim.GetPath())
}

// Save
stage.save()
```

---

## OpenUSD Upgrade Process

This section describes the generic process for upgrading to ANY future OpenUSD version.

### Pre-Upgrade Checklist

1. **Check VFX Reference Platform compatibility** - MetaverseKit versions should align
2. **Review OpenUSD release notes** for breaking changes
3. **Backup current working state** with a git tag

### Step 1: Update Version Constants

In `Sources/pxr/include/pxr/pxrns.h`:
```cpp
#define PXR_MAJOR_VERSION 0
#define PXR_MINOR_VERSION <NEW_MINOR>
#define PXR_PATCH_VERSION <NEW_PATCH>
#define PXR_VERSION <NEW_VERSION_INT>  // e.g., 2511 for 25.11
```

### Step 2: Sync OpenUSD Sources

For each module in `Sources/`:
1. Download the corresponding OpenUSD release source
2. Replace C++ source files (preserving SwiftUSD modifications)
3. Update umbrella headers if new headers were added

**Key directories to update**:
- `Sources/<Module>/*.cpp` - Implementation files
- `Sources/<Module>/include/<Module>/*.h` - Headers

### Step 3: Fix Swift Compatibility Issues

For each module that fails to build:

1. **Identify the error**:
   ```bash
   swift bundler run -c release UsdView 2>&1 | grep "error:"
   ```

2. **Apply source-level fix** (see patterns above)

3. **Verify umbrella header** includes the fixed header

4. **Test incrementally**:
   ```bash
   swift bundler run -c release UsdView
   ```

### Step 4: Update MetaverseKit (if needed)

If the new OpenUSD version requires updated VFX libraries:

**MetaverseKit is available at**: `~/dev/Athem/MetaverseKit`

1. Check VFX Reference Platform requirements for the target OpenUSD version
2. Update library versions in MetaverseKit's `Package.swift` and `VERSIONS.md`
3. Rebuild MetaverseKit and verify all libraries compile
4. For local development, you can use a path dependency:
   ```swift
   // In SwiftUSD's Package.swift (temporary for testing)
   .package(path: "../MetaverseKit"),
   ```
5. Once MetaverseKit changes are pushed, update SwiftUSD's Package.swift to use the new branch/tag

### Step 5: Update Swift Wrappers

Check if new APIs need Swift wrappers in `Sources/PixarUSD/`:
1. Review new OpenUSD APIs in release notes
2. Add typealiases for new types
3. Add extensions for Swift-friendly access
4. Update documentation

### Step 6: Increment Evolution and Test

```cpp
#define SWIFTUSD_EVOLUTION <INCREMENTED>  // Bump for each SwiftUSD release
```

Run full test suite:
```bash
swift bundler run -c release Examples
swift bundler test
```

### Step 7: Update Documentation

1. Update this CLAUDE.md with any new patterns discovered
2. Update VERSIONS.md if it exists
3. Document any breaking changes

---

## Debugging Build Errors

### "Unknown type" or "Undeclared identifier"

1. **First**: Check if the type's header needs a source-level fix
2. **Then**: Ensure the header is included in the umbrella header
3. **Last resort**: If unfixable, exclude from umbrella (document why in a comment)

### Module Circular Dependencies

1. Check include order in umbrella headers
2. Ensure base types are defined before dependent types
3. Use forward declarations carefully (they may not work across module boundaries)

### Template Instantiation Failures

1. Provide explicit instantiations for commonly used template types
2. Move template definitions to be visible at instantiation point

### Linker Errors

1. Check that all required modules are listed as dependencies in Package.swift
2. Verify MetaverseKit is providing expected symbols
3. Check platform-conditional compilation flags

---

## Platform Detection Pattern

SwiftUSD uses the `Arch.OS` enum for platform-conditional dependencies in Package.swift:

```swift
Arch.OS.apple.platform     // All Apple platforms
Arch.OS.linux.platform     // Linux
Arch.OS.windows.platform   // Windows
Arch.OS.linwin.platform    // Linux + Windows
Arch.OS.embeddedapple.platform    // iOS, visionOS, tvOS, watchOS (no macOS)
Arch.OS.noembeddedapple.platform  // macOS, Linux, Windows
```

---

## Repository Structure

```
SwiftUSD/
├── Package.swift              # SPM manifest
├── CLAUDE.md                  # This file
├── Sources/
│   ├── pxr/                   # Root namespace header
│   ├── Arch/                  # Architecture (imports MetaverseKit)
│   ├── Tf/, Gf/, Vt/, ...     # Base libraries
│   ├── Sdf/, Pcp/, Usd/       # USD core
│   ├── UsdGeom/, UsdShade/... # USD schemas
│   ├── Hd/, HdSt/, Hgi/...    # Imaging/Hydra
│   ├── UsdImaging/...         # USD imaging bridge
│   ├── PixarUSD/              # Swift wrapper library
│   │   ├── Base/              # Base library wrappers
│   │   ├── Usd/               # USD wrappers
│   │   ├── Imaging/           # Imaging wrappers
│   │   └── Bundle/            # Resource management
│   ├── PixarMacros/           # Swift macros
│   ├── UsdView/               # Viewer application
│   └── Examples/              # Example code
└── .build/                    # Build artifacts
```

---

## Quick Reference

| Task | Command/Location |
|------|------------------|
| Build & run viewer | `swift bundler run -c release UsdView` |
| Run examples | `swift bundler run -c release Examples` |
| Run tests | `swift bundler test` |
| Version constants | `Sources/pxr/include/pxr/pxrns.h` |
| MetaverseKit deps | `Arch` target in Package.swift |
| Swift wrappers | `Sources/PixarUSD/` |
| Resource init | `Pixar.Bundler.shared.setup(.resources)` |
