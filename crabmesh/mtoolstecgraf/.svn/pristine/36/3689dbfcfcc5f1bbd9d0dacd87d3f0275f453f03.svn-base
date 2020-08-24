#include "PMTools/AdvFront3D.h"

#if USE_MTOOLS

#include "PMTools/MshRegion3D.h"
#include "TetTopo.hpp"
#include "ReverseConn.hpp"

PMTools::AdvFront3D::AdvFront3D(NodeHash *NodeTable,FaceHash *FaceTable, bool noCracks) :
	MTools::AdvFront3D::AdvFront3D(NodeTable, FaceTable, noCracks)
{
	this->pmtoolsUseBoxTest = false;
	this->pmtoolsWorkerTask = false;
	this->pmtoolsEnableBackTracking = true;
	this->pmtoolsRangeProportion = 1.0;
	this->pmtoolsMaxBoundaryNodeId = 0;
	this->FaceHeap = NULL;

	this->TetTable = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    this->timeGeneration = this->timeImprovement = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PMTools::AdvFront3D::AdvFront3D() :
	MTools::AdvFront3D::AdvFront3D()
{
	this->pmtoolsUseBoxTest = false;
	this->pmtoolsWorkerTask = false;
	this->pmtoolsEnableBackTracking = true;
	this->pmtoolsRangeProportion = 1.0;
	this->pmtoolsMaxBoundaryNodeId = 0;

	if (this->FaceHeap)
	{
		delete this->FaceHeap;

		this->FaceHeap = NULL;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    this->timeGeneration = this->timeImprovement = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PMTools::AdvFront3D::~AdvFront3D()
{
	this->Oct_Tree = NULL;
}

void PMTools::AdvFront3D::SetOct_Tree(const FTools::OctTree *Oct_Tree)
{
	this->Oct_Tree = const_cast<FTools::OctTree *>(Oct_Tree);
}

void PMTools::AdvFront3D::SetUseBoxTest(bool useBoxTest)
{
	this->pmtoolsUseBoxTest = useBoxTest;
}

void PMTools::AdvFront3D::SetWorkerTask(bool workerTask)
{
	this->pmtoolsWorkerTask = workerTask;
}

void PMTools::AdvFront3D::SetEnableBackTracking(bool enableBackTracking)
{
	this->pmtoolsEnableBackTracking = enableBackTracking;
}

void PMTools::AdvFront3D::SetBoundingBox(const FTools::Vec3D &min, const FTools::Vec3D &max)
{
	this->pmtoolsBoxMin = min;
	this->pmtoolsBoxMax = max;
}

void PMTools::AdvFront3D::SetTreeBox(const FTools::Vec3D &min, const FTools::Vec3D &max)
{
	this->pmtoolsTreeBoxMin = min;
	this->pmtoolsTreeBoxMax = max;
}

void PMTools::AdvFront3D::SetRangeProportion(double rangeProportion)
{
	this->pmtoolsRangeProportion = rangeProportion;
}

void PMTools::AdvFront3D::SetMaxBoundaryNodeId(int maxBoundaryNodeId)
{
	this->pmtoolsMaxBoundaryNodeId = maxBoundaryNodeId;
}

void PMTools::AdvFront3D::SetTet_Topo(Msh3D::TetTopo *Tet_Topo)
{
	this->Tet_Topo = Tet_Topo;
}

void PMTools::AdvFront3D::SetTetTable(TetHash *TetTable)
{
	this->TetTable = TetTable;
}

void PMTools::AdvFront3D::SetNextElemId(int StartId)
{
	this->StartId = StartId;
}

bool PMTools::AdvFront3D::PMshOutBoxPt(const FTools::Vec3D &pt, double tolerance)
{
	for (int i = 0; i < 3; i++)
	{
		//if ((pt[i] <= this->pmtoolsBoxMin[i] - tolerance) || (pt[i] >= this->pmtoolsBoxMax[i] + tolerance))
		//if ((pt[i] <= pmtoolsBoxMin[i]) || (pt[i] >= pmtoolsBoxMax[i]))
		if ((pt[i] < pmtoolsBoxMin[i] - tolerance) || (pt[i] > pmtoolsBoxMax[i] + tolerance))
		{
			return true;
		}
	}

	return false;
}

bool PMTools::AdvFront3D::PMshOutBox(const FTools::Vec3D &imin, const FTools::Vec3D &imax, double tolerance)
{
	FTools::Vec3D min, max;

	for (int i = 0; i < 3; i++)
	{
		min[i] = std::max(imin[i], this->pmtoolsTreeBoxMin[i] + tolerance);
		max[i] = std::min(imax[i], this->pmtoolsTreeBoxMax[i] - tolerance);
	}

	return (this->PMshOutBoxPt(min, tolerance) || this->PMshOutBoxPt(max, tolerance));
}

namespace Msh3D
{
	extern FTools::Vec3D &GetCoord(int node_id, void *cdata);
	extern bool stopSmooth;
}

void PMTools::AdvFront3D::GenerateMesh(int start_id, int mat_id,
	ElemHash *elem_table, bool pyramids_only, bool restart)
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    this->timeGeneration = this->timeImprovement = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (InsertAndCheckOnly) throw InsertAndCheckTest() ;

    NumRestarts = 0 ;
    NumBackTracks = 0 ;
    NumPyrAttempts = 0 ;
    NumPyrExisting = 0 ;
    NumPyrGenerated = 0 ;
    NumTetAttempts = 0 ;
    NumTetExisting = 0 ;
    NumTetAdjacent = 0 ;
    NumTetAngle = 0 ;
    NumTetGenerated = 0 ;

    if (!restart) {

        // create a place to store the elements

        //debug changed by markos
        //TetTable = new TetHash(false) ;
        //if ((this->pmtoolsWorkerTask) || (!TetTable))
        if (!TetTable)
			TetTable = new TetHash(false) ;
        //end debug changed by markos
        PyramidTable = new PyramidHash(false) ;
        StartId = start_id ;

        // build the octtree

        BuildOctTree() ;
    }

    // do the boundary contraction

    int save_StartId = StartId ;
    int save_NextNodeId = NextNodeId ;
    int save_AdTreeIndx = AdTreeIndx ;
    IntNodeHash save_IntNodeTable = *IntNodeTable ;
    IntFaceHash save_IntFaceTable = *IntFaceTable ;
    //added by markos
    TetHash save_TetTable = *TetTable;
    //end added by markos
    //changed by markos
    //TetTopo save_TetTopo = *Tet_Topo ;
    Msh3D::TetTopo save_TetTopo = *Tet_Topo ;
    //end changed by markos
    //Dict<int,FFace*> save_AdTreeHash = *AdTreeHash ;
    //changed by markos
    //RangeTree save_AdTree = *AdTree ;
    RangeTree save_AdTree(0, NULL, static_cast<double *>(NULL), NULL) ;
    if (save_IntFaceTable.Len() > 0) save_AdTree = *AdTree ;
    //end changed by markos

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    while (!BoundaryContraction(pyramids_only)) {
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
        this->timeGeneration += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    	NumRestarts++ ;
        OptimalSphereFactor *= 0.95 ;
        OptimalSizeFactor *= 0.98 ;
        MinSizeReductionFactor *= 0.98 ;
        StartId = save_StartId ;
        NextNodeId = save_NextNodeId ;
        AdTreeIndx = save_AdTreeIndx ;
        *IntNodeTable = save_IntNodeTable ;
        *IntFaceTable = save_IntFaceTable ;
        *Tet_Topo = save_TetTopo ;
        //*AdTreeHash = save_AdTreeHash ;
        //changed by markos
        //*AdTree = save_AdTree ;
        if (save_IntFaceTable.Len() > 0) *AdTree = save_AdTree ;
        //end changed by markos
        TetTable->Clear() ;
        //added by markos
        *TetTable = save_TetTable ;
        //end added by markos
        PyramidTable->Clear() ;
        AdTreeHash->Clear() ;
        Enum = 0 ;
        Dict<MshRegion3D::FaceHashKey,FFace>::DictIterator iter(IntFaceTable) ;
        for (iter.First() ; iter.More() ; ++iter) {
            for (int i = 0 ; i<3 ; ++i) {
                 iter.EntryPtr()->nodes[i] =
                     IntNodeTable->Get(iter.EntryPtr()->node_id[i]) ;
            }
            AdTreeHash->Store(iter.EntryPtr()->ad_tree_indx,iter.EntryPtr()) ;
                 // iter.EntryPtr()->tet_topo_id =
                 //    Tet_Topo->GetFaceFromVerts(
                 //          iter.EntryPtr()->node_id[0],
                 //          iter.EntryPtr()->node_id[1],
                 //          iter.EntryPtr()->node_id[2]) ;
        }
        if (NumRestarts <= MaxVolumeRestarts) {
            //fprintf(stderr,"Backtracking Stagnating: restarting contraction\n") ;
            fprintf(stdout,"Backtracking Stagnating: restarting contraction\n") ;
        } else {
            throw Max3DRestartError() ;
        }

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
        start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    }

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    this->timeGeneration += Data::time() - start;
    start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    // build a hash table with shape measures for all elements

    Dict<int,double> ShapeMeasures ;
    TetHashIter eiter(TetTable) ;
    for (eiter.First() ; eiter.More() ; ++eiter) {
        Node *n0 = IntNodeTable->Get(eiter.Entry().node_id[0]) ;
        Node *n1 = IntNodeTable->Get(eiter.Entry().node_id[1]) ;
        Node *n2 = IntNodeTable->Get(eiter.Entry().node_id[2]) ;
        Node *n3 = IntNodeTable->Get(eiter.Entry().node_id[3]) ;
        double sm = TetShapeMeasure(n0->coord,n1->coord,
                                    n2->coord,n3->coord) ;
        ShapeMeasures.Store(eiter.Key(),sm) ;
    }

    UpdateQuality(Quality,&ShapeMeasures) ;
    if (SmoothingStatusCallback != 0) {
        (SmoothingStatusCallback)
            (MshRegion3D::ORIGINAL,0,Quality) ;
    }

    // check to see if we want to do any face swapping

    if (DoFaceSwapping) {
        FaceSwapping(ShapeMeasures) ;
        UpdateQuality(Quality,&ShapeMeasures) ;
        if (SmoothingStatusCallback != 0) {
            (SmoothingStatusCallback)
                (MshRegion3D::FACE_SWAPPING,0,Quality) ;
        }
    }

    // now check for edge swapping

    if (DoEdgeSwapping) {
        int NumSwaps = 1;
        for (int j=0 ; j<NumEdgeSwappingSweeps && NumSwaps > 0; ++j) {
            NumSwaps = EdgeSwapping(ShapeMeasures) ;
            UpdateQuality(Quality,&ShapeMeasures) ;
            if ((SmoothingStatusCallback != 0) &&
                 !SmoothingSummaryOnly) {
                (SmoothingStatusCallback)
                    (MshRegion3D::EDGE_SWAPPING,j+1,Quality) ;
            }
        }
        if ((SmoothingStatusCallback != 0) &&
             SmoothingSummaryOnly) {
            (SmoothingStatusCallback)
                (MshRegion3D::EDGE_SWAPPING,NumEdgeSwappingSweeps,Quality) ;
        }
    }

    // do nodal smoothing

    if (DoSmoothNodes && !pyramids_only) {

        // build the reverse connectivity tables
        // rev_nodes takes a node as input and returns a list
        // of adjacent nodes.
        // rev_elems takes a node as input and returns a list
        // of elements

        //changed by markos
        //ReverseConn<int> rev_nodes ;
        Msh3D::ReverseConn<int> rev_nodes ;
        //ReverseConn<ElemRef> rev_elems ;
        Msh3D::ReverseConn<ElemRef> rev_elems ;
        //end changed by markos
        TetHashIter eiter(TetTable) ;
        for (eiter.First() ; eiter.More() ; ++eiter) {
            int elem_id = eiter.Key() ;
            int *enodes = eiter.Entry().node_id ;
            for (int i=0 ; i<4 ; ++i) {
                for (int j=0 ; j<4 ; ++j) {
                    if (i != j) {
                         rev_nodes.AddReference(enodes[i],enodes[j]) ;
                    }
                }
            }
            rev_elems.AddReference(enodes[0],ElemRef(S_TETRAHEDRAL,elem_id)) ;
            rev_elems.AddReference(enodes[1],ElemRef(S_TETRAHEDRAL,elem_id)) ;
            rev_elems.AddReference(enodes[2],ElemRef(S_TETRAHEDRAL,elem_id)) ;
            rev_elems.AddReference(enodes[3],ElemRef(S_TETRAHEDRAL,elem_id)) ;
        }

        PyramidHashIter piter(PyramidTable) ;
        for (piter.First() ; piter.More() ; ++piter) {
            int elem_id = piter.Key() ;
            int *enodes = piter.Entry().node_id ;
            for (int i=0 ; i<5 ; ++i) {
                for (int j=0 ; j<5 ; ++j) {
                    if (i != j) {
                         rev_nodes.AddReference(enodes[i],enodes[j]) ;
                    }
                }
            }
            rev_elems.AddReference(enodes[0],ElemRef(S_PYRAMID,elem_id)) ;
            rev_elems.AddReference(enodes[1],ElemRef(S_PYRAMID,elem_id)) ;
            rev_elems.AddReference(enodes[2],ElemRef(S_PYRAMID,elem_id)) ;
            rev_elems.AddReference(enodes[3],ElemRef(S_PYRAMID,elem_id)) ;
            rev_elems.AddReference(enodes[4],ElemRef(S_PYRAMID,elem_id)) ;
        }

        // do laplace smoothing

        int j = 0 ;
        int moved = 1 ;
        while (moved) {
            moved = LaplaceSmoothing(rev_nodes,rev_elems,ShapeMeasures) ;
            UpdateQuality(Quality,&ShapeMeasures) ;
            if ((SmoothingStatusCallback != 0) &&
                 !SmoothingSummaryOnly) {
                (SmoothingStatusCallback)
                    (MshRegion3D::LAPLACE_SMOOTHING,j+1,Quality) ;
            }
            ++j ;

            //added by markos
            if ((Msh3D::stopSmooth) && (moved < 0.3*IntNodeTable->Len())) break;
            //end added by markos
        }

        if ((SmoothingStatusCallback != 0) &&
             SmoothingSummaryOnly) {
            (SmoothingStatusCallback)
                (MshRegion3D::LAPLACE_SMOOTHING,j+1,Quality) ;
        }
    }

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    this->timeImprovement += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    // copy pyramids into the element table

    PyramidHashIter piter(PyramidTable) ;
    for (piter.First() ; piter.More() ; ++piter) {
        MshRegion3D::MshElem nelem ;
        nelem.elem_id = start_id ;
        nelem.mat_id  = mat_id ;
        nelem.num_nodes = 5 ;
        for (int i=0 ; i<5 ; ++i)
            nelem.nodes[i] = piter.Entry().node_id[i] ;
        elem_table->Store(start_id,nelem) ;
        ++start_id ;
    }

    // copy the tets into the element table

    for (eiter.First() ; eiter.More() ; ++eiter) {
		//added by markos
		if ((!this->pmtoolsWorkerTask) && (eiter.Key() < StartId))
		{
			MshRegion3D::MshElem nelem ;
			nelem.elem_id = eiter.Key() ;
			nelem.mat_id  = mat_id ;
			nelem.num_nodes = 4 ;
			for (int i=0 ; i<4 ; ++i)
				nelem.nodes[i] = eiter.Entry().node_id[i] ;
			elem_table->Store(nelem.elem_id,nelem) ;

			continue;
		}
		//end added by markos
        MshRegion3D::MshElem nelem ;
        nelem.elem_id = start_id ;
        nelem.mat_id  = mat_id ;
        nelem.num_nodes = 4 ;
        for (int i=0 ; i<4 ; ++i)
            nelem.nodes[i] = eiter.Entry().node_id[i] ;
        elem_table->Store(start_id,nelem) ;
        ++start_id ;
    }

    // copy in any new nodes

    IntNodeHashIter niter(IntNodeTable) ;
    for (niter.First() ; niter.More() ; ++niter) {
        //changed by markos
        //if (niter.Entry().id >= save_NextNodeId) {
        if ((this->pmtoolsWorkerTask) ? (niter.Entry().id >= save_NextNodeId) : (niter.Entry().id > this->pmtoolsMaxBoundaryNodeId)) {
		//end changed by markos
            MshRegion3D::IntNode nnode ;
            nnode.id = niter.Entry().id ;
            nnode.coord = niter.Entry().coord ;
            nnode.type = MshRegion3D::INTERIOR ;
            nnode.motion = MshRegion3D::M_FLOATING ;
            nnode.corner = true ;
            NodeTable->Store(niter.Entry().id,nnode) ;
        }
    }

    delete TetTable ;
    TetTable = 0 ;
}

//redefines by markos
#define TWO_PI 6.2831853071795862
#define ACCUTE_THRESHOLD 1.3
#define FACE0_MASK (1)
#define FACEI_MASK(i) (1<<(i))
//end redefines by markos

//namespaced extern variables
namespace Msh3D
{
	extern int FaceMap[4][3];
}
//end namespaced extern variables

bool PMTools::AdvFront3D::BoundaryContraction(bool pyramids_only)
{

for (int ii=0 ; ii<NUM_LEVELS ; ++ii) {
   found_avg[ii] = 0 ;
   found_adj[ii] = 0 ;
}

    num = Enum ;

    // First place all the boundary faces in a priority queue
    // (heap structure) so that we can extract the faces in ranked order

    FaceHeap = new FFaceHeap ;

    Dict<MshRegion3D::FaceHashKey,FFace>::DictIterator fiter(IntFaceTable) ;
    for (fiter.First() ; fiter.More() ; ++fiter) {
        FFace *face = fiter.EntryPtr() ;
        if (!face->quad)
            face->heap_handle = FaceHeap->InsertWithHandle(face) ;
    }

    // place all the candidate nodes into a search tree

    if (!PtTree)
	{
		List<Vec3D> cds ;
		List<int> ids ;
		IntNodeHashIter niter(IntNodeTable) ;
		for (niter.First() ; niter.More() ; ++niter) {
			ids.Append(niter.Key()) ;
			cds.Append(niter.Entry().coord) ;
		}
		PtTree = new PointTree(ids.Len(),cds.Data(),ids.Data()) ;
	}

#ifdef PLOT_ORIGINAL_FRONT
    DebugDumpFront(ofout,0,true) ;
#endif

    // If there are any quad faces, then add pyramids here first

    if (NumQuads > 0) DoBoundaryQuads(PtTree) ;

    if (pyramids_only) {
        delete FaceHeap ;
        FaceHeap = NULL;
        delete PtTree ;
        PtTree = NULL;
        return true ;
    }

#ifdef PLOT_ORIGINAL_FRONT
    DebugDumpFront(ofout,1,true) ;
#endif

    //double min_angle = 10.0 ;

    // set up the adjacent pointers and angles in the advancing front

    FaceHeap->Clear() ;
    Dict<MshRegion3D::FaceHashKey,FFace>::DictIterator ifiter = IntFaceTable->Iterator() ;
    for (ifiter.First() ; ifiter.More() ; ++ifiter) {
        FFace* face = ifiter.EntryPtr() ;
        int adj_faces[3],adj_vts[3] ;
        Tet_Topo->GetAdjacentFaces(face->tet_topo_id,Msh3D::TetTopo::FREE_REGION,adj_faces) ;

        //added by markos
        for (int i = 0; i < 3; i++)
        {
			face->adjacent[i] = NULL;
			face->angle[i] = 7.0; //above 2*PI
        }
        //end added by markos

        for (int j=0 ; j<3 ; ++j) {
			//added by markos
			if (adj_faces[j] < 0) continue;
			//end added by markos
            Tet_Topo->GetVertAboutFace(adj_faces[j],&adj_vts[0],
                                       &adj_vts[1],&adj_vts[2]) ;
            FFace* adj = IntFaceTable->Get(RFaceHashKey(3,adj_vts)) ;

            //added by markos
            if (!adj)
			{
			    //fprintf(stderr,"(1) No adjacent face[%d] for face %d: %d %d %d, which should be %d: %d %d %d\n",
			    fprintf(stdout,"(1) No adjacent face[%d] for face %d: %d %d %d, which should be %d: %d %d %d\n",
                    j, face->tet_topo_id, face->node_id[0], face->node_id[1], face->node_id[2],
                    adj_faces[j], adj_vts[0], adj_vts[1], adj_vts[2]) ;

				throw AdjacentFaceError();
			}
            //added by markos

            // find the node that is not part of the base face

            int opp = -1 ;
            for (int k=0 ; k<3 ; ++k) {
                if ((adj_vts[k] != face->node_id[0]) &&
                    (adj_vts[k] != face->node_id[1]) &&
                    (adj_vts[k] != face->node_id[2])) {
                    opp = adj_vts[k] ;
                    break ;
                }
            }
            assert(opp >= 0) ;

            // find the orientation

            int next = -1 ;
            int opp_indx = -1 ;
            for (int k=0 ; k<3 ; ++k) {
                if (adj->node_id[k] == opp) {
                    next = (k+2)%3 ;
                    opp_indx = k ;
                    break ;
                }
            }
            assert(next >= 0) ;

            // update this faces pointer

            for (int k=0 ; k<3 ; ++k) {
                if (face->node_id[k] == adj->node_id[next]) {
                    face->adjacent[k] = adj ;

                    double angle = DiahedralAngle(face->coords[k],
                                                  face->coords[(k+1)%3],
                                                  face->coords[(k+2)%3],
                                                  adj->coords[opp_indx]) ;

                    Vec3D mid = 0.5 * (face->coords[k] + face->coords[(k+1)%3]) ;
                    Vec3D v1 = (adj->coords[opp_indx] - mid).Normalize() ;
                    if ((v1 * face->normal) < 0) angle = TWO_PI - angle ;

                    face->angle[k] = angle  ;
                    if (angle == 0.0) face->angle[k] = 6.283 ; // crack front
// if (angle < min_angle) {
//     std::cerr << angle*180/PI << ' ' << face->node_id[0]
//                        << ' ' << face->node_id[1]
//                        << ' ' << face->node_id[2] << std::endl ;
//     min_angle = angle ;
// }
                    break ;
                }
            }
        }

        face->angle[3] = face->angle[0] ;
        if (face->angle[1] < face->angle[3]) face->angle[3] = face->angle[1] ;
        if (face->angle[2] < face->angle[3]) face->angle[3] = face->angle[2] ;
        face->heap_handle = FaceHeap->InsertWithHandle(face) ;
    }

//     // check
//
//     for (int i=0 ; i<FaceHeap->Len() ; ++i) {
//         for (int j=0 ; j<3 ; ++j) {
//             FFace* adj = (*front_faces[i])->adjacent[j] ;
//             int num = 0 ;
//             for (int k=0 ; k<3 ; ++k) {
//                 if (adj->node_id[k] == (*front_faces[i])->node_id[j]) ++num ;
//             }
//             for (int k=0 ; k<3 ; ++k) {
//                 if (adj->node_id[k] == (*front_faces[i])->node_id[(j+1)%3]) ++num ;
//             }
//             assert(num == 2) ;
//         }
//     }
//     std::cerr << "checks: " << std::endl ;

    // loop until there are no more faces in the boundary

    NodeRefHeap node_heap ;
    List<Vec3D> rejected ;

    while ((FaceHeap->Len() > 0) &&
           ((DebugStopElem == -1) || (StartId < DebugStopElem))) {

#ifdef PLOT_FRONT
    if (StartId == PLOT_FRONT) {
        DebugDumpFront(fout,1,true) ;
    }
#endif

        NumTetAttempts++ ;

        // get the next face in the boundary

        FFace **ptr = FaceHeap->ViewMin() ;
        FFace *entry = *ptr ;

        //added by markos
        if (this->pmtoolsWorkerTask)
        {
        	if (entry->use >= 3)
			{
				break;
			}
        }

        if (this->pmtoolsUseBoxTest)
        {
			//double tolerance = 0.01*entry->area;
			double tolerance = 0.0;

			bool out = ((this->PMshOutBoxPt(entry->coords[0], tolerance)) ||
				(this->PMshOutBoxPt(entry->coords[1], tolerance)) ||
				(this->PMshOutBoxPt(entry->coords[2], tolerance)));

			if (out)
			{
				ptr = FaceHeap->GetMin();
				entry->use = 4;
				entry->heap_handle = FaceHeap->InsertWithHandle(entry);

				entry->nodes[0]->motion = M_FIXED;
				entry->nodes[1]->motion = M_FIXED;
				entry->nodes[2]->motion = M_FIXED;

				this->Tet_Topo->ChangeToTemporaryOuterFace(entry->tet_topo_id);

				continue;
			}
        }
        //end added by markos

        int p_nd[4] = {entry->nodes[0]->id,entry->nodes[1]->id,
                       entry->nodes[2]->id,-1} ;
        int p_nd_rev[4] = {p_nd[0],p_nd[2],p_nd[1],p_nd[3]} ;

        int adj_vts[3] ;

        Tet_Topo->GetAdjacentVerts(entry->tet_topo_id,Msh3D::TetTopo::FREE_REGION,adj_vts) ;

        int exists = FACE0_MASK ;
        bool ideal_phase = (entry->use == 0) ;

        if (NoInternalNodes) ideal_phase = false ;

        bool valid = false ;
        node_heap.Clear() ;
        Vec3D optim_loc = IdealNodeCoord(entry) ;

        //added by markos
        if (this->pmtoolsUseBoxTest)
        {
			double size = (optim_loc - entry->centroid).Magnitude();
			//double tolerance = 0.01*entry->area;
			double tolerance = 0.0;

			Vec3D min, max;
			min = max = optim_loc;

			double factor = size*this->pmtoolsRangeProportion*1.5;
			//double factor = size*this->pmtoolsRangeProportion;

			for (int i = 0; i < 3; i++)
			{
				min[i] -= factor;
				max[i] += factor;
			}

			bool out = (this->PMshOutBox(min, max, tolerance));

			if (out)
			{
				ptr = FaceHeap->GetMin();

				entry->use = 5;
				entry->heap_handle = FaceHeap->InsertWithHandle(entry);

				entry->nodes[0]->motion = M_FIXED;
				entry->nodes[1]->motion = M_FIXED;
				entry->nodes[2]->motion = M_FIXED;

				this->Tet_Topo->ChangeToTemporaryOuterFace(entry->tet_topo_id);

				continue;
			}
        }
        //end added by markos

        // if the min adjacent angle is small check to see if we should
        // form an element based on the angle

        if (entry->angle[3] < ACCUTE_THRESHOLD) {
            if (CheckAccuteAngle(entry,optim_loc,p_nd,p_nd_rev,exists)) {
               NumTetAngle++ ;
               AddTetToMesh(entry,p_nd,p_nd_rev,exists) ;
               continue ;
            }
            exists = FACE0_MASK ;
        }

        // first look at the face's adjacent faces and see if any of
        // the far vertices will allow us to make a decent element

//         if (CheckAdjacentFaces(entry,p_nd,p_nd_rev,adj_vts,rejected,exists)) {
//             NumTetAdjacent++ ;
//             AddTetToMesh(entry,p_nd,p_nd_rev,exists) ;
//             continue ;
//         }

        // get a ranked list of existing nodes that we will check to
        // see if we can make a valid element.

        GetCandidateNodeList(entry,adj_vts,node_heap) ;

        // start with the node with the best ranking.  Check to make
        // sure it will form a valid element (i.e., does not intersect
        // the current boundary).  If the element is invalid go on
        // to the next.

        valid = false ;
        rejected.Clear() ;
        NodeRef *ref = node_heap.GetMin() ;
        while (ref != 0) {

            Node *cap_node = ref->node ;

            // now we need to check to see if this node has
            // any mates.  If so, we need to determine which
            // one makes the best fit.

            MateList *mlist = Mates->Get(cap_node->id) ;
            if (mlist != 0) {
                cap_node = DetermineNode(entry,cap_node,mlist) ;
            }
            if ((cap_node->id == p_nd[0]) ||
                (cap_node->id == p_nd[1]) ||
                (cap_node->id == p_nd[2])) {
                ref = node_heap.GetMin() ;
                continue ;
            }
            p_nd[3] = cap_node->id ;

            // check the element

            if (CheckForValid(entry,p_nd,p_nd_rev,cap_node,rejected,exists)) {
                NumTetExisting++ ;
                AddTetToMesh(entry,p_nd,p_nd_rev,exists) ;
                valid = true ;
                break ;
            }

            // if this node was not accepted and it is not a mate
            // node then store it in the rejected list

            bool add = true ;
            for (int ll=0 ; ll<3 ; ++ll) {
                if (cap_node->id == p_nd[ll]) {
                    add = false ;
                    break ;
                }
                MateList *mlist = Mates->Get(p_nd[ll]) ;
                if ((mlist != 0) &&
                    mlist->HasElement(cap_node->id)) {
                    add = false ;
                    break ;
                }
            }
            if (add) rejected.Append(cap_node->coord) ;
            exists = FACE0_MASK ;
            ref = node_heap.GetMin() ;
        }
        if (valid) continue ;

        // if no candidate node was found then we try to
        // insert a node at the optimal position (if this
        // is the ideal phase)

        if (ideal_phase &&
            CheckOptimal(entry,p_nd,p_nd_rev,rejected,optim_loc,exists)) {
            NumTetGenerated++ ;
            AddTetToMesh(entry,p_nd,p_nd_rev,exists) ;
            PtTree->AddToTree(optim_loc,p_nd[3]) ;
            continue ;
        }


        // if we get here we have not been able to create a tet even
        // after inserting an additional node.

        // If the face has not gone through the list twice yet,
        // pop it off the heap, increment the use count, and put
        // it back in the heap.  The ordering procedure associated
        // with heap sorts first on the use count.

        if (entry->use < 2) {
            ptr = FaceHeap->GetMin() ;
            entry->use++ ;
            entry->heap_handle = FaceHeap->InsertWithHandle(entry) ;
            continue ;

        // else we've been through the list twice, so we backtrack

        } else {

        	NumBackTracks++ ;

            // construct a star-shaped polygon

#ifdef DEBUG_LOGGING
            //cerr << "Backtracking: " << StartId << endl ;
#endif
            DebugBackTracking = false ;
//cerr << "Backtracking: " << StartId << endl ;

#ifdef DEBUG_BACKTRACKING
            if (StartId >= START_ID) {
                DebugBackTracking = true ;
                DebugDumpFront(btout,StartId,true) ;
            }
#endif

            //added by markos
            if (this->pmtoolsWorkerTask)
            {
            	if (this->pmtoolsEnableBackTracking)
				{
					DoBacktrack(entry);
				}
				else
				{
					ptr = FaceHeap->GetMin() ;
					entry->use++ ;
					entry->heap_handle = FaceHeap->InsertWithHandle(entry) ;
					this->Tet_Topo->ChangeToTemporaryOuterFace(entry->tet_topo_id);
				}
            }
            else
            {
			//end added by markos
				if (!DoBacktrack(entry)) {
					delete FaceHeap ;
					FaceHeap = NULL;
					delete PtTree ;
					PtTree = NULL;
					return false ;   // stagnating
				}

			//added by markos
            }
            //end added by markos
        }
    }

    //added by markos
    if (this->pmtoolsWorkerTask)
    {
		//setting nodes in remaining faces as fixed

		//this should not be necessary if we set them as fixed once we find out
		//  that the face should not advance anymore (or back-track)

		PMTools::AdvFront3D::FFace ***faces = FaceHeap->GetEntryList();
		int len = FaceHeap->Len();

		for (int i = 0; i < len; i++)
		{
			PMTools::AdvFront3D::FFace *face = (*faces[i]);

			face->nodes[0]->motion = M_FIXED;
			face->nodes[1]->motion = M_FIXED;
			face->nodes[2]->motion = M_FIXED;

			this->Tet_Topo->ChangeToTemporaryOuterFace(face->tet_topo_id);
		}

		delete [] faces;
    }
    //end added by markos

    //commented by markos
    //delete FaceHeap ;
    //end commented by markos
    delete PtTree ;
    PtTree = NULL;
    return true ;
}

bool PMTools::AdvFront3D::ConstructStarShapedPoly(
    FFace **bface,
    Dict<int,int> &ttfaces,
    Dict<int,int> &ttnodes,
    Dict<int,FFace *> &ff_table,
    PointTree *point_tree,
    AdvFront3D::Node** trial)
{
    // first thing we want to do is to get a list of all the nodes
    // and topo faces for the region we are trying to mesh.

    int faceid = Tet_Topo->GetFaceFromVerts((*bface)->node_id[0],
                     (*bface)->node_id[1],(*bface)->node_id[2]) ;
    Tet_Topo->GetVertFaceAboutRegion(faceid,Msh3D::TetTopo::FREE_REGION,ttfaces,ttnodes) ;

    //added by markos
    int pmtoolsUseLimit = 3;

    if ((ttfaces.Len() == 0) && (ttnodes.Len() == 0))
	{
		*trial = 0;
		return false;
	}
    //end added by markos

// Dict<int,int>::DictIterator n_iter(&ttnodes) ;
// for (n_iter.First() ; n_iter.More() ; ++n_iter) {
//     if ((n_iter.Key() == 24922) || (n_iter.Key() == 25326) || (n_iter.Key() == 25507)) {
//         std::cerr << "Found Node of Interest" << std::endl ;
//         break ;
//     }
// }


    // check to see if we are stagnating

    if (((*bface)->tet_topo_id == LastBacktrackId) &&
        (ttfaces.Len() == LastBacktrackNum)) {
        return(true) ;
    } else {
        LastBacktrackId = (*bface)->tet_topo_id ;
        LastBacktrackNum = ttfaces.Len() ;

        FaceHashKey key((*bface)->node_id) ;
        int *cnt = StagnateTable.Get(key) ;
        if (cnt == 0) {
            StagnateTable.Store(key,1) ;
        } else {
            *cnt += 1 ;
            //if (*cnt > 4) return(true) ;
            if (*cnt > 10) return(true) ;
       }
    }

#ifdef DEBUG_BACKTRACKING
    if (DebugBackTracking) {
         Vec3D tmp ;
         DebugPlotBacktrack(btout,**bface,ttfaces,ttnodes,tmp,false) ;
         std::cout << "f" << endl ;
    }
#endif
//Vec3D ttmp ;
//DebugPlotBacktrack(**bface,ttfaces,ttnodes,ttmp,false) ;

    // build a table of face pointers

    ff_table.Clear() ;
    Dict<int,int>::DictIterator face_iter(&ttfaces) ;
    for (face_iter.First() ; face_iter.More() ; ++face_iter) {
        int vts[3] ;
        Tet_Topo->GetVertAboutFace(face_iter.Key(),&vts[0],
                                  &vts[1],&vts[2]) ;
// int tnum = 0 ;
// for (int i=0 ; i<3 ; ++i) {
//     if (vts[i] == 87234 || vts[i] == 88664 || vts[i] == 87298) tnum++ ;
// }
// if (tnum == 3) {
//     std::cerr << "Found back face" << std::endl ;
// }

        RFaceHashKey key(3,vts) ;
//FFace *tmp = IntFaceTable->Get(key) ;
//assert(tmp != 0) ;
        ff_table.Store(face_iter.Key(),IntFaceTable->Get(key)) ;
    }

    // loop until we have a valid point

    int num  = 0 ;
    Dict<int,int> crossings ;
    while (1) {

		//added by markos
    	//if (this->pmtoolsWorkerTask)
    	{
			bool found = false;

			Dict<int,FFace *>::DictIterator fit = ff_table.Iterator();
			for (fit.First(); fit.More(); fit.Next())
			{
				if ((!fit.Entry()) ||
					((!fit.Entry()->boundary) && (!this->Tet_Topo->HasFace(fit.Entry()->tet_topo_id))))
				{
					throw BackTrackingFaceError();
				}

				if ((this->pmtoolsWorkerTask) && (fit.Entry()->use > pmtoolsUseLimit))
				{
					found = true;

					break;
				}

				if ((this->pmtoolsWorkerTask) && (!fit.Entry()->boundary) && (this->Tet_Topo->IsOuterFace(fit.Entry()->tet_topo_id)))
				{
					fit.Entry()->boundary = true;
				}
			}

			if (found)
			{
				*trial = 0;
				return false;
			}
    	}
    	//end added by markos

    	int baseid = Tet_Topo->GetFaceFromVerts(
                              (*bface)->node_id[0],
                              (*bface)->node_id[1],
                              (*bface)->node_id[2]) ;

		if ( baseid == -1 ) {
            *trial = 0 ;
            return(false) ;
        }

        // see if we can find a center point for this polyhedron
        // and also find the face we should use as a base
        // face (this is the face with the most crossings
        // in the case that there are crossing).  Note,
        // bface, center, and counts are changed as side-effects to
        // this call.

        Vec3D center ;
        crossings.Clear() ;
        if (FindCenterCoord(bface,ttfaces,ttnodes,
                            ff_table,center,crossings)) {
            *trial = new Node(NewNodeId(),center,true) ;
            return(false) ;
        }

//if ((num%10) == 0)
         if (num >= 30) {
             *trial = 0 ;
             return(false) ;
         }
         if (DebugBackTracking) {

//              PolyKernel pk ;
//              Dict<int,int>::ConstDictIterator face_iter = ttfaces.ConstIterator() ;
//              for (face_iter.First() ; face_iter.More() ; ++face_iter) {
//                  int vts[3] ;
//                  Tet_Topo->GetVertAboutFace(face_iter.Key(),&vts[0],
//                                             &vts[1],&vts[2]) ;
//                  RFaceHashKey key(3,vts) ;
//                  FFace *face = IntFaceTable->Get(key) ;
//                  pk.AddTriangle(face->node_id[0],face->node_id[1],face->node_id[2]) ;
//                  pk.AddVertex(face->node_id[0],face->coords[0]) ;
//                  pk.AddVertex(face->node_id[1],face->coords[1]) ;
//                  pk.AddVertex(face->node_id[2],face->coords[2]) ;
//              }
//              List<Vec3D> vts ;
//              pk.FindKernelVts(vts) ;
//              std::cerr << "Num Vts: " << vts.Len() << std::endl ;

#ifdef DEBUG_BACKTRACKING
             DebugPlotBacktrack(btout,**bface,
                 ttfaces,ttnodes,center,true) ;
             btout << "f" << endl ;
#endif
         }

		++num ;

        // find the face with the max number of crossings that
        // is not a boundary face, if any

        List<int> FacesToDelete ;

        int max = 0 ;
        Dict<int,int>::DictIterator iter(&crossings) ;
        for (iter.First() ; iter.More() ; ++iter) {
            int vts[3] ;
            int rgn0,rgn1 ;
            Tet_Topo->GetVertAboutFace(iter.Key(),&vts[0],
                                      &vts[1],&vts[2]) ;
			RFaceHashKey key(3,vts) ;
            FFace *tmp = IntFaceTable->Get(key) ;
            Tet_Topo->GetRegionAboutFace(tmp->tet_topo_id,&rgn0,&rgn1) ;
            //changed by markos
            //if (!tmp->boundary && (iter.Entry() > max) &&
            //    (rgn0 != Msh3D::TetTopo::PYRAM_REGION) && (rgn1 != Msh3D::TetTopo::PYRAM_REGION)) {
			if (!tmp->boundary && (iter.Entry() > max) &&
                (rgn0 != Msh3D::TetTopo::PYRAM_REGION) && (rgn1 != Msh3D::TetTopo::PYRAM_REGION) &&
                (this->pmtoolsWorkerTask ? (tmp->use <= pmtoolsUseLimit) : true)) {
			//end changed by markos
                max = iter.Entry() ;
            }
        }

        for (iter.First() ; iter.More() ; ++iter) {
            int vts[3] ;
            int rgn0,rgn1 ;
            Tet_Topo->GetVertAboutFace(iter.Key(),&vts[0],
                                      &vts[1],&vts[2]) ;
            RFaceHashKey key(3,vts) ;
            FFace *tmp = IntFaceTable->Get(key) ;
            Tet_Topo->GetRegionAboutFace(tmp->tet_topo_id,&rgn0,&rgn1) ;
            //changed by markos
            //if (!tmp->boundary && (iter.Entry() == max) &&
            //    (rgn0 != Msh3D::TetTopo::PYRAM_REGION) && (rgn1 != Msh3D::TetTopo::PYRAM_REGION)) {
            if (!tmp->boundary && (iter.Entry() == max) &&
                (rgn0 != Msh3D::TetTopo::PYRAM_REGION) && (rgn1 != Msh3D::TetTopo::PYRAM_REGION) &&
                (this->pmtoolsWorkerTask ? (tmp->use <= pmtoolsUseLimit) : true)) {
			//end changed by markos
                FacesToDelete.Append(iter.Key()) ;
            }
        }

        // if we have no non-boundary faces with crossings then
        // delete the smallest face.  Otherwise delete all faces
        // with max number of crossings.

// if (DebugBackTracking) {
//     int vts[3] = {19202,19203,20465} ;
//     RFaceHashKey key(3,vts) ;
//     FFace *tmp = IntFaceTable->Get(key) ;
//     std::cerr << "tmp face: " << tmp << std::endl ;
// }

        if (FacesToDelete.Len() == 0) {

            bool have_one = false ;
            int rgn0,rgn1 ;
            double min_area = 1e26 ;

            Dict<int,FFace *>::DictIterator iter(&ff_table) ;
            for (iter.First() ; iter.More() ; ++iter) {
                FFace *face = iter.Entry() ;
                Tet_Topo->GetRegionAboutFace(face->tet_topo_id,&rgn0,&rgn1) ;

                //changed by markos
                //if (!face->boundary &&
                //    (rgn0 != Msh3D::TetTopo::PYRAM_REGION) && (rgn1 != Msh3D::TetTopo::PYRAM_REGION) &&
                //    (rgn0 != Msh3D::TetTopo::BDRY_REGION) && (rgn1 != Msh3D::TetTopo::BDRY_REGION)) {
				if (!face->boundary &&
                    (rgn0 != Msh3D::TetTopo::PYRAM_REGION) && (rgn1 != Msh3D::TetTopo::PYRAM_REGION) &&
                    (rgn0 != Msh3D::TetTopo::BDRY_REGION) && (rgn1 != Msh3D::TetTopo::BDRY_REGION) &&
                    (this->pmtoolsWorkerTask ? (face->use <= pmtoolsUseLimit) : true)) {
				//end changed by markos
                    if (!have_one || (face->area < min_area)) {
                        have_one = true ;
                        *(bface) = face ;
                        min_area = face->area ;
                    }
                }
            }

            //added by markos
            if ((this->pmtoolsWorkerTask) && (!have_one))
            {
            	*trial = 0;
				return false;
            }
            //end added by markos

// assert(have_one) ;
// assert(!(*bface)->boundary) ;
// int trgn0,trgn1 ;
// Tet_Topo->GetRegionAboutFace((*bface)->tet_topo_id,&trgn0,&trgn1) ;
// assert(trgn0 != Msh3D::TetTopo::PYRAM_REGION) ;
// assert(trgn1 != Msh3D::TetTopo::PYRAM_REGION) ;

			DeleteShadowFace(bface,ttfaces,ttnodes,ff_table,
                             center,point_tree) ;

			//added by markos
			if ((ttfaces.Len() == 0) && (ttnodes.Len() == 0))
			{
				*trial = 0;
				return false;
			}
			//end added by markos

        } else {

            for (int i=0 ; i<FacesToDelete.Len() ; ++i) {
                int vts[3] ;
                if (!Tet_Topo->HasFace(FacesToDelete[i])) continue ;
                Tet_Topo->GetVertAboutFace(FacesToDelete[i],&vts[0],
                                              &vts[1],&vts[2]) ;
				RFaceHashKey key(3,vts) ;
                *(bface) = IntFaceTable->Get(key) ;
                DeleteShadowFace(bface,ttfaces,ttnodes,ff_table,
                                 center,point_tree) ;
				if ((ttfaces.Len() == 0) && (ttnodes.Len() == 0))
				{
					*trial = 0;
					return false;
				}
            }
        }
    }

    assert(0) ;
    *trial = new Node(-1,Vec3D(0.0,0.0,0.0),true) ;
    return(true) ;
}

bool PMTools::AdvFront3D::DoBacktrack(FFace* entry)
{

    Dict<int,int> ttfaces ;
    Dict<int,int> ttnodes ;
    Dict<int,FFace *> ff_table ;

    Node *trial ;

    //added by markos
    int id = entry->tet_topo_id;
    //end added by markos

    bool stagnate = ConstructStarShapedPoly(&entry,
                    ttfaces,ttnodes,ff_table,PtTree,&trial) ;

	if (stagnate) {
        //changed by markos
        FFace** ptr = FaceHeap->ViewMin() ;
        entry = *ptr ;
        if (entry->tet_topo_id == id)
		{
			ptr = FaceHeap->GetMin() ;
			entry = *ptr ;
			entry->use++ ;
			entry->heap_handle = FaceHeap->InsertWithHandle(entry) ;
		}
		else
		{
			Dict<int,FFace *>::DictIterator face_iter(&ff_table);

			for (face_iter.First(); face_iter.More(); ++face_iter)
			{
				FFace *face = face_iter.Entry();

				if (face->tet_topo_id != id)
				{
					continue;
				}

				FaceHeap->RemoveWithHandle(face->heap_handle);
				face->use++;
				face->heap_handle = FaceHeap->InsertWithHandle(face);

				break;
			}
		}
        //FFace** ptr = FaceHeap->GetMin() ;
        //entry = *ptr ;
        //end changed by markos

        //added by markos
        if ((this->pmtoolsWorkerTask) && (entry->use >= 3)) this->Tet_Topo->ChangeToTemporaryOuterFace(entry->tet_topo_id);
        //end added by markos

        //changed by markos
        //fprintf(stderr,"Stagnate at %d : %d %d %d\n",
        //        StartId,entry->node_id[0],
        //        entry->node_id[1],entry->node_id[2]) ;
        if (!this->pmtoolsWorkerTask)
		{
			//fprintf(stderr,"Stagnate at %d : %d %d %d\n",
			fprintf(stdout,"Stagnate at %d : %d %d %d\n",
					StartId,entry->node_id[0],
					entry->node_id[1],entry->node_id[2]) ;
		}
		//end changed by markos

#ifdef PLOT_RESTART_RGNS
        Vec3D tmp ;
        DebugPlotOuterBoundary(std::cout) ;
        DebugPlotBacktrack(std::cout,*entry,ttfaces,ttnodes,tmp,false) ;
        std::cout << "f" << endl ;
        DebugDumpFront(std::cout,Msh3D::num) ;
#endif
        return false ;
    }

    if (trial == 0) {
        //changed by markos
        FFace** ptr = FaceHeap->ViewMin() ;
        entry = *ptr ;
        if (entry->tet_topo_id == id)
		{
			ptr = FaceHeap->GetMin() ;
			entry = *ptr ;
			entry->use++ ;
			entry->heap_handle = FaceHeap->InsertWithHandle(entry) ;
		}
		else
		{
			Dict<int,FFace *>::DictIterator face_iter(&ff_table);

			for (face_iter.First(); face_iter.More(); ++face_iter)
			{
				FFace *face = face_iter.Entry();

				if (face->tet_topo_id != id)
				{
					continue;
				}

				FaceHeap->RemoveWithHandle(face->heap_handle);
				face->use++;
				face->heap_handle = FaceHeap->InsertWithHandle(face);

				break;
			}
		}
		//FFace** ptr = FaceHeap->GetMin() ;
        //entry = *ptr ;
        //entry->use++ ;
        //entry->heap_handle = FaceHeap->InsertWithHandle(entry) ;
        //end changed by markos

        //added by markos
        if ((this->pmtoolsWorkerTask) && (entry->use >= 3)) this->Tet_Topo->ChangeToTemporaryOuterFace(entry->tet_topo_id);
        //end added by markos

        //changed by markos
        //return true ;
        return (this->pmtoolsWorkerTask) ? false : true ;
        //end changed by markos
    }

    IntNodeTable->Store(trial->id,*trial) ;

    // now we have a polyhedral with a center
    // point.  Add elements from all the faces

    Dict<int,FFace *>::DictIterator face_iter(&ff_table) ;
    for (face_iter.First() ; face_iter.More() ; ++face_iter) {
        FFace *face = face_iter.Entry() ;

#ifdef PRINT_ADDED_TETS
if (face == entry) {
  atout << StartId << ' ' << face->node_id[0] << ' ' << face->node_id[1]
                 << ' ' << face->node_id[2] << ' ' << trial->id << std::endl ;
}
#endif

        TetElement elem = {{face->node_id[0],face->node_id[1],
                            face->node_id[2],trial->id}} ;
        TetTable->Store(StartId+1,elem) ;
        int tet_nodes[4] = {face->node_id[0],face->node_id[2],
                            face->node_id[1],trial->id} ;

        Tet_Topo->AddTetrahedron(tet_nodes,StartId+1) ;

        ++StartId ;
        ++Enum ;
        ++num ;

        int exists = FACEI_MASK(0) ;
        FFace *tface ;
        for (int i=1 ; i<4 ; ++i) {
            int fnodes[3] ;
            fnodes[0] = elem.node_id[Msh3D::FaceMap[i][0]] ;
            fnodes[1] = elem.node_id[Msh3D::FaceMap[i][1]] ;
            fnodes[2] = elem.node_id[3] ;
            RFaceHashKey face_key(3,fnodes) ;
            tface = IntFaceTable->Get(face_key) ;
            if (tface != 0) exists |= FACEI_MASK(i) ;
        }
#ifdef CHECK_EACH_TET
        UpdateFront(elem.node_id,exists,face->level+1,1);
#else
        UpdateFront(elem.node_id,exists,face->level+1,0);
#endif

#ifdef VOLUME_BOUNDARY_FREQ
        if ((num > VOLUME_BOUNDARY_START) &&
            ((num%VOLUME_BOUNDARY_FREQ) == 0)) {
            std::cerr << "Outputing boundary: " << num
                      << " : " << IntFaceTable->Len() << std::endl ;
            DebugPlotOuterBoundary(bfout) ;
            DebugDumpFront(bfout,num) ;
        }
#endif
    }
    return true ;
}

void PMTools::AdvFront3D::UpdateFront(int nodes[4],int exists,
                             int level,int use,bool check_valid,
                             int region)
{
#ifdef PRINT_FRONT_UPDATES
    std::cout << "Update: " << StartId << ' ' << nodes[0]
              << ' ' << nodes[1] << ' ' << nodes[2] << ' '
              << nodes[3] << std::endl ;
#endif

    FFace* faces[4] ;
    RFaceHashKey keys[4] ;
    bool do_delete[4] = {false,false,false,false} ;

    // loop through the four faces of the tet.  And check
    // to see if they exist or not.

    for (int i=0 ; i<4 ; ++i) {

        int fnodes[3] ;
        //changed by markos
        //fnodes[0] = nodes[FaceMap[i][0]] ;
        fnodes[0] = nodes[Msh3D::FaceMap[i][0]] ;
        //fnodes[1] = nodes[FaceMap[i][1]] ;
        fnodes[1] = nodes[Msh3D::FaceMap[i][1]] ;
        //fnodes[2] = nodes[FaceMap[i][2]] ;
        fnodes[2] = nodes[Msh3D::FaceMap[i][2]] ;
        //end changed by markos

// int tnum = 0 ;
// for (int j=0 ; j<3 ; ++j) {
//     if (fnodes[j] == 267399 || fnodes[j] == 458688 || fnodes[j] == 267428) {
//         tnum++ ;
//     }
// }
// if (tnum == 3) {
//     std::cerr << "Found Face:" << std::endl ;
// }
//

        RFaceHashKey face_key(3,fnodes) ;

        if (exists & FACEI_MASK(i)) {

            // if the face exists then Remove

            FFace *face = IntFaceTable->Get(face_key) ;
            //if (check_valid) {
            //    Node *opp_node = IntNodeTable->Get(nodes[OppMap[i]]) ;
            //    assert((face->normal * (opp_node->coord-face->centroid)) >= 0) ;
            //}
            //FaceHeap->RemoveWithHandle(face->heap_handle) ;
            AdTree->RemoveFromTree(face->min,face->max,
                                   face->ad_tree_indx) ;
            AdTreeHash->Del(face->ad_tree_indx) ;
            //IntFaceTable->Del(face_key) ;

            keys[i] = face_key ;
            do_delete[i] = true ;
            faces[i] = face ;

        } else {

            // if the face does not exist then add it

            Node *lnodes[3] ;
            lnodes[0] = IntNodeTable->Get(fnodes[0]) ;
            lnodes[1] = IntNodeTable->Get(fnodes[1]) ;
            lnodes[2] = IntNodeTable->Get(fnodes[2]) ;

            IntFaceTable->Store(face_key,FFace(fnodes,lnodes,level)) ;

            FFace *nface = IntFaceTable->Get(face_key) ;
            nface->boundary = false ;
            nface->use = use ;

            // now done below
            //nface->heap_handle =
            //    FaceHeap->InsertWithHandle(nface) ;
            nface->tet_topo_id = Tet_Topo->GetFaceFromVerts(
                                      nface->node_id[0],
                                      nface->node_id[1],
                                      nface->node_id[2]) ;
            nface->ad_tree_indx = AdTreeIndx ;
            AdTreeHash->Store(AdTreeIndx,nface) ;
            AdTree->AddToTree(nface->min,nface->max,AdTreeIndx) ;
            ++AdTreeIndx ;

            faces[i] = nface ;
        }
    }

    // here we update the front adjacent faces, this is like updating
    // a 3d doubly linked list

//if (region == -1) {
    for (int i=0 ; i<4 ; ++i) {
        if ((exists & FACEI_MASK(i)) == 0) {

            // this is a new face so find the adjacent faces

            int adj_faces[3] ;
            Tet_Topo->GetAdjacentFaces(faces[i]->tet_topo_id,Msh3D::TetTopo::FREE_REGION,adj_faces) ;
            //Tet_Topo->GetAdjacentFaces(faces[i]->tet_topo_id,region,adj_faces) ;

            for (int j=0 ; j<3 ; ++j) {

                int adj_nodes[3] ;
                Tet_Topo->GetVertAboutFace(adj_faces[j],&adj_nodes[0],
                                           &adj_nodes[1],&adj_nodes[2]) ;
                RFaceHashKey adj_key(3,adj_nodes) ;
                FFace *adj = IntFaceTable->Get(adj_key) ;

                //added by markos
                if (!adj)
				{
				    //fprintf(stderr,"(2) No adjacent face[%d] for face %d: %d %d %d, which should be %d: %d %d %d\n",
				    fprintf(stdout,"(2) No adjacent face[%d] for face %d: %d %d %d, which should be %d: %d %d %d\n",
                        j, faces[i]->tet_topo_id, faces[i]->node_id[0], faces[i]->node_id[1], faces[i]->node_id[2],
                        adj_faces[j], adj_nodes[0], adj_nodes[1], adj_nodes[2]) ;

					throw AdjacentFaceError();
				}
                //end added by markos

                // find the node that is not part of the base face

                int opp = -1 ;
                for (int k=0 ; k<3 ; ++k) {
                    if ((adj_nodes[k] != faces[i]->node_id[0]) &&
                        (adj_nodes[k] != faces[i]->node_id[1]) &&
                        (adj_nodes[k] != faces[i]->node_id[2])) {
                        opp = adj_nodes[k] ;
                        break ;
                    }
                }
                assert(opp >= 0) ;

                // now find the orientation and update the adj pointer

                int next = -1 ;
                for (int k=0 ; k<3 ; ++k) {
                    if (adj->node_id[k] == opp) {
                        next = (k+1)%3 ;
                        adj->adjacent[next] = faces[i] ;
                        break ;
                    }
                }
                assert(next >= 0) ;

                // update this faces pointer

                for (int k=0 ; k<3 ; ++k) {
                    if (faces[i]->node_id[k] == adj->node_id[next]) {
                        faces[i]->adjacent[(k==0) ? 2 : k-1] = adj ;
                        break ;
                    }
                }
            }
        }
    }

    // now compute the angle that the new faces make with adjacent faces

    for (int i=0 ; i<4 ; ++i) {
        if ((exists & FACEI_MASK(i)) == 0) {
            for (int j=0 ; j<3 ; ++j) {

                // find the mid-point on the edge and the vector pointing to it

                int k = (j+1) % 3 ;
                Vec3D mid = 0.5 * (faces[i]->coords[j] + faces[i]->coords[k]) ;
                //Vec3D v0 = (faces[i]->centroid - mid).Normalize() ;


                // find the far point on the adjacent face and the vector
                // pointing to it

                int adj_orient = -1 ;
                FFace* adj = faces[i]->adjacent[j] ;
                //added by markos
                if (!adj)
				{
				    //fprintf(stderr,"(3) No adjacent face[%d] for face %d: %d %d %d\n",
				    fprintf(stdout,"(3) No adjacent face[%d] for face %d: %d %d %d\n",
						j, faces[i]->tet_topo_id,faces[i]->node_id[0],
						faces[i]->node_id[1],faces[i]->node_id[2]) ;

					throw AdjacentFaceError();
				}
                //end added by markos
                double cur_min = adj->angle[3] ;

                for (int l=0 ; l<3 ; ++l) {
                    if ((adj->node_id[l] != faces[i]->node_id[j]) &&
                        (adj->node_id[l] != faces[i]->node_id[k])) {
                        adj_orient = l ;
                        break ;
                    }
                }
                Vec3D v1 = (adj->coords[adj_orient] - mid).Normalize() ;
                double angle = DiahedralAngle(faces[i]->coords[j],
                                              faces[i]->coords[k],
                                              faces[i]->centroid,
                                              adj->coords[adj_orient]) ;

                //double angle = acos(v0 * v1) ;
                if ((v1 * faces[i]->normal) < 0) angle = TWO_PI - angle ;

                // update this face

                faces[i]->angle[j] = angle ;
                if (angle == 0.0) faces[i]->angle[j] = 6.283 ;  // crack front

                // update the adjacent face and set its min angle

                adj->angle[(adj_orient+1)%3] = angle ;
                adj->angle[3] = adj->angle[0] ;
                if (adj->angle[1] < adj->angle[3]) adj->angle[3] = adj->angle[1] ;
                if (adj->angle[2] < adj->angle[3]) adj->angle[3] = adj->angle[2] ;

                if (adj->heap_handle && (adj->angle[3] < cur_min) &&
                    (adj->angle[3] < ACCUTE_THRESHOLD)) {
                    if (adj->heap_handle) {
                        FaceHeap->RemoveWithHandle(adj->heap_handle) ;
                       adj->heap_handle = FaceHeap->InsertWithHandle(adj) ;
                    }
                }
            }

            // now set the minimum angle on this face

            faces[i]->angle[3] = faces[i]->angle[0] ;
            if (faces[i]->angle[1] < faces[i]->angle[3])
                faces[i]->angle[3] = faces[i]->angle[1] ;
            if (faces[i]->angle[2] < faces[i]->angle[3])
                faces[i]->angle[3] = faces[i]->angle[2] ;
            faces[i]->heap_handle = FaceHeap->InsertWithHandle(faces[i]) ;
        }
    }
//} else {
//    for (int i=0 ; i<4 ; ++i) {
//        if ((exists & FACEI_MASK(i)) == 0) {
//            faces[i]->heap_handle = FaceHeap->InsertWithHandle(faces[i]) ;
//        }
//    }
//}

    // delete faces if necessary

    for (int i=0 ; i<4 ; ++i) {
        if (do_delete[i]) {
            FaceHeap->RemoveWithHandle(faces[i]->heap_handle) ;
            IntFaceTable->Del(keys[i]) ;
        }
    }

//     // check
//
//     Dict<MshRegion3D::FaceHashKey,FFace>::DictIterator fiter = IntFaceTable->Iterator() ;
//     for (fiter.First() ; fiter.More() ; ++fiter) {
//         for (int j=0 ; j<3 ; ++j) {
//             FFace* adj = fiter.Entry().adjacent[j] ;
//             {
//                 int fnodes[3] ;
//                 fnodes[0] = adj->node_id[0] ;
//                 fnodes[1] = adj->node_id[1] ;
//                 fnodes[2] = adj->node_id[2] ;
//                 RFaceHashKey face_key(3,fnodes) ;
//                 FFace *tmp = IntFaceTable->Get(face_key) ;
//             }
//             int num = 0 ;
//             for (int k=0 ; k<3 ; ++k) {
//                 if (adj->node_id[k] == fiter.Entry().node_id[j]) ++num ;
//             }
//             for (int k=0 ; k<3 ; ++k) {
//                 if (adj->node_id[k] == fiter.Entry().node_id[(j+1)%3]) ++num ;
//             }
//             assert(num == 2) ;
//         }
//     }
//     std::cerr << "checks: " << std::endl ;

}

PMTools::AdvFront3D::AdvFront3D(const PMTools::AdvFront3D &/*other*/) :
	MTools::AdvFront3D::AdvFront3D()
{

}

#endif //#if USE_MTOOLS
