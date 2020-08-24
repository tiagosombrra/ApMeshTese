#include "Tools/BoundaryNormalFixer.h"

#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Point.h"
#include "Data/Edge2D.h"
#include "Data/Polygon.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "MJMesh/AnglePointInObjectDecider.h"

Tools::BoundaryNormalFixer::BoundaryNormalFixer() :
	Performer::BoundaryNormalFixer::BoundaryNormalFixer()
{
	this->current = NULL;

	this->decider = NULL;
}

Tools::BoundaryNormalFixer::~BoundaryNormalFixer()
{
	if (this->decider)
	{
		delete this->decider;

		this->decider = NULL;
	}
}

void Tools::BoundaryNormalFixer::execute()
{
	if ((!this->boundary) || (this->boundary->size() == 0))
	{
		this->status = this->success;

		return;
	}

	this->setMeasure(0, static_cast<UInt>(this->boundary->size()) + 1);

	this->status = this->init;

#if USE_C__11
	this->tested.reserve(this->boundary->size());
#endif //#if USE_C__11

	this->dimension = this->boundary->dimension();

	BoundaryTree *root = new BoundaryTree();

	while (this->tested.size() != this->boundary->size())
	{
		if (this->status.getType() == this->error.getType())
		{
			break;
		}

		Data::Vertex *leftmost = NULL;
		Data::Point *leftmostpt = NULL;

		Data::FrontElement *fel = NULL;

		for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
			 iter != end; iter++)
		{
			Data::FrontElement *testing = (*iter);

			if (this->tested.find(testing) != this->tested.end())
			{
				continue;
			}

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

			break;
		}

		{
			Data::FrontElementList fels = this->boundary->adjacency(leftmost);

			//fel = fels.front();

			//fels.pop_front();

			Data::VertexVector verts = fel->getVertices();

			std::sort(verts.begin(), verts.end(), smallerCoords);

			while (!fels.empty())
			{
				Data::FrontElement *other = fels.front();

				fels.pop_front();

				if ((other == fel) ||
					(this->tested.find(other) != this->tested.end()))
				{
					continue;
				}

				Data::VertexVector overts = other->getVertices();

				std::sort(overts.begin(), overts.end(), smallerCoords);

				UInt nverts = static_cast<UInt>(std::min(verts.size(), overts.size()));

				for (UInt i = 0; i < nverts; i++)
				{
					if (smallerCoords(overts[i], verts[i]))
					{
						verts = overts;
						fel = other;

						break;
					}
				}
			}
		}

		if (this->tested.find(fel) != this->tested.end())
		{
			this->status = this->error;

			break;
		}

		bool inner = false;
		BoundaryTree *innermost = root;

		if (!root->children.empty())
		{
			if (!this->decider)
			{
				this->decider = this->makeDecider();
			}

			this->decider->setPoint(leftmostpt);

			inner = this->isInner(root, innermost);
		}

		this->current = new BoundaryTree();

		innermost->children.push_back(this->current);

#if USE_EXPERIMENTAL_3
        struct PointingInside
        {
            static bool pointingInside(const UInt &dimension, const Data::Vector &v)
            {
                for (UInt i = 0; i < dimension; i++)
                {
                    if (v.getCoord(i) > 0)
                    {
                        return true;
                    }

                    if (v.getCoord(i) < 0)
                    {
                        return false;
                    }
                }

                return false;
            }
        };

        bool pointingInside = false;

        if (this->dimension == 2)
        {
            Data::Vector2D n = static_cast<Data::Edge2D *>(fel->getShape())->normal();

            pointingInside = PointingInside::pointingInside(this->dimension, n);
        }
        else
        {
            Data::Vector3D n = static_cast<Data::Polygon *>(fel->getShape())->normal();

            pointingInside = PointingInside::pointingInside(this->dimension, n);
        }
#else
		Data::Vector n = (this->dimension == 2) ?
			Data::Vector(static_cast<Data::Edge2D *>(fel->getShape())->normal()) :
			Data::Vector(static_cast<Data::Polygon *>(fel->getShape())->normal());

		bool pointingInside = false;

		for (UInt i = 0; i < this->dimension; i++)
		{
			if (n.getCoord(i) > 0)
			{
				pointingInside = true;

				break;
			}

			if (n.getCoord(i) < 0)
			{
				pointingInside = false;

				break;
			}
		}
#endif //#if USE_EXPERIMENTAL_3

		if (((!pointingInside) && (!inner)) ||
			(( pointingInside) && ( inner)))
		{
			this->invert(fel);
		}

		if (this->dimension == 2)
		{
			this->test2D(fel);
		}
		else
		{
			this->test3D(fel);
		}
	}

	this->current = NULL;

	this->clear(root);

	if (this->decider)
	{
		delete this->decider;

		this->decider = NULL;
	}

	if (this->status.getType() < this->error.getType())
	{
		this->status = this->success;
	}

	this->endMeasure();
}

bool Tools::BoundaryNormalFixer::smallerCoords(const Data::Vertex *v1, const Data::Vertex *v2)
{
	UInt dimension = v1->getPoint()->dimension();
	const Real *coords1 = v1->getPoint()->getCoordsArray();
	const Real *coords2 = v2->getPoint()->getCoordsArray();

	for (UInt i = 0; i < dimension; i++)
	{
		if (coords1[i] < coords2[i])
		{
			return true;
		}

		if (coords1[i] > coords2[i])
		{
			return false;
		}
	}

	return false;
}

MJMesh::AnglePointInObjectDecider *Tools::BoundaryNormalFixer::makeDecider() const
{
	return new MJMesh::AnglePointInObjectDecider(this->dimension, NULL, NULL, true);
}

bool Tools::BoundaryNormalFixer::test2D(Data::FrontElement *fel, Data::FrontElementList &toBoTested)
{
	bool ret = this->Performer::BoundaryNormalFixer::test2D(fel, toBoTested);

	if (ret)
	{
		this->current->fels.push_back(fel);

		this->addMeasure(1, 0);
	}

	return ret;
}

bool Tools::BoundaryNormalFixer::test3D(Data::FrontElement *fel, Data::FrontElementList &toBoTested)
{
	bool ret = this->Performer::BoundaryNormalFixer::test3D(fel, toBoTested);

	if (ret)
	{
		this->current->fels.push_back(fel);

		this->addMeasure(1, 0);
	}

	return ret;
}

bool Tools::BoundaryNormalFixer::isInner(const BoundaryTree *node, BoundaryTree *&innermost) const
{
	if ((!node) || (node->children.empty()))
	{
		return false;
	}

	for (BoundaryTreeList::const_iterator iter = node->children.begin();
		 iter != node->children.end(); iter++)
	{
		BoundaryTree *n = (*iter);

		this->decider->setFels(&n->fels);

		this->decider->execute();

		Data::Position pos = this->decider->getDecision();

		if (pos == Data::IN)
		{
			innermost = n;

			return !this->isInner(n, innermost);
		}
	}

	return false;
}

void Tools::BoundaryNormalFixer::clear(BoundaryTree *node) const
{
	for (BoundaryTreeList::iterator iter = node->children.begin();
		 iter != node->children.end(); iter++)
	{
		this->clear((*iter));
	}

	//node.children.clear();
	//node.fels.clear();

	delete node;
}
