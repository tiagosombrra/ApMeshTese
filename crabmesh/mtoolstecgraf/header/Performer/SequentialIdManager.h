#ifndef _PERFORMER_SEQUENTIAL_ID_MANAGER_H_
#define _PERFORMER_SEQUENTIAL_ID_MANAGER_H_

#include "Performer/Definitions.h"
#include "Performer/IdManager.h"

namespace Performer
{
	class SequentialIdManager : public Performer::IdManager
	{
	public:

		SequentialIdManager(ULInt id = 1, UInt size = 1);
		virtual ~SequentialIdManager();

		virtual ULInt next(UInt i);
		virtual ULInt next();
	};
}

#endif //#ifndef _PERFORMER_SEQUENTIAL_ID_MANAGER_H_
