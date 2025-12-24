// CameraUtil.swift - Camera Utility wrapper
// Mirrors: pxr/imaging/cameraUtil/*.h (Camera framing, conformance, screen window)

import OpenUSDInterop

// MARK: - CameraUtil Namespace

/// Camera utility functions for framing, window conformance, and screen window parameters.
/// Mirrors pxr::CameraUtil from the USD imaging library.
///
/// The CameraUtil module provides:
/// - Framing information for mapping camera filmback to rendered pixels
/// - Window conformance functions for aspect ratio adjustment
/// - Screen window parameter computation for RenderMan compatibility
///
/// ## Usage
/// ```swift
/// // Create framing for a 1920x1080 render
/// let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 1920, height: 1080)
/// let framing = try CameraUtil.Framing(dataWindow: dataWindow)
///
/// // Conform a window to a target aspect ratio
/// let conformed = try CameraUtil.conformWindow(
///     width: 1920, height: 1080,
///     policy: .fit, targetAspect: 2.35
/// )
/// ```
public enum CameraUtil {

    // MARK: - Conform Window Policy

    /// Policy for conforming a window to a target aspect ratio.
    /// Mirrors pxr::CameraUtilConformWindowPolicy.
    public enum ConformWindowPolicy: Int, Sendable, CustomStringConvertible {
        /// Modify width to match target aspect ratio (keep height fixed).
        case matchVertically = 0
        /// Modify height to match target aspect ratio (keep width fixed).
        case matchHorizontally = 1
        /// Increase width or height to fit (letterbox/pillarbox effect).
        case fit = 2
        /// Decrease width or height to fit (crop effect).
        case crop = 3
        /// Leave unchanged (may cause stretching/shrinking).
        case dontConform = 4

        public var description: String {
            switch self {
            case .matchVertically: return "Match Vertically"
            case .matchHorizontally: return "Match Horizontally"
            case .fit: return "Fit"
            case .crop: return "Crop"
            case .dontConform: return "Don't Conform"
            }
        }

        internal var cValue: CameraUtilConformWindowPolicy {
            CameraUtilConformWindowPolicy(rawValue: UInt32(self.rawValue))
        }
    }

    // MARK: - Range2f

    /// A 2D range with float precision.
    /// Represents a rectangular region with minimum and maximum corners.
    public struct Range2f: Sendable, Equatable, CustomStringConvertible {
        public var minX: Float
        public var minY: Float
        public var maxX: Float
        public var maxY: Float

        public init(minX: Float = 0, minY: Float = 0, maxX: Float = 0, maxY: Float = 0) {
            self.minX = minX
            self.minY = minY
            self.maxX = maxX
            self.maxY = maxY
        }

        /// The width of the range.
        public var width: Float { maxX - minX }

        /// The height of the range.
        public var height: Float { maxY - minY }

        /// The aspect ratio (width/height).
        public var aspectRatio: Float {
            height != 0 ? width / height : 0
        }

        /// Whether the range is empty (has zero or negative area).
        public var isEmpty: Bool {
            width <= 0 || height <= 0
        }

        public var description: String {
            "Range2f((\(minX), \(minY)) - (\(maxX), \(maxY)))"
        }

        internal var cValue: CameraUtilRange2f {
            CameraUtilRange2f(minX: minX, minY: minY, maxX: maxX, maxY: maxY)
        }

        internal init(_ cValue: CameraUtilRange2f) {
            self.minX = cValue.minX
            self.minY = cValue.minY
            self.maxX = cValue.maxX
            self.maxY = cValue.maxY
        }
    }

    // MARK: - Rect2i

    /// A 2D integer rectangle.
    /// Represents a pixel rectangle with origin and size.
    public struct Rect2i: Sendable, Equatable, CustomStringConvertible {
        public var minX: Int32
        public var minY: Int32
        public var width: Int32
        public var height: Int32

        public init(minX: Int32 = 0, minY: Int32 = 0, width: Int32 = 0, height: Int32 = 0) {
            self.minX = minX
            self.minY = minY
            self.width = width
            self.height = height
        }

        /// Convenience initializer with Int values.
        public init(minX: Int, minY: Int, width: Int, height: Int) {
            self.minX = Int32(minX)
            self.minY = Int32(minY)
            self.width = Int32(width)
            self.height = Int32(height)
        }

        /// The maximum X coordinate (minX + width).
        public var maxX: Int32 { minX + width }

        /// The maximum Y coordinate (minY + height).
        public var maxY: Int32 { minY + height }

        /// The aspect ratio (width/height).
        public var aspectRatio: Float {
            height != 0 ? Float(width) / Float(height) : 0
        }

        /// Whether the rectangle is empty (has zero or negative area).
        public var isEmpty: Bool {
            width <= 0 || height <= 0
        }

        public var description: String {
            "Rect2i(origin: (\(minX), \(minY)), size: \(width)x\(height))"
        }

        internal var cValue: CameraUtilRect2i {
            CameraUtilRect2i(minX: minX, minY: minY, width: width, height: height)
        }

        internal init(_ cValue: CameraUtilRect2i) {
            self.minX = cValue.minX
            self.minY = cValue.minY
            self.width = cValue.width
            self.height = cValue.height
        }
    }

    // MARK: - ScreenWindow

    /// Screen window parameters (left, right, bottom, top).
    /// Used for RenderMan-style screen window specification.
    public struct ScreenWindow: Sendable, Equatable, CustomStringConvertible {
        public var left: Double
        public var right: Double
        public var bottom: Double
        public var top: Double

        public init(left: Double = -1, right: Double = 1, bottom: Double = -1, top: Double = 1) {
            self.left = left
            self.right = right
            self.bottom = bottom
            self.top = top
        }

        /// The width of the screen window.
        public var width: Double { right - left }

        /// The height of the screen window.
        public var height: Double { top - bottom }

        /// The aspect ratio (width/height).
        public var aspectRatio: Double {
            height != 0 ? width / height : 0
        }

        public var description: String {
            "ScreenWindow(left: \(left), right: \(right), bottom: \(bottom), top: \(top))"
        }

        internal init(_ cValue: CameraUtilScreenWindow) {
            self.left = cValue.left
            self.right = cValue.right
            self.bottom = cValue.bottom
            self.top = cValue.top
        }
    }
}

// MARK: - CameraUtil Error

/// Errors that can occur in CameraUtil operations.
public enum CameraUtilError: Error, CustomStringConvertible {
    case creationFailed(String)
    case operationFailed(String)
    case invalidArgument(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "CameraUtil creation failed: \(message)"
        case .operationFailed(let message):
            return "CameraUtil operation failed: \(message)"
        case .invalidArgument(let message):
            return "CameraUtil invalid argument: \(message)"
        }
    }
}

// MARK: - Framing

extension CameraUtil {

    /// Framing information for mapping camera filmback to rendered pixels.
    ///
    /// `Framing` describes how the filmback plane of a camera maps to the pixels
    /// of the rendered image. It consists of:
    /// - `displayWindow`: The filmback rectangle in pixel space
    /// - `dataWindow`: The pixels that the renderer will fill
    /// - `pixelAspectRatio`: Width-to-height ratio of pixels
    ///
    /// Mirrors `pxr::CameraUtilFraming` from the USD imaging library.
    ///
    /// ## Usage
    /// ```swift
    /// // Create framing for a 1920x1080 render with overscan
    /// let displayWindow = CameraUtil.Range2f(minX: 100, minY: 50, maxX: 2020, maxY: 1130)
    /// let dataWindow = CameraUtil.Rect2i(minX: 0, minY: 0, width: 2120, height: 1180)
    /// let framing = try CameraUtil.Framing(
    ///     displayWindow: displayWindow,
    ///     dataWindow: dataWindow,
    ///     pixelAspectRatio: 1.0
    /// )
    /// ```
    public final class Framing: @unchecked Sendable {
        internal let handle: CameraUtilFramingRef

        // MARK: Initialization

        /// Creates an invalid (empty) framing.
        public init() throws {
            guard let ref = CameraUtilFraming_Create() else {
                throw CameraUtilError.creationFailed("Failed to create empty Framing")
            }
            self.handle = ref
        }

        /// Creates a framing with given display window, data window, and pixel aspect ratio.
        ///
        /// - Parameters:
        ///   - displayWindow: The display window in pixel space.
        ///   - dataWindow: The data window (pixels to be rendered).
        ///   - pixelAspectRatio: Width-to-height ratio of a pixel (typically 1.0).
        public init(
            displayWindow: Range2f,
            dataWindow: Rect2i,
            pixelAspectRatio: Float = 1.0
        ) throws {
            guard let ref = CameraUtilFraming_CreateWithWindows(
                displayWindow.cValue,
                dataWindow.cValue,
                pixelAspectRatio
            ) else {
                throw CameraUtilError.creationFailed("Failed to create Framing with windows")
            }
            self.handle = ref
        }

        /// Creates a framing with equal display and data window (square pixels).
        ///
        /// - Parameter dataWindow: The data window (also used as display window).
        public init(dataWindow: Rect2i) throws {
            guard let ref = CameraUtilFraming_CreateFromDataWindow(dataWindow.cValue) else {
                throw CameraUtilError.creationFailed("Failed to create Framing from data window")
            }
            self.handle = ref
        }

        /// Internal initializer from an existing handle.
        /// Takes ownership of the handle (does not retain).
        internal init(handle: CameraUtilFramingRef) {
            self.handle = handle
        }

        deinit {
            CameraUtilFraming_Release(handle)
        }

        // MARK: Properties

        /// Whether the framing is valid (display and data window are non-empty).
        public var isValid: Bool {
            CameraUtilFraming_IsValid(handle)
        }

        /// The display window in pixel space.
        public var displayWindow: Range2f {
            get { Range2f(CameraUtilFraming_GetDisplayWindow(handle)) }
            set { CameraUtilFraming_SetDisplayWindow(handle, newValue.cValue) }
        }

        /// The data window (pixels to be rendered).
        public var dataWindow: Rect2i {
            get { Rect2i(CameraUtilFraming_GetDataWindow(handle)) }
            set { CameraUtilFraming_SetDataWindow(handle, newValue.cValue) }
        }

        /// The pixel aspect ratio (width/height of a pixel).
        public var pixelAspectRatio: Float {
            get { CameraUtilFraming_GetPixelAspectRatio(handle) }
            set { CameraUtilFraming_SetPixelAspectRatio(handle, newValue) }
        }

        // MARK: Operations

        /// Computes the filmback window in pixel space.
        ///
        /// The filmback window is the rectangle corresponding to the camera's
        /// filmback plane, obtained by conforming the display window using the
        /// camera's aspect ratio.
        ///
        /// - Parameters:
        ///   - cameraAspectRatio: The camera's aspect ratio (width/height).
        ///   - policy: The window conformance policy.
        /// - Returns: The computed filmback window.
        public func computeFilmbackWindow(
            cameraAspectRatio: Float,
            policy: ConformWindowPolicy
        ) throws -> Range2f {
            var result = CameraUtilRange2f(minX: 0, minY: 0, maxX: 0, maxY: 0)
            let status = CameraUtilFraming_ComputeFilmbackWindow(
                handle,
                cameraAspectRatio,
                policy.cValue,
                &result
            )
            guard status == USD_RESULT_SUCCESS else {
                throw CameraUtilError.operationFailed("Failed to compute filmback window")
            }
            return Range2f(result)
        }

        /// Applies the framing to a projection matrix.
        ///
        /// Given a projection matrix from a camera, applies the framing transformation.
        /// The resulting matrix should be used with the viewport set to the data window.
        ///
        /// - Parameters:
        ///   - projectionMatrix: The camera's projection matrix (16 values, row-major).
        ///   - policy: The window conformance policy.
        /// - Returns: The framed projection matrix.
        public func applyToProjectionMatrix(
            _ projectionMatrix: [Double],
            policy: ConformWindowPolicy
        ) throws -> [Double] {
            guard projectionMatrix.count == 16 else {
                throw CameraUtilError.invalidArgument("Projection matrix must have 16 elements")
            }

            var result = [Double](repeating: 0, count: 16)
            let status = projectionMatrix.withUnsafeBufferPointer { inputPtr in
                result.withUnsafeMutableBufferPointer { outputPtr in
                    CameraUtilFraming_ApplyToProjectionMatrix(
                        handle,
                        inputPtr.baseAddress!,
                        policy.cValue,
                        outputPtr.baseAddress!
                    )
                }
            }

            guard status == USD_RESULT_SUCCESS else {
                throw CameraUtilError.operationFailed("Failed to apply framing to projection matrix")
            }
            return result
        }

        /// Creates a copy of this framing.
        public func copy() throws -> Framing {
            guard let ref = CameraUtilFraming_Copy(handle) else {
                throw CameraUtilError.creationFailed("Failed to copy Framing")
            }
            return Framing(handle: ref)
        }
    }
}

// MARK: - Framing Equatable

extension CameraUtil.Framing: Equatable {
    public static func == (lhs: CameraUtil.Framing, rhs: CameraUtil.Framing) -> Bool {
        CameraUtilFraming_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Framing CustomStringConvertible

extension CameraUtil.Framing: CustomStringConvertible {
    public var description: String {
        "Framing(display: \(displayWindow), data: \(dataWindow), pixelAspect: \(pixelAspectRatio))"
    }
}

// MARK: - ScreenWindowParameters

extension CameraUtil {

    /// Screen window parameters for RenderMan-style rendering.
    ///
    /// Given camera parameters, computes the screen window, field of view,
    /// and z-facing view matrix suitable for use with RenderMan.
    ///
    /// Mirrors `pxr::CameraUtilScreenWindowParameters` from the USD imaging library.
    public final class ScreenWindowParameters: @unchecked Sendable {
        internal let handle: CameraUtilScreenWindowParametersRef

        /// FOV direction for screen window parameter computation.
        public enum FOVDirection: Sendable {
            case horizontal
            case vertical
        }

        // MARK: Initialization

        /// Creates screen window parameters from camera properties.
        ///
        /// - Parameters:
        ///   - horizontalAperture: Camera horizontal aperture in mm.
        ///   - verticalAperture: Camera vertical aperture in mm.
        ///   - horizontalApertureOffset: Horizontal aperture offset in mm (default: 0).
        ///   - verticalApertureOffset: Vertical aperture offset in mm (default: 0).
        ///   - focalLength: Camera focal length in mm.
        ///   - projection: Camera projection type (.perspective or .orthographic).
        ///   - fitDirection: FOV fit direction (.horizontal or .vertical).
        public init(
            horizontalAperture: Float,
            verticalAperture: Float,
            horizontalApertureOffset: Float = 0,
            verticalApertureOffset: Float = 0,
            focalLength: Float,
            projection: Projection = .perspective,
            fitDirection: FOVDirection = .horizontal
        ) throws {
            guard let ref = CameraUtilScreenWindowParameters_Create(
                horizontalAperture,
                verticalAperture,
                horizontalApertureOffset,
                verticalApertureOffset,
                focalLength,
                projection == .perspective ? 0 : 1,
                fitDirection == .horizontal
            ) else {
                throw CameraUtilError.creationFailed("Failed to create ScreenWindowParameters")
            }
            self.handle = ref
        }

        /// Internal initializer from an existing handle.
        internal init(handle: CameraUtilScreenWindowParametersRef) {
            self.handle = handle
        }

        deinit {
            CameraUtilScreenWindowParameters_Release(handle)
        }

        // MARK: Properties

        /// The screen window (left, right, bottom, top).
        /// Defines the rectangle in the image plane for RiScreenWindow.
        public var screenWindow: ScreenWindow {
            ScreenWindow(CameraUtilScreenWindowParameters_GetScreenWindow(handle))
        }

        /// The field of view in degrees.
        /// The full angle perspective field of view between screen space
        /// coordinates (-1, 0) and (1, 0).
        public var fieldOfView: Double {
            CameraUtilScreenWindowParameters_GetFieldOfView(handle)
        }

        /// The z-facing view matrix.
        /// The inverse transform for a y-Up, z-facing camera (vs OpenGL's -z-facing).
        public var zFacingViewMatrix: [Double] {
            var result = [Double](repeating: 0, count: 16)
            result.withUnsafeMutableBufferPointer { ptr in
                CameraUtilScreenWindowParameters_GetZFacingViewMatrix(handle, ptr.baseAddress!)
            }
            return result
        }

        /// Creates a copy of these parameters.
        public func copy() throws -> ScreenWindowParameters {
            guard let ref = CameraUtilScreenWindowParameters_Copy(handle) else {
                throw CameraUtilError.creationFailed("Failed to copy ScreenWindowParameters")
            }
            return ScreenWindowParameters(handle: ref)
        }
    }

    /// Camera projection type.
    public enum Projection: Sendable {
        case perspective
        case orthographic
    }
}

// MARK: - ScreenWindowParameters CustomStringConvertible

extension CameraUtil.ScreenWindowParameters: CustomStringConvertible {
    public var description: String {
        "ScreenWindowParameters(screenWindow: \(screenWindow), fov: \(fieldOfView))"
    }
}

// MARK: - Conform Window Functions

extension CameraUtil {

    /// Conforms a window size (width, height) to a target aspect ratio.
    ///
    /// - Parameters:
    ///   - width: Input window width.
    ///   - height: Input window height.
    ///   - policy: The conformance policy.
    ///   - targetAspect: Target aspect ratio (width/height).
    /// - Returns: The conformed (width, height).
    public static func conformWindow(
        width: Double,
        height: Double,
        policy: ConformWindowPolicy,
        targetAspect: Double
    ) throws -> (width: Double, height: Double) {
        var outWidth: Double = 0
        var outHeight: Double = 0

        let status = CameraUtil_ConformWindowSize(
            width, height,
            policy.cValue,
            targetAspect,
            &outWidth, &outHeight
        )

        guard status == USD_RESULT_SUCCESS else {
            throw CameraUtilError.operationFailed("Failed to conform window size")
        }

        return (outWidth, outHeight)
    }

    /// Conforms a window range to a target aspect ratio.
    ///
    /// - Parameters:
    ///   - minX: Minimum X coordinate.
    ///   - minY: Minimum Y coordinate.
    ///   - maxX: Maximum X coordinate.
    ///   - maxY: Maximum Y coordinate.
    ///   - policy: The conformance policy.
    ///   - targetAspect: Target aspect ratio (width/height).
    /// - Returns: The conformed range (minX, minY, maxX, maxY).
    public static func conformWindow(
        minX: Double, minY: Double, maxX: Double, maxY: Double,
        policy: ConformWindowPolicy,
        targetAspect: Double
    ) throws -> (minX: Double, minY: Double, maxX: Double, maxY: Double) {
        var outMinX: Double = 0
        var outMinY: Double = 0
        var outMaxX: Double = 0
        var outMaxY: Double = 0

        let status = CameraUtil_ConformWindowRange(
            minX, minY, maxX, maxY,
            policy.cValue,
            targetAspect,
            &outMinX, &outMinY, &outMaxX, &outMaxY
        )

        guard status == USD_RESULT_SUCCESS else {
            throw CameraUtilError.operationFailed("Failed to conform window range")
        }

        return (outMinX, outMinY, outMaxX, outMaxY)
    }

    /// Conforms a screen window (left, right, bottom, top) to a target aspect ratio.
    ///
    /// - Parameters:
    ///   - screenWindow: The input screen window.
    ///   - policy: The conformance policy.
    ///   - targetAspect: Target aspect ratio (width/height).
    /// - Returns: The conformed screen window.
    public static func conformScreenWindow(
        _ screenWindow: ScreenWindow,
        policy: ConformWindowPolicy,
        targetAspect: Double
    ) throws -> ScreenWindow {
        var outLeft: Double = 0
        var outRight: Double = 0
        var outBottom: Double = 0
        var outTop: Double = 0

        let status = CameraUtil_ConformScreenWindow(
            screenWindow.left, screenWindow.right,
            screenWindow.bottom, screenWindow.top,
            policy.cValue,
            targetAspect,
            &outLeft, &outRight, &outBottom, &outTop
        )

        guard status == USD_RESULT_SUCCESS else {
            throw CameraUtilError.operationFailed("Failed to conform screen window")
        }

        return ScreenWindow(left: outLeft, right: outRight, bottom: outBottom, top: outTop)
    }

    /// Conforms a projection matrix to a target aspect ratio.
    ///
    /// - Parameters:
    ///   - projectionMatrix: Input projection matrix (16 values, row-major).
    ///   - policy: The conformance policy.
    ///   - targetAspect: Target aspect ratio (width/height).
    /// - Returns: The conformed projection matrix.
    public static func conformProjectionMatrix(
        _ projectionMatrix: [Double],
        policy: ConformWindowPolicy,
        targetAspect: Double
    ) throws -> [Double] {
        guard projectionMatrix.count == 16 else {
            throw CameraUtilError.invalidArgument("Projection matrix must have 16 elements")
        }

        var result = [Double](repeating: 0, count: 16)

        let status = projectionMatrix.withUnsafeBufferPointer { inputPtr in
            result.withUnsafeMutableBufferPointer { outputPtr in
                CameraUtil_ConformProjectionMatrix(
                    inputPtr.baseAddress!,
                    policy.cValue,
                    targetAspect,
                    outputPtr.baseAddress!
                )
            }
        }

        guard status == USD_RESULT_SUCCESS else {
            throw CameraUtilError.operationFailed("Failed to conform projection matrix")
        }

        return result
    }
}
