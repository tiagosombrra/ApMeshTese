#include "Tools/CovarianceMatrixFinder.h"

#if USE_NUMERICAL

#include "Data/Boundary.h"

Tools::CovarianceMatrixFinder::CovarianceMatrixFinder(const Tool *tool) :
	Performer::CovarianceMatrixFinder::CovarianceMatrixFinder(),
	Tools::ExternalTool::ExternalTool(tool)
{

}

Tools::CovarianceMatrixFinder::~CovarianceMatrixFinder()
{

}

void Tools::CovarianceMatrixFinder::execute()
{
	if (!this->boundary)
	{
		return;
	}

	UInt size = this->frontElementCentroids ? this->boundary->size() : this->boundary->verticesSize();

	this->addMeasure(0, 2*size);

	Performer::CovarianceMatrixFinder::execute();
}

void Tools::CovarianceMatrixFinder::average(UInt dimension, Real average[], const Real pt[], Real weight, Real n) const
{
	this->addMeasure(1, 0);

	Performer::CovarianceMatrixFinder::average(dimension, average, pt, weight, n);
}

void Tools::CovarianceMatrixFinder::covariance(UInt dimension, Real average[], const Real pt[], Real weight, Real n) const
{
	this->addMeasure(1, 0);

	Performer::CovarianceMatrixFinder::covariance(dimension, average, pt, weight, n);
}

#if USE_EXACT
void Tools::CovarianceMatrixFinder::average(UInt dimension, Data::EReal average[], const Data::EReal pt[], Data::EReal weight) const
{
	this->addMeasure(1, 0);

	Performer::CovarianceMatrixFinder::average(dimension, average, pt, weight);
}

void Tools::CovarianceMatrixFinder::covariance(UInt dimension, Data::EReal matrix[], Data::EReal average[], const Data::EReal pt[], Data::EReal weight) const
{
	this->addMeasure(1, 0);

	Performer::CovarianceMatrixFinder::covariance(dimension, matrix, average, pt, weight);
}
#endif //#if USE_EXACT

#endif //#if USE_NUMERICAL
