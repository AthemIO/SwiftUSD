// PixarUSD.h - Umbrella header for PixarUSD Swift/C++ module
// This is a clean facade that wraps real Pixar USD without exposing USD headers to Swift.

#pragma once

// Value types (no reference counting needed)
#include "Types.hpp"

// Core USD wrappers (reference counted)
#include "Stage.hpp"
#include "Prim.hpp"

// Schema convenience wrappers
#include "Mesh.hpp"
#include "Xform.hpp"

// Rendering
#include "HydraEngine.hpp"
