// hgi.cpp - Hgi C wrapper implementation
// Mirrors: pxr/imaging/hgi/*.h (Hydra Graphics Interface - GPU abstraction)

#include "../../include/imaging/hgi.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define HGI_USE_USD 1
    #include <pxr/imaging/hgi/hgi.h>
    #include <pxr/imaging/hgi/buffer.h>
    #include <pxr/imaging/hgi/texture.h>
    #include <pxr/imaging/hgi/sampler.h>
    #include <pxr/imaging/hgi/shaderFunction.h>
    #include <pxr/imaging/hgi/shaderProgram.h>
    #include <pxr/imaging/hgi/graphicsPipeline.h>
    #include <pxr/imaging/hgi/computePipeline.h>
    #include <pxr/imaging/hgi/blitCmds.h>
    #include <pxr/imaging/hgi/capabilities.h>
    #include <pxr/imaging/hgi/types.h>
    #include <pxr/base/tf/token.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define HGI_USE_USD 0
#endif

#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <atomic>
#include <memory>

// Helper to duplicate a std::string as a C string
static char* DuplicateString(const std::string& str) {
    char* result = static_cast<char*>(malloc(str.size() + 1));
    if (result) {
        memcpy(result, str.c_str(), str.size() + 1);
    }
    return result;
}

// ============================================================================
// MARK: - Wrapper Structs
// ============================================================================

struct HgiOpaque {
    std::atomic<int> refCount;
#if HGI_USE_USD
    HgiUniquePtr hgi;
#endif
    std::string apiName;

    HgiOpaque() : refCount(1), apiName("Standalone") {}
};

struct HgiBufferOpaque {
#if HGI_USE_USD
    HgiBufferHandle handle;
    Hgi* hgi;
#endif
    size_t byteSize;
    HgiBufferUsage usage;
    std::vector<uint8_t> standaloneData;

    HgiBufferOpaque() : byteSize(0), usage(0) {}
};

struct HgiTextureOpaque {
#if HGI_USE_USD
    HgiTextureHandle handle;
    Hgi* hgi;
#endif
    HgiFormat format;
    HgiTextureType type;
    int32_t width, height, depth;
    uint16_t layerCount;
    uint16_t mipLevels;
    HgiSampleCount sampleCount;
    std::vector<uint8_t> standaloneData;

    HgiTextureOpaque() : format(HGI_FORMAT_INVALID), type(HGI_TEXTURE_TYPE_2D),
        width(0), height(0), depth(1), layerCount(1), mipLevels(1),
        sampleCount(HGI_SAMPLE_COUNT_1) {}
};

struct HgiSamplerOpaque {
#if HGI_USE_USD
    HgiSamplerHandle handle;
    Hgi* hgi;
#endif
    HgiSamplerDesc desc;

    HgiSamplerOpaque() {}
};

struct HgiShaderFunctionOpaque {
#if HGI_USE_USD
    HgiShaderFunctionHandle handle;
    Hgi* hgi;
#endif
    bool isValid;
    std::string compileErrors;
    std::string shaderCode;
    HgiShaderStage stage;

    HgiShaderFunctionOpaque() : isValid(false), stage(HGI_SHADER_STAGE_VERTEX) {}
};

struct HgiShaderProgramOpaque {
#if HGI_USE_USD
    HgiShaderProgramHandle handle;
    Hgi* hgi;
#endif
    bool isValid;
    std::string compileErrors;
    std::vector<HgiShaderFunctionRef> shaderFunctions;

    HgiShaderProgramOpaque() : isValid(false) {}
};

struct HgiGraphicsPipelineOpaque {
#if HGI_USE_USD
    HgiGraphicsPipelineHandle handle;
    Hgi* hgi;
#endif
    std::string debugName;

    HgiGraphicsPipelineOpaque() {}
};

struct HgiComputePipelineOpaque {
#if HGI_USE_USD
    HgiComputePipelineHandle handle;
    Hgi* hgi;
#endif
    std::string debugName;

    HgiComputePipelineOpaque() {}
};

struct HgiBlitCmdsOpaque {
#if HGI_USE_USD
    HgiBlitCmdsUniquePtr cmds;
    Hgi* hgi;
#endif
    bool isSubmitted;

    HgiBlitCmdsOpaque() : isSubmitted(false) {}
};

struct HgiCapabilitiesOpaque {
#if HGI_USE_USD
    const HgiCapabilities* caps;
#endif
    int apiVersion;
    int shaderVersion;
    size_t maxUniformBlockSize;
    size_t maxShaderStorageBlockSize;
    size_t uniformBufferOffsetAlignment;
    size_t maxClipDistances;
    HgiDeviceCapabilities flags;

    HgiCapabilitiesOpaque() : apiVersion(0), shaderVersion(0),
        maxUniformBlockSize(65536), maxShaderStorageBlockSize(134217728),
        uniformBufferOffsetAlignment(256), maxClipDistances(8), flags(0) {}
};

// Global standalone capabilities instance
static HgiCapabilitiesOpaque g_standaloneCapabilities;

extern "C" {

// ============================================================================
// MARK: - Hgi Main Interface
// ============================================================================

HgiRef Hgi_CreatePlatformDefaultHgi(void) {
#if HGI_USE_USD
    try {
        auto* wrapper = new HgiOpaque();
        wrapper->hgi = Hgi::CreatePlatformDefaultHgi();
        if (!wrapper->hgi) {
            delete wrapper;
            return nullptr;
        }
        wrapper->apiName = wrapper->hgi->GetAPIName().GetString();
        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    // Standalone mode: create a stub Hgi
    auto* wrapper = new HgiOpaque();
    wrapper->apiName = "Standalone";
    return wrapper;
#endif
}

HgiRef Hgi_CreateNamedHgi(const char* hgiType) {
#if HGI_USE_USD
    try {
        auto* wrapper = new HgiOpaque();
        TfToken token;
        if (hgiType && strlen(hgiType) > 0) {
            token = TfToken(hgiType);
        }
        wrapper->hgi = Hgi::CreateNamedHgi(token);
        if (!wrapper->hgi) {
            delete wrapper;
            return nullptr;
        }
        wrapper->apiName = wrapper->hgi->GetAPIName().GetString();
        return wrapper;
    } catch (...) {
        return nullptr;
    }
#else
    auto* wrapper = new HgiOpaque();
    wrapper->apiName = hgiType ? hgiType : "Standalone";
    return wrapper;
#endif
}

HgiRef Hgi_Retain(HgiRef hgi) {
    if (hgi) {
        hgi->refCount.fetch_add(1, std::memory_order_relaxed);
    }
    return hgi;
}

void Hgi_Release(HgiRef hgi) {
    if (hgi) {
        if (hgi->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete hgi;
        }
    }
}

bool Hgi_IsBackendSupported(HgiRef hgi) {
    if (!hgi) return false;
#if HGI_USE_USD
    try {
        return hgi->hgi && hgi->hgi->IsBackendSupported();
    } catch (...) {
        return false;
    }
#else
    return true; // Standalone is always "supported"
#endif
}

bool Hgi_IsSupported(const char* hgiType) {
#if HGI_USE_USD
    try {
        TfToken token;
        if (hgiType && strlen(hgiType) > 0) {
            token = TfToken(hgiType);
        }
        return Hgi::IsSupported(token);
    } catch (...) {
        return false;
    }
#else
    return true; // Standalone mode
#endif
}

UsdResult Hgi_GetAPIName(HgiRef hgi, char** outName) {
    if (!hgi || !outName) return USD_RESULT_INVALID_ARGUMENT;

    *outName = DuplicateString(hgi->apiName);
    return *outName ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
}

HgiCapabilitiesRef Hgi_GetCapabilities(HgiRef hgi) {
    if (!hgi) return nullptr;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            static HgiCapabilitiesOpaque capsWrapper;
            capsWrapper.caps = hgi->hgi->GetCapabilities();
            if (capsWrapper.caps) {
                capsWrapper.apiVersion = capsWrapper.caps->GetAPIVersion();
                capsWrapper.shaderVersion = capsWrapper.caps->GetShaderVersion();
                capsWrapper.maxUniformBlockSize = capsWrapper.caps->GetMaxUniformBlockSize();
                capsWrapper.maxShaderStorageBlockSize = capsWrapper.caps->GetMaxShaderStorageBlockSize();
                capsWrapper.uniformBufferOffsetAlignment = capsWrapper.caps->GetUniformBufferOffsetAlignment();
                capsWrapper.maxClipDistances = capsWrapper.caps->GetMaxClipDistances();
            }
            return &capsWrapper;
        }
    } catch (...) {}
#endif
    return &g_standaloneCapabilities;
}

void Hgi_StartFrame(HgiRef hgi) {
    if (!hgi) return;
#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            hgi->hgi->StartFrame();
        }
    } catch (...) {}
#endif
}

void Hgi_EndFrame(HgiRef hgi) {
    if (!hgi) return;
#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            hgi->hgi->EndFrame();
        }
    } catch (...) {}
#endif
}

void Hgi_GarbageCollect(HgiRef hgi) {
    if (!hgi) return;
#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            hgi->hgi->GarbageCollect();
        }
    } catch (...) {}
#endif
}

// ============================================================================
// MARK: - HgiCapabilities
// ============================================================================

bool HgiCapabilities_IsSet(HgiCapabilitiesRef caps, HgiDeviceCapabilities mask) {
    if (!caps) return false;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->IsSet(static_cast<pxr::HgiDeviceCapabilities>(mask));
    }
#endif
    return (caps->flags & mask) != 0;
}

int HgiCapabilities_GetAPIVersion(HgiCapabilitiesRef caps) {
    if (!caps) return 0;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->GetAPIVersion();
    }
#endif
    return caps->apiVersion;
}

int HgiCapabilities_GetShaderVersion(HgiCapabilitiesRef caps) {
    if (!caps) return 0;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->GetShaderVersion();
    }
#endif
    return caps->shaderVersion;
}

size_t HgiCapabilities_GetMaxUniformBlockSize(HgiCapabilitiesRef caps) {
    if (!caps) return 0;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->GetMaxUniformBlockSize();
    }
#endif
    return caps->maxUniformBlockSize;
}

size_t HgiCapabilities_GetMaxShaderStorageBlockSize(HgiCapabilitiesRef caps) {
    if (!caps) return 0;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->GetMaxShaderStorageBlockSize();
    }
#endif
    return caps->maxShaderStorageBlockSize;
}

size_t HgiCapabilities_GetUniformBufferOffsetAlignment(HgiCapabilitiesRef caps) {
    if (!caps) return 0;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->GetUniformBufferOffsetAlignment();
    }
#endif
    return caps->uniformBufferOffsetAlignment;
}

size_t HgiCapabilities_GetMaxClipDistances(HgiCapabilitiesRef caps) {
    if (!caps) return 0;
#if HGI_USE_USD
    if (caps->caps) {
        return caps->caps->GetMaxClipDistances();
    }
#endif
    return caps->maxClipDistances;
}

// ============================================================================
// MARK: - HgiBuffer
// ============================================================================

HgiBufferRef Hgi_CreateBuffer(HgiRef hgi, const HgiBufferDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiBufferOpaque();
    wrapper->byteSize = desc->byteSize;
    wrapper->usage = desc->usage;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiBufferDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;
            pxrDesc.usage = static_cast<pxr::HgiBufferUsage>(desc->usage);
            pxrDesc.byteSize = desc->byteSize;
            pxrDesc.vertexStride = desc->vertexStride;
            pxrDesc.initialData = desc->initialData;

            wrapper->handle = hgi->hgi->CreateBuffer(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    // Standalone mode: store data locally
    if (desc->initialData && desc->byteSize > 0) {
        wrapper->standaloneData.resize(desc->byteSize);
        memcpy(wrapper->standaloneData.data(), desc->initialData, desc->byteSize);
    } else {
        wrapper->standaloneData.resize(desc->byteSize, 0);
    }
    return wrapper;
}

void Hgi_DestroyBuffer(HgiRef hgi, HgiBufferRef* bufferHandle) {
    if (!hgi || !bufferHandle || !*bufferHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*bufferHandle)->handle) {
            hgi->hgi->DestroyBuffer(&(*bufferHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *bufferHandle;
    *bufferHandle = nullptr;
}

size_t HgiBuffer_GetByteSizeOfResource(HgiBufferRef buffer) {
    if (!buffer) return 0;
#if HGI_USE_USD
    if (buffer->handle) {
        try {
            return buffer->handle->GetByteSizeOfResource();
        } catch (...) {}
    }
#endif
    return buffer->byteSize;
}

uint64_t HgiBuffer_GetRawResource(HgiBufferRef buffer) {
    if (!buffer) return 0;
#if HGI_USE_USD
    if (buffer->handle) {
        try {
            return buffer->handle->GetRawResource();
        } catch (...) {}
    }
#endif
    return reinterpret_cast<uint64_t>(buffer->standaloneData.data());
}

void* HgiBuffer_GetCPUStagingAddress(HgiBufferRef buffer) {
    if (!buffer) return nullptr;
#if HGI_USE_USD
    if (buffer->handle) {
        try {
            return buffer->handle->GetCPUStagingAddress();
        } catch (...) {}
    }
#endif
    return buffer->standaloneData.data();
}

HgiBufferUsage HgiBuffer_GetUsage(HgiBufferRef buffer) {
    if (!buffer) return 0;
    return buffer->usage;
}

size_t HgiBuffer_GetByteSize(HgiBufferRef buffer) {
    if (!buffer) return 0;
    return buffer->byteSize;
}

// ============================================================================
// MARK: - HgiTexture
// ============================================================================

HgiTextureRef Hgi_CreateTexture(HgiRef hgi, const HgiTextureDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiTextureOpaque();
    wrapper->format = desc->format;
    wrapper->type = desc->type;
    wrapper->width = desc->width;
    wrapper->height = desc->height;
    wrapper->depth = desc->depth;
    wrapper->layerCount = desc->layerCount;
    wrapper->mipLevels = desc->mipLevels;
    wrapper->sampleCount = desc->sampleCount;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiTextureDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;
            pxrDesc.usage = static_cast<pxr::HgiTextureUsage>(desc->usage);
            pxrDesc.format = static_cast<pxr::HgiFormat>(desc->format);
            pxrDesc.componentMapping.r = static_cast<pxr::HgiComponentSwizzle>(desc->componentMapping.r);
            pxrDesc.componentMapping.g = static_cast<pxr::HgiComponentSwizzle>(desc->componentMapping.g);
            pxrDesc.componentMapping.b = static_cast<pxr::HgiComponentSwizzle>(desc->componentMapping.b);
            pxrDesc.componentMapping.a = static_cast<pxr::HgiComponentSwizzle>(desc->componentMapping.a);
            pxrDesc.type = static_cast<pxr::HgiTextureType>(desc->type);
            pxrDesc.dimensions = GfVec3i(desc->width, desc->height, desc->depth);
            pxrDesc.layerCount = desc->layerCount;
            pxrDesc.mipLevels = desc->mipLevels;
            pxrDesc.sampleCount = static_cast<pxr::HgiSampleCount>(desc->sampleCount);
            pxrDesc.pixelsByteSize = desc->pixelsByteSize;
            pxrDesc.initialData = desc->initialData;

            wrapper->handle = hgi->hgi->CreateTexture(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    // Standalone mode: store data locally
    if (desc->initialData && desc->pixelsByteSize > 0) {
        wrapper->standaloneData.resize(desc->pixelsByteSize);
        memcpy(wrapper->standaloneData.data(), desc->initialData, desc->pixelsByteSize);
    }
    return wrapper;
}

void Hgi_DestroyTexture(HgiRef hgi, HgiTextureRef* textureHandle) {
    if (!hgi || !textureHandle || !*textureHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*textureHandle)->handle) {
            hgi->hgi->DestroyTexture(&(*textureHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *textureHandle;
    *textureHandle = nullptr;
}

size_t HgiTexture_GetByteSizeOfResource(HgiTextureRef texture) {
    if (!texture) return 0;
#if HGI_USE_USD
    if (texture->handle) {
        try {
            return texture->handle->GetByteSizeOfResource();
        } catch (...) {}
    }
#endif
    return texture->standaloneData.size();
}

uint64_t HgiTexture_GetRawResource(HgiTextureRef texture) {
    if (!texture) return 0;
#if HGI_USE_USD
    if (texture->handle) {
        try {
            return texture->handle->GetRawResource();
        } catch (...) {}
    }
#endif
    return reinterpret_cast<uint64_t>(texture->standaloneData.data());
}

HgiFormat HgiTexture_GetFormat(HgiTextureRef texture) {
    if (!texture) return HGI_FORMAT_INVALID;
    return texture->format;
}

void HgiTexture_GetDimensions(HgiTextureRef texture, int32_t* outWidth, int32_t* outHeight, int32_t* outDepth) {
    if (!texture) {
        if (outWidth) *outWidth = 0;
        if (outHeight) *outHeight = 0;
        if (outDepth) *outDepth = 0;
        return;
    }
    if (outWidth) *outWidth = texture->width;
    if (outHeight) *outHeight = texture->height;
    if (outDepth) *outDepth = texture->depth;
}

HgiTextureType HgiTexture_GetType(HgiTextureRef texture) {
    if (!texture) return HGI_TEXTURE_TYPE_2D;
    return texture->type;
}

uint16_t HgiTexture_GetLayerCount(HgiTextureRef texture) {
    if (!texture) return 1;
    return texture->layerCount;
}

uint16_t HgiTexture_GetMipLevels(HgiTextureRef texture) {
    if (!texture) return 1;
    return texture->mipLevels;
}

HgiSampleCount HgiTexture_GetSampleCount(HgiTextureRef texture) {
    if (!texture) return HGI_SAMPLE_COUNT_1;
    return texture->sampleCount;
}

// ============================================================================
// MARK: - HgiSampler
// ============================================================================

HgiSamplerRef Hgi_CreateSampler(HgiRef hgi, const HgiSamplerDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiSamplerOpaque();
    wrapper->desc = *desc;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiSamplerDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;
            pxrDesc.magFilter = static_cast<pxr::HgiSamplerFilter>(desc->magFilter);
            pxrDesc.minFilter = static_cast<pxr::HgiSamplerFilter>(desc->minFilter);
            pxrDesc.mipFilter = static_cast<pxr::HgiMipFilter>(desc->mipFilter);
            pxrDesc.addressModeU = static_cast<pxr::HgiSamplerAddressMode>(desc->addressModeU);
            pxrDesc.addressModeV = static_cast<pxr::HgiSamplerAddressMode>(desc->addressModeV);
            pxrDesc.addressModeW = static_cast<pxr::HgiSamplerAddressMode>(desc->addressModeW);
            pxrDesc.borderColor = static_cast<pxr::HgiBorderColor>(desc->borderColor);
            pxrDesc.enableCompare = desc->enableCompare;
            pxrDesc.compareFunction = static_cast<pxr::HgiCompareFunction>(desc->compareFunction);
            pxrDesc.maxAnisotropy = desc->maxAnisotropy;

            wrapper->handle = hgi->hgi->CreateSampler(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    return wrapper;
}

void Hgi_DestroySampler(HgiRef hgi, HgiSamplerRef* samplerHandle) {
    if (!hgi || !samplerHandle || !*samplerHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*samplerHandle)->handle) {
            hgi->hgi->DestroySampler(&(*samplerHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *samplerHandle;
    *samplerHandle = nullptr;
}

uint64_t HgiSampler_GetRawResource(HgiSamplerRef sampler) {
    if (!sampler) return 0;
#if HGI_USE_USD
    if (sampler->handle) {
        try {
            return sampler->handle->GetRawResource();
        } catch (...) {}
    }
#endif
    return 0;
}

// ============================================================================
// MARK: - HgiShaderFunction
// ============================================================================

HgiShaderFunctionRef Hgi_CreateShaderFunction(HgiRef hgi, const HgiShaderFunctionDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiShaderFunctionOpaque();
    wrapper->stage = desc->shaderStage;
    if (desc->shaderCode) {
        wrapper->shaderCode = std::string(desc->shaderCode, desc->shaderCodeSize > 0 ? desc->shaderCodeSize : strlen(desc->shaderCode));
    }

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiShaderFunctionDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;
            pxrDesc.shaderStage = static_cast<pxr::HgiShaderStage>(desc->shaderStage);
            pxrDesc.shaderCode = wrapper->shaderCode;

            wrapper->handle = hgi->hgi->CreateShaderFunction(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            wrapper->isValid = wrapper->handle && wrapper->handle->IsValid();
            if (!wrapper->isValid && wrapper->handle) {
                wrapper->compileErrors = wrapper->handle->GetCompileErrors();
            }
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    // Standalone mode: always valid (no actual compilation)
    wrapper->isValid = true;
    return wrapper;
}

void Hgi_DestroyShaderFunction(HgiRef hgi, HgiShaderFunctionRef* shaderFunctionHandle) {
    if (!hgi || !shaderFunctionHandle || !*shaderFunctionHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*shaderFunctionHandle)->handle) {
            hgi->hgi->DestroyShaderFunction(&(*shaderFunctionHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *shaderFunctionHandle;
    *shaderFunctionHandle = nullptr;
}

bool HgiShaderFunction_IsValid(HgiShaderFunctionRef shaderFunction) {
    if (!shaderFunction) return false;
    return shaderFunction->isValid;
}

UsdResult HgiShaderFunction_GetCompileErrors(HgiShaderFunctionRef shaderFunction, char** outErrors) {
    if (!shaderFunction || !outErrors) return USD_RESULT_INVALID_ARGUMENT;

    *outErrors = DuplicateString(shaderFunction->compileErrors);
    return *outErrors ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
}

size_t HgiShaderFunction_GetByteSizeOfResource(HgiShaderFunctionRef shaderFunction) {
    if (!shaderFunction) return 0;
#if HGI_USE_USD
    if (shaderFunction->handle) {
        try {
            return shaderFunction->handle->GetByteSizeOfResource();
        } catch (...) {}
    }
#endif
    return shaderFunction->shaderCode.size();
}

uint64_t HgiShaderFunction_GetRawResource(HgiShaderFunctionRef shaderFunction) {
    if (!shaderFunction) return 0;
#if HGI_USE_USD
    if (shaderFunction->handle) {
        try {
            return shaderFunction->handle->GetRawResource();
        } catch (...) {}
    }
#endif
    return 0;
}

// ============================================================================
// MARK: - HgiShaderProgram
// ============================================================================

HgiShaderProgramRef Hgi_CreateShaderProgram(HgiRef hgi, const HgiShaderProgramDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiShaderProgramOpaque();

    // Store shader function references
    for (size_t i = 0; i < desc->shaderFunctionCount; ++i) {
        wrapper->shaderFunctions.push_back(desc->shaderFunctions[i]);
    }

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiShaderProgramDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;

            for (size_t i = 0; i < desc->shaderFunctionCount; ++i) {
                if (desc->shaderFunctions[i] && desc->shaderFunctions[i]->handle) {
                    pxrDesc.shaderFunctions.push_back(desc->shaderFunctions[i]->handle);
                }
            }

            wrapper->handle = hgi->hgi->CreateShaderProgram(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            wrapper->isValid = wrapper->handle && wrapper->handle->IsValid();
            if (!wrapper->isValid && wrapper->handle) {
                wrapper->compileErrors = wrapper->handle->GetCompileErrors();
            }
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    // Standalone mode: always valid
    wrapper->isValid = true;
    return wrapper;
}

void Hgi_DestroyShaderProgram(HgiRef hgi, HgiShaderProgramRef* shaderProgramHandle) {
    if (!hgi || !shaderProgramHandle || !*shaderProgramHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*shaderProgramHandle)->handle) {
            hgi->hgi->DestroyShaderProgram(&(*shaderProgramHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *shaderProgramHandle;
    *shaderProgramHandle = nullptr;
}

bool HgiShaderProgram_IsValid(HgiShaderProgramRef shaderProgram) {
    if (!shaderProgram) return false;
    return shaderProgram->isValid;
}

UsdResult HgiShaderProgram_GetCompileErrors(HgiShaderProgramRef shaderProgram, char** outErrors) {
    if (!shaderProgram || !outErrors) return USD_RESULT_INVALID_ARGUMENT;

    *outErrors = DuplicateString(shaderProgram->compileErrors);
    return *outErrors ? USD_RESULT_SUCCESS : USD_RESULT_OUT_OF_MEMORY;
}

size_t HgiShaderProgram_GetByteSizeOfResource(HgiShaderProgramRef shaderProgram) {
    if (!shaderProgram) return 0;
#if HGI_USE_USD
    if (shaderProgram->handle) {
        try {
            return shaderProgram->handle->GetByteSizeOfResource();
        } catch (...) {}
    }
#endif
    return 0;
}

uint64_t HgiShaderProgram_GetRawResource(HgiShaderProgramRef shaderProgram) {
    if (!shaderProgram) return 0;
#if HGI_USE_USD
    if (shaderProgram->handle) {
        try {
            return shaderProgram->handle->GetRawResource();
        } catch (...) {}
    }
#endif
    return 0;
}

// ============================================================================
// MARK: - HgiGraphicsPipeline
// ============================================================================

HgiGraphicsPipelineRef Hgi_CreateGraphicsPipeline(HgiRef hgi, const HgiGraphicsPipelineDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiGraphicsPipelineOpaque();
    if (desc->debugName) wrapper->debugName = desc->debugName;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiGraphicsPipelineDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;
            pxrDesc.primitiveType = static_cast<pxr::HgiPrimitiveType>(desc->primitiveType);

            if (desc->shaderProgram && desc->shaderProgram->handle) {
                pxrDesc.shaderProgram = desc->shaderProgram->handle;
            }

            // Depth state
            pxrDesc.depthState.depthTestEnabled = desc->depthState.depthTestEnabled;
            pxrDesc.depthState.depthWriteEnabled = desc->depthState.depthWriteEnabled;
            pxrDesc.depthState.depthCompareFn = static_cast<pxr::HgiCompareFunction>(desc->depthState.depthCompareFn);
            pxrDesc.depthState.depthBiasEnabled = desc->depthState.depthBiasEnabled;
            pxrDesc.depthState.depthBiasConstantFactor = desc->depthState.depthBiasConstantFactor;
            pxrDesc.depthState.depthBiasSlopeFactor = desc->depthState.depthBiasSlopeFactor;
            pxrDesc.depthState.stencilTestEnabled = desc->depthState.stencilTestEnabled;

            // Multi-sample state
            pxrDesc.multiSampleState.multiSampleEnable = desc->multiSampleState.multiSampleEnable;
            pxrDesc.multiSampleState.alphaToCoverageEnable = desc->multiSampleState.alphaToCoverageEnable;
            pxrDesc.multiSampleState.alphaToOneEnable = desc->multiSampleState.alphaToOneEnable;
            pxrDesc.multiSampleState.sampleCount = static_cast<pxr::HgiSampleCount>(desc->multiSampleState.sampleCount);

            // Rasterization state
            pxrDesc.rasterizationState.polygonMode = static_cast<pxr::HgiPolygonMode>(desc->rasterizationState.polygonMode);
            pxrDesc.rasterizationState.lineWidth = desc->rasterizationState.lineWidth;
            pxrDesc.rasterizationState.cullMode = static_cast<pxr::HgiCullMode>(desc->rasterizationState.cullMode);
            pxrDesc.rasterizationState.winding = static_cast<pxr::HgiWinding>(desc->rasterizationState.winding);
            pxrDesc.rasterizationState.rasterizerEnabled = desc->rasterizationState.rasterizerEnabled;
            pxrDesc.rasterizationState.depthClampEnabled = desc->rasterizationState.depthClampEnabled;
            pxrDesc.rasterizationState.depthRange = GfVec2f(desc->rasterizationState.depthRangeMin, desc->rasterizationState.depthRangeMax);
            pxrDesc.rasterizationState.conservativeRaster = desc->rasterizationState.conservativeRaster;
            pxrDesc.rasterizationState.numClipDistances = desc->rasterizationState.numClipDistances;

            pxrDesc.resolveAttachments = desc->resolveAttachments;

            wrapper->handle = hgi->hgi->CreateGraphicsPipeline(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    return wrapper;
}

void Hgi_DestroyGraphicsPipeline(HgiRef hgi, HgiGraphicsPipelineRef* pipelineHandle) {
    if (!hgi || !pipelineHandle || !*pipelineHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*pipelineHandle)->handle) {
            hgi->hgi->DestroyGraphicsPipeline(&(*pipelineHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *pipelineHandle;
    *pipelineHandle = nullptr;
}

// ============================================================================
// MARK: - HgiComputePipeline
// ============================================================================

HgiComputePipelineRef Hgi_CreateComputePipeline(HgiRef hgi, const HgiComputePipelineDesc* desc) {
    if (!hgi || !desc) return nullptr;

    auto* wrapper = new HgiComputePipelineOpaque();
    if (desc->debugName) wrapper->debugName = desc->debugName;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            pxr::HgiComputePipelineDesc pxrDesc;
            if (desc->debugName) pxrDesc.debugName = desc->debugName;

            if (desc->shaderProgram && desc->shaderProgram->handle) {
                pxrDesc.shaderProgram = desc->shaderProgram->handle;
            }
            pxrDesc.shaderConstantsDesc.byteSize = desc->shaderConstantsByteSize;

            wrapper->handle = hgi->hgi->CreateComputePipeline(pxrDesc);
            wrapper->hgi = hgi->hgi.get();
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    return wrapper;
}

void Hgi_DestroyComputePipeline(HgiRef hgi, HgiComputePipelineRef* pipelineHandle) {
    if (!hgi || !pipelineHandle || !*pipelineHandle) return;

#if HGI_USE_USD
    try {
        if (hgi->hgi && (*pipelineHandle)->handle) {
            hgi->hgi->DestroyComputePipeline(&(*pipelineHandle)->handle);
        }
    } catch (...) {}
#endif

    delete *pipelineHandle;
    *pipelineHandle = nullptr;
}

// ============================================================================
// MARK: - HgiBlitCmds
// ============================================================================

HgiBlitCmdsRef Hgi_CreateBlitCmds(HgiRef hgi) {
    if (!hgi) return nullptr;

    auto* wrapper = new HgiBlitCmdsOpaque();

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            wrapper->cmds = hgi->hgi->CreateBlitCmds();
            wrapper->hgi = hgi->hgi.get();
            return wrapper;
        }
    } catch (...) {
        delete wrapper;
        return nullptr;
    }
#endif

    return wrapper;
}

void HgiBlitCmds_Release(HgiBlitCmdsRef blitCmds) {
    if (!blitCmds) return;

#if HGI_USE_USD
    // Auto-submit if not already submitted
    if (!blitCmds->isSubmitted && blitCmds->cmds && blitCmds->hgi) {
        try {
            blitCmds->hgi->SubmitCmds(blitCmds->cmds.get());
        } catch (...) {}
    }
#endif

    delete blitCmds;
}

bool HgiBlitCmds_IsSubmitted(HgiBlitCmdsRef blitCmds) {
    if (!blitCmds) return false;
    return blitCmds->isSubmitted;
}

void HgiBlitCmds_CopyBufferCpuToGpu(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef dstBuffer,
    size_t dstByteOffset,
    const void* srcData,
    size_t srcByteSize
) {
    if (!blitCmds || !dstBuffer || !srcData) return;

#if HGI_USE_USD
    if (blitCmds->cmds && dstBuffer->handle) {
        try {
            pxr::HgiBufferCpuToGpuOp op;
            op.cpuSourceBuffer = srcData;
            op.sourceByteOffset = 0;
            op.gpuDestinationBuffer = dstBuffer->handle;
            op.destinationByteOffset = dstByteOffset;
            op.byteSize = srcByteSize;
            blitCmds->cmds->CopyBufferCpuToGpu(op);
            return;
        } catch (...) {}
    }
#endif

    // Standalone mode: copy to buffer's local storage
    if (dstByteOffset + srcByteSize <= dstBuffer->standaloneData.size()) {
        memcpy(dstBuffer->standaloneData.data() + dstByteOffset, srcData, srcByteSize);
    }
}

void HgiBlitCmds_CopyBufferGpuToCpu(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef srcBuffer,
    size_t srcByteOffset,
    void* dstData,
    size_t dstByteSize
) {
    if (!blitCmds || !srcBuffer || !dstData) return;

#if HGI_USE_USD
    if (blitCmds->cmds && srcBuffer->handle) {
        try {
            pxr::HgiBufferGpuToCpuOp op;
            op.gpuSourceBuffer = srcBuffer->handle;
            op.sourceByteOffset = srcByteOffset;
            op.cpuDestinationBuffer = dstData;
            op.destinationByteOffset = 0;
            op.byteSize = dstByteSize;
            blitCmds->cmds->CopyBufferGpuToCpu(op);
            return;
        } catch (...) {}
    }
#endif

    // Standalone mode: copy from buffer's local storage
    if (srcByteOffset + dstByteSize <= srcBuffer->standaloneData.size()) {
        memcpy(dstData, srcBuffer->standaloneData.data() + srcByteOffset, dstByteSize);
    }
}

void HgiBlitCmds_CopyBufferGpuToGpu(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef srcBuffer,
    size_t srcByteOffset,
    HgiBufferRef dstBuffer,
    size_t dstByteOffset,
    size_t byteSize
) {
    if (!blitCmds || !srcBuffer || !dstBuffer) return;

#if HGI_USE_USD
    if (blitCmds->cmds && srcBuffer->handle && dstBuffer->handle) {
        try {
            pxr::HgiBufferGpuToGpuOp op;
            op.gpuSourceBuffer = srcBuffer->handle;
            op.sourceByteOffset = srcByteOffset;
            op.gpuDestinationBuffer = dstBuffer->handle;
            op.destinationByteOffset = dstByteOffset;
            op.byteSize = byteSize;
            blitCmds->cmds->CopyBufferGpuToGpu(op);
            return;
        } catch (...) {}
    }
#endif

    // Standalone mode: copy between buffers
    if (srcByteOffset + byteSize <= srcBuffer->standaloneData.size() &&
        dstByteOffset + byteSize <= dstBuffer->standaloneData.size()) {
        memcpy(dstBuffer->standaloneData.data() + dstByteOffset,
               srcBuffer->standaloneData.data() + srcByteOffset,
               byteSize);
    }
}

void HgiBlitCmds_CopyTextureCpuToGpu(
    HgiBlitCmdsRef blitCmds,
    HgiTextureRef dstTexture,
    uint16_t dstMipLevel,
    uint16_t dstLayer,
    const void* srcData,
    size_t srcByteSize
) {
    if (!blitCmds || !dstTexture || !srcData) return;

#if HGI_USE_USD
    if (blitCmds->cmds && dstTexture->handle) {
        try {
            pxr::HgiTextureCpuToGpuOp op;
            op.cpuSourceBuffer = srcData;
            op.bufferByteSize = srcByteSize;
            op.gpuDestinationTexture = dstTexture->handle;
            op.destinationTexelOffset = GfVec3i(0, 0, 0);
            op.mipLevel = dstMipLevel;
            blitCmds->cmds->CopyTextureCpuToGpu(op);
            return;
        } catch (...) {}
    }
#endif

    // Standalone mode: copy to texture's local storage
    if (srcByteSize <= dstTexture->standaloneData.size()) {
        memcpy(dstTexture->standaloneData.data(), srcData, srcByteSize);
    }
}

void HgiBlitCmds_GenerateMipMaps(HgiBlitCmdsRef blitCmds, HgiTextureRef texture) {
    if (!blitCmds || !texture) return;

#if HGI_USE_USD
    if (blitCmds->cmds && texture->handle) {
        try {
            blitCmds->cmds->GenerateMipMaps(texture->handle);
        } catch (...) {}
    }
#endif
    // Standalone mode: no-op (mipmaps not supported)
}

void HgiBlitCmds_FillBuffer(
    HgiBlitCmdsRef blitCmds,
    HgiBufferRef buffer,
    uint8_t value
) {
    if (!blitCmds || !buffer) return;

#if HGI_USE_USD
    if (blitCmds->cmds && buffer->handle) {
        try {
            blitCmds->cmds->FillBuffer(buffer->handle, value);
            return;
        } catch (...) {}
    }
#endif

    // Standalone mode: fill local buffer
    memset(buffer->standaloneData.data(), value, buffer->standaloneData.size());
}

// ============================================================================
// MARK: - Command Submission
// ============================================================================

UsdResult Hgi_SubmitCmds(HgiRef hgi, void* cmds, HgiSubmitWaitType wait) {
    if (!hgi || !cmds) return USD_RESULT_INVALID_ARGUMENT;

#if HGI_USE_USD
    try {
        if (hgi->hgi) {
            // Try to cast to different command types
            HgiBlitCmdsRef blitCmds = static_cast<HgiBlitCmdsRef>(cmds);
            if (blitCmds && blitCmds->cmds) {
                hgi->hgi->SubmitCmds(blitCmds->cmds.get(), static_cast<pxr::HgiSubmitWaitType>(wait));
                blitCmds->isSubmitted = true;
                return USD_RESULT_SUCCESS;
            }
        }
    } catch (...) {
        return USD_RESULT_ERROR;
    }
#endif

    // Standalone mode: mark as submitted
    HgiBlitCmdsRef blitCmds = static_cast<HgiBlitCmdsRef>(cmds);
    if (blitCmds) {
        blitCmds->isSubmitted = true;
    }
    return USD_RESULT_SUCCESS;
}

// ============================================================================
// MARK: - Format Utilities
// ============================================================================

size_t HgiFormat_GetComponentCount(HgiFormat format) {
#if HGI_USE_USD
    try {
        return pxr::HgiGetComponentCount(static_cast<pxr::HgiFormat>(format));
    } catch (...) {}
#endif

    // Standalone implementation
    switch (format) {
        case HGI_FORMAT_UNORM8:
        case HGI_FORMAT_SNORM8:
        case HGI_FORMAT_FLOAT16:
        case HGI_FORMAT_FLOAT32:
        case HGI_FORMAT_INT16:
        case HGI_FORMAT_UINT16:
        case HGI_FORMAT_INT32:
            return 1;

        case HGI_FORMAT_UNORM8_VEC2:
        case HGI_FORMAT_SNORM8_VEC2:
        case HGI_FORMAT_FLOAT16_VEC2:
        case HGI_FORMAT_FLOAT32_VEC2:
        case HGI_FORMAT_INT16_VEC2:
        case HGI_FORMAT_UINT16_VEC2:
        case HGI_FORMAT_INT32_VEC2:
            return 2;

        case HGI_FORMAT_FLOAT16_VEC3:
        case HGI_FORMAT_FLOAT32_VEC3:
        case HGI_FORMAT_INT16_VEC3:
        case HGI_FORMAT_UINT16_VEC3:
        case HGI_FORMAT_INT32_VEC3:
        case HGI_FORMAT_BC6_FLOAT_VEC3:
        case HGI_FORMAT_BC6_UFLOAT_VEC3:
            return 3;

        case HGI_FORMAT_UNORM8_VEC4:
        case HGI_FORMAT_SNORM8_VEC4:
        case HGI_FORMAT_FLOAT16_VEC4:
        case HGI_FORMAT_FLOAT32_VEC4:
        case HGI_FORMAT_INT16_VEC4:
        case HGI_FORMAT_UINT16_VEC4:
        case HGI_FORMAT_INT32_VEC4:
        case HGI_FORMAT_UNORM8_VEC4_SRGB:
        case HGI_FORMAT_BC7_UNORM8_VEC4:
        case HGI_FORMAT_BC7_UNORM8_VEC4_SRGB:
        case HGI_FORMAT_BC1_UNORM8_VEC4:
        case HGI_FORMAT_BC3_UNORM8_VEC4:
        case HGI_FORMAT_PACKED_INT1010102:
            return 4;

        default:
            return 0;
    }
}

size_t HgiFormat_GetDataSize(HgiFormat format) {
#if HGI_USE_USD
    try {
        return pxr::HgiGetDataSizeOfFormat(static_cast<pxr::HgiFormat>(format));
    } catch (...) {}
#endif

    // Standalone implementation
    switch (format) {
        case HGI_FORMAT_UNORM8:
        case HGI_FORMAT_SNORM8:
            return 1;
        case HGI_FORMAT_UNORM8_VEC2:
        case HGI_FORMAT_SNORM8_VEC2:
        case HGI_FORMAT_FLOAT16:
        case HGI_FORMAT_INT16:
        case HGI_FORMAT_UINT16:
            return 2;
        case HGI_FORMAT_FLOAT16_VEC2:
        case HGI_FORMAT_INT16_VEC2:
        case HGI_FORMAT_UINT16_VEC2:
        case HGI_FORMAT_UNORM8_VEC4:
        case HGI_FORMAT_SNORM8_VEC4:
        case HGI_FORMAT_UNORM8_VEC4_SRGB:
        case HGI_FORMAT_FLOAT32:
        case HGI_FORMAT_INT32:
        case HGI_FORMAT_PACKED_INT1010102:
            return 4;
        case HGI_FORMAT_FLOAT16_VEC3:
        case HGI_FORMAT_INT16_VEC3:
        case HGI_FORMAT_UINT16_VEC3:
            return 6;
        case HGI_FORMAT_FLOAT16_VEC4:
        case HGI_FORMAT_INT16_VEC4:
        case HGI_FORMAT_UINT16_VEC4:
        case HGI_FORMAT_FLOAT32_VEC2:
        case HGI_FORMAT_INT32_VEC2:
            return 8;
        case HGI_FORMAT_FLOAT32_VEC3:
        case HGI_FORMAT_INT32_VEC3:
            return 12;
        case HGI_FORMAT_FLOAT32_VEC4:
        case HGI_FORMAT_INT32_VEC4:
        case HGI_FORMAT_BC6_FLOAT_VEC3:
        case HGI_FORMAT_BC6_UFLOAT_VEC3:
        case HGI_FORMAT_BC7_UNORM8_VEC4:
        case HGI_FORMAT_BC7_UNORM8_VEC4_SRGB:
            return 16;
        case HGI_FORMAT_BC1_UNORM8_VEC4:
            return 8;
        case HGI_FORMAT_BC3_UNORM8_VEC4:
            return 16;
        case HGI_FORMAT_FLOAT32_UINT8:
            return 5;
        default:
            return 0;
    }
}

bool HgiFormat_IsCompressed(HgiFormat format) {
#if HGI_USE_USD
    try {
        return pxr::HgiIsCompressed(static_cast<pxr::HgiFormat>(format));
    } catch (...) {}
#endif

    switch (format) {
        case HGI_FORMAT_BC6_FLOAT_VEC3:
        case HGI_FORMAT_BC6_UFLOAT_VEC3:
        case HGI_FORMAT_BC7_UNORM8_VEC4:
        case HGI_FORMAT_BC7_UNORM8_VEC4_SRGB:
        case HGI_FORMAT_BC1_UNORM8_VEC4:
        case HGI_FORMAT_BC3_UNORM8_VEC4:
            return true;
        default:
            return false;
    }
}

bool HgiFormat_IsFloat(HgiFormat format) {
#if HGI_USE_USD
    try {
        return pxr::HgiIsFloatFormat(static_cast<pxr::HgiFormat>(format));
    } catch (...) {}
#endif

    switch (format) {
        case HGI_FORMAT_FLOAT16:
        case HGI_FORMAT_FLOAT16_VEC2:
        case HGI_FORMAT_FLOAT16_VEC3:
        case HGI_FORMAT_FLOAT16_VEC4:
        case HGI_FORMAT_FLOAT32:
        case HGI_FORMAT_FLOAT32_VEC2:
        case HGI_FORMAT_FLOAT32_VEC3:
        case HGI_FORMAT_FLOAT32_VEC4:
        case HGI_FORMAT_BC6_FLOAT_VEC3:
        case HGI_FORMAT_BC6_UFLOAT_VEC3:
            return true;
        default:
            return false;
    }
}

// ============================================================================
// MARK: - Memory Management
// ============================================================================

void Hgi_FreeString(char* str) {
    free(str);
}

} // extern "C"
