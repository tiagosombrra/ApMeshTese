#ifndef _DATA_ELEMENT_H_
#define _DATA_ELEMENT_H_

#include "Data/Definitions.h"
#include "Data/GeometricShape.h"
#include "Data/Vertex.h"

namespace Data
{
	class Element :
		public Data::Identifiable
#if USE_GUI
		, public Data::Drawable
#endif //#if USE_GUI
	{
	public:

		Element(GeometricShape *shape, ULInt id = 0);
		virtual ~Element();

		virtual void setShape(GeometricShape *shape);
		virtual GeometricShape *getShape() const;

		virtual void resize(UInt numVertices);

		virtual UInt numVertices() const;
		virtual VertexVector getVertices() const;
		virtual Vertex *const *getVerticesArray() const;

		virtual void setVertex(UInt i, Vertex *v);
		virtual Vertex *getVertex(UInt i) const;

		virtual UInt index(const Vertex *v) const;
		virtual bool replace(Vertex *oldVertex, Vertex *newVertex);

		virtual void clear();

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool in(const Vertex &v) const;
		virtual bool in(const Vertex *v) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool on(const Vertex &v) const;
		virtual bool on(const Vertex *v) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual bool out(const Vertex &v) const;
		virtual bool out(const Vertex *v) const;

		virtual UInt numSides() const = 0;
		virtual UInt numVerticesInSide(UInt side) const = 0;
		virtual Vertex *getVertexInSide(UInt vertex, UInt side) const = 0;

		virtual Real quality() const = 0;

		virtual bool isBad() const = 0;

		virtual bool intercept(const Element &e) const = 0;
		virtual bool intercept(const Element *e) const = 0;

		virtual bool intercept(const Edge &e) const = 0;
		virtual bool intercept(const Edge *e) const = 0;

		virtual bool intercept(const Point &p) const = 0;
		virtual bool intercept(const Point *p) const = 0;

		virtual bool haveMatching(const Point &p) const;
		virtual bool have(const Point &p) const;
		virtual bool have(const Point *p) const;

		virtual bool have(const Vertex &v) const;
		virtual bool have(const Vertex *v) const;

		virtual bool have(const Edge &e) const = 0;
		virtual bool have(const Edge *e) const = 0;

		virtual bool have(const FrontElement &e) const = 0;
		virtual bool have(const FrontElement *e) const = 0;

		virtual bool isAdjacent(const Element &e) const = 0;
		virtual bool isAdjacent(const Element *e) const = 0;

#if USE_ARRAY
		virtual Element &operator=(const Element &e);
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void setR(Real r) const;
		virtual Real getR() const;

		virtual void setG(Real g) const;
		virtual Real getG() const;

		virtual void setB(Real b) const;
		virtual Real getB() const;

		virtual void highlight() const;
		virtual void unhighlight() const;
		virtual bool isHighlighted() const;

		//virtual void draw(bool fill) const;
		using Drawable::draw;
		virtual void draw() const;
		virtual void drawId() const;
		virtual void drawId(Real r, Real g, Real b) const;
#endif //#if USE_GUI

		virtual std::string text() const;
		virtual std::string text(const Data::AbstractMesh *mesh) const;

	protected:

		GeometricShape *shape;

#if USE_ARRAY
		Vertex **vertices;
		UInt numVerts;
#else
		VertexVector vertices;
#endif //#if USE_ARRAY
	};
}

#endif //#ifndef _DATA_ELEMENT_H_
