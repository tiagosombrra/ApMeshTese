
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "bind_processor.hpp"

extern "C" {
#include <sys/processor.h>
#include <sys/thread.h>
}

#include <boost/config/abi_prefix.hpp>
#include <stdexcept>

void bind_to_processor(unsigned int n) {
  if (::bindprocessor(BINDTHREAD, ::thread_yield(), static_cast<cpu_t>(n)) ==
      -1)
    throw std::runtime_error("::bindprocessor() failed");
}

#include <boost/config/abi_suffix.hpp>
