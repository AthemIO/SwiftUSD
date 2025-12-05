/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Kind

/**
 * # ``Kind``
 *
 * **Extensible Categorization**
 *
 * ## Overview
 *
 * The **Kind** library provides a runtime-extensible taxonomy known as **"Kinds"**.
 * **Kinds** are just **TfToken** symbols, but the ``Kind/KindRegistry`` allows for organizing
 * kinds into taxonomies of related/refined concepts, and the ``Kind/KindRegistry.GetBaseKind()``
 * and ``Kind/KindRegistry.is(a:)`` queries enable reasoning about the hierarchy and classifying
 * objects by kind. */
public enum Kind
{}
