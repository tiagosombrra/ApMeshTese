#ifndef _DATA_VERTEX_H_
#define _DATA_VERTEX_H_

#include "Data/Definitions.h"
#include "Data/Identifiable.h"
#include "Data/Drawable.h"

namespace Data
{
	class Vertex :
		public Data::Identifiable
#if USE_GUI
		, public Data::Drawable
#endif //#if USE_GUI
	{
	public:

		Vertex(Point *p, ULInt id = 0);
		virtual ~Vertex();

		virtual void setPoint(Point *p);
		virtual Point *getPoint() const;

		//virtual const EdgeList &getAdjacency() const;

		//virtual void add(Edge *e);
		//virtual void remove(Edge *e);

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

		using Drawable::draw;
		virtual void draw() const;
		virtual void drawId() const;
		virtual void drawId(Real r, Real g, Real b) const;
#endif //#if USE_GUI

		virtual std::string text() const;
		virtual std::string text(const Data::AbstractMesh *mesh) const;

	protected:

		Point *point;

		//EdgeList adjacency;
	};
}

#endif //#ifndef _DATA_VERTEX_H_
