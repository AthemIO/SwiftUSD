//
// Copyright 2019 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_SDR_GLSLFX_PARSER_PLUGIN_H
#define PXR_USD_SDR_GLSLFX_PARSER_PLUGIN_H

#include "pxr/pxrns.h"
#include "Sdr/parserPlugin.h"

PXR_NAMESPACE_OPEN_SCOPE

/// \class SdrGlslfxParserPlugin
///
/// Parses shader definitions from glslfx files.
///
class SdrGlslfxParserPlugin: public SdrParserPlugin
{
public:
    SdrGlslfxParserPlugin() = default;

    ~SdrGlslfxParserPlugin() override = default;

    SdrShaderNodeUniquePtr
    ParseShaderNode(const SdrShaderNodeDiscoveryResult &discoveryResult) override;

    const SdrTokenVec &GetDiscoveryTypes() const override;

    const TfToken &GetSourceType() const override;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_SDR_GLSLFX_PARSER_PLUGIN_H
