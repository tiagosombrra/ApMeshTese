/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A Calculator example demonstrating generation of AST which gets dumped into
//  a human readable format afterwards.
//
//  [ JDG April 28, 2008 ]
//  [ HK April 28, 2008 ]
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(SPIRIT_EXAMPLE_CALC2_AST_APR_30_2008_1011AM)
#define SPIRIT_EXAMPLE_CALC2_AST_APR_30_2008_1011AM

#include <boost/spirit/include/karma_domain.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/support_attributes_fwd.hpp>
#include <boost/variant.hpp>

///////////////////////////////////////////////////////////////////////////////
//  Our AST
///////////////////////////////////////////////////////////////////////////////
struct binary_op;
struct unary_op;
struct nil {};

struct expression_ast {
  typedef boost::variant<nil  // can't happen!
                         ,
                         int, boost::recursive_wrapper<binary_op>,
                         boost::recursive_wrapper<unary_op> >
      type;

  // expose variant types
  typedef type::types types;

  // expose variant functionality
  int which() const { return expr.which(); }

  // constructors
  expression_ast() : expr(nil()) {}

  expression_ast(unary_op const& expr) : expr(expr) {}

  expression_ast(binary_op const& expr) : expr(expr) {}

  expression_ast(unsigned int expr) : expr(expr) {}

  expression_ast(type const& expr) : expr(expr) {}

  expression_ast& operator+=(expression_ast const& rhs);
  expression_ast& operator-=(expression_ast const& rhs);
  expression_ast& operator*=(expression_ast const& rhs);
  expression_ast& operator/=(expression_ast const& rhs);

  type expr;
};

// expose variant functionality
namespace boost {
// this function has to live in namespace boost for ADL to correctly find it
template <typename T>
inline T get(expression_ast const& expr) {
  return boost::get<T>(expr.expr);
}

// the specialization below tells Spirit to handle expression_ast as if it
// where a 'real' variant
namespace spirit {
namespace traits {
// the specialization below tells Spirit to handle expression_ast as
// if it where a 'real' variant (if used with Spirit.Karma)
template <>
struct not_is_variant<expression_ast, karma::domain> : mpl::false_ {};

// the specialization of variant_which allows to generically extract
// the current type stored in the given variant like type
template <>
struct variant_which<expression_ast> {
  static int call(expression_ast const& v) { return v.which(); }
};
}  // namespace traits
}  // namespace spirit
}  // namespace boost

enum byte_code {
  op_neg = 1,  //  negate the top stack entry
  op_pos,      //  essentially a no-op (unary plus)
  op_add,      //  add top two stack entries
  op_sub,      //  subtract top two stack entries
  op_mul,      //  multiply top two stack entries
  op_div,      //  divide top two stack entries
  op_int,      //  push constant integer into the stack
};

///////////////////////////////////////////////////////////////////////////////
struct binary_op {
  binary_op() {}

  binary_op(int op, expression_ast const& left, expression_ast const& right)
      : op(op), left(left), right(right) {}

  int op;
  expression_ast left;
  expression_ast right;
};

struct unary_op {
  unary_op(int op, expression_ast const& right) : op(op), right(right) {}

  int op;
  expression_ast right;
};

inline expression_ast& expression_ast::operator+=(expression_ast const& rhs) {
  expr = binary_op(op_add, expr, rhs);
  return *this;
}

inline expression_ast& expression_ast::operator-=(expression_ast const& rhs) {
  expr = binary_op(op_sub, expr, rhs);
  return *this;
}

inline expression_ast& expression_ast::operator*=(expression_ast const& rhs) {
  expr = binary_op(op_mul, expr, rhs);
  return *this;
}

inline expression_ast& expression_ast::operator/=(expression_ast const& rhs) {
  expr = binary_op(op_div, expr, rhs);
  return *this;
}

// We should be using expression_ast::operator-. There's a bug
// in phoenix type deduction mechanism that prevents us from
// doing so. Phoenix will be switching to BOOST_TYPEOF. In the
// meantime, we will use a phoenix::function below:
template <char Op>
struct unary_expr {
  template <typename T>
  struct result {
    typedef T type;
  };

  expression_ast operator()(expression_ast const& expr) const {
    return unary_op(Op, expr);
  }
};

boost::phoenix::function<unary_expr<op_pos> > pos;
boost::phoenix::function<unary_expr<op_neg> > neg;

#endif
