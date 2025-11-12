# Swift 6.2 Code Examples for SwiftUSD

Practical before/after examples showing how to adopt Swift 6.2's enhanced C++ interoperability features in SwiftUSD.

## Table of Contents

1. [Span and MutableSpan](#span-and-mutablespan)
2. [InlineArray for USD Math Types](#inlinearray-for-usd-math-types)
3. [Lifetime Annotations](#lifetime-annotations)
4. [USD Collection Access](#usd-collection-access)
5. [Smart Pointer Improvements](#smart-pointer-improvements)
6. [Performance Comparisons](#performance-comparisons)

---

## Span and MutableSpan

### Example 1: USD Prim Collection Access

**Swift 6.0/6.1 (Current)**:
```swift
import PixarUSD

func processStageGeometry(_ stage: UsdStageRefPtr) {
    // Iterator-based traversal with manual lifetime management
    for prim in Usd.PrimRange(stage.getPseudoRoot()) {
        if prim.isA(UsdGeom.Gprim.self) {
            let gprim = UsdGeom.Gprim(prim)
            print("Processing: \(gprim.getPath())")
        }
    }
}
```

**Swift 6.2 (With Span)**:
```swift
import PixarUSD

// Assuming USD headers annotated with __lifetimebound
func processStageGeometry(_ stage: UsdStageRefPtr) {
    // Safe, compile-time checked view into prim collection
    let prims = stage.getAllPrims()  // Returns Span<UsdPrim>

    for prim in prims where prim.isA(UsdGeom.Gprim.self) {
        let gprim = UsdGeom.Gprim(prim)
        print("Processing: \(gprim.getPath())")
    }

    // Compiler prevents escaping the span beyond stage lifetime
    // This would be a compile error:
    // globalPrims = prims  // ERROR: Cannot escape parent lifetime
}
```

**Benefits**:
- ✅ Compile-time lifetime safety
- ✅ Zero overhead (same as raw pointers)
- ✅ Cannot accidentally create dangling references
- ✅ Explicit lifetime relationships

### Example 2: Attribute Collection

**Swift 6.0/6.1 (Current)**:
```swift
func inspectPrimAttributes(_ prim: Usd.Prim) {
    let attrs = prim.getAttributes()  // Returns vector, copies all attributes

    for attr in attrs {
        print("\(attr.getName()): \(attr.getTypeName())")
    }
    // Potential performance overhead from vector copy
}
```

**Swift 6.2 (With Span)**:
```swift
func inspectPrimAttributes(_ prim: Usd.Prim) {
    let attrs = prim.getAttributesSpan()  // Returns Span<UsdAttribute>

    for attr in attrs {
        print("\(attr.getName()): \(attr.getTypeName())")
    }
    // Zero-copy view, lifetime tied to prim
}
```

**Benefits**:
- ✅ No vector copy
- ✅ Direct memory access
- ✅ Automatic lifetime management

### Example 3: Mutable Buffer Access

**Swift 6.0/6.1 (Current)**:
```swift
func updateMeshPoints(_ mesh: UsdGeom.Mesh, _ newPoints: [Pixar.GfVec3f]) {
    // Must create VtArray, potential copy
    let pointsArray = Vt.Vec3fArray(UInt(newPoints.count), newPoints)
    mesh.getPointsAttr().set(pointsArray)
}
```

**Swift 6.2 (With MutableSpan)**:
```swift
func updateMeshPoints(_ mesh: UsdGeom.Mesh, _ newPoints: [Pixar.GfVec3f]) {
    // Direct mutable view into USD's internal buffer
    var pointsSpan = mesh.getPointsMutableSpan()  // MutableSpan<GfVec3f>

    for (index, point) in newPoints.enumerated() {
        pointsSpan[index] = point  // Direct write, no copy
    }
    // Changes reflected immediately in USD data
}
```

**Benefits**:
- ✅ Zero-copy mutation
- ✅ In-place updates
- ✅ Compile-time bounds checking

---

## InlineArray for USD Math Types

### Example 4: Matrix Operations

**Swift 6.0/6.1 (Current)**:
```swift
import PixarUSD

func createTransformMatrix() -> Pixar.GfMatrix4d {
    // Using C++ type directly
    var matrix = Pixar.GfMatrix4d(1.0)  // Identity

    // Access via C++ methods
    matrix.SetTranslate(Pixar.GfVec3d(5.0, 0.0, 0.0))
    matrix.SetScale(2.0)

    return matrix
}

func extractMatrixData(_ matrix: Pixar.GfMatrix4d) -> [Double] {
    // Manual extraction to Swift array
    var data = [Double](repeating: 0, count: 16)
    for i in 0..<4 {
        for j in 0..<4 {
            data[i * 4 + j] = matrix.Get(Int32(i), Int32(j))
        }
    }
    return data
}
```

**Swift 6.2 (With InlineArray)**:
```swift
import PixarUSD

// Define Swift-native matrix type
typealias Matrix4d = InlineArray<Double, 16>

extension Matrix4d {
    static var identity: Matrix4d {
        var m = Matrix4d()
        m[0] = 1; m[5] = 1; m[10] = 1; m[15] = 1
        return m
    }

    // Convert to/from GfMatrix4d
    init(_ gfMatrix: Pixar.GfMatrix4d) {
        self.init()
        for i in 0..<4 {
            for j in 0..<4 {
                self[i * 4 + j] = gfMatrix.Get(Int32(i), Int32(j))
            }
        }
    }

    func asGfMatrix4d() -> Pixar.GfMatrix4d {
        var m = Pixar.GfMatrix4d()
        for i in 0..<4 {
            for j in 0..<4 {
                m.Set(Int32(i), Int32(j), self[i * 4 + j])
            }
        }
        return m
    }
}

func createTransformMatrix() -> Matrix4d {
    // Pure Swift, stack-allocated
    var matrix = Matrix4d.identity

    // Direct array access
    matrix[12] = 5.0  // Translation X
    matrix[13] = 0.0  // Translation Y
    matrix[14] = 0.0  // Translation Z

    return matrix
}

func extractMatrixData(_ matrix: Matrix4d) -> [Double] {
    // Already an array-like structure
    Array(matrix)  // Zero-copy conversion
}
```

**Benefits**:
- ✅ Stack-allocated (no heap)
- ✅ Native Swift type
- ✅ Direct subscript access
- ✅ C-compatible memory layout
- ✅ Works with std::span APIs

### Example 5: Vector Math

**Swift 6.0/6.1 (Current)**:
```swift
func calculateBounds(_ points: [Pixar.GfVec3f]) -> (min: Pixar.GfVec3f, max: Pixar.GfVec3f) {
    guard let first = points.first else {
        return (Pixar.GfVec3f(0, 0, 0), Pixar.GfVec3f(0, 0, 0))
    }

    var minVec = first
    var maxVec = first

    for point in points {
        minVec = Pixar.GfVec3f(
            min(minVec[0], point[0]),
            min(minVec[1], point[1]),
            min(minVec[2], point[2])
        )
        maxVec = Pixar.GfVec3f(
            max(maxVec[0], point[0]),
            max(maxVec[1], point[1]),
            max(maxVec[2], point[2])
        )
    }

    return (minVec, maxVec)
}
```

**Swift 6.2 (With InlineArray)**:
```swift
typealias Vec3f = InlineArray<Float, 3>

extension Vec3f {
    static func min(_ a: Vec3f, _ b: Vec3f) -> Vec3f {
        var result = Vec3f()
        for i in 0..<3 {
            result[i] = Swift.min(a[i], b[i])
        }
        return result
    }

    static func max(_ a: Vec3f, _ b: Vec3f) -> Vec3f {
        var result = Vec3f()
        for i in 0..<3 {
            result[i] = Swift.max(a[i], b[i])
        }
        return result
    }
}

func calculateBounds(_ points: [Vec3f]) -> (min: Vec3f, max: Vec3f) {
    guard let first = points.first else {
        var zero = Vec3f()
        zero[0] = 0; zero[1] = 0; zero[2] = 0
        return (zero, zero)
    }

    var minVec = first
    var maxVec = first

    for point in points {
        minVec = Vec3f.min(minVec, point)
        maxVec = Vec3f.max(maxVec, point)
    }

    return (minVec, maxVec)
}
```

**Benefits**:
- ✅ Pure Swift operations
- ✅ Stack-allocated vectors
- ✅ Compatible with SIMD optimization
- ✅ Interoperable with USD C++ types

### Example 6: Color Types

**Swift 6.0/6.1 (Current)**:
```swift
func setMaterialColor(_ material: UsdShade.Material, _ color: Pixar.GfVec3f) {
    // Create shader
    let shader = UsdShade.Shader.define(
        material.getPrim().getStage(),
        path: material.getPath().appendChild(Pixar.TfToken("Surface"))
    )

    // Set color as GfVec3f
    shader.createInput(Pixar.TfToken("diffuseColor"), .color3f).set(color)
}
```

**Swift 6.2 (With InlineArray)**:
```swift
typealias Color3f = InlineArray<Float, 3>
typealias Color4f = InlineArray<Float, 4>

extension Color3f {
    static let red = Color3f([1.0, 0.0, 0.0])
    static let green = Color3f([0.0, 1.0, 0.0])
    static let blue = Color3f([0.0, 0.0, 1.0])
    static let white = Color3f([1.0, 1.0, 1.0])
    static let black = Color3f([0.0, 0.0, 0.0])

    var asGfVec3f: Pixar.GfVec3f {
        Pixar.GfVec3f(self[0], self[1], self[2])
    }
}

func setMaterialColor(_ material: UsdShade.Material, _ color: Color3f) {
    let shader = UsdShade.Shader.define(
        material.getPrim().getStage(),
        path: material.getPath().appendChild(Pixar.TfToken("Surface"))
    )

    // Convert to GfVec3f for USD API
    shader.createInput(Pixar.TfToken("diffuseColor"), .color3f)
        .set(color.asGfVec3f)
}

// Usage
setMaterialColor(material, .red)  // Clean API
```

**Benefits**:
- ✅ Type-safe color constants
- ✅ Stack-allocated
- ✅ Seamless USD integration

---

## Lifetime Annotations

### Example 7: USD Stage References

**C++ Header Annotation** (hypothetical future):
```cpp
// Sources/Usd/include/Usd/stage.h
class UsdStage {
public:
    // Return value's lifetime bound to 'this'
    const UsdPrim& GetPseudoRoot() const __lifetimebound;

    // Return view's lifetime bound to 'this'
    std::span<const UsdPrim> GetAllPrims() const __lifetimebound;

    // Output parameter captures 'this' lifetime
    void GetPrimChildren(std::span<UsdPrim> output) __lifetime_capture_by(output);
};
```

**Swift 6.0/6.1 (Current) - No Annotation**:
```swift
func unsafeEscapingReference(_ stage: UsdStageRefPtr) -> Usd.Prim {
    let root = stage.getPseudoRoot()  // Unsafe - could dangle
    return root  // DANGEROUS: root's lifetime unclear
}
// Potential crash if stage is deallocated before root is used
```

**Swift 6.2 (With Lifetime Annotations)**:
```swift
func safeReference(_ stage: UsdStageRefPtr) -> Usd.Prim {
    let root = stage.getPseudoRoot()  // Lifetime bound to stage
    return root  // COMPILE ERROR: Cannot escape parent's lifetime
}

// Correct pattern
func safeUsage(_ stage: UsdStageRefPtr) {
    let root = stage.getPseudoRoot()  // OK: used within stage's scope
    for child in root.getChildren() {
        // Safe: all lifetimes bound to stage
    }
}
```

**Benefits**:
- ✅ Compile-time lifetime verification
- ✅ Prevents dangling references
- ✅ No runtime overhead

### Example 8: Attribute References

**C++ Header Annotation** (hypothetical):
```cpp
class UsdAttribute {
public:
    // Return value depends on 'this' lifetime
    const SdfPath& GetPath() const __lifetimebound;

    // Property stack lifetime bound to attribute
    SdfPropertySpecHandleVector GetPropertyStack() const __lifetimebound;
};
```

**Swift 6.2 (With Annotations)**:
```swift
func getAttributePath(_ attr: Usd.Attribute) -> Sdf.Path {
    return attr.getPath()  // COMPILE ERROR: Cannot escape
}

// Correct usage
func processAttribute(_ attr: Usd.Attribute) {
    let path = attr.getPath()  // OK: used locally
    print("Attribute path: \(path)")
}
```

---

## USD Collection Access

### Example 9: Traversing Geometry Hierarchy

**Swift 6.0/6.1 (Current)**:
```swift
func findAllMeshes(_ stage: UsdStageRefPtr) -> [UsdGeom.Mesh] {
    var meshes: [UsdGeom.Mesh] = []

    for prim in Usd.PrimRange(stage.getPseudoRoot()) {
        if prim.isA(UsdGeom.Mesh.self) {
            meshes.append(UsdGeom.Mesh(prim))
        }
    }

    return meshes  // Array copy overhead
}
```

**Swift 6.2 (With Span)**:
```swift
func processMeshes(_ stage: UsdStageRefPtr, _ processor: (UsdGeom.Mesh) -> Void) {
    let prims = stage.getAllPrims()  // Span<UsdPrim>

    for prim in prims where prim.isA(UsdGeom.Mesh.self) {
        processor(UsdGeom.Mesh(prim))
    }
    // No intermediate array, zero-copy iteration
}

// Usage
processMeshes(stage) { mesh in
    print("Mesh: \(mesh.getPath())")
    print("Point count: \(mesh.getPointsAttr().getNumTimeSamples())")
}
```

**Benefits**:
- ✅ No intermediate collections
- ✅ Callback-based processing
- ✅ Zero-copy traversal

### Example 10: Batch Attribute Updates

**Swift 6.0/6.1 (Current)**:
```swift
func animateRadii(_ spheres: [UsdGeom.Sphere], frames: Int) {
    for frame in 1...frames {
        let time = UsdTimeCode(Double(frame))
        let radius = 1.0 + sin(Double(frame) * 0.1)

        for sphere in spheres {
            sphere.getRadiusAttr().set(radius, time: time)
        }
    }
}
```

**Swift 6.2 (With Span + MutableSpan)**:
```swift
func animateRadii(_ spheresSpan: Span<UsdGeom.Sphere>, frames: Int) {
    for frame in 1...frames {
        let time = UsdTimeCode(Double(frame))
        let radius = 1.0 + sin(Double(frame) * 0.1)

        for sphere in spheresSpan {
            sphere.getRadiusAttr().set(radius, time: time)
        }
    }
}

// Usage - pass view instead of copying array
let allSpheres = stage.getPrimsOfType(UsdGeom.Sphere.self)  // Returns Span
animateRadii(allSpheres, frames: 240)
```

**Benefits**:
- ✅ Pass-by-view semantics
- ✅ No array allocation
- ✅ Clear lifetime relationships

---

## Smart Pointer Improvements

### Example 11: Stage Pointer Access

**Swift 6.0/6.1 (Current - Known Issue)**:
```swift
func processStage(_ stagePtr: UsdStageRefPtr) {
    // WARNING: This copies the entire composed stage!
    let stageCopy = stagePtr.pointee

    // Workaround: Access through pointer
    stagePtr.pointee.Save()
    let prim = stagePtr.pointee.GetPrimAtPath(...)
}
```

**Swift 6.2 (Potential Improvement)**:
```cpp
// Annotated USD header
class TfRefPtr<T> {
public:
    T& operator*() const __lifetimebound;
    T* operator->() const __lifetimebound;
};
```

```swift
func processStage(_ stagePtr: UsdStageRefPtr) {
    // With __lifetimebound, may borrow instead of copy
    let stage = stagePtr.pointee  // Potentially safe borrow

    stage.save()
    let prim = stage.getPrimAtPath(...)
}
```

**Status**: Requires testing with Swift 6.2 to confirm behavior improvement

### Example 12: Layer Stack Access

**Swift 6.0/6.1 (Current)**:
```swift
func inspectLayerStack(_ stage: UsdStageRefPtr) {
    let rootLayer = stage.getRootLayer()  // Returns SdfLayerRefPtr

    // Access layer data
    let identifier = rootLayer.pointee.GetIdentifier()
    let subLayers = rootLayer.pointee.GetSubLayerPaths()

    print("Root: \(identifier)")
    print("Sublayers: \(subLayers.size())")
}
```

**Swift 6.2 (With Span for Sublayers)**:
```swift
func inspectLayerStack(_ stage: UsdStageRefPtr) {
    let rootLayer = stage.getRootLayer()

    let identifier = rootLayer.pointee.GetIdentifier()
    let subLayers = rootLayer.getSubLayersSpan()  // Span<String>

    print("Root: \(identifier)")
    print("Sublayers:")
    for (index, path) in subLayers.enumerated() {
        print("  [\(index)]: \(path)")
    }
}
```

**Benefits**:
- ✅ Direct view into sublayer list
- ✅ No vector copy overhead

---

## Performance Comparisons

### Example 13: Large Prim Traversal

**Benchmark Setup**:
```swift
let stage = Usd.Stage.open("large_scene.usdc")  // 100,000+ prims

func benchmark_current() {
    for prim in Usd.PrimRange(stage.getPseudoRoot()) {
        _ = prim.getName()
    }
}

func benchmark_span() {
    let prims = stage.getAllPrims()  // Span<UsdPrim>
    for prim in prims {
        _ = prim.getName()
    }
}
```

**Expected Results** (hypothetical, needs real measurements):
```
Current (Iterator):     1250ms
Span (Direct View):     1245ms  (within measurement error)
Memory (Current):       ~50MB
Memory (Span):          ~50MB   (no additional allocation)
```

**Analysis**: Similar performance, but Span provides compile-time safety with zero overhead.

### Example 14: Matrix Operations

**Benchmark Setup**:
```swift
func benchmark_gfMatrix() {
    var total = 0.0
    for _ in 0..<10000 {
        let m1 = Pixar.GfMatrix4d(1.0)
        let m2 = Pixar.GfMatrix4d(2.0)
        let m3 = m1 * m2
        total += m3.Get(0, 0)
    }
}

func benchmark_inlineArray() {
    var total = 0.0
    for _ in 0..<10000 {
        let m1 = Matrix4d.identity
        let m2 = Matrix4d.identity
        // Manual matrix multiply (not shown)
        total += m1[0]
    }
}
```

**Expected Results** (hypothetical):
```
GfMatrix4d:         450ms
InlineArray:        445ms  (similar, potentially SIMD optimizable)
```

**Analysis**: InlineArray provides Swift-native types with same performance profile.

### Example 15: Attribute Read Performance

**Benchmark Setup**:
```swift
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")
let radiusAttr = sphere.getRadiusAttr()

// Set up time samples
for frame in 1...240 {
    radiusAttr.set(Double(frame) * 0.1, time: UsdTimeCode(Double(frame)))
}

func benchmark_current() {
    var total = 0.0
    for frame in 1...240 {
        var radius: Double = 0
        radiusAttr.get(&radius, time: UsdTimeCode(Double(frame)))
        total += radius
    }
}

// With potential Span-based API
func benchmark_span() {
    let samples = radiusAttr.getTimeSamplesSpan()  // Span<Double>
    let total = samples.reduce(0.0, +)
}
```

**Analysis**: Span-based API could eliminate per-sample overhead for bulk operations.

---

## Summary of Benefits

### Span/MutableSpan
- Zero-cost abstraction over pointers
- Compile-time lifetime safety
- No heap allocation
- Automatic bounds checking in debug mode

### InlineArray
- Stack-allocated fixed-size arrays
- C-compatible memory layout
- Native Swift type
- SIMD optimization potential

### Lifetime Annotations
- Prevents dangling references at compile time
- No runtime overhead
- Explicit lifetime relationships
- Better API documentation

### Overall Impact on SwiftUSD
- Safer USD API surface
- Same or better performance
- More idiomatic Swift code
- Progressive adoption path

---

**Document Version**: 1.0
**Last Updated**: 2025-01-12
**Swift Version**: 6.2
**SwiftUSD Version**: 24.8.14

**See Also**:
- [SWIFT_6.2_MIGRATION.md](./SWIFT_6.2_MIGRATION.md) - Complete migration guide
- [Documentation.md](./Documentation.md) - SwiftUSD usage guide
