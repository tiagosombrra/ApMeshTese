#include "TecGraf/Msh2D/MainDrive.h"

#if USE_TECGRAF_MSH_2D

#include "Data/State.h"

#include "Performer/SequentialIdManager.h"

#include "MJMesh/Edge2D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/BoundaryBuilder.h"

#include "IO/MJMeshFileReader.h"
#include "IO/TextMeshFileWriter.h"
#include "IO/NeutralFileWriter.h"

#include "msh2d.h"

using namespace TecGraf;

TecGraf::Msh2D::MainDrive::MainDrive() :
	Performer::MainDrive::MainDrive()
{
	this->algorithm = UNKNOWN;

	this->bilinearElementType = 3;
	this->bilinearDiagonalType = 1;
	this->bilinearAngleTolerance = 10.0*M_PI/180.0; //10 degrees
	this->bilinearNumberVerticesDirection1 = 0;
	this->bilinearNumberVerticesDirection2 = 1;

	this->tryBilinearElementType = 3;
	this->tryBilinearDiagonalType = 1;

	this->collBilinearElementType = 3;
	this->collBilinearDiagonalType = 1;
	this->collBilinearAngleTolerance = 10.0*M_PI/180.0; //10 degrees
	this->collBilinearNumberVerticesDirection1 = 0;
	this->collBilinearNumberVerticesDirection2 = 1;

	this->loftElementType = 3;
	this->loftDiagonalType = 1;
	this->loftAngleTolerance = 10.0*M_PI/180.0; //10 degrees
	this->loftNumberVerticesDirection1 = 0;
	this->loftNumberVerticesDirection2 = 1;
	this->loftDirection = 0;
	this->loftWeight = 1.0;

	this->collLoftElementType = 3;
	this->collLoftDiagonalType = 1;
	this->collLoftAngleTolerance = 10.0*M_PI/180.0; //10 degrees
	this->collLoftNumberVerticesDirection1 = 0;
	this->collLoftNumberVerticesDirection2 = 1;
	this->collLoftWeight = 1.0;

	this->trilinearElementType = 3;
	this->trilinearAngleTolerance = 10.0*M_PI/180.0; //10 degrees
	this->trilinearNumberVerticesDirection = 0;

	this->tryTrilinearElementType = 3;

	this->boundContractionInternalPoints = true;
	this->boundContractionQuadtreeInternalPoints = true;

	this->boundContractionWarpInternalPoints = false;
	this->boundContractionWarpQuadtreeInternalPoints = false;
	this->boundContractionWarpBoundaryPoints = true;
	this->boundContractionWarpBoundary = NULL;
	this->boundContractionWarpMesh = NULL;
	this->boundContractionWarpLoadingWarpBoundary = false;

	this->quadBoundElementType = 3;
	this->quadBoundRefineQuadtree = true;

	this->shapeInternalPoints = true;
	this->shapeRefinementFactor = 0.1;

	this->edgeInternalPoints = true;
	this->edgeRefinementFactor = 0.1;

#if USE_QUADS
	this->quadSeamInternalPoints = true;
	this->quadSeamRefinementFactor = 0.1;

	this->quadSeamEdgeInternalPoints = true;
	this->quadSeamEdgeRefinementFactor = 0.1;

	this->templateNumberSides = 0;
	this->templateNumberSubdivision[0] = 0;
	this->templateNumberSubdivision[1] = 0;
	this->templateNumberSubdivision[2] = 0;
	this->templateNumberSubdivision[3] = 0;
	this->templateSmooth = false;
	this->templateU1 = 0.0;
	this->templateV1 = 0.0;
	this->templateU2 = 0.0;
	this->templateV2 = 0.0;
#endif //#if USE_QUADS

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
	this->boundContractionWarpBoundaryBuilder = NULL;

	this->status = this->notInit;
}

TecGraf::Msh2D::MainDrive::~MainDrive()
{
	this->clear();
}

void TecGraf::Msh2D::MainDrive::setAlgorithm(enum Algorithms algorithm)
{
	this->algorithm = algorithm;
}

void TecGraf::Msh2D::MainDrive::setBilinearElementType(UInt bilinearElementType)
{
	this->bilinearElementType = bilinearElementType;
}

UInt TecGraf::Msh2D::MainDrive::getBilinearElementType() const
{
	return this->bilinearElementType;
}

void TecGraf::Msh2D::MainDrive::setBilinearDiagonalType(UInt bilinearDiagonalType)
{
	this->bilinearDiagonalType = bilinearDiagonalType;
}

UInt TecGraf::Msh2D::MainDrive::getBilinearDiagonalType() const
{
	return this->bilinearDiagonalType;
}

void TecGraf::Msh2D::MainDrive::setBilinearAngleTolerance(Real bilinearAngleTolerance)
{
	this->bilinearAngleTolerance = bilinearAngleTolerance*M_PI/180.0;
}

Real TecGraf::Msh2D::MainDrive::getBilinearAngleTolerance() const
{
	return this->bilinearAngleTolerance*180.0/M_PI;
}

void TecGraf::Msh2D::MainDrive::setBilinearNumberVerticesDirection1(UInt bilinearNumberVerticesDirection1)
{
	this->bilinearNumberVerticesDirection1 = bilinearNumberVerticesDirection1;
}

UInt TecGraf::Msh2D::MainDrive::getBilinearNumberVerticesDirection1() const
{
	return this->bilinearNumberVerticesDirection1;
}

void TecGraf::Msh2D::MainDrive::setBilinearNumberVerticesDirection2(UInt bilinearNumberVerticesDirection2)
{
	this->bilinearNumberVerticesDirection2 = bilinearNumberVerticesDirection2;
}

UInt TecGraf::Msh2D::MainDrive::getBilinearNumberVerticesDirection2() const
{
	return this->bilinearNumberVerticesDirection2;
}

void TecGraf::Msh2D::MainDrive::setTryBilinearElementType(UInt tryBilinearElementType)
{
	this->tryBilinearElementType = tryBilinearElementType;
}

UInt TecGraf::Msh2D::MainDrive::getTryBilinearElementType() const
{
	return this->tryBilinearElementType;
}

void TecGraf::Msh2D::MainDrive::setTryBilinearDiagonalType(UInt tryBilinearDiagonalType)
{
	this->tryBilinearDiagonalType = tryBilinearDiagonalType;
}

UInt TecGraf::Msh2D::MainDrive::getTryBilinearDiagonalType() const
{
	return this->tryBilinearDiagonalType;
}

void TecGraf::Msh2D::MainDrive::setCollBilinearElementType(UInt collBilinearElementType)
{
	this->collBilinearElementType = collBilinearElementType;
}

UInt TecGraf::Msh2D::MainDrive::getCollBilinearElementType() const
{
	return this->collBilinearElementType;
}

void TecGraf::Msh2D::MainDrive::setCollBilinearDiagonalType(UInt collBilinearDiagonalType)
{
	this->collBilinearDiagonalType = collBilinearDiagonalType;
}

UInt TecGraf::Msh2D::MainDrive::getCollBilinearDiagonalType() const
{
	return this->collBilinearDiagonalType;
}

void TecGraf::Msh2D::MainDrive::setCollBilinearAngleTolerance(Real collBilinearAngleTolerance)
{
	this->collBilinearAngleTolerance = collBilinearAngleTolerance*M_PI/180.0;
}

Real TecGraf::Msh2D::MainDrive::getCollBilinearAngleTolerance() const
{
	return this->collBilinearAngleTolerance*180.0/M_PI;
}

void TecGraf::Msh2D::MainDrive::setCollBilinearNumberVerticesDirection1(UInt collBilinearNumberVerticesDirection1)
{
	this->collBilinearNumberVerticesDirection1 = collBilinearNumberVerticesDirection1;
}

UInt TecGraf::Msh2D::MainDrive::getCollBilinearNumberVerticesDirection1() const
{
	return this->collBilinearNumberVerticesDirection1;
}

void TecGraf::Msh2D::MainDrive::setCollBilinearNumberVerticesDirection2(UInt collBilinearNumberVerticesDirection2)
{
	this->collBilinearNumberVerticesDirection2 = collBilinearNumberVerticesDirection2;
}

UInt TecGraf::Msh2D::MainDrive::getCollBilinearNumberVerticesDirection2() const
{
	return this->collBilinearNumberVerticesDirection2;
}

void TecGraf::Msh2D::MainDrive::setLoftElementType(UInt loftElementType)
{
	this->loftElementType = loftElementType;
}

UInt TecGraf::Msh2D::MainDrive::getLoftElementType() const
{
	return this->loftElementType;
}

void TecGraf::Msh2D::MainDrive::setLoftDiagonalType(UInt loftDiagonalType)
{
	this->loftDiagonalType = loftDiagonalType;
}

UInt TecGraf::Msh2D::MainDrive::getLoftDiagonalType() const
{
	return this->loftDiagonalType;
}

void TecGraf::Msh2D::MainDrive::setLoftAngleTolerance(Real loftAngleTolerance)
{
	this->loftAngleTolerance = loftAngleTolerance*M_PI/180.0;
}

Real TecGraf::Msh2D::MainDrive::getLoftAngleTolerance() const
{
	return this->loftAngleTolerance*180.0/M_PI;
}

void TecGraf::Msh2D::MainDrive::setLoftNumberVerticesDirection1(UInt loftNumberVerticesDirection1)
{
	this->loftNumberVerticesDirection1 = loftNumberVerticesDirection1;
}

UInt TecGraf::Msh2D::MainDrive::getLoftNumberVerticesDirection1() const
{
	return this->loftNumberVerticesDirection1;
}

void TecGraf::Msh2D::MainDrive::setLoftNumberVerticesDirection2(UInt loftNumberVerticesDirection2)
{
	this->loftNumberVerticesDirection2 = loftNumberVerticesDirection2;
}

UInt TecGraf::Msh2D::MainDrive::getLoftNumberVerticesDirection2() const
{
	return this->loftNumberVerticesDirection2;
}

void TecGraf::Msh2D::MainDrive::setLoftDirection(UInt loftDirection)
{
	this->loftDirection = loftDirection;
}

UInt TecGraf::Msh2D::MainDrive::getLoftDirection() const
{
	return this->loftDirection;
}

void TecGraf::Msh2D::MainDrive::setLoftWeight(Real loftWeight)
{
	this->loftWeight = loftWeight;
}

Real TecGraf::Msh2D::MainDrive::getLoftWeight() const
{
	return this->loftWeight;
}

void TecGraf::Msh2D::MainDrive::setCollLoftElementType(UInt collLoftElementType)
{
	this->collLoftElementType = collLoftElementType;
}

UInt TecGraf::Msh2D::MainDrive::getCollLoftElementType() const
{
	return this->collLoftElementType;
}

void TecGraf::Msh2D::MainDrive::setCollLoftDiagonalType(UInt collLoftDiagonalType)
{
	this->collLoftDiagonalType = collLoftDiagonalType;
}

UInt TecGraf::Msh2D::MainDrive::getCollLoftDiagonalType() const
{
	return this->collLoftDiagonalType;
}

void TecGraf::Msh2D::MainDrive::setCollLoftAngleTolerance(Real collLoftAngleTolerance)
{
	this->collLoftAngleTolerance = collLoftAngleTolerance*M_PI/180.0;
}

Real TecGraf::Msh2D::MainDrive::getCollLoftAngleTolerance() const
{
	return this->collLoftAngleTolerance*180.0/M_PI;
}

void TecGraf::Msh2D::MainDrive::setCollLoftNumberVerticesDirection1(UInt collLoftNumberVerticesDirection1)
{
	this->collLoftNumberVerticesDirection1 = collLoftNumberVerticesDirection1;
}

UInt TecGraf::Msh2D::MainDrive::getCollLoftNumberVerticesDirection1() const
{
	return this->collLoftNumberVerticesDirection1;
}

void TecGraf::Msh2D::MainDrive::setCollLoftNumberVerticesDirection2(UInt collLoftNumberVerticesDirection2)
{
	this->collLoftNumberVerticesDirection2 = collLoftNumberVerticesDirection2;
}

UInt TecGraf::Msh2D::MainDrive::getCollLoftNumberVerticesDirection2() const
{
	return this->collLoftNumberVerticesDirection2;
}

void TecGraf::Msh2D::MainDrive::setCollLoftWeight(Real collLoftWeight)
{
	this->collLoftWeight = collLoftWeight;
}

Real TecGraf::Msh2D::MainDrive::getCollLoftWeight() const
{
	return this->collLoftWeight;
}

void TecGraf::Msh2D::MainDrive::setTrilinearElementType(UInt trilinearElementType)
{
	this->trilinearElementType = trilinearElementType;
}

UInt TecGraf::Msh2D::MainDrive::getTrilinearElementType() const
{
	return this->trilinearElementType;
}

void TecGraf::Msh2D::MainDrive::setTrilinearAngleTolerance(Real trilinearAngleTolerance)
{
	this->trilinearAngleTolerance = trilinearAngleTolerance*M_PI/180.0;
}

Real TecGraf::Msh2D::MainDrive::getTrilinearAngleTolerance() const
{
	return this->trilinearAngleTolerance*180.0/M_PI;
}

void TecGraf::Msh2D::MainDrive::setTrilinearNumberVerticesDirection(UInt trilinearNumberVerticesDirection)
{
	this->trilinearNumberVerticesDirection = trilinearNumberVerticesDirection;
}

UInt TecGraf::Msh2D::MainDrive::getTrilinearNumberVerticesDirection() const
{
	return this->trilinearNumberVerticesDirection;
}

void TecGraf::Msh2D::MainDrive::setTryTrilinearElementType(UInt tryTrilinearElementType)
{
	this->tryTrilinearElementType = tryTrilinearElementType;
}

UInt TecGraf::Msh2D::MainDrive::getTryTrilinearElementType() const
{
	return this->tryTrilinearElementType;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionInternalPoints(bool boundContractionInternalPoints)
{
	this->boundContractionInternalPoints = boundContractionInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isBoundContractionInternalPoints() const
{
	return this->boundContractionInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionQuadtreeInternalPoints(bool boundContractionQuadtreeInternalPoints)
{
	this->boundContractionQuadtreeInternalPoints = boundContractionQuadtreeInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isBoundContractionQuadtreeInternalPoints() const
{
	return this->boundContractionQuadtreeInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionWarpInternalPoints(bool boundContractionWarpInternalPoints)
{
	this->boundContractionWarpInternalPoints = boundContractionWarpInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isBoundContractionWarpInternalPoints() const
{
	return this->boundContractionWarpInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionWarpQuadtreeInternalPoints(bool boundContractionWarpQuadtreeInternalPoints)
{
	this->boundContractionWarpQuadtreeInternalPoints = boundContractionWarpQuadtreeInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isBoundContractionWarpQuadtreeInternalPoints() const
{
	return this->boundContractionWarpQuadtreeInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionWarpBoundaryPoints(bool boundContractionWarpBoundaryPoints)
{
	this->boundContractionWarpBoundaryPoints = boundContractionWarpBoundaryPoints;
}

bool TecGraf::Msh2D::MainDrive::isBoundContractionWarpBoundaryPoints() const
{
	return this->boundContractionWarpBoundaryPoints;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionWarpBoundary(MJMesh::Boundary *boundContractionWarpBoundary)
{
	this->boundContractionWarpBoundary = boundContractionWarpBoundary;

#if USE_GUI
	if (this->boundContractionWarpBoundaryBuilder)
	{
		this->boundContractionWarpBoundaryBuilder->setBoundary(boundContractionWarpBoundary);
	}
#endif //#if USE_GUI
}

MJMesh::Boundary *TecGraf::Msh2D::MainDrive::getBoundContractionWarpBoundary() const
{
#if USE_GUI
	if (this->boundContractionWarpBoundary)
	{
		return this->boundContractionWarpBoundary;
	}

	if (this->boundContractionWarpBoundaryBuilder)
	{
		return static_cast<MJMesh::Boundary *>(this->boundContractionWarpBoundaryBuilder->getBoundary());
	}

	return NULL;
#else
	return this->boundContractionWarpBoundary;
#endif //#if USE_GUI
}

void TecGraf::Msh2D::MainDrive::setBoundContractionWarpMesh(Data::Mesh *boundContractionWarpMesh)
{
	this->boundContractionWarpMesh = boundContractionWarpMesh;
}

Data::Mesh *TecGraf::Msh2D::MainDrive::getBoundContractionWarpMesh() const
{
	return this->boundContractionWarpMesh;
}

void TecGraf::Msh2D::MainDrive::setBoundContractionWarpLoadingWarpBoundary(bool boundContractionWarpLoadingWarpBoundary)
{
	this->boundContractionWarpLoadingWarpBoundary = boundContractionWarpLoadingWarpBoundary;
}

bool TecGraf::Msh2D::MainDrive::isBoundContractionWarpLoadingWarpBoundary() const
{
	return this->boundContractionWarpLoadingWarpBoundary;
}

void TecGraf::Msh2D::MainDrive::setQuadBoundElementType(UInt quadBoundElementType)
{
	this->quadBoundElementType = quadBoundElementType;
}

UInt TecGraf::Msh2D::MainDrive::getQuadBoundElementType() const
{
	return this->quadBoundElementType;
}

void TecGraf::Msh2D::MainDrive::setQuadBoundRefineQuadtree(bool quadBoundRefineQuadtree)
{
	this->quadBoundRefineQuadtree = quadBoundRefineQuadtree;
}

bool TecGraf::Msh2D::MainDrive::isQuadBoundRefineQuadtree() const
{
	return this->quadBoundRefineQuadtree;
}

void TecGraf::Msh2D::MainDrive::setShapeInternalPoints(bool shapeInternalPoints)
{
	this->shapeInternalPoints = shapeInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isShapeInternalPoints() const
{
	return this->shapeInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setShapeRefinementFactor(Real shapeRefinementFactor)
{
	this->shapeRefinementFactor = shapeRefinementFactor;
}

Real TecGraf::Msh2D::MainDrive::getShapeRefinementFactor() const
{
	return this->shapeRefinementFactor;
}

void TecGraf::Msh2D::MainDrive::setEdgeInternalPoints(bool edgeInternalPoints)
{
	this->edgeInternalPoints = edgeInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isEdgeInternalPoints() const
{
	return this->edgeInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setEdgeRefinementFactor(Real edgeRefinementFactor)
{
	this->edgeRefinementFactor = edgeRefinementFactor;
}

Real TecGraf::Msh2D::MainDrive::getEdgeRefinementFactor() const
{
	return this->edgeRefinementFactor;
}

#if USE_QUADS
void TecGraf::Msh2D::MainDrive::setQuadSeamInternalPoints(bool quadSeamInternalPoints)
{
	this->quadSeamInternalPoints = quadSeamInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isQuadSeamInternalPoints() const
{
	return this->quadSeamInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setQuadSeamRefinementFactor(Real quadSeamRefinementFactor)
{
	this->quadSeamRefinementFactor = quadSeamRefinementFactor;
}

Real TecGraf::Msh2D::MainDrive::getQuadSeamRefinementFactor() const
{
	return this->quadSeamRefinementFactor;
}

void TecGraf::Msh2D::MainDrive::setQuadSeamEdgeInternalPoints(bool quadSeamEdgeInternalPoints)
{
	this->quadSeamEdgeInternalPoints = quadSeamEdgeInternalPoints;
}

bool TecGraf::Msh2D::MainDrive::isQuadSeamEdgeInternalPoints() const
{
	return this->quadSeamEdgeInternalPoints;
}

void TecGraf::Msh2D::MainDrive::setQuadSeamEdgeRefinementFactor(Real quadSeamEdgeRefinementFactor)
{
	this->quadSeamEdgeRefinementFactor = quadSeamEdgeRefinementFactor;
}

Real TecGraf::Msh2D::MainDrive::getQuadSeamEdgeRefinementFactor() const
{
	return this->quadSeamEdgeRefinementFactor;
}

void TecGraf::Msh2D::MainDrive::setTemplateNumberSides(UInt templateNumberSides)
{
	this->templateNumberSides = templateNumberSides;
}

UInt TecGraf::Msh2D::MainDrive::getTemplateNumberSides() const
{
	return this->templateNumberSides;
}

void TecGraf::Msh2D::MainDrive::setTemplateNumberSubdivision(UInt i, UInt templateNumberSubdivision)
{
	if (i < 4)
	{
		this->templateNumberSubdivision[i] = templateNumberSubdivision;
	}
}

UInt TecGraf::Msh2D::MainDrive::getTemplateNumberSubdivision(UInt i) const
{
	return (i < 4) ? this->templateNumberSubdivision[i] : 0;
}

void TecGraf::Msh2D::MainDrive::setTemplateSmooth(bool templateSmooth)
{
	this->templateSmooth = templateSmooth;
}

bool TecGraf::Msh2D::MainDrive::isTemplateSmooth() const
{
	return this->templateSmooth;
}

void TecGraf::Msh2D::MainDrive::setTemplateU1(Real templateU1)
{
	this->templateU1 = templateU1;
}

Real TecGraf::Msh2D::MainDrive::getTemplateU1() const
{
	return this->templateU1;
}

void TecGraf::Msh2D::MainDrive::setTemplateV1(Real templateV1)
{
	this->templateV1 = templateV1;
}

Real TecGraf::Msh2D::MainDrive::getTemplateV1() const
{
	return this->templateV1;
}

void TecGraf::Msh2D::MainDrive::setTemplateU2(Real templateU2)
{
	this->templateU2 = templateU2;
}

Real TecGraf::Msh2D::MainDrive::getTemplateU2() const
{
	return this->templateU2;
}

void TecGraf::Msh2D::MainDrive::setTemplateV2(Real templateV2)
{
	this->templateV2 = templateV2;
}

Real TecGraf::Msh2D::MainDrive::getTemplateV2() const
{
	return this->templateV2;
}
#endif //#if USE_QUADS

void TecGraf::Msh2D::MainDrive::execute()
{
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

	case BILINEAR:
		success = this->executeBilinear();
		break;

	case TRY_BILINEAR:
		success = this->executeTryBilinear();
		break;

	case COLL_BILINEAR:
		success = this->executeCollBilinear();
		break;

	case LOFT:
		success = this->executeLoft();
		break;

	case COLL_LOFT:
		success = this->executeCollLoft();
		break;

	case TRILINEAR:
		success = this->executeTrilinear();
		break;

	case TRY_TRILINEAR:
		success = this->executeTryTrilinear();
		break;

	case BOUND_CONTRACTION:
		success = this->executeBoundContraction();
		break;

	case BOUND_CONTRACTION_WARP:
		success = this->executeBoundContractionWarp();
		break;

	case QUAD_BOUND:
		success = this->executeQuadBound();
		break;

	case SHAPE:
		success = this->executeShape();
		break;

	case EDGE:
		success = this->executeEdge();
		break;

#if USE_QUADS
	case QUAD_SEAM:
		success = this->executeQuadSeam();
		break;

	case QUAD_SEAM_EDGE:
		success = this->executeQuadSeamEdge();
		break;

	case TEMPLATE:
		success = this->executeTemplate();
		break;
#endif //#if USE_QUADS

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

#if USE_GUI
void TecGraf::Msh2D::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;

	if (this->boundaryBuilder)
	{
		this->boundaryBuilder->setBoundary(boundary);
	}
}

/*void TecGraf::Msh2D::MainDrive::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}*/

void TecGraf::Msh2D::MainDrive::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}
#endif //#if USE_GUI

const Data::Boundary *TecGraf::Msh2D::MainDrive::getBoundary() const
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

/*const Data::BoxTree *TecGraf::Msh2D::MainDrive::getTree() const
{
	return this->tree;
}*/

const Data::Mesh *TecGraf::Msh2D::MainDrive::getMesh() const
{
	return this->mesh;
}

#if USE_GUI
/*const Data::Front *TecGraf::Msh2D::MainDrive::getFront() const
{
	return this->front;
}*/
#endif //#if USE_GUI

void TecGraf::Msh2D::MainDrive::clear()
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

	if (this->boundContractionWarpMesh)
	{
		if (this->boundContractionWarpBoundary)
		{
			//Data::VertexVector vertices;
			Data::VertexList vertices;
			//Data::FrontElementVector elements;
			Data::FrontElementList elements;

			this->boundContractionWarpBoundary->setVertices(vertices);
			this->boundContractionWarpBoundary->setElements(elements);
		}

		delete this->boundContractionWarpMesh;

		this->boundContractionWarpMesh = NULL;
	}

	if (this->boundContractionWarpBoundary)
	{
#if USE_GUI
		if ((this->boundContractionWarpBoundaryBuilder) && (this->boundContractionWarpBoundaryBuilder->getBoundary() == this->boundContractionWarpBoundary))
		{
			this->boundContractionWarpBoundaryBuilder->setBoundary(NULL);
		}
#endif //#if USE_GUI

		delete this->boundContractionWarpBoundary;

		this->boundContractionWarpBoundary = NULL;
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

	if (this->boundContractionWarpBoundaryBuilder)
	{
		if (this->boundContractionWarpBoundaryBuilder->getBoundary())
		{
			delete this->boundContractionWarpBoundaryBuilder->getBoundary();

#if USE_GUI
			this->boundContractionWarpBoundaryBuilder->setBoundary(NULL);
#endif //#if USE_GUI
		}

		delete this->boundContractionWarpBoundaryBuilder;

		this->boundContractionWarpBoundaryBuilder = NULL;
	}
}

bool TecGraf::Msh2D::MainDrive::loadFile()
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

	if (extension == ".2djmsh")
	{
		return this->loadInputFile(this->inputFileName);
	}

	return false;
}

bool TecGraf::Msh2D::MainDrive::saveFile() const
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

	if ((extension == ".dat") || (extension == ".txt"))
	{
		return this->saveOutputFile(this->outputFileName);
	}

	return false;
}

std::string TecGraf::Msh2D::MainDrive::qualityString() const
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

void TecGraf::Msh2D::MainDrive::findQuality()
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

bool TecGraf::Msh2D::MainDrive::loadInputFile(const std::string &inputFileName)
{
	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if (this->boundContractionWarpLoadingWarpBoundary)
	{
		this->boundContractionWarpBoundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());
	}
	else
	{
		this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());
	}

	if (extension == ".2djmsh")
	{
		IO::MJMeshFileReader reader;

		reader.setFilename(inputFileName);

		if (this->boundContractionWarpLoadingWarpBoundary)
		{
			reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundContractionWarpBoundaryBuilder));
		}
		else
		{
			reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));
		}

		reader.execute();

		UInt dimension = reader.getDimension();
		//Real treeFactor = reader.getTreeFactor();
		Data::setTolerance(reader.getTolerance());

		if (dimension != 2)
		{
			if (this->boundContractionWarpLoadingWarpBoundary)
			{
				this->boundContractionWarpBoundaryBuilder->clear();
			}
			else
			{
				this->boundaryBuilder->clear();
			}
		}

		return (dimension == 2);
	}

	return false;
}

bool TecGraf::Msh2D::MainDrive::saveInputFile(const std::string &/*outputFileName*/) const
{
	return false;
}

bool TecGraf::Msh2D::MainDrive::loadOutputFile(const std::string &/*inputFileName*/)
{
	return false;
}

bool TecGraf::Msh2D::MainDrive::saveOutputFile(const std::string &outputFileName) const
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

	if (extension == ".txt")
	{
		IO::TextMeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getMesh());
		writer.setDimension(2);

		writer.execute();

		return true;
	}

	if (extension == ".dat")
	{
		IO::NeutralFileWriter writer;

		writer.setFilename(outputFileName);

		if ((!this->getMesh()) || (this->getMesh()->size() == 0))
		{
			writer.setMesh(this->getBoundary());
			writer.setBoundary(true);
		}
		else
		{
			writer.setMesh(this->getMesh());
			writer.setBoundary(false);
		}

		writer.setDimension(2);

		writer.execute();

		return true;
	}

	return false;
}

bool TecGraf::Msh2D::MainDrive::convertToMsh2D(double *&pts, int &nPts, Data::VertexVector &map, bool clockWise) const
{
	nPts = static_cast<int>(this->boundary->verticesSize());

	pts = new double[2*nPts];

	/*UInt i = 0;

	Data::Boundary::ConstVertexIterator begin, end;

	if (clockWise)
	{
		begin = this->boundary->vEnd();
		end = this->boundary->vBegin();
	}
	else
	{
		begin = this->boundary->vBegin();
		end = this->boundary->vEnd();
	}

	map.resize(nPts);

	for (Data::Boundary::ConstVertexIterator iter = begin;
		 iter != end; clockWise ? --iter : ++iter, i++)
	{
		if (clockWise)
		{
			--iter;
		}

		pts[2*i+0] = (*iter)->getPoint()->getCoord(0);
		pts[2*i+1] = (*iter)->getPoint()->getCoord(1);

		map[i] = (*iter);

		if (clockWise)
		{
			++iter;
		}
	}

	if (clockWise)
	{
		double x = pts[2*(nPts - 1)+0];
		double y = pts[2*(nPts - 1)+1];
		Data::Vertex *v = map[nPts - 1];

		for (int i = nPts - 1; i > 0; i--)
		{
			pts[2*i+0] = pts[2*(i-1)+0];
			pts[2*i+1] = pts[2*(i-1)+1];
			map[i] = map[i-1];
		}

		pts[0] = x;
		pts[1] = y;
		map[0] = v;
	}*/

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

bool TecGraf::Msh2D::MainDrive::convertToMsh2D(double *&pts, int &nLoops, int *&loops, Data::VertexVector &map, bool clockWise) const
{
	int nPts = static_cast<int>(this->boundary->verticesSize());

	pts = new double[2*nPts];

	Data::IntVector lps;

	/*UInt i = 0;

	Data::Boundary::ConstVertexIterator begin, end;

	if (clockWise)
	{
		begin = this->boundary->vEnd();
		end = this->boundary->vBegin();
	}
	else
	{
		begin = this->boundary->vBegin();
		end = this->boundary->vEnd();
	}

	Data::Boundary::ConstVertexIterator first = this->boundary->vEnd();
	Data::Boundary::ConstVertexIterator prev = this->boundary->vEnd();

	map.resize(nPts);

	for (Data::Boundary::ConstVertexIterator iter = begin;
		 iter != end; clockWise ? --iter : ++iter, i++)
	{
		if (clockWise)
		{
			--iter;
		}

		bool last = false;

		if (first == this->boundary->vEnd())
		{
			nPts = 0;
			first = iter;
		}
		else if ((prev != this->boundary->vEnd()) && (first != prev))
		{
			const MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			for (Data::FrontElementList::const_iterator iter2 = v->feBegin();
				 iter2 != v->feEnd(); iter2++)
			{
				if ((*iter2)->have((*first)))
				{
					last = true;

					break;
				}
			}
		}

		pts[2*i+0] = (*iter)->getPoint()->getCoord(0);
		pts[2*i+1] = (*iter)->getPoint()->getCoord(1);

		map[i] = (*iter);

		prev = iter;

		if (clockWise)
		{
			++iter;
		}

		nPts++;

		if (last)
		{
			first = this->boundary->vEnd();

			lps.push_back(nPts);
		}
	}

	nLoops = static_cast<int>(lps.size());

	loops = new int[nLoops];

	for (int i = 0; i < nLoops; i++)
	{
		loops[i] = lps[i];
	}

	if ((clockWise) && (nLoops > 1))
	{
		double **buffer = new double*[nLoops];
		Data::VertexVector *maps = new Data::VertexVector[nLoops];
		int sum = 0;

		for (int i = 0; i < nLoops; i++)
		{
			buffer[i] = new double[2*loops[i]];
			maps[i].resize(loops[i]);

			for (int j = 0; j < loops[i]; j++)
			{
				buffer[i][2*j+0] = pts[2*(sum + j)+0];
				buffer[i][2*j+1] = pts[2*(sum + j)+1];
				maps[i][j] = map[sum+j];
			}

			sum += loops[i];
		}

		sum = 0;

		for (int i = 0; i < nLoops; i++)
		{
			int i2 = nLoops - i - 1;

			for (int j = 0; j < loops[i2]; j++)
			{
				pts[2*(sum + j)+0] = buffer[i2][2*j+0];
				pts[2*(sum + j)+1] = buffer[i2][2*j+1];
				map[sum+j] = maps[i2][j];
			}

			sum += loops[i2];

			delete [] buffer[i2];
		}

		delete [] buffer;
		delete [] maps;

		for (int i = 0; i < nLoops/2; i++)
		{
			int i2 = nLoops - i - 1;

			int l = loops[i];
			loops[i] = loops[i2];
			loops[i2] = l;
		}
	}

	if (clockWise)
	{
		int first = 0;

		for (int j = 0; j < nLoops; j++)
		{
			int nPts = first + loops[j];

			double x = pts[2*(nPts - 1)+0];
			double y = pts[2*(nPts - 1)+1];
			Data::Vertex *v = map[nPts - 1];

			for (int i = nPts - 1; i > first; i--)
			{
				pts[2*i+0] = pts[2*(i-1)+0];
				pts[2*i+1] = pts[2*(i-1)+1];
				map[i] = map[i-1];
			}

			pts[2*first+0] = x;
			pts[2*first+1] = y;
			map[first] = v;

			first = nPts;
		}
	}*/

#if USE_C__11
	Data::VertexHash used;

	used.reserve(nPts);
#else
	Data::VertexSet used;
#endif //#if USE_C__11

	map.resize(nPts);

	Data::Boundary::VertexIterator firstIter = clockWise ?
        --this->boundary->vEnd() : this->boundary->vBegin();
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

		//std::cout << "mapping original " << v->getId() << " to msh " << i << std::endl;

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

bool TecGraf::Msh2D::MainDrive::convertToMsh2D(double *&pts, int &nPts, int *&edges, int &nEdges, Data::VertexVector &map, bool clockWise) const
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

bool TecGraf::Msh2D::MainDrive::convertToMesh(int nPts, double *pts, int nElems, int *elems, int type,
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

						//std::cout << "vertex " << i << " back mapped to boundary vertex " << v->text() << std::endl;

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

			//std::cout << "vertex " << i << " back mapped to boundary vertex " << v->text() << std::endl;

			inner = false;
		}

		if (!v)
		{
			v = new MJMesh::Vertex(x, y, this->idManager->next(0));

			//std::cout << "vertex " << i << " back mapped to inner vertex " << v->text() << std::endl;

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
		    //std::cout << "element " << i << " back mapped to element " << el->text() << std::endl;

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
}

bool TecGraf::Msh2D::MainDrive::executeBoundaryBuilder(bool del)
{
	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setSorted(false);

	this->boundaryBuilder->execute();

	this->boundary = static_cast<MJMesh::Boundary *>(this->boundaryBuilder->getBoundary());

	this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

	if (this->boundary->dimension() != 2)
	{
		return false;
	}

	return true;
}

bool TecGraf::Msh2D::MainDrive::executeWarpBoundaryBuilder(bool del)
{
	static_cast<MJMesh::BoundaryBuilder *>(this->boundContractionWarpBoundaryBuilder)->setSorted(false);

	this->boundContractionWarpBoundaryBuilder->execute();

	this->boundContractionWarpBoundary = static_cast<MJMesh::Boundary *>(this->boundContractionWarpBoundaryBuilder->getBoundary());

	//this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->boundContractionWarpBoundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->boundContractionWarpBoundaryBuilder;

		this->boundContractionWarpBoundaryBuilder = NULL;
	}

	if (this->boundContractionWarpBoundary->dimension() != 2)
	{
		return false;
	}

	return true;
}

bool TecGraf::Msh2D::MainDrive::executeBilinear()
{
	if ((this->bilinearElementType != 3)
#if USE_QUADS
		&& (this->bilinearElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	if ((this->bilinearDiagonalType < 1) || (this->bilinearDiagonalType > 4))
	{
		return false;
	}

	UInt m1 = 0, n1 = 0;

	if ((this->bilinearNumberVerticesDirection1 == 0) || (this->bilinearNumberVerticesDirection2 == 0))
	{
		UInt m2 = 0, n2 = 0;

		UInt incr = 0;

		bool firstNotColinear = false;

		for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if (v->numAdjacentFrontElements() != 2)
			{
				return false;
			}

			MJMesh::FrontElement2D *fel1 = static_cast<MJMesh::FrontElement2D *>(*v->feBegin());
			MJMesh::FrontElement2D *fel2 = static_cast<MJMesh::FrontElement2D *>(*(++v->feBegin()));

			MJMesh::Edge2D *e1 = static_cast<MJMesh::Edge2D *>(fel1->getShape());
			MJMesh::Edge2D *e2 = static_cast<MJMesh::Edge2D *>(fel2->getShape());

			bool colinear = true;

			Real angle = e1->angle(e2);

			if ((angle < M_PI - this->bilinearAngleTolerance) ||
				(angle > M_PI + this->bilinearAngleTolerance))
			{
				colinear = false;
			}

			if (!colinear)
			{
				if (iter == this->boundary->vBegin())
				{
					firstNotColinear = true;
				}
				else
				{
					incr++;
				}
			}

			if ((incr == 0) || (incr == 4))
			{
				m1++;
			}
			else if (incr == 1)
			{
				n1++;
			}
			else if (incr == 2)
			{
				m2++;
			}
			else if (incr == 3)
			{
				n2++;
			}
		}

		if (((firstNotColinear) && (incr != 3)) ||
			((!firstNotColinear) && (incr != 4)))
		{
			return false;
		}

		if ((m1 != m2) || (n1 != n2))
		{
			return false;
		}
	}
	else
	{
		m1 = this->bilinearNumberVerticesDirection1 - 1;
		n1 = this->bilinearNumberVerticesDirection2 - 1;

		if ((2*m1 + 2*n1) != this->boundary->verticesSize())
		{
			return false;
		}
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DBilinear(pts, n1 + 1, m1 + 1, static_cast<int>(this->bilinearElementType), static_cast<int>(this->bilinearDiagonalType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->bilinearElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeTryBilinear()
{
	if ((this->tryBilinearElementType != 3)
#if USE_QUADS
		&& (this->tryBilinearElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	if ((this->tryBilinearDiagonalType < 1) || (this->tryBilinearDiagonalType > 4))
	{
		return false;
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DTryBilinear(pts, nPts, static_cast<int>(this->tryBilinearElementType), static_cast<int>(this->tryBilinearDiagonalType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->tryBilinearElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeCollBilinear()
{
	if ((this->collBilinearElementType != 3)
#if USE_QUADS
		&& (this->collBilinearElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	if ((this->collBilinearDiagonalType < 1) || (this->collBilinearDiagonalType > 4))
	{
		return false;
	}

	UInt m1 = 0, n1 = 0;

	if ((this->collBilinearNumberVerticesDirection1 == 0) || (this->collBilinearNumberVerticesDirection2 == 0))
	{
		UInt m2 = 0, n2 = 0;

		UInt incr = 0;

		bool firstNotColinear = false;

		for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if (v->numAdjacentFrontElements() != 2)
			{
				return false;
			}

			MJMesh::FrontElement2D *fel1 = static_cast<MJMesh::FrontElement2D *>(*v->feBegin());
			MJMesh::FrontElement2D *fel2 = static_cast<MJMesh::FrontElement2D *>(*(++v->feBegin()));

			MJMesh::Edge2D *e1 = static_cast<MJMesh::Edge2D *>(fel1->getShape());
			MJMesh::Edge2D *e2 = static_cast<MJMesh::Edge2D *>(fel2->getShape());

			bool colinear = true;

			Real angle = e1->angle(e2);

			if ((angle < M_PI - this->collBilinearAngleTolerance) ||
				(angle > M_PI + this->collBilinearAngleTolerance))
			{
				colinear = false;
			}

			if (!colinear)
			{
				if (iter == this->boundary->vBegin())
				{
					firstNotColinear = true;
				}
				else
				{
					incr++;
				}
			}

			if ((incr == 0) || (incr == 4))
			{
				m1++;
			}
			else if (incr == 1)
			{
				n1++;
			}
			else if (incr == 2)
			{
				m2++;
			}
			else if (incr == 3)
			{
				n2++;
			}
		}

		if (((firstNotColinear) && (incr != 3)) ||
			((!firstNotColinear) && (incr != 4)))
		{
			return false;
		}

		if ((m1 != m2) || (n2 != 0))
		{
			return false;
		}
	}
	else
	{
		m1 = this->collBilinearNumberVerticesDirection1 - 1;
		n1 = this->collBilinearNumberVerticesDirection2 - 1;

		if ((2*m1 + n1) != this->boundary->verticesSize())
		{
			return false;
		}
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DCollBilinear(pts, n1 + 1, m1 + 1, static_cast<int>(this->collBilinearElementType), static_cast<int>(this->collBilinearDiagonalType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, (static_cast<int>(this->collBilinearElementType) == 4) ? 0 : static_cast<int>(this->collBilinearElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeLoft()
{
	if ((this->loftElementType != 3)
#if USE_QUADS
		&& (this->loftElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	if ((this->loftDiagonalType < 1) || (this->loftDiagonalType > 4))
	{
		return false;
	}

	if (this->loftDirection > 1)
	{
		return false;
	}

	if (this->loftWeight <= Data::getTolerance())
	{
		return false;
	}

	UInt m1 = 0, n1 = 0;

	if ((this->loftNumberVerticesDirection1 == 0) || (this->loftNumberVerticesDirection2 == 0))
	{
		UInt m2 = 0, n2 = 0;

		UInt incr = 0;

		bool firstNotColinear = false;

		for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if (v->numAdjacentFrontElements() != 2)
			{
				return false;
			}

			MJMesh::FrontElement2D *fel1 = static_cast<MJMesh::FrontElement2D *>(*v->feBegin());
			MJMesh::FrontElement2D *fel2 = static_cast<MJMesh::FrontElement2D *>(*(++v->feBegin()));

			MJMesh::Edge2D *e1 = static_cast<MJMesh::Edge2D *>(fel1->getShape());
			MJMesh::Edge2D *e2 = static_cast<MJMesh::Edge2D *>(fel2->getShape());

			bool colinear = true;

			Real angle = e1->angle(e2);

			if ((angle < M_PI - this->loftAngleTolerance) ||
				(angle > M_PI + this->loftAngleTolerance))
			{
				colinear = false;
			}

			if (!colinear)
			{
				if (iter == this->boundary->vBegin())
				{
					firstNotColinear = true;
				}
				else
				{
					incr++;
				}
			}

			if ((incr == 0) || (incr == 4))
			{
				m1++;
			}
			else if (incr == 1)
			{
				n1++;
			}
			else if (incr == 2)
			{
				m2++;
			}
			else if (incr == 3)
			{
				n2++;
			}
		}

		if (((firstNotColinear) && (incr != 3)) ||
			((!firstNotColinear) && (incr != 4)))
		{
			return false;
		}

		if ((m1 != m2) || (n1 != n2))
		{
			return false;
		}
	}
	else
	{
		m1 = this->loftNumberVerticesDirection1 - 1;
		n1 = this->loftNumberVerticesDirection2 - 1;

		if ((2*m1 + 2*n1) != this->boundary->verticesSize())
		{
			return false;
		}
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DLoft(pts, n1 + 1, m1 + 1, 1 - static_cast<int>(this->loftDirection), static_cast<double>(this->loftWeight), static_cast<int>(this->loftElementType), static_cast<int>(this->loftDiagonalType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->loftElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeCollLoft()
{
	if ((this->collLoftElementType != 3)
#if USE_QUADS
		&& (this->collLoftElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	if ((this->collLoftDiagonalType < 1) || (this->collLoftDiagonalType > 4))
	{
		return false;
	}

	if (this->collLoftWeight <= Data::getTolerance())
	{
		return false;
	}

	UInt m1 = 0, n1 = 0;

	if ((this->collLoftNumberVerticesDirection1 == 0) || (this->collLoftNumberVerticesDirection2 == 0))
	{
		UInt m2 = 0, n2 = 0;

		UInt incr = 0;

		bool firstNotColinear = false;

		for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if (v->numAdjacentFrontElements() != 2)
			{
				return false;
			}

			MJMesh::FrontElement2D *fel1 = static_cast<MJMesh::FrontElement2D *>(*v->feBegin());
			MJMesh::FrontElement2D *fel2 = static_cast<MJMesh::FrontElement2D *>(*(++v->feBegin()));

			MJMesh::Edge2D *e1 = static_cast<MJMesh::Edge2D *>(fel1->getShape());
			MJMesh::Edge2D *e2 = static_cast<MJMesh::Edge2D *>(fel2->getShape());

			bool colinear = true;

			Real angle = e1->angle(e2);

			if ((angle < M_PI - this->collLoftAngleTolerance) ||
				(angle > M_PI + this->collLoftAngleTolerance))
			{
				colinear = false;
			}

			if (!colinear)
			{
				if (iter == this->boundary->vBegin())
				{
					firstNotColinear = true;
				}
				else
				{
					incr++;
				}
			}

			if ((incr == 0) || (incr == 4))
			{
				m1++;
			}
			else if (incr == 1)
			{
				n1++;
			}
			else if (incr == 2)
			{
				m2++;
			}
			else if (incr == 3)
			{
				n2++;
			}
		}

		if (((firstNotColinear) && (incr != 3)) ||
			((!firstNotColinear) && (incr != 4)))
		{
			return false;
		}

		if ((m1 != m2) || (n2 != 0))
		{
			return false;
		}
	}
	else
	{
		m1 = this->collLoftNumberVerticesDirection1 - 1;
		n1 = this->collLoftNumberVerticesDirection2 - 1;

		if ((2*m1 + n1) != this->boundary->verticesSize())
		{
			return false;
		}
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DCollLoft(pts, n1 + 1, m1 + 1, static_cast<double>(this->collLoftWeight), static_cast<int>(this->collLoftElementType), static_cast<int>(this->collLoftDiagonalType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->collLoftElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeTrilinear()
{
	if ((this->trilinearElementType != 3)
#if USE_QUADS
		&& (this->trilinearElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	UInt m1 = 0;

	if (this->trilinearNumberVerticesDirection == 0)
	{
		UInt m2 = 0, m3 = 0;

		UInt incr = 0;

		bool firstNotColinear = false;

		for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

			if (v->numAdjacentFrontElements() != 2)
			{
				return false;
			}

			MJMesh::FrontElement2D *fel1 = static_cast<MJMesh::FrontElement2D *>(*v->feBegin());
			MJMesh::FrontElement2D *fel2 = static_cast<MJMesh::FrontElement2D *>(*(++v->feBegin()));

			MJMesh::Edge2D *e1 = static_cast<MJMesh::Edge2D *>(fel1->getShape());
			MJMesh::Edge2D *e2 = static_cast<MJMesh::Edge2D *>(fel2->getShape());

			bool colinear = true;

			Real angle = e1->angle(e2);

			if ((angle < M_PI - this->trilinearAngleTolerance) ||
				(angle > M_PI + this->trilinearAngleTolerance))
			{
				colinear = false;
			}

			if (!colinear)
			{
				if (iter == this->boundary->vBegin())
				{
					firstNotColinear = true;
				}
				else
				{
					incr++;
				}
			}

			if ((incr == 0) || (incr == 3))
			{
				m1++;
			}
			else if (incr == 1)
			{
				m2++;
			}
			else if (incr == 2)
			{
				m3++;
			}
		}

		if (((firstNotColinear) && (incr != 3)) ||
			((!firstNotColinear) && (incr != 4)))
		{
			return false;
		}

		if ((m1 != m2) || (m1 != m3))
		{
			return false;
		}
	}
	else
	{
		m1 = this->trilinearNumberVerticesDirection - 1;

		if ((3*m1) != this->boundary->verticesSize())
		{
			return false;
		}
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DTrilinear(pts, m1 + 1, static_cast<int>(this->trilinearElementType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->trilinearElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeTryTrilinear()
{
	if ((this->tryTrilinearElementType != 3)
#if USE_QUADS
		&& (this->tryTrilinearElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success =
		Msh2DTryTrilinear(pts, nPts, static_cast<int>(this->tryTrilinearElementType),
			&nGenPts, &nGenElems, &genPts, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->tryTrilinearElementType), map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeBoundContraction()
{
	double *pts = NULL;
	int nLoops = 0;
	int *loops = NULL;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nLoops, loops, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	Msh2DBoundContraction(nLoops, loops, pts,
		this->boundContractionInternalPoints ? 1 : 0, 0,
		this->boundContractionQuadtreeInternalPoints ? 1 : 0, 3, NULL,
		&nGenPts, &genPts, &nGenElems, &genElems);

	int success = (nGenElems == 0) ? 0 : 1;

	delete [] pts;
	delete [] loops;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 3, map, true, false)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeBoundContractionWarp()
{
	if (!this->getBoundContractionWarpBoundary())
	{
		return false;
	}

	double *pts = NULL, *ptsWarp = NULL;
	int nLoops = 0, nLoopsWarp = 0;
	int *loops = NULL, *loopsWarp = NULL;

	Data::VertexVector map, mapWarp;

	if (!this->convertToMsh2D(pts, nLoops, loops, map, true))
	{
		return false;
	}

	if (!this->executeWarpBoundaryBuilder(true))
	{
		return false;
	}

	MJMesh::Boundary *originalBoundary = this->boundary;
	this->boundary = this->boundContractionWarpBoundary;

	if (!this->convertToMsh2D(ptsWarp, nLoopsWarp, loopsWarp, mapWarp, true))
	{
		this->boundary = originalBoundary;

		delete [] pts;
		delete [] loops;

		return false;
	}

	this->boundary = originalBoundary;

	if (nLoops != nLoopsWarp)
	{
		delete [] pts;
		delete [] loops;
		delete [] ptsWarp;
		delete [] loopsWarp;

		return false;
	}

	for (int i = 0; i < nLoops; i++)
	{
		if (loops[i] != loopsWarp[i])
		{
			delete [] pts;
			delete [] loops;
			delete [] ptsWarp;
			delete [] loopsWarp;

			return false;
		}
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL, *genPtsWarp = NULL;
	int *genElems = NULL;

	Msh2DBoundContractionWarp(nLoops, loops, pts, ptsWarp,
		this->boundContractionWarpInternalPoints ? 1 : 0, this->boundContractionWarpBoundaryPoints ? 1 : 0, 0,
		this->boundContractionWarpQuadtreeInternalPoints ? 1 : 0, 3, NULL,
		&nGenPts, &genPts, &genPtsWarp, &nGenElems, &genElems);

	int success = (nGenElems == 0) ? 0 : 1;

	delete [] pts;
	delete [] loops;
	delete [] ptsWarp;
	delete [] loopsWarp;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 3, map, true, false, true)))
	{
		Data::Mesh *originalMesh = this->mesh;
		this->mesh = this->boundContractionWarpMesh;

		this->boundary = this->boundContractionWarpBoundary;

		if (this->convertToMesh(nGenPts, genPtsWarp, nGenElems, genElems, 3, mapWarp, true, false, true))
		{
			value = true;
		}

		this->boundary = originalBoundary;

		this->boundContractionWarpMesh = this->mesh;
		this->mesh = originalMesh;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genPtsWarp)
	{
		free(genPtsWarp);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeQuadBound()
{
	if ((this->quadBoundElementType != 3)
#if USE_QUADS
		&& (this->quadBoundElementType != 4)
#endif //#if USE_QUADS
	)
	{
		return false;
	}

	double *pts = NULL;
	int nLoops = 0;
	int *loops = NULL;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nLoops, loops, map, true))
	{
		return false;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success = Msh2DQuadBound(nLoops, loops, pts,
		static_cast<int>(this->quadBoundElementType),
		this->quadBoundRefineQuadtree ? 1 : 0,
		&nGenPts, &genPts, &nGenElems, &genElems);

	delete [] pts;
	delete [] loops;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, static_cast<int>(this->quadBoundElementType), map, true, false)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeShape()
{
	if (this->shapeRefinementFactor < 0.1)
	{
		return false;
	}

	double *pts = NULL;
	int nLoops = 0;
	int *loops = NULL;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nLoops, loops, map, true))
	{
		return false;
	}

	double params[1] = {this->shapeInternalPoints ? 1.0 : 0.0};

	Msh2DEdgeParams(1, params);
	Msh2DSetRefFactor(static_cast<double>(this->shapeRefinementFactor));

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success = Msh2DShape(nLoops, loops, pts, 3,
		&nGenPts, &genPts, &nGenElems, &genElems);

	delete [] pts;
	delete [] loops;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 3, map, true, false)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeEdge()
{
	if (this->edgeRefinementFactor < 0.1)
	{
		return false;
	}

	double *pts = NULL;
	int nPts = 0;
	int *edges = NULL;
	int nEdges = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, edges, nEdges, map, true))
	{
		return false;
	}

	double params[1] = {this->edgeInternalPoints ? 1.0 : 0.0};

	Msh2DEdgeParams(1, params);
	Msh2DSetRefFactor(static_cast<double>(this->edgeRefinementFactor));

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success = Msh2DEdge(nPts, pts, nEdges, 0, edges, 3,
		&nGenPts, &genPts, &nGenElems, &genElems);

	delete [] pts;
	delete [] edges;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 3, map, true, false)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

#if USE_QUADS
bool TecGraf::Msh2D::MainDrive::executeQuadSeam()
{
	if (this->quadSeamRefinementFactor < 0.1)
	{
		return false;
	}

	double *pts = NULL;
	int nLoops = 0;
	int *loops = NULL;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nLoops, loops, map, true))
	{
		return false;
	}

	double params[1] = {this->quadSeamInternalPoints ? 1.0 : 0.0};

	Msh2DEdgeParams(1, params);
	Msh2DSetRefFactor(static_cast<double>(this->quadSeamRefinementFactor));

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success = Msh2DQuadSeam(nLoops, loops, pts, 4,
		&nGenPts, &genPts, &nGenElems, &genElems);

	delete [] pts;
	delete [] loops;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 4, map, true, false)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeQuadSeamEdge()
{
	if (this->quadSeamEdgeRefinementFactor < 0.1)
	{
		return false;
	}

	double *pts = NULL;
	int nPts = 0;
	int *edges = NULL;
	int nEdges = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, edges, nEdges, map, true))
	{
		return false;
	}

	double params[1] = {this->quadSeamEdgeInternalPoints ? 1.0 : 0.0};

	Msh2DEdgeParams(1, params);
	Msh2DSetRefFactor(static_cast<double>(this->quadSeamEdgeRefinementFactor));

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success = Msh2DQuadSeamEdge(nPts, pts, nEdges, edges,
		&nGenPts, &genPts, &nGenElems, &genElems);

	delete [] pts;
	delete [] edges;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 4, map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}

bool TecGraf::Msh2D::MainDrive::executeTemplate()
{
	if ((this->templateNumberSides < 2) || (this->templateNumberSides > 4))
	{
		return false;
	}

	double *pts = NULL;
	int nPts = 0;

	Data::VertexVector map;

	if (!this->convertToMsh2D(pts, nPts, map, true))
	{
		return false;
	}

	int total = 0;

	for (UInt i = 0; i < this->templateNumberSides; i++)
	{
		total += static_cast<int>(this->templateNumberSubdivision[i]);
	}

	if (total != nPts)
	{
		delete [] pts;

		return false;
	}

	if ((this->templateU1 != 0.0) || (this->templateV1 != 0.0) || (this->templateU2 != 0.0) || (this->templateV2 != 0.0))
	{
		Msh2DTemplateSetParam(0, 1.0);

		if (this->templateU1 != 0.0)
		{
			Msh2DTemplateSetParam(1, static_cast<double>(this->templateU1));
		}

		if (this->templateV1 != 0.0)
		{
			Msh2DTemplateSetParam(2, static_cast<double>(this->templateV1));
		}

		if (this->templateU2 != 0.0)
		{
			Msh2DTemplateSetParam(3, static_cast<double>(this->templateU2));
		}

		if (this->templateV2 != 0.0)
		{
			Msh2DTemplateSetParam(4, static_cast<double>(this->templateV2));
		}
	}
	else
	{
		Msh2DTemplateSetParam(0, 0.0);
	}

	int subdivision[4];

	for (UInt i = 0; i < 4; i++)
	{
		subdivision[i] = (i < this->templateNumberSides) ? static_cast<int>(this->templateNumberSubdivision[i]) : 0;
	}

	int nGenPts = 0, nGenElems = 0;
	double *genPts = NULL;
	int *genElems = NULL;

	int success = Msh2DTemplate(static_cast<int>(this->templateNumberSides), subdivision,
		2, 4, this->templateSmooth ? 1 : 0, pts,
		&nGenPts, &genPts, &nGenElems, &genElems);

	delete [] pts;

	bool value = false;

	if ((success != 0) &&
		(this->convertToMesh(nGenPts, genPts, nGenElems, genElems, 4, map, true, true)))
	{
		value = true;
	}

	if (genPts)
	{
		free(genPts);
	}

	if (genElems)
	{
		free(genElems);
	}

	return value;
}
#endif //#if USE_QUADS

Performer::IdManager *TecGraf::Msh2D::MainDrive::makeIdManager(const Performer::IdManager *idManager) const
{
	Performer::IdManager *idm = new Performer::SequentialIdManager(1, 3);

	if (idManager)
	{
		idm->setId(0, idManager->current(0) + 1);
		idm->setId(1, idManager->current(1) + 1);
	}

	return idm;
}

MJMesh::BoundaryBuilder *TecGraf::Msh2D::MainDrive::makeBoundaryBuilder() const
{
	return new MJMesh::BoundaryBuilder(false);
}

#endif //#if USE_TECGRAF_MSH_2D
