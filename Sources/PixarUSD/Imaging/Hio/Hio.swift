/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import Hio

/**
 * # ``Hio``
 *
 * **Hydra Resource I/O**
 *
 * ## Overview
 *
 * **Hio** provides utility classes for loading
 * resources used by Hydra. */
public enum Hio {
    /// Type alias for the C++ ImageUtils class
    public typealias ImageUtils = Pixar.HioImageUtils
}

// MARK: - Image Utils Extensions

public extension Hio.ImageUtils {
    /// Generate mipmaps for an image file.
    ///
    /// This function reads the source image, generates a full mipmap chain,
    /// and writes the result to the output path. The output format is
    /// determined by the file extension.
    ///
    /// - Parameters:
    ///   - inputPath: Path to the source image file (EXR, HDR, etc.)
    ///   - outputPath: Path for the output file with mipmaps
    /// - Returns: true on success, false on failure
    ///
    /// Example:
    /// ```swift
    /// let success = Hio.ImageUtils.generateMipmaps(
    ///     from: "/path/to/hdri.exr",
    ///     to: "/path/to/hdri_mipmapped.exr"
    /// )
    /// ```
    @discardableResult
    static func generateMipmaps(from inputPath: String, to outputPath: String) -> Bool {
        Pixar.HioImageUtils.GenerateMipmaps(std.string(inputPath), std.string(outputPath))
    }

    /// Generate mipmaps for an image file in-place.
    ///
    /// Convenience function that overwrites the source file with a
    /// mipmapped version.
    ///
    /// - Parameter imagePath: Path to the image file to process
    /// - Returns: true on success, false on failure
    ///
    /// Example:
    /// ```swift
    /// let success = Hio.ImageUtils.generateMipmapsInPlace(at: "/path/to/hdri.exr")
    /// ```
    @discardableResult
    static func generateMipmapsInPlace(at imagePath: String) -> Bool {
        Pixar.HioImageUtils.GenerateMipmapsInPlace(std.string(imagePath))
    }
}
