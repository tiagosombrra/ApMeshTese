/*=============================================================================
    Copyright (c) 2002-2010 Hartmut Kaiser
    Copyright (c) 2002-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  This sample demonstrates a generator for a comma separated list of numbers.
//  No actions. It is based on the example qi/num_lists.cpp for reading in
//  some numbers to generate.
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace client {
///////////////////////////////////////////////////////////////////////////
//  Our number list parser, please see the example qi/numlist1.cpp for
//  more information
///////////////////////////////////////////////////////////////////////////
template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last, std::vector<double>& v) {
  using boost::spirit::ascii::space;
  using boost::spirit::qi::double_;
  using boost::spirit::qi::phrase_parse;

  bool r = phrase_parse(first, last, double_ % ',', space, v);
  if (first != last) return false;
  return r;
}

///////////////////////////////////////////////////////////////////////////
//  Our number list generator
///////////////////////////////////////////////////////////////////////////
//[tutorial_karma_numlist2
template <typename OutputIterator, typename Container>
bool generate_numbers(OutputIterator& sink, Container const& v) {
  using boost::spirit::ascii::space;
  using boost::spirit::karma::double_;
  using boost::spirit::karma::generate_delimited;

  bool r = generate_delimited(sink,           // destination: output iterator
                              double_ % ',',  // the generator
                              space,          // the delimiter-generator
                              v               // the data to output
  );
  return r;
}
//]
}  // namespace client

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int main() {
  std::cout << "/////////////////////////////////////////////////////////\n\n";
  std::cout << "\t\tA comma separated list generator for Spirit...\n\n";
  std::cout << "/////////////////////////////////////////////////////////\n\n";

  std::cout << "Give me a comma separated list of numbers.\n";
  std::cout << "Type [q or Q] to quit\n\n";

  std::string str;
  while (getline(std::cin, str)) {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;

    std::vector<double> v;  // here we put the data to generate
    if (client::parse_numbers(str.begin(), str.end(), v)) {
      // ok, we got some numbers, now print them back out
      std::cout << "-------------------------\n";

      std::string generated;
      std::back_insert_iterator<std::string> sink(generated);
      if (!client::generate_numbers(sink, v)) {
        std::cout << "-------------------------\n";
        std::cout << "Generating failed\n";
        std::cout << "-------------------------\n";
      } else {
        std::cout << "-------------------------\n";
        std::cout << "Generated: " << generated << "\n";
        std::cout << "-------------------------\n";
      }
    } else {
      std::cout << "-------------------------\n";
      std::cout << "Parsing failed\n";
      std::cout << "-------------------------\n";
    }
  }

  std::cout << "Bye... :-) \n\n";
  return 0;
}
