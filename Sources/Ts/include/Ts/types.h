//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#ifndef PXR_BASE_TS_TYPES_H
#define PXR_BASE_TS_TYPES_H

#include "pxr/pxrns.h"
#include "Ts/api.h"

#include "Gf/interval.h"
#include "Gf/vec2d.h"
#include "Gf/quatd.h"
#include "Gf/quatf.h"
#include "Tf/preprocessorUtilsLite.h"

#include <cstdint>
#include <vector>
#include <type_traits>

PXR_NAMESPACE_OPEN_SCOPE

/// \anchor TS_SPLINE_SUPPORTED_VALUE_TYPES
/// Sequence of value types that are supported by the spline system.
/// \li <b>double</b>
/// \li <b>float</b>
/// \li <b>GfHalf</b>
/// \hideinitializer
#define TS_SPLINE_SUPPORTED_VALUE_TYPES         \
    ((Double,   double))                        \
    ((Float,    float))                         \
    ((Half,     GfHalf))

#define TS_SPLINE_SAMPLE_VERTEX_TYPES \
    ((Vec2d,    GfVec2d))                       \
    ((Vec2f,    GfVec2f))                       \
    ((Vec2h,    GfVec2h))

#define TS_SPLINE_VALUE_TYPE_NAME(x) TF_PP_TUPLE_ELEM(0, x)
#define TS_SPLINE_VALUE_CPP_TYPE(x) TF_PP_TUPLE_ELEM(1, x)

/// \brief True if template parameter T is a supported spline data type.
template <class T>
inline constexpr bool
TsSplineIsValidDataType = false;

#define _TS_SUPPORT_DATA_TYPE(unused, tuple)                            \
    template <>                                                         \
    inline constexpr bool                                               \
    TsSplineIsValidDataType< TS_SPLINE_VALUE_CPP_TYPE(tuple) > = true;
TF_PP_SEQ_FOR_EACH(_TS_SUPPORT_DATA_TYPE,
                   ~,
                   TS_SPLINE_SUPPORTED_VALUE_TYPES)
#undef _TS_SUPPORT_DATA_TYPE

/// \brief True if template parameter T is a supported spline sampling vertex
/// type.
template <class T>
inline constexpr bool
TsSplineIsValidSampleType = false;

#define _TS_SUPPORT_SAMPLE_TYPE(unused, tuple)                          \
    template <>                                                         \
    inline constexpr bool                                               \
    TsSplineIsValidSampleType< TS_SPLINE_VALUE_CPP_TYPE(tuple) > = true;
TF_PP_SEQ_FOR_EACH(_TS_SUPPORT_SAMPLE_TYPE,
                   ~,
                   TS_SPLINE_SAMPLE_VERTEX_TYPES)
#undef _TS_SUPPORT_SAMPLE_TYPE

// Times are encoded as double.
using TsTime = double;

//////////////////////////////
// ** NOTE TO MAINTAINERS **
//
// The following enum values are used in the binary crate format.
// Do not change them; only add.

/// Interpolation mode for a spline segment (region between two knots).
///
enum TsInterpMode
{
    TsInterpValueBlock  = 0,  //< No value in this segment.
    TsInterpHeld        = 1,  //< Constant value in this segment.
    TsInterpLinear      = 2,  //< Linear interpolation.
    TsInterpCurve       = 3   //< Bezier or Hermite, depends on curve type.
};

/// Type of interpolation for a spline's \c Curve segments.
///
enum TsCurveType
{
    TsCurveTypeBezier  = 0,  //< Bezier curve, free tangent widths.
    TsCurveTypeHermite = 1   //< Hermite curve, like Bezier but fixed tan width.
};

/// Curve-shaping mode for one of a spline's extrapolation regions (before all
/// knots and after all knots).
///
enum TsExtrapMode
{
    TsExtrapValueBlock    = 0, //< No value in this region.
    TsExtrapHeld          = 1, //< Constant value in this region.
    TsExtrapLinear        = 2, //< Linear interpolation based on edge knots.
    TsExtrapSloped        = 3, //< Linear interpolation with specified slope.
    TsExtrapLoopRepeat    = 4, //< Knot curve repeated, offset so ends meet.
    TsExtrapLoopReset     = 5, //< Curve repeated exactly, discontinuous joins.
    TsExtrapLoopOscillate = 6  //< Like Reset, but every other copy reversed.
};

/// The source for a particular part of a sampled spline. A \c TsSpline can have
/// a number of different regions. The source is not important to the values
/// that vary over time, but if the spline is sampled and displayed in a user
/// interface, the source can be used to highlight different regions of the
/// displayed spline.
///
enum TsSplineSampleSource
{
    TsSourcePreExtrap,          //< Extrapolation before the first knot
    TsSourcePreExtrapLoop,      //< Looped extrapolation before the first knot
    TsSourceInnerLoopPreEcho,   //< Echoed copy of an inner loop prototype
    TsSourceInnerLoopProto,     //< This is the inner loop prototype
    TsSourceInnerLoopPostEcho,  //< Echoed copy of an inner loop prototype
    TsSourceKnotInterp,         //< "Normal" knot interpolation
    TsSourcePostExtrap,         //< Extrapolation after the last knot
    TsSourcePostExtrapLoop,     //< Looped extrapolation after the last knot
};

/// Automatic tangent calculation algorithms.
///
/// Which automatic tangent algorithm to use.
///
/// \li None - Tangents are not automatically calculated, the provided values
/// are used. Note that the tangent values are still subject to modification
/// by the spline's anti-regression setting.
///
/// \li Custom - The tangent algorithm is determined by the "preTanAlgorithm"
/// and "postTanAlgorithm" keys in the knot's custom data. These custom
/// data keys are reserved for this purpose. If the custom data values do not
/// exist or if their value cannot be understood, then Custom behaves as if
/// the None algorithm was used. Note that the Custom algorithm is not yet
/// implemented so it currently always behaves like None.
///
/// \li AutoEase - Use the "Auto Ease" algorithm from Maya/animX. This is a
/// cubic controlled blending algorithm that computes a slope between the slopes
/// to the knots on either side of this knot. If there is a discontinuity in the
/// spline at this knot (this knot has no previous or next knot, is dual valued,
/// or is adjacent to a value blocked segment of the spline) then the slope will
/// be 0 (flat).
///
/// \see \ref TsKnot::SetPreTanAlgorithm, \ref TsKnot::SetPostTanAlgorithm
enum TsTangentAlgorithm
{
    TsTangentAlgorithmNone,
    TsTangentAlgorithmCustom,
    TsTangentAlgorithmAutoEase
};

/// Inner-loop parameters.
/// Full definition in loopParams.h
class TsLoopParams;

/// Extrapolation parameters for the ends of a spline beyond the knots.
///
class TsExtrapolation
{
public:
    TsExtrapMode mode = TsExtrapHeld;
    double slope = 0.0;

public:
    TS_API
    TsExtrapolation();

    TS_API
    TsExtrapolation(TsExtrapMode mode);

    TS_API
    bool operator==(const TsExtrapolation &other) const;

    TS_API
    bool operator!=(const TsExtrapolation &other) const;

    /// Returns whether our mode is one of the looping extrapolation modes.
    TS_API
    bool IsLooping() const;
};

// Compatibility types for v25.11 migration from v24
using TsExtrapolationPair = std::pair<TsExtrapolation, TsExtrapolation>;
using TsExtrapolationType = TsExtrapMode;
enum TsSide { TsSideLeft, TsSideRight };

/// \brief \c TsSplineSamples<Vertex> holds a collection of piecewise linear
/// polylines that approximate a \c TsSpline.
///
/// The vertex must be one of \c GfVec2d, \c GfVec2f, or \c GfVec2h. Note that
/// you may have precision or overflow issues if you use \c GfVec2h.
///
/// \sa \ref TsSplineSamplesWithSources and \ref TsSpline::Sample
template <typename Vertex>
class TsSplineSamples
{
public:
    static_assert(TsSplineIsValidSampleType<Vertex>,
                  "The Vertex template parameter to TsSplineSamples must be one"
                  " of GfVec2d, GfVec2f, or GfVec2h.");

    using Polyline = std::vector<Vertex>;

    std::vector<Polyline> polylines;
};

/// \brief \c TsSplineSamplesWithSources<Vertex> is a \c TsSplineSamples<Vertex>
/// that also includes source information for each polyline.
///
/// The vertex must be one of \c GfVec2d, \c GfVec2f, or \c GfVec2h. Note that
/// you may have precision or overflow issues if you use \c GfVec2h.
///
/// The \c polylines and \c sources vectors are parallel arrays. In other words,
/// the source for the \c Polyline in \c polylines[i] is in \c sources[i] and
/// the two vectors have the same size.
/// \sa \ref TsSplineSamples and \ref TsSpline::SampleWithSources
template <typename Vertex>
class TsSplineSamplesWithSources
{
public:
    static_assert(TsSplineIsValidSampleType<Vertex>,
                  "The Vertex template parameter to TsSplineSamplesWithSources"
                  " must be one of GfVec2d, GfVec2f, or GfVec2h.");

    using Polyline = std::vector<Vertex>;

    std::vector<Polyline> polylines;
    std::vector<TsSplineSampleSource> sources;
};

// Declare sampling classes as extern templates. They are explicitly
// instantiated in types.cpp
#define TS_SAMPLE_EXTERN_IMPL(unused, tuple)                            \
    TS_API_TEMPLATE_CLASS(                                              \
        TsSplineSamples< TS_SPLINE_VALUE_CPP_TYPE(tuple) >);            \
    TS_API_TEMPLATE_CLASS(                                              \
        TsSplineSamplesWithSources< TS_SPLINE_VALUE_CPP_TYPE(tuple) >);
TF_PP_SEQ_FOR_EACH(TS_SAMPLE_EXTERN_IMPL, ~, TS_SPLINE_SAMPLE_VERTEX_TYPES)
#undef TS_SAMPLE_EXTERN_IMPL

/// Modes for enforcing non-regression in splines.
///
/// See \ref page_ts_regression for a general introduction to regression and
/// anti-regression.
///
enum TsAntiRegressionMode
{
    /// Do not enforce.  If there is regression, runtime evaluation will use
    /// KeepRatio.
    TsAntiRegressionNone,

    /// Prevent tangents from crossing neighboring knots.  This guarantees
    /// non-regression, but is slightly over-conservative, preventing the
    /// authoring of some extreme curves that cannot be created without
    /// non-contained tangents.
    TsAntiRegressionContain,

    /// If there is regression in a segment, shorten both of its tangents until
    /// the regression is just barely prevented (the curve comes to a
    /// near-standstill at some time).  Preserve the ratio of the tangent
    /// lengths.
    TsAntiRegressionKeepRatio,

    /// If there is regression in a segment, leave its start tangent alone, and
    /// shorten its end tangent until the regression is just barely prevented.
    /// This matches Maya behavior.
    TsAntiRegressionKeepStart
};

// Compatibility typedefs for v24 -> v25.11 migration
// In v25.11, TsKnotType was replaced by TsInterpMode
using TsKnotType = TsInterpMode;
constexpr TsInterpMode TsKnotHeld = TsInterpHeld;
constexpr TsInterpMode TsKnotLinear = TsInterpLinear;
constexpr TsInterpMode TsKnotBezier = TsInterpCurve;

// TsTraits template for type characteristics
// This was removed in v25.11 but is still referenced in some code
// Note: We cannot use constexpr for zero because many types are not literal types
template <typename T>
struct TsTraits {
    static const inline T zero{};
    static constexpr bool interpolatable = std::is_floating_point<T>::value;
    static constexpr bool extrapolatable = std::is_floating_point<T>::value;
    static constexpr bool supportsTangents = std::is_floating_point<T>::value;
};

// Specializations for quaternions
template <>
struct TsTraits<GfQuatf> {
    static inline const GfQuatf zero{};
    static constexpr bool interpolatable = true;
    static constexpr bool extrapolatable = false;
    static constexpr bool supportsTangents = false;
};

template <>
struct TsTraits<GfQuatd> {
    static inline const GfQuatd zero{};
    static constexpr bool interpolatable = true;
    static constexpr bool extrapolatable = false;
    static constexpr bool supportsTangents = false;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
