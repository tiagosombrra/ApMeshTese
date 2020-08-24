#ifndef _PMJMESH2_CELL_COUNTER_H_
#define _PMJMESH2_CELL_COUNTER_H_

#include "PMJMesh2/Definitions.h"

#if DEBUG_CELL_COUNTER

#include "Performer/Builder.h"
#include "PMJMesh/MinTreeRefiner.h"

namespace PMJMesh2
{
    class CellCounter : public Performer::Builder
    {
    public:

        CellCounter();
        ~CellCounter();

        virtual void setDimension(UInt dimension);

        virtual void setBSP(const PartitionBSPTree *bsp);

        virtual void setTree(Data::BoxTree *tree);

        virtual void setBoundary(const MJMesh::Boundary *boundary);

        virtual void setNumLayers(UInt numLayers);

        virtual void setTasks(const Parallel::TaskVector &tasks);

#if USE_C__11
        virtual void setElementsMap(PMJMesh::FEListHashMap *elementsMap);
#else
        virtual void setElementsMap(PMJMesh::FEListMap *elementsMap);
#endif //#if USE_C__11

        virtual void setBuildHistogram(bool buildHistogram);

        virtual void execute();

    protected:

        typedef std::map<Data::Position, Data::GraphNodeSet> PositionCellMap;
        typedef std::map<UInt, PositionCellMap> SidePositionCellMap;
        typedef std::map<UInt, SidePositionCellMap> LayerSidePositionCellMap;

        struct Cell
        {
            Data::BSPTreeCell *cell;
            Data::GraphNodeList cells;
            LayerSidePositionCellMap layers;

            struct Count
            {
                Count() :
                    minIN(0), maxIN(0), normalIN(0),
                    minON(0), maxON(0), normalON(0),
                    minINLayer(NULL), maxINLayer(NULL), normalINLayer(NULL) {}

                Count(UInt numLayers) :
                    minIN(0), maxIN(0), normalIN(0),
                    minON(0), maxON(0), normalON(0),
                    minINLayer(NULL), maxINLayer(NULL), normalINLayer(NULL)
                {
                    this->resize(numLayers);
                };

                ~Count()
                {
                    if (this->minINLayer) delete [] this->minINLayer;
                    if (this->maxINLayer) delete [] this->maxINLayer;
                    if (this->normalINLayer) delete [] this->normalINLayer;
                }

                void resize(UInt numLayers)
                {
                    if ((this->minINLayer) || (this->maxINLayer) || (this->normalINLayer) || (numLayers == 0))
                    {
                        return;
                    }

                    this->minINLayer = new UInt[numLayers];
                    this->maxINLayer = new UInt[numLayers];
                    this->normalINLayer = new UInt[numLayers];

                    for (UInt i = 0; i < numLayers; i++)
                    {
                        this->minINLayer[i] = 0;
                        this->maxINLayer[i] = 0;
                        this->normalINLayer[i] = 0;
                    }
                }

                void sum(const Count &cnt, UInt numLayers)
                {
                    this->minIN += cnt.minIN;
                    this->maxIN += cnt.maxIN;
                    this->normalIN += cnt.normalIN;

                    this->minON += cnt.minON;
                    this->maxON += cnt.maxON;
                    this->normalON += cnt.normalON;

                    this->resize(numLayers);

                    for (UInt i = 0; i < numLayers; i++)
                    {
                        this->minINLayer[i] += cnt.minINLayer[i];
                        this->maxINLayer[i] += cnt.maxINLayer[i];
                        this->normalINLayer[i] += cnt.normalINLayer[i];
                    }
                }

                /*Count &operator+=(const Count &cnt)
                {
                    this->minIN += cnt.minIN;
                    this->maxIN += cnt.maxIN;
                    this->normalIN += cnt.normalIN;

                    this->minON += cnt.minON;
                    this->maxON += cnt.maxON;
                    this->normalON += cnt.normalON;

                    UInt cntNumLayers = 0;

                    for (Data::UIntMap::const_iterator iter = cnt.minINLayer.begin();
                         iter != cnt.minINLayer.end(); iter++)
                    {
                        if ((*iter).first > cntNumLayers)
                        {
                            cntNumLayers = static_cast<UInt>((*iter).first);
                        }
                    }

                    for (Data::UIntMap::const_iterator iter = cnt.maxINLayer.begin();
                         iter != cnt.maxINLayer.end(); iter++)
                    {
                        if ((*iter).first > cntNumLayers)
                        {
                            cntNumLayers = static_cast<UInt>((*iter).first);
                        }
                    }

                    for (Data::UIntMap::const_iterator iter = cnt.normalINLayer.begin();
                         iter != cnt.normalINLayer.end(); iter++)
                    {
                        if ((*iter).first > cntNumLayers)
                        {
                            cntNumLayers = static_cast<UInt>((*iter).first);
                        }
                    }

                    for (UInt i = 0; i < cntNumLayers; i++)
                    {
                        this->minINLayer.insert(std::make_pair(i, 0));
                        this->maxINLayer.insert(std::make_pair(i, 0));
                        this->normalINLayer.insert(std::make_pair(i, 0));
                    }

                    for (Data::UIntMap::const_iterator iter = cnt.minINLayer.begin();
                         iter != cnt.minINLayer.end(); iter++)
                    {
                        this->minINLayer[(*iter).first] += (*iter).second;
                    }

                    for (Data::UIntMap::const_iterator iter = cnt.maxINLayer.begin();
                         iter != cnt.maxINLayer.end(); iter++)
                    {
                        this->maxINLayer[(*iter).first] += (*iter).second;
                    }

                    for (Data::UIntMap::const_iterator iter = cnt.normalINLayer.begin();
                         iter != cnt.normalINLayer.end(); iter++)
                    {
                        this->normalINLayer[(*iter).first] += (*iter).second;
                    }

                    return *this;
                }*/

                UInt minIN;
                UInt maxIN;
                UInt normalIN;

                UInt minON;
                UInt maxON;
                UInt normalON;

                //Data::UIntMap minINLayer;
                //Data::UIntMap maxINLayer;
                //Data::UIntMap normalINLayer;

                UInt *minINLayer;
                UInt *maxINLayer;
                UInt *normalINLayer;
            };

            Count count;

            Data::ULIntMap histogram;
        };

        typedef std::map<ULInt, Cell> CellMap;

        class MinTreeRefiner : public PMJMesh::MinTreeRefiner
        {
        public:

            MinTreeRefiner(UInt dimension, UInt minLevel, Data::BoxTree *tree,
                const MJMesh::Boundary *boundary);
            virtual ~MinTreeRefiner();

            virtual UInt findMinLevel() const;
        };

    protected:

        virtual void findLevels();
        virtual void findLevels(const Data::BoxTreeCell *cell);

        virtual void findCells();
        virtual void findLayers(const Data::BSPTreeCell *cell);
        virtual void addHistogram(const Data::BoxTreeCell *c, Cell &cell);
        virtual void findCells(const Data::BSPTreeCell *cell);

        virtual void countNormal();
        virtual Cell::Count countNormal(const Data::BSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers) const;
        virtual Cell::Count countNormal(const Data::BoxTreeCell *c, const LayerSidePositionCellMap &layers) const;

        virtual void refine();
        virtual void addRefinedLayers();

        virtual void count();
        virtual Cell::Count count(const Data::BSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers) const;
        virtual Cell::Count count(const Data::BoxTreeCell *c, const LayerSidePositionCellMap &layers) const;

        virtual bool inLayer(const Data::BoxTreeCell *c, const LayerSidePositionCellMap &layers, UInt layer) const;

        virtual bool include(const Data::BoxTreeCell *c, UInt side, UInt pt, UInt coord, Real cmp) const;
        virtual void findInitialLayer(UInt side, const Data::GraphNodeList &cells,
            UInt pt, UInt coord, Real cmp, PositionCellMap &map) const;
        virtual void findInitialLayer(const Data::BSPTreeCell *cell, UInt side,
            const Data::GraphNodeList &cells, LayerSidePositionCellMap &layers) const;
        virtual void findLayer(const Data::GraphNodeSet &inCellsPrev,
            UInt oside, UInt opt, UInt coord, Real ocmp, PositionCellMap &map) const;
        virtual void findLayer(const Data::BSPTreeCell *cell, UInt layer, UInt side, LayerSidePositionCellMap &layers) const;

        virtual bool isOutCell(const Data::BoxTreeCell *c) const;
        virtual bool less(const Data::BoxTreeCell *cell, UInt dir, Real coord) const;
        virtual bool split(const Data::GraphNodeList &cells, UInt dir, Real coord,
            Data::GraphNodeList &cells1, Data::GraphNodeList &cells2, bool skipOut) const;

    protected:

        UInt dimension;

        const PartitionBSPTree *bsp;

        Data::BoxTree *tree;

        const MJMesh::Boundary *boundary;

        UInt numLayers;

        Parallel::TaskVector tasks;

#if USE_C__11
        PMJMesh::FEListHashMap *elementsMap;
#else
        PMJMesh::FEListMap *elementsMap;
#endif //#if USE_C__11

        CellMap map;

        UInt minLevel;
        UInt maxLevel;

        bool buildHistogram;
        bool excludeLayersInHistogram;
        bool onlyINCellsInHistogram;

    };
}

#endif //#if DEBUG_CELL_COUNTER

#endif //#ifndef _PMJMESH2_CELL_COUNTER_H_
