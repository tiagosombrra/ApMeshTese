#include "TemplateMJMesh/TemplateMeshGenerator.h"

#include "Data/BoxTreeCell.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"

using namespace TemplateMJMesh;

TemplateMJMesh::TemplateMeshGenerator::TemplateMeshGenerator() :
	Performer::TemplateMeshGenerator::TemplateMeshGenerator()
{
	this->boundary = NULL;
	this->front = NULL;
}

TemplateMJMesh::TemplateMeshGenerator::~TemplateMeshGenerator()
{

}

void TemplateMJMesh::TemplateMeshGenerator::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void TemplateMJMesh::TemplateMeshGenerator::setFront(MJMesh::Front *front)
{
	this->front = front;
}

MJMesh::Front *TemplateMJMesh::TemplateMeshGenerator::getFront() const
{
	return this->front;
}

void TemplateMJMesh::TemplateMeshGenerator::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

void TemplateMJMesh::TemplateMeshGenerator::execute()
{
	if (!this->boundary)
	{
		this->status = this->error;

		return;
	}

	if (!this->front)
	{
		this->front = this->makeFront();
	}

	Performer::TemplateMeshGenerator::execute();
}

bool TemplateMJMesh::TemplateMeshGenerator::shouldApplyTemplate(const Data::BoxTreeCell *cell)
{
	for (UInt side = 0; side < cell->numSides(); side++)
	{
		if (cell->neighbors(side).empty())
		{
			return false;
		}
	}

	return true;
}

void TemplateMJMesh::TemplateMeshGenerator::applyTemplate(const Data::BoxTreeCell *cell, VertexTree *root,
	Data::ElementList &newElements, Data::FrontElementList &newFrontElements, Data::VertexList &/*newVertices*/)
{
	if (cell->dimension() == 2)
	{
		Data::GraphNodeList neighbors[cell->numSides()];

		for (UInt side = 0; side < cell->numSides(); side++)
		{
			cell->neighbors(side, neighbors[side]);
		}

		if ((neighbors[0].size() <= 1) && (neighbors[1].size() <= 1) && (neighbors[2].size() <= 1) && (neighbors[3].size() <= 1))
		{
			/*
			changeOrder = false:

					  1
				3-----------2
				|         ++|
				|  1    ++  |
			  2	|    +++    |0
				|  ++   0   |
				|++         |
				0-----------1
					  3
			*/

			MJMesh::Vertex *verts[4] =
			{
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 0)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 1)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 2)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 3))
			};

			static const bool changeOrder = false;
			//bool changeOrder = ((cell->position()%2) == 1);

			if (changeOrder)
			{
				MJMesh::Vertex *v = verts[0];
				verts[0] = verts[1];
				verts[1] = verts[2];
				verts[2] = verts[3];
				verts[3] = v;

				Data::GraphNodeList ns;

				ns.swap(neighbors[0]);
				neighbors[0].swap(neighbors[1]);
				neighbors[1].swap(neighbors[2]);
				neighbors[2].swap(neighbors[3]);
				neighbors[3].swap(ns);
			}

			MJMesh::Element *els[2] = {NULL, NULL};

			els[0] = new MJMesh::Element2D(verts[0], verts[1], verts[2]);
			els[1] = new MJMesh::Element2D(verts[2], verts[3], verts[0]);

#if USE_GUI
			els[0]->setColor(1.0, 0.0, 0.0);
			els[1]->setColor(1.0, 0.0, 0.0);
#endif //#if USE_GUI

			newElements.push_back(els[0]);
			newElements.push_back(els[1]);

			MJMesh::FrontElement *fels[4] = {NULL, NULL, NULL, NULL};

			if (this->front)
			{
				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].front())))
				{
					fels[0] = new MJMesh::FrontElement2D(verts[2], verts[1]);

					newFrontElements.push_back(fels[0]);

					fels[0]->add(els[0]);

					this->front->add(fels[0]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].front())))
				{
					fels[1] = new MJMesh::FrontElement2D(verts[3], verts[2]);

					newFrontElements.push_back(fels[1]);

					fels[1]->add(els[1]);

					this->front->add(fels[1]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].front())))
				{
					fels[2] = new MJMesh::FrontElement2D(verts[0], verts[3]);

					newFrontElements.push_back(fels[2]);

					fels[2]->add(els[1]);

					this->front->add(fels[2]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].front())))
				{
					fels[3] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(fels[3]);

					fels[3]->add(els[0]);

					this->front->add(fels[3]);
				}

				for (UInt i = 0; i < 4; i++)
                {
                    if (!fels[i])
                    {
                        continue;
                    }

                    for (UInt j = 0; j < fels[i]->numVertices(); j++)
                    {
                        if (!this->front->have(fels[i]->getVertex(j)))
                        {
                            this->front->add(fels[i]->getVertex(j));
                        }
                    }
                }
			}

#if (!USE_DELETE_FELS)
			Data::FrontElement *meshFels[5] = {fels[0], fels[1], fels[2], fels[3], NULL};

			if ((!meshFels[0]) || (!meshFels[1]))
			{
				Data::FrontElementList adjs2 = this->getMesh()->findAdjacentFrontElements(verts[2]);

				for (Data::FrontElementList::iterator iter = adjs2.begin();
					 iter != adjs2.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[0] = (*iter);
					}

					if ((*iter)->have(verts[3]))
					{
						meshFels[1] = (*iter);
					}

					if ((meshFels[0]) && (meshFels[1]))
					{
						break;
					}
				}

				if (!meshFels[0])
				{
					meshFels[0] = new MJMesh::FrontElement2D(verts[2], verts[1]);

					newFrontElements.push_back(meshFels[0]);
				}

				meshFels[0]->add(els[0]);

				if (!meshFels[1])
				{
					meshFels[1] = new MJMesh::FrontElement2D(verts[3], verts[2]);

					newFrontElements.push_back(meshFels[1]);
				}

				meshFels[1]->add(els[1]);
			}

			if ((!meshFels[2]) || (!meshFels[3]))
			{
				Data::FrontElementList adjs0 = this->getMesh()->findAdjacentFrontElements(verts[0]);

				for (Data::FrontElementList::iterator iter = adjs0.begin();
					 iter != adjs0.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[3] = (*iter);
					}

					if ((*iter)->have(verts[3]))
					{
						meshFels[2] = (*iter);
					}

					if ((meshFels[2]) && (meshFels[3]))
					{
						break;
					}
				}

				if (!meshFels[2])
				{
					meshFels[2] = new MJMesh::FrontElement2D(verts[0], verts[3]);

					newFrontElements.push_back(meshFels[2]);
				}

				meshFels[2]->add(els[1]);

				if (!meshFels[3])
				{
					meshFels[3] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(meshFels[3]);
				}

				meshFels[3]->add(els[0]);
			}

			meshFels[4] = new MJMesh::FrontElement2D(verts[2], verts[0]);

			newFrontElements.push_back(meshFels[4]);

			meshFels[4]->add(els[0]);
			meshFels[4]->add(els[1]);
#endif //#if (!USE_DELETE_FELS)
		}
		else if (((neighbors[0].size() == 2) && (neighbors[1].size() <= 1) && (neighbors[2].size() <= 1) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() == 2) && (neighbors[2].size() <= 1) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() <= 1) && (neighbors[2].size() == 2) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() <= 1) && (neighbors[2].size() <= 1) && (neighbors[3].size() == 2)))
		{
			/*
			rotate = 0:

					  2
				3-----------2
				|++++    1  |
				|    +++    | 1
			  3	| 2    	++++5
				|    +++    |
				|++++    0  | 0
				0-----------1
					  4
			*/

			UInt rotate = 0;

			for (UInt i = 0; i < 4; i++)
			{
				if (neighbors[i].size() == 2)
				{
					rotate = i;

					break;
				}
			}

			MJMesh::Vertex *verts[5] =
			{
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 0)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 2)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 3)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 1)%4))
			};

			for (UInt i = 0; i < rotate; i++)
			{
				Data::GraphNodeList ns;

				ns.swap(neighbors[0]);
				neighbors[0].swap(neighbors[1]);
				neighbors[1].swap(neighbors[2]);
				neighbors[2].swap(neighbors[3]);
				neighbors[3].swap(ns);
			}

			MJMesh::Element *els[3] = {NULL, NULL, NULL};

			els[0] = new MJMesh::Element2D(verts[0], verts[1], verts[4]);
			els[1] = new MJMesh::Element2D(verts[4], verts[2], verts[3]);
			els[2] = new MJMesh::Element2D(verts[0], verts[4], verts[3]);

#if USE_GUI
			els[0]->setColor(0.0, (rotate + 1.0)/4.0, 0.0);
			els[1]->setColor(0.0, (rotate + 1.0)/4.0, 0.0);
			els[2]->setColor(0.0, (rotate + 1.0)/4.0, 0.0);
#endif //#if USE_GUI

			newElements.push_back(els[0]);
			newElements.push_back(els[1]);
			newElements.push_back(els[2]);

			MJMesh::FrontElement *fels[5] = {NULL, NULL, NULL, NULL, NULL};

			static const UInt order[4] = {0, 1, 2, 3};

			if (static_cast<Data::BoxTreeCell *>(neighbors[0].front())->position() != order[rotate])
			{
				neighbors[0].push_back(neighbors[0].front());
				neighbors[0].pop_front();
			}

			if (this->front)
			{
				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].front())))
				{
					fels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(fels[0]);

					fels[0]->add(els[0]);

					this->front->add(fels[0]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].back())))
				{
					fels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(fels[1]);

					fels[1]->add(els[1]);

					this->front->add(fels[1]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].front())))
				{
					fels[2] = new MJMesh::FrontElement2D(verts[3], verts[2]);

					newFrontElements.push_back(fels[2]);

					fels[2]->add(els[1]);

					this->front->add(fels[2]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].front())))
				{
					fels[3] = new MJMesh::FrontElement2D(verts[0], verts[3]);

					newFrontElements.push_back(fels[3]);

					fels[3]->add(els[2]);

					this->front->add(fels[3]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].front())))
				{
					fels[4] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(fels[4]);

					fels[4]->add(els[0]);

					this->front->add(fels[4]);
				}

				for (UInt i = 0; i < 5; i++)
                {
                    if (!fels[i])
                    {
                        continue;
                    }

                    for (UInt j = 0; j < fels[i]->numVertices(); j++)
                    {
                        if (!this->front->have(fels[i]->getVertex(j)))
                        {
                            this->front->add(fels[i]->getVertex(j));
                        }
                    }
                }
			}

#if (!USE_DELETE_FELS)
			Data::FrontElement *meshFels[7] = {fels[0], fels[1], fels[2], fels[3], fels[4], NULL, NULL};

			if ((!meshFels[0]) || (!meshFels[1]))
			{
				Data::FrontElementList adjs5 = this->getMesh()->findAdjacentFrontElements(verts[4]);

				for (Data::FrontElementList::iterator iter = adjs5.begin();
					 iter != adjs5.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[0] = (*iter);
					}

					if ((*iter)->have(verts[2]))
					{
						meshFels[1] = (*iter);
					}

					if ((meshFels[0]) && (meshFels[1]))
					{
						break;
					}
				}

				if (!meshFels[0])
				{
					meshFels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(meshFels[0]);
				}

				meshFels[0]->add(els[0]);

				if (!meshFels[1])
				{
					meshFels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(meshFels[1]);
				}

				meshFels[1]->add(els[1]);
			}

			if ((!meshFels[2]) || (!meshFels[3]))
			{
				Data::FrontElementList adjs3 = this->getMesh()->findAdjacentFrontElements(verts[3]);

				for (Data::FrontElementList::iterator iter = adjs3.begin();
					 iter != adjs3.end(); iter++)
				{
					if ((*iter)->have(verts[2]))
					{
						meshFels[2] = (*iter);
					}

					if ((*iter)->have(verts[0]))
					{
						meshFels[3] = (*iter);
					}

					if ((meshFels[2]) && (meshFels[3]))
					{
						break;
					}
				}

				if (!meshFels[2])
				{
					meshFels[2] = new MJMesh::FrontElement2D(verts[3], verts[2]);

					newFrontElements.push_back(meshFels[2]);
				}

				meshFels[2]->add(els[1]);

				if (!meshFels[3])
				{
					meshFels[3] = new MJMesh::FrontElement2D(verts[0], verts[3]);

					newFrontElements.push_back(meshFels[3]);
				}

				meshFels[3]->add(els[2]);
			}

			if (!meshFels[4])
			{
				Data::FrontElementList adjs0 = this->getMesh()->findAdjacentFrontElements(verts[0]);

				for (Data::FrontElementList::iterator iter = adjs0.begin();
					 iter != adjs0.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[4] = (*iter);

						break;
					}
				}

				if (!meshFels[4])
				{
					meshFels[4] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(meshFels[4]);
				}

				meshFels[4]->add(els[0]);
			}

			meshFels[5] = new MJMesh::FrontElement2D(verts[0], verts[4]);

			newFrontElements.push_back(meshFels[5]);

			meshFels[5]->add(els[0]);
			meshFels[5]->add(els[2]);

			meshFels[6] = new MJMesh::FrontElement2D(verts[4], verts[3]);

			newFrontElements.push_back(meshFels[6]);

			meshFels[6]->add(els[1]);
			meshFels[6]->add(els[2]);
#endif //#if (!USE_DELETE_FELS)
		}
		else if (((neighbors[0].size() == 2) && (neighbors[1].size() <= 1) && (neighbors[2].size() == 2) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() == 2) && (neighbors[2].size() <= 1) && (neighbors[3].size() == 2)))
		{
			/*
			rotate = 0:

					  2
				3-----------2
				|  3  ++++++|
			  3	|+++++  2   | 1
			  	7-----------5
			  4	|  1  ++++++|
				|+++++  0   | 0
				0-----------1
					  5
			*/

			UInt rotate = 0;

			for (UInt i = 0; i < 2; i++)
			{
				if (neighbors[i].size() == 2)
				{
					rotate = i;

					break;
				}
			}

			MJMesh::Vertex *verts[6] =
			{
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 0)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 2)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 3)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 3)%4))
			};

			for (UInt i = 0; i < rotate; i++)
			{
				Data::GraphNodeList ns;

				ns.swap(neighbors[0]);
				neighbors[0].swap(neighbors[1]);
				neighbors[1].swap(neighbors[2]);
				neighbors[2].swap(neighbors[3]);
				neighbors[3].swap(ns);
			}

			MJMesh::Element *els[4] = {NULL, NULL, NULL, NULL};

			els[0] = new MJMesh::Element2D(verts[0], verts[1], verts[4]);
			els[1] = new MJMesh::Element2D(verts[4], verts[5], verts[0]);
			els[2] = new MJMesh::Element2D(verts[5], verts[4], verts[2]);
			els[3] = new MJMesh::Element2D(verts[2], verts[3], verts[5]);

#if USE_GUI
			els[0]->setColor(0.0, 0.0, (rotate + 1.0)/2.0);
			els[1]->setColor(0.0, 0.0, (rotate + 1.0)/2.0);
			els[2]->setColor(0.0, 0.0, (rotate + 1.0)/2.0);
			els[3]->setColor(0.0, 0.0, (rotate + 1.0)/2.0);
#endif //#if USE_GUI

			newElements.push_back(els[0]);
			newElements.push_back(els[1]);
			newElements.push_back(els[2]);
			newElements.push_back(els[3]);

			MJMesh::FrontElement *fels[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

			static const UInt order[4] = {0, 1, 2, 3};

			if (static_cast<Data::BoxTreeCell *>(neighbors[0].front())->position() != order[rotate])
			{
				neighbors[0].push_back(neighbors[0].front());
				neighbors[0].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[2].front())->position() != order[rotate + 2])
			{
				neighbors[2].push_back(neighbors[2].front());
				neighbors[2].pop_front();
			}

			if (this->front)
			{
				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].front())))
				{
					fels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(fels[0]);

					fels[0]->add(els[0]);

					this->front->add(fels[0]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].back())))
				{
					fels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(fels[1]);

					fels[1]->add(els[2]);

					this->front->add(fels[1]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].front())))
				{
					fels[2] = new MJMesh::FrontElement2D(verts[3], verts[2]);

					newFrontElements.push_back(fels[2]);

					fels[2]->add(els[3]);

					this->front->add(fels[2]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].front())))
				{
					fels[3] = new MJMesh::FrontElement2D(verts[5], verts[3]);

					newFrontElements.push_back(fels[3]);

					fels[3]->add(els[3]);

					this->front->add(fels[3]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].back())))
				{
					fels[4] = new MJMesh::FrontElement2D(verts[0], verts[5]);

					newFrontElements.push_back(fels[4]);

					fels[4]->add(els[1]);

					this->front->add(fels[4]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].front())))
				{
					fels[5] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(fels[5]);

					fels[5]->add(els[0]);

					this->front->add(fels[5]);
				}

				for (UInt i = 0; i < 6; i++)
                {
                    if (!fels[i])
                    {
                        continue;
                    }

                    for (UInt j = 0; j < fels[i]->numVertices(); j++)
                    {
                        if (!this->front->have(fels[i]->getVertex(j)))
                        {
                            this->front->add(fels[i]->getVertex(j));
                        }
                    }
                }
			}

#if (!USE_DELETE_FELS)
			Data::FrontElement *meshFels[9] = {fels[0], fels[1], fels[2], fels[3], fels[4], fels[5], NULL, NULL, NULL};

			if ((!meshFels[0]) || (!meshFels[1]))
			{
				Data::FrontElementList adjs5 = this->getMesh()->findAdjacentFrontElements(verts[4]);

				for (Data::FrontElementList::iterator iter = adjs5.begin();
					 iter != adjs5.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[0] = (*iter);
					}

					if ((*iter)->have(verts[2]))
					{
						meshFels[1] = (*iter);
					}

					if ((meshFels[0]) && (meshFels[1]))
					{
						break;
					}
				}

				if (!meshFels[0])
				{
					meshFels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(meshFels[0]);
				}

				meshFels[0]->add(els[0]);

				if (!meshFels[1])
				{
					meshFels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(meshFels[1]);
				}

				meshFels[1]->add(els[2]);
			}

			if ((!meshFels[2]) || (!meshFels[3]))
			{
				Data::FrontElementList adjs3 = this->getMesh()->findAdjacentFrontElements(verts[3]);

				for (Data::FrontElementList::iterator iter = adjs3.begin();
					 iter != adjs3.end(); iter++)
				{
					if ((*iter)->have(verts[2]))
					{
						meshFels[2] = (*iter);
					}

					if ((*iter)->have(verts[5]))
					{
						meshFels[3] = (*iter);
					}

					if ((meshFels[2]) && (meshFels[3]))
					{
						break;
					}
				}

				if (!meshFels[2])
				{
					meshFels[2] = new MJMesh::FrontElement2D(verts[3], verts[2]);

					newFrontElements.push_back(meshFels[2]);
				}

				meshFels[2]->add(els[3]);

				if (!meshFels[3])
				{
					meshFels[3] = new MJMesh::FrontElement2D(verts[5], verts[3]);

					newFrontElements.push_back(meshFels[3]);
				}

				meshFels[3]->add(els[3]);
			}

			if ((!meshFels[4]) || (!meshFels[5]))
			{
				Data::FrontElementList adjs0 = this->getMesh()->findAdjacentFrontElements(verts[0]);

				for (Data::FrontElementList::iterator iter = adjs0.begin();
					 iter != adjs0.end(); iter++)
				{
					if ((*iter)->have(verts[5]))
					{
						meshFels[4] = (*iter);
					}

					if ((*iter)->have(verts[1]))
					{
						meshFels[5] = (*iter);
					}

					if ((meshFels[4]) && (meshFels[5]))
					{
						break;
					}
				}

				if (!meshFels[4])
				{
					meshFels[4] = new MJMesh::FrontElement2D(verts[0], verts[5]);

					newFrontElements.push_back(meshFels[4]);
				}

				meshFels[4]->add(els[1]);

				if (!meshFels[5])
				{
					meshFels[5] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(meshFels[5]);
				}

				meshFels[5]->add(els[0]);
			}

			meshFels[6] = new MJMesh::FrontElement2D(verts[0], verts[4]);

			newFrontElements.push_back(meshFels[6]);

			meshFels[6]->add(els[0]);
			meshFels[6]->add(els[1]);

			meshFels[7] = new MJMesh::FrontElement2D(verts[4], verts[5]);

			newFrontElements.push_back(meshFels[7]);

			meshFels[7]->add(els[1]);
			meshFels[7]->add(els[2]);

			meshFels[8] = new MJMesh::FrontElement2D(verts[5], verts[2]);

			newFrontElements.push_back(meshFels[8]);

			meshFels[8]->add(els[2]);
			meshFels[8]->add(els[3]);
#endif //#if (!USE_DELETE_FELS)
		}
		else if (((neighbors[0].size() == 2) && (neighbors[1].size() == 2) && (neighbors[2].size() <= 1) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() == 2) && (neighbors[2].size() == 2) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() <= 1) && (neighbors[2].size() == 2) && (neighbors[3].size() == 2)) ||
			((neighbors[0].size() == 2) && (neighbors[1].size() <= 1) && (neighbors[2].size() <= 1) && (neighbors[3].size() == 2)))
		{
			/*
			rotate = 0:

				   3     2
				3-----6-----2
				| 2   ++  1 |
			  	|   ++  ++  | 1
			  4 | ++  3   ++5
			  	|+    ++++  |
				|+++++   0  | 0
				0-----------1
					  5
			*/

			UInt rotate = 0;

			for (UInt i = 0; i < 4; i++)
			{
				if ((neighbors[i].size() == 2) && (neighbors[(i+1)%4].size() == 2))
				{
					rotate = i;

					break;
				}
			}

			MJMesh::Vertex *verts[6] =
			{
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 0)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 2)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 3)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 2)%4))
			};

			for (UInt i = 0; i < rotate; i++)
			{
				Data::GraphNodeList ns;

				ns.swap(neighbors[0]);
				neighbors[0].swap(neighbors[1]);
				neighbors[1].swap(neighbors[2]);
				neighbors[2].swap(neighbors[3]);
				neighbors[3].swap(ns);
			}

			MJMesh::Element *els[4] = {NULL, NULL, NULL, NULL};

			els[0] = new MJMesh::Element2D(verts[0], verts[1], verts[4]);
			els[1] = new MJMesh::Element2D(verts[4], verts[2], verts[5]);
			els[2] = new MJMesh::Element2D(verts[5], verts[3], verts[0]);
			els[3] = new MJMesh::Element2D(verts[0], verts[4], verts[5]);

#if USE_GUI
			els[0]->setColor((rotate + 1.0)/4.0, (rotate + 1.0)/4.0, 0.0);
			els[1]->setColor((rotate + 1.0)/4.0, (rotate + 1.0)/4.0, 0.0);
			els[2]->setColor((rotate + 1.0)/4.0, (rotate + 1.0)/4.0, 0.0);
			els[3]->setColor((rotate + 1.0)/4.0, (rotate + 1.0)/4.0, 0.0);
#endif //#if USE_GUI

			newElements.push_back(els[0]);
			newElements.push_back(els[1]);
			newElements.push_back(els[2]);
			newElements.push_back(els[3]);

			MJMesh::FrontElement *fels[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

			static const UInt order[4] = {0, 1, 2, 3};

			if (static_cast<Data::BoxTreeCell *>(neighbors[0].front())->position() != order[rotate])
			{
				neighbors[0].push_back(neighbors[0].front());
				neighbors[0].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[1].front())->position() != order[(rotate + 1)%4])
			{
				neighbors[1].push_back(neighbors[1].front());
				neighbors[1].pop_front();
			}

			if (this->front)
			{
				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].front())))
				{
					fels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(fels[0]);

					fels[0]->add(els[0]);

					this->front->add(fels[0]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].back())))
				{
					fels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(fels[1]);

					fels[1]->add(els[1]);

					this->front->add(fels[1]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].front())))
				{
					fels[2] = new MJMesh::FrontElement2D(verts[5], verts[2]);

					newFrontElements.push_back(fels[2]);

					fels[2]->add(els[1]);

					this->front->add(fels[2]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].back())))
				{
					fels[3] = new MJMesh::FrontElement2D(verts[3], verts[5]);

					newFrontElements.push_back(fels[3]);

					fels[3]->add(els[2]);

					this->front->add(fels[3]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].front())))
				{
					fels[4] = new MJMesh::FrontElement2D(verts[0], verts[3]);

					newFrontElements.push_back(fels[4]);

					fels[4]->add(els[2]);

					this->front->add(fels[4]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].front())))
				{
					fels[5] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(fels[5]);

					fels[5]->add(els[0]);

					this->front->add(fels[5]);
				}

				for (UInt i = 0; i < 6; i++)
                {
                    if (!fels[i])
                    {
                        continue;
                    }

                    for (UInt j = 0; j < fels[i]->numVertices(); j++)
                    {
                        if (!this->front->have(fels[i]->getVertex(j)))
                        {
                            this->front->add(fels[i]->getVertex(j));
                        }
                    }
                }
			}

#if (!USE_DELETE_FELS)
			Data::FrontElement *meshFels[9] = {fels[0], fels[1], fels[2], fels[3], fels[4], fels[5], NULL, NULL, NULL};

			if ((!meshFels[0]) || (!meshFels[1]))
			{
				Data::FrontElementList adjs5 = this->getMesh()->findAdjacentFrontElements(verts[4]);

				for (Data::FrontElementList::iterator iter = adjs5.begin();
					 iter != adjs5.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[0] = (*iter);
					}

					if ((*iter)->have(verts[2]))
					{
						meshFels[1] = (*iter);
					}

					if ((meshFels[0]) && (meshFels[1]))
					{
						break;
					}
				}

				if (!meshFels[0])
				{
					meshFels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(meshFels[0]);
				}

				meshFels[0]->add(els[0]);

				if (!meshFels[1])
				{
					meshFels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(meshFels[1]);
				}

				meshFels[1]->add(els[1]);
			}

			if ((!meshFels[2]) || (!meshFels[3]))
			{
				Data::FrontElementList adjs6 = this->getMesh()->findAdjacentFrontElements(verts[5]);

				for (Data::FrontElementList::iterator iter = adjs6.begin();
					 iter != adjs6.end(); iter++)
				{
					if ((*iter)->have(verts[2]))
					{
						meshFels[2] = (*iter);
					}

					if ((*iter)->have(verts[3]))
					{
						meshFels[3] = (*iter);
					}

					if ((meshFels[2]) && (meshFels[3]))
					{
						break;
					}
				}

				if (!meshFels[2])
				{
					meshFels[2] = new MJMesh::FrontElement2D(verts[5], verts[2]);

					newFrontElements.push_back(meshFels[2]);
				}

				meshFels[2]->add(els[1]);

				if (!meshFels[3])
				{
					meshFels[3] = new MJMesh::FrontElement2D(verts[3], verts[5]);

					newFrontElements.push_back(meshFels[3]);
				}

				meshFels[3]->add(els[2]);
			}

			if ((!meshFels[4]) || (!meshFels[5]))
			{
				Data::FrontElementList adjs0 = this->getMesh()->findAdjacentFrontElements(verts[0]);

				for (Data::FrontElementList::iterator iter = adjs0.begin();
					 iter != adjs0.end(); iter++)
				{
					if ((*iter)->have(verts[3]))
					{
						meshFels[4] = (*iter);
					}

					if ((*iter)->have(verts[1]))
					{
						meshFels[5] = (*iter);
					}

					if ((meshFels[4]) && (meshFels[5]))
					{
						break;
					}
				}

				if (!meshFels[4])
				{
					meshFels[4] = new MJMesh::FrontElement2D(verts[0], verts[3]);

					newFrontElements.push_back(meshFels[4]);
				}

				meshFels[4]->add(els[2]);

				if (!meshFels[5])
				{
					meshFels[5] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(meshFels[5]);
				}

				meshFels[5]->add(els[0]);
			}

			meshFels[6] = new MJMesh::FrontElement2D(verts[0], verts[4]);

			newFrontElements.push_back(meshFels[6]);

			meshFels[6]->add(els[0]);
			meshFels[6]->add(els[3]);

			meshFels[7] = new MJMesh::FrontElement2D(verts[4], verts[5]);

			newFrontElements.push_back(meshFels[7]);

			meshFels[7]->add(els[1]);
			meshFels[7]->add(els[3]);

			meshFels[8] = new MJMesh::FrontElement2D(verts[5], verts[0]);

			newFrontElements.push_back(meshFels[8]);

			meshFels[8]->add(els[2]);
			meshFels[8]->add(els[3]);
#endif //#if (!USE_DELETE_FELS)
		}
		else if (((neighbors[0].size() == 2) && (neighbors[1].size() == 2) && (neighbors[2].size() == 2) && (neighbors[3].size() <= 1)) ||
			((neighbors[0].size() <= 1) && (neighbors[1].size() == 2) && (neighbors[2].size() == 2) && (neighbors[3].size() == 2)) ||
			((neighbors[0].size() == 2) && (neighbors[1].size() <= 1) && (neighbors[2].size() == 2) && (neighbors[3].size() == 2)) ||
			((neighbors[0].size() == 2) && (neighbors[1].size() == 2) && (neighbors[2].size() <= 1) && (neighbors[3].size() == 2)))
		{
			/*
			rotate = 0:

				   3     2
				3-----6-----2
				| 4 ++ ++ 3 |
			  4 |+++  2  +++| 1
			    7-----------5
			  5 |  1  ++++++|
				|+++++   0  | 0
				0-----------1
					  6
			*/

			UInt rotate = 0;

			for (UInt i = 0; i < 4; i++)
			{
				if ((neighbors[i].size() == 2) && (neighbors[(i+1)%4].size() == 2) && (neighbors[(i+2)%4].size() == 2))
				{
					rotate = i;

					break;
				}
			}

			MJMesh::Vertex *verts[7] =
			{
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 0)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 2)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, (rotate + 3)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 1)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 2)%4)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4 + (rotate + 3)%4))
			};

			for (UInt i = 0; i < rotate; i++)
			{
				Data::GraphNodeList ns;

				ns.swap(neighbors[0]);
				neighbors[0].swap(neighbors[1]);
				neighbors[1].swap(neighbors[2]);
				neighbors[2].swap(neighbors[3]);
				neighbors[3].swap(ns);
			}

			MJMesh::Element *els[5] = {NULL, NULL, NULL, NULL, NULL};

			els[0] = new MJMesh::Element2D(verts[0], verts[1], verts[4]);
			els[1] = new MJMesh::Element2D(verts[4], verts[6], verts[0]);
			els[2] = new MJMesh::Element2D(verts[6], verts[4], verts[5]);
			els[3] = new MJMesh::Element2D(verts[4], verts[2], verts[5]);
			els[4] = new MJMesh::Element2D(verts[5], verts[3], verts[6]);

#if USE_GUI
			els[0]->setColor((rotate + 1.0)/4.0, 0.0, (rotate + 1.0)/4.0);
			els[1]->setColor((rotate + 1.0)/4.0, 0.0, (rotate + 1.0)/4.0);
			els[2]->setColor((rotate + 1.0)/4.0, 0.0, (rotate + 1.0)/4.0);
			els[3]->setColor((rotate + 1.0)/4.0, 0.0, (rotate + 1.0)/4.0);
			els[4]->setColor((rotate + 1.0)/4.0, 0.0, (rotate + 1.0)/4.0);
#endif //#if USE_GUI

			newElements.push_back(els[0]);
			newElements.push_back(els[1]);
			newElements.push_back(els[2]);
			newElements.push_back(els[3]);
			newElements.push_back(els[4]);

			MJMesh::FrontElement *fels[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

			static const UInt order[4] = {0, 1, 2, 3};

			if (static_cast<Data::BoxTreeCell *>(neighbors[0].front())->position() != order[rotate])
			{
				neighbors[0].push_back(neighbors[0].front());
				neighbors[0].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[1].front())->position() != order[(rotate + 1)%4])
			{
				neighbors[1].push_back(neighbors[1].front());
				neighbors[1].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[2].front())->position() != order[(rotate + 2)%4])
			{
				neighbors[2].push_back(neighbors[2].front());
				neighbors[2].pop_front();
			}

			if (this->front)
			{
				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].front())))
				{
					fels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(fels[0]);

					fels[0]->add(els[0]);

					this->front->add(fels[0]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].back())))
				{
					fels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(fels[1]);

					fels[1]->add(els[3]);

					this->front->add(fels[1]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].front())))
				{
					fels[2] = new MJMesh::FrontElement2D(verts[5], verts[2]);

					newFrontElements.push_back(fels[2]);

					fels[2]->add(els[3]);

					this->front->add(fels[2]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].back())))
				{
					fels[3] = new MJMesh::FrontElement2D(verts[3], verts[5]);

					newFrontElements.push_back(fels[3]);

					fels[3]->add(els[4]);

					this->front->add(fels[3]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].front())))
				{
					fels[4] = new MJMesh::FrontElement2D(verts[6], verts[3]);

					newFrontElements.push_back(fels[4]);

					fels[4]->add(els[4]);

					this->front->add(fels[4]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].back())))
				{
					fels[5] = new MJMesh::FrontElement2D(verts[0], verts[6]);

					newFrontElements.push_back(fels[5]);

					fels[5]->add(els[1]);

					this->front->add(fels[5]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].front())))
				{
					fels[6] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(fels[6]);

					fels[6]->add(els[0]);

					this->front->add(fels[6]);
				}

				for (UInt i = 0; i < 7; i++)
                {
                    if (!fels[i])
                    {
                        continue;
                    }

                    for (UInt j = 0; j < fels[i]->numVertices(); j++)
                    {
                        if (!this->front->have(fels[i]->getVertex(j)))
                        {
                            this->front->add(fels[i]->getVertex(j));
                        }
                    }
                }
			}

#if (!USE_DELETE_FELS)
			Data::FrontElement *meshFels[11] = {fels[0], fels[1], fels[2], fels[3], fels[4], fels[5], fels[6], NULL, NULL, NULL, NULL};

			if ((!meshFels[0]) || (!meshFels[1]))
			{
				Data::FrontElementList adjs5 = this->getMesh()->findAdjacentFrontElements(verts[4]);

				for (Data::FrontElementList::iterator iter = adjs5.begin();
					 iter != adjs5.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[0] = (*iter);
					}

					if ((*iter)->have(verts[2]))
					{
						meshFels[1] = (*iter);
					}

					if ((meshFels[0]) && (meshFels[1]))
					{
						break;
					}
				}

				if (!meshFels[0])
				{
					meshFels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(meshFels[0]);
				}

				meshFels[0]->add(els[0]);

				if (!meshFels[1])
				{
					meshFels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(meshFels[1]);
				}

				meshFels[1]->add(els[3]);
			}

			if ((!meshFels[2]) || (!meshFels[3]))
			{
				Data::FrontElementList adjs6 = this->getMesh()->findAdjacentFrontElements(verts[5]);

				for (Data::FrontElementList::iterator iter = adjs6.begin();
					 iter != adjs6.end(); iter++)
				{
					if ((*iter)->have(verts[2]))
					{
						meshFels[2] = (*iter);
					}

					if ((*iter)->have(verts[3]))
					{
						meshFels[3] = (*iter);
					}

					if ((meshFels[2]) && (meshFels[3]))
					{
						break;
					}
				}

				if (!meshFels[2])
				{
					meshFels[2] = new MJMesh::FrontElement2D(verts[5], verts[2]);

					newFrontElements.push_back(meshFels[2]);
				}

				meshFels[2]->add(els[3]);

				if (!meshFels[3])
				{
					meshFels[3] = new MJMesh::FrontElement2D(verts[3], verts[5]);

					newFrontElements.push_back(meshFels[3]);
				}

				meshFels[3]->add(els[4]);
			}

			if ((!meshFels[4]) || (!meshFels[5]))
			{
				Data::FrontElementList adjs7 = this->getMesh()->findAdjacentFrontElements(verts[6]);

				for (Data::FrontElementList::iterator iter = adjs7.begin();
					 iter != adjs7.end(); iter++)
				{
					if ((*iter)->have(verts[3]))
					{
						meshFels[4] = (*iter);
					}

					if ((*iter)->have(verts[0]))
					{
						meshFels[5] = (*iter);
					}

					if ((meshFels[4]) && (meshFels[5]))
					{
						break;
					}
				}

				if (!meshFels[4])
				{
					meshFels[4] = new MJMesh::FrontElement2D(verts[6], verts[3]);

					newFrontElements.push_back(meshFels[4]);
				}

				meshFels[4]->add(els[4]);

				if (!meshFels[5])
				{
					meshFels[5] = new MJMesh::FrontElement2D(verts[0], verts[6]);

					newFrontElements.push_back(meshFels[5]);
				}

				meshFels[5]->add(els[1]);
			}

			if (!meshFels[6])
			{
				Data::FrontElementList adjs0 = this->getMesh()->findAdjacentFrontElements(verts[0]);

				for (Data::FrontElementList::iterator iter = adjs0.begin();
					 iter != adjs0.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[6] = (*iter);

						break;
					}
				}

				if (!meshFels[6])
				{
					meshFels[6] = new MJMesh::FrontElement2D(verts[1], verts[0]);

					newFrontElements.push_back(meshFels[6]);
				}

				meshFels[6]->add(els[0]);
			}

			meshFels[7] = new MJMesh::FrontElement2D(verts[0], verts[4]);

			newFrontElements.push_back(meshFels[7]);

			meshFels[7]->add(els[0]);
			meshFels[7]->add(els[1]);

			meshFels[8] = new MJMesh::FrontElement2D(verts[4], verts[6]);

			newFrontElements.push_back(meshFels[8]);

			meshFels[8]->add(els[1]);
			meshFels[8]->add(els[2]);

			meshFels[9] = new MJMesh::FrontElement2D(verts[6], verts[5]);

			newFrontElements.push_back(meshFels[9]);

			meshFels[9]->add(els[2]);
			meshFels[9]->add(els[4]);

			meshFels[10] = new MJMesh::FrontElement2D(verts[5], verts[4]);

			newFrontElements.push_back(meshFels[10]);

			meshFels[10]->add(els[2]);
			meshFels[10]->add(els[3]);
#endif //#if (!USE_DELETE_FELS)
		}
		else if ((neighbors[0].size() == 2) && (neighbors[1].size() == 2) && (neighbors[2].size() == 2) && (neighbors[3].size() == 2))
		{
			/*
			rotate = 0:

				   3     2
				3-----6-----2
				| 3 ++ ++ 5 |
			  4 |+++  4  +++| 1
			    7-----------5
			  5 |+++  1  +++|
				| 0 ++ ++ 2 | 0
				0-----4-----1
				   6     7
			*/

			MJMesh::Vertex *verts[8] =
			{
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 0)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 1)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 2)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 3)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 5)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 6)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 7)),
				static_cast<MJMesh::Vertex *>(this->getVertex(root, 4))
			};

			static const bool changeOrder = false;

			if (changeOrder)
			{
				MJMesh::Vertex *v = verts[0];
				verts[0] = verts[1];
				verts[1] = verts[2];
				verts[2] = verts[3];
				verts[3] = v;
				v = verts[4];
				verts[4] = verts[5];
				verts[5] = verts[6];
				verts[6] = verts[7];
				verts[7] = v;

				Data::GraphNodeList ns;

				ns.swap(neighbors[0]);
				neighbors[0].swap(neighbors[1]);
				neighbors[1].swap(neighbors[2]);
				neighbors[2].swap(neighbors[3]);
				neighbors[3].swap(ns);
			}

			MJMesh::Element *els[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

			els[0] = new MJMesh::Element2D(verts[0], verts[7], verts[6]);
			els[1] = new MJMesh::Element2D(verts[7], verts[4], verts[6]);
			els[2] = new MJMesh::Element2D(verts[7], verts[1], verts[4]);
			els[3] = new MJMesh::Element2D(verts[6], verts[5], verts[3]);
			els[4] = new MJMesh::Element2D(verts[6], verts[4], verts[5]);
			els[5] = new MJMesh::Element2D(verts[4], verts[2], verts[5]);

#if USE_GUI
			els[0]->setColor(0.0, 1.0, 1.0);
			els[1]->setColor(0.0, 1.0, 1.0);
			els[2]->setColor(0.0, 1.0, 1.0);
			els[3]->setColor(0.0, 1.0, 1.0);
			els[4]->setColor(0.0, 1.0, 1.0);
			els[5]->setColor(0.0, 1.0, 1.0);
#endif //#if USE_GUI

			newElements.push_back(els[0]);
			newElements.push_back(els[1]);
			newElements.push_back(els[2]);
			newElements.push_back(els[3]);
			newElements.push_back(els[4]);
			newElements.push_back(els[5]);

			MJMesh::FrontElement *fels[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

			static const UInt order[4] = {0, 1, 2, 3};

			if (static_cast<Data::BoxTreeCell *>(neighbors[0].front())->position() != order[changeOrder ? 1 : 0])
			{
				neighbors[0].push_back(neighbors[0].front());
				neighbors[0].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[1].front())->position() != order[changeOrder ? 2 : 1])
			{
				neighbors[1].push_back(neighbors[1].front());
				neighbors[1].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[2].front())->position() != order[changeOrder ? 3 : 2])
			{
				neighbors[2].push_back(neighbors[2].front());
				neighbors[2].pop_front();
			}

			if (static_cast<Data::BoxTreeCell *>(neighbors[3].front())->position() != order[changeOrder ? 0 : 3])
			{
				neighbors[3].push_back(neighbors[3].front());
				neighbors[3].pop_front();
			}

			if (this->front)
			{
				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].front())))
				{
					fels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(fels[0]);

					fels[0]->add(els[2]);

					this->front->add(fels[0]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[0].back())))
				{
					fels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(fels[1]);

					fels[1]->add(els[5]);

					this->front->add(fels[1]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].front())))
				{
					fels[2] = new MJMesh::FrontElement2D(verts[5], verts[2]);

					newFrontElements.push_back(fels[2]);

					fels[2]->add(els[5]);

					this->front->add(fels[2]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[1].back())))
				{
					fels[3] = new MJMesh::FrontElement2D(verts[3], verts[5]);

					newFrontElements.push_back(fels[3]);

					fels[3]->add(els[3]);

					this->front->add(fels[3]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].front())))
				{
					fels[4] = new MJMesh::FrontElement2D(verts[6], verts[3]);

					newFrontElements.push_back(fels[4]);

					fels[4]->add(els[3]);

					this->front->add(fels[4]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[2].back())))
				{
					fels[5] = new MJMesh::FrontElement2D(verts[0], verts[6]);

					newFrontElements.push_back(fels[5]);

					fels[5]->add(els[0]);

					this->front->add(fels[5]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].front())))
				{
					fels[6] = new MJMesh::FrontElement2D(verts[7], verts[0]);

					newFrontElements.push_back(fels[6]);

					fels[6]->add(els[0]);

					this->front->add(fels[6]);
				}

				if (!this->shouldApplyTemplate(static_cast<Data::BoxTreeCell *>(neighbors[3].back())))
				{
					fels[7] = new MJMesh::FrontElement2D(verts[1], verts[7]);

					newFrontElements.push_back(fels[7]);

					fels[7]->add(els[2]);

					this->front->add(fels[7]);
				}

				for (UInt i = 0; i < 8; i++)
                {
                    if (!fels[i])
                    {
                        continue;
                    }

                    for (UInt j = 0; j < fels[i]->numVertices(); j++)
                    {
                        if (!this->front->have(fels[i]->getVertex(j)))
                        {
                            this->front->add(fels[i]->getVertex(j));
                        }
                    }
                }
			}

#if (!USE_DELETE_FELS)
			Data::FrontElement *meshFels[13] = {fels[0], fels[1], fels[2], fels[3], fels[4], fels[5], fels[6], fels[7], NULL, NULL, NULL, NULL, NULL};

			if ((!meshFels[0]) || (!meshFels[1]))
			{
				Data::FrontElementList adjs5 = this->getMesh()->findAdjacentFrontElements(verts[4]);

				for (Data::FrontElementList::iterator iter = adjs5.begin();
					 iter != adjs5.end(); iter++)
				{
					if ((*iter)->have(verts[1]))
					{
						meshFels[0] = (*iter);
					}

					if ((*iter)->have(verts[2]))
					{
						meshFels[1] = (*iter);
					}

					if ((meshFels[0]) && (meshFels[1]))
					{
						break;
					}
				}

				if (!meshFels[0])
				{
					meshFels[0] = new MJMesh::FrontElement2D(verts[4], verts[1]);

					newFrontElements.push_back(meshFels[0]);
				}

				meshFels[0]->add(els[2]);

				if (!meshFels[1])
				{
					meshFels[1] = new MJMesh::FrontElement2D(verts[2], verts[4]);

					newFrontElements.push_back(meshFels[1]);
				}

				meshFels[1]->add(els[5]);
			}

			if ((!meshFels[2]) || (!meshFels[3]))
			{
				Data::FrontElementList adjs6 = this->getMesh()->findAdjacentFrontElements(verts[5]);

				for (Data::FrontElementList::iterator iter = adjs6.begin();
					 iter != adjs6.end(); iter++)
				{
					if ((*iter)->have(verts[2]))
					{
						meshFels[2] = (*iter);
					}

					if ((*iter)->have(verts[3]))
					{
						meshFels[3] = (*iter);
					}

					if ((meshFels[2]) && (meshFels[3]))
					{
						break;
					}
				}

				if (!meshFels[2])
				{
					meshFels[2] = new MJMesh::FrontElement2D(verts[5], verts[2]);

					newFrontElements.push_back(meshFels[2]);
				}

				meshFels[2]->add(els[3]);

				if (!meshFels[3])
				{
					meshFels[3] = new MJMesh::FrontElement2D(verts[3], verts[5]);

					newFrontElements.push_back(meshFels[3]);
				}

				meshFels[3]->add(els[3]);
			}

			if ((!meshFels[4]) || (!meshFels[5]))
			{
				Data::FrontElementList adjs7 = this->getMesh()->findAdjacentFrontElements(verts[6]);

				for (Data::FrontElementList::iterator iter = adjs7.begin();
					 iter != adjs7.end(); iter++)
				{
					if ((*iter)->have(verts[3]))
					{
						meshFels[4] = (*iter);
					}

					if ((*iter)->have(verts[0]))
					{
						meshFels[5] = (*iter);
					}

					if ((meshFels[4]) && (meshFels[5]))
					{
						break;
					}
				}

				if (!meshFels[4])
				{
					meshFels[4] = new MJMesh::FrontElement2D(verts[6], verts[3]);

					newFrontElements.push_back(meshFels[4]);
				}

				meshFels[4]->add(els[3]);

				if (!meshFels[5])
				{
					meshFels[5] = new MJMesh::FrontElement2D(verts[0], verts[6]);

					newFrontElements.push_back(meshFels[5]);
				}

				meshFels[5]->add(els[0]);
			}

			if ((!meshFels[6]) || (!meshFels[7]))
			{
				Data::FrontElementList adjs4 = this->getMesh()->findAdjacentFrontElements(verts[7]);

				for (Data::FrontElementList::iterator iter = adjs4.begin();
					 iter != adjs4.end(); iter++)
				{
					if ((*iter)->have(verts[0]))
					{
						meshFels[6] = (*iter);
					}

					if ((*iter)->have(verts[1]))
					{
						meshFels[7] = (*iter);
					}

					if ((meshFels[6]) && (meshFels[7]))
					{
						break;
					}
				}

				if (!meshFels[6])
				{
					meshFels[6] = new MJMesh::FrontElement2D(verts[7], verts[0]);

					newFrontElements.push_back(meshFels[6]);
				}

				meshFels[6]->add(els[0]);

				if (!meshFels[7])
				{
					meshFels[7] = new MJMesh::FrontElement2D(verts[1], verts[7]);

					newFrontElements.push_back(meshFels[7]);
				}

				meshFels[7]->add(els[2]);
			}

			meshFels[8] = new MJMesh::FrontElement2D(verts[7], verts[6]);

			newFrontElements.push_back(meshFels[8]);

			meshFels[8]->add(els[0]);
			meshFels[8]->add(els[1]);

			meshFels[9] = new MJMesh::FrontElement2D(verts[7], verts[4]);

			newFrontElements.push_back(meshFels[9]);

			meshFels[9]->add(els[1]);
			meshFels[9]->add(els[2]);

			meshFels[10] = new MJMesh::FrontElement2D(verts[6], verts[4]);

			newFrontElements.push_back(meshFels[10]);

			meshFels[10]->add(els[1]);
			meshFels[10]->add(els[4]);

			meshFels[11] = new MJMesh::FrontElement2D(verts[6], verts[5]);

			newFrontElements.push_back(meshFels[11]);

			meshFels[11]->add(els[3]);
			meshFels[11]->add(els[4]);

			meshFels[12] = new MJMesh::FrontElement2D(verts[4], verts[5]);

			newFrontElements.push_back(meshFels[12]);

			meshFels[12]->add(els[4]);
			meshFels[12]->add(els[5]);
#endif //#if (!USE_DELETE_FELS)
		}
	}
}

Data::Vertex *TemplateMJMesh::TemplateMeshGenerator::makeVertex(const Data::Point2D &p, ULInt id) const
{
	return new MJMesh::Vertex(p.getX(), p.getY(), id);
}

Data::Vertex *TemplateMJMesh::TemplateMeshGenerator::makeVertex(const Data::Point3D &p, ULInt id) const
{
	return new MJMesh::Vertex(p.getX(), p.getY(), p.getZ(), id);
}

Data::Mesh *TemplateMJMesh::TemplateMeshGenerator::makeMesh() const
{
	MJMesh::Mesh *mesh = new MJMesh::Mesh();

	for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
		 iter != end; ++iter)
	{
		mesh->add((*iter), false);
	}

	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
		 iter != end; ++iter)
	{
		mesh->add(static_cast<Data::FrontElement *>((*iter)));
	}

	return mesh;
}

MJMesh::Front *TemplateMJMesh::TemplateMeshGenerator::makeFront() const
{
	return new MJMesh::Front(static_cast<const MJMesh::Boundary *>(this->boundary));
}
