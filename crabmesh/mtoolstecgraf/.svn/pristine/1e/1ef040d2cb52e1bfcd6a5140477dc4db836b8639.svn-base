#ifndef _MTOOLS_MAIN_DRIVE_H_
#define _MTOOLS_MAIN_DRIVE_H_

#include "MTools/Definitions.h"

#if USE_MTOOLS

#include "Performer/MainDrive.h"
#include "Data/Mesh.h"

namespace MTools
{
	class MainDrive : virtual public Performer::MainDrive
	{
	public:

		MainDrive();
		virtual ~MainDrive();

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

#if USE_GUI
		virtual const Data::Front *getFront() const;
#endif //#if USE_GUI

		virtual void clear();

		virtual bool loadFile();
		virtual bool saveFile() const;

		virtual std::string qualityString() const;

		virtual void findQuality();
		virtual void removeHangingVertices();

		virtual void execute();

	protected:

		virtual bool loadInputFile(const std::string &inputFileName);
		virtual bool saveInputFile(const std::string &outputFileName) const;

		virtual bool loadOutputFile(const std::string &inputFileName);
		virtual bool saveOutputFile(const std::string &outputFileName) const;

		virtual bool fillMToolsBoundary();

		virtual bool executeBoundaryBuilder(bool del);
#if USE_NUMERICAL
		virtual bool executeHotellingTransform();
#endif //#if USE_NUMERICAL
		virtual bool executeTreeGenerator(bool del);
		virtual bool executeMeshBuilder();
#if USE_NUMERICAL
		virtual bool executeHotellingBackTransform();
#endif //#if USE_NUMERICAL
#if USE_GUI
		virtual void executeMeshBuilderInError();
#endif //#if USE_GUI
		virtual bool executeFinalize();

	protected:

		//virtual Performer::IdManager *makeIdManager(const Performer::IdManager *idManager) const;
		virtual Performer::BoundaryBuilder *makeBoundaryBuilder() const;
#if USE_NUMERICAL
		virtual MJMesh::HotellingTransformer *makeHotellingTransformer() const;
#endif //#if USE_NUMERICAL
		virtual Performer::TreeGenerator *makeTreeGenerator() const;
		virtual MTools::MshRegion3D *makeMshRegion() const;

    protected:

        static const UInt QUALITY_RANGE;

	protected:

		MJMesh::Boundary *boundary;
		Data::BoxTree *tree;
		Data::Mesh *mesh;

#if USE_GUI
		MJMesh::Front *front;
#endif //#if USE_GUI

		Data::Mesh::Quality quality;

		//Performer::IdManager *idManager;
		Performer::BoundaryBuilder *boundaryBuilder;
#if USE_NUMERICAL
		Performer::HotellingTransformer *hotellingTransformer;
#endif //#if USE_NUMERICAL
        Performer::TreeGenerator *treeGenerator;

#if USE_TIME_MEASUREMENTS
		Real runtime;
#endif //#if USE_TIME_MEASUREMENTS

	protected:

#if USE_NUMERICAL
		bool hotellingTransformEnabled;
#endif //#if USE_NUMERICAL
		bool usingUniformRefinement;
		Real treeFactor;

		MTools::MshRegion3D *mshRegion;
	};
}

#endif //#if USE_MTOOLS

#endif //#ifndef _MTOOLS_MAIN_DRIVE_H_
