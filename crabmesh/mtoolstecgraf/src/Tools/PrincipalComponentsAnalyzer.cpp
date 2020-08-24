#include "Tools/PrincipalComponentsAnalyzer.h"

#if USE_NUMERICAL

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/RowMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/Mesh.h"

Tools::PrincipalComponentsAnalyzer::PrincipalComponentsAnalyzer(const Tool *tool) :
	Performer::PrincipalComponentsAnalyzer::PrincipalComponentsAnalyzer(),
	Tools::ExternalTool::ExternalTool(tool)
{
    this->mesh = NULL;
}

Tools::PrincipalComponentsAnalyzer::~PrincipalComponentsAnalyzer()
{

}

Data::Mesh *Tools::PrincipalComponentsAnalyzer::getMesh() const
{
    return this->mesh;
}

void Tools::PrincipalComponentsAnalyzer::execute()
{
    Performer::PrincipalComponentsAnalyzer::execute();

    this->mesh = new MJMesh::Mesh();

    if (this->getBoundary()->dimension() == 2)
    {
        Data::Point2D *pt = new Data::Point2D(this->mid->getElement(0), this->mid->getElement(1));
        MJMesh::Vertex *mid = new MJMesh::Vertex(pt, 1);

        Data::Point2D *pts[2] = {new Data::Point2D(), new Data::Point2D()};
        MJMesh::Vertex *vts[2] = {new MJMesh::Vertex(pts[0], 2), new MJMesh::Vertex(pts[1], 3)};

        UInt order[2] = {0, 1};

        //if (this->eigenvalues->getElement(1) > this->eigenvalues->getElement(0))
        if (this->eigenvalues->getElement(1) > this->eigenvalues->getElement(0) + Data::getTolerance())
        {
            order[0] = 1;
            order[1] = 0;
        }

        for (UInt i = 0; i < 2; i++)
        {
            UInt k = order[i];

            for (UInt j = 0; j < 2; j++)
            {
                pts[i]->setCoord(j, this->eigenvectors->getElement(j, k)*this->eigenvalues->getElement(k) + pt->getCoord(j));
            }
        }

        MJMesh::FrontElement2D *fels[2];

        fels[0] = new MJMesh::FrontElement2D(mid, vts[0], 1);
        fels[1] = new MJMesh::FrontElement2D(mid, vts[1], 2);

#if USE_GUI
        mid->setColor(1.0, 1.0, 1.0);
        vts[0]->setColor(1.0, 0.0, 0.0);
        vts[1]->setColor(0.0, 1.0, 0.0);

        fels[0]->setColor(1.0, 0.0, 0.0);
        fels[1]->setColor(0.0, 1.0, 0.0);
#endif //#if USE_GUI

        this->mesh->add(mid);
        this->mesh->add(vts[0]);
        this->mesh->add(vts[1]);

        this->mesh->add(fels[0]);
        this->mesh->add(fels[1]);
    }
    else
    {
        Data::Point3D *pt = new Data::Point3D(this->mid->getElement(0), this->mid->getElement(1), this->mid->getElement(2));
        MJMesh::Vertex *mid = new MJMesh::Vertex(pt, 1);

        Data::Point3D *pts[3] = {new Data::Point3D(), new Data::Point3D(), new Data::Point3D()};
        MJMesh::Vertex *vts[3] = {new MJMesh::Vertex(pts[0], 2), new MJMesh::Vertex(pts[1], 3), new MJMesh::Vertex(pts[2], 4)};

        UInt order[3] = {0, 1, 2};

        for (UInt j = 0; j < 3; j++)
        {
            UInt k = (j+1)%3;
            UInt k2 = (j+2)%3;

            if ((this->eigenvalues->getElement(j) > this->eigenvalues->getElement(k) + Data::getTolerance()) &&
                (this->eigenvalues->getElement(j) > this->eigenvalues->getElement(k2) + Data::getTolerance()))
            {
                order[0] = j;

                if (this->eigenvalues->getElement(k2) > this->eigenvalues->getElement(k) + Data::getTolerance())
                {
                    order[1] = k2;
                    order[2] = k;
                }
                else
                {
                    order[1] = k;
                    order[2] = k2;
                }

                break;
            }
        }

        for (UInt i = 0; i < 3; i++)
        {
            UInt k = order[i];

            for (UInt j = 0; j < 3; j++)
            {
                pts[i]->setCoord(j, this->eigenvectors->getElement(j, k)*this->eigenvalues->getElement(k) + pt->getCoord(j));
            }
        }

        MJMesh::FrontElementSurface *fels[3];

        fels[0] = new MJMesh::FrontElementSurface(mid, vts[0], 1);
        fels[1] = new MJMesh::FrontElementSurface(mid, vts[1], 2);
        fels[2] = new MJMesh::FrontElementSurface(mid, vts[2], 3);

#if USE_GUI
        mid->setColor(1.0, 1.0, 1.0);
        vts[0]->setColor(1.0, 0.0, 0.0);
        vts[1]->setColor(0.0, 1.0, 0.0);
        vts[2]->setColor(0.0, 0.0, 1.0);

        fels[0]->setColor(1.0, 0.0, 0.0);
        fels[1]->setColor(0.0, 1.0, 0.0);
        fels[2]->setColor(0.0, 0.0, 1.0);
#endif //#if USE_GUI

        this->mesh->add(mid);
        this->mesh->add(vts[0]);
        this->mesh->add(vts[1]);
        this->mesh->add(vts[2]);

        this->mesh->add(fels[0]);
        this->mesh->add(fels[1]);
        this->mesh->add(fels[2]);
    }
}

#endif //#if USE_NUMERICAL
