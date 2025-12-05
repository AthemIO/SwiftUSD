/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

// #if os(Linux) || os(Windows)
//   import SwiftCrossUI
// #endif
import SwiftCrossUI

/* setup the platform backend. */

#if os(Linux)
  import GtkBackend

  public typealias PlatformBackend = GtkBackend
#elseif os(Windows)
  import WinUIBackend

  public typealias PlatformBackend = WinUIBackend
#elseif os(macOS)
  import AppKitBackend

  public typealias PlatformBackend = AppKitBackend
#else
  import UIKitBackend

  public typealias PlatformBackend = UIKitBackend
#endif
