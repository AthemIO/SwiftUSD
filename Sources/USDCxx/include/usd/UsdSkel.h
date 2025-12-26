// UsdSkel.h - Skeleton Schemas for SwiftUSD
// Mirrors: pxr/usd/usdSkel/*.h (Skeleton, SkelRoot, SkelAnimation, BlendShape, SkelBindingAPI)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Gf.h"
#include "../base/Tf.h"
#include "../base/Vt.h"
#include "Sdf.h"
#include "Usd.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/usdSkel/skeleton.h"
#include "pxr/usd/usdSkel/root.h"
#include "pxr/usd/usdSkel/animation.h"
#include "pxr/usd/usdSkel/blendShape.h"
#include "pxr/usd/usdSkel/bindingAPI.h"
#include "pxr/usd/usdSkel/tokens.h"
#endif

namespace swiftusd {

// Forward declarations
class SkelSkeletonRef;
class SkelRootRef;
class SkelAnimationRef;
class SkelBlendShapeRef;
class SkelBindingAPIRef;

// ============================================================================
// MARK: - Enums (SWIFT_SELF_CONTAINED value types)
// ============================================================================

/// Skinning method for skeletal deformation
struct SkelSkinningMethod {
    enum Value : int {
        ClassicLinear = 0,   // Classic linear blend skinning
        DualQuaternion = 1   // Dual quaternion skinning
    };

    Value value;

    SkelSkinningMethod() SWIFTUSD_NOEXCEPT : value(ClassicLinear) {}
    SkelSkinningMethod(Value v) SWIFTUSD_NOEXCEPT : value(v) {}
    explicit SkelSkinningMethod(int v) SWIFTUSD_NOEXCEPT : value(static_cast<Value>(v)) {}

    bool operator==(const SkelSkinningMethod& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const SkelSkinningMethod& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case DualQuaternion: return PXR_NS::UsdSkelTokens->dualQuaternion;
            default: return PXR_NS::UsdSkelTokens->classicLinear;
        }
    }

    static SkelSkinningMethod FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdSkelTokens->dualQuaternion) return SkelSkinningMethod(DualQuaternion);
        return SkelSkinningMethod(ClassicLinear);
    }
#endif
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - SkelRootRef (UsdSkelRoot reference type)
// ============================================================================

/// A boundable prim that marks a scope containing skeletally-posed primitives.
/// SkelRoot must be defined at or above any skinned primitives.
class SkelRootRef {
public:
    // Factory methods
    static SkelRootRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto root = PXR_NS::UsdSkelRoot::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!root) return nullptr;
        return new SkelRootRef(root, stage);
#else
        return new SkelRootRef(path, stage);
#endif
    }

    static SkelRootRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto root = PXR_NS::UsdSkelRoot(prim->GetUsdPrim());
        if (!root) return nullptr;
        return new SkelRootRef(root, prim->GetStage());
#else
        return new SkelRootRef(prim->GetPath(), prim->GetStage());
#endif
    }

    /// Find the SkelRoot at or above the given prim
    static SkelRootRef* Find(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto root = PXR_NS::UsdSkelRoot::Find(prim->GetUsdPrim());
        if (!root) return nullptr;
        return new SkelRootRef(root, prim->GetStage());
#else
        // Standalone: walk up parent hierarchy looking for SkelRoot type
        return nullptr;
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_root);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_root) return nullptr;
        return new PrimRef(_root.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    SkelRootRef(const PXR_NS::UsdSkelRoot& root, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _root(root), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelRootRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdSkelRoot _root;
#else
    SkelRootRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelRootRef() {
        if (_stage) _stage->release();
    }

    Path _path;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - SkelSkeletonRef (UsdSkelSkeleton reference type)
// ============================================================================

/// Describes a skeleton with joints, bind transforms, and rest transforms.
class SkelSkeletonRef {
public:
    // Factory methods
    static SkelSkeletonRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto skel = PXR_NS::UsdSkelSkeleton::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!skel) return nullptr;
        return new SkelSkeletonRef(skel, stage);
#else
        return new SkelSkeletonRef(path, stage);
#endif
    }

    static SkelSkeletonRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto skel = PXR_NS::UsdSkelSkeleton(prim->GetUsdPrim());
        if (!skel) return nullptr;
        return new SkelSkeletonRef(skel, prim->GetStage());
#else
        return new SkelSkeletonRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_skeleton);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return nullptr;
        return new PrimRef(_skeleton.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Joints - array of tokens identifying joints and their hierarchy
    size_t GetJointsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return 0;
        PXR_NS::VtArray<PXR_NS::TfToken> joints;
        _skeleton.GetJointsAttr().Get(&joints);
        return joints.size();
#else
        return _joints.size();
#endif
    }

    size_t GetJoints(Token* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return 0;
        PXR_NS::VtArray<PXR_NS::TfToken> joints;
        if (!_skeleton.GetJointsAttr().Get(&joints)) return 0;
        size_t count = std::min(joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = Token(joints[i].GetString().c_str());
        }
        return count;
#else
        size_t count = std::min(_joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _joints[i];
        }
        return count;
#endif
    }

    bool SetJoints(const Token* joints, size_t count) SWIFTUSD_NOEXCEPT {
        if (!joints && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return false;
        PXR_NS::VtArray<PXR_NS::TfToken> vtJoints(count);
        for (size_t i = 0; i < count; ++i) {
            vtJoints[i] = joints[i].ToUsd();
        }
        return _skeleton.GetJointsAttr().Set(vtJoints);
#else
        _joints.assign(joints, joints + count);
        return true;
#endif
    }

    // Bind transforms - world space transforms of joints at bind time
    size_t GetBindTransformsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return 0;
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> xforms;
        _skeleton.GetBindTransformsAttr().Get(&xforms);
        return xforms.size();
#else
        return _bindTransforms.size() / 16;
#endif
    }

    size_t GetBindTransforms(double* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return 0;
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> xforms;
        if (!_skeleton.GetBindTransformsAttr().Get(&xforms)) return 0;
        size_t count = std::min(xforms.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            const double* data = xforms[i].GetArray();
            for (int j = 0; j < 16; ++j) {
                outBuffer[i * 16 + j] = data[j];
            }
        }
        return count;
#else
        size_t count = std::min(_bindTransforms.size() / 16, maxCount);
        for (size_t i = 0; i < count * 16; ++i) {
            outBuffer[i] = _bindTransforms[i];
        }
        return count;
#endif
    }

    bool SetBindTransforms(const double* transforms, size_t count) SWIFTUSD_NOEXCEPT {
        if (!transforms && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return false;
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> vtXforms(count);
        for (size_t i = 0; i < count; ++i) {
            vtXforms[i] = PXR_NS::GfMatrix4d(
                transforms[i * 16 + 0], transforms[i * 16 + 1], transforms[i * 16 + 2], transforms[i * 16 + 3],
                transforms[i * 16 + 4], transforms[i * 16 + 5], transforms[i * 16 + 6], transforms[i * 16 + 7],
                transforms[i * 16 + 8], transforms[i * 16 + 9], transforms[i * 16 + 10], transforms[i * 16 + 11],
                transforms[i * 16 + 12], transforms[i * 16 + 13], transforms[i * 16 + 14], transforms[i * 16 + 15]
            );
        }
        return _skeleton.GetBindTransformsAttr().Set(vtXforms);
#else
        _bindTransforms.resize(count * 16);
        for (size_t i = 0; i < count * 16; ++i) {
            _bindTransforms[i] = transforms[i];
        }
        return true;
#endif
    }

    // Rest transforms - local space transforms of joints at rest pose
    size_t GetRestTransformsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return 0;
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> xforms;
        _skeleton.GetRestTransformsAttr().Get(&xforms);
        return xforms.size();
#else
        return _restTransforms.size() / 16;
#endif
    }

    size_t GetRestTransforms(double* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return 0;
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> xforms;
        if (!_skeleton.GetRestTransformsAttr().Get(&xforms)) return 0;
        size_t count = std::min(xforms.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            const double* data = xforms[i].GetArray();
            for (int j = 0; j < 16; ++j) {
                outBuffer[i * 16 + j] = data[j];
            }
        }
        return count;
#else
        size_t count = std::min(_restTransforms.size() / 16, maxCount);
        for (size_t i = 0; i < count * 16; ++i) {
            outBuffer[i] = _restTransforms[i];
        }
        return count;
#endif
    }

    bool SetRestTransforms(const double* transforms, size_t count) SWIFTUSD_NOEXCEPT {
        if (!transforms && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_skeleton) return false;
        PXR_NS::VtArray<PXR_NS::GfMatrix4d> vtXforms(count);
        for (size_t i = 0; i < count; ++i) {
            vtXforms[i] = PXR_NS::GfMatrix4d(
                transforms[i * 16 + 0], transforms[i * 16 + 1], transforms[i * 16 + 2], transforms[i * 16 + 3],
                transforms[i * 16 + 4], transforms[i * 16 + 5], transforms[i * 16 + 6], transforms[i * 16 + 7],
                transforms[i * 16 + 8], transforms[i * 16 + 9], transforms[i * 16 + 10], transforms[i * 16 + 11],
                transforms[i * 16 + 12], transforms[i * 16 + 13], transforms[i * 16 + 14], transforms[i * 16 + 15]
            );
        }
        return _skeleton.GetRestTransformsAttr().Set(vtXforms);
#else
        _restTransforms.resize(count * 16);
        for (size_t i = 0; i < count * 16; ++i) {
            _restTransforms[i] = transforms[i];
        }
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    SkelSkeletonRef(const PXR_NS::UsdSkelSkeleton& skel, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _skeleton(skel), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelSkeletonRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdSkelSkeleton _skeleton;
#else
    SkelSkeletonRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelSkeletonRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::vector<Token> _joints;
    std::vector<double> _bindTransforms;  // Flattened 4x4 matrices
    std::vector<double> _restTransforms;  // Flattened 4x4 matrices
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - SkelAnimationRef (UsdSkelAnimation reference type)
// ============================================================================

/// Describes skeletal animation with joint transforms stored in vectorized form.
class SkelAnimationRef {
public:
    // Factory methods
    static SkelAnimationRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto anim = PXR_NS::UsdSkelAnimation::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!anim) return nullptr;
        return new SkelAnimationRef(anim, stage);
#else
        return new SkelAnimationRef(path, stage);
#endif
    }

    static SkelAnimationRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto anim = PXR_NS::UsdSkelAnimation(prim->GetUsdPrim());
        if (!anim) return nullptr;
        return new SkelAnimationRef(anim, prim->GetStage());
#else
        return new SkelAnimationRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_animation);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_animation) return nullptr;
        return new PrimRef(_animation.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Joints - which joints this animation applies to
    size_t GetJointsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::TfToken> joints;
        _animation.GetJointsAttr().Get(&joints);
        return joints.size();
#else
        return _joints.size();
#endif
    }

    size_t GetJoints(Token* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::TfToken> joints;
        if (!_animation.GetJointsAttr().Get(&joints)) return 0;
        size_t count = std::min(joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = Token(joints[i].GetString().c_str());
        }
        return count;
#else
        size_t count = std::min(_joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _joints[i];
        }
        return count;
#endif
    }

    bool SetJoints(const Token* joints, size_t count) SWIFTUSD_NOEXCEPT {
        if (!joints && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_animation) return false;
        PXR_NS::VtArray<PXR_NS::TfToken> vtJoints(count);
        for (size_t i = 0; i < count; ++i) {
            vtJoints[i] = joints[i].ToUsd();
        }
        return _animation.GetJointsAttr().Set(vtJoints);
#else
        _joints.assign(joints, joints + count);
        return true;
#endif
    }

    // Translations - joint-local translations (float3 per joint)
    size_t GetTranslationsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> translations;
        _animation.GetTranslationsAttr().Get(&translations, time.ToUsd());
        return translations.size();
#else
        return _translations.size() / 3;
#endif
    }

    size_t GetTranslations(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> translations;
        if (!_animation.GetTranslationsAttr().Get(&translations, time.ToUsd())) return 0;
        size_t count = std::min(translations.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = translations[i][0];
            outBuffer[i * 3 + 1] = translations[i][1];
            outBuffer[i * 3 + 2] = translations[i][2];
        }
        return count;
#else
        size_t count = std::min(_translations.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) {
            outBuffer[i] = _translations[i];
        }
        return count;
#endif
    }

    bool SetTranslations(TimeCode time, const float* translations, size_t count) SWIFTUSD_NOEXCEPT {
        if (!translations && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_animation) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3f> vtTranslations(count);
        for (size_t i = 0; i < count; ++i) {
            vtTranslations[i] = PXR_NS::GfVec3f(translations[i * 3], translations[i * 3 + 1], translations[i * 3 + 2]);
        }
        return _animation.GetTranslationsAttr().Set(vtTranslations, time.ToUsd());
#else
        _translations.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) {
            _translations[i] = translations[i];
        }
        return true;
#endif
    }

    // Rotations - joint-local quaternion rotations (quatf per joint: x, y, z, w)
    size_t GetRotationsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::GfQuatf> rotations;
        _animation.GetRotationsAttr().Get(&rotations, time.ToUsd());
        return rotations.size();
#else
        return _rotations.size() / 4;
#endif
    }

    size_t GetRotations(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::GfQuatf> rotations;
        if (!_animation.GetRotationsAttr().Get(&rotations, time.ToUsd())) return 0;
        size_t count = std::min(rotations.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            auto img = rotations[i].GetImaginary();
            outBuffer[i * 4 + 0] = img[0];  // x
            outBuffer[i * 4 + 1] = img[1];  // y
            outBuffer[i * 4 + 2] = img[2];  // z
            outBuffer[i * 4 + 3] = rotations[i].GetReal();  // w
        }
        return count;
#else
        size_t count = std::min(_rotations.size() / 4, maxCount);
        for (size_t i = 0; i < count * 4; ++i) {
            outBuffer[i] = _rotations[i];
        }
        return count;
#endif
    }

    bool SetRotations(TimeCode time, const float* rotations, size_t count) SWIFTUSD_NOEXCEPT {
        if (!rotations && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_animation) return false;
        PXR_NS::VtArray<PXR_NS::GfQuatf> vtRotations(count);
        for (size_t i = 0; i < count; ++i) {
            // Input is x, y, z, w format
            vtRotations[i] = PXR_NS::GfQuatf(
                rotations[i * 4 + 3],  // w (real)
                PXR_NS::GfVec3f(rotations[i * 4 + 0], rotations[i * 4 + 1], rotations[i * 4 + 2])  // x, y, z (imaginary)
            );
        }
        return _animation.GetRotationsAttr().Set(vtRotations, time.ToUsd());
#else
        _rotations.resize(count * 4);
        for (size_t i = 0; i < count * 4; ++i) {
            _rotations[i] = rotations[i];
        }
        return true;
#endif
    }

    // Scales - joint-local scales (half3 stored as float3)
    size_t GetScalesCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3h> scales;
        _animation.GetScalesAttr().Get(&scales, time.ToUsd());
        return scales.size();
#else
        return _scales.size() / 3;
#endif
    }

    size_t GetScales(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3h> scales;
        if (!_animation.GetScalesAttr().Get(&scales, time.ToUsd())) return 0;
        size_t count = std::min(scales.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = static_cast<float>(scales[i][0]);
            outBuffer[i * 3 + 1] = static_cast<float>(scales[i][1]);
            outBuffer[i * 3 + 2] = static_cast<float>(scales[i][2]);
        }
        return count;
#else
        size_t count = std::min(_scales.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) {
            outBuffer[i] = _scales[i];
        }
        return count;
#endif
    }

    bool SetScales(TimeCode time, const float* scales, size_t count) SWIFTUSD_NOEXCEPT {
        if (!scales && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_animation) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3h> vtScales(count);
        for (size_t i = 0; i < count; ++i) {
            vtScales[i] = PXR_NS::GfVec3h(scales[i * 3], scales[i * 3 + 1], scales[i * 3 + 2]);
        }
        return _animation.GetScalesAttr().Set(vtScales, time.ToUsd());
#else
        _scales.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) {
            _scales[i] = scales[i];
        }
        return true;
#endif
    }

    // Blend shape weights
    size_t GetBlendShapeWeightsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<float> weights;
        _animation.GetBlendShapeWeightsAttr().Get(&weights, time.ToUsd());
        return weights.size();
#else
        return _blendShapeWeights.size();
#endif
    }

    size_t GetBlendShapeWeights(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_animation) return 0;
        PXR_NS::VtArray<float> weights;
        if (!_animation.GetBlendShapeWeightsAttr().Get(&weights, time.ToUsd())) return 0;
        size_t count = std::min(weights.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = weights[i];
        }
        return count;
#else
        size_t count = std::min(_blendShapeWeights.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _blendShapeWeights[i];
        }
        return count;
#endif
    }

    bool SetBlendShapeWeights(TimeCode time, const float* weights, size_t count) SWIFTUSD_NOEXCEPT {
        if (!weights && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_animation) return false;
        PXR_NS::VtArray<float> vtWeights(weights, weights + count);
        return _animation.GetBlendShapeWeightsAttr().Set(vtWeights, time.ToUsd());
#else
        _blendShapeWeights.assign(weights, weights + count);
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    SkelAnimationRef(const PXR_NS::UsdSkelAnimation& anim, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _animation(anim), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelAnimationRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdSkelAnimation _animation;
#else
    SkelAnimationRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelAnimationRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::vector<Token> _joints;
    std::vector<float> _translations;      // Flattened vec3f
    std::vector<float> _rotations;         // Flattened quatf (x, y, z, w)
    std::vector<float> _scales;            // Flattened vec3f
    std::vector<float> _blendShapeWeights;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - SkelBlendShapeRef (UsdSkelBlendShape reference type)
// ============================================================================

/// Describes a target blend shape with position offsets and optional point indices.
class SkelBlendShapeRef {
public:
    // Factory methods
    static SkelBlendShapeRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto shape = PXR_NS::UsdSkelBlendShape::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!shape) return nullptr;
        return new SkelBlendShapeRef(shape, stage);
#else
        return new SkelBlendShapeRef(path, stage);
#endif
    }

    static SkelBlendShapeRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto shape = PXR_NS::UsdSkelBlendShape(prim->GetUsdPrim());
        if (!shape) return nullptr;
        return new SkelBlendShapeRef(shape, prim->GetStage());
#else
        return new SkelBlendShapeRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_blendShape);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return nullptr;
        return new PrimRef(_blendShape.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Offsets - position offsets to add to base pose (vec3f array)
    size_t GetOffsetsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> offsets;
        _blendShape.GetOffsetsAttr().Get(&offsets);
        return offsets.size();
#else
        return _offsets.size() / 3;
#endif
    }

    size_t GetOffsets(float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> offsets;
        if (!_blendShape.GetOffsetsAttr().Get(&offsets)) return 0;
        size_t count = std::min(offsets.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = offsets[i][0];
            outBuffer[i * 3 + 1] = offsets[i][1];
            outBuffer[i * 3 + 2] = offsets[i][2];
        }
        return count;
#else
        size_t count = std::min(_offsets.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) {
            outBuffer[i] = _offsets[i];
        }
        return count;
#endif
    }

    bool SetOffsets(const float* offsets, size_t count) SWIFTUSD_NOEXCEPT {
        if (!offsets && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3f> vtOffsets(count);
        for (size_t i = 0; i < count; ++i) {
            vtOffsets[i] = PXR_NS::GfVec3f(offsets[i * 3], offsets[i * 3 + 1], offsets[i * 3 + 2]);
        }
        return _blendShape.GetOffsetsAttr().Set(vtOffsets);
#else
        _offsets.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) {
            _offsets[i] = offsets[i];
        }
        return true;
#endif
    }

    // Normal offsets - normal offsets to add to base pose normals (vec3f array)
    size_t GetNormalOffsetsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> offsets;
        _blendShape.GetNormalOffsetsAttr().Get(&offsets);
        return offsets.size();
#else
        return _normalOffsets.size() / 3;
#endif
    }

    size_t GetNormalOffsets(float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> offsets;
        if (!_blendShape.GetNormalOffsetsAttr().Get(&offsets)) return 0;
        size_t count = std::min(offsets.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = offsets[i][0];
            outBuffer[i * 3 + 1] = offsets[i][1];
            outBuffer[i * 3 + 2] = offsets[i][2];
        }
        return count;
#else
        size_t count = std::min(_normalOffsets.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) {
            outBuffer[i] = _normalOffsets[i];
        }
        return count;
#endif
    }

    bool SetNormalOffsets(const float* offsets, size_t count) SWIFTUSD_NOEXCEPT {
        if (!offsets && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3f> vtOffsets(count);
        for (size_t i = 0; i < count; ++i) {
            vtOffsets[i] = PXR_NS::GfVec3f(offsets[i * 3], offsets[i * 3 + 1], offsets[i * 3 + 2]);
        }
        return _blendShape.GetNormalOffsetsAttr().Set(vtOffsets);
#else
        _normalOffsets.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) {
            _normalOffsets[i] = offsets[i];
        }
        return true;
#endif
    }

    // Point indices - optional sparse indices into original mesh
    size_t GetPointIndicesCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return 0;
        PXR_NS::VtArray<int> indices;
        _blendShape.GetPointIndicesAttr().Get(&indices);
        return indices.size();
#else
        return _pointIndices.size();
#endif
    }

    size_t GetPointIndices(int* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return 0;
        PXR_NS::VtArray<int> indices;
        if (!_blendShape.GetPointIndicesAttr().Get(&indices)) return 0;
        size_t count = std::min(indices.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = indices[i];
        }
        return count;
#else
        size_t count = std::min(_pointIndices.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _pointIndices[i];
        }
        return count;
#endif
    }

    bool SetPointIndices(const int* indices, size_t count) SWIFTUSD_NOEXCEPT {
        if (!indices && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_blendShape) return false;
        PXR_NS::VtArray<int> vtIndices(indices, indices + count);
        return _blendShape.GetPointIndicesAttr().Set(vtIndices);
#else
        _pointIndices.assign(indices, indices + count);
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    SkelBlendShapeRef(const PXR_NS::UsdSkelBlendShape& shape, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _blendShape(shape), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelBlendShapeRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdSkelBlendShape _blendShape;
#else
    SkelBlendShapeRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelBlendShapeRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::vector<float> _offsets;         // Flattened vec3f
    std::vector<float> _normalOffsets;   // Flattened vec3f
    std::vector<int> _pointIndices;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - SkelBindingAPIRef (UsdSkelBindingAPI reference type)
// ============================================================================

/// API schema for authoring and extracting skinning-related data on geometry.
class SkelBindingAPIRef {
public:
    // Factory methods - Apply to existing prim
    static SkelBindingAPIRef* Apply(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdSkelBindingAPI::Apply(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new SkelBindingAPIRef(api, prim->GetStage());
#else
        return new SkelBindingAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    static SkelBindingAPIRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto api = PXR_NS::UsdSkelBindingAPI(prim->GetUsdPrim());
        if (!api) return nullptr;
        return new SkelBindingAPIRef(api, prim->GetStage());
#else
        return new SkelBindingAPIRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_bindingAPI);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return nullptr;
        return new PrimRef(_bindingAPI.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Skinning method
    SkelSkinningMethod GetSkinningMethod() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return SkelSkinningMethod();
        PXR_NS::TfToken method;
        _bindingAPI.GetSkinningMethodAttr().Get(&method);
        return SkelSkinningMethod::FromUsd(method);
#else
        return _skinningMethod;
#endif
    }

    bool SetSkinningMethod(SkelSkinningMethod method) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return false;
        return _bindingAPI.CreateSkinningMethodAttr().Set(method.ToUsd());
#else
        _skinningMethod = method;
        return true;
#endif
    }

    // Geom bind transform - world space transform at bind time
    bool GetGeomBindTransform(double* outMatrix) const SWIFTUSD_NOEXCEPT {
        if (!outMatrix) return false;
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return false;
        PXR_NS::GfMatrix4d xform;
        if (!_bindingAPI.GetGeomBindTransformAttr().Get(&xform)) {
            // Return identity if not authored
            for (int i = 0; i < 16; ++i) outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
            return true;
        }
        const double* data = xform.GetArray();
        for (int i = 0; i < 16; ++i) outMatrix[i] = data[i];
        return true;
#else
        for (int i = 0; i < 16; ++i) outMatrix[i] = _geomBindTransform[i];
        return true;
#endif
    }

    bool SetGeomBindTransform(const double* matrix) SWIFTUSD_NOEXCEPT {
        if (!matrix) return false;
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return false;
        PXR_NS::GfMatrix4d xform(
            matrix[0], matrix[1], matrix[2], matrix[3],
            matrix[4], matrix[5], matrix[6], matrix[7],
            matrix[8], matrix[9], matrix[10], matrix[11],
            matrix[12], matrix[13], matrix[14], matrix[15]
        );
        return _bindingAPI.CreateGeomBindTransformAttr().Set(xform);
#else
        for (int i = 0; i < 16; ++i) _geomBindTransform[i] = matrix[i];
        return true;
#endif
    }

    // Joint indices - indices into skeleton joints for each point
    size_t GetJointIndicesCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return 0;
        PXR_NS::VtArray<int> indices;
        _bindingAPI.GetJointIndicesAttr().Get(&indices);
        return indices.size();
#else
        return _jointIndices.size();
#endif
    }

    size_t GetJointIndices(int* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return 0;
        PXR_NS::VtArray<int> indices;
        if (!_bindingAPI.GetJointIndicesAttr().Get(&indices)) return 0;
        size_t count = std::min(indices.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = indices[i];
        return count;
#else
        size_t count = std::min(_jointIndices.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = _jointIndices[i];
        return count;
#endif
    }

    bool SetJointIndices(const int* indices, size_t count) SWIFTUSD_NOEXCEPT {
        if (!indices && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return false;
        PXR_NS::VtArray<int> vtIndices(indices, indices + count);
        return _bindingAPI.CreateJointIndicesAttr().Set(vtIndices);
#else
        _jointIndices.assign(indices, indices + count);
        return true;
#endif
    }

    // Joint weights - weights for each joint influence
    size_t GetJointWeightsCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return 0;
        PXR_NS::VtArray<float> weights;
        _bindingAPI.GetJointWeightsAttr().Get(&weights);
        return weights.size();
#else
        return _jointWeights.size();
#endif
    }

    size_t GetJointWeights(float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return 0;
        PXR_NS::VtArray<float> weights;
        if (!_bindingAPI.GetJointWeightsAttr().Get(&weights)) return 0;
        size_t count = std::min(weights.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = weights[i];
        return count;
#else
        size_t count = std::min(_jointWeights.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = _jointWeights[i];
        return count;
#endif
    }

    bool SetJointWeights(const float* weights, size_t count) SWIFTUSD_NOEXCEPT {
        if (!weights && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_bindingAPI) return false;
        PXR_NS::VtArray<float> vtWeights(weights, weights + count);
        return _bindingAPI.CreateJointWeightsAttr().Set(vtWeights);
#else
        _jointWeights.assign(weights, weights + count);
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    SkelBindingAPIRef(const PXR_NS::UsdSkelBindingAPI& api, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _bindingAPI(api), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~SkelBindingAPIRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdSkelBindingAPI _bindingAPI;
#else
    SkelBindingAPIRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
        // Initialize identity transform
        for (int i = 0; i < 16; ++i) _geomBindTransform[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }

    ~SkelBindingAPIRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    SkelSkinningMethod _skinningMethod;
    double _geomBindTransform[16];
    std::vector<int> _jointIndices;
    std::vector<float> _jointWeights;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
