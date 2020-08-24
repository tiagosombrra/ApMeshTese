#include "PMTools/MeshGenerator.h"

#if USE_MTOOLS

#include "Data/Point3D.h"
#include "Performer/IdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "PMTools/MshRegion3D.h"
#include "PMTools/AdvFront3D.h"

#include "Data/OStream.h"
#include <mpi.h>

const Performer::Status PMTools::MeshGenerator::success =
	Performer::Status(PMTools::MeshGenerator::SUCCESS, "success");
const Performer::Status PMTools::MeshGenerator::notInit =
	Performer::Status(PMTools::MeshGenerator::NOT_INIT, "not initiated");
const Performer::Status PMTools::MeshGenerator::init =
	Performer::Status(PMTools::MeshGenerator::INIT, "initiated");
const Performer::Status PMTools::MeshGenerator::error =
	Performer::Status(PMTools::MeshGenerator::ERROR, "error");

PMTools::MeshGenerator::MeshGenerator() :
	Performer::MeshGenerator()
{
	this->status = this->notInit;

	this->useBoxTest = false;
	this->workerTask = false;
	this->enableBackTracking = true;

	this->rangeProportion = 1.0;

	this->boundary = NULL;
	this->front = NULL;

	this->ftree = NULL;
	this->mshRegion = NULL;

	this->deleteFels = true;

	this->fromMesh = false;
	this->maxIdFrontVertex = 0;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeOtherStuff = 0.0;
	this->timeMeshGeneration = 0.0;
	this->timeMeshImprovement = 0.0;
	this->timeToMToolsConversion = 0.0;
	this->timeFromMToolsConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PMTools::MeshGenerator::~MeshGenerator()
{
	if (this->mshRegion)
	{
		delete this->mshRegion;

		//this->mshRegion = NULL;
	}
}

void PMTools::MeshGenerator::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void PMTools::MeshGenerator::setUseBoxTest(bool useBoxTest)
{
	this->useBoxTest = useBoxTest;
}

void PMTools::MeshGenerator::setWorkerTask(bool workerTask)
{
	this->workerTask = workerTask;
}

void PMTools::MeshGenerator::setEnableBackTracking(bool enableBackTracking)
{
	this->enableBackTracking = enableBackTracking;
}

void PMTools::MeshGenerator::setBox(const Data::Box &box)
{
	this->box = box;
}

void PMTools::MeshGenerator::setTreeBox(const Data::Box &treeBox)
{
	this->treeBox = treeBox;
}

void PMTools::MeshGenerator::setRangeProportion(double rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

void PMTools::MeshGenerator::setFtree(const FTools::OctTree *ftree)
{
	this->ftree = ftree;
}

void PMTools::MeshGenerator::setFront(MJMesh::Front *front)
{
	this->front = front;
}

void PMTools::MeshGenerator::setBoundary(const MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void PMTools::MeshGenerator::setDeleteFels(bool deleteFels)
{
    this->deleteFels = deleteFels;
}

PMTools::MshRegion3D *PMTools::MeshGenerator::makeMshRegion() const
{
    return new PMTools::MshRegion3D();
}

void PMTools::MeshGenerator::execute()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeOtherStuff = 0.0;
	this->timeMeshGeneration = 0.0;
	this->timeMeshImprovement = 0.0;
	this->timeToMToolsConversion = 0.0;
	this->timeFromMToolsConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->status = this->init;

	if ((this->useBoxTest) && (!this->workerTask))
	{
		this->workerTask = true;
	}

	///TODO
	/// Usar o mesmo mshRegion da geracao da octree, que jah tah com a boundary
	///   setada (quando o modo de update da malha nao for o FINAL)
	/// Usar o mesmo mshRegion da geracao sequencial dos elementos, que jah
	///   tah com a malha setada (quando o update da malha nao for FINAL)

	this->mshRegion = this->makeMshRegion();

	this->mshRegion->SetFtree(this->ftree);

	this->mshRegion->SetUseBoxTest(this->useBoxTest);
	this->mshRegion->SetWorkerTask(this->workerTask);
	this->mshRegion->SetEnableBackTracking(this->enableBackTracking);

	if (this->useBoxTest)
	{
	    this->mshRegion->SetBoundingBox(
			FTools::Vec3D(this->box.getMin()->getCoord(0), this->box.getMin()->getCoord(1), this->box.getMin()->getCoord(2)),
			FTools::Vec3D(this->box.getMax()->getCoord(0), this->box.getMax()->getCoord(1), this->box.getMax()->getCoord(2)));
		this->mshRegion->SetTreeBox(
			FTools::Vec3D(this->treeBox.getMin()->getCoord(0), this->treeBox.getMin()->getCoord(1), this->treeBox.getMin()->getCoord(2)),
			FTools::Vec3D(this->treeBox.getMax()->getCoord(0), this->treeBox.getMax()->getCoord(1), this->treeBox.getMax()->getCoord(2)));
	}

	this->mshRegion->SetRangeProportion(this->rangeProportion);

	Msh3D::MshRegion3D::MeshParams params;

	this->mshRegion->GetMeshParams(params);

	//params.OctreeRefineFactor = this->treeFactor;
	params.MaxGeneratedElements = INT_MAX;
	//params.MaxVolumeRestarts = 0 ;

	this->mshRegion->SetMeshParams(params);

	try
	{
		//debug
		//Data::cout() << "boundary->debug before convertFrontToMTools, " << this->boundary << Data::endl;
		//this->boundary->debug();
		//endebug

		this->convertFrontToMTools();
	}
	catch (std::exception &e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());

		this->mshRegion->SetFtree(NULL);

		delete this->mshRegion;

		this->mshRegion = NULL;

		this->status = this->error;

		this->status.append(e.what());

		return;
	}

	//debug
	//Data::cout() << "boundary->debug before executeFrontAdvancing, " << this->boundary << Data::endl;
	//this->boundary->debug();
	//endebug

	if (!this->executeFrontAdvancing())
	{
	    return;
	}

	//debug
	//Data::cout() << "boundary->debug before executeMeshBuilding, " << this->boundary << Data::endl;
	//this->boundary->debug();
	//endebug

	this->executeMeshBuilding();

	//debug
	//Data::cout() << "boundary->debug after executeMeshBuilding, " << this->boundary << Data::endl;
	//this->boundary->debug();
	//endebug

	this->mshRegion->SetFtree(NULL);

	delete this->mshRegion;

	this->mshRegion = NULL;

	if ((!this->workerTask) && (this->front->size() > 0) && ((!this->fromMesh) || (this->useBoxTest)))
	{
		this->status = this->error;

		this->status.append("there are unmeshed regions");
	}
	else
	{
		this->status = this->success;
	}

	//debug
	//Data::cout() << "boundary->debug before ending PMTools::MeshGenerator::execute(), " << this->boundary << Data::endl;
	//this->boundary->debug();
	//endebug
}

////debug
//#include "Parallel/Definitions.h"
//#include <mpi.h>
////endebug

namespace Msh3D
{
	extern Vec3D &GetCoord(int node_id,void *cdata);
}

void PMTools::MeshGenerator::convertFrontToMTools()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	////debug
	//int debugrank = MPI::COMM_WORLD.Get_rank();
	////endebug

	//this->vMap.clear();

#if USE_C__11
	Data::IntHashMap mapVertices;
#else
	Data::IntMap mapVertices;
#endif //#if USE_C__11

	///Converting vertices
	this->fromMesh = ((this->mesh) && (this->mesh->size() != 0));
	//this->fromMesh = false;

	/*if (this->fromMesh)
    {
        Data::cout() << "debug" << Data::endl;
    }*/

    if ((this->fromMesh) && (!this->workerTask) && (!this->useBoxTest))
    {
        for (Data::Front::VertexIterator iter = this->front->vBegin(), end = this->front->vEnd();
             iter != end; iter = this->front->erase(iter))
        {
            Data::Vertex *v = (*iter);

            if (this->boundary->have(v, false))
            {
                this->verticesNotAddedToBoundary.push_back(v);

                continue;
            }

            this->verticesAddedToBoundary.push_back(v);

            const_cast<MJMesh::Boundary *>(this->boundary)->add(v);
        }

        for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin(), end = this->front->allEEnd();
             iter != end; iter = this->front->erase(iter))
        {
            Data::FrontElement *fel = (*iter);

            if (this->boundary->have(fel))
            {
                this->felsNotAddedToBoundary[iter.getState()].push_back(fel);

                continue;
            }

            this->felsAddedToBoundary[iter.getState()].push_back(fel);

            const_cast<MJMesh::Boundary *>(this->boundary)->add(fel);
        }
    }

#if USE_C__11
	mapVertices.reserve(this->fromMesh ? this->mesh->verticesSize() : this->front->verticesSize());
#endif //#if USE_C__11

	this->maxIdVertex = this->maxIdFrontVertex = 0;

	int i = 1, ib = 1;

	///Converting front vertices
#if USE_SORT_BY_ID_IN_CONVERSION
	//Data::VertexList vertices = this->fromMesh ? this->boundary->getVertices() : this->front->getVertices();
	Data::VertexList vertices = this->front->getVertices();

	//if (debugrank == 0) Data::cout() << "this->vMap initial size = " << this->vMap.size() << ", size = " << size << Data::endl;

	vertices.sort(Data::Identifiable::less);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

	if (this->fromMesh)
	{
		i = static_cast<int>(this->boundary->verticesSize()) + 1;

		//this->vMap.resize(this->mesh->verticesSize(), NULL);
		this->vMap.assign(this->mesh->verticesSize(), NULL);
	}
	else
	{
#if USE_SORT_BY_ID_IN_CONVERSION
		//this->vMap.resize(vertices.size(), NULL);
		this->vMap.assign(vertices.size(), NULL);
#else
		//this->vMap.resize(this->front->verticesSize(), NULL);
		this->vMap.assign(this->front->verticesSize(), NULL);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION
	}

	this->maxIdElement = 0;
#if USE_C__11
	this->eMap.clear();
#endif //#if USE_C__11

#if USE_SORT_BY_ID_IN_CONVERSION
	while (!vertices.empty())
#else
	for (Data::Front::ConstVertexIterator iter = this->front->vBegin(), end = this->front->vEnd();
		 iter != end; iter++)
#endif //
	{
#if USE_SORT_BY_ID_IN_CONVERSION
		Data::Vertex *v = vertices.front();

		vertices.pop_front();
#else
		Data::Vertex *v = (*iter);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

		FTools::Vec3D vec(v->getPoint()->getCoord(0), v->getPoint()->getCoord(1), v->getPoint()->getCoord(2));

		bool inboundary = ((this->fromMesh) && (this->boundary->have(v)));

		int id = inboundary ?
			(*mapVertices.insert(std::make_pair(v->getId(), ib)).first).second :
			(*mapVertices.insert(std::make_pair(v->getId(), i)).first).second;

		this->mshRegion->AddNode(id, vec);

        //Data::cout() << "front vertex at " << v << " mapped to vertex " << id << Data::endl;
        //Data::cout() << "front vertex " << v->text(this->front) << " mapped to vertex " << id << Data::endl;
		/*Data::cout() << "front vertex " << v->getId() << " " <<
			v->getPoint()->getCoord(0) << " " << v->getPoint()->getCoord(1) << " " << v->getPoint()->getCoord(2)
			<< " mapped to vertex " << id << Data::endl;*/

		if (inboundary)
		{
			if (id == ib)
			{
				ib++;
			}
		}
		else
		{
			if (id == i)
			{
				i++;
			}
		}

		////debug
		//if (debugrank == 0) Data::cout() << "adding to this->vMap = " << v->text() << " in position " << i << Data::endl;
		////endebug

/*#if USE_GUI
		//Data::cout() << "front or boundary vertex " << v->text() << " mapped to vertex " << i << Data::endl;
		Data::cout() << "front vertex " << v->text() << " mapped to vertex " << id << Data::endl;
#endif //#if USE_GUI*/

		this->vMap[id - 1] = v;

		if (this->fromMesh)
		{
			if (v->getId() > this->maxIdVertex)
			{
				this->maxIdVertex = v->getId();
			}
		}
		else
		{
			//this->mesh->add(v);
			this->mesh->add(v, (!this->boundary) || (!this->boundary->have(v)));

			if (v->getId() > this->maxIdFrontVertex)
			{
				this->maxIdFrontVertex = v->getId();
			}
		}
	}

	///Converting front front elements
#if USE_SORT_BY_ID_IN_CONVERSION
	Data::FrontElementList fels = this->front->getAllElements();

	fels.sort(Data::Identifiable::less);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

#if USE_SORT_BY_ID_IN_CONVERSION
	while (!fels.empty())
#else
	for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin(), end = this->front->allEEnd();
		 iter != end; iter++)
#endif //#if USE_SORT_BY_ID_IN_CONVERSION
	{
#if USE_SORT_BY_ID_IN_CONVERSION
		Data::FrontElement *fel = fels.front();

		fels.pop_front();
#else
		Data::FrontElement *fel = (*iter);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

        /*if (fel->getId() == 47735)
        {
            std::cout << "debug " << fel->text() << std::endl;
        }*/

		FTools::List<int> nodes;

		if (fel->numVertices() == 3)
		{
			nodes.Append(mapVertices[fel->getVertex(0)->getId()]);
			nodes.Append(mapVertices[fel->getVertex(2)->getId()]);
			nodes.Append(mapVertices[fel->getVertex(1)->getId()]);
		}

		this->mshRegion->AddFace(nodes);

		//Data::cout() << "front fel at " << fel << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;
        //Data::cout() << "front fel " << fel->text(this->front) << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;
		/*Data::cout() << "front fel " << fel->getId()
			<< " " << fel->getVertex(0)->getId() << " " << fel->getVertex(1)->getId() << " " << fel->getVertex(2)->getId()
			<< " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;*/

		if (!this->fromMesh)
		{
			this->mesh->add(fel);
		}

/*#if USE_GUI
		Data::cout() << "front fel " << fel->text() << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;
		//Data::cout() << "face " << fel->getId() << " mapped to " << i << Data::endl;
#endif //#if USE_GUI*/
	}

	if (this->fromMesh)
	{
		//Msh3D::AdvFront does not need to search for cracks
		this->mshRegion->SetNoCracks(true);

		AdvFront3D *advFront = static_cast<AdvFront3D *>(this->mshRegion->BuildAdvFront());

		this->mshRegion->SetNoCracks(false);

#if DEBUG_DISABLE_IMPROVEMENT
		static_cast<MTools::AdvFront3D *>(advFront)->SetDoEdgeSwapping(PMTools::isMasterImprovementEnabled());
		static_cast<MTools::AdvFront3D *>(advFront)->SetDoFaceSwapping(PMTools::isMasterImprovementEnabled());
		advFront->SetDoSmoothing(PMTools::isMasterImprovementEnabled());
#endif //#if DEBUG_DISABLE_IMPROVEMENT

		this->mshRegion->SetAdvFront(advFront);

		int maxBoundaryNodeId = static_cast<int>(this->boundary->verticesSize());

		//internal nodes were set as BOUNDARY and M_FIXED in Initialize()
		PMTools::AdvFront3D::IntNodeHashIter niter = advFront->GetIntNodeTable()->Iterator();

    	for (niter.First(); niter.More(); niter.Next())
    	{
    		if (niter.Key() > maxBoundaryNodeId)
    		{
				niter.Entry().type = PMTools::AdvFront3D::INTERIOR;
				niter.Entry().motion = PMTools::AdvFront3D::M_FLOATING;

				//Data::cout() << "debug interior node: " << niter.Entry().id << ", " << niter.Entry().coord[0] << " " << niter.Entry().coord[1] << " " << niter.Entry().coord[2] << Data::endl;
    		}
    		else
    		{
    			//Data::cout() << "debug boundary node: " << niter.Entry().id << ", " << niter.Entry().coord[0] << " " << niter.Entry().coord[1] << " " << niter.Entry().coord[2] << Data::endl;
    		}
    	}

#if USE_SORT_BY_ID_IN_CONVERSION
    	vertices = this->boundary->getVertices();

		vertices.sort(Data::Identifiable::less);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

		List<Vec3D> cds;
		List<int> ids;

#if USE_SORT_BY_ID_IN_CONVERSION
		while (!vertices.empty())
#else
		for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
			 iter != end; iter++)
#endif //#if USE_SORT_BY_ID_IN_CONVERSION
		{
#if USE_SORT_BY_ID_IN_CONVERSION
			Data::Vertex *v = vertices.front();

			vertices.pop_front();
#else
			Data::Vertex *v = (*iter);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

			FTools::Vec3D vec(v->getPoint()->getCoord(0), v->getPoint()->getCoord(1), v->getPoint()->getCoord(2));

			/*if (!v->getPoint())
            {
                Data::cout() << "debug !point" << Data::endl;
            }*/

			int id = (*mapVertices.insert(std::make_pair(v->getId(), ib)).first).second;

			ids.Append(id);
			cds.Append(vec);

			if (id == ib)
            {
                this->vMap[id - 1] = v;

                if (v->getId() > this->maxIdVertex)
                {
                    this->maxIdVertex = v->getId();
                }
            }

			if (v->getId() > this->maxIdFrontVertex)
			{
				this->maxIdFrontVertex = v->getId();
			}

			if (this->front->have(v))
			{
				//Data::cout() << "boundary and front vertex at " << v << " previously mapped to vertex " << id << Data::endl;
                //Data::cout() << "boundary and front vertex " << v->text(this->boundary) << " previously mapped to vertex " << id << Data::endl;
				/*Data::cout()  << "boundary and front vertex " << v->getId() << " " <<
					v->getPoint()->getCoord(0) << " " << v->getPoint()->getCoord(1) << " " << v->getPoint()->getCoord(2)
					<< " mapped to vertex " << id << Data::endl;*/

				continue;
			}

			//Data::cout() << "boundary vertex at " << v << " mapped to vertex " << id << Data::endl;
			//Data::cout() << "boundary vertex " << v->text(this->boundary) << " mapped to vertex " << id << Data::endl;
			/*Data::cout() << "boundary vertex " << v->getId() << " " <<
				v->getPoint()->getCoord(0) << " " << v->getPoint()->getCoord(1) << " " << v->getPoint()->getCoord(2)
				<< " mapped to vertex " << id << Data::endl;*/

			Msh3D::AdvFront3D::Node nnode(id, vec, false);
			nnode.type = PMTools::AdvFront3D::BOUNDARY;
			nnode.motion = PMTools::AdvFront3D::M_FIXED;
			advFront->GetIntNodeTable()->Store(id, nnode);

			////debug
			//if (debugrank == 0) Data::cout() << "adding to this->vMap = " << v->text() << " in position " << i << Data::endl;
			////endebug

			if (id == ib)
			{
				ib++;
			}

/*#if USE_GUI
			Data::cout() << "boundary vertex " << v->text() << " mapped to vertex " << i << Data::endl;
#endif //#if USE_GUI*/
		}

		FTools::PointTree *PtTree = new FTools::PointTree(ids.Len(), cds.Data(), ids.Data()) ;

#if USE_SORT_BY_ID_IN_CONVERSION
    	vertices = this->mesh->getInnerVertices();

		vertices.sort(Data::Identifiable::less);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

#if USE_SORT_BY_ID_IN_CONVERSION
		while (!vertices.empty())
#else
#if USE_EXPERIMENTAL_MESH
		for (Data::Mesh::InnerVertexIterator iter = this->mesh->ivBegin(), end = this->mesh->ivEnd();
#else
		for (Data::Mesh::ConstVertexIterator iter = this->mesh->ivBegin(), end = this->mesh->ivEnd();
#endif //#if USE_EXPERIMENTAL_MESH
			 iter != end; iter++)
#endif //#if USE_SORT_BY_ID_IN_CONVERSION
		{
#if USE_SORT_BY_ID_IN_CONVERSION
			Data::Vertex *v = vertices.front();

			vertices.pop_front();
#else
			Data::Vertex *v = (*iter);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

			if (this->front->have(v))
			{
				//Data::cout() << "inner and front vertex at " << v << " previously mapped to vertex " << (*mapVertices.find(v->getId())).second << Data::endl;
                //Data::cout() << "inner and front vertex " << v->text(this->mesh) << " previously mapped to vertex " << (*mapVertices.find(v->getId())).second << Data::endl;
				/*Data::cout() << "inner and front vertex " << v->getId() << " " <<
					v->getPoint()->getCoord(0) << " " << v->getPoint()->getCoord(1) << " " << v->getPoint()->getCoord(2)
					<< " mapped to vertex " << (*mapVertices.insert(std::make_pair(v->getId(), i)).first).second << Data::endl;*/

				continue;
			}

			FTools::Vec3D vec(v->getPoint()->getCoord(0), v->getPoint()->getCoord(1), v->getPoint()->getCoord(2));

			int id = (*mapVertices.insert(std::make_pair(v->getId(), i)).first).second;

			//Data::cout() << "inner vertex at " << v << " mapped to vertex " << id << Data::endl;
            //Data::cout() << "inner vertex " << v->text(this->mesh) << " mapped to vertex " << id << Data::endl;
			/*Data::cout() << "inner vertex " << v->getId() << " " <<
				v->getPoint()->getCoord(0) << " " << v->getPoint()->getCoord(1) << " " << v->getPoint()->getCoord(2)
				<< " mapped to vertex " << id << Data::endl;*/

			Msh3D::AdvFront3D::Node nnode(id, vec, true);
			nnode.type = PMTools::AdvFront3D::INTERIOR;
			nnode.motion = PMTools::AdvFront3D::M_FLOATING;
			advFront->GetIntNodeTable()->Store(id, nnode);

			this->vMap[id - 1] = v;

			if (v->getId() > this->maxIdVertex)
			{
				this->maxIdVertex = v->getId();
			}

			if (id == i)
			{
				////debug
				//if (debugrank == 0) Data::cout() << "adding mesh vertex to inner vertices = " << v->text() << Data::endl;
				//Data::cout() << "adding mesh vertex to inner vertices = " << v->text() << Data::endl;
				////endebug

				i++;
			}

/*#if USE_GUI
			Data::cout() << "inner vertex " << v->text() << " mapped to vertex " << id << Data::endl;
#endif //#if USE_GUI*/
		}

		advFront->SetNextNodeId(i);

		AdvFront3D::IntFaceHash::DictIterator fiter = advFront->GetIntFaceTable()->Iterator() ;

    	for (fiter.First(); fiter.More(); fiter.Next())
    	{
    		if ((fiter.Key().id_0 > maxBoundaryNodeId) ||
				(fiter.Key().id_1 > maxBoundaryNodeId) ||
				(fiter.Key().id_2 > maxBoundaryNodeId))
			{
				fiter.Entry().boundary = false;

				//Data::cout() << "debug interior face (1): " << fiter.Entry().node_id[0] << ", " << fiter.Entry().node_id[1] << ", " << fiter.Entry().node_id[2] << Data::endl;

				continue;
			}

    		Data::Vertex *vs[3];
    		vs[0] = this->vMap[fiter.Key().id_0 - 1];
    		vs[1] = this->vMap[fiter.Key().id_1 - 1];
    		vs[2] = this->vMap[fiter.Key().id_2 - 1];

    		Data::FrontElementList adjs = this->boundary->adjacency(vs[0]);

    		bool found = false;

    		for (Data::FrontElementList::iterator iter = adjs.begin();
				 iter != adjs.end(); iter++)
			{
				if (((*iter)->have(vs[1])) && ((*iter)->have(vs[2])))
				{
					found = true;

					break;
				}
			}

    		if (!found)
    		{
    			fiter.Entry().boundary = false;

    			//Data::cout() << "debug interior face (2): " << fiter.Entry().node_id[0] << ", " << fiter.Entry().node_id[1] << ", " << fiter.Entry().node_id[2] << Data::endl;
    		}
    		else
    		{
    			//Data::cout() << "debug boundary face: " << fiter.Entry().node_id[0] << ", " << fiter.Entry().node_id[1] << ", " << fiter.Entry().node_id[2] << Data::endl;
    		}
    	}

    	//clear the data structures
    	delete advFront->GetTet_Topo();

    	Msh3D::TetTopo *tetTopo = new Msh3D::TetTopo();

    	advFront->SetTet_Topo(tetTopo);

    	tetTopo->SetGetCoordCallback(Msh3D::GetCoord, advFront);

    	Dict<int,int> checked;
		Msh3D::ReverseConn<Msh3D::AdvFront3D::FFace *> revcon;

		std::list<Msh3D::AdvFront3D::FFace *> newFaces;

#if USE_SORT_BY_ID_IN_CONVERSION
		fels = this->boundary->getElements();

		fels.sort(Data::Identifiable::less);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

#if USE_SORT_BY_ID_IN_CONVERSION
		while (!fels.empty())
#else
		for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
			 iter != end; iter++)
#endif //#if USE_SORT_BY_ID_IN_CONVERSION
		{
#if USE_SORT_BY_ID_IN_CONVERSION
			Data::FrontElement *fel = fels.front();

			fels.pop_front();
#else
			Data::FrontElement *fel = (*iter);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

			int nodes[3];

			nodes[0] = mapVertices[fel->getVertex(0)->getId()];
			nodes[1] = mapVertices[fel->getVertex(2)->getId()];
			nodes[2] = mapVertices[fel->getVertex(1)->getId()];

			//Data::cout() << "bef boundary fel at " << fel << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;
            //Data::cout() << "bef boundary fel " << fel->text(this->boundary) << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;
			/*Data::cout() << "bef boundary fel " << fel->getId()
				<< " " << fel->getVertex(0)->getId() << " " << fel->getVertex(1)->getId() << " " << fel->getVertex(2)->getId()
				<< " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;*/

			tetTopo->AddOuterFace(nodes);

			//Data::cout() << "aft boundary fel at " << fel << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;
            //Data::cout() << "aft boundary fel " << fel->text() << " mapped to face: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << Data::endl;

			int node_ids[3];
            Msh3D::AdvFront3D::Node *fnodes[3];

            for (int i = 0; i < 3; i++)
			{
                // we need to flip the node order for boundary faces

                int j = i;
                if (j == 1) j = 2;
                else if (j == 2) j = 1;

                node_ids[i] = nodes[j];
                fnodes[i] = advFront->GetIntNodeTable()->Get(node_ids[i]);
            }

            Msh3D::AdvFront3D::FFace *face = new Msh3D::AdvFront3D::FFace(node_ids, fnodes, 0);

            newFaces.push_back(face);

			double tol = sqrt(2.0*face->area)*0.01;

			List<int> pts;

			for (int i = 0; i < 3; i++)
			{
				revcon.AddReference(face->node_id[i], face);

				// check to see if we have visited this node yet.
				// if not, then loop through all nodes in the model
				// and see if they are located within the tolerance
				// distance.

				if (checked.Get(face->node_id[i]) == 0)
				{
					//PMTools::AdvFront3D::IntNodeHashIter iter = advFront->GetIntNodeTable()->Iterator();
					if (pts.Len() == 0)
					{
						PtTree->RangeQuery(face->min - tol, face->max + tol, pts) ;
					}

					//for (iter.First(); iter.More(); ++iter)
					for (int j = 0; j < pts.Len(); ++j)
					{
						int id = pts[j];
						//if (iter.Key() > maxBoundaryNodeId) continue; //internal node
						//if (face->node_id[i] == iter.Entry().id) continue;
						if (face->node_id[i] == id) continue;

						Msh3D::AdvFront3D::Node *p = advFront->GetIntNodeTable()->Get(id);

						//if ((face->coords[i] - iter.Entry().coord).Magnitude() < tol)
						if ((face->coords[i] - p->coord).Magnitude() < tol)
						{
							// then nodes are within tolerance so
							// insert each into each nodes list of mates

							FTools::SmallSet<int,2> *setptr = advFront->GetMates()->Get(face->node_id[i]);

							if (setptr == 0)
							{
								FTools::SmallSet<int,2> set;
								//set.Insert(iter.Entry().id);
								set.Insert(id);
								advFront->GetMates()->Store(face->node_id[i],set);
							}
							//else if (!setptr->HasElement(iter.Entry().id))
							else if (!setptr->HasElement(id))
							{
								//setptr->Insert(iter.Entry().id);
								setptr->Insert(id);
							}

							//setptr = advFront->GetMates()->Get(iter.Entry().id);
							setptr = advFront->GetMates()->Get(id);

							if (setptr == 0)
							{
								FTools::SmallSet<int,2> set ;
								set.Insert(face->node_id[i]) ;
								//advFront->GetMates()->Store(iter.Entry().id,set) ;
								advFront->GetMates()->Store(id,set) ;
							}
							else if (!setptr->HasElement(face->node_id[i]))
							{
								setptr->Insert(face->node_id[i]) ;
							}
						}
					}

					checked.Store(face->node_id[i],1) ;
				}
			}

			//Data::cout() << "debug adding boundary face to tettopo: " << bfiter.Entry().nodes[0] << ", " << bfiter.Entry().nodes[1] << ", " << bfiter.Entry().nodes[2] << Data::endl;
		}

		delete PtTree;

		//Data::cout() << "Time for adding outer faces = " << Data::time() - debugstart << Data::endl;
		//debugstart = Data::time();

		// Now for the nodes that have mates we need to determine
		// an average normal at this point to be used to select the
		// proper noded during meshing

		PMTools::AdvFront3D::MateHashIter miter(advFront->GetMates());
		for (miter.First(); miter.More(); ++miter)
		{
			Vec3D sum;
			int num = 0;
			Msh3D::ReverseConn<Msh3D::AdvFront3D::FFace *>::ReverseConnIter riter = revcon.GetNodeReferences(miter.Key());

			for (riter.First(); riter.More(); ++riter)
			{
				Msh3D::AdvFront3D::FFace *face = *riter;
				sum += face->normal;
				++num;
			}

			Msh3D::AdvFront3D::Node *node = advFront->GetIntNodeTable()->Get(miter.Key());
			node->normal = (sum / double(num)).Normalize();
		}

		while (!newFaces.empty())
		{
			delete newFaces.front();

			newFaces.pop_front();
		}

#if USE_SORT_BY_ID_IN_CONVERSION
		Data::ElementList els = this->mesh->getElements();

		els.sort(Data::Identifiable::less);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

		int StartId = this->mshRegion->GetStartElemId();

		PMTools::AdvFront3D::TetHash *tetTable = advFront->GetTetTable();

		if (!tetTable)
		{
			tetTable = new PMTools::AdvFront3D::TetHash(false);

			advFront->SetTetTable(tetTable);
		}

#if USE_C__11
		this->eMap.reserve(this->mesh->size());
#else
		//this->eMap.resize(StartId + this->mesh->size(), NULL);
		this->eMap.assign(StartId + this->mesh->size(), NULL);
#endif //#if USE_C__11

#if USE_SORT_BY_ID_IN_CONVERSION
		while (!els.empty())
#else
		for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin(), end = this->mesh->eEnd();
			 iter != end; iter++)
#endif //#if USE_SORT_BY_ID_IN_CONVERSION
		{
#if USE_SORT_BY_ID_IN_CONVERSION
			Data::Element *e = els.front();

			els.pop_front();
#else
			Data::Element *e = (*iter);
#endif //#if USE_SORT_BY_ID_IN_CONVERSION

			this->eMap[StartId] = e;

			int nodes[4] = {
				mapVertices[e->getVertex(0)->getId()],
				mapVertices[e->getVertex(2)->getId()],
				mapVertices[e->getVertex(1)->getId()],
				mapVertices[e->getVertex(3)->getId()]};

			//Data::cout() << "element " << e->text(this->mesh) << " added as: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << ", " << nodes[3] << Data::endl;

			//go back to original (right-hand rule) tetrahedron
			Msh3D::AdvFront3D::TetElement elem = {{nodes[0], nodes[2], nodes[1], nodes[3]}};
			tetTable->Store(++StartId, elem);

			tetTopo->AddTetrahedron(nodes, StartId);

			if (StartId > this->maxIdElement)
			{
				this->maxIdElement = StartId;
			}

			//Data::cout() << "debug adding tetrahedron to tettopo: " << nodes[0] << ", " << nodes[1] << ", " << nodes[2] << ", " << nodes[3] << Data::endl;
		}

		this->mshRegion->SetStartElemId(++StartId);

		for (fiter.First(); fiter.More(); ++fiter)
		{
			fiter.Entry().tet_topo_id = tetTopo->GetFaceFromVerts(
				fiter.Entry().node_id[0], fiter.Entry().node_id[1], fiter.Entry().node_id[2]);

			//Data::cout()  << "debug changing tet_topo_id for face: " << fiter.Entry().node_id[0] << ", " << fiter.Entry().node_id[1] << ", " << fiter.Entry().node_id[2] << ": " << fiter.Entry().tet_topo_id << Data::endl;
		}

		advFront->SetMaxBoundaryNodeId(maxBoundaryNodeId);
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeToMToolsConversion = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

bool PMTools::MeshGenerator::executeFrontAdvancing()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	try
	{
		this->mshRegion->execute();
	}
	catch (std::exception &e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());

#if USE_GUI
		Data::setStepByStep(step);
#endif //#if USE_GUI

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		Real total = Data::time() - start;

		this->timeMeshGeneration += this->mshRegion->timeGeneration;
		this->timeMeshImprovement += this->mshRegion->timeImprovement;
		this->timeOtherStuff = total - (this->timeMeshGeneration + this->timeMeshImprovement);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		this->status = this->error;

		this->status.append(e.what());

		return false;
	}

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real total = Data::time() - start;

    this->timeMeshGeneration += this->mshRegion->timeGeneration;
    this->timeMeshImprovement += this->mshRegion->timeImprovement;
    this->timeOtherStuff += total - (this->timeMeshGeneration + this->timeMeshImprovement);
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return true;
}

bool PMTools::MeshGenerator::executeMeshBuilding()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	////debug
	//int debugrank = MPI::COMM_WORLD.Get_rank();
	////endebug

	/*if (debugrank == 0)
	{
		ULInt maxIds[3] = {0, 0, 0};

		if (this->boundary)
		{
			Data::VertexList vs = this->boundary->getVertices();

			while (!vs.empty())
			{
				ULInt id = vs.front()->getId();

				vs.pop_front();

				if (id > maxIds[0])
				{
					maxIds[0] = id;
				}
			}

			Data::FrontElementList fels = this->boundary->getElements();

			while (!fels.empty())
			{
				ULInt id = fels.front()->getId();

				fels.pop_front();

				if (id > maxIds[1])
				{
					maxIds[1] = id;
				}
			}
		}

		if (this->front)
		{
			Data::VertexList vs = this->front->getVertices();

			while (!vs.empty())
			{
				ULInt id = vs.front()->getId();

				vs.pop_front();

				if (id > maxIds[0])
				{
					maxIds[0] = id;
				}
			}

			Data::FrontElementList fels = this->front->getAllElements();

			while (!fels.empty())
			{
				ULInt id = fels.front()->getId();

				fels.pop_front();

				if (id > maxIds[1])
				{
					maxIds[1] = id;
				}
			}
		}

		if (this->mesh)
		{
			Data::VertexList vs = this->mesh->getVertices();

			while (!vs.empty())
			{
				ULInt id = vs.front()->getId();

				vs.pop_front();

				if (id > maxIds[0])
				{
					maxIds[0] = id;
				}
			}

			Data::FrontElementList fels = this->mesh->getFrontElements();

			while (!fels.empty())
			{
				ULInt id = fels.front()->getId();

				fels.pop_front();

				if (id > maxIds[1])
				{
					maxIds[1] = id;
				}
			}

			Data::ElementList els = this->mesh->getElements();

			while (!els.empty())
			{
				ULInt id = els.front()->getId();

				els.pop_front();

				if (id > maxIds[2])
				{
					maxIds[2] = id;
				}
			}
		}

		Data::cout() << "maxids:       " << maxIds[0] << ", " << maxIds[2] << ", " << maxIds[2] << Data::endl;
		Data::cout() << "idManagerIds: " << this->idManager->current(0) << ", " << this->idManager->current(1) << ", " << this->idManager->current(2) << Data::endl;
	}*/

	//Real debugstart = Data::time();

	if (!this->mesh)
	{
		this->mesh = new MJMesh::Mesh();
	}

	//Data::cout() << "mesh sizes = " << this->mesh->size() << ", " << this->mesh->frontElementsSize() << ", " << this->mesh->verticesSize() << ", " << this->mesh->innerVerticesSize() << Data::endl;

	//if (this->fromMesh) Data::cout() << "time for creating new mesh = " << Data::time() - debugstart << Data::endl;
	//debugstart = Data::time();

	this->front->clear();

	//if (this->fromMesh) Data::cout() << "time for clearing front = " << Data::time() - debugstart << Data::endl;
	//debugstart = Data::time();

	if ((this->fromMesh) && (this->deleteFels))
	{
		for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
			 iter != this->mesh->feEnd();)
		{
			Data::FrontElement *fel = (*iter);

			/*//debug
			if (fel->getId() == 84082)
            {
                Data::cout() << "fel " << fel->getId() << " at " << fel <<
                    ", boundary at " << this->boundary <<
                    ", fel in boundary = " << std::boolalpha << this->boundary->have(fel) << Data::endl;
            }
			//endebug*/

			//Data::cout() << "testing fel at " << fel << Data::endl;
			//Data::cout() << "testing fel " << fel->text() << " at " << fel << ", have = " << this->boundary->have(fel) << Data::endl;

			if (this->boundary->have(fel))
			{
				iter++;
			}
			else
			{
				//if (debugrank == 0) Data::cout() << "deleting fel " << fel->text() << Data::endl;
				//Data::cout() << "deleting fel " << fel->text() << Data::endl;
				/*if (fel->getId() == 362599)
				{
					Data::cout() << "deleting not in boundary fel " << fel->text() << " at " << fel << Data::endl;
				}*/

				iter = this->mesh->erase(iter);

				delete fel;
			}
		}

		//if (this->fromMesh) Data::cout() << "time for deleting non-boundary front elements = " << Data::time() - debugstart << Data::endl;
		//debugstart = Data::time();
	}

	//Data::cout() << "mesh sizes = " << this->mesh->size() << ", " << this->mesh->frontElementsSize() << ", " << this->mesh->verticesSize() << ", " << this->mesh->innerVerticesSize() << Data::endl;

#if USE_C__11
    Data::VertexHashMap newVertices;

    newVertices.reserve(this->mshRegion->NumNodes());
#else
    Data::VertexMap newVertices;
#endif //#if USE_C__11

	int vMapSize = static_cast<int>(this->vMap.size());
	int vMapBoundary = this->fromMesh ? static_cast<int>(this->boundary->verticesSize()) : 0;

	///Converting vertices
	{
		//if (debugrank == 0) Data::cout() << "this->vMap resizing size = " << this->vMap.size() << ", this->num_node = " << this->num_node << Data::endl;
		//if (debugrank == 0) Data::cout() << "this->vMap resizing size = " << this->vMap.size() << ", this->num_gen_nodes = " << this->num_gen_nodes << Data::endl;

		Msh3D::MshRegion3D::MshRegNodeIterator niter = this->mshRegion->GetInternalNodes();

		for (niter.First(); niter.More(); niter.Next())
		{
			int id = niter.Id() - 1;

			if (id < vMapSize)
			{
				if ((this->fromMesh) &&
					(id >= vMapBoundary))
				{
					static_cast<Data::Point3D *>(this->vMap[id]->getPoint())->setPosition(niter.NCoord(0), niter.NCoord(1), niter.NCoord(2));
				}

				continue;
			}

			Data::Point3D *p = new Data::Point3D(niter.NCoord(0), niter.NCoord(1), niter.NCoord(2));

			MJMesh::Vertex *v = new MJMesh::Vertex(p, this->idManager->next(0));

			newVertices[niter.Id() - 1] = v;

			//Data::cout() << "new vertex " << v->text() << ", mapped from vertex " << niter.Id() << Data::endl;

			//if (debugrank == 0) Data::cout() << "adding to this->vMap = " << v->text() << " in position " << this->num_node + i << Data::endl;

			//Data::cout()  << "vertex " << niter.Id() << " back-mapped to vertex " << v->text() << Data::endl;
			/*Data::cout() << "vertex " << niter.Id() << " back-mapped to vertex " << v->getId() << " " <<
				v->getPoint()->getCoord(0) << " " << v->getPoint()->getCoord(1) << " " << v->getPoint()->getCoord(2)
				<< Data::endl;*/

			this->mesh->add(v);
		}

		//if (debugrank == 0) Data::cout() << "this->vMap resizing (filled) size = " << this->vMap.size() << ", this->num_node + this->num_gen_nodes = " << this->num_node + this->num_gen_nodes << Data::endl;
	}

	//Data::cout() << "mesh sizes = " << this->mesh->size() << ", " << this->mesh->frontElementsSize() << ", " << this->mesh->verticesSize() << ", " << this->mesh->innerVerticesSize() << Data::endl;

	//if (this->fromMesh) Data::cout() << "time for converting internal vertices = " << Data::time() - debugstart << Data::endl;
	//debugstart = Data::time();

	///Converting front elements
	{
		const PMTools::AdvFront3D *advFront = static_cast<const PMTools::AdvFront3D *>(this->mshRegion->GetAdvFront());
		PMTools::AdvFront3D::FFaceHeap *FaceHeap = advFront->GetFaceHeap();

		//Data::cout() << "converting front elements" << Data::endl;

		//UInt numFaces = 0;

		while ((FaceHeap) && (FaceHeap->Len() > 0))
		{
			PMTools::AdvFront3D::FFace *face = *FaceHeap->GetMin();

			Data::Vertex *vs[3];
			Data::FrontElement *fel = NULL;

			bool fromInput = true;

			//Data::cout() << "this->num_node = " << this->num_node << Data::endl;
			//Data::cout() << "face->vertices are: ";

			for (int i = 0; i < 3; i++)
			{
				vs[i] = (face->node_id[i] - 1 < vMapSize) ?
						this->vMap[face->node_id[i] - 1] : newVertices[face->node_id[i] - 1];

				//Data::cout() << "new face, node[" << i << "] = " << face->node_id[i] << ", found as " << vs[i]->text() << Data::endl;

				//if (debugrank == 0) Data::cout() << "this->vMap size = " << this->vMap.size() << ", face->verts[i] = " << face->verts[i] << ", ok = " << (face->verts[i] < this->vMap.size()) << Data::endl;

				//Data::cout() << face->verts[i] << " ";

				if ((fromInput) && (vs[i]->getId() > this->maxIdFrontVertex))
				{
					fromInput = false;
				}
			}

			//Data::cout() << "new front face: " << face->tet_topo_id << ", from vertices " << face->node_id[0] << ", " << face->node_id[1] << ", " << face->node_id[2] << Data::endl;

			//Data::cout() << "fromInput = " << fromInput << Data::endl;

			if (fromInput)
			{
				for (UInt k = 0; k < 3; k++)
				{
					//Data::FrontElementList adjFels = this->mesh->findAdjacentFrontElements(vs[k]);

					//Data::cout() << "number of fels adjacent to first vertex = " << adjFels.size() << Data::endl;

					//for (Data::FrontElementList::iterator iter = adjFels.begin();
					for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(vs[k])->feBegin();
						 //iter != adjFels.end(); iter++)
						 iter != static_cast<MJMesh::Vertex *>(vs[k])->feEnd(); iter++)
					{
						Data::FrontElement *adj = (*iter);

						//Data::cout() << "adj = " << adj->text() << Data::endl;

						bool found[3] = {false, false, false};

						for (UInt i = 0; i < 3; i++)
						{
							for (UInt j = 0; j < 3; j++)
							{
								if (found[j])
								{
									continue;
								}

								if (adj->getVertex(i) == vs[j])
								{
									found[j] = true;

									//Data::cout() << "found vertex[" << j << "]: " << vs[j]->text() << Data::endl;

									break;
								}
							}
						}

						if ((found[0]) && (found[1]) && (found[2]))
						{
							fel = adj;

							//Data::cout() << "fel existed " << fel->text() << Data::endl;
							//if (debugrank == 0) Data::cout() << "fel existed " << fel->text() << Data::endl;

							break;
						}
					}

					if (fel)
					{
					    /*if (fel->getId() == 47735)
                        {
                            std::cout << "debug" << std::endl;
                        }*/

						break;
					}
				}
			}

			if (!fel)
			{
				fel = new MJMesh::FrontElement3D(
					static_cast<MJMesh::Vertex *>(vs[0]),
					static_cast<MJMesh::Vertex *>(vs[1]),
					static_cast<MJMesh::Vertex *>(vs[2]),
					this->idManager->next(1));

				this->mesh->add(fel);

				/*//debug
				if (fel->getId() == 691658)
				{
					Data::cout() << "PMTools::MeshGenerator fel " << fel->text() << " at " << fel << Data::endl;
				}
				//endebug*/

				//Data::cout() << "new fel " << fel->text() << " mapped from face " << face->tet_topo_id << ", from vertices " << face->node_id[0] << ", " << face->node_id[1] << ", " << face->node_id[2] << Data::endl;

				//Data::cout() << "process " << MPI::COMM_WORLD.Get_rank() << ", mesh generator, created fel " << fel->text() << Data::endl;
			}

			//if (debugrank == 0) Data::cout() << "adding fel to the front: " << fel->text() << Data::endl;
			//Data::cout() << "face id " << face->tet_topo_id << ", verts " << face->node_id[0] << ", " << face->node_id[1] << ", " << face->node_id[2] << " back-mapped to fel " << fel->text() << Data::endl;
			/*Data::cout() << "face id " << face->tet_topo_id << ", verts " << face->node_id[0] << ", " << face->node_id[1] << ", " << face->node_id[2] << " back-mapped to fel " << fel->getId()
				<< " " << fel->getVertex(0)->getId() << " " << fel->getVertex(1)->getId() << " " << fel->getVertex(2)->getId()
				<< Data::endl;*/

			this->front->add(fel);

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				if (!this->front->have(fel->getVertex(i)))
				{
					this->front->add(fel->getVertex(i));
				}
			}

			//numFaces++;
		}
	}

	//Data::cout() << "mesh sizes = " << this->mesh->size() << ", " << this->mesh->frontElementsSize() << ", " << this->mesh->verticesSize() << ", " << this->mesh->innerVerticesSize() << Data::endl;

	//if (this->fromMesh) Data::cout() << "time for converting front elements = " << Data::time() - debugstart << Data::endl;

	//Real timeRemoving, timeVertices, timeNew, timeAddMesh, timeFind, timeAddFel;
	//timeRemoving = timeVertices = timeNew = timeAddMesh = timeFind = timeAddFel = 0.0;

	//debugstart = Data::time();

	//Data::cout() << "number of faces: " << numFaces << Data::endl;

	///Converting elements
	{
		Msh3D::MshRegion3D::MshRegElemIterator eiter = this->mshRegion->GetElements();

		for	(eiter.First(); eiter.More(); eiter.Next())
		{
			//Real st = Data::time();

			MJMesh::Element *el = NULL;

			if ((this->fromMesh) && (eiter.Id() <= this->maxIdElement))
			{
#if USE_C__11
                Data::ElementHashMap::iterator it = this->eMap.find(eiter.Id() - 1);

                el = static_cast<MJMesh::Element *>((*it).second);

                this->eMap.erase(it);
#else
				el = static_cast<MJMesh::Element *>(this->eMap[eiter.Id() - 1]);

				this->eMap[eiter.Id() - 1] = NULL;
#endif //#if USE_C__11

				//timeRemoving += Data::time() - st;

				//continue;
			}

			//timeRemoving += Data::time() - st;
			//st = Data::time();

			Data::Vertex *vs[4];

			//if (debugrank == 0) Data::cout() << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 0] = " << this->elems[i*4 + 0] << ", ok = " << (this->elems[i*4 + 0] < this->vMap.size()) << Data::endl;
			//if (debugrank == 0) Data::cout() << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 1] = " << this->elems[i*4 + 1] << ", ok = " << (this->elems[i*4 + 1] < this->vMap.size()) << Data::endl;
			//if (debugrank == 0) Data::cout() << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 2] = " << this->elems[i*4 + 2] << ", ok = " << (this->elems[i*4 + 2] < this->vMap.size()) << Data::endl;
			//if (debugrank == 0) Data::cout() << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 3] = " << this->elems[i*4 + 3] << ", ok = " << (this->elems[i*4 + 3] < this->vMap.size()) << Data::endl;

			vs[0] = (eiter.Node(0) - 1 < vMapSize) ?
				this->vMap[eiter.Node(0) - 1] : newVertices[eiter.Node(0) - 1];
			vs[1] = (eiter.Node(1) - 1 < vMapSize) ?
				this->vMap[eiter.Node(1) - 1] : newVertices[eiter.Node(1) - 1];
			vs[2] = (eiter.Node(2) - 1 < vMapSize) ?
				this->vMap[eiter.Node(2) - 1] : newVertices[eiter.Node(2) - 1];
			vs[3] = (eiter.Node(3) - 1 < vMapSize) ?
				this->vMap[eiter.Node(3) - 1] : newVertices[eiter.Node(3) - 1];

			//timeVertices += Data::time() - st;
			//st = Data::time();

			//Data::cout() << "new element, node[" << 0 << "] = " << eiter.Node(0) << ", found as " << vs[0]->text() << Data::endl;
			//Data::cout() << "new element, node[" << 1 << "] = " << eiter.Node(1) << ", found as " << vs[1]->text() << Data::endl;
			//Data::cout() << "new element, node[" << 2 << "] = " << eiter.Node(2) << ", found as " << vs[2]->text() << Data::endl;
			//Data::cout() << "new element, node[" << 3 << "] = " << eiter.Node(3) << ", found as " << vs[3]->text() << Data::endl;

			if (el)
			{
				bool change[4] = {
					(el->getVertex(0) != vs[0]),
					(el->getVertex(1) != vs[1]),
					(el->getVertex(2) != vs[2]),
					(el->getVertex(3) != vs[3])};

				bool changed = false;

				for (UInt i = 0; i < 4; i++)
				{
					if (!change[i])
					{
						continue;
					}

					bool found = false;

					for (UInt j = 0; j < 4; j++)
					{
						if ((i != j) && (el->getVertex(i) == vs[j]))
						{
							found = true;

							break;
						}
					}

					if (!found)
					{
						static_cast<MJMesh::Vertex *>(el->getVertex(i))->remove(el);

						//Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(el->getVertex(i));

						//while (!adjs.empty())
						for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(el->getVertex(i))->feBegin();
                             iter != static_cast<MJMesh::Vertex *>(el->getVertex(i))->feEnd(); iter++)
						{
							//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(adjs.front());
							MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

							//adjs.pop_front();

							if (fel->haveAdjacent(el))
							{
								fel->remove(el);
							}
						}
					}

					el->setVertex(i, vs[i]);

					if (!static_cast<MJMesh::Vertex *>(vs[i])->have(el))
					{
						static_cast<MJMesh::Vertex *>(vs[i])->add(el);
					}

					changed = true;
				}

				if (changed)
				{
					Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(el);

					while (!adjs.empty())
					{
						MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(adjs.front());

						adjs.pop_front();

						if (fel->haveAdjacent(el))
						{
							continue;
						}

						fel->add(el);
					}
				}

				continue;
			}

			el = new MJMesh::Element3D(
				static_cast<MJMesh::Vertex *>(vs[0]),
				static_cast<MJMesh::Vertex *>(vs[1]),
				static_cast<MJMesh::Vertex *>(vs[2]),
				static_cast<MJMesh::Vertex *>(vs[3]),
				this->idManager->next(2));

			//timeNew += Data::time() - st;
			//st = Data::time();

			//Data::cout() << "process " << MPI::COMM_WORLD.Get_rank() << ", mesh generator, creating el " << el->text() << Data::endl;

			this->mesh->add(el);

			//timeAddMesh += Data::time() - st;
			//st = Data::time();

			Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(el);

			//timeFind += Data::time() - st;
			//st = Data::time();

			/*//if (debugrank == 0) Data::cout() << "analyzing element " << el->text() << Data::endl;

			for (UInt k = 0; k < 4; k++)
			{
				Data::Vertex *v = vs[k];

				Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(v);

				//if (debugrank == 0) Data::cout() << "analyzing vertex " << v->text() << ", k = " << k << ", adjs.size() = " << adjs.size() << Data::endl;

				while (!adjs.empty())
				{
					Data::FrontElement *fel = adjs.front();

					adjs.pop_front();

					//if (debugrank == 0) Data::cout() << "analyzing fel " << fel->text() << Data::endl;

					if (std::find(fels.begin(), fels.end(), fel) != fels.end())
					{
						continue;
					}

					UInt count = 0;

					for (UInt j = 1; j < 4; j++)
					{
						//if (debugrank == 0) Data::cout() << "j = " << j << ", k+j = " << k + j << ", (k + j)%4 = " << (k + j)%4 << Data::endl;

						if (fel->have(vs[(k + j)%4]))
						{
							count++;
						}
					}

					//if (debugrank == 0) Data::cout() << "count = " << count << Data::endl;

					if (count >= 2)
					{
						fels.push_back(fel);
					}
				}
			}*/

			//if (debugrank == 0) Data::cout() << "fels.size() = " << fels.size() << Data::endl;

			while (!fels.empty())
			{
				MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

				fels.pop_front();

				fel->add(el);
			}

			//timeAddFel += Data::time() - st;
			//st = Data::time();
		}

		//if (this->fromMesh) Data::cout() << "time for converting elements = " << Data::time() - debugstart << Data::endl;

		//Data::cout() << "time for converting elements, removing from vector = " << timeRemoving << Data::endl;
		//Data::cout() << "time for converting elements, finding vertices = " << timeVertices << Data::endl;
		//Data::cout() << "time for converting elements, instantiating = " << timeNew << Data::endl;
		//Data::cout() << "time for converting elements, adding to mesh = " << timeAddMesh << Data::endl;
		//Data::cout() << "time for converting elements, finding adjacent front elements = " << timeFind << Data::endl;
		//Data::cout() << "time for converting elements, adding in front elements = " << timeAddFel << Data::endl;

		//Real timeNot, timeDelete;
		//timeNot = timeFind = timeAddFel = timeRemoving = timeDelete = 0.0;

		//if (this->fromMesh) Data::cout() << "this->eMap.size() = " << this->eMap.size() << Data::endl;
		//UInt count = 0;

		//debugstart = Data::time();

#if USE_C__11
		for (Data::ElementHashMap::iterator iter = this->eMap.begin(), end = this->eMap.end();
#else
		for (Data::ElementVector::iterator iter = this->eMap.begin(), end = this->eMap.end();
#endif //#if USE_C__11
			 iter != end; iter++)
		{
			//Real st = Data::time();

#if USE_C__11
			Data::Element *e = (*iter).second;
#else
			Data::Element *e = (*iter);
#endif //#if USE_C__11

			if (!e)
			{
				//timeNot += Data::time() - st;

				continue;
			}

			//timeNot += Data::time() - st;
			//st = Data::time();

			Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(e);

			//timeFind += Data::time() - st;
			//st = Data::time();

			while (!fels.empty())
			{
				static_cast<MJMesh::FrontElement *>(fels.front())->remove(static_cast<MJMesh::Element *>(e));

				fels.pop_front();
			}

			//timeAddFel += Data::time() - st;
			//st = Data::time();

			this->mesh->remove(e);

			//timeRemoving += Data::time() - st;
			//st = Data::time();

			delete e;

#if USE_C__11
            (*iter).second = NULL;
#else
			(*iter) = NULL;
#endif //#if USE_C__11

			//count++;

			//timeDelete += Data::time() - st;
			//st = Data::time();
		}

		//if (this->fromMesh) Data::cout() << "time for deleting (" << count << ") old elements = " << Data::time() - debugstart << Data::endl;

		//Data::cout() << "time for deleting old elements, checking in vector = " << timeNot << Data::endl;
		//Data::cout() << "time for deleting old elements, finding adjacent front elements = " << timeFind << Data::endl;
		//Data::cout() << "time for deleting old elements, removing in adjacent front elements = " << timeAddFel << Data::endl;
		//Data::cout() << "time for deleting old elements, removing from mesh = " << timeRemoving << Data::endl;
		//Data::cout() << "time for deleting old elements, deleting = " << timeDelete << Data::endl;
	}

	if ((this->fromMesh) && (!this->workerTask) && (!this->useBoxTest))
    {
        while (!this->verticesNotAddedToBoundary.empty())
        {
            Data::Vertex *v = this->verticesNotAddedToBoundary.front();

            this->front->add(v);

            this->verticesNotAddedToBoundary.pop_front();
        }

        while (!this->verticesAddedToBoundary.empty())
        {
            Data::Vertex *v = this->verticesAddedToBoundary.front();

            const_cast<MJMesh::Boundary *>(this->boundary)->remove(v);

            this->front->add(v);

            this->verticesAddedToBoundary.pop_front();
        }

        for (UShort i = 0; i < 4; i++)
        {
            while (!this->felsNotAddedToBoundary[i].empty())
            {
                Data::FrontElement *fel = this->felsNotAddedToBoundary[i].front();

                this->felsNotAddedToBoundary[i].pop_front();

                switch (i)
                {
                case 0:
                    static_cast<PMJMesh::Front *>(this->front)->addBoundary(fel);
                    break;
                case 1:
                    static_cast<PMJMesh::Front *>(this->front)->addElement(fel);
                    break;
                case 2:
                    static_cast<PMJMesh::Front *>(this->front)->addRejected(fel);
                    break;
                case 3:
                    static_cast<PMJMesh::Front *>(this->front)->addCrossing(fel);
                    break;
                }
            }

            while (!this->felsAddedToBoundary[i].empty())
            {
                Data::FrontElement *fel = this->felsAddedToBoundary[i].front();

                this->felsAddedToBoundary[i].pop_front();

                const_cast<MJMesh::Boundary *>(this->boundary)->remove(fel);

                switch (i)
                {
                case 0:
                    static_cast<PMJMesh::Front *>(this->front)->addBoundary(fel);
                    break;
                case 1:
                    static_cast<PMJMesh::Front *>(this->front)->addElement(fel);
                    break;
                case 2:
                    static_cast<PMJMesh::Front *>(this->front)->addRejected(fel);
                    break;
                case 3:
                    static_cast<PMJMesh::Front *>(this->front)->addCrossing(fel);
                    break;
                }
            }
        }
    }

	//Data::cout()  << "mesh sizes = " << this->mesh->size() << ", " << this->mesh->frontElementsSize() << ", " << this->mesh->verticesSize() << ", " << this->mesh->innerVerticesSize() << Data::endl;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeFromMToolsConversion = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return true;
}

#endif //#if USE_MTOOLS
