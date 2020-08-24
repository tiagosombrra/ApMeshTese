#include "MJMesh/LaplacianSmoother.h"

#include "Data/Mesh.h"
#include "Data/FrontElement.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"

MJMesh::LaplacianSmoother::LaplacianSmoother(UInt dimension, Data::Mesh *mesh,
	bool testAdjacency, Real factor, bool sortPointsDisplacement) :
	Performer::LaplacianSmoother(dimension, mesh, testAdjacency, factor, sortPointsDisplacement)
{

}

MJMesh::LaplacianSmoother::~LaplacianSmoother()
{

}

void MJMesh::LaplacianSmoother::execute()
{
	//double time = Data::time();

	Performer::LaplacianSmoother::execute();

	//static int count = 0;
	//count++;

	//double timeClearing = Data::time();

#if (!USE_DELETE_FELS)
#if USE_ATTRIBS
	if (this->status.getType() == Performer::MeshSmoother::ended.getType())
	{
		this->clearAttribs();
	}
#endif //#if USE_ATTRIBS
#endif //#if (!USE_DELETE_FELS)

	//double end = Data::time();

	//time = end - time;
	//timeClearing = end - timeClearing;

	//std::stringstream str;
	//str << "  percentage clearing = " << timeClearing/time;

	//std::cout << str.str() << std::endl;
}

#if (!USE_DELETE_FELS)
#if USE_ATTRIBS
void MJMesh::LaplacianSmoother::clearAttribs() const
{
	//Data::FrontElementList fels = this->mesh->getFrontElements();

	/*if (fels.empty())
	{
		return;
	}*/

	//UInt dim = fels.front()->getShape()->dimension();
	//UInt dim = (*this->mesh->feBegin())->getShape()->dimension();
	//UInt dim = this->dimension;

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
		 iter != this->mesh->feEnd(); iter++)
	{
		//Data::GeometricShape *s = fels.front()->getShape();
		Data::GeometricShape *s = (*iter)->getShape();

		//fels.pop_front();

		//if (dim == 2)
		if (this->dimension == 2)
		{
#if USE_POOL
			static_cast<MJMesh::Edge2D *>(s)->recalcAttribs();
#else
			static_cast<MJMesh::Edge2D *>(s)->clearAttribs();
#endif //#if USE_POOL
		}
		else
		{
#if USE_POOL
			static_cast<MJMesh::Triangle3D *>(s)->recalcAttribs();
#else
			static_cast<MJMesh::Triangle3D *>(s)->clearAttribs();
#endif //#if USE_POOL
		}
	}
}
#endif //#if USE_ATTRIBS
#endif //#if (!USE_DELETE_FELS)
