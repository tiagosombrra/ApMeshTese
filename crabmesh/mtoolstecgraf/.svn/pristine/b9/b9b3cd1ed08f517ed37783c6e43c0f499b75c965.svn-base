#include "Data/Identifiable.h"

#include "Data/OStream.h"

using namespace Data;

Data::Identifiable::Identifiable(ULInt id)
{
	this->setId(id);
}

Data::Identifiable::~Identifiable()
{

}

void Data::Identifiable::setId(ULInt id)
{
	this->id = id;

	/*if (this->id == 740)
	{
		Data::cout() << "Data::Identifiable " << Data::trace() << Data::endl;
	}*/
}

ULInt Data::Identifiable::getId() const
{
	return this->id;
}

bool Data::Identifiable::less(const Identifiable *i1, const Identifiable *i2)
{
	return i1->getId() < i2->getId();
}

bool Data::Identifiable::greater(const Identifiable *i1, const Identifiable *i2)
{
	return i1->getId() > i2->getId();
}

bool Data::Identifiable::equal(const Identifiable *i1, const Identifiable *i2)
{
	return i1->getId() == i2->getId();
}

bool Data::Identifiable::different(const Identifiable *i1, const Identifiable *i2)
{
	return i1->getId() != i2->getId();
}
