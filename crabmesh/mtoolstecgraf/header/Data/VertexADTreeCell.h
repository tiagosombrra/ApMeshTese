#ifndef _DATA_VERTEX_AD_TREE_CELL_H_
#define _DATA_VERTEX_AD_TREE_CELL_H_

#include "Data/Definitions.h"
#include "Data/AlternatingDigitalTreeCell.h"

namespace Data
{
	class VertexADTreeCell : public Data::AlternatingDigitalTreeCell
	{
	public:

		VertexADTreeCell(VertexADTreeCell *parent = NULL);
		virtual ~VertexADTreeCell();

		using Data::AlternatingDigitalTreeCell::build;
		virtual void build(const IdentifiableList &shapes);
		virtual void build(const VertexList &vertices);

		using Data::AlternatingDigitalTreeCell::add;
		using Data::AlternatingDigitalTreeCell::remove;

		virtual bool add(const Identifiable *s);
		virtual bool remove(const Identifiable *s);

		using Data::AlternatingDigitalTreeCell::inRange;
		using Data::AlternatingDigitalTreeCell::operator=;

	protected:

		virtual AlternatingDigitalTreeCell *makeChild();
		virtual void set(const Identifiable *shape, const Point *pt, const Box *box);
		virtual void set(const AlternatingDigitalTreeCell *other);

		virtual void inRange(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const;

		virtual void adjustGlobalWithBoxOrMid();

	};
}

#endif //#define _DATA_VERTEX_AD_TREE_CELL_H_
