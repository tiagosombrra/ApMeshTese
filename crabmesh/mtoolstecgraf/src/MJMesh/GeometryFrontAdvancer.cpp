#include "MJMesh/GeometryFrontAdvancer.h"

#include "Data/Point.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/BoxTree.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"

MJMesh::GeometryFrontAdvancer::GeometryFrontAdvancer(UInt dimension,
	const MJMesh::Boundary *boundary, const Data::BoxTree *tree, bool usingSearchTrees) :
	MJMesh::FrontAdvancer(dimension, boundary, tree, usingSearchTrees)
{

}

MJMesh::GeometryFrontAdvancer::~GeometryFrontAdvancer()
{

}

bool MJMesh::GeometryFrontAdvancer::isThereError(const Data::FrontElement */*fel*/, const Data::Vertex *best) const
{
	return !this->tree->find(best->getPoint());
}

bool MJMesh::GeometryFrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, Real radius) const
{
	//std::cout << "debug distance = " << static_cast<const MJMesh::FrontElement *>(element)->distance(point) << ", threshold = " << radius*0.1 << std::endl;
	//std::cout << "debug distance = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(point) << ", threshold = " << radius*0.1 << std::endl;

	if (radius > 0.0)
	{
#if USE_JMSH_IMPLEMENTATION_DETAILS
		///debug para ficar igual ao jmsh
		if (point->dimension() == 3)
		{
			//code from Prof. Bento's Jmsh implementation
			//static const Real cte1 = 4.0/std::sqrt(3.0);
			static const Real cte1 = 1.0/std::sqrt(3.0);
			//static const Real cte2 = std::sqrt(13.0)/4.0;
			static const Real cte2 = std::sqrt(13.0)/2.0;

			radius = std::sqrt(element->getShape()->size()*cte1)*cte2;
		}
		///endebug para ficar igual ao jmsh
#endif //#if USE_JMSH_IMPLEMENTATION_DETAILS
		//return (static_cast<const MJMesh::FrontElement *>(element)->distance(point) > (radius*0.1));
		return (static_cast<const MJMesh::FrontElement *>(element)->straightDistance(point) > (radius*0.1));
	}

	return true;
}

Data::Vertex *MJMesh::GeometryFrontAdvancer::best(const Data::FrontElement *element) const
{
	//return MJMesh::FrontAdvancer::best(element, true);
	return this->bestWithInsertionFE(element);
}
