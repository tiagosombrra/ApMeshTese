#include "IO/FileWriter.h"

IO::FileWriter::FileWriter() :
	IO::FileManipulator::FileManipulator()
{

}

IO::FileWriter::~FileWriter()
{

}

void IO::FileWriter::execute()
{
	if (!this->isTypeCorrect())
	{
		return;
	}

	if (!this->areStructuresCorrect())
	{
		return;
	}

	this->file.open(this->filename.c_str());

	this->file.setf(std::ios::fixed, std::ios::floatfield);
	this->file.precision(6);

	this->write();

	this->file.flush();
	this->file.close();
}
