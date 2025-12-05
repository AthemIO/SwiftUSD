/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

@preconcurrency import Kind

private extension Kind
{
  /**
   * Private struct to hold the static
   * data for the Kind library. */
  struct StaticData: @unchecked Sendable
  {
    static let shared = StaticData()
    private init()
    {}

    let tokens = Pixar.KindTokens_StaticTokenType()
  }
}

public extension Kind
{
  /**
   * # Kind.Tokens
   *
   * ## Overview
   *
   * Public, client facing api to access
   * the static Kind tokens. */
  enum Tokens: CaseIterable
  {
    case model
    case component
    case group
    case assembly
    case subcomponent

    public func getToken() -> Tf.Token
    {
      switch self
      {
        case .model: StaticData.shared.tokens.model
        case .component: StaticData.shared.tokens.component
        case .group: StaticData.shared.tokens.group
        case .assembly: StaticData.shared.tokens.assembly
        case .subcomponent: StaticData.shared.tokens.subcomponent
      }
    }
  }
}
