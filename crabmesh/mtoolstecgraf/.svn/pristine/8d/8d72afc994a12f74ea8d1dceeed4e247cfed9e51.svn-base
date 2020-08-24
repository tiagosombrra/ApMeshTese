#include "Jmesh/MainDrive.h"

#include <cstring>

#include "Data/State.h"
#include "Data/Point3D.h"
#include "Data/OctTree.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/HotellingTransformer.h"
#include "Jmesh/TreeGenerator.h"

#include "JmeshMainDrive.c"

using namespace Jmesh;

const UInt Jmesh::MainDrive::QUALITY_RANGE = 10;

Jmesh::MainDrive::MainDrive() :
	Performer::MainDrive()
{
	this->setVer(ADVAN);
#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingUniformRefinement(true);
	this->Performer::MainDrive::status = this->notInit;

	max = -1;
	offset = 0;
	tnodes = NULL;
	rnodes = NULL;
	num_gen_nodes = 0;
	tgenerated_nodes = NULL;
	out = 1;
	file = 0;

	nodes = NULL;
	faces = NULL;
	generated_nodes = NULL;
	elems = NULL;
	text = NULL;
	fp = NULL;

	internal = 0;
	fromone = 0;
	version = 0;
	format = 0;
	num_node = 0;
	num_face = 0;
	i = 0;
	j = 0;
	k = 0;
	factor = 0.0;
	tol = 0.0;
	rnode = 0;
	num_elems = 0;
	status = 0;
	number = 0;
	init_time = 0;
	tot_time = 0;
	init_gbl_time = 0;
	tot_gbl_time = 0;

	this->boundary = NULL;
	this->tree = NULL;
	this->mesh = NULL;

#if USE_TIME_MEASUREMENTS
	this->runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	this->treeGenerator = NULL;
#if USE_NUMERICAL
	this->hotellingTransformer = NULL;
#endif //#if USE_NUMERICAL
	this->boundaryBuilder = NULL;
}

Jmesh::MainDrive::~MainDrive()
{
	this->clear();
}

void Jmesh::MainDrive::setVer(enum Versions ver)
{
	this->ver = ver;
}

#if USE_NUMERICAL
void Jmesh::MainDrive::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
	this->hotellingTransformEnabled = hotellingTransformEnabled;
}

bool Jmesh::MainDrive::isHotellingTransformEnabled() const
{
	return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void Jmesh::MainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
	this->usingUniformRefinement = usingUniformRefinement;
}

bool Jmesh::MainDrive::isUsingUniformRefinement() const
{
	return this->usingUniformRefinement;
}

#if USE_GUI
void Jmesh::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;

	if (this->boundaryBuilder)
	{
		this->boundaryBuilder->setBoundary(boundary);
	}
}

void Jmesh::MainDrive::setTree(Data::BoxTree *tree)
{
	this->tree = tree;

	if (this->treeGenerator)
	{
		this->treeGenerator->setTree(tree);
	}
}

void Jmesh::MainDrive::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}
#endif //#if USE_GUI

const Data::Boundary *Jmesh::MainDrive::getBoundary() const
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

const Data::BoxTree *Jmesh::MainDrive::getTree() const
{
	return this->tree;
}

const Data::Mesh *Jmesh::MainDrive::getMesh() const
{
	return this->mesh;
}

std::string Jmesh::MainDrive::qualityString() const
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

void Jmesh::MainDrive::findQuality()
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
		this->quality.findQuality(this->mesh, this->QUALITY_RANGE, &scale[0][0]);
#else
		this->quality.findQuality(this->mesh, this->QUALITY_RANGE);
#endif //#if USE_GUI
	}
}

void Jmesh::MainDrive::clear()
{
	this->Performer::MainDrive::status = this->notInit;

	if (nodes != NULL)
	{
		for (int i = 0; i < num_node; i++)
		{
			free(nodes[i]);
		}

		free(nodes);

		nodes = NULL;
	}

	if (tnodes != NULL)
	{
		free(tnodes);

		tnodes = NULL;
	}

	if (rnodes != NULL)
	{
		free(rnodes);

		rnodes = NULL;
	}

	if (faces != NULL)
	{
		for (int i = 0; i < num_face; i++)
		{
			free(faces[i]);
		}

		free(faces);

		faces = NULL;
	}

	if (generated_nodes != NULL)
	{
		free(generated_nodes);

		generated_nodes = NULL;
	}

	if (tgenerated_nodes != NULL)
	{
		free(generated_nodes);

		tgenerated_nodes = NULL;
	}

	if (elems != NULL)
	{
		free(elems);

		elems = NULL;
	}

	if (text != NULL)
	{
		free(text);

		text = NULL;
	}

	if (fp != NULL)
	{
		fclose(fp);

		fp = NULL;
	}

	max = -1;
	offset = 0;
	num_gen_nodes = 0;
	out = 1;
	file = 0;
	internal = 0;
	fromone = 0;
	version = 0;
	format = 0;
	num_node = 0;
	num_face = 0;
	i = 0;
	j = 0;
	k = 0;
	factor = 0.0;
	tol = 0.0;
	rnode = 0;
	num_elems = 0;
	status = 0;
	number = 0;
	init_time = 0;
	tot_time = 0;
	init_gbl_time = 0;
	tot_gbl_time = 0;

	this->setVer(ADVAN);
#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingUniformRefinement(true);

	if (this->tree)
	{
		delete this->tree;

		this->tree = NULL;
	}

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

#if USE_NUMERICAL
	if (this->hotellingTransformer)
    {
        delete this->hotellingTransformer;

        this->hotellingTransformer = NULL;
    }
#endif //#if USE_NUMERICAL

	if (this->treeGenerator)
	{
		delete this->treeGenerator;

		this->treeGenerator = NULL;
	}
}

bool Jmesh::MainDrive::loadFile()
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

	if ((extension == ".dmsh") || (extension == ".jmsh") || (extension == ".wmsh"))
	{
		return this->loadInputFile(this->inputFileName);
	}

	return false;
}

bool Jmesh::MainDrive::saveFile() const
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

	if (extension == ".dat")
	{
		return this->saveOutputFile(this->outputFileName);
	}

	return false;
}

void teste(int advanced, int total)
{
    std::cout << advanced << " , " << total << " - " << total - advanced << " / " << 100.0*static_cast<Real>(advanced)/static_cast<Real>(total) << " %" << std::endl;
}

void Jmesh::MainDrive::execute()
{
	if ((this->Performer::MainDrive::status.getType() == Performer::MainDrive::success.getType()) ||
		(this->Performer::MainDrive::status.getType() >= Performer::MainDrive::error.getType()))
	{
		return;
	}

	this->Performer::MainDrive::status = this->init;

	if (num_face == 0)
	{
		this->Performer::MainDrive::status = this->notInit;

		return;
	}

	this->executeBoundaryBuilder(true);

#if USE_NUMERICAL
	if (this->hotellingTransformEnabled)
    {
        this->executeHotellingTransform();
    }
#endif //#if USE_NUMERICAL

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

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
    status = Msh3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems,
                              this->usingUniformRefinement ? 1 : 0, teste ) ; //two last parameters added by markos

    if (status == 0) {
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

#if USE_TIME_MEASUREMENTS
		Real end = Data::time();
		this->runtime += end - start;
		std::cout << "Runtime = " << this->runtime << " seconds" << std::endl;
#endif //#if USE_TIME_MEASUREMENTS

		this->Performer::MainDrive::status = this->error;

		return;
    }
#else
    status = Msh3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems,
                              this->usingUniformRefinement ? 1 : 0, teste ) ; //two last parameters added by markos
#endif

	if (status == 0)
	{
#if USE_TIME_MEASUREMENTS
		Real end = Data::time();
		this->runtime += end - start;
		std::cout << "Runtime = " << this->runtime << " seconds" << std::endl;
#endif //#if USE_TIME_MEASUREMENTS

		this->Performer::MainDrive::status = this->error;

		return;
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

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->runtime += end - start;
	std::cout << "Runtime = " << this->runtime << " seconds" << std::endl;
#endif //#if USE_TIME_MEASUREMENTS

	this->executeTreeGenerator(true);

	this->executeMeshBuilder();

#if USE_NUMERICAL
	if (this->hotellingTransformEnabled)
    {
        this->executeHotellingBackTransform();
    }
#endif //#if USE_NUMERICAL

	this->executeFinalize();

	this->Performer::MainDrive::status = this->success;
}

bool Jmesh::MainDrive::loadInputFile(const std::string &inputFileName)
{
	this->clear();

	this->boundaryBuilder = this->makeBoundaryBuilder();

	int argc = 3;
	char *argv[argc];

	//std::string c = "";

	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	std::string ext = (extension == ".dmsh") ? "-dmesh_fmt" :
					  ((extension == ".jmsh") ? "-jmesh_fmt" :
					  ((extension == ".wmsh") ? "-wmesh_fmt" : ""));

	std::string ver = (this->ver == ADVAN) ? "-advan_ver" :
					  ((this->ver == SMALL) ? "-small_ver" : "");

	argv[0] = NULL;//new char[c.size()];
	argv[1] = new char[ext.size()];
	argv[2] = new char[ver.size()];

	//strcpy(argv[0], c.c_str());
	strcpy(argv[1], ext.c_str());
	strcpy(argv[2], ver.c_str());

	FILE *f = freopen(inputFileName.c_str(), "r", stdin);

	if (!f)
	{
		//delete [] argv[0];
		delete [] argv[1];
		delete [] argv[2];

		return false;
	}

	setlocale(LC_ALL, "C");

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
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\n" );

	version  = ADVAN_VERSION ;
    format   = 0 ;
    factor   = 0.4 ;
    tol      = 0.001 ;
    internal = 1 ;
    fromone  = 1 ;

    /* Read the format, if given */

    for( i = 1; i < argc; i++ )
    {
     /* Read the format */
     if (strcmp("-dmesh_fmt",argv[i]) == 0)
      format = 0 ;
     if (strcmp("-jmesh_fmt",argv[i]) == 0)
      format = 1 ;
     if (strcmp("-wmesh_fmt",argv[i]) == 0)
      format = 2 ;
     /* Read the version */
     if (strcmp("-advan_ver",argv[i]) == 0)
      version = ADVAN_VERSION ;
     if (strcmp("-small_ver",argv[i]) == 0)
      version = SMALL_VERSION ;
    }

    /* Read the input data */

    switch( format )
    {
     case 0: /* Default  format */
      scanf( "%lf", &factor ) ;
      scanf( "%lf", &tol ) ;
      scanf( "%d",  &internal ) ;
      scanf( "%d",  &fromone ) ;

      scanf( "%d", &num_node ) ;
      nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ ) {
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       tnodes[i] = i;
      }

      scanf( "%d", &num_face ) ;
      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d %d %d", &faces[i][0], &faces[i][1], &faces[i][2] ) ;
       if( fromone )
       {
        faces[i][0]--;
        faces[i][1]--;
        faces[i][2]--;
       }
      }
      break;

     case 1: /* Joaquim's format */
      scanf( "%lf", &factor ) ;
      scanf( "%lf", &tol ) ;
      scanf( "%d",  &internal ) ;
      scanf( "%d",  &fromone ) ;

#if OLD_LOOK
      scanf( "%d", &num_node ) ;
      nodes  = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      scanf( "%d", &num_face ) ;
      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d %d %d", &faces[i][0], &faces[i][1], &faces[i][2] ) ;
       for( k = 0; k < 3; k++ ) {
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          faces[i][k] = j ;
          break ;
         }
        }
       }
      }
#else
      scanf( "%d", &num_node ) ;
      nodes  = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;            /* Normal look-up table */
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      rnode = max+1 ;
      rnodes = (int *)Msh3DCalloc( rnode, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       rnodes[tnodes[i]] = i ;
      }

      scanf( "%d", &num_face ) ;
      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d %d %d", &faces[i][0], &faces[i][1], &faces[i][2] ) ;
       for( k = 0; k < 3; k++ ) {
#if OLD_DEBU
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          rnode = j ;
          break ;
         }
        }
	if (rnode != rnodes[faces[i][k]])
         fprintf (stderr, "Error on look-up table process\n") ;
#endif
	faces[i][k] = rnodes[faces[i][k]] ;
       }
      }
#endif
      break;

     case 2: /* Wash's format */
      scanf( "%d %d", &num_node, &num_face ) ;

#if OLD_LOOK
      nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d", &j ) ;
       scanf( "%d %d %d", &faces[i][0], &faces[i][2], &faces[i][1] ) ;
       for( k = 0; k < 3; k++ ) {
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          faces[i][k] = j ;
          break ;
         }
        }
       }
      }
#else
      nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;            /* Normal look-up table */
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      rnode = max+1 ;
      rnodes = (int *)Msh3DCalloc( rnode, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       rnodes[tnodes[i]] = i ;
      }

      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d", &j ) ;
       scanf( "%d %d %d", &faces[i][0], &faces[i][2], &faces[i][1] ) ;
       for( k = 0; k < 3; k++ ) {
#if OLD_DEBU
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          rnode = j ;
          break ;
         }
        }
	if (rnode != rnodes[faces[i][k]])
         fprintf (stderr, "Error on look-up table process\n") ;
#endif
	faces[i][k] = rnodes[faces[i][k]] ;
       }
      }
#endif
      break;

     default:
      break;
    }

    setlocale(LC_ALL, "");

    //delete [] argv[0];
	delete [] argv[1];
	delete [] argv[2];

    fclose(stdin);

    this->fillBoundary();

    return true;
}

bool Jmesh::MainDrive::saveInputFile(const std::string &/*outputFileName*/) const
{
	return false;
}

bool Jmesh::MainDrive::loadOutputFile(const std::string &/*inputFileName*/)
{
	return false;
}

bool Jmesh::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
	freopen(outputFileName.c_str(), "w", stdout);

	setlocale(LC_ALL, "C");

	if( out )
    {
     if( status )
     {
      /* Open file */

      if( file )
       fp = fopen ("jmesh.jpos", "w");
      else
       fp = stdout;

      /* Print data */

      switch( format )
      {
       case 0: /* Default  format */
        fprintf (fp,"%%HEADER\n");
        fprintf (fp,"File created by Jmesh \n");
        fprintf (fp,"\n");
        fprintf (fp,"%%NODE\n%d\n\n",num_node+num_gen_nodes);
        fprintf (fp,"%%NODE.COORD\n%d\n",num_node+num_gen_nodes);
        for (i = 0; i < (num_node+num_gen_nodes); i++)
        {
         if( i < num_node )
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", i+1,
                                          nodes[i][0],
   					  nodes[i][1],
   					  nodes[i][2]);
         }
         else
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", i+1,
                                          generated_nodes[(i-num_node)*3+0],
   	 				  generated_nodes[(i-num_node)*3+1],
 					  generated_nodes[(i-num_node)*3+2]);
         }
        }
        fprintf (fp,"\n");
	fprintf (fp,"%%INTEGRATION.ORDER\n1\n");
	fprintf (fp,"1\t1\t1\t1\t1\t1\t1\n\n");
        fprintf (fp,"%%ELEMENT\n%d\n\n",num_elems);
        fprintf (fp,"%%ELEMENT.TETR4\n%d\n",num_elems);
        for( i = 0; i < num_elems; i++)
        {
         fprintf (fp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, 1, 1,
            elems[i*4+1]+1, elems[i*4+0]+1, elems[i*4+2]+1, elems[i*4+3]+1);
        }
        fprintf (fp,"\n");
        fprintf(fp,"%%END\n");
        break;

       case 1: /* Joaquim's format */
        fprintf (fp,"%%HEADER\n");
        fprintf (fp,"File created by Jmesh \n");
        fprintf (fp,"\n");
        fprintf (fp,"%%NODE\n%d\n\n",num_node+num_gen_nodes);
        fprintf (fp,"%%NODE.COORD\n%d\n",num_node+num_gen_nodes);
        for (i = 0; i < (num_node+num_gen_nodes); i++)
        {
         if( i < num_node )
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tnodes[i],
                                          nodes[i][0],
   					  nodes[i][1],
   					  nodes[i][2]);
         }
         else
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tgenerated_nodes[i-num_node],
                                          generated_nodes[(i-num_node)*3+0],
   	 				  generated_nodes[(i-num_node)*3+1],
 					  generated_nodes[(i-num_node)*3+2]);
         }
        }
        fprintf (fp,"\n");
	fprintf (fp,"%%INTEGRATION.ORDER\n1\n");
	fprintf (fp,"1\t1\t1\t1\t1\t1\t1\n\n");
        fprintf (fp,"%%ELEMENT\n%d\n\n",num_elems);
        fprintf (fp,"%%ELEMENT.TETR4\n%d\n",num_elems);
        for( i = 0; i < num_elems; i++)
        {
         fprintf (fp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, 1, 1,
            elems[i*4+1], elems[i*4+0], elems[i*4+2], elems[i*4+3]);
        }
        fprintf (fp,"\n");
        fprintf(fp,"%%END\n");
        break;

       case 2: /* Wash's format */
        fprintf (fp,"%d\n",num_node+num_gen_nodes);
        for (i = 0; i < (num_node+num_gen_nodes); i++)
        {
         if( i < num_node )
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tnodes[i],
                                          nodes[i][0],
   					  nodes[i][1],
   					  nodes[i][2]);
         }
         else
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tgenerated_nodes[i-num_node],
                                          generated_nodes[(i-num_node)*3+0],
   	 				  generated_nodes[(i-num_node)*3+1],
 					  generated_nodes[(i-num_node)*3+2]);
         }
        }
        fprintf (fp,"%d\n",num_elems);
        for( i = 0; i < num_elems; i++)
        {
         fprintf (fp,"%d\t%d\t%d\t%d\t%d\n", i+1,
            elems[i*4+1], elems[i*4+0], elems[i*4+2], elems[i*4+3]);
        }
        fprintf (fp,"\n");
        break;

       default:
        break;
      }

      /* Close file */

      if( file )
       fclose(fp);
     }
    }
    else
    {
     fprintf( stderr, "Status, Num elems : %d %d\n", status, num_elems ) ;
     if ( !status ) {
         fprintf( stderr, "Error status returned \n" ) ;
         Msh3DError( &number, &text ) ;
         fprintf( stderr, "%d %s\n", number, text ) ;
     }
    }

    setlocale(LC_ALL, "");

    fclose(stdout);

    return true;
}

void Jmesh::MainDrive::fillBoundary()
{
	for (UInt i = 0; i < static_cast<UInt>(num_node); i++)
	{
		ULInt id = static_cast<ULInt>(i) + 1;

		static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, nodes[i][0], nodes[i][1], nodes[i][2]);
	}

	for (ULInt i = 0; i < static_cast<UInt>(num_face); i++)
	{
		ULInt id = static_cast<ULInt>(i) + 1;

		static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, static_cast<ULInt>(faces[i][0]) + 1, static_cast<ULInt>(faces[i][1]) + 1, static_cast<ULInt>(faces[i][2]) + 1);
	}
}

bool Jmesh::MainDrive::executeBoundaryBuilder(bool del)
{
	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setSorted(false);

	this->boundaryBuilder->execute();

	this->boundary = static_cast<MJMesh::Boundary *>(this->boundaryBuilder->getBoundary());

	//this->status = Jmesh::MainDrive::boundaryBuilt;

	if (del)
	{
		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

	return true;
}

#if USE_NUMERICAL
bool Jmesh::MainDrive::executeHotellingTransform()
{
    this->hotellingTransformer = this->makeHotellingTransformer();

    this->hotellingTransformer->execute();

    for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
         iter != this->boundary->vEnd(); iter++)
    {
        Data::Vertex *v = (*iter);
        Data::Point3D *p = static_cast<Data::Point3D *>((*iter)->getPoint());

        UInt i = static_cast<UInt>(v->getId() - 1);

        nodes[i][0] = p->getX();
        nodes[i][1] = p->getY();
        nodes[i][2] = p->getZ();
    }

    return true;
}
#endif //#if USE_NUMERICAL

bool Jmesh::MainDrive::executeTreeGenerator(bool del)
{
	this->treeGenerator = this->makeTreeGenerator();

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	this->treeGenerator->execute();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	this->tree = static_cast<Data::OctTree *>(this->treeGenerator->getTree());

	if (del)
	{
		delete this->treeGenerator;

		this->treeGenerator = NULL;
	}

	return true;
}

bool Jmesh::MainDrive::executeMeshBuilder()
{
	this->mesh = new MJMesh::Mesh();

	Data::VertexMap vertices;

	//Data::VertexList bverts = this->boundary->getVertices();

	UInt i = 0;

	//for (Data::VertexList::iterator iter = bverts.begin();
	for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
		 //iter != bverts.end(); iter++, i++)
		 iter != this->boundary->vEnd(); iter++, i++)
	{
		this->mesh->add((*iter), false);

		if (format == 0)
		{
			vertices[i + 1] = (*iter);
		}
		else
		{
			vertices[tnodes[i]] = (*iter);
		}
	}

	for (i = 0; i < static_cast<UInt>(num_gen_nodes); i++)
	{
		ULInt id = static_cast<ULInt>(num_node + i) + 1;

		Data::Point3D *p = new Data::Point3D(generated_nodes[i*3 + 0], generated_nodes[i*3 + 1], generated_nodes[i*3 + 2]);

		MJMesh::Vertex *v = new MJMesh::Vertex(p, id);

		this->mesh->add(v);

		if (format == 0)
		{
			vertices[num_node + i + 1] = v;
		}
		else
		{
			vertices[tgenerated_nodes[i]] = v;
		}
	}

	UInt idoffset = (format == 0) ? 1 : 0;

	for	(i = 0; i < static_cast<UInt>(num_elems); i++)
	{
		ULInt id = static_cast<ULInt>(i) + 1;

		MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(vertices[elems[i*4 + 0] + idoffset]);
		MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(vertices[elems[i*4 + 1] + idoffset]);
		MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(vertices[elems[i*4 + 2] + idoffset]);
		MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(vertices[elems[i*4 + 3] + idoffset]);

		MJMesh::Element3D *el = new MJMesh::Element3D(v1, v2, v3, v4, id);

		this->mesh->add(el);

		Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(el);

		while (!fels.empty())
		{
			MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

			fels.pop_front();

			fel->add(el);
		}
	}

	//Data::FrontElementList fels = this->boundary->getElements();

	//for (Data::FrontElementList::iterator iter = fels.begin();
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 //iter != fels.end(); iter++)
		 iter != this->boundary->eEnd(); iter++)
	{
		this->mesh->add((*iter));
	}

	return true;
}

#if USE_NUMERICAL
bool Jmesh::MainDrive::executeHotellingBackTransform()
{
    this->hotellingTransformer->setMesh(this->mesh);

    this->hotellingTransformer->executeBack();

    delete this->hotellingTransformer;

    this->hotellingTransformer = NULL;

    for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin();
         iter != this->mesh->vEnd(); iter++)
    {
        Data::Vertex *v = (*iter);
        Data::Point3D *p = static_cast<Data::Point3D *>((*iter)->getPoint());

        UInt i = static_cast<UInt>(v->getId() - 1);

        if (i < num_node)
        {
            nodes[i][0] = p->getX();
            nodes[i][1] = p->getY();
            nodes[i][2] = p->getZ();
        }
        else
        {
            i -= num_node;

            generated_nodes[i*3 + 0] = p->getX();
            generated_nodes[i*3 + 1] = p->getY();
            generated_nodes[i*3 + 2] = p->getZ();
        }
    }

    return true;
}
#endif //#if USE_NUMERICAL

bool Jmesh::MainDrive::executeFinalize()
{
	this->findQuality();

	return true;
}

Performer::BoundaryBuilder *Jmesh::MainDrive::makeBoundaryBuilder() const
{
	return new MJMesh::BoundaryBuilder(false);
}

#if USE_NUMERICAL
MJMesh::HotellingTransformer *Jmesh::MainDrive::makeHotellingTransformer() const
{
    MJMesh::HotellingTransformer *hotellingTransformer = new MJMesh::HotellingTransformer();

    hotellingTransformer->setBoundary(this->boundary);
    hotellingTransformer->setMode(Performer::CovarianceMatrixFinder::Mode::NORMAL);
    hotellingTransformer->setFrontElementCentroids(false);
    hotellingTransformer->setWeighted(false);

    return hotellingTransformer;
}
#endif //#if USE_NUMERICAL

Performer::TreeGenerator *Jmesh::MainDrive::makeTreeGenerator() const
{
	return new Jmesh::TreeGenerator(Msh3DTree());
}
