#include "PMJMesh/TreeTransferable.h"

#include "Parallel/Communicator.h"
#include "PMJMesh/QuadTree.h"
#include "PMJMesh/QuadTreeCell.h"
#include "PMJMesh/OctTree.h"
#include "PMJMesh/OctTreeCell.h"

using namespace PMJMesh;

PMJMesh::TreeTransferable::TreeTransferable() :
	Parallel::Transferable(TRANSFERABLE_TREE)
{
	this->setDimension(0);
	this->setTree(NULL);
}

PMJMesh::TreeTransferable::TreeTransferable(UInt dimension, Data::BoxTree *tree) :
		Parallel::Transferable(TRANSFERABLE_TREE)
{
	this->setDimension(dimension);
	this->setTree(tree);
}

PMJMesh::TreeTransferable::~TreeTransferable()
{

}

void PMJMesh::TreeTransferable::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

UInt PMJMesh::TreeTransferable::getDimension() const
{
	return this->dimension;
}

void PMJMesh::TreeTransferable::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}

Data::BoxTree *PMJMesh::TreeTransferable::getTree() const
{
	return this->tree;
}

UInt PMJMesh::TreeTransferable::packSize(const Parallel::Communicator *comm, ULInt numCells) const
{
	UInt size = 0;

	//std::stringstream str;

	/*str << "rank " << comm->rank() << ", pack size, numCells = " << numCells << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	// unsigned - type, dimension
	size += comm->packSize(2, Parallel::Communicator::UINT);

	// unsigned long - numCells
	size += comm->packSize(1, Parallel::Communicator::ULINT);

	/*str << "rank " << comm->rank() << ", pack size, size = " << size << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	// bool - positioned
	size += comm->packSize(1, Parallel::Communicator::BOOL);

	/*str << "rank " << comm->rank() << ", pack size, size = " << size << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	bool positioned = (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot())) != Data::UNKNOWN);

	// linearized tree
	size += comm->packSize(static_cast<UInt>(std::ceil(positioned ? static_cast<Real>(numCells)/4.0 : static_cast<Real>(numCells)/8.0)), Parallel::Communicator::CHAR);

	/*str << "rank " << comm->rank() << ", pack size, size = " << size << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	return size;
}

UInt PMJMesh::TreeTransferable::packSize(const Parallel::Communicator *comm) const
{
	if (!this->tree)
	{
		return comm->packSize(1, Parallel::Communicator::UINT);
	}

	return this->packSize(comm, this->tree->numNodes());
}

void PMJMesh::TreeTransferable::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
	const Data::BoxTree *tree) const
{
	//std::stringstream str;

	comm->pack(this->type, buffer, size, position);

	/*str << "rank " << comm->rank() << ", pack, type = " << this->type << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	if (!tree)
	{
		return;
	}

	comm->pack(this->dimension, buffer, size, position);

	/*str << "rank " << comm->rank() << ", pack, dimension = " << this->dimension << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	comm->pack(tree->numNodes(), buffer, size, position);

	/*str << "rank " << comm->rank() << ", pack, numCells = " << tree->numNodes() << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	/*//debug
	if (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(tree->getRoot())) == Data::UNKNOWN)
	{
		std::cout << "root " << tree->getRoot()->getId() << " is UNKNOWN in process " << comm->rank() << std::endl;
	}
	//endebug*/

	bool positioned = (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(tree->getRoot())) != Data::UNKNOWN);

	comm->pack(positioned, buffer, size, position);

	/*str << "rank " << comm->rank() << ", pack, positioned = " << std::boolalpha << positioned << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	Data::GraphNodeList stack;

	stack.push_back(tree->getRoot());

	UShort used = 0;
	unsigned char c = 0;

	UShort max = 8;
	unsigned char mask[max];

	for (UInt i = 0; i < max; i++)
	{
		// 0 -> 128
		// 1 -> 64
		// 2 -> 32
		// 3 -> 16
		// 4 -> 4
		// 5 -> 8
		// 6 -> 2
		// 7 -> 1

		mask[i] = static_cast<unsigned char>(std::pow(2.0, max - i - 1));
	}

	if (positioned)
	{
		// 1 1 -> subdivided
		// 1 0 -> not subdivided, on
		// 0 1 -> not subdivided, in
		// 0 0 -> not subdivided, out

		while (!stack.empty())
		{
			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(stack.back());

			stack.pop_back();

			/*str << "rank " << comm->rank() << ", pack, cell = " << cell->getId() << std::endl;
			std::cout << str.str();
			str.str(std::string());
			str.clear();*/

			if (cell->isSubdivided())
			{
				c = static_cast<unsigned char>(c + mask[used]);
				c = static_cast<unsigned char>(c + mask[used + 1]);

				/*str << "rank " << comm->rank() << ", pack, cell = " << cell->getId() << ", subdivided" << std::endl;
				std::cout << str.str();
				str.str(std::string());
				str.clear();*/
			}
			else
			{
				enum Data::Position pos = PMJMesh::getPosition(this->dimension, cell);

				/*//debug
				if (pos == Data::UNKNOWN)
				{
					std::cout << "cell " << cell->getId() << " is UNKNOWN in process " << comm->rank() << std::endl;
				}
				//endebug*/

				if (pos == Data::ON)
				{
					c = static_cast<unsigned char>(c + mask[used]);

					/*str << "rank " << comm->rank() << ", pack, cell = " << cell->getId() << ", ON" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();*/
				}
				else if (pos == Data::IN)
				{
					c = static_cast<unsigned char>(c + mask[used + 1]);

					/*str << "rank " << comm->rank() << ", pack, cell = " << cell->getId() << ", IN" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();*/
				}
				/*else
				{
					str << "rank " << comm->rank() << ", pack, cell = " << cell->getId() << ", OUT" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();
				}*/
			}

			used = static_cast<unsigned char>(used + 2);

			if (used == max)
			{
				comm->pack(c, buffer, size, position);

				/*str << "rank " << comm->rank() << ", pack, c = " << static_cast<int>(c) << std::endl;
				std::cout << str.str();
				str.str(std::string());
				str.clear();*/

				c = 0;
				used = 0;
			}

			for (UInt i = 0; i < cell->numChildren(); i++)
			{
				stack.push_back(cell->child(cell->numChildren() - i - 1));
			}
		}
	}
	else
	{
		//1 -> subdivided
		//0 -> not subdivided

		while (!stack.empty())
		{
			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(stack.back());

			stack.pop_back();

			if (cell->isSubdivided())
			{
				c = static_cast<unsigned char>(c + mask[used]);
			}

			used++;

			if (used == max)
			{
				comm->pack(c, buffer, size, position);

				c = 0;
				used = 0;
			}

			for (UInt i = 0; i < cell->numChildren(); i++)
			{
				stack.push_back(cell->child(cell->numChildren() - i - 1));
			}
		}
	}

	if (used != 0)
	{
		comm->pack(c, buffer, size, position);
	}
}

void PMJMesh::TreeTransferable::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	if (!this->tree)
	{
		comm->pack(this->type, buffer, size, position);

		return;
	}

	this->pack(comm, buffer, size, position, this->tree);
}

Parallel::Package PMJMesh::TreeTransferable::pack(const Parallel::Communicator *comm) const
{
	Int size = 0;
	char *buffer = NULL;
	Int position = 0;

	if (!this->tree)
	{
		size += this->packSize(comm);

		buffer = new char[size];

		this->pack(comm, buffer, size, position);

		Parallel::Package p(size, buffer);

		return p;
	}

	size = this->packSize(comm, this->tree->numNodes());

	buffer = new char[size];

	this->pack(comm, buffer, size, position, this->tree);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh::TreeTransferable::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	//std::stringstream str;

	Int size = p.first;
	char *buffer = p.second;

	if (size == comm->packSize(1, Parallel::Communicator::UINT))
	{
		if (this->tree)
		{
			delete this->tree;

			this->tree = NULL;
		}

		return;
	}

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
	}

	/*str << "rank " << comm->rank() << ", unpack, type = " << this->type << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	comm->unpack(this->dimension, buffer, size, position);

	/*str << "rank " << comm->rank() << ", unpack, dimension = " << this->dimension << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	ULInt numCells;

	comm->unpack(numCells, buffer, size, position);

	/*str << "rank " << comm->rank() << ", unpack, numCells = " << numCells << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	bool positioned;

	comm->unpack(positioned, buffer, size, position);

	/*str << "rank " << comm->rank() << ", unpack, positioned = " << std::boolalpha << positioned << std::endl;
	std::cout << str.str();
	str.str(std::string());
	str.clear();*/

	if (this->dimension == 2)
	{
		this->tree = new PMJMesh::QuadTree(Data::Point2D(), Data::Point2D(), 0);
	}
	else
	{
		this->tree = new PMJMesh::OctTree(Data::Point3D(), Data::Point3D(), 0);
	}

	UShort used = 8;
	unsigned char c = 0;

	UShort max = 8;
	unsigned char mask[max];

	for (UInt i = 0; i < max; i++)
	{
		// 0 -> 128
		// 1 -> 64
		// 2 -> 32
		// 3 -> 16
		// 4 -> 4
		// 5 -> 8
		// 6 -> 2
		// 7 -> 1

		mask[i] = static_cast<unsigned char>(std::pow(2.0, max - i - 1));
	}

	Data::GraphNodeList stack;

	stack.push_back(this->tree->getRoot());

	if (positioned)
	{
		for (ULInt i = 0; i < numCells; i++)
		{
			if (used == 8)
			{
				comm->unpack(c, buffer, size, position);

				/*str << "rank " << comm->rank() << ", unpack, c = " << static_cast<int>(c) << std::endl;
				std::cout << str.str();
				str.str(std::string());
				str.clear();*/

				used = 0;
			}

			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(stack.back());

			stack.pop_back();

			/*str << "rank " << comm->rank() << ", unpack, cell = " << cell->getId() << std::endl;
			std::cout << str.str();
			str.str(std::string());
			str.clear();*/

			unsigned char m1 = mask[used++];
			unsigned char m2 = mask[used++];

			bool b1 = (c >= m1);

			if (b1)
			{
				c = static_cast<unsigned char>(c - m1);
			}

			bool b2 = (c >= m2);

			if (b2)
			{
				c = static_cast<unsigned char>(c - m2);
			}

			if (b1)
			{
				if (b2)
				{
					/*str << "rank " << comm->rank() << ", unpack, cell = " << cell->getId() << ", subdivided" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();*/

					cell->subdivide();

					for (UInt i = 0; i < cell->numChildren(); i++)
					{
						stack.push_back(cell->child(cell->numChildren() - i - 1));
					}
				}
				else
				{
					/*str << "rank " << comm->rank() << ", unpack, cell = " << cell->getId() << ", ON" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();*/

					PMJMesh::setPosition(this->dimension, cell, Data::ON);
				}
			}
			else
			{
				if (b2)
				{
					/*str << "rank " << comm->rank() << ", unpack, cell = " << cell->getId() << ", IN" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();*/

					PMJMesh::setPosition(this->dimension, cell, Data::IN);
				}
				else
				{
					/*str << "rank " << comm->rank() << ", unpack, cell = " << cell->getId() << ", OUT" << std::endl;
					std::cout << str.str();
					str.str(std::string());
					str.clear();*/

					PMJMesh::setPosition(this->dimension, cell, Data::OUT);
				}
			}
		}

		if (this->dimension == 2)
		{
			static_cast<PMJMesh::QuadTree *>(this->tree)->adjustPosition();
		}
		else
		{
			static_cast<PMJMesh::OctTree *>(this->tree)->adjustPosition();
		}
	}
	else
	{
		for (ULInt i = 0; i < numCells; i++)
		{
			if (used == 8)
			{
				comm->unpack(c, buffer, size, position);

				used = 0;
			}

			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(stack.back());

			stack.pop_back();

			unsigned char m = mask[used++];

			bool subdivided = (c >= m);

			if (subdivided)
			{
				cell->subdivide();

				for (UInt i = 0; i < cell->numChildren(); i++)
				{
					stack.push_back(cell->child(cell->numChildren() - i - 1));
				}

				c = static_cast<unsigned char>(c - m);
			}
		}
	}
}
