// usdSkel.cpp - UsdSkel module C wrapper implementation
// Mirrors: pxr/usd/usdSkel/*.h

#include "../../include/usdSkel/usdSkel.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdSkel/root.h>
    #include <pxr/usd/usdSkel/skeleton.h>
    #include <pxr/usd/usdSkel/animation.h>
    #include <pxr/usd/usdSkel/blendShape.h>
    #include <pxr/usd/usdSkel/bindingAPI.h>
    #include <pxr/usd/usdSkel/tokens.h>
    #include <pxr/base/gf/matrix4d.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/gf/quatf.h>
    #include <pxr/base/gf/half.h>
    #include <pxr/base/vt/array.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <mutex>

// ============================================================================
// MARK: - Wrapper Structures
// ============================================================================

struct UsdSkelRootOpaque {
#if USD_USE_FULL
    UsdSkelRoot root;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdSkelRootOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdSkelRootOpaque(const UsdSkelRoot& r)
        : root(r), refCount(1), valid(true) {}
#endif
};

struct UsdSkelSkeletonOpaque {
#if USD_USE_FULL
    UsdSkelSkeleton skeleton;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::vector<std::string> joints;
    std::vector<double> bindTransforms;
    std::vector<double> restTransforms;

    UsdSkelSkeletonOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdSkelSkeletonOpaque(const UsdSkelSkeleton& s)
        : skeleton(s), refCount(1), valid(true) {}
#endif
};

struct UsdSkelAnimationOpaque {
#if USD_USE_FULL
    UsdSkelAnimation animation;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::vector<std::string> joints;
    std::vector<float> translations;
    std::vector<float> rotations;
    std::vector<float> scales;
    std::vector<std::string> blendShapes;
    std::vector<float> blendShapeWeights;

    UsdSkelAnimationOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdSkelAnimationOpaque(const UsdSkelAnimation& a)
        : animation(a), refCount(1), valid(true) {}
#endif
};

struct UsdSkelBlendShapeOpaque {
#if USD_USE_FULL
    UsdSkelBlendShape blendShape;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::vector<float> offsets;
    std::vector<float> normalOffsets;
    std::vector<int> pointIndices;

    UsdSkelBlendShapeOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdSkelBlendShapeOpaque(const UsdSkelBlendShape& b)
        : blendShape(b), refCount(1), valid(true) {}
#endif
};

struct UsdSkelBindingAPIOpaque {
#if USD_USE_FULL
    UsdSkelBindingAPI api;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::vector<int> jointIndices;
    std::vector<float> jointWeights;
    double geomBindTransform[16];
    int skinningMethod;

    UsdSkelBindingAPIOpaque() : refCount(1), valid(false), skinningMethod(0) {
        // Identity matrix
        for (int i = 0; i < 16; ++i) {
            geomBindTransform[i] = (i % 5 == 0) ? 1.0 : 0.0;
        }
    }

#if USD_USE_FULL
    explicit UsdSkelBindingAPIOpaque(const UsdSkelBindingAPI& a)
        : api(a), refCount(1), valid(true), skinningMethod(0) {
        for (int i = 0; i < 16; ++i) {
            geomBindTransform[i] = (i % 5 == 0) ? 1.0 : 0.0;
        }
    }
#endif
};

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - UsdSkelRoot Implementation
// ============================================================================

extern "C" {

UsdSkelRootRef UsdSkelRoot_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelRoot root = UsdSkelRoot::Get(stage->stage, path->path);
        if (root) {
            return new UsdSkelRootOpaque(root);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelRootRef UsdSkelRoot_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelRoot root = UsdSkelRoot::Define(stage->stage, path->path);
        if (root) {
            return new UsdSkelRootOpaque(root);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelRootOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelRootRef UsdSkelRoot_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelRoot root(prim->prim);
        if (root) {
            return new UsdSkelRootOpaque(root);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelRootOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelRootRef UsdSkelRoot_Retain(UsdSkelRootRef root) {
    if (!root) return nullptr;
    root->refCount.fetch_add(1, std::memory_order_relaxed);
    return root;
}

void UsdSkelRoot_Release(UsdSkelRootRef root) {
    if (!root) return;
    if (root->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete root;
    }
}

bool UsdSkelRoot_IsValid(UsdSkelRootRef root) {
    if (!root) return false;
#if USD_USE_FULL
    return root->root;
#else
    return root->valid;
#endif
}

UsdPrimRef UsdSkelRoot_GetPrim(UsdSkelRootRef root) {
    if (!root) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = root->root.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdSkelRootRef UsdSkelRoot_Find(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelRoot root = UsdSkelRoot::Find(prim->prim);
        if (root) {
            return new UsdSkelRootOpaque(root);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// ============================================================================
// MARK: - UsdSkelSkeleton Implementation
// ============================================================================

UsdSkelSkeletonRef UsdSkelSkeleton_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelSkeleton skeleton = UsdSkelSkeleton::Get(stage->stage, path->path);
        if (skeleton) {
            return new UsdSkelSkeletonOpaque(skeleton);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelSkeletonRef UsdSkelSkeleton_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelSkeleton skeleton = UsdSkelSkeleton::Define(stage->stage, path->path);
        if (skeleton) {
            return new UsdSkelSkeletonOpaque(skeleton);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelSkeletonOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelSkeletonRef UsdSkelSkeleton_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelSkeleton skeleton(prim->prim);
        if (skeleton) {
            return new UsdSkelSkeletonOpaque(skeleton);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelSkeletonOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelSkeletonRef UsdSkelSkeleton_Retain(UsdSkelSkeletonRef skeleton) {
    if (!skeleton) return nullptr;
    skeleton->refCount.fetch_add(1, std::memory_order_relaxed);
    return skeleton;
}

void UsdSkelSkeleton_Release(UsdSkelSkeletonRef skeleton) {
    if (!skeleton) return;
    if (skeleton->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete skeleton;
    }
}

bool UsdSkelSkeleton_IsValid(UsdSkelSkeletonRef skeleton) {
    if (!skeleton) return false;
#if USD_USE_FULL
    return skeleton->skeleton;
#else
    return skeleton->valid;
#endif
}

UsdPrimRef UsdSkelSkeleton_GetPrim(UsdSkelSkeletonRef skeleton) {
    if (!skeleton) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = skeleton->skeleton.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

size_t UsdSkelSkeleton_GetJointCount(UsdSkelSkeletonRef skeleton) {
    if (!skeleton) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray joints;
        skeleton->skeleton.GetJointsAttr().Get(&joints);
        return joints.size();
#else
        return skeleton->joints.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelSkeleton_GetJoints(
    UsdSkelSkeletonRef skeleton,
    TfTokenRef* outJoints,
    size_t maxCount
) {
    if (!skeleton || !outJoints) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray joints;
        skeleton->skeleton.GetJointsAttr().Get(&joints);
        size_t count = std::min(joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outJoints[i] = TfToken_Create(joints[i].GetText());
        }
        return count;
#else
        size_t count = std::min(skeleton->joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outJoints[i] = TfToken_Create(skeleton->joints[i].c_str());
        }
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelSkeleton_SetJoints(
    UsdSkelSkeletonRef skeleton,
    const TfTokenRef* joints,
    size_t count
) {
    if (!skeleton || !joints) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtTokenArray vtJoints(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(joints[i]);
            vtJoints[i] = TfToken(text ? text : "");
        }
        skeleton->skeleton.GetJointsAttr().Set(vtJoints);
        return USD_RESULT_SUCCESS;
#else
        skeleton->joints.resize(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(joints[i]);
            skeleton->joints[i] = text ? text : "";
        }
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelSkeleton_GetJointNames(
    UsdSkelSkeletonRef skeleton,
    TfTokenRef* outNames,
    size_t maxCount
) {
    if (!skeleton || !outNames) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray names;
        skeleton->skeleton.GetJointNamesAttr().Get(&names);
        size_t count = std::min(names.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outNames[i] = TfToken_Create(names[i].GetText());
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelSkeleton_SetJointNames(
    UsdSkelSkeletonRef skeleton,
    const TfTokenRef* names,
    size_t count
) {
    if (!skeleton || !names) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtTokenArray vtNames(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(names[i]);
            vtNames[i] = TfToken(text ? text : "");
        }
        skeleton->skeleton.GetJointNamesAttr().Set(vtNames);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelSkeleton_GetBindTransforms(
    UsdSkelSkeletonRef skeleton,
    double* outTransforms,
    size_t maxCount
) {
    if (!skeleton || !outTransforms) return 0;
    try {
#if USD_USE_FULL
        VtMatrix4dArray transforms;
        skeleton->skeleton.GetBindTransformsAttr().Get(&transforms);
        size_t count = std::min(transforms.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            const double* data = transforms[i].data();
            std::memcpy(&outTransforms[i * 16], data, 16 * sizeof(double));
        }
        return count;
#else
        size_t count = std::min(skeleton->bindTransforms.size() / 16, maxCount);
        std::memcpy(outTransforms, skeleton->bindTransforms.data(), count * 16 * sizeof(double));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelSkeleton_SetBindTransforms(
    UsdSkelSkeletonRef skeleton,
    const double* transforms,
    size_t count
) {
    if (!skeleton || !transforms) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtMatrix4dArray vtTransforms(count);
        for (size_t i = 0; i < count; ++i) {
            GfMatrix4d mat;
            std::memcpy(mat.data(), &transforms[i * 16], 16 * sizeof(double));
            vtTransforms[i] = mat;
        }
        skeleton->skeleton.GetBindTransformsAttr().Set(vtTransforms);
        return USD_RESULT_SUCCESS;
#else
        skeleton->bindTransforms.resize(count * 16);
        std::memcpy(skeleton->bindTransforms.data(), transforms, count * 16 * sizeof(double));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelSkeleton_GetRestTransforms(
    UsdSkelSkeletonRef skeleton,
    double* outTransforms,
    size_t maxCount
) {
    if (!skeleton || !outTransforms) return 0;
    try {
#if USD_USE_FULL
        VtMatrix4dArray transforms;
        skeleton->skeleton.GetRestTransformsAttr().Get(&transforms);
        size_t count = std::min(transforms.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            const double* data = transforms[i].data();
            std::memcpy(&outTransforms[i * 16], data, 16 * sizeof(double));
        }
        return count;
#else
        size_t count = std::min(skeleton->restTransforms.size() / 16, maxCount);
        std::memcpy(outTransforms, skeleton->restTransforms.data(), count * 16 * sizeof(double));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelSkeleton_SetRestTransforms(
    UsdSkelSkeletonRef skeleton,
    const double* transforms,
    size_t count
) {
    if (!skeleton || !transforms) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtMatrix4dArray vtTransforms(count);
        for (size_t i = 0; i < count; ++i) {
            GfMatrix4d mat;
            std::memcpy(mat.data(), &transforms[i * 16], 16 * sizeof(double));
            vtTransforms[i] = mat;
        }
        skeleton->skeleton.GetRestTransformsAttr().Set(vtTransforms);
        return USD_RESULT_SUCCESS;
#else
        skeleton->restTransforms.resize(count * 16);
        std::memcpy(skeleton->restTransforms.data(), transforms, count * 16 * sizeof(double));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdSkelAnimation Implementation
// ============================================================================

UsdSkelAnimationRef UsdSkelAnimation_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelAnimation anim = UsdSkelAnimation::Get(stage->stage, path->path);
        if (anim) {
            return new UsdSkelAnimationOpaque(anim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelAnimationRef UsdSkelAnimation_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelAnimation anim = UsdSkelAnimation::Define(stage->stage, path->path);
        if (anim) {
            return new UsdSkelAnimationOpaque(anim);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelAnimationOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelAnimationRef UsdSkelAnimation_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelAnimation anim(prim->prim);
        if (anim) {
            return new UsdSkelAnimationOpaque(anim);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelAnimationOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelAnimationRef UsdSkelAnimation_Retain(UsdSkelAnimationRef anim) {
    if (!anim) return nullptr;
    anim->refCount.fetch_add(1, std::memory_order_relaxed);
    return anim;
}

void UsdSkelAnimation_Release(UsdSkelAnimationRef anim) {
    if (!anim) return;
    if (anim->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete anim;
    }
}

bool UsdSkelAnimation_IsValid(UsdSkelAnimationRef anim) {
    if (!anim) return false;
#if USD_USE_FULL
    return anim->animation;
#else
    return anim->valid;
#endif
}

UsdPrimRef UsdSkelAnimation_GetPrim(UsdSkelAnimationRef anim) {
    if (!anim) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = anim->animation.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

size_t UsdSkelAnimation_GetJointCount(UsdSkelAnimationRef anim) {
    if (!anim) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray joints;
        anim->animation.GetJointsAttr().Get(&joints);
        return joints.size();
#else
        return anim->joints.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelAnimation_GetJoints(
    UsdSkelAnimationRef anim,
    TfTokenRef* outJoints,
    size_t maxCount
) {
    if (!anim || !outJoints) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray joints;
        anim->animation.GetJointsAttr().Get(&joints);
        size_t count = std::min(joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outJoints[i] = TfToken_Create(joints[i].GetText());
        }
        return count;
#else
        size_t count = std::min(anim->joints.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outJoints[i] = TfToken_Create(anim->joints[i].c_str());
        }
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelAnimation_SetJoints(
    UsdSkelAnimationRef anim,
    const TfTokenRef* joints,
    size_t count
) {
    if (!anim || !joints) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtTokenArray vtJoints(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(joints[i]);
            vtJoints[i] = TfToken(text ? text : "");
        }
        anim->animation.GetJointsAttr().Set(vtJoints);
        return USD_RESULT_SUCCESS;
#else
        anim->joints.resize(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(joints[i]);
            anim->joints[i] = text ? text : "";
        }
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelAnimation_GetTranslations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outTranslations,
    size_t maxCount
) {
    if (!anim || !outTranslations) return 0;
    try {
#if USD_USE_FULL
        VtVec3fArray translations;
        anim->animation.GetTranslationsAttr().Get(&translations,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(translations.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outTranslations[i * 3 + 0] = translations[i][0];
            outTranslations[i * 3 + 1] = translations[i][1];
            outTranslations[i * 3 + 2] = translations[i][2];
        }
        return count;
#else
        size_t count = std::min(anim->translations.size() / 3, maxCount);
        std::memcpy(outTranslations, anim->translations.data(), count * 3 * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelAnimation_SetTranslations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* translations,
    size_t count
) {
    if (!anim || !translations) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtVec3fArray vtTranslations(count);
        for (size_t i = 0; i < count; ++i) {
            vtTranslations[i] = GfVec3f(translations[i * 3 + 0],
                                        translations[i * 3 + 1],
                                        translations[i * 3 + 2]);
        }
        anim->animation.GetTranslationsAttr().Set(vtTranslations,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        anim->translations.resize(count * 3);
        std::memcpy(anim->translations.data(), translations, count * 3 * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelAnimation_GetRotations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outRotations,
    size_t maxCount
) {
    if (!anim || !outRotations) return 0;
    try {
#if USD_USE_FULL
        VtQuatfArray rotations;
        anim->animation.GetRotationsAttr().Get(&rotations,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(rotations.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outRotations[i * 4 + 0] = rotations[i].GetReal();
            GfVec3f imaginary = rotations[i].GetImaginary();
            outRotations[i * 4 + 1] = imaginary[0];
            outRotations[i * 4 + 2] = imaginary[1];
            outRotations[i * 4 + 3] = imaginary[2];
        }
        return count;
#else
        size_t count = std::min(anim->rotations.size() / 4, maxCount);
        std::memcpy(outRotations, anim->rotations.data(), count * 4 * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelAnimation_SetRotations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* rotations,
    size_t count
) {
    if (!anim || !rotations) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtQuatfArray vtRotations(count);
        for (size_t i = 0; i < count; ++i) {
            vtRotations[i] = GfQuatf(rotations[i * 4 + 0],
                                     rotations[i * 4 + 1],
                                     rotations[i * 4 + 2],
                                     rotations[i * 4 + 3]);
        }
        anim->animation.GetRotationsAttr().Set(vtRotations,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        anim->rotations.resize(count * 4);
        std::memcpy(anim->rotations.data(), rotations, count * 4 * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelAnimation_GetScales(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outScales,
    size_t maxCount
) {
    if (!anim || !outScales) return 0;
    try {
#if USD_USE_FULL
        VtVec3hArray scales;
        anim->animation.GetScalesAttr().Get(&scales,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(scales.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outScales[i * 3 + 0] = static_cast<float>(scales[i][0]);
            outScales[i * 3 + 1] = static_cast<float>(scales[i][1]);
            outScales[i * 3 + 2] = static_cast<float>(scales[i][2]);
        }
        return count;
#else
        size_t count = std::min(anim->scales.size() / 3, maxCount);
        std::memcpy(outScales, anim->scales.data(), count * 3 * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelAnimation_SetScales(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* scales,
    size_t count
) {
    if (!anim || !scales) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtVec3hArray vtScales(count);
        for (size_t i = 0; i < count; ++i) {
            vtScales[i] = GfVec3h(GfHalf(scales[i * 3 + 0]),
                                  GfHalf(scales[i * 3 + 1]),
                                  GfHalf(scales[i * 3 + 2]));
        }
        anim->animation.GetScalesAttr().Set(vtScales,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        anim->scales.resize(count * 3);
        std::memcpy(anim->scales.data(), scales, count * 3 * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelAnimation_GetBlendShapeCount(UsdSkelAnimationRef anim) {
    if (!anim) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray blendShapes;
        anim->animation.GetBlendShapesAttr().Get(&blendShapes);
        return blendShapes.size();
#else
        return anim->blendShapes.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelAnimation_GetBlendShapes(
    UsdSkelAnimationRef anim,
    TfTokenRef* outBlendShapes,
    size_t maxCount
) {
    if (!anim || !outBlendShapes) return 0;
    try {
#if USD_USE_FULL
        VtTokenArray blendShapes;
        anim->animation.GetBlendShapesAttr().Get(&blendShapes);
        size_t count = std::min(blendShapes.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBlendShapes[i] = TfToken_Create(blendShapes[i].GetText());
        }
        return count;
#else
        size_t count = std::min(anim->blendShapes.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBlendShapes[i] = TfToken_Create(anim->blendShapes[i].c_str());
        }
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelAnimation_SetBlendShapes(
    UsdSkelAnimationRef anim,
    const TfTokenRef* blendShapes,
    size_t count
) {
    if (!anim || !blendShapes) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtTokenArray vtBlendShapes(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(blendShapes[i]);
            vtBlendShapes[i] = TfToken(text ? text : "");
        }
        anim->animation.GetBlendShapesAttr().Set(vtBlendShapes);
        return USD_RESULT_SUCCESS;
#else
        anim->blendShapes.resize(count);
        for (size_t i = 0; i < count; ++i) {
            const char* text = TfToken_GetText(blendShapes[i]);
            anim->blendShapes[i] = text ? text : "";
        }
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelAnimation_GetBlendShapeWeights(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outWeights,
    size_t maxCount
) {
    if (!anim || !outWeights) return 0;
    try {
#if USD_USE_FULL
        VtFloatArray weights;
        anim->animation.GetBlendShapeWeightsAttr().Get(&weights,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(weights.size(), maxCount);
        std::memcpy(outWeights, weights.data(), count * sizeof(float));
        return count;
#else
        size_t count = std::min(anim->blendShapeWeights.size(), maxCount);
        std::memcpy(outWeights, anim->blendShapeWeights.data(), count * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelAnimation_SetBlendShapeWeights(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* weights,
    size_t count
) {
    if (!anim || !weights) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtFloatArray vtWeights(count);
        std::memcpy(vtWeights.data(), weights, count * sizeof(float));
        anim->animation.GetBlendShapeWeightsAttr().Set(vtWeights,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        anim->blendShapeWeights.resize(count);
        std::memcpy(anim->blendShapeWeights.data(), weights, count * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdSkelBlendShape Implementation
// ============================================================================

UsdSkelBlendShapeRef UsdSkelBlendShape_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelBlendShape blendShape = UsdSkelBlendShape::Get(stage->stage, path->path);
        if (blendShape) {
            return new UsdSkelBlendShapeOpaque(blendShape);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelBlendShapeRef UsdSkelBlendShape_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelBlendShape blendShape = UsdSkelBlendShape::Define(stage->stage, path->path);
        if (blendShape) {
            return new UsdSkelBlendShapeOpaque(blendShape);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelBlendShapeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelBlendShapeRef UsdSkelBlendShape_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelBlendShape blendShape(prim->prim);
        if (blendShape) {
            return new UsdSkelBlendShapeOpaque(blendShape);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelBlendShapeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelBlendShapeRef UsdSkelBlendShape_Retain(UsdSkelBlendShapeRef blendShape) {
    if (!blendShape) return nullptr;
    blendShape->refCount.fetch_add(1, std::memory_order_relaxed);
    return blendShape;
}

void UsdSkelBlendShape_Release(UsdSkelBlendShapeRef blendShape) {
    if (!blendShape) return;
    if (blendShape->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete blendShape;
    }
}

bool UsdSkelBlendShape_IsValid(UsdSkelBlendShapeRef blendShape) {
    if (!blendShape) return false;
#if USD_USE_FULL
    return blendShape->blendShape;
#else
    return blendShape->valid;
#endif
}

UsdPrimRef UsdSkelBlendShape_GetPrim(UsdSkelBlendShapeRef blendShape) {
    if (!blendShape) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = blendShape->blendShape.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

size_t UsdSkelBlendShape_GetOffsetsCount(UsdSkelBlendShapeRef blendShape) {
    if (!blendShape) return 0;
    try {
#if USD_USE_FULL
        VtVec3fArray offsets;
        blendShape->blendShape.GetOffsetsAttr().Get(&offsets);
        return offsets.size();
#else
        return blendShape->offsets.size() / 3;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelBlendShape_GetOffsets(
    UsdSkelBlendShapeRef blendShape,
    float* outOffsets,
    size_t maxCount
) {
    if (!blendShape || !outOffsets) return 0;
    try {
#if USD_USE_FULL
        VtVec3fArray offsets;
        blendShape->blendShape.GetOffsetsAttr().Get(&offsets);
        size_t count = std::min(offsets.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outOffsets[i * 3 + 0] = offsets[i][0];
            outOffsets[i * 3 + 1] = offsets[i][1];
            outOffsets[i * 3 + 2] = offsets[i][2];
        }
        return count;
#else
        size_t count = std::min(blendShape->offsets.size() / 3, maxCount);
        std::memcpy(outOffsets, blendShape->offsets.data(), count * 3 * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelBlendShape_SetOffsets(
    UsdSkelBlendShapeRef blendShape,
    const float* offsets,
    size_t count
) {
    if (!blendShape || !offsets) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtVec3fArray vtOffsets(count);
        for (size_t i = 0; i < count; ++i) {
            vtOffsets[i] = GfVec3f(offsets[i * 3 + 0],
                                   offsets[i * 3 + 1],
                                   offsets[i * 3 + 2]);
        }
        blendShape->blendShape.GetOffsetsAttr().Set(vtOffsets);
        return USD_RESULT_SUCCESS;
#else
        blendShape->offsets.resize(count * 3);
        std::memcpy(blendShape->offsets.data(), offsets, count * 3 * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelBlendShape_GetNormalOffsets(
    UsdSkelBlendShapeRef blendShape,
    float* outNormalOffsets,
    size_t maxCount
) {
    if (!blendShape || !outNormalOffsets) return 0;
    try {
#if USD_USE_FULL
        VtVec3fArray normals;
        blendShape->blendShape.GetNormalOffsetsAttr().Get(&normals);
        size_t count = std::min(normals.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outNormalOffsets[i * 3 + 0] = normals[i][0];
            outNormalOffsets[i * 3 + 1] = normals[i][1];
            outNormalOffsets[i * 3 + 2] = normals[i][2];
        }
        return count;
#else
        size_t count = std::min(blendShape->normalOffsets.size() / 3, maxCount);
        std::memcpy(outNormalOffsets, blendShape->normalOffsets.data(), count * 3 * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelBlendShape_SetNormalOffsets(
    UsdSkelBlendShapeRef blendShape,
    const float* normalOffsets,
    size_t count
) {
    if (!blendShape || !normalOffsets) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtVec3fArray vtNormals(count);
        for (size_t i = 0; i < count; ++i) {
            vtNormals[i] = GfVec3f(normalOffsets[i * 3 + 0],
                                   normalOffsets[i * 3 + 1],
                                   normalOffsets[i * 3 + 2]);
        }
        blendShape->blendShape.GetNormalOffsetsAttr().Set(vtNormals);
        return USD_RESULT_SUCCESS;
#else
        blendShape->normalOffsets.resize(count * 3);
        std::memcpy(blendShape->normalOffsets.data(), normalOffsets, count * 3 * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelBlendShape_GetPointIndicesCount(UsdSkelBlendShapeRef blendShape) {
    if (!blendShape) return 0;
    try {
#if USD_USE_FULL
        VtIntArray indices;
        blendShape->blendShape.GetPointIndicesAttr().Get(&indices);
        return indices.size();
#else
        return blendShape->pointIndices.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelBlendShape_GetPointIndices(
    UsdSkelBlendShapeRef blendShape,
    int* outIndices,
    size_t maxCount
) {
    if (!blendShape || !outIndices) return 0;
    try {
#if USD_USE_FULL
        VtIntArray indices;
        blendShape->blendShape.GetPointIndicesAttr().Get(&indices);
        size_t count = std::min(indices.size(), maxCount);
        std::memcpy(outIndices, indices.data(), count * sizeof(int));
        return count;
#else
        size_t count = std::min(blendShape->pointIndices.size(), maxCount);
        std::memcpy(outIndices, blendShape->pointIndices.data(), count * sizeof(int));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelBlendShape_SetPointIndices(
    UsdSkelBlendShapeRef blendShape,
    const int* indices,
    size_t count
) {
    if (!blendShape || !indices) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtIntArray vtIndices(count);
        std::memcpy(vtIndices.data(), indices, count * sizeof(int));
        blendShape->blendShape.GetPointIndicesAttr().Set(vtIndices);
        return USD_RESULT_SUCCESS;
#else
        blendShape->pointIndices.resize(count);
        std::memcpy(blendShape->pointIndices.data(), indices, count * sizeof(int));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdSkelBindingAPI Implementation
// ============================================================================

UsdSkelBindingAPIRef UsdSkelBindingAPI_Get(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelBindingAPI api(prim->prim);
        if (api) {
            return new UsdSkelBindingAPIOpaque(api);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelBindingAPIOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelBindingAPIRef UsdSkelBindingAPI_Apply(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdSkelBindingAPI api = UsdSkelBindingAPI::Apply(prim->prim);
        if (api) {
            return new UsdSkelBindingAPIOpaque(api);
        }
        return nullptr;
#else
        auto* wrapper = new UsdSkelBindingAPIOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdSkelBindingAPIRef UsdSkelBindingAPI_Retain(UsdSkelBindingAPIRef api) {
    if (!api) return nullptr;
    api->refCount.fetch_add(1, std::memory_order_relaxed);
    return api;
}

void UsdSkelBindingAPI_Release(UsdSkelBindingAPIRef api) {
    if (!api) return;
    if (api->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete api;
    }
}

bool UsdSkelBindingAPI_IsValid(UsdSkelBindingAPIRef api) {
    if (!api) return false;
#if USD_USE_FULL
    return api->api;
#else
    return api->valid;
#endif
}

UsdPrimRef UsdSkelBindingAPI_GetPrim(UsdSkelBindingAPIRef api) {
    if (!api) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = api->api.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

SdfPathRef UsdSkelBindingAPI_GetSkeletonPath(UsdSkelBindingAPIRef api) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        UsdRelationship rel = api->api.GetSkeletonRel();
        SdfPathVector targets;
        rel.GetTargets(&targets);
        if (!targets.empty()) {
            return new SdfPathOpaque(targets[0]);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdSkelBindingAPI_SetSkeletonPath(UsdSkelBindingAPIRef api, SdfPathRef path) {
    if (!api || !path) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        UsdRelationship rel = api->api.CreateSkeletonRel();
        rel.SetTargets({path->path});
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

SdfPathRef UsdSkelBindingAPI_GetAnimationSourcePath(UsdSkelBindingAPIRef api) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        UsdRelationship rel = api->api.GetAnimationSourceRel();
        SdfPathVector targets;
        rel.GetTargets(&targets);
        if (!targets.empty()) {
            return new SdfPathOpaque(targets[0]);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdSkelBindingAPI_SetAnimationSourcePath(UsdSkelBindingAPIRef api, SdfPathRef path) {
    if (!api || !path) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        UsdRelationship rel = api->api.CreateAnimationSourceRel();
        rel.SetTargets({path->path});
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelBindingAPI_GetJointIndicesCount(UsdSkelBindingAPIRef api) {
    if (!api) return 0;
    try {
#if USD_USE_FULL
        VtIntArray indices;
        api->api.GetJointIndicesAttr().Get(&indices);
        return indices.size();
#else
        return api->jointIndices.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelBindingAPI_GetJointIndices(
    UsdSkelBindingAPIRef api,
    int* outIndices,
    size_t maxCount
) {
    if (!api || !outIndices) return 0;
    try {
#if USD_USE_FULL
        VtIntArray indices;
        api->api.GetJointIndicesAttr().Get(&indices);
        size_t count = std::min(indices.size(), maxCount);
        std::memcpy(outIndices, indices.data(), count * sizeof(int));
        return count;
#else
        size_t count = std::min(api->jointIndices.size(), maxCount);
        std::memcpy(outIndices, api->jointIndices.data(), count * sizeof(int));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelBindingAPI_SetJointIndices(
    UsdSkelBindingAPIRef api,
    const int* indices,
    size_t count,
    bool constant,
    int elementSize
) {
    if (!api || !indices) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        UsdGeomPrimvar primvar = api->api.CreateJointIndicesPrimvar(constant, elementSize);
        VtIntArray vtIndices(count);
        std::memcpy(vtIndices.data(), indices, count * sizeof(int));
        primvar.Set(vtIndices);
        return USD_RESULT_SUCCESS;
#else
        api->jointIndices.resize(count);
        std::memcpy(api->jointIndices.data(), indices, count * sizeof(int));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdSkelBindingAPI_GetJointWeightsCount(UsdSkelBindingAPIRef api) {
    if (!api) return 0;
    try {
#if USD_USE_FULL
        VtFloatArray weights;
        api->api.GetJointWeightsAttr().Get(&weights);
        return weights.size();
#else
        return api->jointWeights.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdSkelBindingAPI_GetJointWeights(
    UsdSkelBindingAPIRef api,
    float* outWeights,
    size_t maxCount
) {
    if (!api || !outWeights) return 0;
    try {
#if USD_USE_FULL
        VtFloatArray weights;
        api->api.GetJointWeightsAttr().Get(&weights);
        size_t count = std::min(weights.size(), maxCount);
        std::memcpy(outWeights, weights.data(), count * sizeof(float));
        return count;
#else
        size_t count = std::min(api->jointWeights.size(), maxCount);
        std::memcpy(outWeights, api->jointWeights.data(), count * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdSkelBindingAPI_SetJointWeights(
    UsdSkelBindingAPIRef api,
    const float* weights,
    size_t count,
    bool constant,
    int elementSize
) {
    if (!api || !weights) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        UsdGeomPrimvar primvar = api->api.CreateJointWeightsPrimvar(constant, elementSize);
        VtFloatArray vtWeights(count);
        std::memcpy(vtWeights.data(), weights, count * sizeof(float));
        primvar.Set(vtWeights);
        return USD_RESULT_SUCCESS;
#else
        api->jointWeights.resize(count);
        std::memcpy(api->jointWeights.data(), weights, count * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdSkelBindingAPI_SetRigidJointInfluence(
    UsdSkelBindingAPIRef api,
    int jointIndex,
    float weight
) {
    if (!api) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        api->api.SetRigidJointInfluence(jointIndex, weight);
        return USD_RESULT_SUCCESS;
#else
        api->jointIndices = {jointIndex};
        api->jointWeights = {weight};
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdSkelBindingAPI_GetGeomBindTransform(
    UsdSkelBindingAPIRef api,
    double* outMatrix
) {
    if (!api || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        GfMatrix4d matrix;
        if (api->api.GetGeomBindTransformAttr().Get(&matrix)) {
            std::memcpy(outMatrix, matrix.data(), 16 * sizeof(double));
            return USD_RESULT_SUCCESS;
        }
        // Return identity if not set
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
        }
        return USD_RESULT_SUCCESS;
#else
        std::memcpy(outMatrix, api->geomBindTransform, 16 * sizeof(double));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdSkelBindingAPI_SetGeomBindTransform(
    UsdSkelBindingAPIRef api,
    const double* matrix
) {
    if (!api || !matrix) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        GfMatrix4d mat;
        std::memcpy(mat.data(), matrix, 16 * sizeof(double));
        api->api.GetGeomBindTransformAttr().Set(mat);
        return USD_RESULT_SUCCESS;
#else
        std::memcpy(api->geomBindTransform, matrix, 16 * sizeof(double));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdSkelSkinningMethod UsdSkelBindingAPI_GetSkinningMethod(UsdSkelBindingAPIRef api) {
    if (!api) return USDSKEL_SKINNING_METHOD_CLASSIC_LINEAR;
    try {
#if USD_USE_FULL
        TfToken method;
        if (api->api.GetSkinningMethodAttr().Get(&method)) {
            if (method == UsdSkelTokens->dualQuaternion) {
                return USDSKEL_SKINNING_METHOD_DUAL_QUATERNION;
            }
        }
        return USDSKEL_SKINNING_METHOD_CLASSIC_LINEAR;
#else
        return static_cast<UsdSkelSkinningMethod>(api->skinningMethod);
#endif
    } CATCH_AND_RETURN(USDSKEL_SKINNING_METHOD_CLASSIC_LINEAR)
}

UsdResult UsdSkelBindingAPI_SetSkinningMethod(
    UsdSkelBindingAPIRef api,
    UsdSkelSkinningMethod method
) {
    if (!api) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken methodToken;
        switch (method) {
            case USDSKEL_SKINNING_METHOD_DUAL_QUATERNION:
                methodToken = UsdSkelTokens->dualQuaternion;
                break;
            default:
                methodToken = UsdSkelTokens->classicLinear;
                break;
        }
        api->api.CreateSkinningMethodAttr().Set(methodToken);
        return USD_RESULT_SUCCESS;
#else
        api->skinningMethod = static_cast<int>(method);
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

} // extern "C"
