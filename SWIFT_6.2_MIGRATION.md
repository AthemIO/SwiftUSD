# Swift 6.2 Migration Guide for SwiftUSD

Complete guide for migrating SwiftUSD to take advantage of Swift 6.2's enhanced C++ interoperability features.

## Table of Contents

1. [Overview](#overview)
2. [Timeline of Changes](#timeline-of-changes)
3. [Current Status](#current-status)
4. [Swift 6.2 New Features](#swift-62-new-features)
5. [Migration Strategy](#migration-strategy)
6. [Breaking Changes](#breaking-changes)
7. [USD-Specific Considerations](#usd-specific-considerations)
8. [Performance Implications](#performance-implications)
9. [Testing and Validation](#testing-and-validation)
10. [Resources](#resources)

---

## Overview

Swift 6.2 introduces groundbreaking improvements to C++ interoperability, particularly around memory safety and lifetime management. This guide helps SwiftUSD adopt these features progressively while maintaining compatibility.

**Key Principle**: Swift maintains strong source compatibility guarantees. Future changes to C++ interop won't break existing code by default.

**Current SwiftUSD Status**:
- ✅ Fully compatible with Swift 6.0, 6.1, and 6.2
- ✅ Uses correct `.interoperabilityMode(.Cxx)` settings
- ✅ No immediate migration required
- 📋 Optional adoption of Swift 6.2 safe interop features

---

## Timeline of Changes

### Swift 6.0 (June 2024) - Foundation

**Major C++ Interop Features Added**:
- Move-only/noncopyable types support
- Virtual methods on reference types
- Default arguments
- `std::map`, `std::optional`, `std::vector`, `std::string` support
- Reference type annotations: `SWIFT_SHARED_REFERENCE`, `SWIFT_IMMORTAL_REFERENCE`, `SWIFT_UNSAFE_REFERENCE`

**Known Issues**:
- `std::shared_ptr<T>.pointee` copies T instead of borrowing
- `std::unique_ptr` with pimpl patterns can fail
- Platform-specific `std::map` issues on iOS

### Swift 6.1 (March 2025) - Consolidation

**C++ Interop Status**: No significant changes

**Focus Areas**:
- Concurrency improvements (nonisolated types, task groups)
- Objective-C interop enhancements
- Package manager stability
- Swift Testing framework

**Impact on SwiftUSD**: Zero migration work required from Swift 6.0

### Swift 6.2 (September 2025) - Safe Interoperability Revolution

**Major C++ Interop Features Added**:
- **Safe C++ Interoperability mode** (experimental)
- **Span and MutableSpan types** for zero-overhead views
- **InlineArray type** for fixed-size, stack-allocated arrays
- **std::span integration** with automatic safe bridging
- **Lifetime annotations** (`__lifetimebound`, `__counted_by`, `__sized_by`)
- **Memory safety** without performance penalties

---

## Current Status

### SwiftUSD is Swift 6.0/6.1/6.2 Compatible

Your codebase already uses the correct patterns:

```swift
// ✅ Package.swift uses correct interoperability mode
.target(
  name: "PixarUSD",
  dependencies: [...],
  swiftSettings: [
    .interoperabilityMode(.Cxx)  // Correct for Swift 6.0+
  ]
)
```

```swift
// ✅ Direct C++ interop via namespace header
import pxr  // Exposes pxr namespace from C++
import PixarUSD  // Swift wrapper layer
```

```swift
// ✅ USD smart pointers work (with known limitations)
let stage: UsdStageRefPtr = Usd.Stage.createNew("scene.usda")
let layer: Sdf.LayerRefPtr = Sdf.Layer.findOrOpen("layer.usda")
```

### No Immediate Action Required

- Swift 6.1 requires **zero changes** from Swift 6.0
- Swift 6.2 requires **zero changes** from Swift 6.1
- Swift 6.2 experimental features are **opt-in**

---

## Swift 6.2 New Features

### 1. Safe C++ Interoperability Mode (Experimental)

Enable with compiler flag:

```swift
// In Package.swift
.target(
  name: "PixarUSD",
  swiftSettings: [
    .interoperabilityMode(.Cxx),
    .unsafeFlags([
      "-enable-experimental-feature", "SafeInteropWrappers"
    ])
  ]
)
```

This unlocks memory-safe abstractions for C++ pointers and views.

### 2. Span and MutableSpan Types

Non-escapable types providing safe, direct memory access to C++ collections:

**Key Characteristics**:
- **Stack-allocated**: Zero heap overhead
- **Non-escapable**: Cannot be stored or outlive parent's lifetime
- **Compile-time safety**: Lifetime checking at compile time
- **Zero-cost**: Same performance as raw pointers

**Swift 6.0 (Current)**:
```swift
// Unsafe pointer access
let prims = stage.getPseudoRoot().getChildren()
for prim in prims {
  // Manual lifetime management
  // Risk of dangling references
}
```

**Swift 6.2 (With Span)**:
```swift
// C++ API annotated with lifetime bounds
// std::span<const UsdPrim> getPrims() const __lifetimebound;

// Swift automatically gets safe wrapper
func getPrims() -> Span<UsdPrim>  // Compile-time safe, zero overhead

// Usage
let prims = stage.getPrims()  // Span<UsdPrim>
for prim in prims {
  // Compiler prevents escape, ensures safety
}
```

**Integration with std::span**:

C++ APIs using `std::span` automatically bridge to safe Swift types:

```cpp
// C++ USD API (hypothetical future annotation)
class UsdStage {
  std::span<const UsdPrim> GetPrimsInSubtree(const SdfPath& path) const;
};
```

```swift
// Swift automatically receives
func getPrimsInSubtree(_ path: Sdf.Path) -> Span<UsdPrim>
```

### 3. InlineArray Type

Fixed-size, stack-allocated arrays with C-compatible memory layout:

**Swift 6.0 (Current)**:
```swift
// USD Matrix4d as tuple or array
typealias Matrix4d = (Double, Double, Double, Double,
                      Double, Double, Double, Double,
                      Double, Double, Double, Double,
                      Double, Double, Double, Double)

// Or using unsafe buffers
var matrix = Array<Double>(repeating: 0, count: 16)
```

**Swift 6.2 (With InlineArray)**:
```swift
// Fixed-size, stack-allocated, C-compatible
typealias Matrix4d = InlineArray<Double, 16>

// Benefits:
// - Same memory layout as C arrays
// - Stack-allocated (no heap)
// - Subscript access
// - Compatible with std::span
```

**USD Use Cases**:
```swift
// Mathematical types
typealias Matrix4d = InlineArray<Double, 16>
typealias Matrix3d = InlineArray<Double, 9>
typealias Vec4d = InlineArray<Double, 4>
typealias Vec3d = InlineArray<Double, 3>
typealias Vec2d = InlineArray<Double, 2>

// Color types
typealias Color4f = InlineArray<Float, 4>  // RGBA
typealias Color3f = InlineArray<Float, 3>  // RGB
```

### 4. Lifetime Annotations

Communicate lifetime dependencies between parameters and return values:

**`__lifetimebound`** - Return value depends on parameter lifetime:

```cpp
// C++ header annotation
class UsdPrim {
  const UsdPrimDefinition& GetPrimDefinition() const __lifetimebound;
  //                                                   ^^^^^^^^^^^^^^
  // Returned reference lives only as long as 'this'
};
```

**`__counted_by(param)`** - Pointer with count:

```cpp
// C++ API with count-based bounds
const UsdAttribute* GetAttributes(size_t& count) __counted_by(count);
```

```swift
// Swift receives safe Span
func getAttributes() -> Span<UsdAttribute>
```

**`__sized_by(param)`** - Pointer with byte size:

```cpp
// C++ API with size in bytes
const void* GetRawData(size_t& byteSize) __sized_by(byteSize);
```

**`__lifetime_capture_by(param)`** - Output parameter captures lifetime:

```cpp
void FillBuffer(std::span<UsdPrim> output) __lifetime_capture_by(output);
```

### 5. Bounds Annotations for Raw Pointers

Legacy C and older C++ APIs can gain Swift 6.2 safety:

```cpp
// From <swift/bridging>
#include <swift/bridging>

// Example: USD C API (hypothetical)
typedef struct UsdStageHandle* UsdStageRef;

// Annotate array-returning function
const UsdPrim* UsdStage_GetChildren(
  UsdStageRef stage,
  size_t* outCount
) __counted_by(*outCount);
```

```swift
// Swift receives:
func getChildren() -> Span<UsdPrim>
```

---

## Migration Strategy

### Phase 1: Current (Swift 6.0/6.1) - **No Changes**

**Status**: Production ready, fully supported

**Approach**: Continue using existing patterns

**Timeline**: Immediate to ongoing

**Actions**:
- ✅ No code changes required
- ✅ Monitor known issues (`std::shared_ptr.pointee` copying)
- ✅ Stay on Swift 6.0 or 6.1 compiler

**Code Pattern**:
```swift
// Current patterns continue to work
import PixarUSD

let stage = Usd.Stage.createNew("scene.usda")
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")
sphere.getRadiusAttr().set(2.0)
```

### Phase 2: Swift 6.2 Experimental (2025-2026) - **Optional Adoption**

**Status**: Experimental, testing recommended

**Approach**: Test safe interop in non-critical paths

**Timeline**: When Swift 6.2 stabilizes (late 2025/early 2026)

**Prerequisites**:
- Swift 6.2 compiler installed
- Understanding of lifetime annotations
- Test infrastructure in place

**Actions**:
1. **Enable experimental feature** (specific targets only):

```swift
// Package.swift
.target(
  name: "UsdViewExperimental",
  dependencies: ["PixarUSD"],
  swiftSettings: [
    .interoperabilityMode(.Cxx),
    .unsafeFlags([
      "-enable-experimental-feature", "SafeInteropWrappers"
    ])
  ]
)
```

2. **Annotate selected USD headers** with lifetime bounds:

```cpp
// Sources/Usd/include/Usd/stage.h
class UsdStage {
  // Add __lifetimebound to appropriate methods
  std::span<const UsdPrim> GetAllPrims() const __lifetimebound;
};
```

3. **Refactor selected APIs** to use Span:

```swift
// Before (Swift 6.0)
func processAllPrims(_ stage: UsdStageRefPtr) {
  for prim in Usd.PrimRange(stage.getPseudoRoot()) {
    // Process prim
  }
}

// After (Swift 6.2 experimental)
func processAllPrims(_ stage: UsdStageRefPtr) {
  let prims = stage.getAllPrims()  // Returns Span<UsdPrim>
  for prim in prims {
    // Compile-time lifetime safety
  }
}
```

4. **Use InlineArray** for USD math types:

```swift
// Before (Swift 6.0)
var matrixData = [Double](repeating: 0, count: 16)

// After (Swift 6.2)
var matrix = Matrix4d()  // InlineArray<Double, 16>
matrix[0] = 1.0
matrix[5] = 1.0
matrix[10] = 1.0
matrix[15] = 1.0
```

5. **Measure performance** impact:

```swift
// Benchmark before/after for critical paths
// - UsdPrim traversal
// - Attribute access
// - Matrix operations
// - Render loop hot paths
```

6. **Gather feedback** and file issues with Swift team

### Phase 3: Swift 6.2 Stable (2026+) - **Gradual Migration**

**Status**: Production ready (future)

**Approach**: Adopt safe interop as default for new code

**Timeline**: When Swift declares feature stable

**Actions**:
1. **Update Package.swift** to require Swift 6.2+:

```swift
// swift-tools-version: 6.2
```

2. **Remove experimental flag**:

```swift
.target(
  name: "PixarUSD",
  swiftSettings: [
    .interoperabilityMode(.Cxx)
    // SafeInteropWrappers now default
  ]
)
```

3. **Annotate all public USD C++ headers**:

```cpp
// Systematic annotation of USD API surface
class UsdStage {
  std::span<const UsdPrim> GetAllPrims() const __lifetimebound;
  const UsdPrim& GetPrimAtPath(const SdfPath& path) const __lifetimebound;
  // ... all lifetime-dependent methods
};
```

4. **Refactor Swift wrapper layer**:

```swift
// Sources/PixarUSD/Usd/Stage/Stage.swift
extension Usd.Stage {
  /// Get all prims in stage (safe view)
  public func allPrims() -> Span<UsdPrim> {
    // Leverage annotated C++ API
    return pointee.GetAllPrims()
  }
}
```

5. **Update documentation** with Swift 6.2 best practices

6. **Deprecate unsafe patterns** where safe alternatives exist

---

## Breaking Changes

### Compatibility Promise

> "Future changes to the design or functionality of C++ interoperability will not break code in existing codebases by default."
>
> — Swift C++ Interoperability Documentation

Swift maintains multiple compatibility versions of C++ interoperability features.

### Known Breaking Changes

#### 1. Enabling C++ Interop Can Break Existing Code

**Issue**: C++ diverges from C in edge cases, breaking existing C/Objective-C imports.

**Example**:
```swift
// Works in C/Objective-C mode
let x = NULL

// May fail with .interoperabilityMode(.Cxx)
// C++ has stricter type rules
```

**Mitigation**:
- Enable `.interoperabilityMode(.Cxx)` only in major semver versions
- Test thoroughly after enabling
- Use `#if compiler(>=6.0)` for conditional compilation

#### 2. Compiler Flag Evolution

**Swift 5.9** (Deprecated):
```bash
-enable-experimental-cxx-interop
```

**Swift 6.0+** (Correct):
```bash
-cxx-interoperability-mode=default
```

**Swift 6.2** (Additional):
```bash
-enable-experimental-feature SafeInteropWrappers
```

**Mitigation**: Always use Package.swift settings (not direct flags)

#### 3. Swift 6.2 Experimental Feature Stability

**Status**: SafeInteropWrappers is experimental in Swift 6.2

**Implications**:
- APIs may change before stabilization
- Annotations may require updates
- Must use feature flag

**Mitigation**:
- Use only in experimental targets initially
- Prepare for API evolution
- Monitor Swift forums and release notes

---

## USD-Specific Considerations

### 1. std::shared_ptr Copying Issue

**Problem**: `UsdStageRefPtr.pointee` copies the entire composed stage.

**Current Workaround (Swift 6.0/6.1)**:
```swift
// Avoid:
let stageValue = stageRefPtr.pointee  // EXPENSIVE COPY

// Instead, work through the pointer:
stageRefPtr.pointee.GetPrimAtPath(...)
stageRefPtr.pointee.Save()
```

**Swift 6.2 Potential Solution**:
```cpp
// Annotate UsdStageRefPtr methods with __lifetimebound
class UsdStageRefPtr {
  const UsdStage& operator*() const __lifetimebound;
  UsdStage& operator*() __lifetimebound;
};
```

```swift
// Swift may optimize to borrowing instead of copying
let stage = stageRefPtr.pointee  // Potentially safe borrow in 6.2
```

**Status**: Requires testing with Swift 6.2 compiler

### 2. USD Collection Types

USD has many collection-returning APIs that could benefit from Span:

**Candidates for Span Conversion**:
```cpp
// Current USD C++ APIs (examples)
class UsdStage {
  UsdPrimRange GetAllPrims() const;  // Iterator-based

  // Could become:
  std::span<const UsdPrim> GetAllPrims() const __lifetimebound;
};

class UsdPrim {
  std::vector<UsdAttribute> GetAttributes() const;

  // Could become:
  std::span<const UsdAttribute> GetAttributes() const __lifetimebound;
};
```

**Benefit**: Zero-copy, compile-time safe access to USD collections

### 3. USD Math Types

USD uses many fixed-size types perfect for InlineArray:

**Current (Swift 6.0)**:
```swift
// Using GfVec3d, GfMatrix4d from C++
let vec = Pixar.GfVec3d(1, 2, 3)
var matrix = Pixar.GfMatrix4d(1.0)
```

**Potential Swift 6.2 Enhancement**:
```swift
// Swift-native fixed-size arrays
typealias Vec3d = InlineArray<Double, 3>
typealias Matrix4d = InlineArray<Double, 16>

// Extension for compatibility
extension Pixar.GfVec3d {
  init(_ array: Vec3d) { ... }
  var asInlineArray: Vec3d { ... }
}
```

**Benefit**: Native Swift types with C-compatible layout

### 4. USD Smart Pointer Types

USD extensively uses smart pointers that could benefit from lifetime annotations:

```cpp
// Major USD smart pointer types
using UsdStageRefPtr = TfRefPtr<UsdStage>;
using SdfLayerRefPtr = TfRefPtr<SdfLayer>;
using UsdPrimDefinitionRefPtr = TfRefPtr<const UsdPrimDefinition>;

// Potential annotations
class TfRefPtr<T> {
  T& operator*() const __lifetimebound;
  T* operator->() const __lifetimebound;
  T* get() const __lifetimebound;
};
```

### 5. UsdPrim Traversal

PrimRange could potentially become Span-based:

**Current (Swift 6.0)**:
```swift
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
  // Iterator-based traversal
}
```

**Potential Swift 6.2**:
```swift
let prims = stage.getAllPrims()  // Span<UsdPrim>
for prim in prims {
  // Direct span iteration
}

// Or with filtering
let geometryPrims = stage.getAllPrims()
  .filter { $0.isA(UsdGeom.Gprim.self) }
```

**Consideration**: PrimRange is lazy; Span is eager. Need to evaluate tradeoffs.

---

## Performance Implications

### Expected Performance Characteristics

**Span vs Raw Pointers**:
- **Compile time**: Minimal increase (lifetime checking)
- **Runtime**: Identical (zero-cost abstraction)
- **Memory**: No heap allocation overhead

**InlineArray vs std::array**:
- **Memory layout**: Identical
- **Performance**: Identical (direct stack allocation)
- **Safety**: Improved (bounds checking in debug mode)

### Performance Testing Plan

**Benchmarks to Create**:

1. **Prim Traversal**:
```swift
// Before
func benchmarkPrimTraversal_Current() {
  for prim in Usd.PrimRange(stage.getPseudoRoot()) {
    _ = prim.getName()
  }
}

// After (if Span-based)
func benchmarkPrimTraversal_Span() {
  let prims = stage.getAllPrims()
  for prim in prims {
    _ = prim.getName()
  }
}
```

2. **Attribute Access**:
```swift
func benchmarkAttributeAccess() {
  let sphere = UsdGeom.Sphere(stage.getPrimAtPath(...))
  for _ in 0..<1000 {
    var radius: Double = 0
    sphere.getRadiusAttr().get(&radius)
  }
}
```

3. **Matrix Operations**:
```swift
// Before
func benchmarkMatrix_Current() {
  var matrix = Pixar.GfMatrix4d(1.0)
  // Operations...
}

// After
func benchmarkMatrix_InlineArray() {
  var matrix = Matrix4d()  // InlineArray
  // Same operations...
}
```

4. **Render Loop**:
```swift
func benchmarkRenderLoop() {
  for frame in 1...240 {
    let texture = engine.render(at: Double(frame), viewSize: viewSize)
    // Measure frame time
  }
}
```

**Metrics to Collect**:
- Execution time (median, p95, p99)
- Memory usage (peak, average)
- Allocations count
- Compilation time

---

## Testing and Validation

### Test Strategy

**Phase 1: Unit Tests**
```swift
// Tests/PixarUSDTests/Swift62InteropTests.swift
import XCTest
@testable import PixarUSD

class Swift62InteropTests: XCTestCase {
  func testSpanSafety() {
    // Test lifetime rules are enforced
  }

  func testInlineArrayLayout() {
    // Verify C-compatible memory layout
  }

  func testLifetimeBounds() {
    // Verify lifetime annotations work correctly
  }
}
```

**Phase 2: Integration Tests**
```swift
func testFullSceneLifecycle() {
  let stage = Usd.Stage.createNew("test.usda")
  // Create complex scene
  // Traverse with Span
  // Verify no crashes or memory issues
}
```

**Phase 3: Performance Regression Tests**
```swift
func testPerformanceRegression() {
  measure {
    // Ensure Swift 6.2 changes don't regress performance
  }
}
```

### Validation Checklist

- [ ] All existing tests pass with Swift 6.2 compiler
- [ ] No new warnings or deprecations
- [ ] Memory usage unchanged (or improved)
- [ ] Performance within 5% of baseline
- [ ] Address sanitizer reports no issues
- [ ] Thread sanitizer reports no issues
- [ ] Build time acceptable
- [ ] API documentation updated
- [ ] Migration guide reviewed
- [ ] Examples updated

---

## Resources

### Official Swift Documentation

- [Mixing Swift and C++](https://www.swift.org/documentation/cxx-interop/)
- [Safe C++ Interoperability](https://www.swift.org/documentation/cxx-interop/safe-interop/)
- [C++ Interop Status](https://www.swift.org/documentation/cxx-interop/status/)
- [Swift 6.2 Release Notes](https://www.swift.org/blog/swift-6.2-released/)

### Swift Evolution Proposals

- Vision: "Using C++ from Swift" (Accepted May 2023)
- Vision: "Using Swift from C++" (Accepted Dec 2022)

### GitHub Resources

- [C++ Interoperability Status](https://github.com/swiftlang/swift/blob/main/docs/CppInteroperability/CppInteroperabilityStatus.md)
- [Getting Started Guide](https://github.com/swiftlang/swift/blob/main/docs/CppInteroperability/GettingStartedWithC++Interop.md)
- [Swift Forums - C++ Interop](https://forums.swift.org/c/development/cxx-interop/)

### SwiftUSD Documentation

- [Documentation.md](./Documentation.md) - Complete SwiftUSD usage guide
- [SWIFT_6.2_EXAMPLES.md](./SWIFT_6.2_EXAMPLES.md) - Before/after code examples
- [CLAUDE.md](./CLAUDE.md) - Developer guidance

### External Resources

- [Pixar USD Documentation](https://openusd.org)
- [USD API Reference](https://openusd.org/release/api/index.html)
- [SwiftUSD GitHub](https://github.com/LynrAI/SwiftUSD)

---

## Feedback and Contributions

If you encounter issues or have suggestions for Swift 6.2 migration:

1. **File Issues**: [GitHub Issues](https://github.com/LynrAI/SwiftUSD/issues)
2. **Swift Forums**: [C++ Interop Category](https://forums.swift.org/c/development/cxx-interop/)
3. **Pull Requests**: Contributions welcome

---

**Document Version**: 1.0
**Last Updated**: 2025-01-12
**Swift Version**: 6.2
**SwiftUSD Version**: 24.8.14
