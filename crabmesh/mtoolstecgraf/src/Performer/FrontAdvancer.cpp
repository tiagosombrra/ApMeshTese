#include "Performer/FrontAdvancer.h"

#include "Data/State.h"
#include "Data/Point.h"
#include "Data/Box.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Front.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "Data/OStream.h"
#include "Performer/IdManager.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Performer;

const Performer::Status Performer::FrontAdvancer::notInit =
	Performer::Status(Performer::FrontAdvancer::NOT_INIT, "not initiated");
const Performer::Status Performer::FrontAdvancer::init =
	Performer::Status(Performer::FrontAdvancer::INIT, "initiated");
const Performer::Status Performer::FrontAdvancer::ended =
	Performer::Status(Performer::FrontAdvancer::ENDED, "ended");
const Performer::Status Performer::FrontAdvancer::rejected =
	Performer::Status(Performer::FrontAdvancer::REJECTED, "front element rejected twice");
const Performer::Status Performer::FrontAdvancer::error =
	Performer::Status(Performer::FrontAdvancer::ERROR, "error");

//kind of workaround to allow the use of std::algoritms for heap
/*bool Performer::better(const HeapWOP *wop1, const HeapWOP *wop2)
{
	return wop1->fa->better(wop2->rank, wop1->rank);
}*/

bool Performer::better(const HeapWOP &wop1, const HeapWOP &wop2)
{
	return wop1.fa->better(wop2.rank, wop1.rank);
}
//end kind of workaround to allow the use of std::algoritms for heap

/*Real Performer::FrontAdvancer::timeInit = 0.0;
Real Performer::FrontAdvancer::timeInitMakingTrees = 0.0;
Real Performer::FrontAdvancer::timeExecute = 0.0;
Real Performer::FrontAdvancer::timeEnd = 0.0;

Real Performer::FrontAdvancer::timeIdeal = 0.0;
Real Performer::FrontAdvancer::timeRadius = 0.0;
Real Performer::FrontAdvancer::timeRestricted = 0.0;
Real Performer::FrontAdvancer::timeCandidates = 0.0;
Real Performer::FrontAdvancer::timeFrontElementsInRange = 0.0;
Real Performer::FrontAdvancer::timeCandidatesBestValid = 0.0;
Real Performer::FrontAdvancer::timeIdealBestValid = 0.0;

Real Performer::FrontAdvancer::timeTopologyCandidates = 0.0;
Real Performer::FrontAdvancer::timeTopologyBestValid = 0.0;

Real Performer::FrontAdvancer::timeIsEmpty = 0.0;
Real Performer::FrontAdvancer::timeNext = 0.0;
Real Performer::FrontAdvancer::timeNextGeometry = 0.0;
Real Performer::FrontAdvancer::timeNextGeometryOut = 0.0;
Real Performer::FrontAdvancer::timeNextGeometryReject = 0.0;
Real Performer::FrontAdvancer::timeNextTopology = 0.0;
Real Performer::FrontAdvancer::timeNextTopologyReject = 0.0;
Real Performer::FrontAdvancer::timeBest = 0.0;
Real Performer::FrontAdvancer::timeReject = 0.0;
Real Performer::FrontAdvancer::timeStatusCheck = 0.0;
Real Performer::FrontAdvancer::timeErrorCheck = 0.0;
Real Performer::FrontAdvancer::timeAdvance = 0.0;

UInt Performer::FrontAdvancer::numBestWithInsertion = 0;
UInt Performer::FrontAdvancer::numBestWithoutInsertion = 0;*/

Performer::FrontAdvancer::FrontAdvancer(const Data::Boundary *boundary) :
	MeshGenerator()
{
	this->front = NULL;

	this->setBoundary(boundary);

	/*this->timeIdeal = timeRadius = timeRestricted = timeCandidates =
		timeFrontElementsInRange = timeCandidatesBestValid =
		timeIdealBestValid = 0.0;*/

	this->status = FrontAdvancer::notInit;
}

Performer::FrontAdvancer::~FrontAdvancer()
{

}

void Performer::FrontAdvancer::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

//markos debug
#if USE_JMSH_DEBUG_MESSAGES
#include "MJMesh/Front.h"
#endif //#if USE_JMSH_DEBUG_MESSAGES
//end markos debug

void Performer::FrontAdvancer::execute()
{
	//Real debugstart, start;
	/*//debug
	ULInt count, countAdvances;
	//Real timeIsEmpty, timeNext, timeBest, timeReject, timeStatusCheck, timeErrorCheck, timeAdvance;
	count = countAdvances = 0;
	//timeIsEmpty = timeNext = timeBest = timeReject = timeStatusCheck = timeErrorCheck = timeAdvance = 0.0;
	//endebug*/

	//Real debugstart = Data::time();
	//Real start = 0.0;

	switch (this->status.getType())
	{
	case FrontAdvancer::REJECTED:
	case FrontAdvancer::ENDED:
	case FrontAdvancer::ERROR:
		break;

	case FrontAdvancer::NOT_INIT:
		{
			//start = Data::time();

			if (!this->boundary)
			{
				this->status = FrontAdvancer::ended;

				return;
			}

			if (!this->mesh)
			{
				this->mesh = this->makeMesh();

				for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
					 iter != end; ++iter)
				{
					this->mesh->add((*iter), false);
				}

				for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
					 iter != end; ++iter)
				{
					this->mesh->add(static_cast<Data::FrontElement *>((*iter)));
				}
			}

			if (!this->front)
			{
				this->front = this->makeFront();
			}

			this->status = FrontAdvancer::init;

			//this->timeInit += Data::time() - debugstart;
			//debugstart = Data::time();
			//std::cout << "time for initializing GeometryFrontAdvancer = " << Data::time() - start << std::endl;
		}

	case FrontAdvancer::INIT:

		//start = Data::time();

		while (!this->isEmpty())
		{
			//timeIsEmpty += Data::time() - start;

			//count++;

			//cout << "debug step " << debug << endl;

			//cout << "debug " << this->front->size() << endl;

			//start = Data::time();

			Data::FrontElement *frontElement = this->next();

			//timeNext += Data::time() - start;

			if (!frontElement)
			{
				break;
			}

			/*if ((frontElement->getId() == 161) || (frontElement->getId() == 162))
			{
				Data::cout() << "debug, " << frontElement->getId() << Data::endl;
			}*/

#if USE_GUI
			frontElement->unhighlight();
#endif //#if USE_GUI

			//Data::cout() << "debug id = " << frontElement->getId() << Data::endl;

			//debug
#if USE_JMSH_DEBUG_MESSAGES
			if (frontElement->getShape()->dimension() == 3)
			{
				std::cout << "markos debug, face = " << frontElement->getVertex(0)->getId() - 1 << ", " << frontElement->getVertex(1)->getId() - 1 << ", " << frontElement->getVertex(2)->getId() - 1 << std::endl;
				std::cout << "markos debug, face->v1 = " << frontElement->getVertex(0)->getPoint()->getCoord(0) << ", " << frontElement->getVertex(0)->getPoint()->getCoord(1) << ", " << frontElement->getVertex(0)->getPoint()->getCoord(2) << std::endl;
				std::cout << "markos debug, face->v2 = " << frontElement->getVertex(1)->getPoint()->getCoord(0) << ", " << frontElement->getVertex(1)->getPoint()->getCoord(1) << ", " << frontElement->getVertex(1)->getPoint()->getCoord(2) << std::endl;
				std::cout << "markos debug, face->v3 = " << frontElement->getVertex(2)->getPoint()->getCoord(0) << ", " << frontElement->getVertex(2)->getPoint()->getCoord(1) << ", " << frontElement->getVertex(2)->getPoint()->getCoord(2) << std::endl;
				std::cout << "markos debug, mid  = " << frontElement->getShape()->mid().getCoord(0) << ", " << frontElement->getShape()->mid().getCoord(1) << ", " << frontElement->getShape()->mid().getCoord(2) << std::endl;
				std::cout << "markos debug, area = " << frontElement->getShape()->size() << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES
			//endebug

			//start = Data::time();

			Data::Vertex *best = this->best(frontElement);

			/*if ((frontElement->getId() == 469) && (best) && (best->getId() != 62))
			{
			    std::cout << "debug 2, " << frontElement->getId() << ", " << best->getId() << std::endl;
			}*/

			//timeBest += Data::time() - start;

			//start = Data::time();

			if (this->status.getType() == this->error.getType())
			{
				//timeStatusCheck += Data::time() - start;

				break;
			}

			//timeStatusCheck += Data::time() - start;

			if (!best)
			{
				//std::cout << "debug a best vertex was NOT found for front element " << frontElement->text() << std::endl;

				//start = Data::time();

				if (!this->reject(frontElement))
				{
					//timeReject += Data::time() - start;
					//std::cout << "debug, rejecting fel " << frontElement->text() << std::endl;

					this->status = FrontAdvancer::rejected;

					break;
				}

				//timeReject += Data::time() - start;

#if USE_GUI
				if (Data::isStepByStep())
				{
					if (this->next())
					{
						this->next()->highlight();
					}

					break;
				}
#endif //#if USE_GUI

				//start = Data::time();

				continue;
			}

			//start = Data::time();

			if (this->isThereError(frontElement, best))
			{
				//timeErrorCheck += Data::time() - start;

#if USE_GUI
				this->advance(frontElement, best);
#endif //#if USE_GUI

				this->status = FrontAdvancer::error;

				break;
			}

			//timeErrorCheck += Data::time() - start;

			//cout << "debug a best vertex was found for element " << frontElement->text() << endl;

			//countAdvances++;

			//start = Data::time();

			this->advance(frontElement, best);

			//timeAdvance += Data::time() - start;

#if USE_GUI
			if (Data::isStepByStep())
			{
				if (this->next())
				{
					this->next()->highlight();
				}

				break;
			}
#endif //#if USE_GUI

			//start = Data::time();
		}

		//timeIsEmpty += Data::time() - start;

		/*//debug
		if ((this->status.getType() != FrontAdvancer::error.getType()) &&
			(this->status.getType() != FrontAdvancer::rejected.getType()))
		{
			timeIsEmpty += Data::time() - start;
		}
		//endebug*/

		if ((this->status.getType() == FrontAdvancer::init.getType()) && (!this->next()))
		{
			this->status = FrontAdvancer::ended;
		}

		//this->timeExecute += Data::time() - debugstart;
		//std::cout << "time for executing GeometryFrontAdvancer = " << Data::time() - start << std::endl;
	}

	/*//debug
	{
		//Real total = timeIsEmpty + timeNext + timeBest + timeReject + timeStatusCheck + timeErrorCheck + timeAdvance;
		//std::cout << "number of advance trials = " << count << std::endl;
		//std::cout << "number of advances       = " << countAdvances << std::endl;
		std::cout << "time is empty      = " << timeIsEmpty
				  //<< ",\t average (trials) = " << timeIsEmpty/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeIsEmpty/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeIsEmpty/total
				  << std::endl;
		std::cout << "time next          = " << timeNext
				  //<< ",\t average (trials) = " << timeNext/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeNext/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeNext/total
				  << std::endl;
		std::cout << "time best          = " << timeBest
				  //<< ",\t average (trials) = " << timeBest/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeBest/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeBest/total
				  << std::endl;
		std::cout << "time reject        = " << timeReject
				  //<< ",\t average (trials) = " << timeReject/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeReject/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeReject/total
				  << std::endl;
		std::cout << "time status check  = " << timeStatusCheck
				  //<< ",\t average (trials) = " << timeStatusCheck/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeStatusCheck/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeStatusCheck/total
				  << std::endl;
		std::cout << "time error check   = " << timeErrorCheck
				  //<< ",\t average (trials) = " << timeErrorCheck/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeErrorCheck/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeErrorCheck/total
				  << std::endl;
		std::cout << "time advance       = " << timeAdvance
				  //<< ",\t average (trials) = " << timeAdvance/static_cast<Real>(count)
				  //<< ",\t average (advances) = " << timeAdvance/static_cast<Real>(countAdvances)
				  //<< ",\t proportion = " << timeAdvance/total
				  << std::endl;

		std::cout << "time ideal                = " << this->timeIdeal << std::endl;
		std::cout << "time radius               = " << this->timeRadius << std::endl;
		std::cout << "time restricted           = " << this->timeRestricted << std::endl;
		std::cout << "time candidates           = " << this->timeCandidates << std::endl;
		std::cout << "time fels in range        = " << this->timeFrontElementsInRange << std::endl;
		std::cout << "time best valid candidate = " << this->timeCandidatesBestValid << std::endl;
		std::cout << "time best valid ideal     = " << this->timeIdealBestValid << std::endl;
	}
	//endebug*/
}

#if (!USE_EXPERIMENTAL_MESH)
Data::Mesh *Performer::FrontAdvancer::makeMesh() const
{
	return new Data::Mesh();
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

Data::Front *Performer::FrontAdvancer::makeFront() const
{
	return new Data::Front(this->boundary);
}

Data::Vertex *Performer::FrontAdvancer::makeVertex(Data::Point *p) const
{
	return new Data::Vertex(p);
}

bool Performer::FrontAdvancer::isThereError(const Data::FrontElement */*fel*/, const Data::Vertex */*best*/) const
{
	return false;
}

Data::FrontElement *Performer::FrontAdvancer::next() const
{
	return this->front->next();
}

bool Performer::FrontAdvancer::isEmpty() const
{
	return this->front->isEmpty();
}

bool Performer::FrontAdvancer::reject(Data::FrontElement *e, UInt code) const
{
	return this->front->reject(e, code);
}

#if USE_DELETE_FELS
bool Performer::FrontAdvancer::shouldDelete(const Data::FrontElement *e) const
{
    return ((!this->front->have(e)) && (!this->boundary->have(e)));
}
#endif //#if USE_DELETE_FELS

#if USE_DELETE_FELS
Data::Element *Performer::FrontAdvancer::advance(Data::FrontElement *e, Data::Vertex *v,
    Data::FrontElementList &newFrontElements, Data::FrontElementList &oldFrontElements)
#else
Data::Element *Performer::FrontAdvancer::advance(Data::FrontElement *e, Data::Vertex *v, Data::FrontElementList &newFrontElements)
#endif //#if USE_DELETE_FELS
{
#if USE_DELETE_FELS
	return this->front->advance(e, v, newFrontElements, oldFrontElements);
#else
	return this->front->advance(e, v, newFrontElements);
#endif //#if USE_DELETE_FELS
}

void Performer::FrontAdvancer::advance(Data::FrontElement *e, Data::Vertex *v)
{
	//cout << "debug best vertex = " << v->text() << endl;

	Data::FrontElementList newFrontElements;

#if USE_GUI
	this->front->freeze();
	this->mesh->freeze();
#endif //#if USE_GUI

#if USE_DELETE_FELS
    Data::FrontElementList oldFrontElements;

	Data::Element *element = this->advance(e, v, newFrontElements, oldFrontElements);
#else
	Data::Element *element = this->advance(e, v, newFrontElements);
#endif //#if USE_DELETE_FELS

	//if (v->getId() == 0)
	if (!this->mesh->have(v))
	{
		this->mesh->add(v);

		v->setId(this->idManager->next(0));
	}

	//std::cout << "debug advanced fel " << e->getId() << " to v " << v->getId() << " generating fels ";

	//cout << "debug front advanced and created " << newFrontElements.size() << " new front elements" << endl;

#if USE_JMSH_DEBUG_MESSAGES
	if (v->getPoint()->dimension() == 3)
	{
		std::cout << "markos debug, adv to pt = " << v->getId() - 1 << ", " << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ", " << v->getPoint()->getCoord(2) << std::endl;
		//Data::PointList ps;
		//std::cout << "markos debug, solid angle = " << frontElement->rank(best->getPoint(), ps) << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	while (!newFrontElements.empty())
	{
		newFrontElements.front()->setId(this->idManager->next(1));

#if USE_JMSH_DEBUG_MESSAGES
		if (v->getPoint()->dimension() == 3)
		{
			std::cout << "markos debug, pushing face " << newFrontElements.front()->getVertex(0)->getId() - 1
				  << ", " << newFrontElements.front()->getVertex(1)->getId() - 1
				  << ", " << newFrontElements.front()->getVertex(2)->getId() - 1 << std::endl;
		}
#endif //#if USE_JMSH_DEBUG_MESSAGES

		this->mesh->add(static_cast<Data::FrontElement *>(newFrontElements.front()));

		//std::cout << newFrontElements.front()->getId() << " ";

		//cout << "debug new front element = " << newFrontElements.front()->text() << endl;

		newFrontElements.pop_front();
	}

	element->setId(this->idManager->next(2));

	//std::cout << "and el " << element->getId() << std::endl;

	this->mesh->add(element);

	//cout << "debug element " << element->text() << " added to mesh" << endl;

#if USE_DELETE_FELS
    oldFrontElements.push_back(e);

    while (!oldFrontElements.empty())
    {
        Data::FrontElement *fel = oldFrontElements.front();

        /*if ((fel->getId() == 161) || (fel->getId() == 162))
        {
            Data::cout() << "oldfel " << fel->text() << " when fel " << e->text() << " advanced to v " << v->text() << Data::endl;
        }*/

        oldFrontElements.pop_front();

        if (!this->shouldDelete(fel))
        {
            continue;
        }

        if (this->mesh->have(fel))
        {
            this->mesh->remove(fel);
        }

        delete fel;
    }
#endif //#if USE_DELETE_FELS

#if USE_GUI
	this->front->unfreeze();
	this->mesh->unfreeze();
#endif //#if USE_GUI
}

Real Performer::FrontAdvancer::rank(const Data::FrontElement *e, const Data::Vertex *v,
	const Data::PointList &ideal) const
{
	return this->rank(e, v->getPoint(), ideal);
}

Real Performer::FrontAdvancer::rank(const Data::FrontElement *e, const Data::Point *p,
	const Data::PointList &ideal) const
{
	return e->rank(p, ideal);
}

void Performer::FrontAdvancer::restricted(const Data::FrontElement */*e*/, Data::PointList &/*ideal*/, Real /*radius*/) const
{

}

Data::VertexList Performer::FrontAdvancer::candidates(const Data::FrontElement */*element*/) const
{
	return this->front->getVertices();
}

Data::VertexList Performer::FrontAdvancer::candidates(const Data::PointList &ideal, Real radius) const
{
	return this->front->verticesInRange(ideal, radius + Data::getTolerance());
}

Data::VertexList Performer::FrontAdvancer::candidates(const Data::PointList &ideal, Real radius, Data::VertexList &vertices) const
{
	return this->front->verticesInRange(ideal, radius + Data::getTolerance(), vertices);
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, const Data::FrontElementList &list) const
{
	return !this->front->intercept(element, list);
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element) const
{
	return !this->front->intercept(element);
}

bool Performer::FrontAdvancer::isValid(const Data::Point *point, const Data::FrontElement *e, const Data::FrontElementList &list) const
{
	return !(this->front->intercept(e, point, list) || this->mesh->in(point));
}

bool Performer::FrontAdvancer::isValid(const Data::Point *point, const Data::FrontElement *e) const
{
	return !(this->front->intercept(e, point) || this->mesh->in(point));
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point) const
{
	//std::cout << "\ncreating temporary element" << std::endl;

#if USE_C__11
	Data::Vertex *v = NULL;
	Data::FrontElementList *els = NULL;
	Data::Element *e = NULL;

	std::tie(v, els, e) = this->makeElement(element, point);
#else
	Data::ElementTuple t = this->makeElement(element, point);

	Data::Vertex *v = t.v;
	Data::FrontElementList *els = t.fes;
	Data::Element *e = t.e;
#endif //#if USE_C__11

	bool valid = true;

	if ((!e) || (this->front->intercept(e)))
	{
		valid = false;
	}

	if (e)
	{
		delete e;
	}

	while (!els->empty())
	{
		Data::FrontElement *e = els->back();
		els->pop_back();

		if ((valid) && (!this->isValid(e)))
		{
			valid = false;
		}

		delete e;
	}

	delete els;

	v->setPoint(NULL);
	delete v;

	//std::cout << "deleting temporary element\n" << std::endl;

	return valid;

	/*Data::Vertex *v = NULL;

	Data::FrontElementList elements = this->makeFrontElements(element, point, v);
	Data::Element *el = this->makeElement(element, elements, v);

	bool valid = true;

	if (!this->front->intercept(el))
	{
		valid = false;
	}

	delete el;

	while (!elements.empty())
	{
		Data::FrontElement *e = elements.back();
		elements.pop_back();

		if ((valid) && (!this->isValid(e)))
		{
			valid = false;
		}

		delete e;
	}

	v->setPoint(NULL);

	delete v;

	return valid;*/
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point,
	const Data::VertexList &vertices) const
{
	//std::cout << "\ncreating temporary element" << std::endl;

#if USE_C__11
	Data::Vertex *v = NULL;
	Data::FrontElementList *els = NULL;
	Data::Element *e = NULL;

	std::tie(v, els, e) = this->makeElement(element, point);
#else
	Data::ElementTuple t = this->makeElement(element, point);

	Data::Vertex *v = t.v;
	Data::FrontElementList *els = t.fes;
	Data::Element *e = t.e;
#endif //#if USE_C__11

	bool valid = true;

	if ((!e) || (this->front->intercept(e, vertices)))
	{
		valid = false;
	}

	if (e)
	{
		delete e;
	}

	while (!els->empty())
	{
		Data::FrontElement *e = els->back();
		els->pop_back();

		if ((valid) && (!this->isValid(e)))
		{
			valid = false;
		}

		delete e;
	}

	delete els;

	v->setPoint(NULL);
	delete v;

	//std::cout << "deleting temporary element\n" << std::endl;

	return valid;
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point,
	const Data::FrontElementList &frontElements) const
{
	//std::cout << "\ncreating temporary element" << std::endl;

#if USE_C__11
	Data::Vertex *v = NULL;
	Data::FrontElementList *els = NULL;
	Data::Element *e = NULL;

	std::tie(v, els, e) = this->makeElement(element, point);
#else
	Data::ElementTuple t = this->makeElement(element, point);

	Data::Vertex *v = t.v;
	Data::FrontElementList *els = t.fes;
	Data::Element *e = t.e;
#endif //#if USE_C__11

	bool valid = true;

	if ((!e) || (this->front->intercept(e)))
	{
		valid = false;
	}

	if (e)
	{
		delete e;
	}

	while (!els->empty())
	{
		Data::FrontElement *e = els->back();
		els->pop_back();

		if ((valid) && (!this->isValid(e, frontElements)))
		{
			valid = false;
		}

		delete e;
	}

	delete els;

	v->setPoint(NULL);
	delete v;

	//std::cout << "deleting temporary element\n" << std::endl;

	return valid;
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point,
	const Data::VertexList &vertices, const Data::FrontElementList &frontElements) const
{
	//std::cout << "\ncreating temporary element" << std::endl;

#if USE_C__11
	Data::Vertex *v = NULL;
	Data::FrontElementList *els = NULL;
	Data::Element *e = NULL;

	std::tie(v, els, e) = this->makeElement(element, point);
#else
	Data::ElementTuple t = this->makeElement(element, point);

	Data::Vertex *v = t.v;
	Data::FrontElementList *els = t.fes;
	Data::Element *e = t.e;
#endif //#if USE_C__11

	bool valid = true;

	if ((!e) || (this->front->intercept(e, vertices)))
	{
		valid = false;
	}

	if (e)
	{
		delete e;
	}

	while (!els->empty())
	{
		Data::FrontElement *e = els->back();
		els->pop_back();

		if ((valid) && (!this->isValid(e, frontElements)))
		{
			valid = false;
		}

		delete e;
	}

	delete els;

	v->setPoint(NULL);
	delete v;

	//std::cout << "deleting temporary element\n" << std::endl;

	return valid;
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, bool inElement, Real radius) const
{
	//std::cout << "debug isvalid, front element = " << element->text() << std::endl;
	//std::cout << "debug isvalid, point         = " << point->text() << std::endl;
	//std::cout << "debug isvalid, inElement = " << (inElement ? "true" : "false") << std::endl;

	//if ((!inElement) && (!this->isValid(element, point, radius)))
	if (!this->isValid(element, point, radius))
	{
		//std::cout << "rejected in first this->isValid(): point too close of front element" << std::endl;

		return false;
	}

	if (!this->isValid(element, point))
	{
		//std::cout << "rejected in second this->isValid(): newly inserted element would intercept existing elements" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element " << endl;

	if (!inElement)
	{
		return true;
	}

	//cout << "debug isvalid, point is valid for element, inElement is true" << endl;

	if (!this->isValid(point, element))
	{
		//std::cout << "rejected in third this->isValid(): newly inserted point would intercept front or elements adjacent to front" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element and mesh" << endl;

	return true;
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, bool inElement,
	const Data::VertexList &vertices, Real radius) const
{
	//std::cout << "debug isvalid, front element = " << element->text() << std::endl;
	//std::cout << "debug isvalid, point         = " << point->text() << std::endl;
	//std::cout << "debug isvalid, inElement = " << (inElement ? "true" : "false") << std::endl;

	//if ((!inElement) && (!this->isValid(element, point, radius)))
	if (!this->isValid(element, point, radius))
	{
		//std::cout << "rejected in first this->isValid(): point too close of front element" << std::endl;

		return false;
	}

	if (!this->isValid(element, point, vertices))
	{
		//std::cout << "rejected in second this->isValid(): newly inserted element would intercept existing elements" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element " << endl;

	if (!inElement)
	{
		return true;
	}

	//cout << "debug isvalid, point is valid for element, inElement is true" << endl;

	if (!this->isValid(point, element))
	{
		//std::cout << "rejected in third this->isValid(): newly inserted point would intercept front or elements adjacent to front" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element and mesh" << endl;

	return true;
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, bool inElement,
	const Data::FrontElementList &frontElements, Real radius) const
{
	//std::cout << "debug isvalid, front element = " << element->text() << std::endl;
	//std::cout << "debug isvalid, point         = " << point->text() << std::endl;
	//std::cout << "debug isvalid, inElement = " << (inElement ? "true" : "false") << std::endl;

	//if ((!inElement) && (!this->isValid(element, point, radius)))
	if (!this->isValid(element, point, radius))
	{
		//std::cout << "rejected in first this->isValid(): point too close of front element" << std::endl;

		return false;
	}

	if (!this->isValid(element, point, frontElements))
	{
		//std::cout << "rejected in second this->isValid(): newly inserted element would intercept existing elements" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element " << endl;

	if (!inElement)
	{
		return true;
	}

	//cout << "debug isvalid, point is valid for element, inElement is true" << endl;

	if (!this->isValid(point, element, frontElements))
	{
		//std::cout << "rejected in third this->isValid(): newly inserted point would intercept front or elements adjacent to front" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element and mesh" << endl;

	return true;
}

bool Performer::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, bool inElement,
	const Data::VertexList &vertices, const Data::FrontElementList &frontElements, Real radius) const
{
	//std::cout << "debug isvalid, front element = " << element->text() << std::endl;
	//std::cout << "debug isvalid, point         = " << point->text() << std::endl;
	//std::cout << "debug isvalid, inElement = " << (inElement ? "true" : "false") << std::endl;

	//if ((!inElement) && (!this->isValid(element, point, radius)))
	if (!this->isValid(element, point, radius))
	{
		//std::cout << "rejected in first this->isValid(): point too close of front element" << std::endl;

		return false;
	}

	if (!this->isValid(element, point, vertices, frontElements))
	{
		//std::cout << "rejected in second this->isValid(): newly inserted element would intercept existing elements" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element " << endl;

	if (!inElement)
	{
		return true;
	}

	//cout << "debug isvalid, point is valid for element, inElement is true" << endl;

	if (!this->isValid(point, element, frontElements))
	{
		//std::cout << "rejected in third this->isValid(): newly inserted point would intercept front or elements adjacent to front" << std::endl;

		return false;
	}

	//cout << "debug isvalid, point is valid for element and mesh" << endl;

	return true;
}

Data::VertexList Performer::FrontAdvancer::valid(const Data::FrontElement *element, Data::VertexList &candidates, Real radius) const
{
	Data::VertexList valid;

	while (!candidates.empty())
	{
		Data::Vertex *candidate = candidates.back();
		candidates.pop_back();

		if (this->isValid(element, candidate->getPoint(), false, radius))
		{
			valid.push_back(candidate);
		}
	}

	return valid;
}

Data::VertexList Performer::FrontAdvancer::valid(const Data::FrontElement *element, Data::PointList &candidates) const
{
	//cout << "debug validity check for newly inserted points, candidates = " << candidates.size() << endl;

	Data::VertexList valid;

	while (!candidates.empty())
	{
		Data::Point *candidate = candidates.back();
		candidates.pop_back();

		//cout << "debug checking for point " << candidate->text() << endl;

		if (this->isValid(element, candidate, true))
		{
			valid.push_back(this->makeVertex(candidate));
		}
		else
		{
			delete candidate;
		}

		//cout << "debug checked point " << endl;
	}

	return valid;
}

void Performer::FrontAdvancer::sortBest(const Data::FrontElement *element, Data::PointList &candidates) const
{
	if (candidates.size() <= 1)
	{
		return;
	}

	Data::PointList sorted;

	for (Data::PointList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		Data::Point *p = (*iter);

		Data::PointList::iterator chosen = sorted.end();

		Real rank = this->rank(element, p, candidates);

		for (Data::PointList::iterator iter = sorted.begin();
			 iter != sorted.end(); iter++)
		{
			Real vRank = this->rank(element, (*iter), candidates);

			if (this->better(vRank, rank))
			{
				chosen = iter;
			}
		}

		sorted.insert(chosen, p);
	}

	candidates.swap(sorted);
}

void Performer::FrontAdvancer::sortBest(const Data::FrontElement *element, Data::VertexList &candidates,
	const Data::PointList &ideal, Data::RealVector &ranks) const
{
	if (candidates.size() <= 1)
	{
		return;
	}

	Data::VertexList sorted;

	for (Data::VertexList::iterator iter2 = candidates.begin();
		 iter2 != candidates.end(); iter2++)
	{
		Data::Vertex *v = (*iter2);

		Data::VertexList::iterator chosen = sorted.end();
		Data::RealVector::iterator chosenRank = ranks.end();
		Data::RealVector::iterator rankIter = ranks.begin();
		UInt i = 0;

		Real rank = this->rank(element, v, ideal);

		for (Data::VertexList::iterator iter = sorted.begin();
			 iter != sorted.end(); iter++, rankIter++, i++)
		{
			Real vRank = ranks[i];

			if (this->better(vRank, rank))
			{
				chosen = iter;
				chosenRank = rankIter;
			}
		}

		sorted.insert(chosen, v);
		ranks.insert(chosenRank, rank);
	}

	candidates.swap(sorted);
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element, Data::PointList &candidates) const
{
	if (candidates.empty())
	{
		return NULL;
	}

	if (candidates.size() == 1)
	{
		Data::Vertex *v = NULL;

		if (this->isValid(element, candidates.front(), true))
		{
			v = this->makeVertex(candidates.front());
		}
		else
		{
			delete candidates.front();
		}

		candidates.clear();

		return v;
	}

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//this->sortBest(element, candidates);

	for (Data::PointList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		//HeapWOP *wop = new HeapWOP();
		HeapWOP wop;

		//wop->p = (*iter);
		//wop->rank = this->rank(element, wop->p, candidates);
		//wop->fa = this;
		wop.p = (*iter);
		wop.rank = this->rank(element, wop.p, candidates);
		wop.fa = this;

		cands.push_back(wop);
	}

	std::make_heap(cands.begin(), cands.end(), Performer::better);

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Point *candidate = wop->p;
		Data::Point *candidate = wop.p;

		//delete wop;

		if ((!best) &&
			(this->isValid(element, candidate, true)))
		{
			best = this->makeVertex(candidate);
		}
		else
		{
			delete candidate;
		}
	}

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element, Data::PointList &candidates,
	const Data::VertexList &vertices) const
{
	if (candidates.empty())
	{
		return NULL;
	}

	if (candidates.size() == 1)
	{
		Data::Vertex *v = NULL;

		if (this->isValid(element, candidates.front(), true, vertices))
		{
			v = this->makeVertex(candidates.front());
		}
		else
		{
			delete candidates.front();
		}

		candidates.clear();

		return v;
	}

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//this->sortBest(element, candidates);

	for (Data::PointList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		//HeapWOP *wop = new HeapWOP();
		HeapWOP wop;

		//wop->p = (*iter);
		//wop->rank = this->rank(element, wop->p, candidates);
		//wop->fa = this;
		wop.p = (*iter);
		wop.rank = this->rank(element, wop.p, candidates);
		wop.fa = this;

		cands.push_back(wop);
	}

	std::make_heap(cands.begin(), cands.end(), Performer::better);

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Point *candidate = wop->p;
		Data::Point *candidate = wop.p;

		//delete wop;

		if ((!best) &&
			(this->isValid(element, candidate, true, vertices)))
		{
			best = this->makeVertex(candidate);
		}
		else
		{
			delete candidate;
		}
	}

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element, Data::PointList &candidates,
	const Data::FrontElementList &frontElements) const
{
	if (candidates.empty())
	{
		return NULL;
	}

	if (candidates.size() == 1)
	{
		Data::Vertex *v = NULL;

		if (this->isValid(element, candidates.front(), true, frontElements))
		{
			v = this->makeVertex(candidates.front());
		}
		else
		{
			delete candidates.front();
		}

		candidates.clear();

		return v;
	}

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//this->sortBest(element, candidates);

	for (Data::PointList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		//HeapWOP *wop = new HeapWOP();
		HeapWOP wop;

		//wop->p = (*iter);
		//wop->rank = this->rank(element, wop->p, candidates);
		//wop->fa = this;
		wop.p = (*iter);
		wop.rank = this->rank(element, wop.p, candidates);
		wop.fa = this;

		cands.push_back(wop);
	}

	std::make_heap(cands.begin(), cands.end(), Performer::better);

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Point *candidate = wop->p;
		Data::Point *candidate = wop.p;

		//delete wop;

		if ((!best) &&
			(this->isValid(element, candidate, true, frontElements)))
		{
			best = this->makeVertex(candidate);
		}
		else
		{
			delete candidate;
		}
	}

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element, Data::PointList &candidates,
	const Data::VertexList &vertices, const Data::FrontElementList &frontElements) const
{
	if (candidates.empty())
	{
		return NULL;
	}

	if (candidates.size() == 1)
	{
		Data::Vertex *v = NULL;

		if (this->isValid(element, candidates.front(), true, vertices, frontElements))
		{
			v = this->makeVertex(candidates.front());
		}
		else
		{
			delete candidates.front();
		}

		candidates.clear();

		return v;
	}

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//this->sortBest(element, candidates);

	for (Data::PointList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		//HeapWOP *wop = new HeapWOP();
		HeapWOP wop;

		//wop->p = (*iter);
		//wop->rank = this->rank(element, wop->p, candidates);
		//wop->fa = this;
		wop.p = (*iter);
		wop.rank = this->rank(element, wop.p, candidates);
		wop.fa = this;

		cands.push_back(wop);
	}

	std::make_heap(cands.begin(), cands.end(), Performer::better);

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Point *candidate = wop->p;
		Data::Point *candidate = wop.p;

		//delete wop;

		if ((!best) &&
			(this->isValid(element, candidate, true, vertices, frontElements)))
		{
			best = this->makeVertex(candidate);
		}
		else
		{
			delete candidate;
		}
	}

	return best;
}

//markos debug
#if USE_JMSH_DEBUG_MESSAGES
#include "MJMesh/FrontElement.h"
#endif //#if USE_JMSH_DEBUG_MESSAGES
//end markos debug

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element,
	Data::VertexList &candidates, const Data::PointList &ideal, Real radius) const
{
    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid finding best for fel " << element->text() << std::endl;
    }*/

	if (candidates.empty())
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid no candidates" << std::endl;
        }*/

		return NULL;
	}

	if (candidates.size() == 1)
	{
		/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid one candidate" << std::endl;
        }*/

		Data::Vertex *v = NULL;
		bool found = false;

		for (UInt i = 0; i < element->numVertices(); i++)
		{
			if (candidates.front() == element->getVertex(i))
			{
				found = true;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " belongs to fel" << std::endl;
                }*/

				break;
			}
		}

		if (!found)
		{
#if USE_JMSH_DEBUG_MESSAGES
			if (candidates.front()->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(candidates.front()->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << candidates.front()->getPoint()->getCoord(0) << ", " << candidates.front()->getPoint()->getCoord(1) << ", " << candidates.front()->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, candidates.front(), ideal) << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			if (this->isValid(element, candidates.front()->getPoint(), false, radius))
			{
				v = candidates.front();

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << v->text() << " is valid" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (v->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, candidate = " << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ", " << v->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, v, ideal) << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES
			}
			/*else
            {
                if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " is NOT valid" << std::endl;
                }
            }*/
		}

		return v;
	}

	/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid several candidates" << std::endl;
    }*/

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//Data::RealVector ranks(candidates.size(), 0.0);

	//this->sortBest(element, candidates, ideal, ranks);
	//if (candidates.size() > 1)
	{
		for (Data::VertexList::iterator iter = candidates.begin();
			 iter != candidates.end(); iter++)
		{
			bool found = false;

			for (UInt i = 0; i < element->numVertices(); i++)
			{
				if ((*iter) == element->getVertex(i))
				{
					found = true;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " belongs to fel" << std::endl;
                    }*/

					break;
				}
			}

			if (found)
			{
				continue;
			}

			Real rank = this->rank(element, (*iter), ideal);

			if (std::isnan(rank))
            {
                //could happen for vertices that have the same position as one
                //  of the vertices of the given front element

                /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank nan" << std::endl;
                }*/

                continue;
            }

            /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank " << rank << std::endl;
            }*/

			//HeapWOP *wop = new HeapWOP();
			HeapWOP wop;

			//wop->v = (*iter);
			//wop->rank = rank;
			//wop->fa = this;
			wop.v = (*iter);
			wop.rank = rank;
			wop.fa = this;

			/*if (element->getId() == 469)
            {
                std::cout << "debug, e " << element->getId() << ", v " << wop->v->getId() << ", rank " << wop->rank << std::endl;
            }*/

#if USE_JMSH_DEBUG_MESSAGES
			if (wop->v->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(wop->v->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << wop->v->getPoint()->getCoord(0) << ", " << wop->v->getPoint()->getCoord(1) << ", " << wop->v->getPoint()->getCoord(2) << ", " << "metric = " << wop->rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			cands.push_back(wop);
		}

		std::make_heap(cands.begin(), cands.end(), Performer::better);
	}

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Vertex *candidate = wop->v;
		Data::Vertex *candidate = wop.v;

		//Real rank = wop->rank;
		Real rank = wop.rank;

		//delete wop;

		if (this->isValid(element, candidate->getPoint(), false, radius))
		{
		    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is valid" << std::endl;
            }*/

#if USE_JMSH_DEBUG_MESSAGES
			if (candidate->getPoint()->dimension() == 3)
			{
				std::cout << "markos debug, candidate = " << candidate->getPoint()->getCoord(0) << ", " << candidate->getPoint()->getCoord(1) << ", " << candidate->getPoint()->getCoord(2) << ", " << "metric = " << rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

            /*if (element->getId() == 469)
            {
                std::cout << "debug, e " << element->getId() << ", cand " << candidate->getId() << ", rank " << rank << ", valid" << std::endl;
            }*/

			while (!cands.empty())
			{
				std::pop_heap(cands.begin(), cands.end(), Performer::better);

				wop = cands.back();
				cands.pop_back();

				//Data::Vertex *next = wop->v;
				Data::Vertex *next = wop.v;

				//Real nextRank = wop->rank;
				Real nextRank = wop.rank;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has rank " << nextRank << std::endl;
                }*/

				/*if (element->getId() == 469)
                {
                    std::cout << "debug, e " << element->getId() << ", next " << next->getId() << ", rank " << nextRank << std::endl;
                }*/

				//delete wop;

				if (!this->equal(nextRank, rank))
				{
				    /*if (element->getId() == 469)
                    {
                        std::cout << "debug, e " << element->getId() << ", different ranks" << std::endl;
                    }*/

                    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has different rank" << std::endl;
                    }*/

					break;
				}

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has equal rank" << std::endl;
                }*/

				/*if (element->getId() == 469)
                {
                    std::cout << "debug, e " << element->getId() << ", equal ranks" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (candidate->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, equal metric candidate = " << next->getPoint()->getCoord(0) << ", " << next->getPoint()->getCoord(1) << ", " << next->getPoint()->getCoord(2) << ", " << "metric = " << nextRank << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES

				//if ((candidate->getPoint()->match(next->getPoint())) &&
				if ((candidate->getPoint()->equal(next->getPoint())) &&
					(this->better(element, next, candidate)) &&
					(this->isValid(element, next->getPoint(), false, radius)))
				{
					candidate = next;

					rank = nextRank;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is better" << std::endl;
                    }*/

					/*if (element->getId() == 469)
                    {
                        std::cout << "debug, e " << element->getId() << ", next is better" << std::endl;
                    }*/

					break;
				}
				/*else
                {
                    if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is worse or not valid" << std::endl;
                    }
                }*/

				/*if (element->getId() == 469)
                {
                    std::cout << "debug, e " << element->getId() << ", cand is better" << std::endl;
                }*/
			}

			best = candidate;

			/*if (element->getId() == 469)
            {
                std::cout << "debug, e " << element->getId() << ", best " << best->getId() << ", rank " << rank << std::endl;
            }*/

			break;
		}
		/*else
        {
            if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is NOT valid" << std::endl;
            }
        }*/
	}

	/*while (!cands.empty())
	{
		delete cands.back();

		cands.pop_back();
	}*/

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element,
	Data::VertexList &candidates, const Data::PointList &ideal,
	const Data::VertexList &vertices, Real radius) const
{
    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid finding best for fel " << element->text() << std::endl;
    }*/

	if (candidates.empty())
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid no candidates" << std::endl;
        }*/

		return NULL;
	}

	if (candidates.size() == 1)
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid one candidate" << std::endl;
        }*/

		Data::Vertex *v = NULL;
		bool found = false;

		for (UInt i = 0; i < element->numVertices(); i++)
		{
			if (candidates.front() == element->getVertex(i))
			{
				found = true;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " belongs to fel" << std::endl;
                }*/

				break;
			}
		}

		if (!found)
		{
#if USE_JMSH_DEBUG_MESSAGES
			if (candidates.front()->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(candidates.front()->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << candidates.front()->getPoint()->getCoord(0) << ", " << candidates.front()->getPoint()->getCoord(1) << ", " << candidates.front()->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, candidates.front(), ideal) << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			if (this->isValid(element, candidates.front()->getPoint(), false, vertices, radius))
			{
				v = candidates.front();

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << v->text() << " is valid" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (v->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, candidate = " << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ", " << v->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, v, ideal) << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES
			}
			/*else
            {
                if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " is NOT valid" << std::endl;
                }
            }*/
		}

		return v;
	}

	/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid several candidates" << std::endl;
    }*/

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//Data::RealVector ranks(candidates.size(), 0.0);

	//this->sortBest(element, candidates, ideal, ranks);
	//if (candidates.size() > 1)
	{
		for (Data::VertexList::iterator iter = candidates.begin();
			 iter != candidates.end(); iter++)
		{
			bool found = false;

			for (UInt i = 0; i < element->numVertices(); i++)
			{
				if ((*iter) == element->getVertex(i))
				{
					found = true;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " belongs to fel" << std::endl;
                    }*/

					break;
				}
			}

			if (found)
			{
				continue;
			}

			Real rank = this->rank(element, (*iter), ideal);

			if (std::isnan(rank))
            {
                //could happen for vertices that have the same position as one
                //  of the vertices of the given front element

                /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank nan" << std::endl;
                }*/

                continue;
            }

            /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank " << rank << std::endl;
            }*/

			//HeapWOP *wop = new HeapWOP();
			HeapWOP wop;

			//wop->v = (*iter);
			//wop->rank = rank;
			//wop->fa = this;
			wop.v = (*iter);
			wop.rank = rank;
			wop.fa = this;

#if USE_JMSH_DEBUG_MESSAGES
			if (wop->v->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(wop->v->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << wop->v->getPoint()->getCoord(0) << ", " << wop->v->getPoint()->getCoord(1) << ", " << wop->v->getPoint()->getCoord(2) << ", " << "metric = " << wop->rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			cands.push_back(wop);
		}

		std::make_heap(cands.begin(), cands.end(), Performer::better);
	}

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Vertex *candidate = wop->v;
		Data::Vertex *candidate = wop.v;

		//Real rank = wop->rank;
		Real rank = wop.rank;

		//delete wop;

		if (this->isValid(element, candidate->getPoint(), false, vertices, radius))
		{
		    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is valid" << std::endl;
            }*/

#if USE_JMSH_DEBUG_MESSAGES
			if (candidate->getPoint()->dimension() == 3)
			{
				std::cout << "markos debug, candidate = " << candidate->getPoint()->getCoord(0) << ", " << candidate->getPoint()->getCoord(1) << ", " << candidate->getPoint()->getCoord(2) << ", " << "metric = " << rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			while (!cands.empty())
			{
				std::pop_heap(cands.begin(), cands.end(), Performer::better);

				wop = cands.back();
				cands.pop_back();

				//Data::Vertex *next = wop->v;
				Data::Vertex *next = wop.v;

				//Real nextRank = wop->rank;
				Real nextRank = wop.rank;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has rank " << nextRank << std::endl;
                }*/

				//delete wop;

				if (!this->equal(nextRank, rank))
				{
				    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has different rank" << std::endl;
                    }*/

					break;
				}

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has equal rank" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (candidate->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, equal metric candidate = " << next->getPoint()->getCoord(0) << ", " << next->getPoint()->getCoord(1) << ", " << next->getPoint()->getCoord(2) << ", " << "metric = " << nextRank << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES

				//if ((candidate->getPoint()->match(next->getPoint())) &&
				if ((candidate->getPoint()->equal(next->getPoint())) &&
					(this->better(element, next, candidate)) &&
					(this->isValid(element, next->getPoint(), false, vertices, radius)))
				{
					candidate = next;

					rank = nextRank;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is better" << std::endl;
                    }*/

					break;
				}
				/*else
                {
                    if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is worse or not valid" << std::endl;
                    }
                }*/
			}

			best = candidate;

			break;
		}
		/*else
        {
            if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is NOT valid" << std::endl;
            }
        }*/
	}

	/*while (!cands.empty())
	{
		delete cands.back();

		cands.pop_back();
	}*/

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element,
	Data::VertexList &candidates, const Data::PointList &ideal,
	const Data::FrontElementList &frontElements, Real radius) const
{
    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid finding best for fel " << element->text() << std::endl;
    }*/

	if (candidates.empty())
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid no candidates" << std::endl;
        }*/

		return NULL;
	}

	if (candidates.size() == 1)
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid one candidate" << std::endl;
        }*/

		Data::Vertex *v = NULL;
		bool found = false;

		for (UInt i = 0; i < element->numVertices(); i++)
		{
			if (candidates.front() == element->getVertex(i))
			{
				found = true;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " belongs to fel" << std::endl;
                }*/

				break;
			}
		}

		if (!found)
		{
#if USE_JMSH_DEBUG_MESSAGES
			if (candidates.front()->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(candidates.front()->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << candidates.front()->getPoint()->getCoord(0) << ", " << candidates.front()->getPoint()->getCoord(1) << ", " << candidates.front()->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, candidates.front(), ideal) << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			if (this->isValid(element, candidates.front()->getPoint(), false, frontElements, radius))
			{
				v = candidates.front();

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << v->text() << " is valid" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (v->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, candidate = " << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ", " << v->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, v, ideal) << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES
			}
			/*else
            {
                if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " is NOT valid" << std::endl;
                }
            }*/
		}

		return v;
	}

	/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid several candidates" << std::endl;
    }*/

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//Data::RealVector ranks(candidates.size(), 0.0);

	//this->sortBest(element, candidates, ideal, ranks);
	//if (candidates.size() > 1)
	{
		for (Data::VertexList::iterator iter = candidates.begin();
			 iter != candidates.end(); iter++)
		{
			bool found = false;

			for (UInt i = 0; i < element->numVertices(); i++)
			{
				if ((*iter) == element->getVertex(i))
				{
					found = true;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " belongs to fel" << std::endl;
                    }*/

					break;
				}
			}

			if (found)
			{
				continue;
			}

			Real rank = this->rank(element, (*iter), ideal);

			if (std::isnan(rank))
            {
                //could happen for vertices that have the same position as one
                //  of the vertices of the given front element

                /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank nan" << std::endl;
                }*/

                continue;
            }

            /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank " << rank << std::endl;
            }*/

			//HeapWOP *wop = new HeapWOP();
			HeapWOP wop;

			//wop->v = (*iter);
			//wop->rank = rank;
			//wop->fa = this;
			wop.v = (*iter);
			wop.rank = rank;
			wop.fa = this;

#if USE_JMSH_DEBUG_MESSAGES
			if (wop->v->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(wop->v->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << wop->v->getPoint()->getCoord(0) << ", " << wop->v->getPoint()->getCoord(1) << ", " << wop->v->getPoint()->getCoord(2) << ", " << "metric = " << wop->rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			cands.push_back(wop);
		}

		std::make_heap(cands.begin(), cands.end(), Performer::better);
	}

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Vertex *candidate = wop->v;
		Data::Vertex *candidate = wop.v;

		//Real rank = wop->rank;
		Real rank = wop.rank;

		//delete wop;

		if (this->isValid(element, candidate->getPoint(), false, frontElements, radius))
		{
		    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is valid" << std::endl;
            }*/

#if USE_JMSH_DEBUG_MESSAGES
			if (candidate->getPoint()->dimension() == 3)
			{
				std::cout << "markos debug, candidate = " << candidate->getPoint()->getCoord(0) << ", " << candidate->getPoint()->getCoord(1) << ", " << candidate->getPoint()->getCoord(2) << ", " << "metric = " << rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			while (!cands.empty())
			{
				std::pop_heap(cands.begin(), cands.end(), Performer::better);

				wop = cands.back();
				cands.pop_back();

				//Data::Vertex *next = wop->v;
				Data::Vertex *next = wop.v;

				//Real nextRank = wop->rank;
				Real nextRank = wop.rank;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has rank " << nextRank << std::endl;
                }*/

				//delete wop;

				if (!this->equal(nextRank, rank))
				{
				    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has different rank" << std::endl;
                    }*/

					break;
				}

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has equal rank" << std::endl;

                    //while (true);
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (candidate->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, equal metric candidate = " << next->getPoint()->getCoord(0) << ", " << next->getPoint()->getCoord(1) << ", " << next->getPoint()->getCoord(2) << ", " << "metric = " << nextRank << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES

                /*//debug
                if ((candidate->getPoint()->equal(next->getPoint())) &&
					(!this->better(element, next, candidate)) &&
                    (!this->better(element, candidate, next)))
                {
                    std::cout << "debug" << std::endl;

                    this->better(element, next, candidate);
                    this->better(element, candidate, next);
                }
                //endebug*/

				//if ((candidate->getPoint()->match(next->getPoint())) &&
				if ((candidate->getPoint()->equal(next->getPoint())) &&
					(this->better(element, next, candidate)) &&
					(this->isValid(element, next->getPoint(), false, frontElements, radius)))
				{
					candidate = next;

					rank = nextRank;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is better" << std::endl;
                    }*/

					break;
				}
				/*else
                {
                    if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is worse or not valid" << std::endl;
                    }
                }*/
			}

			best = candidate;

			break;
		}
		/*else
        {
            if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is NOT valid" << std::endl;
            }
        }*/
	}

	/*while (!cands.empty())
	{
		delete cands.back();

		cands.pop_back();
	}*/

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestValid(const Data::FrontElement *element,
	Data::VertexList &candidates, const Data::PointList &ideal,
	const Data::VertexList &vertices, const Data::FrontElementList &frontElements, Real radius) const
{
    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid finding best for fel " << element->text() << std::endl;
    }*/

	if (candidates.empty())
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid no candidates" << std::endl;
        }*/

		return NULL;
	}

	if (candidates.size() == 1)
	{
	    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
        {
            std::cout << "debug frontadvancer::bestvalid one candidate" << std::endl;
        }*/

		Data::Vertex *v = NULL;
		bool found = false;

		for (UInt i = 0; i < element->numVertices(); i++)
		{
			if (candidates.front() == element->getVertex(i))
			{
				found = true;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " belongs to fel" << std::endl;
                }*/

				break;
			}
		}

		if (!found)
		{
#if USE_JMSH_DEBUG_MESSAGES
			if (candidates.front()->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(candidates.front()->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << candidates.front()->getPoint()->getCoord(0) << ", " << candidates.front()->getPoint()->getCoord(1) << ", " << candidates.front()->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, candidates.front(), ideal) << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			if (this->isValid(element, candidates.front()->getPoint(), false, vertices, frontElements, radius))
			{
				v = candidates.front();

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << v->text() << " is valid" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (v->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, candidate = " << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ", " << v->getPoint()->getCoord(2) << ", " << "metric = " << this->rank(element, v, ideal) << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES
			}
			/*else
            {
                if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << candidates.front()->text() << " is NOT valid" << std::endl;
                }
            }*/
		}

		return v;
	}

	/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
    {
        std::cout << "debug frontadvancer::bestvalid several candidates" << std::endl;
    }*/

	Data::Vertex *best = NULL;

	HeapWOPVector cands;

	cands.reserve(candidates.size());

	//Data::RealVector ranks(candidates.size(), 0.0);

	//this->sortBest(element, candidates, ideal, ranks);
	//if (candidates.size() > 1)
	{
		for (Data::VertexList::iterator iter = candidates.begin();
			 iter != candidates.end(); iter++)
		{
			bool found = false;

			for (UInt i = 0; i < element->numVertices(); i++)
			{
				if ((*iter) == element->getVertex(i))
				{
					found = true;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " belongs to fel" << std::endl;
                    }*/

					break;
				}
			}

			if (found)
			{
				continue;
			}

			Real rank = this->rank(element, (*iter), ideal);

			if (std::isnan(rank))
            {
                //could happen for vertices that have the same position as one
                //  of the vertices of the given front element

                /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank nan" << std::endl;
                }*/

                continue;
            }

            /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << (*iter)->text() << " has rank " << rank << std::endl;
            }*/

			//HeapWOP *wop = new HeapWOP();
			HeapWOP wop;

			//wop->v = (*iter);
			//wop->rank = rank;
			//wop->fa = this;
			wop.v = (*iter);
			wop.rank = rank;
			wop.fa = this;

#if USE_JMSH_DEBUG_MESSAGES
			if (wop->v->getPoint()->dimension() == 3)
			{
				static const Real cte1 = 1.0/std::sqrt(3.0);
				static const Real cte2 = std::sqrt(13.0)/2.0;
				Real r = std::sqrt(element->getShape()->size()*cte1)*cte2;

				//std::cout << "markos debug, dist = " << static_cast<const MJMesh::FrontElement *>(element)->straightDistance(wop->v->getPoint()) << ", height = " << r << ", height/10 = " << r*0.1 << std::endl;
				//std::cout << "markos debug, setting metrics = " << wop->v->getPoint()->getCoord(0) << ", " << wop->v->getPoint()->getCoord(1) << ", " << wop->v->getPoint()->getCoord(2) << ", " << "metric = " << wop->rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			cands.push_back(wop);
		}

		std::make_heap(cands.begin(), cands.end(), Performer::better);
	}

	while (!cands.empty())
	{
		//if (!this->ranks.empty())
		{
			std::pop_heap(cands.begin(), cands.end(), Performer::better);
		}

		//HeapWOP *wop = cands.back();
		HeapWOP wop = cands.back();
		cands.pop_back();

		//Data::Vertex *candidate = wop->v;
		Data::Vertex *candidate = wop.v;

		//Real rank = wop->rank;
		Real rank = wop.rank;

		//delete wop;

		if (this->isValid(element, candidate->getPoint(), false, vertices, frontElements, radius))
		{
		    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is valid" << std::endl;
            }*/

#if USE_JMSH_DEBUG_MESSAGES
			if (candidate->getPoint()->dimension() == 3)
			{
				std::cout << "markos debug, candidate = " << candidate->getPoint()->getCoord(0) << ", " << candidate->getPoint()->getCoord(1) << ", " << candidate->getPoint()->getCoord(2) << ", " << "metric = " << rank << std::endl;
			}
#endif //#if USE_JMSH_DEBUG_MESSAGES

			while (!cands.empty())
			{
				std::pop_heap(cands.begin(), cands.end(), Performer::better);

				wop = cands.back();
				cands.pop_back();

				//Data::Vertex *next = wop->v;
				Data::Vertex *next = wop.v;

				//Real nextRank = wop->rank;
				Real nextRank = wop.rank;

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has rank " << nextRank << std::endl;
                }*/

				//delete wop;

				if (!this->equal(nextRank, rank))
				{
				    /*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has different rank" << std::endl;
                    }*/

					break;
				}

				/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                {
                    std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " has equal rank" << std::endl;
                }*/

#if USE_JMSH_DEBUG_MESSAGES
				if (candidate->getPoint()->dimension() == 3)
				{
					std::cout << "markos debug, equal metric candidate = " << next->getPoint()->getCoord(0) << ", " << next->getPoint()->getCoord(1) << ", " << next->getPoint()->getCoord(2) << ", " << "metric = " << nextRank << std::endl;
				}
#endif //#if USE_JMSH_DEBUG_MESSAGES

				//if ((candidate->getPoint()->match(next->getPoint())) &&
				if ((candidate->getPoint()->equal(next->getPoint())) &&
					(this->better(element, next, candidate)) &&
					(this->isValid(element, next->getPoint(), false, vertices, frontElements, radius)))
				{
					candidate = next;

					rank = nextRank;

					/*if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is better" << std::endl;
                    }*/

					break;
				}
				/*else
                {
                    if ((Data::getState().isDebug()) && (element->getId() == 15942))
                    {
                        std::cout << "debug frontadvancer::bestvalid next candidate " << next->text() << " with rank " << nextRank << " is worse or not valid" << std::endl;
                    }
                }*/
			}

			best = candidate;

			break;
		}
		/*else
        {
            if ((Data::getState().isDebug()) && (element->getId() == 15942))
            {
                std::cout << "debug frontadvancer::bestvalid candidate " << candidate->text() << " with rank " << rank << " is NOT valid" << std::endl;
            }
        }*/
	}

	/*while (!cands.empty())
	{
		delete cands.back();

		cands.pop_back();
	}*/

	return best;
}

Data::Vertex *Performer::FrontAdvancer::best(const Data::FrontElement *element,
	Data::VertexList &valid, const Data::PointList &ideal) const
{
	if (valid.empty())
	{
		return NULL;
	}

	if (valid.size() == 1)
	{
		Data::Vertex *best = valid.front();

		valid.clear();

		return best;
	}

	Data::Vertex *best = valid.front();
	Data::VertexList::iterator bestIter = valid.begin();
	Real bestRank = this->rank(element, best, ideal);

	for (Data::VertexList::iterator iter = ++valid.begin();
		 iter != valid.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		Real vRank = this->rank(element, v, ideal);

		if ((this->better(vRank, bestRank)) ||
			((this->equal(vRank, bestRank)) && (this->better(element, v, best))))
		{
			best = v;
			bestRank = vRank;
			bestIter = iter;
		}
	}

	valid.erase(bestIter);

	return best;
}

/*Data::Vertex *Performer::FrontAdvancer::best(const Data::FrontElement *element, bool insert) const
{
	//cout << "debug best, id = " << element->getId() << ", insert = " << (insert ? "true" : "false") << endl;

	Data::VertexList valid;

	bool newVertex = false;
	Data::Vertex *best = NULL;

	//if insertion of new vertices is allowed
	if (insert)
	{
		//find the ideal points for insertion
		Data::PointList ideal = this->ideal(element);

		//cout << "debug ideal = " << ideal.size() << endl;

		if (ideal.empty())
		{
			return NULL;
		}

		//find the search radius
		Real radius = this->radius(element);

#if USE_JMSH_DEBUG_MESSAGES
		if (ideal[0]->dimension() == 3)
		{
			std::cout << "markos debug, newpt  = " << ideal[0]->getCoord(0) << ", " << ideal[0]->getCoord(1) << ", " << ideal[0]->getCoord(2) << std::endl;
			std::cout << "markos debug, radius = " << radius << std::endl;
		}
#endif //#if USE_JMSH_DEBUG_MESSAGES

		//cout << "debug radius = " << radius << endl;

		if (radius < 0.0)
		{
			while (!ideal.empty())
			{
				delete ideal.back();

				ideal.pop_back();
			}

			return NULL;
		}

		//restrict the ideal points, if necessary
		ideal = this->restricted(element, ideal, radius);

		//cout << "debug after restriction, ideal = " << ideal.size() << endl;

		if (ideal.empty())
		{
			return NULL;
		}

		//find the existing candidate vertices
		Data::VertexList candidates = this->candidates(ideal, radius);

		//cout << "debug candidates = " << candidates.size() << endl;

		//if there is no exisiting candidate vertex
		if (candidates.empty())
		{
			best = this->bestValid(element, ideal);

			if (!best)
			{
				return NULL;
			}

			newVertex = true;
		}
		//if there is existing candidate vertices
		else
		{
			//Data::VertexVector cands(candidates.begin(), candidates.end());

			//candidates.clear();

			best = this->bestValid(element, candidates, ideal, radius);

			while (!ideal.empty())
			{
				delete ideal.back();

				ideal.pop_back();
			}
		}
	}
	//if insertion is not allowed
	else
	{
		//all front vertices are candidate
		//Data::VertexList candidates = this->front->getVertices();
		Data::VertexList candidates = this->candidates(element);

		//cout << "debug candidates = " << candidates.size() << endl;

		if (candidates.empty())
		{
			return NULL;
		}

		//Data::VertexVector cands(candidates.begin(), candidates.end());

		//candidates.clear();

		best = this->bestValid(element, candidates, Data::PointList());
	}

	//cout << "debug valid = " << valid.size() << endl;

	if (newVertex)
	{
		while (!valid.empty())
		{
			delete valid.back();

			valid.pop_back();
		}
	}

	//if (best) cout << "debug best = " << best->text() << endl;

	return best;
}*/

Data::Vertex *Performer::FrontAdvancer::bestWithInsertion(const Data::FrontElement *element) const
{
	//std::cout << "debug best with insertion, id = " << element->getId() << std::endl;

	//find the ideal points for insertion
	Data::PointList ideal = this->ideal(element);

	//cout << "debug ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		return NULL;
	}

	//find the search radius
	Real radius = this->radius(element);

#if USE_JMSH_DEBUG_MESSAGES
	if (ideal[0]->dimension() == 3)
	{
		std::cout << "markos debug, newpt  = " << ideal[0]->getCoord(0) << ", " << ideal[0]->getCoord(1) << ", " << ideal[0]->getCoord(2) << std::endl;
		std::cout << "markos debug, radius = " << radius << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	//cout << "debug radius = " << radius << endl;

	if (radius < 0.0)
	{
		while (!ideal.empty())
		{
			delete ideal.back();

			ideal.pop_back();
		}

		return NULL;
	}

	//restrict the ideal points, if necessary
	this->restricted(element, ideal, radius);

	//cout << "debug after restriction, ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		return NULL;
	}

	//find the existing candidate vertices
	Data::VertexList candidates = this->candidates(ideal, radius);

	//cout << "debug candidates = " << candidates.size() << endl;

	//if there is no exisiting candidate vertex
	if (candidates.empty())
	{
		return this->bestValid(element, ideal);
	}
	//if there are existing candidate vertices

	Data::Vertex *best = this->bestValid(element, candidates, ideal, radius);

	while (!ideal.empty())
	{
		delete ideal.back();

		ideal.pop_back();
	}

	//if (best) cout << "debug best = " << best->text() << endl;

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestWithInsertionV(const Data::FrontElement *element) const
{
	//std::cout << "debug best with insertion, id = " << element->getId() << std::endl;

	//find the ideal points for insertion
	Data::PointList ideal = this->ideal(element);

	//cout << "debug ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		return NULL;
	}

	//find the search radius
	Real radius = this->radius(element);

#if USE_JMSH_DEBUG_MESSAGES
	if (ideal[0]->dimension() == 3)
	{
		std::cout << "markos debug, newpt  = " << ideal[0]->getCoord(0) << ", " << ideal[0]->getCoord(1) << ", " << ideal[0]->getCoord(2) << std::endl;
		std::cout << "markos debug, radius = " << radius << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	//cout << "debug radius = " << radius << endl;

	if (radius < 0.0)
	{
		while (!ideal.empty())
		{
			delete ideal.back();

			ideal.pop_back();
		}

		return NULL;
	}

	//restrict the ideal points, if necessary
	this->restricted(element, ideal, radius);

	//cout << "debug after restriction, ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		return NULL;
	}

	Data::VertexList vertices;

	//find the existing candidate vertices
	Data::VertexList candidates = this->candidates(ideal, radius, vertices);

	//cout << "debug candidates = " << candidates.size() << endl;

	//if there is no exisiting candidate vertex
	if (candidates.empty())
	{
		return this->bestValid(element, ideal, vertices);
	}
	//if there are existing candidate vertices

	Data::Vertex *best = this->bestValid(element, candidates, ideal, vertices, radius);

	while (!ideal.empty())
	{
		delete ideal.back();

		ideal.pop_back();
	}

	//if (best) cout << "debug best = " << best->text() << endl;

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestWithInsertionFE(const Data::FrontElement *element) const
{
	//++this->numBestWithInsertion;

	//std::cout << "debug best with insertion, id = " << element->getId() << std::endl;

	//Real start = Data::time();

	//find the ideal points for insertion
	Data::PointList ideal = this->ideal(element);

	//cout << "debug ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		//this->timeIdeal += Data::time() - start;

		return NULL;
	}

	//this->timeIdeal += Data::time() - start;
	//start = Data::time();

	//find the search radius
	Real radius = this->radius(element);

#if USE_JMSH_DEBUG_MESSAGES
	if (ideal[0]->dimension() == 3)
	{
		std::cout << "markos debug, newpt  = " << ideal[0]->getCoord(0) << ", " << ideal[0]->getCoord(1) << ", " << ideal[0]->getCoord(2) << std::endl;
		std::cout << "markos debug, radius = " << radius << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	//cout << "debug radius = " << radius << endl;

	if (radius < 0.0)
	{
		while (!ideal.empty())
		{
			delete ideal.back();

			ideal.pop_back();
		}

		//this->timeRadius += Data::time() - start;

		return NULL;
	}

	//this->timeRadius += Data::time() - start;
	//start = Data::time();

	//restrict the ideal points, if necessary
	this->restricted(element, ideal, radius);

	//cout << "debug after restriction, ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		//this->timeRestricted += Data::time() - start;

		return NULL;
	}

	//this->timeRestricted += Data::time() - start;
	//start = Data::time();

	//find the existing candidate vertices
	Data::VertexList candidates = this->candidates(ideal, radius);

	//cout << "debug candidates = " << candidates.size() << endl;

	//this->timeCandidates += Data::time() - start;
	//start = Data::time();


#if USE_EXPERIMENTAL_3
	UInt dim = ideal.front()->dimension();

	Data::Point2D min2d, max2d;
	Data::Point3D min3d, max3d;

	Data::Point &min = (dim == 2) ?
        (min2d = ideal.front(), static_cast<Data::Point &>(min2d)) :
        (min3d = ideal.front(), static_cast<Data::Point &>(min3d));
    Data::Point &max = (dim == 2) ?
        (max2d = ideal.front(), static_cast<Data::Point &>(max2d)) :
        (max3d = ideal.front(), static_cast<Data::Point &>(max3d));
#else
	Data::Point min(ideal.front()), max(ideal.front());
	UInt dim = min.dimension();
#endif //#if USE_EXPERIMENTAL_3

	for (Data::PointList::iterator iter = ++ideal.begin();
		 iter != ideal.end(); iter++)
	{
		for (UInt i = 0; i < dim; i++)
		{
			Real coord = (*iter)->getCoord(i);

			if (coord < min.getCoord(i))
			{
				min.setCoord(i, coord);
			}

			if (coord > max.getCoord(i))
			{
				max.setCoord(i, coord);
			}
		}
	}

	for (UInt i = 0; i < element->numVertices(); i++)
	{
		Data::Point *p = element->getVertex(i)->getPoint();

		for (UInt i = 0; i < dim; i++)
		{
			Real coord = p->getCoord(i);

			if (coord < min.getCoord(i))
			{
				min.setCoord(i, coord);
			}

			if (coord > max.getCoord(i))
			{
				max.setCoord(i, coord);
			}
		}
	}

	for (Data::VertexList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		for (UInt i = 0; i < dim; i++)
		{
			Real coord = p->getCoord(i);

			if (coord < min.getCoord(i))
			{
				min.setCoord(i, coord);
			}

			if (coord > max.getCoord(i))
			{
				max.setCoord(i, coord);
			}
		}
	}

	Data::FrontElementList frontElements = this->front->frontElementsInRange(Data::Box(min, max));

	//this->timeFrontElementsInRange += Data::time() - start;
	//start = Data::time();

	//if there is no exisiting candidate vertex
	if (candidates.empty())
	{
		return this->bestValid(element, ideal, frontElements);
		//Data::Vertex *best = this->bestValid(element, ideal, frontElements);

		//this->timeCandidatesBestValid += Data::time() - start;

		//return best;
	}
	//if there are existing candidate vertices

	Data::Vertex *best = this->bestValid(element, candidates, ideal, frontElements, radius);

	while (!ideal.empty())
	{
		delete ideal.back();

		ideal.pop_back();
	}

	//this->timeIdealBestValid += Data::time() - start;

	//if (best) cout << "debug best = " << best->text() << endl;

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestWithInsertionVFE(const Data::FrontElement *element) const
{
	//std::cout << "debug best with insertion, id = " << element->getId() << std::endl;

	//find the ideal points for insertion
	Data::PointList ideal = this->ideal(element);

	//cout << "debug ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		return NULL;
	}

	//find the search radius
	Real radius = this->radius(element);

#if USE_JMSH_DEBUG_MESSAGES
	if (ideal[0]->dimension() == 3)
	{
		std::cout << "markos debug, newpt  = " << ideal[0]->getCoord(0) << ", " << ideal[0]->getCoord(1) << ", " << ideal[0]->getCoord(2) << std::endl;
		std::cout << "markos debug, radius = " << radius << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	//cout << "debug radius = " << radius << endl;

	if (radius < 0.0)
	{
		while (!ideal.empty())
		{
			delete ideal.back();

			ideal.pop_back();
		}

		return NULL;
	}

	//restrict the ideal points, if necessary
	this->restricted(element, ideal, radius);

	//cout << "debug after restriction, ideal = " << ideal.size() << endl;

	if (ideal.empty())
	{
		return NULL;
	}

	Data::VertexList vertices;

	//find the existing candidate vertices
	Data::VertexList candidates = this->candidates(ideal, radius, vertices);

	//cout << "debug candidates = " << candidates.size() << endl;

#if USE_EXPERIMENTAL_3
	UInt dim = ideal.front()->dimension();

	Data::Point2D min2d, max2d;
	Data::Point3D min3d, max3d;

	Data::Point &min = (dim == 2) ?
        (min2d = ideal.front(), static_cast<Data::Point &>(min2d)) :
        (min3d = ideal.front(), static_cast<Data::Point &>(min3d));
    Data::Point &max = (dim == 2) ?
        (max2d = ideal.front(), static_cast<Data::Point &>(max2d)) :
        (max3d = ideal.front(), static_cast<Data::Point &>(max3d));
#else
	Data::Point min(ideal.front()), max(ideal.front());
	UInt dim = min.dimension();
#endif //#if USE_EXPERIMENTAL_3

	for (Data::PointList::iterator iter = ++ideal.begin();
		 iter != ideal.end(); iter++)
	{
		for (UInt i = 0; i < dim; i++)
		{
			Real coord = (*iter)->getCoord(i);

			if (coord < min.getCoord(i))
			{
				min.setCoord(i, coord);
			}

			if (coord > max.getCoord(i))
			{
				max.setCoord(i, coord);
			}
		}
	}

	for (UInt i = 0; i < element->numVertices(); i++)
	{
		Data::Point *p = element->getVertex(i)->getPoint();

		for (UInt i = 0; i < dim; i++)
		{
			Real coord = p->getCoord(i);

			if (coord < min.getCoord(i))
			{
				min.setCoord(i, coord);
			}

			if (coord > max.getCoord(i))
			{
				max.setCoord(i, coord);
			}
		}
	}

	for (Data::VertexList::iterator iter = candidates.begin();
		 iter != candidates.end(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		for (UInt i = 0; i < dim; i++)
		{
			Real coord = p->getCoord(i);

			if (coord < min.getCoord(i))
			{
				min.setCoord(i, coord);
			}

			if (coord > max.getCoord(i))
			{
				max.setCoord(i, coord);
			}
		}
	}

	Data::FrontElementList frontElements = this->front->frontElementsInRange(Data::Box(min, max));

	//if there is no exisiting candidate vertex
	if (candidates.empty())
	{
		return this->bestValid(element, ideal, vertices, frontElements);
	}
	//if there are existing candidate vertices

	Data::Vertex *best = this->bestValid(element, candidates, ideal, vertices, frontElements, radius);

	while (!ideal.empty())
	{
		delete ideal.back();

		ideal.pop_back();
	}

	//if (best) cout << "debug best = " << best->text() << endl;

	return best;
}

Data::Vertex *Performer::FrontAdvancer::bestWithoutInsertion(const Data::FrontElement *element) const
{
	//++this->numBestWithoutInsertion;

	//std::cout << "debug best without insertion, id = " << element->getId() << std::endl;

	//Real start = Data::time();

	Data::VertexList candidates = this->candidates(element);

	//std::cout << "debug candidates = " << candidates.size() << std::endl;

	if (candidates.empty())
	{
		//this->timeTopologyCandidates += Data::time() - start;

		return NULL;
	}

	//this->timeTopologyCandidates += Data::time() - start;
	//start = Data::time();

	//Data::Vertex *v =  this->bestValid(element, candidates, Data::PointList());

	//this->timeTopologyBestValid += Data::time() - start;

	//return v;
	return this->bestValid(element, candidates, Data::PointList());
}
