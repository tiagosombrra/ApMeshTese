#ifndef _TECGRAF_MSHSURF_MAIN_DRIVE_H_
#define _TECGRAF_MSHSURF_MAIN_DRIVE_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF_MSH_SURF

#include "MJMesh/Definitions.h"
#include "Data/Mesh.h"
#include "Performer/MainDrive.h"
#include "PMJMesh2/Definitions.h"

#include "mshsurf3d.h"

namespace TecGraf
{
	namespace MshSurf
	{
		class MainDrive : public Performer::MainDrive
		{
        friend
            class PMJMesh2::APrioriParallelMeshGenerator;

		public:

			MainDrive();
			virtual ~MainDrive();

			virtual void setAlgorithm(enum Algorithms algorithm);

			virtual void setAnalyticalMaxElementSize(Real analyticalMaxElementSize);
			virtual Real getAnalyticalMaxElementSize() const;

			virtual void setAnalyticalComputeCurvature(bool analyticalComputeCurvature);
			virtual bool isAnalyticalComputeCurvature() const;

			virtual void setSupportMaxElementSize(Real supportMaxElementSize);
			virtual Real getSupportMaxElementSize() const;

			virtual void setSupportMaxNumberElements(Int supportMaxNumberElements);
			virtual Int getSupportMaxNumberElements() const;

			virtual void setSupportComputeCurvature(bool supportComputeCurvature);
			virtual bool isSupportComputeCurvature() const;

			virtual void setSupportInvertMesh(bool supportInvertMesh);
			virtual bool isSupportInvertMesh() const;

			virtual void setSupportMesh(Data::Mesh *supportMesh);
			virtual Data::Mesh *getSupportMesh() const;

			virtual void setSuportLoadingSupportMesh(bool supportLoadingSupportMesh);
			virtual bool isSupportLoadingSupportMesh() const;

			virtual void setMessageFunction(Surf3DMessFunc *messageFunction);

			virtual void setIdManager(Performer::IdManager *idManager);

//#if USE_GUI
			virtual void setBoundary(MJMesh::Boundary *boundary);
			//virtual void setTree(Data::BoxTree *tree);
			virtual void setMesh(Data::Mesh *mesh);
//#endif //#if USE_GUI

			virtual const Data::Boundary *getBoundary() const;
			//virtual const Data::BoxTree *getTree() const;
			virtual const Data::Mesh *getMesh() const;

#if USE_GUI
			//virtual const Data::Front *getFront() const;
#endif //#if USE_GUI

			virtual void clear();

			virtual bool loadFile();
			virtual bool saveFile() const;

			virtual std::string qualityString() const;

			virtual void findQuality();

			virtual void execute();

		protected:

			virtual bool loadInputFile(const std::string &inputFileName);
			virtual bool saveInputFile(const std::string &outputFileName) const;

			virtual bool loadOutputFile(const std::string &inputFileName);
			virtual bool saveOutputFile(const std::string &outputFileName) const;

		protected:

			/*virtual bool convertToMsh2D(double *&pts, int &nPts, Data::VertexVector &map, bool clockWise) const;
			virtual bool convertToMsh2D(double *&pts, int &nLoops, int *&loops, Data::VertexVector &map, bool clockWise) const;
			virtual bool convertToMsh2D(double *&pts, int &nPts, int *&edges, int &nEdges, Data::VertexVector &map, bool clockWise) const;
			virtual bool convertToMesh(int nPts, double *pts, int nElems, int *elems, int type,
				Data::VertexVector &map, bool clockWise = true, bool rebuildMap = false, bool checkBoundary = false);*/

            virtual bool convertToMeshSurf(int &nPts, double *&pts, int &nElems, int *&elems, const Data::Mesh *mesh, int type, bool clockWise = true) const;
            virtual bool convertToBoundarySurf(int &nPts, double *&pts, int &nBoundEdges, int &nInterEdges, int *&edges,
                Data::VertexVector &map, const Data::Boundary *boundary, bool clockWise = true) const;
            virtual bool convertToMesh(int nPts, double *pts, int nElems, int *elems, int type,
                Data::VertexVector &map, bool clockWise = true, bool rebuildMap = false, bool checkBoundary = false);

		protected:

			virtual bool executeBoundaryBuilder(bool del);

			virtual bool executeAnalytical();
			virtual bool executeSupport();

		protected:

			virtual Performer::IdManager *makeIdManager(const Performer::IdManager *idManager) const;
			virtual MJMesh::BoundaryBuilder *makeBoundaryBuilder() const;

		protected:

			enum Algorithms algorithm;

			Real analyticalMaxElementSize;
			bool analyticalComputeCurvature;

			Real supportMaxElementSize;
			Int supportMaxNumberElements;
			bool supportComputeCurvature;
			bool supportInvertMesh;
			Data::Mesh *supportMesh;
			bool supportLoadingSupportMesh;

			Surf3DMessFunc *messageFunction;

			MJMesh::Boundary *boundary;
			//Data::BoxTree *tree;
			Data::Mesh *mesh;

#if USE_GUI
			//MJMesh::Front *front;
#endif //#if USE_GUI

			Data::Mesh::Quality quality;

#if USE_TIME_MEASUREMENTS
			Real runtime;
#endif //#if USE_TIME_MEASUREMENTS

		protected:

			Performer::IdManager *idManager;
			MJMesh::BoundaryBuilder *boundaryBuilder;

		};
	}
}

#endif //#if USE_TECGRAF_MSH_SURF

#endif //#ifndef _TECGRAF_MSHSURF_MAIN_DRIVE_H_
