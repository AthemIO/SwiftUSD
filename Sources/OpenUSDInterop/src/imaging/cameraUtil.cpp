// cameraUtil.cpp - CameraUtil C wrapper implementation
// Mirrors: pxr/imaging/cameraUtil/*.h (Camera framing, conformance, screen window)

#include "../../include/imaging/cameraUtil.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define CAMERAUTIL_USE_USD 1
    #include <pxr/imaging/cameraUtil/framing.h>
    #include <pxr/imaging/cameraUtil/conformWindow.h>
    #include <pxr/imaging/cameraUtil/screenWindowParameters.h>
    #include <pxr/base/gf/camera.h>
    #include <pxr/base/gf/vec2d.h>
    #include <pxr/base/gf/vec4d.h>
    #include <pxr/base/gf/range2d.h>
    #include <pxr/base/gf/range2f.h>
    #include <pxr/base/gf/rect2i.h>
    #include <pxr/base/gf/matrix4d.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define CAMERAUTIL_USE_USD 0
#endif

#include <atomic>
#include <cmath>
#include <algorithm>

// ============================================================================
// MARK: - CameraUtilFraming Wrapper
// ============================================================================

struct CameraUtilFramingOpaque {
    std::atomic<int> refCount;
#if CAMERAUTIL_USE_USD
    CameraUtilFraming framing;
#else
    // Standalone data
    CameraUtilRange2f displayWindow;
    CameraUtilRect2i dataWindow;
    float pixelAspectRatio;
#endif

    CameraUtilFramingOpaque() : refCount(1) {
#if !CAMERAUTIL_USE_USD
        displayWindow = {0, 0, 0, 0};
        dataWindow = {0, 0, 0, 0};
        pixelAspectRatio = 1.0f;
#endif
    }
};

// ============================================================================
// MARK: - CameraUtilScreenWindowParameters Wrapper
// ============================================================================

struct CameraUtilScreenWindowParametersOpaque {
    std::atomic<int> refCount;
    CameraUtilScreenWindow screenWindow;
    double fieldOfView;
    double zFacingViewMatrix[16];

    CameraUtilScreenWindowParametersOpaque() : refCount(1), fieldOfView(90.0) {
        screenWindow = {-1.0, 1.0, -1.0, 1.0};
        // Identity matrix
        for (int i = 0; i < 16; ++i) {
            zFacingViewMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
        }
    }
};

// ============================================================================
// MARK: - Helper Functions
// ============================================================================

#if CAMERAUTIL_USE_USD
static GfRange2f ToGfRange2f(const CameraUtilRange2f& r) {
    return GfRange2f(GfVec2f(r.minX, r.minY), GfVec2f(r.maxX, r.maxY));
}

static CameraUtilRange2f FromGfRange2f(const GfRange2f& r) {
    CameraUtilRange2f result;
    result.minX = r.GetMin()[0];
    result.minY = r.GetMin()[1];
    result.maxX = r.GetMax()[0];
    result.maxY = r.GetMax()[1];
    return result;
}

static GfRect2i ToGfRect2i(const CameraUtilRect2i& r) {
    return GfRect2i(GfVec2i(r.minX, r.minY), r.width, r.height);
}

static CameraUtilRect2i FromGfRect2i(const GfRect2i& r) {
    CameraUtilRect2i result;
    result.minX = r.GetMinX();
    result.minY = r.GetMinY();
    result.width = r.GetWidth();
    result.height = r.GetHeight();
    return result;
}

static pxr::CameraUtilConformWindowPolicy ToPxrPolicy(CameraUtilConformWindowPolicy policy) {
    switch (policy) {
        case CAMERAUTIL_MATCH_VERTICALLY: return pxr::CameraUtilMatchVertically;
        case CAMERAUTIL_MATCH_HORIZONTALLY: return pxr::CameraUtilMatchHorizontally;
        case CAMERAUTIL_FIT: return pxr::CameraUtilFit;
        case CAMERAUTIL_CROP: return pxr::CameraUtilCrop;
        case CAMERAUTIL_DONT_CONFORM: return pxr::CameraUtilDontConform;
        default: return pxr::CameraUtilDontConform;
    }
}

static GfMatrix4d ToGfMatrix4d(const double* m) {
    return GfMatrix4d(
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]
    );
}

static void FromGfMatrix4d(const GfMatrix4d& m, double* out) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            out[row * 4 + col] = m[row][col];
        }
    }
}
#endif

// Standalone helper: conform window size to target aspect
static void ConformWindowSizeStandalone(
    double width, double height,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double& outWidth, double& outHeight)
{
    if (targetAspect <= 0 || width <= 0 || height <= 0) {
        outWidth = width;
        outHeight = height;
        return;
    }

    double currentAspect = width / height;

    switch (policy) {
        case CAMERAUTIL_MATCH_VERTICALLY:
            // Modify width to match target aspect ratio
            outWidth = height * targetAspect;
            outHeight = height;
            break;

        case CAMERAUTIL_MATCH_HORIZONTALLY:
            // Modify height to match target aspect ratio
            outWidth = width;
            outHeight = width / targetAspect;
            break;

        case CAMERAUTIL_FIT:
            // Increase width or height to fit (letterbox/pillarbox)
            if (currentAspect < targetAspect) {
                // Current is too tall, increase width
                outWidth = height * targetAspect;
                outHeight = height;
            } else {
                // Current is too wide, increase height
                outWidth = width;
                outHeight = width / targetAspect;
            }
            break;

        case CAMERAUTIL_CROP:
            // Decrease width or height to fit (crop)
            if (currentAspect > targetAspect) {
                // Current is too wide, decrease width
                outWidth = height * targetAspect;
                outHeight = height;
            } else {
                // Current is too tall, decrease height
                outWidth = width;
                outHeight = width / targetAspect;
            }
            break;

        case CAMERAUTIL_DONT_CONFORM:
        default:
            outWidth = width;
            outHeight = height;
            break;
    }
}

extern "C" {

// ============================================================================
// MARK: - CameraUtilFraming Lifecycle
// ============================================================================

CameraUtilFramingRef CameraUtilFraming_Create(void) {
    try {
        auto* wrapper = new CameraUtilFramingOpaque();
#if CAMERAUTIL_USE_USD
        wrapper->framing = pxr::CameraUtilFraming();
#endif
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

CameraUtilFramingRef CameraUtilFraming_CreateWithWindows(
    CameraUtilRange2f displayWindow,
    CameraUtilRect2i dataWindow,
    float pixelAspectRatio)
{
    try {
        auto* wrapper = new CameraUtilFramingOpaque();
#if CAMERAUTIL_USE_USD
        wrapper->framing = pxr::CameraUtilFraming(
            ToGfRange2f(displayWindow),
            ToGfRect2i(dataWindow),
            pixelAspectRatio
        );
#else
        wrapper->displayWindow = displayWindow;
        wrapper->dataWindow = dataWindow;
        wrapper->pixelAspectRatio = pixelAspectRatio;
#endif
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

CameraUtilFramingRef CameraUtilFraming_CreateFromDataWindow(CameraUtilRect2i dataWindow) {
    try {
        auto* wrapper = new CameraUtilFramingOpaque();
#if CAMERAUTIL_USE_USD
        wrapper->framing = pxr::CameraUtilFraming(ToGfRect2i(dataWindow));
#else
        // When using data window only, display window matches data window
        wrapper->displayWindow.minX = static_cast<float>(dataWindow.minX);
        wrapper->displayWindow.minY = static_cast<float>(dataWindow.minY);
        wrapper->displayWindow.maxX = static_cast<float>(dataWindow.minX + dataWindow.width);
        wrapper->displayWindow.maxY = static_cast<float>(dataWindow.minY + dataWindow.height);
        wrapper->dataWindow = dataWindow;
        wrapper->pixelAspectRatio = 1.0f;
#endif
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

CameraUtilFramingRef CameraUtilFraming_Copy(CameraUtilFramingRef framing) {
    if (!framing) return nullptr;
    try {
        auto* wrapper = new CameraUtilFramingOpaque();
#if CAMERAUTIL_USE_USD
        wrapper->framing = framing->framing;
#else
        wrapper->displayWindow = framing->displayWindow;
        wrapper->dataWindow = framing->dataWindow;
        wrapper->pixelAspectRatio = framing->pixelAspectRatio;
#endif
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

CameraUtilFramingRef CameraUtilFraming_Retain(CameraUtilFramingRef framing) {
    if (framing) {
        framing->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return framing;
}

void CameraUtilFraming_Release(CameraUtilFramingRef framing) {
    if (framing) {
        if (framing->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete framing;
        }
    }
}

// ============================================================================
// MARK: - CameraUtilFraming Properties
// ============================================================================

bool CameraUtilFraming_IsValid(CameraUtilFramingRef framing) {
    if (!framing) return false;
#if CAMERAUTIL_USE_USD
    try {
        return framing->framing.IsValid();
    } catch (...) {
        return false;
    }
#else
    // Check if both windows are non-empty
    bool displayValid = (framing->displayWindow.maxX > framing->displayWindow.minX) &&
                        (framing->displayWindow.maxY > framing->displayWindow.minY);
    bool dataValid = (framing->dataWindow.width > 0) && (framing->dataWindow.height > 0);
    return displayValid && dataValid;
#endif
}

CameraUtilRange2f CameraUtilFraming_GetDisplayWindow(CameraUtilFramingRef framing) {
    CameraUtilRange2f result = {0, 0, 0, 0};
    if (!framing) return result;
#if CAMERAUTIL_USE_USD
    try {
        return FromGfRange2f(framing->framing.displayWindow);
    } catch (...) {
        return result;
    }
#else
    return framing->displayWindow;
#endif
}

void CameraUtilFraming_SetDisplayWindow(CameraUtilFramingRef framing, CameraUtilRange2f displayWindow) {
    if (!framing) return;
#if CAMERAUTIL_USE_USD
    try {
        framing->framing.displayWindow = ToGfRange2f(displayWindow);
    } catch (...) {}
#else
    framing->displayWindow = displayWindow;
#endif
}

CameraUtilRect2i CameraUtilFraming_GetDataWindow(CameraUtilFramingRef framing) {
    CameraUtilRect2i result = {0, 0, 0, 0};
    if (!framing) return result;
#if CAMERAUTIL_USE_USD
    try {
        return FromGfRect2i(framing->framing.dataWindow);
    } catch (...) {
        return result;
    }
#else
    return framing->dataWindow;
#endif
}

void CameraUtilFraming_SetDataWindow(CameraUtilFramingRef framing, CameraUtilRect2i dataWindow) {
    if (!framing) return;
#if CAMERAUTIL_USE_USD
    try {
        framing->framing.dataWindow = ToGfRect2i(dataWindow);
    } catch (...) {}
#else
    framing->dataWindow = dataWindow;
#endif
}

float CameraUtilFraming_GetPixelAspectRatio(CameraUtilFramingRef framing) {
    if (!framing) return 1.0f;
#if CAMERAUTIL_USE_USD
    try {
        return framing->framing.pixelAspectRatio;
    } catch (...) {
        return 1.0f;
    }
#else
    return framing->pixelAspectRatio;
#endif
}

void CameraUtilFraming_SetPixelAspectRatio(CameraUtilFramingRef framing, float pixelAspectRatio) {
    if (!framing) return;
#if CAMERAUTIL_USE_USD
    try {
        framing->framing.pixelAspectRatio = pixelAspectRatio;
    } catch (...) {}
#else
    framing->pixelAspectRatio = pixelAspectRatio;
#endif
}

// ============================================================================
// MARK: - CameraUtilFraming Operations
// ============================================================================

UsdResult CameraUtilFraming_ComputeFilmbackWindow(
    CameraUtilFramingRef framing,
    float cameraAspectRatio,
    CameraUtilConformWindowPolicy policy,
    CameraUtilRange2f* outFilmbackWindow)
{
    if (!framing || !outFilmbackWindow) return USD_RESULT_INVALID_ARGUMENT;

#if CAMERAUTIL_USE_USD
    try {
        GfRange2f filmback = framing->framing.ComputeFilmbackWindow(
            cameraAspectRatio,
            ToPxrPolicy(policy)
        );
        *outFilmbackWindow = FromGfRange2f(filmback);
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Simplified standalone implementation
    // Compute filmback window by conforming display window to camera aspect ratio
    CameraUtilRange2f dw = framing->displayWindow;
    double displayWidth = dw.maxX - dw.minX;
    double displayHeight = dw.maxY - dw.minY;

    if (displayWidth <= 0 || displayHeight <= 0) {
        *outFilmbackWindow = dw;
        return USD_RESULT_SUCCESS;
    }

    double newWidth, newHeight;
    // Note: For filmback, we invert the policy logic
    CameraUtilConformWindowPolicy invertedPolicy = policy;
    if (policy == CAMERAUTIL_FIT) {
        invertedPolicy = CAMERAUTIL_CROP;
    } else if (policy == CAMERAUTIL_CROP) {
        invertedPolicy = CAMERAUTIL_FIT;
    }

    ConformWindowSizeStandalone(displayWidth, displayHeight, invertedPolicy,
                                 cameraAspectRatio, newWidth, newHeight);

    // Center the filmback window
    double centerX = (dw.minX + dw.maxX) / 2.0;
    double centerY = (dw.minY + dw.maxY) / 2.0;

    outFilmbackWindow->minX = static_cast<float>(centerX - newWidth / 2.0);
    outFilmbackWindow->maxX = static_cast<float>(centerX + newWidth / 2.0);
    outFilmbackWindow->minY = static_cast<float>(centerY - newHeight / 2.0);
    outFilmbackWindow->maxY = static_cast<float>(centerY + newHeight / 2.0);

    return USD_RESULT_SUCCESS;
#endif
}

UsdResult CameraUtilFraming_ApplyToProjectionMatrix(
    CameraUtilFramingRef framing,
    const double* projectionMatrix,
    CameraUtilConformWindowPolicy policy,
    double* outMatrix)
{
    if (!framing || !projectionMatrix || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;

#if CAMERAUTIL_USE_USD
    try {
        GfMatrix4d proj = ToGfMatrix4d(projectionMatrix);
        GfMatrix4d result = framing->framing.ApplyToProjectionMatrix(proj, ToPxrPolicy(policy));
        FromGfMatrix4d(result, outMatrix);
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Simplified standalone: just copy the matrix
    // A full implementation would compute the viewport transform
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = projectionMatrix[i];
    }
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - CameraUtilFraming Comparison
// ============================================================================

bool CameraUtilFraming_Equal(CameraUtilFramingRef a, CameraUtilFramingRef b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
#if CAMERAUTIL_USE_USD
    try {
        return a->framing == b->framing;
    } catch (...) {
        return false;
    }
#else
    return a->displayWindow.minX == b->displayWindow.minX &&
           a->displayWindow.minY == b->displayWindow.minY &&
           a->displayWindow.maxX == b->displayWindow.maxX &&
           a->displayWindow.maxY == b->displayWindow.maxY &&
           a->dataWindow.minX == b->dataWindow.minX &&
           a->dataWindow.minY == b->dataWindow.minY &&
           a->dataWindow.width == b->dataWindow.width &&
           a->dataWindow.height == b->dataWindow.height &&
           a->pixelAspectRatio == b->pixelAspectRatio;
#endif
}

bool CameraUtilFraming_NotEqual(CameraUtilFramingRef a, CameraUtilFramingRef b) {
    return !CameraUtilFraming_Equal(a, b);
}

// ============================================================================
// MARK: - Conform Window Functions
// ============================================================================

UsdResult CameraUtil_ConformWindowSize(
    double windowWidth,
    double windowHeight,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outWidth,
    double* outHeight)
{
    if (!outWidth || !outHeight) return USD_RESULT_INVALID_ARGUMENT;

#if CAMERAUTIL_USE_USD
    try {
        GfVec2d window(windowWidth, windowHeight);
        GfVec2d conformed = pxr::CameraUtilConformedWindow(window, ToPxrPolicy(policy), targetAspect);
        *outWidth = conformed[0];
        *outHeight = conformed[1];
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    ConformWindowSizeStandalone(windowWidth, windowHeight, policy, targetAspect, *outWidth, *outHeight);
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult CameraUtil_ConformWindowRange(
    double minX, double minY, double maxX, double maxY,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outMinX, double* outMinY, double* outMaxX, double* outMaxY)
{
    if (!outMinX || !outMinY || !outMaxX || !outMaxY) return USD_RESULT_INVALID_ARGUMENT;

#if CAMERAUTIL_USE_USD
    try {
        GfRange2d window(GfVec2d(minX, minY), GfVec2d(maxX, maxY));
        GfRange2d conformed = pxr::CameraUtilConformedWindow(window, ToPxrPolicy(policy), targetAspect);
        *outMinX = conformed.GetMin()[0];
        *outMinY = conformed.GetMin()[1];
        *outMaxX = conformed.GetMax()[0];
        *outMaxY = conformed.GetMax()[1];
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone: conform the size while keeping center
    double width = maxX - minX;
    double height = maxY - minY;
    double centerX = (minX + maxX) / 2.0;
    double centerY = (minY + maxY) / 2.0;

    double newWidth, newHeight;
    ConformWindowSizeStandalone(width, height, policy, targetAspect, newWidth, newHeight);

    *outMinX = centerX - newWidth / 2.0;
    *outMaxX = centerX + newWidth / 2.0;
    *outMinY = centerY - newHeight / 2.0;
    *outMaxY = centerY + newHeight / 2.0;

    return USD_RESULT_SUCCESS;
#endif
}

UsdResult CameraUtil_ConformScreenWindow(
    double left, double right, double bottom, double top,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outLeft, double* outRight, double* outBottom, double* outTop)
{
    if (!outLeft || !outRight || !outBottom || !outTop) return USD_RESULT_INVALID_ARGUMENT;

#if CAMERAUTIL_USE_USD
    try {
        GfVec4d window(left, right, bottom, top);
        GfVec4d conformed = pxr::CameraUtilConformedWindow(window, ToPxrPolicy(policy), targetAspect);
        *outLeft = conformed[0];
        *outRight = conformed[1];
        *outBottom = conformed[2];
        *outTop = conformed[3];
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone: convert screen window to size, conform, convert back
    double width = right - left;
    double height = top - bottom;
    double centerX = (left + right) / 2.0;
    double centerY = (bottom + top) / 2.0;

    double newWidth, newHeight;
    ConformWindowSizeStandalone(width, height, policy, targetAspect, newWidth, newHeight);

    *outLeft = centerX - newWidth / 2.0;
    *outRight = centerX + newWidth / 2.0;
    *outBottom = centerY - newHeight / 2.0;
    *outTop = centerY + newHeight / 2.0;

    return USD_RESULT_SUCCESS;
#endif
}

UsdResult CameraUtil_ConformProjectionMatrix(
    const double* projectionMatrix,
    CameraUtilConformWindowPolicy policy,
    double targetAspect,
    double* outMatrix)
{
    if (!projectionMatrix || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;

#if CAMERAUTIL_USE_USD
    try {
        GfMatrix4d proj = ToGfMatrix4d(projectionMatrix);
        GfMatrix4d conformed = pxr::CameraUtilConformedWindow(proj, ToPxrPolicy(policy), targetAspect);
        FromGfMatrix4d(conformed, outMatrix);
        return USD_RESULT_SUCCESS;
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#else
    // Standalone: just copy the matrix (simplified)
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = projectionMatrix[i];
    }
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - CameraUtilScreenWindowParameters Lifecycle
// ============================================================================

CameraUtilScreenWindowParametersRef CameraUtilScreenWindowParameters_Create(
    float horizontalAperture,
    float verticalAperture,
    float horizontalApertureOffset,
    float verticalApertureOffset,
    float focalLength,
    int projection,
    bool fitHorizontal)
{
    try {
        auto* wrapper = new CameraUtilScreenWindowParametersOpaque();

#if CAMERAUTIL_USE_USD
        // Create a GfCamera and use it to compute screen window parameters
        GfCamera camera;
        camera.SetHorizontalAperture(horizontalAperture);
        camera.SetVerticalAperture(verticalAperture);
        camera.SetHorizontalApertureOffset(horizontalApertureOffset);
        camera.SetVerticalApertureOffset(verticalApertureOffset);
        camera.SetFocalLength(focalLength);
        camera.SetProjection(projection == 0 ? GfCamera::Perspective : GfCamera::Orthographic);

        GfCamera::FOVDirection fitDirection = fitHorizontal ?
            GfCamera::FOVHorizontal : GfCamera::FOVVertical;

        pxr::CameraUtilScreenWindowParameters params(camera, fitDirection);

        // Extract values
        const GfVec4d& sw = params.GetScreenWindow();
        wrapper->screenWindow.left = sw[0];
        wrapper->screenWindow.right = sw[1];
        wrapper->screenWindow.bottom = sw[2];
        wrapper->screenWindow.top = sw[3];

        wrapper->fieldOfView = params.GetFieldOfView();

        FromGfMatrix4d(params.GetZFacingViewMatrix(), wrapper->zFacingViewMatrix);
#else
        // Standalone implementation
        // Compute aspect ratio
        float aspectRatio = (verticalAperture > 0) ?
            (horizontalAperture / verticalAperture) : 1.0f;

        // Compute screen window based on fit direction
        if (fitHorizontal) {
            // Horizontal has length 2 (from -1 to 1)
            wrapper->screenWindow.left = -1.0;
            wrapper->screenWindow.right = 1.0;
            wrapper->screenWindow.bottom = -1.0 / aspectRatio;
            wrapper->screenWindow.top = 1.0 / aspectRatio;
        } else {
            // Vertical has length 2 (from -1 to 1)
            wrapper->screenWindow.left = -aspectRatio;
            wrapper->screenWindow.right = aspectRatio;
            wrapper->screenWindow.bottom = -1.0;
            wrapper->screenWindow.top = 1.0;
        }

        // Apply offsets (normalized)
        if (horizontalAperture > 0) {
            double offsetX = 2.0 * horizontalApertureOffset / horizontalAperture;
            wrapper->screenWindow.left += offsetX;
            wrapper->screenWindow.right += offsetX;
        }
        if (verticalAperture > 0) {
            double offsetY = 2.0 * verticalApertureOffset / verticalAperture;
            wrapper->screenWindow.bottom += offsetY;
            wrapper->screenWindow.top += offsetY;
        }

        // Compute FOV (perspective only)
        if (projection == 0 && focalLength > 0) {
            // FOV for fit direction
            float aperture = fitHorizontal ? horizontalAperture : verticalAperture;
            wrapper->fieldOfView = 2.0 * atan(aperture / (2.0 * focalLength)) * 180.0 / M_PI;
        } else {
            wrapper->fieldOfView = 90.0;
        }

        // Z-facing view matrix: rotate 180 degrees around Y to face +Z instead of -Z
        // This is the identity matrix with a sign flip in the Z column
        for (int i = 0; i < 16; ++i) {
            wrapper->zFacingViewMatrix[i] = 0.0;
        }
        wrapper->zFacingViewMatrix[0] = -1.0;   // Flip X
        wrapper->zFacingViewMatrix[5] = 1.0;    // Keep Y
        wrapper->zFacingViewMatrix[10] = -1.0;  // Flip Z
        wrapper->zFacingViewMatrix[15] = 1.0;   // W
#endif
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

CameraUtilScreenWindowParametersRef CameraUtilScreenWindowParameters_Copy(
    CameraUtilScreenWindowParametersRef params)
{
    if (!params) return nullptr;
    try {
        auto* wrapper = new CameraUtilScreenWindowParametersOpaque();
        wrapper->screenWindow = params->screenWindow;
        wrapper->fieldOfView = params->fieldOfView;
        for (int i = 0; i < 16; ++i) {
            wrapper->zFacingViewMatrix[i] = params->zFacingViewMatrix[i];
        }
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

CameraUtilScreenWindowParametersRef CameraUtilScreenWindowParameters_Retain(
    CameraUtilScreenWindowParametersRef params)
{
    if (params) {
        params->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return params;
}

void CameraUtilScreenWindowParameters_Release(CameraUtilScreenWindowParametersRef params) {
    if (params) {
        if (params->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete params;
        }
    }
}

// ============================================================================
// MARK: - CameraUtilScreenWindowParameters Properties
// ============================================================================

CameraUtilScreenWindow CameraUtilScreenWindowParameters_GetScreenWindow(
    CameraUtilScreenWindowParametersRef params)
{
    CameraUtilScreenWindow result = {-1.0, 1.0, -1.0, 1.0};
    if (!params) return result;
    return params->screenWindow;
}

double CameraUtilScreenWindowParameters_GetFieldOfView(CameraUtilScreenWindowParametersRef params) {
    if (!params) return 90.0;
    return params->fieldOfView;
}

void CameraUtilScreenWindowParameters_GetZFacingViewMatrix(
    CameraUtilScreenWindowParametersRef params,
    double* outMatrix)
{
    if (!params || !outMatrix) return;
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = params->zFacingViewMatrix[i];
    }
}

} // extern "C"
