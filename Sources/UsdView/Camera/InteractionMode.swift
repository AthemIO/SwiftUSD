/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * This software is Licensed under the terms of the Apache License,
 * version 2.0 (the "Apache License") with the following additional
 * modification; you may not use this file except within compliance
 * of the Apache License and the following modification made to it.
 * Section 6. Trademarks. is deleted and replaced with:
 *
 * Trademarks. This License does not grant permission to use any of
 * its trade names, trademarks, service marks, or the product names
 * of this Licensor or its affiliates, except as required to comply
 * with Section 4(c.) of this License, and to reproduce the content
 * of the NOTICE file.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND without even an
 * implied warranty of MERCHANTABILITY, or FITNESS FOR A PARTICULAR
 * PURPOSE. See the Apache License for more details.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
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
