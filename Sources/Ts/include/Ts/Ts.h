#ifndef __PXR_BASE_TS_H__
#define __PXR_BASE_TS_H__

// Ts - Core types first
#include <Ts/api.h>
#include <Ts/types.h>

// Ts - Data structures
#include <Ts/knotData.h>
#include <Ts/knot.h>
#include <Ts/knotMap.h>
#include <Ts/splineData.h>
#include <Ts/sample.h>
#include <Ts/binary.h>

// Ts - Spline and evaluation
#include <Ts/spline.h>
#include <Ts/eval.h>

// Ts - Utilities
#include <Ts/mathUtils.h>
#include <Ts/tangentConversions.h>
#include <Ts/regressionPreventer.h>
#include <Ts/typeHelpers.h>
#include <Ts/valueTypeDispatch.h>
#include <Ts/debugCodes.h>

// Note: Many headers excluded due to legacy API or incompatibility with Swift module builds
// The Ts module in OpenUSD 25.11 had significant API changes from earlier versions

#endif  // __PXR_BASE_TS_H__
