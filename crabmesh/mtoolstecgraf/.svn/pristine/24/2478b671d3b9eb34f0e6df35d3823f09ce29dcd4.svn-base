#include "Data/State.h"

#include "Data/Point.h"
#include "Data/GeometricShape.h"
#include "Data/OStream.h"

using namespace Data;

Data::State::State()
{
	this->tolerance = 1.e-4;
#if USE_GUI
	//this->debug = false;
	this->stepByStep = false;
	this->animating = false;
	this->shrink = 1.0;
	this->explode = 1.0;
	this->explodeExternal = 1.0;

	this->elementDrawn = true;
	this->frontElementDrawn = true;
	this->vertexDrawn = true;
	this->idDrawn = false;
	this->triangleBorderDrawn = true;
	this->normalByVertex = false;
	this->normalInverted = true;

#if USE_SCREENSHOT
	this->lineWidth = 3.0;
    this->pointSize = 1.0;
#else
	this->lineWidth = 1.0;
    this->pointSize = 2.0;
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[UNKNOWN][0] = 0.0;
    this->colors[UNKNOWN][1] = 0.0;
    this->colors[UNKNOWN][2] = 0.0;
#else
    this->colors[UNKNOWN][0] = 1.0;
    this->colors[UNKNOWN][1] = 1.0;
    this->colors[UNKNOWN][2] = 1.0;
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[BOUNDARY_VERTEX][0] = 0.0;
    this->colors[BOUNDARY_VERTEX][1] = 0.0;
    this->colors[BOUNDARY_VERTEX][2] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][0] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][1] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][2] = 0.0;
#else
    this->colors[BOUNDARY_VERTEX][0] = 1.0;
    this->colors[BOUNDARY_VERTEX][1] = 1.0;
    this->colors[BOUNDARY_VERTEX][2] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][0] = 1.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][1] = 1.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][2] = 0.0;

    /*this->colors[BOUNDARY_VERTEX][0] = 0.0;
    this->colors[BOUNDARY_VERTEX][1] = 0.0;
    this->colors[BOUNDARY_VERTEX][2] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][0] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][1] = 0.0;
    this->colors[BOUNDARY_FRONT_ELEMENT][2] = 0.0;*/
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[MESH_VERTEX][0] = 0.0;
    this->colors[MESH_VERTEX][1] = 0.0;
    this->colors[MESH_VERTEX][2] = 0.0;
    this->colors[MESH_FRONT_ELEMENT][0] = 0.0;
    this->colors[MESH_FRONT_ELEMENT][1] = 0.0;
    this->colors[MESH_FRONT_ELEMENT][2] = 0.0;
    this->colors[MESH_ELEMENT][0] = 0.0;
    this->colors[MESH_ELEMENT][1] = 0.0;
    this->colors[MESH_ELEMENT][2] = 0.0;
#else
    this->colors[MESH_VERTEX][0] = 1.0;
    this->colors[MESH_VERTEX][1] = 1.0;
    this->colors[MESH_VERTEX][2] = 1.0;
    this->colors[MESH_FRONT_ELEMENT][0] = 1.0;
    this->colors[MESH_FRONT_ELEMENT][1] = 1.0;
    this->colors[MESH_FRONT_ELEMENT][2] = 1.0;
    this->colors[MESH_ELEMENT][0] = 1.0;
    this->colors[MESH_ELEMENT][1] = 1.0;
    this->colors[MESH_ELEMENT][2] = 1.0;

    /*this->colors[MESH_VERTEX][0] = 0.5;
    this->colors[MESH_VERTEX][1] = 0.5;
    this->colors[MESH_VERTEX][2] = 0.5;
    this->colors[MESH_FRONT_ELEMENT][0] = 0.5;
    this->colors[MESH_FRONT_ELEMENT][1] = 0.5;
    this->colors[MESH_FRONT_ELEMENT][2] = 0.5;
    this->colors[MESH_ELEMENT][0] = 0.5;
    this->colors[MESH_ELEMENT][1] = 0.5;
    this->colors[MESH_ELEMENT][2] = 0.5;*/

    /*this->colors[MESH_VERTEX][0] = 0.0;
    this->colors[MESH_VERTEX][1] = 0.0;
    this->colors[MESH_VERTEX][2] = 0.0;
    this->colors[MESH_FRONT_ELEMENT][0] = 0.0;
    this->colors[MESH_FRONT_ELEMENT][1] = 0.0;
    this->colors[MESH_FRONT_ELEMENT][2] = 0.0;
    this->colors[MESH_ELEMENT][0] = 0.0;
    this->colors[MESH_ELEMENT][1] = 0.0;
    this->colors[MESH_ELEMENT][2] = 0.0;*/
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[MESH_ELEMENT_QUALITY_0][0] = 0.1;
    this->colors[MESH_ELEMENT_QUALITY_0][1] = 0.1;
    this->colors[MESH_ELEMENT_QUALITY_0][2] = 0.1;
    this->colors[MESH_ELEMENT_QUALITY_1][0] = 0.2;
    this->colors[MESH_ELEMENT_QUALITY_1][1] = 0.2;
    this->colors[MESH_ELEMENT_QUALITY_1][2] = 0.2;
    this->colors[MESH_ELEMENT_QUALITY_2][0] = 0.3;
    this->colors[MESH_ELEMENT_QUALITY_2][1] = 0.3;
    this->colors[MESH_ELEMENT_QUALITY_2][2] = 0.3;
    this->colors[MESH_ELEMENT_QUALITY_3][0] = 0.4;
    this->colors[MESH_ELEMENT_QUALITY_3][1] = 0.4;
    this->colors[MESH_ELEMENT_QUALITY_3][2] = 0.4;
    this->colors[MESH_ELEMENT_QUALITY_4][0] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_4][1] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_4][2] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_5][0] = 0.6;
    this->colors[MESH_ELEMENT_QUALITY_5][1] = 0.6;
    this->colors[MESH_ELEMENT_QUALITY_5][2] = 0.6;
    this->colors[MESH_ELEMENT_QUALITY_6][0] = 0.7;
    this->colors[MESH_ELEMENT_QUALITY_6][1] = 0.7;
    this->colors[MESH_ELEMENT_QUALITY_6][2] = 0.7;
    this->colors[MESH_ELEMENT_QUALITY_7][0] = 0.8;
    this->colors[MESH_ELEMENT_QUALITY_7][1] = 0.8;
    this->colors[MESH_ELEMENT_QUALITY_7][2] = 0.8;
    this->colors[MESH_ELEMENT_QUALITY_8][0] = 0.9;
    this->colors[MESH_ELEMENT_QUALITY_8][1] = 0.9;
    this->colors[MESH_ELEMENT_QUALITY_8][2] = 0.9;
    this->colors[MESH_ELEMENT_QUALITY_9][0] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_9][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_9][2] = 1.0;
#else
    this->colors[MESH_ELEMENT_QUALITY_0][0] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_0][1] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_0][2] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_1][0] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_1][1] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_1][2] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_2][0] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_2][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_2][2] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_3][0] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_3][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_3][2] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_4][0] = 0.25;
    this->colors[MESH_ELEMENT_QUALITY_4][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_4][2] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_5][0] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_5][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_5][2] = 0.25;
    this->colors[MESH_ELEMENT_QUALITY_6][0] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_6][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_6][2] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_7][0] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_7][1] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_7][2] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_8][0] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_8][1] = 0.5;
    this->colors[MESH_ELEMENT_QUALITY_8][2] = 1.0;
    this->colors[MESH_ELEMENT_QUALITY_9][0] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_9][1] = 0.0;
    this->colors[MESH_ELEMENT_QUALITY_9][2] = 1.0;
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[FRONT_VERTEX][0] = 0.0;
    this->colors[FRONT_VERTEX][1] = 0.0;
    this->colors[FRONT_VERTEX][2] = 0.0;
    this->colors[FRONT_BOUNDARY_FRONT_ELEMENT][0] = 0.0;
    this->colors[FRONT_BOUNDARY_FRONT_ELEMENT][1] = 0.0;
    this->colors[FRONT_BOUNDARY_FRONT_ELEMENT][2] = 0.0;
    this->colors[FRONT_ELEMENT_FRONT_ELEMENT][0] = 0.0;
    this->colors[FRONT_ELEMENT_FRONT_ELEMENT][1] = 0.0;
    this->colors[FRONT_ELEMENT_FRONT_ELEMENT][2] = 0.0;
    this->colors[FRONT_REJECTED_FRONT_ELEMENT][0] = 0.0;
    this->colors[FRONT_REJECTED_FRONT_ELEMENT][1] = 0.0;
    this->colors[FRONT_REJECTED_FRONT_ELEMENT][2] = 0.0;
    this->colors[FRONT_CROSSING_FRONT_ELEMENT][0] = 0.0;
    this->colors[FRONT_CROSSING_FRONT_ELEMENT][1] = 0.0;
    this->colors[FRONT_CROSSING_FRONT_ELEMENT][2] = 0.0;
#else
    this->colors[FRONT_VERTEX][0] = 0.0;
    this->colors[FRONT_VERTEX][1] = 0.0;
    this->colors[FRONT_VERTEX][2] = 1.0;
    this->colors[FRONT_BOUNDARY_FRONT_ELEMENT][0] = 0.5;
    this->colors[FRONT_BOUNDARY_FRONT_ELEMENT][1] = 1.0;
    this->colors[FRONT_BOUNDARY_FRONT_ELEMENT][2] = 0.5;
    this->colors[FRONT_ELEMENT_FRONT_ELEMENT][0] = 0.0;
    this->colors[FRONT_ELEMENT_FRONT_ELEMENT][1] = 0.0;
    this->colors[FRONT_ELEMENT_FRONT_ELEMENT][2] = 1.0;
    this->colors[FRONT_REJECTED_FRONT_ELEMENT][0] = 1.0;
    this->colors[FRONT_REJECTED_FRONT_ELEMENT][1] = 0.5;
    this->colors[FRONT_REJECTED_FRONT_ELEMENT][2] = 0.5;
    this->colors[FRONT_CROSSING_FRONT_ELEMENT][0] = 0.5;
    this->colors[FRONT_CROSSING_FRONT_ELEMENT][1] = 0.5;
    this->colors[FRONT_CROSSING_FRONT_ELEMENT][2] = 1.0;
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[TREE_CELL_DEFAULT][0] = 0.0;
    this->colors[TREE_CELL_DEFAULT][1] = 0.0;
    this->colors[TREE_CELL_DEFAULT][2] = 0.0;
    this->colors[TREE_CELL_IN][0] = 0.0;
    this->colors[TREE_CELL_IN][1] = 0.0;
    this->colors[TREE_CELL_IN][2] = 0.0;
    this->colors[TREE_CELL_ON][0] = 0.5;
    this->colors[TREE_CELL_ON][1] = 0.5;
    this->colors[TREE_CELL_ON][2] = 0.5;
    this->colors[TREE_CELL_OUT][0] = 1.0;
    this->colors[TREE_CELL_OUT][1] = 1.0;
    this->colors[TREE_CELL_OUT][2] = 1.0;
    this->colors[TREE_CELL_UNKNOWN][0] = 0.25;
    this->colors[TREE_CELL_UNKNOWN][1] = 0.25;
    this->colors[TREE_CELL_UNKNOWN][2] = 0.25;
#else
    this->colors[TREE_CELL_DEFAULT][0] = 1.0;
    this->colors[TREE_CELL_DEFAULT][1] = 1.0;
    this->colors[TREE_CELL_DEFAULT][2] = 1.0;
    this->colors[TREE_CELL_IN][0] = 0.0;
    this->colors[TREE_CELL_IN][1] = 1.0;
    this->colors[TREE_CELL_IN][2] = 0.0;
    this->colors[TREE_CELL_ON][0] = 1.0;
    this->colors[TREE_CELL_ON][1] = 1.0;
    this->colors[TREE_CELL_ON][2] = 0.0;
    this->colors[TREE_CELL_OUT][0] = 1.0;
    this->colors[TREE_CELL_OUT][1] = 0.0;
    this->colors[TREE_CELL_OUT][2] = 0.0;
    this->colors[TREE_CELL_UNKNOWN][0] = 0.0;
    this->colors[TREE_CELL_UNKNOWN][1] = 0.0;
    this->colors[TREE_CELL_UNKNOWN][2] = 1.0;
#endif //#if USE_SCREENSHOT

#if USE_SCREENSHOT
    this->colors[PARTITION][0] = 0.0;
    this->colors[PARTITION][1] = 0.0;
    this->colors[PARTITION][2] = 0.0;
#else
    this->colors[PARTITION][0] = 1.0;
    this->colors[PARTITION][1] = 1.0;
    this->colors[PARTITION][2] = 1.0;

    /*this->colors[PARTITION][0] = 0.0;
    this->colors[PARTITION][1] = 0.0;
    this->colors[PARTITION][2] = 0.0;*/
#endif //#if USE_SCREENSHOT

	this->glutFont = NULL;

	for (UInt i = 0; i < COUNT; i++)
	{
		this->planes[i].enabled = false;
		this->planes[i].coord = 0.0;
	}

	this->anyPlaneEnabled = false;
	this->planeInclusive = false;

	this->qualityRangeMin = 0.0;
	this->qualityRangeMax = 1.0;

	this->elementPickable = true;
	this->frontElementPickable = true;
	this->vertexPickable = true;
#endif //#if USE_GUI
}

Data::State::~State()
{
#if USE_GUI
	this->clearDisplayLists();
#endif //#if USE_GUI
}

/*void Data::State::enableDebug()
{
    this->setDebug(true);
}

void Data::State::disableDebug()
{
    this->setDebug(false);
}

bool Data::State::toggleDebug()
{
    this->setDebug(!this->isDebug());

    return this->isDebug();
}

void Data::State::setDebug(bool debug)
{
    this->debug = debug;
}

bool Data::State::isDebug() const
{
    return this->debug;
}*/

void Data::State::setTolerance(Real tolerance)
{
    //Data::cout() << "changing tolerance from " << this->tolerance << " to " << tolerance << " at " << Data::trace() << Data::endl;

	this->tolerance = tolerance;
}

Real Data::State::getTolerance() const
{
	return this->tolerance;
}

#if USE_BUFFER
void Data::State::setBuffer(const std::string &buffer)
{
    this->buffer = buffer;
}

const std::string &Data::State::getBuffer() const
{
    return this->buffer;
}
#endif //#if USE_BUFFER

#if USE_GUI
void Data::State::setStepByStep(bool stepByStep)
{
	this->stepByStep = stepByStep;
}

bool Data::State::isStepByStep() const
{
	return this->stepByStep;
}

void Data::State::setAnimating(bool animating)
{
	this->animating = animating;
}

bool Data::State::isAnimating() const
{
	return this->animating;
}

void Data::State::setBox(const Box &box)
{
	this->box = box;

	if (this->box.getMin())
	{
		this->planes[LEFT].coord = this->box.getMin()->getCoord(0);
		this->planes[BOTTOM].coord = this->box.getMin()->getCoord(1);
		this->planes[BACK].coord = this->box.getMin()->getCoord(2);
	}

	if (this->box.getMax())
	{
		this->planes[RIGHT].coord = this->box.getMax()->getCoord(0);
		this->planes[TOP].coord = this->box.getMax()->getCoord(1);
		this->planes[FRONT].coord = this->box.getMax()->getCoord(2);
	}
}

const Box &Data::State::getBox() const
{
	return this->box;
}

void Data::State::setMid(const Point &mid)
{
	this->mid = mid;

	this->resetMidExternal();
}

const Point &Data::State::getMid() const
{
	return this->mid;
}

void Data::State::setShrink(Real shrink)
{
	this->shrink = shrink;
}

Real Data::State::getShrink() const
{
	return this->shrink;
}

void Data::State::setExplode(Real explode)
{
	this->explode = explode;
}

Real Data::State::getExplode() const
{
	return this->explode;
}

void Data::State::setExplodeExternal(Real explodeExternal)
{
    this->explodeExternal = explodeExternal;
}

Real Data::State::getExplodeExternal() const
{
    return this->explodeExternal;
}

void Data::State::setMidExternal(const Data::Point3D &midExternal)
{
    this->midExternal.setCoord(0, midExternal.getCoord(0) - this->mid.getCoord(0));
    this->midExternal.setCoord(1, midExternal.getCoord(1) - this->mid.getCoord(1));
    this->midExternal.setCoord(2, midExternal.getCoord(2) - this->mid.getCoord(2));
}

const Data::Point3D &Data::State::getMidExternal() const
{
    return this->midExternal;
}

void Data::State::resetMidExternal()
{
    this->midExternal.setCoord(0, 0.0);
    this->midExternal.setCoord(1, 0.0);
    this->midExternal.setCoord(2, 0.0);
}

void Data::State::setElementDrawn(bool elementDrawn)
{
	this->elementDrawn = elementDrawn;
}

bool Data::State::isElementDrawn() const
{
	return this->elementDrawn;
}

void Data::State::setFrontElementDrawn(bool frontElementDrawn)
{
	this->frontElementDrawn = frontElementDrawn;
}

bool Data::State::isFrontElementDrawn() const
{
	return this->frontElementDrawn;
}

void Data::State::setVertexDrawn(bool vertexDrawn)
{
	this->vertexDrawn = vertexDrawn;
}

bool Data::State::isVertexDrawn() const
{
	return this->vertexDrawn;
}

void Data::State::setIdDrawn(bool idDrawn)
{
	this->idDrawn = idDrawn;
}

bool Data::State::isIdDrawn() const
{
	return this->idDrawn;
}

void Data::State::setTriangleBorderDrawn(bool triangleBorderDrawn)
{
    this->triangleBorderDrawn = triangleBorderDrawn;
}

bool Data::State::isTriangleBorderDrawn() const
{
    return this->triangleBorderDrawn;
}

void Data::State::setNormalByVertex(bool normalByVertex)
{
    this->normalByVertex = normalByVertex;
}

bool Data::State::isNormalByVertex() const
{
    return this->normalByVertex;
}

void Data::State::setLineWidth(Real width)
{
    this->lineWidth = width;
}

Real Data::State::getLineWidth() const
{
    return this->lineWidth;
}

Real Data::State::getLineWidth(bool highlighted)
{
    return highlighted ? this->getLineWidth() + 1.0 : this->getLineWidth();
}

void Data::State::setPointSize(Real size)
{
    this->pointSize = size;
}

Real Data::State::getPointSize() const
{
    return this->pointSize;
}

Real Data::State::getPointSize(bool highlighted)
{
#if USE_SCREENSHOT
	return highlighted ? this->getPointSize() + 4.0 : this->getPointSize();
#else
	return highlighted ? this->getPointSize() + 3.0 : this->getPointSize();
#endif //#if USE_SCREENSHOT
}

void Data::State::setTypeColor(enum ColoredType type, Real r, Real g, Real b) const
{
    if (type >= ColoredType::SIZE)
    {
        return;
    }

    this->colors[type][0] = r;
    this->colors[type][1] = g;
    this->colors[type][2] = b;
}

void Data::State::getTypeColor(enum ColoredType type, Real &r, Real &g, Real &b) const
{
    if (type >= ColoredType::SIZE)
    {
        return;
    }

    r = this->colors[type][0];
    g = this->colors[type][1];
    b = this->colors[type][2];
}

Real Data::State::getTypeColorR(enum ColoredType type) const
{
    return (type >= ColoredType::SIZE) ? 0.0 : this->colors[type][0];
}

Real Data::State::getTypeColorG(enum ColoredType type) const
{
    return (type >= ColoredType::SIZE) ? 0.0 : this->colors[type][1];
}

Real Data::State::getTypeColorB(enum ColoredType type) const
{
    return (type >= ColoredType::SIZE) ? 0.0 : this->colors[type][2];
}

void Data::State::setNormalInverted(bool normalInverted)
{
    this->normalInverted = normalInverted;
}

bool Data::State::isNormalInverted() const
{
    return this->normalInverted;
}

void Data::State::setGlutFont(void *glutFont)
{
	if ((glutFont == GLUT_BITMAP_9_BY_15) ||
		(glutFont == GLUT_BITMAP_8_BY_13) ||
		(glutFont == GLUT_BITMAP_TIMES_ROMAN_10) ||
		(glutFont == GLUT_BITMAP_TIMES_ROMAN_24) ||
		(glutFont == GLUT_BITMAP_HELVETICA_10) ||
		(glutFont == GLUT_BITMAP_HELVETICA_12) ||
		(glutFont == GLUT_BITMAP_HELVETICA_18))
	{
		this->glutFont = glutFont;
	}
	else
	{
		this->glutFont = NULL;
	}
}

void *Data::State::getGlutFont() const
{
	return this->glutFont;
}

void Data::State::enablePlane(enum PlanePosition pos)
{
	this->setPlaneEnabled(pos, true);
}

void Data::State::enablePlane(UInt pos)
{
	if (pos < State::COUNT)
	{
		this->enablePlane(static_cast<State::PlanePosition>(pos));
	}
}

void Data::State::disablePlane(enum PlanePosition pos)
{
	this->setPlaneEnabled(pos, false);
}

void Data::State::disablePlane(UInt pos)
{
	if (pos < State::COUNT)
	{
		this->disablePlane(static_cast<State::PlanePosition>(pos));
	}
}

bool Data::State::isAnyPlaneEnabled() const
{
	return this->anyPlaneEnabled;
}

void Data::State::setPlaneEnabled(enum PlanePosition pos, bool enabled)
{
	this->planes[pos].enabled = enabled;

	if (enabled)
	{
		this->anyPlaneEnabled = true;
	}
	else
	{
		this->anyPlaneEnabled = false;

		for (UInt i = 0; (i < COUNT) && (!this->anyPlaneEnabled); i++)
		{
			//this->anyPlaneEnabled ||= this->planes[i].enabled;
			this->anyPlaneEnabled = this->anyPlaneEnabled || this->planes[i].enabled;
		}
	}
}

void Data::State::setPlaneEnabled(UInt pos, bool enabled)
{
	if (pos < State::COUNT)
	{
		this->setPlaneEnabled(static_cast<State::PlanePosition>(pos), enabled);
	}
}

bool Data::State::isPlaneEnabled(enum PlanePosition pos) const
{
	return this->planes[pos].enabled;
}

bool Data::State::isPlaneEnabled(UInt pos) const
{
	return (pos < State::COUNT) ? this->isPlaneEnabled(static_cast<State::PlanePosition>(pos)) : false;
}

void Data::State::setPlaneCoord(enum PlanePosition pos, Real coord)
{
	this->planes[pos].coord = coord;
}

void Data::State::setPlaneCoord(UInt pos, Real coord)
{
	if (pos < State::COUNT)
	{
		this->setPlaneCoord(static_cast<State::PlanePosition>(pos), coord);
	}
}

Real Data::State::getPlaneCoord(enum PlanePosition pos) const
{
	return this->planes[pos].coord;
}

Real Data::State::getPlaneCoord(UInt pos) const
{
	return (pos < State::COUNT) ? this->getPlaneCoord(static_cast<State::PlanePosition>(pos)) : false;
}

void Data::State::setPlaneInclusive(bool inclusive)
{
	this->planeInclusive = inclusive;
}

bool Data::State::isPlaneInclusive() const
{
	return this->planeInclusive;
}

bool Data::State::isInVisibleSpace(const GeometricShape *shape) const
{
	return this->isAnyPlaneEnabled() ? shape->isInVisibleSpace() : true;
}

bool Data::State::isInVisibleSpace(const GeometricShape &shape) const
{
	return this->isInVisibleSpace(&shape);
}

bool Data::State::isInVisibleSpace(const Point *point) const
{
	return this->isAnyPlaneEnabled() ? point->isInVisibleSpace() : true;
}

bool Data::State::isInVisibleSpace(const Point &point) const
{
	return this->isInVisibleSpace(&point);
}

void Data::State::setQualityRangeMin(Real min)
{
	this->qualityRangeMin = min;
}

Real Data::State::getQualityRangeMin() const
{
	return this->qualityRangeMin;
}

void Data::State::setQualityRangeMax(Real max)
{
	this->qualityRangeMax = max;
}

Real Data::State::getQualityRangeMax() const
{
	return this->qualityRangeMax;
}

void Data::State::setElementPickable(bool elementPickable)
{
	this->elementPickable = elementPickable;
}

bool Data::State::isElementPickable() const
{
	return this->elementPickable;
}

void Data::State::setFrontElementPickable(bool frontElementPickable)
{
	this->frontElementPickable = frontElementPickable;
}

bool Data::State::isFrontElementPickable() const
{
	return this->frontElementPickable;
}

void Data::State::setVertexPickable(bool vertexPickable)
{
	this->vertexPickable = vertexPickable;
}

bool Data::State::isVertexPickable() const
{
	return this->vertexPickable;
}

void Data::State::clearDisplayList(UInt list, UInt numLists)
{
	DisplayList dl;

	dl.list = list;
	dl.numLists = numLists;

	this->clearedLists.push_back(dl);
}

void Data::State::clearDisplayLists()
{
	while (!this->clearedLists.empty())
	{
		DisplayList dl = this->clearedLists.front();

		this->clearedLists.pop_front();

		glDeleteLists(dl.list, dl.numLists);
	}
}
#endif //#if USE_GUI
