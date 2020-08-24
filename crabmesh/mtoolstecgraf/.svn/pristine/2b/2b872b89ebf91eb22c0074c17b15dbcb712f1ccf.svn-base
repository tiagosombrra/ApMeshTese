#ifndef _IO_DEFINITIONS_H_
#define _IO_DEFINITIONS_H_

#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#if USE_TINYXML
#include <tinyxml.h>
#endif //#if USE_TINYXML

#include "Data/Definitions.h"

namespace IO
{
	using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	class FileManipulator;

	class FileReader;
	class FileWriter;

	class MeshFileReader;
	class MeshFileWriter;

	class MJMeshFileReader;
	class MJMeshFileWriter;

	class JmeshFileReader;
	class JmeshFileWriter;

	class TextMeshFileReader;
	class TextMeshFileWriter;

	class NeutralFileReader;
	class NeutralFileWriter;

	class GmshMshFileReader;

	class STLFileReader;
	class VTKFileReader;
	class CDBFileReader;
	class OFFFileReader;
#if USE_TINYXML
	class XMLFileReader;
#endif //#if USE_TINYXML

	class AbaqusFileReader;
	class AbaqusFileWriter;

	class GenFileReader;
	class GenFileWriter;

#if USE_TECGRAF_MSH_2D
	class TecGrafWarpMeshFileReader;
#endif //#if USE_TECGRAF_MSH_2D

	typedef std::list<std::string> StringList;
}

#endif //#ifndef _IO_DEFINITIONS_H_
