#ifndef _IO_TEXT_FILE_READER_H_
#define _IO_TEXT_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class TextMeshFileReader :
		public IO::MeshFileReader
	{
	public:

		TextMeshFileReader();
		virtual ~TextMeshFileReader();

		virtual UInt getDimension() const;

	protected:

		virtual Data::AbstractMesh *makeMesh() const;

		virtual void read();

		virtual bool readText();

	protected:

		UInt dimension;
	};
}

#endif //#ifndef _IO_TEXT_FILE_READER_H_
