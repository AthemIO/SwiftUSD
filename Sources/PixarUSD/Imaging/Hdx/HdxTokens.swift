/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

@preconcurrency import Hdx

private extension Hdx
{
  /**
   * Private struct to hold the static
   * data for the Hdx library. */
  struct StaticData: @unchecked Sendable
  {
    static let shared = StaticData()
    private init()
    {}

    let tokens = Pixar.HdxColorCorrectionTokens_StaticTokenType()
  }
}

public extension Hdx
{
  /**
   * # Hdx.Tokens
   *
   * ## Overview
   *
   * Public, client facing api to access
   * the static Hdx tokens. */
  enum ColorCorrectionTokens: String, CaseIterable
  {
    case sRGB

    public var token: Tf.Token
    {
      switch self
      {
        case .sRGB: StaticData.shared.tokens.sRGB
      }
    }
  }
}

public extension Tf.Token
{
  static let sRGB = Hdx.ColorCorrectionTokens.sRGB.token
}
