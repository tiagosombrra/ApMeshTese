#ifndef _DATA_ABSTRACT_MESH_H_
#define _DATA_ABSTRACT_MESH_H_

#include "Data/Definitions.h"
#include "Data/Identifiable.h"
#include "Data/ListDisplayable.h"

namespace Data
{
	class AbstractMesh :
		public Data::Identifiable
#if USE_GUI
		, public Data::ListDisplayable
#endif //#if USE_GUI
	{
	public:

		AbstractMesh(ULInt id = 0);
		virtual ~AbstractMesh();

		virtual bool have(const Vertex *v) const;
		virtual bool have(const FrontElement *fel) const;
		virtual bool have(const Element *e) const;

		virtual ULInt verticesSize() const;
		virtual ULInt size() const;

		virtual UInt dimension() const;

#if USE_GUI
	public:

		struct AdjacencyFinder
		{
			virtual Data::FrontElementList operator()(const Data::Vertex *v) const = 0;
		};

    protected:

		template <typename Iterator>
		void drawWithArrayPoints(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
			bool ids, bool lighting,
			bool highlighted, Real unhighlightedPointSize, Real highlightedPointSize,
			bool externalColor, Real r, Real g, Real b, bool search) const;

		template <typename Iterator>
		void drawWithArrayLines(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
			bool ids, bool lighting, Real lineWidth,
			bool externalColor, Real r, Real g, Real b) const;

		template <typename Iterator>
		void drawWithArrayPlanes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
			bool ids, bool lighting, Real lineWidth,
			bool quads, bool fill, bool border,
			bool normalByVertex, AdjacencyFinder *adjacency,
			bool quality, Real minQuality, Real maxQuality,
			bool externalColor, Real r, Real g, Real b, bool search) const;

		template <typename Iterator>
		void drawWithArrayVolumes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
			bool ids, bool lighting, Real lineWidth,
			bool quads, bool fill, bool border,
			bool quality, Real minQuality, Real maxQuality,
			bool externalColor, Real r, Real g, Real b) const;

		template <typename Iterator>
		void drawWithArrayLinesOrPlanes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
			bool ids, bool lighting, Real lineWidth,
			bool quads, bool fill, bool border,
			bool normalByVertex, AdjacencyFinder *adjacency,
			bool externalColor, Real r, Real g, Real b, bool search) const;

		template <typename Iterator>
		void drawWithArrayPlanesOrVolumes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
			bool ids, bool lighting, Real lineWidth,
			bool quads, bool fill, bool border,
			bool normalByVertex, AdjacencyFinder *adjacency,
			bool quality, Real minQuality, Real maxQuality,
			bool externalColor, Real r, Real g, Real b, bool search) const;

#endif //#if USE_GUI
	};
}

#if USE_GUI
#include "Data/Point.h"
#include "Data/Polygon.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"

template <typename Iterator>
void Data::AbstractMesh::drawWithArrayPoints(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
	bool ids, bool lighting, bool highlighted, Real unhighlightedPointSize, Real highlightedPointSize,
	bool externalColor, Real r, Real g, Real b, bool search) const
{
	if ((size == 0) || (begin == end))
	{
		return;
	}

	this->drawingTotal += size;

	ULInt nsize = size, hsize = size;

	if ((search) && (highlighted))
	{
	    this->drawingTotal += size;

		nsize = 0;
		hsize = 0;

		ULInt j = 0;

		for (Iterator iter = begin; (iter != end) && (j < size); ++iter, ++j, ++this->drawingMeasure)
		{
		    if ((*iter)->isHighlighted())
			{
				hsize++;
			}
			else
			{
				nsize++;
			}
		}
	}

	lighting = ((lighting) && (dimension == 3));

	GLfloat *vertices = new GLfloat[dimension*nsize];
	GLfloat *colors = new GLfloat[3*nsize];

	GLfloat *hvertices = highlighted ? new GLfloat[dimension*hsize] : NULL;
	GLfloat *hcolors = highlighted ? new GLfloat[3*hsize] : NULL;

	GLuint i = 0;
	GLuint hi = 0;

	for (Iterator iter = begin; (iter != end) && ((i < static_cast<GLuint>(nsize)) || (hi < static_cast<GLuint>(hsize))); ++iter, ++this->drawingMeasure)
	{
		GLuint j = i;
		GLuint hj = hi;

		if ((highlighted) && ((*iter)->isHighlighted()))
		{
			if (hi < static_cast<GLuint>(hsize))
            {
                hi += (*iter)->getPoint()->fill(hvertices + dimension*hi, externalColor ? NULL : hcolors + 3*hi);
            }

            if (externalColor)
            {
                for (; hj < hi; hj++)
                {
                    hcolors[3*hj + 0] = static_cast<GLfloat>(r);
                    hcolors[3*hj + 1] = static_cast<GLfloat>(g);
                    hcolors[3*hj + 2] = static_cast<GLfloat>(b);
                }
            }
		}
		else
		{
			if ((i < static_cast<GLuint>(nsize)) && ((*iter)) && ((*iter)->getPoint()))
            {
                i += (*iter)->getPoint()->fill(vertices + dimension*i, externalColor ? NULL : colors + 3*i);
            }

            if (externalColor)
            {
                for (; j < i; j++)
                {
                    colors[3*j + 0] = static_cast<GLfloat>(r);
                    colors[3*j + 1] = static_cast<GLfloat>(g);
                    colors[3*j + 2] = static_cast<GLfloat>(b);
                }
            }
		}
	}

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_COLOR_MATERIAL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	if (lighting) glEnable(GL_LIGHTING);
	else  glDisable(GL_LIGHTING);

	glPointSize(static_cast<GLfloat>(unhighlightedPointSize));

	glVertexPointer(dimension, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	glDrawArrays(GL_POINTS, 0, i);

	if (highlighted)
	{
		glPointSize(static_cast<GLfloat>(highlightedPointSize));

		glVertexPointer(dimension, GL_FLOAT, 0, hvertices);
		glColorPointer(3, GL_FLOAT, 0, hcolors);

		glDrawArrays(GL_POINTS, 0, hi);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopAttrib();

	delete [] vertices;
	delete [] colors;
	if (hvertices) delete [] hvertices;
	if (hcolors) delete [] hcolors;

	if (ids)
	{
	    this->drawingTotal += size;

		ULInt j = 0;

		for (Iterator iter = begin; (iter != end) && (j < size); ++iter, ++j, ++this->drawingMeasure)
		{
			if (externalColor)
			{
				(*iter)->drawId(r, g, b);
			}
			else
			{
				(*iter)->drawId();
			}
		}
	}
}

template <typename Iterator>
void Data::AbstractMesh::drawWithArrayLines(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
	bool ids, bool lighting, Real lineWidth, bool externalColor, Real r, Real g, Real b) const
{
	if ((size == 0) || (begin == end))
	{
		return;
	}

	this->drawingTotal += size;

	lighting = ((lighting) && (dimension == 3));

	ULInt lsize = 2*size;

	GLfloat *fels = new GLfloat[dimension*lsize];
	GLfloat *colors = new GLfloat[3*lsize];

	GLuint i = 0;

	for (Iterator iter = begin; (iter != end) && (i < static_cast<GLuint>(lsize)); ++iter, ++this->drawingMeasure)
	{
		GLuint j = i;

        if ((*iter) && ((*iter)->getShape()))
        {
            if (i < static_cast<GLuint>(lsize))
            {
                i += (*iter)->getShape()->fill(fels + dimension*i, externalColor ? NULL : colors + 3*i, NULL);
            }
        }

        if (externalColor)
        {
            for (; j < i; j++)
            {
                colors[3*j + 0] = static_cast<GLfloat>(r);
                colors[3*j + 1] = static_cast<GLfloat>(g);
                colors[3*j + 2] = static_cast<GLfloat>(b);
            }
        }
	}

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	if (lighting) glEnable(GL_LIGHTING);
	else  glDisable(GL_LIGHTING);

	glLineWidth(static_cast<GLfloat>(lineWidth));

	glVertexPointer(dimension, GL_FLOAT, 0, fels);
	glColorPointer(3, GL_FLOAT, 0, colors);

	glDrawArrays(GL_LINES, 0, i);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopAttrib();

	delete [] fels;
	delete [] colors;

	if (ids)
	{
	    this->drawingTotal += size;

	    ULInt j = 0;

		for (Iterator iter = begin; (iter != end) && (j < size); ++iter, ++j, ++this->drawingMeasure)
		{
			if (externalColor)
			{
				(*iter)->drawId(r, g, b);
			}
			else
			{
				(*iter)->drawId();
			}
		}
	}
}

template <typename Iterator>
void Data::AbstractMesh::drawWithArrayPlanes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
	bool ids, bool lighting, Real lineWidth, bool quads, bool fill, bool border,
	bool normalByVertex, AdjacencyFinder *adjacency,
	bool quality, Real minQuality, Real maxQuality,
	bool externalColor, Real r, Real g, Real b, bool search) const
{
	if ((size == 0) || (begin == end))
	{
		return;
	}

	this->drawingTotal += size;

#if (!USE_QUADS)
	quads = false;
#endif //#if (!USE_QUADS)

	ULInt tsize = size, qsize = quads ? size : 0;

	if ((search) && (quads))
	{
	    this->drawingTotal += size;

		tsize = 0;
		qsize = 0;

		for (Iterator iter = begin; iter != end; ++iter, ++this->drawingMeasure)
		{
			if ((*iter) && ((*iter)->getShape()) && ((*iter)->numVertices() == 4))
			{
				qsize++;
			}
			else
			{
				tsize++;
			}
		}
	}

	lighting = ((lighting) && (dimension == 3));
	normalByVertex = (normalByVertex) && (dimension == 3) && (adjacency);
	border = (border) && (fill);

	tsize *= 3;
	qsize *= 4;

	GLfloat *elements = new GLfloat[dimension*tsize];
	GLfloat *colors = new GLfloat[3*tsize];
	GLfloat *bcolors = (border) ? new GLfloat[3*tsize] : NULL;
	GLfloat *normals = (dimension == 3) ? new GLfloat[3*tsize] : NULL;

	GLfloat *qelements = quads ? new GLfloat[dimension*qsize] : NULL;
	GLfloat *qcolors = quads ? new GLfloat[3*qsize] : NULL;
	GLfloat *bqcolors = ((quads) && (border)) ? new GLfloat[3*qsize] : NULL;
	GLfloat *qnormals = ((quads) && (dimension == 3)) ? new GLfloat[3*qsize] : NULL;

	GLuint i = 0;
	GLuint qi = 0;

	for (Iterator iter = begin;
         (iter != end) && (i/3 + qi/4 < static_cast<GLuint>(size)) && ((i < static_cast<GLuint>(tsize)) || (qi < static_cast<GLuint>(qsize)));
         ++iter, ++this->drawingMeasure)
	{
		GLuint j = i;
		GLuint qj = qi;

		if ((*iter) && ((*iter)->getShape()))
		{
			if (quality)
			{
				Real q = (*iter)->quality();

				q = (q < 0.0) ? 0.0 : ((q > 1.0) ? 1.0 : q);

				bool qualityInRange = ((q >= minQuality) && (q <= maxQuality));

				if (!qualityInRange)
				{
					continue;
				}
			}

			Data::PolygonList *adj = NULL;

			if (normalByVertex)
			{
				adj = new Data::PolygonList[(*iter)->numVertices()];

				for (UInt k = 0; k < (*iter)->numVertices(); k++)
				{
					Data::FrontElementList adjs = (*adjacency)((*iter)->getVertex(k));

					for (Data::FrontElementList::iterator iter2 = adjs.begin();
						 iter2 != adjs.end(); iter2++)
					{
						adj[k].push_back(static_cast<Data::Polygon *>((*iter2)->getShape()));
					}
				}
			}

			if (quads)
			{
				if (qi < static_cast<GLuint>(qsize))
                {
                    qi += (*iter)->getShape()->qfill(qelements + dimension*qi, externalColor ? NULL : qcolors + 3*qi, qnormals ? qnormals + 3*qi : NULL, adj);
                }

                if (border)
                {
                    for (GLuint itj = qj; itj < qi; itj++)
                    {
                        bqcolors[3*itj + 0] = static_cast<GLfloat>(r);
                        bqcolors[3*itj + 1] = static_cast<GLfloat>(g);
                        bqcolors[3*itj + 2] = static_cast<GLfloat>(b);
                    }
                }

                if (externalColor)
                {
                    for (GLuint itj = qj; itj < qi; itj++)
                    {
                        qcolors[3*itj + 0] = static_cast<GLfloat>(r);
                        qcolors[3*itj + 1] = static_cast<GLfloat>(g);
                        qcolors[3*itj + 2] = static_cast<GLfloat>(b);
                    }
                }
			}

			if (qi == qj)
			{
				if (i < static_cast<GLuint>(tsize))
                {
                    i += (*iter)->getShape()->fill(elements + dimension*i, externalColor ? NULL : colors + 3*i, normals ? normals + 3*i : NULL, adj);
                }

                if (border)
                {
                    for (GLuint itj = j; itj < i; itj++)
                    {
                        bcolors[3*itj + 0] = static_cast<GLfloat>(r);
                        bcolors[3*itj + 1] = static_cast<GLfloat>(g);
                        bcolors[3*itj + 2] = static_cast<GLfloat>(b);
                    }
                }

                if (externalColor)
                {
                    for (GLuint itj = j; itj < i; itj++)
                    {
                        colors[3*itj + 0] = static_cast<GLfloat>(r);
                        colors[3*itj + 1] = static_cast<GLfloat>(g);
                        colors[3*itj + 2] = static_cast<GLfloat>(b);
                    }
                }
			}

			if (adj)
			{
				delete [] adj;
			}
		}
	}

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	if (normals) glEnableClientState(GL_NORMAL_ARRAY);
	else glDisableClientState(GL_NORMAL_ARRAY);

	if (lighting) glEnable(GL_LIGHTING);
	else  glDisable(GL_LIGHTING);

	glLineWidth(static_cast<GLfloat>(lineWidth));

	glVertexPointer(dimension, GL_FLOAT, 0, elements);
	glColorPointer(3, GL_FLOAT, 0, colors);
	if (normals) glNormalPointer(GL_FLOAT, 0, normals);

	glDrawArrays(GL_TRIANGLES, 0, i);

	if (border)
	{
		int modes[2];
		glGetIntegerv(GL_POLYGON_MODE, modes);

		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (bcolors) glColorPointer(3, GL_FLOAT, 0, bcolors);

		glDrawArrays(GL_TRIANGLES, 0, i);

		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK,  modes[1]);
	}

	if (quads)
	{
		if (lighting) glEnable(GL_LIGHTING);
		else  glDisable(GL_LIGHTING);

		glVertexPointer(dimension, GL_FLOAT, 0, qelements);
		glColorPointer(3, GL_FLOAT, 0, qcolors);
		if (qnormals) glNormalPointer(GL_FLOAT, 0, qnormals);
		else glDisableClientState(GL_NORMAL_ARRAY);

		glDrawArrays(GL_QUADS, 0, qi);

		if (border)
		{
			int modes[2];
			glGetIntegerv(GL_POLYGON_MODE, modes);

			glDisable(GL_LIGHTING);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			if (bqcolors) glColorPointer(3, GL_FLOAT, 0, bqcolors);

			glDrawArrays(GL_QUADS, 0, qi);

			glPolygonMode(GL_FRONT, modes[0]);
			glPolygonMode(GL_BACK,  modes[1]);
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopAttrib();

	delete [] elements;
	delete [] colors;
	if (bcolors) delete [] bcolors;
	if (normals) delete [] normals;
	delete [] qelements;
	delete [] qcolors;
	if (bqcolors) delete [] bqcolors;
	if (qnormals) delete [] qnormals;

	if (ids)
	{
	    this->drawingTotal += size;

		ULInt j = 0;

		for (Iterator iter = begin; (iter != end) && (j < size); ++iter, ++j, ++this->drawingMeasure)
		{
		    if (quality)
			{
				Real q = (*iter)->quality();

				q = (q < 0.0) ? 0.0 : ((q > 1.0) ? 1.0 : q);

				bool qualityInRange = ((q >= minQuality) && (q <= maxQuality));

				if (!qualityInRange)
				{
					continue;
				}
			}

			if (externalColor)
			{
				(*iter)->drawId(r, g, b);
			}
			else
			{
				(*iter)->drawId();
			}
		}
	}
}

template <typename Iterator>
void Data::AbstractMesh::drawWithArrayVolumes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
	bool ids, bool lighting, Real lineWidth, bool quads, bool fill, bool border,
	bool quality, Real minQuality, Real maxQuality,
	bool externalColor, Real r, Real g, Real b) const
{
	if ((size == 0) || (begin == end) || (dimension != 3))
	{
		return;
	}

	this->drawingTotal += size;

#if (!USE_BRICKS)
	quads = false;
#endif //#if (!USE_BRICKS)

	border = (border) && (fill);

	ULInt tsize = size, qsize = quads ? size : 0;

	if (!quads)
	{
		tsize *= 3;
	}

	tsize *= 3*4*2;
	qsize *= 3*4*2;

	GLfloat *elements = new GLfloat[dimension*tsize];
	GLfloat *colors = new GLfloat[3*tsize];
	GLfloat *bcolors = (border) ? new GLfloat[3*tsize] : NULL;
	GLfloat *normals = (dimension == 3) ? new GLfloat[3*tsize] : NULL;

	GLfloat *qelements = quads ? new GLfloat[dimension*qsize] : NULL;
	GLfloat *qcolors = quads ? new GLfloat[3*qsize] : NULL;
	GLfloat *bqcolors = ((quads) && (border)) ? new GLfloat[3*qsize] : NULL;
	GLfloat *qnormals = ((quads) && (dimension == 3)) ? new GLfloat[3*qsize] : NULL;

	GLuint i = 0;
	GLuint qi = 0;

	for (Iterator iter = begin; (iter != end) && ((i < static_cast<GLuint>(tsize)) || (qi < static_cast<GLuint>(qsize))); ++iter, ++this->drawingMeasure)
	{
		GLuint j = i;
		GLuint qj = qi;

		if ((*iter) && ((*iter)->getShape()))
		{
			if (quality)
			{
				Real q = (*iter)->quality();

				q = (q < 0.0) ? 0.0 : ((q > 1.0) ? 1.0 : q);

				bool qualityInRange = ((q >= minQuality) && (q <= maxQuality));

				if (!qualityInRange)
				{
					continue;
				}
			}

			if (quads)
			{
				if (qi < static_cast<GLuint>(qsize))
                {
                    qi += (*iter)->getShape()->qfill(qelements + dimension*qi, externalColor ? NULL : qcolors + 3*qi, qnormals ? qnormals + 3*qi : NULL);
                }

                if (border)
                {
                    for (GLuint itj = qj; itj < qi; itj++)
                    {
                        bqcolors[3*itj + 0] = static_cast<GLfloat>(r);
                        bqcolors[3*itj + 1] = static_cast<GLfloat>(g);
                        bqcolors[3*itj + 2] = static_cast<GLfloat>(b);
                    }
                }

                if (externalColor)
                {
                    for (GLuint itj = qj; itj < qi; itj++)
                    {
                        colors[3*itj + 0] = static_cast<GLfloat>(r);
                        colors[3*itj + 1] = static_cast<GLfloat>(g);
                        colors[3*itj + 2] = static_cast<GLfloat>(b);
                    }
                }
			}

			if ((!quads) || (qi == qj))
			{
				if (i < static_cast<GLuint>(tsize))
                {
                    i += (*iter)->getShape()->fill(elements + dimension*i, externalColor ? NULL : colors + 3*i, normals ? normals + 3*i : NULL);
                }
			}
			else
			{
				if (i < static_cast<GLuint>(tsize))
                {
                    i += (*iter)->getShape()->tfill(elements + dimension*i, externalColor ? NULL : colors + 3*i, normals ? normals + 3*i : NULL);
                }
			}

			if (border)
			{
				for (GLuint itj = j; itj < i; itj++)
				{
					bcolors[3*itj + 0] = static_cast<GLfloat>(r);
					bcolors[3*itj + 1] = static_cast<GLfloat>(g);
					bcolors[3*itj + 2] = static_cast<GLfloat>(b);
				}
			}

			if (externalColor)
			{
				for (GLuint itj = j; itj < i; itj++)
				{
					colors[3*itj + 0] = static_cast<GLfloat>(r);
					colors[3*itj + 1] = static_cast<GLfloat>(g);
					colors[3*itj + 2] = static_cast<GLfloat>(b);
				}
			}
		}
	}

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	if (normals) glEnableClientState(GL_NORMAL_ARRAY);
	else glDisableClientState(GL_NORMAL_ARRAY);

	if (lighting) glEnable(GL_LIGHTING);
	else  glDisable(GL_LIGHTING);

	glLineWidth(static_cast<GLfloat>(lineWidth));

	glVertexPointer(dimension, GL_FLOAT, 0, elements);
	glColorPointer(3, GL_FLOAT, 0, colors);
	if (normals) glNormalPointer(GL_FLOAT, 0, normals);

	glDrawArrays(GL_TRIANGLES, 0, i);

	if (border)
	{
		int modes[2];
		glGetIntegerv(GL_POLYGON_MODE, modes);

		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (bcolors) glColorPointer(3, GL_FLOAT, 0, bcolors);

		glDrawArrays(GL_TRIANGLES, 0, i);

		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK,  modes[1]);
	}

	if (quads)
	{
		if (lighting) glEnable(GL_LIGHTING);
		else  glDisable(GL_LIGHTING);

		glVertexPointer(dimension, GL_FLOAT, 0, qelements);
		glColorPointer(3, GL_FLOAT, 0, qcolors);
		if (qnormals) glNormalPointer(GL_FLOAT, 0, qnormals);
		else glDisableClientState(GL_NORMAL_ARRAY);

		glDrawArrays(GL_QUADS, 0, qi);

		if (border)
		{
			int modes[2];
			glGetIntegerv(GL_POLYGON_MODE, modes);

			glDisable(GL_LIGHTING);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			if (bqcolors) glColorPointer(3, GL_FLOAT, 0, bqcolors);

			glDrawArrays(GL_QUADS, 0, qi);

			glPolygonMode(GL_FRONT, modes[0]);
			glPolygonMode(GL_BACK,  modes[1]);
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopAttrib();

	delete [] elements;
	delete [] colors;
	if (bcolors) delete [] bcolors;
	if (normals) delete [] normals;
	delete [] qelements;
	delete [] qcolors;
	if (bqcolors) delete [] bqcolors;
	if (qnormals) delete [] qnormals;

	if (ids)
	{
	    this->drawingTotal += size;

	    ULInt j = 0;

		for (Iterator iter = begin; (iter != end) && (j < size); ++iter, ++j, ++this->drawingMeasure)
		{
		    if (quality)
			{
				Real q = (*iter)->quality();

				q = (q < 0.0) ? 0.0 : ((q > 1.0) ? 1.0 : q);

				bool qualityInRange = ((q >= minQuality) && (q <= maxQuality));

				if (!qualityInRange)
				{
					continue;
				}
			}

			if (externalColor)
			{
				(*iter)->drawId(r, g, b);
			}
			else
			{
				(*iter)->drawId();
			}
		}
	}
}

template <typename Iterator>
void Data::AbstractMesh::drawWithArrayLinesOrPlanes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
	bool ids, bool lighting, Real lineWidth, bool quads, bool fill, bool border,
	bool normalByVertex, AdjacencyFinder *adjacency,
	bool externalColor, Real r, Real g, Real b, bool search) const
{
	if ((size == 0) || (begin == end))
	{
		return;
	}

	if ((dimension == 2) || ((*begin)->numVertices() == 2))
	{
		drawWithArrayLines(begin, end, dimension, size,
			ids, lighting, lineWidth, externalColor, r, g, b);
	}
	else
	{
		drawWithArrayPlanes(begin, end, dimension, size,
			ids, lighting, lineWidth, quads, fill, border,
			normalByVertex, adjacency,
			false, 0.0, 1.0,
			externalColor, r, g, b, search);
	}
}

template <typename Iterator>
void Data::AbstractMesh::drawWithArrayPlanesOrVolumes(const Iterator &begin, const Iterator &end, UInt dimension, ULInt size,
	bool ids, bool lighting, Real lineWidth, bool quads, bool fill, bool border,
	bool normalByVertex, AdjacencyFinder *adjacency,
	bool quality, Real minQuality, Real maxQuality,
	bool externalColor, Real r, Real g, Real b, bool search) const
{
	if ((size == 0) || (begin == end))
	{
		return;
	}

	if ((dimension == 2) || (dynamic_cast<Data::Polygon *>((*begin)->getShape())))
	{
		drawWithArrayPlanes(begin, end, dimension, size,
			ids, lighting, lineWidth, quads, fill, border,
			normalByVertex, adjacency,
			quality, minQuality, maxQuality,
			externalColor, r, g, b, search);
	}
	else
	{
		drawWithArrayVolumes(begin, end, dimension, size,
			ids, lighting, lineWidth, quads, fill, border,
			quality, minQuality, maxQuality,
			externalColor, r, g, b);
	}
}
#endif //#if USE_GUI

#endif //#ifndef _DATA_ABSTRACT_MESH_H_
