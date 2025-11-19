//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "pxr/pxrns.h"
#include "Vt/valueFromPython.h"
#include "Sdr/sdfTypeIndicator.h"

#include "pxr/external/boost/python.hpp"
#include <iostream>

PXR_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

void wrapSdfTypeIndicator()
{
    typedef SdrSdfTypeIndicator This;
    class_<This>("SdfTypeIndicator", no_init)
        .def("GetSdfType", &This::GetSdfType)
        .def("HasSdfType", &This::HasSdfType)
        .def("GetSdrType", &This::GetSdrType)
    ;
}
