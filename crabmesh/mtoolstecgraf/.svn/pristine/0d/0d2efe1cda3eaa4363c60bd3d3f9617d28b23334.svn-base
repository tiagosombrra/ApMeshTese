#include "Performer/MeshSmoother.h"

#include "Data/Vertex.h"
#include "Data/Mesh.h"

Performer::MeshSmoother::MeshSmoother(Data::Mesh *mesh) :
	MeshOptimizer(mesh)
{
	//this->currVertex = this->vertices.end();
	if (this->mesh)
	{
		this->currVertex = this->mesh->ivEnd();
	}
}

Performer::MeshSmoother::~MeshSmoother()
{

}

void Performer::MeshSmoother::execute()
{
#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	switch (this->status.getType())
	{
	case MeshSmoother::NOT_INIT:
		if (!this->mesh)
		{
			this->status = MeshSmoother::ended;

			return;
		}

		//this->vertices = this->mesh->getInnerVertices(true);
		//this->vertices = this->mesh->getSortedInnerVertices();
		//this->vertices = this->mesh->getInnerVertices();
		//this->vertices.sort(Data::Identifiable::less);

		//this->currVertex = this->vertices.end();
		this->currVertex = this->mesh->ivEnd();

		this->status = MeshSmoother::init;

	case MeshSmoother::INIT:
		//if (this->currVertex == this->vertices.end())
		if (this->currVertex == this->mesh->ivEnd())
		{
			//this->currVertex = this->vertices.begin();
			this->currVertex = this->mesh->ivBegin();
		}

		//while (this->currVertex != this->vertices.end())
		while (this->currVertex != this->mesh->ivEnd())
		{
			this->smooth((*this->currVertex));

			++this->currVertex;

#if USE_GUI
			if (Data::isStepByStep()) break;
#endif //#if USE_GUI
		}

		//if (this->currVertex == this->vertices.end())
		if (this->currVertex == this->mesh->ivEnd())
		{
			this->status = MeshSmoother::ended;
		}

	case MeshSmoother::ENDED:
		break;
	}

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	/*the above swicth replaces these commented lines
	if (!this->mesh)
	{
		return;
	}

	VertexList vertices = this->mesh->getInnerVertices();

	for (VertexList::iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		this->smooth((*iter));
	}*/
}
