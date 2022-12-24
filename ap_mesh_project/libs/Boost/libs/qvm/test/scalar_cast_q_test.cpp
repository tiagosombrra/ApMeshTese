// Copyright (c) 2008-2016 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef BOOST_QVM_TEST_SINGLE_HEADER
#include BOOST_QVM_TEST_SINGLE_HEADER
#else
#include <boost/qvm/quat_operations.hpp>
#endif

#include "gold.hpp"
#include "test_qvm_quaternion.hpp"

namespace {
void test() {
  using namespace boost::qvm::sfinae;
  test_qvm::quaternion<Q1, double> x(42, 1);
  test_qvm::quaternion<Q1, float> y;
  assign(y, scalar_cast<float>(x));
  for (int i = 0; i != 4; ++i) y.b[i] = static_cast<float>(x.a[i]);
  BOOST_QVM_TEST_EQ(y.a, y.b);
}
}  // namespace

int main() {
  test();
  return boost::report_errors();
}
