# SwiftUSD Documentation

Practical guide for building USD applications in Swift.

## Quick Start

```swift
import PixarUSD

// CRITICAL: Always setup resources first!
Pixar.Bundler.shared.setup(.resources)

// Create a stage
let stage = Usd.Stage.createNew("MyScene.usda")

// Add geometry
let sphere = UsdGeom.Sphere.define(stage, path: "/Sphere")
sphere.getRadiusAttr().set(2.0)

// Save
stage.save()
```

---

## Core Concepts

### USD Stage

The `UsdStage` is your scene graph - it owns all prims and provides the main API:

```swift
// Create stages
let stage = Usd.Stage.createNew("scene.usda")       // ASCII (human-readable)
let stage = Usd.Stage.createNew("scene.usdc")       // Binary crate (faster)
let stage = Usd.Stage.createInMemory()              // In-memory only

// Open existing stage
let stage = Usd.Stage.open("scene.usd")
let stage = Usd.Stage.open("scene.usda")
let layer = Sdf.Layer.findOrOpen("scene.usda")
let stage = Usd.Stage.open(layer)

// Stage operations
stage.save()
stage.export("output.usdc")
stage.export("output.usda")

// Get prims
let rootPrim = stage.getPseudoRoot()
let prim = stage.getPrimAtPath(Sdf.Path("/World/Sphere"))
let defaultPrim = stage.getDefaultPrim()

// Set default prim (entry point for asset)
stage.setDefaultPrim(stage.getPrimAtPath(Sdf.Path("/World")))

// Check validity
if stage.pointee.isValid() {
  print("Stage is valid")
}

// Get layers
let rootLayer = stage.getRootLayer()
let sessionLayer = stage.getSessionLayer()

// Time/animation
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

// Stage metadata
print("Up axis: \(Pixar.UsdGeomGetStageUpAxis(stage.pointee.getPtr()))")
print("Meters per unit: \(Pixar.UsdGeomGetStageMetersPerUnit(stage.pointee.getPtr()))")

Pixar.UsdGeomSetStageUpAxis(stage, .y)              // Y-up or Z-up
Pixar.UsdGeomSetStageMetersPerUnit(stage, 0.01)     // Centimeters
```

### Sdf.Path - USD Path System

`Sdf.Path` is USD's efficient path representation:

```swift
// Create prim paths
let worldPath = Sdf.Path("/World")
let spherePath = Sdf.Path("/World/Sphere")

// Special paths
let rootPath = Sdf.Path.absoluteRootPath()          // "/"
let emptyPath = Sdf.Path.emptyPath()

// Check path properties
print("Is absolute: \(worldPath.isAbsolutePath())")
print("Is prim path: \(worldPath.isPrimPath())")
print("Is property path: \(spherePath.isPropertyPath())")

// Path manipulation
let parentPath = spherePath.getParentPath()         // "/World"
let primName = spherePath.getName()                 // "Sphere"
let childPath = worldPath.appendChild(Pixar.TfToken("Sphere"))  // "/World/Sphere"

// Property paths
let radiusPath = spherePath.appendProperty(Pixar.TfToken("radius"))  // "/World/Sphere.radius"
let materialPath = spherePath.appendProperty(Pixar.TfToken("material:binding"))

// Path relationships
let hasPrefix = spherePath.hasPrefix(worldPath)     // true
let ancestor = Sdf.Path.findLongestPrefix(spherePath, Sdf.Path("/World/Cube"))
let relativePath = spherePath.makeRelativePath(worldPath)  // "Sphere"
```

### Prims (Primitives)

Prims are the fundamental nodes in your scene hierarchy:

```swift
// Get a prim by path
let prim = stage.getPrimAtPath(Sdf.Path("/World/Sphere"))

// Check validity
if prim.isValid() {
  print("Prim is valid")
}

if prim.isDefined() {
  print("Prim has authored scene description")
}

// Get properties
print("Prim name: \(prim.getName())")               // "Sphere"
print("Prim path: \(prim.getPath())")               // "/World/Sphere"
print("Prim type: \(prim.getTypeName())")           // "Sphere"

// Type checking
if prim.isA(UsdGeom.Sphere.self) {
  let sphere = UsdGeom.Sphere(prim)
  print("This is a Sphere")
}

let isGeometry = prim.isA(UsdGeom.Gprim.self)       // True for all geometry
let isXformable = prim.isA(UsdGeom.Xformable.self)  // True for transformable prims

// Create new prims
let world = stage.definePrim(Sdf.Path("/World"))
let sphere = stage.definePrim(Sdf.Path("/World/Sphere"), Pixar.TfToken("Sphere"))
let overridePrim = stage.overridePrim(Sdf.Path("/World/Sphere"))

// Get prim specifier
let specifier = prim.getSpecifier()  // .def, .over, or .class

// Hierarchy navigation
let parent = prim.getParent()
let children = prim.getChildren()
let allChildren = prim.getAllChildren()  // Includes inactive

for child in children {
  print("Child: \(child.getName())")
}

// Filtered children
let filterFunc: (Usd.Prim) -> Bool = { $0.isA(UsdGeom.Gprim.self) }
let geometryChildren = prim.getFilteredChildren(Usd.PrimDefaultPredicate, filterFunc)

let nextSibling = prim.getNextSibling()

// Prim activation
prim.setActive(true)
let isActive = prim.isActive()

// Payload loading
if prim.isLoaded() {
  print("Prim payload is loaded")
}

stage.load(prim.getPath())
stage.unload(prim.getPath())

// Prim metadata
prim.setMetadata(Pixar.TfToken("comment"), Pixar.VtValue("This is a sphere"))
var comment = Pixar.VtValue()
if prim.getMetadata(Pixar.TfToken("comment"), &comment) {
  print("Comment: \(comment)")
}

prim.setDocumentation("This sphere represents the player")
var doc = ""
prim.getDocumentation(&doc)

prim.setHidden(true)
let isHidden = prim.isHidden()

// Instancing
let isInstance = prim.isInstance()
if isInstance {
  let prototype = prim.getPrototype()
  print("Instance of: \(prototype.getPath())")
}
```

### Prim Traversal

```swift
// Basic depth-first traversal
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
  print("Visiting: \(prim.getPath())")
}

// Traversal with predicate (skip inactive prims)
let range = Usd.PrimRange.makeStage(stage, Usd.PrimDefaultPredicate)
for prim in range {
  print("Active prim: \(prim.getPath())")
}

// Custom traversal with filter
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

Attributes store time-sampled or default data:

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

let numSamples = radiusAttr.getNumTimeSamples()

// Check if time-varying
if radiusAttr.valueMightBeTimeVarying() {
  print("Attribute has animation")
}

// Get value type
let valueType = radiusAttr.getTypeName()
print("Type: \(valueType)")

// Attribute metadata
radiusAttr.setMetadata(Pixar.TfToken("units"), Pixar.VtValue("meters"))
radiusAttr.setDocumentation("The radius of the sphere in meters")

// Connection sources (for shading networks)
radiusAttr.addConnection(Sdf.Path("/Materials/Shader.outputs:radius"))
let sources = radiusAttr.getConnections()

// Clear operations
radiusAttr.clear()
radiusAttr.clearAtTime(UsdTimeCode(12.0))
radiusAttr.block()

// Check authoring
if radiusAttr.hasAuthoredValue() {
  print("Attribute has explicit value")
}

// Get all attributes on prim
for attr in prim.getAttributes() {
  print("Attribute: \(attr.getName())")
}

// Get attributes matching pattern
let displayAttrs = prim.getAttributes { name in
  name.hasPrefix("display")
}
```

### Attribute Value Types

```swift
// Scalars
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

// Vectors
let vec2fAttr = prim.createAttribute(Pixar.TfToken("myVec2f"), .float2, false)
vec2fAttr.set(Pixar.GfVec2f(1.0, 2.0))

let vec3fAttr = prim.createAttribute(Pixar.TfToken("myVec3f"), .float3, false)
vec3fAttr.set(Pixar.GfVec3f(1.0, 2.0, 3.0))

let vec3dAttr = prim.createAttribute(Pixar.TfToken("myVec3d"), .double3, false)
vec3dAttr.set(Pixar.GfVec3d(1.0, 2.0, 3.0))

let color3fAttr = prim.createAttribute(Pixar.TfToken("myColor"), .color3f, false)
color3fAttr.set(Pixar.GfVec3f(1.0, 0.5, 0.0))

// Matrices
let matrix4dAttr = prim.createAttribute(Pixar.TfToken("myMatrix"), .matrix4d, false)
var identity = Pixar.GfMatrix4d(1.0)
matrix4dAttr.set(identity)

// Arrays
let intArrayAttr = prim.createAttribute(Pixar.TfToken("myIntArray"), .intArray, false)
let intArray = Vt.IntArray(3, [1, 2, 3])
intArrayAttr.set(intArray)

let vec3fArrayAttr = prim.createAttribute(Pixar.TfToken("myVec3fArray"), .float3Array, false)
let vec3fArray = Vt.Vec3fArray(2, [
  Pixar.GfVec3f(1, 0, 0),
  Pixar.GfVec3f(0, 1, 0)
])
vec3fArrayAttr.set(vec3fArray)

// Asset paths
let assetAttr = prim.createAttribute(Pixar.TfToken("myAsset"), .asset, false)
assetAttr.set(Sdf.AssetPath("textures/diffuse.png"))
```

### Relationships

Relationships create connections between prims without storing data:

```swift
// Create relationship
let materialRel = prim.createRelationship(Pixar.TfToken("material:binding"), false)

// Add targets
materialRel.addTarget(Sdf.Path("/Materials/RedMaterial"))
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

// Get forwarded targets
var forwardedTargets: Sdf.PathVector = .init()
materialRel.getForwardedTargets(&forwardedTargets)

// Check and modify
let hasTarget = materialRel.hasAuthoredTargets()
materialRel.removeTarget(Sdf.Path("/Materials/BlueMaterial"))
materialRel.clearTargets(true)
materialRel.blockTargets()

// Get all relationships
for rel in prim.getRelationships() {
  print("Relationship: \(rel.getName())")
}
```

### Tokens

USD uses `TfToken` for efficient string interning:

```swift
// Create token
let myToken = Pixar.TfToken("myIdentifier")

// Predefined schema tokens
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

// Shading tokens
let surface = UsdShade.Tokens.surface
let displacement = UsdShade.Tokens.displacement
let volume = UsdShade.Tokens.volume

// Token operations
if myToken == defaultToken {
  print("Tokens are equal")  // Pointer comparison - very fast!
}

let tokenString = String(myToken.GetString())
if myToken.isEmpty() {
  print("Token is empty")
}

let hash = myToken.hash()

// Custom tokens
let customToken = Pixar.TfToken("custom:myAttribute")
let namespacedToken = Pixar.TfToken("myNamespace:myProperty")
```

---

## Working with Geometry

### Primitive Shapes

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

### Meshes

```swift
let mesh = UsdGeom.Mesh.define(stage, path: "/Mesh")

// Topology (two triangles forming a quad)
let faceVertexCounts = Vt.IntArray(2, [3, 3])
mesh.getFaceVertexCountsAttr().set(faceVertexCounts)

let faceVertexIndices = Vt.IntArray(6, [0, 1, 2, 2, 3, 0])
mesh.getFaceVertexIndicesAttr().set(faceVertexIndices)

// Points
let points = Vt.Vec3fArray(4, [
  Pixar.GfVec3f(0, 0, 0),
  Pixar.GfVec3f(1, 0, 0),
  Pixar.GfVec3f(1, 1, 0),
  Pixar.GfVec3f(0, 1, 0)
])
mesh.getPointsAttr().set(points)

// Normals
let normals = Vt.Vec3fArray(4, [
  Pixar.GfVec3f(0, 0, 1),
  Pixar.GfVec3f(0, 0, 1),
  Pixar.GfVec3f(0, 0, 1),
  Pixar.GfVec3f(0, 0, 1)
])
mesh.getNormalsAttr().set(normals)

// Subdivision
mesh.getSubdivisionSchemeAttr().set(UsdGeom.Tokens.catmullClark.token)
```

### Transforms

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

---

## Materials and Shading

### Materials

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

### Shading Networks

```swift
// Create material with texture
let material = UsdShade.Material.define(stage, path: "/Materials/CheckerMaterial")

// Texture coordinate reader
let texCoordReader = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/TexCoord")
texCoordReader.createIdAttr().set(Pixar.TfToken("UsdPrimvarReader_float2"))
texCoordReader.createInput(Pixar.TfToken("varname"), .token)
  .set(Pixar.TfToken("st"))

// Texture
let checker = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/Checker")
checker.createIdAttr().set(Pixar.TfToken("UsdUVTexture"))
checker.createInput(Pixar.TfToken("file"), .asset)
  .set(Sdf.AssetPath("checker.png"))

// Connect texture coordinates
checker.createInput(Pixar.TfToken("st"), .float2)
  .connectToSource(texCoordReader.connectableAPI(), Pixar.TfToken("result"))

// Surface shader
let surface = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/Surface")
surface.createIdAttr().set(Pixar.TfToken("UsdPreviewSurface"))

// Connect texture to diffuse
surface.createInput(Pixar.TfToken("diffuseColor"), .color3f)
  .connectToSource(checker.connectableAPI(), Pixar.TfToken("rgb"))

// Connect to material
material.createSurfaceOutput().connectToSource(
  surface.connectableAPI(),
  Pixar.TfToken("surface")
)
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

let rotateOp = xformable.addRotateXYZOp()
rotateOp.set(Pixar.GfVec3f(-30, 45, 0))
```

### Lights

```swift
// Distant light (sun/directional)
let distantLight = UsdLux.DistantLight.define(stage, path: "/Lights/Sun")
distantLight.getIntensityAttr().set(Float(1.0))
distantLight.getColorAttr().set(Pixar.GfVec3f(1.0, 0.95, 0.8))

// Sphere light (point light)
let sphereLight = UsdLux.SphereLight.define(stage, path: "/Lights/Point")
sphereLight.getIntensityAttr().set(Float(100.0))
sphereLight.getRadiusAttr().set(Float(0.5))

// Dome light (environment/HDRI)
let domeLight = UsdLux.DomeLight.define(stage, path: "/Lights/Environment")
domeLight.getIntensityAttr().set(Float(0.5))
domeLight.getTextureFileAttr().set(Sdf.AssetPath("hdri/studio.hdr"))
```

---

## Composition

USD's power comes from composing multiple files non-destructively:

### Layers and Sublayers

```swift
// Create layers
let rootLayer = Sdf.Layer.createNew("root.usda")
let layoutLayer = Sdf.Layer.createNew("layout.usda")
let animLayer = Sdf.Layer.createNew("anim.usda")
let lightingLayer = Sdf.Layer.createNew("lighting.usda")

// Add sublayers (stronger layers first)
let subLayerPaths = rootLayer.pointee.GetSubLayerPaths()
subLayerPaths.push_back(lightingLayer.pointee.GetIdentifier())
subLayerPaths.push_back(animLayer.pointee.GetIdentifier())
subLayerPaths.push_back(layoutLayer.pointee.GetIdentifier())

// Open stage with root layer
let stage = Usd.Stage.open(rootLayer)

// Layer operations
print("Layer identifier: \(rootLayer.pointee.GetIdentifier())")
print("Is dirty: \(rootLayer.pointee.IsDirty())")

rootLayer.pointee.Save()
rootLayer.pointee.Export("output.usdc")
rootLayer.pointee.Clear()

// Layer metadata
rootLayer.pointee.SetDocumentation("This layer contains...")
let comment = rootLayer.pointee.GetComment()

// Set sublayer offset (time offset/scale)
var offset = Pixar.SdfLayerOffset(offset: 10.0, scale: 1.0)
rootLayer.pointee.SetSubLayerOffset(offset, index: 0)

// Anonymous (in-memory) layers
let anonLayer = Sdf.Layer.createAnonymous()
let namedAnonLayer = Sdf.Layer.createAnonymous("TempLayer")

// Find or open (caches layers)
let existingLayer = Sdf.Layer.findOrOpen("asset.usda")

// Edit target controls which layer receives edits
let sessionTarget = Usd.EditTarget(stage.getSessionLayer())
stage.setEditTarget(sessionTarget)

// Edits now go to session layer (temporary overrides)
let sphere = UsdGeom.Sphere.define(stage, path: "/TempSphere")

// Reset to root layer
stage.setEditTarget(Usd.EditTarget(stage.getRootLayer()))
```

### References

```swift
// Reference external file
let prim = stage.definePrim(Sdf.Path("/AssetA"))
prim.getReferences().addReference("assets/table.usd")

// Reference specific prim in file
prim.getReferences().addReference("assets/table.usd", Sdf.Path("/Table"))

// Reference with layer offset (time offset/scale)
prim.getReferences().addReference(
  "assets/table.usd",
  Sdf.Path("/Table"),
  Sdf.LayerOffset(offset: 0.0, scale: 1.0)
)

// Internal reference (within same stage)
prim.getReferences().addInternalReference(Sdf.Path("/Prototypes/Table"))

// Remove reference
prim.getReferences().removeReference("assets/table.usd")

// Clear all references
prim.getReferences().clearReferences()

// Check for references
if prim.hasAuthoredReferences() {
  print("Prim has references")
}
```

### Payloads (Lazy Loading)

```swift
// Payloads are like references but lazy-loaded for scalability
let heavyPrim = stage.definePrim(Sdf.Path("/HeavyAsset"))

// Add payload
heavyPrim.getPayloads().addPayload("assets/heavy_model.usd")

// Add payload to specific prim
heavyPrim.getPayloads().addPayload("assets/heavy_model.usd", Sdf.Path("/Model"))

// Remove/clear payloads
heavyPrim.getPayloads().removePayload("assets/heavy_model.usd")
heavyPrim.getPayloads().clearPayloads()

// Load/unload payloads at stage level
stage.load(Sdf.Path("/HeavyAsset"))
stage.unload(Sdf.Path("/HeavyAsset"))

// Load with control
let loadSet = stage.getLoadSet()
stage.loadAndUnload(Sdf.PathSet([Sdf.Path("/HeavyAsset")]), Sdf.PathSet())
```

### Inherits (Class-based Inheritance)

```swift
// Create class prim
let classPath = Sdf.Path("/_class_Table")
let classPrim = stage.createClassPrim(classPath)

// Define properties on class
let classTable = UsdGeom.Mesh(classPrim)
// ... define table geometry ...

// Inherit from class
let instance = stage.definePrim(Sdf.Path("/Tables/DiningTable"))
instance.getInherits().addInherit(classPath)

// All instances inherit the class properties
let instance2 = stage.definePrim(Sdf.Path("/Tables/CoffeeTable"))
instance2.getInherits().addInherit(classPath)
```

### Specializes

```swift
// Less common, but useful for overriding strong opinions
let basePrim = stage.definePrim(Sdf.Path("/BaseAsset"))
let specializedPrim = stage.definePrim(Sdf.Path("/SpecializedAsset"))
specializedPrim.getSpecializes().addSpecialize(basePrim.getPath())
```

### Variants (Switchable Alternatives)

```swift
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
```

### Composition Query

```swift
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

## Rendering with Hydra

### Setup Render Engine

```swift
import PixarUSD

// Setup resources (REQUIRED)
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

### Render Frame

```swift
// Render at specific time with viewport size
let viewSize = CGSize(width: 1920, height: 1080)
let timeCode = 1.0

let texture = engine.render(at: timeCode, viewSize: viewSize)

// texture is a Pixar.HgiTextureHandle that can be converted
// to MTLTexture on Metal platforms
```

### Camera Controller

```swift
// Create camera controller
let camera = CameraController(
  eye: Pixar.GfVec3d(10, 5, 10),    // Camera position
  at: Pixar.GfVec3d(0, 0, 0),       // Look-at point
  up: Pixar.GfVec3d(0, 1, 0)        // Up vector
)

// Pan (move eye and look-at together)
camera.pan(delta: CGPoint(x: 10, y: -5))

// Orbit around look-at point
camera.orbit(delta: CGPoint(x: 45, y: 30))

// Zoom (move along view direction)
camera.zoom(delta: 2.0)
camera.zoomScroll(delta: scrollDelta)

// Focus on specific point
camera.focus(on: Pixar.GfVec3d(5, 0, 0), distance: 10.0)

// Reset to default view
camera.reset()

// Get view matrix for rendering
let viewMatrix = camera.getViewMatrix()
```

---

## Declarative Swift DSL

SwiftUSD provides a SwiftUI-like syntax:

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

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
.set(doc: "Example scene")
.save()
```

---

## Complete Examples

### Example 1: Multi-Object Scene

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let stage = Usd.Stage.createNew("multi_object_scene.usda")

// Create root
let world = UsdGeom.Xform.define(stage, path: "/World")

// Red sphere on left
let sphere = UsdGeom.Sphere.define(stage, path: "/World/Sphere")
sphere.getRadiusAttr().set(2.0)
sphere.getDisplayColorAttr().set(Vt.Vec3fArray(1, [Pixar.GfVec3f(1, 0, 0)]))

let sphereXform = UsdGeom.Xformable(sphere.getPrim())
sphereXform.addTranslateOp().set(Pixar.GfVec3d(-3, 0, 0))

// Green cube in center
let cube = UsdGeom.Cube.define(stage, path: "/World/Cube")
cube.getSizeAttr().set(2.0)
cube.getDisplayColorAttr().set(Vt.Vec3fArray(1, [Pixar.GfVec3f(0, 1, 0)]))

// Blue cylinder on right
let cylinder = UsdGeom.Cylinder.define(stage, path: "/World/Cylinder")
cylinder.getRadiusAttr().set(1.0)
cylinder.getHeightAttr().set(3.0)
cylinder.getDisplayColorAttr().set(Vt.Vec3fArray(1, [Pixar.GfVec3f(0, 0, 1)]))

let cylinderXform = UsdGeom.Xformable(cylinder.getPrim())
cylinderXform.addTranslateOp().set(Pixar.GfVec3d(3, 0, 0))

// Camera
let camera = UsdGeom.Camera.define(stage, path: "/Camera")
camera.getFocalLengthAttr().set(Float(50.0))

let cameraXform = UsdGeom.Xformable(camera.getPrim())
cameraXform.addTranslateOp().set(Pixar.GfVec3d(0, 5, 15))
cameraXform.addRotateXYZOp().set(Pixar.GfVec3f(-15, 0, 0))

// Light
let light = UsdLux.DistantLight.define(stage, path: "/Light")
light.getIntensityAttr().set(Float(1.5))

stage.save()
print("Scene created!")
```

### Example 2: Load and Modify Scene

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

### Example 4: Shading Network

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

let stage = Usd.Stage.createNew("shading_example.usda")

// Create material
let material = UsdShade.Material.define(stage, path: "/Materials/CheckerMaterial")

// Texture coordinate reader
let texCoordReader = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/TexCoord")
texCoordReader.createIdAttr().set(Pixar.TfToken("UsdPrimvarReader_float2"))
texCoordReader.createInput(Pixar.TfToken("varname"), .token)
  .set(Pixar.TfToken("st"))

// Checker texture
let checker = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/Checker")
checker.createIdAttr().set(Pixar.TfToken("UsdUVTexture"))
checker.createInput(Pixar.TfToken("file"), .asset)
  .set(Sdf.AssetPath("checker.png"))

// Connect texture coordinates
checker.createInput(Pixar.TfToken("st"), .float2)
  .connectToSource(texCoordReader.connectableAPI(), Pixar.TfToken("result"))

// Surface shader
let surface = UsdShade.Shader.define(stage, path: "/Materials/CheckerMaterial/Surface")
surface.createIdAttr().set(Pixar.TfToken("UsdPreviewSurface"))

// Connect checker to diffuse
surface.createInput(Pixar.TfToken("diffuseColor"), .color3f)
  .connectToSource(checker.connectableAPI(), Pixar.TfToken("rgb"))

// Connect to material
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

## Debug and Inspection

### Print Stage Structure

```swift
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

### Inspect Attribute Values

```swift
let prim = stage.getPrimAtPath(Sdf.Path("/Sphere"))
for attr in prim.getAttributes() {
  var value = Pixar.VtValue()
  if attr.get(&value) {
    print("\(attr.getName()): \(value)")
  }
}
```

---

## Resources

- **API Documentation**: https://wabiverse.github.io/SwiftUSD/documentation/pixarusd/
- **USD Documentation**: https://openusd.org
- **GitHub Repository**: https://github.com/wabiverse/SwiftUSD
- **Examples**: See `Sources/UsdView/` and `Sources/Examples/`
- **Migration Guides**:
  - [SWIFT_6.2_MIGRATION.md](./SWIFT_6.2_MIGRATION.md) - Swift 6.2 C++ interop features
  - [XCODE_26_MIGRATION.md](./XCODE_26_MIGRATION.md) - Xcode compatibility guide

---

**Version**: 24.8.14
**Swift**: 6.0+
**USD**: 24.8
