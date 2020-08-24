#include "Performer/MeshMerger.h"

#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "Performer/IdManager.h"

Performer::MeshMerger::MeshMerger() :
	Performer::Builder::Builder()
{
	this->shouldMakeBoundary = false;
	this->keepOldIds = false;

	this->boundary = NULL;

	this->mesh = NULL;

	this->idManager = NULL;
}

Performer::MeshMerger::~MeshMerger()
{

}

void Performer::MeshMerger::setShouldMakeBoundary(bool shouldMakeBoundary)
{
	this->shouldMakeBoundary = shouldMakeBoundary;
}

void Performer::MeshMerger::setKeepOldIds(bool keepOldIds)
{
    this->keepOldIds = keepOldIds;
}

void Performer::MeshMerger::setMeshes(const Data::MeshList &meshes)
{
	this->meshes = meshes;
}

void Performer::MeshMerger::setIdManager(IdManager *idManager)
{
	this->idManager = idManager;
}

Data::Boundary *Performer::MeshMerger::getBoundary() const
{
	return this->boundary;
}

Data::Mesh *Performer::MeshMerger::getMesh() const
{
	return this->mesh;
}

void Performer::MeshMerger::merge(const Data::Mesh *mesh)
{
	if (!mesh)
	{
		return;
	}

	VMap vMap;
	FEMap feMap;
	EMap eMap;

	VertexIdMap idVMap;
	FrontElementIdMap idFEMap;
	ElementIdMap idEMap;

#if USE_C__11
	vMap.reserve(mesh->verticesSize());
	feMap.reserve(mesh->frontElementsSize());
	eMap.reserve(mesh->size());

	if (this->keepOldIds)
    {
        idVMap.reserve(mesh->verticesSize());
        idFEMap.reserve(mesh->frontElementsSize());
        idEMap.reserve(mesh->size());
    }
#endif //#if USE_C__11

	for (Data::Mesh::ConstVertexIterator iter = mesh->vBegin(), end = mesh->vEnd();
		 iter != end; iter++)
	{
		Data::Vertex *v = this->makeVertex((*iter), vMap);

#if USE_GUI
		v->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

        if (this->keepOldIds)
        {
            idVMap[v] = v->getId();
        }

		v->setId(this->idManager->next(0));

		vMap[(*iter)->getId()] = v;

		bool inner = mesh->haveInner((*iter));

		this->mesh->add(v, inner);

		if ((!inner) && (this->shouldMakeBoundary))
		{
			this->boundary->add(v);
		}
	}

	for (Data::Mesh::ConstFrontElementIterator iter = mesh->feBegin(), end = mesh->feEnd();
		 iter != end; iter++)
	{
		Data::FrontElement *fe = this->makeFrontElement((*iter), vMap, feMap);

#if USE_GUI
		fe->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

        if (this->keepOldIds)
        {
            idFEMap[fe] = fe->getId();
        }

		fe->setId(this->idManager->next(1));

		feMap[(*iter)->getId()] = fe;

		this->mesh->add(fe);

		if ((this->shouldMakeBoundary) && ((mesh->adjacency((*iter)).size() <= 1)))
		{
			this->boundary->add(fe);
		}
	}

	for (Data::Mesh::ConstElementIterator iter = mesh->eBegin(), end = mesh->eEnd();
		 iter != end; iter++)
	{
		Data::Element *e = this->makeElement((*iter), vMap, feMap, eMap);

#if USE_GUI
		e->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

		if (this->keepOldIds)
        {
            idEMap[e] = e->getId();
        }

		e->setId(this->idManager->next(2));

		eMap[(*iter)->getId()] = e;

		this->mesh->add(e);
	}

	if (this->keepOldIds)
    {
        for (VertexIdMap::iterator iter = idVMap.begin();
             iter != idVMap.end(); iter++)
        {
            (*iter).first->setId((*iter).second);
        }

        for (FrontElementIdMap::iterator iter = idFEMap.begin();
             iter != idFEMap.end(); iter++)
        {
            (*iter).first->setId((*iter).second);
        }

        for (ElementIdMap::iterator iter = idEMap.begin();
             iter != idEMap.end(); iter++)
        {
            (*iter).first->setId((*iter).second);
        }
    }
}

void Performer::MeshMerger::execute()
{
	if (this->meshes.empty())
	{
		return;
	}

	if (this->shouldMakeBoundary)
	{
		this->boundary = this->makeBoundary();
	}

	this->mesh = this->makeMesh();

#if USE_GUI
	Int i = 0;
#endif //#if USE_GUI

	for (Data::MeshList::const_iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
#if USE_GUI
		Data::pallete(i++, this->r, this->g, this->b);
#endif //#if USE_GUI

		this->merge((*iter));
	}
}
