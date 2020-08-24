#ifndef _IO_GEN_FILE_WRITER_H_
#define _IO_GEN_FILE_WRITER_H_

#include "IO/MeshFileWriter.h"
#include "MJMesh/Definitions.h"

namespace IO
{
	class GenFileWriter :
		public IO::MeshFileWriter
	{
    public:
        enum Type
        {
            BOUNDARY = 0,
            VOLUME,
            SURFACE
        };

	public:

		GenFileWriter();
		virtual ~GenFileWriter();

		virtual void setType(enum Type type);

		static bool write(const std::string &filename, const Data::Boundary *boundary, const Data::Mesh *mesh);

	protected:

		virtual bool areStructuresCorrect() const;

		virtual void write() const;

		virtual void write(Data::Vertex *v) const;
		virtual void write(Data::Element *e) const;

		virtual void writeGen() const;

	protected:

		enum Type type;

		mutable UInt dimension;
	};
}

#endif //#ifndef _IO_GEN_FILE_WRITER_H_
