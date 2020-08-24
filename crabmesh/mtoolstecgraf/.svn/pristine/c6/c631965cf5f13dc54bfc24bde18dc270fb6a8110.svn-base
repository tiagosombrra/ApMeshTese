#include "PMJMesh/GeometryFrontAdvancer.h"

#include "Data/State.h"
#include "Data/FrontElement.h"
#include "Data/BoxTreeCell.h"
#include "Data/BoxTree.h"
#include "Data/Box.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Vertex.h"
#include "Parallel/Communicator.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/VertexSearchTreeGenerator.h"
#include "PMJMesh/FrontElementSearchTreeGenerator.h"

#include "Data/GeometricSearchTree.h"
#include "Data/BoxSearchTreeCell.h"

using namespace PMJMesh;

PMJMesh::GeometryFrontAdvancer::GeometryFrontAdvancer(UInt dimension,
	const MJMesh::Boundary *boundary, const Data::BoxTree *tree,
	Parallel::Communicator *comm, Real rangeProportion, bool usingSearchTrees,
	bool testingBox) :
		MJMesh::GeometryFrontAdvancer(dimension, boundary, tree, usingSearchTrees),
		//onlyInBox(false), rangeInBox(false), notInTest(true), //teste do prof. creto
		onlyInBox(true),  rangeInBox(true),  notInTest(false) //default values
{
	this->setBox(NULL);

	//teste do prof. creto
	//this->onlyInBox = false;
	//this->rangeInBox = false;
	//this->notInTest = true;

	//default values:
	//this->onlyInBox = true;
	//this->rangeInBox = true;
	//this->notInTest = false;

	this->setCommunicator(comm);
	this->setTestingBox(testingBox);
	this->setRangeProportion(rangeProportion);

#if USE_DELETE_FELS
    this->setOldFront(NULL);
#endif //#if USE_DELETE_FELS

	this->treeRootBox = NULL;
}

PMJMesh::GeometryFrontAdvancer::~GeometryFrontAdvancer()
{
	if (this->treeRootBox)
	{
		delete this->treeRootBox;
	}
}

UInt PMJMesh::GeometryFrontAdvancer::getDimension() const
{
	return this->dimension;
}

void PMJMesh::GeometryFrontAdvancer::setTestingBox(bool testingBox)
{
	this->testingBox = testingBox;
}

bool PMJMesh::GeometryFrontAdvancer::isTestingBox() const
{
	return this->testingBox;
}

void PMJMesh::GeometryFrontAdvancer::setRangeProportion(Real rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

Real PMJMesh::GeometryFrontAdvancer::getRangeProportion() const
{
	return this->rangeProportion;
}

#if USE_DELETE_FELS
void PMJMesh::GeometryFrontAdvancer::setOldFront(const PMJMesh::Front *oldFront)
{
    this->oldFront = oldFront;
}
#endif //#if USE_DELETE_FELS

const MJMesh::Boundary *PMJMesh::GeometryFrontAdvancer::getBoundary() const
{
	return static_cast<const MJMesh::Boundary *>(this->boundary);
}

const Data::BoxTree *PMJMesh::GeometryFrontAdvancer::getTree() const
{
	return this->tree;
}

void PMJMesh::GeometryFrontAdvancer::setCommunicator(Parallel::Communicator *comm)
{
	this->comm = comm;
}

void PMJMesh::GeometryFrontAdvancer::setBox(const Data::Box *box)
{
	this->box = box;
}

const Data::Box *PMJMesh::GeometryFrontAdvancer::getBox() const
{
	return this->box;
}

/*const Data::FrontElementList &PMJMesh::GeometryFrontAdvancer::getCrossing() const
{
	return this->crossing;
}*/

/*const Data::BoxTreeCell *PMJMesh::GeometryFrontAdvancer::cell(const Data::Point &p) const
{
	return this->vstree ?
		static_cast<Data::BoxTreeCell *>(static_cast<Data::RecursiveDecompositionTreeCell *>(static_cast<Data::BoxSearchTreeCell *>(this->vstree->getRoot())->getCell())->find(p)) :
		MJMesh::GeometryFrontAdvancer::cell(p);
}*/

MJMesh::VertexSearchTreeGenerator *PMJMesh::GeometryFrontAdvancer::makeVertexSearchTreeGenerator(const MJMesh::Front *front) const
{
	if (front)
	{
		return new PMJMesh::VertexSearchTreeGenerator(front, this->tree, this->box);
	}

	return new PMJMesh::VertexSearchTreeGenerator(static_cast<MJMesh::Front *>(this->front), this->tree, this->box);
}

MJMesh::FrontElementSearchTreeGenerator *PMJMesh::GeometryFrontAdvancer::makeFrontElementSearchTreeGenerator(const MJMesh::Front *front) const
{
	if (front)
	{
		return new PMJMesh::FrontElementSearchTreeGenerator(front, this->tree, this->box);
	}

	return new PMJMesh::FrontElementSearchTreeGenerator(static_cast<MJMesh::Front *>(this->front), this->tree, this->box);
}

void PMJMesh::GeometryFrontAdvancer::execute()
{
	//Real debugstart = Data::time();

	if (!this->front)
	{
		this->status = MJMesh::GeometryFrontAdvancer::ended;

		//this->timeInit += Data::time() - debugstart;

		return;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	//this->crossing.clear();
	//Data::VertexSet crossingVertices;

	static_cast<PMJMesh::Front *>(this->front)->moveCrossingToRejected();

	/*//if (!this->isTestingBox())
	if (false)
	{
		this->crossing = static_cast<PMJMesh::Front *>(this->front)->getCrossing();

		static_cast<PMJMesh::Front *>(this->front)->moveCrossingToRejected();

		///TODO funciona somente em 2D
		if (this->dimension == 2)
		{
			Data::FrontElementList removed, disconsider;

			*//* //comentado por Markos em 16/12/2011

			bool changed = false;

			do
			{
				changed = false;

				for (Data::FrontElementList::iterator iter = this->crossing.begin();
					 iter != this->crossing.end();)
				{
					//std::cout << "this->crossing.size = " << this->crossing.size() << std::endl;

					Data::FrontElement *fel = (*iter);

					UInt count[fel->numVertices()];

					for (UInt i = 0; i < fel->numVertices(); i++)
					{
						MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

						Data::FrontElementList adjs = v->getAdjacentFrontElements();

						count[i] = 1;

						while (!adjs.empty())
						{
							Data::FrontElement *adj = adjs.front();

							adjs.pop_front();

							if (adj == fel)
							{
								continue;
							}

							if (front->haveAny(Data::FrontElementList(1, adj)))
							{
								count[i]++;

								//std::cout << "adjacent " << adj->getId() << " is on the front" << std::endl;
							}
						}

						//std::cout << "fel->id = " << fel->getId() << ", v->id = " << v->getId() << ", count = " << count[i] << std::endl;
					}

					//there are only two vertices
					bool remove = false;

					if ((count[0] == 1) || (count[1] == 1))
					{
						if (count[0] == 1)
						{
							if (crossingVertices.insert(fel->getVertex(0)).second)
							{
								this->front->remove(fel->getVertex(0));
							}
						}

						if (count[1] == 1)
						{
							if (crossingVertices.insert(fel->getVertex(1)).second)
							{
								this->front->remove(fel->getVertex(1));
							}
						}

						remove = true;
					}
					else
					{
						iter++;

						continue;
					}

					if (remove)
					{
						//std::cout << "fel " << fel->getId() << " is non-manifold" << std::endl;

						this->front->remove(fel);

						removed.push_back(fel);

						iter = this->crossing.erase(iter);

						changed = true;
					}
				}
			}
			while (changed);*//*

			//adicionado por Markos em 16/12/2011
			VertexMultiMap countVertices;

			VertexList vertices = this->front->getVertices();

			while (!vertices.empty())
			{
				MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());

				vertices.pop_front();

				if (this->comm->isMaster())
				{
					UInt count = 0;

					Data::FrontElementList fels = v->getAdjacentFrontElements();

					while (!fels.empty())
					{
						Data::FrontElement *fel = fels.front();

						fels.pop_front();

						if (this->front->haveAny(Data::FrontElementList(1, fel)))
						{
							count++;
						}
						else
						{
							disconsider.push_back(fel);
						}
					}

					countVertices.insert(std::make_pair(count, v));
				}
				else
				{
					countVertices.insert(std::make_pair(static_cast<UInt>(v->getAdjacentFrontElements().size()), v));
				}
			}

			while (true)
			{
				//VertexMultiMap::iterator iter = countVertices.lower_bound(1);
				VertexMultiMap::iterator iter = countVertices.begin();

				if ((*iter).first != 1)
				{
					break;
				}

				MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second);

				countVertices.erase(iter);

				if (crossingVertices.insert(v).second)
				{
					this->front->remove(v);

					Data::FrontElementList fels = v->getAdjacentFrontElements();

					while (!fels.empty())
					{
						Data::FrontElement *fel = fels.front();

						fels.pop_front();

						if ((this->comm->isMaster()) && (std::find(disconsider.begin(), disconsider.end(), fel) != disconsider.end()))
						{
							continue;
						}

						if (std::find(removed.begin(), removed.end(), fel) == removed.end())
						{
							removed.push_back(fel);

							this->front->remove(fel);

							Data::Vertex *other = (fel->getVertex(0) == v) ? fel->getVertex(1) : fel->getVertex(0);

							for (iter = countVertices.begin();
								 iter != countVertices.end(); iter++)
							{
								if ((*iter).second == other)
								{
									UInt count = (*iter).first;

									countVertices.erase(iter);

									if (count == 1)
									{
										if (crossingVertices.insert(other).second)
										{
											this->front->remove(other);
										}
									}
									else
									{
										countVertices.insert(std::make_pair(count - 1, other));
									}

									break;
								}
							}
						}
					}
				}
			}

			this->crossing.swap(removed);
		}
		else
		{
			///TODO remove non manifold triangles in 3 dimensions
		}
	}*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry = Data::time() - start;

	start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#if USE_EXPERIMENTAL_3
	Data::Point2D min2d, max2d;
	Data::Point3D min3d, max3d;

	Data::Point &min = (this->dimension == 2) ?
        (min2d = static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->min2D(), static_cast<Data::Point &>(min2d)) :
        (min3d = static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->min3D(), static_cast<Data::Point &>(min3d));
    Data::Point &max = (this->dimension == 2) ?
        (max2d = static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->max2D(), static_cast<Data::Point &>(max2d)) :
        (max3d = static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->max3D(), static_cast<Data::Point &>(max3d));

	this->treeRootBox = new Data::Box(min, max);
#else
	this->treeRootBox = new Data::Box(static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->min(), static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->max());
#endif //#if USE_EXPERIMENTAL_3

	if (this->usingSearchTrees)
	{
		//#pragma omp critical (executing)
		{
			//Real debugstart = Data::time();

			this->vstree = this->makeVertexSearchTree();
			this->festree = makeFrontElementSearchTree();

			static_cast<MJMesh::Front *>(this->front)->setVSTree(this->vstree);
			static_cast<MJMesh::Front *>(this->front)->setFESTree(this->festree);

			//this->timeInitMakingTrees += Data::time() - debugstart;

			//std::cout << "this->timeInitMakingTrees = " << this->timeInitMakingTrees << std::endl;
		}
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTrees = Data::time() - start;

	start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (!this->mesh)
	{
		this->mesh = this->makeMesh();

		static_cast<PMJMesh::Front *>(this->front)->moveRejectedToElements();

		for (Data::Front::ConstVertexIterator iter = this->front->vBegin(), end = this->front->vEnd();
			 iter != end; ++iter)
		{
			this->mesh->add((*iter), false);
		}

		for (Data::Front::ConstFrontElementIterator iter = this->front->eBegin(0), end = this->front->eEnd(0);
			 iter != end; ++iter)
		{
			this->mesh->add((*iter));
		}

		for (Data::Front::ConstFrontElementIterator iter = this->front->eBegin(1), end = this->front->eEnd(1);
			 iter != end; ++iter)
		{
			this->mesh->add((*iter));
		}

		/*for (Data::Front::ConstFrontElementIterator iter = this->front->eBegin(2), end = this->front->eEnd(2);
			 iter != end; ++iter)
		{
			this->mesh->add((*iter));
		}*/

		this->status = FrontAdvancer::init;
	}

	//this->timeInit += Data::time() - debugstart;

	//std::cout << "time for initializing GeometryFrontAdvancer = " << Data::time() - start << std::endl;

	//#pragma omp critical (executing)
	{
		MJMesh::GeometryFrontAdvancer::execute();

		/*//if adicionado por Markos em 16/12/2011
		if (!this->isTestingBox())
		{
			for (Data::VertexSet::iterator iter = crossingVertices.begin();
				 iter != crossingVertices.end(); iter++)
			{
				this->mesh->add((*iter), false);
			}

			for (Data::FrontElementList::iterator iter = this->crossing.begin();
				 iter != this->crossing.end(); iter++)
			{
				this->mesh->add((*iter));
			}
		}*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeGeometry += Data::time() - start;

		start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	}

	//debugstart = Data::time();

	delete this->treeRootBox;

	this->treeRootBox = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTrees += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	//this->timeGeometry = Data::time() - start;

	//this->timeEnd += Data::time() - debugstart;
}

bool PMJMesh::GeometryFrontAdvancer::out(const Data::Point &p) const
{
	return this->out(&p);
}

bool PMJMesh::GeometryFrontAdvancer::out(const Data::Point *p) const
{
	//return this->notInTest ? !this->box->in(p) : this->box->out(p);
	return this->notInTest ? !this->box->optIn(p) : this->box->optOut(p);
}

bool PMJMesh::GeometryFrontAdvancer::outRange(const Data::FrontElement */*e*/, const Data::Point *p, Real radius) const
{
#if USE_EXPERIMENTAL_3
	Data::Point2D min2d, max2d;
	Data::Point3D min3d, max3d;

	Data::Point &min = (this->dimension == 2) ? static_cast<Data::Point &>(min2d) : static_cast<Data::Point &>(min3d);
	Data::Point &max = (this->dimension == 2) ? static_cast<Data::Point &>(max2d) : static_cast<Data::Point &>(max3d);
#else
	Data::Point min(this->dimension), max(this->dimension);
#endif //#if USE_EXPERIMENTAL_3

	if (this->treeRootBox)
	{
		for (UInt i = 0; i < this->dimension; i++)
		{
			min.setCoord(i, std::max(p->getCoord(i) - radius, this->treeRootBox->getMin()->getCoord(i)) + Data::getTolerance());
			max.setCoord(i, std::min(p->getCoord(i) + radius, this->treeRootBox->getMax()->getCoord(i)) - Data::getTolerance());
		}
	}
	else
	{
#if USE_EXPERIMENTAL_3
        if (this->dimension == 2)
        {
            for (UInt i = 0; i < this->dimension; i++)
            {
                min.setCoord(i, std::max(p->getCoord(i) - radius, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->min2D().getCoord(i)) + Data::getTolerance());
                max.setCoord(i, std::min(p->getCoord(i) + radius, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->max2D().getCoord(i)) - Data::getTolerance());
            }
        }
        else
        {
            for (UInt i = 0; i < this->dimension; i++)
            {
                min.setCoord(i, std::max(p->getCoord(i) - radius, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->min3D().getCoord(i)) + Data::getTolerance());
                max.setCoord(i, std::min(p->getCoord(i) + radius, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->max3D().getCoord(i)) - Data::getTolerance());
            }
        }
#else
		for (UInt i = 0; i < this->dimension; i++)
		{
			min.setCoord(i, std::max(p->getCoord(i) - radius, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->min().getCoord(i)) + Data::getTolerance());
			max.setCoord(i, std::min(p->getCoord(i) + radius, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())->max().getCoord(i)) - Data::getTolerance());
		}
#endif //#if USE_EXPERIMENTAL_3
	}

	return ((this->out(min)) || (this->out(max)));
}

bool PMJMesh::GeometryFrontAdvancer::isRestricted(const Data::FrontElement *e, const Data::Point *point, Real radius) const
{
	return ((this->out(point)) ||
			((this->rangeInBox) && (this->outRange(e, point, this->rangeProportion*radius))));

	/*if (this->out(point))
	{
		return true;
	}

	if (this->rangeInBox)
	{
		if (this->outRange(e, point, this->rangeProportion*radius))
		{
			return true;
		}
	}

	return false;*/
}

void PMJMesh::GeometryFrontAdvancer::restricted(const Data::FrontElement *e, Data::PointList &ideal, Real radius) const
{
	if ((!this->isTestingBox()) || (!this->onlyInBox))
	{
		MJMesh::GeometryFrontAdvancer::restricted(e, ideal, radius);

		return;
	}

	if (!this->box)
	{
		for (Data::PointList::iterator iter = ideal.begin();
			 iter != ideal.end(); iter++)
		{
			delete (*iter);
		}

		ideal.clear();

		return;
	}

	for (Data::PointList::iterator iter = ideal.begin();
		 iter != ideal.end();)
	{
		Data::Point *p = (*iter);

		if (this->isRestricted(e, p, radius))
		{
			iter = ideal.erase(iter);

			delete p;
		}
		else
		{
			iter++;
		}
	}

	MJMesh::GeometryFrontAdvancer::restricted(e, ideal, radius);

	if (ideal.empty())
	{
		this->reject(const_cast<Data::FrontElement *>(e), 2);
	}
}

void PMJMesh::GeometryFrontAdvancer::restricted(Data::VertexList &candidates) const
{
	if ((!this->isTestingBox()) || (!this->onlyInBox))
	{
		return;
	}

	for (Data::VertexList::iterator iter = candidates.begin();
		 iter != candidates.end();)
	{
		if (this->out((*iter)->getPoint()))
		{
			iter = candidates.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

/*Data::Vertex *PMJMesh::GeometryFrontAdvancer::bestValid(const Data::FrontElement *element,
	Data::PointList &candidates) const
{
	for (Data::PointList::iterator iter = candidates.begin();
		 iter != candidates.end();)
	{
		if (this->out((*iter)))
		{
			iter = candidates.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return MJMesh::GeometryFrontAdvancer::bestValid(element, candidates);
}*/

Data::Vertex *PMJMesh::GeometryFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, Real radius) const
{
	this->restricted(candidates);

	return MJMesh::GeometryFrontAdvancer::bestValid(element, candidates, ideal, radius);
}

Data::Vertex *PMJMesh::GeometryFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, Real radius) const
{
	this->restricted(candidates);

	return MJMesh::GeometryFrontAdvancer::bestValid(element, candidates, ideal, vertices, radius);
}

Data::Vertex *PMJMesh::GeometryFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::FrontElementList &frontElements, Real radius) const
{
	this->restricted(candidates);

	return MJMesh::GeometryFrontAdvancer::bestValid(element, candidates, ideal, frontElements, radius);
}

Data::Vertex *PMJMesh::GeometryFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, const Data::FrontElementList &frontElements, Real radius) const
{
	this->restricted(candidates);

	return MJMesh::GeometryFrontAdvancer::bestValid(element, candidates, ideal, vertices, frontElements, radius);
}

Data::VertexList PMJMesh::GeometryFrontAdvancer::valid(const Data::FrontElement *element, Data::VertexList &candidates, Real radius) const
{
	if ((!this->isTestingBox()) || (!this->onlyInBox))
	{
		return MJMesh::GeometryFrontAdvancer::valid(element, candidates, radius);
	}

	Data::VertexList valid;

	while (!candidates.empty())
	{
		Data::Vertex *candidate = candidates.back();
		candidates.pop_back();

		if (!this->out(candidate->getPoint()))
		{
			valid.push_back(candidate);
		}
	}

	return MJMesh::GeometryFrontAdvancer::valid(element, valid, radius);
}

Data::FrontElement *PMJMesh::GeometryFrontAdvancer::next() const
{
/*#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

	if (!this->isTestingBox())
	{
/*#if DEBUG_TIMING_PMJMESH
		Data::FrontElement *fel = MJMesh::GeometryFrontAdvancer::next();

		this->timeNextGeometry += Data::time() - debugstart;

		return fel;
#else*/
		return MJMesh::GeometryFrontAdvancer::next();
//#endif //#if DEBUG_TIMING_PMJMESH
	}

	Data::FrontElement *fel = NULL;

	while (!fel)
	{
		/*if (this->isEmpty())
		{
			break;
		}*/

		fel = MJMesh::GeometryFrontAdvancer::next();

		if (!fel)
		{
			break;
		}

		Data::GeometricShape *s = fel->getShape();

		if (this->onlyInBox)
		{
			for (UInt i = 0; i < s->numPoints(); i++)
			{
/*#if DEBUG_TIMING_PMJMESH
				this->timeNextGeometry += Data::time() - debugstart;

				debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

				if (this->out((s->getPoint(i))))
				{
/*#if DEBUG_TIMING_PMJMESH
					this->timeNextGeometryOut += Data::time() - debugstart;

					debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

					if (!this->reject(fel, 1))
					{
/*#if DEBUG_TIMING_PMJMESH
						this->timeNextGeometryReject += Data::time() - debugstart;

						debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

						this->status = FrontAdvancer::rejected;

/*#if DEBUG_TIMING_PMJMESH
						this->timeNextGeometry += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH*/

						return NULL;
					}

/*#if DEBUG_TIMING_PMJMESH
					this->timeNextGeometryReject += Data::time() - debugstart;

					debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

					fel = NULL;

					break;
				}

/*#if DEBUG_TIMING_PMJMESH
				this->timeNextGeometryOut += Data::time() - debugstart;

				debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/
			}
		}
		else
		{
			bool allOut = true;

			for (UInt i = 0; i < s->numPoints(); i++)
			{
				if (!this->out((s->getPoint(i))))
				{
					allOut = false;

					break;
				}
			}

			if (allOut)
			{
/*#if DEBUG_TIMING_PMJMESH
				this->timeNextGeometry += Data::time() - debugstart;

				debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

				if (!this->reject(fel, 1))
				{
/*#if DEBUG_TIMING_PMJMESH
					this->timeNextGeometryReject += Data::time() - debugstart;

					debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

					this->status = FrontAdvancer::rejected;

/*#if DEBUG_TIMING_PMJMESH
					this->timeNextGeometry += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH*/

					return NULL;
				}

/*#if DEBUG_TIMING_PMJMESH
				this->timeNextGeometryReject += Data::time() - debugstart;

				debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

				fel = NULL;
			}
		}
	}

/*#if DEBUG_TIMING_PMJMESH
	this->timeNextGeometry += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH*/

	return fel;
}

#if USE_DELETE_FELS
bool PMJMesh::GeometryFrontAdvancer::shouldDelete(const Data::FrontElement *e) const
{
    return ((!this->front->have(e)) && (!this->boundary->have(e)) && (!this->oldFront->have(e)));
}
#endif //#if USE_DELETE_FELS

/*bool PMJMesh::GeometryFrontAdvancer::better(const Data::FrontElement *element, Data::Vertex *candidate,
	Data::Vertex * *//*old*//*) const
{
	*//*//if candidate and old do not have the same coordinates, than they are in
	// the same circuncircle, which means that the old vertex is kept
	//THIS DOES NOT HAPPEN, BECAUSE AN EQUAL TEST IS PERFORMED PRIOR TO ENTERING THIS METHOD
	if (!candidate->getPoint()->equal(*(old->getPoint())))
	{
		return false;
	}*//*

	for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(candidate)->feBegin();
         iter != static_cast<MJMesh::Vertex *>(candidate)->feEnd(); iter++)
	{
		Data::FrontElement *e = (*iter);

		//if (this->boundary->have(e, true))
		if (this->boundary->have(e, false))
		{
			if (!this->accordingToNormal(element, e))
            {
                return false;
            }
		}
	}

	return true;

	*//*Data::FrontElementList els;

	for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(candidate)->feBegin();
         iter != static_cast<MJMesh::Vertex *>(candidate)->feEnd(); iter++)
    {
        if (this->boundary->have((*iter), false))
        {
            els.push_back((*iter));
        }
    }

    if (els.empty())
    {
        return false;
    }

    if (els.size() == 1)
    {
        Data::FrontElement *adj = els.front();

        if (!this->accordingToNormal(element, adj))
        {
            return false;
        }

        return true;
    }

    if (els.size() == 2)
    {
        Data::FrontElement *adj1 = els.front();
        Data::FrontElement *adj2 = els.back();

        Data::Vertex *v2 = NULL;

        for (UInt i = 0; i < adj2->numVertices(); i++)
        {
            if (adj1->have(adj2->getVertex(i)))
            {
                continue;
            }

            v2 = adj2->getVertex(i);

            break;
        }

        if (!v2)
        {
            return false;
        }

        bool doAnd = adj1->accordingToNormal(v2, false);

        if (doAnd)
        {
            if (!this->accordingToNormal(element, adj1))
            {
                return false;
            }

            if (!this->accordingToNormal(element, adj2))
            {
                return false;
            }

            //equivalent to
            // this->accordingToNormal(element, adj1) && this->accordingToNormal(element, adj2)

            return true;
        }
        else
        {
            if (!this->accordingToNormal(element, adj1))
            {
                if (!this->accordingToNormal(element, adj2))
                {
                    return false;
                }
            }

            //equivalent to
            // this->accordingToNormal(element, adj1) || this->accordingToNormal(element, adj2)

            return true;
        }

        return true;
    }

    typedef std::list<Data::FrontElementList > FrontElementListList;

    FrontElementListList adjs;

    adjs.push_back(Data::FrontElementList());

    adjs.front().push_back(els.front());

    els.pop_front();

    UInt dimension = candidate->getPoint()->dimension();

    while (!els.empty())
    {
        for (Data::FrontElementList::iterator iter = els.begin();
             iter != els.end(); iter++)
        {
            Data::FrontElement *fel = (*iter);

            bool found = false;
            bool inserted = false;

            for (FrontElementListList::iterator it = adjs.begin();
                 it != adjs.end(); it++)
            {
                for (Data::FrontElementList::iterator iter2 = (*it).begin();
                     iter2 != (*it).end(); iter2++)
                {
                    Data::FrontElement *ofel = (*iter2);

                    UInt count = 0;
                    Data::Vertex *v2 = NULL;

                    for (UInt i = 0; i < ofel->numVertices(); i++)
                    {
                        if (fel->have(ofel->getVertex(i)))
                        {
                            count++;
                        }
                        else
                        {
                            v2 = ofel->getVertex(i);
                        }

                        if ((count == dimension - 1) && (v2))
                        {
                            break;
                        }
                    }

                    if ((count != dimension - 1) || (!v2))
                    {
                        continue;
                    }

                    found = true;

                    bool doAnd = fel->accordingToNormal(v2, false);

                    if (doAnd)
                    {
                        (*it).push_back(fel);

                        inserted = true;

                        break;
                    }
                }

                if (found)
                {
                    if (!inserted)
                    {
                        adjs.push_back(Data::FrontElementList());

                        adjs.back().push_back(fel);
                    }

                    break;
                }
            }

            if (found)
            {
                els.erase(iter);

                break;
            }
        }
    }

    while (!adjs.empty())
    {
        bool according = true;

        while (!adjs.front().empty())
        {
            Data::FrontElement *adj = adjs.front().front();

            adjs.front().pop_front();

            if (!this->accordingToNormal(element, adj))
            {
                according = false;

                break;
            }
        }

        if (according)
        {
            return true;
        }

        adjs.pop_front();
    }

	return false;*//*
}*/
