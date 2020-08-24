#ifndef _PMJMESH2_LOAD_BSP_TREE_GENERATOR_H_
#define _PMJMESH2_LOAD_BSP_TREE_GENERATOR_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh2/PartitionBSPTreeGenerator.h"

#define USE_ARRAY_ON_PARAMS true

namespace PMJMesh2
{
    class LoadBSPTreeGenerator : public PMJMesh2::PartitionBSPTreeGenerator
    {
    public:

        LoadBSPTreeGenerator();
        virtual ~LoadBSPTreeGenerator();

        virtual void setCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition);
        virtual void setProportional(bool proportional);
        virtual void setLoadThreshold(Real loadThreshold);

        virtual void execute();

/*//debug
#if USE_GUI
    protected:

        virtual void subdivide(PartitionBSPTreeCell *cell, const Split &split) const;
#endif //#if USE_GUI
//endebug*/

    protected:

        struct LoadSplit : public PMJMesh2::PartitionBSPTreeGenerator::Split
        {
            Real load[3]; //load[2] is the splitting load
        };

#if USE_C__11
        typedef std::unordered_map<Data::Position, Data::GraphNodeHash, std::hash<UInt> > PositionCellMap;
#else
        typedef std::map<Data::Position, Data::GraphNodeSet> PositionCellMap;
#endif //#if USE_C__11
#if (!USE_ARRAY_ON_PARAMS)
        typedef std::vector<PositionCellMap> SidePositionCellMap;
        typedef std::vector<SidePositionCellMap> LayerSidePositionCellMap;
#endif //#if USE_ARRAY_ON_PARAMS

        struct Params
        {
            Params();

#if USE_ARRAY_ON_PARAMS
            void resize(UInt numLayers, UInt numSides, bool layers, bool splitted, bool fixed);
            static void resize(PositionCellMap **&layers, UInt numLayers, UInt numSides);
            void clear(UInt numLayers, bool layers, bool splitted, bool fixed);
            static void clear(PositionCellMap **&layers, UInt numLayers);
#endif //#if USE_ARRAY_ON_PARAMS

            ULInt id;

            UInt dir;
            Real coord;

            Real limits[2];
            const Data::GraphNodeList *cells;

            Data::GraphNodeList splittedCells[2];
            Real splittedLoads[2];

            Data::GraphNodeList fixedCells[2];
            Real fixedLoads[2];

            bool equal;

            Data::GraphNodeList splittingCells;
            Real splittingLoad;

#if USE_ARRAY_ON_PARAMS
            PositionCellMap **layers;
            PositionCellMap **splittedLayers[2];
            PositionCellMap **fixedLayers[2];
#else
            LayerSidePositionCellMap layers;
            LayerSidePositionCellMap splittedLayers[2];
            LayerSidePositionCellMap fixedLayers[2];
#endif //#if USE_ARRAY_ON_PARAMS
            Real layersLoads[2];
            Real layersLimits[2];
        };

    protected:

        virtual Real load(const PartitionBSPTreeCell *cell) const;
        virtual Real innerLoad(const PartitionBSPTreeCell *cell) const;

        using PMJMesh2::PartitionBSPTreeGenerator::executeSubdivision;
        virtual PartitionBSPTreeCell *executeSubdivision(PartitionBSPTreeCell *cell, Int rank, Int size);

        virtual void adjustLoadsForLayers(Real &l0, Real &l1) const;
        virtual void adjustProportion(Real &l0, Real &l1) const;
        virtual bool better(Real diffLoad1, Real diffLoad2) const;
        virtual bool better(const Params &params1, const Params &params2) const;
        virtual bool equal(const Params &params) const;

        using PMJMesh2::PartitionBSPTreeGenerator::heavier;
        //virtual UInt heavier(PartitionBSPTreeCell *cell1, PartitionBSPTreeCell *cell2) const;
        virtual UInt heavier(const Params &params) const;
        virtual void move(Params &params, UInt i) const;

        /*virtual void distanceInBoxTree(Real min, Real max, UInt dir, Real coord, UInt &d1, UInt &d2) const;
        virtual void distanceInBoxTree(Real min, Real max, UInt dir, Real coord, const Data::BoxTreeCell *cell, UInt &d1, UInt &d2) const;
        virtual void combineDistancesInBoxTree(UInt dir, UInt d1Children[], UInt d2Children[], UInt &d1, UInt &d2) const;*/

        virtual Real initialSplitCoord(Real min, Real max, UInt dir, const Data::GraphNodeList *cells) const;
        virtual Real initialSplitCoord(Real min, Real max, UInt dir, const Data::GraphNodeList &cells) const;

        virtual PMJMesh2::PartitionBSPTreeGenerator::Split findBestSubdivision(PartitionBSPTreeCell *cell) const;
#if USE_ARRAY_ON_PARAMS
        virtual LoadSplit findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **layers, UInt dir) const;
#else
        virtual LoadSplit findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers, UInt dir) const;
#endif //#if USE_ARRAY_ON_PARAMS

#if USE_ARRAY_ON_PARAMS
        virtual bool findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **layers,
            LoadSplit &spl,
            Data::GraphNodeList &cells1, PositionCellMap **&layers1,
            Data::GraphNodeList &cells2, PositionCellMap **&layers2) const;
#else
        virtual bool findBestSubdivision(PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, const LayerSidePositionCellMap &layers,
            LoadSplit &spl,
            Data::GraphNodeList &cells1, LayerSidePositionCellMap &layers1,
            Data::GraphNodeList &cells2, LayerSidePositionCellMap &layers2) const;
#endif //#if USE_ARRAY_ON_PARAMS

        virtual bool findBestSubdivision(Params &params) const;

        virtual bool include(const Data::BoxTreeCell *c, UInt side, UInt pt, UInt coord, Real cmp) const;
        virtual void findInitialLayer(UInt side, const Data::GraphNodeList &cells, UInt pt, UInt coord, Real cmp, PositionCellMap &map/*, bool debug = false*/) const;
#if USE_ARRAY_ON_PARAMS
        virtual void findInitialLayer(const PartitionBSPTreeCell *cell, UInt side, const Data::GraphNodeList &cells, PositionCellMap **&layers) const;
#else
        virtual void findInitialLayer(const PartitionBSPTreeCell *cell, UInt side, const Data::GraphNodeList &cells, LayerSidePositionCellMap &layers) const;
#endif //#if USE_ARRAY_ON_PARAMS
#if USE_C__11
        virtual void findLayer(const Data::GraphNodeHash &inCellsPrev,
#else
        virtual void findLayer(const Data::GraphNodeSet &inCellsPrev,
#endif //#if USE_C__11
            UInt oside, UInt opt, UInt coord, Real ocmp, PositionCellMap &map) const;
#if USE_ARRAY_ON_PARAMS
        virtual void findLayer(const PartitionBSPTreeCell *cell, UInt layer, UInt side, PositionCellMap **&layers) const;
        virtual void findLayers(const PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, PositionCellMap **&layers) const;
#else
        virtual void findLayer(const PartitionBSPTreeCell *cell, UInt layer, UInt side, LayerSidePositionCellMap &layers) const;
        virtual void findLayers(const PartitionBSPTreeCell *cell, const Data::GraphNodeList &cells, LayerSidePositionCellMap &layers) const;
#endif //#if USE_ARRAY_ON_PARAMS
        virtual void splitLayers(Params &param) const;
        virtual void removeLoadFromLayers(Params &param) const;

    protected:

        bool proportional;
        bool creatingInnerFrontInDecomposition;
        Real loadThreshold;
        Int proportions[2];

        struct CellLoad
        {
#if USE_ARRAY_ON_PARAMS
            CellLoad() : load(-1.0), layers(NULL) {};
#else
            CellLoad() : load(-1.0) {};
#endif //#if USE_ARRAY_ON_PARAMS
            Data::GraphNodeList cells;
            Real load;
#if USE_ARRAY_ON_PARAMS
            PositionCellMap **layers;
#else
            LayerSidePositionCellMap layers;
#endif //#if USE_ARRAY_ON_PARAMS
        };

        struct NodeLayer
        {
            NodeLayer() : node(NULL), layer(0) {};
            NodeLayer(Data::GraphNode *n, UInt l) : node(n), layer(l) {};
            NodeLayer(const NodeLayer &nl) : node(nl.node), layer(nl.layer) {};

            Data::GraphNode *node;
            UInt layer;

            struct Less
            {
                bool operator()(const NodeLayer &n1, const NodeLayer &n2) const
                {
                    return n1.node < n2.node;
                }
            };

            struct Equal
            {
                bool operator()(const NodeLayer &n1, const NodeLayer &n2) const
                {
                    return n1.node == n2.node;
                }
            };

#if USE_C__11
            struct Hash
            {
                size_t operator()(const NodeLayer &n) const
                {
                    return std::hash<void *>()(n.node);
                }
            };
#endif //#if USE_C__11
        };

//public:
#if USE_C__11
        typedef std::unordered_map<ULInt, CellLoad> CellLoadMap;
        typedef std::unordered_set<NodeLayer, NodeLayer::Hash, NodeLayer::Equal> CellLayerSet;
#else
        typedef std::map<ULInt, CellLoad> CellLoadMap;
        typedef std::set<NodeLayer, NodeLayer::Less> CellLayerSet;
#endif //#if USE_C_11
//protected:

        mutable CellLoadMap loads;
        //mutable IdLayerSidePositionCellMap layers;
#if USE_C__11
        mutable Data::RealHashMap innerLoads;
#else
        mutable Data::RealMap innerLoads;
#endif //#if USE_C__11

/*#if USE_C__11
		const Data::RealHashMap *treeLoad;
#else
		const Data::RealMap *treeLoad;
#endif //#if USE_C__11*/

    protected:

        bool usingFrontTest;

        const MJMesh::Boundary *boundary;

        Data::BoxSearchTree *searchTree;

        Real interfaceBadAngleCos;
        Real interfaceShiftDelta;

    public:

        virtual void setUsingFrontTest(bool usingFrontTest);
        virtual bool isUsingFrontTest() const;

        virtual void setBoundary(const MJMesh::Boundary *boundary);

        virtual void setInterfaceBadAngleCos(Real interfaceBadAngleCos);
        virtual Real getInterfaceBadAngleCos() const;

        virtual void setInterfaceShiftDelta(Real interfaceShiftDelta);
        virtual Real getInterfaceShiftDelta() const;

    };
}

#endif //#ifndef _PMJMESH2_LOAD_BSP_TREE_GENERATOR_H_

