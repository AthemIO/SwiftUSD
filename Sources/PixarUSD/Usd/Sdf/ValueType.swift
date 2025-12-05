/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Sdf

public typealias SdfValueTypeNameType = Pixar.SdfValueTypeNameType
public typealias SdfValueTypeName = Pixar.SdfValueTypeName

public extension Sdf
{
  typealias ValueTypeNameType = SdfValueTypeNameType
  typealias ValueTypeName = SdfValueTypeName
}

public extension Sdf
{
  static func getValueType(for type: SdfValueTypeNameType) -> Sdf.ValueTypeName
  {
    Pixar.SdfGetValueType(type)
  }
}
