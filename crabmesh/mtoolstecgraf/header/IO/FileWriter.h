#ifndef _IO_FILE_WRITER_H_
#define _IO_FILE_WRITER_H_

#include "IO/FileManipulator.h"

namespace IO
{
	class FileWriter :
		public IO::FileManipulator
	{
	public:

		FileWriter();
		virtual ~FileWriter();

		virtual void execute();

	protected:

		virtual bool areStructuresCorrect() const = 0;

		virtual void write() const = 0;

	protected:

		mutable std::ofstream file;
	};
}

#endif //#ifndef _IO_FILE_WRITER_H_
