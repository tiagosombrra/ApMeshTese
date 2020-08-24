#ifndef _DATA_STATE_H_
#define _DATA_STATE_H_

#include "Data/Definitions.h"
#include "Data/Point.h"
#include "Data/Box.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

namespace Data
{
	class State
	{
	public:

		State();
		virtual ~State();

#if USE_GUI
	public:

		enum PlanePosition
		{
			RIGHT = 0,
			LEFT,
			TOP,
			BOTTOM,
			FRONT,
			BACK,
			COUNT
		};

		enum ColoredType
		{
		    UNKNOWN = 0,

		    BOUNDARY_VERTEX,
		    BOUNDARY_FRONT_ELEMENT,

		    MESH_VERTEX,
		    MESH_FRONT_ELEMENT,
		    MESH_ELEMENT,

		    MESH_ELEMENT_QUALITY_0,
		    MESH_ELEMENT_QUALITY_1,
		    MESH_ELEMENT_QUALITY_2,
		    MESH_ELEMENT_QUALITY_3,
		    MESH_ELEMENT_QUALITY_4,
		    MESH_ELEMENT_QUALITY_5,
		    MESH_ELEMENT_QUALITY_6,
		    MESH_ELEMENT_QUALITY_7,
		    MESH_ELEMENT_QUALITY_8,
		    MESH_ELEMENT_QUALITY_9,

		    FRONT_VERTEX,
		    FRONT_BOUNDARY_FRONT_ELEMENT,
		    FRONT_ELEMENT_FRONT_ELEMENT,
		    FRONT_REJECTED_FRONT_ELEMENT,
		    FRONT_CROSSING_FRONT_ELEMENT,

		    TREE_CELL_DEFAULT,
		    TREE_CELL_IN,
		    TREE_CELL_ON,
		    TREE_CELL_OUT,
		    TREE_CELL_UNKNOWN,

		    PARTITION,

		    SIZE
		};
#endif //#if USE_GUI

		virtual void setTolerance(Real tolerance);
		virtual Real getTolerance() const;

#if USE_BUFFER
		virtual void setBuffer(const std::string &buffer);
		virtual const std::string &getBuffer() const;
#endif //#if USE_BUFFER

#if USE_GUI
		/*virtual void enableDebug();
		virtual void disableDebug();
		virtual bool toggleDebug();
		virtual void setDebug(bool debug);
		virtual bool isDebug() const;*/

		virtual void setStepByStep(bool stepByStep);
		virtual bool isStepByStep() const;

		virtual void setAnimating(bool animating);
		virtual bool isAnimating() const;

		virtual void setBox(const Box &box);
		virtual const Box &getBox() const;

		virtual void setMid(const Point &mid);
		virtual const Point &getMid() const;

		virtual void setShrink(Real shrink);
		virtual Real getShrink() const;

		virtual void setExplode(Real explode);
		virtual Real getExplode() const;

		virtual void setExplodeExternal(Real explodeExternal);
		virtual Real getExplodeExternal() const;

		virtual void setMidExternal(const Point3D &midExternal);
		virtual const Point3D &getMidExternal() const;
		virtual void resetMidExternal();

		virtual void setElementDrawn(bool elementDrawn);
		virtual bool isElementDrawn() const;

		virtual void setFrontElementDrawn(bool frontElementDrawn);
		virtual bool isFrontElementDrawn() const;

		virtual void setVertexDrawn(bool vertexDrawn);
		virtual bool isVertexDrawn() const;

		virtual void setIdDrawn(bool idDrawn);
		virtual bool isIdDrawn() const;

		virtual void setTriangleBorderDrawn(bool triangleBorderDrawn);
		virtual bool isTriangleBorderDrawn() const;

		virtual void setNormalByVertex(bool normalByVertex);
		virtual bool isNormalByVertex() const;

		virtual void setNormalInverted(bool normalInverted);
		virtual bool isNormalInverted() const;

		virtual void setLineWidth(Real width);
		virtual Real getLineWidth() const;
        virtual Real getLineWidth(bool highlighted);

		virtual void setPointSize(Real size);
		virtual Real getPointSize() const;
		virtual Real getPointSize(bool highlighted);

		virtual void setTypeColor(enum ColoredType type, Real r, Real g, Real b) const;
		virtual void getTypeColor(enum ColoredType type, Real &r, Real &g, Real &b) const;
		virtual Real getTypeColorR(enum ColoredType type) const;
		virtual Real getTypeColorG(enum ColoredType type) const;
		virtual Real getTypeColorB(enum ColoredType type) const;

		virtual void setGlutFont(void *glutFont);
		virtual void *getGlutFont() const;

		virtual void enablePlane(enum PlanePosition pos);
		virtual void enablePlane(UInt pos);
		virtual void disablePlane(enum PlanePosition pos);
		virtual void disablePlane(UInt pos);
		virtual bool isAnyPlaneEnabled() const;
		virtual void setPlaneEnabled(enum PlanePosition pos, bool enabled);
		virtual void setPlaneEnabled(UInt pos, bool enabled);
		virtual bool isPlaneEnabled(enum PlanePosition pos) const;
		virtual bool isPlaneEnabled(UInt pos) const;
		virtual void setPlaneCoord(enum PlanePosition pos, Real coord);
		virtual void setPlaneCoord(UInt pos, Real coord);
		virtual Real getPlaneCoord(enum PlanePosition pos) const;
		virtual Real getPlaneCoord(UInt pos) const;

		virtual bool isInVisibleSpace(const GeometricShape *shape) const;
		virtual bool isInVisibleSpace(const GeometricShape &shape) const;
		virtual bool isInVisibleSpace(const Point *point) const;
		virtual bool isInVisibleSpace(const Point &point) const;

		virtual void setPlaneInclusive(bool inclusive);
		virtual bool isPlaneInclusive() const;

		virtual void setQualityRangeMin(Real min);
		virtual Real getQualityRangeMin() const;
		virtual void setQualityRangeMax(Real max);
		virtual Real getQualityRangeMax() const;

		virtual void setElementPickable(bool elementPickable);
		virtual bool isElementPickable() const;

		virtual void setFrontElementPickable(bool frontElementPickable);
		virtual bool isFrontElementPickable() const;

		virtual void setVertexPickable(bool vertexPickable);
		virtual bool isVertexPickable() const;

		virtual void clearDisplayList(UInt list, UInt numLists);
		virtual void clearDisplayLists();
#endif //#if USE_GUI

#if USE_GUI
	protected:

		struct Plane
		{
			bool enabled;
			Real coord;
		};

		struct DisplayList
		{
			UInt list;
			UInt numLists;
		};

		typedef std::list<DisplayList> DisplayListList;
#endif //#if USE_GUI

	protected:

		Real tolerance;
#if USE_BUFFER
		std::string buffer;
#endif //#if USE_BUFFER

#if USE_GUI
		//bool debug;
		bool stepByStep;
		bool animating;

		Box box;
#if USE_EXPERIMENTAL_3
		Point3D mid;
#else
		Point mid;
#endif //#if USE_EXPERIMENTAL_3

		Real shrink;
		Real explode;
		Real explodeExternal;
        Point3D midExternal;

		bool elementDrawn;
		bool frontElementDrawn;
		bool vertexDrawn;
		bool idDrawn;
		bool triangleBorderDrawn;
		bool normalByVertex;
		bool normalInverted;

		Real lineWidth;
		Real pointSize;

		mutable Real colors[ColoredType::SIZE][3];

		void *glutFont;

		struct Plane planes[COUNT];
		bool anyPlaneEnabled;
		bool planeInclusive;

		Real qualityRangeMin;
		Real qualityRangeMax;

		bool elementPickable;
		bool frontElementPickable;
		bool vertexPickable;

		DisplayListList clearedLists;
#endif //#if USE_GUI
	};
}

#endif //#ifndef _DATA_STATE_H_
