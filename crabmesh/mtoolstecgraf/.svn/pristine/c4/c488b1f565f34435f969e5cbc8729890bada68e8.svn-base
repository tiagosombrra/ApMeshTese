#include "PJmesh/MeshGenerator.h"

#include "Data/Point3D.h"
#include "Performer/IdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "PJmesh/JmeshMainDef3D.h"

PJmesh::MeshGenerator::MeshGenerator() :
	Performer::MeshGenerator()
{
	this->pjmesh_num_original_nodes = 0;
	this->pjmesh_num_original_faces = 0;
	this->pjmesh_original_faces = NULL;
	this->pjmesh_num_original_elems = 0;
	this->pjmesh_use_box_test = 0;

	for (int i = 0; i < 3; i++)
	{
		this->pjmesh_box_min[i] = 0.0;
		this->pjmesh_box_max[i] = 0.0;
	}

	this->pjmesh_range_proportion = 1.0;

	offset = 0;
	tnodes = NULL;
	num_gen_nodes = 0;
	tgenerated_nodes = NULL;

	nodes = NULL;
	faces = NULL;
	generated_nodes = NULL;
	elems = NULL;

	internal = 0;
	fromone = 0;
	version = 0;
	num_node = 0;
	num_face = 0;
	factor = 0.0;
	tol = 0.0;
	num_elems = 0;
	status = 0;

	this->boundary = NULL;
	this->front = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeMeshGeneration = 0.0;
	this->timeToJmeshConversion = 0.0;
	this->timeFromJmeshConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PJmesh::MeshGenerator::~MeshGenerator()
{

}

void PJmesh::MeshGenerator::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void PJmesh::MeshGenerator::setUseBoxTest(bool useBoxTest)
{
	this->pjmesh_use_box_test = ((useBoxTest) ? 1 : 0);
}

void PJmesh::MeshGenerator::setBox(const double *min, const double *max)
{
	for (int i = 0; i < 3; i++)
	{
		this->pjmesh_box_min[i] = min[i];
		this->pjmesh_box_max[i] = max[i];
	}
}

void PJmesh::MeshGenerator::setRangeProportion(double rangeProportion)
{
	this->pjmesh_range_proportion = rangeProportion;
}

void PJmesh::MeshGenerator::setFront(MJMesh::Front *front)
{
	this->front = front;
}

void PJmesh::MeshGenerator::setBoundary(const MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void PJmesh::MeshGenerator::setVersion(int version)
{
	this->version = version;
}

void PJmesh::MeshGenerator::setInternal(int internal)
{
	this->internal = internal;
}

void PJmesh::MeshGenerator::setFactor(double factor)
{
	this->factor = factor;
}

void PJmesh::MeshGenerator::setTol(double tol)
{
	this->tol = tol;
}

void PJmesh::MeshGenerator::setOffset(int offset)
{
	this->offset = offset;
}

void PJmesh::MeshGenerator::setFromone(int fromone)
{
	this->fromone = fromone;
}

int PJmesh::MeshGenerator::getFromone() const
{
	return this->fromone;
}

int PJmesh::MeshGenerator::getJmeshStatus() const
{
	return this->status;
}

void PJmesh::MeshGenerator::execute()
{
	this->convertFrontToJmesh();

	this->executeFrontAdvancing();

	this->executeMeshBuilding();
}

////debug
//#include "Parallel/Definitions.h"
//#include <mpi.h>
////endebug

void PJmesh::MeshGenerator::convertFrontToJmesh()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	fromone = 0;

	////debug
	//int debugrank = MPI::COMM_WORLD.Get_rank();
	////endebug

	this->vMap.clear();

	///Converting vertices
	Data::IntMap map;

	bool fromMesh = ((this->mesh) && (this->mesh->size() != 0));

	Data::VertexList vertices = fromMesh ? this->boundary->getVertices() : this->front->getVertices();

	int size = static_cast<int>(vertices.size());

	this->vMap.resize(size);

	//if (debugrank == 0) std::cout << "this->vMap initial size = " << this->vMap.size() << ", size = " << size << std::endl;

	this->pjmesh_num_original_nodes = (fromMesh) ? static_cast<int>(this->mesh->innerVerticesSize()) : 0;

	if (this->num_node == 0)
	{
		this->num_node = size;

		this->nodes = (double **)Msh3DCalloc(num_node, sizeof(double *));

		for (int i = 0; i < this->num_node; i++)
		{
			this->nodes[i] = (double *)Msh3DCalloc(3, sizeof(double));
		}

		this->tnodes = (int *)Msh3DCalloc(num_node, sizeof(int));
	}
	else if (this->num_node != size)
	{
		for (int i = 0; i < this->num_node; i++)
		{
			Msh3DFree(this->nodes[i]);
		}

		this->num_node = size;

		this->nodes = (double **)Msh3DRealloc(this->nodes, num_node*sizeof(double *));

		for (int i = 0; i < this->num_node; i++)
		{
			this->nodes[i] = (double *)Msh3DCalloc(3, sizeof(double));
		}

		this->tnodes = (int *)Msh3DRealloc(this->tnodes, num_node*sizeof(int));
	}

	int i = 0;

	vertices.sort(Data::Identifiable::less);

	while (!vertices.empty())
	{
		Data::Vertex *v = vertices.front();

		vertices.pop_front();

		for (int j = 0; j < 3; j++)
		{
			this->nodes[i][j] = v->getPoint()->getCoord(j);
		}

		this->tnodes[i] = i;

		this->vMap[i] = v;
		map[v->getId()] = this->tnodes[i];

		////debug
		//if (debugrank == 0) std::cout << "adding to this->vMap = " << v->text() << " in position " << i << std::endl;
		////endebug

		//std::cout << "setting vertex map, mjmesh vertex = " << v->getId() << " mapped to jmesh vertex " << i << std::endl;
		//std::cout << "vertex " << v->text() << " mapped to vertex " << i << std::endl;

		if (!fromMesh)
		{
			this->mesh->add(v);
		}

		i++;
	}

	//if (debugrank == 0) std::cout << "this->vMap initial (filled) size = " << this->vMap.size() << ", i = " << i << std::endl;

	if (fromMesh)
	{
		i = 0;

		this->num_gen_nodes = this->pjmesh_num_original_nodes;

		this->generated_nodes = (double *)Msh3DMalloc(3*this->num_gen_nodes*sizeof(double));
		this->tgenerated_nodes = (int *)Msh3DCalloc(this->num_gen_nodes, sizeof(int));

		//Data::VertexList inner = this->mesh->getInnerVertices();

		//while (!inner.empty())
#if USE_EXPERIMENTAL_MESH
		for (Data::Mesh::InnerVertexIterator iter = this->mesh->ivBegin();
#else
		for (Data::Mesh::ConstVertexIterator iter = this->mesh->ivBegin();
#endif //#if USE_EXPERIMENTAL_MESH
			 iter != this->mesh->ivEnd(); iter++)
		{
			//Data::Vertex *v = inner.front();
			Data::Vertex *v = (*iter);

			//inner.pop_front();

			for (int j = 0; j < 3; j++)
			{
				this->generated_nodes[3*i + j] = v->getPoint()->getCoord(j);
			}

			this->tgenerated_nodes[i] = i + this->num_node;

			map[v->getId()] = this->tgenerated_nodes[i];

			////debug
			//if (debugrank == 0) std::cout << "adding mesh vertex to inner vertices = " << v->text() << std::endl;
			////endebug

			i++;
		}
	}

	///Converting front elements
	Data::FrontElementList fels = this->front->getAllElements();

	if (this->num_face == 0)
	{
		this->num_face = static_cast<int>(fels.size());

		this->faces = (int **)Msh3DCalloc(num_face, sizeof(int *));

		for (int i = 0; i < this->num_face; i++)
		{
			this->faces[i] = (int *)Msh3DCalloc(3, sizeof(int));
		}
	}
	else if (this->num_face != static_cast<int>(fels.size()))
	{
		for (int i = 0; i < this->num_face; i++)
		{
			Msh3DFree(this->faces[i]);
		}

		this->num_face = static_cast<int>(fels.size());

		this->faces = (int **)Msh3DRealloc(this->faces, num_face*sizeof(int *));

		for (int i = 0; i < this->num_face; i++)
		{
			this->faces[i] = (int *)Msh3DCalloc(3, sizeof(int));
		}
	}

	i = 0;

	fels.sort(Data::Identifiable::less);

	while (!fels.empty())
	{
		Data::FrontElement *fel = fels.front();

		fels.pop_front();

		if (!fromMesh)
		{
			this->mesh->add(fel);
		}

		for (int j = 0; j < 3; j++)
		{
			this->faces[i][j] = map[fel->getVertex(j)->getId()];
		}

		//std::cout << "fel " << fel->text() << " mapped to face " << i << ": " << this->faces[i][0] << ", " << this->faces[i][1] << ", " << this->faces[i][2] << std::endl;
		//std::cout << "face " << fel->getId() << " mapped to " << i << std::endl;

		i++;
	}

	if (fromMesh)
	{
		fels = this->boundary->getElements();

		this->pjmesh_num_original_faces = static_cast<int>(fels.size());

		this->pjmesh_original_faces = (int *)Msh3DCalloc(3*this->pjmesh_num_original_faces, sizeof(int));

		i = 0;

		fels.sort(Data::Identifiable::less);

		while (!fels.empty())
		{
			Data::FrontElement *fel = fels.front();

			fels.pop_front();

			for (int j = 0; j < 3; j++)
			{
				this->pjmesh_original_faces[i++] = map[fel->getVertex(j)->getId()];
			}
		}
	}

	if (fromMesh)
	{
		//Data::ElementList els = this->mesh->getSortedElements();
		Data::ElementList els = this->mesh->getElements();
		els.sort(Data::Identifiable::less);

		this->pjmesh_num_original_elems = static_cast<int>(els.size());

		this->elems = (int *)Msh3DMalloc(this->pjmesh_num_original_elems*4*sizeof(int));

		i = 0;

		while (!els.empty())
		{
			Data::Element *e = els.front();
			els.pop_front();

			this->elems[4*i+0] = map[e->getVertex(0)->getId()];
			this->elems[4*i+1] = map[e->getVertex(1)->getId()];
			this->elems[4*i+2] = map[e->getVertex(2)->getId()];
			this->elems[4*i+3] = map[e->getVertex(3)->getId()];

			i++;
		}
	}
	else
	{
		this->pjmesh_num_original_elems = 0;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeToJmeshConversion = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

bool PJmesh::MeshGenerator::executeFrontAdvancing()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	fprintf (stderr, "Model Creation:") ;
    fprintf( stderr, "\n" );

#if DSP_TIME
    init_time = clock( );
    init_gbl_time = clock( );
    fprintf( stderr, "\n" );
    fprintf( stderr, "	Building look-up table................" );
    fprintf( stderr, "\n" );
#endif

#if DSP_TIME
    fprintf( stderr,"\n") ;
    tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
    fprintf( stderr,"\t\t Initial front size: %8d fac.\n", num_face);
    fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.", (double)tot_time);
    fprintf( stderr,"\n") ;
#endif

#if VER_BOTH
    this->status = PMsh3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems,
                              pjmesh_num_original_elems, pjmesh_num_original_faces,
                              pjmesh_original_faces, pjmesh_use_box_test,
                              pjmesh_box_min, pjmesh_box_max, pjmesh_range_proportion ) ;

    if (this->status == 0) {
#if VER_PRNT
     fprintf (stderr, "\n");
     fprintf (stderr, "\n");
     fprintf (stderr, "\t--------------------------------------------------\n");
     fprintf (stderr, "\t                  J M E S H                       \n");
     fprintf (stderr, "\tWARNING:  The volume mesh could not be generated!!\n");
     fprintf (stderr, "\tIt is probably related  to problems in the surface\n");
     fprintf (stderr, "\tmesh,  regarding the input requirements for JMESH.\n");
     fprintf (stderr, "\tFor more details,  please see JMESH documentation.\n");
     fprintf (stderr, "\t--------------------------------------------------\n");
     fprintf (stderr, "\n");
#endif
     //exit (1) ;

		if (num_elems != 0)
		{
			Msh3DFree(elems);
			elems = NULL;

			num_elems = 0;
		}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeMeshGeneration = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		return false;
    }
#else
    this->status = PMsh3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems,
                              pjmesh_num_original_elems, pjmesh_num_original_faces,
                              pjmesh_original_faces, pjmesh_use_box_test,
                              pjmesh_box_min, pjmesh_box_max, pjmesh_range_proportion ) ;
#endif

	if (this->status == 0)
	{
		if (num_elems != 0)
		{
			Msh3DFree(elems);
			elems = NULL;

			num_elems = 0;
		}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeMeshGeneration = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		return false;
	}

#if DSP_PRNT
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\t                  J M E S H                      \n" );
    fprintf (stderr, "\t  THREE-DIMENSIONAL TETRAHEDRAL MESH GENERATOR   \n" );
    fprintf (stderr, "\t    TECGRAF/PUC-Rio & CFG/Cornell University     \n" );
    fprintf (stderr, "\t                 October/2003                    \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t             Joaquim Cavalcante-Neto             \n" );
    fprintf (stderr, "\t                 Wash Wawrzynek                  \n" );
    fprintf (stderr, "\t                  Bruce Carter                   \n" );
    fprintf (stderr, "\t               Luiz Fernando Martha              \n" );
    fprintf (stderr, "\t                Anthony Ingraffea                \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t           VOLUME MESH GENERATED!!!              \n" );
    fprintf( stderr,"\t      Generated elements: %8d ele.  \n", num_elems);
#if DSP_TIME
    tot_gbl_time = (clock( ) - init_gbl_time)/CLOCKS_PER_SEC;
    if (tot_gbl_time >= 0)
     fprintf( stderr,"\t      Total elapsed time: %8.2f sec.\n",
		                                     (double)tot_gbl_time);
#endif
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\n" );
#endif

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeMeshGeneration = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return true;
}

bool PJmesh::MeshGenerator::executeMeshBuilding()
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

		std::cout << "maxids:       " << maxIds[0] << ", " << maxIds[2] << ", " << maxIds[2] << "\n";
		std::cout << "idManagerIds: " << this->idManager->current(0) << ", " << this->idManager->current(1) << ", " << this->idManager->current(2) << std::endl;
	}*/

	if (!this->mesh)
	{
		this->mesh = new MJMesh::Mesh();
	}

	Data::VertexList vBuffer;
	Data::ElementList eBuffer;

	bool fromMesh = (this->pjmesh_num_original_elems != 0);

	if (this->status != 0)
	{
		this->front->clear();

		if (fromMesh)
		{
			//eBuffer = this->mesh->getElements();

			//for (Data::ElementList::iterator iter = eBuffer.begin();
			for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin();
				 iter != this->mesh->eEnd(); iter++)
			{
				eBuffer.push_back((*iter));

				Data::FrontElementList fels = this->mesh->findAdjacentFrontElements((*iter));

				while (!fels.empty())
				{
					static_cast<MJMesh::FrontElement *>(fels.front())->remove(static_cast<MJMesh::Element *>((*iter)));

					fels.pop_front();
				}
			}

			this->mesh->setElements(Data::ElementList());

			//vBuffer = this->boundary->getVertices();

			//while (!vBuffer.empty())
			for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
				 iter != this->boundary->vEnd(); iter++)
			{
				//static_cast<MJMesh::Vertex *>(vBuffer.front())->setAdjacentElements(Data::ElementList());
				static_cast<MJMesh::Vertex *>((*iter))->setAdjacentElements(Data::ElementList());

				//vBuffer.pop_front();
			}

			//vBuffer = this->mesh->getInnerVertices();
			//vBuffer = this->mesh->getSortedInnerVertices(); ///CHANGE IT BACK TO getInnerVertices()
			//this->mesh->setInnerVertices(Data::VertexList());

			//for (Data::VertexList::iterator iter = vBuffer.begin();
#if USE_EXPERIMENTAL_MESH
			for (Data::Mesh::InnerVertexIterator iter = this->mesh->ivBegin();
#else
			for (Data::Mesh::VertexIterator iter = this->mesh->ivBegin();
#endif //#if USE_EXPERIMENTAL_MESH
				 //iter != vBuffer.end(); iter++)
				 iter != this->mesh->ivEnd();)
			{
				//this->mesh->remove((*iter));
				vBuffer.push_back((*iter));

				static_cast<MJMesh::Vertex *>((*iter))->setAdjacentElements(Data::ElementList());
				static_cast<MJMesh::Vertex *>((*iter))->setAdjacentFrontElements(Data::FrontElementList());

				//if (debugrank == 0) std::cout << "vertex in buffer = " << (*iter)->text() << std::endl;

				iter = this->mesh->eraseFromInner(iter);
			}

			//Data::FrontElementList fels = this->mesh->getFrontElements();

			//this->mesh->setFrontElements(Data::FrontElementList());

			//while (!fels.empty())
			for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
				 iter != this->mesh->feEnd();)
			{
				//Data::FrontElement *fel = fels.front();
				Data::FrontElement *fel = (*iter);

				//fels.pop_front();

				if (this->boundary->have(fel))
				{
					//this->mesh->add(fel);

					iter++;
				}
				else
				{
					//if (debugrank == 0) std::cout << "deleting fel " << fel->text() << std::endl;

					iter = this->mesh->erase(iter);

					delete fel;
				}
			}

			/*while (!eBuffer.empty())
			{
				delete eBuffer.front();

				eBuffer.pop_front();
			}

			while (!vBuffer.empty())
			{
				delete vBuffer.front();

				vBuffer.pop_front();
			}*/
		}
	}

	///Converting vertices
	if (this->status != 0)
	{
		this->vMap.resize(this->vMap.size() + this->num_gen_nodes);

		//if (debugrank == 0) std::cout << "this->vMap resizing size = " << this->vMap.size() << ", this->num_node = " << this->num_node << std::endl;
		//if (debugrank == 0) std::cout << "this->vMap resizing size = " << this->vMap.size() << ", this->num_gen_nodes = " << this->num_gen_nodes << std::endl;

		//for (int i = this->pjmesh_num_original_nodes; i < this->num_gen_nodes; i++)
		for (int i = 0; i < this->num_gen_nodes; i++)
		{
			Data::Point3D *p = NULL;

			//if (vBuffer.empty())
			if ((!fromMesh) || (vBuffer.empty()))
			{
				p = new Data::Point3D();
			}
			else
			{
				p = static_cast<Data::Point3D *>(vBuffer.front()->getPoint());
			}

			p->setPosition(this->generated_nodes[i*3 + 0],
				this->generated_nodes[i*3 + 1],
				this->generated_nodes[i*3 + 2]);

			MJMesh::Vertex *v = NULL;

			if ((!fromMesh) || (vBuffer.empty()))
			{
				v = new MJMesh::Vertex(p, this->idManager->next(0));

				//if (debugrank == 0) std::cout << "creating vertex = " << v->text() << std::endl;
			}
			else
			{
				v = static_cast<MJMesh::Vertex *>(vBuffer.front());

				vBuffer.pop_front();

				//if (debugrank == 0) std::cout << "replacing vertex = " << v->text() << std::endl;
			}

			this->vMap[this->num_node + i] = v;

			//if (debugrank == 0) std::cout << "adding to this->vMap = " << v->text() << " in position " << this->num_node + i << std::endl;

			//std::cout << "vertex " << this->num_node + i << " back-mapped to vertex " << v->text() << std::endl;

			this->mesh->add(v);
		}

		//if (debugrank == 0) std::cout << "this->vMap resizing (filled) size = " << this->vMap.size() << ", this->num_node + this->num_gen_nodes = " << this->num_node + this->num_gen_nodes << std::endl;

		while (!vBuffer.empty())
		{
			//if (debugrank == 0) std::cout << "deleting vertex from buffer = " << vBuffer.front()->text() << std::endl;

			delete vBuffer.front();

			vBuffer.pop_front();
		}
	}

	if (this->num_gen_nodes != 0)
	{
		Msh3DFree(this->generated_nodes);
		this->generated_nodes = NULL;

		Msh3DFree(this->tgenerated_nodes);
		this->tgenerated_nodes = NULL;

		this->num_gen_nodes = 0;
	}

	///Converting front elements
	if (this->status != 0)
	{
		Msh3DBdryFace face = bdry_stack;

		//std::cout << "converting front elements" << std::endl;

		UInt numFaces = 0;

		while (face)
		{
			Data::Vertex *vs[3];
			Data::FrontElement *fel = NULL;

			bool fromInput = true;

			//std::cout << "this->num_node = " << this->num_node << std::endl;
			//std::cout << "face->vertices are: ";

			for (int i = 0; i < 3; i++)
			{
				vs[i] = this->vMap[face->verts[i]];

				//if (debugrank == 0) std::cout << "this->vMap size = " << this->vMap.size() << ", face->verts[i] = " << face->verts[i] << ", ok = " << (face->verts[i] < this->vMap.size()) << std::endl;

				//std::cout << face->verts[i] << " ";

				if ((fromInput) && (face->verts[i] >= this->num_node))
				{
					fromInput = false;
				}
			}

			//std::cout << "fromInput = " << fromInput << std::endl;

			if (fromInput)
			{
				for (UInt k = 0; k < 3; k++)
				{
					//Data::FrontElementList adjFels = this->mesh->findAdjacentFrontElements(vs[k]);
					//Data::FrontElementList adjFels = static_cast<MJMesh::Vertex *>(vs[0])->getAdjacentFrontElements();

					//std::cout << "number of fels adjacent to first vertex = " << adjFels.size() << std::endl;

					//for (Data::FrontElementList::iterator iter = adjFels.begin();
					for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(vs[k])->feBegin();
						 //iter != adjFels.end(); iter++)
						 iter != static_cast<MJMesh::Vertex *>(vs[k])->feEnd(); iter++)
					{
						Data::FrontElement *adj = (*iter);

						//std::cout << "adj = " << adj->text() << std::endl;

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

									//std::cout << "found vertex[" << j << "]: " << vs[j]->text() << std::endl;

									break;
								}
							}
						}

						if ((found[0]) && (found[1]) && (found[2]))
						{
							fel = adj;

							//if (debugrank == 0) std::cout << "fel existed " << fel->text() << std::endl;

							break;
						}
					}

					if (fel)
					{
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

				/*std::stringstream str;
				str << "process " << MPI::COMM_WORLD.Get_rank() << ", mesh generator, created fel " << fel->text() << std::endl;
				std::cout << str.str();*/
			}

			//if (debugrank == 0) std::cout << "adding fel to the front: " << fel->text() << std::endl;
			//std::cout << "face " << face->verts[0] << ", " << face->verts[1] << ", " << face->verts[2] << " back-mapped to fel " << fel->text() << std::endl;

			this->front->add(fel);

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				if (!this->front->have(fel->getVertex(i)))
				{
					this->front->add(fel->getVertex(i));
				}
			}

			face = face->next;

			numFaces++;
		}
	}

	PMsh3DFaceFreeAll();

	if (this->pjmesh_num_original_faces != 0)
	{
		Msh3DFree(this->pjmesh_original_faces);
		this->pjmesh_original_faces = NULL;

		this->pjmesh_num_original_faces = 0;
	}

	//std::cout << "number of faces: " << numFaces << std::endl;

	///Converting elements
	if (this->status != 0)
	{
		//for (int i = this->pjmesh_num_original_elems; i < this->num_elems; i++)
		for (int i = 0; i < this->num_elems; i++)
		{
			Data::Vertex *vs[4];

			//if (debugrank == 0) std::cout << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 0] = " << this->elems[i*4 + 0] << ", ok = " << (this->elems[i*4 + 0] < this->vMap.size()) << std::endl;
			//if (debugrank == 0) std::cout << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 1] = " << this->elems[i*4 + 1] << ", ok = " << (this->elems[i*4 + 1] < this->vMap.size()) << std::endl;
			//if (debugrank == 0) std::cout << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 2] = " << this->elems[i*4 + 2] << ", ok = " << (this->elems[i*4 + 2] < this->vMap.size()) << std::endl;
			//if (debugrank == 0) std::cout << "this->vMap size = " << this->vMap.size() << ", this->elems[i*4 + 3] = " << this->elems[i*4 + 3] << ", ok = " << (this->elems[i*4 + 3] < this->vMap.size()) << std::endl;

			vs[0] = this->vMap[this->elems[i*4 + 0]];
			vs[1] = this->vMap[this->elems[i*4 + 1]];
			vs[2] = this->vMap[this->elems[i*4 + 2]];
			vs[3] = this->vMap[this->elems[i*4 + 3]];

			MJMesh::Element3D *el = NULL;

			if ((!fromMesh) || (eBuffer.empty()))
			{
				el = new MJMesh::Element3D(
					static_cast<MJMesh::Vertex *>(vs[0]),
					static_cast<MJMesh::Vertex *>(vs[1]),
					static_cast<MJMesh::Vertex *>(vs[2]),
					static_cast<MJMesh::Vertex *>(vs[3]),
					this->idManager->next(2));

				/*std::stringstream str;
				str << "process " << MPI::COMM_WORLD.Get_rank() << ", mesh generator, creating el " << el->text() << std::endl;
				std::cout << str.str();*/
			}
			else
			{
				el = static_cast<MJMesh::Element3D *>(eBuffer.front());

				eBuffer.pop_front();

				el->setVertices(
					static_cast<MJMesh::Vertex *>(vs[0]),
					static_cast<MJMesh::Vertex *>(vs[1]),
					static_cast<MJMesh::Vertex *>(vs[2]),
					static_cast<MJMesh::Vertex *>(vs[3]));

				/*std::stringstream str;
				str << "process " << MPI::COMM_WORLD.Get_rank() << ", mesh generator, replacing el " << el->text() << std::endl;
				std::cout << str.str();*/
			}

			this->mesh->add(el);

			Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(el);

			/*//if (debugrank == 0) std::cout << "analyzing element " << el->text() << std::endl;

			for (UInt k = 0; k < 4; k++)
			{
				Data::Vertex *v = vs[k];

				Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(v);

				//if (debugrank == 0) std::cout << "analyzing vertex " << v->text() << ", k = " << k << ", adjs.size() = " << adjs.size() << std::endl;

				while (!adjs.empty())
				{
					Data::FrontElement *fel = adjs.front();

					adjs.pop_front();

					//if (debugrank == 0) std::cout << "analyzing fel " << fel->text() << std::endl;

					if (std::find(fels.begin(), fels.end(), fel) != fels.end())
					{
						continue;
					}

					UInt count = 0;

					for (UInt j = 1; j < 4; j++)
					{
						//if (debugrank == 0) std::cout << "j = " << j << ", k+j = " << k + j << ", (k + j)%4 = " << (k + j)%4 << std::endl;

						if (fel->have(vs[(k + j)%4]))
						{
							count++;
						}
					}

					//if (debugrank == 0) std::cout << "count = " << count << std::endl;

					if (count >= 2)
					{
						fels.push_back(fel);
					}
				}
			}*/

			//if (debugrank == 0) std::cout << "fels.size() = " << fels.size() << std::endl;

			while (!fels.empty())
			{
				MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

				fels.pop_front();

				fel->add(el);
			}

			//this->mesh->add(el);
		}

		while (!eBuffer.empty())
		{
			Data::Element *e = eBuffer.front();

			eBuffer.pop_front();

			e->setVertex(0, NULL);
			e->setVertex(1, NULL);
			e->setVertex(2, NULL);
			e->setVertex(3, NULL);

			delete e;
		}
	}

	if (this->num_elems != 0)
	{
		Msh3DFree(this->elems);
		this->elems = NULL;

		this->num_elems = 0;
	}

	this->vMap.clear();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeFromJmeshConversion = Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return true;
}
