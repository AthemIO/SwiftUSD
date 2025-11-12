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

/// Camera controller that manages interactive camera manipulation for UsdView.
/// Wraps Hydra.Camera and provides orbit, pan, and zoom functionality
/// inspired by ImGuiHydraEditor's camera system.
@Observable
public final class CameraController {
    // MARK: - Properties

    /// The underlying Hydra camera that this controller manages
    public var camera: Hydra.Camera

    /// Sensitivity factor for orbit operations (mouse delta scaling)
    public var orbitSensitivity: Double = 0.5

    /// Sensitivity factor for pan operations (mouse delta scaling)
    public var panSensitivity: Double = 0.01

    /// Sensitivity factor for zoom operations (scroll/drag scaling)
    public var zoomSensitivity: Double = 0.01

    /// Minimum camera distance from focus point (prevents passing through)
    public var minimumDistance: Double = 0.1

    /// Maximum camera distance from focus point
    public var maximumDistance: Double = 100_000.0

    // MARK: - Initialization

    /// Initialize camera controller with an existing camera
    public init(camera: Hydra.Camera) {
        self.camera = camera
    }

    /// Initialize camera controller with default orbit camera parameters
    public convenience init(isZUp: Bool = false) {
        self.init(camera: Hydra.Camera(isZUp: isZUp))
        // Set good default camera position
        reset()
    }

    // MARK: - Camera Property Access

    /// Camera rotation (Euler angles in degrees)
    public var rotation: Pixar.GfVec3d
    {
        get { camera.params.rotation }
        set { camera.params.rotation = newValue }
    }

    /// Camera focus point (look-at target, orbit center)
    public var focus: Pixar.GfVec3d
    {
        get { camera.params.focus }
        set { camera.params.focus = newValue }
    }

    /// Camera distance from focus point
    public var distance: Double
    {
        get { camera.params.distance }
        set {
            camera.params.distance = Swift.max(
                minimumDistance, Swift.min(maximumDistance, newValue))
        }
    }

    /// Camera focal length (lens parameter)
    public var focalLength: Double
    {
        get { camera.params.focalLength }
        set { camera.params.focalLength = newValue }
    }

    /// Camera projection mode (perspective or orthographic)
    public var projection: Pixar.GfCamera.Projection
    {
        get { camera.params.projection }
        set { camera.params.projection = newValue }
    }

    // MARK: - Camera Manipulation

    /// Orbit camera around focus point using two-axis rotation.
    /// This implements the ImGuiHydraEditor pattern:
    /// 1. Horizontal rotation around world up-axis
    /// 2. Vertical rotation around camera-local right axis
    /// This avoids gimbal lock and provides intuitive control.
    ///
    /// - Parameter delta: Mouse movement delta (x: horizontal, y: vertical)
    public func orbit(delta: CGPoint) {
        let scaledDelta = CGPoint(
            x: delta.x * orbitSensitivity,
            y: delta.y * orbitSensitivity
        )

        // Get current camera vectors
        let upVector = camera.isZUp ? Pixar.GfVec3d(0, 0, 1) : Pixar.GfVec3d(0, 1, 0)
        let cameraTransform = camera.getTransform()
        let cameraPosition = Pixar.GfVec3d(
            cameraTransform[3][0],
            cameraTransform[3][1],
            cameraTransform[3][2]
        )

        // Compute camera direction and right vectors
        let cameraToFocus = focus - cameraPosition
        let cameraFront = cameraToFocus.GetNormalized()
        let cameraRight = Pixar.GfCross(cameraFront, upVector).GetNormalized()

        // Apply rotations to the rotation parameters
        // Horizontal rotation (around up axis)
        rotation[1] += scaledDelta.x

        // Vertical rotation (around right axis) - constrain to avoid flipping
        let newVerticalRotation = rotation[0] - scaledDelta.y
        rotation[0] = Swift.max(-89.0, Swift.min(89.0, newVerticalRotation))
    }

    /// Pan camera by moving the focus point in camera-local space.
    /// The focus point moves perpendicular to the view direction,
    /// maintaining the camera distance and orientation.
    ///
    /// - Parameter delta: Mouse movement delta (x: right, y: up in screen space)
    public func pan(delta: CGPoint) {
        let scaledDelta = CGPoint(
            x: delta.x * panSensitivity * distance,
            y: delta.y * panSensitivity * distance
        )

        // Get current camera transform
        let cameraTransform = camera.getTransform()
        let cameraPosition = Pixar.GfVec3d(
            cameraTransform[3][0],
            cameraTransform[3][1],
            cameraTransform[3][2]
        )

        // Compute camera-local coordinate system
        let upVector = camera.isZUp ? Pixar.GfVec3d(0, 0, 1) : Pixar.GfVec3d(0, 1, 0)
        let cameraFront = (focus - cameraPosition).GetNormalized()
        let cameraRight = Pixar.GfCross(cameraFront, upVector).GetNormalized()
        let cameraUp = Pixar.GfCross(cameraRight, cameraFront).GetNormalized()

        // Calculate pan delta in world space
        let panDelta = cameraRight * (-scaledDelta.x) + cameraUp * scaledDelta.y

        // Move focus point (this shifts both camera and target together)
        focus = focus + panDelta
    }

    /// Zoom camera by adjusting distance from focus point.
    /// Uses multiplicative scaling for smooth, scale-independent zooming.
    ///
    /// - Parameter delta: Zoom amount (positive = zoom in, negative = zoom out)
    public func zoom(delta: Double) {
        let scaleFactor = 1.0 - (delta * zoomSensitivity)
        distance = distance * scaleFactor
    }

    /// Zoom camera using scroll wheel input.
    /// Applies exponential scaling for natural scroll-based zooming.
    ///
    /// - Parameter scrollDelta: Scroll wheel delta
    public func zoomScroll(delta: Double) {
        let scaleFactor = exp(-delta * zoomSensitivity * 0.1)
        distance = distance * scaleFactor
    }

    /// Frame a bounding box in the viewport by positioning the camera
    /// to view the entire box. The camera maintains its current orientation
    /// but adjusts focus and distance.
    ///
    /// - Parameter bbox: Bounding box to frame
    public func frameBoundingBox(_ bbox: Pixar.GfBBox3d) {
        // Handle empty/invalid bounding boxes
        let range = bbox.GetRange().pointee
        guard range.IsEmpty() == false else {
            // Fallback to world origin
            focus = Pixar.GfVec3d(0, 0, 0)
            distance = 10.0
            return
        }

        // Set focus to bounding box center
        let center = (range.GetMin().pointee + range.GetMax().pointee) * 0.5
        focus = center

        // Calculate appropriate distance based on bounding box size
        // Use 2x the diagonal length to ensure the entire box is visible
        let bboxSize = range.GetMax().pointee - range.GetMin().pointee
        let diagonal = sqrt(
            bboxSize[0] * bboxSize[0] + bboxSize[1] * bboxSize[1] + bboxSize[2] * bboxSize[2])

        distance = Swift.max(diagonal * 2.0, minimumDistance)
    }

    /// Reset camera to default view.
    /// Positions camera looking at world origin from a standard angle.
    public func reset() {
        focus = Pixar.GfVec3d(0, 0, 0)  // Look at center between sphere and cube
        distance = 10.0  // Distance from focus point to camera
        rotation = Pixar.GfVec3d(-30.0, 0.0, 0.0)  // Look down at scene from slight angle
    }

    /// Get the current camera transform matrix.
    /// This is the camera-to-world transformation.
    ///
    /// - Returns: Camera transformation matrix
    public func getTransform() -> Pixar.GfMatrix4d {
        return camera.getTransform()
    }
}
