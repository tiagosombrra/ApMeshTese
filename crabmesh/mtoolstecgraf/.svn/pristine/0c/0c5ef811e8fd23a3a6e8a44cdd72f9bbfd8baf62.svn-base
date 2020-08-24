#include "PJmesh/TreeGenerator.h"

#include "MJMesh/Boundary.h"
#include "Data/OctTree.h"
#include "PMJMesh/AngleTreeCellClassifier.h"

#if ((USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))
#include "Data/Box.h"
#endif //#if ((USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))

#include "PMJMesh/OctTree.h"

PJmesh::TreeGenerator::TreeGenerator(MJMesh::Boundary *boundary, Msh3DOctTree root
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	, const Parallel::ThreadManager *tm
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	) :
		Jmesh::TreeGenerator(root),
		PMJMesh::TreeGenerator(3, boundary, 0.0
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			, tm
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
			)
{

}

PJmesh::TreeGenerator::~TreeGenerator()
{

}

void PJmesh::TreeGenerator::execute()
{
	this->tcc = this->makeTCC();

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP) && (USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))
	//to avoid the recreation of attribs in MJMesh::Edge2D and in
	//  MJMesh::Triangle3D
	const Data::FrontElementList *fels = this->findElements();

	for (Data::FrontElementList::const_iterator iter = fels->begin();
		 iter != fels->end(); iter++)
	{
		(*iter)->getShape()->box();
	}
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP) && (USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))

#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)
	if (!this->elementsMap)
	{
#if USE_C__11
		this->elementsMap = new PMJMesh::FEListHashMap();
#else
		this->elementsMap = new PMJMesh::FEListMap();
#endif //#if USE_C__11
	}

	this->elementsMap->insert(std::make_pair(0, *this->findElements()));
#endif //#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)

	Jmesh::TreeGenerator::execute();

	if ((this->boundary) && (this->boundary->size() == 0))
	{
		PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot()), Data::OUT);
	}
	else
	{
		this->classify();

		static_cast<PMJMesh::OctTree *>(this->getTree())->adjustPosition();
	}

	delete this->tcc;

	this->tcc = NULL;
}

Data::Tree *PJmesh::TreeGenerator::makeTree(const Data::Point3D &min, const Data::Point3D &max) const
{
	return PMJMesh::TreeGenerator::makeTree(min, max);
}

void PJmesh::TreeGenerator::subdivide(Data::OctTreeCell *cell) const
{
	Jmesh::TreeGenerator::subdivide(cell);

	PMJMesh::TreeGenerator::classifySubdividedCell(cell);
}
