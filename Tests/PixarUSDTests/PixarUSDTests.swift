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
