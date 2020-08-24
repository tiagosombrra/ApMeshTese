#ifndef _DATA_DRAWABLE_H_
#define _DATA_DRAWABLE_H_

#include "Data/Definitions.h"

#if USE_GUI

namespace Data
{
	class Drawable
	{
	public:

		Drawable();
		virtual ~Drawable();

		virtual void setR(Real r) const = 0;
		virtual Real getR() const = 0;

		virtual void setG(Real g) const = 0;
		virtual Real getG() const = 0;

		virtual void setB(Real b) const = 0;
		virtual Real getB() const = 0;

		virtual void setColor(Real r, Real g, Real b) const;
		virtual void getColor(Real &r, Real &g, Real &b) const;

		virtual void highlight() const = 0;
		virtual void unhighlight() const = 0;
		virtual bool isHighlighted() const = 0;

		virtual void draw() const = 0;
		///draws with a temporary specified color
		virtual void draw(Real r, Real g, Real b) const;
		///for traditional picking to work, the drawable objects must not be
		///  deleted between drawing and getting objects
		virtual void drawPick() const;
		virtual DrawableList picked(GLint numHits, GLuint buffer[]) const;

		virtual std::string text() const = 0;

		///Filling only with triangles, independently of the existence of quads
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		virtual UInt fill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const;
		///Filling with quads
		virtual UInt qfill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		virtual UInt qfill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const;
		///Filling with triangles what cannot be filled with quads (prismatic shapes, for example)
		virtual UInt tfill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
		virtual UInt tfill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const;

	protected:

		virtual void explodeAndShrink(const Point &mid, bool point = false, bool vector = false) const;
		virtual void explodeAndShrink(const Point *mid, bool point = false, bool vector = false) const;
		virtual void explodeAndShrink(GLfloat *coord, const Point &mid, bool point = false, bool vector = false) const;

	protected:

		static const UInt numBytes = sizeof(Drawable *)/sizeof(GLbyte);
		static const UInt numUints = sizeof(Drawable *)/sizeof(GLuint);

	public:

		static const UInt numElems = (sizeof(Drawable *)%sizeof(GLuint) == 0) ? numUints : numBytes;

	protected:

		static const bool useUints = (numElems == numUints);

		union Name
		{
			Drawable *ptr;
			GLbyte bytes[numBytes];
			GLuint uints[numUints];
		};
	};
}

#endif //#if USE_GUI

#endif //#ifndef _DATA_DRAWABLE_H_
