#include "IO/FileManipulator.h"

IO::FileManipulator::FileManipulator() :
	Performer::Builder::Builder()
{

}

IO::FileManipulator::~FileManipulator()
{

}

void IO::FileManipulator::setFilename(const std::string &filename)
{
	this->filename = filename;
}

const std::string &IO::FileManipulator::getFilename() const
{
	return this->filename;
}

void IO::FileManipulator::addFiletype(const std::string &filetype)
{
	this->filetypes.push_back(filetype);
}

void IO::FileManipulator::removeFiletype(const std::string &filetype)
{
	this->filetypes.remove(filetype);
}

bool IO::FileManipulator::hasFiletype(const std::string &filetype) const
{
	return (std::find(this->filetypes.begin(), this->filetypes.end(), filetype) != this->filetypes.end());
}

bool IO::FileManipulator::isFiletype(const std::string &filetype) const
{
	return (this->filename.substr(this->filename.find_last_of('.')) == filetype);
}

bool IO::FileManipulator::isTypeCorrect() const
{
	std::string type = this->filename.substr(this->filename.find_last_of('.'));

	if (type.size() == 0)
	{
		return false;
	}

	return this->hasFiletype(type);
}
