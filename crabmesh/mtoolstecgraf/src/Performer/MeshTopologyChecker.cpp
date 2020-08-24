#include "Performer/MeshTopologyChecker.h"

#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Tetrahedron.h"
#include "Data/Pyramid.h"
#include "Data/Wedge.h"
#include "Data/Brick.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Boundary.h"
#include "Data/Front.h"
#include "Data/Mesh.h"

Performer::MeshTopologyChecker::MeshTopologyChecker() :
	Performer::Builder::Builder()
{
	this->mesh = NULL;
	this->boundary = NULL;
	this->front = NULL;

	this->stopOnFirst = false;

	this->consistent = true;

	this->dimension = 0;
	this->testElements = true;
	this->internalFrontElements = true;
}

Performer::MeshTopologyChecker::~MeshTopologyChecker()
{

}

void Performer::MeshTopologyChecker::setMesh(const Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::MeshTopologyChecker::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::MeshTopologyChecker::setFront(const Data::Front *front)
{
	this->front = front;
}

void Performer::MeshTopologyChecker::setStopOnFirst(bool stopOnFirst)
{
	this->stopOnFirst = stopOnFirst;
}

bool Performer::MeshTopologyChecker::isConsistent() const
{
	return this->consistent;
}

void Performer::MeshTopologyChecker::execute()
{
	if (!this->mesh)
	{
		return;
	}

	this->consistent = true;

	//std::cout << "testing mesh " << this->mesh->getId() << std::endl;

	this->dimension = this->mesh->dimension();

	if ((this->dimension != 2) && (this->dimension != 3))
	{
		std::cout << "dimension of mesh is " << this->dimension << ", skipping mesh" << std::endl;

		this->consistent = false;

		return;
	}

	this->testElements = (this->mesh->size() != 0);
	this->internalFrontElements = (this->mesh->frontElementsSize() == 0) ? false :
		(this->mesh->frontElementsSize() != ((this->boundary ? this->boundary->size() : 0) + (this->front ? this->front->size() : 0)));

#if USE_C__11
    this->idvMap.reserve(this->mesh->verticesSize());
    this->idfeMap.reserve(this->mesh->frontElementsSize());
    this->ideMap.reserve(this->mesh->size());

    this->vMap.reserve(this->mesh->verticesSize());
    this->feMap.reserve(this->mesh->frontElementsSize());
    this->eMap.reserve(this->mesh->size());
#endif //#if USE_C__11

	for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
		 iter != end; iter++)
    {
        this->idvMap.insert(std::make_pair((*iter)->getId(), (*iter)));

        if (!(*iter)->getPoint())
        {
            continue;
        }

        this->vMap.insert(std::make_pair((*iter)->getPoint()->getCoord(0), (*iter)));
    }

	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin(), end = this->mesh->feEnd();
		 iter != end; iter++)
    {
        this->idfeMap.insert(std::make_pair((*iter)->getId(), (*iter)));

        if (!(*iter)->getShape())
        {
            continue;
        }

        bool valid = true;

        for (UInt i = 0; i < (*iter)->getShape()->numPoints(); i++)
        {
            if (!(*iter)->getShape()->getPoint(i))
            {
                valid = false;

                break;
            }
        }

        if (!valid)
        {
            continue;
        }

        this->feMap.insert(std::make_pair((*iter)->getShape()->box().getMin()->getCoord(0), (*iter)));
    }

	for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin(), end = this->mesh->eEnd();
		 iter != end; iter++)
    {
        this->ideMap.insert(std::make_pair((*iter)->getId(), (*iter)));

        if (!(*iter)->getShape())
        {
            continue;
        }

        bool valid = true;

        for (UInt i = 0; i < (*iter)->getShape()->numPoints(); i++)
        {
            if (!(*iter)->getShape()->getPoint(i))
            {
                valid = false;

                break;
            }
        }

        if (!valid)
        {
            continue;
        }

        this->eMap.insert(std::make_pair((*iter)->getShape()->box().getMin()->getCoord(0), (*iter)));
    }

/*#if USE_TIME_MEASUREMENTS
	Real tstart, tend, time, total, tstep;
#endif //#if USE_TIME_MEASUREMENTS
	ULInt size, count;

	count = 0;
#if USE_TIME_MEASUREMENTS
	tstart = tend = time = tstep = total = 0.0;
#endif //#if USE_TIME_MEASUREMENTS
	size = this->mesh->verticesSize();*/

	for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
		 iter != end; iter++)
	{
		/*if (((++count)%500 == 0) || (count == 1) || (count == size))
		{
			std::cout << "testing vertex " << count << " of " << size
#if USE_TIME_MEASUREMENTS
					  << ", timings: current: " << Data::time(tstep)
					  << ", total: " << Data::time(total);

			if (time > 0.0)
			{
				std::cout << ", estimated: " << Data::time(time*static_cast<Real>(size - count)/2.0);
			}

			std::cout << " (hh:mm:ss)" << std::endl;
#else
					  << std::endl;
#endif
		}

#if USE_TIME_MEASUREMENTS
		tend = Data::time();

		if (tstart > 0.0)
		{
			time = tend - tstart;
			tstep += time;
			total += time;
		}

		tstart = tend;
#endif //#if USE_TIME_MEASUREMENTS*/

#if USE_GUI
		bool ok =
#endif //#if USE_GUI
            //this->check(iter, end);
            this->check((*iter));

#if USE_GUI
        if ((*iter) && (!ok))
        {
            (*iter)->highlight();
        }
#endif // USE_GUI

		if ((!this->consistent) && (this->stopOnFirst))
		{
#if USE_GUI
            this->mesh->clearLists();
#endif // USE_GUI

			return;
		}
	}

	/*count = 0;
#if USE_TIME_MEASUREMENTS
	tstart = tend = time = tstep = 0.0;
#endif //#if USE_TIME_MEASUREMENTS
	size = this->mesh->frontElementsSize();*/

	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin(), end = this->mesh->feEnd();
		 iter != end; iter++)
	{
		/*if (((++count)%500 == 0) || (count == 1) || (count == size))
		{
			std::cout << "testing front element " << count << " of " << size
#if USE_TIME_MEASUREMENTS
					  << ", timings: current: " << Data::time(tstep)
					  << ", total: " << Data::time(total);

			if (time > 0.0)
			{
				std::cout << ", estimated: " << Data::time(time*static_cast<Real>(size - count)/2.0);
			}

			std::cout << " (hh:mm:ss)" << std::endl;
#else
					  << std::endl;
#endif //#if USE_TIME_MEASUREMENTS
		}

#if USE_TIME_MEASUREMENTS
		tend = Data::time();

		if (tstart > 0.0)
		{
			time = tend - tstart;
			tstep += time;
			total += time;
		}

		tstart = tend;
#endif //#if USE_TIME_MEASUREMENTS*/

#if USE_GUI
		bool ok =
#endif //#if USE_GUI
            //this->check(iter, end);
            this->check((*iter));

#if USE_GUI
        if ((*iter) && (!ok))
        {
            (*iter)->highlight();

            this->mesh->clearLists();
        }
#endif // USE_GUI

		if ((!this->consistent) && (this->stopOnFirst))
		{
#if USE_GUI
            this->mesh->clearLists();
#endif // USE_GUI

			return;
		}
	}

	/*count = 0;
#if USE_TIME_MEASUREMENTS
	tstart = tend = time = tstep = 0.0;
#endif //#if USE_TIME_MEASUREMENTS
	size = this->mesh->size();*/

	for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin(), end = this->mesh->eEnd();
		 iter != end; iter++)
	{
		/*if (((++count)%500 == 0) || (count == 1) || (count == size))
		{
			std::cout << "testing element " << count << " of " << size
#if USE_TIME_MEASUREMENTS
					  << ", timings: current: " << Data::time(tstep)
					  << ", total: " << Data::time(total);

			if (time > 0.0)
			{
				std::cout << ", estimated: " << Data::time(time*static_cast<Real>(size - count)/2.0);
			}

			std::cout << " (hh:mm:ss)" << std::endl;
#else
					  << std::endl;
#endif
		}

#if USE_TIME_MEASUREMENTS
		tend = Data::time();

		if (tstart > 0.0)
		{
			time = tend - tstart;
			tstep += time;
			total += time;
		}

		tstart = tend;
#endif //#if USE_TIME_MEASUREMENTS*/

#if USE_GUI
		bool ok =
#endif //#if USE_GUI
            //this->check(iter, end);
            this->check((*iter));

#if USE_GUI
        if ((*iter) && (!ok))
        {
            (*iter)->highlight();

            this->mesh->clearLists();
        }
#endif // USE_GUI

		if ((!this->consistent) && (this->stopOnFirst))
		{
#if USE_GUI
            this->mesh->clearLists();
#endif // USE_GUI

			return;
		}
	}

#if USE_GUI
    if (!this->consistent)
    {
        this->mesh->clearLists();
    }
#endif // USE_GUI

	//std::cout << "ended testing mesh " << this->mesh->getId() << std::endl;
}

bool Performer::MeshTopologyChecker::isTestingAngles() const
{
    return true;
}

bool Performer::MeshTopologyChecker::isTestingAngles(const Data::Vertex *test) const
{
    Data::ElementList elements = this->mesh->adjacency(test);

    for (Data::ElementList::iterator iter = elements.begin();
         iter != elements.end(); iter++)
    {
        if (!dynamic_cast<Data::Tetrahedron *>((*iter)))
        {
            return false;
        }
    }

    return true;
}

//bool Performer::MeshTopologyChecker::check(const Data::Mesh::ConstVertexIterator &iter, const Data::Mesh::ConstVertexIterator &end) const
bool Performer::MeshTopologyChecker::check(const Data::Vertex *test) const
{
	//Data::Vertex *test = (*iter);

	bool ok = true;

	if (!test->getPoint())
	{
		std::cout << "vertex " << test->text() << " has no point, skipping vertex" << std::endl;

		this->consistent = false;

		ok = false;

		return ok;
	}

	std::pair<IdVMap::const_iterator, IdVMap::const_iterator> idp = this->idvMap.equal_range(test->getId());

	for (IdVMap::const_iterator iter2 = idp.first; iter2 != idp.second; iter2++)
    {
        Data::Vertex *other = (*iter2).second;

        if ((other != test) && (other->getId() == test->getId()))
		{
			std::cout << "vertex " << test->text() << " has other with same id: " << other->text() << std::endl;

			this->consistent = false;

			ok = false;
		}
    }

	//Data::Mesh::ConstVertexIterator iter2 = iter;
	std::pair<RVMap::const_iterator, RVMap::const_iterator> p = this->vMap.equal_range(test->getPoint()->getCoord(0));

	UInt appear = 0;

	//for (iter2++; iter2 != end; iter2++)
	for (RVMap::const_iterator iter2 = p.first; iter2 != p.second; iter2++)
	{
		//Data::Vertex *other = (*iter2);
		Data::Vertex *other = (*iter2).second;

		if (test == other)
		{
		    appear++;

		    if (appear == 1)
            {
                continue;
            }

			std::cout << "vertex " << test->text() << " appears twice in mesh" << std::endl;

			this->consistent = false;

			ok = false;
		}

		/*if (other->getId() == test->getId())
		{
			std::cout << "vertex " << test->text() << " has other with same id: " << other->text() << std::endl;

			this->consistent = false;

			ok = false;
		}*/

		bool point = false, geometricPoint = false;

		if (other->getPoint() == test->getPoint())
		{
			point = true;
		}

		if (other->getPoint())
		{
			bool match = test->getPoint()->match(*other->getPoint());

			if (match)
			{
				geometricPoint = true;
			}
			else if (test->getPoint()->equal(*other->getPoint()))
			{
				std::cout << "vertex " << test->text() << " and vertex " << other->text() << " are dangerously close to each other" << std::endl;

				this->consistent = false;
			}
		}

		bool crack = false;

		if ((point) || (geometricPoint))
		{
			if (this->boundary)
			{
				Data::FrontElementList testFels = this->boundary->adjacency(test);
				Data::FrontElementList otherFels = this->boundary->adjacency(other);

				Data::FrontElementList testCrackFels, otherCrackFels;

				while (!testFels.empty())
				{
					Data::FrontElement *testFel = testFels.front();

					testFels.pop_front();

					Data::FrontElementList::iterator iter = otherFels.end();

					for (iter = otherFels.begin(); iter != otherFels.end(); iter++)
					{
						Data::FrontElement *otherFel = (*iter);

						if (testFel->numVertices() != otherFel->numVertices())
						{
							continue;
						}

						bool allGeometricPoints = true;

						for (UInt i = 0; i < testFel->numVertices(); i++)
						{
							Data::Vertex *tv = testFel->getVertex(i);

							bool foundGeometricPoint = false;

							for (UInt j = 0; j < otherFel->numVertices(); j++)
							{
								Data::Vertex *ov = otherFel->getVertex(j);

								if (tv->getPoint()->match(ov->getPoint()))
								{
									foundGeometricPoint = true;

									break;
								}
							}

							if (!foundGeometricPoint)
							{
								allGeometricPoints = false;

								break;
							}
						}

						if (allGeometricPoints)
						{
							break;
						}
					}

					if (iter != otherFels.end())
					{
						testCrackFels.push_back(testFel);
						otherCrackFels.push_back((*iter));

						otherFels.erase(iter);
					}
				}

				if (!testCrackFels.empty())
				{
					crack = true;

					while (!testCrackFels.empty())
					{
						Data::FrontElement *testFel = testCrackFels.front();
						Data::FrontElement *otherFel = otherCrackFels.front();

						testCrackFels.pop_front();
						otherCrackFels.pop_front();

						Real dot1 = 0.0, dot2 = 0.0;

						if (this->dimension == 2)
						{
							dot1 = static_cast<Data::Edge2D *>(testFel->getShape())->normal().dot(static_cast<Data::Edge2D *>(otherFel->getShape())->normal());
							dot2 = static_cast<Data::Edge2D *>(testFel->getShape())->normal().dot(static_cast<Data::Edge2D *>(testFel->getShape())->normal());
						}
						else
						{
							dot1 = static_cast<Data::Polygon *>(testFel->getShape())->normal().dot(static_cast<Data::Polygon *>(otherFel->getShape())->normal());
							dot2 = static_cast<Data::Polygon *>(testFel->getShape())->normal().dot(static_cast<Data::Polygon *>(testFel->getShape())->normal());
						}

						//dot1 and dot2 should be opposite (avoid normalization)
						if (std::fabs(dot1 + dot2) > Data::getTolerance())
						{
							crack = false;

							break;
						}
					}
				}
			}
		}

		if (crack)
		{
			std::cout << "vertex " << test->text() << " and vertex " << other->text() << " were detected as in a crack" << std::endl;
		}
		else
		{
			if (point)
			{
				std::cout << "vertex " << test->text() << " and vertex " << other->text() << " share the same point" << std::endl;

				this->consistent = false;

                ok = false;
			}

			if (geometricPoint)
			{
				std::cout << "vertex " << test->text() << " and vertex " << other->text() << " are in the same position" << std::endl;

				this->consistent = false;

                ok = false;
			}
		}
	}

	if (this->testElements)
	{
	    bool testingAngles = (this->isTestingAngles() && this->isTestingAngles(test));

#if USE_EXACT
		Data::EReal accumAngle = 0.0;
#else
		Real accumAngle = 0.0;
#endif //#if USE_EXACT

		//Data::ElementList elements = vtest->getAdjacentElements();
		Data::ElementList elements = this->mesh->adjacency(test);

		if (elements.empty())
		{
			std::cout << "vertex " << test->text() << " has no adjacent elements" << std::endl;

			this->consistent = false;

			ok = false;
		}

        Data::FrontElementList boundaryFrontElements;

        if ((this->boundary) && (this->boundary->have(test)))
        {
            boundaryFrontElements = this->boundary->adjacency(test);
        }

        UInt numAdjacencies[elements.size()], numAdjacenciesShouldBe[elements.size()];

        UInt elementIndex = 0;

        for (Data::ElementList::iterator iter = elements.begin();
             iter != elements.end(); iter++, elementIndex++)
        {
            Data::Element *e = (*iter);

            if (dynamic_cast<Data::Polygon *>(e->getShape()))
            {
                numAdjacenciesShouldBe[elementIndex] = 2;
            }
            else if (dynamic_cast<Data::Tetrahedron *>(e->getShape()))
            {
                numAdjacenciesShouldBe[elementIndex] = 3;
            }
            else if (dynamic_cast<Data::Brick *>(e->getShape()))
            {
                numAdjacenciesShouldBe[elementIndex] = 3;
            }
            else if (dynamic_cast<Data::Pyramid *>(e->getShape()))
            {
                if (e->index(test) == 4)
                {
                    numAdjacenciesShouldBe[elementIndex] = 4;
                }
                else
                {
                    numAdjacenciesShouldBe[elementIndex] = 3;
                }
            }
            else if (dynamic_cast<Data::Wedge *>(e->getShape()))
            {
                numAdjacenciesShouldBe[elementIndex] = 3;
            }

            numAdjacencies[elementIndex] = 0;

            for (Data::ElementList::iterator iter2 = elements.begin();
                 iter2 != elements.end(); iter2++)
            {
                Data::Element *e2 = (*iter2);

                if (e2 == e)
                {
                    continue;
                }

                if (e->isAdjacent(e2))
                {
                    numAdjacencies[elementIndex]++;
                }
            }

            for (Data::FrontElementList::iterator iter2 = boundaryFrontElements.begin();
                 iter2 != boundaryFrontElements.end(); iter2++)
            {
                Data::FrontElement *fel = (*iter2);

                if (fel->isAdjacent(e))
                {
                    numAdjacencies[elementIndex]++;
                }
            }
        }

        for (elementIndex = 0; !elements.empty(); elementIndex++)
		{
			Data::Element *e = elements.front();

			elements.pop_front();

			if (std::find(elements.begin(), elements.end(), e) != elements.end())
			{
				std::cout << "vertex " << test->text() << " has element " << e->text() << " more than once in adjacent elements list" << std::endl;

				this->consistent = false;

                ok = false;
			}

			if (!this->mesh->have(e))
			{
				std::cout << "vertex " << test->text() << " has element " << e->text() << " that is not in this mesh" << std::endl;

				this->consistent = false;

                ok = false;
			}

			UInt index = e->numVertices();

			bool skip = false;

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				if (e->getVertex(i) == test)
				{
					if (index == e->numVertices())
					{
						index = i;
					}
					else
					{
						std::cout << "vertex " << test->text() << " appears more than once (" << index << " and " << i << ") in adjacent element: " << e->text() << std::endl;

						skip = true;
					}
				}
			}

			if (skip)
			{
				std::cout << "skipping adjacent element" << std::endl;

				this->consistent = false;

                ok = false;

				continue;
			}

			if (index == e->numVertices())
			{
				std::cout << "vertex " << test->text() << " has adjacent element that does not have it: " << e->text() << ", skipping adjacent element" << std::endl;

				this->consistent = false;

                ok = false;

				continue;
			}

			if (!e->getShape())
			{
				std::cout << "vertex " << test->text() << " has adjacent element " << e->text() << " that does not have shape, skipping adjacent element" << std::endl;

				this->consistent = false;

                ok = false;

				continue;
			}

			if (e->getShape()->getPoint(index) != test->getPoint())
			{
				std::cout << "vertex " << test->text() << " is in adjacent element " << e->text() << ", but the points in index " << index << " do not correspond, skipping adjacent element" << std::endl;

				this->consistent = false;

                ok = false;

				continue;
			}

            if (numAdjacencies[elementIndex] != numAdjacenciesShouldBe[elementIndex])
            {
                std::cout << "vertex " << test->text() << " is not topologically surrounded by elements, adjacent element " << e->text() << " has " << numAdjacencies[elementIndex] << " adjacencies instead of " << numAdjacenciesShouldBe[elementIndex] << std::endl;

                this->consistent = false;

                ok = false;
            }

			if (testingAngles)
            {
#if USE_EXACT
                Data::EReal angle = 0.0;
#else
                Real angle = 0.0;
#endif //#if USE_EXACT

                if (this->dimension == 2)
                {
                    for (UInt i = 0; i < e->numVertices() - 2; i++)
                    {
                        Data::Edge2D edge(
                            static_cast<Data::Point2D *>(e->getVertex((index + i + 1)%(e->numVertices()))->getPoint()),
                            static_cast<Data::Point2D *>(e->getVertex((index + i + 2)%(e->numVertices()))->getPoint()));

                        Real tol = Data::getTolerance();

                        Data::setTolerance(edge.size()*0.0001);

                        angle += edge.angle(test->getPoint());

                        Data::setTolerance(tol);
                    }
                }
                else
                {
                    for (UInt i = 0; i < e->numVertices() - 3; i++)
                    {
                        Data::Triangle3D triangle(
                            static_cast<Data::Point3D *>(e->getVertex((index + i + 1)%(e->numVertices()))->getPoint()),
                            static_cast<Data::Point3D *>(e->getVertex((index + i + 2)%(e->numVertices()))->getPoint()),
                            static_cast<Data::Point3D *>(e->getVertex((index + i + 3)%(e->numVertices()))->getPoint()));

                        Real tol = Data::getTolerance();

                        Data::setTolerance(triangle.size()*0.00001);

                        angle += triangle.solidAngle(test->getPoint());

                        Data::setTolerance(tol);
                    }
                }

                accumAngle += angle;
            }
		}

		bool inner = this->mesh->haveInner(test);
        //bool inboundary = this->boundary ? this->boundary->have(vtest) : false;
        bool inboundary = this->boundary ? this->boundary->have(test) : false;
        //bool infront = front ? front->have(vtest) : false;
        bool infront = this->front ? this->front->have(test) : false;

		if (testingAngles)
        {
            Real testAngle = (this->dimension == 2) ? 2*M_PI : 4*M_PI;

#if USE_EXACT
            Real showAngle = accumAngle;
#else
            Real showAngle = accumAngle;
#endif //#if USE_EXACT

            if (std::fabs(testAngle - accumAngle) > Data::getTolerance())
            {
                ///TODO check angles formed by boundary and/or front

                if ((!inboundary) && (!infront))
                {
                    std::cout << "vertex " << test->text() << " is not completely surrounded by elements, total angle is " << showAngle << ", instead of " << testAngle << std::endl;

                    this->consistent = false;

                    ok = false;
                }

                if (inner)
                {
                    std::cout << "vertex " << test->text() << " is marked as inner but is not completely surrounded by elements, total angle is " << showAngle << ", instead of " << testAngle << std::endl;

                    this->consistent = false;

                    ok = false;
                }
            }
            else
            {
                bool crackTip = false;

                if (this->boundary)
                {
                    crackTip = true;

                    Data::FrontElementList adjs = this->boundary->adjacency(test);

                    while (!adjs.empty())
                    {
                        Data::FrontElement *fel = adjs.front();

                        adjs.pop_front();

                        Data::FrontElement *fel2 = NULL;

                        for (Data::FrontElementList::iterator iter = adjs.begin();
                             iter != adjs.end(); iter++)
                        {
                            Data::FrontElement *f = (*iter);

                            if (f->numVertices() != fel->numVertices())
                            {
                                continue;
                            }

                            bool haveAll = true;

                            for (Data::UInt i = 0; i < fel->numVertices(); i++)
                            {
                                if (fel->getVertex(i) == test)
                                {
                                    continue;
                                }

                                if ((!f->have(fel->getVertex(i))) &&
                                    (!fel->getShape()->have(*fel->getVertex(i)->getPoint())))
                                {
                                    haveAll = false;

                                    break;
                                }
                            }

                            if (haveAll)
                            {
                                fel2 = (*iter);

                                adjs.erase(iter);

                                break;
                            }
                        }

                        if (!fel2)
                        {
                            crackTip = false;

                            break;
                        }
                    }
                }

                if ((crackTip) && (inboundary) && (!inner))
                {
                    std::cout << "vertex " << test->text() << " was detected as a crack tip, because it is in boundary and is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << std::endl;
                }
                else
                {
                    if (!inner)
                    {
                        std::cout << "vertex " << test->text() << " is not marked as inner but is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << std::endl;

                        this->consistent = false;

                        ok = false;
                    }

                    if (inboundary)
                    {
                        std::cout << "vertex " << test->text() << " is in boundary but is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << std::endl;

                        this->consistent = false;

                        ok = false;
                    }

                    if (infront)
                    {
                        std::cout << "vertex " << test->text() << " is in front but is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << std::endl;

                        this->consistent = false;

                        ok = false;
                    }
                }
            }
        }
	}

	//Data::FrontElementList frontElements = vtest->getAdjacentFrontElements();
	Data::FrontElementList frontElements = this->mesh->findAdjacentFrontElements(test);

	if ((frontElements.empty()) && (this->internalFrontElements))
	{
		std::cout << "vertex " << test->text() << " has no adjacent front elements" << std::endl;

		this->consistent = false;

        ok = false;
	}

	bool allInBoundaryOrInFront = true;

	while (!frontElements.empty())
	{
		Data::FrontElement *e = frontElements.front();

		frontElements.pop_front();

		if (std::find(frontElements.begin(), frontElements.end(), e) != frontElements.end())
		{
			std::cout << "vertex " << test->text() << " has front element " << e->text() << " more than once in adjacent front elements list" << std::endl;

			this->consistent = false;

			ok = false;
		}

		if (!this->mesh->have(e))
		{
			std::cout << "vertex " << test->text() << " has front element " << e->text() << " that is not in this mesh" << std::endl;

			this->consistent = false;

			ok = false;
		}

		UInt index = e->numVertices();

		bool skip = false;

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			if (e->getVertex(i) == test)
			{
				if (index == e->numVertices())
				{
					index = i;
				}
				else
				{
					std::cout << "vertex " << test->text() << " appears more than once (" << index << " and " << i << ") in adjacent front element: " << e->text() << std::endl;

					skip = true;
				}
			}
		}

		if (skip)
		{
			std::cout << "skipping adjacent front element" << std::endl;

			this->consistent = false;

			ok = false;

			continue;
		}

		if (index == e->numVertices())
		{
			std::cout << "vertex " << test->text() << " has adjacent front element that does not have it: " << e->text() << ", skipping adjacent front element" << std::endl;

			this->consistent = false;

			ok = false;

			continue;
		}

		if (!e->getShape())
		{
			std::cout << "vertex " << test->text() << " has adjacent front element " << e->text() << " that does not have shape, skipping adjacent front element" << std::endl;

			this->consistent = false;

			ok = false;

			continue;
		}

		if (e->getShape()->getPoint(index) != test->getPoint())
		{
			std::cout << "vertex " << test->text() << " is in adjacent front element " << e->text() << ", but the points in index " << index << " do not correspond, skipping adjacent front element" << std::endl;

			this->consistent = false;

			ok = false;

			continue;
		}

		if (!this->internalFrontElements)
		{
			bool inboundary = false;
			bool infront = false;

			if ((this->boundary) && (this->boundary->have(e)))
			{
				inboundary = true;
			}

			if ((this->front) && (this->front->have(e)))
			{
				infront = true;
			}

			if ((!inboundary) && (!infront))
			{
				allInBoundaryOrInFront = false;
			}
		}

		///TODO check angles
	}

	if (!allInBoundaryOrInFront)
	{
		std::cout << "vertex " << test->text() << " has adjacent front element(s) that do not belong to boundary or front" << std::endl;

		this->consistent = false;

        ok = false;
	}

	return ok;
}

//bool Performer::MeshTopologyChecker::check(const Data::Mesh::ConstFrontElementIterator &iter, const Data::Mesh::ConstFrontElementIterator &end) const
bool Performer::MeshTopologyChecker::check(const Data::FrontElement *test) const
{
	//Data::FrontElement *test = (*iter);

	bool ok = true;

	if (!test->getShape())
	{
		std::cout << "front element " << test->text() << " has no shape, skipping front element" << std::endl;

		this->consistent = false;

        ok = false;

		return ok;
	}

	if (test->getShape()->numPoints() != test->numVertices())
	{
#if USE_GUI
		std::cout << "front element " << test->text() << " has shape with different number of points: " << test->getShape()->text() << ", skipping front element" << std::endl;
#else
		std::cout << "front element " << test->text() << " has shape with different number of points: " << test->getShape()->numPoints() << ", skipping front element" << std::endl;
#endif //#if USE_GUI

		this->consistent = false;

        ok = false;

		return ok;
	}

	for (UInt i = 0; i < test->numVertices(); i++)
	{
		this->check(test, i);
	}

	if ((this->boundary) && (this->boundary->have(test)))
    {
        UInt numSides = test->numSides();

        for (UInt side = 0; side < numSides; side++)
        {
            UInt numVertices = test->numVerticesInSide(side);

            if (numVertices < 2)
            {
                continue;
            }

            Data::Vertex *v0 = test->getVertexInSide(0, side);

            Data::FrontElementList adjsBySide;

            Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(v0);

            for (Data::FrontElementList::iterator iter2 = adjs.begin();
                 iter2 != adjs.end(); iter2++)
            {
                Data::FrontElement *adj = (*iter2);

                if (adj == test)
                {
                    continue;
                }

                if (!this->boundary->have(adj))
                {
                    continue;
                }

                bool foundAll = true;

                for (UInt vertex = 1; vertex < numVertices; vertex++)
                {
                    Data::Vertex *v = test->getVertexInSide(vertex, side);

                    if (!adj->have(v))
                    {
                        foundAll = false;

                        break;
                    }
                }

                if (foundAll)
                {
                    adjsBySide.push_back(adj);
                }
            }

            if (adjsBySide.size() != 1)
            {
                if (adjsBySide.empty())
                {
                    std::cout << "front element " << test->text() << " has no adjacent front element in side " << side << " of vertices";

                    for (UInt vertex = 0; vertex < numVertices; vertex++)
                    {
                        Data::Vertex *v = test->getVertexInSide(vertex, side);

                        std::cout << " " << v->getId();
                    }

                    std::cout << std::endl;
                }
                else
                {
                    std::cout << "front element " << test->text() << " has more than one adjacent front element in side " << side << " of vertices";

                    for (UInt vertex = 0; vertex < numVertices; vertex++)
                    {
                        Data::Vertex *v = test->getVertexInSide(vertex, side);

                        std::cout << " " << v->getId();
                    }

                    std::cout << ":";

                    for (Data::FrontElementList::iterator iter2 = adjsBySide.begin();
                         iter2 != adjsBySide.end(); iter2++)
                    {
                        std::cout << " " << (*iter2)->text();

                        if (iter2 != --adjsBySide.end())
                        {
                            std::cout << ";";
                        }
                    }

                    std::cout << std::endl;
                }

                this->consistent = false;

                ok = false;
            }
        }
    }

	std::pair<IdFEMap::const_iterator, IdFEMap::const_iterator> idp = this->idfeMap.equal_range(test->getId());

	for (IdFEMap::const_iterator iter2 = idp.first; iter2 != idp.second; iter2++)
    {
        Data::FrontElement *other = (*iter2).second;

        if ((other != test) && (other->getId() == test->getId()))
		{
			std::cout << "front element " << test->text() << " has other with same id: " << other->text() << std::endl;

			this->consistent = false;

			ok = false;
		}
    }

	//Data::Mesh::ConstFrontElementIterator iter2 = iter;
	std::pair<RFEMap::const_iterator, RFEMap::const_iterator> p = this->feMap.equal_range(test->getShape()->box().getMin()->getCoord(0));

	UInt appear = 0;

	//for (iter2++; iter2 != end; iter2++)
    for (RFEMap::const_iterator iter2 = p.first; iter2 != p.second; iter2++)
	{
		//Data::FrontElement *other = (*iter2);
		Data::FrontElement *other = (*iter2).second;

		if (test == other)
		{
			appear++;

		    if (appear == 1)
            {
                continue;
            }

			std::cout << "front element " << test->text() << " appears twice in mesh" << std::endl;

			this->consistent = false;

			ok = false;
		}

		/*if (other->getId() == test->getId())
		{
			std::cout << "front element " << test->text() << " has other with same id: " << other->text() << std::endl;

			this->consistent = false;

			ok = false;
		}*/

		if (other->getShape() == test->getShape())
		{
			std::cout << "front element " << test->text() << " and front element " << other->text() << " share the same shape" << std::endl;

			this->consistent = false;

			ok = false;
		}

		if ((other->getShape()) && (test->getShape()->match(*other->getShape())))
		{
			std::cout << "front element " << test->text() << " and front element " << other->text() << " have the same shape" << std::endl;

			this->consistent = false;

			ok = false;
		}

		if (test->numVertices() == other->numVertices())
		{
			bool allVertices = true, allPoints = true, allGeometricPoints = true;

			for (UInt i = 0; i < test->numVertices(); i++)
			{
				Data::Vertex *v = test->getVertex(i);

				bool foundVertex = false, foundPoint = false, foundGeometricPoint = false;

				for (UInt j = 0; j < other->numVertices(); j++)
				{
					Data::Vertex *ov = other->getVertex(j);

					if (v == ov)
					{
						foundVertex = true;
					}

					if (v->getPoint() == ov->getPoint())
					{
						foundPoint = true;
					}

					if (v->getPoint()->match(ov->getPoint()))
					{
						foundGeometricPoint = true;
					}
				}

				if (!foundVertex)
				{
					allVertices = false;
				}

				if (!foundPoint)
				{
					allPoints = false;
				}

				if (!foundGeometricPoint)
				{
					allGeometricPoints = false;
				}

				if ((!allVertices) && (!allPoints) && (!allGeometricPoints))
				{
					break;
				}
			}

			bool crack = false;

			if ((allVertices) || (allPoints) || (allGeometricPoints))
			{
				Real dot1 = 0.0, dot2 = 0.0;

				if (this->dimension == 2)
				{
					dot1 = static_cast<Data::Edge2D *>(test->getShape())->normal().dot(static_cast<Data::Edge2D *>(other->getShape())->normal());
					dot2 = static_cast<Data::Edge2D *>(test->getShape())->normal().dot(static_cast<Data::Edge2D *>(test->getShape())->normal());
				}
				else
				{
					dot1 = static_cast<Data::Polygon *>(test->getShape())->normal().dot(static_cast<Data::Polygon *>(other->getShape())->normal());
					dot2 = static_cast<Data::Polygon *>(test->getShape())->normal().dot(static_cast<Data::Polygon *>(test->getShape())->normal());
				}

				//dot1 and dot2 should be opposite (avoid normalization)
				if (std::fabs(dot1 + dot2) <= Data::getTolerance())
				{
					crack = true;
				}
			}

			if (crack)
			{
				std::cout << "front element " << test->text() << " and front element " << other->text() << " were detected as in a crack" << std::endl;
			}
			else
			{
				if (allVertices)
				{
					std::cout << "front element " << test->text() << " and front element " << other->text() << " share the same vertices" << std::endl;

					this->consistent = false;

                    ok = false;
				}

				if (allPoints)
				{
					std::cout << "front element " << test->text() << " and front element " << other->text() << " share the same points" << std::endl;

					this->consistent = false;

                    ok = false;
				}

				if (allGeometricPoints)
				{
					std::cout << "front element " << test->text() << " and front element " << other->text() << " have the same points" << std::endl;

					this->consistent = false;

                    ok = false;
				}
			}
		}
	}

	if (this->testElements)
	{
		//Data::ElementList elements = fetest->getAdjacency();
		Data::ElementList elements = this->mesh->adjacency(test);

		if (elements.size() == 0)
		{
			std::cout << "front element " << test->text() << " has no adjacent element" << std::endl;

			this->consistent = false;

			ok = false;
		}
		else if ((elements.size() == 1) || (elements.size() == 2))
		{
			bool inboundary = (this->boundary) ? this->boundary->have(test) : false;
			bool infront = (this->front) ? this->front->have(test) : false;

			if ((elements.size() == 1) && (!inboundary) && (!infront))
			{
				std::cout << "front element " << test->text() << " has only one adjacent element: " << elements.front()->text() << ", but is neither in boundary nor in front" << std::endl;

				this->consistent = false;

                ok = false;
			}

			if (elements.size() == 2)
			{
				if (inboundary)
				{
					std::cout << "front element " << test->text() << " has two adjacent elements: " << elements.front()->text() << " and " << elements.back()->text() << ", but is in boundary" << std::endl;

					this->consistent = false;

                    ok = false;
				}

				if (infront)
				{
					std::cout << "front element " << test->text() << " has two adjacent elements: " << elements.front()->text() << " and " << elements.back()->text() << ", but is in front" << std::endl;

					this->consistent = false;

                    ok = false;
				}
			}
		}
		else if (elements.size() > 2)
		{
			std::cout << "front element " << test->text() << " has more than 2 adjacent elements" << std::endl;

			this->consistent = false;

			ok = false;
		}

		while (!elements.empty())
		{
			Data::Element *e = elements.front();

			elements.pop_front();

			if (!this->mesh->have(e))
			{
				std::cout << "front element " << test->text() << " has element " << e->text() << " that is not in this mesh" << std::endl;

				this->consistent = false;

                ok = false;
			}

			UInt count = 0;

			for (UInt i = 0; i < test->numVertices(); i++)
			{
				for (UInt j = 0; j < e->numVertices(); j++)
				{
					if (test->getVertex(i) == e->getVertex(j))
					{
						count++;
					}
				}
			}

			if (count != test->numVertices())
			{
				std::cout << "front element " << test->text() << " and element " << e->text() << " share " << count << " vertices, instead of " << test->numVertices() << std::endl;

				this->consistent = false;

                ok = false;
			}
		}
	}

	return ok;
}

//bool Performer::MeshTopologyChecker::check(const Data::Mesh::ConstElementIterator &iter, const Data::Mesh::ConstElementIterator &end) const
bool Performer::MeshTopologyChecker::check(const Data::Element *test) const
{
	//Data::Element *test = (*iter);

	bool ok = true;

	if (!test->getShape())
	{
		std::cout << "element " << test->text() << " has no shape, skipping element" << std::endl;

		this->consistent = false;

        ok = false;

		return ok;
	}

	if (test->getShape()->numPoints() != test->numVertices())
	{
#if USE_GUI
		std::cout << "element " << test->text() << " has shape with different number of points: " << test->getShape()->text() << ", skipping element" << std::endl;
#else
		std::cout << "element " << test->text() << " has shape with different number of points: " << test->getShape()->numPoints() << ", skipping element" << std::endl;
#endif //#if USE_GUI

		this->consistent = false;

        ok = false;

		return ok;
	}

	for (UInt i = 0; i < test->numVertices(); i++)
	{
		this->check(test, i);
	}

	UInt numSides = test->numSides();

	for (UInt side = 0; side < numSides; side++)
    {
        UInt numVertices = test->numVerticesInSide(side);

        if (numVertices < 2)
        {
            continue;
        }

        Data::Vertex *v0 = test->getVertexInSide(0, side);

        Data::ElementList adjsBySide;

        Data::ElementList adjs = this->mesh->adjacency(v0);

        for (Data::ElementList::iterator iter2 = adjs.begin();
             iter2 != adjs.end(); iter2++)
        {
            Data::Element *adj = (*iter2);

            if (adj == test)
            {
                continue;
            }

            bool foundAll = true;

            for (UInt vertex = 1; vertex < numVertices; vertex++)
            {
                Data::Vertex *v = test->getVertexInSide(vertex, side);

                if (!adj->have(v))
                {
                    foundAll = false;

                    break;
                }
            }

            if (foundAll)
            {
                adjsBySide.push_back(adj);
            }
        }

        if (this->boundary)
        {
            Data::FrontElementList badjs = this->boundary->adjacency(v0);

            for (Data::FrontElementList::iterator iter2 = badjs.begin();
                 iter2 != badjs.end(); iter2++)
            {
                Data::FrontElement *adj = (*iter2);

                if (adj == test)
                {
                    continue;
                }

                bool foundAll = true;

                for (UInt vertex = 1; vertex < numVertices; vertex++)
                {
                    Data::Vertex *v = test->getVertexInSide(vertex, side);

                    if (!adj->have(v))
                    {
                        foundAll = false;

                        break;
                    }
                }

                if (foundAll)
                {
                    adjsBySide.push_back(adj);
                }
            }
        }

        if (adjsBySide.size() != 1)
        {
            if (adjsBySide.empty())
            {
                std::cout << "element " << test->text() << " has no adjacent element or front element in side " << side << " of vertices";

                for (UInt vertex = 0; vertex < numVertices; vertex++)
                {
                    Data::Vertex *v = test->getVertexInSide(vertex, side);

                    std::cout << " " << v->getId();
                }

                std::cout << std::endl;
            }
            else
            {
                std::cout << "element " << test->text() << " has " << adjsBySide.size() << " adjacent elements or front elements in side " << side << " of vertices";

                for (UInt vertex = 0; vertex < numVertices; vertex++)
                {
                    Data::Vertex *v = test->getVertexInSide(vertex, side);

                    std::cout << " " << v->getId();
                }

                std::cout << ":";

                for (Data::ElementList::iterator iter2 = adjsBySide.begin();
                     iter2 != adjsBySide.end(); iter2++)
                {
                    bool fel = dynamic_cast<Data::FrontElement *>((*iter2));

                    std::cout << " " << ((fel) ? "front " : "") << "element " << (*iter2)->text();

                    if (iter2 != --adjsBySide.end())
                    {
                        std::cout << ";";
                    }
                }

                std::cout << std::endl;
            }

            this->consistent = false;

            ok = false;
        }
    }

	std::pair<IdEMap::const_iterator, IdEMap::const_iterator> idp = this->ideMap.equal_range(test->getId());

	for (IdEMap::const_iterator iter2 = idp.first; iter2 != idp.second; iter2++)
    {
        Data::Element *other = (*iter2).second;

        if ((other != test) && (other->getId() == test->getId()))
		{
			std::cout << "element " << test->text() << " has other with same id: " << other->text() << std::endl;

			this->consistent = false;

			ok = false;
		}
    }

	//Data::Mesh::ConstElementIterator iter2 = iter;
	std::pair<REMap::const_iterator, REMap::const_iterator> p = this->eMap.equal_range(test->getShape()->box().getMin()->getCoord(0));

	UInt appear = 0;

	//for (iter2++; iter2 != end; iter2++)
	for (REMap::const_iterator iter2 = p.first; iter2 != p.second; iter2++)
	{
		//Data::Element *other = (*iter2);
		Data::Element *other = (*iter2).second;

		if (test == other)
		{
			appear++;

		    if (appear == 1)
            {
                continue;
            }

			std::cout << "element " << test->text() << " appears twice in mesh" << std::endl;

			this->consistent = false;

			ok = false;
		}

		/*if (other->getId() == test->getId())
		{
			std::cout << "element " << test->text() << " has other with same id: " << other->text() << std::endl;

			this->consistent = false;

			ok = false;
		}*/

		if (other->getShape() == test->getShape())
		{
			std::cout << "element " << test->text() << " and element " << other->text() << " share the same shape" << std::endl;

			this->consistent = false;

			ok = false;
		}

		if ((other->getShape()) && (test->getShape()->match(*other->getShape())))
		{
			std::cout << "element " << test->text() << " and element " << other->text() << " have the same shape" << std::endl;

			this->consistent = false;

			ok = false;
		}
	}

	return ok;
}

bool Performer::MeshTopologyChecker::check(const Data::FrontElement *test, UInt i) const
{
	Data::Vertex *v = test->getVertex(i);

	if (!v)
	{
		std::cout << "front element " << test->text() << " has no " << i << "th vertex, skipping adjacent vertex" << std::endl;

		this->consistent = false;

		return false;
	}

	if (!this->mesh->have(v))
	{
		std::cout << "front element " << test->text() << " has vertex " << v->text() << " that is not in this mesh" << std::endl;

		this->consistent = false;

		return false;
	}

	return true;
}

bool Performer::MeshTopologyChecker::check(const Data::Element *test, UInt i) const
{
	Data::Vertex *v = test->getVertex(i);

	if (!v)
	{
		std::cout << "element " << test->text() << " has no " << i << "th vertex, skipping adjacent vertex" << std::endl;

		this->consistent = false;

		return false;
	}

	if (!this->mesh->have(v))
	{
		std::cout << "element " << test->text() << " has vertex " << v->text() << " that is not in this mesh" << std::endl;

		this->consistent = false;

		return false;
	}

	return true;
}
