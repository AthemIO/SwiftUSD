/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar

// MARK: - ArNotice Type Aliases

/// Typealias for ArNotice namespace.
public typealias ArNotice = Pixar.ArNotice

/// Typealias for ResolverNotice (base class for resolver notifications).
public typealias ArResolverNotice = Pixar.ArNotice.ResolverNotice

/// Typealias for ResolverChanged notification.
public typealias ArResolverChanged = Pixar.ArNotice.ResolverChanged

// MARK: - Ar.Notice Namespace

public extension Ar
{
  /// Container for asset resolver notification types.
  ///
  /// ArNotice provides a notification system for informing clients
  /// about changes to the asset resolver that may affect previously
  /// resolved paths.
  enum Notice
  {
    /// Base class for all ArResolver-related notices.
    public typealias ResolverNotice = ArResolverNotice

    /// Notice sent when asset paths may resolve to different paths
    /// than before due to a change in the resolver.
    ///
    /// Clients that resolve paths should listen for this notice
    /// and re-resolve any cached paths when received.
    public typealias ResolverChanged = ArResolverChanged
  }
}

// MARK: - ArResolverChanged Extensions

public extension ArResolverChanged
{
  /// Returns true if the results of asset resolution when the given
  /// context is bound may be affected by this resolver change.
  ///
  /// - Parameter ctx: The context to check
  /// - Returns: True if this change affects the given context
  func affectsContext(_ ctx: ArResolverContext) -> Bool
  {
    AffectsContext(ctx)
  }
}
