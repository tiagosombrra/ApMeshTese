#ifndef _DATA_LIST_DISPLAYABLE_H_
#define _DATA_LIST_DISPLAYABLE_H_

#include "Data/Definitions.h"

#if USE_GUI

#include "Data/Colorable.h"

namespace Data
{
	class ListDisplayable : public Data::Colorable
	{
	public:

		ListDisplayable(Real r = 1.0, Real g = 1.0, Real b = 1.0);
		virtual ~ListDisplayable();

		virtual void clearList(UInt list = 0) const;
		virtual void clearLists() const;

		virtual void createList(UInt numLists = 1) const;
		virtual void beginList(UInt list = 0) const;
		virtual void endList() const;

		virtual void drawList(UInt list = 0) const;

		virtual bool hasList(UInt list = 0) const;
		virtual bool hasLists() const;

		virtual void setFrozen(bool frozen) const;
		virtual bool isFrozen() const;
		virtual void freeze() const;
		virtual void unfreeze() const;

		virtual bool isOutdated() const;
		virtual bool isDrawing() const;

		virtual void setR(Real r) const;
		virtual void setG(Real g) const;
		virtual void setB(Real b) const;
		virtual void highlight() const;
		virtual void unhighlight() const;

		virtual ULInt getDrawingMeasure() const;
		virtual ULInt getDrawingTotal() const;

	protected:

		virtual void addPendingClear() const;
		virtual void addPendingClear(UInt list) const;
		virtual void resolvePendingClear() const;

		virtual void outdate() const;
		virtual void update() const;

	protected:

		mutable UInt list;
		mutable BoolVector lists;
		mutable BoolVector outdated;
		mutable BoolVector pending;
		mutable bool frozen;
		mutable volatile bool drawing;

		mutable ULInt drawingMeasure;
		mutable ULInt drawingTotal;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _DATA_LIST_DISPLAYABLE_H_
