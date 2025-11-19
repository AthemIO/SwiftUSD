//
// Copyright 2023 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_SDF_PATH_PARSER_H
#define PXR_USD_SDF_PATH_PARSER_H

#include "pxr/pxrns.h"
#include "Tf/stringUtils.h"
#include "Tf/unicodeUtils.h"
#include "Sdf/path.h"

#include "Pegtl/pegtl.hpp"

PXR_NAMESPACE_OPEN_SCOPE

bool
Sdf_ParsePath(std::string const &pathStr, SdfPath *path, std::string *errMsg);

namespace Sdf_PathParser {

namespace PEGTL_NS = PXR_PEGTL_NAMESPACE;

////////////////////////////////////////////////////////////////////////
// Helper rules for parsing UTF8 content
struct XidStart
{
    template <typename ParseInput>
    static bool match(ParseInput& in)
    {
        if (!in.empty())
        {
            // peek at the next character in the input
            // if the size is not 0, it was a valid code point
            auto utf8_char = PEGTL_NS::internal::peek_utf8::peek(in);
            if (utf8_char.size != 0)
            {
                // valid utf8_char, data has the code point
                if (TfIsUtf8CodePointXidStart(static_cast<uint32_t>(utf8_char.data)))
                {
                    // it has the property we want, consume the input
                    in.bump(utf8_char.size);
                    return true;
                }
            }
        }

        return false;
    }
};

struct XidContinue
{
    template <typename ParseInput>
    static bool match(ParseInput& in)
    {
        if (!in.empty())
        {
            // peek at the next character in the input
            // if the size is not 0, it was a valid code point
            auto utf8_char = PEGTL_NS::internal::peek_utf8::peek(in);
            if (utf8_char.size != 0)
            {
                // valid utf8_char, data has the code point
                if (TfIsUtf8CodePointXidContinue(static_cast<uint32_t>(utf8_char.data)))
                {
                    // it has the property we want, consume the input
                    in.bump(utf8_char.size); 
                    return true;
                }
            }
        }

        return false;
    }
};

////////////////////////////////////////////////////////////////////////
// SdfPath grammar:

struct Slash : PEGTL_NS::one<'/'> {};
struct Dot : PEGTL_NS::one<'.'> {};
struct DotDot : PEGTL_NS::two<'.'> {};

struct AbsoluteRoot : Slash {};
struct ReflexiveRelative : Dot {};

struct DotDots : PEGTL_NS::internal::list<DotDot, Slash> {};

// valid identifiers start with an '_' character or anything in the XidStart
// character class, then continue with zero or more characters in the
// XidContinue character class
struct Utf8IdentifierStart : PEGTL_NS::internal::sor<
    PEGTL_NS::one<'_'>,
    XidStart> {};
struct Utf8Identifier : PEGTL_NS::internal::seq<
    Utf8IdentifierStart,
    PEGTL_NS::internal::star<XidContinue>> {};

struct PrimName : Utf8Identifier {};

// XXX This replicates old behavior where '-' chars are allowed in variant set
// names in SdfPaths, but variant sets in layers cannot have '-' in their names.
// For now we preserve the behavior. Internal bug USD-8321 tracks removing
// support for '-' characters in variant set names in SdfPath.
struct VariantSetName :
    PEGTL_NS::internal::seq<Utf8IdentifierStart,
    PEGTL_NS::internal::star<PEGTL_NS::internal::sor<
    XidContinue, PEGTL_NS::one<'-'>>>> {};

struct VariantName :
    PEGTL_NS::internal::seq<PEGTL_NS::internal::opt<
    PEGTL_NS::one<'.'>>, PEGTL_NS::internal::star<
    PEGTL_NS::internal::sor<XidContinue,
    PEGTL_NS::one<'|', '-'>>>> {};

struct VarSelOpen : PEGTL_NS::internal::pad<PEGTL_NS::one<'{'>, PEGTL_NS::blank> {};
struct VarSelClose : PEGTL_NS::internal::pad<PEGTL_NS::one<'}'>, PEGTL_NS::blank> {};

struct VariantSelection : PEGTL_NS::internal::if_must< false,
    VarSelOpen,
    VariantSetName, PEGTL_NS::internal::pad<PEGTL_NS::one<'='>, 
    PEGTL_NS::blank>, PEGTL_NS::internal::opt<VariantName>,
    VarSelClose>
{};

struct VariantSelections : PEGTL_NS::internal::plus<VariantSelection> {};

template <class Rule, class Sep>
struct LookaheadList : PEGTL_NS::internal::seq<Rule, PEGTL_NS::internal::star<
    PEGTL_NS::internal::at<Sep, Rule>, Sep, Rule>> {};

struct PrimElts : PEGTL_NS::internal::seq<
    LookaheadList<PrimName, PEGTL_NS::internal::sor<Slash, VariantSelections>>,
    PEGTL_NS::internal::opt<VariantSelections>> {};

struct PropertyName : PEGTL_NS::internal::list<Utf8Identifier, PEGTL_NS::one<':'>> {};

struct MapperPath;
struct TargetPath;

struct TargetPathOpen : PEGTL_NS::one<'['> {};
struct TargetPathClose : PEGTL_NS::one<']'> {};

template <class TargPath>
struct BracketPath : PEGTL_NS::internal::if_must< false,TargetPathOpen, TargPath, TargetPathClose> {};

struct RelationalAttributeName : PropertyName {};

struct MapperKW : PXR_PEGTL_KEYWORD("mapper") {};

struct MapperArg : PEGTL_NS::identifier {};

struct MapperPathSeq : PEGTL_NS::internal::if_must< false,
    PEGTL_NS::internal::seq<Dot, MapperKW>, BracketPath<MapperPath>,
    PEGTL_NS::internal::opt<Dot, MapperArg>> {};

struct Expression : PXR_PEGTL_KEYWORD("expression") {};

struct RelAttrSeq : PEGTL_NS::internal::if_must< false,
    PEGTL_NS::one<'.'>, RelationalAttributeName,
    PEGTL_NS::internal::opt<PEGTL_NS::internal::sor<BracketPath<TargetPath>,
            MapperPathSeq,
            PEGTL_NS::internal::if_must< false,Dot, Expression>>>> {};

struct TargetPathSeq : PEGTL_NS::internal::seq<BracketPath<TargetPath>, 
    PEGTL_NS::internal::opt<RelAttrSeq>> {};

struct PropElts :
    PEGTL_NS::internal::seq<PEGTL_NS::one<'.'>, PropertyName,
        PEGTL_NS::internal::opt<PEGTL_NS::internal::sor<TargetPathSeq, MapperPathSeq,
        PEGTL_NS::internal::if_must< false,Dot, Expression>>>
    >
{};

struct PathElts : PEGTL_NS::internal::if_then_else<PrimElts, PEGTL_NS::internal::opt<PropElts>, PropElts> {};

struct PrimFirstPathElts : PEGTL_NS::internal::seq<PrimElts, PEGTL_NS::internal::opt<PropElts>> {};

struct Path : PEGTL_NS::internal::sor<
    PEGTL_NS::internal::seq<AbsoluteRoot, PEGTL_NS::internal::opt<PrimFirstPathElts>>,
    PEGTL_NS::internal::seq<DotDots, PEGTL_NS::internal::opt<PEGTL_NS::internal::seq<Slash, PathElts>>>,
    PathElts,
    ReflexiveRelative
    > {};

struct TargetPath : Path {};
struct MapperPath : Path {};

////////////////////////////////////////////////////////////////////////
// Actions.

struct PPContext {
    std::vector<SdfPath> paths { 1 };
    enum { IsTargetPath, IsMapperPath } targetType;
    std::string varSetName;
    std::string varName;
};

template <class Input>
TfToken GetToken(Input const &in) {
    constexpr int BufSz = 32;
    char buf[BufSz];
    size_t strSize = std::distance(in.begin(), in.end());
    TfToken tok;
    if (strSize < BufSz) {
        // copy & null-terminate.
        std::copy(in.begin(), in.end(), buf);
        buf[strSize] = '\0';
        tok = TfToken(buf);
    }
    else {
        // fall back to string path.
        tok = TfToken(in.string());
    }
    return tok;
}

template <class Rule>
struct Action : PEGTL_NS::nothing<Rule> {};

} // Sdf_PathParser

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_SDF_PATH_PARSER_H
