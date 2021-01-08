#ifndef _NUMERICAL_INTERPRETED_FUNCTION_H_
#define _NUMERICAL_INTERPRETED_FUNCTION_H_

#include "Numerical/Definitions.h"

#if USE_NUMERICAL
#if USE_EXPRTK

#include "Numerical/Function.h"
#include "Numerical/exprtk.hpp"

namespace Numerical
{
    class InterpretedFunction : public Numerical::Function
    {
    public:

        InterpretedFunction();
        virtual ~InterpretedFunction();

        virtual void setFunction(const std::string &function);
        virtual const std::string &getFunction() const;

        using Numerical::Function::f;
        virtual Real f(Real x);
        virtual Real f(const std::string &value);

    protected:

        typedef exprtk::symbol_table<Real> SymbolTable;
        typedef exprtk::expression<Real> Expression;
        typedef exprtk::parser<Real> Parser;

    protected:

        std::string function;
    };
}

#endif //#if USE_EXPRTK
#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_INTERPRETED_FUNCTION_H_
