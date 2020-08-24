#ifndef _PERFORMER_ANGLE_TREE_CELL_CLASSIFIER_H_
#define _PERFORMER_ANGLE_TREE_CELL_CLASSIFIER_H_

#include "Performer/Definitions.h"
#include "Performer/TreeCellClassifier.h"

namespace Performer
{
	class AngleTreeCellClassifier : public TreeCellClassifier
	{
	public:

		AngleTreeCellClassifier(UInt dimension,
			const Data::Boundary *boundary, const Data::BoxTree *tree);
		/*AngleTreeCellClassifier(UInt dimension,
			const Data::Front *front, const Data::BoxTree *tree);*/
		virtual ~AngleTreeCellClassifier();

		virtual void setDimension(UInt dimension);

		virtual enum Data::Position classify(const Data::BoxTreeCell *cell,
			//const Data::FrontElementList &elements);
			const Data::FrontElementList *elements);

	protected:

		virtual void classifyChildren(const Data::BoxTreeCell *cell,
			//const Data::FrontElementList &elements);
			const Data::FrontElementList *elements);

		virtual enum Data::Position classify(Data::Point *p,
			//const Data::FrontElementList &elements, bool canBeUnknown = false) const;
			const Data::FrontElementList *elements, bool canBeUnknown = false) const;

		virtual AnglePointInObjectDecider *makeApiod(Data::Point *p,
			const Data::FrontElementList *elements, bool canBeUnknown) const = 0;

		virtual Data::Point *mid(const Data::BoxTreeCell *cell) const;

#if USE_EXPERIMENTAL_3
		virtual Data::Point2DObjList clip2D(const Data::Box *box, const Data::GeometricShape *s) const;
		virtual Data::Point3DObjList clip3D(const Data::Box *box, const Data::GeometricShape *s) const;
#else
		virtual Data::PointObjList clip(const Data::Box *box, const Data::GeometricShape *s) const;
#endif //#if USE_EXPERIMENTAL_3
		//this method tests if the given cell outside the front
		virtual bool out(const Data::Box &box, const Data::FrontElement *e) const;
		//this method tests if a box is inside a boundary defined by the given
		// front element list. a box classified as inside if either it crosses
		// the boundary or its centroid is inside the boundary; otherwise, it is
		// classified as outside. the method used for classifying the centroid
		// is the classical point in polygon (polyhedra) problem, solved by, in
		// this case, the angle method. this method returns true if the box
		// crosses the boundary, returning false otherwise. the sum of the
		// angles is returned by the parameter angle. the intercepting parameter
		// returns the front elements in elements whose bounding box intercept
		// the given cell
		//virtual bool on(const Data::BoxTreeCell *cell, const Data::FrontElementList &elements,
		virtual bool on(const Data::BoxTreeCell *cell, const Data::FrontElementList *elements,
						Data::FrontElementList &intercepting, bool clipTest = true) const;
		//virtual bool on(const Data::BoxTreeCell *cell, const Data::FrontElementList &elements,
		virtual bool on(const Data::BoxTreeCell *cell, const Data::FrontElementList *elements,
						bool clipTest = true) const;
		//this method is the same as the above, but removes from the input elements
		//  the ones that belong exclusively to the cell (have all its points inside
		//  the cell
		virtual bool onRemoving(const Data::BoxTreeCell *cell, Data::FrontElementList &elements,
						Data::FrontElementList &intercepting, bool clipTest = true) const;
		virtual bool onRemoving(const Data::BoxTreeCell *cell, Data::FrontElementList &elements,
						bool clipTest = true) const;

	protected:

		UInt dimension;
	};
}

#endif //#ifndef _PERFORMER_ANGLE_TREE_CELL_CLASSIFIER_H_
