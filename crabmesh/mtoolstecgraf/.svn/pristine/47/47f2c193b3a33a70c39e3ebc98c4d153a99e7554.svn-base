#include "IO/MJMeshFileWriter.h"

#include "Data/Boundary.h"

IO::MJMeshFileWriter::MJMeshFileWriter() :
	IO::MeshFileWriter::MeshFileWriter()
{
	this->dimension = 0;
	this->treeFactor = 0.0;
	this->tolerance = 0.0;

	this->addFiletype(".2djmsh");
	this->addFiletype(".3djmsh");
}

IO::MJMeshFileWriter::~MJMeshFileWriter()
{

}

void IO::MJMeshFileWriter::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void IO::MJMeshFileWriter::setTreeFactor(Real treeFactor)
{
	this->treeFactor = treeFactor;
}

void IO::MJMeshFileWriter::setTolerance(Real tolerance)
{
	this->tolerance = tolerance;
}

void IO::MJMeshFileWriter::write() const
{
	if (((this->isFiletype(this->filetypes.front())) && (this->dimension == 2)) ||
		((this->isFiletype(this->filetypes.back())) && (this->dimension == 3)))
	{
		this->writeMJMesh();
	}
}

void IO::MJMeshFileWriter::writeMJMesh() const
{
	file << this->treeFactor << std::endl;
	file << this->tolerance << std::endl;

	//Data::VertexList vertices = static_cast<const Data::Boundary *>(this->mesh)->getVertices();

	//file << vertices.size() << std::endl;
	file << static_cast<const Data::Boundary *>(this->mesh)->verticesSize() << std::endl;

	if (this->dimension == 2)
    {
    	/*//Data::FrontElementList elements = static_cast<const Data::Boundary *>(this->mesh)->getElements();
		//Data::FrontElementList::iterator iterEl = elements.begin();
		Data::Boundary::ConstFrontElementIterator iterEl = static_cast<const Data::Boundary *>(this->mesh)->eBegin();

		//Data::VertexList::iterator first = vertices.begin();
		Data::Boundary::ConstVertexIterator first = static_cast<const Data::Boundary *>(this->mesh)->vBegin();

    	//for (Data::VertexList::iterator iter = vertices.begin();
    	for (Data::Boundary::ConstVertexIterator iter = static_cast<const Data::Boundary *>(this->mesh)->vBegin();
			 //iter != vertices.end(); iter++, iterEl++)
			 iter != static_cast<const Data::Boundary *>(this->mesh)->vEnd(); iter++, iterEl++)
    	{
			file << (*iter)->getId() << " ";
			file << (*iter)->getPoint()->getCoord(0) << " ";
    		file << (*iter)->getPoint()->getCoord(1) << std::endl;

    		if ((*iterEl)->getVertex(1) == (*first))
    		{
    			file << "-1" << std::endl;

    			first = iter;
    			first++;
    		}
    	}*/

    	Data::FrontElementList elements = static_cast<const Data::Boundary *>(this->mesh)->getElements();

		Data::Vertex *first = NULL;

    	bool last = false;

    	Data::VertexSet done;

    	while (!elements.empty())
		{
			Data::FrontElement *e = elements.front();

			elements.pop_front();

			if (!first)
			{
				first = e->getVertex(0);
			}

			file << e->getVertex(0)->getId() << " ";
			file << e->getVertex(0)->getPoint()->getCoord(0) << " ";
			file << e->getVertex(0)->getPoint()->getCoord(1) << std::endl;

			done.insert(e->getVertex(0));

			if (last)
			{
				last = false;

				first = NULL;

				file << "-1" << std::endl;

				continue;
			}

			Data::FrontElementList::iterator iter = elements.end();

			for (iter = elements.begin(); iter != elements.end(); iter++)
			{
				if ((*iter)->getVertex(0) == e->getVertex(1))
				{
					if ((*iter)->getVertex(1) == first)
					{
						last = true;
					}

					break;
				}
			}

			if ((iter != elements.begin()) &&
				(iter != elements.end()))
			{
				e = (*iter);

				elements.erase(iter);

				elements.push_front(e);
			}
		}

		//dangling vertices
		for (Data::Boundary::ConstVertexIterator iter = static_cast<const Data::Boundary *>(this->mesh)->vBegin();
			 iter != static_cast<const Data::Boundary *>(this->mesh)->vEnd(); iter++)
		{
			if (done.find((*iter)) == done.end())
			{
				file << (*iter)->getId() << " ";
				file << (*iter)->getPoint()->getCoord(0) << " ";
				file << (*iter)->getPoint()->getCoord(1) << std::endl;
				file << "-1" << std::endl;
			}
		}
    }
    else if (this->dimension == 3)
    {
		//for (Data::VertexList::iterator iter = vertices.begin();
		for (Data::Boundary::ConstVertexIterator iter = static_cast<const Data::Boundary *>(this->mesh)->vBegin();
			 //iter != vertices.end(); iter++)
			 iter != static_cast<const Data::Boundary *>(this->mesh)->vEnd(); iter++)
    	{
			file << (*iter)->getId() << " ";
			file << (*iter)->getPoint()->getCoord(0) << " ";
    		file << (*iter)->getPoint()->getCoord(1) << " ";
    		file << (*iter)->getPoint()->getCoord(2) << std::endl;
    	}

    	//Data::FrontElementList elements = static_cast<const Data::Boundary *>(this->mesh)->getElements();

    	//file << elements.size() << std::endl;
    	file << static_cast<const Data::Boundary *>(this->mesh)->size() << std::endl;

		//for (Data::FrontElementList::iterator iter = elements.begin();
		for (Data::Boundary::ConstFrontElementIterator iter = static_cast<const Data::Boundary *>(this->mesh)->eBegin();
			 //iter != elements.end(); iter++)
			 iter != static_cast<const Data::Boundary *>(this->mesh)->eEnd(); iter++)
    	{
			file << (*iter)->getId() << " ";
			file << (*iter)->getVertex(0)->getId() << " ";
    		file << (*iter)->getVertex(1)->getId() << " ";
    		file << (*iter)->getVertex(2)->getId() << std::endl;
    	}
    }
}
