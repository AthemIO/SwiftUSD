# SwiftUSD Documentation

Complete guide for using SwiftUSD - Swift bindings for Pixar's Universal Scene Description (USD) library.

## Table of Contents

1. [Overview](#overview)
2. [Installation](#installation)
3. [Getting Started](#getting-started)
4. [Core Concepts](#core-concepts)
5. [API Usage](#api-usage)
6. [Rendering with Hydra](#rendering-with-hydra)
7. [Cross-Platform Development](#cross-platform-development)
8. [Advanced Topics](#advanced-topics)
9. [Examples](#examples)
10. [Troubleshooting](#troubleshooting)

---

## Overview

SwiftUSD provides production-grade Swift bindings for Pixar's Universal Scene Description (USD) library using Swift/C++ interop. This enables you to work with USD scenes, geometry, materials, and rendering directly from Swift code.

**Key Features:**
- Direct C++ interop (no Objective-C bridging)
- Full USD API coverage
- Native Metal rendering via Hydra
- Cross-platform support (macOS, iOS, visionOS, tvOS, watchOS, Linux, Windows)
- Ergonomic Swift DSL for scene construction
- Swift 6 strict concurrency support

**Supported Platforms:**
- macOS 14+
- iOS 17+
- visionOS 1+
- tvOS 17+
- watchOS 10+
- Linux (Ubuntu 20.04+)
- Windows 10+

---

## Installation

### Adding SwiftUSD to Your Project

Add SwiftUSD as a dependency in your `Package.swift`:

```swift
// swift-tools-version: 6.0
import PackageDescription

let package = Package(
  name: "MyProject",
  platforms: [
    .macOS(.v14),
    .iOS(.v17),
    .visionOS(.v1),
    .tvOS(.v17),
    .watchOS(.v10)
  ],
  dependencies: [
    .package(url: "https://github.com/wabiverse/SwiftUSD.git", from: "24.8.14")
  ],
  targets: [
    .target(
      name: "MyTarget",
      dependencies: [
        .product(name: "PixarUSD", package: "SwiftUSD")
      ],
      swiftSettings: [
        .interoperabilityMode(.Cxx)  // REQUIRED for C++ interop
      ]
    )
  ]
)
```

### Platform-Specific Requirements

#### Windows
Add these C++ defines to your target:

```swift
cxxSettings: [
  .define("_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH", .when(platforms: [.windows])),
  .define("_ALLOW_KEYWORD_MACROS", to: "1", .when(platforms: [.windows])),
  .define("static_assert(_conditional, ...)", to: "", .when(platforms: [.windows])),
]
```

#### Linux
Install required dependencies:

```bash
sudo apt install libstdc++-12-dev libdeflate-dev libbz2-dev zlib1g-dev \
                 python3-dev freeglut3-dev libboost-all-dev \
                 libxcursor-dev libxt-dev libxi-dev libxinerama-dev \
                 libxrandr-dev libgtk-4-dev clang
```

---

## Getting Started

### Your First USD Scene

```swift
import PixarUSD

// CRITICAL: Always setup resources first!
Pixar.Bundler.shared.setup(.resources)

// Create a new stage
let stage = Usd.Stage.createNew("MyScene.usda")

// Define a transform (group)
let xform = UsdGeom.Xform.define(stage, path: "/World")

// Define a sphere
let sphere = UsdGeom.Sphere.define(stage, path: "/World/Sphere")
sphere.getRadiusAttr().set(2.0)

// Set sphere color
let displayColor = sphere.getDisplayColorAttr()
displayColor.set(Vt.Vec3fArray(1, [Pixar.GfVec3f(1.0, 0.0, 0.0)]))  // Red

// Save to disk
stage.save()

print("Created scene at: MyScene.usda")
```

### Opening Existing Files

```swift
// Open a USD file
let stage = Usd.Stage.open("/path/to/scene.usd")

// Traverse all prims
let rootPrim = stage.getPseudoRoot()
for prim in Usd.PrimRange(rootPrim) {
  print("Prim: \(prim.getName())")
}
```

### Using the Declarative Swift DSL

SwiftUSD provides a SwiftUI-like DSL for scene construction:

```swift
import PixarUSD

// Setup resources
Pixar.Bundler.shared.setup(.resources)

// Create scene with declarative syntax
let stage = USDStage("MyScene", ext: .usda) {
  USDPrim("World", type: .xform) {
    USDPrim("Camera", type: .camera) {
      Attribute("focalLength", value: 50.0)
    }

    USDPrim("RedSphere", type: .sphere) {
      Attribute("radius", value: 2.0)
      Attribute("displayColor", value: Pixar.GfVec3f(1, 0, 0))
    }

    USDPrim("BlueCube", type: .cube) {
      Attribute("size", value: 1.5)
      Attribute("displayColor", value: Pixar.GfVec3f(0, 0, 1))
    }
  }
}
.set(doc: "Example scene with camera and geometry")
.save()
```

---

## Core Concepts

### USD Stage

The `UsdStage` is the primary entry point and owns the entire scene graph. It represents the composed result of all layers and provides the main interface for querying and authoring scene description.

```swift
// Create new stage (ASCII format)
let stage = Usd.Stage.createNew("scene.usda")

// Create binary crate stage (faster)
let binaryStage = Usd.Stage.createNew("scene.usdc")

// Open existing stage
let stage = Usd.Stage.open("scene.usd")

// Create in-memory stage (no file backing)
let memoryStage = Usd.Stage.createInMemory()

// Open with specific layer
let layer = Sdf.Layer.findOrOpen("scene.usda")
let stage = Usd.Stage.open(layer)

// Get root layer (the authoring target)
let rootLayer = stage.getRootLayer()

// Get session layer (for temporary overrides)
let sessionLayer = stage.getSessionLayer()

// Get default prim (entry point for asset)
let defaultPrim = stage.getDefaultPrim()

// Set default prim
stage.setDefaultPrim(stage.getPrimAtPath(Sdf.Path("/World")))

// Get pseudo-root (parent of all root prims)
let pseudoRoot = stage.getPseudoRoot()

// Check if stage is valid
if stage.pointee.isValid() {
  print("Stage is valid")
}

// Get stage metadata
print("Up axis: \(Pixar.UsdGeomGetStageUpAxis(stage.pointee.getPtr()))")
print("Meters per unit: \(Pixar.UsdGeomGetStageMetersPerUnit(stage.pointee.getPtr()))")

// Set stage metadata
Pixar.UsdGeomSetStageUpAxis(stage, .y)  // Y-up
Pixar.UsdGeomSetStageMetersPerUnit(stage, 0.01)  // Centimeters

// Time-related metadata
stage.setStartTimeCode(1.0)
stage.setEndTimeCode(240.0)
stage.setFramesPerSecond(24.0)
stage.setTimeCodesPerSecond(24.0)

// Get time range
if stage.pointee.HasAuthoredTimeCodeRange() {
  let start = stage.pointee.GetStartTimeCode()
  let end = stage.pointee.GetEndTimeCode()
  print("Frame range: \(start) to \(end)")
}

// Save stage
stage.save()

// Export to different format
stage.export("output.usdc")  // Binary format
stage.export("output.usda")  // ASCII format
```

### Sdf.Path - USD Path System

`Sdf.Path` is USD's efficient path representation for addressing prims, attributes, and relationships.

```swift
// Create prim paths
let worldPath = Sdf.Path("/World")
let spherePath = Sdf.Path("/World/Sphere")

// Absolute root path (/)
let rootPath = Sdf.Path.absoluteRootPath()

// Empty path
let emptyPath = Sdf.Path.emptyPath()

// Check path properties
print("Is absolute: \(worldPath.isAbsolutePath())")
print("Is prim path: \(worldPath.isPrimPath())")
print("Is property path: \(spherePath.isPropertyPath())")

// Path manipulation
let parentPath = spherePath.getParentPath()  // "/World"
let primName = spherePath.getName()  // "Sphere"

// Append child path
let childPath = worldPath.appendChild(Pixar.TfToken("Sphere"))  // "/World/Sphere"

// Create attribute path
let radiusPath = spherePath.appendProperty(Pixar.TfToken("radius"))  // "/World/Sphere.radius"

// Create relationship path
let materialPath = spherePath.appendProperty(Pixar.TfToken("material:binding"))

// Path prefix checking
let hasPrefix = spherePath.hasPrefix(worldPath)  // true

// Common ancestor
let ancestor = Sdf.Path.findLongestPrefix(spherePath, Sdf.Path("/World/Cube"))

// Make relative path
let relativePath = spherePath.makeRelativePath(worldPath)  // "Sphere"
```

### Prims (Primitives)

Prims are the fundamental building blocks of USD scenes. They form a hierarchical namespace and can have types, attributes, relationships, and metadata.

```swift
// Get a prim by path
let prim = stage.getPrimAtPath(Sdf.Path("/World/Sphere"))

// Check if prim exists and is valid
if prim.isValid() {
  print("Prim is valid")
}

if prim.isDefined() {
  print("Prim has authored scene description")
}

// Get prim properties
print("Prim name: \(prim.getName())")  // "Sphere"
print("Prim path: \(prim.getPath())")  // "/World/Sphere"
print("Prim type: \(prim.getTypeName())")  // "Sphere"

// Check prim type
if prim.isA(UsdGeom.Sphere.self) {
  print("Prim is a Sphere")
}

// Type checking with multiple types
let isGeometry = prim.isA(UsdGeom.Gprim.self)  // True for all geometry prims
let isXformable = prim.isA(UsdGeom.Xformable.self)  // True for transformable prims

// Create new prims
let world = stage.definePrim(Sdf.Path("/World"))  // Generic prim
let sphere = stage.definePrim(Sdf.Path("/World/Sphere"), Pixar.TfToken("Sphere"))  // Typed prim

// Override existing prim (create if doesn't exist)
let overridePrim = stage.overridePrim(Sdf.Path("/World/Sphere"))

// Get prim specifier
let specifier = prim.getSpecifier()  // .def, .over, or .class

// Prim hierarchy navigation
let parent = prim.getParent()
let children = prim.getChildren()
let allChildren = prim.getAllChildren()  // Includes inactive children

for child in children {
  print("Child: \(child.getName())")
}

// Filtered children
let filterFunc: (Usd.Prim) -> Bool = { $0.isA(UsdGeom.Gprim.self) }
let geometryChildren = prim.getFilteredChildren(Usd.PrimDefaultPredicate, filterFunc)

// Get sibling
let nextSibling = prim.getNextSibling()

// Prim activation (visibility in composition)
prim.setActive(true)
let isActive = prim.isActive()

// Prim loading (for payloads)
if prim.isLoaded() {
  print("Prim payload is loaded")
}

// Load/unload prim
stage.load(prim.getPath())
stage.unload(prim.getPath())

// Prim metadata
prim.setMetadata(Pixar.TfToken("comment"), Pixar.VtValue("This is a sphere"))
var comment = Pixar.VtValue()
if prim.getMetadata(Pixar.TfToken("comment"), &comment) {
  print("Comment: \(comment)")
}

// Documentation metadata
prim.setDocumentation("This sphere represents the player character")
var doc = ""
prim.getDocumentation(&doc)

// Hidden flag (for tools, not rendering)
prim.setHidden(true)
let isHidden = prim.isHidden()

// Instance proxies (for instancing)
let isInstance = prim.isInstance()
if isInstance {
  let prototype = prim.getPrototype()
  print("Instance of: \(prototype.getPath())")
}

// Clear prim (remove all scene description)
// prim.clear()  // Uncomment to use - destructive!
```

### Prim Traversal Patterns in Swift

```swift
// Basic range traversal (depth-first)
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
  print("Visiting: \(prim.getPath())")
}

// Traversal with predicate (skip inactive prims)
let range = Usd.PrimRange.makeStage(stage, Usd.PrimDefaultPredicate)
for prim in range {
  print("Active prim: \(prim.getPath())")
}

// Custom traversal with Swift closures
func traverseWithFilter(_ root: Usd.Prim, filter: (Usd.Prim) -> Bool) {
  if filter(root) {
    print("Matched: \(root.getPath())")
  }

  for child in root.getChildren() {
    traverseWithFilter(child, filter: filter)
  }
}

// Find all geometry prims
traverseWithFilter(stage.getPseudoRoot()) { prim in
  prim.isA(UsdGeom.Gprim.self)
}

// Find prims by name pattern
traverseWithFilter(stage.getPseudoRoot()) { prim in
  prim.getName().contains("Sphere")
}

// Breadth-first traversal
func traverseBreadthFirst(root: Usd.Prim, visit: (Usd.Prim) -> Void) {
  var queue: [Usd.Prim] = [root]

  while !queue.isEmpty {
    let prim = queue.removeFirst()
    visit(prim)
    queue.append(contentsOf: prim.getChildren())
  }
}

traverseBreadthFirst(root: stage.getPseudoRoot()) { prim in
  print("Level-order: \(prim.getPath())")
}

// Pruning traversal (skip subtrees)
var range = Usd.PrimRange(stage.getPseudoRoot())
while !range.isEmpty() {
  let prim = range.front()

  // Skip this subtree if it's a reference
  if prim.hasAuthoredReferences() {
    range.pruneChildren()
  }

  print("Visiting: \(prim.getPath())")
  range.increment()
}
```

### Attributes

Attributes store time-sampled or default data values on prims.

```swift
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")

// Get built-in attribute from schema
let radiusAttr = sphere.getRadiusAttr()

// Create custom attribute
let customAttr = prim.createAttribute(
  Pixar.TfToken("custom:temperature"),
  .double,
  false  // custom attribute
)

// Create custom attribute with more control
let velocityAttr = prim.createAttribute(
  Pixar.TfToken("custom:velocity"),
  .vector3d,
  false,
  .varying  // interpolation
)

// Set attribute value (default time)
radiusAttr.set(5.0)

// Set typed values
let colorAttr = sphere.getDisplayColorAttr()
colorAttr.set(Vt.Vec3fArray(1, [Pixar.GfVec3f(1.0, 0.0, 0.0)]))

// Set value at specific time (animation)
radiusAttr.set(3.0, time: UsdTimeCode(1.0))
radiusAttr.set(5.0, time: UsdTimeCode(12.0))
radiusAttr.set(8.0, time: UsdTimeCode(24.0))

// Get attribute value
var radius: Double = 0
if radiusAttr.get(&radius) {
  print("Radius: \(radius)")
}

// Get value at specific time
radiusAttr.get(&radius, time: UsdTimeCode(12.0))

// Get time samples
var times = [Double]()
radiusAttr.getTimeSamples(&times)
print("Time samples: \(times)")

// Get time sample count
let numSamples = radiusAttr.getNumTimeSamples()

// Check if attribute is time-varying
if radiusAttr.valueM ightBeTimeVarying() {
  print("Attribute has animation")
}

// Get value type
let valueType = radiusAttr.getTypeName()
print("Type: \(valueType)")

// Attribute metadata
radiusAttr.setMetadata(Pixar.TfToken("units"), Pixar.VtValue("meters"))

// Set attribute documentation
radiusAttr.setDocumentation("The radius of the sphere in meters")

// Connection sources (for shading networks)
radiusAttr.addConnection(Sdf.Path("/Materials/Shader.outputs:radius"))
let sources = radiusAttr.getConnections()

// Clear attribute value
radiusAttr.clear()

// Clear value at specific time
radiusAttr.clearAtTime(UsdTimeCode(12.0))

// Block attribute (prevent inheritance)
radiusAttr.block()

// Check if attribute is authored
if radiusAttr.hasAuthoredValue() {
  print("Attribute has explicit value")
}

// Get all attributes on a prim
for attr in prim.getAttributes() {
  print("Attribute: \(attr.getName())")
}

// Get attributes matching pattern
let displayAttrs = prim.getAttributes { name in
  name.hasPrefix("display")
}
```

### Attribute Value Types in Swift

```swift
// Scalar types
let doubleAttr = prim.createAttribute(Pixar.TfToken("myDouble"), .double, false)
doubleAttr.set(3.14159)

let floatAttr = prim.createAttribute(Pixar.TfToken("myFloat"), .float, false)
floatAttr.set(Float(2.718))

let intAttr = prim.createAttribute(Pixar.TfToken("myInt"), .int, false)
intAttr.set(Int32(42))

let boolAttr = prim.createAttribute(Pixar.TfToken("myBool"), .bool, false)
boolAttr.set(true)

let stringAttr = prim.createAttribute(Pixar.TfToken("myString"), .string, false)
stringAttr.set("Hello USD")

let tokenAttr = prim.createAttribute(Pixar.TfToken("myToken"), .token, false)
tokenAttr.set(Pixar.TfToken("constant"))

// Vector types
let vec2fAttr = prim.createAttribute(Pixar.TfToken("myVec2f"), .float2, false)
vec2fAttr.set(Pixar.GfVec2f(1.0, 2.0))

let vec3fAttr = prim.createAttribute(Pixar.TfToken("myVec3f"), .float3, false)
vec3fAttr.set(Pixar.GfVec3f(1.0, 2.0, 3.0))

let vec3dAttr = prim.createAttribute(Pixar.TfToken("myVec3d"), .double3, false)
vec3dAttr.set(Pixar.GfVec3d(1.0, 2.0, 3.0))

let color3fAttr = prim.createAttribute(Pixar.TfToken("myColor"), .color3f, false)
color3fAttr.set(Pixar.GfVec3f(1.0, 0.5, 0.0))

// Matrix types
let matrix4dAttr = prim.createAttribute(Pixar.TfToken("myMatrix"), .matrix4d, false)
var identity = Pixar.GfMatrix4d(1.0)
matrix4dAttr.set(identity)

// Array types
let intArrayAttr = prim.createAttribute(Pixar.TfToken("myIntArray"), .intArray, false)
let intArray = Vt.IntArray(3, [1, 2, 3])
intArrayAttr.set(intArray)

let vec3fArrayAttr = prim.createAttribute(Pixar.TfToken("myVec3fArray"), .float3Array, false)
let vec3fArray = Vt.Vec3fArray(2, [
  Pixar.GfVec3f(1, 0, 0),
  Pixar.GfVec3f(0, 1, 0)
])
vec3fArrayAttr.set(vec3fArray)

// Asset path
let assetAttr = prim.createAttribute(Pixar.TfToken("myAsset"), .asset, false)
assetAttr.set(Sdf.AssetPath("textures/diffuse.png"))
```

### Relationships

Relationships create connections between prims without storing data values.

```swift
// Create relationship
let materialRel = prim.createRelationship(Pixar.TfToken("material:binding"), false)

// Add target
materialRel.addTarget(Sdf.Path("/Materials/RedMaterial"))

// Add multiple targets
materialRel.addTarget(Sdf.Path("/Materials/BlueMaterial"))

// Set targets (replaces existing)
materialRel.setTargets([
  Sdf.Path("/Materials/RedMaterial"),
  Sdf.Path("/Materials/BlueMaterial")
])

// Get targets
var targets: Sdf.PathVector = .init()
materialRel.getTargets(&targets)
for i in 0..<targets.size() {
  print("Target: \(targets[i])")
}

// Get forwarded targets (follows relationships on target prims)
var forwardedTargets: Sdf.PathVector = .init()
materialRel.getForwardedTargets(&forwardedTargets)

// Check if target exists
let hasTarget = materialRel.hasAuthoredTargets()

// Remove target
materialRel.removeTarget(Sdf.Path("/Materials/BlueMaterial"))

// Clear all targets
materialRel.clearTargets(true)  // remove all targets

// Block relationship (prevent inheritance)
materialRel.blockTargets()

// Get all relationships on prim
for rel in prim.getRelationships() {
  print("Relationship: \(rel.getName())")
}
```

### Tokens - High-Performance String System

USD uses `TfToken` for string interning, providing O(1) comparison and minimal memory overhead.

```swift
// Create token from string
let myToken = Pixar.TfToken("myIdentifier")

// Access predefined schema tokens
let defaultToken = UsdGeom.Tokens.default_
let renderToken = UsdGeom.Tokens.render
let proxyToken = UsdGeom.Tokens.proxy
let guideToken = UsdGeom.Tokens.guide

// Geometry tokens
let catmullClark = UsdGeom.Tokens.catmullClark
let subdivisionScheme = UsdGeom.Tokens.subdivisionScheme
let points = UsdGeom.Tokens.points
let normals = UsdGeom.Tokens.normals

// Transform tokens
let xformOpTranslate = UsdGeom.Tokens.xformOpTranslate
let xformOpRotateXYZ = UsdGeom.Tokens.xformOpRotateXYZ
let xformOpScale = UsdGeom.Tokens.xformOpScale

// Shading tokens (UsdShade)
let surface = UsdShade.Tokens.surface
let displacement = UsdShade.Tokens.displacement
let volume = UsdShade.Tokens.volume

// Compare tokens (pointer comparison - very fast!)
if myToken == defaultToken {
  print("Tokens are equal")
}

// Get token string
let tokenString = String(myToken.GetString())
print("Token as string: \(tokenString)")

// Check if token is empty
if myToken.isEmpty() {
  print("Token is empty")
}

// Token hash (for dictionaries)
let hash = myToken.hash()

// Creating tokens for custom purposes
let customToken = Pixar.TfToken("custom:myAttribute")
let namespacedToken = Pixar.TfToken("myNamespace:myProperty")
```

### Layers and Composition

USD's composition system combines multiple layers using a set of composition arcs. This enables non-destructive editing and powerful asset workflows.

```swift
// ==================== Layers ====================

// Create new layer (ASCII)
let layer = Sdf.Layer.createNew("scene.usda")

// Create new layer (binary)
let binaryLayer = Sdf.Layer.createNew("scene.usdc")

// Create anonymous (in-memory) layer
let anonLayer = Sdf.Layer.createAnonymous()
let namedAnonLayer = Sdf.Layer.createAnonymous("TempLayer")

// Find or open layer (caches layers)
let existingLayer = Sdf.Layer.findOrOpen("asset.usda")

// Open layer (bypasses cache)
let freshLayer = Sdf.Layer.openAsAnonymous("asset.usda")

// Get layer properties
print("Layer identifier: \(layer.pointee.GetIdentifier())")
print("Layer display name: \(layer.pointee.GetDisplayName())")
print("Is anonymous: \(layer.pointee.IsAnonymous())")
print("Is dirty: \(layer.pointee.IsDirty())")

// Save layer
layer.pointee.Save()

// Export layer to different format
layer.pointee.Export("output.usdc")

// Clear layer
layer.pointee.Clear()

// Layer metadata
layer.pointee.SetDocumentation("This layer contains...")
let comment = layer.pointee.GetComment()

// Set custom layer data
var customData = Pixar.VtDictionary()
customData["pipeline:version"] = Pixar.VtValue("1.0")
layer.pointee.SetCustomLayerData(customData)

// ==================== Sublayers ====================

// Create stage with root layer
let rootLayer = Sdf.Layer.createNew("root.usda")
let stage = Usd.Stage.open(rootLayer)

// Create sublayers
let layoutLayer = Sdf.Layer.createNew("layout.usda")
let animLayer = Sdf.Layer.createNew("anim.usda")
let lightingLayer = Sdf.Layer.createNew("lighting.usda")

// Add sublayers (stronger layers first)
let subLayerPaths = rootLayer.pointee.GetSubLayerPaths()
subLayerPaths.push_back(lightingLayer.pointee.GetIdentifier())
subLayerPaths.push_back(animLayer.pointee.GetIdentifier())
subLayerPaths.push_back(layoutLayer.pointee.GetIdentifier())

// Insert sublayer at specific position
subLayerPaths.insert(subLayerPaths.begin(), rootLayer.pointee.GetIdentifier())

// Remove sublayer
subLayerPaths.erase(subLayerPaths.begin())

// Get sublayer offsets (time offsets/scales)
let offsets = rootLayer.pointee.GetSubLayerOffsets()

// Set sublayer offset
var offset = Pixar.SdfLayerOffset(offset: 10.0, scale: 1.0)
rootLayer.pointee.SetSubLayerOffset(offset, index: 0)

// ==================== References ====================

// Add reference to external file
let prim = stage.definePrim(Sdf.Path("/AssetA"))
prim.getReferences().addReference("assets/table.usd")

// Add reference to specific prim in file
prim.getReferences().addReference("assets/table.usd", Sdf.Path("/Table"))

// Add reference with layer offset
prim.getReferences().addReference(
  "assets/table.usd",
  Sdf.Path("/Table"),
  Sdf.LayerOffset(offset: 0.0, scale: 1.0)
)

// Add internal reference (within same stage)
prim.getReferences().addInternalReference(Sdf.Path("/Prototypes/Table"))

// Remove reference
prim.getReferences().removeReference("assets/table.usd")

// Clear all references
prim.getReferences().clearReferences()

// Check for references
if prim.hasAuthoredReferences() {
  print("Prim has references")
}

// ==================== Payloads ====================

// Payloads are like references but lazy-loaded for scalability
let heavyPrim = stage.definePrim(Sdf.Path("/HeavyAsset"))

// Add payload
heavyPrim.getPayloads().addPayload("assets/heavy_model.usd")

// Add payload to specific prim
heavyPrim.getPayloads().addPayload("assets/heavy_model.usd", Sdf.Path("/Model"))

// Remove payload
heavyPrim.getPayloads().removePayload("assets/heavy_model.usd")

// Clear payloads
heavyPrim.getPayloads().clearPayloads()

// Load/unload payloads at stage level
stage.load(Sdf.Path("/HeavyAsset"))  // Load payload
stage.unload(Sdf.Path("/HeavyAsset"))  // Unload payload

// Load with descendants
let loadSet = stage.getLoadSet()
stage.loadAndUnload(Sdf.PathSet([Sdf.Path("/HeavyAsset")]), Sdf.PathSet())

// ==================== Inherits ====================

// Class-based inheritance
let classPath = Sdf.Path("/_class_Table")
let classPrim = stage.createClassPrim(classPath)

// Set properties on class
let classTable = UsdGeom.Mesh(classPrim)
// ... define table geometry ...

// Inherit from class
let instance = stage.definePrim(Sdf.Path("/Tables/DiningTable"))
instance.getInherits().addInherit(classPath)

// ==================== Specializes ====================

// Less common, but useful for overriding strong opinions
let basePrim = stage.definePrim(Sdf.Path("/BaseAsset"))
let specializedPrim = stage.definePrim(Sdf.Path("/SpecializedAsset"))
specializedPrim.getSpecializes().addSpecialize(basePrim.getPath())

// ==================== Variants ====================

// Variants provide switchable alternatives
let assetPrim = stage.definePrim(Sdf.Path("/Asset"))

// Create variant set
let variantSet = assetPrim.getVariantSets().addVariantSet("lodVariant")

// Add variant options
variantSet.addVariant("high")
variantSet.addVariant("medium")
variantSet.addVariant("low")

// Edit "high" variant
variantSet.setVariantSelection("high")
let editTarget = stage.getEditTarget()
let variantEditTarget = editTarget.forLocalDirectVariant(assetPrim, "lodVariant", "high")
stage.setEditTarget(variantEditTarget)

// Author content in "high" variant
let highDetailMesh = UsdGeom.Mesh.define(stage, path: "/Asset/Mesh")
highDetailMesh.getSubdivisionSchemeAttr().set(UsdGeom.Tokens.catmullClark.token)

// Reset edit target
stage.setEditTarget(Usd.EditTarget(stage.getRootLayer()))

// Edit "low" variant
variantSet.setVariantSelection("low")
let lowEditTarget = editTarget.forLocalDirectVariant(assetPrim, "lodVariant", "low")
stage.setEditTarget(lowEditTarget)

let lowDetailMesh = UsdGeom.Mesh.define(stage, path: "/Asset/Mesh")
lowDetailMesh.getSubdivisionSchemeAttr().set(UsdGeom.Tokens.none.token)

stage.setEditTarget(Usd.EditTarget(stage.getRootLayer()))

// Get variant selection
let selectedVariant = variantSet.getVariantSelection()
print("Selected variant: \(selectedVariant)")

// Get all variant names
let variantNames = variantSet.getVariantNames()

// Multiple variant sets
let shadingVariantSet = assetPrim.getVariantSets().addVariantSet("shadingVariant")
shadingVariantSet.addVariant("default")
shadingVariantSet.addVariant("weathered")
shadingVariantSet.addVariant("damaged")

// ==================== Edit Targets ====================

// Edit target controls which layer receives edits
let sessionTarget = Usd.EditTarget(stage.getSessionLayer())
stage.setEditTarget(sessionTarget)

// Now all edits go to session layer (temporary overrides)
let sphere = UsdGeom.Sphere.define(stage, path: "/TempSphere")

// Reset to root layer
stage.setEditTarget(Usd.EditTarget(stage.getRootLayer()))

// ==================== Composition Query ====================

// Get prim stack (all opinions contributing to prim)
let primStack = prim.getPrimStack()
for spec in primStack {
  print("Layer: \(spec.pointee.GetLayer().pointee.GetIdentifier())")
}

// Get property stack
let attr = prim.getAttribute(Pixar.TfToken("radius"))
let propertyStack = attr.getPropertyStack()

// Query composition arcs
let query = Usd.PrimCompositionQuery(prim)
let arcs = query.getCompositionArcs()
for arc in arcs {
  print("Arc type: \(arc.GetArcType())")
  print("Introduces: \(arc.GetIntroducingLayer().pointee.GetIdentifier())")
}
```

---

## API Usage

### Working with Geometry

#### Meshes

```swift
let mesh = UsdGeom.Mesh.define(stage, path: "/Mesh")

// Set topology
let faceVertexCounts = Vt.IntArray(2, [3, 3])  // Two triangles
mesh.getFaceVertexCountsAttr().set(faceVertexCounts)

let faceVertexIndices = Vt.IntArray(6, [0, 1, 2, 2, 3, 0])
mesh.getFaceVertexIndicesAttr().set(faceVertexIndices)

// Set points
let points = Vt.Vec3fArray(4, [
  Pixar.GfVec3f(0, 0, 0),
  Pixar.GfVec3f(1, 0, 0),
  Pixar.GfVec3f(1, 1, 0),
  Pixar.GfVec3f(0, 1, 0)
])
mesh.getPointsAttr().set(points)

// Set normals
let normals = Vt.Vec3fArray(4, [
  Pixar.GfVec3f(0, 0, 1),
  Pixar.GfVec3f(0, 0, 1),
  Pixar.GfVec3f(0, 0, 1),
  Pixar.GfVec3f(0, 0, 1)
])
mesh.getNormalsAttr().set(normals)

// Set subdivision scheme
mesh.getSubdivisionSchemeAttr().set(UsdGeom.Tokens.catmullClark.token)
```

#### Primitive Shapes

```swift
// Sphere
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")
sphere.getRadiusAttr().set(2.0)

// Cube
let cube = UsdGeom.Cube.define(stage, path: "/Cube")
cube.getSizeAttr().set(3.0)

// Cylinder
let cylinder = UsdGeom.Cylinder.define(stage, path: "/Cylinder")
cylinder.getRadiusAttr().set(1.0)
cylinder.getHeightAttr().set(5.0)

// Cone
let cone = UsdGeom.Cone.define(stage, path: "/Cone")
cone.getRadiusAttr().set(1.5)
cone.getHeightAttr().set(3.0)
```

### Transforms and Hierarchies

```swift
// Create transform hierarchy
let world = UsdGeom.Xform.define(stage, path: "/World")
let group = UsdGeom.Xform.define(stage, path: "/World/Group")
let sphere = UsdGeom.Sphere.define(stage, path: "/World/Group/Sphere")

// Set transform operations
let xformable = UsdGeom.Xformable(group.getPrim())

// Translation
let translateOp = xformable.addTranslateOp()
translateOp.set(Pixar.GfVec3d(5, 0, 0))

// Rotation (in degrees)
let rotateOp = xformable.addRotateXYZOp()
rotateOp.set(Pixar.GfVec3f(0, 45, 0))

// Scale
let scaleOp = xformable.addScaleOp()
scaleOp.set(Pixar.GfVec3f(2, 2, 2))

// Get computed transform matrix
var worldTransform = Pixar.GfMatrix4d()
xformable.getLocalTransformation(&worldTransform, time: UsdTimeCode.Default())
```

### Animation

```swift
let sphere = UsdGeom.Sphere.define(stage, path: "/AnimatedSphere")
let radiusAttr = sphere.getRadiusAttr()

// Animate radius over time
for frame in 1...24 {
  let time = UsdTimeCode(Double(frame))
  let radius = 1.0 + sin(Double(frame) * 0.1)
  radiusAttr.set(radius, time: time)
}

// Set time range
stage.setStartTimeCode(1.0)
stage.setEndTimeCode(24.0)
```

### Materials and Shading

```swift
// Create material
let material = UsdShade.Material.define(stage, path: "/Materials/RedMetal")

// Create surface shader
let shader = UsdShade.Shader.define(stage, path: "/Materials/RedMetal/Surface")
shader.createIdAttr().set(Pixar.TfToken("UsdPreviewSurface"))

// Set shader parameters
shader.createInput(Pixar.TfToken("diffuseColor"), .color3f)
  .set(Pixar.GfVec3f(0.8, 0.1, 0.1))

shader.createInput(Pixar.TfToken("metallic"), .float)
  .set(Float(1.0))

shader.createInput(Pixar.TfToken("roughness"), .float)
  .set(Float(0.3))

// Connect shader to material
material.createSurfaceOutput().connectToSource(
  shader.connectableAPI(),
  Pixar.TfToken("surface")
)

// Bind material to geometry
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")
UsdShade.MaterialBindingAPI.apply(sphere.getPrim()).bind(material)
```

### Cameras

```swift
let camera = UsdGeom.Camera.define(stage, path: "/Camera")

// Set camera properties
camera.getFocalLengthAttr().set(Float(50.0))
camera.getHorizontalApertureAttr().set(Float(24.0))
camera.getVerticalApertureAttr().set(Float(18.0))
camera.getClippingRangeAttr().set(Pixar.GfVec2f(0.1, 10000.0))

// Position camera
let xformable = UsdGeom.Xformable(camera.getPrim())
let translateOp = xformable.addTranslateOp()
translateOp.set(Pixar.GfVec3d(10, 5, 10))

// Look at target
let rotateOp = xformable.addRotateXYZOp()
rotateOp.set(Pixar.GfVec3f(-30, 45, 0))
```

### Lights

```swift
// Distant light (directional)
let distantLight = UsdLux.DistantLight.define(stage, path: "/Lights/Sun")
distantLight.getIntensityAttr().set(Float(1.0))
distantLight.getColorAttr().set(Pixar.GfVec3f(1.0, 0.95, 0.8))

// Sphere light (point light)
let sphereLight = UsdLux.SphereLight.define(stage, path: "/Lights/Point")
sphereLight.getIntensityAttr().set(Float(100.0))
sphereLight.getRadiusAttr().set(Float(0.5))

// Dome light (environment)
let domeLight = UsdLux.DomeLight.define(stage, path: "/Lights/Environment")
domeLight.getIntensityAttr().set(Float(0.5))
```

---

## Rendering with Hydra

### Setting Up the Render Engine

```swift
import PixarUSD

// Setup resources
Pixar.Bundler.shared.setup(.resources)

// Create or open stage
let stage = Usd.Stage.open("scene.usd")

// Create render engine
let engine = Hydra.RenderEngine(stage: stage)

// Create camera controller
let isZUp = Hydra.RenderEngine.isZUp(for: stage)
let cameraController = CameraController(isZUp: isZUp)
engine.setCameraController(cameraController)
```

### Rendering a Frame

```swift
// Render at specific time with given viewport size
let viewSize = CGSize(width: 1920, height: 1080)
let timeCode = 1.0

let texture = engine.render(at: timeCode, viewSize: viewSize)

// The texture is a Pixar.HgiTextureHandle that can be converted
// to MTLTexture on Metal platforms
```

### Camera Control

```swift
// Create camera controller
let camera = CameraController(
  eye: Pixar.GfVec3d(10, 5, 10),    // Camera position
  at: Pixar.GfVec3d(0, 0, 0),       // Look-at point
  up: Pixar.GfVec3d(0, 1, 0)        // Up vector
)

// Pan camera (move eye and look-at together)
camera.pan(delta: CGPoint(x: 10, y: -5))

// Orbit camera around look-at point
camera.orbit(delta: CGPoint(x: 45, y: 30))

// Zoom camera (move along view direction)
camera.zoom(delta: 2.0)

// Focus on specific point
camera.focus(on: Pixar.GfVec3d(5, 0, 0), distance: 10.0)

// Reset to default view
camera.reset()

// Get view matrix for rendering
let viewMatrix = camera.getViewMatrix()
```

### Metal Integration (macOS/iOS/visionOS)

```swift
#if canImport(Metal)
import Metal
import MetalKit

class MyRenderer: NSObject, MTKViewDelegate {
  let engine: Hydra.RenderEngine

  func draw(in view: MTKView) {
    let viewSize = view.drawableSize
    let hgiTexture = engine.render(at: 0.0, viewSize: viewSize)

    // Convert to Metal texture
    if let metalTexture = getMetalTexture(from: hgiTexture) {
      // Blit to view...
    }
  }

  func getMetalTexture(from hgiTexture: Pixar.HgiTextureHandle) -> MTLTexture? {
    guard let hgiTex = hgiTexture.Get() else { return nil }
    let rawPtr = UnsafeRawPointer(hgiTex)
    let texPtr: Pixar.HgiMetalTexture = Unmanaged.fromOpaque(rawPtr).takeUnretainedValue()
    return texPtr.GetTextureId()
  }
}
#endif
```

---

## Cross-Platform Development

### Platform Detection

```swift
#if os(macOS)
  // macOS-specific code
  import AppKit
#elseif os(iOS) || os(visionOS)
  // iOS/visionOS-specific code
  import UIKit
#elseif os(Linux)
  // Linux-specific code
#elseif os(Windows)
  // Windows-specific code
#endif
```

### SwiftCrossUI Integration

SwiftUSD works with SwiftCrossUI for cross-platform UI:

```swift
import SwiftCrossUI
import PixarUSD

@main
struct MyApp: App {
  typealias Backend = PlatformBackend

  let stage: UsdStageRefPtr
  let engine: Hydra.RenderEngine

  init() {
    Pixar.Bundler.shared.setup(.resources)
    stage = Usd.Stage.createNew("scene.usda")
    engine = Hydra.RenderEngine(stage: stage)
  }

  var body: some Scene {
    WindowGroup("USD Viewer") {
      ContentView(engine: engine, stage: stage)
    }
  }
}
```

### File Formats

```swift
// ASCII format (human-readable, larger file size)
stage.export("scene.usda")

// Binary crate format (compact, faster loading)
stage.export("scene.usdc")

// Package format (directory with multiple files)
stage.export("scene.usdz")

// Detect format from extension
let layer = Sdf.Layer.createNew("scene.usd", args: Sdf.FileFormat.FileFormatArguments())
```

---

## Advanced Topics

### Custom Schema Definition

Create custom USD schemas for your domain-specific data:

```python
# schema.usda
class MyCustomPrim "MyCustomPrim" {
    double myProperty = 1.0
    string myString = "default"
}
```

Generate Swift bindings:

```bash
swift package plugin genschema
```

Use in Swift:

```swift
let customPrim = MyCustomPrim.define(stage, path: "/Custom")
customPrim.getMyPropertyAttr().set(42.0)
```

### Performance Optimization

```swift
// Use binary format for large scenes
stage.export("scene.usdc")  // Much faster than .usda

// Use payloads for lazy loading
let prim = stage.definePrim(Sdf.Path("/HeavyAsset"))
prim.getPayloads().addPayload("asset.usd")

// Unload/load payloads dynamically
stage.unload(Sdf.Path("/HeavyAsset"))
stage.load(Sdf.Path("/HeavyAsset"))

// Use variants for asset variations
let prim = stage.definePrim(Sdf.Path("/Asset"))
let variantSet = prim.getVariantSets().addVariantSet("LOD")
variantSet.addVariant("high")
variantSet.addVariant("low")
variantSet.setVariantSelection("high")
```

### Multi-Threading

```swift
import Work

// Use USD's work dispatcher for parallelism
var results = [Int](repeating: 0, count: 1000)

WorkParallelForN(1000) { startIndex, endIndex in
  for i in startIndex..<endIndex {
    // Process item i
    results[Int(i)] = processItem(i)
  }
}
```

### Custom Render Delegates

```swift
// Register custom render delegate
let hgi = HgiMetal.createHgi()
let driver = HdDriver(name: .renderDriver, driver: hgi.value)

let engine = UsdImagingGL.Engine.createEngine(
  rootPath: stage.getPseudoRoot().getPath(),
  excludedPaths: Sdf.PathVector(),
  invisedPaths: Sdf.PathVector(),
  sceneDelegateId: Sdf.Path.absoluteRootPath(),
  driver: driver
)
```

### Scene Traversal Patterns

```swift
// Depth-first traversal
func traverseDepthFirst(prim: Usd.Prim) {
  print("Visiting: \(prim.getPath())")

  for child in prim.getChildren() {
    traverseDepthFirst(prim: child)
  }
}

// Breadth-first traversal
func traverseBreadthFirst(root: Usd.Prim) {
  var queue: [Usd.Prim] = [root]

  while !queue.isEmpty {
    let prim = queue.removeFirst()
    print("Visiting: \(prim.getPath())")

    queue.append(contentsOf: prim.getChildren())
  }
}

// Filtered traversal
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
  // Only process geometry
  if prim.isA(UsdGeom.Gprim.self) {
    let gprim = UsdGeom.Gprim(prim)
    print("Geometry: \(gprim.getPath())")
  }
}
```

---

## Examples

### Example 1: Create Scene with Multiple Objects

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let stage = Usd.Stage.createNew("multi_object_scene.usda")

// Create root transform
let world = UsdGeom.Xform.define(stage, path: "/World")

// Create sphere
let sphere = UsdGeom.Sphere.define(stage, path: "/World/Sphere")
sphere.getRadiusAttr().set(2.0)
sphere.getDisplayColorAttr().set(Vt.Vec3fArray(1, [Pixar.GfVec3f(1, 0, 0)]))

// Position sphere
let sphereXform = UsdGeom.Xformable(sphere.getPrim())
sphereXform.addTranslateOp().set(Pixar.GfVec3d(-3, 0, 0))

// Create cube
let cube = UsdGeom.Cube.define(stage, path: "/World/Cube")
cube.getSizeAttr().set(2.0)
cube.getDisplayColorAttr().set(Vt.Vec3fArray(1, [Pixar.GfVec3f(0, 1, 0)]))

// Create cylinder
let cylinder = UsdGeom.Cylinder.define(stage, path: "/World/Cylinder")
cylinder.getRadiusAttr().set(1.0)
cylinder.getHeightAttr().set(3.0)
cylinder.getDisplayColorAttr().set(Vt.Vec3fArray(1, [Pixar.GfVec3f(0, 0, 1)]))

let cylinderXform = UsdGeom.Xformable(cylinder.getPrim())
cylinderXform.addTranslateOp().set(Pixar.GfVec3d(3, 0, 0))

// Add camera
let camera = UsdGeom.Camera.define(stage, path: "/Camera")
camera.getFocalLengthAttr().set(Float(50.0))

let cameraXform = UsdGeom.Xformable(camera.getPrim())
cameraXform.addTranslateOp().set(Pixar.GfVec3d(0, 5, 15))
cameraXform.addRotateXYZOp().set(Pixar.GfVec3f(-15, 0, 0))

// Add light
let light = UsdLux.DistantLight.define(stage, path: "/Light")
light.getIntensityAttr().set(Float(1.5))

stage.save()
print("Scene created successfully!")
```

### Example 2: Load and Modify Existing Scene

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

// Open existing scene
let stage = Usd.Stage.open("scene.usd")

// Find all spheres and scale them
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
  if prim.isA(UsdGeom.Sphere.self) {
    let sphere = UsdGeom.Sphere(prim)

    // Double the radius
    var currentRadius: Double = 0
    sphere.getRadiusAttr().get(&currentRadius)
    sphere.getRadiusAttr().set(currentRadius * 2.0)

    print("Scaled sphere: \(sphere.getPath())")
  }
}

// Save modified scene
stage.save()
```

### Example 3: Animated Scene

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let stage = Usd.Stage.createNew("animated.usda")

// Set frame range
stage.setStartTimeCode(1.0)
stage.setEndTimeCode(120.0)
stage.setFramesPerSecond(24.0)

// Create animated sphere
let sphere = UsdGeom.Sphere.define(stage, path: "/AnimatedSphere")
sphere.getRadiusAttr().set(1.0)

let xformable = UsdGeom.Xformable(sphere.getPrim())
let translateOp = xformable.addTranslateOp()

// Animate position in a circle
for frame in 1...120 {
  let time = UsdTimeCode(Double(frame))
  let angle = Double(frame) * 0.05

  let x = cos(angle) * 5.0
  let z = sin(angle) * 5.0

  translateOp.set(Pixar.GfVec3d(x, 0, z), time: time)
}

stage.save()
```

### Example 4: Material and Shading Network

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let stage = Usd.Stage.createNew("shading_example.usda")

// Create material
let material = UsdShade.Material.define(stage, path: "/Materials/CheckerMaterial")

// Create texture coordinate reader
let texCoordReader = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/TexCoord")
texCoordReader.createIdAttr().set(Pixar.TfToken("UsdPrimvarReader_float2"))
texCoordReader.createInput(Pixar.TfToken("varname"), .token)
  .set(Pixar.TfToken("st"))

// Create checker texture
let checker = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/Checker")
checker.createIdAttr().set(Pixar.TfToken("UsdUVTexture"))
checker.createInput(Pixar.TfToken("file"), .asset)
  .set(Sdf.AssetPath("checker.png"))

// Connect texture coordinates
checker.createInput(Pixar.TfToken("st"), .float2)
  .connectToSource(texCoordReader.connectableAPI(), Pixar.TfToken("result"))

// Create surface shader
let surface = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/Surface")
surface.createIdAttr().set(Pixar.TfToken("UsdPreviewSurface"))

// Connect checker to diffuse color
surface.createInput(Pixar.TfToken("diffuseColor"), .color3f)
  .connectToSource(checker.connectableAPI(), Pixar.TfToken("rgb"))

// Connect surface to material
material.createSurfaceOutput().connectToSource(
  surface.connectableAPI(),
  Pixar.TfToken("surface")
)

// Apply to geometry
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")
UsdShade.MaterialBindingAPI.apply(sphere.getPrim()).bind(material)

stage.save()
```

---

## Troubleshooting

### Common Issues

#### 1. "Failed to load plugin" Error

**Problem**: USD can't find plugins or resources.

**Solution**: Always call resource setup before any USD operations:

```swift
Pixar.Bundler.shared.setup(.resources)
```

#### 2. C++ Interop Errors

**Problem**: Compilation fails with C++ namespace errors.

**Solution**: Enable C++ interop in your target:

```swift
swiftSettings: [
  .interoperabilityMode(.Cxx)
]
```

#### 3. Empty or Invalid Stage

**Problem**: Stage operations fail or return nil.

**Solution**: Check if stage is valid:

```swift
let stage = Usd.Stage.open("scene.usd")
if !stage.isValid() {
  print("Failed to open stage")
  return
}
```

#### 4. Performance Issues with Large Scenes

**Problem**: Slow loading or rendering of large scenes.

**Solutions**:
- Use binary `.usdc` format instead of ASCII `.usda`
- Use payloads for lazy loading: `prim.getPayloads().addPayload("asset.usd")`
- Load only what you need: `stage.load(path)` / `stage.unload(path)`
- Use variants for LOD: Create high/low detail variants

#### 5. Memory Issues

**Problem**: High memory usage or crashes.

**Solution**: USD uses reference counting automatically. Ensure you don't create retain cycles:

```swift
// Use weak references for delegates
weak var cameraController: CameraController?

// Release large stages when done
var stage: UsdStageRefPtr? = Usd.Stage.open("huge.usd")
// ... use stage ...
stage = nil  // Release immediately
```

#### 6. Rendering Shows Nothing

**Problem**: Viewport is black or empty.

**Solutions**:
1. Check camera setup:
   ```swift
   let camera = CameraController(isZUp: true)
   engine.setCameraController(camera)
   ```

2. Verify stage has geometry:
   ```swift
   for prim in Usd.PrimRange(stage.getPseudoRoot()) {
     print("Prim: \(prim.getPath())")
   }
   ```

3. Check viewport size is valid:
   ```swift
   let viewSize = CGSize(width: max(1, width), height: max(1, height))
   ```

#### 7. SwiftCrossUI View Not Updating

**Problem**: Changes to USD stage don't reflect in UI.

**Solution**: SwiftCrossUI requires explicit view updates:

```swift
// Force redraw
view.setNeedsDisplay(view.bounds)

// Or recreate the view with updated state
```

#### 8. Windows Build Failures

**Problem**: Compilation errors on Windows.

**Solution**: Add required C++ defines:

```swift
cxxSettings: [
  .define("_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH", .when(platforms: [.windows])),
  .define("_ALLOW_KEYWORD_MACROS", to: "1", .when(platforms: [.windows])),
  .define("static_assert(_conditional, ...)", to: "", .when(platforms: [.windows])),
]
```

### Debug Tips

#### Enable USD Logging

```swift
// Set TF_DEBUG environment variable before running
// export TF_DEBUG="*"  # All debug output
// export TF_DEBUG="USD_STAGE,USD_PRIM"  # Specific subsystems
```

#### Inspect Stage Structure

```swift
// Print entire stage hierarchy
func printStage(_ stage: UsdStageRefPtr) {
  func printPrim(_ prim: Usd.Prim, indent: Int = 0) {
    let prefix = String(repeating: "  ", count: indent)
    print("\(prefix)\(prim.getPath()) [\(prim.getTypeName())]")

    for child in prim.getChildren() {
      printPrim(child, indent: indent + 1)
    }
  }

  printPrim(stage.getPseudoRoot())
}
```

#### Check Attribute Values

```swift
let prim = stage.getPrimAtPath(Sdf.Path("/Sphere"))
for attr in prim.getAttributes() {
  var value = Pixar.VtValue()
  if attr.get(&value) {
    print("\(attr.getName()): \(value)")
  }
}
```

### Getting Help

- **Documentation**: https://wabiverse.github.io/SwiftUSD/documentation/pixarusd/
- **USD Documentation**: https://openusd.org
- **GitHub Issues**: https://github.com/wabiverse/SwiftUSD/issues
- **Examples**: See `Sources/UsdView/` and `Sources/Examples/` in the repository

---

## Additional Resources

### USD Concepts
- [USD Glossary](https://openusd.org/release/glossary.html)
- [USD Tutorials](https://openusd.org/release/tut_usd_tutorials.html)
- [USD Best Practices](https://openusd.org/release/api/best_practices_8dox.html)

### SwiftUSD Specific
- [API Documentation](https://wabiverse.github.io/SwiftUSD/documentation/pixarusd/)
- [GitHub Repository](https://github.com/wabiverse/SwiftUSD)
- [Example Applications](https://github.com/wabiverse/SwiftUSD/tree/main/Sources)

### Tools
- **usdview**: Official USD viewer (Python-based)
- **UsdView**: Swift-based viewer in this repository
- **usdcat**: Command-line tool for USD inspection
- **usdedit**: Text editor for USD files

---

**Version**: 24.8.14
**Last Updated**: 2025-01-12
**License**: Modified Apache 2.0 (see LICENSE.txt)
