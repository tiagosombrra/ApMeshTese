// Copyright (C) 2017 Tom Hughes
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <boost/thread/mutex.hpp>

// class mutex;

#include <boost/detail/lightweight_test.hpp>
#include <boost/thread/mutex.hpp>

void pass() {
  boost::mutex m0;
  m0.lock();
  m0.unlock();
}
