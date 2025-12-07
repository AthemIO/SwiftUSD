/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

#ifndef __PXR_BASE_GF_H__
#define __PXR_BASE_GF_H__

// Tf dependencies - must be included first for Swift module builds
// (forward declarations are not sufficient across module boundaries)
#include <Tf/hash.h>
#include <Tf/token.h>
#include <Tf/staticTokens.h>

// gf
#include <Gf/api.h>
#include <Gf/declare.h>

#include <Gf/bbox3d.h>
#include <Gf/camera.h>
#include <Gf/dualQuatd.h>
#include <Gf/dualQuatf.h>
#include <Gf/dualQuath.h>
#include <Gf/frustum.h>
#include <Gf/gamma.h>
#include <Gf/half.h>
#include <Gf/homogeneous.h>
#include <Gf/ilmbase_half.h>
#include <Gf/ilmbase_halfLimits.h>
#include <Gf/interval.h>
#include <Gf/gfLimits.h>
#include <Gf/line.h>
#include <Gf/line2d.h>
#include <Gf/lineSeg.h>
#include <Gf/lineSeg2d.h>
#include <Gf/gfMath.h>
#include <Gf/matrix2d.h>
#include <Gf/matrix2f.h>
#include <Gf/matrix3d.h>
#include <Gf/matrix3f.h>
#include <Gf/matrix4d.h>
#include <Gf/matrix4f.h>
#include <Gf/matrixData.h>
#include <Gf/multiInterval.h>
#include <Gf/plane.h>
#include <Gf/quatd.h>
#include <Gf/quaternion.h>
#include <Gf/quatf.h>
#include <Gf/quath.h>
#include <Gf/range1d.h>
#include <Gf/range1f.h>
#include <Gf/range2d.h>
#include <Gf/range2f.h>
#include <Gf/range3d.h>
#include <Gf/range3f.h>
#include <Gf/ray.h>
#include <Gf/rect2i.h>
#include <Gf/rotation.h>
#include <Gf/size2.h>
#include <Gf/size3.h>
#include <Gf/traits.h>
#include <Gf/transform.h>
#include <Gf/vec2d.h>
#include <Gf/vec2f.h>
#include <Gf/vec2h.h>
#include <Gf/vec2i.h>
#include <Gf/vec3d.h>
#include <Gf/vec3f.h>
#include <Gf/vec3h.h>
#include <Gf/vec3i.h>
#include <Gf/vec4d.h>
#include <Gf/vec4f.h>
#include <Gf/vec4h.h>
#include <Gf/vec4i.h>

// nanocolor must be included before colorSpace_data.h (which uses NcColorSpace)
#include <Gf/nanocolor.h>
#include <Gf/color.h>
#include <Gf/colorSpace.h>
#include <Gf/colorSpace_data.h>
#include <Gf/numericCast.h>
#include <Gf/ostreamHelpers.h>
// Note: ilmbase_eLut.h and ilmbase_toFloat.h are data-only headers
// (array initializers) meant to be #included inside variable definitions,
// not as standalone headers. They should not be in the umbrella header.

// Python-related headers - only include when Python support is enabled
#if PXR_PYTHON_SUPPORT_ENABLED
#include <Gf/pyBufferUtils.h>
#endif // PXR_PYTHON_SUPPORT_ENABLED

#endif  // __PXR_BASE_GF_H__
