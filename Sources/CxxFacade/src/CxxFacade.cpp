// CxxFacade.cpp - Main module implementation file
//
// This file ensures the CxxFacade module has at least one compilation unit.
// The actual implementations are in subdirectories:
//   - base/   : Tf, Gf, Vt (foundation types)
//   - usd/    : Sdf, Usd (core USD)
//   - usdGeom/: UsdGeom (geometry schemas)
//   - usdShade/: UsdShade (shading)
//   - usdLux/ : UsdLux (lighting)
//   - usdSkel/: UsdSkel (skeletal animation)
//   - imaging/: Hydra (rendering)

#include "../include/CxxFacade.h"

namespace cxxfacade {

// Module version info
const char* GetVersion() {
    return "2.0.0";
}

const char* GetBuildInfo() {
#if CXXFACADE_PLATFORM_APPLE
    return "CxxFacade - macOS";
#elif CXXFACADE_PLATFORM_LINUX
    return "CxxFacade - Linux";
#elif CXXFACADE_PLATFORM_WINDOWS
    return "CxxFacade - Windows";
#else
    return "CxxFacade - Unknown Platform";
#endif
}

} // namespace cxxfacade
