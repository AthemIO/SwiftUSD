// UsdGeomTests.swift - Tests for UsdGeom module (Geometry Schemas)

import XCTest
import SwiftUSD
@testable import CxxFacade

final class UsdGeomTests: XCTestCase {

    // MARK: - UsdGeomXform Tests

    func testXformDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/World")
        let xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, path)

        XCTAssertTrue(xform.GetPrim().IsValid())
        XCTAssertEqual(String(xform.GetPrim().GetPath().GetString()), "/World")
    }

    func testXformAddTranslateOp() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/World")
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, path)

        let precision = cxxfacade.usdGeom.UsdGeomXformOpPrecision.PrecisionDouble
        let suffix = cxxfacade.tf.TfToken()
        let translateOp = xform.AddTranslateOp(precision, suffix, false)
        XCTAssertTrue(translateOp.IsValid())
        XCTAssertEqual(translateOp.GetOpType(), cxxfacade.usdGeom.UsdGeomXformOpType.TypeTranslate)
    }

    func testXformAddScaleOp() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/World")
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, path)

        let precision = cxxfacade.usdGeom.UsdGeomXformOpPrecision.PrecisionFloat
        let suffix = cxxfacade.tf.TfToken()
        let scaleOp = xform.AddScaleOp(precision, suffix, false)
        XCTAssertTrue(scaleOp.IsValid())
        XCTAssertEqual(scaleOp.GetOpType(), cxxfacade.usdGeom.UsdGeomXformOpType.TypeScale)
    }

    func testXformAddRotateOps() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, cxxfacade.sdf.SdfPath("/World"))

        let precision = cxxfacade.usdGeom.UsdGeomXformOpPrecision.PrecisionFloat
        let suffix = cxxfacade.tf.TfToken()

        let rotateX = xform.AddRotateXOp(precision, suffix, false)
        XCTAssertEqual(rotateX.GetOpType(), cxxfacade.usdGeom.UsdGeomXformOpType.TypeRotateX)

        let rotateY = xform.AddRotateYOp(precision, suffix, false)
        XCTAssertEqual(rotateY.GetOpType(), cxxfacade.usdGeom.UsdGeomXformOpType.TypeRotateY)

        let rotateZ = xform.AddRotateZOp(precision, suffix, false)
        XCTAssertEqual(rotateZ.GetOpType(), cxxfacade.usdGeom.UsdGeomXformOpType.TypeRotateZ)

        let rotateXYZ = xform.AddRotateXYZOp(precision, suffix, false)
        XCTAssertEqual(rotateXYZ.GetOpType(), cxxfacade.usdGeom.UsdGeomXformOpType.TypeRotateXYZ)
    }

    func testXformGetLocalTransformation() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, cxxfacade.sdf.SdfPath("/World"))

        var matrix = cxxfacade.gf.Matrix4d()
        var resetsStack = false
        let success = xform.GetLocalTransformation(&matrix, &resetsStack)
        XCTAssertTrue(success)
    }

    func testXformClearOps() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, cxxfacade.sdf.SdfPath("/World"))

        let precision = cxxfacade.usdGeom.UsdGeomXformOpPrecision.PrecisionDouble
        let suffix = cxxfacade.tf.TfToken()
        let _ = xform.AddTranslateOp(precision, suffix, false)
        let _ = xform.AddScaleOp(cxxfacade.usdGeom.UsdGeomXformOpPrecision.PrecisionFloat, suffix, false)

        let cleared = xform.ClearXformOpOrder()
        XCTAssertTrue(cleared)
    }

    // MARK: - UsdGeomScope Tests

    func testScopeDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Scope")
        let scope = cxxfacade.usdGeom.UsdGeomScope.Define(&stage, path)

        XCTAssertTrue(scope.GetPrim().IsValid())
        XCTAssertEqual(String(cString: scope.GetPrim().GetTypeName().GetText()), "Scope")
    }

    // MARK: - UsdGeomMesh Tests

    func testMeshDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Mesh")
        let mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, path)

        XCTAssertTrue(mesh.GetPrim().IsValid())
        XCTAssertEqual(String(cString: mesh.GetPrim().GetTypeName().GetText()), "Mesh")
    }

    func testMeshAttributes() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/Mesh"))

        let pointsAttr = mesh.CreatePointsAttr()
        XCTAssertTrue(pointsAttr.IsValid())

        let faceVertexCountsAttr = mesh.CreateFaceVertexCountsAttr()
        XCTAssertTrue(faceVertexCountsAttr.IsValid())

        let faceVertexIndicesAttr = mesh.CreateFaceVertexIndicesAttr()
        XCTAssertTrue(faceVertexIndicesAttr.IsValid())
    }

    func testMeshSubdivisionScheme() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/Mesh"))

        let subdivSchemeAttr = mesh.CreateSubdivisionSchemeAttr()
        XCTAssertTrue(subdivSchemeAttr.IsValid())
    }

    // MARK: - UsdGeomPoints Tests

    func testPointsDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Points")
        let points = cxxfacade.usdGeom.UsdGeomPoints.Define(&stage, path)

        XCTAssertTrue(points.GetPrim().IsValid())
        XCTAssertEqual(String(cString: points.GetPrim().GetTypeName().GetText()), "Points")
    }

    func testPointsAttributes() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var points = cxxfacade.usdGeom.UsdGeomPoints.Define(&stage, cxxfacade.sdf.SdfPath("/Points"))

        let pointsAttr = points.CreatePointsAttr()
        XCTAssertTrue(pointsAttr.IsValid())

        let widthsAttr = points.CreateWidthsAttr()
        XCTAssertTrue(widthsAttr.IsValid())

        let idsAttr = points.CreateIdsAttr()
        XCTAssertTrue(idsAttr.IsValid())
    }

    // MARK: - UsdGeomBasisCurves Tests

    func testBasisCurvesDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Curves")
        let curves = cxxfacade.usdGeom.UsdGeomBasisCurves.Define(&stage, path)

        XCTAssertTrue(curves.GetPrim().IsValid())
        XCTAssertEqual(String(cString: curves.GetPrim().GetTypeName().GetText()), "BasisCurves")
    }

    func testBasisCurvesAttributes() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var curves = cxxfacade.usdGeom.UsdGeomBasisCurves.Define(&stage, cxxfacade.sdf.SdfPath("/Curves"))

        let curveVertexCountsAttr = curves.CreateCurveVertexCountsAttr()
        XCTAssertTrue(curveVertexCountsAttr.IsValid())

        let typeAttr = curves.CreateTypeAttr()
        XCTAssertTrue(typeAttr.IsValid())

        let basisAttr = curves.CreateBasisAttr()
        XCTAssertTrue(basisAttr.IsValid())

        let wrapAttr = curves.CreateWrapAttr()
        XCTAssertTrue(wrapAttr.IsValid())
    }

    // MARK: - UsdGeomNurbsCurves Tests

    func testNurbsCurvesDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/NurbsCurves")
        let curves = cxxfacade.usdGeom.UsdGeomNurbsCurves.Define(&stage, path)

        XCTAssertTrue(curves.GetPrim().IsValid())
        XCTAssertEqual(String(cString: curves.GetPrim().GetTypeName().GetText()), "NurbsCurves")
    }

    // MARK: - UsdGeomCamera Tests

    func testCameraDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        let path = cxxfacade.sdf.SdfPath("/Camera")
        let camera = cxxfacade.usdGeom.UsdGeomCamera.Define(&stage, path)

        XCTAssertTrue(camera.GetPrim().IsValid())
        XCTAssertEqual(String(cString: camera.GetPrim().GetTypeName().GetText()), "Camera")
    }

    func testCameraAttributes() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var camera = cxxfacade.usdGeom.UsdGeomCamera.Define(&stage, cxxfacade.sdf.SdfPath("/Camera"))

        let projectionAttr = camera.CreateProjectionAttr()
        XCTAssertTrue(projectionAttr.IsValid())

        let focalLengthAttr = camera.CreateFocalLengthAttr()
        XCTAssertTrue(focalLengthAttr.IsValid())

        let horizontalApertureAttr = camera.CreateHorizontalApertureAttr()
        XCTAssertTrue(horizontalApertureAttr.IsValid())

        let clippingRangeAttr = camera.CreateClippingRangeAttr()
        XCTAssertTrue(clippingRangeAttr.IsValid())
    }

    // MARK: - Geometric Primitives Tests

    func testCubeDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var cube = cxxfacade.usdGeom.UsdGeomCube.Define(&stage, cxxfacade.sdf.SdfPath("/Cube"))

        XCTAssertTrue(cube.GetPrim().IsValid())
        let sizeAttr = cube.CreateSizeAttr()
        XCTAssertTrue(sizeAttr.IsValid())
    }

    func testSphereDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var sphere = cxxfacade.usdGeom.UsdGeomSphere.Define(&stage, cxxfacade.sdf.SdfPath("/Sphere"))

        XCTAssertTrue(sphere.GetPrim().IsValid())
        let radiusAttr = sphere.CreateRadiusAttr()
        XCTAssertTrue(radiusAttr.IsValid())
    }

    func testCylinderDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var cylinder = cxxfacade.usdGeom.UsdGeomCylinder.Define(&stage, cxxfacade.sdf.SdfPath("/Cylinder"))

        XCTAssertTrue(cylinder.GetPrim().IsValid())
        let radiusAttr = cylinder.CreateRadiusAttr()
        XCTAssertTrue(radiusAttr.IsValid())
        let heightAttr = cylinder.CreateHeightAttr()
        XCTAssertTrue(heightAttr.IsValid())
        let axisAttr = cylinder.CreateAxisAttr()
        XCTAssertTrue(axisAttr.IsValid())
    }

    func testCapsuleDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var capsule = cxxfacade.usdGeom.UsdGeomCapsule.Define(&stage, cxxfacade.sdf.SdfPath("/Capsule"))

        XCTAssertTrue(capsule.GetPrim().IsValid())
        let radiusAttr = capsule.CreateRadiusAttr()
        XCTAssertTrue(radiusAttr.IsValid())
    }

    func testConeDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var cone = cxxfacade.usdGeom.UsdGeomCone.Define(&stage, cxxfacade.sdf.SdfPath("/Cone"))

        XCTAssertTrue(cone.GetPrim().IsValid())
        let radiusAttr = cone.CreateRadiusAttr()
        XCTAssertTrue(radiusAttr.IsValid())
    }

    // MARK: - UsdGeomImageable Tests

    func testImageableVisibility() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, cxxfacade.sdf.SdfPath("/World"))

        let visibilityAttr = xform.CreateVisibilityAttr()
        XCTAssertTrue(visibilityAttr.IsValid())
    }

    func testImageablePurpose() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var xform = cxxfacade.usdGeom.UsdGeomXform.Define(&stage, cxxfacade.sdf.SdfPath("/World"))

        let purposeAttr = xform.CreatePurposeAttr()
        XCTAssertTrue(purposeAttr.IsValid())
    }

    // MARK: - Token Tests

    func testGeomTokens() {
        // Test that token functions work
        let defaultToken = cxxfacade.usdGeom.UsdGeomTokens.default_()
        XCTAssertTrue(strlen(defaultToken.GetText()) > 0)

        let renderToken = cxxfacade.usdGeom.UsdGeomTokens.render()
        XCTAssertTrue(strlen(renderToken.GetText()) > 0)

        let proxyToken = cxxfacade.usdGeom.UsdGeomTokens.proxy()
        XCTAssertTrue(strlen(proxyToken.GetText()) > 0)
    }

    // MARK: - UsdGeomPrimvar Tests

    func testPrimvarDefine() {
        var stage = cxxfacade.usd.UsdStage.CreateInMemory()
        var mesh = cxxfacade.usdGeom.UsdGeomMesh.Define(&stage, cxxfacade.sdf.SdfPath("/Mesh"))
        let prim = mesh.GetPrim()

        let api = cxxfacade.usdGeom.UsdGeomPrimvarsAPI(prim)
        XCTAssertTrue(api.GetPrim().IsValid())
    }
}
