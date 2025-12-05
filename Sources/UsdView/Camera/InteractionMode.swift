/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Foundation

/// Represents the current camera interaction mode based on user input.
/// Used to determine how mouse movements affect the camera.
public enum InteractionMode
{
  /// No interaction is active
  case none

  /// Orbit mode: Rotate camera around focus point (Alt + Left Mouse)
  case orbit

  /// Pan mode: Translate focus point in screen space (Shift + Left Mouse)
  case pan

  /// Zoom mode: Adjust camera distance (Alt + Right Mouse or Scroll Wheel)
  case zoom

  /// Free-look mode: First-person camera rotation (not currently implemented)
  case freeLook
}
