/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
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

  public extension Pixar.HgiGL
  {
    func getValue(_ ptr: Pixar.HgiGLPtr) -> VtValue
    {
      GetValue(ptr)
    }
  }

  public extension Pixar.HgiGLPtr
  {
    var value: VtValue
    {
      pointee.getValue(self)
    }
  }
#endif /* canImport(HgiGL) */
