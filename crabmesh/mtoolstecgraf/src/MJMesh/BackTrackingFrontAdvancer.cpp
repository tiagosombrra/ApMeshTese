#include "MJMesh/BackTrackingFrontAdvancer.h"

#include "Data/OStream.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/MeshBackTracker.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "MJMesh/Front.h"

#include "Performer/IdManager.h"

#include "Data/State.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Edge2D.h"
#include "Data/Edge3D.h"
#include "Data/Triangle3D.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

using namespace MJMesh;

const Performer::Status MJMesh::BackTrackingFrontAdvancer::notInitBackTracking =
	Performer::Status(MJMesh::BackTrackingFrontAdvancer::NOT_INIT_BACK_TRACKING, "back tracking not initiated");
const Performer::Status MJMesh::BackTrackingFrontAdvancer::initBackTracking =
	Performer::Status(MJMesh::BackTrackingFrontAdvancer::INIT_BACK_TRACKING, "back tracking initiated");
const Performer::Status MJMesh::BackTrackingFrontAdvancer::adjElementsFound =
	Performer::Status(MJMesh::BackTrackingFrontAdvancer::ADJ_ELEMENTS_FOUND, "elements adjacent to front found");
const Performer::Status MJMesh::BackTrackingFrontAdvancer::midPointFound =
	Performer::Status(MJMesh::BackTrackingFrontAdvancer::MID_POINT_FOUND, "centroid found");
const Performer::Status MJMesh::BackTrackingFrontAdvancer::visibilityChecked =
	Performer::Status(MJMesh::BackTrackingFrontAdvancer::VISIBILITY_CHECKED, "visibility checked");
const Performer::Status MJMesh::BackTrackingFrontAdvancer::processElementFound =
	Performer::Status(MJMesh::BackTrackingFrontAdvancer::PROCESS_ELEMENT_FOUND, "element to be back-tracked found");

MJMesh::BackTrackingFrontAdvancer::BackTrackingFrontAdvancer(UInt dimension,
	const MJMesh::Boundary *boundary, const Data::BoxTree *tree,
	MJMesh::Mesh *mesh, MJMesh::Front *front,
	bool adjustFront, bool deleteElements, UInt maxBackTracks, enum Mode mode) :
	MJMesh::FrontAdvancer(dimension, boundary, tree, false)
{
	this->setMesh(mesh);
	this->setFront(front);

	this->setAdjustFront(adjustFront);
	this->setDeleteElements(deleteElements);
	this->setMaxBackTracks(maxBackTracks);
	this->setMode(mode);

	this->mid = NULL;
	this->chosen = NULL;
	this->maxHits = 0;
	this->ignoreBoundary = false;
	//this->debug = false;
}

MJMesh::BackTrackingFrontAdvancer::~BackTrackingFrontAdvancer()
{
#if USE_DELETE_FELS
    while (!this->created.empty())
    {
        delete this->created.front();

        this->created.pop_front();
    }
#endif //#if USE_DELETE_FELS
}

void MJMesh::BackTrackingFrontAdvancer::setAdjustFront(bool adjustFront)
{
	this->shouldAdjustFront = adjustFront;
}

void MJMesh::BackTrackingFrontAdvancer::setDeleteElements(bool deleteElements)
{
	this->deleteElements = deleteElements;
}

void MJMesh::BackTrackingFrontAdvancer::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

void MJMesh::BackTrackingFrontAdvancer::setMode(enum Mode mode)
{
    this->mode = mode;
}

void MJMesh::BackTrackingFrontAdvancer::setOldElements(const Data::ElementSet &oldElements)
{
	this->oldElements = oldElements;
}

const Data::ElementSet &MJMesh::BackTrackingFrontAdvancer::getOldElements() const
{
	return this->oldElements;
}

const Data::ElementSet &MJMesh::BackTrackingFrontAdvancer::getNewElements() const
{
	return this->newElements;
}

void MJMesh::BackTrackingFrontAdvancer::setOldFrontElements(const Data::FrontElementSet &oldFrontElements)
{
	this->oldFrontElements = oldFrontElements;
}

const Data::FrontElementSet &MJMesh::BackTrackingFrontAdvancer::getOldFrontElements() const
{
	return this->oldFrontElements;
}

const Data::FrontElementSet &MJMesh::BackTrackingFrontAdvancer::getNewFrontElements() const
{
	return newFrontElements;
}

void MJMesh::BackTrackingFrontAdvancer::setOldVertices(const Data::VertexSet &oldVertices)
{
	this->oldVertices = oldVertices;
}

const Data::VertexSet &MJMesh::BackTrackingFrontAdvancer::getOldVertices() const
{
	return this->oldVertices;
}

const Data::VertexSet &MJMesh::BackTrackingFrontAdvancer::getNewVertices() const
{
	return this->newVertices;
}

#if USE_DELETE_FELS
void MJMesh::BackTrackingFrontAdvancer::setCreated(Data::FrontElementList *created) const
{
    if (created)
    {
        this->created.push_back(created);
    }
    else
    {
        this->created.clear();
    }
}
#endif //#if USE_DELETE_FELS

void MJMesh::BackTrackingFrontAdvancer::deleteOldMesh()
{
	while (!this->frontElements.empty())
	{
		//Data::FrontElement *fel = this->frontElements.back();
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(this->frontElements.back());
		this->frontElements.pop_back();

		//Data::ElementList elements = this->mesh->adjacency(fel);

		//cout << "debug deletemesh fel = " << fel->getId() << endl;
		//cout << "debug deletemesh fel.adj.size = " << elements.size() << endl;

#if USE_GUI
        Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::MESH_FRONT_ELEMENT, r, g, b);
        fel->setColor(r, g, b);

        Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::Vertex *v = fel->getVertex(i);

			v->setColor(r, g, b);
		}
#endif //#if USE_GUI

		//while (!elements.empty())
		for (Data::ElementList::iterator iter = fel->eBegin();
             iter != fel->eEnd();)
		{
			//Data::Element *e = elements.back();
			Data::Element *e = (*iter);
			//elements.pop_back();

			if (this->oldElements.find(e) != this->oldElements.end())
			{
				//cout << "debug deletemesh removed adj = " << e->getId() << endl;

				//static_cast<MJMesh::FrontElement *>(fel)->remove(static_cast<MJMesh::Element *>(e));
				iter = fel->erase(iter);
			}
			else
			{
				//cout << "debug deletemesh not removed adj = " << e->getId() << endl;

				iter++;
			}
		}

		//cout << "debug deletemesh fel.adj.size = " << ((MJMesh::FrontElement *)fel)->getAdjacency().size() << endl;
	}

	while (!this->oldElements.empty())
	{
		Data::Element *e = *this->oldElements.begin();
		this->oldElements.erase(this->oldElements.begin());

		//std::cout << "deleting old element " << e->getId() << " " << e << std::endl;

		delete e;
	}

#if USE_DELETE_FELS
    this->oldFrontElements.clear();

    Data::FrontElementList *created = this->created.front();

    this->created.pop_front();

    //std::cout << "will delete " << created->size() << " created fels" << std::endl;

    while (!created->empty())
    {
        Data::FrontElement *fel = created->front();

        created->pop_front();

        this->mesh->remove(fel);

        delete fel;
    }
#else
	while (!this->oldFrontElements.empty())
	{
		Data::FrontElement *e = *this->oldFrontElements.begin();
		this->oldFrontElements.erase(this->oldFrontElements.begin());

		//std::cout << "deleting old fel " << e->getId() << " " << e << std::endl;

		delete e;
	}
#endif //#if USE_DELETE_FELS

	while (!this->oldVertices.empty())
	{
		Data::Vertex *v = *this->oldVertices.begin();
		this->oldVertices.erase(this->oldVertices.begin());

		/*{
		    std::stringstream str;
		    str << "deleting old vertex at " << v << std::endl;
		    str << "deleting old vertex " << v->getId() << std::endl;
		    std::cout << str.str();
		}*/

		delete v;
	}

	//debug
	while (!this->newElements.empty())
	{
		Data::Element *e = *newElements.begin();
		newElements.erase(newElements.begin());

		//e->setColor(0.5, 0.5, 0.0);
		//e->highlight();

		e->setId(this->idManager->next(2));
	}

	while (!this->newFrontElements.empty())
	{
		Data::FrontElement *e = *newFrontElements.begin();
		newFrontElements.erase(newFrontElements.begin());

		//e->setColor(0.5, 0.5, 0.0);
		//e->highlight();

#if USE_DELETE_FELS
        this->mesh->remove(e);

        delete e;
#else
		e->setId(this->idManager->next(1));
#endif //#if USE_DELETE_FELS
	}

	while (!this->newVertices.empty())
	{
		Data::Vertex *v = *newVertices.begin();
		newVertices.erase(newVertices.begin());

		//v->setColor(0.5, 0.5, 0.0);
		//v->highlight();

		v->setId(this->idManager->next(0));

		/*//debug
		if (v->getId() == 4146)
        {
            std::cout << "debug generated vertex " << v->getId() << std::endl;
        }
		//endebug*/
	}
	//endebug
}

void MJMesh::BackTrackingFrontAdvancer::restoreOldMesh()
{
	while (!this->frontElements.empty())
	{
		//Data::FrontElement *fel = this->frontElements.back();
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(this->frontElements.back());
		this->frontElements.pop_back();

		//Data::ElementList elements = this->mesh->adjacency(fel);

		//while (!elements.empty())
		for (Data::ElementList::iterator iter = fel->eBegin();
             iter != fel->eEnd();)
		{
			//Data::Element *e = elements.back();
			Data::Element *e = (*iter);
			//elements.pop_back();

			if (this->newElements.find(e) != this->newElements.end())
			{
				//static_cast<MJMesh::FrontElement *>(fel)->remove(static_cast<MJMesh::Element *>(e));

				iter = fel->erase(iter);
			}
			else
            {
                iter++;
            }
		}
	}

	while (!this->newElements.empty())
	{
		Data::Element *e = *this->newElements.begin();
		this->newElements.erase(this->newElements.begin());

		this->mesh->remove(e);

		delete e;
	}

	while (!this->newFrontElements.empty())
	{
		Data::FrontElement *e = *this->newFrontElements.begin();
		this->newFrontElements.erase(this->newFrontElements.begin());

		this->mesh->remove(e);

		delete e;
	}

	while (!this->newVertices.empty())
	{
		Data::Vertex *v = *this->newVertices.begin();
		this->newVertices.erase(this->newVertices.begin());

		this->mesh->remove(v);

		delete v;
	}

	while (!this->oldElements.empty())
	{
		Data::Element *e = *this->oldElements.begin();
		this->oldElements.erase(this->oldElements.begin());

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			static_cast<MJMesh::Vertex *>(e->getVertex(i))->add(e);
		}

		//debug
		//e->setColor(0.5, 0.0, 0.5);

		//e->highlight();
		//endebug

		this->mesh->add(e);
	}

#if USE_DELETE_FELS
    this->oldFrontElements.clear();

    Data::FrontElementList *created = this->created.front();

    //std::cout << "will delete " << created->size() << " created fels" << std::endl;

    while (!created->empty())
    {
        Data::FrontElement *fel = created->front();

        created->pop_front();

        this->mesh->remove(fel);

        delete fel;
    }
#else
	while (!this->oldFrontElements.empty())
	{
		Data::FrontElement *e = *this->oldFrontElements.begin();
		this->oldFrontElements.erase(this->oldFrontElements.begin());

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			static_cast<MJMesh::Vertex *>(e->getVertex(i))->add(static_cast<MJMesh::FrontElement *>(e));
		}

		//debug
		//e->setColor(0.5, 0.0, 0.5);

		//e->highlight();
		//endebug

		this->mesh->add(e);
	}
#endif //#if USE_DELETE_FELS

	while (!this->oldVertices.empty())
	{
		Data::Vertex *v = *this->oldVertices.begin();
		this->oldVertices.erase(this->oldVertices.begin());

		//debug
		//v->setColor(0.5, 0.0, 0.5);

		//v->highlight();
		//endebug

		this->mesh->add(v);
	}
}

void MJMesh::BackTrackingFrontAdvancer::execute()
{
	switch (this->status.getType())
	{
	case MJMesh::BackTrackingFrontAdvancer::NOT_INIT:
		if (this->shouldAdjustFront)
		{
			this->fronts = this->adjustFront();
		}
		else
		{
			this->fronts.push_front(this->front);
		}

		if (this->fronts.empty())
		{
			this->front = NULL;
		}
		else
		{
			this->front = this->fronts.front();
		}

		this->status = MJMesh::BackTrackingFrontAdvancer::init;

		if (!this->front)
		{
			this->status = MJMesh::BackTrackingFrontAdvancer::ended;

			break;
		}

#if USE_GUI
		if ((Data::isStepByStep()) && (this->shouldAdjustFront))
		{
			break;
		}
#endif //#if USE_GUI

	case MJMesh::BackTrackingFrontAdvancer::INIT:
	case MJMesh::BackTrackingFrontAdvancer::NOT_INIT_BACK_TRACKING:
	case MJMesh::BackTrackingFrontAdvancer::INIT_BACK_TRACKING:
	case MJMesh::BackTrackingFrontAdvancer::ADJ_ELEMENTS_FOUND:
	case MJMesh::BackTrackingFrontAdvancer::MID_POINT_FOUND:
	case MJMesh::BackTrackingFrontAdvancer::VISIBILITY_CHECKED:
	case MJMesh::BackTrackingFrontAdvancer::PROCESS_ELEMENT_FOUND:
		while (this->front)
		{
			if (this->status.getType() == MJMesh::BackTrackingFrontAdvancer::init.getType())
			{
				this->front = fronts.front();
				this->fronts.pop_front();

				this->step = 0;

#if USE_GUI
				static Int count = 0;

				r = g = b = 0.0;

				Data::pallete(count++, r, g, b);
#endif //#if USE_GUI

				this->status = MJMesh::BackTrackingFrontAdvancer::notInitBackTracking;
			}

			this->backTrack();

			if ((this->status.getType() == FrontAdvancer::ended.getType()) ||
				(this->status.getType() == FrontAdvancer::rejected.getType()))
			{
				if (this->deleteElements)
				{
					if (this->status.getType() == FrontAdvancer::ended.getType())
					{
						this->deleteOldMesh();

#if USE_JMSH_DEBUG_MESSAGES
						if (this->dimension == 3)
						{
							std::cout << "markos debug, real back tracking worked" << std::endl;
						}
#endif //#if USE_JMSH_DEBUG_MESSAGES
					}
					else if (this->status.getType() == FrontAdvancer::rejected.getType())
					{
#if ((USE_DELETE_FELS) && (USE_GUI))
                        //if ((!this->fronts.empty()) && (this->maxBackTracks == 0))
                        if ((!this->fronts.empty()) && (this->mode == FRONT_ADVANCING))
                        {
                            Data::FrontElementList *created = this->created.front();

                            for (Data::FrontElementList::iterator iter = created->begin();
                                 iter != created->end();)
                            {
                                Data::FrontElement *fel = (*iter);

                                if (this->front->have(fel))
                                {
                                    iter = created->erase(iter);

                                    continue;
                                }

                                iter++;
                            }
                        }
#endif //#if ((USE_DELETE_FELS) && (USE_GUI))

						this->restoreOldMesh();

#if USE_JMSH_DEBUG_MESSAGES
						if (this->dimension == 3)
						{
							std::cout << "markos debug, real back tracking failed" << std::endl;
						}
#endif //#if USE_JMSH_DEBUG_MESSAGES
					}
				}

				if (this->fronts.empty())
				{
					this->front = NULL;
				}
				//else if ((this->maxBackTracks == 0) &&
				else if ((this->mode == FRONT_ADVANCING) &&
						 (this->status.getType() == FrontAdvancer::rejected.getType()))
				{
#if USE_GUI
					Data::FrontElementSet fels;
					Data::VertexSet vs;

					while (!this->fronts.empty())
					{
						MJMesh::Front *front = static_cast<MJMesh::Front *>(this->fronts.front());

						this->fronts.pop_front();

						for (Data::Front::AllFrontElementIterator iter = front->allEBegin(), end = front->allEEnd();
							 iter != end; iter++)
						{
							Data::FrontElement *fel = (*iter);

							fels.insert(fel);

							for (UInt i = 0; i < fel->numVertices(); i++)
							{
								vs.insert(fel->getVertex(i));
							}
						}

						front->clear();

						delete front;
						front = NULL;
					}

					for (Data::FrontElementSet::iterator iter = fels.begin();
						 iter != fels.end(); iter++)
					{
						if (!this->front->have((*iter)))
                        {
                            this->front->add((*iter));
                        }
					}

					for (Data::VertexSet::iterator iter = vs.begin();
						 iter != vs.end(); iter++)
					{
						if (!this->front->have((*iter)))
                        {
                            this->front->add((*iter));
                        }
					}
#endif //#if USE_GUI

					break;
				}
				//else if (this->maxBackTracks != 0)
				//else if (this->mode == OPTIMIZATION)
				else
				{
					this->status = MJMesh::BackTrackingFrontAdvancer::init;
				}
			}
#if USE_GUI
			if (Data::isStepByStep()) break;
#endif //#if USE_GUI
		}

	case MJMesh::BackTrackingFrontAdvancer::ENDED:
	case MJMesh::BackTrackingFrontAdvancer::REJECTED:
		break;
	}
}

Data::FrontList MJMesh::BackTrackingFrontAdvancer::adjustFront() const
{
	Data::FrontList fronts;

	static_cast<MJMesh::Front *>(this->front)->prepareForTopologyPhase(false);

	Data::FrontElementList fels = this->front->getElements();

	fels.sort(Data::Identifiable::less);

	while (!fels.empty())
	{
#if USE_DELETE_FELS
        Data::FrontElementList *created = new Data::FrontElementList();

        this->created.push_back(created);
#endif //#if USE_DELETE_FELS

		Data::VertexSet tested;

		Data::FrontElement *fel = fels.front();

		fels.pop_front();

		Data::FrontElementSet adjs, cavity;

		adjs.insert(fel);

		while (!adjs.empty())
		{
			fel = *adjs.begin();

			adjs.erase(adjs.begin());

			if (!cavity.insert(fel).second)
			{
				continue;
			}

			//std::cout << "cavity fel = " << fel->text() << std::endl;

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				Data::Vertex *v = fel->getVertex(i);

//#if USE_DELETE_FELS
//                static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(v, *created, true, this->boundary);
//#endif //#if USE_DELETE_FELS

				//in 3 dimensions, the same vertex can be tested many times,
				//  due to the edge adjacency restriction
				if ((this->dimension != 3) && (!tested.insert(v).second))
				{
					continue;
				}

				//Data::FrontElementList vAdjs = this->mesh->findAdjacentFrontElements(v);

				//while (!vAdjs.empty())
				for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
				{
					//Data::FrontElement *f = vAdjs.front();
					MJMesh::FrontElement *f = static_cast<MJMesh::FrontElement *>((*iter));

					//vAdjs.pop_front();

					Data::FrontElementList::iterator it;

					if ((f == fel) ||
						//(this->mesh->adjacency(f).size() == 2) ||
						(f->numAdjacents() == 2) ||
						(cavity.find(f) != cavity.end()) ||
						(adjs.find(f) != adjs.end()) ||
						//(this->boundary->have(f)))
						((it = std::find(fels.begin(), fels.end(), f)) == fels.end()))
					{
						continue;
					}

					if (this->dimension == 3)
					{
						//if front elements are adjacent by edge, not by
						// vertex

						bool edgeAdjacency = false;

						for (UInt j = 0; j < fel->numVertices(); j++)
						{
							if (i == j)
							{
								continue;
							}

							Data::Edge e(v->getPoint(), fel->getVertex(j)->getPoint());

							if (f->have(e))
							{
								edgeAdjacency = true;

								break;
							}
						}

						if (!edgeAdjacency)
						{
							continue;
						}
					}

					//std::cout << "adj fel = " << f->text() << std::endl;

					//fels.remove(f);
					fels.erase(it);

					adjs.insert(f);
				}
			}
		}

#if USE_GUI
		static Int count = 0;

		Real r, g, b;
		r = g = b = 0.0;

		Data::pallete(count++, r, g, b);
#endif //#if USE_GUI

		MJMesh::Front *f = new MJMesh::Front();

		Data::VertexSet vertices;

//#if USE_DELETE_FELS
//        std::cout << "adding " << cavity.size() << " fels to created" << std::endl;
//#endif //#if USE_DELETE_FELS

		for (Data::FrontElementSet::iterator iter = cavity.begin();
			 iter != cavity.end(); iter++)
		{
#if USE_GUI
			(*iter)->setColor(r, g, b);
#endif //#if USE_GUI

#if USE_DELETE_FELS
            created->push_back((*iter));
#endif //#if USE_DELETE_FELS

			f->add((*iter));

			for (UInt i = 0; i < (*iter)->numVertices(); i++)
			{
				vertices.insert((*iter)->getVertex(i));
			}
		}

		for (Data::VertexSet::iterator iter = vertices.begin();
			 iter != vertices.end(); iter++)
		{
#if USE_GUI
			(*iter)->setColor(r, g, b);
#endif //#if USE_GUI

			f->add((*iter));
		}

		fronts.push_back(f);
	}

	//this->front->clear();

	delete this->front;

	//this->front = NULL;

	return fronts;
}

/*Data::ElementList MJMesh::BackTrackingFrontAdvancer::findAdjacentElements() const
{
	Data::ElementList adjacency;

	for (Data::Front::ConstFrontElementIterator iter = this->front->eBegin(1);
		 iter != this->front->eEnd(1); iter++)
	{
		//this->mesh->adjacency((*iter)) should return a list with only one
		// element
		adjacency.push_back(this->mesh->adjacency((*iter)).back());
	}

	return adjacency;
}*/

Data::Element *MJMesh::BackTrackingFrontAdvancer::adjacency(Data::FrontElement *fel) const
{
	/*Data::ElementList els = this->mesh->adjacency(fel);

	if (els.empty())
	{
		return NULL;
	}*/

	if (this->boundary->have(fel, false))
    {
        return NULL;
    }

	/*if (this->oldElements.find(els.front()) == this->oldElements.end())
	{
		return els.front();
	}

	if (els.size() > 1)
	{
		return els.back();
	}

	return NULL;*/

	for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
         iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
    {
        if (this->oldElements.find((*iter)) == this->oldElements.end())
        {
            return (*iter);
        }
    }

    return NULL;
}

void MJMesh::BackTrackingFrontAdvancer::findFrontElements()
{
	this->frontElements = this->front->getElements();

	//std::cout << std::endl << std::endl;

	for (Data::FrontElementList::iterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
#if USE_GUI
		//if (this->maxBackTracks == 0)
		if (this->mode == FRONT_ADVANCING)
		{
			(*iter)->setColor(r, g, b);

			for (UInt i = 0; i < (*iter)->numVertices(); i++)
			{
				(*iter)->getVertex(i)->setColor(r, g, b);
			}
		}
#endif //#if USE_GUI

		/*//if (((*iter)->getId() == 163) || ((*iter)->getId() == 164))
        {
            Data::cout() << "front element in front: " << (*iter)->text() << Data::endl;
            Data::cout() << "front element in front at: " << (*iter) << Data::endl;
            Data::cout() << "front element in boundary " << this->boundary << " : " << std::boolalpha << this->boundary->have((*iter), false) << Data::endl;
        }*/

		Data::Element *e = this->adjacency((*iter));

		if (e)
		{
            /*if (fel->getId() == 1280)
            {
                //Data::cout() << "front element in front: " << (*iter)->text() << Data::endl;
                //Data::cout() << "front element in boundary: " << std::boolalpha << this->boundary->have((*iter), false) << Data::endl;
                //Data::cout() << "adjacent element is: " << e->text() << Data::endl;
            }*/

			//std::cout << "adjacent element is " << e->text() << std::endl;

			this->elements.push_back(e);

			this->mapElementFront[e].push_back((*iter));
		}
	}
}

bool MJMesh::BackTrackingFrontAdvancer::hasInnerElement()
{
	UInt max = this->dimension + 1;

	for (ElFElsMap::iterator iter = this->mapElementFront.begin();
		 iter != this->mapElementFront.end(); iter++)
	{
		if ((*iter).second.size() < max)
		{
			this->adjacentElements.insert((*iter).first);
		}
		else
		{
			Data::FrontElementList fels = this->mesh->findAdjacentFrontElements((*iter).first);

			this->process((*iter).first, fels, (*iter).second, false);

			return true;
		}
	}

	return false;
}

Data::Point *MJMesh::BackTrackingFrontAdvancer::makeMid() const
{
	//debug do not consider the mid point

	//change this boolean variable to false if you want the normal back-tracking.
	// If it is true, when the 2D(3D) front has only 3(4) edges (triangles),
	// only one element will be created. The normal back-tracking inserts a
	// point in the middle position, leading to a mesh worse than if you
	// do not insert the point.
	bool doNotConsiderMidPoint = true;

	if ((doNotConsiderMidPoint) &&
		(this->frontVertices.size() == this->dimension + 1))
	{
		return NULL;
	}

	//endebug do not consider the mid point

#if USE_JMSH_IMPLEMENTATION_DETAILS
	///debug para ficar igual ao jmsh
	if (this->dimension == 3)
	{
		Data::Vector v(this->dimension);

		for (Data::FrontElementList::const_iterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			for (UInt i = 0; i < (*iter)->numVertices(); i++)
			{
				v.sum((*iter)->getVertex(i)->getPoint());
			}
		}

		v.multiply(1.0/(static_cast<Real>(this->frontElements.size())*3.0));

		return new Data::Point3D(v);
	}
	///endebug para ficar igual ao jmsh
#endif //#if USE_JMSH_IMPLEMENTATION_DETAILS

#if USE_EXPERIMENTAL_3
    if (this->dimension == 2)
    {
        Data::Vector2D v;

        for (Data::VertexList::const_iterator iter = this->frontVertices.begin();
             iter != this->frontVertices.end(); iter++)
        {
            v.sum((*iter)->getPoint());
        }

        v.multiply(1.0/static_cast<Real>(this->frontVertices.size()));

        return new Data::Point2D(v);
    }

    Data::Vector3D v;

	for (Data::VertexList::const_iterator iter = this->frontVertices.begin();
		 iter != this->frontVertices.end(); iter++)
	{
		v.sum((*iter)->getPoint());
	}

	v.multiply(1.0/static_cast<Real>(this->frontVertices.size()));

	return new Data::Point3D(v);
#else
	Data::Vector v(this->dimension);

	for (Data::VertexList::const_iterator iter = this->frontVertices.begin();
		 iter != this->frontVertices.end(); iter++)
	{
		v.sum((*iter)->getPoint());
	}

	v.multiply(1.0/static_cast<Real>(this->frontVertices.size()));

	return (this->dimension == 2) ?
		static_cast<Data::Point *>(new Data::Point2D(v)) :
		static_cast<Data::Point *>(new Data::Point3D(v));
#endif //#if USE_EXPERIMENTAL_3
}

void MJMesh::BackTrackingFrontAdvancer::makeMids()
{
	Data::Point *mid = this->makeMid();

	if (!mid)
	{
		return;
	}

	this->mids.push_back(mid);

	//if (this->maxBackTracks != 0) //improvement
	if (this->mode == OPTIMIZATION) //improvement
	{
		return;
	}

	//Data::Vector v(this->dimension);
	VECTORd(v, this->dimension);
	UInt num = 0;

	for (Data::FrontElementList::const_iterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		Data::PointList ideals = this->ideal((*iter));

		for (Data::PointList::iterator iter2 = ideals.begin();
			 iter2 != ideals.end();)
		{
			Data::Point *p = (*iter2);

			bool ok = true;

			for (Data::FrontElementList::const_iterator iter3 = this->frontElements.begin();
				 iter3 != this->frontElements.end(); iter3++)
			{
				if ((*iter3) == (*iter))
				{
					continue;
				}

				if ((*iter3)->accordingToNormal(p))
				{
					ok = false;

					break;
				}
			}

			if (ok)
			{
				v.sum(p);
				num++;

				iter2++;
			}
			else
			{
				delete p;

				iter2 = ideals.erase(iter2);
			}
		}

		this->mids.splice(this->mids.end(), ideals);
	}

	if (num > 1)
	{
		v.multiply(1.0/static_cast<Real>(num));

		Data::Point *p = (this->dimension == 2) ?
			static_cast<Data::Point *>(new Data::Point2D(v)) :
			static_cast<Data::Point *>(new Data::Point3D(v));

		this->mids.push_back(p);
	}
}

void MJMesh::BackTrackingFrontAdvancer::makeEdges()
{
	//debug do not consider the mid point

	//if (!this->mid)
	if (this->mids.empty())
	{
		this->frontVertices.clear();

		return;
	}

	//endebug do not consider the mid point

	for (Data::PointList::iterator piter = this->mids.begin();
		 piter != this->mids.end(); piter++)
	{
		this->mid = (*piter);

		Data::EdgeList edges;

		for (Data::VertexList::const_iterator iter = this->frontVertices.begin();
			 iter != this->frontVertices.end(); iter++)
		{
			if (this->dimension == 2)
			{
				Data::Edge2D *e = new Data::Edge2D(static_cast<Data::Point2D *>(const_cast<Data::Point *>(this->mid)), static_cast<Data::Point2D *>((*iter)->getPoint()));

				//this->edges.push_back(e);
				edges.push_back(e);
			}
			else
			{
				Data::Edge3D *e = new Data::Edge3D(static_cast<Data::Point3D *>(const_cast<Data::Point *>(this->mid)), static_cast<Data::Point3D *>((*iter)->getPoint()));

				//this->edges.push_back(e);
				edges.push_back(e);
			}
		}

		this->edges[this->mid].swap(edges);
	}

	this->mid = NULL;

	this->frontVertices.clear();
}

void MJMesh::BackTrackingFrontAdvancer::clearEdges()
{
	while (!this->edges.empty())
	{
		PointEdgeListMap::iterator it = this->edges.begin();

		while (!(*it).second.empty())
		{
			delete (*it).second.back();

			(*it).second.pop_back();
		}

		this->edges.erase(it);
	}
}

UInt MJMesh::BackTrackingFrontAdvancer::checkVisibility(
	const Data::Element *e, const Data::EdgeList &edges) const
{
	UInt numHits = 0;

	for (Data::EdgeList::const_iterator iter = edges.begin();
		 iter!= edges.end(); iter++)
	{
		if ((e->getShape()->have(*((*iter)->getP1()))) ||
			(e->intercept((*iter))))
		{
			numHits++;
		}
	}

	//Data::cout() << "checkVisibility(e, edges), numHits = " << numHits << ", this->maxHits = " << this->maxHits << Data::endl;

	return numHits;
}

UInt MJMesh::BackTrackingFrontAdvancer::checkVisibility(
	const Data::FrontElement *fel, const Data::EdgeList &edges) const
{
	UInt numHits = 0;

	for (Data::EdgeList::const_iterator iter = edges.begin();
		 iter!= edges.end(); iter++)
	{
#if USE_JMSH_DEBUG_MESSAGES
		/*if (this->dimension == 3)
		{
			std::cout << "markos debug, visibility test" << std::endl;
			std::cout << "markos debug, edge[0] is (" << (*iter)->getPoint(0)->getCoord(0) << ", " << (*iter)->getPoint(0)->getCoord(1) << ", " << (*iter)->getPoint(0)->getCoord(2) << ")" << std::endl;
			std::cout << "markos debug, edge[1] is (" << (*iter)->getPoint(1)->getCoord(0) << ", " << (*iter)->getPoint(1)->getCoord(1) << ", " << (*iter)->getPoint(1)->getCoord(2) << ")" << std::endl;
			std::cout << "markos debug, triangle[0] is (" << fel->getVertex(0)->getPoint()->getCoord(0) << ", " << fel->getVertex(0)->getPoint()->getCoord(1) << ", " << fel->getVertex(0)->getPoint()->getCoord(2) << ")" << std::endl;
			std::cout << "markos debug, triangle[1] is (" << fel->getVertex(1)->getPoint()->getCoord(0) << ", " << fel->getVertex(1)->getPoint()->getCoord(1) << ", " << fel->getVertex(1)->getPoint()->getCoord(2) << ")" << std::endl;
			std::cout << "markos debug, triangle[2] is (" << fel->getVertex(2)->getPoint()->getCoord(0) << ", " << fel->getVertex(2)->getPoint()->getCoord(1) << ", " << fel->getVertex(2)->getPoint()->getCoord(2) << ")" << std::endl;
		}*/
#endif //#if USE_JMSH_DEBUG_MESSAGES

		if ((fel->getShape()->have(*((*iter)->getP1()))) ||
			(fel->intercept((*iter))))
		{
			numHits++;
#if USE_JMSH_DEBUG_MESSAGES
			//if (this->dimension == 3) std::cout << "markos debug interception" << std::endl;
#endif //#if USE_JMSH_DEBUG_MESSAGES
		}
#if USE_JMSH_DEBUG_MESSAGES
		/*else
		{
			if (this->dimension == 3) std::cout << "markos debug no interception" << std::endl;
		}*/
#endif //#if USE_JMSH_DEBUG_MESSAGES
	}

	//Data::cout() << "checkVisibility(fel, edges), fel = " << fel->text() << " inboundary = " << std::boolalpha << this->boundary->have(fel, false) << ", numHits = " << numHits << ", this->maxHits = " << this->maxHits << Data::endl;

	return numHits;
}

/*UInt MJMesh::BackTrackingFrontAdvancer::checkVisibility(
	const Data::ElementSet &elements, const Data::EdgeList &edges,
	IntElsMap &mapHitsElements) const
{
	//debug do not consider the mid point

	if (edges.empty())
	{
		return 0;
	}

	//endebug do not consider the mid point

	UInt maxHits = 0;

	for (Data::ElementSet::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		Data::Element *e = (*iter);

		UInt numHits = this->checkVisibility(e, edges);

		mapHitsElements.insert(std::make_pair(numHits, e));

		if (numHits > maxHits)
		{
			maxHits = numHits;
		}
	}

	return maxHits;
}*/

UInt MJMesh::BackTrackingFrontAdvancer::checkVisibility(
	const Data::FrontElementList &fels, const Data::EdgeList &edges,
	IntFElsMap &mapHitsFels)
{
	//debug do not consider the mid point

	if (edges.empty())
	{
		return 0;
	}

	//endebug do not consider the mid point

	UInt maxHits = 0;

	bool first = mapHitsFels.empty();

	for (Data::FrontElementList::const_iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		//std::cout << "checking visibility for fel " << fel->getId() << ", " << fel->getVertex(0)->getId() << ", " << fel->getVertex(1)->getId() << std::endl;
		//std::cout << "checking visibility for fel->vertices[0]->getPoint() = " << fel->getVertex(0)->getPoint() << std::endl;
		//std::cout << "checking visibility for fel->vertices[1]->getPoint() = " << fel->getVertex(1)->getPoint() << std::endl;

		UInt numHits = this->checkVisibility(fel, edges);

		UInt total = numHits;

		//Data::cout() << "checkVisibility(fels, edges, map) (1), fel " << fel->text() << " numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

		if (first)
		{
			mapHitsFels.insert(std::make_pair(numHits, fel));
		}
		else if (numHits > 0)
		{
			total = this->updateHits(numHits, fel, mapHitsFels);
		}

        //Data::cout() << "checkVisibility(fels, edges, map) (2), fel " << fel->text() << " numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

		if (total > this->maxHits)
		{
			this->maxHits = total;
		}

		//Data::cout() << "checkVisibility(fels, edges, map) (3), fel " << fel->text() << " numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

		if (numHits > maxHits)
		{
			maxHits = numHits;
		}

		//Data::cout() << "checkVisibility(fels, edges, map) (4), fel " << fel->text() << " numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;
	}

	//Data::cout() << "checkVisibility(fels, edges, map) (5), maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

	return maxHits;
}

UInt MJMesh::BackTrackingFrontAdvancer::updateHits(UInt add,
	Data::FrontElement *fel, IntFElsMap &mapHitsFels) const
{
	IntFElsMap::iterator it = mapHitsFels.begin();

	for (; it != mapHitsFels.end(); it++)
	{
		if ((*it).second == fel)
		{
			break;
		}
	}

	//Data::cout() << "updateHits(add, fel, map) (1), fel = " << fel->text() << ", add = " << add << ", (*it).first = " << (*it).first << ", this->maxHits = " << this->maxHits << Data::endl;

	UInt total = add + (*it).first;

	//Data::cout() << "updateHits(add, fel, map) (2), fel = " << fel->text() << ", total = " << total << ", this->maxHits = " << this->maxHits << Data::endl;

	if (total != (*it).first)
	{
		mapHitsFels.erase(it);
		mapHitsFels.insert(std::make_pair(total, fel));
	}

	return total;
}

UInt MJMesh::BackTrackingFrontAdvancer::checkInterception()
{
	Data::PolygonList newPolygons;

	for (Data::FrontElementList::iterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		bool found[3] = {false, false, false};

		for (Data::PolygonList::iterator iter2 = newPolygons.begin();
			 iter2 != newPolygons.end(); iter2++)
		{
			for (UInt i = 0; i < 3; i++)
			{
				if ((!found[i]) && ((*iter2)->have(fel->getVertex(i)->getPoint())) &&
					((*iter2)->have(fel->getVertex((i + 1)%3)->getPoint())))
				{
					found[i] = true;
				}
			}

			if ((found[0]) && (found[1]) && (found[2]))
			{
				break;
			}
		}

		for (UInt i = 0; i < 3; i++)
		{
			if (found[i])
			{
				continue;
			}

			Data::Triangle3D *t = new Data::Triangle3D(
				static_cast<Data::Point3D *>(fel->getVertex(i)->getPoint()),
				static_cast<Data::Point3D *>(fel->getVertex((i + 1)%3)->getPoint()),
				static_cast<Data::Point3D *>(this->mid));

			newPolygons.push_back(t);
		}
	}

	UInt maxHits = 0;

	for (Data::FrontElementList::iterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		UInt numHits = 0;

		MJMesh::FrontElement3D *fel = static_cast<MJMesh::FrontElement3D *>((*iter));

		for (Data::PolygonList::iterator iter2 = newPolygons.begin();
			 iter2 != newPolygons.end(); iter2++)
		{
			if (fel->intercept(static_cast<Data::Triangle3D *>((*iter2))))
			{
				numHits++;
			}
		}

		UInt total = numHits;

		//Data::cout() << "checkInterception() (1), fel = " << fel->text() << ", numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

		if (numHits > 0)
		{
			total = this->updateHits(numHits, fel, this->mapHitsFels);
		}

		//Data::cout() << "checkInterception() (2), fel = " << fel->text() << ", numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

		if (total > this->maxHits)
		{
			this->maxHits = total;
		}

		//Data::cout() << "checkInterception() (3), fel = " << fel->text() << ", numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

		if (numHits > maxHits)
		{
			maxHits = numHits;
		}

		//Data::cout() << "checkInterception() (4), fel = " << fel->text() << ", numHits = " << numHits << ", total = " << total << ", maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;
	}

	while (!newPolygons.empty())
	{
		delete newPolygons.front();

		newPolygons.pop_front();
	}

	//Data::cout() << "checkInterception() (5), maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

	return maxHits;
}

//debug
//#include "IO/MJMeshFileWriter.h"
//endebug

void MJMesh::BackTrackingFrontAdvancer::makeNewMesh()
{
	//debug do not consider the mid point

	if (!this->mid)
	{
		Data::Vertex *v = NULL;
		Data::FrontElement *fel = this->frontElements.front();

		for (Data::FrontElementList::iterator iter = ++this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			Data::FrontElement *f = (*iter);

			for (UInt i = 0; i < f->numVertices(); i++)
			{
				if (!fel->have(f->getVertex(i)))
				{
					v = f->getVertex(i);

					break;
				}
			}

			if (v)
			{
				break;
			}
		}

		Data::FrontElementList newFront;

		Data::Element *e = this->front->advance(fel, v, newFront);

		this->mesh->add(e);

		this->newElements.insert(e);

		//debug
		if (!newFront.empty())
		{
			std::cout << "ERROR: something is wrong in void MJMesh::BackTrackingFrontAdvancer::makeNewMesh()" << std::endl;
			/*std::cout << "ERROR (cont.): element " << e->text() << " generated from advancing front element " << fel->text() << " to vertex " << v->text() << std::endl;
			std::cout << "ERROR (cont.): but new front element(s) was (were) generated (which should not happen at this moment)" << std::endl;
			for (Data::FrontElementList::iterator iter = newFront.begin();
				 iter != newFront.end(); iter++)
			{
				std::cout << "ERROR (cont.): generated front element: " << (*iter)->text() << std::endl;
				//this->mesh->add((*iter));
				//this->newFrontElements.insert((*iter));
			}*/
		}
		//endebug

		return;
	}

	//endebug do not consider the mid point

	//insert the point
	MJMesh::Vertex *v = new MJMesh::Vertex(this->mid);

	/*//debug
	{
		MJMesh::Boundary b;

        for (Data::FrontElementList::iterator iter = this->frontElements.begin();
             iter != this->frontElements.end(); iter++)
        {
            b.add((*iter));

            for (UInt i = 0; i < (*iter)->numVertices(); i++)
            {
                if (!b.have((*iter)->getVertex(i)))
                {
                    b.add((*iter)->getVertex(i));
                }
            }
        }

        static UInt count = 0;

        std::stringstream str;
        str << "/tmp/cavity." << this->dimension << "djmsh";

        IO::MJMeshFileWriter writer;

        writer.setFilename(str.str());
        writer.setMesh(&b);
        writer.setDimension(this->dimension);
        writer.setTreeFactor((this->dimension == 2) ? 0.85 : 0.4);
        writer.setTolerance(0.0001);

        writer.execute();

        b.clear();
	}
	//endebug*/

	this->mesh->add(v);

	this->front->add(v);

	this->newVertices.insert(v);

	/*Data::VertexList debug;
	for (Data::Front::VertexIterator iter2 = this->front->vBegin();
		 iter2 != this->front->vEnd(); iter2++)
	{
		debug.push_back((*iter2));

		std::cout << "vertex " << (*iter2)->getId() << " in fronts";

		for (MJMesh::VertexSpecificData::FrontDataMap::iterator iter3 = static_cast<MJMesh::Vertex *>((*iter2))->data.fronts.begin();
			 iter3 != static_cast<MJMesh::Vertex *>((*iter2))->data.fronts.end(); iter3++)
		{
			std::cout << " " << (*iter3).first;
		}

		std::cout << std::endl;
	}*/

	//make the elements and front elements
	for (Data::FrontElementList::iterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		//std::cout << "advancing front " << this->front->size() << ", " << this->front->verticesSize() << std::endl;

		Data::FrontElementList newFront;//, oldFront;

		Data::Element *e = this->front->advance((*iter), v, newFront);

		this->mesh->add(e);

		//std::cout << e->getShape()->orientedSize() << std::endl;

		this->newElements.insert(e);

		while (!newFront.empty())
		{
			Data::FrontElement *e = newFront.back();
			newFront.pop_back();

			this->mesh->add(e);

			this->newFrontElements.insert(e);
		}
	}

	/*std::cout << "end advancing front " << this->front->size() << ", " << this->front->verticesSize() << std::endl;

	for (Data::VertexList::iterator iter2 = debug.begin();
		 iter2 != debug.end(); iter2++)
	{
		std::cout << "vertex " << (*iter2)->getId() << " in fronts";

		for (MJMesh::VertexSpecificData::FrontDataMap::iterator iter3 = static_cast<MJMesh::Vertex *>((*iter2))->data.fronts.begin();
			 iter3 != static_cast<MJMesh::Vertex *>((*iter2))->data.fronts.end(); iter3++)
		{
			std::cout << " " << (*iter3).first;
		}

		std::cout << std::endl;
	}*/
}

/*Data::Element *MJMesh::BackTrackingFrontAdvancer::findElementToProcess(
	IntElsMap &mapHitsElements, UInt maxHits,
	Data::FrontElementList &fels, bool ignoreBoundary) const
{
	for (UInt i = maxHits; i > 0; i--)
	{
		for (MJMesh::IntElsMap::const_iterator iter = mapHitsElements.equal_range(i).first;
			 iter != mapHitsElements.equal_range(i).second; iter++)
		{
			Data::Element *e = (*iter).second;

			//if the algorithm is in the improvement phase, a boundary element
			// cannot be removed. if the algorithm is in the back tracking
			// phase, a boundary element can be removed
			//p.s.: the method Boundary::have() has to be called because it sets
			//  the adjacency for the front element being tested

			//if ((!this->boundary->have(e, this->mesh, &fels)) || (this->maxBackTracks == 0) || (ignoreBoundary))
			if ((!this->boundary->have(e, this->mesh, &fels)) || (this->mode == FRONT_ADVANCING) || (ignoreBoundary))
			{
				return e;
			}

			fels.clear();
		}
	}

	return NULL;
}*/

/*Data::Element *MJMesh::BackTrackingFrontAdvancer::findElementToProcess(
	IntFElsMap &mapHitsFels, UInt maxHits, Data::FrontElementList &fels, bool ignoreBoundary) const
{
    //Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

	for (UInt i = maxHits; i > 0; i--)
	{
	    //Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), i (hits) = " << i << Data::endl;

		for (MJMesh::IntFElsMap::const_iterator iter = mapHitsFels.equal_range(i).first;
			 iter != mapHitsFels.equal_range(i).second; iter++)
		{
			Data::Element *e = this->adjacency((*iter).second);

			//Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), testing fel " << (*iter).second->text() << Data::endl;

			if (!e)
			{
				continue;
			}

			//Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), testing e " << e->text() << Data::endl;

			//if the algorithm is in the improvement phase, a boundary element
			// cannot be removed. if the algorithm is in the back tracking
			// phase, a boundary element can be removed
			//p.s.: the method Boundary::have() has to be called because it sets
			//  the adjacency for the front element being tested

			//if ((this->maxBackTracks == 0) || (!this->boundary->have(fel)))
			//if ((this->mode == FRONT_ADVANCING) || (!this->boundary->have(fel)))
			{
#if USE_DELETE_FELS
                */  /*//if (debug)
                {
                    std::stringstream str;
                    str << "btfa e = " << e->text() << std::endl;
                    std::cout << str.str();
                }*/  /*

                for (UInt i = 0; i < e->numVertices(); i++)
                {
                    static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(e->getVertex(i), *this->created.front(), true, this->boundary);
                    */  /*UInt debugc = static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(e->getVertex(i), *this->created.front(), true);

                    //if (debug)
                    {
                        std::stringstream str;
                        str << "vertex " << e->getVertex(i)->getId() <<
                            ", inboundary = " << std::boolalpha << this->boundary->have(e->getVertex(i)) <<
                            ", created " << debugc << " of " << static_cast<MJMesh::Vertex *>(e->getVertex(i))->numAdjacentFrontElements() << " fels" << std::endl;
                        std::cout << str.str();
                    }*/  /*
                }
#endif //#if USE_DELETE_FELS

				//if ((!e) || (!this->boundary->have(e, this->mesh, &fels)))
				//if ((!this->boundary->have(e, this->mesh, &fels)) ||  */ /*(this->maxBackTracks == 0) || */  /*(ignoreBoundary))
				if ((!this->boundary->have(e, this->mesh, &fels)) ||  */ /*(this->mode == FRONT_ADVANCING) || */  /*(ignoreBoundary))
				{
				    *//*Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), chose element " << e->text() << Data::endl;

				    Data::cout() << "  adj fels =";

				    for (Data::FrontElementList::iterator iter = fels.begin();
                         iter != fels.end(); iter++)
                    {
                        Data::cout() << " " << (*iter)->getId();
                    }

                    Data::cout() << Data::endl;*//*

					return e;
				}

				fels.clear();
			}
		}
	}

	//Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), no element chosen" << Data::endl;

	return NULL;
}*/

Data::Element *MJMesh::BackTrackingFrontAdvancer::tryToProcess(Data::FrontElement *fel,
    Data::FrontElementList &fels, bool ignoreBoundary) const
{
    Data::Element *e = this->adjacency(fel);

    //Data::cout() << "tryToProcess(map, maxHits, fels, ignoreBoundary), testing fel " << fel->text() << ", ignore = " << std::boolalpha << ignoreBoundary << Data::endl;

    if (!e)
    {
        fels.clear();

        return NULL;
    }

    //Data::cout() << "tryToProcess(map, maxHits, fels, ignoreBoundary), testing e " << e->text() << Data::endl;

    //if the algorithm is in the improvement phase, a boundary element
    // cannot be removed. if the algorithm is in the back tracking
    // phase, a boundary element can be removed
    //p.s.: the method Boundary::have() has to be called because it sets
    //  the adjacency for the front element being tested

    //if ((this->maxBackTracks == 0) || (!this->boundary->have(fel)))
    //if ((this->mode == FRONT_ADVANCING) || (!this->boundary->have(fel)))
    {
#if USE_DELETE_FELS
        /*//if (debug)
        {
            std::stringstream str;
            str << "btfa e = " << e->text() << std::endl;
            std::cout << str.str();
        }*/

        for (UInt i = 0; i < e->numVertices(); i++)
        {
            static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(e->getVertex(i), *this->created.front(), true, this->boundary);
            /*UInt debugc = static_cast<MJMesh::Mesh *>(this->mesh)->createAdjacentFrontElements(e->getVertex(i), *this->created.front(), true);

            //if (debug)
            {
                std::stringstream str;
                str << "vertex " << e->getVertex(i)->getId() <<
                    ", inboundary = " << std::boolalpha << this->boundary->have(e->getVertex(i)) <<
                    ", created " << debugc << " of " << static_cast<MJMesh::Vertex *>(e->getVertex(i))->numAdjacentFrontElements() << " fels" << std::endl;
                std::cout << str.str();
            }*/
        }
#endif //#if USE_DELETE_FELS

        //if ((!e) || (!this->boundary->have(e, this->mesh, &fels)))
        //if ((!this->boundary->have(e, this->mesh, &fels)) || /*(this->maxBackTracks == 0) || */(ignoreBoundary))
        if ((!this->boundary->have(e, this->mesh, &fels)) || /*(this->mode == FRONT_ADVANCING) || */(ignoreBoundary))
        {
            /*Data::cout() << "tryToProcess(map, maxHits, fels, ignoreBoundary), chose element " << e->text() << Data::endl;

            Data::cout() << "  adj fels =";

            for (Data::FrontElementList::iterator iter = fels.begin();
                 iter != fels.end(); iter++)
            {
                Data::cout() << " " << (*iter)->getId();
            }

            Data::cout() << Data::endl;*/

            return e;
        }
    }

    fels.clear();

    return NULL;
}

Data::Element *MJMesh::BackTrackingFrontAdvancer::findElementToProcess(
	IntFElsMap &mapHitsFels, UInt maxHits, Data::FrontElementList &fels, bool &ignoreBoundary) const
{
    //Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), maxHits = " << maxHits << ", this->maxHits = " << this->maxHits << Data::endl;

	for (UInt i = maxHits; i > 0; i--)
	{
	    //Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), i (hits) = " << i << Data::endl;

	    std::pair<MJMesh::IntFElsMap::const_iterator, MJMesh::IntFElsMap::const_iterator> p = mapHitsFels.equal_range(i);

		ignoreBoundary = false;

		for (MJMesh::IntFElsMap::const_iterator iter = p.first; iter != p.second; iter++)
		{
		    Data::Element *e = tryToProcess((*iter).second, fels, ignoreBoundary);

		    if (e)
            {
                return e;
            }
		}

		ignoreBoundary = true;

		for (MJMesh::IntFElsMap::const_iterator iter = p.first; iter != p.second; iter++)
		{
		    Data::Element *e = tryToProcess((*iter).second, fels, ignoreBoundary);

		    if (e)
            {
                return e;
            }
		}
	}

	//Data::cout() << "findElementToProcess(map, maxHits, fels, ignoreBoundary), no element chosen" << Data::endl;

	return NULL;
}

void MJMesh::BackTrackingFrontAdvancer::process(Data::Element *e,
	Data::FrontElementList &fels, Data::FrontElementList &front, bool ignoreBoundary)
{
	Data::FrontElementSet newFront;

	//std::cout << "processing e " << e->getId() << std::endl;

/*#if USE_DELETE_FELS
	MJMesh::MeshBackTracker::process(*this->created.front(),
        e, fels, this->oldElements,
		newFront, this->oldFrontElements, front,
		this->mesh);
#else*/
	MJMesh::MeshBackTracker::process(e, fels, this->oldElements,
		newFront, this->oldFrontElements, front,
		this->mesh);
/*#endif //#if USE_DELETE_FELS*/

	//if (this->maxBackTracks == 0)
	if (this->mode == FRONT_ADVANCING)
	{
		//added for the real back tracking front advancer, not just the
		// mesh back tracker.

		Data::FrontElementSet moreFront;

		for (Data::FrontElementSet::iterator iter = newFront.begin();
			 iter != newFront.end();)
		{
			//Data::ElementList adjacency = this->mesh->adjacency((*iter));

			bool hasAdjacent = false;

			//for (Data::ElementList::iterator iter2 = adjacency.begin();
			for (Data::ElementList::iterator iter2 = static_cast<MJMesh::FrontElement *>((*iter))->eBegin();
				 //iter2 != adjacency.end(); iter2++)
				 iter2 != static_cast<MJMesh::FrontElement *>((*iter))->eEnd(); iter2++)
			{
				//if (std::find(this->oldElements.begin(), this->oldElements.end(), (*iter2)) == this->oldElements.end())
				if (this->oldElements.find((*iter2)) == this->oldElements.end())
				{
					hasAdjacent = true;

					break;
				}
			}

			if ((!hasAdjacent) && (this->mergeFronts((*iter), moreFront)))
			{
				//front.push_back((*iter));

				iter = newFront.erase(iter);

				continue;
			}

			iter++;
		}

		newFront.insert(moreFront.begin(), moreFront.end());
	}

	//Data::VertexVector vertices = e->getVertices();

	for (Data::FrontElementList::iterator iter = front.begin();
		 iter != front.end(); iter++)
	{
		this->front->remove((*iter));
	}

	for (Data::FrontElementSet::iterator iter = newFront.begin();
		 iter != newFront.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);
		this->front->add(fel);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::Vertex *v = fel->getVertex(i);

			if (!this->front->have(v))
			{
				this->front->add(v);
			}
		}
	}

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		Data::Vertex *v = e->getVertex(i);

		if ((this->front->shouldRemove(v)) &&
			//((!ignoreBoundary) || (this->mesh->findAdjacentFrontElements(v).empty())))
			((!ignoreBoundary) || (static_cast<MJMesh::Vertex *>(v)->numAdjacentFrontElements() == 0)))
		{
		    /*if (v->getId() == 239)
            {
                std::cout << "debug" << std::endl;
                while (true);
            }*/

			MJMesh::MeshBackTracker::markAsOld(this->oldVertices, v, this->mesh);

			this->front->remove(v);
		}
	}
}

bool MJMesh::BackTrackingFrontAdvancer::mergeFronts(Data::FrontElement *fel, Data::FrontElementSet &newFront)
{
	if (this->boundary->have(fel))
	{
		return false;
	}

	Data::Front *front = NULL;

	for (Data::FrontList::iterator iter = this->fronts.begin();
		 iter != this->fronts.end(); iter++)
	{
		if ((*iter) == this->front)
		{
			continue;
		}

		if ((*iter)->have(fel))
		{
			front = (*iter);

			this->fronts.erase(iter);

			break;
		}
	}

	if (front)
	{
		for (Data::Front::ConstFrontElementIterator iter = front->eBegin(1);
			 iter != front->eEnd(1); iter++)
		{
			Data::FrontElement *e = (*iter);

			if (e == fel)
			{
				continue;
			}

			newFront.insert(e);
		}

		delete front;
	}

	MJMesh::MeshBackTracker::markAsOld(this->oldFrontElements, fel, this->mesh);

	return true;
}

//debug
//#include "IO/MJMeshFileWriter.h"
//#include <mpi.h>
//endebug

bool MJMesh::BackTrackingFrontAdvancer::backTrackOnce()
{
	switch (this->status.getType())
	{
	case MJMesh::BackTrackingFrontAdvancer::NOT_INIT:
	case MJMesh::BackTrackingFrontAdvancer::INIT:
	case MJMesh::BackTrackingFrontAdvancer::REJECTED:
	case MJMesh::BackTrackingFrontAdvancer::ENDED:
		break;

	case MJMesh::BackTrackingFrontAdvancer::NOT_INIT_BACK_TRACKING:
		this->elements.clear();
		this->fels.clear();
		this->edges.clear();
		this->adjacentElements.clear();
		this->frontVertices.clear();
		this->mapElementFront.clear();
		//this->mapHitsElements.clear();
		this->mapHitsFels.clear();
		this->mid = NULL;
		this->chosen = NULL;
		this->maxHits = 0;
		this->ignoreBoundary = false;

		this->status = MJMesh::BackTrackingFrontAdvancer::initBackTracking;

	case MJMesh::BackTrackingFrontAdvancer::INIT_BACK_TRACKING:
		this->findFrontElements();

		/*//debug
		//if (this->debug)
		{
			MJMesh::Boundary b;

			for (Data::FrontElementList::iterator iter = this->frontElements.begin();
				 iter != this->frontElements.end(); iter++)
			{
				b.add((*iter));

				for (UInt i = 0; i < (*iter)->numVertices(); i++)
				{
					if (!b.have((*iter)->getVertex(i)))
					{
						b.add((*iter)->getVertex(i));
					}
				}
			}

			static UInt count = 0;
			int rank = 0;
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);

			std::stringstream str;
			//str << "/tmp/cavity" << ++count << "." << this->dimension << "djmsh";
			str << "/tmp/cavity" << "-" << rank << "-" << ++count << "." << this->dimension << "djmsh";

			IO::MJMeshFileWriter writer;

			writer.setFilename(str.str());
			writer.setMesh(&b);
			writer.setDimension(this->dimension);
			writer.setTreeFactor((this->dimension == 2) ? 0.85 : 0.4);
			writer.setTolerance(0.0001);

			writer.execute();

			b.clear();
		}
		//endebug*/

		if (this->frontElements.size() < this->dimension + 1)
		{
			std::cout << "ERROR: less than " << this->dimension + 1 << " elements in front in back tracking" << std::endl;

			this->status = MJMesh::BackTrackingFrontAdvancer::rejected;

			return false;
		}

		//if an element appears 3 times (2D) or 4 times (3D), then it must be
		// removed, because all its vertices cannot be linked to the mid point
		// at the same time
		if (this->hasInnerElement())
		{
			this->status = MJMesh::BackTrackingFrontAdvancer::notInitBackTracking;

			return true;
		}

#if USE_GUI
		if (Data::isStepByStep())
		{
			for (Data::ElementList::iterator iter = this->elements.begin();
				 iter != this->elements.end(); iter++)
			{
				//(*iter)->setColor(0.0, 0.5, 0.5);

				(*iter)->highlight();
			}
		}
#endif //#if USE_GUI

		this->status = MJMesh::BackTrackingFrontAdvancer::adjElementsFound;

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::BackTrackingFrontAdvancer::ADJ_ELEMENTS_FOUND:
#if USE_GUI
		if (Data::isStepByStep())
		{
			for (Data::ElementList::iterator iter = this->elements.begin();
				 iter != this->elements.end(); iter++)
			{
				//(*iter)->setColor(1.0, 1.0, 1.0);

				(*iter)->unhighlight();
			}
		}
#endif //#if USE_GUI

		if (this->adjacentElements.empty())
		{
			//there are no adjacent elements to the front, meaning that the whole
			// mesh has been back tracked

			this->frontElements.clear();

			this->status = MJMesh::BackTrackingFrontAdvancer::rejected;

			return false;
		}

		//find the centroid of the vertices in the front
		//  OBS.: this line is here and not in method makeMid() because
		//  makeMid() is const
		this->frontVertices = this->front->getVertices();

		/*//debug
		for (Data::FrontElementList::iterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			std::cout << "fel in back tracking " << (*iter)->text() << std::endl;
		}
		for (Data::VertexList::iterator iter = this->frontVertices.begin();
			 iter != this->frontVertices.end(); iter++)
		{
			std::cout << "ver in back tracking " << (*iter)->text() << std::endl;
		}
		//endebug*/

		//this->mid = this->makeMid();
		this->makeMids();

#if USE_JMSH_DEBUG_MESSAGES
		if ((this->dimension == 3) && (mid))
		{
			std::cout << "markos debug, mid point is " << mid->getCoord(0) << ", " << mid->getCoord(1) << ", " << mid->getCoord(2) << std::endl;
		}
#endif //#if USE_JMSH_DEBUG_MESSAGES

		this->status = MJMesh::BackTrackingFrontAdvancer::midPointFound;

		//if (Data::isStepByStep()) break;

	case MJMesh::BackTrackingFrontAdvancer::MID_POINT_FOUND:
		//creates edges for visibility checks
		this->makeEdges();

		// visibility checks
		for (Data::PointList::iterator piter = this->mids.begin();
			 piter != this->mids.end(); piter++)
		{
			this->mid = (*piter);

			UInt hits = 0;

			//this->maxHits = this->checkVisibility(this->adjacentElements, this->edges, this->mapHitsElements);
			hits = this->checkVisibility(this->frontElements, this->edges[this->mid], this->mapHitsFels);

			//Data::cout() << "backTrackOnce() (1), hits = " << hits << ", this->maxHits = " << this->maxHits << Data::endl;

			if ((this->dimension == 3) && (this->maxHits == 0))
			//if (this->dimension == 3)
			{
				UInt nhits = this->checkInterception();

				//Data::cout() << "backTrackOnce() (2), hits = " << hits << ", nhits = " << nhits << ", this->maxHits = " << this->maxHits << Data::endl;

				if (nhits > hits)
				{
					hits = nhits;
				}

				//Data::cout() << "backTrackOnce() (3), hits = " << hits << ", nhits = " << nhits << ", this->maxHits = " << this->maxHits << Data::endl;
			}

			if (hits == 0)
			{
				this->maxHits = 0;

				break;
			}

			this->mid = NULL;
		}

		//Data::cout() << "backTrackOnce() (4), this->maxHits = " << this->maxHits << Data::endl;

		this->clearEdges();

		this->status = MJMesh::BackTrackingFrontAdvancer::visibilityChecked;

		//if (Data::isStepByStep()) break;

	case MJMesh::BackTrackingFrontAdvancer::VISIBILITY_CHECKED:

		while (!this->mids.empty())
		{
			if (this->mids.front() != this->mid)
			{
				delete this->mids.front();
			}

			this->mids.pop_front();
		}

		//back track or not (that is the question):
		// this method will succeed when all points in the front are visible to
		// their centroid.
		// this method will not succeed when:
		// 1. all the mesh has to be removed and, yet, the centroid cannot be
		//    inserted
		// 2. all the remaining elements to be removed belong to the boundary

		if (this->maxHits == 0)
		{
			//no element hit, meaning that the point can be inserted

			this->makeNewMesh();

			this->status = MJMesh::BackTrackingFrontAdvancer::ended;

			return false;
		}

		//delete this->mid;

		//this->mid = NULL;

		/*this->ignoreBoundary = false;
		//this->chosen = this->findElementToProcess(this->mapHitsElements, this->maxHits, this->fels, false);
		this->chosen = this->findElementToProcess(this->mapHitsFels, this->maxHits, this->fels, false);

		//if (*//*(this->maxBackTracks != 0) &&*//*
		if (*//*(this->mode == OPTIMIZATION) &&*//*
			(!this->chosen))
		{
			this->ignoreBoundary = true;
			this->chosen = this->findElementToProcess(this->mapHitsFels, this->maxHits, this->fels, true);
		}*/

		this->chosen = this->findElementToProcess(this->mapHitsFels, this->maxHits, this->fels, this->ignoreBoundary);

		if (!this->chosen)
		{
			//failed to find an element to back track

			this->frontElements.clear();

			this->status = MJMesh::BackTrackingFrontAdvancer::rejected;

			return false;
		}

#if USE_GUI
		this->chosen->highlight();
#endif //#if USE_GUI

		this->status = MJMesh::BackTrackingFrontAdvancer::processElementFound;

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::BackTrackingFrontAdvancer::PROCESS_ELEMENT_FOUND:
#if USE_GUI
		this->chosen->unhighlight();
#endif //#if USE_GUI

		this->process(this->chosen, this->fels, this->mapElementFront[this->chosen], this->ignoreBoundary);

		this->status = MJMesh::BackTrackingFrontAdvancer::notInitBackTracking;

		return true;
	}

	return false;
}

void MJMesh::BackTrackingFrontAdvancer::backTrack()
{
	//debug
	//std::cout << "debug, fel " << (*this->front->allEBegin())->text() << std::endl;
	//std::cout << "debug, front->size " << this->front->size() << std::endl;
	//this->debug = ((*this->front->allEBegin())->getId() == 100140);
	//endebug

	//while ((this->maxBackTracks == 0) || (this->step < this->maxBackTracks))
	while (this->step < this->maxBackTracks)
	{
		if (!this->backTrackOnce())
		{
			break;
		}

		if ((this->status.getType() == MJMesh::BackTrackingFrontAdvancer::ended.getType()) ||
			(this->status.getType() == MJMesh::BackTrackingFrontAdvancer::rejected.getType()))
		{
			break;
		}

		this->step++;

		if (this->step == this->maxBackTracks)
		{
			if (this->mode == OPTIMIZATION)
            {
                this->status = MJMesh::BackTrackingFrontAdvancer::rejected;
#if USE_JMSH_DEBUG_MESSAGES
                std::cout << "markos debug, rejecting improvement back tracking, reaching " << this->step << " of " << this->maxBackTracks << std::endl;
#endif //#if USE_JMSH_DEBUG_MESSAGES
            }
            else
            {
                this->status = MJMesh::BackTrackingFrontAdvancer::error;
            }

			break;
		}

		/*if (this->step == this->maxBackTracks)
		{
			this->status = MJMesh::BackTrackingFrontAdvancer::rejected;
#if USE_JMSH_DEBUG_MESSAGES
			std::cout << "markos debug, rejecting improvement back tracking, reaching " << this->step << " of " << this->maxBackTracks << std::endl;
#endif //#if USE_JMSH_DEBUG_MESSAGES
			break;
		}

		//debug
		if (this->step == 600) exit(1);
		//endebug*/

		//if (Data::isStepByStep()) break;
	}
}

/*Data::PointList MJMesh::BackTrackingFrontAdvancer::ideal(const Data::FrontElement *element, Real factor) const
{
	return Data::PointList();
}*/

Real MJMesh::BackTrackingFrontAdvancer::radius(const Data::FrontElement */*element*/) const
{
	return -1.0;
}

Data::Vertex *MJMesh::BackTrackingFrontAdvancer::best(const Data::FrontElement */*element*/) const
{
	return NULL;
}
