#ifndef _IO_GMSH_MSH_FILE_READER_H_
#define _IO_GMSH_MSH_FILE_READER_H_

#include "IO/MeshFileReader.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class GmshMshFileReader :
		public IO::MeshFileReader
	{
	public:

		GmshMshFileReader();
		virtual ~GmshMshFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

		virtual UInt getDimension() const;

	protected:

		virtual bool makeStructures();

		virtual Data::AbstractMesh *makeMesh() const;

		virtual void read();

		virtual bool readMsh();

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

		UInt dimension;
	};
}

#endif //#ifndef _IO_GMSH_MSH_FILE_READER_H_
