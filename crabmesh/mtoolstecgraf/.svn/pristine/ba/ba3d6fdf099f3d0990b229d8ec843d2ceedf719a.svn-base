#include "Performer/BoundaryNormalFixer.h"

#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Point.h"
#include "Data/Edge2D.h"
#include "Data/Polygon.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"

const Performer::Status Performer::BoundaryNormalFixer::notInit =
	Performer::Status(Performer::BoundaryNormalFixer::NOT_INIT, "not initiated");
const Performer::Status Performer::BoundaryNormalFixer::init =
	Performer::Status(Performer::BoundaryNormalFixer::INIT, "initiated");
const Performer::Status Performer::BoundaryNormalFixer::success =
	Performer::Status(Performer::BoundaryNormalFixer::SUCCESS, "success");
const Performer::Status Performer::BoundaryNormalFixer::error =
	Performer::Status(Performer::BoundaryNormalFixer::ERROR, "error");

Performer::BoundaryNormalFixer::BoundaryNormalFixer() :
	Performer::Builder::Builder()
{
	this->boundary = NULL;

	this->dimension = 0;

	this->anyInverted = false;

	this->status = this->notInit;
}

Performer::BoundaryNormalFixer::~BoundaryNormalFixer()
{

}

void Performer::BoundaryNormalFixer::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

bool Performer::BoundaryNormalFixer::isAnyInverted() const
{
	return this->anyInverted;
}

void Performer::BoundaryNormalFixer::execute()
{
	if ((!this->boundary) || (this->boundary->size() == 0))
	{
		this->status = this->success;

		return;
	}

	this->status = init;

	this->anyInverted = true;

#if USE_C__11
	this->tested.reserve(this->boundary->size());
#endif //#if USE_C__11

	this->dimension = this->boundary->dimension();

	Data::Vertex *leftmost = NULL;
	Data::Point *leftmostpt = NULL;

	Data::FrontElement *fel = NULL;

	for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
		 iter != end; iter++)
	{
		Data::FrontElement *testing = (*iter);

		for (UInt i = 0; i < testing->numVertices(); i++)
		{
			Data::Vertex *v = testing->getVertex(i);
			Data::Point *p = v->getPoint();

			if (!leftmost)
			{
				leftmost = v;
				leftmostpt = p;

				fel = testing;

				continue;
			}

			for (UInt i = 0; i < this->dimension; i++)
			{
				Real pcoord = p->getCoord(i);
				Real lcoord = leftmostpt->getCoord(i);

				if (pcoord > lcoord)
				{
					break;
				}

				if (pcoord < lcoord)
				{
					leftmost = v;
					leftmostpt = p;

					fel = testing;

					break;
				}
			}
		}
	}

	if (!fel)
	{
		this->status = this->error;

		return;
	}

#if USE_EXPERIMENTAL_3
    bool ok = false;

    if (this->dimension == 2)
    {
        Data::Vector2D n = static_cast<Data::Edge2D *>(fel->getShape())->normal();

        for (UInt i = 0; i < this->dimension; i++)
        {
            if (n.getCoord(i) > 0)
            {
                ok = true;

                break;
            }

            if (n.getCoord(i) < 0)
            {
                ok = false;

                break;
            }
        }
    }
    else
    {
        Data::Vector3D n = static_cast<Data::Polygon *>(fel->getShape())->normal();

        for (UInt i = 0; i < this->dimension; i++)
        {
            if (n.getCoord(i) > 0)
            {
                ok = true;

                break;
            }

            if (n.getCoord(i) < 0)
            {
                ok = false;

                break;
            }
        }
    }
#else
	Data::Vector n = (this->dimension == 2) ?
		Data::Vector(static_cast<Data::Edge2D *>(fel->getShape())->normal()) :
		Data::Vector(static_cast<Data::Polygon *>(fel->getShape())->normal());

	bool ok = false;

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (n.getCoord(i) > 0)
		{
			ok = true;

			break;
		}

		if (n.getCoord(i) < 0)
		{
			ok = false;

			break;
		}
	}
#endif //#if USE_EXPERIMENTAL_3

	if (!ok)
	{
		this->invert(fel);

		this->anyInverted = true;
	}

	if (this->dimension == 2)
	{
		this->test2D(fel);
	}
	else
	{
		this->test3D(fel);
	}

	this->status = this->success;
}

bool Performer::BoundaryNormalFixer::shouldInvert2D(const Data::FrontElement *adj,
	const Data::FrontElement */*fel*/, const Data::Vertex *v) const
{
	return (adj->getVertex(1) == v);
}

bool Performer::BoundaryNormalFixer::shouldInvert3D(const Data::FrontElement *adj,
	const Data::FrontElement */*fel*/,
	const Data::Vertex *v1, const Data::Vertex *v2) const
{
	UInt numVerts = adj->numVertices();

	for (UInt i = 0; i < numVerts; i++)
	{
		if (adj->getVertex(i) == v1)
		{
			return (adj->getVertex((i + 1)%numVerts) == v2);
		}
	}

#if USE_GUI
	adj->setColor(1.0, 0.0, 0.0);
#endif //#if USE_GUI

	this->status = this->error;

	return false;
}

void Performer::BoundaryNormalFixer::test2D(Data::FrontElement *fel)
{
	Data::FrontElementList toBeTested;

	toBeTested.push_back(fel);

	while (!toBeTested.empty())
	{
		Data::FrontElement *fel = toBeTested.front();

		toBeTested.pop_front();

		this->test2D(fel, toBeTested);

		if (this->status.getType() == this->error.getType())
		{
			return;
		}
	}
}

void Performer::BoundaryNormalFixer::test3D(Data::FrontElement *fel)
{
	Data::FrontElementList toBeTested;

	toBeTested.push_back(fel);

	while (!toBeTested.empty())
	{
		Data::FrontElement *fel = toBeTested.front();

		toBeTested.pop_front();

		this->test3D(fel, toBeTested);

		if (this->status.getType() == this->error.getType())
		{
			return;
		}
	}
}

bool Performer::BoundaryNormalFixer::test2D(Data::FrontElement *fel, Data::FrontElementList &toBeTested)
{
	if (this->status.getType() == this->error.getType())
	{
		return false;
	}

	if (this->tested.find(fel) != this->tested.end())
	{
		return false;
	}

	Data::Vertex *v = fel->getVertex(1);

	Data::FrontElementList adjs = this->boundary->adjacency(v);

	if (adjs.size() != 2)
	{
#if USE_GUI
		fel->setColor(1.0, 0.0, 0.0);
#endif //#if USE_GUI

		this->status = this->error;

		return false;
	}

	this->tested.insert(fel);

	Data::FrontElement *adj = NULL;

	for (Data::FrontElementList::iterator iter = adjs.begin();
		 iter != adjs.end(); iter++)
	{
		if ((*iter) == fel)
		{
			continue;
		}

		adj = (*iter);

		break;
	}

	if (this->shouldInvert2D(adj, fel, v))
	{
		this->invert(adj);

		this->anyInverted = true;
	}

	//this->test2D(adj);
	toBeTested.push_back(adj);

	return true;
}

bool Performer::BoundaryNormalFixer::test3D(Data::FrontElement *fel, Data::FrontElementList &toBeTested)
{
	if (this->status.getType() == this->error.getType())
	{
		return false;
	}

	if (this->tested.find(fel) != this->tested.end())
	{
		return false;
	}

	this->tested.insert(fel);

	//Data::FrontElementList toBeTested;

	UInt numVerts = fel->numVertices();

	for (UInt i = 0; i < numVerts; i++)
	{
		Data::Vertex *v = fel->getVertex(i);
		Data::Vertex *o = fel->getVertex((i + 1)%numVerts);

		Data::FrontElement *adj = NULL;

		Data::FrontElementList adjs = this->boundary->adjacency(v);

		for (Data::FrontElementList::iterator iter = adjs.begin();
			 iter != adjs.end(); iter++)
		{
			if ((*iter) == fel)
			{
				continue;
			}

			if ((*iter)->have(o))
			{
				if (adj)
				{
#if USE_GUI
					adj->setColor(1.0, 0.0, 0.0);
					(*iter)->setColor(1.0, 0.0, 0.0);
#endif //#if USE_GUI

					this->status = this->error;

					return false;
				}

				adj = (*iter);

				//break;
			}
		}

		if (!adj)
		{
#if USE_GUI
			fel->setColor(1.0, 0.0, 0.0);
#endif //#if USE_GUI

			this->status = this->error;

			return false;
		}

		if (this->shouldInvert3D(adj, fel, v, o))
		{
			this->invert(adj);

			this->anyInverted = true;
		}

		toBeTested.push_back(adj);
	}

	/*for (Data::FrontElementList::iterator iter = toBeTested.begin();
		 iter != toBeTested.end(); iter++)
	{
		this->test3D((*iter));
	}*/

	return true;
}

void Performer::BoundaryNormalFixer::invert(Data::FrontElement *fel)
{
	fel->invert();
}
