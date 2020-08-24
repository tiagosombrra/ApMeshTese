#ifndef _DATA_VERTEX_AD_TREE_H_
#define _DATA_VERTEX_AD_TREE_H_

#include "Data/Definitions.h"
#include "Data/AlternatingDigitalTree.h"

namespace Data
{
	class VertexADTree : public Data::AlternatingDigitalTree
	{
	public:

		VertexADTree();
		virtual ~VertexADTree();

		using Data::AlternatingDigitalTree::build;
		virtual bool build(const VertexList &vertices);

	protected:

		virtual void makeRoot();
	};
}

#endif //#ifndef _DATA_VERTEX_AD_TREE_H_
