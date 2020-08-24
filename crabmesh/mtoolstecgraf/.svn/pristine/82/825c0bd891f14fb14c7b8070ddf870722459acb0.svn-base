#include "MTools/AdvFront3D.h"

#if USE_MTOOLS

MTools::AdvFront3D::AdvFront3D(NodeHash *NodeTable,FaceHash *FaceTable, bool noCracks) :
	Msh3D::AdvFront3D::AdvFront3D(NodeTable, FaceTable, noCracks)
{
	this->SetUseUniformRefinement(true);
	this->SetUseUniformSize(true);

	//this->SetDoEdgeSwapping(false);
	//this->SetDoFaceSwapping(false);
	//this->SetDoSmoothing(false);
}

MTools::AdvFront3D::AdvFront3D() :
	Msh3D::AdvFront3D::AdvFront3D()
{

}

MTools::AdvFront3D::~AdvFront3D()
{

}

void MTools::AdvFront3D::SetUseUniformRefinement(bool useUniformRefinement)
{
	this->useUniformRefinement = useUniformRefinement;
}

void MTools::AdvFront3D::SetUseUniformSize(bool useUniformSize)
{
	this->useUniformSize = useUniformSize;
}

const FTools::OctTree *MTools::AdvFront3D::GetOct_Tree() const
{
	return this->Oct_Tree;
}

Msh3D::AdvFront3D::FFaceHeap *MTools::AdvFront3D::GetFaceHeap() const
{
	return this->FaceHeap;
}

MTools::AdvFront3D::IntNodeHash *MTools::AdvFront3D::GetIntNodeTable() const
{
	return this->IntNodeTable;
}

MTools::AdvFront3D::IntFaceHash *MTools::AdvFront3D::GetIntFaceTable() const
{
	return this->IntFaceTable;
}

Msh3D::TetTopo *MTools::AdvFront3D::GetTet_Topo() const
{
	return this->Tet_Topo;
}

MTools::AdvFront3D::MateHash *MTools::AdvFront3D::GetMates() const
{
	return this->Mates;
}

MTools::AdvFront3D::TetHash *MTools::AdvFront3D::GetTetTable() const
{
	return this->TetTable;
}

void MTools::AdvFront3D::SetDoEdgeSwapping(bool flag)
{
	this->DoEdgeSwapping = flag;
}

void MTools::AdvFront3D::SetDoFaceSwapping(bool flag)
{
	this->DoFaceSwapping = flag;
}

void MTools::AdvFront3D::GenerateOctTree()
{
	this->BuildOctTree();
}

void MTools::AdvFront3D::BuildOctTree()
{
	if (this->GetOct_Tree())
	{
		return;
	}

#if DEBUG_TIMING_PMTOOLS
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	// loop through all the nodes and find the max and min
    // x, y, z values

    IntNodeHashIter niter(IntNodeTable) ;
    Vec3D max = niter.Entry().coord ;
    Vec3D min = niter.Entry().coord ;
    ++niter ;

    while (niter.More()) {
        max.Maxv(niter.Entry().coord) ;
        min.Minv(niter.Entry().coord) ;
        ++niter ;
    }

    // Create an octree that is centered on the boundary and
    // large enough to contain all the nodes plus a little fudge
    // factor to avoid numerical tolerance troubles.

    Vec3D delta = max - min ;
    Vec3D center = 0.5 * (max + min) ;

    double size = (delta.x() > delta.y()) ? delta.x() : delta.y() ;
    size = (size > delta.z()) ? size : delta.z() ;

    Oct_Tree = new OctTree(center.x(),center.y(),
                           center.z(),size*1.01) ;

    //added by markos
    Oct_Tree->SetUseUniformSize(this->useUniformSize);
    //end added by markos

    // Locally refine the octree at the center of each face
    // Locate the face mid-point in the octree and look at the
    // cell size.  If the cell size is approximately equal
    // to or smaller than the face size, go on to the next face.
    // If not, subdivide the octant.

    FTools::Dict<Msh3D::MshRegion3D::FaceHashKey,FFace>::DictIterator fiter(IntFaceTable) ;

    //added by markos
    double maxSize = 0.0;
    //end added by markos

    for (fiter.First() ; fiter.More() ; ++fiter) {
    	//added by markos
    	double size = sqrt(fiter.Entry().area)* OctreeRefineFactor;

    	if (size > maxSize) maxSize = size;
    	//end added by markos

        //changed by markos
        //Oct_Tree->RefineToSize(fiter.Entry().centroid.x(),
        //                      fiter.Entry().centroid.y(),
        //                      fiter.Entry().centroid.z(),
        //                      sqrt(fiter.Entry().area)*
        //                      OctreeRefineFactor) ;
		Oct_Tree->RefineToSize(fiter.Entry().centroid.x(),
                              fiter.Entry().centroid.y(),
                              fiter.Entry().centroid.z(),
                              size) ;
		//end change by markos
    }

#if 0
    Oct_Tree->VisitLevels(0,DisplayOTree) ;
    dout << "f" << endl ;
#endif

	//added by markos
	if (this->useUniformRefinement)
	{
		Oct_Tree->UniformRefine(maxSize);
	}
	//end added by markos

    // Refine the octree cells so that neighbor cells
    // differ by no more than one level

    Oct_Tree->RefineOneLevelDiff() ;

#if 0
    Oct_Tree->VisitLevels(0,DisplayOTree) ;

    for (fiter.First() ; fiter.More() ; ++fiter) {
        dout << "p 3 " << fiter.Entry().coords[0] << ' '
                       << fiter.Entry().coords[1] << ' '
                       << fiter.Entry().coords[2] << endl ;
        dout << "e " << fiter.Entry().coords[0] << ' '
                     << fiter.Entry().coords[1] << " 1 0 0" << endl ;
        dout << "e " << fiter.Entry().coords[1] << ' '
                     << fiter.Entry().coords[2] << " 1 0 0" << endl ;
        dout << "e " << fiter.Entry().coords[2] << ' '
                     << fiter.Entry().coords[0] << " 1 0 0" << endl ;
    }

    dout << "f" << endl ;
    exit(1) ;
#endif

#if 0
    ofstream pickleo ;
    pickleo.open("pickle.tmp") ;
    pickleo << (*Oct_Tree) ;
    pickleo.close() ;

    delete Oct_Tree ;
    Oct_Tree = new OctTree(center.x(),center.y(),
                           center.z(),size*1.01) ;

    ifstream picklei ;
    picklei.open("pickle.tmp") ;
    picklei >> *Oct_Tree ;
    picklei.close() ;

    pickleo.open("pickle2.tmp") ;
    pickleo << (*Oct_Tree) ;
    pickleo.close() ;
#endif

#if DEBUG_TIMING_PMTOOLS
	this->timeTree = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS
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

void MTools::AdvFront3D::UpdateFront(int nodes[4],int exists,
                             int level,int use,bool /*check_valid*/,
                             int /*region*/)
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
					fprintf(stderr,"No adjacent face %d at %d : %d %d %d\n",
						j, faces[i]->tet_topo_id,faces[i]->node_id[0],
						faces[i]->node_id[1],faces[i]->node_id[2]) ;

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
					fprintf(stderr,"No adjacent face %d at %d : %d %d %d when updating angle\n",
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

/*bool MTools::AdvFront3D::FindCenterCoord(
    FFace **bface,
    Dict<int,int> &ttfaces,
    Dict<int,int> &ttnodes,
    Dict<int,FFace *> &ff_table,
    Vec3D &center,
    Dict<int,int> &crossings) const {

	bool ok = Msh3D::AdvFront3D::FindCenterCoord(bface, ttfaces, ttnodes, ff_table, center, crossings);

	if (ok)
	{
		//std::cout << "original find center coord (" << ff_table.Len() << " faces)" << std::endl;

		return true;
	}

	Dict<int,FFace *>::ConstDictIterator face_iter = ff_table.ConstIterator() ;
	Dict<int, int> ncrossings;

	*//*{
		///redo the average of the nodes, but without the test in...
		int vnum = 0 ;
		center = Vec3D(0.0,0.0,0.0) ;
		Dict<int,int>::ConstDictIterator nodes_iter = ttnodes.ConstIterator() ;
		for (nodes_iter.First() ; nodes_iter.More() ; ++nodes_iter) {
			Node *node = IntNodeTable->Get(nodes_iter.Key()) ;
			center += node->coord ;
			vnum++ ;
		}
		center /= double(vnum) ;

		// check to make sure that the center coordinate is
		// on the correct side of the base face and that it
		// can be seen by all other verts.

		/// ... in here
		//if ((*bface)->normal*center - (*bface)->d) {
			if (VisibleFromAll(bface,ttfaces,ttnodes,ff_table,center,crossings)) {
				if (ValidElemFromAll(ttfaces,ttnodes,ff_table,center)) {
					//std::cout << "average of nodes (" << ff_table.Len() << " faces)" << std::endl;

					found_center++ ;
					found_total++ ;
					return(true) ;
				}
			}
		//}
	}*//*

	int numsteps = 4;

	for (int i = 1; i <= numsteps; i++)
	{
		center = Vec3D(0.0,0.0,0.0);

		for (face_iter.First(); face_iter.More(); ++face_iter)
		{
			FFace *face = face_iter.Entry();

			Vec3D optim_loc = IdealNodeCoord(face, 1.0/static_cast<double>(i));

			center += optim_loc;
		}

		center /= double(ff_table.Len()) ;

		if (//((*bface)->normal*center - (*bface)->d) &&
			(VisibleFromAll(bface, ttfaces, ttnodes, ff_table, center, ncrossings)) &&
			(ValidElemFromAll(ttfaces, ttnodes, ff_table, center)))
		{
			//std::cout << "average of ideals (" << ff_table.Len() << " faces) in step " << i << std::endl;

			crossings.Clear();

			found_center++;
			found_total++;

			return true;
		}
	}

    for (int i = 1; i <= numsteps; i++)
	{
		for (face_iter.First(); face_iter.More(); ++face_iter)
		{
			center = Vec3D(0.0,0.0,0.0) ;

			FFace *face = face_iter.Entry() ;

			Vec3D optim_loc = IdealNodeCoord(face, 1.0/static_cast<double>(i));
			center += optim_loc;

			optim_loc = IdealNodeCoord(face->adjacent[0]);
			center += optim_loc;

			optim_loc = IdealNodeCoord(face->adjacent[1]);
			center += optim_loc;

			optim_loc = IdealNodeCoord(face->adjacent[2]);
			center += optim_loc;

			center /= 4.0;

			if (//((*bface)->normal*center - (*bface)->d) &&
				(VisibleFromAll(bface,ttfaces,ttnodes,ff_table,center,ncrossings)) &&
				(ValidElemFromAll(ttfaces,ttnodes,ff_table,center)))
			{
				//std::cout << "average of ideals of adjacents (" << ff_table.Len() << " faces) in step " << i << std::endl;

				crossings.Clear();

				found_center++;
				found_total++;

				return true;
			}
		}
	}

    //std::cout << "no center found (" << ff_table.Len() << " faces)" << std::endl;

	return false;
}*/

bool MTools::AdvFront3D::DoBacktrack(FFace* entry)
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
        FFace** ptr = FaceHeap->GetMin() ;
        entry = *ptr ;
        fprintf(stderr,"Stagnate at %d : %d %d %d\n",
                StartId,entry->node_id[0],
                entry->node_id[1],entry->node_id[2]) ;
#ifdef PLOT_RESTART_RGNS
        Vec3D tmp ;
        DebugPlotOuterBoundary(std::cout) ;
        DebugPlotBacktrack(std::cout,*entry,ttfaces,ttnodes,tmp,false) ;
        std::cout << "f" << endl ;
        DebugDumpFront(std::cout,num) ;
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
        return true ;
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

bool MTools::AdvFront3D::DeleteElement(int region,FFace *face,FFace **bface,
                                   int nodes[4],bool check_first)
{
    int i ;

    // determine which faces exist and check to see if any of
    // these are the base face

    bool update_base = (face == *bface) ;
    int start = check_first ? 0 : 1 ;
    int exists = check_first ? 0 : FACEI_MASK(0) ;
    for (i=start ; i<4 ; ++i) {
        int fnodes[3] ;
        fnodes[0] = nodes[Msh3D::FaceMap[i][0]] ;
        fnodes[1] = nodes[Msh3D::FaceMap[i][1]] ;
        fnodes[2] = nodes[3] ;
        RFaceHashKey face_key(3,fnodes) ;
        FFace *face = IntFaceTable->Get(face_key) ;
        if (face != 0) {
            exists |= FACEI_MASK(i) ;
            if (face == *bface) update_base = true ;
        }
    }

    //changed by markos
    //UpdateFront(nodes,exists,face->level-1,1,false,region) ;
    UpdateFront(nodes,exists,face->level-1,0,false,region) ;
    //end changed by markos

    // check to see if the current base face was deleted.
    // if so we need to update this to point to a new face

    if (!update_base) return(true) ;

    bool found = false ;
    for (i=1 ; i<4 ; ++i) {
        if ((exists & FACEI_MASK(i)) == 0) {
            int fnodes[3] ;
            fnodes[0] = nodes[Msh3D::FaceMap[i][0]] ;
            fnodes[1] = nodes[Msh3D::FaceMap[i][1]] ;
            fnodes[2] = nodes[3] ;
            RFaceHashKey face_key(3,fnodes) ;
            FFace *face = IntFaceTable->Get(face_key) ;
            *bface = face ;
            found = true ;
            break ;
        }
    }
    return(found) ;
}

MTools::AdvFront3D::AdvFront3D(const MTools::AdvFront3D &/*other*/) :
	Msh3D::AdvFront3D::AdvFront3D()
{

}

#endif //#if USE_MTOOLS
