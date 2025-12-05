/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
@_exported import pxr

/* --- xxx --- */

public extension Pixar
{
  /**
   * The current version of ``PixarUSD``.
   *
   * The semantic versioning used for ``PixarUSD`` tracks both the upstream
   * Pixar USD version, as well as the evolution iteration of the ``PixarUSD``
   * SwiftPM package, which is setup as follows:
   * - ``PXR_MINOR_VERSION``.``PXR_PATCH_VERSION``.``SWIFTUSD_EVOLUTION`` */
  static let version = "25.11.\(SWIFTUSD_EVOLUTION)"
}

/* --- xxx --- */
