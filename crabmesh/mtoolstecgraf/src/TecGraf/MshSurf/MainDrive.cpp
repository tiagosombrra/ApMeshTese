#include "TecGraf/MshSurf/MainDrive.h"

#if USE_TECGRAF_MSH_SURF

#include "Data/State.h"
#include "Data/Triangle3D.h"

#include "Performer/SequentialIdManager.h"

#include "PMJMesh2/APrioriSupportMeshLaplacianSmoother.h"

#include "MJMesh/Edge3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/BoundaryBuilder.h"

#include "IO/GenFileReader.h"
#include "IO/GenFileWriter.h"

using namespace TecGraf;

TecGraf::MshSurf::MainDrive::MainDrive() :
	Performer::MainDrive::MainDrive()
{
	this->algorithm = UNKNOWN;

	this->analyticalMaxElementSize = 0.0;
	this->analyticalComputeCurvature = false;

	this->supportMaxElementSize = 0.0;
	this->supportMaxNumberElements = -1;
	this->supportComputeCurvature = false;
	this->supportInvertMesh = false;
    this->supportMesh = NULL;
    this->supportLoadingSupportMesh = false;

    this->messageFunction = NULL;

	this->boundary = NULL;
	//this->tree = NULL;
	this->mesh = NULL;

#if USE_GUI
	//this->front = NULL;
#endif //#if USE_GUI

#if USE_TIME_MEASUREMENTS
	this->runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	this->idManager = NULL;
	this->boundaryBuilder = NULL;

	this->status = this->notInit;
}

TecGraf::MshSurf::MainDrive::~MainDrive()
{
	this->clear();
}

void TecGraf::MshSurf::MainDrive::setAlgorithm(enum Algorithms algorithm)
{
	this->algorithm = algorithm;
}

void TecGraf::MshSurf::MainDrive::setAnalyticalMaxElementSize(Real analyticalMaxElementSize)
{
	this->analyticalMaxElementSize = analyticalMaxElementSize;
}

Real TecGraf::MshSurf::MainDrive::getAnalyticalMaxElementSize() const
{
	return this->analyticalMaxElementSize;
}

void TecGraf::MshSurf::MainDrive::setAnalyticalComputeCurvature(bool analyticalComputeCurvature)
{
	this->analyticalComputeCurvature = analyticalComputeCurvature;
}

bool TecGraf::MshSurf::MainDrive::isAnalyticalComputeCurvature() const
{
	return this->analyticalComputeCurvature;
}

void TecGraf::MshSurf::MainDrive::setSupportMaxElementSize(Real supportMaxElementSize)
{
	this->supportMaxElementSize = supportMaxElementSize;
}

Real TecGraf::MshSurf::MainDrive::getSupportMaxElementSize() const
{
	return this->supportMaxElementSize;
}

void TecGraf::MshSurf::MainDrive::setSupportMaxNumberElements(Int supportMaxNumberElements)
{
	this->supportMaxNumberElements = supportMaxNumberElements;
}

Int TecGraf::MshSurf::MainDrive::getSupportMaxNumberElements() const
{
	return this->supportMaxNumberElements;
}

void TecGraf::MshSurf::MainDrive::setSupportComputeCurvature(bool supportComputeCurvature)
{
	this->supportComputeCurvature = supportComputeCurvature;
}

bool TecGraf::MshSurf::MainDrive::isSupportComputeCurvature() const
{
	return this->supportComputeCurvature;
}

void TecGraf::MshSurf::MainDrive::setSupportInvertMesh(bool supportInvertMesh)
{
    this->supportInvertMesh = supportInvertMesh;
}

bool TecGraf::MshSurf::MainDrive::isSupportInvertMesh() const
{
    return this->supportInvertMesh;
}

void TecGraf::MshSurf::MainDrive::setSupportMesh(Data::Mesh *supportMesh)
{
	this->supportMesh = supportMesh;
}

Data::Mesh *TecGraf::MshSurf::MainDrive::getSupportMesh() const
{
	return this->supportMesh;
}

void TecGraf::MshSurf::MainDrive::setSuportLoadingSupportMesh(bool supportLoadingSupportMesh)
{
	this->supportLoadingSupportMesh = supportLoadingSupportMesh;
}

bool TecGraf::MshSurf::MainDrive::isSupportLoadingSupportMesh() const
{
	return this->supportLoadingSupportMesh;
}

void TecGraf::MshSurf::MainDrive::setMessageFunction(Surf3DMessFunc *messageFunction)
{
    this->messageFunction = messageFunction;
}

void TecGraf::MshSurf::MainDrive::setIdManager(Performer::IdManager *idManager)
{
    this->idManager = idManager;
}

void TecGraf::MshSurf::MainDrive::execute()
{
    //std::cout << "1" << std::endl;
    //std::cout << "" << std::endl;

	if ((this->status.getType() == success.getType()) ||
		(this->status.getType() >= error.getType()))
	{
		return;
	}

	this->status = this->init;

	if (((this->getBoundary()) && (this->getBoundary()->size() == 0)) ||
		((!this->getBoundary()) && ((!this->boundaryBuilder) ||
			(!this->boundaryBuilder->getBoundary()) ||
			(this->boundaryBuilder->getBoundary()->size() == 0))) ||
		(this->algorithm == UNKNOWN))
	{
		this->status = this->notInit;

		return;
	}

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	if (!this->executeBoundaryBuilder(true))
	{
		this->status = this->error;

#if USE_TIME_MEASUREMENTS
		this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

		return;
	}

	bool success = false;

	switch (this->algorithm)
	{
	case UNKNOWN:
		break;

	case EDGE:
		break;

	case CREATE_3D_BIL:
		break;

	case SURF_3D_ANALYTICAL:
		success = this->executeAnalytical();
		break;

	case SURF_3D_SUPPORT:
		success = this->executeSupport();
		break;

	case CREATE_2D:
		break;

	case CREATE_2D_EDGE:
		break;

	case EDGE_2D:
		break;

	case TEMPLATE:
		break;

	default:
		break;
	}

#if USE_TIME_MEASUREMENTS
	this->runtime += Data::time() - start;
#endif //#if USE_TIME_MEASUREMENTS

	if (success)
	{
		this->findQuality();

		this->status = this->success;
	}
	else
	{
		this->status = this->error;
	}
}

//#if USE_GUI
void TecGraf::MshSurf::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;

	if (this->boundaryBuilder)
	{
		this->boundaryBuilder->setBoundary(boundary);
	}
}

/*void TecGraf::MshSurf::MainDrive::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}*/

void TecGraf::MshSurf::MainDrive::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}
//#endif //#if USE_GUI

const Data::Boundary *TecGraf::MshSurf::MainDrive::getBoundary() const
{
#if USE_GUI
	if (this->boundary)
	{
		return this->boundary;
	}

	if (this->boundaryBuilder)
	{
		return this->boundaryBuilder->getBoundary();
	}

	return NULL;
#else
	return this->boundary;
#endif //#if USE_GUI
}

/*const Data::BoxTree *TecGraf::MshSurf::MainDrive::getTree() const
{
	return this->tree;
}*/

const Data::Mesh *TecGraf::MshSurf::MainDrive::getMesh() const
{
	return this->mesh;
}

#if USE_GUI
/*const Data::Front *TecGraf::MshSurf::MainDrive::getFront() const
{
	return this->front;
}*/
#endif //#if USE_GUI

void TecGraf::MshSurf::MainDrive::clear()
{
	this->status = this->notInit;

	if (this->idManager)
	{
		delete this->idManager;

		this->idManager = NULL;
	}

#if USE_GUI
	/*if (this->front)
	{
		delete this->front;

		this->front = NULL;
	}*/
#endif //#if USE_GUI

	/*if (this->tree)
	{
		delete this->tree;

		this->tree = NULL;
	}*/

	if (this->mesh)
	{
		if (this->boundary)
		{
			//Data::VertexVector vertices;
			Data::VertexList vertices;
			//Data::FrontElementVector elements;
			Data::FrontElementList elements;

			this->boundary->setVertices(vertices);
			this->boundary->setElements(elements);
		}

		delete this->mesh;

		this->mesh = NULL;
	}

	if (this->boundary)
	{
#if USE_GUI
		if ((this->boundaryBuilder) && (this->boundaryBuilder->getBoundary() == this->boundary))
		{
			this->boundaryBuilder->setBoundary(NULL);
		}
#endif //#if USE_GUI

		delete this->boundary;

		this->boundary = NULL;
	}

	if (this->supportMesh)
	{
		delete this->supportMesh;

		this->supportMesh = NULL;
	}

	this->quality.clear();

#if USE_TIME_MEASUREMENTS
	this->runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	if (this->boundaryBuilder)
	{
		if (this->boundaryBuilder->getBoundary())
		{
			delete this->boundaryBuilder->getBoundary();

#if USE_GUI
			this->boundaryBuilder->setBoundary(NULL);
#endif //#if USE_GUI
		}

		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

	this->analyticalMaxElementSize = 0.0;
	this->analyticalComputeCurvature = false;

	this->supportMaxElementSize = 0.0;
	this->supportMaxNumberElements = -1;
	this->supportComputeCurvature = false;
    this->supportInvertMesh = false;
    this->supportMesh = NULL;
    this->supportLoadingSupportMesh = false;

    this->messageFunction = NULL;
}

bool TecGraf::MshSurf::MainDrive::loadFile()
{
	if (this->inputFileName.empty())
	{
		return false;
	}

	size_t pos = this->inputFileName.find_last_of('.');

	if (pos == std::string::npos)
	{
		return false;
	}

	std::string extension = this->inputFileName.substr(pos);

	if (extension.size() == 0)
	{
		return false;
	}

	if (extension == ".gen")
	{
		return this->loadInputFile(this->inputFileName);
	}

	return false;
}

bool TecGraf::MshSurf::MainDrive::saveFile() const
{
	if (this->outputFileName.empty())
	{
		return false;
	}

	std::string extension = this->outputFileName.substr(this->outputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if (extension == ".gen")
	{
		return this->saveOutputFile(this->outputFileName);
	}

	return false;
}

std::string TecGraf::MshSurf::MainDrive::qualityString() const
{
	if (!this->mesh)
	{
		return "";
	}

	std::stringstream str;

	str << this->quality.qualityString();

#if USE_TIME_MEASUREMENTS
	str << std::endl;

	ULInt size = this->mesh ? this->mesh->size() : 0;

	if (size == 0)
    {
        size = 1;
    }

	str << "Runtime: " << this->runtime << " seconds" << std::endl;
	str << "Elements per second: " << static_cast<Real>(size)/static_cast<Real>(this->runtime);
#endif //#if USE_TIME_MEASUREMENTS

	return str.str();
}

void TecGraf::MshSurf::MainDrive::findQuality()
{
#if USE_GUI
    Real scale[3][10];

    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_0, scale[0][0], scale[1][0], scale[2][0]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_1, scale[0][1], scale[1][1], scale[2][1]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_2, scale[0][2], scale[1][2], scale[2][2]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_3, scale[0][3], scale[1][3], scale[2][3]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_4, scale[0][4], scale[1][4], scale[2][4]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_5, scale[0][5], scale[1][5], scale[2][5]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_6, scale[0][6], scale[1][6], scale[2][6]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_7, scale[0][7], scale[1][7], scale[2][7]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_8, scale[0][8], scale[1][8], scale[2][8]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_9, scale[0][9], scale[1][9], scale[2][9]);
#endif //#if USE_GUI

	//if (this->mesh)
	{
#if USE_GUI
		this->quality.findQuality(this->mesh, 10, &scale[0][0]);
#else
		this->quality.findQuality(this->mesh, 10);
#endif //#if USE_GUI
	}
}

bool TecGraf::MshSurf::MainDrive::loadInputFile(const std::string &inputFileName)
{
	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if (!this->supportLoadingSupportMesh)
	{
		this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());
	}

	if (extension == ".gen")
	{
		IO::GenFileReader reader;

		reader.setFilename(inputFileName);

		if (!this->supportLoadingSupportMesh)
		{
			reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));
		}

		reader.execute();

		UInt dimension = reader.getDimension();

		if (this->supportLoadingSupportMesh)
        {
            if (dimension != 3)
            {
                return false;
            }

            Data::Mesh *mesh = static_cast<Data::Mesh *>(reader.getMesh());

            if (!mesh)
            {
                return false;
            }

            if (!reader.isSurface())
            {
                delete mesh;

                return false;
            }

            if (this->supportMesh)
            {
                delete this->supportMesh;
            }

            this->supportMesh = mesh;

            return true;
        }

        if (dimension != 3)
        {
            this->boundaryBuilder->clear();

            return false;
        }

        if (reader.getMesh())
        {
            this->boundaryBuilder->clear();

            delete reader.getMesh();

            return false;
        }

		return true;
	}

	return false;
}

bool TecGraf::MshSurf::MainDrive::saveInputFile(const std::string &/*outputFileName*/) const
{
	return false;
}

bool TecGraf::MshSurf::MainDrive::loadOutputFile(const std::string &/*inputFileName*/)
{
	return false;
}

bool TecGraf::MshSurf::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
	std::string extension = outputFileName.substr(outputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

#if USE_GUI
	if ((!this->getMesh()) || (this->getMesh()->size() == 0))
#else
	if ((!this->mesh) || (this->mesh->size() == 0))
#endif //#if USE_GUI
	{
		return false;
	}

	if (extension == ".gen")
	{
	    Data::VertexList vertices = this->mesh->getVertices();
	    Data::ElementList elements = this->mesh->getElements();

        std::cout << "2.5" << std::endl ;
        std::cout << this->mesh->getVertices().size() << std::endl ;
        for (std::list<Data::Vertex*>::iterator it = vertices.begin(); it != vertices.end(); it++)
        {
            std::cout << (*it)->getId() << " "  << (*it)->getPoint()->getCoord(0) << " " << (*it)->getPoint()->getCoord(1) << " " << (*it)->getPoint()->getCoord(2) << std::endl ;
        }
        std::cout << 0 << std::endl ;
        std::cout << this->mesh->getElements().size() << std::endl ;
        for (std::list<Data::Element*>::iterator it=elements.begin(); it!=elements.end(); ++it)
        {
            std::cout << (*it)->getId() << " 3 "  <<  (*it)->getVertex(0)->getId() << " " << (*it)->getVertex(1)->getId() << " " << (*it)->getVertex(2)->getId() << std::endl;
        }


	    return false;


		/*IO::GenFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getMesh());
		writer.setDimension(3);
		writer.setSurface(true);

		writer.execute();

		return true;*/
	}

	return false;
}

/*bool TecGraf::MshSurf::MainDrive::convertToMsh2D(double *&pts, int &nPts, Data::VertexVector &map, bool clockWise) const
{
	nPts = static_cast<int>(this->boundary->verticesSize());

	pts = new double[2*nPts];

	map.resize(nPts);

	MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(*this->boundary->vBegin());

	UInt j = clockWise ? 1 : 0;

	for (int i = 0; i < nPts; i++)
	{
		pts[2*i+0] = v->getPoint()->getCoord(0);
		pts[2*i+1] = v->getPoint()->getCoord(1);
		map[i] = v;

		for (Data::FrontElementList::iterator iter = v->feBegin();
			 iter != v->feEnd(); iter++)
		{
			Data::FrontElement *fel = (*iter);

			if (fel->getVertex(j) == v)
			{
				v = static_cast<MJMesh::Vertex *>(fel->getVertex(1 - j));

				break;
			}
		}
	}

	return true;
}

bool TecGraf::MshSurf::MainDrive::convertToMsh2D(double *&pts, int &nLoops, int *&loops, Data::VertexVector &map, bool clockWise) const
{
	int nPts = static_cast<int>(this->boundary->verticesSize());

	pts = new double[2*nPts];

	Data::IntVector lps;

#if USE_C__11
	Data::VertexHash used;

	used.reserve(nPts);
#else
	Data::VertexSet used;
#endif //#if USE_C__11

	map.resize(nPts);

	Data::Boundary::VertexIterator firstIter = this->boundary->vBegin();
	MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(*firstIter);
	MJMesh::Vertex *first = NULL;
	int firstI = 0;

	UInt j = clockWise ? 1 : 0;

	for (int i = 0; i < nPts; i++)
	{
		if (!first)
		{
			first = v;
			firstI = i;
		}

		pts[2*i+0] = v->getPoint()->getCoord(0);
		pts[2*i+1] = v->getPoint()->getCoord(1);
		map[i] = v;

		used.insert(v);

		for (Data::FrontElementList::iterator iter = v->feBegin();
			 iter != v->feEnd(); iter++)
		{
			Data::FrontElement *fel = (*iter);

			if (fel->getVertex(j) == v)
			{
				v = static_cast<MJMesh::Vertex *>(fel->getVertex(1 - j));

				break;
			}
		}

		if (v == first)
		{
			lps.push_back(i - firstI + 1);

			if (i - firstI + 1 != nPts)
			{
				for (Data::Boundary::VertexIterator iter = firstIter;
					 iter != this->boundary->vEnd(); iter++)
				{
					if (used.find((*iter)) == used.end())
					{
						firstIter = iter;
						v = static_cast<MJMesh::Vertex *>((*firstIter));

						break;
					}
				}
			}

			first = NULL;
		}
	}

	nLoops = static_cast<int>(lps.size());

	loops = new int[nLoops];

	for (int i = 0; i < nLoops; i++)
	{
		loops[i] = lps[i];
	}

	return true;
}

bool TecGraf::MshSurf::MainDrive::convertToMsh2D(double *&pts, int &nPts, int *&edges, int &nEdges, Data::VertexVector &map, bool clockWise) const
{
	nPts = static_cast<int>(this->boundary->verticesSize());
	nEdges = static_cast<int>(this->boundary->size());

	pts = new double[2*nPts];
	edges = new int[2*nEdges];

#if USE_C__11
	Data::IntHashMap vMap;

	vMap.reserve(nPts);
#else
	Data::IntMap vMap;
#endif //#if USE_C__11

	map.resize(nPts);

	Data::Boundary::VertexIterator viter = this->boundary->vBegin();

	for (int i = 0; i < nPts; i++, viter++)
	{
		Data::Vertex *v = (*viter);

		pts[2*i+0] = v->getPoint()->getCoord(0);
		pts[2*i+1] = v->getPoint()->getCoord(1);
		map[i] = v;

		vMap[v->getId()] = i;
	}

	Data::Boundary::FrontElementIterator eiter = this->boundary->eBegin();

	UInt j = clockWise ? 1 : 0;

	for (int i = 0; i < nEdges; i++, eiter++)
	{
		Data::FrontElement *fel = (*eiter);

		Data::Vertex *v1 = fel->getVertex(j);
		Data::Vertex *v2 = fel->getVertex(1 - j);

		edges[2*i+0] = vMap[v1->getId()];
		edges[2*i+1] = vMap[v2->getId()];
	}

	return true;
}

bool TecGraf::MshSurf::MainDrive::convertToMesh(int nPts, double *pts, int nElems, int *elems, int type,
	Data::VertexVector &map, bool clockWise, bool rebuildMap, bool checkBoundary)
{
	if ((nPts == 0) || (nElems == 0) || (pts == NULL) || (elems == NULL))
	{
		return false;
	}

	if (!this->mesh)
	{
		this->mesh = new MJMesh::Mesh();
	}

#if USE_GUI
	this->mesh->freeze();
#endif //#if USE_GUI

	//Data::VertexMap map;
	if (nPts > static_cast<int>(map.size()))
	{
		map.resize(nPts);
	}

	UInt numBoundaryVerticesFound = 0;

	for (int i = 0; i < nPts; i++)
	{
		MJMesh::Vertex *v = NULL;

		double x = pts[2*i+0];
		double y = pts[2*i+1];

		bool inner = true;

		if (rebuildMap)
		{
			if (numBoundaryVerticesFound < this->boundary->verticesSize())
			{
				for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
					 iter != this->boundary->vEnd(); iter++)
				{
					//if ((*iter)->getPoint()->match(Data::Point2D(x, y)))
					if ((*iter)->getPoint()->equal(Data::Point2D(x, y)))
					{
						v = static_cast<MJMesh::Vertex *>((*iter));

						numBoundaryVerticesFound++;

						inner = false;

						break;
					}
				}
			}
		}
		else if (i < static_cast<int>(this->boundary->verticesSize()))
		{
			v = static_cast<MJMesh::Vertex *>(map[i]);

			//if (v->getPoint()->getCoord(0) != x)
			{
				v->getPoint()->setCoord(0, x);
			}

			//if (v->getPoint()->getCoord(1) != y)
			{
				v->getPoint()->setCoord(1, y);
			}

			inner = false;
		}

		if (!v)
		{
			v = new MJMesh::Vertex(x, y, this->idManager->next(0));

			if (checkBoundary)
			{
				for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
					 iter != this->boundary->eEnd(); iter++)
				{
					MJMesh::FrontElement2D *fel = static_cast<MJMesh::FrontElement2D *>((*iter));
					MJMesh::Edge2D *edge = static_cast<MJMesh::Edge2D *>(fel->getShape());

					if (edge->in(v->getPoint()))
					{
						inner = false;

						//MJMesh::Vertex *v1 = fel->getV1();
						MJMesh::Vertex *v2 = fel->getV2();

						fel->setVertex(1, v);

						v2->remove(fel);
						v->add(fel);

#if USE_ATTRIBS
						edge->recalcAttribs();
#endif //#if USE_ATTRIBS

						MJMesh::FrontElement *newFel = new MJMesh::FrontElement2D(v, v2, this->idManager->next(1));

						this->boundary->add(newFel);

						break;
					}
				}
			}
		}

		if (v)
		{
			map[i] = v;

			this->mesh->add(v, inner);
		}
	}

	int accum = 0;

	for (int i = 0; i < nElems; i++)
	{
		int ltype = type;

		if (ltype == 0)
		{
			ltype = elems[accum];
		}

		accum += ltype + 1;

		MJMesh::Vertex *v[ltype];

		if (ltype == 3)
		{
			if (clockWise)
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
			}
			else
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
			}
		}
#if USE_QUADS
		else if (ltype == 4)
		{
			if (clockWise)
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+4)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[3] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
			}
			else
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
				v[3] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+4)]]);
			}

			if (v[0] == v[3])
			{
				ltype = 3;
			}
		}
#endif //#if USE_QUADS

		MJMesh::Element2D *el = NULL;

		if (ltype == 3)
		{
			el = new MJMesh::Element2D(v[0], v[1], v[2], this->idManager->next(2));
		}
#if USE_QUADS
		else if (ltype == 4)
		{
			el = new MJMesh::Element2D(v[0], v[1], v[2], v[3], this->idManager->next(2));
		}
#endif //#if USE_QUADS

		if (el)
		{
			this->mesh->add(el);

			for (int i = 0; i < ltype; i++)
			{
				for (Data::FrontElementList::iterator iter = v[i]->feBegin();
					 iter != v[i]->feEnd(); iter++)
				{
					MJMesh::FrontElement2D *fel = static_cast<MJMesh::FrontElement2D *>((*iter));

					if (fel->have(v[(i+1)%ltype]))
					{
						fel->add(el);

						break;
					}
				}
			}
		}
	}

	for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		this->mesh->add((*iter));
	}

#if USE_GUI
	this->mesh->unfreeze();
#endif //#if USE_GUI

	return true;
}*/

bool TecGraf::MshSurf::MainDrive::convertToMeshSurf(int &nPts, double *&pts, int &nElems, int *&elems, const Data::Mesh *mesh, int type, bool clockWise) const
{
    nPts = static_cast<int>(mesh->verticesSize());
    nElems = static_cast<int>(mesh->size());

    pts = new double[3*nPts];
    elems = new int[(type+1)*nElems];

#if USE_C__11
	Data::IntHashMap vMap;

	vMap.reserve(nPts);
#else
	Data::IntMap vMap;
#endif //#if USE_C__11

    Data::Mesh::ConstVertexIterator viter = mesh->vBegin();

    std::streamsize prec = std::cout.precision();
    std::cout.precision(30);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);

    for (int i = 0; i < nPts; i++, viter++)
    {
        Data::Vertex *v = (*viter);

		pts[3*i+0] = v->getPoint()->getCoord(0);
		pts[3*i+1] = v->getPoint()->getCoord(1);
		pts[3*i+2] = v->getPoint()->getCoord(2);

		vMap[v->getId()] = i;

		//std::cout << "mapping support original vertex " << v->getId() << " " << pts[3*i+0] << " " << pts[3*i+1] << " " << pts[3*i+2] << " to " << i+1 << std::endl;
    }

    Data::Mesh::ConstElementIterator eiter = mesh->eBegin();

    for (int i = 0; i < nElems; i++, eiter++)
    {
        Data::Element *e = (*eiter);

        elems[(type+1)*i + 0] = type;

        //std::cout << "mapping support original element " << e->text() << " to elem " << i << " (";

		for (int j = 0; j < type; j++)
        {
            UInt jj = clockWise ? (type - j)%type : j;

            Data::Vertex *v = e->getVertex(jj);

            elems[(type+1)*i + j+1] = vMap[v->getId()];

            //std::cout << elems[(type+1)*i + j+1] << " ";
        }

        //std::cout << ")" << std::endl;
    }

    std::cout.precision(prec);

    return true;
}

bool TecGraf::MshSurf::MainDrive::convertToBoundarySurf(int &nPts, double *&pts, int &nBoundEdges, int &nInterEdges, int *&edges,
    Data::VertexVector &map, const Data::Boundary *boundary, bool clockWise) const
{
    nPts = static_cast<int>(boundary->verticesSize());
	nBoundEdges = static_cast<int>(boundary->size());
	nInterEdges = 0;

	pts = new double[3*nPts];
	edges = new int[2*nBoundEdges];

#if USE_C__11
	Data::IntHashMap vMap;

	vMap.reserve(nPts);
#else
	Data::IntMap vMap;
#endif //#if USE_C__11

	map.resize(nPts);

	Data::Boundary::ConstVertexIterator viter = boundary->vBegin();

	for (int i = 0; i < nPts; i++, viter++)
	{
		Data::Vertex *v = (*viter);

		pts[3*i+0] = v->getPoint()->getCoord(0);
		pts[3*i+1] = v->getPoint()->getCoord(1);
		pts[3*i+2] = v->getPoint()->getCoord(2);
		map[i] = v;

		vMap[v->getId()] = i;

		//std::cout << "mapping original vertex " << v->text() << " to " << i+1 << std::endl;
	}

	Data::Boundary::ConstFrontElementIterator eiter = boundary->eBegin();

	UInt j = clockWise ? 1 : 0;

	for (int i = 0; i < nBoundEdges; i++, eiter++)
	{
		Data::FrontElement *fel = (*eiter);

		Data::Vertex *v1 = fel->getVertex(j);
		Data::Vertex *v2 = fel->getVertex(1 - j);

		edges[2*i+0] = vMap[v1->getId()];
		edges[2*i+1] = vMap[v2->getId()];

		//std::cout << "mapping original edge " << fel->getId() << " (" << v1->getId() << ", " << v2->getId() << ") to (" << edges[2*i+0]+1 << ", " << edges[2*i+1]+1 << ")" << std::endl;
	}

	return true;
}

bool TecGraf::MshSurf::MainDrive::convertToMesh(int nPts, double *pts, int nElems, int *elems, int type,
	Data::VertexVector &map, bool clockWise, bool rebuildMap, bool checkBoundary)
{
	if ((nPts == 0) || (nElems == 0) || (pts == NULL) || (elems == NULL))
	{
		return false;
	}

	/*if ((type != 3)
#if USE_QUADS
		&& (type != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}*/

	if (!this->mesh)
	{
		this->mesh = new MJMesh::Mesh();
	}

#if USE_GUI
	this->mesh->freeze();
#endif //#if USE_GUI

	//Data::VertexMap map;
	if (nPts > static_cast<int>(map.size()))
	{
		map.resize(nPts);
	}

	UInt numBoundaryVerticesFound = 0;

	for (int i = 0; i < nPts; i++)
	{
		MJMesh::Vertex *v = NULL;

		double x = pts[3*i+0];
		double y = pts[3*i+1];
		double z = pts[3*i+2];

		bool inner = true;

		if (rebuildMap)
		{
			if (numBoundaryVerticesFound < this->boundary->verticesSize())
			{
				for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
					 iter != this->boundary->vEnd(); iter++)
				{
					//if ((*iter)->getPoint()->match(Data::Point2D(x, y)))
					if ((*iter)->getPoint()->equal(Data::Point3D(x, y, z)))
					{
						v = static_cast<MJMesh::Vertex *>((*iter));

						numBoundaryVerticesFound++;

						inner = false;

						break;
					}
				}
			}
		}
		else if (i < static_cast<int>(this->boundary->verticesSize()))
		{
			v = static_cast<MJMesh::Vertex *>(map[i]);

			//if (v->getPoint()->getCoord(0) != x)
			{
				v->getPoint()->setCoord(0, x);
			}

			//if (v->getPoint()->getCoord(1) != y)
			{
				v->getPoint()->setCoord(1, y);
			}

			//if (v->getPoint()->getCoord(2) != z)
			{
				v->getPoint()->setCoord(2, z);
			}

			inner = false;
		}

		if (!v)
		{
			v = new MJMesh::Vertex(x, y, z, this->idManager->next(0));

			if (checkBoundary)
			{
#if USE_GUI
                this->boundary->freeze();
#endif //#if USE_GUI

				for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
					 iter != this->boundary->eEnd(); iter++)
				{
					MJMesh::FrontElementSurface *fel = static_cast<MJMesh::FrontElementSurface *>((*iter));
					MJMesh::Edge3D *edge = static_cast<MJMesh::Edge3D *>(fel->getShape());

					if (edge->in(v->getPoint()))
					{
						inner = false;

						//MJMesh::Vertex *v1 = fel->getV1();
						MJMesh::Vertex *v2 = fel->getV2();

						fel->setVertex(1, v);

						v2->remove(fel);
						v->add(fel);

#if USE_ATTRIBS
						edge->recalcAttribs();
#endif //#if USE_ATTRIBS

						MJMesh::FrontElement *newFel = new MJMesh::FrontElementSurface(v, v2, this->idManager->next(1));

						this->boundary->add(newFel);

						break;
					}
				}

#if USE_GUI
                this->boundary->unfreeze();
#endif //#if USE_GUI
			}
		}

		if (v)
		{
			map[i] = v;

			//std::cout << "back mapping vertex " << v->text() << " from " << i+1 << std::endl;

			this->mesh->add(v, inner);
		}
	}

	int accum = 0;

	for (int i = 0; i < nElems; i++)
	{
		int ltype = type;

		if (ltype == 0)
		{
			ltype = elems[accum];
		}

		accum += ltype + 1;

		MJMesh::Vertex *v[ltype];

		if (ltype == 3)
		{
			if (clockWise)
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
			}
			else
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
			}
		}
#if USE_QUADS
		else if (ltype == 4)
		{
			if (clockWise)
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+4)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[3] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
			}
			else
			{
				v[0] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+1)]]);
				v[1] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+2)]]);
				v[2] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+3)]]);
				v[3] = static_cast<MJMesh::Vertex *>(map[elems[static_cast<UInt>((ltype+1)*i+4)]]);
			}

			if (v[0] == v[3])
			{
				ltype = 3;
			}
		}
#endif //#if USE_QUADS

		MJMesh::ElementSurface *el = NULL;

		if (ltype == 3)
		{
			el = new MJMesh::ElementSurface(v[0], v[1], v[2], this->idManager->next(2));
		}
#if USE_QUADS
		else if (ltype == 4)
		{
			el = new MJMesh::ElementSurface(v[0], v[1], v[2], v[3], this->idManager->next(2));
		}
#endif //#if USE_QUADS

		if (el)
		{
			this->mesh->add(el);

			for (int i = 0; i < ltype; i++)
			{
				for (Data::FrontElementList::iterator iter = v[i]->feBegin();
					 iter != v[i]->feEnd(); iter++)
				{
					MJMesh::FrontElementSurface *fel = static_cast<MJMesh::FrontElementSurface *>((*iter));

					if (fel->have(v[(i+1)%ltype]))
					{
						fel->add(el);

						break;
					}
				}
			}
		}
	}

	for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		this->mesh->add((*iter));
	}

#if USE_GUI
	this->mesh->unfreeze();
#endif //#if USE_GUI

	return true;
}

bool TecGraf::MshSurf::MainDrive::executeBoundaryBuilder(bool del)
{
    if ((!this->boundaryBuilder) && (this->boundary) && (this->idManager))
    {
        return true;
    }

	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setSorted(false);

	this->boundaryBuilder->execute();

	this->boundary = static_cast<MJMesh::Boundary *>(this->boundaryBuilder->getBoundary());

	this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

	if (this->boundary->dimension() != 3)
	{
		return false;
	}

	return true;
}

bool TecGraf::MshSurf::MainDrive::executeAnalytical()
{
	return false;
}

bool TecGraf::MshSurf::MainDrive::executeSupport()
{
    if ((!this->supportMesh) || (this->supportMaxElementSize <= 0.0))
    {
        return false;
    }

    /*///DESLOCAMENTO DOS VERTICES DA GRADE
    {
        UInt side = 0;

        Data::Triangle3D *t = static_cast<Data::Triangle3D *>((*this->supportMesh->eBegin())->getShape());

        Data::Vector3D n = t->normal();

        if ((n.getCoord(0) > n.getCoord(1)) && (n.getCoord(0) > n.getCoord(2)))
        {
            side = 0;
        }
        else if ((n.getCoord(1) > n.getCoord(0)) && (n.getCoord(1) > n.getCoord(2)))
        {
            side = 1;
        }
        else
        {
            side = 2;
        }

        Real new_pos = 0.0;
        Real pos_max = -DBL_MAX;
        Real pos_min = DBL_MAX;

        for (Data::Boundary::VertexIterator it = this->boundary->vBegin();
             it != this->boundary->vEnd(); ++it)
        {
            if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
            {
                new_pos += (*it)->getPoint()->getCoord(0);

                if( pos_max < (*it)->getPoint()->getCoord(0) )
                    pos_max = (*it)->getPoint()->getCoord(0);
                if( pos_min > (*it)->getPoint()->getCoord(0) )
                    pos_min = (*it)->getPoint()->getCoord(0);
            }
            if( side == 1 || side == 4 ) ///TOP E BASE
            {
                new_pos += (*it)->getPoint()->getCoord(1);

                if( pos_max < (*it)->getPoint()->getCoord(1) )
                    pos_max = (*it)->getPoint()->getCoord(1);
                if( pos_min > (*it)->getPoint()->getCoord(1) )
                    pos_min = (*it)->getPoint()->getCoord(1);
            }
            if( side == 2 || side == 5 ) ///FRONT E BACK
            {
                new_pos += (*it)->getPoint()->getCoord(2);

                if( pos_max < (*it)->getPoint()->getCoord(2) )
                    pos_max = (*it)->getPoint()->getCoord(2);
                if( pos_min > (*it)->getPoint()->getCoord(2) )
                    pos_min = (*it)->getPoint()->getCoord(2);
            }
        }

        Real med = new_pos / static_cast<Real>(this->boundary->verticesSize());
        Real eixo_med = (pos_max + pos_min) / 2.0;
        //Real med_med = (eixo_med + med) / 2.0;

        new_pos = med;
        //new_pos = eixo_med;
        //new_pos /= static_cast<Real>(this->boundary->verticesSize());
        //new_pos = (pos_max + pos_min) / 2.0;

        for (Data::Mesh::VertexIterator it = this->supportMesh->vBegin(); it != this->supportMesh->vEnd(); it++)
        {
            if( side == 0 || side == 3 ) ///DIREITA E ESQUERDA
                (*it)->getPoint()->setCoord(0, new_pos);
            if( side == 1 || side == 4 ) ///TOP E BASE
                (*it)->getPoint()->setCoord(1, new_pos);
            if( side == 2 || side == 5 ) ///FRONT E BACK
                (*it)->getPoint()->setCoord(2, new_pos);
        }
    }*/

    if (false)
    {
        ///Performing Laplacian Smoother on supportMesh with respect to the boundary

        PMJMesh2::APrioriSupportMeshLaplacianSmoother smoother(this->supportMesh, this->boundary, true, 0.5, 10, 0);

        smoother.execute();

        //return false;
    }

    int nSupPts = 0;
    double *supPts = NULL;
    int nSupElems = 0;
    int *supElems = NULL;

    //this->convertToMeshSurf(nSupPts, supPts, nSupElems, supElems, this->supportMesh, 3, false);
    this->convertToMeshSurf(nSupPts, supPts, nSupElems, supElems, this->supportMesh, 3, this->supportInvertMesh);


    int nPts = 0;
    double *pts = NULL;
    int nBoundEdges = 0;
    int nInterEdges = 0;
    int *edges = NULL;

    Data::VertexVector map;

    this->convertToBoundarySurf(nPts, pts, nBoundEdges, nInterEdges, edges, map, this->boundary, false);


    int nGenPts = 0;
    double *genPts = NULL;
    int nGenElems = 0;
    int *genElems = NULL;

    MshSurfSetSupportMesh(NULL, nSupPts, supPts, nSupElems, supElems);


    int status = MshSurf3D(nPts, pts, nBoundEdges, nInterEdges, edges,
        this->supportMaxElementSize, this->supportComputeCurvature ? 1 : 0, this->supportMaxNumberElements, this->messageFunction,
        &nGenPts, &genPts, &nGenElems, &genElems);



    this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 0, map, false, true);


    delete [] supPts;
    delete [] supElems;
    delete [] pts;
    delete [] edges;
    free(genPts);
    free(genElems);



	return (status > 0);
}

Performer::IdManager *TecGraf::MshSurf::MainDrive::makeIdManager(const Performer::IdManager *idManager) const
{
	Performer::IdManager *idm = new Performer::SequentialIdManager(1, 3);

	if (idManager)
	{
		idm->setId(0, idManager->current(0) + 1);
		idm->setId(1, idManager->current(1) + 1);
	}

	return idm;
}

MJMesh::BoundaryBuilder *TecGraf::MshSurf::MainDrive::makeBoundaryBuilder() const
{
	return new MJMesh::BoundaryBuilder(false);
}

#endif //#if USE_TECGRAF_MSH_SURF
