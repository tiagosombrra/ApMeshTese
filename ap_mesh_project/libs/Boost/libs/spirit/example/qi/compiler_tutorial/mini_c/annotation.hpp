/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINIC_ANNOTATION_HPP)
#define BOOST_SPIRIT_MINIC_ANNOTATION_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <map>

#include "ast.hpp"

namespace client {
///////////////////////////////////////////////////////////////////////////////
//  The annotation handler links the AST to a map of iterator positions
//  for the purpose of subsequent semantic error handling when the
//  program is being compiled.
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct annotation {
  template <typename, typename>
  struct result {
    typedef void type;
  };

  std::vector<Iterator>& iters;
  annotation(std::vector<Iterator>& iters) : iters(iters) {}

  struct set_id {
    typedef void result_type;

    int id;
    set_id(int id) : id(id) {}

    void operator()(ast::function_call& x) const { x.function_name.id = id; }

    void operator()(ast::identifier& x) const { x.id = id; }

    template <typename T>
    void operator()(T& x) const {
      // no-op
    }
  };

  void operator()(ast::operand& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    boost::apply_visitor(set_id(id), ast);
  }

  void operator()(ast::variable_declaration& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.lhs.id = id;
  }

  void operator()(ast::assignment& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.lhs.id = id;
  }

  void operator()(ast::return_statement& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.id = id;
  }

  void operator()(ast::identifier& ast, Iterator pos) const {
    int id = iters.size();
    iters.push_back(pos);
    ast.id = id;
  }
};
}  // namespace client

#endif
