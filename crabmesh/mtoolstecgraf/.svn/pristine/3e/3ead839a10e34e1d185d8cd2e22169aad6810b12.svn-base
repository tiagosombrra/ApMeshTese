#include "MTools/TreeGenerator.h"

#if USE_MTOOLS

#include "Data/Point3D.h"
#include "Data/Box.h"
#include "Data/OctTree.h"

MTools::TreeGenerator::TreeGenerator(const FTools::OctTree *ftree) :
	Performer::TreeGenerator()
{
	this->setFTree(ftree);
}

MTools::TreeGenerator::~TreeGenerator()
{

}

void MTools::TreeGenerator::setFTree(const FTools::OctTree *ftree)
{
	this->ftree = ftree;
}

void MTools::TreeGenerator::execute()
{
	if (!this->ftree)
	{
		return;
	}

	//*
	Data::Point3D min, max;

	this->graph = this->makeTree(min, max);

	Data::GraphNodeList cells;

	cells.push_back(static_cast<Data::OctTree *>(this->graph)->getRoot());

	TreeGeneratorList gen;
	gen.generator = this;
	gen.cells = &cells;

	const_cast<FTools::OctTree *>(this->ftree)->VisitLevels(&gen, MTools::TreeGenerator::visitList);
	/*/
	std::stringstream linearized;

	const_cast<FTools::OctTree *>(this->ftree)->VisitLevels(&linearized, MTools::TreeGenerator::visitStream);

	//std::cout << linearized.str() << std::endl;

	double min[3], max[3];

	linearized >> min[0];
	linearized >> min[1];
	linearized >> min[2];
	linearized >> max[0];
	linearized >> max[1];
	linearized >> max[2];

	Data::Point3D pmin(static_cast<Real>(min[0]), static_cast<Real>(min[1]), static_cast<Real>(min[2]));
	Data::Point3D pmax(static_cast<Real>(max[0]), static_cast<Real>(max[1]), static_cast<Real>(max[2]));

	this->graph = this->makeTree(pmin, pmax);

	//std::cout << std::endl;

	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(static_cast<Data::OctTree *>(this->graph)->getRoot()));
	//*/
}

Data::Tree *MTools::TreeGenerator::makeTree(const Data::Point3D &min, const Data::Point3D &max) const
{
	return new Data::OctTree(min, max);
}

/*bool MTools::TreeGenerator::shouldSubdivideChildren(const FTools::OctTree *ftree) const
{
	for (UInt i = 0; i < 8; i++)
	{
		if (ftree->child[i])
		{
			return true;
		}
	}

	return false;
}*/

void MTools::TreeGenerator::visitStream(void *ptr, double x, double y, double z, double size, bool root, bool leaf)
{
	std::stringstream *stream = static_cast<std::stringstream *>(ptr);

	//std::cout << x << " " << y << " " << z << " " << size << " " << std::boolalpha << root << " " << leaf << std::endl;

	if (root)
	{
		//double halfSize = size/2.0;

		// (*stream) << x - halfSize << " " << y - halfSize << " " << z - halfSize << " "
		//	<< x + halfSize << " " << y + halfSize << " " << z + halfSize << " ";
		(*stream) << x - size << " " << y - size << " " << z - size << " "
			<< x + size << " " << y + size << " " << z + size << " ";
	}
	else
	{
		(*stream) << (leaf ? 'b' : 'g'); //black or grey
	}
}

void MTools::TreeGenerator::visitList(void *ptr, double x, double y, double z, double size, bool root, bool leaf)
{
	TreeGeneratorList *tgl = static_cast<TreeGeneratorList *>(ptr);

	Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(tgl->cells->back());

	//std::cout << x << " " << y << " " << z << " " << size << " " << std::boolalpha << root << " " << leaf << std::endl;

	if (root)
	{
		//double halfSize = size/2.0;

		// (*stream) << x - halfSize << " " << y - halfSize << " " << z - halfSize << " "
		//	<< x + halfSize << " " << y + halfSize << " " << z + halfSize << " ";
		// (*stream) << x - size << " " << y - size << " " << z - size << " "
		//	<< x + size << " " << y + size << " " << z + size << " ";

		static_cast<Data::Box *>(static_cast<Data::BoxTree *>(cell->getTree())->getShape())->getMin()->setCoord(0, x - size);
		static_cast<Data::Box *>(static_cast<Data::BoxTree *>(cell->getTree())->getShape())->getMin()->setCoord(1, y - size);
		static_cast<Data::Box *>(static_cast<Data::BoxTree *>(cell->getTree())->getShape())->getMin()->setCoord(2, z - size);
		static_cast<Data::Box *>(static_cast<Data::BoxTree *>(cell->getTree())->getShape())->getMax()->setCoord(0, x + size);
		static_cast<Data::Box *>(static_cast<Data::BoxTree *>(cell->getTree())->getShape())->getMax()->setCoord(1, y + size);
		static_cast<Data::Box *>(static_cast<Data::BoxTree *>(cell->getTree())->getShape())->getMax()->setCoord(2, z + size);
	}
	else
	{
		tgl->cells->pop_back();

		if (!leaf)
		{
			dynamic_cast<MTools::TreeGenerator *>(tgl->generator)->subdivide(static_cast<Data::OctTreeCell *>(cell));

			tgl->cells->push_back(cell->child(1));
			tgl->cells->push_back(cell->child(0));
			tgl->cells->push_back(cell->child(2));
			tgl->cells->push_back(cell->child(3));
			tgl->cells->push_back(cell->child(5));
			tgl->cells->push_back(cell->child(4));
			tgl->cells->push_back(cell->child(6));
			tgl->cells->push_back(cell->child(7));
		}
	}
}

void MTools::TreeGenerator::subdivide(Data::OctTreeCell *cell) const
{
	cell->subdivide();
}

void MTools::TreeGenerator::subdivide(std::stringstream &linearized, Data::OctTreeCell *cell) const
{
	//std::cout << cell->mid().getCoord(0) << " " << cell->mid().getCoord(1) << " " << cell->mid().getCoord(2) << " " << cell->size() << " " << std::boolalpha << cell->isRoot() << " " << cell->isLeaf() << std::endl;

	char code = 'b';
	linearized >> code;

	//std::cout << code;

	if (code == 'b')
	{
		return;
	}

	this->subdivide(cell);

	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(7)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(6)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(4)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(5)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(3)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(2)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(0)));
	this->subdivide(linearized, static_cast<Data::OctTreeCell *>(cell->child(1)));
}

#endif //#if USE_MTOOLS
