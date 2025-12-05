/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

#if canImport(Metal)
  import HgiMetal
  import Metal

  public enum HgiMetal
  {
    public static func createHgi() -> Pixar.HgiMetalPtr
    {
      Pixar.HgiMetal.CreateHgi()
    }
  }

  public extension Pixar.HgiMetal
  {
    var apiVersion: Int
    {
      Int(GetAPIVersion())
    }

    private borrowing func GetPrimaryDeviceCopy() -> MTLDevice
    {
      GetPrimaryDevice()
    }

    var device: MTLDevice
    {
      GetPrimaryDeviceCopy()
    }

    func getValue(_ ptr: Pixar.HgiMetalPtr) -> VtValue
    {
      GetValue(ptr)
    }
  }

  public extension Pixar.HgiMetalPtr
  {
    var apiVersion: Int
    {
      pointee.apiVersion
    }

    var device: MTLDevice
    {
      pointee.device
    }

    var value: VtValue
    {
      pointee.getValue(self)
    }
  }
#endif /* canImport(Metal) */
