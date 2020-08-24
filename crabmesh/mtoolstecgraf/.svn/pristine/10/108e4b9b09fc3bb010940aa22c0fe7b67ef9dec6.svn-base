#include "Jmesh/TreeGenerator.h"

#include "Data/Point3D.h"
#include "Data/OctTree.h"

Jmesh::TreeGenerator::TreeGenerator(const Msh3DOctTree root) :
	Performer::TreeGenerator()
{
	this->setRoot(root);
}

Jmesh::TreeGenerator::~TreeGenerator()
{

}

void Jmesh::TreeGenerator::setRoot(const Msh3DOctTree root)
{
	this->root = root;
}

void Jmesh::TreeGenerator::execute()
{
	if (!this->root)
	{
		return;
	}

	Box3d box = Msh3DTreeBox();

	Data::Point3D min(box.min[0], box.min[1], box.min[2]);
	Data::Point3D max(box.max[0], box.max[1], box.max[2]);

	this->graph = this->makeTree(min, max);

	this->subdivide(this->root, static_cast<Data::OctTreeCell *>(this->getTree()->getRoot()));
}

Data::Tree *Jmesh::TreeGenerator::makeTree(const Data::Point3D &min, const Data::Point3D &max) const
{
	return new Data::OctTree(min, max);
}

bool Jmesh::TreeGenerator::shouldSubdivideChildren(const Msh3DOctTree root) const
{
	for (UInt i = 0; i < 8; i++)
	{
		if (root->child[i])
		{
			return true;
		}
	}

	return false;
}

void Jmesh::TreeGenerator::subdivide(Data::OctTreeCell *cell) const
{
	cell->subdivide();
}

void Jmesh::TreeGenerator::subdivide(const Msh3DOctTree root, Data::OctTreeCell *cell) const
{
	if (!root)
	{
		return;
	}

	this->subdivide(cell);

	if (!this->shouldSubdivideChildren(root))
	{
		return;
	}

	this->subdivide(root->child[0], static_cast<Data::OctTreeCell *>(cell->child(0)));
	this->subdivide(root->child[1], static_cast<Data::OctTreeCell *>(cell->child(1)));
	this->subdivide(root->child[2], static_cast<Data::OctTreeCell *>(cell->child(3)));
	this->subdivide(root->child[3], static_cast<Data::OctTreeCell *>(cell->child(2)));
	this->subdivide(root->child[4], static_cast<Data::OctTreeCell *>(cell->child(4)));
	this->subdivide(root->child[5], static_cast<Data::OctTreeCell *>(cell->child(5)));
	this->subdivide(root->child[6], static_cast<Data::OctTreeCell *>(cell->child(7)));
	this->subdivide(root->child[7], static_cast<Data::OctTreeCell *>(cell->child(6)));
}
