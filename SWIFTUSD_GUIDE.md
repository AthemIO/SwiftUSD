# SwiftUSD API Reference

Native Swift bindings for Pixar's Universal Scene Description (USD).

---

## Quick Start

```swift
import PixarUSD

// CRITICAL: Must be called before any USD operations!
Pixar.Bundler.shared.setup(.resources)

// Create a new stage
let stage = Usd.Stage.createNew("HelloWorld.usda")

// Create geometry
let world = UsdGeom.Xform.define(stage, path: "/World")
let sphere = UsdGeom.Sphere.define(stage, path: "/World/Sphere")
sphere.getRadiusAttr().set(2.0)

// Save
stage.save()
```

---

## Stage Operations

```swift
// Creation
let stage = Usd.Stage.createNew("file.usda")      // ASCII format
let stage = Usd.Stage.createNew("file.usdc")      // Binary format (faster)
let stage = Usd.Stage.createInMemory()            // In-memory only

// Opening
let stage = Usd.Stage.open("file.usd")
let stage = Usd.Stage.open("file.usd", load: .none)  // Don't load payloads

// Saving
stage.save()
stage.export("output.usdc")
stage.export("output.usdz")

// Export to string
var text = std.string()
stage.pointee.ExportToString(&text)
```

### Stage Metadata

```swift
stage.setMetadata(Pixar.TfToken("comment"), Pixar.VtValue("My scene"))
stage.setStartTimeCode(1.0)
stage.setEndTimeCode(240.0)
stage.pointee.SetFramesPerSecond(24.0)

// Default prim
let rootPrim = stage.getPrim(at: "/World")
stage.setDefaultPrim(rootPrim)
```

### Layer Management

```swift
let rootLayer = stage.getRootLayer()
let subLayerPaths = rootLayer.pointee.GetSubLayerPaths()
subLayerPaths.push_back(std.string("override.usda"))
```

### Stage Traversal

```swift
// Simple traversal
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
    print("\(prim.getPath()): \(prim.getTypeName())")
}

// Find prims by type
for prim in Usd.PrimRange(stage.getPseudoRoot()) {
    if prim.isA(UsdGeom.Mesh.self) {
        let mesh = UsdGeom.Mesh(prim)
    }
}
```

---

## Path Operations

```swift
// Create paths
let path = Sdf.Path("/World/Sphere")
let path = Sdf.Path("/World").appendChild(Pixar.TfToken("Sphere"))

// Path manipulation
let parent = path.getParentPath()           // "/World"
let name = path.getName()                   // "Sphere"
let child = path.appendChild(token)         // "/World/Sphere/Child"
let prop = path.appendProperty(token)       // "/World/Sphere.radius"

// Path queries
let isEmpty = path.isEmpty()
let isAbsolute = path.isAbsolutePath()
```

---

## Prim Operations

```swift
// Define prims
let prim = stage.definePrim("/World")
let prim = stage.definePrim("/World", type: Pixar.TfToken("Xform"))

// Get prims
let prim = stage.getPrim(at: "/World")
let prim = stage.getPrim(at: Sdf.Path("/World"))

// Prim queries
let name = prim.getName()
let path = prim.getPath()
let typeName = prim.getTypeName()
let isValid = prim.isValid()
let isActive = prim.isActive()

// Hierarchy
let parent = prim.getParent()
let children = prim.getChildren()

// Type checking
if prim.isA(UsdGeom.Sphere.self) {
    let sphere = UsdGeom.Sphere(prim)
}

// Traversal
for child in Usd.PrimRange(prim) {
    print(child.getPath())
}
```

---

## Attribute Operations

```swift
// Get attribute
let attr = prim.getAttribute(Pixar.TfToken("radius"))

// Create custom attribute
let customAttr = prim.createAttribute(
    Pixar.TfToken("custom:myValue"),
    Sdf.ValueTypeName.double,
    false  // custom (not built-in)
)

// Set values
attr.set(5.0)                                    // Default value
attr.set(10.0, time: UsdTimeCode(24.0))         // At specific time

// Get values
var value: Double = 0
attr.get(&value)                                 // Default value
attr.get(&value, time: UsdTimeCode(24.0))       // At specific time

// Animation queries
let isAnimated = attr.valueMightBeTimeVarying()
var timeSamples = [Double]()
attr.getTimeSamples(&timeSamples)
```

### Tokens

```swift
let token = Pixar.TfToken("myToken")
let sphereToken = UsdGeom.Tokens.Sphere
let xformToken = UsdGeom.Tokens.Xform
```

---

## Geometry

### Transform Hierarchy (Xform)

```swift
let xform = UsdGeom.Xform.define(stage, path: "/World")
let xformable = UsdGeom.Xformable(xform.GetPrim())

// Transform operations
let translateOp = xformable.addTranslateOp()
translateOp.set(Pixar.GfVec3d(10, 0, 0))

let rotateOp = xformable.addRotateXYZOp()
rotateOp.set(Pixar.GfVec3f(0, 45, 0))  // Degrees

let scaleOp = xformable.addScaleOp()
scaleOp.set(Pixar.GfVec3f(2, 2, 2))

// Get computed transform
var localMatrix = Pixar.GfMatrix4d()
var resetStack = false
xformable.getLocalTransformation(&localMatrix, &resetStack, UsdTimeCode.Default())
```

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
cylinder.getAxisAttr().set(UsdGeom.Tokens.Y)

// Cone
let cone = UsdGeom.Cone.define(stage, path: "/Cone")
cone.getRadiusAttr().set(1.0)
cone.getHeightAttr().set(3.0)

// Capsule
let capsule = UsdGeom.Capsule.define(stage, path: "/Capsule")
capsule.getRadiusAttr().set(0.5)
capsule.getHeightAttr().set(2.0)
```

### Polygon Meshes

```swift
let mesh = UsdGeom.Mesh.define(stage, path: "/Mesh")

// Vertices
let points = Vt.Vec3fArray()
points.push_back(Pixar.GfVec3f(0, 0, 0))
points.push_back(Pixar.GfVec3f(1, 0, 0))
points.push_back(Pixar.GfVec3f(1, 1, 0))
points.push_back(Pixar.GfVec3f(0, 1, 0))
mesh.getPointsAttr().set(points)

// Face vertex counts
let faceCounts = Vt.IntArray()
faceCounts.push_back(4)
mesh.getFaceVertexCountsAttr().set(faceCounts)

// Face vertex indices
let faceIndices = Vt.IntArray()
[0, 1, 2, 3].forEach { faceIndices.push_back($0) }
mesh.getFaceVertexIndicesAttr().set(faceIndices)

// Normals
let normals = Vt.Vec3fArray()
for _ in 0..<4 { normals.push_back(Pixar.GfVec3f(0, 0, 1)) }
mesh.getNormalsAttr().set(normals)
mesh.setNormalsInterpolation(UsdGeom.Tokens.vertex)

// UVs
let uvs = Vt.Vec2fArray()
uvs.push_back(Pixar.GfVec2f(0, 0))
uvs.push_back(Pixar.GfVec2f(1, 0))
uvs.push_back(Pixar.GfVec2f(1, 1))
uvs.push_back(Pixar.GfVec2f(0, 1))

let primvar = UsdGeom.PrimvarsAPI(mesh.GetPrim())
let stPrimvar = primvar.createPrimvar(
    Pixar.TfToken("st"),
    Sdf.ValueTypeName.texCoord2fArray,
    UsdGeom.Tokens.vertex
)
stPrimvar.set(uvs)

// Subdivision
mesh.getSubdivisionSchemeAttr().set(UsdGeom.Tokens.catmullClark)
mesh.getSubdivisionSchemeAttr().set(UsdGeom.Tokens.none)  // No subdivision
```

### Curves

```swift
let curves = UsdGeom.BasisCurves.define(stage, path: "/Curves")

curves.getTypeAttr().set(UsdGeom.Tokens.cubic)
curves.getBasisAttr().set(UsdGeom.Tokens.bspline)
curves.getWrapAttr().set(UsdGeom.Tokens.nonperiodic)

let points = Vt.Vec3fArray()
// Add control points...
curves.getPointsAttr().set(points)

let vertexCounts = Vt.IntArray()
vertexCounts.push_back(10)  // 10 control points
curves.getCurveVertexCountsAttr().set(vertexCounts)

let widths = Vt.FloatArray()
widths.push_back(0.1)
curves.getWidthsAttr().set(widths)
```

### Points (Point Clouds)

```swift
let points = UsdGeom.Points.define(stage, path: "/PointCloud")

let positions = Vt.Vec3fArray()
for _ in 0..<1000 {
    positions.push_back(Pixar.GfVec3f(
        Float.random(in: -10...10),
        Float.random(in: -10...10),
        Float.random(in: -10...10)
    ))
}
points.getPointsAttr().set(positions)

let widths = Vt.FloatArray()
for _ in 0..<1000 { widths.push_back(0.1) }
points.getWidthsAttr().set(widths)
```

### Cameras

```swift
let camera = UsdGeom.Camera.define(stage, path: "/Camera")

camera.getFocalLengthAttr().set(Float(50.0))
camera.getHorizontalApertureAttr().set(Float(36.0))
camera.getVerticalApertureAttr().set(Float(24.0))
camera.getClippingRangeAttr().set(Pixar.GfVec2f(0.1, 10000.0))
camera.getFocusDistanceAttr().set(Float(10.0))
camera.getFStopAttr().set(Float(2.8))
camera.getProjectionAttr().set(UsdGeom.Tokens.perspective)
```

---

## Materials and Shading

### UsdPreviewSurface Material

```swift
// Create material
let material = UsdShade.Material.define(stage, path: "/Materials/MyMaterial")

// Create surface shader
let shader = UsdShade.Shader.define(stage, path: "/Materials/MyMaterial/Surface")
shader.createIdAttr().set(Pixar.TfToken("UsdPreviewSurface"))

// Set parameters
shader.createInput(Pixar.TfToken("diffuseColor"), Sdf.ValueTypeName.color3f)
    .set(Pixar.GfVec3f(0.8, 0.2, 0.2))

shader.createInput(Pixar.TfToken("metallic"), Sdf.ValueTypeName.float)
    .set(Float(0.0))

shader.createInput(Pixar.TfToken("roughness"), Sdf.ValueTypeName.float)
    .set(Float(0.4))

shader.createInput(Pixar.TfToken("opacity"), Sdf.ValueTypeName.float)
    .set(Float(1.0))

// Connect shader to material
let surfaceOutput = material.createSurfaceOutput()
surfaceOutput.connectToSource(shader.connectableAPI(), Pixar.TfToken("surface"))

// Bind to geometry
let bindingAPI = UsdShade.MaterialBindingAPI.apply(sphere.GetPrim())
bindingAPI.bind(material)
```

### UsdPreviewSurface Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `diffuseColor` | color3f | Base color (RGB) |
| `emissiveColor` | color3f | Emission color |
| `metallic` | float | Metallic factor (0-1) |
| `roughness` | float | Surface roughness (0-1) |
| `opacity` | float | Opacity (0-1) |
| `ior` | float | Index of refraction |
| `clearcoat` | float | Clearcoat intensity |
| `clearcoatRoughness` | float | Clearcoat roughness |
| `normal` | normal3f | Normal map input |
| `displacement` | float | Displacement amount |
| `occlusion` | float | Ambient occlusion |

### Texture Mapping

```swift
// Texture coordinate reader
let stReader = UsdShade.Shader.define(stage, path: "/Materials/Mat/STReader")
stReader.createIdAttr().set(Pixar.TfToken("UsdPrimvarReader_float2"))
stReader.createInput(Pixar.TfToken("varname"), Sdf.ValueTypeName.token)
    .set(Pixar.TfToken("st"))

// Texture sampler
let diffuseTexture = UsdShade.Shader.define(stage, path: "/Materials/Mat/DiffuseTexture")
diffuseTexture.createIdAttr().set(Pixar.TfToken("UsdUVTexture"))
diffuseTexture.createInput(Pixar.TfToken("file"), Sdf.ValueTypeName.asset)
    .set(Sdf.AssetPath("textures/diffuse.png"))
diffuseTexture.createInput(Pixar.TfToken("wrapS"), Sdf.ValueTypeName.token)
    .set(Pixar.TfToken("repeat"))
diffuseTexture.createInput(Pixar.TfToken("wrapT"), Sdf.ValueTypeName.token)
    .set(Pixar.TfToken("repeat"))

// Connect texture coordinates
diffuseTexture.createInput(Pixar.TfToken("st"), Sdf.ValueTypeName.float2)
    .connectToSource(stReader.connectableAPI(), Pixar.TfToken("result"))

// Connect to surface
surfaceShader.createInput(Pixar.TfToken("diffuseColor"), Sdf.ValueTypeName.color3f)
    .connectToSource(diffuseTexture.connectableAPI(), Pixar.TfToken("rgb"))
```

### Normal Mapping

```swift
let normalTexture = UsdShade.Shader.define(stage, path: "/Materials/Mat/NormalTexture")
normalTexture.createIdAttr().set(Pixar.TfToken("UsdUVTexture"))
normalTexture.createInput(Pixar.TfToken("file"), Sdf.ValueTypeName.asset)
    .set(Sdf.AssetPath("textures/normal.png"))
normalTexture.createInput(Pixar.TfToken("scale"), Sdf.ValueTypeName.float4)
    .set(Pixar.GfVec4f(2, 2, 2, 1))
normalTexture.createInput(Pixar.TfToken("bias"), Sdf.ValueTypeName.float4)
    .set(Pixar.GfVec4f(-1, -1, -1, 0))

surfaceShader.createInput(Pixar.TfToken("normal"), Sdf.ValueTypeName.normal3f)
    .connectToSource(normalTexture.connectableAPI(), Pixar.TfToken("rgb"))
```

---

## Lighting

### Light Types

```swift
// Distant Light (Sun)
let distantLight = UsdLux.DistantLight.define(stage, path: "/Lights/Sun")
distantLight.getIntensityAttr().set(Float(1.0))
distantLight.getColorAttr().set(Pixar.GfVec3f(1.0, 0.95, 0.9))
distantLight.getAngleAttr().set(Float(0.53))

// Sphere Light (Point)
let sphereLight = UsdLux.SphereLight.define(stage, path: "/Lights/Point")
sphereLight.getIntensityAttr().set(Float(500.0))
sphereLight.getRadiusAttr().set(Float(0.5))

// Rectangle Light (Area)
let rectLight = UsdLux.RectLight.define(stage, path: "/Lights/Area")
rectLight.getIntensityAttr().set(Float(100.0))
rectLight.getWidthAttr().set(Float(2.0))
rectLight.getHeightAttr().set(Float(2.0))

// Disk Light
let diskLight = UsdLux.DiskLight.define(stage, path: "/Lights/Disk")
diskLight.getIntensityAttr().set(Float(100.0))
diskLight.getRadiusAttr().set(Float(1.0))

// Cylinder Light
let cylinderLight = UsdLux.CylinderLight.define(stage, path: "/Lights/Cylinder")
cylinderLight.getIntensityAttr().set(Float(100.0))
cylinderLight.getRadiusAttr().set(Float(0.5))
cylinderLight.getLengthAttr().set(Float(3.0))

// Dome Light (Environment/HDRI)
let domeLight = UsdLux.DomeLight.define(stage, path: "/Lights/Environment")
domeLight.getIntensityAttr().set(Float(1.0))
domeLight.getTextureFileAttr().set(Sdf.AssetPath("environment.exr"))
domeLight.getTextureFormatAttr().set(UsdLux.Tokens.latlong)
```

### Positioning Lights

```swift
let xformable = UsdGeom.Xformable(sphereLight.GetPrim())
xformable.addTranslateOp().set(Pixar.GfVec3d(0, 10, 0))
xformable.addRotateXYZOp().set(Pixar.GfVec3f(-45, 0, 0))
```

### Light Properties

```swift
light.getIntensityAttr().set(Float(1.0))
light.getColorAttr().set(Pixar.GfVec3f(1, 1, 1))
light.getEnableColorTemperatureAttr().set(true)
light.getColorTemperatureAttr().set(Float(6500))  // Kelvin
light.getShadowEnableAttr().set(true)
light.getShadowColorAttr().set(Pixar.GfVec3f(0, 0, 0))
```

---

## Animation

### Time Configuration

```swift
stage.setStartTimeCode(1.0)
stage.setEndTimeCode(240.0)
stage.pointee.SetFramesPerSecond(24.0)
stage.pointee.SetTimeCodesPerSecond(24.0)
```

### Animating Attributes

```swift
let sphere = UsdGeom.Sphere.define(stage, path: "/AnimatedSphere")
let radiusAttr = sphere.getRadiusAttr()

for frame in 1...240 {
    let time = UsdTimeCode(Double(frame))
    let t = Double(frame) / 240.0
    let radius = 1.0 + 0.5 * sin(t * .pi * 4.0)
    radiusAttr.set(radius, time: time)
}
```

### Animating Transforms

```swift
let xform = UsdGeom.Xform.define(stage, path: "/AnimatedXform")
let xformable = UsdGeom.Xformable(xform.GetPrim())

let translateOp = xformable.addTranslateOp()
let rotateOp = xformable.addRotateYOp()

for frame in 1...240 {
    let time = UsdTimeCode(Double(frame))
    let t = Double(frame) / 240.0

    // Circular motion
    let x = cos(t * .pi * 2.0) * 5.0
    let z = sin(t * .pi * 2.0) * 5.0
    translateOp.set(Pixar.GfVec3d(x, 0, z), time: time)

    // Rotation
    rotateOp.set(Float(t * 360.0), time: time)
}
```

### Reading Animation

```swift
if attr.valueMightBeTimeVarying() {
    print("Attribute is animated")
}

var timeSamples = [Double]()
attr.getTimeSamples(&timeSamples)

for time in timeSamples {
    var value: Double = 0
    attr.get(&value, time: UsdTimeCode(time))
}

// Interpolated value
var interpolatedValue: Double = 0
attr.get(&interpolatedValue, time: UsdTimeCode(12.5))
```

### Skeletal Animation (UsdSkel)

```swift
let skeleton = UsdSkel.Skeleton.define(stage, path: "/Character/Skeleton")

let jointOrder = Vt.TokenArray()
jointOrder.push_back(Pixar.TfToken("Root"))
jointOrder.push_back(Pixar.TfToken("Root/Spine"))
jointOrder.push_back(Pixar.TfToken("Root/Spine/Head"))
skeleton.getJointsAttr().set(jointOrder)

// Bind skeleton to mesh
let mesh = UsdGeom.Mesh.define(stage, path: "/Character/Body")
let skelBinding = UsdSkel.BindingAPI.apply(mesh.GetPrim())
skelBinding.createSkeletonRel().addTarget(Sdf.Path("/Character/Skeleton"))
```

---

## Rendering with Hydra

### Render Engine

```swift
let stage = Usd.Stage.open("scene.usd")

let engine = Hydra.RenderEngine(stage: stage)

let camera = CameraController(isZUp: stage.pointee.GetUpAxis() == UsdGeom.Tokens.Z)
camera.setDistance(10.0)
engine.setCameraController(camera)

let viewSize = CGSize(width: 1920, height: 1080)
let texture = engine.render(at: UsdTimeCode(1.0), viewSize: viewSize)
```

### Camera Controller

```swift
let camera = CameraController(isZUp: true)

camera.orbit(delta: CGPoint(x: 45, y: 30))   // Tumble
camera.pan(delta: CGPoint(x: 10, y: 5))      // Track
camera.zoom(delta: 2.0)                       // Dolly
camera.frameSelection(boundingBox: sceneBounds)

let viewMatrix = camera.getViewMatrix()
let projectionMatrix = camera.getProjectionMatrix(aspectRatio: 16.0/9.0)
```

### Metal Integration

```swift
#if canImport(Metal)
import Metal
import MetalKit

class USDMetalView: MTKView, MTKViewDelegate {
    var engine: Hydra.RenderEngine!
    var camera: CameraController!

    func setup(stage: UsdStageRefPtr) {
        self.device = MTLCreateSystemDefaultDevice()
        self.delegate = self
        engine = Hydra.RenderEngine(stage: stage)
        camera = CameraController(isZUp: true)
        engine.setCameraController(camera)
    }

    func draw(in view: MTKView) {
        let texture = engine.render(at: UsdTimeCode.Default(), viewSize: view.drawableSize)
        // Blit texture to drawable...
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {}
}
#endif
```

---

## Composition

### References

```swift
let prim = stage.definePrim("/ReferencedAsset")
prim.getReferences().addReference("asset.usd")
prim.getReferences().addReference("asset.usd", Sdf.Path("/Root/Asset"))
prim.getReferences().addInternalReference(Sdf.Path("/Templates/Asset"))
prim.getReferences().clearReferences()
```

### Payloads (Lazy Loading)

```swift
let prim = stage.definePrim("/HeavyAsset")
prim.getPayloads().addPayload("heavy_geometry.usd")

stage.unload(Sdf.Path("/HeavyAsset"))
stage.load(Sdf.Path("/HeavyAsset"))

// Open without loading payloads
let stage = Usd.Stage.open("scene.usd", load: .none)
```

### Variants

```swift
let prim = stage.definePrim("/Asset")
let variantSets = prim.getVariantSets()
let lodVariant = variantSets.addVariantSet("LOD")

lodVariant.addVariant("high")
lodVariant.addVariant("medium")
lodVariant.addVariant("low")

// Author in variant context
lodVariant.setVariantSelection("high")
// ... create high-res geometry

lodVariant.setVariantSelection("low")
// ... create low-res geometry

// Query
let variants = lodVariant.getVariantNames()
let current = lodVariant.getVariantSelection()
```

### Sublayers

```swift
let rootLayer = stage.getRootLayer()
let subLayerPaths = rootLayer.pointee.GetSubLayerPaths()
subLayerPaths.insert(std.string("animation.usda"), 0)  // Strongest
subLayerPaths.push_back(std.string("layout.usda"))     // Weaker

let newLayer = Sdf.Layer.createNew("override.usda")
subLayerPaths.insert(newLayer.pointee.GetIdentifier(), 0)
```

### Inherits

```swift
let classPrim = stage.createClassPrim(Sdf.Path("/class_SharedProperties"))

let prim1 = stage.definePrim("/Object1")
prim1.getInherits().addInherit(Sdf.Path("/class_SharedProperties"))

let prim2 = stage.definePrim("/Object2")
prim2.getInherits().addInherit(Sdf.Path("/class_SharedProperties"))
```

---

## Examples

### Complete Scene

```swift
import PixarUSD

@main
enum CreateScene {
    static func main() {
        Pixar.Bundler.shared.setup(.resources)

        let stage = Usd.Stage.createNew("CompleteScene.usda")
        stage.setStartTimeCode(1.0)
        stage.setEndTimeCode(240.0)

        // Hierarchy
        let world = UsdGeom.Xform.define(stage, path: "/World")

        // Sphere with material
        let sphere = UsdGeom.Sphere.define(stage, path: "/World/Sphere")
        sphere.getRadiusAttr().set(2.0)

        let sphereXform = UsdGeom.Xformable(sphere.GetPrim())
        sphereXform.addTranslateOp().set(Pixar.GfVec3d(0, 2, 0))

        // Material
        let material = UsdShade.Material.define(stage, path: "/Materials/Red")
        let shader = UsdShade.Shader.define(stage, path: "/Materials/Red/Surface")
        shader.createIdAttr().set(Pixar.TfToken("UsdPreviewSurface"))
        shader.createInput(Pixar.TfToken("diffuseColor"), Sdf.ValueTypeName.color3f)
            .set(Pixar.GfVec3f(0.8, 0.1, 0.1))
        material.createSurfaceOutput()
            .connectToSource(shader.connectableAPI(), Pixar.TfToken("surface"))
        UsdShade.MaterialBindingAPI.apply(sphere.GetPrim()).bind(material)

        // Lighting
        let sun = UsdLux.DistantLight.define(stage, path: "/Lights/Sun")
        sun.getIntensityAttr().set(Float(1.0))
        UsdGeom.Xformable(sun.GetPrim()).addRotateXYZOp()
            .set(Pixar.GfVec3f(-45, 30, 0))

        // Camera
        let camera = UsdGeom.Camera.define(stage, path: "/Camera")
        camera.getFocalLengthAttr().set(Float(35))
        let camXform = UsdGeom.Xformable(camera.GetPrim())
        camXform.addTranslateOp().set(Pixar.GfVec3d(10, 5, 10))
        camXform.addRotateXYZOp().set(Pixar.GfVec3f(-20, 45, 0))

        // Animation
        let animOp = sphereXform.addTranslateOp(opSuffix: Pixar.TfToken("anim"))
        for frame in 1...240 {
            let t = Double(frame) / 240.0
            let y = 2.0 + sin(t * .pi * 4) * 0.5
            animOp.set(Pixar.GfVec3d(0, y, 0), time: UsdTimeCode(Double(frame)))
        }

        stage.setDefaultPrim(world.GetPrim())
        stage.save()
    }
}
```

### Declarative Scene Building

```swift
import PixarUSD

Pixar.Bundler.shared.setup(.resources)

USDStage("DeclarativeScene", ext: .usda) {
    USDPrim("World", type: .xform) {
        USDPrim("Ground", type: .mesh)
        USDPrim("Objects", type: .xform) {
            USDPrim("Sphere1", type: .sphere)
            USDPrim("Sphere2", type: .sphere)
            USDPrim("Cube", type: .cube)
        }
        USDPrim("Lights", type: .xform) {
            USDPrim("Key", type: .distantLight)
            USDPrim("Fill", type: .sphereLight)
        }
        USDPrim("Camera", type: .camera)
    }
}
.set(doc: "Declaratively built scene")
.save()
```
