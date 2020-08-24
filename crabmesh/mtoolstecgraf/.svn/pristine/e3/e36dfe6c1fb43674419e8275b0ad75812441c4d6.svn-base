#ifndef _IO_JMESH_FILE_WRITER_H_
#define _IO_JMESH_FILE_WRITER_H_

#include "IO/MeshFileWriter.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class JmeshFileWriter :
		public IO::MeshFileWriter
	{
	public:

		JmeshFileWriter();
		virtual ~JmeshFileWriter();

		virtual void setTreeFactor(Real treeFactor);
		virtual void setTolerance(Real tolerance);

	protected:

		virtual void write() const;

		virtual void writeDMsh() const;
		virtual void writeJMsh() const;
		virtual void writeWMsh() const;

	protected:

		Real treeFactor;
		Real tolerance;
	};
}

#endif //#ifndef _IO_JMESH_FILE_WRITER_H_
