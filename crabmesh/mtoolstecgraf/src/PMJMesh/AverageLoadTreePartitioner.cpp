#include "PMJMesh/AverageLoadTreePartitioner.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/LoadCalculator.h"

using namespace PMJMesh;

PMJMesh::AverageLoadTreePartitioner::AverageLoadTreePartitioner(UInt dimension, const Data::BoxTree *tree,
#if USE_C__11
	const Data::RealHashMap *load,
#else
	const Data::RealMap *load,
#endif //#if USE_C__11
	bool skippingInner, UInt numProcesses, LoadCalculator *loadCalculator, bool useMean, Real maxAllowedLoad) :
		PMJMesh::MaxLoadTreePartitioner(dimension, tree, load, skippingInner,
			numProcesses, loadCalculator, useMean, maxAllowedLoad)
{

}

PMJMesh::AverageLoadTreePartitioner::~AverageLoadTreePartitioner()
{

}

Real PMJMesh::AverageLoadTreePartitioner::findMaxLoad() const
{
	Data::BoxTreeCell *root = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	/*if (this->loadCalculator->isAddingLoadForNodes())
	{
		Real basicLoad = this->loadCalculator->loadFor(PMJMesh::getPosition(this->dimension, root));

		if (basicLoad > 0.0)
		{
			Real l = basicLoad*(static_cast<Real>(this->numProcesses) - 1.0)/(std::pow(2.0, static_cast<Real>(this->dimension)) - 1.0);

			// L / N descontando a carga das celulas internas
			// ( L - (N-1)/(2^d - 1) ) / N
			return ((*this->load->find(root->getId())).second - l)/static_cast<Real>(this->numProcesses);
		}
	}*/

	//L / N
	return (*this->load->find(root->getId())).second/static_cast<Real>(this->numProcesses);
}
