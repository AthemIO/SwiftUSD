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
import PixarUSD

#if os(macOS)
    import AppKit
#elseif os(iOS) || os(visionOS)
    import UIKit
#endif

/// Camera controller using eye/at/up vectors (ImGuiHydraEditor approach)
/// This provides direct position control instead of orbit parameters
@Observable
public final class CameraController {
    // MARK: - Properties

    /// Camera eye position (where the camera is)
    public var eye: Pixar.GfVec3d

    /// Camera look-at point (what the camera is looking at)
    public var at: Pixar.GfVec3d

    /// Camera up vector (which way is up for the camera)
    public var up: Pixar.GfVec3d

    /// Sensitivity factors
    public var panSensitivity: Double = 0.01
    public var orbitSensitivity: Double = 0.5
    public var zoomSensitivity: Double = 0.05  // Reduced for smoother zooming

    // MARK: - Initialization

    public init(eye: Pixar.GfVec3d, at: Pixar.GfVec3d, up: Pixar.GfVec3d) {
        self.eye = eye
        self.at = at
        self.up = up
    }

    /// Initialize with default view
    public convenience init(isZUp: Bool = false) {
        let defaultEye = Pixar.GfVec3d(5, 3, 5)
        let defaultAt = Pixar.GfVec3d(0, 0, 0)
        let defaultUp = isZUp ? Pixar.GfVec3d(0, 0, 1) : Pixar.GfVec3d(0, 1, 0)
        self.init(eye: defaultEye, at: defaultAt, up: defaultUp)
    }

    // MARK: - Camera Manipulation

    /// Pan camera by moving both eye and look-at point
    /// Based on ImGuiHydraEditor::_PanActiveCam
    public func pan(delta: CGPoint) {
        let camFront = at - eye
        let camRight = Pixar.GfCross(camFront, up).GetNormalized()
        let camUp = Pixar.GfCross(camRight, camFront).GetNormalized()

        // Invert both X and Y to match expected pan direction
        let panDelta = camRight * (-delta.x * panSensitivity) + camUp * (-delta.y * panSensitivity)

        eye = eye + panDelta
        at = at + panDelta
    }

    /// Orbit camera around the look-at point
    /// Based on ImGuiHydraEditor::_OrbitActiveCam
    public func orbit(delta: CGPoint) {
        // Horizontal rotation around up axis
        let horizRot = Pixar.GfRotation(up, delta.x * orbitSensitivity)
        var rotMatrix = Pixar.GfMatrix4d(1.0)
        rotMatrix.SetRotate(horizRot)
        var eyeRelative = eye - at
        let rotatedVec = rotMatrix.Transform(eyeRelative)
        eye = at + rotatedVec

        // Vertical rotation around right axis
        let camFront = at - eye
        let camRight = Pixar.GfCross(camFront, up).GetNormalized()
        let vertRot = Pixar.GfRotation(camRight, delta.y * orbitSensitivity)
        rotMatrix = Pixar.GfMatrix4d(1.0)
        rotMatrix.SetRotate(vertRot)
        eyeRelative = eye - at
        let rotatedVec2 = rotMatrix.Transform(eyeRelative)
        eye = at + rotatedVec2

    }

    /// Zoom camera by moving eye position along view direction
    /// Based on ImGuiHydraEditor::_ZoomActiveCam
    public func zoom(delta: Double) {
        let camFront = (at - eye).GetNormalized()
        eye = eye + (camFront * delta * zoomSensitivity)
    }

    /// Zoom using scroll wheel
    public func zoomScroll(delta: Double) {
        let camFront = (at - eye).GetNormalized()
        eye = eye + (camFront * delta * zoomSensitivity * 1.0)  // Reduced multiplier for smoother zoom
    }

    /// Focus on a point by setting look-at and adjusting eye position
    /// Based on ImGuiHydraEditor::_FocusOnPrim
    public func focus(on point: Pixar.GfVec3d, distance: Double) {
        let viewDirection = (eye - at).GetNormalized()
        at = point
        eye = at + (viewDirection * distance)
    }

    /// Reset camera to default view
    public func reset() {
        eye = Pixar.GfVec3d(5, 3, 5)
        at = Pixar.GfVec3d(0, 0, 0)
    }

    /// Get the view matrix (camera transform)
    /// Uses SetLookAt like ImGuiHydraEditor
    public func getViewMatrix() -> Pixar.GfMatrix4d {
        var viewMatrix = Pixar.GfMatrix4d()
        viewMatrix.SetLookAt(eye, at, up)
        return viewMatrix
    }

    /// Get camera transform (inverse of view matrix)
    public func getTransform() -> Pixar.GfMatrix4d {
        return getViewMatrix().GetInverse()
    }
}
