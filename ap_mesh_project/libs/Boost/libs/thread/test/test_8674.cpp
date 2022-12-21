// Copyright (C) 2013 Vicente Botet
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#define USE_STD 0
#define USE_BOOST 1

#define USED_THREAD_API USE_BOOST
//#define USED_THREAD_API USE_STD

#if USED_THREAD_API == USE_BOOST

#define BOOST_THREAD_VERSION 4
#include <boost/thread/future.hpp>

using boost::async;
using boost::future;

#endif
#if USED_THREAD_API == USE_STD
#include <future>
using std::async;
using std::future;
#endif

future<void> do_something() {
  auto result = async([] { std::cout << "A\n"; });
  std::cout << "B\n";
  return result;  // error here
}

int main() {
  do_something().wait();
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
