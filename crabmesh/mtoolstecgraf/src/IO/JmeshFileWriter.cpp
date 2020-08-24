#include "IO/JmeshFileWriter.h"

#include "Data/Boundary.h"

IO::JmeshFileWriter::JmeshFileWriter() :
	IO::MeshFileWriter::MeshFileWriter()
{
	this->treeFactor = 0.0;
	this->tolerance = 0.0;

	this->addFiletype(".dmsh");
	this->addFiletype(".jmsh");
	this->addFiletype(".wmsh");
}

IO::JmeshFileWriter::~JmeshFileWriter()
{

}

void IO::JmeshFileWriter::setTreeFactor(Real treeFactor)
{
	this->treeFactor = treeFactor;
}

void IO::JmeshFileWriter::setTolerance(Real tolerance)
{
	this->tolerance = tolerance;
}

void IO::JmeshFileWriter::write() const
{
	if (this->isFiletype(this->filetypes.front()))
	{
		this->writeDMsh();
	}
	else if (this->isFiletype(*(++this->filetypes.begin())))
	{
		this->writeJMsh();
	}
	else if (this->isFiletype(this->filetypes.back()))
	{
		this->writeWMsh();
	}
}

void IO::JmeshFileWriter::writeDMsh() const
{
	file << this->treeFactor << std::endl;
    file << Data::getTolerance() << std::endl;
    file << 1 << std::endl;
    file << 1 << std::endl;

    //Data::VertexList vertices = static_cast<const Data::Boundary *>(this->mesh)->getVertices();

	//file << vertices.size() << std::endl;
	file << static_cast<const Data::Boundary *>(this->mesh)->verticesSize() << std::endl;

#if USE_C__11
	typedef std::unordered_map<ULInt, ULInt> ULIntULIntHashMap;
#else
	typedef std::map<ULInt, ULInt> ULIntULIntMap;
#endif //#endif //#if USE_C__11

#if USE_C__11
	ULIntULIntHashMap mapVertices;
#else
	ULIntULIntMap mapVertices;
#endif //#if USE_C__11

	ULInt idVertices = 0;

	//for (Data::VertexList::iterator iter = vertices.begin();
	for (Data::Boundary::ConstVertexIterator iter = static_cast<const Data::Boundary *>(this->mesh)->vBegin();
		 //iter != vertices.end(); iter++)
		 iter != static_cast<const Data::Boundary *>(this->mesh)->vEnd(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		mapVertices[(*iter)->getId()] = ++idVertices;

		file << p->getCoord(0) << " ";
		file << p->getCoord(1) << " ";
		file << p->getCoord(2) << std::endl;
	}

	//Data::FrontElementList elements = static_cast<const Data::Boundary *>(this->mesh)->getElements();

	//file << elements.size() << std::endl;
	file << static_cast<const Data::Boundary *>(this->mesh)->size() << std::endl;

	//for (Data::FrontElementList::iterator iter = elements.begin();
	for (Data::Boundary::ConstFrontElementIterator iter = static_cast<const Data::Boundary *>(this->mesh)->eBegin();
		 //iter != elements.end(); iter++)
		 iter != static_cast<const Data::Boundary *>(this->mesh)->eEnd(); iter++)
	{
		file << mapVertices[(*iter)->getVertex(0)->getId()] << " ";
		file << mapVertices[(*iter)->getVertex(1)->getId()] << " ";
		file << mapVertices[(*iter)->getVertex(2)->getId()] << std::endl;
	}
}

void IO::JmeshFileWriter::writeJMsh() const
{
	file << this->treeFactor << std::endl;
    file << Data::getTolerance() << std::endl;
    file << 1 << std::endl;
    file << 1 << std::endl;

    //Data::VertexList vertices = static_cast<const Data::Boundary *>(this->mesh)->getVertices();

	//file << vertices.size() << std::endl;
	file << static_cast<const Data::Boundary *>(this->mesh)->verticesSize() << std::endl;

#if USE_C__11
	typedef std::unordered_map<ULInt, ULInt> ULIntULIntHashMap;
#else
	typedef std::map<ULInt, ULInt> ULIntULIntMap;
#endif //#endif //#if USE_C__11

#if USE_C__11
	ULIntULIntHashMap mapVertices;
#else
	ULIntULIntMap mapVertices;
#endif //#if USE_C__11

	ULInt idVertices = 0;

	//for (Data::VertexList::iterator iter = vertices.begin();
	for (Data::Boundary::ConstVertexIterator iter = static_cast<const Data::Boundary *>(this->mesh)->vBegin();
		 //iter != vertices.end(); iter++)
		 iter != static_cast<const Data::Boundary *>(this->mesh)->vEnd(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		mapVertices[(*iter)->getId()] = ++idVertices;

		file << idVertices << " ";
		file << p->getCoord(0) << " ";
		file << p->getCoord(1) << " ";
		file << p->getCoord(2) << std::endl;
	}

	//Data::FrontElementList elements = static_cast<const Data::Boundary *>(this->mesh)->getElements();

	//file << elements.size() << std::endl;
	file << static_cast<const Data::Boundary *>(this->mesh)->size() << std::endl;

	//for (Data::FrontElementList::iterator iter = elements.begin();
	for (Data::Boundary::ConstFrontElementIterator iter = static_cast<const Data::Boundary *>(this->mesh)->eBegin();
		 //iter != elements.end(); iter++)
		 iter != static_cast<const Data::Boundary *>(this->mesh)->eEnd(); iter++)
	{
		file << mapVertices[(*iter)->getVertex(0)->getId()] << " ";
		file << mapVertices[(*iter)->getVertex(1)->getId()] << " ";
		file << mapVertices[(*iter)->getVertex(2)->getId()] << std::endl;
	}
}

void IO::JmeshFileWriter::writeWMsh() const
{
	//Data::VertexList vertices = static_cast<const Data::Boundary *>(this->mesh)->getVertices();
    //Data::FrontElementList elements = static_cast<const Data::Boundary *>(this->mesh)->getElements();

	//file << vertices.size() << " " << elements.size() << std::endl;
	file << static_cast<const Data::Boundary *>(this->mesh)->verticesSize() << " " << static_cast<const Data::Boundary *>(this->mesh)->size() << std::endl;

	//for (Data::VertexList::iterator iter = vertices.begin();
	for (Data::Boundary::ConstVertexIterator iter = static_cast<const Data::Boundary *>(this->mesh)->vBegin();
		 //iter != vertices.end(); iter++)
		 iter != static_cast<const Data::Boundary *>(this->mesh)->vEnd(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		file << (*iter)->getId() << " ";
		file << p->getCoord(0) << " ";
		file << p->getCoord(1) << " ";
		file << p->getCoord(2) << std::endl;
	}

	//for (Data::FrontElementList::iterator iter = elements.begin();
	for (Data::Boundary::ConstFrontElementIterator iter = static_cast<const Data::Boundary *>(this->mesh)->eBegin();
		 //iter != elements.end(); iter++)
		 iter != static_cast<const Data::Boundary *>(this->mesh)->eEnd(); iter++)
	{
		file << (*iter)->getId() << " ";
		file << (*iter)->getVertex(0)->getId() << " ";
		file << (*iter)->getVertex(2)->getId() << " ";
		file << (*iter)->getVertex(1)->getId() << std::endl;
	}
}
