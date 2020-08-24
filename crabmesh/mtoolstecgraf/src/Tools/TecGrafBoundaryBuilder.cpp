#include "Tools/TecGrafBoundaryBuilder.h"

#if USE_TECGRAF

#include "Data/Boundary.h"
#include "MJMesh/Mesh.h"
#include "TecGraf/Model.h"
#include "Tools/MeshTransformer.h"

using namespace Tools;

Tools::TecGrafBoundaryBuilder::TecGrafBoundaryBuilder(bool sorted) :
	TecGraf::BoundaryBuilder::BoundaryBuilder(sorted)
{
	this->model = NULL;
	this->makeMeshes = false;
}

Tools::TecGrafBoundaryBuilder::~TecGrafBoundaryBuilder()
{

}

void Tools::TecGrafBoundaryBuilder::setModel(TecGraf::Model *model)
{
	this->model = model;
}

TecGraf::Model *Tools::TecGrafBoundaryBuilder::getModel() const
{
	return this->model;
}

void Tools::TecGrafBoundaryBuilder::setMakeMeshes(bool makeMeshes)
{
	this->makeMeshes = makeMeshes;
}

const Data::BoundaryList &Tools::TecGrafBoundaryBuilder::getBoundaries() const
{
	return this->boundaries;
}

const Data::MeshList &Tools::TecGrafBoundaryBuilder::getMeshes() const
{
	return this->meshes;
}

const Data::Point3DObjList &Tools::TecGrafBoundaryBuilder::getTranslations() const
{
    return this->translations;
}

Data::Mesh *Tools::TecGrafBoundaryBuilder::makeMesh() const
{
	return new MJMesh::Mesh();
}

void Tools::TecGrafBoundaryBuilder::execute()
{
	if (!this->model)
	{
		return;
	}

	this->translations.clear();

	this->startMeasure(this->model->numRegions());

	for (TecGraf::RegionList::iterator iter = this->model->rBegin();
		 iter != this->model->rEnd(); iter++)
	{
		this->region = (*iter);

		this->TecGraf::BoundaryBuilder::execute();

		this->boundaries.push_back(this->boundary);

		if (this->makeMeshes)
		{
			Data::Mesh *mesh = this->makeMesh();

			Data::EPoint3D pt;

			for (Data::Boundary::VertexIterator iter2 = this->boundary->vBegin();
				 iter2 != this->boundary->vEnd(); iter2++)
			{
				mesh->add((*iter2), false);

				for (UInt i = 0; i < 3; i++)
                {
                    pt.setCoord(i, pt.getCoord(i) + (*iter2)->getPoint()->getCoord(i));
                }
			}

			for (Data::Boundary::FrontElementIterator iter2 = this->boundary->eBegin();
				 iter2 != this->boundary->eEnd(); iter2++)
			{
				mesh->add((*iter2));
			}

			for (UInt i = 0; i < 3; i++)
            {
                pt.setCoord(i, -pt.getCoord(i)/static_cast<Real>(this->boundary->verticesSize()));
            }

			Tools::MeshTransformer t;

			t.setMesh(mesh);
			t.translate(pt.point3D());
			t.execute();

			this->translations.push_back(pt.point3D());

			this->meshes.push_back(mesh);
		}

		this->boundary = NULL;

		this->clear();

		this->addMeasure(1, 0);
	}

	this->endMeasure();
}

#endif //#if USE_TECGRAF
