#include "IO/FileReader.h"

IO::FileReader::FileReader() :
	IO::FileManipulator::FileManipulator()
{

}

IO::FileReader::~FileReader()
{

}

void IO::FileReader::execute()
{
	this->file.open(this->filename.c_str());

	if (!file.is_open())
	{
		return;
	}

	if (!this->isTypeCorrect())
	{
		return;
	}

	if (!this->makeStructures())
	{
		return;
	}

	if (!this->file.is_open())
	{
	    return;
	}

	this->read();

	this->file.close();
}
