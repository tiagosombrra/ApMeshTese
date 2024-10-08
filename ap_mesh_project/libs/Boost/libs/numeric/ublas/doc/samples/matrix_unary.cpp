//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

int main() {
  using namespace boost::numeric::ublas;
  matrix<std::complex<double> > m(3, 3);
  for (unsigned i = 0; i < m.size1(); ++i)
    for (unsigned j = 0; j < m.size2(); ++j)
      m(i, j) = std::complex<double>(3 * i + j, 3 * i + j);

  std::cout << -m << std::endl;
  std::cout << conj(m) << std::endl;
  std::cout << real(m) << std::endl;
  std::cout << imag(m) << std::endl;
  std::cout << trans(m) << std::endl;
  std::cout << herm(m) << std::endl;
}
