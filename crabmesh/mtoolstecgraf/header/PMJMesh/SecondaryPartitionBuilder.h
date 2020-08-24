#ifndef _PMJMESH_SECONDARY_PARTITION_BUILDER_H_
#define _PMJMESH_SECONDARY_PARTITION_BUILDER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/Builder.h"

namespace PMJMesh
{
	class SecondaryPartitionBuilder : public Performer::Builder
	{
	public:

		SecondaryPartitionBuilder(UInt dimension, Data::BoxTree *partitionTree,
			bool face = true, bool edge = true, bool vertex = true);
		virtual ~SecondaryPartitionBuilder();

		virtual void setDimension(UInt dimension);

		virtual void setPartitionTree(Data::BoxTree *partitionTree);

		virtual void setFace(bool face);
		virtual void setEdge(bool edge);
		virtual void setVertex(bool vertex);

		virtual Data::BoxList getFaceBoxes() const;
		virtual Data::BoxList getEdgeBoxes() const;
		virtual Data::BoxList getVertexBoxes() const;

		virtual Data::BoxList getSecondaryPartitions() const;

		virtual void execute();

	protected:

		virtual Data::Box *makeBox(const Data::Point &min, const Data::Point &max) const;

		virtual Data::Box *makeFaceBox(UInt coord, const Data::BoxTreeCell *c, bool greater) const;
		///TODO only works for two dimensions
		virtual Data::Box *makeEdgeBox(UInt coord, const Data::BoxTreeCell *c) const;

		virtual void makeFaceBoxes();
		virtual void makeEdgeBoxes();
		virtual void makeVertexBoxes();

	protected:

		Data::BoxTree *partitionTree;

		UInt dimension;

		Data::BoxList faceBoxes;
		Data::BoxList edgeBoxes;
		Data::BoxList vertexBoxes;

		bool face;
		bool edge;
		bool vertex;
	};
}

#endif //#ifndef _PMJMESH_SECONDARY_PARTITION_BUILDER_H_
