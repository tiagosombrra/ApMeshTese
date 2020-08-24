#ifndef _IO_MESH_FILE_READER_H_
#define _IO_MESH_FILE_READER_H_

#include "IO/FileReader.h"

namespace IO
{
	class MeshFileReader :
		public IO::FileReader
	{
	public:

		MeshFileReader();
		virtual ~MeshFileReader();

		virtual Data::AbstractMesh *getMesh() const;

	protected:

		virtual bool makeStructures();

		virtual Data::AbstractMesh *makeMesh() const = 0;

	protected:

		Data::AbstractMesh *mesh;
	};
}

#endif //#ifndef _IO_MESH_FILE_READER_H_
