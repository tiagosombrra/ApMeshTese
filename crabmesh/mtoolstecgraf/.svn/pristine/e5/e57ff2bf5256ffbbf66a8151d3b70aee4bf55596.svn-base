#ifndef _TECGRAF_MSH2D_MAIN_DRIVE_H_
#define _TECGRAF_MSH2D_MAIN_DRIVE_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF_MSH_2D

#include "MJMesh/Definitions.h"
#include "Data/Mesh.h"
#include "Performer/MainDrive.h"

namespace TecGraf
{
	namespace Msh2D
	{
		class MainDrive : public Performer::MainDrive
		{
		public:

			MainDrive();
			virtual ~MainDrive();

			virtual void setAlgorithm(enum Algorithms algorithm);

			virtual void setBilinearElementType(UInt bilinearElementType);
			virtual UInt getBilinearElementType() const;

			virtual void setBilinearDiagonalType(UInt bilinearDiagonalType);
			virtual UInt getBilinearDiagonalType() const;

			virtual void setBilinearAngleTolerance(Real bilinearAngleTolerance);
			virtual Real getBilinearAngleTolerance() const;

			virtual void setBilinearNumberVerticesDirection1(UInt bilinearNumberVerticesDirection1);
			virtual UInt getBilinearNumberVerticesDirection1() const;

			virtual void setBilinearNumberVerticesDirection2(UInt bilinearNumberVerticesDirection2);
			virtual UInt getBilinearNumberVerticesDirection2() const;

			virtual void setTryBilinearElementType(UInt tryBilinearElementType);
			virtual UInt getTryBilinearElementType() const;

			virtual void setTryBilinearDiagonalType(UInt tryBilinearDiagonalType);
			virtual UInt getTryBilinearDiagonalType() const;

			virtual void setCollBilinearElementType(UInt collBilinearElementType);
			virtual UInt getCollBilinearElementType() const;

			virtual void setCollBilinearDiagonalType(UInt collBilinearDiagonalType);
			virtual UInt getCollBilinearDiagonalType() const;

			virtual void setCollBilinearAngleTolerance(Real collBilinearAngleTolerance);
			virtual Real getCollBilinearAngleTolerance() const;

			virtual void setCollBilinearNumberVerticesDirection1(UInt collBilinearNumberVerticesDirection1);
			virtual UInt getCollBilinearNumberVerticesDirection1() const;

			virtual void setCollBilinearNumberVerticesDirection2(UInt collBilinearNumberVerticesDirection2);
			virtual UInt getCollBilinearNumberVerticesDirection2() const;

			virtual void setLoftElementType(UInt loftElementType);
			virtual UInt getLoftElementType() const;

			virtual void setLoftDiagonalType(UInt loftDiagonalType);
			virtual UInt getLoftDiagonalType() const;

			virtual void setLoftAngleTolerance(Real loftAngleTolerance);
			virtual Real getLoftAngleTolerance() const;

			virtual void setLoftNumberVerticesDirection1(UInt loftNumberVerticesDirection1);
			virtual UInt getLoftNumberVerticesDirection1() const;

			virtual void setLoftNumberVerticesDirection2(UInt loftNumberVerticesDirection2);
			virtual UInt getLoftNumberVerticesDirection2() const;

			virtual void setLoftDirection(UInt loftDirection);
			virtual UInt getLoftDirection() const;

			virtual void setLoftWeight(Real loftWeight);
			virtual Real getLoftWeight() const;

			virtual void setCollLoftElementType(UInt collLoftElementType);
			virtual UInt getCollLoftElementType() const;

			virtual void setCollLoftDiagonalType(UInt collLoftDiagonalType);
			virtual UInt getCollLoftDiagonalType() const;

			virtual void setCollLoftAngleTolerance(Real collLoftAngleTolerance);
			virtual Real getCollLoftAngleTolerance() const;

			virtual void setCollLoftNumberVerticesDirection1(UInt collLoftNumberVerticesDirection1);
			virtual UInt getCollLoftNumberVerticesDirection1() const;

			virtual void setCollLoftNumberVerticesDirection2(UInt collLoftNumberVerticesDirection2);
			virtual UInt getCollLoftNumberVerticesDirection2() const;

			virtual void setCollLoftWeight(Real collLoftWeight);
			virtual Real getCollLoftWeight() const;

			virtual void setTrilinearElementType(UInt trilinearElementType);
			virtual UInt getTrilinearElementType() const;

			virtual void setTrilinearAngleTolerance(Real trilinearAngleTolerance);
			virtual Real getTrilinearAngleTolerance() const;

			virtual void setTrilinearNumberVerticesDirection(UInt trilinearNumberVerticesDirection1);
			virtual UInt getTrilinearNumberVerticesDirection() const;

			virtual void setTryTrilinearElementType(UInt tryTrilinearElementType);
			virtual UInt getTryTrilinearElementType() const;

			virtual void setBoundContractionInternalPoints(bool boundContractionInternalPoints);
			virtual bool isBoundContractionInternalPoints() const;

			virtual void setBoundContractionQuadtreeInternalPoints(bool boundContractionQuadtreeInternalPoints);
			virtual bool isBoundContractionQuadtreeInternalPoints() const;

			virtual void setBoundContractionWarpInternalPoints(bool boundContractionWarpInternalPoints);
			virtual bool isBoundContractionWarpInternalPoints() const;

			virtual void setBoundContractionWarpQuadtreeInternalPoints(bool boundContractionWarpQuadtreeInternalPoints);
			virtual bool isBoundContractionWarpQuadtreeInternalPoints() const;

			virtual void setBoundContractionWarpBoundaryPoints(bool boundContractionWarpBoundaryPoints);
			virtual bool isBoundContractionWarpBoundaryPoints() const;

			//virtual void setBoundContractionWarpInvertReadOrderOnTxt(bool boundContractionWarpInvertReadOrderOnTxt);
			//virtual bool isBoundContractionWarpInvertReadOrderOnTxt() const;

			virtual void setBoundContractionWarpBoundary(MJMesh::Boundary *boundContractionWarpBoundary);
			virtual MJMesh::Boundary *getBoundContractionWarpBoundary() const;

			virtual void setBoundContractionWarpMesh(Data::Mesh *boundContractionWarpMesh);
			virtual Data::Mesh *getBoundContractionWarpMesh() const;

			virtual void setBoundContractionWarpLoadingWarpBoundary(bool boundContractionWarpLoadingWarpBoundary);
			virtual bool isBoundContractionWarpLoadingWarpBoundary() const;

			virtual void setQuadBoundElementType(UInt quadBoundElementType);
			virtual UInt getQuadBoundElementType() const;

			virtual void setQuadBoundRefineQuadtree(bool quadBoundRefineQuadtree);
			virtual bool isQuadBoundRefineQuadtree() const;

			virtual void setShapeInternalPoints(bool shapeInternalPoints);
			virtual bool isShapeInternalPoints() const;

			virtual void setShapeRefinementFactor(Real shapeRefinementFactor);
			virtual Real getShapeRefinementFactor() const;

			virtual void setEdgeInternalPoints(bool edgeInternalPoints);
			virtual bool isEdgeInternalPoints() const;

			virtual void setEdgeRefinementFactor(Real edgeRefinementFactor);
			virtual Real getEdgeRefinementFactor() const;

#if USE_QUADS
			virtual void setQuadSeamInternalPoints(bool quadSeamInternalPoints);
			virtual bool isQuadSeamInternalPoints() const;

			virtual void setQuadSeamRefinementFactor(Real quadSeamRefinementFactor);
			virtual Real getQuadSeamRefinementFactor() const;

			virtual void setQuadSeamEdgeInternalPoints(bool quadSeamEdgeInternalPoints);
			virtual bool isQuadSeamEdgeInternalPoints() const;

			virtual void setQuadSeamEdgeRefinementFactor(Real quadSeamEdgeRefinementFactor);
			virtual Real getQuadSeamEdgeRefinementFactor() const;

			virtual void setTemplateNumberSides(UInt templateNumberSides);
			virtual UInt getTemplateNumberSides() const;

			virtual void setTemplateNumberSubdivision(UInt i, UInt templateNumberSubdivision);
			virtual UInt getTemplateNumberSubdivision(UInt i) const;

			virtual void setTemplateSmooth(bool templateSmooth);
			virtual bool isTemplateSmooth() const;

			virtual void setTemplateU1(Real templateU1);
			virtual Real getTemplateU1() const;

			virtual void setTemplateV1(Real templateV1);
			virtual Real getTemplateV1() const;

			virtual void setTemplateU2(Real templateU2);
			virtual Real getTemplateU2() const;

			virtual void setTemplateV2(Real templateV2);
			virtual Real getTemplateV2() const;
#endif //#if USE_QUADS

#if USE_GUI
			virtual void setBoundary(MJMesh::Boundary *boundary);
			//virtual void setTree(Data::BoxTree *tree);
			virtual void setMesh(Data::Mesh *mesh);
#endif //#if USE_GUI

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

			virtual bool convertToMsh2D(double *&pts, int &nPts, Data::VertexVector &map, bool clockWise) const;
			virtual bool convertToMsh2D(double *&pts, int &nLoops, int *&loops, Data::VertexVector &map, bool clockWise) const;
			virtual bool convertToMsh2D(double *&pts, int &nPts, int *&edges, int &nEdges, Data::VertexVector &map, bool clockWise) const;
			virtual bool convertToMesh(int nPts, double *pts, int nElems, int *elems, int type,
				Data::VertexVector &map, bool clockWise = true, bool rebuildMap = false, bool checkBoundary = false);

		protected:

			virtual bool executeBoundaryBuilder(bool del);
			virtual bool executeWarpBoundaryBuilder(bool del);

			virtual bool executeBilinear();
			virtual bool executeTryBilinear();
			virtual bool executeCollBilinear();
			virtual bool executeLoft();
			virtual bool executeCollLoft();
			virtual bool executeTrilinear();
			virtual bool executeTryTrilinear();
			virtual bool executeBoundContraction();
			virtual bool executeBoundContractionWarp();
			virtual bool executeQuadBound();
			virtual bool executeShape();
			virtual bool executeEdge();
#if USE_QUADS
			virtual bool executeQuadSeam();
			virtual bool executeQuadSeamEdge();
			virtual bool executeTemplate();
#endif //#if USE_QUADS

		protected:

			virtual Performer::IdManager *makeIdManager(const Performer::IdManager *idManager) const;
			virtual MJMesh::BoundaryBuilder *makeBoundaryBuilder() const;

		protected:

			enum Algorithms algorithm;

			UInt bilinearElementType;
			UInt bilinearDiagonalType;
			Real bilinearAngleTolerance;
			UInt bilinearNumberVerticesDirection1;
			UInt bilinearNumberVerticesDirection2;

			UInt tryBilinearElementType;
			UInt tryBilinearDiagonalType;

			UInt collBilinearElementType;
			UInt collBilinearDiagonalType;
			Real collBilinearAngleTolerance;
			UInt collBilinearNumberVerticesDirection1;
			UInt collBilinearNumberVerticesDirection2;

			UInt loftElementType;
			UInt loftDiagonalType;
			Real loftAngleTolerance;
			UInt loftNumberVerticesDirection1;
			UInt loftNumberVerticesDirection2;
			UInt loftDirection;
			Real loftWeight;

			UInt collLoftElementType;
			UInt collLoftDiagonalType;
			Real collLoftAngleTolerance;
			UInt collLoftNumberVerticesDirection1;
			UInt collLoftNumberVerticesDirection2;
			Real collLoftWeight;

			UInt trilinearElementType;
			Real trilinearAngleTolerance;
			UInt trilinearNumberVerticesDirection;

			UInt tryTrilinearElementType;

			bool boundContractionInternalPoints;
			bool boundContractionQuadtreeInternalPoints;

			bool boundContractionWarpInternalPoints;
			bool boundContractionWarpQuadtreeInternalPoints;
			bool boundContractionWarpBoundaryPoints;
			//bool boundContractionWarpInvertReadOrderOnTxt;
			MJMesh::Boundary *boundContractionWarpBoundary;
			Data::Mesh *boundContractionWarpMesh;
			bool boundContractionWarpLoadingWarpBoundary;

			UInt quadBoundElementType;
			bool quadBoundRefineQuadtree;

			bool shapeInternalPoints;
			Real shapeRefinementFactor;

			bool edgeInternalPoints;
			Real edgeRefinementFactor;

#if USE_QUADS
			bool quadSeamInternalPoints;
			Real quadSeamRefinementFactor;

			bool quadSeamEdgeInternalPoints;
			Real quadSeamEdgeRefinementFactor;

			UInt templateNumberSides;
			UInt templateNumberSubdivision[4];
			bool templateSmooth;
			Real templateU1;
			Real templateV1;
			Real templateU2;
			Real templateV2;
#endif //#if USE_QUADS

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
			MJMesh::BoundaryBuilder *boundContractionWarpBoundaryBuilder;

		};
	}
}

#endif //#if USE_TECGRAF_MSH_2D

#endif //#ifndef _TECGRAF_MSH2D_MAIN_DRIVE_H_
