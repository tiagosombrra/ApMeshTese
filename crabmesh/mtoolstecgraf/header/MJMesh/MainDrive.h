#ifndef _MJMESH_MAIN_DRIVE_H_
#define _MJMESH_MAIN_DRIVE_H_

#include "MJMesh/Definitions.h"
#include "Performer/MainDrive.h"
#include "Data/Mesh.h"

namespace MJMesh
{
	class MainDrive : virtual public Performer::MainDrive
	{
	public:

		MainDrive();
		virtual ~MainDrive();

		virtual UInt getDimension() const;

		virtual void setBoundarySorted(bool boundarySorted);
		virtual bool isBoundarySorted() const;

#if USE_NUMERICAL
		virtual void setHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool isHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

		virtual void setTreeFactor(Real treeFactor);
		virtual Real getTreeFactor() const;

		virtual void setUsingUniformRefinement(bool usingUniformRefinement);
		virtual bool isUsingUniformRefinement() const;

		virtual void setUsingSearchTrees(bool usingSearchTrees);
		virtual bool isUsingSearchTrees() const;

		virtual void setNumImproves(UInt numImproves);
		virtual UInt getNumImproves() const;

		virtual void setSmoothingFactor(Real smoothingFactor);
		virtual Real getSmoothingFactor() const;

		virtual void setMeshSorted(bool meshSorted);
		virtual bool isMeshSorted() const;

		virtual void setLocalBackTrackingEnabled(bool localBackTrackingEnabled);
		virtual bool isLocalBackTrackingEnabled() const;

		virtual void setMaxBackTracks(UInt maxBackTracks);
		virtual UInt getMaxBackTracks() const;

		virtual void setDimension(UInt dimension);
		virtual BoundaryBuilder *getBoundaryBuilder(bool make = false);

#if USE_GUI
		virtual void setBoundary(MJMesh::Boundary *boundary);
		virtual void setTree(Data::BoxTree *tree);
		virtual void setFront(MJMesh::Front *front);
		virtual void setMesh(MJMesh::Mesh *mesh);
#endif //#if USE_GUI

		virtual const Data::Boundary *getBoundary() const;
		virtual const Data::BoxTree *getTree() const;
		virtual const MJMesh::Front *getFront() const;
		virtual const MJMesh::Mesh *getMesh() const;

#if USE_TIME_MEASUREMENTS
		virtual Real getRuntime() const;
#endif //#if USE_TIME_MEASUREMENTS

		virtual std::string qualityString() const;

		virtual void findQuality();

		virtual Data::UI2Tuple measure() const;

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &outputFileName) const;

		virtual bool loadOutputFile(const std::string &inputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		//Adding vertices to the front
		virtual void add(ULInt id, Real x, Real y);
		virtual void add(ULInt id, Real x, Real y, Real z);
		//Adding edges/triangles/quads (disabled by default) to the front
		virtual void add(ULInt id, ULInt id1, ULInt id2);
		virtual void add(ULInt id, ULInt id1, ULInt id2, ULInt id3);
#if USE_QUADS
		virtual void add(ULInt id, ULInt id1, ULInt id2, ULInt id3, ULInt id4);
#endif //#if USE_QUADS

		//Iterating through all vertices
		virtual Data::Mesh::VertexIterator vBegin();
		virtual Data::Mesh::ConstVertexIterator vBegin() const;
		virtual Data::Mesh::VertexIterator vEnd();
		virtual Data::Mesh::ConstVertexIterator vEnd() const;

		//Iterating through internal vertices
#if USE_EXPERIMENTAL_MESH
		virtual Data::Mesh::InnerVertexIterator ivBegin();
		virtual Data::Mesh::ConstInnerVertexIterator ivBegin() const;
		virtual Data::Mesh::InnerVertexIterator ivEnd();
		virtual Data::Mesh::ConstInnerVertexIterator ivEnd() const;
#else
		virtual Data::Mesh::VertexIterator ivBegin();
		virtual Data::Mesh::ConstVertexIterator ivBegin() const;
		virtual Data::Mesh::VertexIterator ivEnd();
		virtual Data::Mesh::ConstVertexIterator ivEnd() const;
#endif //#if USE_EXPERIMENTAL_MESH

		//Iterating through elements
		virtual Data::Mesh::ElementIterator eBegin();
		virtual Data::Mesh::ConstElementIterator eBegin() const;
		virtual Data::Mesh::ElementIterator eEnd();
		virtual Data::Mesh::ConstElementIterator eEnd() const;

		virtual void execute();

		virtual void clear();

	public:

		enum
		{
			BOUNDARY_BUILT = INIT + 1,
#if USE_NUMERICAL
			HOTELLING_TRANSFORM_APPLIED,
#endif //#if USE_NUMERICAL
			TREE_GENERATED,
			TREE_MIN_REFINED,
			TREE_NEIGHBOR_REFINED,
			FRONT_GEOMETRY_ADVANCED,
			FRONT_TOPOLOGY_ADVANCED,
			FRONT_BACK_TRACKING_ADVANCED,
			MESH_IMPROVED,
#if USE_NUMERICAL
			HOTELLING_BACK_TRANSFORM_APPLIED,
#endif //#if USE_NUMERICAL

			ERROR_DIMENSION = ERROR + 1,
			ERROR_POINT_OUT_OF_TREE,
			ERROR_MESH_NOT_GENERATED_TOPOLOGY,
			ERROR_MESH_NOT_GENERATED_BACK_TRACKING,
		};

		static const Performer::Status errorDimension;
		static const Performer::Status errorPointOutOfTree;
		static const Performer::Status errorMeshNotGeneratedTopology;
		static const Performer::Status errorMeshNotGeneratedBackTracking;

		static const Performer::Status boundaryBuilt;
#if USE_NUMERICAL
		static const Performer::Status hotellingTransformApplied;
#endif //#if USE_NUMERICAL
		static const Performer::Status treeGenerated;
		static const Performer::Status treeMinRefined;
		static const Performer::Status treeNeighborRefined;
		static const Performer::Status frontGeometryAdvanced;
		static const Performer::Status frontTopologyAdvanced;
		static const Performer::Status frontBackTrackingAdvanced;
		static const Performer::Status meshImproved;
#if USE_NUMERICAL
		static const Performer::Status hotellingBackTransformApplied;
#endif //#if USE_NUMERICAL

	protected:

		virtual bool loadJMeshFile(const std::string &fileName);
		virtual bool loadDMshFile(const std::string &fileName);
		virtual bool loadJMshFile(const std::string &fileName);
		virtual bool loadWMshFile(const std::string &fileName);
		virtual bool loadGmshMshFile(const std::string &fileName);
		virtual bool loadTxtFile(const std::string &fileName);
		virtual bool loadNeutralFile(const std::string &fileName);
		virtual bool loadSTLFile(const std::string &fileName);
		virtual bool loadVTKFile(const std::string &fileName);
		virtual bool loadCDBFile(const std::string &fileName);
		virtual bool loadOFFFile(const std::string &fileName);
		virtual bool loadAbaqusFile(const std::string &fileName);
#if USE_TINYXML
		virtual bool loadXMLFile(const std::string &fileName);
#endif //#if USE_TINYXML
        virtual bool loadInputGenFile(const std::string &fileName);
        virtual bool loadOutputGenFile(const std::string &fileName);

		virtual bool saveJMeshFile(const std::string &fileName) const;
		virtual bool saveDMshFile(const std::string &fileName) const;
		virtual bool saveJMshFile(const std::string &fileName) const;
		virtual bool saveWMshFile(const std::string &fileName) const;
		virtual bool saveAbaqusFile(const std::string &fileName) const;
		virtual bool saveTxtFile(const std::string &fileName) const;
		virtual bool saveNeutralFile(const std::string &fileName) const;
		virtual bool saveInputGenFile(const std::string &fileName) const;
        virtual bool saveOutputGenFile(const std::string &fileName) const;

	protected:

		virtual bool executeInit();
		virtual bool executeBoundaryBuilder(bool del);
#if USE_NUMERICAL
		virtual bool executeHotellingTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeTreeGenerator(bool del);
		virtual bool executeTreeMinRefiner(bool del);
		virtual bool executeTreeNeighborRefiner(bool del);
		virtual bool executeFrontGeometryAdvancer();
		virtual bool executeFrontTopologyAdvancer();
		virtual bool executeFrontBackTrackingAdvancer();
		virtual bool executeMeshImprover();
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeFinalize();

		virtual Performer::IdManager *makeIdManager(const Performer::IdManager *idManager) const;
		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
#if USE_NUMERICAL
		virtual MJMesh::HotellingTransformer *makeHotellingTransformer() const;
#endif //#if USE_NUMERICAL
		virtual Performer::TreeGenerator *makeTreeGenerator() const;
		virtual MJMesh::MinTreeRefiner *makeTreeMinRefiner() const;
		virtual MJMesh::NeighborTreeRefiner *makeTreeNeighborRefiner() const;
		virtual MJMesh::GeometryFrontAdvancer *makeFrontGeometryAdvancer() const;
		virtual MJMesh::TopologyFrontAdvancer *makeFrontTopologyAdvancer() const;
		virtual MJMesh::BackTrackingFrontAdvancer *makeFrontBackTrackingAdvancer() const;
		virtual MJMesh::MeshImprover *makeMeshImprover() const;

    protected:

        static const UInt QUALITY_RANGE;

	protected:

		Performer::IdManager *idManager;

		UInt dimension;

		bool boundarySorted;
		Real treeFactor;
#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		bool usingUniformRefinement;
		bool usingSearchTrees;
		UInt numImproves;
		Real smoothingFactor;
		bool meshSorted;
		bool localBackTrackingEnabled;
		UInt maxBackTracks;

		Data::Mesh::Quality quality;

		MJMesh::Boundary *boundary;
		Data::BoxTree *tree;
		Data::GeometricSearchTree *vertexSearchTree;
		Data::GeometricSearchTree *frontElementSearchTree;
		MJMesh::Front *front;
		MJMesh::Mesh *mesh;

		Performer::BoundaryBuilder *boundaryBuilder;

#if USE_NUMERICAL
		Performer::HotellingTransformer *hotellingTransformer;
#endif //#if USE_NUMERICAL

		Performer::TreeGenerator *treeGenerator;
		MJMesh::MinTreeRefiner *minTreeRefiner;
		MJMesh::NeighborTreeRefiner *neighborRefiner;

		MJMesh::GeometryFrontAdvancer *geometryAdvancer;
		MJMesh::TopologyFrontAdvancer *topologyAdvancer;
		MJMesh::BackTrackingFrontAdvancer *backTracking;

		MJMesh::MeshImprover *improver;

#if USE_TIME_MEASUREMENTS
		Real runtime;
#endif //#if USE_TIME_MEASUREMENTS

        UInt minTreeLevel;
	};
}

#endif //#ifndef _MJMESH_MAIN_DRIVE_H_
