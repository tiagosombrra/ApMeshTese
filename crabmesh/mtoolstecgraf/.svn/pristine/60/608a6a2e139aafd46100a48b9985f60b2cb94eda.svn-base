#include "MJMesh/MinTreeRefiner.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/FrontElement.h"
#include "Data/GeometricShape.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace MJMesh;

const Performer::Status MJMesh::MinTreeRefiner::notInit =
	Performer::Status(MJMesh::MinTreeRefiner::NOT_INIT, "not initiated");
const Performer::Status MJMesh::MinTreeRefiner::ended =
	Performer::Status(MJMesh::MinTreeRefiner::ENDED, "ended");

MJMesh::MinTreeRefiner::MinTreeRefiner(Data::BoxTree *tree, UInt minLevel) :
	Performer::MinTreeRefiner(tree, minLevel)
{
	this->setBoundary(NULL);

	this->status = MJMesh::MinTreeRefiner::notInit;
}

MJMesh::MinTreeRefiner::MinTreeRefiner(Data::BoxTree *tree,
	const MJMesh::Boundary *boundary) :
	Performer::MinTreeRefiner(tree)
{
	this->setBoundary(boundary);

	this->status = MJMesh::MinTreeRefiner::notInit;
}

MJMesh::MinTreeRefiner::~MinTreeRefiner()
{

}

void MJMesh::MinTreeRefiner::setBoundary(const MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

UInt MJMesh::MinTreeRefiner::findMinLevel() const
{
	if (!this->boundary)
	{
		return this->getMinLevel();
	}

	UInt minLevel = UINT_MAX;

	//Data::FrontElementVector elements = this->boundary->getElements();
	//Data::FrontElementList elements = this->boundary->getElements();

	//while (!elements.empty())
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		//MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(elements.back());
		MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>((*iter));
		//elements.pop_back();

#if USE_EXPERIMENTAL_3
		Data::Point2D mid2D;
        Data::Point3D mid3D;

        Data::Point &mid = (e->getShape()->dimension() == 2) ?
            (mid2D = e->getShape()->mid2D(), static_cast<Data::Point &>(mid2D)) :
            (mid3D = e->getShape()->mid3D(), static_cast<Data::Point &>(mid3D));

		Data::BoxTreeCell *c = static_cast<Data::BoxTree *>(tree)->find(mid);
#else
		Data::BoxTreeCell *c = static_cast<Data::BoxTree *>(tree)->find(e->getShape()->mid());
#endif //#if USE_EXPERIMENTAL_3

		UInt level = c->level();

		if (level < minLevel)
		{
			minLevel = level;
		}
	}

#if USE_JMSH_DEBUG_MESSAGES
	//std::cout << "markos debug, generating tree, min_level = " << minLevel << std::endl;
#endif //#if USE_JMSH_DEBUG_MESSAGES

	return minLevel;
}

void MJMesh::MinTreeRefiner::execute()
{
#if USE_JMSH_IMPLEMENTATION_DETAILS
	///debug para ficar igual ao jmsh
	if (static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->dimension() == 3)
	{
		this->status = MJMesh::MinTreeRefiner::ended;

		return;
	}
	///endebug para ficar igual ao jmsh
#endif //#if USE_JMSH_IMPLEMENTATION_DETAILS

#if USE_GUI
	static_cast<Data::BoxTree *>(this->tree)->freeze();
#endif //#if USE_GUI

	Performer::MinTreeRefiner::execute();

#if USE_GUI
    for (Data::Tree::LeafIterator iter = this->tree->lBegin();
         iter != this->tree->lEnd(); iter++)
    {
        static_cast<Data::BoxTreeCell *>((*iter))->box();
    }

	static_cast<Data::BoxTree *>(this->tree)->unfreeze();
#endif //#if USE_GUI

	this->status = MJMesh::MinTreeRefiner::ended;
}
