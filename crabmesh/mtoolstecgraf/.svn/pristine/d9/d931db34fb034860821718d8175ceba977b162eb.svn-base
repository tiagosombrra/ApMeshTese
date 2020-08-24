#ifndef _IO_FILE_MANIPULATOR_H
#define _IO_FILE_MANIPULATOR_H

#include "IO/Definitions.h"
#include "Performer/Builder.h"

namespace IO
{
	class FileManipulator :
		public Performer::Builder
	{
	public:

		FileManipulator();
		virtual ~FileManipulator();

		virtual void setFilename(const std::string &filename);
		virtual const std::string &getFilename() const;

		virtual void addFiletype(const std::string &filetype);
		virtual void removeFiletype(const std::string &filetype);

		virtual bool hasFiletype(const std::string &filetype) const;

		virtual bool isFiletype(const std::string &filetype) const;
		virtual bool isTypeCorrect() const;

	protected:

		std::string filename;

		StringList filetypes;
	};
}

#endif //#ifndef _IO_FILE_MANIPULATOR_H
