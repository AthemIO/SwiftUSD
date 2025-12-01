/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

#if canImport(HgiGL)
  import HgiGL

  public enum HgiGL
  {
    public static func createHgi() -> Pixar.HgiGLPtr
    {
      Pixar.HgiGL.CreateHgi()
    }
  }

  // TODO: HgiGLDevice and GetPrimaryDevice return raw pointers which have
  // Swift C++ interop issues. These APIs need further work to expose safely.
  // public extension Pixar.HgiGL
  // {
  //   var device: Pixar.HgiGLDevice
  //   {
  //     GetPrimaryDevice()
  //   }
  // }
#endif /* canImport(HgiGL) */
