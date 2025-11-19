// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef PXR_PEGTL_HPP
#define PXR_PEGTL_HPP

#include "config.hpp"
#include "demangle.hpp"
#include "parse.hpp"
#include "version.hpp"

#include "ascii.hpp"
#include "rules.hpp"
#include "utf8.hpp"

#include "argv_input.hpp"
#include "buffer_input.hpp"
#include "cstream_input.hpp"
#include "istream_input.hpp"
#include "memory_input.hpp"
#include "read_input.hpp"
#include "string_input.hpp"

// This has to be included *after* the above inputs,
// otherwise the amalgamated header will not work!
#include "file_input.hpp"

#include "change_action.hpp"
#include "change_action_and_state.hpp"
#include "change_action_and_states.hpp"
#include "change_control.hpp"
#include "change_state.hpp"
#include "change_states.hpp"

#include "disable_action.hpp"
#include "enable_action.hpp"

#include "discard_input.hpp"
#include "discard_input_on_failure.hpp"
#include "discard_input_on_success.hpp"

#include "visit.hpp"

#if defined( __cpp_exceptions )
#include "must_if.hpp"
#endif

#endif
