#ifndef _JMESH_MAIN_DRIVE_H_
#define _JMESH_MAIN_DRIVE_H_

#include "Jmesh/Definitions.h"
#include "Performer/MainDrive.h"
#include "Data/Mesh.h"

#include "Jmesh/JmeshMainDef3D.h"

namespace Jmesh
{
	class MainDrive : virtual public Performer::MainDrive
	{
	public:

		enum Versions
		{
			ADVAN = 1,
			SMALL,
		};

	public:

		MainDrive();
		virtual ~MainDrive();

		virtual void setVer(enum Versions ver);

#if USE_NUMERICAL
		virtual void setHotellingTransformEnabled(bool hotellingTransformEnabled);
		virtual bool isHotellingTransformEnabled() const;
#endif //#if USE_NUMERICAL

		virtual void setUsingUniformRefinement(bool usingUniformRefinement);
		virtual bool isUsingUniformRefinement() const;

#if USE_GUI
		virtual void setBoundary(MJMesh::Boundary *boundary);
		virtual void setTree(Data::BoxTree *tree);
		virtual void setMesh(Data::Mesh *mesh);
#endif //#if USE_GUI

		virtual const Data::Boundary *getBoundary() const;
		virtual const Data::BoxTree *getTree() const;
		virtual const Data::Mesh *getMesh() const;

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

		virtual void fillBoundary();

		virtual bool executeBoundaryBuilder(bool del);
#if USE_NUMERICAL
		virtual bool executeHotellingTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeTreeGenerator(bool del);
		//virtual bool executeTreeBuilder();
		virtual bool executeMeshBuilder();
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeFinalize();

		//virtual void buildCell(Msh3DOctTree root, Data::OctTreeCell *cell);

    protected:

        static const UInt QUALITY_RANGE;

	protected:

		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
#if USE_NUMERICAL
		virtual MJMesh::HotellingTransformer *makeHotellingTransformer() const;
#endif //#if USE_NUMERICAL
		virtual Performer::TreeGenerator *makeTreeGenerator() const;

	protected:

		enum Versions ver;

		MJMesh::Boundary *boundary;
		Data::BoxTree *tree;
		Data::Mesh *mesh;

#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		bool usingUniformRefinement;

		Data::Mesh::Quality quality;

		Performer::BoundaryBuilder *boundaryBuilder;
#if USE_NUMERICAL
		MJMesh::HotellingTransformer *hotellingTransformer;
#endif //#if USE_NUMERICAL
		Performer::TreeGenerator *treeGenerator;

#if USE_TIME_MEASUREMENTS
		Real runtime;
#endif //#if USE_TIME_MEASUREMENTS

	protected:

		int     internal, version;
		int     fromone, format;
		int     num_node, num_face;
		double  **nodes, factor, tol;
		int     offset;
		int     *tnodes;
		int     **faces;
		int     num_gen_nodes;
		double  *generated_nodes;
		int     *tgenerated_nodes;
		int     num_elems, *elems;
		int     status;

	private:

		int     max;
		mutable int 	i, j, k;
		int     *rnodes;
		int     rnode;
		int		out;
		mutable int 	number;
		mutable char    *text;
		int     file;
		mutable FILE    *fp;
		long    init_time, tot_time;
		long    init_gbl_time, tot_gbl_time;
	};
}

#endif //#ifndef _JMESH_MAIN_DRIVE_H_
