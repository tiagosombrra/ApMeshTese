#include "CRAbMesh/TecGrafMsh2DModule.h"

#if USE_GUI
#if USE_TECGRAF_MSH_2D

#include <wx/gbsizer.h>
#include <wx/choicebk.h>
#include "Data/OStream.h"
#include "Data/Box.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/Element2D.h"
#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/Camera.h"
#include "CRAbMesh/ViewModule.h"
#include "CRAbMesh/TreeItemData.h"
#include "TecGraf/Definitions.h"
#include "TecGraf/Msh2D/MainDrive.h"

CRAbMesh::TecGrafMsh2DModule::TecGrafMsh2DModule(Frame *frame, Canvas *canvas, bool makeMainDrive) :
	CRAbMesh::MeshModule::MeshModule(frame, canvas)
{
	WARP_VERTICES = "Warped vertices";
	WARP_FRONT_ELEMENTS = "Warped front elements";
	WARP_ELEMENTS = "Warped elements";

	DRAW_WARP_BOUNDARY = "Draw warp boundary";
	DRAW_WARP_MESH     = "Draw warp mesh";
	DRAW_DEFORMED_MESH = "Draw deformed mesh";

	this->InsertOption(WARP_VERTICES,       TREE_CELLS,          TYPE_TREE_ITEM);
	this->InsertOption(WARP_FRONT_ELEMENTS, WARP_VERTICES,       TYPE_TREE_ITEM);
	this->InsertOption(WARP_ELEMENTS,       WARP_FRONT_ELEMENTS, TYPE_TREE_ITEM);
	this->InsertOption(DRAW_WARP_BOUNDARY,  DRAW_MESH,          TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_WARP_MESH,      DRAW_WARP_BOUNDARY, TYPE_MENU_AND_CHECK_ITEM);
	this->InsertOption(DRAW_DEFORMED_MESH,  DRAW_WARP_MESH,     TYPE_MENU_AND_CHECK_ITEM);

	this->SetConfiguration(DRAW_WARP_BOUNDARY, true, false);
	this->SetConfiguration(DRAW_WARP_MESH,     true, false);
	this->SetConfiguration(DRAW_DEFORMED_MESH, true, false);

	this->SetOption(VERTICES, true);
	this->SetOption(ELEMENTS, true);
	this->SetOption(FRONT_ELEMENTS, true);
	this->SetOption(TREE_CELLS, false);

	this->SetOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, true, false);
	this->SetOption(HIGHLIGHT_ADJACENT_ELEMENTS, true, false);

	this->SetOption(DRAW_NORMAL, true, true);
	this->SetOption(DRAW_DISCRETIZATION, true, false);

	this->SetOption(DRAW_BOUNDARY, true, true);
	this->SetOption(DRAW_TREE, false, false);
	this->SetOption(DRAW_FRONT, true, true);
	this->SetOption(DRAW_MESH, true, true);

	this->SetOption(NOT_ADDING_TREE_ITEMS, true, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, true, false);

	this->SetOption(WARP_VERTICES,       true);
	this->SetOption(WARP_FRONT_ELEMENTS, true);
	this->SetOption(WARP_ELEMENTS,       true);
	this->SetOption(DRAW_WARP_BOUNDARY,  true, true);
	this->SetOption(DRAW_WARP_MESH,      true, true);
	this->SetOption(DRAW_DEFORMED_MESH,  true, true);

	if (makeMainDrive)
	{
		this->MakeMainDrive();

		this->MakeMenus();

		this->MakePanels();
	}

	this->useRedirector = false;

	this->deformed = NULL;
}

CRAbMesh::TecGrafMsh2DModule::~TecGrafMsh2DModule()
{

}

wxString CRAbMesh::TecGrafMsh2DModule::Name() const
{
	return _("TecGraf Msh2D");
}

wxMenu *CRAbMesh::TecGrafMsh2DModule::GetMenu(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("&Configuration");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->menus.size()) ? this->menus[i] : NULL;
}

wxAuiToolBar *CRAbMesh::TecGrafMsh2DModule::GetToolbar(unsigned int /*i*/, wxString &title) const
{
	title = _("");

	return NULL;
}

wxPanel *CRAbMesh::TecGrafMsh2DModule::GetPanel(unsigned int i, wxString &title) const
{
	switch (i)
	{
	case 0:
		title = _("Configuration");
		break;
	case 1:
		title = _("Mesh items");
		break;
	case 2:
		title = _("Log");
		break;
	default:
		title = _("");
		break;
	}

	return (i < this->panels.size()) ? this->panels[i] : NULL;
}

bool CRAbMesh::TecGrafMsh2DModule::LoadFile(const std::string &filename)
{
	bool loaded = CRAbMesh::MeshModule::LoadFile(filename);

	if (loaded)
	{
		//Module::LoadFile(filename);

		this->FillTree();

		//TecGraf::Msh2D::MainDrive *drive = dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive);

		this->frame->GetViewModule()->SetDepthTestEnabled(false);
		this->frame->GetViewModule()->SetCullFaceEnabled(false);
	}

	return loaded;
}

bool CRAbMesh::TecGrafMsh2DModule::SaveFile(const std::string &filename)
{
	this->drive->setOutputFileName(filename);

	bool saved = this->drive->saveFile();

	this->drive->setOutputFileName("");

	return saved;
}

bool CRAbMesh::TecGrafMsh2DModule::LoadWarpFile(const std::string &filename)
{
    dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundContractionWarpLoadingWarpBoundary(true);

    bool opened = this->LoadFile(filename);

    dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundContractionWarpLoadingWarpBoundary(false);

    return opened;
}

void CRAbMesh::TecGrafMsh2DModule::SetParameters(bool firstTime)
{
	if (firstTime)
	{
		TecGraf::Msh2D::MainDrive *drive = dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive);

		if (this->bcbAlgorithm->GetSelection() == 0)
		{
			unsigned long int elementType = 0;
			unsigned long int diagonalType = 0;
			double angle = 0.0;
			unsigned long int numberVerticesDirection1 = 0;
			unsigned long int numberVerticesDirection2 = 0;

			this->tcBilinearElementType->GetValue().ToULong(&elementType);
			this->tcBilinearDiagonalType->GetValue().ToULong(&diagonalType);
			this->tcBilinearAngleTolerance->GetValue().ToDouble(&angle);
			this->tcBilinearNumberVerticesDirection1->GetValue().ToULong(&numberVerticesDirection1);
			this->tcBilinearNumberVerticesDirection2->GetValue().ToULong(&numberVerticesDirection2);

            drive->setAlgorithm(TecGraf::Msh2D::BILINEAR);

            drive->setBilinearElementType(static_cast<UInt>(elementType));
            drive->setBilinearDiagonalType(static_cast<UInt>(diagonalType));
            drive->setBilinearAngleTolerance(static_cast<Real>(angle));
            drive->setBilinearNumberVerticesDirection1(static_cast<UInt>(numberVerticesDirection1));
            drive->setBilinearNumberVerticesDirection2(static_cast<UInt>(numberVerticesDirection2));
		}
		else if (this->bcbAlgorithm->GetSelection() == 1)
		{
			unsigned long int elementType = 0;
			unsigned long int diagonalType = 0;

			this->tcTryBilinearElementType->GetValue().ToULong(&elementType);
			this->tcTryBilinearDiagonalType->GetValue().ToULong(&diagonalType);

            drive->setAlgorithm(TecGraf::Msh2D::TRY_BILINEAR);

            drive->setTryBilinearElementType(static_cast<UInt>(elementType));
            drive->setTryBilinearDiagonalType(static_cast<UInt>(diagonalType));
		}
		else if (this->bcbAlgorithm->GetSelection() == 2)
		{
			unsigned long int elementType = 0;
			unsigned long int diagonalType = 0;
			double angle = 0.0;
			unsigned long int numberVerticesDirection1 = 0;
			unsigned long int numberVerticesDirection2 = 0;

			this->tcCollBilinearElementType->GetValue().ToULong(&elementType);
			this->tcCollBilinearDiagonalType->GetValue().ToULong(&diagonalType);
			this->tcCollBilinearAngleTolerance->GetValue().ToDouble(&angle);
			this->tcCollBilinearNumberVerticesDirection1->GetValue().ToULong(&numberVerticesDirection1);
			this->tcCollBilinearNumberVerticesDirection2->GetValue().ToULong(&numberVerticesDirection2);

            drive->setAlgorithm(TecGraf::Msh2D::COLL_BILINEAR);

            drive->setCollBilinearElementType(static_cast<UInt>(elementType));
            drive->setCollBilinearDiagonalType(static_cast<UInt>(diagonalType));
            drive->setCollBilinearAngleTolerance(static_cast<Real>(angle));
            drive->setCollBilinearNumberVerticesDirection1(static_cast<UInt>(numberVerticesDirection1));
            drive->setCollBilinearNumberVerticesDirection2(static_cast<UInt>(numberVerticesDirection2));
		}
		if (this->bcbAlgorithm->GetSelection() == 3)
		{
			unsigned long int elementType = 0;
			unsigned long int diagonalType = 0;
			double angle = 0.0;
			unsigned long int numberVerticesDirection1 = 0;
			unsigned long int numberVerticesDirection2 = 0;
			unsigned long int direction = 0;
			double weight = 0;

			this->tcLoftElementType->GetValue().ToULong(&elementType);
			this->tcLoftDiagonalType->GetValue().ToULong(&diagonalType);
			this->tcLoftAngleTolerance->GetValue().ToDouble(&angle);
			this->tcLoftNumberVerticesDirection1->GetValue().ToULong(&numberVerticesDirection1);
			this->tcLoftNumberVerticesDirection2->GetValue().ToULong(&numberVerticesDirection2);
			this->tcLoftDirection->GetValue().ToULong(&direction);
			this->tcLoftWeight->GetValue().ToDouble(&weight);

            drive->setAlgorithm(TecGraf::Msh2D::LOFT);

            drive->setLoftElementType(static_cast<UInt>(elementType));
            drive->setLoftDiagonalType(static_cast<UInt>(diagonalType));
            drive->setLoftAngleTolerance(static_cast<Real>(angle));
            drive->setLoftNumberVerticesDirection1(static_cast<UInt>(numberVerticesDirection1));
            drive->setLoftNumberVerticesDirection2(static_cast<UInt>(numberVerticesDirection2));
            drive->setLoftDirection(static_cast<UInt>(direction));
            drive->setLoftWeight(static_cast<Real>(weight));
		}
		else if (this->bcbAlgorithm->GetSelection() == 4)
		{
			unsigned long int elementType = 0;
			unsigned long int diagonalType = 0;
			double angle = 0.0;
			unsigned long int numberVerticesDirection1 = 0;
			unsigned long int numberVerticesDirection2 = 0;
			double weight = 0;

			this->tcCollLoftElementType->GetValue().ToULong(&elementType);
			this->tcCollLoftDiagonalType->GetValue().ToULong(&diagonalType);
			this->tcCollLoftAngleTolerance->GetValue().ToDouble(&angle);
			this->tcCollLoftNumberVerticesDirection1->GetValue().ToULong(&numberVerticesDirection1);
			this->tcCollLoftNumberVerticesDirection2->GetValue().ToULong(&numberVerticesDirection2);
			this->tcCollLoftWeight->GetValue().ToDouble(&weight);

            drive->setAlgorithm(TecGraf::Msh2D::COLL_LOFT);

            drive->setCollLoftElementType(static_cast<UInt>(elementType));
            drive->setCollLoftDiagonalType(static_cast<UInt>(diagonalType));
            drive->setCollLoftAngleTolerance(static_cast<Real>(angle));
            drive->setCollLoftNumberVerticesDirection1(static_cast<UInt>(numberVerticesDirection1));
            drive->setCollLoftNumberVerticesDirection2(static_cast<UInt>(numberVerticesDirection2));
            drive->setCollLoftWeight(static_cast<Real>(weight));
		}
		else if (this->bcbAlgorithm->GetSelection() == 5)
		{
			unsigned long int elementType = 0;
			double angle = 0.0;
			unsigned long int numberVerticesDirection = 0;

			this->tcTrilinearElementType->GetValue().ToULong(&elementType);
			this->tcTrilinearAngleTolerance->GetValue().ToDouble(&angle);
			this->tcTrilinearNumberVerticesDirection->GetValue().ToULong(&numberVerticesDirection);

            drive->setAlgorithm(TecGraf::Msh2D::TRILINEAR);

            drive->setTrilinearElementType(static_cast<UInt>(elementType));
            drive->setTrilinearAngleTolerance(static_cast<Real>(angle));
            drive->setTrilinearNumberVerticesDirection(static_cast<UInt>(numberVerticesDirection));
		}
		else if (this->bcbAlgorithm->GetSelection() == 6)
		{
			unsigned long int elementType = 0;

			this->tcTryTrilinearElementType->GetValue().ToULong(&elementType);

            drive->setAlgorithm(TecGraf::Msh2D::TRY_TRILINEAR);

            drive->setTryTrilinearElementType(static_cast<UInt>(elementType));
		}
		else if (this->bcbAlgorithm->GetSelection() == 7)
		{
			bool internalPoints = false;
			bool quadtreeInternalPoints = false;

			internalPoints = this->cbBoundContractionInternalVertices->GetValue();
			quadtreeInternalPoints = this->cbBoundContractionInternalVerticesWithQuadtree->GetValue();

            drive->setAlgorithm(TecGraf::Msh2D::BOUND_CONTRACTION);

            drive->setBoundContractionInternalPoints(internalPoints);
            drive->setBoundContractionQuadtreeInternalPoints(quadtreeInternalPoints);
		}
		else if (this->bcbAlgorithm->GetSelection() == 8)
		{
			bool internalPoints = false;
			bool quadtreeInternalPoints = false;
			bool boundaryPoints = false;
			//bool invertOrder = false;

			internalPoints = this->cbBoundContractionWarpInternalVertices->GetValue();
			quadtreeInternalPoints = this->cbBoundContractionWarpInternalVerticesWithQuadtree->GetValue();
			boundaryPoints = this->cbBoundContractionWarpBoundaryVertices->GetValue();
			//invertOrder = this->cbBoundContractionWarpInvertReadOrderOnTxt->GetValue();

            drive->setAlgorithm(TecGraf::Msh2D::BOUND_CONTRACTION_WARP);

            drive->setBoundContractionWarpInternalPoints(internalPoints);
            drive->setBoundContractionWarpQuadtreeInternalPoints(quadtreeInternalPoints);
            drive->setBoundContractionWarpBoundaryPoints(boundaryPoints);
            //drive->setBoundContractionWarpInvertReadOrderOnTxt(invertOrder);
		}
		else if (this->bcbAlgorithm->GetSelection() == 9)
		{
			unsigned long int elementType = 0;
			bool refineQuadtree = false;

			this->tcQuadBoundElementType->GetValue().ToULong(&elementType);
			refineQuadtree = this->cbQuadBoundRefineQuadtree->GetValue();

            drive->setAlgorithm(TecGraf::Msh2D::QUAD_BOUND);

            drive->setQuadBoundElementType(static_cast<UInt>(elementType));
            drive->setQuadBoundRefineQuadtree(refineQuadtree);
		}
		else if (this->bcbAlgorithm->GetSelection() == 10)
		{
			bool internalPoints = false;
			double refinementFactor = 0.0;

			internalPoints = this->cbShapeInternalPoints->GetValue();
			this->tcShapeRefinementFactor->GetValue().ToDouble(&refinementFactor);

            drive->setAlgorithm(TecGraf::Msh2D::SHAPE);

            drive->setShapeInternalPoints(internalPoints);
            drive->setShapeRefinementFactor(refinementFactor);
		}
		else if (this->bcbAlgorithm->GetSelection() == 11)
		{
			bool internalPoints = false;
			double refinementFactor = 0.0;

			internalPoints = this->cbEdgeInternalPoints->GetValue();
			this->tcEdgeRefinementFactor->GetValue().ToDouble(&refinementFactor);

            drive->setAlgorithm(TecGraf::Msh2D::EDGE);

            drive->setEdgeInternalPoints(internalPoints);
            drive->setEdgeRefinementFactor(refinementFactor);
		}
#if USE_QUADS
		else if (this->bcbAlgorithm->GetSelection() == 12)
		{
			bool internalPoints = false;
			double refinementFactor = 0.0;

			internalPoints = this->cbQuadSeamInternalPoints->GetValue();
			this->tcQuadSeamRefinementFactor->GetValue().ToDouble(&refinementFactor);

            drive->setAlgorithm(TecGraf::Msh2D::QUAD_SEAM);

            drive->setQuadSeamInternalPoints(internalPoints);
            drive->setQuadSeamRefinementFactor(refinementFactor);
		}
		else if (this->bcbAlgorithm->GetSelection() == 13)
		{
			bool internalPoints = false;
			double refinementFactor = 0.0;

			internalPoints = this->cbQuadSeamEdgeInternalPoints->GetValue();
			this->tcQuadSeamEdgeRefinementFactor->GetValue().ToDouble(&refinementFactor);

            drive->setAlgorithm(TecGraf::Msh2D::QUAD_SEAM_EDGE);

            drive->setQuadSeamEdgeInternalPoints(internalPoints);
            drive->setQuadSeamEdgeRefinementFactor(refinementFactor);
		}
		else if (this->bcbAlgorithm->GetSelection() == 14)
		{
			unsigned long int numberSides = 0;
			unsigned long int numberSubdivision[4] = {0, 0, 0, 0};
			bool smooth = false;
			double u1 = 0.0;
			double v1 = 0.0;
			double u2 = 0.0;
			double v2 = 0.0;

			this->tcTemplateNumberSides->GetValue().ToULong(&numberSides);
			this->tcTemplateNumberSubdivision[0]->GetValue().ToULong(&numberSubdivision[0]);
			this->tcTemplateNumberSubdivision[1]->GetValue().ToULong(&numberSubdivision[1]);
			this->tcTemplateNumberSubdivision[2]->GetValue().ToULong(&numberSubdivision[2]);
			this->tcTemplateNumberSubdivision[3]->GetValue().ToULong(&numberSubdivision[3]);
			smooth = this->cbTemplateSmooth->GetValue();
			this->tcTemplateU1->GetValue().ToDouble(&u1);
			this->tcTemplateV1->GetValue().ToDouble(&v1);
			this->tcTemplateU2->GetValue().ToDouble(&u2);
			this->tcTemplateV2->GetValue().ToDouble(&v2);

            drive->setAlgorithm(TecGraf::Msh2D::TEMPLATE);

            drive->setTemplateNumberSides(static_cast<UInt>(numberSides));
			drive->setTemplateNumberSubdivision(0, static_cast<UInt>(numberSubdivision[0]));
			drive->setTemplateNumberSubdivision(1, static_cast<UInt>(numberSubdivision[1]));
			drive->setTemplateNumberSubdivision(2, static_cast<UInt>(numberSubdivision[2]));
			drive->setTemplateNumberSubdivision(3, static_cast<UInt>(numberSubdivision[3]));
			drive->setTemplateSmooth(smooth);
			drive->setTemplateU1(u1);
			drive->setTemplateV1(v1);
			drive->setTemplateU2(u2);
			drive->setTemplateV2(v2);
		}
#endif //#if USE_QUADS
	}
}

void CRAbMesh::TecGrafMsh2DModule::Execute()
{
	if (this->useRedirector)
    {
        this->redirector.Start(this->tcLog, &std::cout);
    }

	this->frame->GetViewModule()->Execute();

	//disable items
	//if (this->bcbAlgorithm) this->bcbAlgorithm->Enable(false);

	if (this->tcBilinearElementType) this->tcBilinearElementType->Enable(false);
	if (this->tcBilinearDiagonalType) this->tcBilinearDiagonalType->Enable(false);
	if (this->tcBilinearAngleTolerance) this->tcBilinearAngleTolerance->Enable(false);
	if (this->tcBilinearNumberVerticesDirection1) this->tcBilinearNumberVerticesDirection1->Enable(false);
	if (this->tcBilinearNumberVerticesDirection2) this->tcBilinearNumberVerticesDirection2->Enable(false);

	if (this->tcTryBilinearElementType) this->tcTryBilinearElementType->Enable(false);
	if (this->tcTryBilinearDiagonalType) this->tcTryBilinearDiagonalType->Enable(false);

	if (this->tcCollBilinearElementType) this->tcCollBilinearElementType->Enable(false);
	if (this->tcCollBilinearDiagonalType) this->tcCollBilinearDiagonalType->Enable(false);
	if (this->tcCollBilinearAngleTolerance) this->tcCollBilinearAngleTolerance->Enable(false);
	if (this->tcCollBilinearNumberVerticesDirection1) this->tcCollBilinearNumberVerticesDirection1->Enable(false);
	if (this->tcCollBilinearNumberVerticesDirection2) this->tcCollBilinearNumberVerticesDirection2->Enable(false);

	if (this->tcLoftElementType) this->tcLoftElementType->Enable(false);
	if (this->tcLoftDiagonalType) this->tcLoftDiagonalType->Enable(false);
	if (this->tcLoftAngleTolerance) this->tcLoftAngleTolerance->Enable(false);
	if (this->tcLoftNumberVerticesDirection1) this->tcLoftNumberVerticesDirection1->Enable(false);
	if (this->tcLoftNumberVerticesDirection2) this->tcLoftNumberVerticesDirection2->Enable(false);
	if (this->tcLoftDirection) this->tcLoftDirection->Enable(false);
	if (this->tcLoftWeight) this->tcLoftWeight->Enable(false);

	if (this->tcCollLoftElementType) this->tcCollLoftElementType->Enable(false);
	if (this->tcCollLoftDiagonalType) this->tcCollLoftDiagonalType->Enable(false);
	if (this->tcCollLoftAngleTolerance) this->tcCollLoftAngleTolerance->Enable(false);
	if (this->tcCollLoftNumberVerticesDirection1) this->tcCollLoftNumberVerticesDirection1->Enable(false);
	if (this->tcCollLoftNumberVerticesDirection2) this->tcCollLoftNumberVerticesDirection2->Enable(false);
	if (this->tcCollLoftWeight) this->tcCollLoftWeight->Enable(false);

	if (this->tcTrilinearElementType) this->tcTrilinearElementType->Enable(false);
	if (this->tcTrilinearAngleTolerance) this->tcTrilinearAngleTolerance->Enable(false);
	if (this->tcTrilinearNumberVerticesDirection) this->tcTrilinearNumberVerticesDirection->Enable(false);

	if (this->tcTryTrilinearElementType) this->tcTryTrilinearElementType->Enable(false);

	if (this->cbBoundContractionInternalVertices) this->cbBoundContractionInternalVertices->Enable(false);
	if (this->cbBoundContractionInternalVerticesWithQuadtree) this->cbBoundContractionInternalVerticesWithQuadtree->Enable(false);

	if (this->cbBoundContractionWarpInternalVertices) this->cbBoundContractionWarpInternalVertices->Enable(false);
	if (this->cbBoundContractionWarpInternalVerticesWithQuadtree) this->cbBoundContractionWarpInternalVerticesWithQuadtree->Enable(false);
	if (this->cbBoundContractionWarpBoundaryVertices) this->cbBoundContractionWarpBoundaryVertices->Enable(false);
	//if (this->cbBoundContractionWarpInvertReadOrderOnTxt) this->cbBoundContractionWarpInvertReadOrderOnTxt->Enable(false);
	if (this->bBoundContractionWarpLoadWarpBoundary) this->bBoundContractionWarpLoadWarpBoundary->Enable(false);

	if (this->tcQuadBoundElementType) this->tcQuadBoundElementType->Enable(false);
	if (this->cbQuadBoundRefineQuadtree) this->cbQuadBoundRefineQuadtree->Enable(false);

	if (this->cbShapeInternalPoints) this->cbShapeInternalPoints->Enable(false);
	if (this->tcShapeRefinementFactor) this->tcShapeRefinementFactor->Enable(false);

	if (this->cbEdgeInternalPoints) this->cbEdgeInternalPoints->Enable(false);
	if (this->tcEdgeRefinementFactor) this->tcEdgeRefinementFactor->Enable(false);

#if USE_QUADS
	if (this->cbQuadSeamInternalPoints) this->cbQuadSeamInternalPoints->Enable(false);
	if (this->tcQuadSeamRefinementFactor) this->tcQuadSeamRefinementFactor->Enable(false);

	if (this->cbQuadSeamEdgeInternalPoints) this->cbQuadSeamEdgeInternalPoints->Enable(false);
	if (this->tcQuadSeamEdgeRefinementFactor) this->tcQuadSeamEdgeRefinementFactor->Enable(false);

	if (this->tcTemplateNumberSides) this->tcTemplateNumberSides->Enable(false);
	if (this->tcTemplateNumberSubdivision[0]) this->tcTemplateNumberSubdivision[0]->Enable(false);
	if (this->tcTemplateNumberSubdivision[1]) this->tcTemplateNumberSubdivision[1]->Enable(false);
	if (this->tcTemplateNumberSubdivision[2]) this->tcTemplateNumberSubdivision[2]->Enable(false);
	if (this->tcTemplateNumberSubdivision[3]) this->tcTemplateNumberSubdivision[3]->Enable(false);
	if (this->cbTemplateSmooth) this->cbTemplateSmooth->Enable(false);
	if (this->tcTemplateU1) this->tcTemplateU1->Enable(false);
	if (this->tcTemplateV1) this->tcTemplateV1->Enable(false);
	if (this->tcTemplateU2) this->tcTemplateU2->Enable(false);
	if (this->tcTemplateV2) this->tcTemplateV2->Enable(false);
#endif //#if USE_QUADS

	if (this->bExecute) this->bExecute->Enable(false);
	if (this->bClear) this->bClear->Enable(false);
}

void CRAbMesh::TecGrafMsh2DModule::Ready(bool clearing, bool pending)
{
	this->frame->GetViewModule()->Ready(clearing, pending);

	TecGraf::Msh2D::MainDrive *drive = dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive);

	if (clearing)
	{
		drive->clear();
	}

	if (this->useRedirector)
    {
        this->redirector.LogText(pending);
    }

	if (pending)
	{
		this->canvas->Execute();
	}
	else
	{
		this->frame->Ready();
		this->FillTree();

		if (this->bExecute) this->bExecute->Enable();
		if (this->bClear) this->bClear->Enable();

		if (this->bExecute) this->bExecute->SetFocus();

		wxString s(drive->getStatus().getMessage().c_str(), wxMBConvLibc());

		this->frame->SetStatusText(s);

		if ((!this->sbsQuality) &&
			(drive->getStatus().getType() == drive->success.getType()))
		{
			this->AddQualityString();
		}

		if ((this->GetMesh()) && (this->GetWarpMesh()) && (!this->deformed) && (this->GetDimension() == 2))
        {
            this->deformed = new MJMesh::Mesh();

            Data::VertexMap vMap;

            for (Data::Mesh::ConstVertexIterator iter = this->GetMesh()->vBegin();
                 iter != this->GetMesh()->vEnd(); iter++)
            {
                MJMesh::Vertex *v = new MJMesh::Vertex((*iter)->getPoint()->getCoord(0), (*iter)->getPoint()->getCoord(1), (*iter)->getId());

                vMap[v->getId()] = v;

                this->deformed->add(v);
            }

            for (Data::Mesh::ConstFrontElementIterator iter = this->GetMesh()->feBegin();
                 iter != this->GetMesh()->feEnd(); iter++)
            {
                MJMesh::FrontElement2D *fel = new MJMesh::FrontElement2D(
                    static_cast<MJMesh::Vertex *>(vMap[(*iter)->getVertex(0)->getId()]),
                    static_cast<MJMesh::Vertex *>(vMap[(*iter)->getVertex(1)->getId()]), (*iter)->getId());

                this->deformed->add(fel);
            }

            for (Data::Mesh::ConstElementIterator iter = this->GetMesh()->eBegin();
                 iter != this->GetMesh()->eEnd(); iter++)
            {
                MJMesh::Element2D *e = new MJMesh::Element2D(
                    static_cast<MJMesh::Vertex *>(vMap[(*iter)->getVertex(0)->getId()]),
                    static_cast<MJMesh::Vertex *>(vMap[(*iter)->getVertex(1)->getId()]),
                    static_cast<MJMesh::Vertex *>(vMap[(*iter)->getVertex(2)->getId()]), (*iter)->getId());

                this->deformed->add(e);
            }

            wxScrollEvent event;

            this->OnDeformWarpMesh(event);
        }
	}
}

void CRAbMesh::TecGrafMsh2DModule::Clear(bool onlyDrive, bool destroying)
{
	this->frame->GetViewModule()->Clear(onlyDrive, destroying);

	MeshModule::Clear(onlyDrive, destroying);

    if (this->deformed)
    {
        Data::Mesh *m = this->deformed;

        this->deformed = NULL;

        delete m;
    }

	if (onlyDrive)
	{
		return;
	}

	//clear tree items
	this->ClearTree();

    //enable and reset items
    if (this->bcbAlgorithm) this->bcbAlgorithm->Enable();

    if (this->tcBilinearElementType) this->tcBilinearElementType->Enable();
    if (this->tcBilinearDiagonalType) this->tcBilinearDiagonalType->Enable();
    if (this->tcBilinearAngleTolerance) this->tcBilinearAngleTolerance->Enable();
    if (this->tcBilinearNumberVerticesDirection1) this->tcBilinearNumberVerticesDirection1->Enable();
    if (this->tcBilinearNumberVerticesDirection2) this->tcBilinearNumberVerticesDirection2->Enable();

    if (this->tcTryBilinearElementType) this->tcTryBilinearElementType->Enable();
    if (this->tcTryBilinearDiagonalType) this->tcTryBilinearDiagonalType->Enable();

    if (this->tcCollBilinearElementType) this->tcCollBilinearElementType->Enable();
    if (this->tcCollBilinearDiagonalType) this->tcCollBilinearDiagonalType->Enable();
    if (this->tcCollBilinearAngleTolerance) this->tcCollBilinearAngleTolerance->Enable();
    if (this->tcCollBilinearNumberVerticesDirection1) this->tcCollBilinearNumberVerticesDirection1->Enable();
    if (this->tcCollBilinearNumberVerticesDirection2) this->tcCollBilinearNumberVerticesDirection2->Enable();

    if (this->tcLoftElementType) this->tcLoftElementType->Enable();
	if (this->tcLoftDiagonalType) this->tcLoftDiagonalType->Enable();
	if (this->tcLoftAngleTolerance) this->tcLoftAngleTolerance->Enable();
	if (this->tcLoftNumberVerticesDirection1) this->tcLoftNumberVerticesDirection1->Enable();
	if (this->tcLoftNumberVerticesDirection2) this->tcLoftNumberVerticesDirection2->Enable();
	if (this->tcLoftDirection) this->tcLoftDirection->Enable();
	if (this->tcLoftWeight) this->tcLoftWeight->Enable();

	if (this->tcCollLoftElementType) this->tcCollLoftElementType->Enable();
	if (this->tcCollLoftDiagonalType) this->tcCollLoftDiagonalType->Enable();
	if (this->tcCollLoftAngleTolerance) this->tcCollLoftAngleTolerance->Enable();
	if (this->tcCollLoftNumberVerticesDirection1) this->tcCollLoftNumberVerticesDirection1->Enable();
	if (this->tcCollLoftNumberVerticesDirection2) this->tcCollLoftNumberVerticesDirection2->Enable();
	if (this->tcCollLoftWeight) this->tcCollLoftWeight->Enable();

	if (this->tcTrilinearElementType) this->tcTrilinearElementType->Enable();
	if (this->tcTrilinearAngleTolerance) this->tcTrilinearAngleTolerance->Enable();
	if (this->tcTrilinearNumberVerticesDirection) this->tcTrilinearNumberVerticesDirection->Enable();

	if (this->tcTryTrilinearElementType) this->tcTryTrilinearElementType->Enable();

	if (this->cbBoundContractionInternalVertices) this->cbBoundContractionInternalVertices->Enable();
	if (this->cbBoundContractionInternalVerticesWithQuadtree) this->cbBoundContractionInternalVerticesWithQuadtree->Enable();

	if (this->cbBoundContractionWarpInternalVertices) this->cbBoundContractionWarpInternalVertices->Enable();
	if (this->cbBoundContractionWarpInternalVerticesWithQuadtree) this->cbBoundContractionWarpInternalVerticesWithQuadtree->Enable();
	if (this->cbBoundContractionWarpBoundaryVertices) this->cbBoundContractionWarpBoundaryVertices->Enable();
	//if (this->cbBoundContractionWarpInvertReadOrderOnTxt) this->cbBoundContractionWarpInvertReadOrderOnTxt->Enable();
	if (this->bBoundContractionWarpLoadWarpBoundary) this->bBoundContractionWarpLoadWarpBoundary->Enable();

	if (this->tcQuadBoundElementType) this->tcQuadBoundElementType->Enable();
	if (this->cbQuadBoundRefineQuadtree) this->cbQuadBoundRefineQuadtree->Enable();

	if (this->cbShapeInternalPoints) this->cbShapeInternalPoints->Enable();
	if (this->tcShapeRefinementFactor) this->tcShapeRefinementFactor->Enable();

	if (this->cbEdgeInternalPoints) this->cbEdgeInternalPoints->Enable();
	if (this->tcEdgeRefinementFactor) this->tcEdgeRefinementFactor->Enable();

#if USE_QUADS
	if (this->cbQuadSeamInternalPoints) this->cbQuadSeamInternalPoints->Enable();
	if (this->tcQuadSeamRefinementFactor) this->tcQuadSeamRefinementFactor->Enable();

	if (this->cbQuadSeamEdgeInternalPoints) this->cbQuadSeamEdgeInternalPoints->Enable();
	if (this->tcQuadSeamEdgeRefinementFactor) this->tcQuadSeamEdgeRefinementFactor->Enable();

	if (this->tcTemplateNumberSides) this->tcTemplateNumberSides->Enable();
	if (this->tcTemplateNumberSubdivision[0]) this->tcTemplateNumberSubdivision[0]->Enable();
	if (this->tcTemplateNumberSubdivision[1]) this->tcTemplateNumberSubdivision[1]->Enable();
	if (this->tcTemplateNumberSubdivision[2]) this->tcTemplateNumberSubdivision[2]->Enable();
	if (this->tcTemplateNumberSubdivision[3]) this->tcTemplateNumberSubdivision[3]->Enable();
	if (this->cbTemplateSmooth) this->cbTemplateSmooth->Enable();
	if (this->tcTemplateU1) this->tcTemplateU1->Enable();
	if (this->tcTemplateV1) this->tcTemplateV1->Enable();
	if (this->tcTemplateU2) this->tcTemplateU2->Enable();
	if (this->tcTemplateV2) this->tcTemplateV2->Enable();
#endif //#if USE_QUADS

	//remove string, if any
	if (this->sbsQuality)
	{
		wxBoxSizer *pConfSizer = static_cast<wxBoxSizer *>(this->GetPanel(0)->GetSizer());

		this->sbsQuality->Clear(true);

		pConfSizer->Detach(this->sbsQuality);

		delete this->sbsQuality;

		this->sbsQuality = NULL;

		pConfSizer->Layout();
		this->GetPanel(0)->FitInside();
	}
}

void CRAbMesh::TecGrafMsh2DModule::SetGUIState()
{

}

void CRAbMesh::TecGrafMsh2DModule::SetWarpBoundaryDrawable(bool warpBoundaryDrawable)
{
	this->SetConfiguration(DRAW_WARP_BOUNDARY, warpBoundaryDrawable);
}

bool CRAbMesh::TecGrafMsh2DModule::IsWarpBoundaryDrawable() const
{
	return this->IsConfiguration(DRAW_WARP_BOUNDARY);
}

void CRAbMesh::TecGrafMsh2DModule::SetWarpMeshDrawable(bool warpMeshDrawable)
{
	this->SetConfiguration(DRAW_WARP_MESH, warpMeshDrawable);
}

bool CRAbMesh::TecGrafMsh2DModule::IsWarpMeshDrawable() const
{
	return this->IsConfiguration(DRAW_WARP_MESH);
}

void CRAbMesh::TecGrafMsh2DModule::SetDeformedMeshDrawable(bool deformedMeshDrawable)
{
	this->SetConfiguration(DRAW_DEFORMED_MESH, deformedMeshDrawable);
}

bool CRAbMesh::TecGrafMsh2DModule::IsDeformedMeshDrawable() const
{
	return this->IsConfiguration(DRAW_DEFORMED_MESH);
}

bool CRAbMesh::TecGrafMsh2DModule::IsDrawable() const
{
	return (MeshModule::IsDrawable() ||
			this->IsWarpBoundaryDrawable() ||
			this->IsWarpMeshDrawable() ||
			this->IsDeformedMeshDrawable());
}

void CRAbMesh::TecGrafMsh2DModule::MakeMainDrive()
{
	this->drive = new TecGraf::Msh2D::MainDrive();
}

void CRAbMesh::TecGrafMsh2DModule::MakeMenus()
{
	this->menus.push_back(this->MakeConfigurationMenu());
}

void CRAbMesh::TecGrafMsh2DModule::MakeToolbars()
{

}

void CRAbMesh::TecGrafMsh2DModule::MakePanels()
{
	this->tcLog = NULL;

	this->panels.push_back(this->MakeConfigurationPanel());
	this->panels.push_back(this->MakeTreePanel());
	//this->panels.push_back(this->MakeLogPanel());
}

wxPanel *CRAbMesh::TecGrafMsh2DModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = MeshModule::MakeConfigurationPanel();
	wxSizer *sConf = pConfiguration->GetSizer();

	//configuration panel
    this->bcbAlgorithm = new wxChoicebook(pConfiguration, wxID_ANY);
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Bilinear"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Try bilinear"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Coll bilinear"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Loft"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Coll Loft"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Trilinear"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Try trilinear"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Bound contraction"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Bound contraction warp"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Quad bound"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Shape"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Edge"));
#if USE_QUADS
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Quad seam"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Quad seam edge"));
    this->bcbAlgorithm->AddPage(new wxScrolledWindow(this->bcbAlgorithm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL), _("Template"));
#endif //#if USE_QUADS

#if USE_QUADS
    wxStaticText *stBilinearElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stBilinearElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcBilinearElementType            = new wxTextCtrl(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stBilinearDiagonalType   = new wxStaticText(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("Diagonal type (1-4)"));
    this->tcBilinearDiagonalType           = new wxTextCtrl(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stBilinearAngleTolerance = new wxStaticText(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("Angle tolerance (0-360)"));
    this->tcBilinearAngleTolerance         = new wxTextCtrl(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stBilinearNumberVerticesDirection1 = new wxStaticText(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("Nodes in direction 1"));
    this->tcBilinearNumberVerticesDirection1         = new wxTextCtrl(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stBilinearNumberVerticesDirection2 = new wxStaticText(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("Nodes in direction 2"));
    this->tcBilinearNumberVerticesDirection2         = new wxTextCtrl(this->bcbAlgorithm->GetPage(0), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    wxStaticText *stTryBilinearElementType  = new wxStaticText(this->bcbAlgorithm->GetPage(1), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stTryBilinearElementType  = new wxStaticText(this->bcbAlgorithm->GetPage(1), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcTryBilinearElementType          = new wxTextCtrl(this->bcbAlgorithm->GetPage(1), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTryBilinearDiagonalType = new wxStaticText(this->bcbAlgorithm->GetPage(1), wxID_ANY, _("Diagonal type (1-4)"));
    this->tcTryBilinearDiagonalType         = new wxTextCtrl(this->bcbAlgorithm->GetPage(1), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    wxStaticText *stCollBilinearElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stCollBilinearElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcCollBilinearElementType            = new wxTextCtrl(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollBilinearDiagonalType   = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Diagonal type (1-4)"));
    this->tcCollBilinearDiagonalType           = new wxTextCtrl(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollBilinearAngleTolerance = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Angle tolerance (0-360)"));
    this->tcCollBilinearAngleTolerance         = new wxTextCtrl(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollBilinearNumberVerticesDirection1 = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Nodes in direction 1"));
    this->tcCollBilinearNumberVerticesDirection1         = new wxTextCtrl(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollBilinearNumberVerticesDirection2 = new wxStaticText(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("Nodes in direction 2"));
    this->tcCollBilinearNumberVerticesDirection2         = new wxTextCtrl(this->bcbAlgorithm->GetPage(2), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    wxStaticText *stLoftElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stLoftElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcLoftElementType            = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stLoftDiagonalType   = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Diagonal type (1-4)"));
    this->tcLoftDiagonalType           = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stLoftAngleTolerance = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Angle tolerance (0-360)"));
    this->tcLoftAngleTolerance         = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stLoftNumberVerticesDirection1 = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Nodes in direction 1"));
    this->tcLoftNumberVerticesDirection1         = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stLoftNumberVerticesDirection2 = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Nodes in direction 2"));
    this->tcLoftNumberVerticesDirection2         = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stLoftDirection      = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Direction (0,1)"));
    this->tcLoftDirection              = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stLoftWeight         = new wxStaticText(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("Weight (>0)"));
    this->tcLoftWeight                 = new wxTextCtrl(this->bcbAlgorithm->GetPage(3), wxID_ANY, _("1.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    wxStaticText *stCollLoftElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stCollLoftElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcCollLoftElementType            = new wxTextCtrl(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollLoftDiagonalType   = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Diagonal type (1-4)"));
    this->tcCollLoftDiagonalType           = new wxTextCtrl(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollLoftAngleTolerance = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Angle tolerance (0-360)"));
    this->tcCollLoftAngleTolerance         = new wxTextCtrl(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollLoftNumberVerticesDirection1 = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Nodes in direction 1"));
    this->tcCollLoftNumberVerticesDirection1         = new wxTextCtrl(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollLoftNumberVerticesDirection2 = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Nodes in direction 2"));
    this->tcCollLoftNumberVerticesDirection2         = new wxTextCtrl(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stCollLoftWeight         = new wxStaticText(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("Weight (>0)"));
    this->tcCollLoftWeight                 = new wxTextCtrl(this->bcbAlgorithm->GetPage(4), wxID_ANY, _("1.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    wxStaticText *stTrilinearElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stTrilinearElementType    = new wxStaticText(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcTrilinearElementType            = new wxTextCtrl(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTrilinearAngleTolerance = new wxStaticText(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("Angle tolerance (0-360)"));
    this->tcTrilinearAngleTolerance         = new wxTextCtrl(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("10"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTrilinearNumberVerticesDirection = new wxStaticText(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("Nodes in direction"));
    this->tcTrilinearNumberVerticesDirection         = new wxTextCtrl(this->bcbAlgorithm->GetPage(5), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    wxStaticText *stTryTrilinearElementType = new wxStaticText(this->bcbAlgorithm->GetPage(6), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stTryTrilinearElementType = new wxStaticText(this->bcbAlgorithm->GetPage(6), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcTryTrilinearElementType         = new wxTextCtrl(this->bcbAlgorithm->GetPage(6), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

    this->cbBoundContractionInternalVertices             = new wxCheckBox(this->bcbAlgorithm->GetPage(7), wxID_ANY, _("Internal nodes"));
    this->cbBoundContractionInternalVerticesWithQuadtree = new wxCheckBox(this->bcbAlgorithm->GetPage(7), wxID_ANY, _("Quadtree"));

    this->cbBoundContractionWarpInternalVertices             = new wxCheckBox(this->bcbAlgorithm->GetPage(8), wxID_ANY, _("Internal nodes"));
    this->cbBoundContractionWarpInternalVerticesWithQuadtree = new wxCheckBox(this->bcbAlgorithm->GetPage(8), wxID_ANY, _("Quadtree"));
    this->cbBoundContractionWarpBoundaryVertices             = new wxCheckBox(this->bcbAlgorithm->GetPage(8), wxID_ANY, _("Boundary nodes"));
    //this->cbBoundContractionWarpInvertReadOrderOnTxt         = new wxCheckBox(this->bcbAlgorithm->GetPage(8), wxID_ANY, _("Invert order on reading txt"));
    this->bBoundContractionWarpLoadWarpBoundary              = new wxButton(this->bcbAlgorithm->GetPage(8), wxID_ANY, _("Load warp boundary"));
    this->sBoundCountractionWarpDeformMesh                   = new wxSlider(this->bcbAlgorithm->GetPage(8), wxID_ANY, 0, 0, 100);

#if USE_QUADS
    wxStaticText *stQuadBoundElementType = new wxStaticText(this->bcbAlgorithm->GetPage(9), wxID_ANY, _("Element type (3,4)"));
#else
    wxStaticText *stQuadBoundElementType = new wxStaticText(this->bcbAlgorithm->GetPage(9), wxID_ANY, _("Element type (3)"));
#endif //#if USE_QUADS
    this->tcQuadBoundElementType         = new wxTextCtrl(this->bcbAlgorithm->GetPage(9), wxID_ANY, _("3"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbQuadBoundRefineQuadtree      = new wxCheckBox(this->bcbAlgorithm->GetPage(9), wxID_ANY, _("Refine quadtree"));

    this->cbShapeInternalPoints   = new wxCheckBox(this->bcbAlgorithm->GetPage(10), wxID_ANY, _("Internal points"));
    wxStaticText *stShapeRefinementFactor = new wxStaticText(this->bcbAlgorithm->GetPage(10), wxID_ANY, _("Refinement factor (>=0.1)"));
    this->tcShapeRefinementFactor = new wxTextCtrl(this->bcbAlgorithm->GetPage(10), wxID_ANY, _("0.1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

    this->cbEdgeInternalPoints   = new wxCheckBox(this->bcbAlgorithm->GetPage(11), wxID_ANY, _("Internal points"));
    wxStaticText *stEdgeRefinementFactor = new wxStaticText(this->bcbAlgorithm->GetPage(11), wxID_ANY, _("Refinement factor (>=0.1)"));
    this->tcEdgeRefinementFactor = new wxTextCtrl(this->bcbAlgorithm->GetPage(11), wxID_ANY, _("0.1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

#if USE_QUADS
    this->cbQuadSeamInternalPoints   = new wxCheckBox(this->bcbAlgorithm->GetPage(12), wxID_ANY, _("Internal points"));
    wxStaticText *stQuadSeamRefinementFactor = new wxStaticText(this->bcbAlgorithm->GetPage(12), wxID_ANY, _("Refinement factor (>=0.1)"));
    this->tcQuadSeamRefinementFactor = new wxTextCtrl(this->bcbAlgorithm->GetPage(12), wxID_ANY, _("0.1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

    this->cbQuadSeamEdgeInternalPoints   = new wxCheckBox(this->bcbAlgorithm->GetPage(13), wxID_ANY, _("Internal points"));
    wxStaticText *stQuadSeamEdgeRefinementFactor = new wxStaticText(this->bcbAlgorithm->GetPage(13), wxID_ANY, _("Refinement factor (>=0.1)"));
    this->tcQuadSeamEdgeRefinementFactor = new wxTextCtrl(this->bcbAlgorithm->GetPage(13), wxID_ANY, _("0.1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);

    wxStaticText *stTemplateNumberSides = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("Number of sides (2-4)"));
    this->tcTemplateNumberSides         = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("4"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateNumberSubdivision0 = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("First subdivision"));
    this->tcTemplateNumberSubdivision[0]       = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateNumberSubdivision1 = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("Second subdivision"));
    this->tcTemplateNumberSubdivision[1]       = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateNumberSubdivision2 = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("Third subdivision"));
    this->tcTemplateNumberSubdivision[2]       = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateNumberSubdivision3 = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("Fourth subdivision"));
    this->tcTemplateNumberSubdivision[3]       = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    this->cbTemplateSmooth                     = new wxCheckBox(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("Smooth"));
    wxStaticText *stTemplateU1          = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("U1"));
    this->tcTemplateU1                  = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateV1          = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("V1"));
    this->tcTemplateV1                  = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateU2          = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("U2"));
    this->tcTemplateU2                  = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
    wxStaticText *stTemplateV2          = new wxStaticText(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("V2"));
    this->tcTemplateV2                  = new wxTextCtrl(this->bcbAlgorithm->GetPage(14), wxID_ANY, _("0.0"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT);
#endif //#if USE_QUADS

    this->bExecute = new wxButton(pConfiguration, wxID_ANY, _("Execute"));
    this->bClear   = new wxButton(pConfiguration, wxID_ANY, _("Clear"));

    //default values
    this->cbBoundContractionInternalVertices->SetValue(true);
    this->cbBoundContractionInternalVerticesWithQuadtree->SetValue(true);
    this->cbBoundContractionWarpInternalVertices->SetValue(false);
    this->cbBoundContractionWarpInternalVerticesWithQuadtree->SetValue(false);
    this->cbBoundContractionWarpBoundaryVertices->SetValue(true);
    //this->cbBoundContractionWarpInvertReadOrderOnTxt->SetValue(true);
    this->cbShapeInternalPoints->SetValue(true);
    this->cbEdgeInternalPoints->SetValue(true);
#if USE_QUADS
    this->cbQuadSeamInternalPoints->SetValue(true);
    this->cbQuadSeamEdgeInternalPoints->SetValue(true);
    this->cbTemplateSmooth->SetValue(true);
#endif //#if USE_QUADS

    //sizers
    wxFlexGridSizer *gsBilinear = new wxFlexGridSizer(2);
    gsBilinear->SetFlexibleDirection(wxVERTICAL);
    gsBilinear->Add(stBilinearElementType,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBilinear->Add(this->tcBilinearElementType,    0, wxEXPAND | wxALL, 0);
    gsBilinear->Add(stBilinearDiagonalType,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBilinear->Add(this->tcBilinearDiagonalType,   0, wxEXPAND | wxALL, 0);
    gsBilinear->Add(stBilinearAngleTolerance,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBilinear->Add(this->tcBilinearAngleTolerance, 0, wxEXPAND | wxALL, 0);
    gsBilinear->Add(stBilinearNumberVerticesDirection1,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBilinear->Add(this->tcBilinearNumberVerticesDirection1, 0, wxEXPAND | wxALL, 0);
    gsBilinear->Add(stBilinearNumberVerticesDirection2,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsBilinear->Add(this->tcBilinearNumberVerticesDirection2, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(0)->SetSizer(gsBilinear);

    wxFlexGridSizer *gsTryBilinear = new wxFlexGridSizer(2);
    gsTryBilinear->SetFlexibleDirection(wxVERTICAL);
    gsTryBilinear->Add(stTryBilinearElementType,        0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTryBilinear->Add(this->tcTryBilinearElementType,  0, wxEXPAND | wxALL, 0);
    gsTryBilinear->Add(stTryBilinearDiagonalType,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTryBilinear->Add(this->tcTryBilinearDiagonalType, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(1)->SetSizer(gsTryBilinear);

    wxFlexGridSizer *gsCollBilinear = new wxFlexGridSizer(2);
    gsCollBilinear->SetFlexibleDirection(wxVERTICAL);
    gsCollBilinear->Add(stCollBilinearElementType,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollBilinear->Add(this->tcCollBilinearElementType,    0, wxEXPAND | wxALL, 0);
    gsCollBilinear->Add(stCollBilinearDiagonalType,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollBilinear->Add(this->tcCollBilinearDiagonalType,   0, wxEXPAND | wxALL, 0);
    gsCollBilinear->Add(stCollBilinearAngleTolerance,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollBilinear->Add(this->tcCollBilinearAngleTolerance, 0, wxEXPAND | wxALL, 0);
    gsCollBilinear->Add(stCollBilinearNumberVerticesDirection1,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollBilinear->Add(this->tcCollBilinearNumberVerticesDirection1, 0, wxEXPAND | wxALL, 0);
    gsCollBilinear->Add(stCollBilinearNumberVerticesDirection2,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollBilinear->Add(this->tcCollBilinearNumberVerticesDirection2, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(2)->SetSizer(gsCollBilinear);

    wxFlexGridSizer *gsLoft = new wxFlexGridSizer(2);
    gsLoft->SetFlexibleDirection(wxVERTICAL);
    gsLoft->Add(stLoftElementType,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftElementType,    0, wxEXPAND | wxALL, 0);
    gsLoft->Add(stLoftDiagonalType,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftDiagonalType,   0, wxEXPAND | wxALL, 0);
    gsLoft->Add(stLoftAngleTolerance,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftAngleTolerance, 0, wxEXPAND | wxALL, 0);
    gsLoft->Add(stLoftNumberVerticesDirection1,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftNumberVerticesDirection1, 0, wxEXPAND | wxALL, 0);
    gsLoft->Add(stLoftNumberVerticesDirection2,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftNumberVerticesDirection2, 0, wxEXPAND | wxALL, 0);
    gsLoft->Add(stLoftDirection,           0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftDirection,     0, wxEXPAND | wxALL, 0);
    gsLoft->Add(stLoftWeight,              0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsLoft->Add(this->tcLoftWeight,        0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(3)->SetSizer(gsLoft);

    wxFlexGridSizer *gsCollLoft = new wxFlexGridSizer(2);
    gsCollLoft->SetFlexibleDirection(wxVERTICAL);
    gsCollLoft->Add(stCollLoftElementType,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollLoft->Add(this->tcCollLoftElementType,    0, wxEXPAND | wxALL, 0);
    gsCollLoft->Add(stCollLoftDiagonalType,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollLoft->Add(this->tcCollLoftDiagonalType,   0, wxEXPAND | wxALL, 0);
    gsCollLoft->Add(stCollLoftAngleTolerance,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollLoft->Add(this->tcCollLoftAngleTolerance, 0, wxEXPAND | wxALL, 0);
    gsCollLoft->Add(stCollLoftNumberVerticesDirection1,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollLoft->Add(this->tcCollLoftNumberVerticesDirection1, 0, wxEXPAND | wxALL, 0);
    gsCollLoft->Add(stCollLoftNumberVerticesDirection2,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollLoft->Add(this->tcCollLoftNumberVerticesDirection2, 0, wxEXPAND | wxALL, 0);
    gsCollLoft->Add(stCollLoftWeight,              0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsCollLoft->Add(this->tcCollLoftWeight,        0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(4)->SetSizer(gsCollLoft);

    wxFlexGridSizer *gsTrilinear = new wxFlexGridSizer(2);
    gsTrilinear->SetFlexibleDirection(wxVERTICAL);
    gsTrilinear->Add(stTrilinearElementType,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTrilinear->Add(this->tcTrilinearElementType,    0, wxEXPAND | wxALL, 0);
    gsTrilinear->Add(stTrilinearAngleTolerance,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTrilinear->Add(this->tcTrilinearAngleTolerance, 0, wxEXPAND | wxALL, 0);
    gsTrilinear->Add(stTrilinearNumberVerticesDirection,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTrilinear->Add(this->tcTrilinearNumberVerticesDirection, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(5)->SetSizer(gsTrilinear);

    wxFlexGridSizer *gsTryTrilinear = new wxFlexGridSizer(2);
    gsTryTrilinear->SetFlexibleDirection(wxVERTICAL);
    gsTryTrilinear->Add(stTryTrilinearElementType,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTryTrilinear->Add(this->tcTryTrilinearElementType, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(6)->SetSizer(gsTryTrilinear);

    wxFlexGridSizer *gsBoundContraction = new wxFlexGridSizer(2);
    gsBoundContraction->SetFlexibleDirection(wxVERTICAL);
    gsBoundContraction->Add(this->cbBoundContractionInternalVertices,             0, wxEXPAND | wxALL, 0);
    gsBoundContraction->Add(1, 1,                                                 0, wxEXPAND | wxALL, 0);
    gsBoundContraction->Add(this->cbBoundContractionInternalVerticesWithQuadtree, 0, wxEXPAND | wxALL, 0);
    gsBoundContraction->Add(1, 1,                                                 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(7)->SetSizer(gsBoundContraction);

    wxFlexGridSizer *gsBoundContractionWarp = new wxFlexGridSizer(2);
    gsBoundContractionWarp->SetFlexibleDirection(wxVERTICAL);
    gsBoundContractionWarp->Add(this->cbBoundContractionWarpInternalVertices,             0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(1, 1,                                                     0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(this->cbBoundContractionWarpInternalVerticesWithQuadtree, 0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(1, 1,                                                     0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(this->cbBoundContractionWarpBoundaryVertices,             0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(1, 1,                                                     0, wxEXPAND | wxALL, 0);
    //gsBoundContractionWarp->Add(this->cbBoundContractionWarpInvertReadOrderOnTxt,         0, wxEXPAND | wxALL, 0);
    //gsBoundContractionWarp->Add(1, 1,                                                     0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(this->bBoundContractionWarpLoadWarpBoundary,              0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(1, 1,                                                     0, wxEXPAND | wxALL, 0);
    gsBoundContractionWarp->Add(this->sBoundCountractionWarpDeformMesh,                   0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(8)->SetSizer(gsBoundContractionWarp);

    wxFlexGridSizer *gsQuadBound = new wxFlexGridSizer(2);
    gsQuadBound->SetFlexibleDirection(wxVERTICAL);
    gsQuadBound->Add(stQuadBoundElementType,          0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsQuadBound->Add(this->tcQuadBoundElementType,    0, wxEXPAND | wxALL, 0);
    gsQuadBound->Add(this->cbQuadBoundRefineQuadtree, 0, wxEXPAND | wxALL, 0);
    gsQuadBound->Add(1, 1,                            0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(9)->SetSizer(gsQuadBound);

    wxFlexGridSizer *gsShape = new wxFlexGridSizer(2);
    gsShape->SetFlexibleDirection(wxVERTICAL);
    gsShape->Add(this->cbShapeInternalPoints,   0, wxEXPAND | wxALL, 0);
    gsShape->Add(1, 1,                          0, wxEXPAND | wxALL, 0);
    gsShape->Add(stShapeRefinementFactor,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsShape->Add(this->tcShapeRefinementFactor, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(10)->SetSizer(gsShape);

    wxFlexGridSizer *gsEdge = new wxFlexGridSizer(2);
    gsEdge->SetFlexibleDirection(wxVERTICAL);
    gsEdge->Add(this->cbEdgeInternalPoints,   0, wxEXPAND | wxALL, 0);
    gsEdge->Add(1, 1,                         0, wxEXPAND | wxALL, 0);
    gsEdge->Add(stEdgeRefinementFactor,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsEdge->Add(this->tcEdgeRefinementFactor, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(11)->SetSizer(gsEdge);

#if USE_QUADS
    wxFlexGridSizer *gsQuadSeam = new wxFlexGridSizer(2);
    gsQuadSeam->SetFlexibleDirection(wxVERTICAL);
    gsQuadSeam->Add(this->cbQuadSeamInternalPoints,   0, wxEXPAND | wxALL, 0);
    gsQuadSeam->Add(1, 1,                             0, wxEXPAND | wxALL, 0);
    gsQuadSeam->Add(stQuadSeamRefinementFactor,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsQuadSeam->Add(this->tcQuadSeamRefinementFactor, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(12)->SetSizer(gsQuadSeam);

    wxFlexGridSizer *gsQuadSeamEdge = new wxFlexGridSizer(2);
    gsQuadSeamEdge->SetFlexibleDirection(wxVERTICAL);
    gsQuadSeamEdge->Add(this->cbQuadSeamEdgeInternalPoints,   0, wxEXPAND | wxALL, 0);
    gsQuadSeamEdge->Add(1, 1,                                 0, wxEXPAND | wxALL, 0);
    gsQuadSeamEdge->Add(stQuadSeamEdgeRefinementFactor,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsQuadSeamEdge->Add(this->tcQuadSeamEdgeRefinementFactor, 0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(13)->SetSizer(gsQuadSeamEdge);

    wxFlexGridSizer *gsTemplate = new wxFlexGridSizer(2);
    gsTemplate->SetFlexibleDirection(wxVERTICAL);
    gsTemplate->Add(stTemplateNumberSides,       0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateNumberSides, 0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateNumberSubdivision0,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateNumberSubdivision[0], 0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateNumberSubdivision1,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateNumberSubdivision[1], 0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateNumberSubdivision2,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateNumberSubdivision[2], 0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateNumberSubdivision3,         0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateNumberSubdivision[3], 0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(this->cbTemplateSmooth,               0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(1, 1,                                 0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateU1,                0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateU1,          0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateV1,                0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateV1,          0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateU2,                0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateU2,          0, wxEXPAND | wxALL, 0);
    gsTemplate->Add(stTemplateV2,                0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    gsTemplate->Add(this->tcTemplateV2,          0, wxEXPAND | wxALL, 0);
    this->bcbAlgorithm->GetPage(14)->SetSizer(gsTemplate);
#endif //#if USE_QUADS

    wxStaticBoxSizer *sbsExecute = new wxStaticBoxSizer(wxHORIZONTAL, pConfiguration, _("Execute"));
    sbsExecute->Add(this->bExecute, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);
    sbsExecute->Add(this->bClear,   1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 1);

    wxBoxSizer *bsConf = new wxBoxSizer(wxVERTICAL);
	bsConf->Add(this->bcbAlgorithm, 0, wxEXPAND | wxALL, 0);
	bsConf->Add(sbsExecute,         0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 1);
	bsConf->Add(sConf,              0, wxEXPAND | wxALL, 0);
    pConfiguration->SetSizer(bsConf, false);

    this->Connect(this->bBoundContractionWarpLoadWarpBoundary->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TecGrafMsh2DModule::OnLoadWarpBoundary));
    this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_TOP,             wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_BOTTOM,          wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_LINEUP,          wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_LINEDOWN,        wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_PAGEUP,          wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_PAGEDOWN,        wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_THUMBTRACK,      wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_THUMBRELEASE,    wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));
	this->Connect(this->sBoundCountractionWarpDeformMesh->GetId(),      wxEVT_SCROLL_CHANGED,         wxScrollEventHandler(TecGrafMsh2DModule::OnDeformWarpMesh));

    this->Connect(this->bExecute->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TecGrafMsh2DModule::OnExecute));
    this->Connect(this->bClear->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TecGrafMsh2DModule::OnClear));

	return pConfiguration;
}

wxPanel *CRAbMesh::TecGrafMsh2DModule::MakeLogPanel()
{
	wxPanel *pLog = new wxScrolledWindow(this->frame, wxID_ANY, wxDefaultPosition, wxSize(210, 440), wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL);
	static_cast<wxScrolledWindow *>(pLog)->SetScrollRate(5, 5);

	this->tcLog = new wxTextCtrl(pLog, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize,
		wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_RICH);

	this->tcLog->SetFont(wxFont(this->tcLog->GetFont().GetPointSize(),
		wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	wxBoxSizer *bsLog = new wxBoxSizer(wxVERTICAL);
	bsLog->Add(this->tcLog, 1, wxEXPAND | wxALL, 1);
	pLog->SetSizer(bsLog);

	return pLog;
}

void CRAbMesh::TecGrafMsh2DModule::OnLoadWarpBoundary(wxCommandEvent &WXUNUSED(event))
{
	wxFileDialog *open = new wxFileDialog(this->frame, _("Open file"), this->frame->GetOpenPath(), _(""),
        this->frame->GetOpenWildcard(),
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
#else
        wxOPEN | wxFILE_MUST_EXIST);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (open->ShowModal() == wxID_OK)
    {
        wxString path = open->GetPath();

        std::string inputFile(path.mb_str(wxMBConvLibc()));

		/*bool opened = false;

		dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundContractionWarpLoadingWarpBoundary(true);

		opened = this->LoadFile(inputFile);

        if (opened)
		{
			this->canvas->FitCamera();

			this->canvas->Refresh(false);
		}

		dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundContractionWarpLoadingWarpBoundary(false);*/

		bool opened = this->LoadWarpFile(inputFile);

		if (opened)
		{
			this->canvas->FitCamera();

			this->canvas->Refresh(false);
		}
    }

    delete open;
}

void CRAbMesh::TecGrafMsh2DModule::OnDeformWarpMesh(wxScrollEvent &WXUNUSED(event))
{
    if (!this->deformed)
    {
        return;
    }

    int val = this->sBoundCountractionWarpDeformMesh->GetValue();
    int max = this->sBoundCountractionWarpDeformMesh->GetMax();

    double t = static_cast<double>(val)/static_cast<double>(max);

    this->deformed->freeze();

    Data::Mesh::ConstVertexIterator iter1 = this->GetMesh()->vBegin();
    Data::Mesh::ConstVertexIterator iter2 = this->GetWarpMesh()->vBegin();

    for (Data::Mesh::VertexIterator iter = this->deformed->vBegin();
         iter != this->deformed->vEnd(); iter++, iter1++, iter2++)
    {
        for (UInt i = 0; i < this->GetDimension(); i++)
        {
            Real coord = (*iter1)->getPoint()->getCoord(i)*(1.0 - t) + (*iter2)->getPoint()->getCoord(i)*t;

            (*iter)->getPoint()->setCoord(i, coord);
        }
    }

    this->deformed->unfreeze();

    this->deformed->clearLists();

    this->canvas->Refresh(false);
}

void CRAbMesh::TecGrafMsh2DModule::OnExecute(wxCommandEvent &WXUNUSED(event))
{
	this->Execute();

	this->frame->Execute();
}

void CRAbMesh::TecGrafMsh2DModule::OnClear(wxCommandEvent &WXUNUSED(event))
{
	this->Clear();

	this->frame->Clear();
}

bool CRAbMesh::TecGrafMsh2DModule::BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const
{
	bool bounding = CRAbMesh::MeshModule::BoundingBox(minx, miny, minz, maxx, maxy, maxz);

	if (this->GetWarpBoundary())
	{
		Data::Box *box = this->GetWarpBoundary()->box();

		UInt dim = box->dimension();

		minx = std::min(minx, box->getMin()->getCoord(0));
		miny = std::min(miny, box->getMin()->getCoord(1));
		minz = std::min(minz, (dim == 2) ? 0.0 : box->getMin()->getCoord(2));

		maxx = std::min(maxx, box->getMax()->getCoord(0));
		maxy = std::min(maxy, box->getMax()->getCoord(1));
		maxz = std::min(maxz, (dim == 2) ? 0.0 : box->getMax()->getCoord(2));

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}

	if (this->GetWarpMesh())
	{
		for (Data::Mesh::ConstVertexIterator iter = this->GetWarpMesh()->vBegin();
			 iter != this->GetWarpMesh()->vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

			if (v->getPoint()->getCoord(0) < minx)
			{
				minx = v->getPoint()->getCoord(0);
			}

			if (v->getPoint()->getCoord(1) < miny)
			{
				miny = v->getPoint()->getCoord(1);
			}

			if (v->getPoint()->getCoord(0) > maxx)
			{
				maxx = v->getPoint()->getCoord(0);
			}

			if (v->getPoint()->getCoord(1) > maxy)
			{
				maxy = v->getPoint()->getCoord(1);
			}

			if (v->getPoint()->dimension() == 2)
            {
                if (0.0 < minz)
                {
                    minz = 0.0;
                }

                if (0.0 > maxz)
                {
                    maxz = 0.0;
                }
            }
            else
            {
                if (v->getPoint()->getCoord(2) < minz)
                {
                    minz = v->getPoint()->getCoord(2);
                }

                if (v->getPoint()->getCoord(2) > maxz)
                {
                    maxz = v->getPoint()->getCoord(2);
                }
            }
		}

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}

	return bounding;
}

void CRAbMesh::TecGrafMsh2DModule::SetDimension(unsigned int /*dimension*/)
{

}

unsigned int CRAbMesh::TecGrafMsh2DModule::GetDimension() const
{
	unsigned int dimension = 0;

	if ((dimension == 0) && (this->GetMesh()))
	{
		dimension = this->GetMesh()->dimension();
	}

	/*if ((dimension == 0) && (this->GetFront()))
	{
		dimension = this->GetFront()->dimension();
	}*/

	if ((dimension == 0) && (this->GetBoundary()))
	{
		dimension = this->GetBoundary()->dimension();
	}

	return dimension;
}

void CRAbMesh::TecGrafMsh2DModule::SetTree(Data::BoxTree */*tree*/)
{
	//dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setTree(tree);
}

const Data::BoxTree *CRAbMesh::TecGrafMsh2DModule::GetTree() const
{
	//return dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->getTree();
	return NULL;
}

void CRAbMesh::TecGrafMsh2DModule::SetBoundary(Data::Boundary *boundary)
{
	dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundary(static_cast<MJMesh::Boundary *>(boundary));
}

const Data::Boundary *CRAbMesh::TecGrafMsh2DModule::GetBoundary() const
{
	return dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->getBoundary();
}

void CRAbMesh::TecGrafMsh2DModule::SetFront(Data::Front */*front*/)
{
	//dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setFront(front);
}

const Data::Front *CRAbMesh::TecGrafMsh2DModule::GetFront() const
{
	//return dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->getFront();
	return NULL;
}

void CRAbMesh::TecGrafMsh2DModule::SetMesh(Data::Mesh *mesh)
{
	dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setMesh(mesh);
}

const Data::Mesh *CRAbMesh::TecGrafMsh2DModule::GetMesh() const
{
	return dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->getMesh();
}

void CRAbMesh::TecGrafMsh2DModule::SetWarpBoundary(Data::Boundary *boundary)
{
	dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundContractionWarpBoundary(static_cast<MJMesh::Boundary *>(boundary));
}

const Data::Boundary *CRAbMesh::TecGrafMsh2DModule::GetWarpBoundary() const
{
	return dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->getBoundContractionWarpBoundary();
}

void CRAbMesh::TecGrafMsh2DModule::SetWarpMesh(Data::Mesh *mesh)
{
	dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->setBoundContractionWarpMesh(mesh);
}

const Data::Mesh *CRAbMesh::TecGrafMsh2DModule::GetWarpMesh() const
{
	return dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->getBoundContractionWarpMesh();
}

const Data::Mesh *CRAbMesh::TecGrafMsh2DModule::GetDeformedMesh() const
{
    return this->deformed;
}

wxString CRAbMesh::TecGrafMsh2DModule::QualityString() const
{
	wxString s(dynamic_cast<TecGraf::Msh2D::MainDrive *>(this->drive)->qualityString().c_str(), wxMBConvLibc());

	return s;
}

void CRAbMesh::TecGrafMsh2DModule::ClearDisplayLists(bool force) const
{
	MeshModule::ClearDisplayLists(force);

	if (this->GetWarpBoundary())
	{
		if ((force) || (this->GetWarpBoundary()->isOutdated()))
		{
			this->GetWarpBoundary()->clearLists();
		}
	}

	if (this->GetWarpMesh())
	{
		if ((force) || (this->GetWarpMesh()->isOutdated()))
		{
			this->GetWarpMesh()->clearLists();
		}
	}

	if (this->deformed)
    {
        this->deformed->clearLists();
    }
}

void CRAbMesh::TecGrafMsh2DModule::Render() const
{
	if (this->IsDrawable())
	{
#if USE_SCREEENSHOT
		glShadeModel(GL_FLAT);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_BLEND);
#endif //#if USE_SCREEENSHOT

		if ((this->IsDeformedMeshDrawable()) && (this->deformed))
		{
			this->deformed->draw();
		}

		if ((this->IsWarpMeshDrawable()) && (this->GetWarpMesh()))
		{
			this->GetWarpMesh()->draw();
		}

		if ((this->IsWarpBoundaryDrawable()) && (this->GetWarpBoundary()))
		{
			this->GetWarpBoundary()->draw();
		}
	}

	CRAbMesh::MeshModule::Render();

	if (this->IsDrawable())
	{
		wxTreeItemId item = this->tree->GetSelection();

		if (item.IsOk())
		{
			if (((this->IsEnabled(WARP_VERTICES, TREE_ITEM))       && (this->tree->GetItemParent(item) == (*this->tiItems.find(WARP_VERTICES)).second)) ||
				((this->IsEnabled(WARP_FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == (*this->tiItems.find(WARP_FRONT_ELEMENTS)).second)) ||
				((this->IsEnabled(WARP_ELEMENTS, TREE_ITEM))       && (this->tree->GetItemParent(item) == (*this->tiItems.find(WARP_ELEMENTS)).second)))
			{
				if (this->tree->GetItemParent(item) == (*this->tiItems.find(WARP_VERTICES)).second)
				{
					if ((this->IsAdjacentFrontElementHighlighted()) || (this->IsAdjacentElementHighlighted()))
					{
						Data::Vertex *v = static_cast<Data::Vertex *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

						if (this->IsAdjacentElementHighlighted())
						{
							Data::ElementList adjs = (this->GetWarpMesh()) ?
								this->GetWarpMesh()->adjacency(v) : Data::ElementList();

							for (Data::ElementList::const_iterator iter = adjs.begin();
								 iter != adjs.end(); iter++)
							{
								(*iter)->highlight();
								(*iter)->draw();
								(*iter)->unhighlight();
							}
						}

						if (this->IsAdjacentFrontElementHighlighted())
						{
							Data::FrontElementList adjs = (this->GetWarpMesh()) ? this->GetWarpMesh()->findAdjacentFrontElements(v) :
								((this->GetWarpBoundary()) ? this->GetWarpBoundary()->adjacency(v) : Data::FrontElementList());

							for (Data::FrontElementList::const_iterator iter = adjs.begin();
								 iter != adjs.end(); iter++)
							{
								(*iter)->highlight();
								(*iter)->draw();
								(*iter)->unhighlight();
							}
						}
					}
				}
				if (this->tree->GetItemParent(item) == (*this->tiItems.find(WARP_FRONT_ELEMENTS)).second)
				{
					if (this->IsAdjacentElementHighlighted())
					{
						Data::FrontElement *fel = static_cast<Data::FrontElement *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

						Data::ElementList adjs = (this->GetWarpMesh()) ?
							this->GetWarpMesh()->adjacency(fel) : Data::ElementList();

						for (Data::ElementList::const_iterator iter = adjs.begin();
							 iter != adjs.end(); iter++)
						{
							(*iter)->highlight();
							(*iter)->draw();
							(*iter)->unhighlight();
						}
					}
				}

				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->highlight();
				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->draw();
				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->unhighlight();
			}
		}
	}
}

void CRAbMesh::TecGrafMsh2DModule::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	if (this->IsEnabled(WARP_VERTICES, TREE_ITEM))
	{
		if (this->GetWarpMesh())
		{
			Data::VertexList vertices = this->GetWarpMesh()->getVertices();
			vertices.sort(Data::Identifiable::less);

			while (!vertices.empty())
			{
				this->AddTreeItem(vertices.front());

				vertices.pop_front();
			}
		}
		else if (this->GetWarpBoundary())
		{
			Data::VertexList vertices = this->GetWarpBoundary()->getVertices();

			vertices.sort(Data::Identifiable::less);

			while (!vertices.empty())
			{
				this->AddTreeItem(vertices.front());

				vertices.pop_front();
			}
		}
	}

	if (this->IsEnabled(WARP_FRONT_ELEMENTS, TREE_ITEM))
	{
		if (this->GetWarpMesh())
		{
			Data::FrontElementList frontElements = this->GetWarpMesh()->getFrontElements();
			frontElements.sort(Data::Identifiable::less);

			while (!frontElements.empty())
			{
				this->AddTreeItem(frontElements.front());

				frontElements.pop_front();
			}
		}
		else if (this->GetWarpBoundary())
		{
			Data::FrontElementList frontElements = this->GetWarpBoundary()->getElements();
			frontElements.sort(Data::Identifiable::less);

			while (!frontElements.empty())
			{
				this->AddTreeItem(frontElements.front());

				frontElements.pop_front();
			}
		}
	}

	if (this->IsEnabled(WARP_ELEMENTS, TREE_ITEM))
	{
		if (this->GetWarpMesh())
		{
			Data::ElementList elements = this->GetWarpMesh()->getElements();

			elements.sort(Data::Identifiable::less);

			while (!elements.empty())
			{
				this->AddTreeItem(elements.front());

				elements.pop_front();
			}
		}
	}

	MeshModule::ForceFillTree(false);
}

void CRAbMesh::TecGrafMsh2DModule::AddTreeItem(Data::Vertex *v)
{
	bool added = false;

	if (this->IsEnabled(WARP_VERTICES, TREE_ITEM))
	{
		if (((this->GetWarpMesh()) && (this->GetWarpMesh()->have(v))) ||
			((this->GetWarpBoundary()) && (this->GetWarpBoundary()->have(v))))
		{
			TreeItemData *item = new TreeItemData(v);
			this->tree->AppendItem(this->tiItems[WARP_VERTICES], item->GetText(), -1, -1, item);

			added = true;
		}
	}

	if (!added)
	{
		CRAbMesh::MeshModule::AddTreeItem(v);
	}
}

void CRAbMesh::TecGrafMsh2DModule::AddTreeItem(Data::FrontElement *e)
{
	bool added = false;

	if (this->IsEnabled(WARP_FRONT_ELEMENTS, TREE_ITEM))
	{
		if (((this->GetWarpMesh()) && (this->GetWarpMesh()->have(e))) ||
			((this->GetWarpBoundary()) && (this->GetWarpBoundary()->have(e))))
		{
			TreeItemData *item = new TreeItemData(e);
			this->tree->AppendItem(this->tiItems[WARP_FRONT_ELEMENTS], item->GetText(), -1, -1, item);

			added = true;
		}
	}

	if (!added)
	{
		CRAbMesh::MeshModule::AddTreeItem(e);
	}
}

void CRAbMesh::TecGrafMsh2DModule::AddTreeItem(Data::Element *e)
{
	bool added = false;

	if (this->IsEnabled(WARP_ELEMENTS, TREE_ITEM))
	{
		if ((this->GetWarpMesh()) && (this->GetWarpMesh()->have(e)))
		{
			TreeItemData *item = new TreeItemData(e);
			this->tree->AppendItem(this->tiItems[WARP_ELEMENTS], item->GetText(), -1, -1, item);

			added = true;
		}
	}

	if (!added)
	{
		CRAbMesh::MeshModule::AddTreeItem(e);
	}
}

#endif //#if USE_TECGRAF_MSH_2D
#endif //#if USE_GUI
