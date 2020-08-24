#ifndef _TOOLS_COVARIANCE_MATRIX_FINDER_H_
#define _TOOLS_COVARIANCE_MATRIX_FINDER_H_

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Performer/CovarianceMatrixFinder.h"

#if USE_NUMERICAL

namespace Tools
{
	class CovarianceMatrixFinder :
		public Performer::CovarianceMatrixFinder,
		public Tools::ExternalTool
	{
	public:

		CovarianceMatrixFinder(const Tool *tool = NULL);
		virtual ~CovarianceMatrixFinder();

		virtual void execute();

	protected:

		using Performer::CovarianceMatrixFinder::average;
		using Performer::CovarianceMatrixFinder::covariance;

		virtual void average(UInt dimension, Real average[], const Real pt[], Real weight, Real n = 0) const;
		virtual void covariance(UInt dimension, Real average[], const Real pt[], Real weight, Real n = 0) const;

#if USE_EXACT
        virtual void average(UInt dimension, Data::EReal average[], const Data::EReal pt[], Data::EReal weight) const;
        virtual void covariance(UInt dimension, Data::EReal matrix[], Data::EReal average[], const Data::EReal pt[], Data::EReal weight) const;
#endif //#if USE_EXACT
	};
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _TOOLS_COVARIANCE_MATRIX_FINDER_H_
