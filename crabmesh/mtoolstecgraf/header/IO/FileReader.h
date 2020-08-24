#ifndef _IO_FILE_READER_H_
#define _IO_FILE_READER_H_

#include "IO/FileManipulator.h"

namespace IO
{
	class FileReader :
		public IO::FileManipulator
	{
	public:

		FileReader();
		virtual ~FileReader();

		virtual void execute();

	protected:

		virtual bool makeStructures() = 0;

		virtual void read() = 0;

		template <typename T, int size = sizeof(T)>
        T binaryRead(bool invertEnd = false);

	protected:

		std::ifstream file;
	};
}

template <typename T, int size>
T IO::FileReader::binaryRead(bool invertEnd)
{
    union U
	{
		T t;
		char c[size];
	};

	U u;

	this->file.read(u.c, size);

	if ((invertEnd) &&
        ((typeid(T) == typeid(float)) ||
		(typeid(T) == typeid(double)) ||
		(typeid(T) == typeid(long double))))
	{
		for (int i = 0; i < size/2; i++)
		{
			std::swap(u.c[i], u.c[size - i - 1]);
		}
	}

	return u.t;
}

#endif //#ifndef _IO_FILE_READER_H_
