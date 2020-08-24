#include "Tools/ExternalTool.h"

Tools::ExternalTool::ExternalTool(const Tool *tool) :
	Tools::Tool::Tool()
{
	this->setTool(tool);
}

Tools::ExternalTool::~ExternalTool()
{

}

void Tools::ExternalTool::setTool(const Tool *tool) const
{
	this->tool = tool;

	if (this->tool)
	{
		this->setMeasure(this->tool->measure(0), this->tool->measure(1));
	}
}

void Tools::ExternalTool::setMeasure(UInt current, UInt total) const
{
	Tools::Tool::setMeasure(current, total);

	if (this->tool)
	{
		this->tool->setMeasure(this->current, this->total);
	}
}
