#include "MJMesh/Mesh.h"

#include "Data/OStream.h"
#include "Data/Box.h"
#include "Data/Element.h"
#include "Data/GeometricShape.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"

#if USE_DELETE_FELS
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElementSurface.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Element3D.h"
#endif //#if USE_DELETE_FELS

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

#define PRINT_SPLIT_TO_DEBUG false

//debug
//#include <typeinfo>
//endebug

using namespace MJMesh;

MJMesh::Mesh::Mesh(ULInt id) :
	Data::Mesh(id)
{

}

MJMesh::Mesh::~Mesh()
{
	/*std::stringstream str;

	str << "deleting mesh" << Data::endl;
	str << "elements.size() = " << this->elements.size() << Data::endl;
	str << "frontElements.size() = " << this->frontElements.size() << Data::endl;
	str << "vertices.size() = " << this->vertices.size() << Data::endl;
	str << "trace() = " << Data::trace() << Data::endl;

	Data::cout() << str.str();*/

	/*while (!this->frontElements.empty())
	{
		MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(this->frontElements.front());
		this->frontElements.pop_front();

		if (e)
		{
			e->removeMeshData(this);

			delete e;
		}
	}

	while (!this->elements.empty())
	{
		MJMesh::Element *e = static_cast<MJMesh::Element *>(this->elements.front());
		this->elements.pop_front();

		if (e)
		{
			e->removeMeshData(this);

			delete e;
		}
	}

	while (!this->vertices.empty())
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(this->vertices.front());
		this->vertices.pop_front();

		if (v)
		{
			v->removeMeshData(this);

			delete v;
		}
	}*/
}

void MJMesh::Mesh::setVertices(const Data::VertexList &vertices)
{
	this->clearVertices();

	this->vertices.clear();

	for (Data::VertexList::const_iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		//static_cast<MJMesh::Vertex *>((*iter))->addMeshData(this, this->vertices.insert((*iter)).first);
		this->vertices.push_back((*iter));

		static_cast<MJMesh::Vertex *>((*iter))->addMeshData(this, --this->vertices.end());
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void MJMesh::Mesh::setFrontElements(const Data::FrontElementList &frontElements)
{
	this->clearFrontElements();

	this->frontElements.clear();

	for (Data::FrontElementList::const_iterator iter = frontElements.begin();
		 iter != frontElements.end(); iter++)
	{
		//static_cast<MJMesh::FrontElement *>((*iter))->addMeshData(this, this->frontElements.insert((*iter)).first);
		this->frontElements.push_back((*iter));

		static_cast<MJMesh::FrontElement *>((*iter))->addMeshData(this, --this->frontElements.end());
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void MJMesh::Mesh::setElements(const Data::ElementList &elements)
{
	this->clearElements();

	this->elements.clear();

	for (Data::ElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		//static_cast<MJMesh::Element *>((*iter))->addMeshData(this, this->elements.insert((*iter)).first);
		this->elements.push_back((*iter));

		static_cast<MJMesh::Element *>((*iter))->addMeshData(this, --this->elements.end());
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

#if (!USE_EXPERIMENTAL_MESH)
void MJMesh::Mesh::setInnerVertices(const Data::VertexList &innerVertices)
{
	for (Data::Mesh::VertexIterator iter = this->innerVertices.begin();
		 iter != this->innerVertices.end(); iter++)
	{
		static_cast<MJMesh::Vertex *>((*iter))->setMeshInner(this, false, Data::Mesh::VertexIterator());
	}

	this->innerVertices.clear();

	for (Data::VertexList::const_iterator iter = innerVertices.begin();
		 iter != innerVertices.end(); iter++)
	{
		//static_cast<MJMesh::Vertex *>((*iter))->setMeshInner(this, true, this->innerVertices.insert((*iter)).first);
		this->innerVertices.push_back((*iter));
		static_cast<MJMesh::Vertex *>((*iter))->setMeshInner(this, true, --this->innerVertices.end());
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

bool MJMesh::Mesh::isSurface() const
{
    if (this->dimension() != 3)
    {
        return false;
    }

    if (this->size() != 0)
    {
        Data::Element *e = *this->eBegin();

        if (e->numVertices() == 3)
        {
            return true;
        }

#if USE_BRICKS
        if (e->numVertices() > 4)
        {
            return false;
        }
#endif //#if USE_BRICKS

#if (!USE_QUADS)
        return false;
#endif //#if (!USE_QUADS)

        /*///geometric test
        Data::Point3D *p1 = static_cast<Data::Point3D *>(e->getVertex(0)->getPoint());
        Data::Point3D *p2 = static_cast<Data::Point3D *>(e->getVertex(1)->getPoint());
        Data::Point3D *p3 = static_cast<Data::Point3D *>(e->getVertex(2)->getPoint());
        Data::Point3D *p4 = static_cast<Data::Point3D *>(e->getVertex(3)->getPoint());

        Data::Vector3D v1(p1, p2);
        Data::Vector3D v2(p1, p3);
        Data::Vector3D v3(p1, p4);

        Data::Vector3D n = v1.cross(v2);

        Real dot = n.dot(v3);

        return (std::fabs(dot) < Data::getTolerance());*/

        return dynamic_cast<MJMesh::ElementSurface *>(e);
    }

    if (this->frontElementsSize() != 0)
    {
        Data::FrontElement *fel = *this->feBegin();

        return (fel->numVertices() == 2);

        //return dynamic_cast<MJMesh::FrontElementSurface *>(fel);
    }

    return false;
}

void MJMesh::Mesh::clear()
{
	this->clearVertices();
	this->clearFrontElements();
	this->clearElements();

	Data::Mesh::clear();
}

void MJMesh::Mesh::add(Data::Vertex *v, bool inner)
{
	if (this->have(v))
	{
		return;
	}

	//static_cast<MJMesh::Vertex *>(v)->addMeshData(this, this->vertices.insert(v).first, inner, Data::VertexSet::iterator());

	this->vertices.push_back(v);

#if USE_EXPERIMENTAL_MESH
	static_cast<MJMesh::Vertex *>(v)->addMeshData(this, --this->vertices.end(), inner);
#else
	static_cast<MJMesh::Vertex *>(v)->addMeshData(this, --this->vertices.end(), inner, Data::Mesh::VertexIterator());
#endif //#if USE_EXPERIMENTAL_MESH

	if (inner)
	{
#if USE_EXPERIMENTAL_MESH
		++this->numInnerVertices;
#else
		this->addInner(v);
#endif //#if USE_EXPERIMENTAL_MESH
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void MJMesh::Mesh::addInner(Data::Vertex *v)
{
#if USE_EXPERIMENTAL_MESH
    ++this->numInnerVertices;

    static_cast<MJMesh::Vertex *>(v)->setMeshInner(this, true);
#else
	//static_cast<MJMesh::Vertex *>(v)->setMeshInner(this, true, this->innerVertices.insert(v).first);
	this->innerVertices.push_back(v);

	static_cast<MJMesh::Vertex *>(v)->setMeshInner(this, true, --this->innerVertices.end());
#endif //#if USE_EXPERIMENTAL_MESH
}

void MJMesh::Mesh::add(Data::FrontElement *e)
{
    /*if ((e->getId() == 1411) || (e->getId() == 1367))
    {
        std::cout << "adding fel " << e->text() << " at " << e << " to mesh; backtrace:" << std::endl;
        std::cout << Data::trace() << std::endl;
    }*/

    /*if (e->getId() == 194111)
    {
        std::cout << "adding fel " << e->text() << " at " << e << " to mesh at " << this << "; backtrace:" << std::endl;
    }*/

	if (this->have(e))
	{
	    /*if (e->getId() == 194111)
        {
            std::cout << "but not adding fel " << e->text() << " at " << e << " to mesh at " << this << "; backtrace:" << std::endl;
        }*/

		return;
	}

	//static_cast<MJMesh::FrontElement *>(e)->addMeshData(this, this->frontElements.insert(e).first);
	this->frontElements.push_back(e);

	static_cast<MJMesh::FrontElement *>(e)->addMeshData(this, --this->frontElements.end());

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void MJMesh::Mesh::add(Data::Element *e)
{
	if (this->have(e))
	{
		return;
	}

	//static_cast<MJMesh::Element *>(e)->addMeshData(this, this->elements.insert(e).first);
	this->elements.push_back(e);

	static_cast<MJMesh::Element *>(e)->addMeshData(this, --this->elements.end());

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

bool MJMesh::Mesh::remove(Data::Vertex *v)
{
	if (!this->have(v))
	{
		return false;
	}

	this->forceRemove(v);

	return true;
}

void MJMesh::Mesh::removeInner(Data::Vertex *v)
{
	this->forceRemoveInner(v);
}

bool MJMesh::Mesh::have(const Data::Vertex *v) const
{
	return static_cast<const MJMesh::Vertex *>(v)->isInMesh(this);
}

bool MJMesh::Mesh::haveInner(const Data::Vertex *v) const
{
	return static_cast<const MJMesh::Vertex *>(v)->isMeshInner(this);
}

bool MJMesh::Mesh::have(const Data::FrontElement *e) const
{
	return static_cast<const MJMesh::FrontElement *>(e)->isInMesh(this);
}

bool MJMesh::Mesh::have(const Data::Element *e) const
{
	return static_cast<const MJMesh::Element *>(e)->isInMesh(this);
}

Data::Mesh::VertexIterator MJMesh::Mesh::erase(const Data::Mesh::VertexIterator &iter)
{
	VertexSpecificData::MeshSpecificData msp = static_cast<MJMesh::Vertex *>((*iter))->getAndRemoveMeshData(this);

	if (msp.inner)
	{
#if USE_EXPERIMENTAL_MESH
		--this->numInnerVertices;
#else
		this->innerVertices.erase(msp.innerMeshIt);
#endif //#endif //#if USE_EXPERIMENTAL_MESH
	}

#if USE_GUI
	Data::Mesh::VertexIterator iter2 = this->vertices.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->vertices.erase(iter);
#endif //#if USE_GUI

	//VertexIterator iter2 = iter;
	//iter2++;

	//this->vertices.erase(iter);
//#if USE_GUI
//	this->outdate();
//#endif //#if USE_GUI
	//return iter2;
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::InnerVertexIterator MJMesh::Mesh::eraseInner(const Data::Mesh::InnerVertexIterator &iter)
#else
Data::Mesh::VertexIterator MJMesh::Mesh::eraseInner(const Data::Mesh::VertexIterator &iter)
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	Data::Mesh::InnerVertexIterator next = iter;

	++next;

	static_cast<MJMesh::Vertex *>((*iter))->setMeshInner(this, false);

	--this->numInnerVertices;

	return next;
#else
	static_cast<MJMesh::Vertex *>((*iter))->setMeshInner(this, false, Data::Mesh::VertexIterator());

	return this->innerVertices.erase(iter);
#endif //#if USE_EXPERIMENTAL_MESH

	//VertexIterator iter2 = iter;
	//iter2++;

	//this->innerVertices.erase(iter);

	//return iter2;
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::InnerVertexIterator MJMesh::Mesh::eraseFromInner(const Data::Mesh::InnerVertexIterator &iter)
#else
Data::Mesh::VertexIterator MJMesh::Mesh::eraseFromInner(const Data::Mesh::VertexIterator &iter)
#endif //#if USE_EXPERIMENTAL_MESH
{
	VertexSpecificData::MeshSpecificData msp = static_cast<MJMesh::Vertex *>((*iter))->getAndRemoveMeshData(this);

#if USE_EXPERIMENTAL_MESH
    --this->numInnerVertices;

	return Data::Mesh::InnerVertexIterator(this, this->vertices.erase(msp.meshIt));
#else
	this->vertices.erase(msp.meshIt);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	return this->innerVertices.erase(iter);
#endif //#if USE_EXPERIMENTAL_MESH

	//VertexIterator iter2 = iter;
	//iter2++;

	//this->innerVertices.erase(iter);

	//return iter2;
}

Data::Mesh::FrontElementIterator MJMesh::Mesh::erase(const Data::Mesh::FrontElementIterator &iter)
{
	static_cast<MJMesh::FrontElement *>((*iter))->removeMeshData(this);

#if USE_GUI
	Data::Mesh::FrontElementIterator iter2 = this->frontElements.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->frontElements.erase(iter);
#endif //#endif //#if USE_GUI

	//FrontElementIterator iter2 = iter;
	//iter2++;

	//this->frontElements.erase(iter);
//#if USE_GUI
//	this->outdate();
//#endif //#if USE_GUI
	//return iter2;
}

Data::Mesh::ElementIterator MJMesh::Mesh::erase(const Data::Mesh::ElementIterator &iter)
{
	static_cast<MJMesh::Element *>((*iter))->removeMeshData(this);

#if USE_GUI
	Data::Mesh::ElementIterator iter2 = this->elements.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->elements.erase(iter);
#endif //#endif //#if USE_GUI

	//ElementIterator iter2 = iter;
	//iter2++;

	//this->elements.erase(iter);
//#if USE_GUI
//	this->outdate();
//#endif //#if USE_GUI
	//return iter2;
}

void MJMesh::Mesh::swap(MJMesh::Mesh *mesh)
{
	for (Data::Mesh::VertexIterator iter = this->vBegin();
		 iter != this->vEnd(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		if (v->isInMesh(mesh))
		{
			v->swapMeshData(this, mesh);
		}
		else
		{
			v->replaceMeshData(this, mesh);
		}
	}

	for (Data::Mesh::VertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		if (!v->isInMesh(this))
		{
			v->replaceMeshData(mesh, this);
		}
	}

	for (Data::Mesh::FrontElementIterator iter = this->feBegin();
		 iter != this->feEnd(); iter++)
	{
		MJMesh::FrontElement *fe = static_cast<MJMesh::FrontElement *>((*iter));

		if (fe->isInMesh(mesh))
		{
			fe->swapMeshData(this, mesh);
		}
		else
		{
			fe->replaceMeshData(this, mesh);
		}
	}

	for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter++)
	{
		MJMesh::FrontElement *fe = static_cast<MJMesh::FrontElement *>((*iter));

		if (!fe->isInMesh(this))
		{
			fe->replaceMeshData(mesh, this);
		}
	}

	for (Data::Mesh::ElementIterator iter = this->eBegin();
		 iter != this->eEnd(); iter++)
	{
		MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

		if (e->isInMesh(mesh))
		{
			e->swapMeshData(this, mesh);
		}
		else
		{
			e->replaceMeshData(this, mesh);
		}
	}

	for (Data::Mesh::ElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter++)
	{
		MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

		if (!e->isInMesh(this))
		{
			e->replaceMeshData(mesh, this);
		}
	}

	this->vertices.swap(mesh->vertices);
#if (!USE_EXPERIMENTAL_MESH)
	this->innerVertices.swap(mesh->innerVertices);
#endif //#if (!USE_EXPERIMENTAL_MESH)
	this->frontElements.swap(mesh->frontElements);
	this->elements.swap(mesh->elements);
}

Data::ElementList MJMesh::Mesh::adjacency(const Data::Vertex *v) const
{
	//return this->adjacency(static_cast<const MJMesh::Vertex *>(v));
	return static_cast<const MJMesh::Vertex *>(v)->getAdjacentElements();
}

/*Data::ElementList MJMesh::Mesh::adjacency(const MJMesh::Vertex *v) const
{
	return v->getAdjacentElements();
}*/

Data::ElementList MJMesh::Mesh::adjacency(const Data::FrontElement *e) const
{
	//return this->adjacency(static_cast<const MJMesh::FrontElement *>(e));
	return static_cast<const MJMesh::FrontElement *>(e)->getAdjacency();
}

/*Data::ElementList MJMesh::Mesh::adjacency(const MJMesh::FrontElement *e) const
{
	return e->getAdjacency();
}*/

Data::FrontElementList MJMesh::Mesh::findAdjacentFrontElements(const Data::Vertex *v) const
{
	//debug
	//Data::cout() << "type(v) = " << typeid(v).name() << "\ntypename(*v) = " << typeid(*v).name() << Data::endl;
	//endebug

	return static_cast<const MJMesh::Vertex *>(v)->getAdjacentFrontElements();
}

Data::FrontElementList MJMesh::Mesh::findAdjacentFrontElements(const Data::Element *e) const
{
	/*Data::FrontElementList adjs;
	Data::FrontElementSet frontElements;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		Data::FrontElementList adjFels = static_cast<MJMesh::Vertex *>(e->getVertex(i))->getAdjacentFrontElements();

		frontElements.insert(adjFels.begin(), adjFels.end());
	}

	for (Data::FrontElementSet::const_iterator iter = frontElements.begin();
		 iter != frontElements.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		if (e->have(fel))
		{
			adjs.push_back(fel);
		}
	}

	return adjs;*/

	/*Data::FrontElementList frontElements;
	Data::FrontElementSet adjs;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		frontElements.splice(frontElements.end(), this->findAdjacentFrontElements(e->getVertex(i)));
	}

	for (Data::FrontElementList::const_iterator iter = frontElements.begin();
		 iter != frontElements.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		if (e->have(fel))
		{
			adjs.insert(fel);
		}
	}

	return Data::FrontElementList(adjs.begin(), adjs.end());*/

	Data::FrontElementSet adjs;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		//for (Data::FrontElementList::const_iterator iter = static_cast<MJMesh::Vertex *>(e->getVertex(i))->getAdjacentFrontElements().begin(),
		for (Data::FrontElementList::const_iterator iter = static_cast<MJMesh::Vertex *>(e->getVertex(i))->feBegin(),
			 //end = static_cast<MJMesh::Vertex *>(e->getVertex(i))->getAdjacentFrontElements().end();
			 end = static_cast<MJMesh::Vertex *>(e->getVertex(i))->feEnd();
			 iter != end; ++iter)
		{
			Data::FrontElement *fel = (*iter);

			if (e->have(fel))
			{
				adjs.insert(fel);
			}
		}
	}

	return Data::FrontElementList(adjs.begin(), adjs.end());
}

#if USE_DELETE_FELS
UInt MJMesh::Mesh::createAdjacentFrontElements(Data::Vertex *v,
    Data::FrontElementList &created, bool add, const Data::Boundary *boundary)
{
    UInt count = 0;

    UInt dim = v->getPoint()->dimension();

    if (dim == 2)
    {
        for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
        {
            MJMesh::Element2D *e = static_cast<MJMesh::Element2D *>((*iter));

            if (!this->have(e))
            {
                continue;
            }

            for (UInt i = 0; i < e->numVertices(); i++)
            {
                MJMesh::Vertex *other = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                if (other == v)
                {
                    continue;
                }

                bool found = false;

                for (Data::FrontElementList::iterator iter2 = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter2 != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter2++)
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter2));

                    if (!fel->have(other))
                    {
                        continue;
                    }

                    if ((boundary->have(fel)) && (fel->numAdjacents() == 1))
                    {
                        if (fel->haveAdjacent(e))
                        {
                            found = true;

                            break;
                        }

                        continue;
                    }

                    if (!fel->haveAdjacent(e))
                    {
                        fel->add(e);
                    }

                    found = true;

                    break;
                }

                if (found)
                {
                    continue;
                }

                MJMesh::FrontElement *fel = new MJMesh::FrontElement2D(static_cast<MJMesh::Vertex *>(v), other);

                fel->add(e);

                if (add)
                {
                    this->add(fel);
                }

                created.push_back(fel);

                count++;
            }
        }
    }
    else
    {
        for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
        {
            MJMesh::Element3D *e = static_cast<MJMesh::Element3D *>((*iter));

            if (!this->have(e))
            {
                continue;
            }

            for (UInt i = 0; i < e->numVertices() - 1; i++)
            {
                MJMesh::Vertex *other1 = static_cast<MJMesh::Vertex *>(e->getVertex(i));

                if (other1 == v)
                {
                    continue;
                }

                for (UInt j = i + 1; j < e->numVertices(); j++)
                {
                    MJMesh::Vertex *other2 = static_cast<MJMesh::Vertex *>(e->getVertex(j));

                    if (other2 == v)
                    {
                        continue;
                    }

                    bool found = false;

                    for (Data::FrontElementList::iterator iter2 = static_cast<MJMesh::Vertex *>(v)->feBegin();
                         iter2 != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter2++)
                    {
                        MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter2));

                        if ((!fel->have(other1)) || (!fel->have(other2)))
                        {
                            continue;
                        }

                        if ((boundary->have(fel)) && (fel->numAdjacents() == 1))
                        {
                            if (fel->haveAdjacent(e))
                            {
                                found = true;

                                break;
                            }

                            continue;
                        }

                        if (!fel->haveAdjacent(e))
                        {
                            fel->add(e);
                        }

                        found = true;

                        break;
                    }

                    if (found)
                    {
                        continue;
                    }

                    MJMesh::FrontElement *fel = new MJMesh::FrontElement3D(static_cast<MJMesh::Vertex *>(v), other1, other2);

                    fel->add(e);

                    if (add)
                    {
                        this->add(fel);
                    }

                    created.push_back(fel);

                    count++;
                }
            }
        }
    }

    /*if (count > 0)
    {
        Data::cout() << "created " << count << " fels for vertex " << v->getId() << Data::endl;
        //Data::cout() << "created " << count << " fels for vertex " << v->getId() << " at\n" << Data::trace() << Data::endl;
    }*/

    return count;
}
#endif //#if USE_DELETE_FELS

bool MJMesh::Mesh::remove(Data::FrontElement *e)
{
	return this->remove(e, true);
}

bool MJMesh::Mesh::remove(Data::FrontElement *e, bool removeFromAdjacentVertices)
{
    /*if (e->getId() == 194111)
    {
        std::cout << "removing fel " << e->text() << " at " << e << " from mesh at " << this << "; backtrace:" << std::endl;
    }*/

	if (!this->have(e))
	{
	    /*if (e->getId() == 194111)
        {
            std::cout << "but not removing fel " << e->text() << " at " << e << " from mesh at " << this << "; backtrace:" << std::endl;
        }*/

		return false;
	}

	this->forceRemove(e);

	if (removeFromAdjacentVertices)
	{
		//Data::VertexVector vertices = e->getVertices();

		//while (!vertices.empty())
		for (UInt i = 0; i < e->numVertices(); i++)
		{
			//MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.back());
			MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));
			//vertices.pop_back();

			v->remove(e);
		}
	}

	return true;
}

bool MJMesh::Mesh::remove(Data::Element *e)
{
	return this->remove(e, true);
}

bool MJMesh::Mesh::remove(Data::Element *e, bool removeFromAdjacentVertices)
{
	if (!this->have(e))
	{
		return false;
	}

	this->forceRemove(e);

	if (removeFromAdjacentVertices)
	{
		for (UInt i = 0; i < e->numVertices(); i++)
		{
			static_cast<MJMesh::Vertex *>(e->getVertex(i))->remove(static_cast<MJMesh::Element *>(e));
		}
	}

	return true;
}

bool sizeSmaller(Data::Element *e1, Data::Element *e2)
{
	return e1->getShape()->size() < e2->getShape()->size();
}

void MJMesh::Mesh::split(const Data::VertexList &vertices, UInt layers, Mesh *mesh/*, bool noIntersection*/)
{
	MJMesh::Boundary sharedBoundary;

	this->split(vertices, layers, mesh, false, sharedBoundary/*, noIntersection*/);
}

void MJMesh::Mesh::split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
	MJMesh::Boundary &sharedBoundary/*, bool noIntersection*/,
	const Data::FrontList &except)
{
	this->split(vertices, layers, mesh, true, sharedBoundary/*, noIntersection*/, except);
}

void MJMesh::Mesh::split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
	const MJMesh::Boundary *boundary/*, bool noIntersection*/)
{
	MJMesh::Boundary sharedBoundary;

	this->split(vertices, layers, mesh, false, sharedBoundary, boundary/*, noIntersection*/);
}

void MJMesh::Mesh::split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary/*,
	bool noIntersection*/, const Data::FrontList &except)
{
	this->split(vertices, layers, mesh, true, sharedBoundary, boundary/*, noIntersection*/, except);
}

bool MJMesh::Mesh::frontHave(const Data::FrontList &except, const Data::Vertex *v) const
{
	for (Data::FrontList::const_iterator iter = except.begin();
		 iter != except.end(); iter++)
	{
		if ((*iter)->have(v))
		{
			return true;
		}
	}

	return false;
}

bool MJMesh::Mesh::frontHave(const Data::FrontList &except, const Data::FrontElement *fel) const
{
	for (Data::FrontList::const_iterator iter = except.begin();
		 iter != except.end(); iter++)
	{
		if ((*iter)->have(fel))
		{
			return true;
		}
	}

	return false;
}

bool MJMesh::Mesh::boundaryHave(const Data::BoundaryList &boundaries, const Data::Vertex *v) const
{
    for (Data::BoundaryList::const_iterator iter = boundaries.begin();
		 iter != boundaries.end(); iter++)
	{
		if ((*iter)->have(v))
		{
			return true;
		}
	}

	return false;
}

bool MJMesh::Mesh::boundaryHave(const Data::BoundaryList &boundaries, const Data::FrontElement *fel) const
{
    for (Data::BoundaryList::const_iterator iter = boundaries.begin();
		 iter != boundaries.end(); iter++)
	{
		if ((*iter)->have(fel))
		{
			return true;
		}
	}

	return false;
}

void MJMesh::Mesh::split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
	bool outputShared, MJMesh::Boundary &sharedBoundary,
	/*bool noIntersection, */const Data::FrontList &except)
{
	/*Data::VertexSet layerVertices;
	Data::FrontElementSet layerFrontElements;
	Data::ElementSet layerElements;

	bool vempty = ((noIntersection) || (mesh->verticesSize() == 0));
	bool felempty = ((noIntersection) || (mesh->frontElementsSize() == 0));
	bool elempty = ((noIntersection) || (mesh->size() == 0));*/

#if USE_ID_HASH_IN_MESH_SPLIT
	VertexIdHash current(vertices.begin(), vertices.end());
	VertexIdHash previous, next;
#elif USE_C__11
	Data::VertexHash current(vertices.begin(), vertices.end());
	Data::VertexHash previous, next;
#else
	Data::VertexSet current(vertices.begin(), vertices.end());
	Data::VertexSet previous, next;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

	for (UInt i = 0; i < layers; i++)
	{
#if USE_ID_HASH_IN_MESH_SPLIT
		for (VertexIdHash::iterator iter = current.begin();
#elif USE_C__11
		for (Data::VertexHash::iterator iter = current.begin();
#else
		for (Data::VertexSet::iterator iter = current.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
			 iter != current.end(); iter++)
		{
			Data::Vertex *v = (*iter);

			/*if ((v->getId() == 184) || (v->getId() == 1423) || (v->getId() == 1018))
			{
				Data::cout() << "debug vertex in current " << v->text() << Data::endl;
			}*/

			//Data::cout() << "force removing current vertex " << v << " " << v->text() << Data::endl;

			/*if (!this->have(v))
			{
				Data::cout() << "mesh::split, accessing v at " << v << " not of this mesh" << Data::endl;
				Data::cout() << "mesh::split, accessing v " << v->getId() << " not of this mesh" << Data::endl;
			}*/

			this->remove(v);

			if (!this->frontHave(except, v))
			{
				//Data::cout() << "adding vertex to mesh" << Data::endl;

				mesh->add(v);
			}

			//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

			//while (!fadjs.empty())
			for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
			{
				//Data::FrontElement *fel = fadjs.front();
				Data::FrontElement *fel = (*iter);

				//fadjs.pop_front();

				/*if (fel->getId() == 650)
				{
					Data::cout() << "debug adj fel " << fel->text() << Data::endl;
				}*/

				//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

				if (/*(felempty) ? (*/!mesh->have(fel)/*) : layerFrontElements.insert(fel).second*/)
				{
					//Data::cout() << "force removing adjacent fel " << fel->text() << Data::endl;

					//this->remove(fel, false);

					//if (!this->frontHave(except, fel))
					if ((this->remove(fel, false)) && (!this->frontHave(except, fel)))
					{
						//Data::cout() << "adding fel at " << fel << " to mesh" << Data::endl;
						//Data::cout() << "adding fel " << fel->getId() << " to mesh" << Data::endl;

						mesh->add(fel);
					}
				}
			}

			//Data::ElementList adjs = this->adjacency(v);

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
			{
				//Data::Element *e = adjs.front();
				Data::Element *e = (*iter);

				//adjs.pop_front();

				//Data::cout() << "adjacent el " << e->text() << Data::endl;

				if (/*(elempty) && (*/!this->have(e)/*)*/)
				{
					//Data::cout() << "adjacent el " << e->text() << " is not in this mesh" << Data::endl;

					continue;
				}

				if (/*(elempty) ? (*/!mesh->have(e)/*) : layerElements.insert(e).second*/)
				{
					//Data::cout() << "force removing adjacent el " << e->text() << Data::endl;

					/*if (!this->have(e))
					{
						Data::cout() << "mesh::split, accessing e at " << e << " not of this mesh" << Data::endl;
						Data::cout() << "mesh::split, accessing e " << e->getId() << " not of this mesh" << Data::endl;
					}*/

					this->forceRemove(e);

					//Data::cout() << "adding el to mesh" << Data::endl;

					mesh->add(e);
				}

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					Data::Vertex *adj = e->getVertex(i);

					//Data::cout() << "element vertex " << adj << " " <<adj->text() << Data::endl;

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()) ||
						(!next.insert(adj).second))
					{
						continue;
					}

					/*if (!vempty)
					{
						layerVertices.insert(adj);
					}*/
				}
			}
		}

		previous.clear();

		previous.swap(current);

		current.swap(next);
	}

#if USE_ID_HASH_IN_MESH_SPLIT
	for (VertexIdHash::iterator iter = current.begin();
#elif USE_C__11
	for (Data::Vertexhash::iterator iter = current.begin();
#else
	for (Data::VertexSet::iterator iter = current.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		 iter != current.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		/*if ((v->getId() == 184) || (v->getId() == 1423) || (v->getId() == 1018))
		{
			Data::cout() << "debug vertex in current after " << v->text() << Data::endl;
		}*/

		//Data::cout() << "force removing current vertex " << v << " " << v->text() << Data::endl;

		/*if (!this->have(v))
		{
			Data::cout() << "mesh::split, accessing v at " << v << " not of this mesh" << Data::endl;
			Data::cout() << "mesh::split, accessing v " << v->getId() << " not of this mesh" << Data::endl;
		}*/

		this->remove(v);

		if (!this->frontHave(except, v))
		{
			//Data::cout() << "adding vertex to mesh" << Data::endl;

			mesh->add(v);
		}

		//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//Data::FrontElement *fel = fadjs.front();
			Data::FrontElement *fel = (*iter);

			//fadjs.pop_front();

			/*if (fel->getId() == 650)
			{
				Data::cout() << "debug adj fel " << fel->text() << Data::endl;
			}*/

			//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

			if (/*(felempty) ? (*/!mesh->have(fel)/*) : layerFrontElements.insert(fel).second*/)
			{
				//Data::cout() << "force removing adjacent fel " << fel->text() << Data::endl;

				/*if (!this->have(fel))
				{
					Data::cout() << "mesh::split, accessing fel at " << fel << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, accessing fel " << fel->getId() << " not of this mesh" << Data::endl;
				}*/

				//this->remove(fel, false);

				//if (!this->frontHave(except, fel))
				if ((this->remove(fel, false)) && (!this->frontHave(except, fel)))
				{
					//Data::cout() << "adding fel at " << fel << " to mesh" << Data::endl;
					//Data::cout() << "adding fel " << fel->getId() << " to mesh" << Data::endl;

					mesh->add(fel);
				}
			}
		}

		//Data::ElementList adjs = this->adjacency(v);

		//while (!adjs.empty())
		for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
		{
			//Data::Element *el = adjs.front();
			Data::Element *el = (*iter);

			//adjs.pop_front();

			//Data::cout() << "adjacent el " << el->text() << Data::endl;

			if (/*(elempty) && (*/!this->have(el)/*)*/)
			{
				//Data::cout() << "adjacent el " << el->text() << " is not in this mesh" << Data::endl;

				continue;
			}

			if (/*(elempty) ? (*/!mesh->have(el)/*) : layerElements.insert(el).second*/)
			{
				//Data::cout() << "force removing adjacent el " << el->text() << Data::endl;

				/*if (!this->have(el))
				{
					Data::cout() << "mesh::split, accessing el at " << el << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, accessing el " << el->getId() << " not of this mesh" << Data::endl;
				}*/

				this->forceRemove(el);

				//Data::cout() << "adding el to mesh" << Data::endl;

				mesh->add(el);

				for (UInt i = 0; i < el->numVertices(); i++)
				{
					Data::Vertex *adj = el->getVertex(i);

					//Data::cout() << "element vertex " << adj << " " << adj->text() << Data::endl;

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()))
					{
						continue;
					}

					//Data::cout() << "added element vertex to next vertices" << Data::endl;

					next.insert(adj);
				}
			}
		}
	}

#if USE_ID_HASH_IN_MESH_SPLIT
	for (VertexIdHash::iterator iter = next.begin();
#elif USE_C__11
	for (Data::VertexHash::iterator iter = next.begin();
#else
	for (Data::VertexSet::iterator iter = next.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		 iter != next.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		/*if ((v->getId() == 184) || (v->getId() == 1423) || (v->getId() == 1018))
		{
			Data::cout() << "debug vertex in next " << v->text() << Data::endl;
		}*/

		//Data::ElementList adjs = this->adjacency(v);

		bool found = false;

		//for (Data::ElementList::iterator iter2 = adjs.begin();
		for (Data::ElementList::iterator iter2 = static_cast<MJMesh::Vertex *>(v)->eBegin();
			 //iter2 != adjs.end(); iter2++)
			 iter2 != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter2++)
		{
			if (this->have((*iter2)))
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			if ((outputShared) && (!sharedBoundary.have(v)))
			{
				sharedBoundary.add(v);
			}
		}
		else
		{
			//Data::cout() << "force removing current vertex " << v << " " << v->text() << Data::endl;

			/*if (!this->have(v))
			{
				Data::cout() << "mesh::split, accessing v at " << v << " not of this mesh" << Data::endl;
				Data::cout() << "mesh::split, accessing v " << v->getId() << " not of this mesh" << Data::endl;
			}*/

			this->remove(v);

			if (!this->frontHave(except, v))
			{
				//Data::cout() << "adding vertex to mesh" << Data::endl;

				mesh->add(v);
			}
		}

		//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//Data::FrontElement *fel = fadjs.front();
			Data::FrontElement *fel = (*iter);

			//fadjs.pop_front();

			/*if (fel->getId() == 650)
			{
				Data::cout() << "debug adj fel " << fel->text() << Data::endl;
			}*/

			//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

			//if (found)
			{
				//Data::ElementList adjs = this->adjacency(fel);

				//if (adjs.empty())
				if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0)
				{
					continue;
				}

				found = false;

				//while (!adjs.empty())
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
					//if (/*(felempty) ? (*/this->have(adjs.front())/*) : layerElements.find(adjs.front()) != layerElements.end()*/)
					if (/*(felempty) ? (*/this->have((*iter))/*) : layerElements.find(adjs.front()) != layerElements.end()*/)
					{
						found = true;

						break;
					}

					//adjs.pop_front();
				}

				if (found)
				{
					continue;
				}
			}

			if (/*(felempty) ? (*/!mesh->have(fel)/*) : layerFrontElements.insert(fel).second*/)
			{
				//Data::cout() << "force removing adjacent fel " << fel->text() << Data::endl;

				/*if (!this->have(fel))
				{
					Data::cout() << "mesh::split, accessing fel at " << fel << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, accessing fel " << fel->getId() << " not of this mesh" << Data::endl;
				}*/

				//this->forceRemove(fel);

				//if (!this->frontHave(except, fel))
				if ((this->remove(fel, false)) && (!this->frontHave(except, fel)))
				{
					//Data::cout() << "adding fel at " << fel << " to mesh" << Data::endl;
					//Data::cout() << "adding fel " << fel->getId() << " to mesh" << Data::endl;

					mesh->add(fel);
				}
			}
		}
	}

	if (!outputShared)
	{
		return;
	}

	for (Data::Boundary::ConstVertexIterator iter = sharedBoundary.vBegin();
		 iter != sharedBoundary.vEnd(); iter++)
	{
		Data::Vertex *v = (*iter);

		/*if ((v->getId() == 184) || (v->getId() == 1423) || (v->getId() == 1018))
		{
			Data::cout() << "debug vertex in shared boundary " << v->text() << Data::endl;
		}*/

		/*if (!this->have(v))
		{
			Data::cout() << "mesh::split, shared, accessing v at " << v << " not of this mesh" << Data::endl;
			Data::cout() << "mesh::split, shared, accessing v " << v->getId() << " not of this mesh" << Data::endl;
		}*/

		//Data::cout() << "shared vertex " << v->text() << Data::endl;

		//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

		//while (!adjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//Data::FrontElement *fel = adjs.front();
			Data::FrontElement *fel = (*iter);

			//adjs.pop_front();

			/*if (fel->getId() == 650)
			{
				Data::cout() << "debug adj fel " << v->text() << Data::endl;
			}*/

			if (sharedBoundary.have(fel))
			{
				continue;
			}

			/*if (!this->have(fel))
			{
				Data::cout() << "mesh::split, shared, accessing fel at " << fel << " not of this mesh" << Data::endl;
				Data::cout() << "mesh::split, shared, accessing fel " << fel->getId() << " not of this mesh" << Data::endl;
			}*/

			//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

			//Data::ElementList els = this->adjacency(fel);

			//Data::BoolVector inMesh(els.size(), false);
			Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

			UInt i = 0;

			//while (!els.empty())
			for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                 iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
			{
				//Data::Element *e = els.front();
				Data::Element *e = (*iter);

				//els.pop_front();

				//Data::cout() << "adjacent el " << e->text() << Data::endl;

				inMesh[i++] = mesh->have(e);
			}

			if (((inMesh.size() == 1) && (inMesh[0])) ||
				((inMesh[0]) && (!inMesh[1])) ||
				((inMesh[1]) && (!inMesh[0])))
			{
				//Data::cout() << "adding fel " << fel->text() << " to shared front elements" << Data::endl;

				sharedBoundary.add(fel);
			}
		}
	}
}

void MJMesh::Mesh::split(const Data::VertexList &vertices, UInt layers, Mesh *mesh,
	bool outputShared, MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
	/*bool noIntersection, */const Data::FrontList &except)
{
	/*Data::VertexSet layerVertices;
	Data::FrontElementSet layerFrontElements;
	Data::ElementSet layerElements;

	bool vempty = ((noIntersection) || (mesh->verticesSize() == 0));
	bool felempty = ((noIntersection) || (mesh->frontElementsSize() == 0));
	bool elempty = ((noIntersection) || (mesh->size() == 0));*/

#if USE_ID_HASH_IN_MESH_SPLIT
	VertexIdHash current(vertices.begin(), vertices.end());
	VertexIdHash previous, next;
#elif USE_C__11
	Data::VertexHash current(vertices.begin(), vertices.end());
	Data::VertexHash previous, next;
#else
	Data::VertexSet current(vertices.begin(), vertices.end());
	Data::VertexSet previous, next;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

	for (UInt i = 0; i < layers; i++)
	{
#if USE_ID_HASH_IN_MESH_SPLIT
		for (VertexIdHash::iterator iter = current.begin();
#elif USE_C__11
		for (Data::VertexHash::iterator iter = current.begin();
#else
		for (Data::VertexSet::iterator iter = current.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
			 iter != current.end(); iter++)
		{
			Data::Vertex *v = (*iter);

			//Data::cout() << "force removing current vertex " << v << " " << v->text() << Data::endl;

			/*if (!this->have(v))
			{
				Data::cout() << "mesh::split, accessing v at " << v << " not of this mesh" << Data::endl;
				Data::cout() << "mesh::split, accessing v " << v->getId() << " not of this mesh" << Data::endl;
			}*/

			this->remove(v);

			if (!this->frontHave(except, v))
			{
				//Data::cout() << "adding vertex to mesh" << Data::endl;

				mesh->add(v);

				if ((outputShared) && (boundary) && (boundary->have(v)) && (!sharedBoundary.have(v)))
				{
					//Data::cout() << "adding vertex " << v << " " << v->text() << " to shared vertices" << Data::endl;

					sharedBoundary.add(v);
				}
			}

			//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

			//while (!fadjs.empty())
			for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
			{
				//Data::FrontElement *fel = fadjs.front();
				Data::FrontElement *fel = (*iter);

				//fadjs.pop_front();

				//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

				if (/*(felempty) ? (*/!mesh->have(fel)/*) : layerFrontElements.insert(fel).second*/)
				{
					//Data::cout() << "force removing adjacent fel " << fel->text() << Data::endl;

					//this->remove(fel, false);

					//if (!this->frontHave(except, fel))
					if ((this->remove(fel, false)) && (!this->frontHave(except, fel)))
					{
						//Data::cout() << "adding fel at " << fel << " to mesh" << Data::endl;
						//Data::cout() << "adding fel " << fel->getId() << " to mesh" << Data::endl;

						mesh->add(fel);

						if ((outputShared) && (boundary) && (boundary->have(fel)) && (!sharedBoundary.have(fel)))
						{
							//Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;

							sharedBoundary.add(fel);

							for (UInt i = 0; i < fel->numVertices(); i++)
							{
								Data::Vertex *adj = fel->getVertex(i);

								if ((adj == v) ||
									(sharedBoundary.have(adj)))
								{
									continue;
								}

								//Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;

								sharedBoundary.add(adj);
							}
						}
					}
				}
			}

			//Data::ElementList adjs = this->adjacency(v);

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
			{
				//Data::Element *e = adjs.front();
				Data::Element *e = (*iter);

				//adjs.pop_front();

				//Data::cout() << "adjacent el " << e->text() << Data::endl;

				if (/*(elempty) && (*/!this->have(e)/*)*/)
				{
					//Data::cout() << "adjacent el " << e->text() << " is not in this mesh" << Data::endl;

					continue;
				}

				if (/*(elempty) ? (*/!mesh->have(e)/*) : layerElements.insert(e).second*/)
				{
					//Data::cout() << "force removing adjacent el " << e->text() << Data::endl;

					/*if (!this->have(e))
					{
						Data::cout() << "mesh::split, accessing e at " << e << " not of this mesh" << Data::endl;
						Data::cout() << "mesh::split, accessing e " << e->getId() << " not of this mesh" << Data::endl;
					}*/

					this->forceRemove(e);

					//Data::cout() << "adding el to mesh" << Data::endl;

					mesh->add(e);
				}

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					Data::Vertex *adj = e->getVertex(i);

					//Data::cout() << "element vertex " << adj << " " <<adj->text() << Data::endl;

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()) ||
						(!next.insert(adj).second))
					{
						continue;
					}

					/*if (!vempty)
					{
						layerVertices.insert(adj);
					}*/
				}
			}
		}

		previous.clear();

		previous.swap(current);

		current.swap(next);
	}

#if USE_ID_HASH_IN_MESH_SPLIT
	for (VertexIdHash::iterator iter = current.begin();
#elif USE_C__11
	for (Data::VertexHash::iterator iter = current.begin();
#else
	for (Data::VertexSet::iterator iter = current.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		 iter != current.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		//Data::cout() << "force removing current vertex " << v << " " << v->text() << Data::endl;

		/*if (!this->have(v))
		{
			Data::cout() << "mesh::split, accessing v at " << v << " not of this mesh" << Data::endl;
			Data::cout() << "mesh::split, accessing v " << v->getId() << " not of this mesh" << Data::endl;
		}*/

		this->remove(v);

		if (!this->frontHave(except, v))
		{
			//Data::cout() << "adding vertex to mesh" << Data::endl;

			mesh->add(v);

			if ((outputShared) && (boundary) && (boundary->have(v)) && (!sharedBoundary.have(v)))
			{
				//Data::cout() << "adding vertex " << v << " " << v->text() << " to shared vertices" << Data::endl;

				sharedBoundary.add(v);
			}
		}

		//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//Data::FrontElement *fel = fadjs.front();
			Data::FrontElement *fel = (*iter);

			//fadjs.pop_front();

			//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

			if (/*(felempty) ? (*/!mesh->have(fel)/*) : layerFrontElements.insert(fel).second*/)
			{
				//Data::cout() << "force removing adjacent fel " << fel->text() << Data::endl;

				/*if (!this->have(fel))
				{
					Data::cout() << "mesh::split, accessing fel at " << fel << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, accessing fel " << fel->getId() << " not of this mesh" << Data::endl;
				}*/

				//this->remove(fel, false);

				//if (!this->frontHave(except, fel))
				if ((this->remove(fel, false)) && (!this->frontHave(except, fel)))
				{
					//Data::cout() << "adding fel at " << fel << " to mesh" << Data::endl;
					//Data::cout() << "adding fel " << fel->getId() << " to mesh" << Data::endl;

					mesh->add(fel);

					if ((outputShared) && (boundary) && (boundary->have(fel)) && (!sharedBoundary.have(fel)))
					{
						//Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;

						sharedBoundary.add(fel);

						for (UInt i = 0; i < fel->numVertices(); i++)
						{
							Data::Vertex *adj = fel->getVertex(i);

							if ((adj == v) ||
								(sharedBoundary.have(adj)))
							{
								continue;
							}

							//Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;

							sharedBoundary.add(adj);
						}
					}
				}
			}
		}

		//Data::ElementList adjs = this->adjacency(v);

		//while (!adjs.empty())
		for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
		{
			//Data::Element *el = adjs.front();
			Data::Element *el = (*iter);

			//adjs.pop_front();

			//Data::cout() << "adjacent el " << el->text() << Data::endl;

			if (/*(elempty) && (*/!this->have(el)/*)*/)
			{
				//Data::cout() << "adjacent el " << el->text() << " is not in this mesh" << Data::endl;

				continue;
			}

			if (/*(elempty) ? (*/!mesh->have(el)/*) : layerElements.insert(el).second*/)
			{
				//Data::cout() << "force removing adjacent el " << el->text() << Data::endl;

				/*if (!this->have(el))
				{
					Data::cout() << "mesh::split, accessing el at " << el << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, accessing el " << el->getId() << " not of this mesh" << Data::endl;
				}*/

				this->forceRemove(el);

				//Data::cout() << "adding el to mesh" << Data::endl;

				mesh->add(el);

				for (UInt i = 0; i < el->numVertices(); i++)
				{
					Data::Vertex *adj = el->getVertex(i);

					//Data::cout() << "element vertex " << adj << " " << adj->text() << Data::endl;

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()))
					{
						continue;
					}

					//Data::cout() << "added element vertex to next vertices" << Data::endl;

					next.insert(adj);
				}
			}
		}
	}

#if USE_ID_HASH_IN_MESH_SPLIT
	for (VertexIdHash::iterator iter = next.begin();
#elif USE_C__11
	for (Data::VertexHash::iterator iter = next.begin();
#else
	for (Data::VertexSet::iterator iter = next.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		 iter != next.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		//Data::ElementList adjs = this->adjacency(v);

		bool found = false;

		//for (Data::ElementList::iterator iter2 = adjs.begin();
		for (Data::ElementList::iterator iter2 = static_cast<MJMesh::Vertex *>(v)->eBegin();
			 //iter2 != adjs.end(); iter2++)
			 iter2 != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter2++)
		{
			if (this->have((*iter2)))
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			if ((outputShared) && ((!boundary) || (!sharedBoundary.have(v))))
			{
				sharedBoundary.add(v);
			}
		}
		else
		{
			//Data::cout() << "force removing current vertex " << v << " " << v->text() << Data::endl;

			/*if (!this->have(v))
			{
				Data::cout() << "mesh::split, accessing v at " << v << " not of this mesh" << Data::endl;
				Data::cout() << "mesh::split, accessing v " << v->getId() << " not of this mesh" << Data::endl;
			}*/

			this->remove(v);

			if (!this->frontHave(except, v))
			{
				//Data::cout() << "adding vertex to mesh" << Data::endl;

				mesh->add(v);

				if ((outputShared) && (boundary) && (boundary->have(v)) && (!sharedBoundary.have(v)))
				{
					//Data::cout() << "adding vertex " << v << " " << v->text() << " to shared vertices" << Data::endl;

					sharedBoundary.add(v);
				}
			}
		}

		//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//Data::FrontElement *fel = fadjs.front();
			Data::FrontElement *fel = (*iter);

			//fadjs.pop_front();

			//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

			//if (found)
			{
				//Data::ElementList adjs = this->adjacency(fel);

				//if (adjs.empty())
				if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0)
				{
					continue;
				}

				found = false;

				//while (!adjs.empty())
                for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
					//if (/*(felempty) ? (*/this->have(adjs.front())/*) : layerElements.find(adjs.front()) != layerElements.end()*/)
					if (/*(felempty) ? (*/this->have((*iter))/*) : layerElements.find(adjs.front()) != layerElements.end()*/)
					{
						found = true;

						break;
					}

					//adjs.pop_front();
				}

				if (found)
				{
					continue;
				}
			}

			if (/*(felempty) ? (*/!mesh->have(fel)/*) : layerFrontElements.insert(fel).second*/)
			{
				//Data::cout() << "force removing adjacent fel " << fel->text() << Data::endl;

				/*if (!this->have(fel))
				{
					Data::cout() << "mesh::split, accessing fel at " << fel << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, accessing fel " << fel->getId() << " not of this mesh" << Data::endl;
				}*/

				//this->forceRemove(fel);

				//if (!this->frontHave(except, fel))
				if ((this->remove(fel, false)) && (!this->frontHave(except, fel)))
				{
					//Data::cout() << "adding fel at " << fel << " to mesh" << Data::endl;
					//Data::cout() << "adding fel " << fel->getId() << " to mesh" << Data::endl;

					mesh->add(fel);

					if ((outputShared) && (boundary) && (boundary->have(fel)) && (!sharedBoundary.have(fel)))
					{
						//Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;

						sharedBoundary.add(fel);

						for (UInt i = 0; i < fel->numVertices(); i++)
						{
							Data::Vertex *adj = fel->getVertex(i);

							if ((adj == v) ||
								(sharedBoundary.have(adj)))
							{
								continue;
							}

							//Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;

							sharedBoundary.add(adj);
						}
					}
				}
			}
		}
	}

	if (!outputShared)
	{
		return;
	}

	if (boundary)
	{
#if USE_ID_HASH_IN_MESH_SPLIT
		VertexIdHash newVertices;
#elif USE_C__11
		Data::VertexHash newVertices;
#else
		Data::VertexSet newVertices;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

		for (Data::Boundary::ConstVertexIterator iter = sharedBoundary.vBegin();
			 iter != sharedBoundary.vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

			//Data::cout() << "testing shared vertex " << v << " " << v->text() << Data::endl;

			//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

			//while (!adjs.empty())
			for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
			{
				//Data::FrontElement *fel = adjs.front();
				Data::FrontElement *fel = (*iter);

				//adjs.pop_front();

				//Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;

				if (sharedBoundary.have(fel))
				{
					//Data::cout() << "adjacent front element " << fel << " " << fel->text() << " is already in shared front elements" << Data::endl;

					continue;
				}

				//Data::ElementList els = this->adjacency(fel);

				//if ((els.size() == 1) && (!boundary->have(fel)))
				if ((static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 1) && (!boundary->have(fel)))
				{
					//Data::cout() << "adjacent front element " << fel << " " << fel->text() << " has only one adjacent element and is not in boundary" << Data::endl;

					continue;
				}

				//Data::BoolVector inMesh(els.size(), false);
				Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

				UInt i = 0;

				//while (!els.empty())
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
					//Data::Element *e = els.front();
					Data::Element *e = (*iter);

					//els.pop_front();

					inMesh[i++] = this->have(e);

					/*if (inMesh[i-1])
					{
						Data::cout() << "element adjacent to front element " << e << " " << e->text() << " found in this mesh " << this << Data::endl;
					}
					else
					{
						Data::cout() << "element adjacent to front element " << e << " " << e->text() << " not found in this mesh " << this << Data::endl;
					}*/
				}

				if (((inMesh.size() == 1) && (inMesh[0])) ||
					((inMesh[0]) && (!inMesh[1])) ||
					((inMesh[1]) && (!inMesh[0])))
				{
					//Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;

					sharedBoundary.add(fel);

					for (UInt i = 0; i < fel->numVertices(); i++)
					{
						Data::Vertex *adj = fel->getVertex(i);

						if ((adj == v) ||
							(sharedBoundary.have(adj)))
						{
							continue;
						}

						newVertices.insert(adj);
					}
				}
			}
		}

		while (!newVertices.empty())
		{
#if USE_ID_HASH_IN_MESH_SPLIT
			VertexIdHash oldVertices;
#elif USE_C__11
			Data::VertexHash oldVertices;
#else
			Data::VertexSet oldVertices;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

			oldVertices.swap(newVertices);

			while (!oldVertices.empty())
			{
				Data::Vertex *v = *oldVertices.begin();

				oldVertices.erase(oldVertices.begin());

				//Data::cout() << "testing new shared vertex " << v << " " << v->text() << Data::endl;

				//Data::cout() << "adding new shared vertex " << v << " " << v->text() << " to shared vertices" << Data::endl;

				sharedBoundary.add(v);

				//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

				//while (!adjs.empty())
				for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
				{
					//Data::FrontElement *fel = adjs.front();
					Data::FrontElement *fel = (*iter);

					//adjs.pop_front();

					//Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;

					if (sharedBoundary.have(fel))
					{
						//Data::cout() << "adjacent front element " << fel << " " << fel->text() << " is already in shared front elements" << Data::endl;

						continue;
					}

					//Data::ElementList els = this->adjacency(fel);

					//if ((els.size() == 1) && (!boundary->have(fel)))
					if ((static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 1) && (!boundary->have(fel)))
					{
						//Data::cout() << "adjacent front element " << fel << " " << fel->text() << " has only one adjacent element and is not in boundary" << Data::endl;

						continue;
					}

					//Data::BoolVector inMesh(els.size(), false);
					Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

					UInt i = 0;

					//while (!els.empty())
					for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                         iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
					{
						//Data::Element *e = els.front();
						Data::Element *e = (*iter);

						//els.pop_front();

						inMesh[i++] = mesh->have(e);

						/*if (inMesh[i-1])
						{
							Data::cout() << "element adjacent to front element " << e << " " << e->text() << " found in this mesh " << this << Data::endl;
						}
						else
						{
							Data::cout() << "element adjacent to front element " << e << " " << e->text() << " not found in this mesh " << this << Data::endl;
						}*/
					}

					if (((inMesh.size() == 1) && (inMesh[0])) ||
						((inMesh[0]) && (!inMesh[1])) ||
						((inMesh[1]) && (!inMesh[0])))
					{
						//Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;

						sharedBoundary.add(fel);

						for (UInt i = 0; i < fel->numVertices(); i++)
						{
							Data::Vertex *adj = fel->getVertex(i);

							if ((adj == v) ||
								(sharedBoundary.have(adj)) ||
								(oldVertices.find(adj) != oldVertices.end()))
							{
								continue;
							}

							newVertices.insert(adj);
						}
					}
				}
			}
		}
	}
	else
	{
		for (Data::Boundary::ConstVertexIterator iter = sharedBoundary.vBegin();
			 iter != sharedBoundary.vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

			/*if (!this->have(v))
			{
				Data::cout() << "mesh::split, shared, accessing v at " << v << " not of this mesh" << Data::endl;
				Data::cout() << "mesh::split, shared, accessing v " << v->getId() << " not of this mesh" << Data::endl;
			}*/

			//Data::cout() << "shared vertex " << v->text() << Data::endl;

			//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

			//while (!adjs.empty())
			for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
			{
				//Data::FrontElement *fel = adjs.front();
				Data::FrontElement *fel = (*iter);

				//adjs.pop_front();

				if (sharedBoundary.have(fel))
				{
					continue;
				}

				/*if (!this->have(fel))
				{
					Data::cout() << "mesh::split, shared, accessing fel at " << fel << " not of this mesh" << Data::endl;
					Data::cout() << "mesh::split, shared, accessing fel " << fel->getId() << " not of this mesh" << Data::endl;
				}*/

				//Data::cout() << "adjacent fel " << fel->text() << Data::endl;

				//Data::ElementList els = this->adjacency(fel);

				//Data::BoolVector inMesh(els.size(), false);
				Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

				UInt i = 0;

				//while (!els.empty())
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
					//Data::Element *e = els.front();
					Data::Element *e = (*iter);

					//els.pop_front();

					//Data::cout() << "adjacent el " << e->text() << Data::endl;

					inMesh[i++] = mesh->have(e);
				}

				if (((inMesh.size() == 1) && (inMesh[0])) ||
					((inMesh[0]) && (!inMesh[1])) ||
					((inMesh[1]) && (!inMesh[0])))
				{
					//Data::cout() << "adding fel " << fel->text() << " to shared front elements" << Data::endl;

					sharedBoundary.add(fel);
				}
			}
		}
	}
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers,
    const Data::Box *box, const MJMesh::Front *front)
{
	MJMesh::Boundary sharedBoundary;

	this->splitTo(vertices, layers, false, sharedBoundary, NULL, box, front);
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers,
	MJMesh::Boundary &sharedBoundary, const Data::Box *box, const MJMesh::Front *front)
{
	this->splitTo(vertices, layers, true, sharedBoundary, NULL, box, front);
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
	const Data::Box *box, const MJMesh::Front *front)
{
	this->splitTo(vertices, layers, true, sharedBoundary, boundary, box, front);
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary1, const MJMesh::Boundary *boundary2,
	const Data::Box *box, const MJMesh::Front *front)
{
	this->splitTo(vertices, layers, true, sharedBoundary, boundary1, boundary2, box, front);
}

/*void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers,
    MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries, const Data::Box *box)
{
    this->splitTo(vertices, layers, true, sharedBoundary, boundaries, box);
}*/

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToBeginning(VertexIdHash &current, /*VertexIdHash &next,*/
#elif USE_C__11
void MJMesh::Mesh::splitToBeginning(Data::VertexHash &current, /*Data::VertexHash &next,*/
#else
void MJMesh::Mesh::splitToBeginning(Data::VertexSet &current, /*Data::VertexSet &next,*/
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
	const Data::VertexList &vertices/*, bool outputShared,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary*/)
{
	current.insert(vertices.begin(), vertices.end());
}

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToIteration(VertexIdHash &current, VertexIdHash &next, const Data::Box *box, const MJMesh::Front *front, bool swap)
#elif USE_C__11
void MJMesh::Mesh::splitToIteration(Data::VertexHash &current, Data::VertexHash &next, const Data::Box *box, const MJMesh::Front *front, bool swap)
#else
void MJMesh::Mesh::splitToIteration(Data::VertexSet &current, Data::VertexSet &next, const Data::Box *box, const MJMesh::Front *front, bool swap)
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
{
	MJMesh::Boundary sharedBoundary;

	return this->splitToIteration(current, next, false, sharedBoundary, NULL, box, front, swap);
}

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToIteration(VertexIdHash &current, VertexIdHash &next,
#elif USE_C__11
void MJMesh::Mesh::splitToIteration(Data::VertexHash &current, Data::VertexHash &next,
#else
void MJMesh::Mesh::splitToIteration(Data::VertexSet &current, Data::VertexSet &next,
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
	/*const Data::VertexList &vertices, */bool outputShared,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
	const Data::Box *box, const MJMesh::Front *front, bool swap)
{
    Data::BoundaryList boundaries;

    if (boundary)
    {
        boundaries.push_back(const_cast<MJMesh::Boundary *>(boundary));
    }

    this->splitToIteration(current, next, outputShared, sharedBoundary, boundaries, box, front, swap);
}

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToIteration(VertexIdHash &current, VertexIdHash &next,
#elif USE_C__11
void MJMesh::Mesh::splitToIteration(Data::VertexHash &current, Data::VertexHash &next,
#else
void MJMesh::Mesh::splitToIteration(Data::VertexSet &current, Data::VertexSet &next,
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
	/*const Data::VertexList &vertices, */bool outputShared,
	MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
	const Data::Box *box, const MJMesh::Front *front, bool swap)
{
#if USE_ID_HASH_IN_MESH_SPLIT
	for (VertexIdHash::iterator iter = current.begin();
#elif USE_C__11
	for (Data::VertexHash::iterator iter = current.begin();
#else
	for (Data::VertexSet::iterator iter = current.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		 iter != current.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

#if PRINT_SPLIT_TO_DEBUG
		bool print = ((v->getId() == 1125) || (v->getId() == 49098) || (v->getId() == 92559));
		//bool print = true;

		if (print)
        {
            Data::cout() << "testing vertex " << v << " " << v->text() << Data::endl;
        }
#endif //#if PRINT_SPLIT_TO_DEBUG

		bool found = false;

		Data::MeshList meshes;

		for (MJMesh::VertexSpecificData::MeshDataMap::iterator iter = v->getData().meshes.begin();
			 iter != v->getData().meshes.end(); iter++)
		{
			if ((*iter).first == this)
			{
				found = true;

#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "found vertex " << v << " " << v->text() << " in this mesh " << this << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				continue;
			}

			meshes.push_back(const_cast<MJMesh::Mesh *>((*iter).first));
		}

		while (!meshes.empty())
		{
#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "removing vertex " << v << " " << v->text() << " from mesh " << meshes.front() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			static_cast<MJMesh::Mesh *>(meshes.front())->forceRemove(v);

			meshes.pop_front();
		}

		bool removedFromBoundary = false;

		if (!found)
		{
#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "adding vertex " << v << " " << v->text() << " to this mesh " << this << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			this->add(v);

			if ((outputShared) && (!boundaries.empty()))
			{
				bool inBoundary = this->boundaryHave(boundaries, v);
				bool inSharedBoundary = sharedBoundary.have(v);

				if ((inBoundary) && (!inSharedBoundary))
                {
#if PRINT_SPLIT_TO_DEBUG
                    if (print)
                    {
                        Data::cout() << "adding vertex " << v << " " << v->text() << " to shared boundary" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

                    sharedBoundary.add(v);
                }
				else if ((!inBoundary) && (inSharedBoundary))
                {
#if PRINT_SPLIT_TO_DEBUG
                    if (print)
                    {
                        Data::cout() << "removing vertex " << v << " " << v->text() << " from shared boundary" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

                    sharedBoundary.remove(v);

                    removedFromBoundary = true;
                }
			}
		}

		if ((box) && (box->optOut(v->getPoint())))
        {
#if PRINT_SPLIT_TO_DEBUG
            if (print)
            {
                Data::cout() << "vertex " << v << " " << v->text() << " is out of box" << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

            if (removedFromBoundary)
            {
                //Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

                //while (!fadjs.empty())
                for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
                {
                    //MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fadjs.front());
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                    //fadjs.pop_front();

                    if (sharedBoundary.have(fel))
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from shared boundary" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.remove(fel);
                    }
                }
            }
            else if (outputShared)
            {
                for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                    if (sharedBoundary.have(fel))
                    {
                        continue;
                    }

                    bool all = true;

                    for (UInt i = 0; i < fel->numVertices(); i++)
                    {
                        Data::Vertex *v = fel->getVertex(i);

                        if ((!this->have(v)) || (!sharedBoundary.have(v)))
                        {
                            all = false;

                            break;
                        }
                    }

                    if (all)
                    {
                        all = false;

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *o = fel->getVertex(i);

                            if (o == v)
                            {
                                continue;
                            }

                            if (!box->optOut(o->getPoint()))
                            {
                                all = true;

                                break;
                            }
                        }

                        if ((!all) && (front) && (front->have(fel)))
                        {
                            all = true;
                        }
                    }

                    if (all)
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared boundary" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.add(fel);
                    }
                }
            }

            continue;
        }

		//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fadjs.front());
			MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

			//fadjs.pop_front();

#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

            ///Talvez precise colocar esse if com o continue tambem no splitToEnding
            if ((!box) && (fel->numAdjacents() == 0))
            {
                bool skip = false;

                for (UInt i = 0; i < fel->numVertices(); i++)
                {
                    MJMesh::Vertex *adjv = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

                    if (adjv == v)
                    {
                        continue;
                    }

                    if (!this->have(adjv))
                    {
                        skip = true;

                        break;
                    }

                    MJMesh::VertexSpecificData::MeshDataMap meshes = adjv->getData().meshes;

                    if (meshes.size() > 1)
                    {
                        skip = true;

                        break;
                    }

                    if ((*meshes.begin()).first != this)
                    {
                        skip = true;

                        break;
                    }
                }

                if (skip)
                {
#if PRINT_SPLIT_TO_DEBUG
                    if (print)
                    {
                        Data::cout() << "skipping " << fel << " " << fel->text() << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

                    continue;
                }
            }

			found = false;

			for (MJMesh::FrontElementSpecificData::MeshDataMap::iterator iter = fel->getData().meshes.begin();
				 iter != fel->getData().meshes.end(); iter++)
			{
				if ((*iter).first == this)
				{
					found = true;

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "found adjacent front element " << fel << " " << fel->text() << " in this mesh " << this << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

				meshes.push_back(const_cast<MJMesh::Mesh *>((*iter).first));
			}

			while (!meshes.empty())
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from mesh " << meshes.front() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				static_cast<MJMesh::Mesh *>(meshes.front())->forceRemove(fel);

				meshes.pop_front();
			}

			if (!found)
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to this mesh " << this << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				this->add(fel);

				if ((outputShared) && (!boundaries.empty()))
				{
				    bool inBoundary = this->boundaryHave(boundaries, fel);
                    bool inSharedBoundary = sharedBoundary.have(fel);

				    if ((inBoundary) && (!inSharedBoundary))
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.add(fel);

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *adj = fel->getVertex(i);

                            if //((adj == v) ||
                                (sharedBoundary.have(adj))//)
                            {
                                continue;
                            }

#if PRINT_SPLIT_TO_DEBUG
                            if (print)
                            {
                                Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;
                            }
#endif //#if PRINT_SPLIT_TO_DEBUG

                            sharedBoundary.add(adj);
                        }
                    }
                    else if ((!inBoundary) && (inSharedBoundary))
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from shared front elements" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.remove(fel);

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *adj = fel->getVertex(i);

                            if //((adj == v) ||
                                (!sharedBoundary.have(adj))//)
                            {
                                continue;
                            }

                            //Data::FrontElementList adjFels = this->findAdjacentFrontElements(adj);

                            bool found = false;

                            //while (!adjFels.empty())
                            for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(adj)->feBegin();
                                 iter != static_cast<MJMesh::Vertex *>(adj)->feEnd(); iter++)
                            {
                                //Data::FrontElement *adjFel = adjFels.front();
                                Data::FrontElement *adjFel = (*iter);

                                //adjFels.pop_front();

                                if (adjFel == fel)
                                {
                                    continue;
                                }

                                if (sharedBoundary.have(adjFel))
                                {
                                    found = true;

                                    break;
                                }
                            }

                            if (!found)
                            {
#if PRINT_SPLIT_TO_DEBUG
                                if (print)
                                {
                                    Data::cout() << "removing adjacent vertex " << adj << " " << adj->text() << " from shared vertices" << Data::endl;
                                }
#endif //#if PRINT_SPLIT_TO_DEBUG

                                sharedBoundary.remove(adj);
                            }
                        }
                    }
				}
			}
		}

		//Data::ElementList adjs = this->adjacency(v);

		//while (!adjs.empty())
		for (Data::ElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->eBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter++)
		{
			//MJMesh::Element *e = static_cast<MJMesh::Element *>(adjs.front());
			MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

			//adjs.pop_front();

#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "testing adjacent element " << e << " " << e->text() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			found = false;

			for (MJMesh::ElementSpecificData::MeshDataMap::iterator iter = e->getData().meshes.begin();
				 iter != e->getData().meshes.end(); iter++)
			{
				if ((*iter).first == this)
				{
					found = true;

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "found adjacent element " << e << " " << e->text() << " in this mesh " << this << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

				meshes.push_back(const_cast<MJMesh::Mesh *>((*iter).first));
			}

			while (!meshes.empty())
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "removing adjacent element " << e << " " << e->text() << " from mesh " << meshes.front() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				static_cast<MJMesh::Mesh *>(meshes.front())->forceRemove(e);

				meshes.pop_front();
			}

			if (found)
			{
				continue;
			}

#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "adding adjacent element " << e << " " << e->text() << " to this mesh " << this << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			this->add(e);

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				Data::Vertex *adj = e->getVertex(i);

#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "testing adjacent element vertex " << adj << " " << adj->text() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				if ((adj == v) ||
					(current.find(adj) != current.end())/* ||
					(previous.find(adj) != previous.end())*/)
				{
					continue;
				}

#if PRINT_SPLIT_TO_DEBUG
				if (print)
				{
					if (next.insert(adj).second)
                    {
                        Data::cout() << "adding adjacent element vertex " << adj << " " << adj->text() << " to next layer" << Data::endl;
                    }
				}
				else
#endif //#if PRINT_SPLIT_TO_DEBUG
				{
				    next.insert(adj);
				}
			}
		}
	}

	if (swap)
	{
		//previous.clear();

		//previous.swap(current);

		current.clear();

		current.swap(next);
	}
}

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToEnding(VertexIdHash &current, VertexIdHash &next, const Data::Box *box, const MJMesh::Front *front)
#elif USE_C__11
void MJMesh::Mesh::splitToEnding(Data::VertexHash &current, Data::VertexHash &next, const Data::Box *box, const MJMesh::Front *front)
#else
void MJMesh::Mesh::splitToEnding(Data::VertexSet &current, Data::VertexSet &next, const Data::Box *box, const MJMesh::Front *front)
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
{
	MJMesh::Boundary sharedBoundary;

	this->splitToEnding(current, next, false, sharedBoundary, NULL, box, front);
}

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToEnding(VertexIdHash &current, VertexIdHash &next,
#elif USE_C__11
void MJMesh::Mesh::splitToEnding(Data::VertexHash &current, Data::VertexHash &next,
#else
void MJMesh::Mesh::splitToEnding(Data::VertexSet &current, Data::VertexSet &next,
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
	/*const Data::VertexList &vertices, */bool outputShared,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
	const Data::Box *box, const MJMesh::Front *front)
{
    Data::BoundaryList boundaries;

    if (boundary)
    {
        boundaries.push_back(const_cast<MJMesh::Boundary *>(boundary));
    }

    this->splitToEnding(current, next, outputShared, sharedBoundary, boundaries, box, front);
}

#if USE_ID_HASH_IN_MESH_SPLIT
void MJMesh::Mesh::splitToEnding(VertexIdHash &current, VertexIdHash &next,
#elif USE_C__11
void MJMesh::Mesh::splitToEnding(Data::VertexHash &current, Data::VertexHash &next,
#else
void MJMesh::Mesh::splitToEnding(Data::VertexSet &current, Data::VertexSet &next,
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
	/*const Data::VertexList &vertices, */bool outputShared,
	MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
	const Data::Box *box, const MJMesh::Front *front)
{
	this->splitToIteration(current, next, /*vertices, */outputShared, sharedBoundary, boundaries, box, front, false);

	//Data::cout() << "next (layer " << layers + 1 << ")" << Data::endl;

/*#if USE_C__11
	Data::FrontElementHash felsRemovedFromBoundary;
#else
	Data::FrontElementSet felsRemovedFromBoundary;
#endif //#if USE_C__11*/

#if USE_ID_HASH_IN_MESH_SPLIT
	for (VertexIdHash::iterator iter = next.begin();
#elif USE_C__11
	for (Data::VertexHash::iterator iter = next.begin();
#else
	for (Data::VertexSet::iterator iter = next.begin();
#endif //#if USE_ID_HASH_IN_MESH_SPLIT
		 iter != next.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

#if PRINT_SPLIT_TO_DEBUG
		bool print = ((v->getId() == 1125) || (v->getId() == 49098) || (v->getId() == 92559));
		//bool print = true;

		if (print)
        {
            Data::cout() << "(end) testing vertex " << v << " " << v->text() << Data::endl;
        }
#endif //#if PRINT_SPLIT_TO_DEBUG

		//Data::ElementList adjs = this->adjacency(v);

		bool found = false;

		//for (Data::ElementList::iterator iter2 = adjs.begin();
		for (Data::ElementList::iterator iter2 = static_cast<MJMesh::Vertex *>(v)->eBegin();
			 //iter2 != adjs.end(); iter2++)
			 iter2 != static_cast<MJMesh::Vertex *>(v)->eEnd(); iter2++)
		{
#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "testing adjacent element " << (*iter2) << " " << (*iter2)->text() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			if (!this->have((*iter2)))
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "adjacent element " << (*iter2) << " " << (*iter2)->text() << " not found in this mesh " << this << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				found = true;

				break;
			}
		}

		bool removedFromBoundary = false;

		if (found)
		{
#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "vertex " << v << " " << v->text() << ", there was an adjacent element not found in this mesh " << this << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			//if ((outputShared) && ((!boundaries.empty()) || (!sharedBoundary.have(v))))
			if ((outputShared) && (!sharedBoundary.have(v)))
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "adding vertex " << v << " " << v->text() << " to shared boundary" << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				sharedBoundary.add(v);
			}
		}
		else
		{
			Data::MeshList meshes;

			for (MJMesh::VertexSpecificData::MeshDataMap::iterator iter = v->getData().meshes.begin();
				 iter != v->getData().meshes.end(); iter++)
			{
				meshes.push_back(const_cast<MJMesh::Mesh *>((*iter).first));
			}

			while (!meshes.empty())
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "removing vertex " << v << " " << v->text() << " from mesh " << meshes.front() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				static_cast<MJMesh::Mesh *>(meshes.front())->forceRemove(v);

				meshes.pop_front();
			}

#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "adding vertex " << v << " " << v->text() << " to this mesh " << this << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			this->add(v);

			/*if ((outputShared) && (!boundaries.empty()) && (this->boundaryHave(boundaries, v)) && (!sharedBoundary.have(v)))
			{
				//Data::cout() << "adding vertex " << v << " " << v->text() << " to shared boundary" << Data::endl;

				sharedBoundary.add(v);
			}*/
			if ((outputShared) && (!boundaries.empty()))
			{
				bool inBoundary = this->boundaryHave(boundaries, v);
				bool inSharedBoundary = sharedBoundary.have(v);

				if ((inBoundary) && (!inSharedBoundary))
                {
#if PRINT_SPLIT_TO_DEBUG
                    if (print)
                    {
                        Data::cout() << "adding vertex " << v << " " << v->text() << " to shared boundary" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

                    sharedBoundary.add(v);
                }
				else if ((!inBoundary) && (inSharedBoundary))
                {
#if PRINT_SPLIT_TO_DEBUG
                    if (print)
                    {
                        Data::cout() << "removing vertex " << v << " " << v->text() << " from shared boundary" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

                    sharedBoundary.remove(v);

                    removedFromBoundary = true;
                }
			}
		}

		if ((box) && (box->optOut(v->getPoint())))
        {
#if PRINT_SPLIT_TO_DEBUG
            if (print)
            {
                Data::cout() << "vertex " << v << " " << v->text() << " is out of box" << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

            if (removedFromBoundary)
            {
                //Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

                //while (!fadjs.empty())
                for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
                {
                    //MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fadjs.front());
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                    //fadjs.pop_front();

                    if (sharedBoundary.have(fel))
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from shared boundary" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.remove(fel);
                    }
                }
            }
            else if (outputShared)
            {
                for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
                {
                    MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

                    if (sharedBoundary.have(fel))
                    {
                        continue;
                    }

                    bool all = true;

                    for (UInt i = 0; i < fel->numVertices(); i++)
                    {
                        Data::Vertex *v = fel->getVertex(i);

                        if ((!this->have(v)) || (!sharedBoundary.have(v)))
                        {
                            all = false;

                            break;
                        }
                    }

                    if (all)
                    {
                        all = false;

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *o = fel->getVertex(i);

                            if (o == v)
                            {
                                continue;
                            }

                            if (!box->optOut(o->getPoint()))
                            {
                                all = true;

                                break;
                            }
                        }

                        if ((!all) && (front) && (front->have(fel)))
                        {
                            all = true;
                        }
                    }

                    if (all)
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared boundary" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.add(fel);
                    }
                }
            }

            continue;
        }

		bool foundAnyOutOfMesh = false;
		bool foundFelInBoundary = false;

		//Data::FrontElementList fadjs = this->findAdjacentFrontElements(v);

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
             iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
		{
			//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fadjs.front());
			MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

			//fadjs.pop_front();

#if PRINT_SPLIT_TO_DEBUG
			if (print)
            {
                Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			//if (found)
			{
				//Data::ElementList adjs = this->adjacency(fel);

				//if (adjs.empty())
				if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0)
				{
					if (sharedBoundary.have(fel))
					{
						foundFelInBoundary = true;
					}

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "skipping " << fel << " " << fel->text() << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

				found = false;

				//while (!adjs.empty())
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "testing element adjacent to front element at " << (*iter) << " " << (*iter)->text() << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					//if (!this->have(adjs.front()))
					if (!this->have((*iter)))
					{
						found = true;

						foundAnyOutOfMesh = true;

#if PRINT_SPLIT_TO_DEBUG
						if (print)
                        {
                            Data::cout() << "adjacent element at " << (*iter) << " " << (*iter)->text() << " not found in this mesh " << this << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

						break;
					}

					//adjs.pop_front();
				}

				if (found)
				{
					if (sharedBoundary.have(fel))
					{
						foundFelInBoundary = true;
					}

					continue;
				}

				if ((outputShared) && (sharedBoundary.have(fel)))
                {
					///Velho -> if (true)
					///Novo  -> if ((!boundaries.empty()) && (!this->boundaryHave(boundaries, fel)))

					if ((!boundaries.empty()) && (!this->boundaryHave(boundaries, fel)))
					//if (true)
					{
#if PRINT_SPLIT_TO_DEBUG
						if (print)
						{
							Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from shared boundary" << Data::endl;
						}
#endif //#if PRINT_SPLIT_TO_DEBUG

						sharedBoundary.remove(fel);

						//felsRemovedFromBoundary.insert(fel);
					}
					else
					{
						for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *adj = fel->getVertex(i);

                            if ((adj == v) ||
                                (sharedBoundary.have(adj)))
                            {
                                continue;
                            }

#if PRINT_SPLIT_TO_DEBUG
                            if (print)
                            {
                                Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;
                            }
#endif //#if PRINT_SPLIT_TO_DEBUG

                            sharedBoundary.add(adj);

                            foundFelInBoundary = true;
                        }
					}
                }
			}

			found = false;

			Data::MeshList meshes;

			for (MJMesh::FrontElementSpecificData::MeshDataMap::iterator iter = fel->getData().meshes.begin();
				 iter != fel->getData().meshes.end(); iter++)
			{
				if ((*iter).first == this)
				{
					found = true;

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "found adjacent front element " << fel << " " << fel->text() << " in this mesh " << this << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

				meshes.push_back(const_cast<MJMesh::Mesh *>((*iter).first));
			}

			while (!meshes.empty())
			{
				static_cast<MJMesh::Mesh *>(meshes.front())->forceRemove(fel);

#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from mesh " << meshes.front() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				meshes.pop_front();
			}

			if (!found)
			{
#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to this mesh " << this << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				this->add(fel);

				/*if ((outputShared) && (!boundaries.empty()) && (this->boundaryHave(boundaries, fel)) && (!sharedBoundary.have(fel)))
				{
					//Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;

					sharedBoundary.add(fel);

					for (UInt i = 0; i < fel->numVertices(); i++)
					{
						Data::Vertex *adj = fel->getVertex(i);

						if ((adj == v) ||
							(sharedBoundary.have(adj)))
						{
							continue;
						}

						//Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;

						sharedBoundary.add(adj);
					}
				}*/
				if ((outputShared) && (!boundaries.empty()))
				{
				    bool inBoundary = this->boundaryHave(boundaries, fel);
                    bool inSharedBoundary = sharedBoundary.have(fel);

				    if ((inBoundary) && (!inSharedBoundary))
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        foundFelInBoundary = true;

                        sharedBoundary.add(fel);

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *adj = fel->getVertex(i);

                            if ((adj == v) ||
                                (sharedBoundary.have(adj)))
                            {
                                continue;
                            }

#if PRINT_SPLIT_TO_DEBUG
                            if (print)
                            {
                                Data::cout() << "adding adjacent vertex " << adj << " " << adj->text() << " to shared vertices" << Data::endl;
                            }
#endif //#if PRINT_SPLIT_TO_DEBUG

                            sharedBoundary.add(adj);
                        }
                    }
                    else if ((!inBoundary) && (inSharedBoundary))
                    {
#if PRINT_SPLIT_TO_DEBUG
                        if (print)
                        {
                            Data::cout() << "removing adjacent front element " << fel << " " << fel->text() << " from shared front elements" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

                        sharedBoundary.remove(fel);

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *adj = fel->getVertex(i);

                            if ((adj == v) ||
                                (!sharedBoundary.have(adj)))
                            {
                                continue;
                            }

                            //Data::FrontElementList adjFels = this->findAdjacentFrontElements(adj);

                            bool found = false;

                            //while (!adjFels.empty())
                            for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(adj)->feBegin();
                                 iter != static_cast<MJMesh::Vertex *>(adj)->feEnd(); iter++)
                            {
                                //Data::FrontElement *adjFel = adjFels.front();
                                Data::FrontElement *adjFel = (*iter);

                                //adjFels.pop_front();

                                if (adjFel == fel)
                                {
                                    continue;
                                }

                                if (sharedBoundary.have(adjFel))
                                {
                                    found = true;

                                    break;
                                }
                            }

                            if (!found)
                            {
#if PRINT_SPLIT_TO_DEBUG
                                if (print)
                                {
                                    Data::cout() << "removing adjacent vertex " << adj << " " << adj->text() << " from shared vertices" << Data::endl;
                                }
#endif //#if PRINT_SPLIT_TO_DEBUG

                                sharedBoundary.remove(adj);
                            }
                        }
                    }
				}
			}
			else
			{
				if (sharedBoundary.have(fel))
				{
					foundFelInBoundary = true;
				}
			}
		}

        if ((!foundAnyOutOfMesh) && (!foundFelInBoundary) && (static_cast<MJMesh::Vertex *>(v)->numAdjacentFrontElements() != 0) &&
            (outputShared) && (sharedBoundary.have(v)))
        {
#if PRINT_SPLIT_TO_DEBUG
            if (print)
            {
                Data::cout() << "removing vertex " << v << " " << v->text() << " from shared boundary" << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

            sharedBoundary.remove(v);
        }
	}

	if (!outputShared)
	{
		return;
	}

	if (!boundaries.empty())
	{
#if USE_ID_HASH_IN_MESH_SPLIT
		VertexIdHash newVertices;
#elif USE_C__11
		Data::VertexHash newVertices;
#else
		Data::VertexSet newVertices;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

		for (Data::Boundary::ConstVertexIterator iter = sharedBoundary.vBegin();
			 iter != sharedBoundary.vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

#if PRINT_SPLIT_TO_DEBUG
			bool print = ((v->getId() == 1125) || (v->getId() == 49098) || (v->getId() == 92559));
			//bool print = true;

			if (print)
            {
                Data::cout() << "(shared) testing vertex " << v << " " << v->text() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

			//while (!adjs.empty())
			for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
			{
				//Data::FrontElement *fel = adjs.front();
				Data::FrontElement *fel = (*iter);

				//adjs.pop_front();

#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				if (sharedBoundary.have(fel))
				{
#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "adjacent front element " << fel << " " << fel->text() << " is already in shared front elements" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

				//Data::ElementList els = this->adjacency(fel);

				if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0)
				{
#if PRINT_SPLIT_TO_DEBUG
				    if (print)
                    {
                        Data::cout() << "skipping " << fel << " " << fel->text() << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

				    continue;
				}

				//if ((els.size() == 1) && (!this->boundaryHave(boundaries, fel)))
				if ((static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 1) && (!this->boundaryHave(boundaries, fel)))
				{
#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "adjacent front element " << fel << " " << fel->text() << " has only one adjacent element and is not in boundary" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "fel " << fel->getId() << " has " << static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() << " adjacent elements" << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				//Data::BoolVector inMesh(els.size(), false);
				Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

				UInt i = 0;

				//while (!els.empty())
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
					//Data::Element *e = els.front();
					Data::Element *e = (*iter);

					//els.pop_front();

					inMesh[i++] = this->have(e);

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        if (inMesh[i-1])
                        {
                            Data::cout() << "element adjacent to front element " << e << " " << e->text() << " found in this mesh " << this << Data::endl;
                        }
                        else
                        {
                            Data::cout() << "element adjacent to front element " << e << " " << e->text() << " not found in this mesh " << this << Data::endl;
                        }
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG
				}

				if (((inMesh.size() == 1) && (inMesh[0])) ||
					((inMesh[0]) && (!inMesh[1])) ||
					((inMesh[1]) && (!inMesh[0])))
				{
#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					sharedBoundary.add(fel);

					for (UInt i = 0; i < fel->numVertices(); i++)
					{
						Data::Vertex *adj = fel->getVertex(i);

						if ((adj == v) ||
							(sharedBoundary.have(adj)))
						{
							continue;
						}

						newVertices.insert(adj);
					}
				}
			}
		}

		while (!newVertices.empty())
		{
#if USE_ID_HASH_IN_MESH_SPLIT
			VertexIdHash oldVertices;
#elif USE_C__11
			Data::VertexHash oldVertices;
#else
			Data::VertexSet oldVertices;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

			oldVertices.swap(newVertices);

			while (!oldVertices.empty())
			{
				Data::Vertex *v = *oldVertices.begin();

				oldVertices.erase(oldVertices.begin());

#if PRINT_SPLIT_TO_DEBUG
				bool print = ((v->getId() == 1125) || (v->getId() == 49098) || (v->getId() == 92559));
				//bool print = true;

				if (print)
                {
                    Data::cout() << "(new shared) testing vertex " << v << " " << v->text() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				//Data::cout() << "adding new shared vertex " << v << " " << v->text() << " to shared boundary" << Data::endl;

				sharedBoundary.add(v);

				//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

				//while (!adjs.empty())
				for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                     iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
				{
					//Data::FrontElement *fel = adjs.front();
					Data::FrontElement *fel = (*iter);

					//adjs.pop_front();

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					if (sharedBoundary.have(fel))
					{
#if PRINT_SPLIT_TO_DEBUG
						if (print)
                        {
                            Data::cout() << "adjacent front element " << fel << " " << fel->text() << " is already in shared front elements" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

						continue;
					}

					//Data::ElementList els = this->adjacency(fel);

					if (static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 0)
                    {
                        continue;
                    }

					//if ((els.size() == 1) && (!this->boundaryHave(boundaries, fel)))
					if ((static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() == 1) && (!this->boundaryHave(boundaries, fel)))
					{
#if PRINT_SPLIT_TO_DEBUG
						if (print)
                        {
                            Data::cout() << "adjacent front element " << fel << " " << fel->text() << " has only one adjacent element and is not in boundary" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

						continue;
					}

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "fel " << fel->getId() << " has " << static_cast<MJMesh::FrontElement *>(fel)->numAdjacents() << " adjacent elements" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					//Data::BoolVector inMesh(els.size(), false);
					Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

					UInt i = 0;

					//while (!els.empty())
					for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                         iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
					{
						//Data::Element *e = els.front();
						Data::Element *e = (*iter);

						//els.pop_front();

						inMesh[i++] = this->have(e);

#if PRINT_SPLIT_TO_DEBUG
						if (print)
                        {
                            if (inMesh[i-1])
                            {
                                Data::cout() << "element adjacent to front element " << e << " " << e->text() << " found in this mesh " << this << Data::endl;
                            }
                            else
                            {
                                Data::cout() << "element adjacent to front element " << e << " " << e->text() << " not found in this mesh " << this << Data::endl;
                            }
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG
					}

					if (((inMesh.size() == 1) && (inMesh[0])) ||
						((inMesh[0]) && (!inMesh[1])) ||
						((inMesh[1]) && (!inMesh[0])))
					{
#if PRINT_SPLIT_TO_DEBUG
						if (print)
                        {
                            Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;
                        }
#endif //#if PRINT_SPLIT_TO_DEBUG

						sharedBoundary.add(fel);

						for (UInt i = 0; i < fel->numVertices(); i++)
						{
							Data::Vertex *adj = fel->getVertex(i);

							if ((adj == v) ||
								(sharedBoundary.have(adj)) ||
								(oldVertices.find(adj) != oldVertices.end()))
							{
								continue;
							}

							newVertices.insert(adj);
						}
					}
				}
			}
		}

/*#if USE_C__11
		for (Data::FrontElementHash::iterator iter = felsRemovedFromBoundary.begin();
#else
		for (Data::FrontElementSet::iterator iter = felsRemovedFromBoundary.begin();
#endif //#if USE_C__11
			 iter != felsRemovedFromBoundary.end(); iter++)
		{
			Data::FrontElement *fel = (*iter);

			Data::cout() << "retesting fel " << fel->text() << Data::endl;

			if (this->have(fel) && (!boundaries.empty()) && (this->boundaryHave(boundaries, fel)) && (!sharedBoundary.have(fel)))
			{
				Data::cout() << "readding fel " << fel->text() << " to shared front elements" << Data::endl;

				sharedBoundary.add(fel);

				for (UInt i = 0; i < fel->numVertices(); i++)
				{
					Data::Vertex *v = fel->getVertex(i);

					if (!sharedBoundary.have(v))
					{
						Data::cout() << "readding v " << v->text() << " to shared vertices" << Data::endl;

						sharedBoundary.add(v);
					}
				}
			}
		}*/
	}
	else
	{
		for (Data::Boundary::ConstVertexIterator iter = sharedBoundary.vBegin();
			 iter != sharedBoundary.vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

#if PRINT_SPLIT_TO_DEBUG
			bool print = ((v->getId() == 1125) || (v->getId() == 49098) || (v->getId() == 92559));
			//bool print = true;

			if (print)
            {
                Data::cout() << "(shared 2) testing vertex " << v << " " << v->text() << Data::endl;
            }
#endif //#if PRINT_SPLIT_TO_DEBUG

			//Data::FrontElementList adjs = this->findAdjacentFrontElements(v);

			//while (!adjs.empty())
			for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(v)->feBegin();
                 iter != static_cast<MJMesh::Vertex *>(v)->feEnd(); iter++)
			{
				//Data::FrontElement *fel = adjs.front();
				Data::FrontElement *fel = (*iter);

				//adjs.pop_front();

#if PRINT_SPLIT_TO_DEBUG
				if (print)
                {
                    Data::cout() << "testing adjacent front element " << fel << " " << fel->text() << Data::endl;
                }
#endif //#if PRINT_SPLIT_TO_DEBUG

				if (sharedBoundary.have(fel))
				{
#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "adjacent front element " << fel << " " << fel->text() << " is already in shared front elements" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					continue;
				}

				//Data::ElementList els = this->adjacency(fel);

				//Data::BoolVector inMesh(els.size(), false);
				Data::BoolVector inMesh(static_cast<MJMesh::FrontElement *>(fel)->numAdjacents(), false);

				UInt i = 0;

				//while (!els.empty())
				for (Data::ElementList::iterator iter = static_cast<MJMesh::FrontElement *>(fel)->eBegin();
                     iter != static_cast<MJMesh::FrontElement *>(fel)->eEnd(); iter++)
				{
					//Data::Element *e = els.front();
					Data::Element *e = (*iter);

					//els.pop_front();

					inMesh[i++] = this->have(e);

#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        if (inMesh[i-1])
                        {
                            Data::cout() << "element adjacent to front element " << e << " " << e->text() << " found in this mesh " << this << Data::endl;
                        }
                        else
                        {
                            Data::cout() << "element adjacent to front element " << e << " " << e->text() << " not found in this mesh " << this << Data::endl;
                        }
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG
				}

				if (((inMesh.size() == 1) && (inMesh[0])) ||
					((inMesh[0]) && (!inMesh[1])) ||
					((inMesh[1]) && (!inMesh[0])))
				{
#if PRINT_SPLIT_TO_DEBUG
					if (print)
                    {
                        Data::cout() << "adding adjacent front element " << fel << " " << fel->text() << " to shared front elements" << Data::endl;
                    }
#endif //#if PRINT_SPLIT_TO_DEBUG

					sharedBoundary.add(fel);
				}
			}
		}
	}
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers, bool outputShared,
	MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary,
	const Data::Box *box, const MJMesh::Front *front)
{
    this->splitTo(vertices, layers, outputShared, sharedBoundary, boundary, NULL, box, front);
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers, bool outputShared,
    MJMesh::Boundary &sharedBoundary, const MJMesh::Boundary *boundary1, const MJMesh::Boundary *boundary2,
    const Data::Box *box, const MJMesh::Front *front)
{
    Data::BoundaryList boundaries;

    if (boundary1)
    {
        boundaries.push_back(const_cast<MJMesh::Boundary *>(boundary1));
    }

    if ((boundary2) && (boundary2 != boundary1))
    {
        boundaries.push_back(const_cast<MJMesh::Boundary *>(boundary2));
    }

    this->splitTo(vertices, layers, outputShared, sharedBoundary, boundaries, box, front);
}

void MJMesh::Mesh::splitTo(const Data::VertexList &vertices, UInt layers, bool outputShared,
	MJMesh::Boundary &sharedBoundary, const Data::BoundaryList &boundaries,
	const Data::Box *box, const MJMesh::Front *front)
{
#if USE_ID_HASH_IN_MESH_SPLIT
	VertexIdHash current, next;//, previous;
#elif USE_C__11
	Data::VertexHash current, next;//, previous;
#else
	Data::VertexSet current, next;//, previous;
#endif //#if USE_ID_HASH_IN_MESH_SPLIT

	this->splitToBeginning(current, /*next, */vertices/*, outputShared, sharedBoundary, boundary*/);

	for (UInt i = 0; i < layers; i++)
	{
		//Data::cout() << "layer " << i << Data::endl;

		this->splitToIteration(current, next, /*vertices, */outputShared, sharedBoundary, boundaries, box, front);
	}

	this->splitToEnding(current, next, /*vertices, */outputShared, sharedBoundary, boundaries, box, front);
}

void MJMesh::Mesh::merge(MJMesh::Mesh *mesh, const Data::Boundary *boundary)
{
    this->merge(mesh, Data::FrontList(), boundary);
}

void MJMesh::Mesh::merge(MJMesh::Mesh *mesh, const Data::FrontList &except, const Data::Boundary *boundary)
{
	for (Data::Mesh::VertexIterator iter = mesh->vBegin();
		 iter != mesh->vEnd(); iter = mesh->erase(iter))
	{
		Data::Vertex *v = (*iter);

		//Data::cout() << "trying to merge, vertex: " << v << Data::endl;
		//Data::cout() << "trying to merge, vertex: " << v->text() << Data::endl;

		if (!this->frontHave(except, v))
		{
		    this->add(v, (!boundary) || (!boundary->have(v)));
		}
	}

	for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
		 iter != mesh->feEnd(); iter = mesh->erase(iter))
	{
		Data::FrontElement *fel = (*iter);

		if (!this->frontHave(except, fel))
		{
			this->add(fel);
		}
	}

	for (Data::Mesh::ElementIterator iter = mesh->eBegin();
		 iter != mesh->eEnd(); iter = mesh->erase(iter))
	{
		Data::Element *el = (*iter);

		this->add(el);
	}
}

Data::ElementList MJMesh::Mesh::sort() const
{
	/*Data::ElementList elements(this->elements.begin(), this->elements.end());
	Data::ElementList sorted;

	while (!elements.empty())
	{
		Data::Element *least = elements.front();
		Data::ElementList::iterator leastIter = elements.begin();
		Real leastSize = least->getShape()->size();

		for (Data::ElementList::iterator iter = ++elements.begin();
			 iter != elements.end(); iter++)
		{
			Real iterSize = (*iter)->getShape()->size();

			if (iterSize < leastSize)
			{
				least = (*iter);
				leastIter = iter;
				leastSize = iterSize;
			}
		}

		elements.erase(leastIter);

		sorted.push_back(least);
	}

	return sorted;*/

	//Data::ElementList sorted(this->elements);
	Data::ElementList sorted(this->elements.begin(), this->elements.end());

	sorted.sort(sizeSmaller);

	return sorted;
}

#if USE_GUI
void MJMesh::Mesh::draw() const
{
	if (!this->hasLists())
	{
		this->createList(2);
	}

	if (this->hasList())
	{
		this->drawList();

		return;
	}

	this->beginList();

	if ((Data::getState().isElementDrawn()) ||
		(Data::getState().isFrontElementDrawn()) ||
		(Data::getState().isVertexDrawn()))
	{
		UInt dimension = this->dimension();

		int modes[2];

		glGetIntegerv(GL_POLYGON_MODE, modes);

		//bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

		glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

        glPointSize(static_cast<GLfloat>(Data::getState().getPointSize()));

		GLboolean lighting = glIsEnabled(GL_LIGHTING);

		if (Data::getState().isElementDrawn())
		{
			struct AdjacencyFinder : public Data::AbstractMesh::AdjacencyFinder
			{
				const MJMesh::Mesh *mesh;

				Data::FrontElementList operator()(const Data::Vertex *v) const
				{
				    Data::ElementList els = this->mesh->adjacency(v);

				    Data::FrontElementList fels;

				    while (!els.empty())
                    {
                        fels.push_back(static_cast<Data::FrontElement *>(els.front()));

                        els.pop_front();
                    }

					return fels;
				}
			};

			AdjacencyFinder af;

			af.mesh = this;

			this->drawWithArrayPlanesOrVolumes(this->elements.begin(), this->elements.end(), dimension, this->size(),
				Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
				true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
                (Data::getState().isNormalByVertex()) && (this->isSurface()), &af,
				true, Data::getState().getQualityRangeMin(), Data::getState().getQualityRangeMax(),
				false, this->getR(), this->getG(), this->getB(), false);

			/*UInt size = (dimension == 2) ? 1 : 4;

			if ((dimension == 2) && (!this->elements.empty()) && ((*this->elements.begin())->numVertices() == 4))
			{
				size *= 2;
			}
			else if ((dimension == 3) && (!this->elements.empty()) && ((*this->elements.begin())->numVertices() == 8))
			{
				size *= 3;
			}

			size *= 3*static_cast<UInt>(this->elements.size());

			GLfloat *elements = new GLfloat[dimension*size];
			GLfloat *colors = new GLfloat[3*size];
			GLfloat *normals = (dimension == 2) ? NULL : new GLfloat[3*size];

			bool border = ((dimension == 2) && (!wireframe) && (Data::getState().isTriangleBorderDrawn()));

			Real br, bg, bb;
			GLfloat *bcolors = NULL;

			if (border)
            {
                Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT, br, bg, bb);
                bcolors = new GLfloat[3*size];
            }

			GLuint i = 0;

			for (Data::Mesh::ConstElementIterator iter = this->elements.begin();
				 (iter != this->elements.end()) && (i < size); iter++)
			{
				Real quality = (*iter)->quality();

				quality = (quality < 0.0) ? 0.0 : ((quality > 1.0) ? 1.0 : quality);

				if ((quality >= Data::getState().getQualityRangeMin()) && (quality <= Data::getState().getQualityRangeMax()))
				{
					GLuint oldi = i;

					i += (*iter)->getShape()->fill(elements + dimension*i, colors + 3*i, normals ? normals + 3*i : NULL);

					if (border)
                    {
                        for (GLuint j = oldi; j < i; j++)
                        {
                            bcolors[3*j + 0] = static_cast<GLfloat>(br);
                            bcolors[3*j + 1] = static_cast<GLfloat>(bg);
                            bcolors[3*j + 2] = static_cast<GLfloat>(bb);
                        }
                    }
				}
			}

			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			if (normals) glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(dimension, GL_FLOAT, 0, elements);
			glColorPointer(3, GL_FLOAT, 0, colors);
			if (normals) glNormalPointer(GL_FLOAT, 0, normals);

			glDrawArrays(GL_TRIANGLES, 0, i);

			if (border)
            {
                //GLboolean lighting = glIsEnabled(GL_LIGHTING);

				//if (lighting) glDisable(GL_LIGHTING);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				if (bcolors) glColorPointer(3, GL_FLOAT, 0, bcolors);

				glDrawArrays(GL_TRIANGLES, 0, i);

				//if (lighting) glEnable(GL_LIGHTING);

				glPolygonMode(GL_FRONT, modes[0]);
				glPolygonMode(GL_BACK,  modes[1]);
            }

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			if (normals) glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_COLOR_MATERIAL);

			delete [] elements;
			delete [] colors;
			if (normals) delete [] normals;
			if (bcolors) delete [] bcolors;

			if (Data::getState().isIdDrawn())
			{
				i = 0;

				for (Data::Mesh::ConstElementIterator iter = this->elements.begin();
					 (iter != this->elements.end()) && (i < size); iter++)
				{
					(*iter)->drawId();
				}
			}*/
		}

		if (Data::getState().isFrontElementDrawn())
		{
			this->drawWithArrayLinesOrPlanes(this->frontElements.begin(), this->frontElements.end(), dimension, this->frontElementsSize(),
				Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
				true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
				false, NULL, false, this->getR(), this->getG(), this->getB(), false);

			/*UInt size = dimension*static_cast<UInt>(this->frontElements.size());

			if ((dimension == 3) && (!this->frontElements.empty()) && ((*this->frontElements.begin())->numVertices() == 4))
			{
				size *= 2;
			}

			GLfloat *fels = new GLfloat[dimension*size];
			GLfloat *colors = new GLfloat[3*size];
			GLfloat *normals = (dimension == 2) ? NULL : new GLfloat[3*size];

			GLuint i = 0;

			for (Data::Mesh::ConstFrontElementIterator iter = this->frontElements.begin();
				 (iter != this->frontElements.end()) && (i < size); iter++)
			{
				i += (*iter)->getShape()->fill(fels + dimension*i, colors + 3*i, normals ? normals + 3*i : NULL);
			}

			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			if (normals) glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(dimension, GL_FLOAT, 0, fels);
			glColorPointer(3, GL_FLOAT, 0, colors);
			if (normals) glNormalPointer(GL_FLOAT, 0, normals);

			bool lines = (dimension == 2) || ((!this->frontElements.empty()) && ((*this->frontElements.begin())->numVertices() == 2));

			if ((lighting) && (lines) && (dimension == 3)) glDisable(GL_LIGHTING);

			glDrawArrays(lines ? GL_LINES : GL_TRIANGLES, 0, i);

			if ((lighting) && (lines) && (dimension == 3)) glEnable(GL_LIGHTING);

			if ((!lines) && (dimension == 3) && (!wireframe) && (Data::getState().isTriangleBorderDrawn()))
			{
				//GLboolean lighting = glIsEnabled(GL_LIGHTING);

				if (lighting) glDisable(GL_LIGHTING);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glDrawArrays(GL_TRIANGLES, 0, i);

				if (lighting) glEnable(GL_LIGHTING);

				glPolygonMode(GL_FRONT, modes[0]);
				glPolygonMode(GL_BACK,  modes[1]);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			if (normals) glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_COLOR_MATERIAL);

			delete [] fels;
			delete [] colors;
			if (normals) delete [] normals;

			if (Data::getState().isIdDrawn())
			{
				i = 0;

				for (Data::Mesh::ConstFrontElementIterator iter = this->frontElements.begin();
					 (iter != this->frontElements.end()) && (i < size); iter++)
				{
					(*iter)->drawId();
				}
			}*/
		}

		if (Data::getState().isVertexDrawn())
		{
			this->drawWithArrayPoints(this->vertices.begin(), this->vertices.end(), dimension, static_cast<UInt>(this->vertices.size()),
				Data::getState().isIdDrawn(), false, true, Data::getState().getPointSize(false), Data::getState().getPointSize(true),
				false, this->getR(), this->getG(), this->getB(), false);
		}
	}

	this->endList();
}
#endif //#if USE_GUI

#if USE_GUI
//*debug
/*#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"

bool MJMesh::Mesh::test(const MJMesh::Boundary *boundary, const MJMesh::Front *front) const
{
	Data::cout() << "testing mesh " << this->getId() << Data::endl;

	UInt dimension = this->dimension();

	if ((dimension != 2) && (dimension != 3))
	{
		Data::cout() << "dimension of mesh is " << dimension << ", skipping mesh" << Data::endl;

		return false;
	}

	bool consistent = true;

	bool testElements = (this->size() != 0);
	bool internalFrontElements = (this->frontElementsSize() == 0) ? false :
		(this->frontElementsSize() != ((boundary ? boundary->size() : 0) + (front ? front->size() : 0)));

	Real tstart, tend, time, total, tstep;
	UInt size, count;

	count = 0;
	tstart = tend = time = tstep = total = 0.0;
	size = this->verticesSize();

	for (Data::Mesh::ConstVertexIterator iter = this->vBegin(), end = this->vEnd();
		 iter != end; iter++)
	{
		if (((++count)%500 == 0) || (count == 1) || (count == size))
		{
			Data::cout() << "testing vertex " << count << " of " << size
					  << ", timings: current: " << Data::time(tstep)
					  << ", total: " << Data::time(total);

			if (time > 0.0)
			{
				Data::cout() << ", estimated: " << Data::time(time*static_cast<Real>(size - count)/2.0);
			}

			Data::cout() << " (hh:mm:ss)" << Data::endl;
		}

		tend = Data::time();

		if (tstart > 0.0)
		{
			time = tend - tstart;
			tstep += time;
			total += time;
		}

		tstart = tend;

		Data::Vertex *test = (*iter);

		MJMesh::Vertex *vtest = dynamic_cast<MJMesh::Vertex *>(test);

		if (!vtest)
		{
			Data::cout() << "could not convert vertex " << test->text() << " to MJMesh::Vertex, skipping vertex" << Data::endl;

			consistent = false;

			continue;
		}

		if (!test->getPoint())
		{
			Data::cout() << "vertex " << test->text() << " has no point, skipping vertex" << Data::endl;

			consistent = false;

			continue;
		}

		if (!vtest->isInMesh(this))
		{
			Data::cout() << "vertex " << test->text() << " does not have mesh information" << Data::endl;

			consistent = false;
		}
		else if (boundary)
		{
			bool inner = vtest->isMeshInner(this);
			bool bhave = boundary->have(vtest);

			if ((inner) && (bhave))
			{
				Data::cout() << "vertex " << test->text() << " is marked as inner but is also in boundary" << Data::endl;

				consistent = false;
			}

			if ((!inner) && (!bhave))
			{
				Data::cout() << "vertex " << test->text() << " is marked as outer but is not in boundary" << Data::endl;

				consistent = false;
			}
		}

		Data::Mesh::ConstVertexIterator iter2 = iter;

		for (iter2++; iter2 != end; iter2++)
		{
			Data::Vertex *other = (*iter2);

			if (test == other)
			{
				Data::cout() << "vertex " << test->text() << " appears twice in mesh" << Data::endl;

				consistent = false;
			}

			if (other->getId() == test->getId())
			{
				Data::cout() << "vertex " << test->text() << " has other with same id: " << other->text() << Data::endl;

				consistent = false;
			}

			bool point = false, geometricPoint = false;

			if (other->getPoint() == test->getPoint())
			{
				point = true;
			}

			if (other->getPoint())
			{
				bool match = test->getPoint()->match(*other->getPoint());

				if (match)
				{
					geometricPoint = true;
				}
				else if (test->getPoint()->equal(*other->getPoint()))
				{
					Data::cout() << "vertex " << test->text() << " and vertex " << other->text() << " are dangerously close to each other" << Data::endl;
				}
			}

			bool crack = false;

			if ((point) || (geometricPoint))
			{
				if (boundary)
				{
					Data::FrontElementList testFels = boundary->adjacency(test);
					Data::FrontElementList otherFels = boundary->adjacency(other);

					Data::FrontElementList testCrackFels, otherCrackFels;

					while (!testFels.empty())
					{
						Data::FrontElement *testFel = testFels.front();

						testFels.pop_front();

						Data::FrontElementList::iterator iter = otherFels.end();

						for (iter = otherFels.begin(); iter != otherFels.end(); iter++)
						{
							Data::FrontElement *otherFel = (*iter);

							if (testFel->numVertices() != otherFel->numVertices())
							{
								continue;
							}

							bool allGeometricPoints = true;

							for (UInt i = 0; i < testFel->numVertices(); i++)
							{
								Data::Vertex *tv = testFel->getVertex(i);

								bool foundGeometricPoint = false;

								for (UInt j = 0; j < otherFel->numVertices(); j++)
								{
									Data::Vertex *ov = otherFel->getVertex(j);

									if (tv->getPoint()->match(ov->getPoint()))
									{
										foundGeometricPoint = true;

										break;
									}
								}

								if (!foundGeometricPoint)
								{
									allGeometricPoints = false;

									break;
								}
							}

							if (allGeometricPoints)
							{
								break;
							}
						}

						if (iter != otherFels.end())
						{
							testCrackFels.push_back(testFel);
							otherCrackFels.push_back((*iter));

							otherFels.erase(iter);
						}
					}

					if (!testCrackFels.empty())
					{
						crack = true;

						while (!testCrackFels.empty())
						{
							Data::FrontElement *testFel = testCrackFels.front();
							Data::FrontElement *otherFel = otherCrackFels.front();

							testCrackFels.pop_front();
							otherCrackFels.pop_front();

							Real dot1 = 0.0, dot2 = 0.0;

							if (dimension == 2)
							{
								dot1 = static_cast<Data::Edge2D *>(testFel->getShape())->normal().dot(static_cast<Data::Edge2D *>(otherFel->getShape())->normal());
								dot2 = static_cast<Data::Edge2D *>(testFel->getShape())->normal().dot(static_cast<Data::Edge2D *>(testFel->getShape())->normal());
							}
							else
							{
								dot1 = static_cast<Data::Polygon *>(testFel->getShape())->normal().dot(static_cast<Data::Polygon *>(otherFel->getShape())->normal());
								dot2 = static_cast<Data::Polygon *>(testFel->getShape())->normal().dot(static_cast<Data::Polygon *>(testFel->getShape())->normal());
							}

							//dot1 and dot2 should be opposite (avoid normalization)
							if (std::fabs(dot1 + dot2) > Data::getTolerance())
							{
								crack = false;

								break;
							}
						}
					}
				}
			}

			if (crack)
			{
				Data::cout() << "vertex " << test->text() << " and vertex " << other->text() << " were detected as in a crack" << Data::endl;
			}
			else
			{
				if (point)
				{
					Data::cout() << "vertex " << test->text() << " and vertex " << other->text() << " share the same point" << Data::endl;

					consistent = false;
				}

				if (geometricPoint)
				{
					Data::cout() << "vertex " << test->text() << " and vertex " << other->text() << " are in the same position" << Data::endl;

					consistent = false;
				}
			}
		}

		if (testElements)
		{
#if USE_EXACT
			Data::EReal accumAngle = 0.0;
#else
			Real accumAngle = 0.0;
#endif //#if USE_EXACT

			Data::ElementList elements = vtest->getAdjacentElements();

			if (elements.empty())
			{
				Data::cout() << "vertex " << test->text() << " has no adjacent elements" << Data::endl;

				consistent = false;
			}

			while (!elements.empty())
			{
				Data::Element *e = elements.front();

				elements.pop_front();

				if (std::find(elements.begin(), elements.end(), e) != elements.end())
				{
					Data::cout() << "vertex " << test->text() << " has element " << e->text() << " more than once in adjacent elements list" << Data::endl;

					consistent = false;
				}

				if (!this->have(e))
				{
					Data::cout() << "vertex " << test->text() << " has element " << e->text() << " that is not in this mesh" << Data::endl;

					consistent = false;
				}

				UInt index = e->numVertices();

				bool skip = false;

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					if (e->getVertex(i) == test)
					{
						if (index == e->numVertices())
						{
							index = i;
						}
						else
						{
							Data::cout() << "vertex " << test->text() << " appears more than once (" << index << " and " << i << ") in adjacent element: " << e->text() << Data::endl;

							skip = true;
						}
					}
				}

				if (skip)
				{
					Data::cout() << "skipping adjacent element" << Data::endl;

					consistent = false;

					continue;
				}

				if (index == e->numVertices())
				{
					Data::cout() << "vertex " << test->text() << " has adjacent element that does not have it: " << e->text() << ", skipping adjacent element" << Data::endl;

					consistent = false;

					continue;
				}

				if (!e->getShape())
				{
					Data::cout() << "vertex " << test->text() << " has adjacent element " << e->text() << " that does not have shape, skipping adjacent element" << Data::endl;

					consistent = false;

					continue;
				}

				if (e->getShape()->getPoint(index) != test->getPoint())
				{
					Data::cout() << "vertex " << test->text() << " is in adjacent element " << e->text() << ", but the points in index " << index << " do not correspond, skipping adjacent element" << Data::endl;

					consistent = false;

					continue;
				}

#if USE_EXACT
				Data::EReal angle = 0.0;
#else
				Real angle = 0.0;
#endif //#if USE_EXACT

				if (dimension == 2)
				{
					for (UInt i = 0; i < e->numVertices() - 2; i++)
					{
						Data::Edge2D edge(
							e->getVertex((index + i + 1)%(e->numVertices()))->getPoint(),
							e->getVertex((index + i + 2)%(e->numVertices()))->getPoint());

						Real tol = Data::getTolerance();

						Data::setTolerance(edge.size()*0.0001);

						angle += edge.angle(test->getPoint());

						Data::setTolerance(tol);
					}
				}
				else
				{
					for (UInt i = 0; i < e->numVertices() - 3; i++)
					{
						Data::Triangle3D triangle(
							e->getVertex((index + i + 1)%(e->numVertices()))->getPoint(),
							e->getVertex((index + i + 2)%(e->numVertices()))->getPoint(),
							e->getVertex((index + i + 3)%(e->numVertices()))->getPoint());

						Real tol = Data::getTolerance();

						Data::setTolerance(triangle.size()*0.00001);

						angle += triangle.solidAngle(test->getPoint());

						Data::setTolerance(tol);
					}
				}

				accumAngle += angle;
			}

			bool inner = this->haveInner(test);
			bool inboundary = boundary ? boundary->have(vtest) : false;
			bool infront = front ? front->have(vtest) : false;

			Real testAngle = (dimension == 2) ? 2*M_PI : 4*M_PI;

#if USE_EXACT
			Real showAngle = accumAngle;
#else
			Real showAngle = accumAngle;
#endif //#if USE_EXACT

			if (std::fabs(testAngle - accumAngle) > Data::getTolerance())
			{
				///TODO check angles formed by boundary and/or front

				if ((!inboundary) && (!infront))
				{
					Data::cout() << "vertex " << test->text() << " is not completely surrounded by elements, total angle is " << showAngle << ", instead of " << testAngle << Data::endl;

					consistent = false;
				}

				if (inner)
				{
					Data::cout() << "vertex " << test->text() << " is marked as inner but is not completely surrounded by elements, total angle is " << showAngle << ", instead of " << testAngle << Data::endl;

					consistent = false;
				}
			}
			else
			{
				bool crackTip = false;

				if (boundary)
				{
					crackTip = true;

					Data::FrontElementList adjs = boundary->adjacency(test);

					while (!adjs.empty())
					{
						Data::FrontElement *fel = adjs.front();

						adjs.pop_front();

						Data::FrontElement *fel2 = NULL;

						for (Data::FrontElementList::iterator iter = adjs.begin();
							 iter != adjs.end(); iter++)
						{
							Data::FrontElement *f = (*iter);

							if (f->numVertices() != fel->numVertices())
							{
								continue;
							}

							bool haveAll = true;

							for (Data::UInt i = 0; i < fel->numVertices(); i++)
							{
								if (fel->getVertex(i) == test)
								{
									continue;
								}

								if ((!f->have(fel->getVertex(i))) &&
									(!fel->getShape()->have(*fel->getVertex(i)->getPoint())))
								{
									haveAll = false;

									break;
								}
							}

							if (haveAll)
							{
								fel2 = (*iter);

								adjs.erase(iter);

								break;
							}
						}

						if (!fel2)
						{
							crackTip = false;

							break;
						}
					}
				}

				if ((crackTip) && (inboundary) && (!inner))
				{
					Data::cout() << "vertex " << test->text() << " was detected as a crack tip, because it is in boundary and is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << Data::endl;
				}
				else
				{
					if (!inner)
					{
						Data::cout() << "vertex " << test->text() << " is not marked as inner but is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << Data::endl;

						consistent = false;
					}

					if (inboundary)
					{
						Data::cout() << "vertex " << test->text() << " is in boundary but is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << Data::endl;

						consistent = false;
					}

					if (infront)
					{
						Data::cout() << "vertex " << test->text() << " is in front but is completely surrounded by elements, total angle is " << showAngle << ", compared to " << testAngle << Data::endl;

						consistent = false;
					}
				}
			}
		}

		Data::FrontElementList frontElements = vtest->getAdjacentFrontElements();

		if ((frontElements.empty()) && (internalFrontElements))
		{
			Data::cout() << "vertex " << test->text() << " has no adjacent front elements" << Data::endl;

			consistent = false;
		}

		bool allInBoundaryOrInFront = true;

		while (!frontElements.empty())
		{
			Data::FrontElement *e = frontElements.front();

			frontElements.pop_front();

			if (std::find(frontElements.begin(), frontElements.end(), e) != frontElements.end())
			{
				Data::cout() << "vertex " << test->text() << " has front element " << e->text() << " more than once in adjacent front elements list" << Data::endl;

				consistent = false;
			}

			if (!this->have(e))
			{
				Data::cout() << "vertex " << test->text() << " has front element " << e->text() << " that is not in this mesh" << Data::endl;

				consistent = false;
			}

			UInt index = e->numVertices();

			bool skip = false;

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				if (e->getVertex(i) == test)
				{
					if (index == e->numVertices())
					{
						index = i;
					}
					else
					{
						Data::cout() << "vertex " << test->text() << " appears more than once (" << index << " and " << i << ") in adjacent front element: " << e->text() << Data::endl;

						skip = true;
					}
				}
			}

			if (skip)
			{
				Data::cout() << "skipping adjacent front element" << Data::endl;

				consistent = false;

				continue;
			}

			if (index == e->numVertices())
			{
				Data::cout() << "vertex " << test->text() << " has adjacent front element that does not have it: " << e->text() << ", skipping adjacent front element" << Data::endl;

				consistent = false;

				continue;
			}

			if (!e->getShape())
			{
				Data::cout() << "vertex " << test->text() << " has adjacent front element " << e->text() << " that does not have shape, skipping adjacent front element" << Data::endl;

				consistent = false;

				continue;
			}

			if (e->getShape()->getPoint(index) != test->getPoint())
			{
				Data::cout() << "vertex " << test->text() << " is in adjacent front element " << e->text() << ", but the points in index " << index << " do not correspond, skipping adjacent front element" << Data::endl;

				consistent = false;

				continue;
			}

			if (!internalFrontElements)
			{
				bool inboundary = false;
				bool infront = false;

				if ((boundary) && (boundary->have(e)))
				{
					inboundary = true;
				}

				if ((front) && (front->have(e)))
				{
					infront = true;
				}

				if ((!inboundary) && (!infront))
				{
					allInBoundaryOrInFront = false;
				}
			}

			///TODO check angles
		}

		if (!allInBoundaryOrInFront)
		{
			Data::cout() << "vertex " << test->text() << " has adjacent front element(s) that do not belong to boundary or front" << Data::endl;

			consistent = false;
		}
	}

	count = 0;
	tstart = tend = time = tstep = 0.0;
	size = this->frontElementsSize();

	for (Data::Mesh::ConstFrontElementIterator iter = this->feBegin(), end = this->feEnd();
		 iter != end; iter++)
	{
		if (((++count)%500 == 0) || (count == 1) || (count == size))
		{
			Data::cout() << "testing front element " << count << " of " << size
					  << ", timings: current: " << Data::time(tstep)
					  << ", total: " << Data::time(total);

			if (time > 0.0)
			{
				Data::cout() << ", estimated: " << Data::time(time*static_cast<Real>(size - count)/2.0);
			}

			Data::cout() << " (hh:mm:ss)" << Data::endl;
		}

		tend = Data::time();

		if (tstart > 0.0)
		{
			time = tend - tstart;
			tstep += time;
			total += time;
		}

		tstart = tend;

		Data::FrontElement *test = (*iter);

		MJMesh::FrontElement *fetest = dynamic_cast<MJMesh::FrontElement *>(test);

		if (!fetest)
		{
			Data::cout() << "could not convert front element " << test->text() << " to MJMesh::FrontElement, skipping front element" << Data::endl;

			consistent = false;

			continue;
		}

		if (!test->getShape())
		{
			Data::cout() << "front element " << test->text() << " has no shape, skipping front element" << Data::endl;

			consistent = false;

			continue;
		}

		if (test->getShape()->numPoints() != test->numVertices())
		{
			Data::cout() << "front element " << test->text() << " has shape with different number of points: " << test->getShape()->text() << ", skipping front element" << Data::endl;

			consistent = false;

			continue;
		}

		if (!fetest->isInMesh(this))
		{
			Data::cout() << "front element " << test->text() << " does not have mesh information" << Data::endl;

			consistent = false;
		}

		for (UInt i = 0; i < test->numVertices(); i++)
		{
			Data::Vertex *v = test->getVertex(i);

			if (!v)
			{
				Data::cout() << "front element " << test->text() << " has no " << i << "th vertex, skipping adjacent vertex" << Data::endl;

				consistent = false;

				continue;
			}

			MJMesh::Vertex *vv = dynamic_cast<MJMesh::Vertex *>(v);

			if (!vv)
			{
				Data::cout() << "front element " << test->text() << " could not convert " << i << "th vertex " << v->text() << " to MJMesh::Vertex, skipping adjacent vertex" << Data::endl;

				consistent = false;

				continue;
			}

			if (!this->have(v))
			{
				Data::cout() << "front element " << test->text() << " has vertex " << v->text() << " that is not in this mesh" << Data::endl;

				consistent = false;
			}

			//Data::FrontElementList adjs = vv->getAdjacentFrontElements();

			//if (std::find(adjs.begin(), adjs.end(), test) == adjs.end())
			if (!vv->have(test))
			{
				Data::cout() << "front element " << test->text() << " has " << i << "th vertex " << v->text() << " that does not have it as adjacent front element, skipping adjacent vertex" << Data::endl;

				consistent = false;
			}
		}

		Data::Mesh::ConstFrontElementIterator iter2 = iter;

		for (iter2++; iter2 != end; iter2++)
		{
			Data::FrontElement *other = (*iter2);

			if (test == other)
			{
				Data::cout() << "front element " << test->text() << " appears twice in mesh" << Data::endl;

				consistent = false;
			}

			if (other->getId() == test->getId())
			{
				Data::cout() << "front element " << test->text() << " has other with same id: " << other->text() << Data::endl;

				consistent = false;
			}

			if (other->getShape() == test->getShape())
			{
				Data::cout() << "front element " << test->text() << " and front element " << other->text() << " share the same shape" << Data::endl;

				consistent = false;
			}

			if ((other->getShape()) && (test->getShape()->match(*other->getShape())))
			{
				Data::cout() << "front element " << test->text() << " and front element " << other->text() << " have the same shape" << Data::endl;

				consistent = false;
			}

			if (test->numVertices() == other->numVertices())
			{
				bool allVertices = true, allPoints = true, allGeometricPoints = true;

				for (UInt i = 0; i < test->numVertices(); i++)
				{
					Data::Vertex *v = test->getVertex(i);

					bool foundVertex = false, foundPoint = false, foundGeometricPoint = false;

					for (UInt j = 0; j < other->numVertices(); j++)
					{
						Data::Vertex *ov = other->getVertex(j);

						if (v == ov)
						{
							foundVertex = true;
						}

						if (v->getPoint() == ov->getPoint())
						{
							foundPoint = true;
						}

						if (v->getPoint()->match(ov->getPoint()))
						{
							foundGeometricPoint = true;
						}
					}

					if (!foundVertex)
					{
						allVertices = false;
					}

					if (!foundPoint)
					{
						allPoints = false;
					}

					if (!foundGeometricPoint)
					{
						allGeometricPoints = false;
					}

					if ((!allVertices) && (!allPoints) && (!allGeometricPoints))
					{
						break;
					}
				}

				bool crack = false;

				if ((allVertices) || (allPoints) || (allGeometricPoints))
				{
					Real dot1 = 0.0, dot2 = 0.0;

					if (dimension == 2)
					{
						dot1 = static_cast<Data::Edge2D *>(test->getShape())->normal().dot(static_cast<Data::Edge2D *>(other->getShape())->normal());
						dot2 = static_cast<Data::Edge2D *>(test->getShape())->normal().dot(static_cast<Data::Edge2D *>(test->getShape())->normal());
					}
					else
					{
						dot1 = static_cast<Data::Polygon *>(test->getShape())->normal().dot(static_cast<Data::Polygon *>(other->getShape())->normal());
						dot2 = static_cast<Data::Polygon *>(test->getShape())->normal().dot(static_cast<Data::Polygon *>(test->getShape())->normal());
					}

					//dot1 and dot2 should be opposite (avoid normalization)
					if (std::fabs(dot1 + dot2) <= Data::getTolerance())
					{
						crack = true;
					}
				}

				if (crack)
				{
					Data::cout() << "front element " << test->text() << " and front element " << other->text() << " were detected as in a crack" << Data::endl;
				}
				else
				{
					if (allVertices)
					{
						Data::cout() << "front element " << test->text() << " and front element " << other->text() << " share the same vertices" << Data::endl;

						consistent = false;
					}

					if (allPoints)
					{
						Data::cout() << "front element " << test->text() << " and front element " << other->text() << " share the same points" << Data::endl;

						consistent = false;
					}

					if (allGeometricPoints)
					{
						Data::cout() << "front element " << test->text() << " and front element " << other->text() << " have the same points" << Data::endl;

						consistent = false;
					}
				}
			}
		}

		if (testElements)
		{
			Data::ElementList elements = fetest->getAdjacency();

			if (elements.size() == 0)
			{
				Data::cout() << "front element " << test->text() << " has no adjacent element" << Data::endl;

				consistent = false;
			}
			else if ((elements.size() == 1) || (elements.size() == 2))
			{
				bool inboundary = (boundary) ? boundary->have(test) : false;
				bool infront = (front) ? front->have(test) : false;

				if ((elements.size() == 1) && (!inboundary) && (!infront))
				{
					Data::cout() << "front element " << test->text() << " has only one adjacent element: " << elements.front()->text() << ", but is neither in boundary nor in front" << Data::endl;

					consistent = false;
				}

				if (elements.size() == 2)
				{
					if (inboundary)
					{
						Data::cout() << "front element " << test->text() << " has two adjacent elements: " << elements.front()->text() << " and " << elements.back()->text() << ", but is in boundary" << Data::endl;

						consistent = false;
					}

					if (infront)
					{
						Data::cout() << "front element " << test->text() << " has two adjacent elements: " << elements.front()->text() << " and " << elements.back()->text() << ", but is in front" << Data::endl;

						consistent = false;
					}
				}
			}
			else if (elements.size() > 2)
			{
				Data::cout() << "front element " << test->text() << " has more than 2 adjacent elements" << Data::endl;

				consistent = false;
			}

			while (!elements.empty())
			{
				Data::Element *e = elements.front();

				elements.pop_front();

				if (!this->have(e))
				{
					Data::cout() << "front element " << test->text() << " has element " << e->text() << " that is not in this mesh" << Data::endl;

					consistent = false;
				}

				UInt count = 0;

				for (UInt i = 0; i < test->numVertices(); i++)
				{
					for (UInt j = 0; j < e->numVertices(); j++)
					{
						if (test->getVertex(i) == e->getVertex(j))
						{
							count++;
						}
					}
				}

				if (count != test->numVertices())
				{
					Data::cout() << "front element " << test->text() << " and element " << e->text() << " share " << count << " vertices, instead of " << test->numVertices() << Data::endl;

					consistent = false;
				}
			}
		}
	}

	count = 0;
	tstart = tend = time = tstep = 0.0;
	size = this->size();

	for (Data::Mesh::ConstElementIterator iter = this->eBegin(), end = this->eEnd();
		 iter != end; iter++)
	{
		if (((++count)%500 == 0) || (count == 1) || (count == size))
		{
			Data::cout() << "testing element " << count << " of " << size
					  << ", timings: current: " << Data::time(tstep)
					  << ", total: " << Data::time(total);

			if (time > 0.0)
			{
				Data::cout() << ", estimated: " << Data::time(time*static_cast<Real>(size - count)/2.0);
			}

			Data::cout() << " (hh:mm:ss)" << Data::endl;
		}

		tend = Data::time();

		if (tstart > 0.0)
		{
			time = tend - tstart;
			tstep += time;
			total += time;
		}

		tstart = tend;

		Data::Element *test = (*iter);

		MJMesh::Element *etest = dynamic_cast<MJMesh::Element *>(test);

		if (!etest)
		{
			Data::cout() << "could not convert element " << test->text() << " to MJMesh::Element, skipping element" << Data::endl;

			consistent = false;

			continue;
		}

		if (!test->getShape())
		{
			Data::cout() << "element " << test->text() << " has no shape, skipping element" << Data::endl;

			consistent = false;

			continue;
		}

		if (test->getShape()->numPoints() != test->numVertices())
		{
			Data::cout() << "element " << test->text() << " has shape with different number of points: " << test->getShape()->text() << ", skipping element" << Data::endl;

			consistent = false;

			continue;
		}

		if (!etest->isInMesh(this))
		{
			Data::cout() << "element " << test->text() << " does not have mesh information" << Data::endl;

			consistent = false;
		}

		for (UInt i = 0; i < test->numVertices(); i++)
		{
			Data::Vertex *v = test->getVertex(i);

			if (!v)
			{
				Data::cout() << "element " << test->text() << " has no " << i << "th vertex, skipping adjacent vertex" << Data::endl;

				consistent = false;

				continue;
			}

			MJMesh::Vertex *vv = dynamic_cast<MJMesh::Vertex *>(v);

			if (!vv)
			{
				Data::cout() << "element " << test->text() << " could not convert " << i << "th vertex " << v->text() << " to MJMesh::Vertex, skipping adjacent vertex" << Data::endl;

				consistent = false;

				continue;
			}

			if (!this->have(v))
			{
				Data::cout() << "element " << test->text() << " has vertex " << v->text() << " that is not in this mesh" << Data::endl;

				consistent = false;
			}

			//Data::ElementList adjs = vv->getAdjacentElements();

			//if (std::find(adjs.begin(), adjs.end(), test) == adjs.end())
			if (!vv->have(test))
			{
				Data::cout() << "element " << test->text() << " has " << i << "th vertex " << v->text() << " that does not have it as adjacent element, skipping adjacent vertex" << Data::endl;

				consistent = false;
			}
		}

		Data::Mesh::ConstElementIterator iter2 = iter;

		for (iter2++; iter2 != end; iter2++)
		{
			Data::Element *other = (*iter2);

			if (test == other)
			{
				Data::cout() << "element " << test->text() << " appears twice in mesh" << Data::endl;

				consistent = false;
			}

			if (other->getId() == test->getId())
			{
				Data::cout() << "element " << test->text() << " has other with same id: " << other->text() << Data::endl;

				consistent = false;
			}

			if (other->getShape() == test->getShape())
			{
				Data::cout() << "element " << test->text() << " and element " << other->text() << " share the same shape" << Data::endl;

				consistent = false;
			}

			if ((other->getShape()) && (test->getShape()->match(*other->getShape())))
			{
				Data::cout() << "element " << test->text() << " and element " << other->text() << " have the same shape" << Data::endl;

				consistent = false;
			}
		}
	}

	Data::cout() << "ended testing mesh " << this->getId() << Data::endl;

	return consistent;
}

void MJMesh::Mesh::oldTest(const MJMesh::Boundary *boundary, const MJMesh::Front *front) const
{
	Data::cout() << "testing mesh\n";

	UInt dimension = this->dimension();

	bool noInternalFels = false;

	{
		UInt bsize = (boundary) ? boundary->size() : 0;
		UInt fsize = (front) ? front->size() : 0;

		noInternalFels = (this->frontElementsSize() == (bsize + fsize));
	}

	//Data::VertexList vertices = this->getVertices();

	//while (!vertices.empty())
	for (Data::Mesh::ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		//Data::Vertex *v = vertices.front();
		Data::Vertex *v = (*iter);

		//vertices.pop_front();

		//while (true)
		{
			//Data::VertexList::iterator iter = vertices.begin();
			Data::Mesh::ConstVertexIterator iter2 = iter;

			//for (; iter != vertices.end(); iter++)
			for (iter2++; iter2 != this->vertices.end(); iter2++)
			{
				//if ((*iter) == v)
				if ((*iter2) == v)
				{
					Data::cout() << "repeated vertex: " << v->text() << "\n";

					//break;
				}

				//if ((*iter)->getId() == v->getId())
				if ((*iter2)->getId() == v->getId())
				{
					Data::cout() << "repeated vertex id: " << v->text() << " and " << (*iter2)->text() << "\n";

					//break;
				}

				//if ((*iter)->getPoint()->match(v->getPoint()))
				if ((*iter2)->getPoint()->match(v->getPoint()))
				{
					Data::cout() << "matched vertex: " << v->text() << " and " << (*iter2)->text() << "\n";
				}
			}
		}

		Data::ElementList els = this->adjacency(v);
		Data::FrontElementList fels = this->findAdjacentFrontElements(v);

		bool inconsistent = false;

		if (noInternalFels)
		{
			if (!fels.empty())
			{
				bool bhave = (boundary) ? boundary->have(v) : false;
				bool fhave = (front) ? front->have(v) : false;

				if ((!bhave) && (!fhave))
				{
					inconsistent = true;

					Data::cout() << "!fels.empty(); bhave: " << std::boolalpha << bhave << "; fhave: " << std::boolalpha << fhave << Data::endl;
				}
			}
		}
		else if (dimension == 2)
		{
			if (els.size() != fels.size())
			{
				if (!boundary)
				{
					inconsistent = true;

					Data::cout() << "(els.size() != fels.size()); (els.size() == fels.size() - 1); (!boundary)" << Data::endl;
				}
				else if (!boundary->have(v))
				{
					if ((!front) || (!front->have(v)))
					{
						inconsistent = true;

						Data::cout() << "(els.size() != fels.size()); (els.size() == fels.size() - 1); (!boundary->have(v)); ((!front) || (!front->have(v)))" << Data::endl;
					}
				}
			}
		}

		if (inconsistent)
		{
			Data::cout() << "inconsistent vertex: " << v->text() << "\n";

			Data::cout() << "adjacent elements:";

			while (!els.empty())
			{
				Data::Element *e = els.front();

				els.pop_front();

				Data::cout() << "\n" << e->text();
			}

			Data::cout() << "\n";

			Data::cout() << "adjacent front elements:";

			while (!fels.empty())
			{
				Data::FrontElement *fel = fels.front();

				fels.pop_front();

				Data::cout() << "\n" << fel->text();
			}

			Data::cout() << "\n";
		}
	}

	//Data::FrontElementList fels = this->getFrontElements();

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		//Data::FrontElement *fel = fels.front();
		Data::FrontElement *fel = (*iter);

		//fels.pop_front();

		//Data::FrontElementList::iterator iter = fels.begin();
		Data::Mesh::ConstFrontElementIterator iter2 = iter;

		//while (true)
		{
			//for (; iter != fels.end(); iter++)
			for (iter2++; iter2 != this->frontElements.end(); iter2++)
			{
				if ((*iter2) == fel)
				{
					Data::cout() << "repeated front element: " << fel->text() << "\n";

					//break;
				}

				if ((*iter2)->getId() == fel->getId())
				{
					Data::cout() << "repeated front element id: " << fel->text() << " and " << (*iter2)->text() << "\n";

					//break;
				}
			}
		}

		Data::ElementList els = this->adjacency(fel);

		bool inconsistent = false;

		if (els.size() == 0)
		{
			if (!front)
			{
				inconsistent = true;
			}
			else
			{
				//Data::FrontElementList fels = front->getAllElements();

				//if (it == fels.end())
				if (!front->have(fel))
				{
					inconsistent = true;
				}
			}
		}
		else if (els.size() > 2)
		{
			inconsistent = true;
		}
		else if (els.size() == 1)
		{
			if (!boundary)
			{
				inconsistent = true;
			}
			else if (!boundary->have(fel))
			{
				if (!front)
				{
					inconsistent = true;
				}
				else
				{
					//Data::FrontElementList fels = front->getAllElements();

					//if (it == fels.end())
					if (!front->have(fel))
					{
						inconsistent = true;
					}
				}
			}
		}

		if (inconsistent)
		{
			Data::cout() << "inconsistent front element: " << fel->text() << "\n";
		}
	}

	for (Data::Mesh::ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		Data::Element *el = (*iter);

		Data::Mesh::ConstElementIterator iter2 = iter;

		for (iter2++; iter2 != this->elements.end(); iter2++)
		{
			if ((*iter2) == el)
			{
				Data::cout() << "repeated element: " << el->text() << "\n";
			}

			if ((*iter2)->getId() == el->getId())
			{
				Data::cout() << "repeated element id: " << el->text() << " and " << (*iter2)->text() << "\n";
			}
		}
	}

	Data::cout() << "ended testing mesh" << Data::endl;
}*/

void MJMesh::Mesh::print() const
{
	Data::cerr() << "Mesh is:\n\n";
	Data::cerr() << "Vertices:\n";

	//Data::VertexList vertices = this->getVertices();

	//while (!vertices.empty())
	for (Data::Mesh::ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		//MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(vertices.front());
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		//vertices.pop_front();

		Data::cerr() << "Vertex " << v->getId() << " is located at (" << v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ")\n";

		Data::FrontElementList adjFels = v->getAdjacentFrontElements();

		Data::cerr() << "Vertex " << v->getId() << " has " << adjFels.size() << " adjacent front elements:";

		while (!adjFels.empty())
		{
			Data::FrontElement *fel = adjFels.front();

			Data::cerr() << " " << fel->getId();

			adjFels.pop_front();
		}

		Data::cerr() << "\n";

		Data::ElementList adjEls = v->getAdjacentElements();

		Data::cerr() << "Vertex " << v->getId() << " has " << adjEls.size() << " adjacent elements:";

		while (!adjEls.empty())
		{
			Data::Element *e = adjEls.front();

			Data::cerr() << " " << e->getId();

			adjEls.pop_front();
		}

		Data::cerr() << "\n";

		Data::cerr() << "Vertex " << v->getId() << " has " << v->getAdjacentElements().size() << " adjacent edges\n";
	}

	Data::cerr() << "\n";

	//Data::FrontElementList fels = this->getFrontElements();

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		//MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

		//fels.pop_front();

		Data::cerr() << "Front element " << fel->getId() << " has vertices";

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::cerr() << " " << fel->getVertex(i)->getId();
		}

		Data::cerr() << "\n";

		Data::ElementList adjEls = fel->getAdjacency();

		Data::cerr() << "Front element " << fel->getId() << " has " << adjEls.size() << " adjacent elements:";

		while (!adjEls.empty())
		{
			Data::Element *e = adjEls.front();

			Data::cerr() << " " << e->getId();

			adjEls.pop_front();
		}

		Data::cerr() << "\n";
	}

	//Data::ElementList els = this->getElements();

	//while (!els.empty())
	for (Data::Mesh::ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		//MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
		MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

		//els.pop_front();

		Data::cerr() << "Element " << e->getId() << " has vertices";

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			Data::cerr() << " " << e->getVertex(i)->getId();
		}

		Data::cerr() << "\n";
	}

	Data::cerr() << Data::endl;
}
//endebug*/
#endif //#if USE_GUI

void MJMesh::Mesh::forceRemove(Data::Vertex *v)
{
	VertexSpecificData::MeshSpecificData msp = static_cast<MJMesh::Vertex *>(v)->getAndRemoveMeshData(this);

	this->vertices.erase(msp.meshIt);

	if (msp.inner)
	{
#if USE_EXPERIMENTAL_MESH
		--this->numInnerVertices;
#else
		this->innerVertices.erase(msp.innerMeshIt);
#endif //#if USE_EXPERIMENTAL_MESH
	}
}

void MJMesh::Mesh::forceRemoveInner(Data::Vertex *v)
{
#if USE_EXPERIMENTAL_MESH
	static_cast<MJMesh::Vertex *>(v)->setMeshInner(this, false);

	--this->numInnerVertices;
#else
    Data::Mesh::VertexIterator it = static_cast<MJMesh::Vertex *>(v)->getMeshInnerIterator(this);

	static_cast<MJMesh::Vertex *>(v)->setMeshInner(this, false, Data::Mesh::VertexIterator());

	this->innerVertices.erase(it);
#endif //#if USE_EXPERIMENTAL_MESH
}

void MJMesh::Mesh::forceRemove(Data::FrontElement *e)
{
	this->frontElements.erase(static_cast<MJMesh::FrontElement *>(e)->getAndRemoveMeshIterator(this));
}

void MJMesh::Mesh::forceRemove(Data::Element *e)
{
	this->elements.erase(static_cast<MJMesh::Element *>(e)->getAndRemoveMeshIterator(this));
}

void MJMesh::Mesh::clearVertices() const
{
	for (Data::Mesh::ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		static_cast<MJMesh::Vertex *>((*iter))->removeMeshData(this);
	}
}

void MJMesh::Mesh::clearFrontElements() const
{
	for (Data::Mesh::ConstFrontElementIterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		static_cast<MJMesh::FrontElement *>((*iter))->removeMeshData(this);
	}
}

void MJMesh::Mesh::clearElements() const
{
	for (Data::Mesh::ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		static_cast<MJMesh::Element *>((*iter))->removeMeshData(this);
	}
}
