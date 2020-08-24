#include "Performer/InnerSurfacesIdentifier.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Edge2D.h"
#include "Data/Edge3D.h"
#include "Data/Polygon.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

Performer::InnerSurfacesIdentifier::InnerSurfacesIdentifier() :
	Performer::Builder::Builder()
{
	this->boundary = NULL;
	this->mesh = NULL;
	this->dimension = 0;
	this->manifold = true;
	this->root = NULL;
	this->surfaceDirection = UINT_MAX;
	this->needTree = false;
}

Performer::InnerSurfacesIdentifier::~InnerSurfacesIdentifier()
{
	/*while (!this->surfaces.empty())
	{
		if ((this->needTree) && (this->root))
        {
            delete this->surfaces.front();
        }

		//delete this->surfaces.front();        ///DOIS DELETES?

		this->surfaces.pop_front();
	}*/

	if ((!this->needTree) || (this->root))
    {
        while (!this->surfaces.empty())
        {
            delete this->surfaces.front();

            this->surfaces.pop_front();
        }
    }

	if (this->root)
	{
		this->clearTree();
	}
}

void Performer::InnerSurfacesIdentifier::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::InnerSurfacesIdentifier::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::InnerSurfacesIdentifier::setSurfaceDirection(UInt surfaceDirection)
{
    this->surfaceDirection = surfaceDirection;
}

void Performer::InnerSurfacesIdentifier::setNeedTree(bool needTree)
{
    this->needTree = needTree;
}

const Performer::InnerSurfacesIdentifier::SurfaceTree *Performer::InnerSurfacesIdentifier::getTree() const
{
    return this->root;
}

void Performer::InnerSurfacesIdentifier::unsetTree()
{
    this->root = NULL;
    this->needTree = false;
}

bool Performer::InnerSurfacesIdentifier::isManifold() const
{
	return this->manifold;
}

Data::FrontElementList Performer::InnerSurfacesIdentifier::getInnerSurfaces() const
{
	return this->innerSurfaces;
}

void Performer::InnerSurfacesIdentifier::execute()
{
	if ((!this->boundary) || (!this->mesh))
	{
		return;
	}

	this->manifold = true;

	this->dimension = this->boundary->dimension();

	this->propagate();

	if (this->surfaces.size() == 1)
	{
	    if ((this->needTree) && (this->manifold))
        {
            this->findCompleteBoundaries();
        }
        else
        {
            delete this->surfaces.front();

            this->surfaces.pop_front();
        }

		return;
	}

	if ((!this->manifold) && (!this->needTree))
	{///TODO
		return;
	}

	this->findCompleteBoundaries();

#if USE_GUI
	/*Int i = 0;

	for (SurfaceList::iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++, i++)
	{
		Real r, g, b;

		Data::pallete(i, r, g, b);

		for (Data::Boundary::FrontElementIterator iter2 = (*iter)->boundary->eBegin(), end = (*iter)->boundary->eEnd();
			 iter2 != end; iter2++)
		{
			(*iter2)->setColor(r, g, b);
		}
	}*/

	SurfaceTreeList treeSearch;

	//treeSearch.push_back(this->root);

	UInt level = 0;

	for (SurfaceTreeList::iterator iter = this->root->children.begin();
         iter != this->root->children.end(); iter++)
    {
        treeSearch.push_back((*iter));
    }

	SurfaceTree *last = treeSearch.empty() ? NULL : treeSearch.back();

	while (!treeSearch.empty())
    {
        SurfaceTree *node = treeSearch.front();

        treeSearch.pop_front();

        Real r, g, b;
        Data::pallete(level, r, g, b);

        if (node->surface)
        {
            for (Data::Boundary::FrontElementIterator iter = node->surface->boundary->eBegin(), end = node->surface->boundary->eEnd();
                 iter != end; iter++)
            {
                (*iter)->setColor(r, g, b);

                for (UInt i = 0; i < (*iter)->numVertices(); i++)
                {
                    (*iter)->getVertex(i)->setColor(r, g, b);
                }
            }
        }

        for (SurfaceTreeList::iterator iter = node->children.begin();
             iter != node->children.end(); iter++)
        {
            treeSearch.push_back((*iter));
        }

        if (node == last)
        {
            level++;

            last = treeSearch.empty() ? NULL : treeSearch.back();
        }
    }
#endif //#if USE_GUI

	if (this->needTree)
    {
        SurfaceTreeList treeSearch;

        enum Data::Position pos = Data::OUT;

        for (SurfaceTreeList::iterator iter = this->root->children.begin();
             iter != this->root->children.end(); iter++)
        {
            treeSearch.push_back((*iter));
        }

        SurfaceTree *last = treeSearch.empty() ? NULL : treeSearch.back();

        while (!treeSearch.empty())
        {
            SurfaceTree *node = treeSearch.front();

            treeSearch.pop_front();

            if (node->surface->position != Data::UNKNOWN)
            {
                node->surface->position = pos;
            }

            for (SurfaceTreeList::iterator iter = node->children.begin();
                 iter != node->children.end(); iter++)
            {
                treeSearch.push_back((*iter));
            }

            if (node == last)
            {
                pos = (pos == Data::OUT) ? Data::IN : Data::OUT;

                last = treeSearch.empty() ? NULL : treeSearch.back();
            }
        }
    }
    else
    {
        while (!this->surfaces.empty())
        {
            Surface *s = this->surfaces.front();

            this->surfaces.pop_front();

            if ((s->position != Data::OUT) && (s->position != Data::UNKNOWN))
            {
                for (Data::Boundary::FrontElementIterator iter = s->boundary->eBegin();
                     iter != s->boundary->eEnd(); iter++)
                {
                    this->innerSurfaces.push_back((*iter));
                }
            }

            delete s;
        }

        this->clearTree();
    }
}

Performer::InnerSurfacesIdentifier::Surface *Performer::InnerSurfacesIdentifier::surface(Data::FrontElement *fe) const
{
    for (SurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		if ((*iter)->boundary->have(fe))
		{
			return (*iter);
		}
	}

	return NULL;
}

void Performer::InnerSurfacesIdentifier::propagate()
{
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		this->propagate((*iter));
	}
}

void Performer::InnerSurfacesIdentifier::propagate(Data::FrontElement *fe)
{
	if (this->surface(fe))
	{
		return;
	}

	Data::Boundary *boundary = this->makeBoundary();

	boundary->add(fe);

	for (UInt i = 0; i < fe->numVertices(); i++)
	{
		boundary->add(fe->getVertex(i));
	}

	Surface *s = new Surface();

	s->boundary = boundary;
	s->position = Data::UNKNOWN;
	s->manifold = true;

	this->surfaces.push_front(s);

	this->propagate(fe, s);
}

void Performer::InnerSurfacesIdentifier::propagate(Data::FrontElement *fe, Surface *surface)
{
	Data::FrontElementList topropagate;

	for (UInt i = 0; i < fe->numVertices(); i++)
	{
		Data::Vertex *v = fe->getVertex(i);
		Data::Vertex *n = NULL;

		Data::FrontElementList adjs = this->boundary->adjacency(v);

		if ((this->dimension == 3) && (this->surfaceDirection >= this->dimension))
		{
			n = fe->getVertex((i + 1)%fe->numVertices());

			for (Data::FrontElementList::iterator iter = adjs.begin();
				 iter != adjs.end();)
			{
				Data::FrontElement *adj = (*iter);

				if (adj->have(n))
				{
					iter++;

					continue;
				}

				iter = adjs.erase(iter);
			}
		}

		if (adjs.size() == 2)
		{
			Data::FrontElement *adj = adjs.front();

			if (adj == fe)
			{
				adj = adjs.back();
			}

			if (this->surface(adj))
			{
				continue;
			}

			surface->boundary->add(adj);

			for (UInt i = 0; i < adj->numVertices(); i++)
			{
				if (surface->boundary->have(adj->getVertex(i)))
				{
					continue;
				}

				surface->boundary->add(adj->getVertex(i));
			}

			topropagate.push_back(adj);
		}
		else
		{
			Opening op;

			op.v1 = v;
			op.v2 = NULL;

			if ((n) && (n->getId()))
			{
				op.v2 = n;

				if (op.v1 < op.v2)
				{
					op.v1 = n;
					op.v2 = v;
				}
			}

			std::pair<OpeningMap::iterator, bool> p = this->openings[surface->boundary].insert(std::make_pair(op, Data::FrontElementList()));

			(*p.first).second.push_back(fe);

			if ((!p.second) && ((*p.first).second.size() > 2))
			{
				this->manifold = false;
				surface->manifold = false;
			}
		}
	}

	for (Data::FrontElementList::iterator iter = topropagate.begin();
		 iter != topropagate.end(); iter++)
	{
		this->propagate((*iter), surface);
	}
}

bool Performer::InnerSurfacesIdentifier::smallerCoords(const Data::Vertex *v1, const Data::Vertex *v2, UInt surfaceDirection)
{
	switch (surfaceDirection)
	{
    case 0:
        return smallerCoords0(v1, v2);
    case 1:
        return smallerCoords1(v1, v2);
    case 2:
        return smallerCoords2(v1, v2);
	}

	return smallerCoordsAll(v1, v2);
}

bool Performer::InnerSurfacesIdentifier::smallerCoords0(const Data::Vertex *v1, const Data::Vertex *v2)
{
	UInt dimension = v1->getPoint()->dimension();
	const Real *coords1 = v1->getPoint()->getCoordsArray();
	const Real *coords2 = v2->getPoint()->getCoordsArray();

	for (UInt i = 1; i < dimension; i++)
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

bool Performer::InnerSurfacesIdentifier::smallerCoords1(const Data::Vertex *v1, const Data::Vertex *v2)
{
	UInt dimension = v1->getPoint()->dimension();
	const Real *coords1 = v1->getPoint()->getCoordsArray();
	const Real *coords2 = v2->getPoint()->getCoordsArray();

	for (UInt i = 1; i < dimension; i++)
	{
	    UInt j = (i+1)%dimension;

		if (coords1[j] < coords2[j])
		{
			return true;
		}

		if (coords1[j] > coords2[j])
		{
			return false;
		}
	}

	return false;
}

bool Performer::InnerSurfacesIdentifier::smallerCoords2(const Data::Vertex *v1, const Data::Vertex *v2)
{
	UInt dimension = v1->getPoint()->dimension();
	const Real *coords1 = v1->getPoint()->getCoordsArray();
	const Real *coords2 = v2->getPoint()->getCoordsArray();

	for (UInt i = 1; i < dimension; i++)
	{
	    UInt j = (i+2)%dimension;

		if (coords1[j] < coords2[j])
		{
			return true;
		}

		if (coords1[j] > coords2[j])
		{
			return false;
		}
	}

	return false;
}

bool Performer::InnerSurfacesIdentifier::smallerCoordsAll(const Data::Vertex *v1, const Data::Vertex *v2)
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

void Performer::InnerSurfacesIdentifier::sort(Data::VertexVector &verts, UInt surfaceDirection)
{
    switch (surfaceDirection)
	{
    case 0:
        std::sort(verts.begin(), verts.end(), smallerCoords0);
        break;
    case 1:
        std::sort(verts.begin(), verts.end(), smallerCoords1);
        break;
    case 2:
        std::sort(verts.begin(), verts.end(), smallerCoords2);
        break;
    default:
        std::sort(verts.begin(), verts.end(), smallerCoordsAll);
        break;
	}
}

Data::FrontElement *Performer::InnerSurfacesIdentifier::leftmost(const Data::Boundary *boundary, Data::Vertex *&leftmost) const
{
	leftmost = NULL;
	Data::Point *leftmostpt = NULL;

	for (Data::Boundary::ConstVertexIterator iter = boundary->vBegin(), end = boundary->vEnd();
		 iter != end; iter++)
	{
		Data::Vertex *v = (*iter);
		Data::Point *p = v->getPoint();

		if (!leftmost)
		{
			leftmost = v;
			leftmostpt = p;

			continue;
		}

		for (UInt i = 0; i < this->dimension; i++)
		{
		    UInt j = i;

		    if (this->surfaceDirection < this->dimension)
            {
                if (j == 0)
                {
                    continue;
                }

                j = (i + this->surfaceDirection)%this->dimension;
            }

			Real pcoord = p->getCoord(j);
			Real lcoord = leftmostpt->getCoord(j);

			if (pcoord > lcoord)
			{
				break;
			}

			if (pcoord < lcoord)
			{
				leftmost = v;
				leftmostpt = p;

				break;
			}
		}
	}

	if (!leftmost)
	{
		return NULL;
	}

	Data::FrontElementList fels = boundary->adjacency(leftmost);

	Data::FrontElement *fel = fels.front();

	fels.pop_front();

	Data::VertexVector verts = fel->getVertices();

	this->sort(verts, this->surfaceDirection);

	while (!fels.empty())
	{
		Data::FrontElement *other = fels.front();

		fels.pop_front();

		Data::VertexVector overts = other->getVertices();

		this->sort(overts, this->surfaceDirection);

		UInt nverts = static_cast<UInt>(std::min(verts.size(), overts.size()));

		for (UInt i = 0; i < nverts; i++)
		{
			if (smallerCoords(overts[i], verts[i], this->surfaceDirection))
			{
				verts = overts;
				fel = other;

				break;
			}
		}
	}

	return fel;
}

Performer::InnerSurfacesIdentifier::FESurf Performer::InnerSurfacesIdentifier::leftmost() const
{
	FESurf leftmost;

	leftmost.fel = NULL;
	leftmost.vertex = NULL;
	leftmost.surface = NULL;

	Data::VertexVector verts;

	for (SurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		if ((*iter)->position != Data::UNKNOWN)
		{
			continue;
		}

		Data::Vertex *vertex = NULL;
		Data::FrontElement *fel = NULL;

		FESurfMap::iterator found = this->leftmosts.find((*iter));

		if (found == this->leftmosts.end())
        {
            fel = this->leftmost((*iter)->boundary, vertex);

            FESurf fesurf;

            fesurf.fel = fel;
            fesurf.vertex = vertex;
            fesurf.surface = (*iter);

            this->leftmosts.insert(found, std::pair<Surface *, FESurf>((*iter), fesurf));
        }
		else
        {
            fel = (*found).second.fel;
            vertex = (*found).second.vertex;
        }

		Data::VertexVector fverts = fel->getVertices();

		this->sort(fverts, this->surfaceDirection);

		if (!leftmost.fel)
		{
			leftmost.fel = fel;
			leftmost.vertex = vertex;
			leftmost.surface = (*iter);

			verts = fverts;

			continue;
		}

		UInt nverts = static_cast<UInt>(std::min(verts.size(), fverts.size()));

		for (UInt i = 0; i < nverts; i++)
		{
			if (smallerCoords(fverts[i], verts[i], this->surfaceDirection))
			{
				leftmost.fel = fel;
				leftmost.vertex = vertex;
				leftmost.surface = (*iter);

				verts = fverts;

				break;
			}
		}
	}

	return leftmost;
}

bool Performer::InnerSurfacesIdentifier::isInner(const SurfaceTree *node, const Data::Point *point, SurfaceTree *&innermost) const
{
	if ((!node) || (node->children.empty()))
	{
		return false;
	}

	for (SurfaceTreeList::const_iterator iter = node->children.begin();
		 iter != node->children.end(); iter++)
	{
		SurfaceTree *n = (*iter);

		Data::Position pos = this->position(point, n->surface->boundary);

		if (pos == Data::IN)
		{
			innermost = n;

			return !this->isInner(n, point, innermost);
		}
	}

	return false;
}

bool Performer::InnerSurfacesIdentifier::shouldInvert2D(const Data::FrontElement *adj,
	const Data::FrontElement */*fel*/, const Data::Vertex *v) const
{
	return (adj->getVertex(1) == v);
}

bool Performer::InnerSurfacesIdentifier::shouldInvert3D(const Data::FrontElement *adj,
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

	return false;
}

void Performer::InnerSurfacesIdentifier::invert(Data::FrontElement *fel) const
{
	fel->invert();
}

void Performer::InnerSurfacesIdentifier::invertIfNecessary(Data::FrontElement *fel, bool inner) const
{
#if USE_EXPERIMENTAL_3
    struct PointingInside
    {
        static bool pointingInside(const UInt &dimension, const Data::Vector *v)
        {
            for (UInt i = 0; i < dimension; i++)
            {
                if (v->getCoord(i) > 0)
                {
                    return true;
                }

                if (v->getCoord(i) < 0)
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

        pointingInside = PointingInside::pointingInside(dimension, &n);
    }
    else if (this->surfaceDirection < this->dimension)
    {
        Data::Point2D p1, p2;

        for (UInt i = 1; i < this->dimension; i++)
        {
            UInt j = (i + this->surfaceDirection)%this->dimension;

            p1.setCoord(i - 1, static_cast<Data::Edge3D *>(fel->getShape())->getPoint(0)->getCoord(j));
            p2.setCoord(i - 1, static_cast<Data::Edge3D *>(fel->getShape())->getPoint(1)->getCoord(j));
        }

        Data::Edge2D e(&p1, &p2);

        Data::Vector2D n = e.normal();

        pointingInside = PointingInside::pointingInside(2, &n);

        e.setPoints(NULL, NULL);
    }
    else
    {
        Data::Vector3D n = static_cast<Data::Polygon *>(fel->getShape())->normal();

        pointingInside = PointingInside::pointingInside(dimension, &n);
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
}

bool Performer::InnerSurfacesIdentifier::findCompleteBoundary2D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested)
{
	if (this->tested.find(fel) != this->tested.end())
	{
		return false;
	}

	Data::Vertex *v = fel->getVertex(1);

	Data::FrontElementList adjs = surface->boundary->adjacency(v);

	if (adjs.size() != 2)
	{
		return false;
	}

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

	this->tested.insert(fel);

	if (this->shouldInvert2D(adj, fel, v))
	{
		this->invert(adj);

		//this->anyInverted = true;
	}

	toBeTested.push_back(adj);

	return true;
}

void Performer::InnerSurfacesIdentifier::findCompleteBoundary2D(Data::FrontElement *fel, Surface *surface)
{
	OpeningSet openings;

	while (fel)
	{
		Data::FrontElementList toBeTested;

		toBeTested.push_back(fel);

		fel = NULL;

		while (!toBeTested.empty())
		{
			Data::FrontElement *fel = toBeTested.front();
			//fel = toBeTested.front();

			toBeTested.pop_front();

			this->findCompleteBoundary2D(fel, surface, toBeTested);

			/*if (this->status.getType() == this->error.getType())
			{
				return;
			}*/
		}

		if (surface->position == Data::IN)
		{
			return;
		}

		BoundaryOpeningMap::iterator bit = this->openings.find(surface->boundary);

		if (bit == this->openings.end())
        {
            continue;
        }

		Data::FrontElement *adj = NULL;
		OpeningMap::iterator feiter = (*bit).second.end();
		UInt adji = 0;

		for (OpeningMap::iterator iter = (*bit).second.begin();
			 iter != (*bit).second.end();)
		{
			if ((*iter).second.size() > 1)
			{
#if USE_C__11
				iter = (*bit).second.erase(iter);
#else
				(*bit).second.erase(iter++);
#endif //#if USE_C__11

				continue;
			}

			Data::FrontElement *openFel = (*iter).second.front();

			UInt i = (openFel->getVertex(0) == (*iter).first.v1) ? 0 : 1;

			Data::FrontElementList adjs = this->mesh->findSortedAdjacentFrontElements(openFel, i, this->boundary);

			Data::FrontElement *openAdj = NULL;

			if (!adjs.empty())
			{
				if (adjs.front() == openFel)
				{
					adjs.pop_front();
				}

				if (i == 0)
				{
					openAdj = adjs.back();

					adjs.pop_back();

					bool allUnknown = true;
					Data::FrontElement *first = NULL;

					for (Data::FrontElementList::iterator aditer = adjs.begin();
						 aditer != adjs.end(); aditer++)
					{
						Data::FrontElement *ad = (*aditer);

						if (this->surface(ad)->position != Data::UNKNOWN)
						{
							allUnknown = false;
						}
						else
						{
							first = ad;
						}
					}

					if ((!allUnknown) && (first))
					{
						openAdj = first;
					}
				}
				else
				{
					openAdj = adjs.front();

					adjs.pop_front();

					bool allUnknown = true;
					Data::FrontElement *first = NULL;

					for (Data::FrontElementList::reverse_iterator aditer = adjs.rbegin();
						 aditer != adjs.rend(); aditer++)
					{
						Data::FrontElement *ad = (*aditer);

						if (this->surface(ad)->position != Data::UNKNOWN)
						{
							allUnknown = false;
						}
						else
						{
							first = ad;
						}
					}

					if ((!allUnknown) && (first))
					{
						openAdj = first;
					}
				}

				if (this->surface(openAdj)->position != Data::UNKNOWN)
				{
					openAdj = NULL;
				}
			}

			if (!openAdj)
			{
				surface->position = Data::IN;

				return;
			}

			if (!fel)
			{
				fel = openFel;
				adj = openAdj;
				adji = i;

				feiter = iter;
			}

			iter++;
		}

		if (fel)
		{
			if (this->shouldInvert2D(adj, fel, fel->getVertex(adji)))
			{
				this->invert(adj);
			}

			openings.insert((*feiter).first);

			Surface *adjSurf = this->surface(adj);

			for (Data::Boundary::VertexIterator viter = adjSurf->boundary->vBegin();
				 viter != adjSurf->boundary->vEnd(); viter = adjSurf->boundary->erase(viter))
			{
				if (!surface->boundary->have((*viter)))
				{
					surface->boundary->add((*viter));
				}
			}

			for (Data::Boundary::FrontElementIterator fiter = adjSurf->boundary->eBegin();
				 fiter != adjSurf->boundary->eEnd(); fiter = adjSurf->boundary->erase(fiter))
			{
				surface->boundary->add((*fiter));
			}

			BoundaryOpeningMap::iterator adjbit = this->openings.find(adjSurf->boundary);

			while (!(*adjbit).second.empty())
			{
				OpeningMap::iterator iter2 = (*adjbit).second.begin();

				if ((*iter2).first.v1 == (*feiter).first.v1)
				{
					(*adjbit).second.erase(iter2);

					continue;
				}

				std::pair<OpeningMap::iterator, bool> p = (*bit).second.insert((*iter2));

				if (!p.second)
				{
					(*p.first).second.splice((*p.first).second.end(), (*iter2).second);

					openings.insert((*iter2).first);
				}

				(*adjbit).second.erase(iter2);
			}

			this->surfaces.remove(adjSurf);
			this->openings.erase(adjbit);

			delete adjSurf;

			fel = adj;

			(*bit).second.erase(feiter);
		}
	}

	this->openings.erase(surface->boundary);

	while (!openings.empty())
	{
		Opening op = *openings.begin();

		openings.erase(openings.begin());

		this->setInSurface(op);
	}
}

bool Performer::InnerSurfacesIdentifier::findCompleteBoundary3D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested)
{
	if (this->tested.find(fel) != this->tested.end())
	{
		return false;
	}

	this->tested.insert(fel);

	UInt numVerts = fel->numVertices();

	for (UInt i = 0; i < numVerts; i++)
	{
		Data::Vertex *v = fel->getVertex(i);
		Data::Vertex *o = fel->getVertex((i + 1)%numVerts);

		Data::FrontElement *adj = NULL;

		Data::FrontElementList adjs = surface->boundary->adjacency(v);

		for (Data::FrontElementList::iterator iter = adjs.begin();
			 iter != adjs.end(); iter++)
		{
			if ((*iter) == fel)
			{
				continue;
			}

			if ((*iter)->have(o))
			{
				adj = (*iter);

				break;
			}
		}

		if (!adj)
		{
			continue;
		}

		if (this->shouldInvert3D(adj, fel, v, o))
		{
			this->invert(adj);

			//this->anyInverted = true;
		}

		toBeTested.push_back(adj);
	}

	return true;
}

void Performer::InnerSurfacesIdentifier::findCompleteBoundary3D(Data::FrontElement *fel, Surface *surface)
{
	OpeningSet openings;

	while (fel)
	{
		Data::FrontElementList toBeTested;

		toBeTested.push_back(fel);

		fel = NULL;

		while (!toBeTested.empty())
		{
			Data::FrontElement *fel = toBeTested.front();

			toBeTested.pop_front();

			this->findCompleteBoundary3D(fel, surface, toBeTested);

			/*if (this->status.getType() == this->error.getType())
			{
				return;
			}*/
		}

		if (surface->position == Data::IN)
		{
			return;
		}

		if (surface->position == Data::UNKNOWN)
        {
            return;
        }

		BoundaryOpeningMap::iterator bit = this->openings.find(surface->boundary);

		Data::FrontElement *adj = NULL;
		OpeningMap::iterator feiter = (*bit).second.end();
		UInt adji = 0;

		for (OpeningMap::iterator iter = (*bit).second.begin();
			 iter != (*bit).second.end();)
		{
			if ((*iter).second.size() > 1)
			{
#if USE_C__11
				iter = (*bit).second.erase(iter);
#else
				(*bit).second.erase(iter++);
#endif //#if USE_C__11

				continue;
			}

			Data::FrontElement *openFel = (*iter).second.front();

			UInt i = openFel->numVertices();

			for (i = 0; i < openFel->numVertices(); i++)
			{
				if (((openFel->getVertex(i) == (*iter).first.v1) && (openFel->getVertex((i+1)%openFel->numVertices()) == (*iter).first.v2)) ||
					((openFel->getVertex(i) == (*iter).first.v2) && (openFel->getVertex((i+1)%openFel->numVertices()) == (*iter).first.v1)))
				{
					break;
				}
			}

			/*if (i == openFel->numVertices())
			{
				continue;
			}*/

			Data::FrontElementList adjs = this->mesh->findSortedAdjacentFrontElements(openFel, i, this->boundary);

			Data::FrontElement *openAdj = NULL;

			if (!adjs.empty())
			{
				if (adjs.front() == openFel)
				{
					adjs.pop_front();
				}

				openAdj = adjs.back();

				adjs.pop_back();

				bool allUnknown = true;
				Data::FrontElement *first = NULL;

				for (Data::FrontElementList::iterator aditer = adjs.begin();
					 aditer != adjs.end(); aditer++)
				{
					Data::FrontElement *ad = (*aditer);

					if (this->surface(ad)->position != Data::UNKNOWN)
					{
						allUnknown = false;
					}
					else
					{
						first = ad;
					}
				}

				if ((!allUnknown) && (first))
				{
					openAdj = first;
				}

				if (this->surface(openAdj)->position != Data::UNKNOWN)
				{
					openAdj = NULL;
				}
			}

			if (!openAdj)
			{
				surface->position = Data::IN;

				return;
			}

			if (!fel)
			{
				fel = openFel;
				adj = openAdj;
				adji = i;

				feiter = iter;
			}

			iter++;
		}

		if (fel)
		{
			if (this->shouldInvert3D(adj, fel, fel->getVertex(adji), fel->getVertex((adji + 1)%fel->numVertices())))
			{
				this->invert(adj);
			}

			openings.insert((*feiter).first);

			Surface *adjSurf = this->surface(adj);

			for (Data::Boundary::VertexIterator viter = adjSurf->boundary->vBegin();
				 viter != adjSurf->boundary->vEnd(); viter = adjSurf->boundary->erase(viter))
			{
				if (!surface->boundary->have((*viter)))
				{
					surface->boundary->add((*viter));
				}
			}

			for (Data::Boundary::FrontElementIterator fiter = adjSurf->boundary->eBegin();
				 fiter != adjSurf->boundary->eEnd(); fiter = adjSurf->boundary->erase(fiter))
			{
				surface->boundary->add((*fiter));
			}

			BoundaryOpeningMap::iterator adjbit = this->openings.find(adjSurf->boundary);

			while (!(*adjbit).second.empty())
			{
				OpeningMap::iterator iter2 = (*adjbit).second.begin();

				if (((*iter2).first.v1 == (*feiter).first.v1) &&
					((*iter2).first.v2 == (*feiter).first.v2))
				{
					(*adjbit).second.erase(iter2);

					continue;
				}

				std::pair<OpeningMap::iterator, bool> p = (*bit).second.insert((*iter2));

				if (!p.second)
				{
					(*p.first).second.splice((*p.first).second.end(), (*iter2).second);

					openings.insert((*iter2).first);
				}

				(*adjbit).second.erase(iter2);
			}

			this->surfaces.remove(adjSurf);
			this->openings.erase(adjbit);

			delete adjSurf;

			fel = adj;

			(*bit).second.erase(feiter);
		}
	}

	this->openings.erase(surface->boundary);

	while (!openings.empty())
	{
		Opening op = *openings.begin();

		openings.erase(openings.begin());

		this->setInSurface(op);
	}
}

void Performer::InnerSurfacesIdentifier::findCompleteBoundary(Data::FrontElement *fel, Surface *surface)
{
	if ((this->dimension == 2) || (this->surfaceDirection < this->dimension))
	{
		this->findCompleteBoundary2D(fel, surface);
	}
	else
	{
		this->findCompleteBoundary3D(fel, surface);
	}
}

void Performer::InnerSurfacesIdentifier::findCompleteBoundary(FESurf leftmost)
{
    if (!this->root)
	{
		this->root = new SurfaceTree();

		this->root->surface = NULL;
	}

	bool inner = false;

	SurfaceTree *innermost = this->root;

	if ((leftmost.surface->manifold) && (!this->root->children.empty()))
	{
		inner = this->isInner(root, leftmost.vertex->getPoint(), innermost);
	}

	SurfaceTree *current = new SurfaceTree();

	innermost->children.push_back(current);

	current->surface = leftmost.surface;
	current->surface->position = leftmost.surface->manifold ? Data::OUT : Data::UNKNOWN;

	if (current->surface->position != Data::UNKNOWN)
    {
        this->invertIfNecessary(leftmost.fel, inner);
    }

	this->findCompleteBoundary(leftmost.fel, current->surface);
}

void Performer::InnerSurfacesIdentifier::findCompleteBoundaries()
{
#if USE_C__11
	this->tested.reserve(this->boundary->size());
#endif //#if USE_C__11

	for (FESurf leftmost = this->leftmost(); leftmost.fel; leftmost = this->leftmost())
	{
		this->findCompleteBoundary(leftmost);
	}
}

void Performer::InnerSurfacesIdentifier::setInSurface(Opening op)
{
	for (BoundaryOpeningMap::iterator iter = this->openings.begin();
		 iter != this->openings.end(); iter++)
	{
		OpeningMap::iterator it = (*iter).second.find(op);

		if (it == (*iter).second.end())
		{
			continue;
		}

		Data::FrontElement *fel = (*it).second.front();

		Surface *surface = this->surface(fel);

		surface->position = Data::IN;

		this->findCompleteBoundary(fel, surface);
	}
}

void Performer::InnerSurfacesIdentifier::clear(SurfaceTree *node)
{
	for (SurfaceTreeList::iterator iter = node->children.begin();
		 iter != node->children.end(); iter++)
	{
		this->clear((*iter));
	}

	//node.children.clear();
	//node.fels.clear();

	delete node;
}

void Performer::InnerSurfacesIdentifier::clearTree()
{
	this->clear(this->root);

	this->root = NULL;
    this->needTree = false;
}

Performer::InnerSurfacesIdentifier::Surface::~Surface()
{
	this->boundary->clear();

	delete this->boundary;
}
