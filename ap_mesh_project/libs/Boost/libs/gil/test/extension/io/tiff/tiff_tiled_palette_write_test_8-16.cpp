//
// Copyright 2013 Christian Henning
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/core/lightweight_test.hpp>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/tiff.hpp>

#include "tiff_tiled_write_macros.hpp"

namespace gil = boost::gil;

#ifdef BOOST_GIL_IO_USE_TIFF_GRAPHICSMAGICK_TEST_SUITE_IMAGES

// CHH - not supported palette
// BOOST_PP_REPEAT_FROM_TO(9, 17, GENERATE_WRITE_TILE_BIT_ALIGNED_PALETTE,
// palette )

int main() {
  // TODO: Make sure generated test cases are executed. See
  // tiff_subimage_test.cpp. ~mloskot

  return boost::report_errors();
}

#else
int main() {}
#endif  // BOOST_GIL_IO_USE_TIFF_GRAPHICSMAGICK_TEST_SUITE_IMAGES
