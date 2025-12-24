// cameraUtil.h - CameraUtil C wrapper
// Mirrors: pxr/imaging/cameraUtil/*.h (Camera framing, conformance, screen window)

#ifndef OPENUSDINTEROP_IMAGING_CAMERAUTIL_H
#define OPENUSDINTEROP_IMAGING_CAMERAUTIL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

/// Opaque handle to a CameraUtilFraming wrapper.
typedef struct CameraUtilFramingOpaque* CameraUtilFramingRef;

/// Opaque handle to a CameraUtilScreenWindowParameters wrapper.
typedef struct CameraUtilScreenWindowParametersOpaque* CameraUtilScreenWindowParametersRef;

// ============================================================================
// MARK: - Conform Window Policy Enum
// ============================================================================

/// Policy for conforming a window to a target aspect ratio.
/// Mirrors pxr::CameraUtilConformWindowPolicy.
typedef enum {
    /// Modify width to match target aspect ratio.
    CAMERAUTIL_MATCH_VERTICALLY = 0,
    /// Modify height to match target aspect ratio.
    CAMERAUTIL_MATCH_HORIZONTALLY = 1,
    /// Increase width or height to fit (letterbox/pillarbox).
    CAMERAUTIL_FIT = 2,
    /// Decrease width or height to fit (crop).
    CAMERAUTIL_CROP = 3,
    /// Leave unchanged (may cause stretching/shrinking).
    CAMERAUTIL_DONT_CONFORM = 4
} CameraUtilConformWindowPolicy;

// ============================================================================
// MARK: - Data Structures
// ============================================================================

/// A 2D range with float precision (min, max).
/// Equivalent to GfRange2f.
typedef struct {
    float minX;
    float minY;
    float maxX;
    float maxY;
} CameraUtilRange2f;

/// A 2D integer rectangle (origin + size).
/// Equivalent to GfRect2i.
typedef struct {
    int minX;
    int minY;
    int width;
    int height;
} CameraUtilRect2i;

/// Screen window parameters (left, right, bottom, top).
typedef struct {
    double left;
    double right;
    double bottom;
    double top;
} CameraUtilScreenWindow;

// ============================================================================
// MARK: - CameraUtilFraming Lifecycle
// ============================================================================

/// Creates an invalid (empty) framing.
/// Returns NULL on failure. Caller must release with CameraUtilFraming_Release.
USD_INTEROP_API CameraUtilFramingRef CameraUtilFraming_Create(void);

/// Creates a framing with given display window, data window, and pixel aspect ratio.
/// - Parameters:
///   - displayWindow: The display window in pixel space.
///   - dataWindow: The data window (pixels to be rendered).
///   - pixelAspectRatio: Width-to-height ratio of a pixel (typically 1.0).
/// Returns NULL on failure. Caller must release with CameraUtilFraming_Release.
USD_INTEROP_API CameraUtilFramingRef CameraUtilFraming_CreateWithWindows(
    CameraUtilRange2f displayWindow,
    CameraUtilRect2i dataWindow,
    float pixelAspectRatio
);

/// Creates a framing with equal display and data window (square pixels).
/// - Parameter dataWindow: The data window (also used as display window).
/// Returns NULL on failure. Caller must release with CameraUtilFraming_Release.
USD_INTEROP_API CameraUtilFramingRef CameraUtilFraming_CreateFromDataWindow(
    CameraUtilRect2i dataWindow
);

/// Creates a copy of an existing framing.
/// Returns NULL on failure. Caller must release with CameraUtilFraming_Release.
USD_INTEROP_API CameraUtilFramingRef CameraUtilFraming_Copy(CameraUtilFramingRef framing);

/// Increments the reference count.
USD_INTEROP_API CameraUtilFramingRef CameraUtilFraming_Retain(CameraUtilFramingRef framing);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void CameraUtilFraming_Release(CameraUtilFramingRef framing);

// ============================================================================
// MARK: - CameraUtilFraming Properties
// ============================================================================

/// Returns true if the framing is valid (display and data window are non-empty).
USD_INTEROP_API bool CameraUtilFraming_IsValid(CameraUtilFramingRef framing);

/// Gets the display window.
USD_INTEROP_API CameraUtilRange2f CameraUtilFraming_GetDisplayWindow(CameraUtilFramingRef framing);

/// Sets the display window.
USD_INTEROP_API void CameraUtilFraming_SetDisplayWindow(
    CameraUtilFramingRef framing,
    CameraUtilRange2f displayWindow
);

/// Gets the data window.
USD_INTEROP_API CameraUtilRect2i CameraUtilFraming_GetDataWindow(CameraUtilFramingRef framing);

/// Sets the data window.
USD_INTEROP_API void CameraUtilFraming_SetDataWindow(
    CameraUtilFramingRef framing,
    CameraUtilRect2i dataWindow
);

/// Gets the pixel aspect ratio.
USD_INTEROP_API float CameraUtilFraming_GetPixelAspectRatio(CameraUtilFramingRef framing);

/// Sets the pixel aspect ratio.
USD_INTEROP_API void CameraUtilFraming_SetPixelAspectRatio(
    CameraUtilFramingRef framing,
    float pixelAspectRatio
);

// ============================================================================
// MARK: - CameraUtilFraming Operations
// ============================================================================

/// Computes the filmback window in pixel space.
/// The filmback window is the rectangle corresponding to the camera's filmback plane,
/// obtained by conforming the display window using the camera's aspect ratio.
/// - Parameters:
///   - framing: The framing to compute from.
///   - cameraAspectRatio: The camera's aspect ratio (width/height).
///   - policy: The window conformance policy.
///   - outFilmbackWindow: Output parameter for the computed filmback window.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult CameraUtilFraming_ComputeFilmbackWindow(
    CameraUtilFramingRef framing,
    float cameraAspectRatio,
    CameraUtilConformWindowPolicy policy,
    CameraUtilRange2f* outFilmbackWindow
);

/// Applies the framing to a projection matrix.
/// Given a projection matrix from a camera, applies the framing transformation.
/// The resulting matrix should be used with the viewport set to the data window.
/// - Parameters:
///   - framing: The framing to apply.
///   - projectionMatrix: Input projection matrix (16 doubles, row-major).
///   - policy: The window conformance policy.
///   - outMatrix: Output projection matrix (16 doubles, row-major).
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult CameraUtilFraming_ApplyToProjectionMatrix(
    CameraUtilFramingRef framing,
    const double* projectionMatrix,
    CameraUtilConformWindowPolicy policy,
    double* outMatrix
);

// ============================================================================
// MARK: - CameraUtilFraming Comparison
// ============================================================================

/// Compares two framings for equality.
USD_INTEROP_API bool CameraUtilFraming_Equal(
    CameraUtilFramingRef a,
    CameraUtilFramingRef b
);

/// Compares two framings for inequality.
USD_INTEROP_API bool CameraUtilFraming_NotEqual(
    CameraUtilFramingRef a,
    CameraUtilFramingRef b
);

// ============================================================================
// MARK: - Conform Window Functions
// ============================================================================

/// Conforms a window (width, height) to a target aspect ratio.
/// - Parameters:
///   - windowWidth: Input window width.
///   - windowHeight: Input window height.
///   - policy: The conformance policy.
///   - targetAspect: Target aspect ratio (width/height).
///   - outWidth: Output conformed width.
///   - outHeight: Output conformed height.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult CameraUtil_ConformWindowSize(
    double windowWidth,
    double windowHeight,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outWidth,
    double* outHeight
);

/// Conforms a window (GfRange2d equivalent) to a target aspect ratio.
/// - Parameters:
///   - minX, minY, maxX, maxY: Input window bounds.
///   - policy: The conformance policy.
///   - targetAspect: Target aspect ratio (width/height).
///   - outMinX, outMinY, outMaxX, outMaxY: Output window bounds.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult CameraUtil_ConformWindowRange(
    double minX, double minY, double maxX, double maxY,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outMinX, double* outMinY, double* outMaxX, double* outMaxY
);

/// Conforms a screen window (left, right, bottom, top) to a target aspect ratio.
/// - Parameters:
///   - left, right, bottom, top: Input screen window (RenderMan style).
///   - policy: The conformance policy.
///   - targetAspect: Target aspect ratio (width/height).
///   - outLeft, outRight, outBottom, outTop: Output screen window.
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult CameraUtil_ConformScreenWindow(
    double left, double right, double bottom, double top,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outLeft, double* outRight, double* outBottom, double* outTop
);

/// Conforms a projection matrix to a target aspect ratio.
/// - Parameters:
///   - projectionMatrix: Input projection matrix (16 doubles, row-major).
///   - policy: The conformance policy.
///   - targetAspect: Target aspect ratio (width/height).
///   - outMatrix: Output projection matrix (16 doubles, row-major).
/// Returns USD_RESULT_SUCCESS on success.
USD_INTEROP_API UsdResult CameraUtil_ConformProjectionMatrix(
    const double* projectionMatrix,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outMatrix
);

// ============================================================================
// MARK: - CameraUtilScreenWindowParameters Lifecycle
// ============================================================================

/// Creates screen window parameters from camera parameters.
/// This is a simplified version that takes the essential camera properties.
/// - Parameters:
///   - horizontalAperture: Camera horizontal aperture in mm.
///   - verticalAperture: Camera vertical aperture in mm.
///   - horizontalApertureOffset: Horizontal aperture offset in mm.
///   - verticalApertureOffset: Vertical aperture offset in mm.
///   - focalLength: Camera focal length in mm.
///   - projection: 0 for perspective, 1 for orthographic.
///   - fitHorizontal: True to fit horizontally, false to fit vertically.
/// Returns NULL on failure. Caller must release with CameraUtilScreenWindowParameters_Release.
USD_INTEROP_API CameraUtilScreenWindowParametersRef CameraUtilScreenWindowParameters_Create(
    float horizontalAperture,
    float verticalAperture,
    float horizontalApertureOffset,
    float verticalApertureOffset,
    float focalLength,
    int projection,
    bool fitHorizontal
);

/// Creates a copy of existing screen window parameters.
/// Returns NULL on failure. Caller must release with CameraUtilScreenWindowParameters_Release.
USD_INTEROP_API CameraUtilScreenWindowParametersRef CameraUtilScreenWindowParameters_Copy(
    CameraUtilScreenWindowParametersRef params
);

/// Increments the reference count.
USD_INTEROP_API CameraUtilScreenWindowParametersRef CameraUtilScreenWindowParameters_Retain(
    CameraUtilScreenWindowParametersRef params
);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void CameraUtilScreenWindowParameters_Release(
    CameraUtilScreenWindowParametersRef params
);

// ============================================================================
// MARK: - CameraUtilScreenWindowParameters Properties
// ============================================================================

/// Gets the screen window (left, right, bottom, top).
/// These parameters define the rectangle in the image plane.
USD_INTEROP_API CameraUtilScreenWindow CameraUtilScreenWindowParameters_GetScreenWindow(
    CameraUtilScreenWindowParametersRef params
);

/// Gets the field of view in degrees.
/// This is the full angle perspective field of view between screen space
/// coordinates (-1,0) and (1,0).
USD_INTEROP_API double CameraUtilScreenWindowParameters_GetFieldOfView(
    CameraUtilScreenWindowParametersRef params
);

/// Gets the z-facing view matrix (16 doubles, row-major).
/// This is the inverse transform for a y-Up, z-facing camera.
/// - Parameters:
///   - params: The screen window parameters.
///   - outMatrix: Output buffer for the 4x4 matrix (16 doubles, row-major).
USD_INTEROP_API void CameraUtilScreenWindowParameters_GetZFacingViewMatrix(
    CameraUtilScreenWindowParametersRef params,
    double* outMatrix
);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_CAMERAUTIL_H
