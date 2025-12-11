//
// Copyright 2025 Afloat Technologies
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "Hio/imageUtils.h"

#include "Tf/diagnostic.h"
#include "Tf/stringUtils.h"
#include "Arch/pragmas.h"

ARCH_PRAGMA_PUSH
ARCH_PRAGMA_MACRO_REDEFINITION
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/imageio.h>
ARCH_PRAGMA_POP

#include <sstream>

PXR_NAMESPACE_OPEN_SCOPE

OIIO_NAMESPACE_USING

/* static */
bool
HioImageUtils::GenerateMipmaps(
    const std::string& inputPath,
    const std::string& outputPath)
{
    // Read the source image
    ImageBuf srcImage(inputPath);
    if (!srcImage.read()) {
        TF_RUNTIME_ERROR("Failed to read image: %s - %s",
                         inputPath.c_str(), srcImage.geterror().c_str());
        return false;
    }

    // Configure make_texture settings
    ImageSpec config;
    // Use lanczos3 for high-quality downsampling (matches OIIO's recommendation)
    config["maketx:filtername"] = "lanczos3";
    // Enable mipmap generation
    config["maketx:mipmap"] = 1;
    // Preserve original data format
    config["maketx:opaquedetect"] = 0;
    // Don't modify color space
    config["maketx:colorconvert"] = 0;

    // Capture any output messages
    std::stringstream outstream;

    // Determine the texture mode based on aspect ratio
    // Environment maps (lat-long) typically have 2:1 aspect ratio
    const ImageSpec& srcSpec = srcImage.spec();
    bool isEnvMap = (srcSpec.width == srcSpec.height * 2);

    ImageBufAlgo::MakeTextureMode mode = isEnvMap ?
        ImageBufAlgo::MakeTxEnvLatl : ImageBufAlgo::MakeTxTexture;

    // Use OIIO's make_texture for robust mipmap generation
    bool success = ImageBufAlgo::make_texture(mode, srcImage, outputPath,
                                               config, &outstream);

    if (!success) {
        std::string errMsg = outstream.str();
        if (errMsg.empty()) {
            errMsg = OIIO::geterror();
        }
        TF_RUNTIME_ERROR("Failed to generate mipmapped texture: %s - %s",
                         outputPath.c_str(), errMsg.c_str());
        return false;
    }

    TF_STATUS("Generated mipmapped %s: %s",
              isEnvMap ? "environment map" : "texture", outputPath.c_str());
    return true;
}

/* static */
bool
HioImageUtils::GenerateMipmapsInPlace(const std::string& imagePath)
{
    // Generate to a temporary file first
    std::string tempPath = imagePath + ".mipmap_tmp";

    if (!GenerateMipmaps(imagePath, tempPath)) {
        return false;
    }

    // Replace original with mipmapped version
    if (std::rename(tempPath.c_str(), imagePath.c_str()) != 0) {
        TF_RUNTIME_ERROR("Failed to replace original file: %s", imagePath.c_str());
        std::remove(tempPath.c_str());
        return false;
    }

    return true;
}

PXR_NAMESPACE_CLOSE_SCOPE
