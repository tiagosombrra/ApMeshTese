#include "PMJMesh/BoundaryLoadTreePartitioner.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/LoadCalculator.h"

using namespace PMJMesh;

PMJMesh::BoundaryLoadTreePartitioner::BoundaryLoadTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses,
	LoadCalculator *loadCalculator, bool useMean, Real maxAllowedLoad) :
		PMJMesh::MaxLoadTreePartitioner(dimension, tree, load, skippingInner,
			numProcesses, loadCalculator, useMean, maxAllowedLoad)
{

}

PMJMesh::BoundaryLoadTreePartitioner::~BoundaryLoadTreePartitioner()
{

}

Real PMJMesh::BoundaryLoadTreePartitioner::findMaxLoad() const
{
	Data::BoxTreeCell *root = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	// L / N*logN
	//return (*this->load->find(root->getId())).second/std::pow(static_cast<Real>(this->numProcesses), this->dimension);
	// L / N*sqrtN
	//return (*this->load->find(root->getId())).second/(static_cast<Real>(this->numProcesses)*std::pow(static_cast<Real>(this->numProcesses), 1.0/static_cast<Real>(this->dimension)));
	// L / dim*N
	//return (*this->load->find(root->getId())).second/(static_cast<Real>(this->numProcesses)*static_cast<Real>(this->dimension));
	// L / 2*dim*sqrtN
	//return (*this->load->find(root->getId())).second/(2.0*static_cast<Real>(this->dimension)*std::pow(static_cast<Real>(this->numProcesses), 1.0/static_cast<Real>(this->dimension)));
	// L / (N/4)^2
	//return (*this->load->find(root->getId())).second/std::pow(static_cast<Real>(this->numProcesses)/(2.0*static_cast<Real>(this->dimension)), static_cast<Real>(this->dimension)/static_cast<Real>(this->dimension - 1));

	if (this->numProcesses == 1)
	{
		//total load
		return (*this->load->find(root->getId())).second;
	}

	//dim == 2 -> L / (N/4 + 1)^2
	//dim == 3 -> L / (sqrt(6*(N - 2))/6 + 1)^3

	//Real den = (this->dimension == 2) ? std::pow(1.0 + static_cast<Real>(this->numProcesses)/4.0, 2.0) :
	//	((this->numProcesses == 2) ? 2.0 : std::pow(1.0 + std::sqrt(6.0*(static_cast<Real>(this->numProcesses) - 2.0))/6.0, 3.0));

	Real den = 1.0;

	if (this->dimension == 2)
	{
		den = std::pow(1.0 + static_cast<Real>(this->numProcesses)/4.0, 2.0);
	}
	else if (this->numProcesses == 2)
	{
		den = 2.0;
	}
	else
	{
		den = std::pow(1.0 + std::sqrt(6.0*(static_cast<Real>(this->numProcesses) - 2.0))/6.0, 3.0);
	}

	/*if (this->loadCalculator->isAddingLoadForNodes())
	{
		Real basicLoad = this->loadCalculator->loadFor(PMJMesh::getPosition(this->dimension, root));

		if (basicLoad > 0.0)
		{
			Real l = ((this->dimension == 3) && (this->numProcesses == 2)) ? basicLoad :
				basicLoad*(den - 1.0)/(std::pow(2.0, static_cast<Real>(this->dimension)) - 1.0);

			// descontando a carga das celulas internas
			return ((*this->load->find(root->getId())).second - l)/den;
		}
	}*/

	return (*this->load->find(root->getId())).second/den;
}
