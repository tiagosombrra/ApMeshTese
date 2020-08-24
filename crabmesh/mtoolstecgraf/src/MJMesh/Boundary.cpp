#include "MJMesh/Boundary.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "MJMesh/Mesh.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"

#include "Data/OStream.h"

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

MJMesh::Boundary::Boundary(ULInt id) :
	Data::Boundary(true, id)
{

}

MJMesh::Boundary::Boundary(MJMesh::Boundary *boundary) :
	Data::Boundary(0)
{
	//UInt dim = boundary->vertices[0]->getPoint()->dimension();
	UInt dim = boundary->vertices.front()->getPoint()->dimension();

	Data::VertexMap verticesMap;

	//for (Data::VertexVector::iterator iter = boundary->vertices.begin();
	for (Data::VertexList::iterator iter = boundary->vertices.begin();
		 iter != boundary->vertices.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(*iter);
		Data::Point *p = v->getPoint();

		Data::Point *newp = (dim == 2) ?
			static_cast<Data::Point *>(new Data::Point2D(p)) : static_cast<Data::Point *>(new Data::Point3D(p));
		MJMesh::Vertex *newv = new MJMesh::Vertex(newp, v->getId());

		this->add(newv);

		verticesMap[newv->getId()] = newv;
	}

	//this->vertices = boundary->vertices;

	//for (Data::FrontElementVector::iterator iter = boundary->elements.begin();
	for (Data::FrontElementList::iterator iter = boundary->elements.begin();
		 iter != boundary->elements.end(); iter++)
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(*iter);

		MJMesh::FrontElement *newfel = (dim == 2) ?
			static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement2D(
				static_cast<MJMesh::Vertex *>(verticesMap[fel->getVertex(0)->getId()]),
				static_cast<MJMesh::Vertex *>(verticesMap[fel->getVertex(1)->getId()]), fel->getId())) :
			static_cast<MJMesh::FrontElement *>(new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(verticesMap[fel->getVertex(0)->getId()]),
				static_cast<MJMesh::Vertex *>(verticesMap[fel->getVertex(1)->getId()]),
				static_cast<MJMesh::Vertex *>(verticesMap[fel->getVertex(2)->getId()]), fel->getId()));

		this->add(newfel);
	}

	//this->elements = boundary->elements;

	//this->verticesMap = boundary->verticesMap;
	//this->elementsMap = boundary->elementsMap;
	this->sorted = boundary->sorted;

	this->id = boundary->id;

#if USE_GUI
	this->r = boundary->r;
	this->g = boundary->g;
	this->b = boundary->b;

	this->highlighted = boundary->highlighted;
#endif //#if USE_GUI
}

MJMesh::Boundary::~Boundary()
{

}

void MJMesh::Boundary::setVertices(const Data::VertexList &vertices)
{
	this->clearVertices();

	this->vertices = vertices;

	//this->clearVerticesMap();

	for (Data::Boundary::VertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		static_cast<MJMesh::Vertex *>((*iter))->addBoundaryData(this, iter);
		//this->verticesMap[(*iter)->getId()] = (*iter);

		/*if ((*iter)->getId() == 41)
		{
			Data::cout() << "MJMesh::Boundary::setVertices " << (*iter) << " " << (*iter)->text() << " in boundary " << this << std::endl << Data::trace() << Data::endl;
		}*/
	}
}

void MJMesh::Boundary::setElements(const Data::FrontElementList &elements)
{
	this->clearElements();

	this->elements = elements;

	//this->clearElementsMap();

	for (Data::Boundary::FrontElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		static_cast<MJMesh::FrontElement *>((*iter))->addBoundaryData(this, iter);
		//this->elementsMap[(*iter)->getId()] = (*iter);

		/*if ((*iter)->getId() == 40)
		{
			Data::cout() << "MJMesh::Boundary::setElements " << (*iter) << " " << (*iter)->text() << " in boundary " << this << std::endl << Data::trace() << Data::endl;

			if (static_cast<MJMesh::FrontElement2D *>((*iter))->numAdjacents() != 1)
            {
                Data::cout() << Data::trace() << Data::endl;
            }
		}*/
	}
}

bool MJMesh::Boundary::isSurfaceBoundary() const
{
    if (this->dimension() != 3)
    {
        return false;
    }

    if (this->size() == 0)
    {
        return false;
    }

    return ((*this->eBegin())->numVertices() == 2);
}

void MJMesh::Boundary::add(Data::Vertex *v)
{
	Data::Boundary::add(v);

	static_cast<MJMesh::Vertex *>(v)->addBoundaryData(this, --this->vertices.end());

	/*if (v->getId() == 41)
	{
		Data::cout() << "MJMesh::Boundary::add " << v << " " << v->text() << " to boundary " << this << std::endl << Data::trace() << Data::endl;
	}*/
}

void MJMesh::Boundary::remove(Data::Vertex *v)
{
	/*static_cast<MJMesh::Vertex *>(v)->removeBoundaryData(this);

	Data::Boundary::remove(v);*/

	this->vertices.erase(static_cast<MJMesh::Vertex *>(v)->getAndRemoveBoundaryData(this));

	/*if (v->getId() == 41)
	{
		Data::cout() << "MJMesh::Boundary::remove " << v << " " << v->text() << " from boundary " << this << std::endl << Data::trace() << Data::endl;
	}*/
}

void MJMesh::Boundary::add(Data::FrontElement *e)
{
	//bool debug = this->have(e);

	Data::Boundary::add(e);

	static_cast<MJMesh::FrontElement *>(e)->addBoundaryData(this, --this->elements.end());

	/*if (e->getId() == 40)
	{
		Data::cout() << "MJMesh::Boundary::add " << e << " " << e->text() << " to boundary " << this << std::endl << Data::trace() << Data::endl;

		if ((debug) || (static_cast<MJMesh::FrontElement2D *>(e)->numAdjacents() != 1))
        {
            if (debug) Data::cout() << "already here!!" << Data::endl;

            Data::cout() << Data::trace() << Data::endl;
        }
	}*/
}

void MJMesh::Boundary::remove(Data::FrontElement *e)
{
	/*static_cast<MJMesh::FrontElement *>(e)->removeBoundaryData(this);

	Data::Boundary::remove(e);*/

    /*if (e->getId() == 272)
	{
		Data::cout() << "MJMesh::Boundary::remove " << e << " " << e->text() << " from boundary " << this << std::endl << Data::trace() << Data::endl;
	}*/

	this->elements.erase(static_cast<MJMesh::FrontElement *>(e)->getAndRemoveBoundaryData(this));
}

Data::FrontElementList MJMesh::Boundary::adjacency(const Data::Vertex *v) const
{
	/*if (this->elementsMap.empty())
	{
		return Data::Boundary::adjacency(v);
	}*/

	//Data::FrontElementList adjs = static_cast<const MJMesh::Vertex *>(v)->getAdjacentFrontElements();

	Data::FrontElementList adjacency;

	//for (Data::FrontElementList::const_iterator iter = adjs.begin();
	for (Data::FrontElementList::const_iterator iter = static_cast<const MJMesh::Vertex *>(v)->feBegin();
		 iter != static_cast<const MJMesh::Vertex *>(v)->feEnd(); iter++)
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

		//if (fel->getAdjacency().size() == 1)
		if (this->have(fel))
		{
			adjacency.push_back(fel);
		}
	}

	return adjacency;
}

bool MJMesh::Boundary::have(const Data::Vertex *v, bool matchingCoords) const
{
	if (static_cast<const MJMesh::Vertex *>(v)->isInBoundary(this))
	{
		return true;
	}

	if (!matchingCoords)
	{
		return false;
	}

	return Data::Boundary::have(v, matchingCoords);
}

bool MJMesh::Boundary::have(const Data::FrontElement *e, bool matchingCoords) const
{
	if (static_cast<const MJMesh::FrontElement *>(e)->isInBoundary(this))
	{
		return true;
	}

	if (!matchingCoords)
	{
		return false;
	}

	return Data::Boundary::have(e, matchingCoords);
}

void MJMesh::Boundary::clear()
{
	this->clearVertices();

	this->clearElements();

	Data::Boundary::clear();
}

Data::Boundary::VertexIterator MJMesh::Boundary::erase(const Data::Boundary::VertexIterator &iter)
{
	/*if ((*iter)->getId() == 41)
	{
		Data::cout() << "MJMesh::Boundary::erase " << (*iter) << " " << (*iter)->text() << " from boundary " << this << std::endl << Data::trace() << Data::endl;
	}*/

	static_cast<MJMesh::Vertex *>((*iter))->removeBoundaryData(this);

	return Data::Boundary::erase(iter);
}

Data::Boundary::FrontElementIterator MJMesh::Boundary::erase(const Data::Boundary::FrontElementIterator &iter)
{
	/*if ((*iter)->getId() == 40)
	{
		Data::cout() << "MJMesh::Boundary::erase " << (*iter) << " " << (*iter)->text() << " from boundary " << this << std::endl << Data::trace() << Data::endl;
	}*/

	static_cast<MJMesh::FrontElement *>((*iter))->removeBoundaryData(this);

	return Data::Boundary::erase(iter);
}

void MJMesh::Boundary::clearVertices()
{
    while (!this->vertices.empty())
	{
	    /*if (this->vertices.front()->getId() == 41)
        {
            Data::cout() << "MJMesh::Boundary::clear " << this->vertices.front() << " " << this->vertices.front()->text() << " from boundary " << this << std::endl << Data::trace() << Data::endl;
        }*/

	    static_cast<MJMesh::Vertex *>(this->vertices.front())->removeBoundaryData(this);

		this->vertices.pop_front();
	}
}

void MJMesh::Boundary::clearElements()
{
	while (!this->elements.empty())
	{
	    /*if (this->elements.front()->getId() == 40)
        {
            Data::cout() << "MJMesh::Boundary::clear " << this->elements.front() << " " << this->elements.front()->text() << " from boundary " << this << std::endl << Data::trace() << Data::endl;
        }*/

		static_cast<MJMesh::FrontElement *>(this->elements.front())->removeBoundaryData(this);

		this->elements.pop_front();
	}
}

bool MJMesh::less(Data::FrontElement *e1, Data::FrontElement *e2)
{
	return (e1->getShape()->size() < e2->getShape()->size());
	//return (e1->getShape()->size() <= e2->getShape()->size());
	//return ((e1->getShape()->size() - e2->getShape()->size()) < Data::getTolerance());
	//Real s1 = e1->getShape()->size();
	//Real s2 = e2->getShape()->size();
	//return ((std::fabs(s1 - s2) < Data::getTolerance()) ? (e2->getId() < e1->getId()) : (s1 < s2));
	//return ((s1 == s2) ? (e2->getId() < e1->getId()) : (s1 < s2));
}

void MJMesh::Boundary::swap(MJMesh::Boundary *boundary)
{
	for (Data::Boundary::VertexIterator iter = this->vBegin();
		 iter != this->vEnd(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		/*if (v->getId() == 41)
		{
		    Data::cout() << "MJMesh::Boundary::swap (this) " << v << " " << v->text() << " between " << this << " and " << boundary << std::endl << Data::trace() << Data::endl;
		}*/

		/*if (!v->isInBoundary(boundary))
		{
			v->removeBoundaryData(this);
			v->addBoundaryData(boundary);
		}*/

		if (v->isInBoundary(boundary))
		{
			v->swapBoundaryData(this, boundary);
		}
		else
		{
			v->replaceBoundaryData(this, boundary);
		}
	}

	for (Data::Boundary::VertexIterator iter = boundary->vBegin();
		 iter != boundary->vEnd(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		/*if (v->getId() == 41)
		{
		    Data::cout() << "MJMesh::Boundary::swap (boundary) " << v << " " << v->text() << " between " << this << " and " << boundary << std::endl << Data::trace() << Data::endl;
		}*/

		/*if (!v->isInBoundary(this))
		{
			v->removeBoundary(boundary);
			v->addBoundary(this);
		}*/

		if (!v->isInBoundary(this))
		{
			v->replaceBoundaryData(boundary, this);
		}
	}

	for (Data::Boundary::FrontElementIterator iter = this->eBegin();
		 iter != this->eEnd(); iter++)
	{
		MJMesh::FrontElement *fe = static_cast<MJMesh::FrontElement *>((*iter));

		/*if (fe->getId() == 40)
		{
		    Data::cout() << "MJMesh::Boundary::swap (this) " << fe << " " << fe->text() << " between " << this << " and " << boundary << std::endl << Data::trace() << Data::endl;
		}*/

		/*if (!fe->isInBoundary(boundary))
		{
			fe->removeBoundary(this);
			fe->addBoundary(boundary);
		}*/

		if (fe->isInBoundary(boundary))
		{
			fe->swapBoundaryData(this, boundary);
		}
		else
		{
			fe->replaceBoundaryData(this, boundary);
		}
	}

	for (Data::Boundary::FrontElementIterator iter = boundary->eBegin();
		 iter != boundary->eEnd(); iter++)
	{
		MJMesh::FrontElement *fe = static_cast<MJMesh::FrontElement *>((*iter));

		/*if (fe->getId() == 40)
		{
		    Data::cout() << "MJMesh::Boundary::swap (boundary) " << fe << " " << fe->text() << " between " << this << " and " << boundary << std::endl << Data::trace() << Data::endl;
		}*/

		/*if (!fe->isInBoundary(this))
		{
			fe->removeBoundary(boundary);
			fe->addBoundary(this);
		}*/

		if (!fe->isInBoundary(this))
		{
			fe->replaceBoundaryData(boundary, this);
		}
	}

	this->vertices.swap(boundary->vertices);
	this->elements.swap(boundary->elements);

	//this->verticesMap.swap(boundary->verticesMap);
	//this->elementsMap.swap(boundary->elementsMap);
}

void MJMesh::Boundary::sort()
{
	if (!this->sorted)
	{
		return;
	}

#if USE_JMSH_DEBUG_MESSAGES
	//debug markos
	/*if (this->vertices.front()->getPoint()->dimension() == 3)
	{
		std::cout.precision(7);

		std::cout << "markos debug, order of input faces:" << std::endl;

		for (FrontElementList::iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			std::cout << "markos debug, face = " << (*iter)->getVertex(0)->getId() - 1 << "\t" << (*iter)->getVertex(1)->getId() - 1 << "\t" << (*iter)->getVertex(2)->getId() - 1 << std::endl;
		}
	}*/
	//endebug markos
#endif //#if USE_JMSH_DEBUG_MESSAGES

#if USE_JMSH_IMPLEMENTATION_DETAILS
	///debug para ficar igual ao jmsh
	Data::FrontElementList sorted;

	Real tol = Data::getTolerance();
	Data::setTolerance(1.0e-12);

	while (!this->elements.empty())
	{
		Data::FrontElement *fel = this->elements.front();

		this->elements.pop_front();

		if (sorted.empty())
		{
			sorted.push_back(fel);

			continue;
		}

		Real s = fel->getShape()->size();
		Real s2 = sorted.front()->getShape()->size();

		if ((s < s2) || (std::fabs(s - s2) < Data::getTolerance()))
		{
			sorted.push_front(fel);

			continue;
		}

		if (sorted.size() == 1)
		{
			sorted.push_back(fel);

			continue;
		}

		s2 = sorted.back()->getShape()->size();

		if ((s > s2) || (std::fabs(s - s2) < Data::getTolerance()))
		{
			sorted.push_back(fel);

			continue;
		}

		for (Data::FrontElementList::iterator iter = sorted.begin();
			 iter != sorted.end(); iter++)
		{
			Real s2 = (*iter)->getShape()->size();

			if ((std::fabs(s - s2) < Data::getTolerance()) ||
				(s < s2))
			{
				sorted.insert(iter, fel);

				break;
			}
		}
	}

	this->elements.swap(sorted);

	Data::setTolerance(tol);
	///endebug para ficar igual ao jmsh
#else
	//std::sort(this->elements.begin(), this->elements.end(), MJMesh::less);

	this->elements.sort(MJMesh::less);
#endif //#if USE_JMSH_IMPLEMENTATION_DETAILS

#if USE_JMSH_DEBUG_MESSAGES
	/*//debug markos
	if (this->vertices.front()->getPoint()->dimension() == 3)
	{
		std::cout << "markos debug, order of input faces after insertion:" << std::endl;

		for (FrontElementList::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			std::cout << "markos debug, face = " << (*iter)->getVertex(0)->getId() - 1 << "\t" << (*iter)->getVertex(1)->getId() - 1 << "\t" << (*iter)->getVertex(2)->getId() - 1 << ", area = " << (*iter)->getShape()->size() << std::endl;
		}
	}
	//endebug markos*/
#endif //#if USE_JMSH_DEBUG_MESSAGES

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

/*void MJMesh::Boundary::debug() const
{
	for (Data::FrontElementList::const_iterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

		if (!fel->isInBoundary(this))
		{
			Data::cout() << "MJMesh::Boundary::debug ERROR! " << (*iter)->getId() << Data::endl;
		}
	}
}*/

#if USE_GUI
void MJMesh::Boundary::draw(bool lighting) const
{
	if (!this->hasLists())
	{
		this->createList(4);
	}

	if (this->hasList(0))
	{
		this->drawList(0);

		return;
	}

	this->beginList(0);

	if ((Data::getState().isFrontElementDrawn()) || (Data::getState().isVertexDrawn()))
	{
		UInt dimension = this->dimension();

		//int modes[2];

		//glGetIntegerv(GL_POLYGON_MODE, modes);

		//bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

		//glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

        //glPointSize(static_cast<GLfloat>(Data::getState().getPointSize()));

		if (Data::getState().isFrontElementDrawn())
		{
			struct AdjacencyFinder : public Data::AbstractMesh::AdjacencyFinder
			{
				const MJMesh::Boundary *boundary;

				Data::FrontElementList operator()(const Data::Vertex *v) const
				{
					return this->boundary->adjacency(v);
				}
			};

			AdjacencyFinder af;

			af.boundary = this;

			int modes[2];
			glGetIntegerv(GL_POLYGON_MODE, modes);

			this->drawWithArrayLinesOrPlanes(this->elements.begin(), this->elements.end(), dimension, this->size(),
				Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
				true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
				Data::getState().isNormalByVertex(), &af,
				true, this->getR(), this->getG(), this->getB(), false);

			/*UInt size = dimension*static_cast<UInt>(this->elements.size());
#if USE_QUADS
			UInt qsize = (dimension == 2) ? 0 : (dimension+1)*static_cast<UInt>(this->elements.size());
#endif //#if USE_QUADS

			GLfloat *fels = new GLfloat[dimension*size];
			GLfloat *colors = new GLfloat[3*size];
			GLfloat *normals = (dimension == 2) ? NULL : new GLfloat[3*size];
#if USE_QUADS
			GLfloat *qfels = (dimension == 2) ? NULL : new GLfloat[dimension*qsize];
			GLfloat *qcolors = (dimension == 2) ? NULL : new GLfloat[3*qsize];
			GLfloat *qnormals = (dimension == 2) ? NULL : new GLfloat[3*qsize];
#endif //#if USE_QUADS

			GLuint i = 0;
#if USE_QUADS
			GLuint qi = 0;
#endif //#if USE_QUADS

			bool lines = (dimension == 2) || ((!this->elements.empty()) && ((*this->elements.begin())->numVertices() == 2));

			bool normalByVertex = (!lines) && (normals) && (Data::getState().isNormalByVertex());

			for (Data::FrontElementList::const_iterator iter = this->elements.begin();
				 (iter != this->elements.end()) && (i < size); iter++)
			{
				UInt j = i;
#if USE_QUADS
				UInt qj = qi;
#endif //#if USE_QUADS

				if (normalByVertex)
                {
                    Data::PolygonList adjacency[(*iter)->numVertices()];

                    for (UInt k = 0; k < (*iter)->numVertices(); k++)
                    {
                        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter)->getVertex(k));

                        for (Data::FrontElementList::iterator iter2 = v->feBegin();
                             iter2 != v->feEnd(); iter2++)
                        {
                            if (!this->have((*iter2)))
                            {
                                continue;
                            }

                            adjacency[k].push_back(static_cast<Data::Polygon *>((*iter2)->getShape()));
                        }
                    }

#if USE_QUADS
					if (qsize > 0)
					{
						qi += (*iter)->getShape()->qfill(qfels + (dimension+1)*qi, NULL, qnormals + 3*qi, adjacency);
					}

					if (qi == qj)
					{
						i += (*iter)->getShape()->fill(fels + dimension*i, NULL, normals + 3*i, adjacency);
					}
#else
                    i += (*iter)->getShape()->fill(fels + dimension*i, NULL, normals + 3*i, adjacency);
#endif //#endif //#if USE_QUADS
                }
				else
                {
#if USE_QUADS
                    if (qsize > 0)
					{
						qi += (*iter)->getShape()->qfill(qfels + (dimension+1)*qi, NULL, qnormals ? qnormals + 3*qi : NULL);
					}

                    if (qi == qj)
					{
						i += (*iter)->getShape()->fill(fels + dimension*i, NULL, normals ? normals + 3*i : NULL);
					}
#else
                    i += (*iter)->getShape()->fill(fels + dimension*i, NULL, normals ? normals + 3*i : NULL);
#endif //#if USE_QUADS
                }

#if USE_QUADS
				for (; qj < qi; qj++)
				{
					colors[3*qj + 0] = static_cast<GLfloat>(this->getR());
					colors[3*qj + 1] = static_cast<GLfloat>(this->getG());
					colors[3*qj + 2] = static_cast<GLfloat>(this->getB());
				}
#endif //#if USE_QUADS
				for (; j < i; j++)
				{
					colors[3*j + 0] = static_cast<GLfloat>(this->getR());
					colors[3*j + 1] = static_cast<GLfloat>(this->getG());
					colors[3*j + 2] = static_cast<GLfloat>(this->getB());
				}
			}

			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			if (normals) glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(dimension, GL_FLOAT, 0, fels);
			glColorPointer(3, GL_FLOAT, 0, colors);
			if (normals) glNormalPointer(GL_FLOAT, 0, normals);

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

#if USE_QUADS
			if (qi > 0)
			{
				glVertexPointer(dimension, GL_FLOAT, 0, qfels);
				glColorPointer(3, GL_FLOAT, 0, qcolors);
				if (qnormals)
				{
					glNormalPointer(GL_FLOAT, 0, qnormals);
				}
				else
				{
					glDisableClientState(GL_NORMAL_ARRAY);
				}

				if ((lighting) && (lines) && (dimension == 3)) glDisable(GL_LIGHTING);

				glDrawArrays(GL_QUADS, 0, qi);

				if ((lighting) && (lines) && (dimension == 3)) glEnable(GL_LIGHTING);

				if ((!lines) && (dimension == 3) && (!wireframe) && (Data::getState().isTriangleBorderDrawn()))
				{
					//GLboolean lighting = glIsEnabled(GL_LIGHTING);

					if (lighting) glDisable(GL_LIGHTING);

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					glDrawArrays(GL_QUADS, 0, qi);

					if (lighting) glEnable(GL_LIGHTING);

					glPolygonMode(GL_FRONT, modes[0]);
					glPolygonMode(GL_BACK,  modes[1]);
				}
			}
#endif //#if USE_QUADS

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			if (normals) glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_COLOR_MATERIAL);

			delete [] fels;
			delete [] colors;
			if (normals) delete [] normals;

#if USE_QUADS
			if (qfels) delete [] qfels;
			if (qcolors) delete [] qcolors;
			if (qnormals) delete [] qnormals;
#endif //#if USE_QUADS

			if (Data::getState().isIdDrawn())
			{
				i = 0;

				for (Data::FrontElementList::const_iterator iter = this->elements.begin();
					 (iter != this->elements.end()) && (i < size); iter++)
				{
					(*iter)->drawId(this->getR(), this->getG(), this->getB());
				}
			}*/
		}

		if (Data::getState().isVertexDrawn())
		{
			this->drawWithArrayPoints(this->vertices.begin(), this->vertices.end(), dimension, static_cast<UInt>(this->vertices.size()),
				Data::getState().isIdDrawn(), false, false, Data::getState().getPointSize(false), Data::getState().getPointSize(true),
				true, this->getR(), this->getG(), this->getB(), false);
		}
	}

	this->endList();
}

void MJMesh::Boundary::draw() const
{
	GLboolean lighting = glIsEnabled(GL_LIGHTING);

	this->draw(lighting);
}

void MJMesh::Boundary::drawDiscretization() const
{
	if (!this->hasLists())
	{
		this->createList(4);
	}

	if (this->hasList(3))
	{
		this->drawList(3);

		return;
	}

	this->beginList(3);

	if (Data::getState().isVertexDrawn())
	{
		if (!this->vertices.empty())
		{
			//UInt dimension = this->vertices[0]->getPoint()->dimension();
			UInt dimension = this->vertices.front()->getPoint()->dimension();

			//for (Data::VertexVector::iterator iter = this->vertices.begin();
			for (Data::VertexList::const_iterator iter = this->vertices.begin();
				 iter != this->vertices.end(); iter++)
			{
				if (!Data::getState().isInVisibleSpace((*iter)->getPoint()))
				{
					continue;
				}

				MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

				//Data::FrontElementList fels = v->getAdjacentFrontElements();
				Data::FrontElementList bfels;

				//while (!fels.empty())
				for (Data::FrontElementList::iterator iter2 = v->feBegin();
					 iter2 != v->feEnd(); iter2++)
				{
					if (this->have((*iter2)))
					{
						bfels.push_back((*iter2));
					}

					//fels.pop_front();
				}

				if (bfels.empty())
				{
					continue;
				}

				//Data::Vector vec(dimension);
				VECTORd(vec, dimension);

				Real size = 0.0, count = static_cast<Real>(bfels.size());

				while (!bfels.empty())
				{
					MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(bfels.front());
					bfels.pop_front();

					//Data::Vector normal = (dimension == 2) ?
					//	static_cast<Data::Vector>(static_cast<Data::Edge2D *>(e->getShape())->normal()) :
					//	static_cast<Data::Vector>(static_cast<Data::Triangle3D *>(e->getShape())->normal());

					size += e->getShape()->size();

					//normal.normalize();

					//vec.sum(normal);
					if (dimension == 2)
					{
						vec.sum(static_cast<Data::Edge2D *>(e->getShape())->normal());
					}
					else
					{
						vec.sum(static_cast<Data::Triangle3D *>(e->getShape())->normal());
					}
				}

				vec.normalize();

				size /= count*2.0;

				vec.multiply(size);
				//vec.multiply(10.0);
				//vec.multiply(0.5);

				//Data::Vector s = vec;
				VECTORv(s, dimension, vec);

				s.multiply(-1.0);

				vec.sum(v->getPoint());
				s.sum(v->getPoint());

				if (dimension == 2)
				{
					Data::Point2D p1(vec);
					Data::Point2D p2(s);

					Data::Edge2D e(&p1, &p2);

					e.unhighlight();

					e.setColor(this->getR(), this->getG(), this->getB());
					//e.draw(false);
					e.draw();

					e.setPoints(NULL, NULL);
				}
				else if (dimension == 3)
				{
					Data::Point3D p1(vec);
					Data::Point3D p2(s);

					Data::Edge e(&p1, &p2);

					e.unhighlight();

					e.setColor(this->getR(), this->getG(), this->getB());
					//e.draw(false);
					e.draw();

					e.setPoints(NULL, NULL);
				}
			}
		}
	}

	this->endList();
}
#endif //#if USE_GUI
