#include "Numerical/InterpretedFunction.h"

#if USE_NUMERICAL
#if USE_EXPRTK

using namespace Data;
using namespace Numerical;

Numerical::InterpretedFunction::InterpretedFunction() :
    Numerical::Function::Function()
{

}

Numerical::InterpretedFunction::~InterpretedFunction()
{

}

void Numerical::InterpretedFunction::setFunction(const std::string &function)
{
    this->function = function;
}

const std::string &Numerical::InterpretedFunction::getFunction() const
{
    return this->function;
}

Real Numerical::InterpretedFunction::f(Real x)
{
    SymbolTable symbol_table;
    symbol_table.add_variable("x", x);
    symbol_table.add_constants();

    Expression expression;
    expression.register_symbol_table(symbol_table);

    Parser parser;
    parser.compile(this->function, expression);

    return expression.value();
}

Real Numerical::InterpretedFunction::f(const std::string &value)
{
    SymbolTable symbol_table;
    symbol_table.add_constants();

    Expression expression;
    expression.register_symbol_table(symbol_table);

    Parser parser;
    parser.compile(value, expression);

    return this->f(expression.value());
}

#endif //#if USE_EXPRTK
#endif //#if USE_NUMERICAL
