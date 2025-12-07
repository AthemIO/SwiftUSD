/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import XCTest
@testable import PixarUSD

/* ---- xxx ----
 *  ARCH  TESTS
 * ---- xxx ---- */

final class ArchTests: XCTestCase
{
  func testCwd()
  {
    Msg.logger.log(level: .info, "Arch.getCwd() -> \(Arch.getCwd())")
  }

  func testExecutablePath()
  {
    Msg.logger.log(level: .info, "Arch.getExecutablePath() -> \(Arch.getExecutablePath())")
  }

  func testPageSize()
  {
    Msg.logger.log(level: .info, "Arch.getPageSize() -> \(Arch.getPageSize())")
  }

  func testIsMainThread()
  {
    Msg.logger.log(level: .info, "Arch.isMainThread() -> \(Arch.isMainThread())")
  }

  func testAlignMemoryOfSize()
  {
    let size = 1024
    let alignedPtr = Arch.alignMemory(of: size)
    XCTAssert(alignedPtr >= 1024)
  }

  func testAlignMemoryOfPointer()
  {
    let size = 1024
    let alignment = 16
    let ptr = UnsafeMutableRawPointer.allocate(byteCount: size, alignment: alignment)
    defer { ptr.deallocate() }
    let alignedPtr = Arch.alignMemory(of: ptr)
    XCTAssert(alignedPtr != nil)
  }

  func testAlignedAlloc()
  {
    let size = 1024
    let alignment = 16
    let alignedPtr = Arch.alignedAlloc(byteCount: size, alignment: alignment)
    XCTAssert(alignedPtr != nil)
  }

  func testAlignedFree()
  {
    let size = 1024
    let alignment = 16
    var alignedPtr = Arch.alignedAlloc(byteCount: size, alignment: alignment)
    XCTAssert(alignedPtr != nil)
    Arch.alignedFree(pointer: &alignedPtr)
    XCTAssert(alignedPtr == nil)
  }
}

/* ---- xxx ----
 *   GF  TESTS
 * ---- xxx ---- */

final class GfTests: XCTestCase
{
  func testVec2f()
  {
    let vec2f = Gf.Vec2f(1.0, 2.0)
    Msg.logger.log(level: .info, "Gf.Vec2f -> \(vec2f)")

    let gfVec2f = GfVec2f(1.0, 2.0)
    Msg.logger.log(level: .info, "GfVec2f -> \(gfVec2f)")

    XCTAssertEqual(vec2f, gfVec2f)
    XCTAssertEqual(MemoryLayout<GfVec2f>.size, MemoryLayout<(Float, Float)>.size)
  }

  func testVec3f()
  {
    let vec3fA = GfVec3f(1.0, 2.0, 3.0)
    Msg.logger.log(level: .info, "GfVec3f(1.0, 2.0, 3.0) -> \(vec3fA)")

    let vec3fB = GfVec3f(4.0, 5.0, 6.0)
    Msg.logger.log(level: .info, "GfVec3f(4.0, 5.0, 6.0) -> \(vec3fB)")

    var vec3fC = vec3fA + vec3fB
    Msg.logger.log(level: .info, "vec3fC=(vec3fA + vec3fB) -> \(vec3fC)")

    vec3fC *= vec3fA
    Msg.logger.log(level: .info, "vec3fC*=(vec3fA) -> \(vec3fC)")

    Msg.logger.log(level: .info, "MemoryLayout<SIMD3<Float>>.size -> \(MemoryLayout<SIMD3<Float>>.size)")
    Msg.logger.log(level: .info, "MemoryLayout<GfVec3f>.size -> \(MemoryLayout<GfVec3f>.size)")
    Msg.logger.log(level: .info, "MemoryLayout<(Float, Float, Float)>.size -> \(MemoryLayout<(Float, Float, Float)>.size)")

    XCTAssertEqual(MemoryLayout<GfVec3f>.size, MemoryLayout<(Float, Float, Float)>.size)
  }
}

/* ---- xxx ----
 *   AR  TESTS
 * ---- xxx ---- */

final class ArTests: XCTestCase
{
  func testGetAllResolvers()
  {
    let resolvers = Ar.getAllResolvers()

    if resolvers.isEmpty
    {
      Msg.logger.log(level: .info, "Ar.getAllResolvers() -> None")
    }

    for (i, r) in resolvers.enumerated()
    {
      Msg.logger.log(level: .info, "Ar.getAllResolvers() [\(i + 1)/\(resolvers.count)] -> \(r)")
    }
  }
}

/* ---- xxx ----
 *  KIND  TESTS
 * ---- xxx ---- */

final class KindTests: XCTestCase
{
  func testKindTokens()
  {
    let tokens = Kind.Tokens.allCases

    for (i, t) in tokens.enumerated()
    {
      Msg.logger.log(level: .info, "Kind.Tokens [\(i + 1)/\(tokens.count)] -> \(t.getToken().string)")
    }

    XCTAssertEqual(tokens.count, 5)
  }
}

/* ---- xxx ----
 *  USDVALIDATION TESTS
 * ---- xxx ---- */

final class UsdValidationTests: XCTestCase
{
  // MARK: - ErrorSeverity Tests

  func testErrorSeverityEnumValues()
  {
    // Test that all severity enum cases have correct raw values
    XCTAssertEqual(UsdValidation.ErrorSeverity.none.rawValue, 0)
    XCTAssertEqual(UsdValidation.ErrorSeverity.error.rawValue, 1)
    XCTAssertEqual(UsdValidation.ErrorSeverity.warn.rawValue, 2)
    XCTAssertEqual(UsdValidation.ErrorSeverity.info.rawValue, 3)

    Msg.logger.log(level: .info, "UsdValidation.ErrorSeverity cases: \(UsdValidation.ErrorSeverity.allCases.count)")
  }

  func testErrorSeverityDescriptions()
  {
    XCTAssertEqual(UsdValidation.ErrorSeverity.none.description, "None")
    XCTAssertEqual(UsdValidation.ErrorSeverity.error.description, "Error")
    XCTAssertEqual(UsdValidation.ErrorSeverity.warn.description, "Warning")
    XCTAssertEqual(UsdValidation.ErrorSeverity.info.description, "Info")
  }

  func testErrorSeverityCxxConversion()
  {
    // Test round-trip conversion from C++ type
    let errorSeverity = UsdValidation.ErrorSeverity(.Error)
    XCTAssertEqual(errorSeverity, .error)

    // Test conversion back to C++ type
    let cxxType = UsdValidation.ErrorSeverity.warn.cxxType
    XCTAssertEqual(cxxType, .Warn)
  }

  // MARK: - Validator Token Tests

  func testValidatorNames()
  {
    let names = UsdValidation.ValidatorNames.allCases
    XCTAssertEqual(names.count, 2)

    for name in names
    {
      let token = name.getToken()
      XCTAssertFalse(token.string.isEmpty)
      Msg.logger.log(level: .info, "UsdValidation.ValidatorNames -> \(token.string)")
    }

    // Test specific token values
    XCTAssertEqual(
      UsdValidation.ValidatorNames.compositionErrorTest.getToken().string,
      "usdValidation:CompositionErrorTest"
    )
    XCTAssertEqual(
      UsdValidation.ValidatorNames.stageMetadataChecker.getToken().string,
      "usdValidation:StageMetadataChecker"
    )
  }

  func testValidatorKeywords()
  {
    let keywords = UsdValidation.ValidatorKeywords.allCases
    XCTAssertEqual(keywords.count, 1)

    let token = UsdValidation.ValidatorKeywords.usdCoreValidators.getToken()
    XCTAssertEqual(token.string, "UsdCoreValidators")
    Msg.logger.log(level: .info, "UsdValidation.ValidatorKeywords.usdCoreValidators -> \(token.string)")
  }

  func testErrorNames()
  {
    let names = UsdValidation.ErrorNames.allCases
    XCTAssertEqual(names.count, 2)

    XCTAssertEqual(UsdValidation.ErrorNames.compositionError.getToken().string, "CompositionError")
    XCTAssertEqual(UsdValidation.ErrorNames.missingDefaultPrim.getToken().string, "MissingDefaultPrim")
  }

  // MARK: - TimeRange Tests

  func testTimeRangeFull()
  {
    _ = UsdValidation.TimeRange.full()
    // TimeRange.full() creates a default time range
    Msg.logger.log(level: .info, "UsdValidation.TimeRange.full() created successfully")
  }

  // MARK: - Registry Tests

  func testRegistryHasValidator()
  {
    // Test checking for a known validator
    let hasCompositionTest = UsdValidation.Registry.hasValidator(
      named: "usdValidation:CompositionErrorTest"
    )
    Msg.logger.log(level: .info, "Registry.hasValidator(CompositionErrorTest) -> \(hasCompositionTest)")

    // Test checking for a non-existent validator
    let hasNonExistent = UsdValidation.Registry.hasValidator(named: "nonexistent:Validator")
    XCTAssertFalse(hasNonExistent)
  }

  func testRegistryGetAllValidatorMetadata()
  {
    let metadata = UsdValidation.Registry.getAllValidatorMetadata()
    Msg.logger.log(level: .info, "Registry.getAllValidatorMetadata() count -> \(metadata.size())")

    // There should be at least some validators registered
    for i in 0 ..< min(metadata.size(), 5)
    {
      let m = metadata[i]
      Msg.logger.log(level: .info, "  Validator[\(i)]: \(m.validatorName.string)")
    }
  }

  func testRegistryGetValidatorMetadataForKeyword()
  {
    let keyword = UsdValidation.ValidatorKeywords.usdCoreValidators.getToken()
    let metadata = UsdValidation.Registry.getValidatorMetadata(forKeyword: keyword)

    Msg.logger.log(
      level: .info,
      "Registry.getValidatorMetadata(forKeyword: UsdCoreValidators) count -> \(metadata.size())"
    )
  }

  // MARK: - Context Tests

  func testContextCreationFromKeywords()
  {
    let keywords = [UsdValidation.ValidatorKeywords.usdCoreValidators.getToken()]
    let context = UsdValidation.Context(keywords: keywords)

    XCTAssertTrue(context.isValid)
    XCTAssertTrue(context.includeAllAncestors)
    Msg.logger.log(level: .info, "UsdValidation.Context created from keywords successfully")
  }

  func testContextValidateStage()
  {
    // Create a simple stage
    let stage = Usd.Stage.createInMemory()

    // Create a context with core validators
    let keywords = [UsdValidation.ValidatorKeywords.usdCoreValidators.getToken()]
    let context = UsdValidation.Context(keywords: keywords)

    // Validate the stage
    let errors = context.validate(stage: stage)
    Msg.logger.log(level: .info, "Context.validate(stage:) returned \(errors.size()) errors")
  }

  // MARK: - Validator Tests

  func testValidatorGet()
  {
    let validatorName = UsdValidation.ValidatorNames.stageMetadataChecker.getToken()
    if let validator = UsdValidation.Validator.get(named: validatorName)
    {
      XCTAssertFalse(validator.name.IsEmpty())
      Msg.logger.log(level: .info, "Validator.get(named:) -> \(validator.name.string)")
      Msg.logger.log(level: .info, "  documentation: \(validator.documentation)")
      Msg.logger.log(level: .info, "  isTimeDependent: \(validator.isTimeDependent)")
    }
    else
    {
      Msg.logger.log(level: .info, "Validator not found (plugins may not be loaded)")
    }
  }
}

/* ---- xxx ----
 *  PXOSD TESTS
 * ---- xxx ---- */

final class PxOsdTests: XCTestCase
{
  // MARK: - Token Tests

  func testPxOsdTokens()
  {
    let tokens = PxOsd.Tokens.allCases
    Msg.logger.log(level: .info, "PxOsd.Tokens count: \(tokens.count)")

    for (i, token) in tokens.enumerated()
    {
      let tfToken = token.getToken()
      XCTAssertFalse(tfToken.string.isEmpty)
      Msg.logger.log(level: .info, "PxOsd.Tokens[\(i)] -> \(tfToken.string)")
    }

    // Test specific token values
    XCTAssertEqual(PxOsd.Tokens.catmullClark.getToken().string, "catmullClark")
    XCTAssertEqual(PxOsd.Tokens.loop.getToken().string, "loop")
    XCTAssertEqual(PxOsd.Tokens.bilinear.getToken().string, "bilinear")
    XCTAssertEqual(PxOsd.Tokens.rightHanded.getToken().string, "rightHanded")
    XCTAssertEqual(PxOsd.Tokens.leftHanded.getToken().string, "leftHanded")
  }

  // MARK: - SubdivisionScheme Tests

  func testSubdivisionScheme()
  {
    // Test token conversion
    XCTAssertEqual(PxOsd.SubdivisionScheme.catmullClark.token.string, "catmullClark")
    XCTAssertEqual(PxOsd.SubdivisionScheme.loop.token.string, "loop")
    XCTAssertEqual(PxOsd.SubdivisionScheme.bilinear.token.string, "bilinear")
    XCTAssertEqual(PxOsd.SubdivisionScheme.none.token.string, "none")

    // Test init from token
    XCTAssertEqual(PxOsd.SubdivisionScheme(token: Tf.Token("catmullClark")), .catmullClark)
    XCTAssertEqual(PxOsd.SubdivisionScheme(token: Tf.Token("loop")), .loop)
    XCTAssertNil(PxOsd.SubdivisionScheme(token: Tf.Token("invalid")))
  }

  // MARK: - Orientation Tests

  func testOrientation()
  {
    XCTAssertEqual(PxOsd.Orientation.leftHanded.token.string, "leftHanded")
    XCTAssertEqual(PxOsd.Orientation.rightHanded.token.string, "rightHanded")

    XCTAssertEqual(PxOsd.Orientation(token: Tf.Token("leftHanded")), .leftHanded)
    XCTAssertEqual(PxOsd.Orientation(token: Tf.Token("rightHanded")), .rightHanded)
    XCTAssertNil(PxOsd.Orientation(token: Tf.Token("invalid")))
  }

  // MARK: - BoundaryInterpolation Tests

  func testBoundaryInterpolation()
  {
    XCTAssertEqual(PxOsd.BoundaryInterpolation.none.token.string, "none")
    XCTAssertEqual(PxOsd.BoundaryInterpolation.cornersOnly.token.string, "cornersOnly")
    XCTAssertEqual(PxOsd.BoundaryInterpolation.cornersPlus1.token.string, "cornersPlus1")
    XCTAssertEqual(PxOsd.BoundaryInterpolation.cornersPlus2.token.string, "cornersPlus2")
    XCTAssertEqual(PxOsd.BoundaryInterpolation.all.token.string, "all")

    XCTAssertEqual(PxOsd.BoundaryInterpolation(token: Tf.Token("cornersPlus1")), .cornersPlus1)
    XCTAssertNil(PxOsd.BoundaryInterpolation(token: Tf.Token("invalid")))
  }

  // MARK: - CreaseMethod Tests

  func testCreaseMethod()
  {
    XCTAssertEqual(PxOsd.CreaseMethod.uniform.token.string, "uniform")
    XCTAssertEqual(PxOsd.CreaseMethod.chaikin.token.string, "chaikin")

    XCTAssertEqual(PxOsd.CreaseMethod(token: Tf.Token("uniform")), .uniform)
    XCTAssertEqual(PxOsd.CreaseMethod(token: Tf.Token("chaikin")), .chaikin)
    XCTAssertNil(PxOsd.CreaseMethod(token: Tf.Token("invalid")))
  }

  // MARK: - MeshTopology Tests

  func testMeshTopologyCreate()
  {
    // Create a simple quad mesh topology
    var faceVertexCounts = Vt.IntArray()
    faceVertexCounts.push_back(4) // One quad

    var faceVertexIndices = Vt.IntArray()
    faceVertexIndices.push_back(0)
    faceVertexIndices.push_back(1)
    faceVertexIndices.push_back(2)
    faceVertexIndices.push_back(3)

    let topology = PxOsd.MeshTopology.create(
      scheme: .catmullClark,
      orientation: .rightHanded,
      faceVertexCounts: faceVertexCounts,
      faceVertexIndices: faceVertexIndices
    )

    // Verify topology is valid
    XCTAssertTrue(PxOsd.isValid(topology))
    Msg.logger.log(level: .info, "PxOsd.MeshTopology created and validated successfully")

    // Verify accessors
    let counts = PxOsd.getFaceVertexCounts(topology)
    XCTAssertEqual(counts.size(), 1)

    let indices = PxOsd.getFaceVertexIndices(topology)
    XCTAssertEqual(indices.size(), 4)

    let orientation = PxOsd.getOrientation(topology)
    XCTAssertEqual(orientation.string, "rightHanded")
  }

  func testMeshTopologyWithHoles()
  {
    var faceVertexCounts = Vt.IntArray()
    faceVertexCounts.push_back(4)
    faceVertexCounts.push_back(4)

    var faceVertexIndices = Vt.IntArray()
    for i: Int32 in 0 ..< 8
    {
      faceVertexIndices.push_back(i)
    }

    var holeIndices = Vt.IntArray()
    holeIndices.push_back(1) // Second face is a hole

    let topology = PxOsd.MeshTopology.create(
      scheme: .catmullClark,
      orientation: .rightHanded,
      faceVertexCounts: faceVertexCounts,
      faceVertexIndices: faceVertexIndices,
      holeIndices: holeIndices
    )

    let holes = PxOsd.getHoleIndices(topology)
    XCTAssertEqual(holes.size(), 1)
    Msg.logger.log(level: .info, "PxOsd.MeshTopology with holes created successfully")
  }

  // MARK: - SubdivTags Tests

  func testSubdivTagsCreate()
  {
    let tags = PxOsd.SubdivTags.create(
      vertexInterpolation: .cornersPlus1,
      faceVaryingInterpolation: .cornersOnly,
      creaseMethod: .uniform
    )

    // Verify empty crease data
    let creaseIndices = PxOsd.getCreaseIndices(tags)
    XCTAssertEqual(creaseIndices.size(), 0)

    Msg.logger.log(level: .info, "PxOsd.SubdivTags created successfully")
  }

  func testSubdivTagsWithCreases()
  {
    var creaseIndices = Vt.IntArray()
    creaseIndices.push_back(0)
    creaseIndices.push_back(1)

    var creaseLengths = Vt.IntArray()
    creaseLengths.push_back(2)

    var creaseWeights = Vt.FloatArray()
    creaseWeights.push_back(2.0) // Sharp crease

    let tags = PxOsd.SubdivTags.create(
      vertexInterpolation: .cornersPlus1,
      faceVaryingInterpolation: .cornersOnly,
      creaseMethod: .uniform,
      creaseIndices: creaseIndices,
      creaseLengths: creaseLengths,
      creaseWeights: creaseWeights
    )

    let indices = PxOsd.getCreaseIndices(tags)
    XCTAssertEqual(indices.size(), 2)

    let weights = PxOsd.getCreaseWeights(tags)
    XCTAssertEqual(weights.size(), 1)

    Msg.logger.log(level: .info, "PxOsd.SubdivTags with creases created successfully")
  }

  // MARK: - ValidationCode Tests

  func testValidationCodeDescriptions()
  {
    let codes = PxOsd.ValidationCode.allCases
    Msg.logger.log(level: .info, "PxOsd.ValidationCode count: \(codes.count)")

    for code in codes
    {
      XCTAssertFalse(code.description.isEmpty)
    }

    // Test specific descriptions
    XCTAssertEqual(PxOsd.ValidationCode.invalidScheme.description, "Invalid subdivision scheme")
    XCTAssertEqual(PxOsd.ValidationCode.invalidOrientation.description, "Invalid orientation")
  }
}

/* ---- xxx ----
 *  USDMTLX TESTS
 * ---- xxx ---- */

final class UsdMtlxTests: XCTestCase
{
  // MARK: - Token Tests

  func testMtlxTokens()
  {
    let tokens = UsdMtlx.Tokens.allCases
    XCTAssertEqual(tokens.count, 3)

    for token in tokens
    {
      let tfToken = token.getToken()
      XCTAssertFalse(tfToken.string.isEmpty)
      Msg.logger.log(level: .info, "UsdMtlx.Tokens -> \(tfToken.string)")
    }

    // Test specific token values
    XCTAssertEqual(UsdMtlx.Tokens.configMtlxVersion.getToken().string, "config:mtlx:version")
    XCTAssertEqual(UsdMtlx.Tokens.defaultOutputName.getToken().string, "out")
    XCTAssertEqual(UsdMtlx.Tokens.materialXConfigAPI.getToken().string, "MaterialXConfigAPI")
  }

  // MARK: - Search Path Tests

  func testStandardLibraryPaths()
  {
    let paths = UsdMtlx.standardLibraryPaths()
    Msg.logger.log(level: .info, "UsdMtlx.standardLibraryPaths() count: \(paths.count)")

    for (i, path) in paths.enumerated()
    {
      Msg.logger.log(level: .info, "  standardLibraryPaths[\(i)]: \(path)")
    }

    // Should have at least one standard library path when properly configured
    // (may be empty if MaterialX not properly bundled)
  }

  func testSearchPaths()
  {
    let paths = UsdMtlx.searchPaths()
    Msg.logger.log(level: .info, "UsdMtlx.searchPaths() count: \(paths.count)")

    for (i, path) in paths.enumerated()
    {
      Msg.logger.log(level: .info, "  searchPaths[\(i)]: \(path)")
    }
  }

  func testCustomSearchPaths()
  {
    let paths = UsdMtlx.customSearchPaths()
    Msg.logger.log(level: .info, "UsdMtlx.customSearchPaths() count: \(paths.count)")
    // Custom paths are set via environment variable, may be empty
  }

  func testStandardFileExtensions()
  {
    let extensions = UsdMtlx.standardFileExtensions()
    Msg.logger.log(level: .info, "UsdMtlx.standardFileExtensions() count: \(extensions.count)")

    for ext in extensions
    {
      Msg.logger.log(level: .info, "  extension: \(ext)")
    }

    // Should include .mtlx
    XCTAssertTrue(extensions.contains("mtlx") || extensions.contains(".mtlx"))
  }

  // MARK: - Type Conversion Tests

  func testGetUsdType()
  {
    // Test common MaterialX type conversions
    let floatInfo = UsdMtlx.getUsdType("float")
    Msg.logger.log(level: .info, "UsdMtlx.getUsdType('float') -> arraySize: \(floatInfo.arraySize)")

    let color3Info = UsdMtlx.getUsdType("color3")
    Msg.logger.log(level: .info, "UsdMtlx.getUsdType('color3') -> arraySize: \(color3Info.arraySize)")

    let vector3Info = UsdMtlx.getUsdType("vector3")
    Msg.logger.log(level: .info, "UsdMtlx.getUsdType('vector3') -> arraySize: \(vector3Info.arraySize)")
  }

  // MARK: - String Utilities Tests

  func testSplitStringArray()
  {
    // Test splitting a MaterialX string array
    let result = UsdMtlx.splitStringArray("one, two, three")
    XCTAssertEqual(result.count, 3)
    XCTAssertEqual(result[0], "one")
    XCTAssertEqual(result[1], "two")
    XCTAssertEqual(result[2], "three")

    Msg.logger.log(level: .info, "UsdMtlx.splitStringArray() -> \(result)")
  }

  func testSplitStringArrayEmpty()
  {
    let result = UsdMtlx.splitStringArray("")
    XCTAssertEqual(result.count, 0)
  }

  // MARK: - MaterialX Conversion Tests

  func testMtlxStringConversion()
  {
    // Test converting a simple MaterialX XML to USD
    let mtlxXml = """
      <?xml version="1.0"?>
      <materialx version="1.38">
        <constant name="test_constant" type="color3">
          <input name="value" type="color3" value="1.0, 0.0, 0.0"/>
        </constant>
      </materialx>
      """

    let stage = UsdMtlx.testString(mtlxXml)

    // Verify stage was created
    XCTAssertNotNil(stage)
    Msg.logger.log(level: .info, "UsdMtlx.testString() created stage successfully")

    // Traverse the stage to see what was created
    for prim in stage.traverse()
    {
      Msg.logger.log(level: .info, "  Prim: \(prim.GetPath())")
    }
  }

  func testMtlxStringConversionNodeGraphsOnly()
  {
    let mtlxXml = """
      <?xml version="1.0"?>
      <materialx version="1.38">
        <nodegraph name="test_nodegraph">
          <constant name="test_constant" type="color3">
            <input name="value" type="color3" value="1.0, 0.0, 0.0"/>
          </constant>
        </nodegraph>
      </materialx>
      """

    let stage = UsdMtlx.testString(mtlxXml, nodeGraphsOnly: true)
    XCTAssertNotNil(stage)
    Msg.logger.log(level: .info, "UsdMtlx.testString(nodeGraphsOnly: true) created stage successfully")
  }
}

/* ---- xxx ----
 *  AR ENHANCED TESTS
 * ---- xxx ---- */

final class ArEnhancedTests: XCTestCase
{
  // MARK: - Resolver Tests

  func testGetResolver()
  {
    let resolver = Ar.getResolver()
    // ArResolver is a reference type - just verify we can access it
    Msg.logger.log(level: .info, "Ar.getResolver() returned successfully")

    // Verify we can access methods
    let context = resolver.createDefaultContext()
    Msg.logger.log(level: .info, "createDefaultContext() returned")
    _ = context
  }

  func testResolverCreateIdentifier()
  {
    let resolver = Ar.getResolver()

    // Test creating an identifier
    let identifier = resolver.createIdentifier("test.usda")
    XCTAssertFalse(identifier.isEmpty)
    Msg.logger.log(level: .info, "createIdentifier('test.usda') -> \(identifier)")
  }

  func testResolverCreateIdentifierForNewAsset()
  {
    let resolver = Ar.getResolver()

    let identifier = resolver.createIdentifierForNewAsset("newfile.usda")
    XCTAssertFalse(identifier.isEmpty)
    Msg.logger.log(level: .info, "createIdentifierForNewAsset('newfile.usda') -> \(identifier)")
  }

  func testResolverResolve()
  {
    let resolver = Ar.getResolver()

    // Resolve a non-existent file - should return empty path
    let resolvedPath = resolver.resolve("nonexistent_file_12345.usda")
    XCTAssertTrue(resolvedPath.isEmpty)
    Msg.logger.log(level: .info, "resolve('nonexistent') -> isEmpty: \(resolvedPath.isEmpty)")
  }

  func testResolverResolveForNewAsset()
  {
    let resolver = Ar.getResolver()

    // resolveForNewAsset returns a path where an asset could be created
    let newPath = resolver.resolveForNewAsset("/tmp/test_new_asset.usda")
    Msg.logger.log(level: .info, "resolveForNewAsset -> path: \(newPath.path)")
  }

  func testResolverGetExtension()
  {
    let resolver = Ar.getResolver()

    let ext = resolver.getExtension("myfile.usda")
    XCTAssertEqual(ext, "usda")

    let ext2 = resolver.getExtension("another.usd")
    XCTAssertEqual(ext2, "usd")

    Msg.logger.log(level: .info, "getExtension tests passed")
  }

  func testResolverIsContextDependentPath()
  {
    let resolver = Ar.getResolver()

    let isDependent = resolver.isContextDependentPath("test.usda")
    Msg.logger.log(level: .info, "isContextDependentPath('test.usda') -> \(isDependent)")
    // Most simple paths are not context-dependent with the default resolver
  }

  // MARK: - ResolvedPath Tests

  func testResolvedPathCreation()
  {
    // Test empty path
    let emptyPath = ArResolvedPath.empty()
    XCTAssertTrue(emptyPath.isEmpty)
    XCTAssertFalse(emptyPath.isValid)

    // Test path from string
    let path = ArResolvedPath("/some/path/to/file.usda")
    XCTAssertFalse(path.isEmpty)
    XCTAssertTrue(path.isValid)
    XCTAssertEqual(path.path, "/some/path/to/file.usda")

    Msg.logger.log(level: .info, "ArResolvedPath creation tests passed")
  }

  func testResolvedPathProperties()
  {
    let path = ArResolvedPath("/test/path.usda")

    // Test path accessors
    XCTAssertEqual(path.path, "/test/path.usda")
    XCTAssertEqual(path.pathString, "/test/path.usda")

    // Test hash
    let hashValue = path.hash
    XCTAssert(hashValue != 0)
    Msg.logger.log(level: .info, "ArResolvedPath.hash -> \(hashValue)")

    // Test description
    XCTAssertEqual(path.description, "/test/path.usda")
  }

  func testResolvedPathEquality()
  {
    let path = ArResolvedPath("/test/file.usda")

    // Test equality with string
    XCTAssertTrue(path == "/test/file.usda")
    XCTAssertFalse(path != "/test/file.usda")
    XCTAssertFalse(path == "/other/file.usda")
    XCTAssertTrue(path != "/other/file.usda")

    Msg.logger.log(level: .info, "ArResolvedPath equality tests passed")
  }

  // MARK: - ScopedCache Tests

  func testScopedCacheCreation()
  {
    let cache = Ar.ScopedCache()
    Msg.logger.log(level: .info, "Ar.ScopedCache created successfully")

    // End the cache
    cache.end()
    Msg.logger.log(level: .info, "Ar.ScopedCache ended successfully")
  }

  func testScopedCacheWithScope()
  {
    var wasExecuted = false

    Ar.withCacheScope {
      wasExecuted = true
      // Perform some resolver operations
      let resolver = Ar.getResolver()
      _ = resolver.createIdentifier("test.usda")
    }

    XCTAssertTrue(wasExecuted)
    Msg.logger.log(level: .info, "Ar.withCacheScope executed successfully")
  }

  func testScopedCacheNested()
  {
    let parentCache = Ar.ScopedCache()
    let childCache = Ar.ScopedCache(sharingWith: parentCache)

    Msg.logger.log(level: .info, "Nested ScopedCache created successfully")

    childCache.end()
    parentCache.end()
  }

  // MARK: - DefaultResolver Tests

  func testDefaultResolverSetSearchPath()
  {
    // Set a search path
    Ar.setDefaultSearchPath(["/tmp", "/var/tmp"])
    Msg.logger.log(level: .info, "Ar.setDefaultSearchPath() called successfully")

    // Reset to empty
    Ar.setDefaultSearchPath([])
  }

  // MARK: - Context Tests

  func testResolverContextCreation()
  {
    let resolver = Ar.getResolver()

    // Create default context
    let context = resolver.createDefaultContext()
    Msg.logger.log(level: .info, "Default context created")

    // Create context for a specific asset
    let assetContext = resolver.createDefaultContext(forAsset: "test.usda")
    Msg.logger.log(level: .info, "Asset context created")

    _ = context
    _ = assetContext
  }

  func testResolverContextFromString()
  {
    let resolver = Ar.getResolver()

    // Create context from string (format depends on resolver implementation)
    let context = resolver.createContext(fromString: "")
    Msg.logger.log(level: .info, "Context from string created")
    _ = context
  }
}

/* ---- xxx ----
 *  USDGEOM ENHANCED TESTS
 * ---- xxx ---- */

final class UsdGeomEnhancedTests: XCTestCase
{
  // MARK: - @Xformable Macro Tests

  func testXformableAddTranslateOp()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestTranslate")

    // Add a translate operation using the @Xformable macro-generated method
    let translateOp = xform.addTranslateOp()

    // Verify the op was created and can be used
    XCTAssertTrue(translateOp.set(GfVec3d(10.0, 20.0, 30.0)))
    Msg.logger.log(level: .info, "addTranslateOp() works correctly")
  }

  func testXformableAddScaleOp()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestScale")

    let scaleOp = xform.addScaleOp()

    XCTAssertTrue(scaleOp.set(GfVec3f(2.0, 2.0, 2.0)))
    Msg.logger.log(level: .info, "addScaleOp() works correctly")
  }

  func testXformableAddRotateXOp()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestRotateX")

    let rotateXOp = xform.addRotateXOp()

    XCTAssertTrue(rotateXOp.set(Float(45.0)))
    Msg.logger.log(level: .info, "addRotateXOp() works correctly")
  }

  func testXformableAddRotateYOp()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestRotateY")

    let rotateYOp = xform.addRotateYOp()

    XCTAssertTrue(rotateYOp.set(Float(90.0)))
    Msg.logger.log(level: .info, "addRotateYOp() works correctly")
  }

  func testXformableAddRotateZOp()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestRotateZ")

    let rotateZOp = xform.addRotateZOp()

    XCTAssertTrue(rotateZOp.set(Float(180.0)))
    Msg.logger.log(level: .info, "addRotateZOp() works correctly")
  }

  func testXformableAddRotateXYZOp()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestRotateXYZ")

    let rotateXYZOp = xform.addRotateXYZOp()

    XCTAssertTrue(rotateXYZOp.set(GfVec3f(45.0, 90.0, 180.0)))
    Msg.logger.log(level: .info, "addRotateXYZOp() works correctly")
  }

  func testXformableMultipleOps()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestMultipleOps")

    // Add multiple transform operations in sequence
    let translateOp = xform.addTranslateOp()
    let rotateOp = xform.addRotateXYZOp()
    let scaleOp = xform.addScaleOp()

    // Set values for each
    XCTAssertTrue(translateOp.set(GfVec3d(5.0, 10.0, 15.0)))
    XCTAssertTrue(rotateOp.set(GfVec3f(0.0, 45.0, 0.0)))
    XCTAssertTrue(scaleOp.set(GfVec3f(1.5, 1.5, 1.5)))

    Msg.logger.log(level: .info, "Multiple xform ops added successfully")
  }

  // MARK: - Sphere with @Xformable Tests

  func testSphereXformable()
  {
    let stage = Usd.Stage.createInMemory()
    let sphere = UsdGeom.Sphere.define(stage, path: "/TestSphere")

    // Sphere should also have xformable operations via the @Xformable macro
    let translateOp = sphere.addTranslateOp()
    let scaleOp = sphere.addScaleOp()

    XCTAssertTrue(translateOp.set(GfVec3d(1.0, 2.0, 3.0)))
    XCTAssertTrue(scaleOp.set(GfVec3f(2.0, 2.0, 2.0)))

    Msg.logger.log(level: .info, "Sphere Xformable operations work correctly")
  }

  // MARK: - XformOp Set Methods Tests

  func testXformOpSetFloat()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestSetFloat")

    let rotateOp = xform.addRotateZOp()

    // Test set with Float
    XCTAssertTrue(rotateOp.set(Float(30.0)))
    Msg.logger.log(level: .info, "XformOp.set(Float) works")
  }

  func testXformOpSetDouble()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestSetDouble")

    let rotateOp = xform.addRotateXOp()

    // Test set with Double
    XCTAssertTrue(rotateOp.set(Double(60.0)))
    Msg.logger.log(level: .info, "XformOp.set(Double) works")
  }

  func testXformOpSetVec3f()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestSetVec3f")

    let scaleOp = xform.addScaleOp()

    // Test set with GfVec3f
    XCTAssertTrue(scaleOp.set(GfVec3f(1.0, 2.0, 3.0)))
    Msg.logger.log(level: .info, "XformOp.set(GfVec3f) works")
  }

  func testXformOpSetVec3d()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestSetVec3d")

    let translateOp = xform.addTranslateOp()

    // Test set with GfVec3d
    XCTAssertTrue(translateOp.set(GfVec3d(100.0, 200.0, 300.0)))
    Msg.logger.log(level: .info, "XformOp.set(GfVec3d) works")
  }

  func testXformOpSetWithTime()
  {
    let stage = Usd.Stage.createInMemory()
    let xform = UsdGeom.Xform.define(stage, path: "/TestSetWithTime")

    let translateOp = xform.addTranslateOp()

    // Test setting at different time codes
    XCTAssertTrue(translateOp.set(GfVec3d(0.0, 0.0, 0.0), time: Usd.TimeCode(0.0)))
    XCTAssertTrue(translateOp.set(GfVec3d(10.0, 10.0, 10.0), time: Usd.TimeCode(24.0)))
    XCTAssertTrue(translateOp.set(GfVec3d(20.0, 20.0, 20.0), time: Usd.TimeCode(48.0)))

    Msg.logger.log(level: .info, "XformOp.set() with time works correctly")
  }
}
