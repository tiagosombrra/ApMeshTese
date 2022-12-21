/*==============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_UTREE_EXAMPLE_UTF8_PARSER_HPP)
#define BOOST_SPIRIT_UTREE_EXAMPLE_UTF8_PARSER_HPP

#include <boost/cstdint.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <string>

namespace utf8 {

namespace qi = boost::spirit::qi;
namespace px = boost::phoenix;
namespace standard = boost::spirit::standard;

using boost::spirit::binary_string_type;
using boost::spirit::utf8_string_type;
using boost::spirit::utf8_symbol_type;
using boost::spirit::utree;

typedef boost::uint32_t uchar;

struct push_string {
  template <typename, typename>
  struct result {
    typedef void type;
  };

  void operator()(std::string& s, uchar code_point) const {
    typedef std::back_insert_iterator<std::string> insert_iter;
    insert_iter out_iter(s);
    boost::utf8_output_iterator<insert_iter> s_iter(out_iter);
    *s_iter++ = code_point;
  }
};

struct push_escaped_string {
  template <typename, typename>
  struct result {
    typedef void type;
  };

  void operator()(std::string& s, uchar c) const {
    switch (c) {
      case 'b':
        s += '\b';
        break;
      case 't':
        s += '\t';
        break;
      case 'n':
        s += '\n';
        break;
      case 'f':
        s += '\f';
        break;
      case 'r':
        s += '\r';
        break;
      case '"':
        s += '"';
        break;
      case '\\':
        s += '\\';
        break;
    }
  }
};

template <typename Iterator>
struct parser : qi::grammar<Iterator, std::string()> {
  qi::rule<Iterator, void(std::string&)> escaped;

  qi::rule<Iterator, std::string()> start;

  px::function<push_string> push_str;

  px::function<push_escaped_string> push_esc;

  parser() : parser::base_type(start) {
    using qi::_1;
    using qi::_r1;
    using qi::_val;
    using qi::uint_parser;
    using standard::char_;

    uint_parser<uchar, 16, 4, 4> hex4;
    uint_parser<uchar, 16, 8, 8> hex8;

    escaped = '\\' > (('u' > hex4)[push_str(_r1, _1)] |
                      ('U' > hex8)[push_str(_r1, _1)] |
                      char_("btnfr\\\"'")[push_esc(_r1, _1)]);

    start = '"' > *(escaped(_val) | (~char_('"'))[_val += _1]) > '"';

    escaped.name("escaped_string");
    start.name("string");
  }
};

}  // namespace utf8

#endif  // BOOST_SPIRIT_UTREE_EXAMPLE_UTF8_PARSER_HPP
