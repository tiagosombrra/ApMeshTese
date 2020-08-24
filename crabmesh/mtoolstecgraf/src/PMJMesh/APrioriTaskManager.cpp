#include "PMJMesh/APrioriTaskManager.h"

#include "Data/Edge.h"
#include "Data/Edge2D.h"
#include "Performer/IdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/QuadTreeCell.h"
#include "MJMesh/OctTreeCell.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/OctTree.h"
#include "MJMesh/Mesh.h"
#include "Parallel/MPIMessage.h"
#include "Parallel/LoadBalancer.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MinTreeRefiner.h"
#include "PMJMesh/PositionTreeRefiner.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/FrontPartitioner.h"
#include "PMJMesh/BoxTreeShifter.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/Communicator.h"
#include "PMJMesh/AngleTreeCellClassifier.h"
#include "PMJMesh/LoadCalculator.h"
#include "PMJMesh/AverageLoadTreePartitioner.h"
#include "PMJMesh/MinNumberTreePartitioner.h"
#include "PMJMesh/MinNumberBSPTreePartitioner.h"
#include "PMJMesh/SmartMinNumberBSPTreePartitioner.h"
#include "PMJMesh/MasterCellClassifier.h"
#include "PMJMesh/ActualFrontUpdater.h"
#include "PMJMesh/MeshUpdater.h"
#include "PMJMesh/PartialMeshUpdater.h"
#include "PMJMesh/EmptyMeshUpdater.h"
#include "PMJMesh/FinalMeshUpdater.h"
#include "PMJMesh/WorkerMainDrive.h"

using namespace PMJMesh;

PMJMesh::APrioriTaskManager::APrioriTaskManager(bool sharedParallel, UInt dimension,
	MJMesh::Boundary *boundary, Data::BoxTree *tree, Real percentageNonWorkingTask,
	bool heightDependentInLoad, bool smoothInSlavesEnabled, UInt numImproves, Real smoothingFactor,
#if (!USE_DELETE_FELS)
	UInt unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
	UInt smoothingLayer,	bool localBackTrackingEnabled,
	bool meshSorted, UInt maxBackTracks, enum PartitionerType partitionerType,
	enum MeshUpdateType updateType,
	bool usePartitioningThreshold, Real partitioningThreshold, bool sorted,
	bool usingSearchTrees, UInt minRefinementLevel, bool skippingInnerInDecomposition,
	bool createInnerFrontInDecomposition,
	enum Parallel::AvailableLoadBalancers balancerType,
	Parallel::LoadBalancer *loadBalancer, Parallel::Communicator *comm) :
    PMJMesh::GlobalTaskManager(sharedParallel, dimension, boundary, tree,
		percentageNonWorkingTask, heightDependentInLoad, 0, smoothInSlavesEnabled, numImproves,
		smoothingFactor,
#if (!USE_DELETE_FELS)
		unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
		smoothingLayer,
		localBackTrackingEnabled, meshSorted, maxBackTracks, partitionerType,
		updateType, PMJMesh::NONE,
		usePartitioningThreshold, partitioningThreshold, sorted,
		usingSearchTrees, 1.0, minRefinementLevel, skippingInnerInDecomposition,
		createInnerFrontInDecomposition, balancerType, loadBalancer, comm)
{

}

PMJMesh::APrioriTaskManager::~APrioriTaskManager()
{

}

void PMJMesh::APrioriTaskManager::interceptPartitionFront(Data::Box * box, Data::TreeNode *root,//)
                                            std::list<Data::TreeNode *> &topBoundaryCells, std::list<Data::TreeNode *> &baseBoundaryCells,
                                            std::list<Data::TreeNode *> &leftBoundaryCells, std::list<Data::TreeNode *> &rightBoundaryCells)
{
    UInt dim = box->dimension();

    //int num_children;
    UInt num_children;

    //#pragma omp critical (root)   //SOMENTE LEITURA
    num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        //#pragma omp critical (root)   //SOMENTE LEITURA
        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        if (!box->intercept(b))
            continue;


        if( !(n->isLeaf()) )
        {
            bool call = true;

            if (box->in(b))
            {
                call = false;

                for (UInt i = 0; i < dim; i++)
                {
                    if ((b->getMin()->getCoord(i) <= box->getMin()->getCoord(i) + Data::getTolerance()) ||
                        (b->getMax()->getCoord(i) >= box->getMax()->getCoord(i) - Data::getTolerance()))
                    {
                        call = true;

                        break;
                    }
                }
            }

            if(call)
            {
                interceptPartitionFront(box, n,//);
                                        topBoundaryCells, baseBoundaryCells, leftBoundaryCells, rightBoundaryCells);
                continue;
            }
        }


        bool on = true;

        if (box->in(b))
        {
            on = false;

            for (UInt i = 0; i < dim; i++)
            {
                if ((b->getMin()->getCoord(i) <= box->getMin()->getCoord(i) + Data::getTolerance()) ||
                    (b->getMax()->getCoord(i) >= box->getMax()->getCoord(i) - Data::getTolerance()))
                {
                    on = true;

                    break;
                }
            }
        }

        if(on)
        {
			if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::IN )
            {
#if USE_GUI
                //PINTA CELULAS DA BORDA DA PARTIÇÃO
                static_cast<Data::BoxTreeCell *>(n)->setColor(0.0, 0.0, 1.0);
                static_cast<Data::BoxTreeCell *>(n)->highlight();
#endif //#if USE_GUI

                //allBoundaryCells.push_back( n );

                if ( b->getMax()->getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ||
                     b->getMin()->getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) )
                    {
                    topBoundaryCells.push_back( n );
                    //std::cout << "top" << std::endl;
                    }

                if ( b->getMin()->getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                     b->getMax()->getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() )
                    {
                    baseBoundaryCells.push_back( n );
                    //std::cout << "base" << std::endl;
                    }
                if ( b->getMin()->getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                     b->getMax()->getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() )
                    {
                    leftBoundaryCells.push_back( n );
                    //std::cout << "left" << std::endl;
                    }


                if ( b->getMax()->getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) ||
                     b->getMin()->getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) )
                    {
                    rightBoundaryCells.push_back( n );
                    //std::cout << "right" << std::endl;
                    }

            }

        }

    }
}

//ORDENA PELO ALTURA EIXO Y
bool PMJMesh::APrioriTaskManager::compareBoxY(Data::TreeNode *first, Data::TreeNode *second)
{
#if USE_EXPERIMENTAL_3
    if( static_cast<Data::BoxTreeCell *>(first)->max2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(second)->max2D().getCoord(1) + Data::getTolerance() )
#else
    if( static_cast<Data::BoxTreeCell *>(first)->max().getCoord(1) <= static_cast<Data::BoxTreeCell *>(second)->max().getCoord(1) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
        return true;
    else
        return false;
}

//ORDENA PELO ALTURA EIXO X
bool PMJMesh::APrioriTaskManager::compareBoxX(Data::TreeNode *first, Data::TreeNode *second)
{
#if USE_EXPERIMENTAL_3
    if( static_cast<Data::BoxTreeCell *>(first)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(second)->min2D().getCoord(0) + Data::getTolerance() )
#else
    if( static_cast<Data::BoxTreeCell *>(first)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(second)->min().getCoord(0) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
        return true;
    else
        return false;
}

bool PMJMesh::APrioriTaskManager::existEdgeInGlobal(MJMesh::FrontElement *e)
{
    Data::FrontElementList list;
    bool value = false;

#if USE_OPENMP
    #pragma omp critical (front)    ///Front é alterada nesse techo, por isso pode acontecer de novos elementos serem adicionados a ela
#endif //#if USE_OPENMP
    {
        list = this->front->frontElementsInRange( e->getShape()->box() );

        if( list.size() == 0 )
            this->front->add(e);

        if( list.size() != 0 )
        {
            //return false;       //ADICIONAR AQUI !!!
            Data::VertexVector vertices = e->getVertices();

            std::list<Data::FrontElement *>::iterator iter;
            for (iter = list.begin(); iter != list.end(); iter++)
            {
                bool equal = false;
                Data::VertexVector verticesVec = (*iter)->getVertices();


                //for(uint i = 0; i < vertices.size(); i++)
                for(UInt i = 0; i < vertices.size(); i++)
                {
                    equal = false;

                    //for(uint j = 0; j < verticesVec.size(); j++)
                    for(UInt j = 0; j < verticesVec.size(); j++)
                    {
                        if( vertices[i]->getPoint()->equal( verticesVec[j]->getPoint() ) )
                        {
                            equal = true;   //IGUAL
                            j = static_cast<UInt>(verticesVec.size());
                            //break;
                        }
                    }

                    if( equal == false )
                        i = static_cast<UInt>(vertices.size());
                        //break;
                }

                if( equal == true )     //ACHOU
                {
                    e->setId( (*iter)->getId() );

                    for(uint i = 0; i < vertices.size(); i++)
                    {
                        for(uint j = 0; j < verticesVec.size(); j++)
                        {
                            if( vertices[i]->getPoint()->equal( verticesVec[j]->getPoint() ) )
                                vertices[i]->setId( verticesVec[j]->getId() );
                        }
                    }

                    value = true;
                    //return true;
                }
            }

            if(value == false)
            {
                //#pragma omp critical (front)
                this->front->add(e);
            }
        }
    }

        return value;
        //return false;
}

bool PMJMesh::APrioriTaskManager::boxEdgeTest(Data::Box *box, Data::TreeNode *root)
{
    //int num_children;
    UInt num_children;

    //#pragma omp critical (root)   //SOMENTE LEITURA
    num_children = root->numChildren();

    for(UInt i = 0; i < num_children; i++)
    {
        Data::TreeNode *n;

        //#pragma omp critical (root)   //SOMENTE LEITURA
        n = root->child(i);

        Data::Box *b = static_cast<Data::BoxTreeCell *>(n)->box();

        if (!box->intercept(b))
            continue;

        if( !(n->isLeaf()) )
        {
            if( boxEdgeTest(box, n) == false )
                return false;
        }
        else
        {
			if( PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n)) == Data::OUT )
                return false;
        }
    }
    return true;
}

MJMesh::Vertex* PMJMesh::APrioriTaskManager::nearestVertexSubFront(Data::Box *box, PMJMesh::Front *sub_front, Data::FrontElementList element_front, MJMesh::FrontElement2D *e, MJMesh::Vertex* vertex)
{
        double dist = HUGE_VAL;
        MJMesh::Vertex* v = NULL;
        Data::FrontElement *e_aux = NULL;

        for( std::list<Data::FrontElement *>::iterator it = element_front.begin(); it != element_front.end(); it++ )
        {
            if( e->intercept( (*it) ) || e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() )
                  || (*it)->in( e->getV1()->getPoint() ) || (*it)->in( e->getV2()->getPoint() ) )
            {

                if( e->in( (*it)->getVertex(1)->getPoint() ) )
                {
                    v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                    e_aux = (*it);
                    break;
                }
                if( e->in( (*it)->getVertex(0)->getPoint() ) )
                {
                    v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                    e_aux = (*it);
                    break;
                }


/*
                if( e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                    e->straightDistance( (*it)->getVertex(1)->getPoint() ) )
*/
                if( vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                    vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
                {
                    double d = vertex->getPoint()->distance( (*it)->getVertex(0)->getPoint() );
                    //double d = e->straightDistance( (*it)->getVertex(0)->getPoint() );

                    if(d <= dist)
                    {
                        dist = d;
                        v = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                        e_aux = (*it);
                    }
                }
                else
                {
                    double d = vertex->getPoint()->distance( (*it)->getVertex(1)->getPoint() );
                    //double d = e->straightDistance( (*it)->getVertex(1)->getPoint() );

                    if(d <= dist)
                    {
                        dist = d;
                        v = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                        e_aux = (*it);
                    }
                }
            }
        }

//std::cout << "PROCUROU 1: " << v->getId() << std::endl;
//std::cout << "PROCUROU 2: " << vertex->getId() << std::endl;

        if( v == NULL || v->getPoint()->equal(vertex->getPoint()) )
            return NULL;
//std::cout << "pasouuuuu" << std::endl;
        bool exist = false;
        Data::FrontElementList element = sub_front->getRejected();

        for( std::list<Data::FrontElement *>::iterator it = element.begin(); it != element.end(); it++ )
        {
//std::cout << "PROCURANDO" << std::endl;
            if( ( e_aux->getVertex(0)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || e_aux->getVertex(0)->getPoint()->equal((*it)->getVertex(1)->getPoint()) )
             || ( e_aux->getVertex(1)->getPoint()->equal((*it)->getVertex(0)->getPoint()) || e_aux->getVertex(1)->getPoint()->equal((*it)->getVertex(1)->getPoint()) ) )
            {
                exist = true;
                break;
            }
        }

        if(exist == false)
        {
            if( box->out(v->getPoint()) )
            {
                sub_front->addRejected(e_aux);
                sub_front->add(v);
            }
        }
//std::cout << "JA ERA" << std::endl;
        return v;
}

bool PMJMesh::APrioriTaskManager::includeInModel(Data::Box *box, PMJMesh::Front *sub_front, Data::FrontElementList element_front, Data::FrontElementList &rejected, MJMesh::FrontElement2D *e, MJMesh::Vertex *v1, MJMesh::Vertex *v2)
{
    Data::Box box1( *e->getV1()->getPoint(), *e->getV2()->getPoint() );
    Data::Box box2( *e->getV2()->getPoint(), *e->getV1()->getPoint() );

    bool flag;

    //#pragma omp critical (root)   //SOMENTE LEITURA
    flag = boxEdgeTest( &box1, tree->getRoot() ) == false || boxEdgeTest( &box2, tree->getRoot() ) == false;

    if( flag )
    {
        //PRIMEIRO VERTEX

        MJMesh::Vertex* v_axu1 = nearestVertexSubFront(box, sub_front, element_front, e, v1);

        MJMesh::FrontElement2D *edge1 ;

std::cout << "V1: " << v1->getId() << std::endl;
std::cout << "V2: " << v2->getId() << std::endl;

if(v_axu1 == NULL)
std::cout << "nulo 1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
else
std::cout << v_axu1->getId() << std::endl;

        if(v_axu1 != NULL)
        {
            edge1 = new MJMesh::FrontElement2D( v1, v_axu1, this->idManager->next(1) );

            existEdgeInGlobal(edge1);
            rejected.push_back( edge1 );

            removeDanglingEdges(box, sub_front, edge1);
        }
        //SEGUNDO VERTEX

        MJMesh::Vertex* v_axu2 = nearestVertexSubFront(box, sub_front, element_front, e, v2);

        MJMesh::FrontElement2D *edge2;

if(v_axu2 == NULL)
std::cout << "nulo 2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
else
std::cout << v_axu2->getId() << std::endl;

        if(v_axu2 != NULL)
        {
            edge2 = new MJMesh::FrontElement2D( v_axu2, v2, this->idManager->next(1) );

            existEdgeInGlobal(edge2);
            rejected.push_back( edge2 );

            removeDanglingEdges(box, sub_front, edge2);
        }


#if USE_OPENMP
        #pragma omp critical (mesh)
#endif //#if USE_OPENMP
        {
            if(v_axu1 != NULL)
            {
                this->mesh->add(edge1);
                this->mesh->add(v1);
                this->mesh->add(v_axu1);
            }
            if(v_axu2 != NULL)
            {
                this->mesh->add(edge2);
                this->mesh->add(v_axu2);
                this->mesh->add(v2);
            }
        }

        delete e;

        return false;
    }
    else
    {
        existEdgeInGlobal(e);
        rejected.push_back( e );

#if USE_OPENMP
        #pragma omp critical (mesh)
#endif //#if USE_OPENMP
        {
            this->mesh->add(e);
            this->mesh->add(e->getV1());
            this->mesh->add(e->getV2());
        }

        return true;
    }
}

MJMesh::Vertex* PMJMesh::APrioriTaskManager::bestVertex(PMJMesh::Front *sub_front, Data::Edge2D *e)
{
        Data::FrontElementList element = sub_front->getRejected();
        for( std::list<Data::FrontElement *>::iterator it = element.begin(); it != element.end(); it++ )
        {
            if( e->intercept( (*it)->getVertex(0)->getPoint(), (*it)->getVertex(1)->getPoint() ) ||
               e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() ) )
            {
                Data::Edge2D aux_edge = Data::Edge2D( static_cast<Data::Point2D*>((*it)->getVertex(0)->getPoint()), static_cast<Data::Point2D*>((*it)->getVertex(1)->getPoint()) );
                if( (aux_edge).right( e->getP2() ) )
                {
                    return  findOrCreate(sub_front, e->getP1()->getCoord(0),
                                                    e->getP1()->getCoord(1));
                }
                else
                {
                    return  findOrCreate(sub_front, e->getP2()->getCoord(0),
                                                    e->getP2()->getCoord(1));
                }
            }
        }

        return  findOrCreate(sub_front, e->getP1()->getCoord(0),
                                        e->getP1()->getCoord(1));
}

//MARKOS
MJMesh::Vertex *PMJMesh::APrioriTaskManager::findOrCreate(PMJMesh::Front *sub_front, Real x, Real y, Real z)
{
#if USE_EXPERIMENTAL_3
	Data::Point2D p;
#else
	Data::Point p(this->dimension);
#endif //#if USE_EXPERIMENTAL_3

	p.setCoord(0, x);
	p.setCoord(1, y);

	if (this->dimension > 2)
	{
		p.setCoord(2, z);
	}

	Data::Box b(p, p);

	Data::VertexList vertices;
	MJMesh::Vertex *v;

    vertices = sub_front->verticesInRange(b);

    if (!vertices.empty())
    {
        return static_cast<MJMesh::Vertex *>(vertices.front());
    }

    v = new MJMesh::Vertex(
        (this->dimension == 2) ?
            static_cast<Data::Point *>(new Data::Point2D(p)) :
            static_cast<Data::Point *>(new Data::Point3D(p)),
        this->idManager->next(0));

    sub_front->add(v);

#if USE_OPENMP
    #pragma omp critical (front)
#endif //#if USE_OPENMP
    {
        vertices = this->front->verticesInRange(b);

        if (vertices.empty())
        {
            this->front->add(v);
        }
        else
        {
            v->setId(vertices.front()->getId());
        }
    }

	return v;
}

MJMesh::FrontElement2D * PMJMesh::APrioriTaskManager::InitialEdge(Data::Box *box, Data::FrontElementList element_front, PMJMesh::Front *sub_front, Data::FrontElementList &rejected, MJMesh::Vertex *vMin, MJMesh::FrontElement2D *e, bool invert)
{
std::cout << "Fazendo chamada: " << element_front.size() << std::endl;
    double dist = HUGE_VAL;
    MJMesh::Vertex *v_aux = NULL;

    Data::FrontElementList element = sub_front->getRejected();

    for( std::list<Data::FrontElement *>::iterator it = element.begin(); it != element.end(); it++ )
    {
        if( e->intercept( (*it) ) || e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() )
           || (*it)->in( e->getV1()->getPoint() ) || (*it)->in( e->getV2()->getPoint() ) )
        {
            /*
            if( vMin->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                vMin->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) )
                */
            if( e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                e->straightDistance( (*it)->getVertex(1)->getPoint() ) )
            {
               /*if( v_aux == NULL || vMin->getPoint()->distance( (*it)->getVertex(0)->getPoint() ) <=
                                     vMin->getPoint()->distance( v_aux->getPoint() ) )
                */
                double d = e->straightDistance( (*it)->getVertex(0)->getPoint() );

                if( v_aux == NULL || d <= dist)
                {
                    dist = d;
                    v_aux = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                }

            }
            else
            {
                /*
                if( v_aux == NULL || vMin->getPoint()->distance( (*it)->getVertex(1)->getPoint() ) <=
                                     vMin->getPoint()->distance( v_aux->getPoint() ) )
                */
                double d = e->straightDistance( (*it)->getVertex(1)->getPoint() );

                if(v_aux == NULL || d <= dist)
                {
                    dist = d;
                    v_aux = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                }
            }
        }
    }

if(v_aux == NULL)
    std::cout << "nulo!" << std::endl;

    if(v_aux == NULL)
    {
        Data::FrontElement *e_aux;
        for( std::list<Data::FrontElement *>::iterator it = element_front.begin(); it != element_front.end(); it++ )
        {
            if( ( e->intercept( (*it) ) || e->in( (*it)->getVertex(0)->getPoint() ) || e->in( (*it)->getVertex(1)->getPoint() )
               || (*it)->in( e->getV1()->getPoint() ) || (*it)->in( e->getV2()->getPoint() ) ) )
            {
std::cout << "Vazio: " << (*it)->getId() << "  " << sub_front->getRejected().size() << std::endl;
                if( e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                    e->straightDistance( (*it)->getVertex(1)->getPoint() ) )
                {
                    if( v_aux == NULL || e->straightDistance( (*it)->getVertex(0)->getPoint() ) <=
                        e->straightDistance( v_aux->getPoint() ) )
                    {
                        if(vMin->getPoint()->equal((*it)->getVertex(0)->getPoint()))
                            continue;

                        v_aux = static_cast<MJMesh::Vertex*>((*it)->getVertex(0));
                        e_aux = (*it);
                    }
                }
                else
                {
                    if( v_aux == NULL || e->straightDistance( (*it)->getVertex(1)->getPoint() ) <=
                                     e->straightDistance( v_aux->getPoint() ) )
                    {
                        if(vMin->getPoint()->equal((*it)->getVertex(1)->getPoint()))
                            continue;

                        v_aux = static_cast<MJMesh::Vertex*>((*it)->getVertex(1));
                        e_aux = (*it);
                    }
                }
            }
        }
        if( box->out(v_aux->getPoint()) )         //1 VERTICE FORA
        {
            sub_front->addRejected(e_aux);
            sub_front->add(v_aux);
        }

    }



    if(invert == false)
    {
        MJMesh::FrontElement2D *edge = new MJMesh::FrontElement2D( v_aux, vMin, this->idManager->next(1) );

std::cout << "DEUBG 1: " << edge->getV1()->getPoint()->getCoord(0) << std::endl;
std::cout << "DEUBG 2: " << edge->getV2()->getPoint()->getCoord(0) << std::endl;
        if(includeInModel(box, sub_front, element_front, rejected, edge, v_aux, vMin))
            return edge;
        else
            return NULL;
    }
    else
    {
        MJMesh::FrontElement2D *edge = new MJMesh::FrontElement2D( vMin, v_aux, this->idManager->next(1) );

std::cout << "DEUBG 1: " << edge->getV1()->getPoint()->getCoord(0) << std::endl;
std::cout << "DEUBG 2: " << edge->getV2()->getPoint()->getCoord(0) << std::endl;
        if(includeInModel(box, sub_front, element_front, rejected, edge, vMin, v_aux))
            return edge;
        else
            return NULL;
    }
}

void PMJMesh::APrioriTaskManager::removeOutEdges(Data::Box *box, PMJMesh::Front *sub_front)
{
	return;
    Data::FrontElementList rej = sub_front->getRejected();

    for( Data::FrontElementList::iterator it = rej.begin(); it != rej.end(); )
    {
        Data::VertexVector vertices = (*it)->getVertices();

        if (!box->in(vertices[0]->getPoint()) && !box->in(vertices[1]->getPoint()))         //2 VERTICES FORA
        {
            if( box->on(vertices[0]->getPoint()) && box->on(vertices[1]->getPoint()) )
            {
                it++;
                continue;
            }
std::cout << "REMO: " << (*it)->getId() << std::endl;
            sub_front->remove( (*it) );

            it = rej.erase(it);

            if(box->out(vertices[1]->getPoint()))
                {
std::cout << "FOI 1: " << vertices[1]->getId() << std::endl;
                sub_front->remove(vertices[1]);
                }
            else
                {
std::cout << "FOI 2: " << vertices[0]->getId() << std::endl;
                sub_front->remove(vertices[0]);
                }
        }
        else
            it++;
    }
    sub_front->setRejected(rej);
}

void PMJMesh::APrioriTaskManager::removeDanglingEdges(Data::Box *box, PMJMesh::Front *sub_front, MJMesh::FrontElement *e)
{
	return;
std::cout << "removeDanglingEdges " << e->getId() << std::endl;
    if( !box->out(e->getV1()->getPoint()) && !box->out(e->getV2()->getPoint()) )
    {
        Data::FrontElementList rej = sub_front->getRejected();
std::cout << "TAM: " << rej.size() << std::endl;
        for( Data::FrontElementList::iterator it = rej.begin(); it != rej.end(); )
        {
            Data::VertexVector vertices = (*it)->getVertices();

            if (box->out(vertices[0]->getPoint()) || box->out(vertices[1]->getPoint()))         //1 VERTICE FORA
            {
                if(  e->getV1()->getPoint()->equal(vertices[0]->getPoint()) || e->getV1()->getPoint()->equal(vertices[1]->getPoint())  ||
                     e->getV2()->getPoint()->equal(vertices[0]->getPoint()) || e->getV2()->getPoint()->equal(vertices[1]->getPoint())
                   )
                {
                    if( ( e->getV1()->getPoint()->equal(vertices[0]->getPoint()) || e->getV2()->getPoint()->equal(vertices[0]->getPoint()) ) )
                    {
                        if(box->in(vertices[0]->getPoint()))
                        {
std::cout << "REMO 3: " << (*it)->getId() << std::endl;
std::cout << "REMO 3: " << vertices[0]->getId() << std::endl;
                            sub_front->remove( (*it) );
                            sub_front->remove(vertices[1]);
                            it = rej.erase(it);
                        }
                        else
                            it++;
                    }
                    else
                    {
                        if(box->in(vertices[1]->getPoint()))
                        {
std::cout << "REMO 4: " << (*it)->getId() << std::endl;
std::cout << "REMO 4: " << vertices[1]->getId() << std::endl;
                            sub_front->remove( (*it) );
                            sub_front->remove(vertices[0]);
                            it = rej.erase(it);
                        }
                        else
                            it++;
                    }
                }
                else
                    it++;
            }
            else
                it++;

        }
        sub_front->setRejected(rej);
        std::cout << "OKEY" << std::endl;
    }
std::cout << "OKEY!!!!" << std::endl;
}

void PMJMesh::APrioriTaskManager::cleanOutEdges(Data::Box *box, PMJMesh::Front *sub_front)
{
    Data::FrontElementList crossing = sub_front->getCrossing();


    Data::VertexSet crossingVertices;

    Data::FrontElementList removed, disconsider;

    bool changed = false;

    do
    {
        changed = false;

        for (Data::FrontElementList::iterator iter = crossing.begin();
             iter != crossing.end();)
        {
            std::cout << "crossing.size = " << crossing.size() << "ID: " << (*iter)->getId() << std::endl;

            Data::FrontElement *fel = (*iter);

            UInt count[fel->numVertices()];

            for (UInt i = 0; i < fel->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

                //Data::FrontElementList adjs = v->getAdjacentFrontElements();

                count[i] = 1;

                //while (!adjs.empty())
                for (Data::FrontElementList::iterator iter2 = static_cast<MJMesh::Vertex *>(fel->getVertex(i))->feBegin();
                     iter2 != static_cast<MJMesh::Vertex *>(fel->getVertex(i))->feEnd(); iter2++)
                {
                    //Data::FrontElement *adj = adjs.front();
                    Data::FrontElement *adj = (*iter2);

                    //adjs.pop_front();

                    if (adj == fel)
                    {
                        continue;
                    }

                    //if (front->haveAny(Data::FrontElementList(1, adj)))
                    if (sub_front->haveAny(Data::FrontElementList(1, adj)))
                    {
                        count[i]++;

                        std::cout << "adjacent " << adj->getId() << " is on the front" << std::endl;
                    }
                }

                std::cout << "fel->id = " << fel->getId() << ", v->id = " << v->getId() << ", count = " << count[i] << std::endl;
            }

            //there are only two vertices
            bool remove = false;

            if ((count[0] == 1) || (count[1] == 1))
            {
                if (count[0] == 1)
                {
                    if (crossingVertices.insert(fel->getVertex(0)).second)
                    {
                        //this->front->remove(fel->getVertex(0));
                        sub_front->remove(fel->getVertex(0));
                    }
                }

                if (count[1] == 1)
                {
                    if (crossingVertices.insert(fel->getVertex(1)).second)
                    {
                        //this->front->remove(fel->getVertex(1));
                        sub_front->remove(fel->getVertex(1));
                    }
                }

                remove = true;
            }
            else
            {
                iter++;

                continue;
            }

            if (remove)
            {
                std::cout << "fel " << fel->getId() << " is non-manifold" << std::endl;

                //this->front->remove(fel);
                sub_front->remove(fel);

                removed.push_back(fel);

                iter = crossing.erase(iter);

                changed = true;
            }
        }
    }
    while (changed);

    //adicionado por Markos em 16/12/2011
    std::multimap<UInt, Data::Vertex *> countVertices;

    //VertexList vertices = this->front->getVertices();
    Data::VertexList vertices = sub_front->getVertices();

    while (!vertices.empty())
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());

        vertices.pop_front();

        if (this->comm->isMaster())
        {
            UInt count = 0;

            //Data::FrontElementList fels = v->getAdjacentFrontElements();

            //while (!fels.empty())
            for (Data::FrontElementList::iterator iter = v->feBegin();
                iter != v->feEnd(); iter++)
            {
                //Data::FrontElement *fel = fels.front();
                Data::FrontElement *fel = (*iter);

                //fels.pop_front();

                //if (this->front->haveAny(Data::FrontElementList(1, fel)))
                if (sub_front->haveAny(Data::FrontElementList(1, fel)))
                {
                    count++;
                }
                else
                {
                    disconsider.push_back(fel);
                }
            }

            countVertices.insert(std::make_pair(count, v));
        }
        else
        {
            countVertices.insert(std::make_pair(v->numAdjacentFrontElements(), v));
        }
    }

    while (true)
    {
        //VertexMultiMap::iterator iter = countVertices.lower_bound(1);
        VertexMultiMap::iterator iter = countVertices.begin();

        if ((*iter).first != 1)
        {
            break;
        }

        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second);

        countVertices.erase(iter);

        if (crossingVertices.insert(v).second)
        {
            //this->front->remove(v);
            sub_front->remove(v);

            //Data::FrontElementList fels = v->getAdjacentFrontElements();

            //while (!fels.empty())
            for (Data::FrontElementList::iterator iter2 = v->feBegin();
                 iter2 != v->feEnd(); iter2++)
            {
                //Data::FrontElement *fel = fels.front();
                Data::FrontElement *fel = (*iter2);

                //fels.pop_front();

                if ((this->comm->isMaster()) && (std::find(disconsider.begin(), disconsider.end(), fel) != disconsider.end()))
                {
                    continue;
                }

                if (std::find(removed.begin(), removed.end(), fel) == removed.end())
                {
                    removed.push_back(fel);

                    //this->front->remove(fel);
                    sub_front->remove(fel);

                    Data::Vertex *other = (fel->getVertex(0) == v) ? fel->getVertex(1) : fel->getVertex(0);

                    for (iter = countVertices.begin();
                         iter != countVertices.end(); iter++)
                    {
                        if ((*iter).second == other)
                        {
                            UInt count = (*iter).first;

                            countVertices.erase(iter);

                            if (count == 1)
                            {
                                if (crossingVertices.insert(other).second)
                                {
                                    //this->front->remove(other);
                                    sub_front->remove(other);
                                }
                            }
                            else
                            {
                                countVertices.insert(std::make_pair(count - 1, other));
                            }

                            break;
                        }
                    }
                }
            }
        }
    }
std::cout << "crossing.size = " << crossing.size() << std::endl;
    crossing.swap(removed);

}

// ORIGINAL
void PMJMesh::APrioriTaskManager::buildSubDomain(Data::Box * box, PMJMesh::Front *sub_front)
{

/*
#pragma omp critical
{
    Data::FrontElementList rejected = sub_front->getRejected();
    Data::FrontElementList crossing = sub_front->getCrossing();

    std::cout << "Tamanho:   " << sub_front->getRejected().size() << std::endl;
    std::cout << "Tamanho REJ:   " << rejected.size() << std::endl; //novas
    std::cout << "Tamanho CROSS: " << crossing.size() << std::endl; //novas

    for( Data::FrontElementList::iterator it = rejected.begin(); it != rejected.end(); it++ )
    {
        std::cout << "rej " << (*it)->getId() << std::endl;
    }

    for( Data::FrontElementList::iterator it = crossing.begin(); it != crossing.end(); it++ )
    {
        std::cout << "cross " << (*it)->getId() << std::endl;
    }
}
*/
    std::list<Data::TreeNode *> topBoundaryCells;
    std::list<Data::TreeNode *> baseBoundaryCells;
    std::list<Data::TreeNode *> leftBoundaryCells;
    std::list<Data::TreeNode *> rightBoundaryCells;
    //std::list<Data::TreeNode *> allBoundaryCells;

    //Data::FrontElementList element_front = this->front->getAllElements();
    Data::FrontElementList element_front = this->front->getBoundary();
    removeOutEdges(box, sub_front);     //Remove arestas completamente fora do subdominio

    Data::TreeNode *root;

    //#pragma omp critical (root)   //SOMENTE LEITURA
    root = tree->getRoot();

    interceptPartitionFront( box, root, topBoundaryCells, baseBoundaryCells, leftBoundaryCells, rightBoundaryCells);

    std::list<Data::TreeNode *> onTopBoundaryCells;
    std::list<Data::TreeNode *> onBaseBoundaryCells;
    std::list<Data::TreeNode *> onLeftBoundaryCells;
    std::list<Data::TreeNode *> onRightBoundaryCells;

    UInt dim = box->dimension();
    std::list<Data::TreeNode *>::iterator iter;
    std::list<Data::TreeNode *>::iterator iter2;

    Data::FrontElementList rejected;// = sub_front->getRejected();

    for (UInt i = 0; i < 2*dim; i++)
    {
        switch(i)
        {

            //DIREITA
            case 0:
            {
std::cout << "DIREITA" << std::endl;
                if( rightBoundaryCells.size() == 0 ) continue;

                //DEIXA APENAS ARESTAS IN
                for (iter = rightBoundaryCells.begin(); iter != rightBoundaryCells.end(); )
                {

                    if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
                    {
                        onRightBoundaryCells.push_back( (*iter) );

                        iter = rightBoundaryCells.erase( iter );
                    }
					else
					{
						iter++;
					}
                }

                onRightBoundaryCells.sort( compareBoxY );
                rightBoundaryCells.sort( compareBoxY );

                iter = rightBoundaryCells.begin();
                iter2 = onRightBoundaryCells.begin();

                while( (iter2 != onRightBoundaryCells.end()) &&
#if USE_EXPERIMENTAL_3
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#else
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#endif //#if USE_EXPERIMENTAL_3
                {
                    iter2++;
                }

                MJMesh::Vertex *vMin;
                if(rightBoundaryCells.empty())
                {
                    //vMin = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));
#if USE_EXPERIMENTAL_3
                    if( iter2 != onRightBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) < box->getMax()->getCoord(1) )
#else
                    if( iter2 != onRightBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) < box->getMax()->getCoord(1) )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    }
                    else
                    {
                        Data::Point2D p1 = Data::Point2D(box->getMax()->getCoord(0), box->getMin()->getCoord(1));
                        Data::Point2D p2 = Data::Point2D(box->getMax()->getCoord(0), box->getMax()->getCoord(1));

                        Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                        vMin = bestVertex( sub_front, box_edge);
                    }
                }
                else
                {
                    if( iter2 == onRightBoundaryCells.end() )
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    else
                    {
#if USE_EXPERIMENTAL_3
                        if ( static_cast<Data::BoxTreeCell *>(*iter )->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) )
#else
                        if ( static_cast<Data::BoxTreeCell *>(*iter )->min().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) )
#endif //#if USE_EXPERIMENTAL_3
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), this->idManager->next(0));
                        }
                        else
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min().getCoord(1), this->idManager->next(0));
                        }
                }

                }

                //CRIA AS ARESTAS INICIAIS
                if( vMin->getPoint()->getCoord(1) > box->getMin()->getCoord(1) + Data::getTolerance()  )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMin()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::FrontElement2D * e0 = InitialEdge(box, element_front, sub_front, rejected, vMin, e_ori, false);

                    if(e0 != NULL )
                        removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"

                    delete e_ori;
                    delete v;
                }

                //CRIA AS ARESTAS INTERNAS
                //while( iter!=rightBoundaryCells.end() && iter2!=onRightBoundaryCells.end() )
                while( iter!=rightBoundaryCells.end() || iter2!=onRightBoundaryCells.end() )
                {
                    while( (iter2 != onRightBoundaryCells.end()) &&
#if USE_EXPERIMENTAL_3
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#else
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#endif //#if USE_EXPERIMENTAL_3
                    {
                        iter2++;
                    }

                    if( iter == rightBoundaryCells.end() || iter2 == onRightBoundaryCells.end())
                        break;

                    if( (iter == rightBoundaryCells.end() && iter2!=onRightBoundaryCells.end() ) || rightBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMin, vMax);

                        vMin = vMax;

                        iter2++;

                        continue;
                    }

                    if( (iter2 == onRightBoundaryCells.end() && iter!=rightBoundaryCells.end()) || onRightBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMin, vMax);

                        vMin = vMax;

                        iter++;

                        continue;
                    }

                    //CELULAS IGUAIS
#if USE_EXPERIMENTAL_3
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) + Data::getTolerance() )
#else
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vMin, vAux) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) )
#endif //#if USE_EXPERIMENTAL_3
                        {
                            iter2++;
                        }

                        iter++;
                    }
                    else
                    {
                        //CELULA DO LADO EXTERNO MENOR
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vMin, vAux) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

                        iter2++;
                    }
                }

                //CRIA AS ARESTAS FINAIS
                if( vMin->getPoint()->getCoord(1) + Data::getTolerance() < box->getMax()->getCoord(1) )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMax()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMax()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, element_front, e_ori, vMin);
                    if(vFinal == NULL)
                        vFinal = findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMax()->getCoord(1));
std::cout << "FINAISSSSSSSSSSSSSSSSSSs: " << vMin->getId() << std::endl;
std::cout << "FINAISSSSSSSSSSSSSSSSSSs: " << vFinal->getId() << std::endl;
                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vFinal, this->idManager->next(1) );

                    removeDanglingEdges(box, sub_front, e);

                    //includeInModel(box, sub_front, element_front, rejected, e, vFinal, vMin);
                    includeInModel(box, sub_front, element_front, rejected, e, vMin, vFinal);

                    delete e_ori;
                    delete v;
                }

            }
            break;

            //TOP
            case 1:
            {
std::cout << "TOP" << std::endl;
                if( topBoundaryCells.empty() ) continue;

                for (iter = topBoundaryCells.begin(); iter != topBoundaryCells.end(); )
                {
                    if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
                    {
                        onTopBoundaryCells.push_back( (*iter) );

                        iter = topBoundaryCells.erase( iter );
                    }
                    else
					{
						iter++;
					}

                }

                onTopBoundaryCells.sort( compareBoxX );
                topBoundaryCells.sort( compareBoxX );

                iter = topBoundaryCells.begin();
                iter2 = onTopBoundaryCells.begin();

                while( (iter2 != onTopBoundaryCells.end()) &&
#if USE_EXPERIMENTAL_3
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
#else
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
#endif //#if USE_EXPERIMENTAL_3
                {
                        iter2++;
                }


                MJMesh::Vertex *vMin;
                if ( topBoundaryCells.empty() )
				{
                    //vMin = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));
#if USE_EXPERIMENTAL_3
                    if( iter2 != onTopBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) < box->getMax()->getCoord(0) )
#else
                    if( iter2 != onTopBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) < box->getMax()->getCoord(0) )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    }
                    else
                    {
                        Data::Point2D p1 = Data::Point2D(box->getMin()->getCoord(0), box->getMax()->getCoord(1));
                        Data::Point2D p2 = Data::Point2D(box->getMax()->getCoord(0), box->getMax()->getCoord(1));

                        Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                        vMin = bestVertex( sub_front, box_edge);
                    }
				}
                else
                {
                    if( iter2 == onTopBoundaryCells.end() )
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    else
                    {
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) )
#endif //#if USE_EXPERIMENTAL_3
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max().getCoord(1), this->idManager->next(0));
                        }
                        else
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min().getCoord(1), this->idManager->next(0));
                        }
                    }
                }

                //CRIA AS ARESTAS INICIAIS
                if( vMin->getPoint()->getCoord(0) >= box->getMin()->getCoord(0) + Data::getTolerance()  )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMax()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::FrontElement2D * e0 = InitialEdge(box, element_front, sub_front, rejected, vMin, e_ori, true);

                    if(e0 != NULL )
                        removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"

                    delete e_ori;
                    delete v;
                }


                //CRIA AS ARESTAS INTERNAS
                //while( iter!=topBoundaryCells.end() && iter2!=onTopBoundaryCells.end())
                while( iter!=topBoundaryCells.end() || iter2!=onTopBoundaryCells.end())
                {
                    while( ( (iter2 != onTopBoundaryCells.end()) ) &&
#if USE_EXPERIMENTAL_3
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) )  )
#else
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) <= box->getMin()->getCoord(0) + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) + Data::getTolerance() >= box->getMax()->getCoord(0) )  )
#endif //#if USE_EXPERIMENTAL_3
                    {
                            iter2++;
                    }

                    if( iter == topBoundaryCells.end() && iter2 == onTopBoundaryCells.end())
                        break;


                    if( (iter == topBoundaryCells.end() && iter2!=onTopBoundaryCells.end()) || topBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMax, vMin);

                        vMin = vMax;

                        iter2++;

                        continue;
                    }

                    if( (iter2 == onTopBoundaryCells.end() && iter!=topBoundaryCells.end()) || onTopBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                                                includeInModel(box, sub_front, element_front, rejected, e, vMax, vMin);

                        vMin = vMax;

                        iter++;

                        continue;
                    }

                    //CELULAS IGUAIS  FICA COM A INTERNA
#if USE_EXPERIMENTAL_3
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) + Data::getTolerance() )
#else
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vAux, vMin) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) )
#endif //#if USE_EXPERIMENTAL_3
                        {
                            iter2++;
                        }

                        iter++;
                    }
                    else
                    {
                        //CELULA DO LADO EXTERNO MENOR
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vAux, vMin) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

                        iter2++;
                    }
                }

                //CRIA AS ARESTAS FINAIS
                if( vMin->getPoint()->getCoord(0) + Data::getTolerance() < box->getMax()->getCoord(0))
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMax()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMax()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, element_front, e_ori, vMin);
                    if(vFinal == NULL)
                        vFinal = findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMax()->getCoord(1));

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vFinal,vMin, this->idManager->next(1) );

                    removeDanglingEdges(box, sub_front, e);     //TESTA SE TEM ARESTAS "SOLTAS"

                    //includeInModel(box, sub_front, element_front, rejected, e, vMin, vFinal);
                    includeInModel(box, sub_front, element_front, rejected, e, vFinal, vMin );

                    delete e_ori;
                    delete v;
                }

            }
            break;

            //ESQUERDA
            case 2:
            {
std::cout << "ESQUERDA" << std::endl;
                if( leftBoundaryCells.size() == 0 ) continue;

                for (iter = leftBoundaryCells.begin(); iter != leftBoundaryCells.end(); )
                {
                    if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
                    {
                        onLeftBoundaryCells.push_back( (*iter) );

                        iter = leftBoundaryCells.erase( iter );
                    }
                    else
					{
						iter++;
					}
                }

                onLeftBoundaryCells.sort( compareBoxY );
                leftBoundaryCells.sort( compareBoxY );

                iter = leftBoundaryCells.begin();
                iter2 = onLeftBoundaryCells.begin();

                while( (iter2 != onLeftBoundaryCells.end()) &&
#if USE_EXPERIMENTAL_3
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#else
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#endif //#if USE_EXPERIMENTAL_3
                {
                    iter2++;
                }

                MJMesh::Vertex *vMin;

                if(leftBoundaryCells.empty())
                {
                    //vMin = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
#if USE_EXPERIMENTAL_3
                    if( iter2 != onLeftBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) < box->getMax()->getCoord(1) )
#else
                    if( iter2 != onLeftBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) < box->getMax()->getCoord(1) )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    }
                    else
                    {
                        Data::Point2D p1 = Data::Point2D(box->getMin()->getCoord(0), box->getMin()->getCoord(1));
                        Data::Point2D p2 = Data::Point2D(box->getMin()->getCoord(0), box->getMax()->getCoord(1));

                        Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                        vMin = bestVertex( sub_front, box_edge);
                    }
                }
                else
                {
                    if( iter2 == onLeftBoundaryCells.end() )
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    else
                    {
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) )
#endif //#if USE_EXPERIMENTAL_3
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), this->idManager->next(0));
                        }
                        else
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->min().getCoord(1), this->idManager->next(0));
                        }
                    }

                }

                //CRIA AS ARESTAS INICIAIS
                if( vMin->getPoint()->getCoord(1) > box->getMin()->getCoord(1) + Data::getTolerance()  )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMin()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D * e_ori= new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::FrontElement2D * e0 = InitialEdge(box, element_front, sub_front, rejected, vMin, e_ori, true);

                    if(e0 != NULL )
                        removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"

                    delete e_ori;
                    delete v;
                }


                //CRIA AS ARESTAS INTERNAS
                //while( iter!=leftBoundaryCells.end() && iter2!=onLeftBoundaryCells.end() )
                while( iter!=leftBoundaryCells.end() || iter2!=onLeftBoundaryCells.end() )
                {
                    while( (iter2 != onLeftBoundaryCells.end()) &&
#if USE_EXPERIMENTAL_3
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#else
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) <= box->getMin()->getCoord(1) + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() >= box->getMax()->getCoord(1) ) )
#endif //#if USE_EXPERIMENTAL_3
                    {
                        iter2++;
                    }

                    if( iter == leftBoundaryCells.end() || iter2 == onLeftBoundaryCells.end())
                        break;

                    if( (iter == leftBoundaryCells.end() && iter2!=onLeftBoundaryCells.end()) || leftBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMin, vMax);

                        vMin = vMax;

                        iter2++;

                        continue;
                    }

                    if( (iter2 == onLeftBoundaryCells.end() && iter!=leftBoundaryCells.end()) || onLeftBoundaryCells.empty()  )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMax, vMin);

                        vMin = vMax;

                        iter++;

                        continue;
                    }

                    //CELULAS IGUAIS
#if USE_EXPERIMENTAL_3
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
#else
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vAux, vMin) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

#if USE_EXPERIMENTAL_3
                        if(static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1) )
#else
                        if(static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(1) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1) )
#endif //#if USE_EXPERIMENTAL_3
                        {
                            iter2++;
                        }

                        iter++;
                        continue;
                    }
                    else
                    {
                        //CELULA DO LADO EXTERNO MENOR
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMax, vMin, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1) + Data::getTolerance() + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vAux, vMin) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

                        iter2++;
                        continue;
                    }
                }

                //CRIA AS ARESTAS FINAIS
                if( vMin->getPoint()->getCoord(1) + Data::getTolerance() < box->getMax()->getCoord(1) )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMax()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, element_front, e_ori, vMin);

                    if(vFinal == NULL)
                        vFinal = findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMax()->getCoord(1));

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vFinal,vMin, this->idManager->next(1) );

                    removeDanglingEdges(box, sub_front, e);     //TESTA SE TEM ARESTAS "SOLTAS"

                    //includeInModel(box, sub_front, element_front, rejected, e, vMin, vFinal);
                    includeInModel(box, sub_front, element_front, rejected, e, vFinal, vMin);

                    delete e_ori;
                    delete v;
                }

            }
            break;

            //BASE
            case 3:
            {
std::cout << "BASE" << std::endl;
                if( baseBoundaryCells.empty() ) continue;

                for (iter = baseBoundaryCells.begin(); iter != baseBoundaryCells.end(); )
                {
                    if( box->on( static_cast<Data::BoxTreeCell *>(*iter)->box() ))
                    {
                        onBaseBoundaryCells.push_back( (*iter) );

                        iter = baseBoundaryCells.erase( iter );
                    }
                    else
					{
						iter++;
					}
                }

                onBaseBoundaryCells.sort( compareBoxX );
                baseBoundaryCells.sort( compareBoxX );

                iter = baseBoundaryCells.begin();
                iter2 = onBaseBoundaryCells.begin();

                while( ( (iter2 != onBaseBoundaryCells.end()) ) &&
#if USE_EXPERIMENTAL_3
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0)  + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0)  + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
#else
                     ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) <= box->getMin()->getCoord(0)  + Data::getTolerance() ||
                       static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0)  + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
#endif //#if USE_EXPERIMENTAL_3
                {
                        iter2++;
                }

                MJMesh::Vertex *vMin = NULL;
                if ( baseBoundaryCells.empty() )
				{
                    //vMin = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
#if USE_EXPERIMENTAL_3
                    if( iter2 != onBaseBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) < box->getMax()->getCoord(0) )
#else
                    if( iter2 != onBaseBoundaryCells.end() && static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) < box->getMax()->getCoord(0) )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    }
                    else
                    {
                        Data::Point2D p1 = Data::Point2D(box->getMin()->getCoord(0), box->getMin()->getCoord(1));
                        Data::Point2D p2 = Data::Point2D(box->getMax()->getCoord(0), box->getMin()->getCoord(1));

                        Data::Edge2D *box_edge = new Data::Edge2D( &p1, &p2);

                        vMin = bestVertex( sub_front, box_edge);
                    }
				}
                else
                {
                    if( iter2 == onBaseBoundaryCells.end() )
#if USE_EXPERIMENTAL_3
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                        vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                    else
                    {
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) )
#endif //#if USE_EXPERIMENTAL_3
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), this->idManager->next(0));
                        }
                        else
                        {
#if USE_EXPERIMENTAL_3
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                            vMin = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                            //vMin = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->min().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max().getCoord(1), this->idManager->next(0));
                        }
                    }

				}


                //CRIA AS ARESTAS INICIAIS
                if( vMin->getPoint()->getCoord(0) > box->getMin()->getCoord(0) + Data::getTolerance()  )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMin()->getCoord(0), box->getMin()->getCoord(1));
					MJMesh::Vertex *v = new MJMesh::Vertex( box->getMin()->getCoord(0), box->getMin()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::FrontElement2D * e0 = InitialEdge(box, element_front, sub_front, rejected, vMin, e_ori, false);

                    if(e0 != NULL )
                        removeDanglingEdges(box, sub_front, e0);    //TESTA SE TEM ARESTAS "SOLTAS"

                    delete e_ori;
                    delete v;
                }

                //CRIA AS ARESTAS INTERNAS
                //while( iter!=baseBoundaryCells.end() && iter2!=onBaseBoundaryCells.end() )
                while( iter!=baseBoundaryCells.end() || iter2!=onBaseBoundaryCells.end() )
                {
                    while( ( (iter2 != onBaseBoundaryCells.end()) ) &&
#if USE_EXPERIMENTAL_3
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) <= box->getMin()->getCoord(0)  + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0)  + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
#else
                         ( static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) <= box->getMin()->getCoord(0)  + Data::getTolerance() ||
                           static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0)  + Data::getTolerance() >= box->getMax()->getCoord(0) ) )
#endif //#if USE_EXPERIMENTAL_3
                    {
                            iter2++;
                    }

                    if( iter == baseBoundaryCells.end()  && iter2 == onBaseBoundaryCells.end() )
                        break;

                    if( (iter == baseBoundaryCells.end() && iter2!=onBaseBoundaryCells.end()) || baseBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMin, vMax);

                        vMin = vMax;

                        iter2++;

                        continue;
                    }

                    if( (iter2 == onBaseBoundaryCells.end() && iter!=baseBoundaryCells.end()) || onBaseBoundaryCells.empty() )
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        includeInModel(box, sub_front, element_front, rejected, e, vMin, vMax);

                        vMin = vMax;

                        iter++;

                        continue;
                    }

                    //CELULAS IGUAIS FICA COM A INTERNA
#if USE_EXPERIMENTAL_3
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
#else
                    if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
                    {
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter))->min().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vMin, vAux) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max2D().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0) )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->max().getCoord(0) + Data::getTolerance() >= static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0) )
#endif //#if USE_EXPERIMENTAL_3
                        {
                            iter2++;
                        }

                        iter++;

                    }
                    else
                    {
                        //CELULA DO LADO EXTERNO MENOR
#if USE_EXPERIMENTAL_3
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                        MJMesh::Vertex *vMax = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        //MJMesh::Vertex *vMax = new MJMesh::Vertex( ((Data::BoxTreeCell *)(*iter2))->max().getCoord(0), ((Data::BoxTreeCell *)(*iter2))->max().getCoord(1), this->idManager->next(0));

                        MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vMax, this->idManager->next(1) );

                        // vAux é usado somente em caso de buraco na partição
                        MJMesh::Vertex *vAux = NULL;
#if USE_EXPERIMENTAL_3
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
#else
                        if( static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0) <= static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0) + Data::getTolerance() + Data::getTolerance() )
#endif //#if USE_EXPERIMENTAL_3
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter)->min().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3
                        else
#if USE_EXPERIMENTAL_3
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min2D().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max2D().getCoord(1));
#else
                            vAux = this->findOrCreate(sub_front, static_cast<Data::BoxTreeCell *>(*iter2)->min().getCoord(0), static_cast<Data::BoxTreeCell *>(*iter2)->max().getCoord(1));
#endif //#if USE_EXPERIMENTAL_3

                        if( includeInModel(box, sub_front, element_front, rejected, e, vMin, vAux) == false )
                        {
                            vMin = vAux;
                            continue;
                        }
                        else
                            vMin = vMax;

                        iter2++;
                    }
                }

                //CRIA AS ARESTAS FINAIS
                if( vMin->getPoint()->getCoord(0) + Data::getTolerance() < box->getMax()->getCoord(0) )
                {
                    //MJMesh::Vertex *v = this->findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));
                    MJMesh::Vertex *v = new MJMesh::Vertex( box->getMax()->getCoord(0), box->getMin()->getCoord(1), this->idManager->next(0));
                    MJMesh::FrontElement2D *e_ori = new MJMesh::FrontElement2D( v, vMin );

                    MJMesh::Vertex *vFinal = nearestVertexSubFront(box, sub_front, element_front, e_ori, vMin);
                    if(vFinal == NULL)
                        vFinal = findOrCreate(sub_front, box->getMax()->getCoord(0), box->getMin()->getCoord(1));

                    MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D( vMin, vFinal, this->idManager->next(1) );

                    removeDanglingEdges(box, sub_front, e);     //TESTA SE TEM ARESTAS "SOLTAS"

                    //includeInModel(box, sub_front, element_front, rejected, e, vFinal, vMin);
                    includeInModel(box, sub_front, element_front, rejected, e, vMin, vFinal);

                    delete e_ori;
                    delete v;
                }
            }
            break;


            default:
                std::cout << "Not implemented..." << std::endl;
            break;
        }
    }

//PEGAR AS REJECTED E ELIMINAR AS ARESTAS TOTALMENTE FORA DA PARTIÇÃO

	//sub_front->setCrossing(sub_front->getRejected());
	sub_front->moveRejectedToCrossing();

    sub_front->setRejected(rejected);
//std::cout << "KKKKKKKKKKkkkkkkkkkkkKKKKKKKKKKKKKKKKKKKKKKKkk" << std::endl;

	cleanOutEdges(box, sub_front);

    return;
}

void PMJMesh::APrioriTaskManager::adjustSubfronts()
{
	Data::VertexMap vMap;
	Data::FrontElementMap felMap;

	for (Data::Front::VertexIterator iter = this->front->vBegin();
		 iter != this->front->vEnd(); iter++)
	{
		vMap[(*iter)->getId()] = (*iter);
	}

	for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin(), end = this->front->allEEnd();
		 iter != end; iter++)
	{
		felMap[(*iter)->getId()] = (*iter);
	}

	Parallel::TaskVector tasks = this->comm->isParallel() ? this->done : this->tasks;

	for (Parallel::TaskVector::const_iterator iter = tasks.begin();
		 iter != tasks.end(); iter++)
	{
		PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>((*iter));
		PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(task->getBuilder());
		PMJMesh::Partition *partition = NULL;

		Data::FrontElementList fels;

		if (this->comm->isParallel())
		{
			partition = static_cast<PMJMesh::Partition *>(task->getPartition());

			partition->getSubfront()->moveCrossingToRejected();

			fels = partition->getSubfront()->getAllElements();
		}
		else
		{
			Data::FrontElementList oldFront = drive->getOldFront()->getAllElements();
			Data::FrontElementList commonFront = drive->getCommonFront()->getAllElements();
			Data::FrontElementList newFront = drive->getNewFront()->getAllElements();

			fels.swap(oldFront);
			fels.splice(fels.end(), commonFront);
			fels.splice(fels.end(), newFront);

			const_cast<MJMesh::Front *>(drive->getFront())->clear();
		}

		Data::Mesh *mesh = const_cast<MJMesh::Mesh *>(drive->getMesh());

		Data::VertexSet oldVertices;

		for (Data::FrontElementList::iterator iter = fels.begin();
			 iter != fels.end(); iter++)
		{
			for (UInt i = 0; i < (*iter)->numVertices(); i++)
			{
				oldVertices.insert((*iter)->getVertex(i));
			}
		}

		Data::VertexList vertices(oldVertices.begin(), oldVertices.end());

		//same code from FullFrontUpdater::updateMesh(vertices)
		{
			Data::VertexList deletedVertices;

			while (!vertices.empty())
			{
				MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());

				//std::cout << "debug v = " << v->getId() << std::endl;

				vertices.pop_front();

				Data::VertexMap::iterator it = vMap.find(v->getId());

				if (it != vMap.end())
				{
					//std::cout << "debug vertex existed" << std::endl;

					MJMesh::Vertex *original = static_cast<MJMesh::Vertex *>((*it).second);

					//apriori cases
					if (original == v)
					{
						//std::cout << "debug vertex was the same" << std::endl;

						continue;
					}

					//std::cout << "debug original = " << original->getId() << std::endl;

					//Data::ElementList els = v->getAdjacentElements();

					//std::cout << "debug v->adjacentElements = " << els.size() << std::endl;

					//while (!els.empty())
					for (Data::ElementList::iterator iter = v->eBegin();
                         iter != v->eEnd(); iter = v->erase(iter))
					{
						//MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
						MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

						//els.pop_front();

						//std::cout << "debug adjacent element " << e->getId() << std::endl;

						//v->remove(e);
						original->add(e);

						e->replace(v, original);
					}

					//Data::FrontElementList fels = v->getAdjacentFrontElements();

					//std::cout << "debug v->adjacentFels = " << fels.size() << std::endl;

					//while (!fels.empty())
					for (Data::FrontElementList::iterator iter = v->feBegin();
                         iter != v->feEnd();)
					{
						//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());
						MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

						//fels.pop_front();

						//std::cout << "debug adjacent front element " << fel->getId() << std::endl;

						//if front element already existed
						Data::FrontElementMap::iterator it = felMap.find(fel->getId());

						if ((it != felMap.end()) && ((*it).second != fel))
						{
							//std::cout << "debug adjacent front element will be deleted later" << std::endl;

							iter++;

							continue;
						}

						//v->remove(fel);
						original->add(fel);

						fel->replace(v, original);

						iter = v->erase(iter);
					}

					deletedVertices.push_back(v);

					if (mesh)
					{
						mesh->remove(v);
						mesh->add(original);
					}
					else
					{
						partition->getSubfront()->remove(v);
						partition->getSubfront()->add(original);
					}

					/*if (!this->comm->isParallel())
					{
						if (drive->getOldFront()->have(v))
						{
							drive->getOldFront()->remove(v);
							drive->getOldFront()->add(v);
						}

						if (drive->getCommonFront()->have(v))
						{
							drive->getCommonFront()->remove(v);
							drive->getCommonFront()->add(v);
						}

						if (drive->getNewFront()->have(v))
						{
							drive->getNewFront()->remove(v);
							drive->getNewFront()->add(v);
						}
					}*/
				}
			}

			vertices.swap(deletedVertices);
		}

		//same code from FullFrontUpdater::updateMesh(frontelements)
		{
			Data::FrontElementList deletedFels;

			while (!fels.empty())
			{
				MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

				//std::cout << "debug fel = " << fel->getId() << std::endl;

				fels.pop_front();

				Data::FrontElementMap::iterator it = felMap.find(fel->getId());

				if (it != felMap.end())
				{
					//std::cout << "debug fel existed" << std::endl;

					MJMesh::FrontElement *original = static_cast<MJMesh::FrontElement *>((*it).second);

					//apriori cases
					if (original == fel)
					{
						//std::cout << "debug fel was the same" << std::endl;

						continue;
					}

					//std::cout << "debug original = " << original->getId() << std::endl;

					//Data::ElementList els = fel->getAdjacency();

					//std::cout << "debug fel->adjacentElements = " << els.size() << std::endl;

					//while (!els.empty())
					for (Data::ElementList::iterator iter = fel->eBegin();
                         iter != fel->eEnd(); iter = fel->erase(iter))
					{
						//MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
						MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

						//std::cout << "debug adjacent element " << e->getId() << std::endl;

						//els.pop_front();

						//fel->remove(e);
						original->add(e);
					}

					deletedFels.push_back(fel);

					if (mesh)
					{
						mesh->remove(fel);
						mesh->add(original);

						/*if (!this->comm->isParallel())
						{
							if (drive->getOldFront()->have(fel))
							{
								UInt i = 0;

								for (i = 0; i < 4; i++)
								{
									if (drive->getOldFront()->have(fel, i))
									{
										break;
									}
								}

								drive->getOldFront()->remove(fel);

								if (i == 0)
								{
									drive->getOldFront()->addBoundary(fel);
								}
								else if (i == 1)
								{
									drive->getOldFront()->addElement(fel);
								}
								else if (i == 2)
								{
									drive->getOldFront()->addRejected(fel);
								}
								else if (i == 3)
								{
									drive->getOldFront()->addCrossing(fel);
								}
							}

							if (drive->getCommonFront()->have(fel))
							{
								UInt i = 0;

								for (i = 0; i < 4; i++)
								{
									if (drive->getCommonFront()->have(fel, i))
									{
										break;
									}
								}

								drive->getCommonFront()->remove(fel);

								if (i == 0)
								{
									drive->getCommonFront()->addBoundary(fel);
								}
								else if (i == 1)
								{
									drive->getCommonFront()->addElement(fel);
								}
								else if (i == 2)
								{
									drive->getCommonFront()->addRejected(fel);
								}
								else if (i == 3)
								{
									drive->getCommonFront()->addCrossing(fel);
								}
							}

							if (drive->getNewFront()->have(fel))
							{
								UInt i = 0;

								for (i = 0; i < 4; i++)
								{
									if (drive->getNewFront()->have(fel, i))
									{
										break;
									}
								}

								drive->getNewFront()->remove(fel);

								if (i == 0)
								{
									drive->getNewFront()->addBoundary(fel);
								}
								else if (i == 1)
								{
									drive->getNewFront()->addElement(fel);
								}
								else if (i == 2)
								{
									drive->getNewFront()->addRejected(fel);
								}
								else if (i == 3)
								{
									drive->getNewFront()->addCrossing(fel);
								}
							}
						}*/

						/*Data::FrontElementList::iterator felIter = std::find(newFront.begin(), newFront.end(), fel);

						if (felIter != newFront.end())
						{
							newFront.erase(felIter);
							newFront.push_back(original);

							continue;
						}

						felIter = std::find(commonFront.begin(), commonFront.end(), fel);

						if (felIter != commonFront.end())
						{
							commonFront.erase(felIter);
							commonFront.push_back(original);

							continue;
						}

						felIter = std::find(oldFront.begin(), oldFront.end(), fel);

						if (felIter != oldFront.end())
						{
							oldFront.erase(felIter);
							oldFront.push_back(original);
						}*/
					}
					else
					{
						partition->getSubfront()->remove(fel);
						partition->getSubfront()->add(original);
					}
				}
			}

			fels.swap(deletedFels);
		}

		if (mesh)
		{
			drive->getOldFront()->clear();
			drive->getCommonFront()->clear();
			drive->getNewFront()->clear();

			/*delete drive->getOldFront();
			delete drive->getCommonFront();
			delete drive->getNewFront();

			drive->setOldFront(NULL);
			drive->setCommonFront(NULL);
			drive->setNewFront(NULL);*/
		}

		while (!fels.empty())
		{
			this->mesh->remove(fels.front());

			delete fels.front();

			fels.pop_front();
		}

		while (!vertices.empty())
		{
			this->mesh->remove(vertices.front());

			delete vertices.front();

			vertices.pop_front();
		}
	}
}

void PMJMesh::APrioriTaskManager::execute()
{
    if (this->comm->isSlave())
	{
		this->initializeSlave();

		if (!this->receiveTasksFromMaster())
		{
			this->status = GlobalTaskManager::workersFinalized;

			return;
		}

		//std::cout << "debug slave " << this->comm->rank() << " will now generate mesh" << std::endl;

		this->generateMeshInCells();

		//std::cout << "debug slave " << this->comm->rank() << " will now send results for master" << std::endl;

		this->sendTasksForMaster(false);

		this->clearBoundaries();

		this->taskSent = true;

		if (this->updateType == PMJMesh::FULL)
		{
			this->comm->finalize();
		}
		else
		{
			this->splitMesh();
		}

		this->status = GlobalTaskManager::success;

		return;
	}

    switch (this->status.getType())
	{
	case GlobalTaskManager::NOT_INIT:
		//std::cout << "debug master or single will initialize" << std::endl;

		this->initializeMaster();

		if (this->usingSearchTrees)
		{
			MJMesh::GeometryFrontAdvancer *geometryAdvancer =
				new MJMesh::GeometryFrontAdvancer(this->dimension, this->boundary, this->tree);

			this->front->setVSTree(geometryAdvancer->makeVertexSearchTree(this->front));

			this->front->setFESTree(geometryAdvancer->makeFrontElementSearchTree(this->front));

			delete geometryAdvancer;
		}

		this->status = GlobalTaskManager::init;

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case GlobalTaskManager::INIT:

		{
			//this->mesh->test(this->boundary, this->front);
int ver = 1;
#if USE_OPENMP
            #pragma omp parallel for if(static_cast<PMJMesh::Communicator *>(this->comm)->isSharedParallelismEnabled())
#endif //#if USE_OPENMP
            //for(UInt pos = 0; pos < 1; pos++)
            for(UInt pos = 0; pos < this->partitions.size(); pos++)
            {
std::cout << "VER: " << ver << std::endl;
ver++;
				if ( static_cast<PMJMesh::Partition*>(this->partitions[pos])->getSubfront() == NULL )
				{
					PMJMesh::Front *subfront = new PMJMesh::Front();

					static_cast<PMJMesh::Partition*>(this->partitions[pos])->setSubfront(subfront);
				}

				buildSubDomain(static_cast<PMJMesh::Partition*>(this->partitions[pos])->getBox(),
                                static_cast<PMJMesh::Partition*>(this->partitions[pos])->getSubfront() );
            }

			this->balanceLoad(!this->comm->isParallel());

			this->tasks = this->loadBalancer->getTasks();
		}

		this->status = GlobalTaskManager::loadBalanced;
//return;

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case GlobalTaskManager::LOAD_BALANCED:

		if (this->tasks.size() > 1)
		{
			if (this->comm->isParallel())
			{
				this->sendTasksForSlaves();

				this->receiveTasksFromSlaves();
			}
			else
			{
				this->tasks.clear();

				this->generateMeshInCells();
			}

			this->adjustSubfronts();

			//this->mesh->test(this->boundary, this->front);

			this->updateFront(!this->comm->isParallel(), false);

			//this->mesh->test(this->boundary, this->front);

			this->front->clear();

			this->clearDone();
		}

		if (this->updateType == PMJMesh::FULL)
		{
			this->comm->finalize();
		}

		this->executeGeometryGeneration();

		this->status = GlobalTaskManager::success;

	}
}

bool PMJMesh::APrioriTaskManager::setTaskParameters(Parallel::Task *t,
	MJMesh::Boundary *boundary, Data::BoxTree *tree,
	Performer::IdManager *idManager) const
{
	bool set = GlobalTaskManager::setTaskParameters(t, boundary, tree, idManager);

	if (set)
	{
		PMJMesh::WorkerMainDrive *drive = dynamic_cast<PMJMesh::WorkerMainDrive *>(t->getBuilder());

		drive->setTestingBox(false);
		drive->setClassifyTreeCells(false);
	}

	return set;
}

LoadCalculator *PMJMesh::APrioriTaskManager::makeLoadCalculator(bool /*initializing*/) const
{
	return new LoadCalculator(this->tree,
		this->considerOnCellsInLoad, this->heightDependentInLoad, this->dimension, this->minRefinementLevel);
}

#if USE_C__11
TreePartitioner *PMJMesh::APrioriTaskManager::makeTreePartitioner(const Data::RealHashMap *load, PMJMesh::LoadCalculator *loadCalculator) const
#else
TreePartitioner *PMJMesh::APrioriTaskManager::makeTreePartitioner(const Data::RealMap *load, PMJMesh::LoadCalculator *loadCalculator) const
#endif //#if USE_C__11
{
	switch (this->partitionerType)
	{
	case PMJMesh::BOX_LOAD:
		return new AverageLoadTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(), loadCalculator,
			this->usePartitioningThreshold, this->partitioningThreshold);

	case PMJMesh::BOX_NUMBER:
		return new MinNumberTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(), loadCalculator,
			this->usePartitioningThreshold, static_cast<UInt>(this->partitioningThreshold));

	case PMJMesh::BSP_NUMBER:
		return new MinNumberBSPTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(), loadCalculator,
			this->usePartitioningThreshold, static_cast<UInt>(this->partitioningThreshold));

	case PMJMesh::SMART_BSP_NUMBER:
		return new SmartMinNumberBSPTreePartitioner(this->dimension, this->tree, load,
			this->skippingInnerInDecomposition, this->comm->numWorkers(),
			loadCalculator, this->usePartitioningThreshold, static_cast<UInt>(this->partitioningThreshold));
	}

	return NULL;
}

FrontPartitioner *PMJMesh::APrioriTaskManager::makeFrontPartitioner() const
{
	return new PMJMesh::FrontPartitioner(this->dimension, this->front, this->tasks, 0);
}

/*#if USE_NEW_FRONT_UPDATER
MeshUpdater *PMJMesh::APrioriTaskManager::makeMeshUpdater() const
{
if (this->comm->isMaster())
	{
		switch (this->updateType)
		{
		case PMJMesh::FULL:
			return new MeshUpdater(this->dimension, this->mesh, this->comm, false);

		case PMJMesh::PARTIAL:
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, false);
			break;

		case PMJMesh::PARTIAL_NO_BOUNDARY:
			return NULL;
			break;

		case PMJMesh::EMPTY:
			return NULL;
			break;

		case PMJMesh::EMPTY_NO_BOUNDARY:
			return NULL;
			break;

		case PMJMesh::FINAL:
			return NULL;
			break;
		}
	}
	else
	{
		switch (this->updateType)
		{
		case PMJMesh::FULL:
			return NULL;

		case PMJMesh::PARTIAL:
			return new EmptyMeshUpdater(this->front, this->smoothingLayer, this->boundary, this->idManager,
				this->dimension, this->mesh, this->comm, false, false);
			break;

		case PMJMesh::PARTIAL_NO_BOUNDARY:
			return NULL;
			break;

		case PMJMesh::EMPTY:
			return NULL;
			break;

		case PMJMesh::EMPTY_NO_BOUNDARY:
			return NULL;
			break;

		case PMJMesh::FINAL:
			return NULL;
			break;
		}
	}

	return NULL;
}
#endif //#if USE_NEW_FRONT_UPDATER*/
