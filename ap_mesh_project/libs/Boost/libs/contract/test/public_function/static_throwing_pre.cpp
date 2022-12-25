
// Copyright (C) 2008-2018 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0 (see accompanying
// file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt).
// See: http://www.boost.org/doc/libs/release/libs/contract/doc/html/index.html

// Test throw from public static member function .pre().

#include <boost/contract/check.hpp>
#include <boost/contract/public_function.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <sstream>

#include "../detail/oteststream.hpp"

boost::contract::test::detail::oteststream out;

struct a_err {};  // Global decl so visible in MSVC10 lambdas.

struct a {
  static void static_invariant() { out << "a::static_inv" << std::endl; }
  void invariant() const { out << "a::inv" << std::endl; }

  static void f() {
    boost::contract::check c =
        boost::contract::public_function<a>()
            .precondition([] {
              out << "a::f::pre" << std::endl;
              throw a_err();  // Test this throws.
            })
            .old([] { out << "a::f::old" << std::endl; })
            .postcondition([] { out << "a::f::post" << std::endl; })
            .except([] { out << "a::f::except" << std::endl; });
    out << "a::f::body" << std::endl;
  }
};

int main() {
  std::ostringstream ok;

  boost::contract::set_precondition_failure(
      [](boost::contract::from) { throw; });

  try {
    out.str("");
    a::f();
#ifndef BOOST_CONTRACT_NO_PRECONDITIONS
    BOOST_TEST(false);
  } catch (a_err const&) {
#endif
    ok.str("");
    ok
#ifndef BOOST_CONTRACT_NO_ENTRY_INVARIANTS
        << "a::static_inv" << std::endl
#endif
#ifndef BOOST_CONTRACT_NO_PRECONDITIONS
        << "a::f::pre" << std::endl  // Test this threw.
#else
#ifndef BOOST_CONTRACT_NO_OLDS
        << "a::f::old" << std::endl
#endif
        << "a::f::body" << std::endl
#ifndef BOOST_CONTRACT_NO_EXIT_INVARIANTS
        << "a::static_inv" << std::endl
#endif
#ifndef BOOST_CONTRACT_NO_POSTCONDITIONS
        << "a::f::post" << std::endl
#endif
#endif
        ;
    BOOST_TEST(out.eq(ok.str()));
  } catch (...) {
    BOOST_TEST(false);
  }

  return boost::report_errors();
}