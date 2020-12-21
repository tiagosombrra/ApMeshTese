#ifndef _DATA_NUMERICAL_EQUATION_ROOT_H_
#define _DATA_NUMERICAL_EQUATION_ROOT_H_

#include "../Definitions.h"
#include "EquationRootFunction.h"

namespace Data
{
namespace Numerical
{
	class EquationRoot
	{
	public:

        virtual double execute(EquationRootFunction *function) = 0;

	};
}
}

#endif //#ifndef _DATA_NUMERICAL_EQUATION_ROOT_H_
