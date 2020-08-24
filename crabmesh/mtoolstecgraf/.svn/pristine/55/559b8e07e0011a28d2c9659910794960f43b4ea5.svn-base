#ifndef _DATA_FRONT_ELEMENT_AD_TREE_H_
#define _DATA_FRONT_ELEMENT_AD_TREE_H_

#include "Data/Definitions.h"
#include "Data/AlternatingDigitalTree.h"

namespace Data
{
	class FrontElementADTree : public Data::AlternatingDigitalTree
	{
	public:

		FrontElementADTree();
		virtual ~FrontElementADTree();

		using Data::AlternatingDigitalTree::build;
		virtual bool build(const FrontElementList &fels);

	protected:

		virtual void makeRoot();
	};
}

#endif //#ifndef _DATA_FRONT_ELEMENT_AD_TREE_H_
