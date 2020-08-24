#include "MJMesh/MeshBackTracker.h"

#include "MJMesh/Element.h"
#include "MJMesh/Front.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/BackTrackingFrontAdvancer.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"

MJMesh::MeshBackTracker::MeshBackTracker(UInt dimension, Data::Mesh *mesh,
	Data::Boundary *boundary, Data::BoxTree *tree, Data::Front *front,
	bool meshSorted, UInt maxBackTracks) :
	Performer::MeshBackTracker(mesh, boundary, front, meshSorted)
{
	this->setDimension(dimension);
	this->setMaxBackTracks(maxBackTracks);
	this->setTree(tree);

	this->btfa = NULL;

#if USE_DELETE_FELS
    this->created = NULL;
#endif //#if USE_DELETE_FELS
}

MJMesh::MeshBackTracker::~MeshBackTracker()
{
#if USE_DELETE_FELS
    if (this->created)
    {
        delete this->created;
    }
#endif //#if USE_DELETE_FELS
}

void MJMesh::MeshBackTracker::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void MJMesh::MeshBackTracker::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

void MJMesh::MeshBackTracker::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}

void MJMesh::MeshBackTracker::execute()
{
	switch (this->status.getType())
	{
	case MeshBackTracker::NOT_INIT:
		if (!this->mesh)
		{
			this->status = MeshBackTracker::ended;

			return;
		}

		this->elements = this->sort();

		this->status = MeshBackTracker::init;

	case MeshBackTracker::INIT:
		while ((this->btfa) || (!this->elements.empty()))
		{
			if (!this->btfa)
			{
				Data::Element *e = this->elements.front();

				/*if (e->getId() == 8231)
				{
                    std::cout << "debug e->id = " << e->getId() << std::endl;
                }*/

				this->elements.pop_front();

				if ((!this->meshSorted) && (!e->isBad()))
				{
					continue;
				}

				//debug
				//std::cout << "now making front for element " << e->text() << " in MeshBackTracker" << std::endl;
				//endebug

#if USE_JMSH_DEBUG_MESSAGES
				std::cout << "markos debug now making front for element " << e->getId() << std::endl;

				if (this->dimension == 3)
				{
					std::cout << "markos debug, back tracking, elem = " << e->getId() - 1 << " will be improved by back tracking" << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES

				Data::ElementSet oldElements;
				Data::FrontElementSet oldFrontElements;
				Data::VertexSet oldVertices;

				this->front = this->makeFront(e, oldElements, oldFrontElements, oldVertices);

				if (!this->front)
				{
					//debug
					//std::cout << "front not created" << std::endl;
					//endebug

#if USE_DELETE_FELS
                    //std::cout << "will delete " << this->created->size() << " created fels" << std::endl;

                    while (!this->created->empty())
                    {
                        Data::FrontElement *fel = this->created->front();

                        this->created->pop_front();

                        this->mesh->remove(fel);

                        delete fel;
                    }
#endif //#if USE_DELETE_FELS

					continue;
				}

				//debug
				//std::cout << "front->size = " << this->front->size() << std::endl;
				//std::cout << "front->vsize = " << this->front->verticesSize() << std::endl;
				//endebug

				/*std::stringstream str;
				str << "element " << e->getId() << " generated this->front = " << this->front << std::endl;
				std::cout << str.str();*/

#if USE_JMSH_DEBUG_MESSAGES
				//markos debug
				if (this->dimension == 3)
				{
					for (Data::ElementSet::iterator iter = oldElements.begin();
						 iter != oldElements.end(); iter++)
					{
						std::cout << "markos debug, old element " << (*iter)->getId() - 1 << ", (" << (*iter)->getVertex(0)->getId() - 1 << ", "
								  << (*iter)->getVertex(1)->getId() - 1 << ", " << (*iter)->getVertex(2)->getId() - 1 << ", "
								  << (*iter)->getVertex(3)->getId() - 1 << ")" << std::endl;
					}

					std::cout << "markos debug, back tracking, elem = " << e->getId() - 1 << " will be improved by back tracking" << std::endl;
				}
				//end markos debug
#endif //#if USE_JMSH_DEBUG_MESSAGES

				this->btfa = this->makeBackTrackingFrontAdvancer();

				/*this->btfa = new MJMesh::BackTrackingFrontAdvancer(this->dimension,
					this->boundary, this->mesh,
					this->front, false, false, this->maxBackTracks);*/

				this->btfa->setIdManager(this->idManager);

				this->btfa->setOldElements(oldElements);
				this->btfa->setOldFrontElements(oldFrontElements);
				this->btfa->setOldVertices(oldVertices);
			}

#if USE_DELETE_FELS
            this->btfa->setCreated(this->created);
#endif //#if USE_DELETE_FELS

			this->btfa->execute();

			if ((this->btfa->getStatus().getType() == MJMesh::BackTrackingFrontAdvancer::ended.getType()) ||
				(this->btfa->getStatus().getType() == MJMesh::BackTrackingFrontAdvancer::rejected.getType()))
			{
#if USE_DELETE_FELS
                this->front->clear();
#endif //#if USE_DELETE_FELS

				Data::ElementSet oldElements = this->btfa->getOldElements();

				if ((this->btfa->getStatus().getType() == MJMesh::BackTrackingFrontAdvancer::ended.getType()) &&
					(this->betterMesh(this->btfa->getNewElements(), oldElements)))
				{
#if USE_JMSH_DEBUG_MESSAGES
					if (this->dimension == 3)
					{
						std::cout << "markos debug, improvement back tracking worked" << std::endl;
					}
#endif //#if USE_JMSH_DEBUG_MESSAGES

					while (!oldElements.empty())
					{
						Data::Element *e = *oldElements.begin();
						oldElements.erase(oldElements.begin());

						this->elements.remove(e);

						//debug
						/*Data::FrontElementList fels;

						if (this->boundary->have(e, this->mesh, &fels))
						{
							std::cout << "element " << e->getId() << " " << e << " should not be deleted" << std::endl;
						}*/
						//endebug
					}

					//debug just for testing
					/*Data::FrontElementSet fels = this->btfa->getOldFrontElements();

					while (!fels.empty())
					{
						Data::FrontElement *fel = *fels.begin();
						fels.erase(fels.begin());

						if (this->boundary->have(fel))
						{
							std::cout << "fel " << fel->getId() << " " << fel << " should not be deleted" << std::endl;
						}

						{
							std::cout << "found fel " << fel->getId() << " " << fel << std::endl;
						}
					}*/
					//endebug

					//std::cout << "delete old mesh, " << this->front->size() << ", " << this->front->verticesSize() << std::endl;

					this->btfa->deleteOldMesh();
				}
				else
				{
#if USE_JMSH_DEBUG_MESSAGES
					if (this->dimension == 3)
					{
						if (this->btfa->getStatus().getType() == MJMesh::BackTrackingFrontAdvancer::rejected.getType())
						{
							std::cout << "markos debug, improvment back tracking failed, tried to create too many elements" << std::endl;
						}
						else
						{
							std::cout << "markos debug, improvment back tracking failed, created worse mesh" << std::endl;
						}
					}
#endif //#if USE_JMSH_DEBUG_MESSAGES

					//std::cout << "restore old mesh, " << this->front->size() << ", " << this->front->verticesSize() << std::endl;

					this->btfa->restoreOldMesh();
				}

#if USE_DELETE_FELS
                this->btfa->setCreated(NULL);
#endif //#if USE_DELETE_FELS

				this->clearBtfa();

				/*std::stringstream str;
				str << "deleting this->front = " << this->front << std::endl;
				std::cout << str.str();*/

				delete this->front;

				this->front = NULL;
			}

#if USE_GUI
			if (Data::isStepByStep()) break;
#endif //#if USE_GUI
		}

		if ((!this->btfa) && (this->elements.empty()))
		{
			this->status = MeshBackTracker::ended;
		}

	case MeshBackTracker::ENDED:
		break;
	}
}

MJMesh::BackTrackingFrontAdvancer *MJMesh::MeshBackTracker::makeBackTrackingFrontAdvancer() const
{
	return new MJMesh::BackTrackingFrontAdvancer(this->dimension,
		static_cast<MJMesh::Boundary *>(this->boundary), this->tree,
		static_cast<MJMesh::Mesh *>(this->mesh),
		static_cast<MJMesh::Front *>(this->front), false, false, this->maxBackTracks,
        MJMesh::BackTrackingFrontAdvancer::OPTIMIZATION);
}

void MJMesh::MeshBackTracker::clearBtfa()
{
	delete this->btfa;

	this->btfa = NULL;
}

void MJMesh::MeshBackTracker::invert(Data::FrontElement *fel, Data::Element *e)
{
	Performer::MeshBackTracker::invert(fel, e);

	//((MJMesh::FrontElement *)fel)->remove((MJMesh::Element *)e);
}

void MJMesh::MeshBackTracker::markAsOld(Data::VertexSet &old, Data::Vertex *v, Data::Mesh *mesh)
{
	std::pair<Data::VertexSet::iterator, bool> p = old.insert(v);

	if (p.second == true)
	{
		mesh->remove(v);
	}
}

void MJMesh::MeshBackTracker::markAsOld(Data::FrontElementSet &old, Data::FrontElement *e, Data::Mesh *mesh)
{
	std::pair<Data::FrontElementSet::iterator, bool> p = old.insert(e);

	if (p.second == true)
	{
		mesh->remove(e);

		/*Data::VertexVector vertices = e->getVertices();

		while (!vertices.empty())
		{
			MJMesh::Vertex *v = (MJMesh::Vertex *)vertices.back();
			vertices.pop_back();

			v->remove(e);
		}*/
	}
}

void MJMesh::MeshBackTracker::markAsOld(Data::ElementSet &old, Data::Element *e, Data::Mesh *mesh)
{
	std::pair<Data::ElementSet::iterator, bool> p = old.insert(e);

	if (p.second == true)
	{
		mesh->remove(e);

		/*Data::VertexVector vertices = e->getVertices();

		while (!vertices.empty())
		{
			MJMesh::Vertex *v = (MJMesh::Vertex *)vertices.back();
			vertices.pop_back();

			v->remove(e);
		}*/
	}
}

/*#if USE_DELETE_FELS
bool MJMesh::MeshBackTracker::process(Data::FrontElementList &created,
    Data::Element *e, const Data::Mesh *mesh,
	const Data::Boundary *boundary, Data::FrontElementSet &newFront, bool ignoreBoundary)
#else*/
bool MJMesh::MeshBackTracker::process(Data::Element *e, const Data::Mesh *mesh,
	const Data::Boundary *boundary, Data::FrontElementSet &newFront, bool ignoreBoundary)
/*#endif //#if USE_DELETE_FELS*/
{
	Data::FrontElementList fels;

/*#if USE_DELETE_FELS
    for (UInt i = 0; i < e->numVertices(); i++)
    {
        static_cast<MJMesh::Mesh *>(const_cast<Data::Mesh *>(mesh))->createAdjacentFrontElements(e->getVertex(i), created, true);
    }
#endif //#if USE_DELETE_FELS*/

	if ((boundary->have(e, mesh, &fels)) && (!ignoreBoundary))
	{
		return false;
	}

	for (Data::FrontElementList::iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		newFront.insert((*iter));

		MJMesh::MeshBackTracker::invert((*iter), e);
	}

	return true;
}

void MJMesh::MeshBackTracker::process(Data::Element *e, Data::FrontElementList &fels,
	Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
	const Data::FrontElementList &connectors, Data::Mesh *mesh)
{
	for (Data::FrontElementList::iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		/*bool connects = false;

		for (Data::FrontElementList::const_iterator iter2 = connectors.begin();
			 iter2 != connectors.end(); iter2++)
		{
			if ((*iter) == (*iter2))
			{
				connects = true;

				MJMesh::MeshBackTracker::markAsOld(oldFrontElements, (*iter), mesh);

				break;
			}
		}

		if (connects)
		{
			continue;
		}*/

		if (!static_cast<MJMesh::FrontElement *>((*iter))->haveAdjacent(static_cast<MJMesh::Element *>(e)))
        {
            continue;
        }

		if (std::find(connectors.begin(), connectors.end(), (*iter)) != connectors.end())
		{
			MJMesh::MeshBackTracker::markAsOld(oldFrontElements, (*iter), mesh);

			continue;
		}

		std::pair<Data::FrontElementSet::iterator, bool> p = newFront.insert((*iter));

		if (p.second)
		{
			MJMesh::MeshBackTracker::invert((*iter), e);
		}
		else
		{
			newFront.erase(p.first);

			MJMesh::MeshBackTracker::markAsOld(oldFrontElements, (*iter), mesh);
		}
	}
}

/*#if USE_DELETE_FELS
void MJMesh::MeshBackTracker::process(Data::FrontElementList &created,
    Data::Element *e, Data::FrontElementList &fels,
	Data::ElementSet &oldElements, Data::FrontElementSet &newFront,
	Data::FrontElementSet &oldFrontElements,
	const Data::FrontElementList &connectors, Data::Mesh *mesh)
#else*/
void MJMesh::MeshBackTracker::process(Data::Element *e, Data::FrontElementList &fels,
	Data::ElementSet &oldElements, Data::FrontElementSet &newFront,
	Data::FrontElementSet &oldFrontElements,
	const Data::FrontElementList &connectors, Data::Mesh *mesh)
/*#endif //#if USE_DELETE_FELS*/
{
	MJMesh::MeshBackTracker::markAsOld(oldElements, e, mesh);

/*#if USE_DELETE_FELS
    for (UInt i = 0; i < e->numVertices(); i++)
    {
        static_cast<MJMesh::Mesh *>(const_cast<Data::Mesh *>(mesh))->createAdjacentFrontElements(e->getVertex(i), created, true);
    }
#endif //#if USE_DELETE_FELS*/

	MJMesh::MeshBackTracker::process(e, fels, newFront, oldFrontElements, connectors, mesh);
}

/*#if USE_DELETE_FELS
bool MJMesh::MeshBackTracker::process(Data::FrontElementList &created,
    Data::Element *e,
	Data::Mesh *mesh, const Data::Boundary *boundary,
	Data::ElementSet &oldElements,
	Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
	Data::FrontElementList &connectors, bool ignoreBoundary)
#else*/
bool MJMesh::MeshBackTracker::process(Data::Element *e,
	Data::Mesh *mesh, const Data::Boundary *boundary,
	Data::ElementSet &oldElements,
	Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
	Data::FrontElementList &connectors, bool ignoreBoundary)
/*#endif //#if USE_DELETE_FELS*/
{
	Data::FrontElementList fels;

/*#if USE_DELETE_FELS
    for (UInt i = 0; i < e->numVertices(); i++)
    {
        static_cast<MJMesh::Mesh *>(const_cast<Data::Mesh *>(mesh))->createAdjacentFrontElements(e->getVertex(i), created, true);
    }
#endif //#if USE_DELETE_FELS*/

	if ((boundary->have(e, mesh, &fels)) && (!ignoreBoundary))
	{
		return false;
	}

/*#if USE_DELETE_FELS
	MJMesh::MeshBackTracker::process(created, e, fels, oldElements, newFront, oldFrontElements, connectors, mesh);
#else*/
	MJMesh::MeshBackTracker::process(e, fels, oldElements, newFront, oldFrontElements, connectors, mesh);
/*#endif //#if USE_DELETE_FELS*/

	return true;
}

/*#if USE_DELETE_FELS
bool MJMesh::MeshBackTracker::process(Data::FrontElementList &created,
    Data::Element *e,
	Data::Mesh *mesh, const Data::Boundary *boundary,
	Data::ElementSet &oldElements,
	Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
	Data::FrontElement *connector, bool ignoreBoundary)
#else*/
bool MJMesh::MeshBackTracker::process(Data::Element *e,
	Data::Mesh *mesh, const Data::Boundary *boundary,
	Data::ElementSet &oldElements,
	Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
	Data::FrontElement *connector, bool ignoreBoundary)
/*#endif //#if USE_DELETE_FELS*/
{
	Data::FrontElementList fels;

/*#if USE_DELETE_FELS
    for (UInt i = 0; i < e->numVertices(); i++)
    {
        static_cast<MJMesh::Mesh *>(const_cast<Data::Mesh *>(mesh))->createAdjacentFrontElements(e->getVertex(i), created, true);
    }
#endif //#if USE_DELETE_FELS*/

	if ((boundary->have(e, mesh, &fels)) && (!ignoreBoundary))
	{
		return false;
	}

	Data::FrontElementList connectors;

	if (connector)
	{
		connectors.push_back(connector);
	}

/*#if USE_DELETE_FELS
	MJMesh::MeshBackTracker::process(created, e, fels, oldElements, newFront, oldFrontElements, connectors, mesh);
#else*/
	MJMesh::MeshBackTracker::process(e, fels, oldElements, newFront, oldFrontElements, connectors, mesh);
/*#endif //#if USE_DELETE_FELS*/

	return true;
}

Data::Front *MJMesh::MeshBackTracker::makeFront(Data::Element *e,
	Data::ElementSet &oldElements,
	Data::FrontElementSet &oldFrontElements,
	Data::VertexSet &oldVertices) const
{
	/*bool debug = (e->getId() == 8231);

	if (debug)
    {
        std::stringstream str;
        str << "e = " << e->text() << std::endl;
        std::cout << str.str();
    }*/

#if USE_DELETE_FELS
    if (!this->created)
    {
        this->created = new Data::FrontElementList();
    }

    for (UInt i = 0; i < e->numVertices(); i++)
    {
        static_cast<MJMesh::Mesh *>(const_cast<Data::Mesh *>(this->mesh))->createAdjacentFrontElements(e->getVertex(i), *this->created, true, this->boundary);
        /*UInt debugc = static_cast<MJMesh::Mesh *>(const_cast<Data::Mesh *>(this->mesh))->createAdjacentFrontElements(e->getVertex(i), *this->created, true);

        if (debug)
        {
            std::stringstream str;
            str << "vertex " << e->getVertex(i)->getId() <<
                ", inboundary = " << std::boolalpha << this->boundary->have(e->getVertex(i)) <<
                ", created " << debugc << " of " << static_cast<MJMesh::Vertex *>(e->getVertex(i))->numAdjacentFrontElements() << " fels" << std::endl;
            std::cout << str.str();
        }*/
    }
#endif //#if USE_DELETE_FELS

	static const bool ignoreBoundary = true;

	Data::FrontElementSet newFront;

	//std::cout << "trying to make front for element " << e->text() << std::endl;

/*#if USE_DELETE_FELS
	if (!MJMesh::MeshBackTracker::process(*this->created, e, this->mesh, this->boundary, newFront))
#else*/
	if (!MJMesh::MeshBackTracker::process(e, this->mesh, this->boundary, newFront))
/*#endif //#if USE_DELETE_FELS*/
	{
		//std::cout << "could NOT process element " << e->text() << std::endl;

		return NULL;
	}

	//std::cout << "could process element " << e->text() << std::endl;

	Data::FrontElementList fels(newFront.begin(), newFront.end());

	newFront.clear();

#if USE_JMSH_DEBUG_MESSAGES
	if (this->dimension == 3)
	{
		std::cout << "markos debug makefront fels.size = " << fels.size() << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	Data::ElementList elements;

	for (Data::FrontElementList::iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		//std::cout << "debug makefront fel = " << fel->text() << std::endl;

		//Data::ElementList adjEls = this->mesh->adjacency(fel);

		//std::cout << "debug makefront fel.adjacency.size = " << adjEls.size() << std::endl;

		//debug
		//if (adjEls.size() != 2)
		if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() != 2)
		{
			//if ((ignoreBoundary) && (this->boundary->have(fel)) && (adjEls.size() == 1))
			if ((ignoreBoundary) && (this->boundary->have(fel)) && (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 1))
			{
				elements.push_back(NULL);

				continue;
			}

			//TODO Gambiarra, ajeitar isso depois.
			/*
			std::stringstream str;
			str << "ERROR: adjEls.size = " << adjEls.size() << " for fel " << fel->getId() << std::endl;
			str << "ERROR: adjEls.size = " << static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() << " for fel " << fel->getId() << std::endl;
			str << "ERROR: adjEls.size = " << static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() << " for fel " << fel->text() << std::endl;
			str << "this->boundary->have(fel) = " << std::boolalpha << this->boundary->have(fel) << std::endl;
			bool found = false;
			for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
                 iter != this->boundary->eEnd(); iter++)
            {
                if ((*iter)->getId() == fel->getId())
                {
                    found = true;

                    break;
                }
            }
			str << "this->boundary->haveFel(fel->getId()) = " << std::boolalpha << found << std::endl;
			std::cout << str.str();*/

			return NULL;
		}
		//endebug

		//debug
		/*if (this->boundary->have(fel))
		{
			std::cout << "fel " << fel->getId() << " in boundary" << std::endl;
			std::cout << "e " << e->getId() << " in boundary" << std::endl;
		}*/
		//endebug

		//debug
		//std::cout << "1st adjacent = " << adjEls.front()->text() << std::endl;
		//std::cout << "2nd adjacent = " << adjEls.back()->text() << std::endl;
		//endebug

		//if (adjEls.front() == e)
		if ((*static_cast<MJMesh::FrontElement *>(fel)->eBegin()) == e)
		{
			//elements.push_back(adjEls.back());
			elements.push_back((*(++static_cast<MJMesh::FrontElement *>(fel)->eBegin())));
		}
		else
		{
			//elements.push_back(adjEls.front());
			elements.push_back((*static_cast<MJMesh::FrontElement *>(fel)->eBegin()));
		}

		//cout << "debug makefront added = " << elements.back()->getId() << endl;
	}

	UInt count = 0;

	while (!elements.empty())
	{
		Data::Element *adj = elements.front();
		elements.pop_front();

		Data::FrontElement *fel = fels.front();
		fels.pop_front();

#if USE_DELETE_FELS
        if (adj)
        {
            /*if (debug)
            {
                std::stringstream str;
                str << "adj = " << adj->text() << std::endl;
                std::cout << str.str();
            }*/

            for (UInt i = 0; i < adj->numVertices(); i++)
            {
                static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(adj->getVertex(i), *this->created, true, this->boundary);
                /*UInt debugc = static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(adj->getVertex(i), *this->created, true);

                if (debug)
                {
                    std::stringstream str;
                    str << "vertex " << adj->getVertex(i)->getId() <<
                        ", inboundary = " << std::boolalpha << this->boundary->have(adj->getVertex(i)) <<
                        ", created " << debugc << " of " << static_cast<MJMesh::Vertex *>(adj->getVertex(i))->numAdjacentFrontElements() << " fels" << std::endl;
                    std::cout << str.str();
                }*/
            }
        }
#endif //#if USE_DELETE_FELS

		//std::cout << "fel = " << fel->text() << ", adj = " << (adj ? adj->text() : std::string(" no adj")) << std::endl;

		if ((adj) &&
/*#if USE_DELETE_FELS
			(MJMesh::MeshBackTracker::process(*this->created, adj, this->mesh, this->boundary, oldElements, newFront, oldFrontElements, fel, ignoreBoundary)))
#else*/
			(MJMesh::MeshBackTracker::process(adj, this->mesh, this->boundary, oldElements, newFront, oldFrontElements, fel, ignoreBoundary)))
/*#endif //#if USE_DELETE_FELS*/
		{
			//std::cout << "processed" << std::endl;

#if USE_JMSH_DEBUG_MESSAGES
			if (this->dimension == 3)
			{
				std::cout << "markos debug makefront adj = " << adj->getId() - 1 << " is being deleted" << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

#if USE_DELETE_FELS
            if (count == 0)
            {
                MJMesh::MeshBackTracker::markAsOld(oldElements, e, this->mesh);
            }
#endif //#if USE_DELETE_FELS

			count++;
		}
		else
		{
			//std::cout << "did not process" << std::endl;

#if USE_JMSH_DEBUG_MESSAGES
			if (this->dimension == 3)
			{
				std::cout << "markos debug makefront adj = " << adj->getId() - 1 << " is in boundary" << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			newFront.insert(fel);
		}
	}

	if (count == 0)
	{
		//std::cout << "did not process any fel" << std::endl;

		return NULL;
	}

#if (!USE_DELETE_FELS)
	MJMesh::MeshBackTracker::markAsOld(oldElements, e, this->mesh);
#endif //#if (!USE_DELETE_FELS)

	//std::cout << "element " << e->text() << " marked as old" << std::endl;

	//Data::VertexVector vertices = e->getVertices();

	//while (!vertices.empty())
	for (UInt i = 0; i < e->numVertices(); i++)
	{
		//Data::Vertex *v = vertices.back();
		Data::Vertex *v = e->getVertex(i);
		//vertices.pop_back();

		//if (this->mesh->adjacency(v).empty())
		//if (this->mesh->findAdjacentFrontElements(v).empty())
		if (static_cast<MJMesh::Vertex *>(v)->numAdjacentFrontElements() == 0)
		{
			MJMesh::MeshBackTracker::markAsOld(oldVertices, v, this->mesh);

			//std::cout << "vertex " << v->text() << " marked as old" << std::endl;
		}
		/*else
		{
			std::cout << "vertex " << v->text() << " not marked as old" << std::endl;
		}*/
	}

	MJMesh::Front *front = new MJMesh::Front(false);

	while (!newFront.empty())
	{
		Data::FrontElement *e = *newFront.begin();
		newFront.erase(newFront.begin());

		front->add(e);

		//std::cout << "adding to front fel " << e->text() << std::endl;
		//Data::VertexVector vertices = e->getVertices();

		//while (!vertices.empty())
		for (UInt i = 0; i < e->numVertices(); i++)
		{
			//Data::Vertex *v = vertices.back();
			Data::Vertex *v = e->getVertex(i);
			//vertices.pop_back();

			/*//debug
			if ((front->verticesSize() < e->numVertices()) && (front->have(v)))
			{
				std::cout << "ERROR" << std::endl;
			}
			//endebug*/

			if (!front->have(v))
			{
				//std::cout << "adding to front vertex " << v->text() << std::endl;
				front->add(v);
			}
			/*else
			{
				std::cout << "not adding to front vertex " << v->text() << std::endl;
			}*/
		}
	}

#if USE_JMSH_BACK_TRACKING
	///debug para ficar igual ao jmsh:
	///  1. Identifica o elemento ruim (sliver)
	///  2. Identifica os vertices do sliver e os vertices dos elementos
	///     adjacentes por front element (aresta (2D) ou face (3D))
	///  3. Identifica os vertices que tem 2 ou mais elementos adjacentes que
	///     tem 2 (2D) ou 3 (3D) vertices na lista anterior (do passo 2)
	///  4. Identifica os elementos que tem todos os 3 (2D) ou 4 (3D) vertices
	///     na lista dos passos anteriores (dos passos 2 e 3)
	///  Ate aqui, tem-se os passos 1 e 2. Ainda faltam os passos 3 e 4, cujo
	///     codigo esta escrito abaixo

	Data::VertexList vs = front->getVertices();
	//fels = front->getElements(); //unused from now on

	Data::ElementSet adjs;

	for (Data::VertexList::iterator iter = vs.begin();
		 iter != vs.end(); iter++)
	{
		//Data::ElementList els = this->mesh->adjacency((*iter));

		//adjs.insert(els.begin(), els.end());
		adjs.insert(static_cast<MJMesh::Vertex *>((*iter))->eBegin(), static_cast<MJMesh::Vertex *>((*iter))->eEnd());
	}

	Data::UIntMap counts;
	Data::VertexSet newVertices;

	for (Data::ElementSet::iterator iter = adjs.begin();
		 iter != adjs.end(); iter++)
	{
		Data::Element *el = (*iter);

		Data::VertexList nonAdj;

		for (UInt i = 0; i < el->numVertices(); i++)
		{
			Data::Vertex *v = el->getVertex(i);

			if (std::find(vs.begin(), vs.end(), v) == vs.end())
			{
				nonAdj.push_back(v);
			}
		}

		if (nonAdj.size() == 1)
		{
			Data::Vertex *v = nonAdj.front();

			UInt c = (++counts[v->getId()]);

			if (c >= 2)
			{
				newVertices.insert(v);

				/*if (debug)
                {
                    std::stringstream str;
                    str << "el = " << el->text() << std::endl;
                    std::cout << str.str();
                }*/

#if USE_DELETE_FELS
                static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(v, *this->created, true, this->boundary);
                /*UInt debugc = static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(v, *this->created, true);

                if (debug)
                {
                    std::stringstream str;
                    str << "vertex " << v->getId() <<
                        ", inboundary = " << std::boolalpha << this->boundary->have(v) <<
                        ", created " << debugc << " of " << static_cast<MJMesh::Vertex *>(v)->numAdjacentFrontElements() << " fels" << std::endl;
                    std::cout << str.str();
                }*/
#endif //#if USE_DELETE_FELS

				//std::cout << "adding (plus) vertex " << v->text() << std::endl;
			}
		}
	}

#if USE_JMSH_DEBUG_MESSAGES
	if (this->dimension == 3)
	{
		for (Data::VertexList::iterator iter = vs.begin(); iter != vs.end(); iter++)
		{
			std::cout << "markos debug, node = " << (*iter)->getId() - 1 << std::endl;
		}
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	if (!newVertices.empty())
	{
		//vs eh agora a lista a que se refere o passo 3
		vs.insert(vs.end(), newVertices.begin(), newVertices.end());

#if USE_JMSH_DEBUG_MESSAGES
		if (this->dimension == 3)
		{
			for (Data::VertexSet::iterator iter = newVertices.begin(); iter != newVertices.end(); iter++)
			{
				std::cout << "markos debug, add node = " << (*iter)->getId() - 1 << std::endl;
			}
		}
#endif //#if USE_JMSH_DEBUG_MESSAGES

		newVertices.clear();

		adjs.clear();

		for (Data::VertexList::iterator iter = vs.begin();
			 iter != vs.end(); iter++)
		{
			//Data::ElementList els = this->mesh->adjacency((*iter));

			//adjs.insert(els.begin(), els.end());
			adjs.insert(static_cast<MJMesh::Vertex *>((*iter))->eBegin(), static_cast<MJMesh::Vertex *>((*iter))->eEnd());
		}
	}

	elements.clear();

	for (Data::ElementSet::iterator iter = adjs.begin();
		 iter != adjs.end(); iter++)
	{
		if (oldElements.find((*iter)) != oldElements.end())
		{
			continue;
		}

		bool notFound = false;

		for (UInt i = 0; i < (*iter)->numVertices(); i++)
		{
			Data::Vertex *v = (*iter)->getVertex(i);

			if (std::find(vs.begin(), vs.end(), v) == vs.end())
			{
				notFound = true;

				break;
			}
		}

		if (notFound)
		{
			continue;
		}

		elements.push_back((*iter));
	}

	ElFElsMap connectors;

	while (!elements.empty())
	{
		Data::Element *e = elements.front();

		elements.pop_front();

		ElFElsMap::iterator it = connectors.find(e);

		Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(e);

		if (it == connectors.end())
		{
			bool found = false;

			for (Data::FrontElementList::iterator iter = adjs.begin();
				 iter != adjs.end(); iter++)
			{
				Data::FrontElement *fel = (*iter);

				if (front->have(fel))
				{
					connectors[e].push_back(fel);

					found = true;
				}
			}

			if (found)
			{
				elements.push_back(e);
			}

			continue;
		}

		//same as MJMesh::BackTrackingFrontAdvancer::process()
		Data::FrontElementList connects = (*it).second;

		Data::FrontElementSet newFront;

		//boundary cannot be ignored here
		if (/*(!ignoreBoundary) && */(this->boundary->have(e, adjs)))
		{
#if USE_JMSH_DEBUG_MESSAGES
			if (this->dimension == 3)
			{
				std::cout << "markos debug makefront el = " << e->getId() - 1 << " is in boundary" << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES
			continue;
		}

#if USE_JMSH_DEBUG_MESSAGES
		if (this->dimension == 3)
		{
			std::cout << "markos debug makefront el = " << e->getId() - 1 << " is being deleted" << std::endl;
		}
#endif //#if USE_JMSH_DEBUG_MESSAGESs

/*#if USE_DELETE_FELS
		MJMesh::MeshBackTracker::process(*this->created, e, adjs, oldElements, newFront,
			oldFrontElements, connects, this->mesh);
#else*/
		MJMesh::MeshBackTracker::process(e, adjs, oldElements, newFront,
			oldFrontElements, connects, this->mesh);
/*#endif //#if USE_DELETE_FELS*/

		//Data::VertexVector vertices = e->getVertices();

		for (Data::FrontElementList::iterator iter = connects.begin();
			 iter != connects.end(); iter++)
		{
			front->remove((*iter));
		}

		for (Data::FrontElementSet::iterator iter = newFront.begin();
			 iter != newFront.end(); iter++)
		{
			Data::FrontElement *fel = (*iter);
			front->add(fel);

			//Data::VertexVector vertices = fel->getVertices();

			//while (!vertices.empty())
			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				//Data::Vertex *v = vertices.back();
				Data::Vertex *v = fel->getVertex(i);
				//vertices.pop_back();

				if (!front->have(v))
				{
					front->add(v);

#if USE_DELETE_FELS
                    static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(v, *this->created, true, this->boundary);
                    /*UInt debugc = static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(v, *this->created, true);

                    if (debug)
                    {
                        std::stringstream str;
                        str << "vertex " << v->getId() <<
                            ", inboundary = " << std::boolalpha << this->boundary->have(v) <<
                            ", created " << debugc << " of " << static_cast<MJMesh::Vertex *>(v)->numAdjacentFrontElements() << " fels" << std::endl;
                        std::cout << str.str();
                    }*/
#endif //#if USE_DELETE_FELS
				}
			}

			Data::Element *adj = NULL;

			//sames as MJMesh::BackTrackingFrontAdvancer::adjacency()
			//Data::ElementList els = this->mesh->adjacency(fel);

			//if (els.empty())
			if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0)
			{
				continue;
			}

			//if (els.front() != e)
			if ((*static_cast<MJMesh::FrontElement *>(fel)->eBegin()) != e)
			{
				//adj = els.front();
				adj = (*static_cast<MJMesh::FrontElement *>(fel)->eBegin());
			}
			//else if (els.size() > 1)
			else if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() > 1)
			{
				//adj = els.back();
				adj = (*(++static_cast<MJMesh::FrontElement *>(fel)->eBegin()));
			}
			//end sames as MJMesh::BackTrackingFrontAdvancer::adjacency()

			if (adj)
			{
				connectors[adj].push_back(fel);
			}
		}

		//while (!vertices.empty())
		for (UInt i = 0; i < e->numVertices(); i++)
		{
			//Data::Vertex *v = vertices.back();
			Data::Vertex *v = e->getVertex(i);
			//vertices.pop_back();

			//if (this->mesh->adjacency(v).empty())
			if ((front->shouldRemove(v)) &&
				//((!ignoreBoundary) || (this->mesh->findAdjacentFrontElements(v).empty())))
				((!ignoreBoundary) || (static_cast<MJMesh::Vertex *>(v)->numAdjacentFrontElements() == 0)))
			{
				MJMesh::MeshBackTracker::markAsOld(oldVertices, v, this->mesh);

				front->remove(v);
			}
		}
		//end same as MJMesh::BackTrackingFrontAdvancer::process()
	}
	///endebug para ficar igual ao jmsh
#endif //#if USE_JMSH_BACK_TRACKING

	return front;

	/*//find the front elements adjacent to the given element
	Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(e);

	//if the element is adjacent to the boundary, then do not back track
	if (this->boundary->haveAny(fels))
	{
		return NULL;
	}

	Data::FrontElementSet newFront;

	MJMesh::MeshBackTracker::markAsOld(oldElements, e, this->mesh);

	for (Data::FrontElementList::iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		MJMesh::FrontElement *fel = (MJMesh::FrontElement *)(*iter);

		Data::ElementList adjElements = fel->getAdjacency();

		MJMesh::Element *adj = (adjElements.front() == e) ? (MJMesh::Element *)adjElements.back() : (MJMesh::Element *)adjElements.front();

		Data::FrontElementList adjFels = this->mesh->findAdjacentFrontElements(adj);

		if (this->boundary->haveAny(adjFels))
		{
			this->invert(fel, e);

			newFront.insert(fel);

			continue;
		}

		MJMesh::MeshBackTracker::markAsOld(oldElements, adj, this->mesh);
		MJMesh::MeshBackTracker::markAsOld(oldFrontElements, fel, this->mesh);

		for (Data::FrontElementList::iterator iter2 = adjFels.begin();
			 iter2 != adjFels.end(); iter2++)
		{
			MJMesh::FrontElement *adjFel = (MJMesh::FrontElement *)(*iter2);

			if (adjFel == fel)
			{
				continue;
			}

			this->invert(adjFel, adj);

			newFront.insert(adjFel);
		}
	}

	Data::VertexVector vertices = e->getVertices();

	while (!vertices.empty())
	{
		MJMesh::Vertex *v = (MJMesh::Vertex *)vertices.back();
		vertices.pop_back();

		if (v->getAdjacentElements().empty())
		{
			MeshBackTracker::markAsOld(oldVertices, v, this->mesh);
		}
	}

	MJMesh::Front *front = new MJMesh::Front(false);

	while (!newFront.empty())
	{
		Data::FrontElement *e = *newFront.begin();
		newFront.erase(newFront.begin());

		front->add(e);

		Data::VertexVector vertices = e->getVertices();

		while (!vertices.empty())
		{
			Vertex *v = vertices.back();
			vertices.pop_back();

			if (!front->have(v))
			{
				front->Front::add(v);
			}
		}
	}

	return front;*/
}

void MJMesh::MeshBackTracker::insert(Data::ElementList &elements, Data::Element *e) const
{
	/*Data::ElementList::iterator pos = elements.end();

	Real gamma = ((MJMesh::Element *)e)->gamma();

	for (Data::ElementList::iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		MJMesh::Element *el = (MJMesh::Element *)(*iter);

		if (el->gamma() < gamma)
		{
			pos = iter;

			break;
		}
	}

	elements.insert(pos, e);*/

	elements.push_back(e);
}

Data::ElementList MJMesh::MeshBackTracker::sort() const
{
	if (!this->meshSorted)
	{
		//return this->mesh->getElements(true);
		//return this->mesh->getSortedElements();
		return this->mesh->getElements();
	}

	Data::ElementList sorted = Performer::MeshBackTracker::sort();

	sorted.sort(MJMesh::Element::gammaGreater);

	return sorted;
}

bool MJMesh::MeshBackTracker::worse(Real rank1, Real rank2) const
{
	return rank1 >= rank2;
}

bool MJMesh::MeshBackTracker::betterMesh(const Data::ElementSet &newMesh, const Data::ElementSet &oldMesh) const
{
	Real worst = static_cast<MJMesh::Element *>(*oldMesh.begin())->gamma();

	for (Data::ElementSet::iterator iter = ++oldMesh.begin();
		 iter != oldMesh.end(); iter++)
	{
		//std::cout << "debug bettermesh worst = " << worst << std::endl;

		Real gamma = static_cast<MJMesh::Element *>((*iter))->gamma();

		if (this->worse(gamma, worst))
		{
			worst = gamma;
		}
	}

	//std::cout << "debug bettermesh worst! = " << worst << std::endl;

	for (Data::ElementSet::iterator iter = newMesh.begin();
		 iter != newMesh.end(); iter++)
	{
	    if ((*iter)->getShape()->orientedSize() < Data::getTolerance())
        {
            return false;
        }

		Real gamma = static_cast<MJMesh::Element *>((*iter))->gamma();

		//std::cout << "debug bettermesh gamma = " << gamma << std::endl;

		if (this->worse(gamma, worst))
		{
			return false;
		}
	}

	return true;
}
