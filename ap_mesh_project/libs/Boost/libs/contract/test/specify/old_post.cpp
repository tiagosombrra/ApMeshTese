
// Copyright (C) 2008-2018 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0 (see accompanying
// file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).
// See: http://www.boost.org/doc/libs/release/libs/contract/doc/html/index.html

// Test specifying old and post, no pre or except (same if not free func).

#include <boost/contract/check.hpp>
#include <boost/contract/function.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <sstream>

#include "../detail/oteststream.hpp"

boost::contract::test::detail::oteststream out;

void f() {
  boost::contract::check c =
      boost::contract::function()
          .old([] { out << "f::old" << std::endl; })
          .postcondition([] { out << "f::post" << std::endl; });
  out << "f::body" << std::endl;
}

int main() {
  std::ostringstream ok;

  out.str("");
  f();
  ok.str("");
  ok
#ifndef BOOST_CONTRACT_NO_OLDS
      << "f::old" << std::endl
#endif
      << "f::body" << std::endl
#ifndef BOOST_CONTRACT_NO_POSTCONDITIONS
      << "f::post" << std::endl
#endif
      ;
  BOOST_TEST(out.eq(ok.str()));

  return boost::report_errors();
}
