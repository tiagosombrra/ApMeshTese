#include "Tools/MeshTopologyChecker.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"
#include "MJMesh/Mesh.h"
#if USE_TECGRAF
#include "TecGraf/Model.h"
#endif //#if USE_TECGRAF

Tools::MeshTopologyChecker::MeshTopologyChecker() :
	Performer::MeshTopologyChecker::MeshTopologyChecker(),
	Tools::Tool::Tool()
{
	this->mjmesh = NULL;

#if USE_TECGRAF
	this->model = NULL;
#endif //#if USE_TECGRAF
}

Tools::MeshTopologyChecker::~MeshTopologyChecker()
{

}

#if USE_TECGRAF
void Tools::MeshTopologyChecker::setModel(const TecGraf::Model *model)
{
	this->model = model;
}
#endif //#if USE_TECGRAF

void Tools::MeshTopologyChecker::execute()
{
	if (!this->mesh)
	{
#if USE_TECGRAF
		if (!this->model)
		{
			return;
		}

		this->mesh = new MJMesh::Mesh();
		this->boundary = new MJMesh::Boundary();
		this->front = NULL;

		this->model->to(const_cast<Data::Mesh *>(this->mesh), const_cast<Data::Boundary *>(this->boundary));
#else
		return;
#endif
	}
#if USE_TECGRAF
	else
	{
		this->model = NULL;
	}
#endif //#if USE_TECGRAF

    this->surface = false;

	this->mjmesh = static_cast<const MJMesh::Mesh *>(this->mesh);

	if (this->mjmesh)
    {
        this->surface = this->mjmesh->isSurface();
    }

	this->setMeasure(0, static_cast<UInt>(this->mesh->verticesSize() + this->mesh->frontElementsSize() + this->mesh->size() + 1));

	Performer::MeshTopologyChecker::execute();

#if USE_TECGRAF
	if (this->model)
	{
		const_cast<Data::Boundary *>(this->boundary)->clear();

		const_cast<Data::Mesh *>(this->mesh)->clear();

		delete this->mesh;
		this->mesh = NULL;

		delete this->boundary;
		this->boundary = NULL;

		this->mjmesh = NULL;
	}
#endif //#if USE_TECGRAF

	this->endMeasure();
}

bool Tools::MeshTopologyChecker::isTestingAngles() const
{
    return !this->surface;
}

//bool Tools::MeshTopologyChecker::check(const Data::Mesh::ConstVertexIterator &iter, const Data::Mesh::ConstVertexIterator &end) const
bool Tools::MeshTopologyChecker::check(const Data::Vertex *test) const
{
	this->addMeasure(1, 0);

	bool ok = true;

	if (this->mjmesh)
	{
		//Data::Vertex *test = (*iter);

		const MJMesh::Vertex *vtest = dynamic_cast<const MJMesh::Vertex *>(test);

		if (!vtest)
		{
			std::cout << "could not convert vertex " << test->text() << " to MJMesh::Vertex, skipping vertex" << std::endl;

			this->consistent = false;

			ok = false;

			return ok;
		}

		if (!vtest->isInMesh(this->mjmesh))
		{
			std::cout << "vertex " << test->text() << " does not have mesh information" << std::endl;

			this->consistent = false;

			ok = false;
		}
		else if (this->boundary)
		{
			bool inner = vtest->isMeshInner(this->mjmesh);
			bool bhave = this->boundary->have(vtest);

			if ((inner) && (bhave))
			{
				std::cout << "vertex " << test->text() << " is marked as inner but is also in boundary" << std::endl;

				this->consistent = false;

                ok = false;
			}

			if ((!inner) && (!bhave))
			{
				std::cout << "vertex " << test->text() << " is marked as outer but is not in boundary" << std::endl;

				this->consistent = false;

                ok = false;
			}
		}
	}

	//if (!Performer::MeshTopologyChecker::check(iter, end))
	if (!Performer::MeshTopologyChecker::check(test))
    {
        ok = false;
    }

    return ok;
}

//bool Tools::MeshTopologyChecker::check(const Data::Mesh::ConstFrontElementIterator &iter, const Data::Mesh::ConstFrontElementIterator &end) const
bool Tools::MeshTopologyChecker::check(const Data::FrontElement *test) const
{
	this->addMeasure(1, 0);

	bool ok = true;

	if (this->mjmesh)
	{
		//Data::FrontElement *test = (*iter);

		const MJMesh::FrontElement *fetest = dynamic_cast<const MJMesh::FrontElement *>(test);

		if (!fetest)
		{
			std::cout << "could not convert front element " << test->text() << " to MJMesh::FrontElement, skipping front element" << std::endl;

			this->consistent = false;

			ok = false;

			return ok;
		}

		if (!fetest->isInMesh(this->mjmesh))
		{
			std::cout << "front element " << test->text() << " does not have mesh information" << std::endl;

			this->consistent = false;

			ok = false;
		}
	}

	//if (!Performer::MeshTopologyChecker::check(iter, end))
	if (!Performer::MeshTopologyChecker::check(test))
    {
        ok = false;
    }

    return ok;
}

//bool Tools::MeshTopologyChecker::check(const Data::Mesh::ConstElementIterator &iter, const Data::Mesh::ConstElementIterator &end) const
bool Tools::MeshTopologyChecker::check(const Data::Element *test) const
{
	this->addMeasure(1, 0);

	bool ok = true;

	if (this->mjmesh)
	{
		//Data::Element *test = (*iter);

		const MJMesh::Element *etest = dynamic_cast<const MJMesh::Element *>(test);

		if (!etest)
		{
			std::cout << "could not convert element " << test->text() << " to MJMesh::Element, skipping element" << std::endl;

			this->consistent = false;

			ok = false;

			return ok;
		}

		if (!etest->isInMesh(this->mjmesh))
		{
			std::cout << "element " << test->text() << " does not have mesh information" << std::endl;

			this->consistent = false;

			ok = false;
		}
	}

	if (!Performer::MeshTopologyChecker::check(test))
    {
        ok = false;
    }

    return ok;
}

bool Tools::MeshTopologyChecker::check(const Data::FrontElement *test, UInt i) const
{
	bool ok = Performer::MeshTopologyChecker::check(test, i);

	if (this->mjmesh)
	{
		Data::Vertex *v = test->getVertex(i);

		MJMesh::Vertex *vv = dynamic_cast<MJMesh::Vertex *>(v);

		if ((v) && (!vv))
		{
			std::cout << "front element " << test->text() << " could not convert " << i << "th vertex " << v->text() << " to MJMesh::Vertex, skipping adjacent vertex" << std::endl;

			this->consistent = false;

			return false;
		}

		if (!vv->have(const_cast<Data::FrontElement *>(test)))
		{
			std::cout << "front element " << test->text() << " has " << i << "th vertex " << v->text() << " that does not have it as adjacent front element, skipping adjacent vertex" << std::endl;

			this->consistent = false;

			ok = false;
		}
	}

	return ok;
}

bool Tools::MeshTopologyChecker::check(const Data::Element *test, UInt i) const
{
	bool ok = Performer::MeshTopologyChecker::check(test, i);

	if (this->mjmesh)
	{
		Data::Vertex *v = test->getVertex(i);

		MJMesh::Vertex *vv = dynamic_cast<MJMesh::Vertex *>(v);

		if ((v) && (!vv))
		{
			std::cout << "element " << test->text() << " could not convert " << i << "th vertex " << v->text() << " to MJMesh::Vertex, skipping adjacent vertex" << std::endl;

			this->consistent = false;

			return false;
		}

		if (!vv->have(const_cast<Data::Element *>(test)))
		{
			std::cout << "element " << test->text() << " has " << i << "th vertex " << v->text() << " that does not have it as adjacent element, skipping adjacent vertex" << std::endl;

			this->consistent = false;

			ok = false;
		}
	}

	return ok;
}
