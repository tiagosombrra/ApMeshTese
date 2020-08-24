#include "Performer/BoundaryWaterTightnessChecker.h"

#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"

Performer::BoundaryWaterTightnessChecker::BoundaryWaterTightnessChecker() :
	Performer::Builder::Builder()
{
	this->boundary = NULL;
	this->allowMultiple = false;
	this->stopOnFirst = false;
}

Performer::BoundaryWaterTightnessChecker::~BoundaryWaterTightnessChecker()
{

}

void Performer::BoundaryWaterTightnessChecker::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::BoundaryWaterTightnessChecker::setAllowMultiple(bool allowMultiple)
{
	this->allowMultiple = allowMultiple;
}

void Performer::BoundaryWaterTightnessChecker::setStopOnFirst(bool stopOnFirst)
{
	this->stopOnFirst = stopOnFirst;
}

const Performer::BoundaryWaterTightnessChecker::AdjacencyMap &Performer::BoundaryWaterTightnessChecker::getInconsistents() const
{
	return this->inconsistents;
}

void Performer::BoundaryWaterTightnessChecker::execute()
{
	this->inconsistents.clear();

	if (!this->boundary)
	{
		return;
	}

	UInt dimension = this->boundary->dimension();

	AdjacencyMap map;

	if (dimension == 2)
	{
		this->buildAdjMap2D(map);
	}
	else if (dimension == 3)
	{
		this->buildAdjMap3D(map);
	}

	this->check(map);
}

void Performer::BoundaryWaterTightnessChecker::buildAdjMap2D(AdjacencyMap &map)
{
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
		 iter != end; ++iter)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			map[fel->getVertex(i)][NULL].insert(fel);
		}
	}
}

void Performer::BoundaryWaterTightnessChecker::buildAdjMap3D(AdjacencyMap &map)
{
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
		 iter != end; ++iter)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			UInt j = (i+1)%fel->numVertices();

			Data::Vertex *v1 = fel->getVertex(i);
			Data::Vertex *v2 = fel->getVertex(j);

			if (v2 < v1)
			{
				Data::Vertex *tmp = v1;
				v1 = v2;
				v2 = tmp;
			}

			map[v1][v2].insert(fel);
		}
	}
}

void Performer::BoundaryWaterTightnessChecker::check(AdjacencyMap &map)
{
	for (AdjacencyMap::iterator iter = map.begin();
		 iter != map.end(); iter++)
	{
		this->check(iter);

		if ((this->stopOnFirst) && (!this->inconsistents.empty()))
		{
			break;
		}
	}
}

void Performer::BoundaryWaterTightnessChecker::check(const AdjacencyMap::iterator &iter)
{
	for (VertexFEListMap::iterator iter2 = (*iter).second.begin();
		 iter2 != (*iter).second.end(); iter2++)
	{
		if (this->check(iter2))
		{
			continue;
		}

		this->inconsistents.insert((*iter));

#if USE_GUI
		(*iter).first->setColor(1.0, 0.0, 0.0);

		if ((*iter2).first)
		{
			(*iter2).first->setColor(1.0, 0.0, 0.0);
		}

#if USE_C__11
		for (Data::FrontElementHash::const_iterator iter3 = (*iter2).second.begin();
#else
		for (Data::FrontElementSet::const_iterator iter3 = (*iter2).second.begin();
#endif //#if USE_C__11
			 iter3 != (*iter2).second.end(); iter3++)
		{
			(*iter3)->setColor(1.0, 0.0, 0.0);
		}
#endif //#if USE_GUI

		if ((*iter2).first)
		{
			std::cout << "Inconsistency detected in edge of vertices " << (*iter).first->text() << " and " << (*iter2).first->text() << std::endl;
		}
		else
		{
			std::cout << "Inconsistency detected in vertex " << (*iter).first->text() << std::endl;
		}

		if (this->stopOnFirst)
		{
			break;
		}
	}
}

bool Performer::BoundaryWaterTightnessChecker::check(const VertexFEListMap::iterator &iter2)
{
	return (this->allowMultiple ? (((*iter2).second.size()%2) == 0) : ((*iter2).second.size() == 2));
}
