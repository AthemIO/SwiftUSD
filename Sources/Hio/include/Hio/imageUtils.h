//
// Copyright 2025 Afloat Technologies
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_IMAGING_HIO_IMAGE_UTILS_H
#define PXR_IMAGING_HIO_IMAGE_UTILS_H

#include "pxr/pxrns.h"
#include "Hio/api.h"

#include <string>

PXR_NAMESPACE_OPEN_SCOPE

/// \class HioImageUtils
///
/// Utility functions for image processing using OpenImageIO.
///
class HioImageUtils
{
public:
    /// Generate mipmaps for an image file and write to output.
    ///
    /// This function reads the source image, generates a full mipmap chain,
    /// and writes the result to the output path. The output format is
    /// determined by the file extension.
    ///
    /// \param inputPath  Path to the source image file (EXR, HDR, etc.)
    /// \param outputPath Path for the output file with mipmaps
    /// \return true on success, false on failure
    HIO_API
    static bool GenerateMipmaps(
        const std::string& inputPath,
        const std::string& outputPath);

    /// Generate mipmaps for an image file in-place.
    ///
    /// Convenience function that overwrites the source file with a
    /// mipmapped version.
    ///
    /// \param imagePath Path to the image file to process
    /// \return true on success, false on failure
    HIO_API
    static bool GenerateMipmapsInPlace(const std::string& imagePath);
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_IMAGING_HIO_IMAGE_UTILS_H
