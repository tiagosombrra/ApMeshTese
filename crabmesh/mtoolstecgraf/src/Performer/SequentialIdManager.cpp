#include "Performer/SequentialIdManager.h"

using namespace Performer;

Performer::SequentialIdManager::SequentialIdManager(ULInt id, UInt size)
	: Performer::IdManager(id, size)
{

}

Performer::SequentialIdManager::~SequentialIdManager()
{

}

ULInt Performer::SequentialIdManager::next(UInt i)
{
	return this->ids[i]++;
}

ULInt Performer::SequentialIdManager::next()
{
	return this->next(0);
}
