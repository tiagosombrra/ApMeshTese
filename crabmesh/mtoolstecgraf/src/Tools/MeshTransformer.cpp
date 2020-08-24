#include "Tools/MeshTransformer.h"

#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Edge3D.h"
#include "MJMesh/Triangle3D.h"

Tools::MeshTransformer::MeshTransformer() :
	Performer::MeshTransformer::MeshTransformer(),
	Tools::Tool::Tool()
{

}

Tools::MeshTransformer::~MeshTransformer()
{

}

void Tools::MeshTransformer::execute()
{
	if ((!this->mesh) && (!this->boundary))
	{
		return;
	}

	this->startMeasure(this->boundary ? static_cast<UInt>(this->boundary->verticesSize()) : static_cast<UInt>(this->mesh->verticesSize()));

	Performer::MeshTransformer::execute();

#if USE_ATTRIBS
    if (this->boundary)
    {
        for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
             iter != this->boundary->eEnd(); iter++)
        {
            Data::GeometricShape *shape = (*iter)->getShape();

            MJMesh::Edge2D *e2d = dynamic_cast<MJMesh::Edge2D *>(shape);

            if (e2d)
            {
                e2d->clearAttribs();

                continue;
            }

            MJMesh::Edge3D *e3d = dynamic_cast<MJMesh::Edge3D *>(shape);

            if (e3d)
            {
                e3d->clearAttribs();

                continue;
            }

            MJMesh::Triangle3D *t3d = dynamic_cast<MJMesh::Triangle3D *>(shape);

            if (t3d)
            {
                t3d->clearAttribs();

                continue;
            }
        }
    }
    else
    {
        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd(); iter++)
        {
            Data::GeometricShape *shape = (*iter)->getShape();

            MJMesh::Edge2D *e2d = dynamic_cast<MJMesh::Edge2D *>(shape);

            if (e2d)
            {
                e2d->clearAttribs();

                continue;
            }

            MJMesh::Edge3D *e3d = dynamic_cast<MJMesh::Edge3D *>(shape);

            if (e3d)
            {
                e3d->clearAttribs();

                continue;
            }

            MJMesh::Triangle3D *t3d = dynamic_cast<MJMesh::Triangle3D *>(shape);

            if (t3d)
            {
                t3d->clearAttribs();

                continue;
            }
        }
    }
#endif //#if USE_ATTRIBS

	this->endMeasure();
}

void Tools::MeshTransformer::applyTranslation(Data::Point *p) const
{
	this->addMeasure(1, 0);

	Performer::MeshTransformer::applyTranslation(p);
}

void Tools::MeshTransformer::applyScale(Data::Point *p) const
{
	this->addMeasure(1, 0);

	Performer::MeshTransformer::applyScale(p);
}

void Tools::MeshTransformer::applyRotation(Data::Point *p) const
{
	this->addMeasure(1, 0);

	Performer::MeshTransformer::applyRotation(p);
}

void Tools::MeshTransformer::applyTransform(Data::Point *p, Real *matrix[4]) const
{
	this->addMeasure(1, 0);

	Performer::MeshTransformer::applyTransform(p, matrix);
}
