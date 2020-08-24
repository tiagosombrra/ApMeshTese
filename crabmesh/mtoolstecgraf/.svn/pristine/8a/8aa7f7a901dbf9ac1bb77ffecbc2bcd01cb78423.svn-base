#include "Performer/MeshInterceptionChecker.h"

#include "Data/Box.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

Performer::MeshInterceptionChecker::MeshInterceptionChecker() :
	Performer::Builder::Builder()
{
	this->boundary = NULL;
	this->mesh = NULL;
	this->dimension = 0;
	this->stopOnFirst = false;
}

Performer::MeshInterceptionChecker::~MeshInterceptionChecker()
{

}

void Performer::MeshInterceptionChecker::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::MeshInterceptionChecker::setMesh(const Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::MeshInterceptionChecker::setStopOnFirst(bool stopOnFirst)
{
	this->stopOnFirst = stopOnFirst;
}

const Performer::MeshInterceptionChecker::InterceptionMap &Performer::MeshInterceptionChecker::getInterceptions() const
{
	return this->interceptions;
}

void Performer::MeshInterceptionChecker::execute()
{
	if (!this->mesh)
	{
		return;
	}

	this->dimension = this->mesh->dimension();

	bool useFels = false;

	if ((this->mesh->size() == 0) || (!this->boundary) || (this->boundary->size() != this->mesh->frontElementsSize()))
	{
		useFels = true;
	}

	if (!useFels)
	{
		this->makeFrontElements();
	}

	this->checkInterceptions();

	this->clearNewFrontElements();
}

void Performer::MeshInterceptionChecker::makeFrontElements()
{
	Data::VertexVector vertices(this->dimension, NULL);

	for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
		 iter != end; iter++)
	{
		Data::Vertex *v = (*iter);

		Data::ElementList els = this->mesh->adjacency(v);

		Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(v);

		if (this->dimension == 2)
		{
			for (Data::ElementList::iterator iter2 = els.begin();
				 iter2 != els.end(); iter2++)
			{
				Data::Element *el = (*iter2);

				for (UInt i = 0; i < el->numVertices(); i++)
				{
					Data::Vertex *o = el->getVertex(i);

					if (o == v)
					{
						continue;
					}

					Data::FrontElement *fel = NULL;

					for (Data::FrontElementList::iterator iter3 = fels.begin();
						 iter3 != fels.end(); iter3++)
					{
						if ((*iter3)->have(o))
						{
							fel = (*iter3);

							break;
						}
					}

					if (fel)
					{
						this->adjustTopology(fel, el);

						continue;
					}

					vertices[0] = v;
					vertices[1] = o;

					fel = this->makeFrontElement(vertices, el);

					fels.push_back(fel);
					this->newFels.insert(fel);

					const_cast<Data::Mesh *>(mesh)->add(fel);
				}
			}
		}
		else if (this->dimension == 3)
		{
			for (Data::ElementList::iterator iter2 = els.begin();
				 iter2 != els.end(); iter2++)
			{
				Data::Element *el = (*iter2);

				for (UInt i = 0; i < el->numVertices() - 1; i++)
				{
					Data::Vertex *o1 = el->getVertex(i);

					if (o1 == v)
					{
						continue;
					}

					for (UInt j = i + 1; j < el->numVertices(); j++)
					{
						Data::Vertex *o2 = el->getVertex(j);

						if (o2 == v)
						{
							continue;
						}

						Data::FrontElement *fel = NULL;

						for (Data::FrontElementList::iterator iter3 = fels.begin();
							 iter3 != fels.end(); iter3++)
						{
							if (((*iter3)->have(o1)) && ((*iter3)->have(o2)))
							{
								fel = (*iter3);

								break;
							}
						}

						if (fel)
						{
							this->adjustTopology(fel, el);

							continue;
						}

						vertices[0] = v;
						vertices[1] = o1;
						vertices[2] = o2;

						fel = this->makeFrontElement(vertices, el);

						fels.push_back(fel);
						newFels.insert(fel);

						const_cast<Data::Mesh *>(mesh)->add(fel);
					}
				}
			}
		}
	}
}

void Performer::MeshInterceptionChecker::checkInterceptions()
{
	this->interceptions.clear();

	bool intercepted = false;

	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
		 iter != this->mesh->feEnd(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		Data::Box box = fel->getShape()->box();

		Data::Mesh::ConstFrontElementIterator iter2 = iter;

		for (iter2++; iter2 != this->mesh->feEnd(); iter2++)
		{
			Data::FrontElement *fel2 = (*iter2);

			if ((box.optIntercept(fel2->getShape()->box())) && (fel->eintercept(fel2)))
			{
				this->interceptions[fel].insert(fel2);
				this->interceptions[fel2].insert(fel);

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
}

void Performer::MeshInterceptionChecker::clearNewFrontElements()
{
#if USE_C__11
	for (Data::FrontElementHash::iterator iter = this->newFels.begin();
#else
	for (Data::FrontElementSet::iterator iter = this->newFels.begin();
#endif //#if USE_C__11
		 iter != this->newFels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		const_cast<Data::Mesh *>(mesh)->remove(fel);

		delete fel;
	}

	this->newFels.clear();
}

/*Data::FrontElement *Performer::MeshInterceptionChecker::makeFrontElement(
	const Data::VertexVector &vertices, Data::Element *el) const
{
	return NULL;
}*/

void Performer::MeshInterceptionChecker::adjustTopology(Data::FrontElement */*fel*/, Data::Element */*el*/) const
{

}
