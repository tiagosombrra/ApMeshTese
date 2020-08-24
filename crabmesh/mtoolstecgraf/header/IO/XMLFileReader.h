#ifndef _IO_XML_FILE_READER_H_
#define _IO_XML_FILE_READER_H_

#include "IO/MeshFileReader.h"

#if USE_TINYXML
#include <tinyxml.h>

#include "MJMesh/Definitions.h"
#include "TecGraf/Definitions.h"
#include "TecGraf/Model.h"

namespace IO
{
	class XMLFileReader :
		public IO::MeshFileReader
	{
	public:

		XMLFileReader();
		virtual ~XMLFileReader();

		virtual void setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder);

#if USE_TECGRAF
		virtual void setForceModel(bool forceModel);
#endif //#if USE_TECGRAF

		virtual bool isBoundary() const;
		virtual bool isMesh() const;

#if USE_TECGRAF
		virtual TecGraf::Model *getModel() const;
#endif //#if USE_TECGRAF

		virtual bool makeStructures();

	protected:

		virtual Data::AbstractMesh *makeMesh() const;
		virtual Data::AbstractMesh *makeMesh(bool boundary) const;

#if USE_TECGRAF
		virtual TecGraf::Model *makeModel() const;
#endif //#if USE_TECGRAF

		virtual void read();

		virtual bool readXML();
		//virtual bool readBSM();
		virtual bool readBSM(TiXmlElement *bsmio);

		//virtual std::string extract(const std::string &s) const;
		//virtual ULInt extractULInt(const std::string &s) const;
		//virtual Real extractReal(const std::string &s) const;

	protected:

		MJMesh::BoundaryBuilder *boundaryBuilder;

#if USE_TECGRAF
		TecGraf::Model *model;

		bool forceModel;
#endif //#if USE_TECGRAF

		bool boundary;
		bool ismesh;

	};
}
#endif //#if USE_TINYXML

#endif //#ifndef _IO_XML_FILE_READER_H_
