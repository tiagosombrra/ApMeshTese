#include "Numerical/DerivativePolynomial.h"

#if USE_NUMERICAL

using namespace Data;
using namespace Numerical;

Numerical::DerivativePolynomial::DerivativePolynomial(const IPolynomial *polynomial) :
    Numerical::IPolynomial::IPolynomial()
{
    this->setPolynomial(polynomial);
}

Numerical::DerivativePolynomial::~DerivativePolynomial()
{

}

void Numerical::DerivativePolynomial::setPolynomial(const IPolynomial *polynomial)
{
    this->polynomial = polynomial;
}

const IPolynomial *Numerical::DerivativePolynomial::getPolynomial() const
{
    return this->polynomial;
}

UInt Numerical::DerivativePolynomial::degree() const
{
    return (this->polynomial->degree() == 0) ? 0 : this->polynomial->degree() - 1;
}

Real Numerical::DerivativePolynomial::getCoefficient(UInt i) const
{
    UInt pdeg = this->polynomial->degree();

    return ((i >= pdeg) || (pdeg == 0)) ? 0.0 :
        this->polynomial->getCoefficient(i + 1)*static_cast<Real>(i + 1);
}

#endif //#if USE_NUMERICAL
