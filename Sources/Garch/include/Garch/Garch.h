#ifndef __PXR_IMAGING_GARCH_H__
#define __PXR_IMAGING_GARCH_H__

// Garch.
#include <Garch/api.h>
#include <Garch/gl.h>
#include <Garch/glApi.h>
#include <Garch/glDebugWindow.h>
#include <Garch/glPlatformContext.h>
#include <Garch/glPlatformDebugContext.h>
#include <Garch/khrplatform.h>
#if defined(__APPLE__)
#  include <Garch/GarchDarwin/glPlatformContextDarwin.h>
#  include <Garch/GarchDarwin/glPlatformDebugWindowDarwin.h>
#elif defined(__linux__)
#  include <Garch/GarchGLX/glPlatformContextGLX.h>
#  include <Garch/GarchGLX/glPlatformDebugWindowGLX.h>
#elif defined(_WIN32)
#  include <Garch/GarchWindows/glPlatformContextWindows.h>
#  include <Garch/GarchWindows/glPlatformDebugWindowWindows.h>
#endif /* defined(_WIN32) */

#include <Garch/glPlatformDebugWindowDarwin.h>
#include <Garch/glPlatformContextDarwin.h>
#include <Garch/glPlatformContextWindows.h>
#include <Garch/glPlatformDebugWindowWindows.h>
#include <Garch/glPlatformDebugWindowGLX.h>
#include <Garch/glPlatformContextGLX.h>
#endif  // __PXR_IMAGING_GARCH_H__
