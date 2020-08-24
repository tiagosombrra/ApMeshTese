#ifndef _PERFORMER_TREE_CELL_CLASSIFIER_H_
#define _PERFORMER_TREE_CELL_CLASSIFIER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class TreeCellClassifier : public Performer::Builder
	{
	public:

		TreeCellClassifier(const Data::Boundary *boundary, const Data::BoxTree *tree);
		//TreeCellClassifier(const Data::Front *front, const Data::BoxTree *tree);
		virtual ~TreeCellClassifier();

		virtual void setBoundary(const Data::Boundary *boundary);

		//virtual void setFront(const Data::Front *front);

		virtual void setTree(const Data::BoxTree *tree);

		virtual const Data::PositionMap &getPosition() const;

		virtual enum Data::Position classify(const Data::BoxTreeCell *cell,
			//const Data::FrontElementList &elements) = 0;
			const Data::FrontElementList *elements) = 0;

		virtual void execute();

#if USE_GUI
		static void setColor(const Data::BoxTreeCell *cell, enum Data::Position pos);
		static void setPosition(Data::PositionMap &position, const Data::BoxTreeCell *cell, enum Data::Position pos);
#endif //#if USE_GUI

	protected:

		//virtual Data::FrontElementList findElements() const;
		virtual const Data::FrontElementList *findElements() const;

		virtual void setPosition(const Data::BoxTreeCell *cell, enum Data::Position pos);

		virtual void setChildrenPosition(const Data::BoxTreeCell *cell, enum Data::Position pos);

	protected:

		const Data::Boundary *boundary;
		//const Data::Front *front;

		const Data::BoxTree *tree;

		Data::PositionMap position;
	};
}

#endif //#ifndef _PERFORMER_TREE_CELL_CLASSIFIER_H_
