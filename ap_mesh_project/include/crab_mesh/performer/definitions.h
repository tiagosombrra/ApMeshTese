#ifndef _PERFORMER_DEFINITIONS_H_
#define _PERFORMER_DEFINITIONS_H_

#include <memory>

#include "../../definitions.h"

namespace Performer {
using Data::Int;
using Data::LInt;
using Data::LReal;
using Data::Real;
using Data::Short;
using Data::UInt;
using Data::ULInt;
using Data::UShort;

class IdManager;

typedef std::vector<std::shared_ptr<IdManager>> IdManagerVector;
}  // namespace Performer

#endif  // #ifndef _PERFORMER_DEFINITIONS_H_
