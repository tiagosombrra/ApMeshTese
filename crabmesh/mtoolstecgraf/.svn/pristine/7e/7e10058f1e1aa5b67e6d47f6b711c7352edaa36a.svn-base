#include "Tools/MeshInterceptionChecker.h"

#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "MJMesh/Front.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/TreeGenerator.h"
#include "MJMesh/FrontElementSearchTreeGenerator.h"

Tools::MeshInterceptionChecker::MeshInterceptionChecker() :
	Performer::MeshInterceptionChecker::MeshInterceptionChecker(),
	Tools::Tool::Tool()
{

}

Tools::MeshInterceptionChecker::~MeshInterceptionChecker()
{

}

const Data::FrontElementList &Tools::MeshInterceptionChecker::getSmall() const
{
	return this->small;
}

void Tools::MeshInterceptionChecker::execute()
{
	this->startMeasure();

	Performer::MeshInterceptionChecker::execute();

	this->endMeasure();
}

void Tools::MeshInterceptionChecker::checkInterceptions()
{
	this->small.clear();
	this->interceptions.clear();

	MJMesh::Front *tmpFront = new MJMesh::Front();
	MJMesh::Boundary *tmpBoundary = new MJMesh::Boundary();

	Real tolerance = Data::getTolerance();

	Data::setTolerance(tolerance*0.00001);

	this->setMeasure(0, static_cast<UInt>(this->mesh->frontElementsSize()) + 1);

	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
		 iter != this->mesh->feEnd(); iter++)
	{
		this->addMeasure(1, 0);

		if ((*iter)->getShape()->size() <= Data::getTolerance())
		{
			this->small.push_back((*iter));

			std::cout << "too small front element: " << (*iter)->text() << std::endl;

			continue;
		}

		tmpFront->add((*iter));
		tmpBoundary->add((*iter));

		for (UInt i = 0; i < (*iter)->numVertices(); i++)
		{
			if (!tmpBoundary->have((*iter)->getVertex(i)))
			{
				tmpBoundary->add((*iter)->getVertex(i));
			}
		}
	}

	Data::setTolerance(tolerance);

	UInt dimension = mesh->dimension();

	MJMesh::TreeGenerator tg(dimension, tmpBoundary, 1.0);

	tg.execute();

	Data::BoxTree *tree = static_cast<Data::BoxTree *>(tg.getTree());

	tmpBoundary->clear();

	delete tmpBoundary;

	MJMesh::FrontElementSearchTreeGenerator festg(tmpFront, tree);

	festg.execute();

	Data::BoxSearchTree *stree = static_cast<Data::BoxSearchTree *>(festg.getTree());

	tmpFront->setFESTree(stree);

	bool intercepted = false;

	this->addMeasure(1, static_cast<UInt>(this->mesh->frontElementsSize()));

	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
		 iter != this->mesh->feEnd(); iter++)
	{
		this->addMeasure(1, 0);

		Data::FrontElement *fel = (*iter);

		Data::Box box = fel->getShape()->box();

		if (tmpFront->have(fel))
		{
			tmpFront->remove(fel);
		}

		Data::FrontElementList cands = tmpFront->frontElementsInRange(box);

		for (Data::FrontElementList::iterator iter2 = cands.begin();
			 iter2 != cands.end(); iter2++)
		{
			Data::FrontElement *fel2 = (*iter2);

			if ((box.optIntercept(fel2->getShape()->box())) && (fel->eintercept(fel2)))
			{
				this->interceptions[fel].insert(fel2);
				this->interceptions[fel2].insert(fel);

#if USE_GUI
				fel->setColor(1.0, 0.0, 0.0);
				fel2->setColor(1.0, 0.0, 0.0);
#endif //#if USE_GUI

				std::cout <<  "intercepting front elements: " << fel->text() << " and " << fel2->text() << std::endl;

				//std::cout << "start comparison" << std::endl;

				//fel->eintercept(fel2);
				//fel->intercept(fel2);

				intercepted = true;

				if (this->stopOnFirst)
				{
					break;
				}
			}
		}

		if ((intercepted) && (this->stopOnFirst))
		{
			break;
		}
	}

	tmpFront->clear();

	tmpFront->setFESTree(NULL);

	delete tmpFront;

	delete stree;

	delete tree;
}

Data::FrontElement *Tools::MeshInterceptionChecker::makeFrontElement(
	const Data::VertexVector &vertices, Data::Element *el) const
{
	if (this->dimension == 2)
	{
		MJMesh::FrontElement *fel = new MJMesh::FrontElement2D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]));

		fel->add(static_cast<MJMesh::Element *>(el));

		return fel;
	}

	if ((this->dimension == 3) && (vertices.size() == 3))
	{
		MJMesh::FrontElement *fel = new MJMesh::FrontElement3D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]),
			static_cast<MJMesh::Vertex *>(vertices[2]));

		fel->add(static_cast<MJMesh::Element *>(el));

		return fel;
	}

	return NULL;
}

void Tools::MeshInterceptionChecker::adjustTopology(Data::FrontElement *fel, Data::Element *el) const
{
	if ((!static_cast<MJMesh::FrontElement *>(fel)->haveAdjacent(static_cast<MJMesh::Element *>(el))) && (this->newFels.find(fel) != this->newFels.end()))
	{
		static_cast<MJMesh::FrontElement *>(fel)->add(static_cast<MJMesh::Element *>(el));
	}
}
