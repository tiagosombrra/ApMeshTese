#include "Tools/Tool.h"

using namespace Tools;

Tools::Tool::Tool() :
	Performer::Builder::Builder()
{
	this->startMeasure();
	this->startTiming();
}

Tools::Tool::~Tool()
{

}

Data::UI2Tuple Tools::Tool::measure() const
{
#if USE_C__11
	return std::make_tuple(this->current, this->total);
#else
	Data::UI2Tuple t;

	t.values[0] = this->current;
	t.values[1] = this->total;

	return t;
#endif //#if USE_C__11
}

UInt Tools::Tool::measure(UInt i) const
{
	return (i == 0) ? this->current : this->total;
}

void Tools::Tool::startMeasure(UInt total) const
{
	this->setMeasure(0, total);
}

void Tools::Tool::setMeasure(UInt current, UInt total) const
{
	this->current = current;
	this->total = std::max(this->current, total);

	//std::cout << this->current << " of " << this->total << " (" << 100.0*static_cast<Real>(this->current)/static_cast<Real>(this->total) << "%)" << std::endl;
}

void Tools::Tool::addMeasure(UInt current, UInt total) const
{
	this->setMeasure(this->current + current, this->total + total);
}

void Tools::Tool::subtractMeasure(UInt current, UInt total) const
{
	this->setMeasure(this->current - current, this->total - total);
}

void Tools::Tool::endMeasure() const
{
	this->setMeasure(this->total, this->total);
}

void Tools::Tool::startTiming(UInt size) const
{
	this->count = 0;
#if USE_TIME_MEASUREMENTS
	this->tstart = this->tend = this->ttime = this->tstep = this->ttotal = 0.0;
#endif //#if USE_TIME_MEASUREMENTS
	this->size = size;
}

void Tools::Tool::timing(std::ostream *str, const std::string &message) const
{
	if (((++this->count)%500 == 0) || (this->count == 1) || (this->count == this->size))
	{
		(*str) << "testing ";

		if (!message.empty())
		{
			(*str) << message << " ";
		}

		(*str) << count << " of " << size
#if USE_TIME_MEASUREMENTS
			   << ", timings: current: " << Data::time(this->tstep)
			   << ", total: " << Data::time(this->total);

		if (this->ttime > 0.0)
		{
			(*str) << ", estimated: " << Data::time(this->ttime*static_cast<Real>(this->size - this->count)/2.0);
		}

		(*str) << " (hh:mm:ss)" << std::endl;
#else
			   << std::endl;
#endif //#if USE_TIME_MEASUREMENTS
	}

#if USE_TIME_MEASUREMENTS
	this->tend = Data::time();

	if (this->tstart > 0.0)
	{
		this->ttime = this->tend - this->tstart;
		this->tstep += this->ttime;
		this->ttotal += this->ttime;
	}

	this->tstart = this->tend;
#endif //#if USE_TIME_MEASUREMENTS
}

void Tools::Tool::timing(const std::string &message) const
{
	this->timing(&std::cout, message);
}
