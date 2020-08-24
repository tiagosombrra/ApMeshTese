#include "MTools/MshRegion3D.h"

#if USE_MTOOLS

#include "MTools/AdvFront3D.h"

#if DEBUG_TIMING_PMTOOLS
double MTools::MshRegion3D::timeBuildAdvFront = 0.0;
#endif //#if DEBUG_TIMING_PMTOOLS

const Performer::Status MTools::MshRegion3D::success =
	Performer::Status(MTools::MshRegion3D::SUCCESS, "success");
const Performer::Status MTools::MshRegion3D::notInit =
	Performer::Status(MTools::MshRegion3D::NOT_INIT, "not initiated");
const Performer::Status MTools::MshRegion3D::init =
	Performer::Status(MTools::MshRegion3D::INIT, "initiated");
const Performer::Status MTools::MshRegion3D::error =
	Performer::Status(MTools::MshRegion3D::ERROR, "error");

MTools::MshRegion3D::MshRegion3D(const MshOrder iorder, const int istart_id, const int imat_id) :
	Msh3D::MshRegion3D::MshRegion3D(iorder, istart_id, imat_id),
	Performer::Builder::Builder()
{
	this->BoundaryChecks = false;

	this->SetNoCracks(false);
	this->SetUseUniformRefinement(true);
	this->SetUseUniformSize(true);

	this->status = this->notInit;
}

MTools::MshRegion3D::~MshRegion3D()
{
	if (this->AdvFront)
	{
		delete this->AdvFront;

		//this->AdvFront = NULL;
	}
}

void MTools::MshRegion3D::SetNoCracks(bool noCracks)
{
	this->noCracks = noCracks;
}

void MTools::MshRegion3D::SetUseUniformRefinement(bool useUniformRefinement)
{
	this->useUniformRefinement = useUniformRefinement;
}

void MTools::MshRegion3D::SetUseUniformSize(bool useUniformSize)
{
	this->useUniformSize = useUniformSize;
}

const Msh3D::AdvFront3D *MTools::MshRegion3D::GetAdvFront() const
{
	return this->AdvFront;
}

const FTools::OctTree *MTools::MshRegion3D::GetOctTree() const
{
	return this->AdvFront ? static_cast<AdvFront3D *>(this->AdvFront)->GetOct_Tree() : NULL;
}

const FTools::OctTree *MTools::MshRegion3D::GenerateOctTree()
{
	if (!this->AdvFront)
	{
		this->AdvFront = this->BuildAdvFront();

		static_cast<AdvFront3D *>(this->AdvFront)->GenerateOctTree();
	}

	return this->GetOctTree();
}

void MTools::MshRegion3D::execute()
{
	//added by markos
	bool pyramid_only = false;

	bool delete_front = false ;
	//end added by markos

	//switch added by markos
	switch (this->status.getType())
	{
	case NOT_INIT:
		{
			if (DbgFd) DebugGenerateMesh() ;

			//commented by markos
			//bool delete_front = false ;
			//end commented by markos
			NumRestarts = 0 ;

			// if necessary, check the validity of the boundary

			if (BoundaryChecks) CheckBoundary() ;

			//changed by markos
			if (AdvFront == 0) {

				AdvFront = this->BuildAdvFront() ;

				static_cast<MTools::AdvFront3D *>(this->AdvFront)->GenerateOctTree();
			}
			//end changed by markos

			this->status = this->init;
			//end added by markos
		}

	case INIT:
		{
			// do the advancing front

			AdvFront->GenerateMesh(StartElemId,MatId,ElemTable,pyramid_only) ;
			StartId = AdvFront->GetNextNodeId() ;
			StartElemId = AdvFront->GetNextElemId() ;

			AdvFront->GetMeshingStatistics(NumRestarts,NumBackTracks,
					NumPyrAttempts,NumPyrExisting,NumPyrGenerated,
					NumTetAttempts,NumTetExisting,NumTetAngle,
					NumTetGenerated) ;

		#ifdef PRINT_NUM_GEN
			fprintf(stderr,"Generated elements: %d\n",ElemTable->Len()) ;
		#endif

			// if this is a quadratic order mesh then we need to generate
			// all the side nodes

			if (Order == QUADRATIC) {
				int next_node_id = AdvFront->GetNextNodeId() ;
				ElemHashIter eiter(ElemTable) ;
				for (eiter.First() ; eiter.More() ; ++eiter) {
					MshElem *elem = eiter.EntryPtr() ;

					if (elem->num_nodes == 4) {

						// map of the nodes on each element edge

						int map[6][2] = {{ 0, 1 }, { 1, 2 }, { 2, 0 },
										 { 0, 3 }, { 1, 3 }, { 2, 3 }} ;

						// loop through all edges.  If necessary create
						// new nodes

						elem->num_nodes = 10 ;
						for (int i=0 ; i<6 ; ++i) {
							EdgeHashKey key(elem->nodes[map[i][0]],
											elem->nodes[map[i][1]]) ;
							int *id = SideTable->Get(key) ;
							if (id != 0) {
								elem->nodes[4+i] = *id ;
							} else {
								IntNode node,*nd0,*nd1 ;
								nd0 = NodeTable->Get(key.id_0) ;
								nd1 = NodeTable->Get(key.id_1) ;
								node.id = next_node_id ;
								next_node_id++ ;
								node.coord = 0.5*(nd0->coord+nd1->coord) ;
								node.type = INTERIOR ;
								node.motion = M_FLOATING ;
								node.corner = false ;
								NodeTable->Store(node.id,node) ;
								elem->nodes[4+i] = node.id ;
								SideTable->Store(key,node.id) ;
							}
						}
					} else {

						// map of the nodes on each element edge

						int map[8][2] = {{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
										 { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 }} ;

						// loop through all edges.  If necessary create
						// new nodes

						elem->num_nodes = 13 ;
						for (int i=0 ; i<8 ; ++i) {
							EdgeHashKey key(elem->nodes[map[i][0]],
											elem->nodes[map[i][1]]) ;
							int *id = SideTable->Get(key) ;
							if (id != 0) {
								elem->nodes[5+i] = *id ;
							} else {
								IntNode node,*nd0,*nd1 ;
								nd0 = NodeTable->Get(key.id_0) ;
								nd1 = NodeTable->Get(key.id_1) ;
								node.id = next_node_id ;
								next_node_id++ ;
								node.coord = 0.5*(nd0->coord+nd1->coord) ;
								node.type = INTERIOR ;
								node.motion = M_FLOATING ;
								node.corner = false ;
								NodeTable->Store(node.id,node) ;
								elem->nodes[5+i] = node.id ;
								SideTable->Store(key,node.id) ;
							}
						}
					}
				}
			}

			if (delete_front) {
				delete AdvFront ;
				AdvFront = 0 ;
			}

			//added by markos
			this->status = this->success;
			//end added by markos
		}
	}
	//end switch added by markos
}

Msh3D::AdvFront3D *MTools::MshRegion3D::BuildAdvFront()
{
#if DEBUG_TIMING_PMTOOLS
	double debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	// if this is a quadratic order mesh we need to create a
	// table for side nodes and fill it with the side node
	// information for the initial boundary

	if (Order == QUADRATIC) {
		SideTable = new EdgeHash ;
		FaceHashIter fiter(FaceTable) ;
		for (fiter.First() ; fiter.More() ; ++fiter) {
			MshFace *face = fiter.EntryPtr() ;
			int num_node = (face->type == TRIANGLE ? 3 : 4) ;
			for (int i=0 ; i<num_node ; ++i) {
				int j = (i+1) % num_node ;
				SideTable->Store(
					EdgeHashKey(face->nodes[i],face->nodes[j]),
					face->nodes[num_node+i]) ;
			}
		}
	}

	Msh3D::AdvFront3D *AdvFront = this->MakeAdvFront();

	//added by markos
	static_cast<MTools::AdvFront3D *>(AdvFront)->SetUseUniformRefinement(this->useUniformRefinement);
	static_cast<MTools::AdvFront3D *>(AdvFront)->SetUseUniformSize(this->useUniformSize);
	//end added by markos

	if (DebugDumpElem > 0) AdvFront->SetDebugDumpElem(DebugDumpElem) ;
	if (DebugStopElem > 0) AdvFront->SetDebugStopElem(DebugStopElem) ;
	if (CheckPointNum >= 0) AdvFront->SetCheckPointNum(CheckPointNum) ;
	if (StartId == 0) StartId = MaxId + 1 ;
	AdvFront->SetNextNodeId(StartId) ;
	AdvFront->SetDoSmoothing(DoSmoothNodes) ;
	if (GenerationStatusCallback != 0)
		AdvFront->RegisterGenerationCallback(
			GenerationStatusFrequency,GenerationStatusCallback) ;
	if (SmoothingStatusCallback != 0)
		AdvFront->RegisterSmoothingCallback(
			SmoothingSummaryOnly,SmoothingStatusCallback) ;
	if (InsertAndCheckOnly) AdvFront->TestInsertAndCheckOnly() ;
	if (NoInternalNodes) AdvFront->SetNoInternalNodes() ;

	//changed by markos
	//AdvFront3D::MeshParams params ;
	Msh3D::AdvFront3D::MeshParams params ;
	//end changed by markos
	params.MaxGeneratedElements = MaxGeneratedElements ;
	params.OptimalSphereFactor = OptimalSphereFactor ;
	params.OptimalSizeFactor = OptimalSizeFactor ;
	params.OctreeRefineFactor = OctreeRefineFactor ;
	params.MaxVolumeRestarts = MaxVolumeRestarts ;
	AdvFront->SetMeshParams(params) ;

#if DEBUG_DISABLE_IMPROVEMENT
	static_cast<MTools::AdvFront3D *>(AdvFront)->SetDoEdgeSwapping(MTools::isImprovementEnabled());
	static_cast<MTools::AdvFront3D *>(AdvFront)->SetDoFaceSwapping(MTools::isImprovementEnabled());
	AdvFront->SetDoSmoothing(MTools::isImprovementEnabled());
#endif //#if DEBUG_DISABLE_IMPROVEMENT

#if DEBUG_TIMING_PMTOOLS
	this->timeBuildAdvFront = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

	return AdvFront;
}

Msh3D::AdvFront3D *MTools::MshRegion3D::MakeAdvFront()
{
	return new MTools::AdvFront3D(NodeTable,FaceTable,this->noCracks);
}

#endif //#if USE_MTOOLS
